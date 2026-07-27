"""Safely convert audio files to MP3; source deletion requires --replace-source."""

from __future__ import annotations

import argparse
import shutil
import subprocess
import tempfile
from pathlib import Path

AUDIO_EXTENSIONS = {".wav", ".flac", ".m4a", ".aac", ".opus", ".mp3", ".wma", ".ogg"}


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("--output", type=Path, help="Output root; defaults beside sources")
    parser.add_argument("--bitrate", default="128k")
    parser.add_argument("--sample-rate", type=int, default=44100)
    parser.add_argument("--channels", type=int, default=2)
    parser.add_argument("--recursive", action="store_true")
    parser.add_argument("--overwrite", action="store_true")
    parser.add_argument(
        "--replace-source",
        action="store_true",
        help="Delete the source only after FFmpeg succeeds and the MP3 is verified.",
    )
    return parser


def iter_audio(path: Path, recursive: bool) -> list[Path]:
    if path.is_file():
        return [path]
    iterator = path.rglob("*") if recursive else path.iterdir()
    return sorted(
        item for item in iterator
        if item.is_file() and item.suffix.lower() in AUDIO_EXTENSIONS
    )


def main() -> int:
    args = build_parser().parse_args()
    ffmpeg = shutil.which("ffmpeg")
    ffprobe = shutil.which("ffprobe")
    if ffmpeg is None or ffprobe is None:
        raise SystemExit("ffmpeg and ffprobe must both be available in PATH")

    source_root = args.input.expanduser().resolve()
    if not source_root.exists():
        raise SystemExit(f"Input does not exist: {source_root}")
    base = source_root.parent if source_root.is_file() else source_root
    output_root = args.output.expanduser().resolve() if args.output else None
    failures = 0

    for source in iter_audio(source_root, args.recursive):
        relative_parent = source.parent.relative_to(base)
        destination_dir = (output_root / relative_parent) if output_root else source.parent
        destination_dir.mkdir(parents=True, exist_ok=True)
        destination = destination_dir / f"{source.stem}.mp3"

        same_file = source.resolve() == destination.resolve()
        if same_file and not (args.overwrite or args.replace_source):
            print(f"SKIP source is already the destination; add --overwrite to re-encode: {source}")
            continue
        if destination.exists() and not same_file and not args.overwrite:
            print(f"SKIP exists: {destination}")
            continue

        with tempfile.TemporaryDirectory(prefix="mp3-convert-", dir=str(destination_dir)) as temp:
            temporary = Path(temp) / f"{source.stem}.mp3"
            result = subprocess.run(
                [
                    ffmpeg,
                    "-hide_banner", "-loglevel", "error",
                    "-y",
                    "-i", str(source),
                    "-vn",
                    "-ar", str(args.sample_rate),
                    "-ac", str(args.channels),
                    "-c:a", "libmp3lame",
                    "-b:a", args.bitrate,
                    str(temporary),
                ],
                capture_output=True,
                text=True,
            )
            if result.returncode != 0 or not temporary.exists() or temporary.stat().st_size == 0:
                failures += 1
                print(f"FAIL {source}: {result.stderr.strip()}")
                continue

            verify = subprocess.run(
                [
                    ffprobe, "-v", "error",
                    "-select_streams", "a:0",
                    "-show_entries", "stream=codec_name",
                    "-of", "default=noprint_wrappers=1:nokey=1",
                    str(temporary),
                ],
                capture_output=True,
                text=True,
            )
            if verify.returncode != 0 or verify.stdout.strip() != "mp3":
                failures += 1
                print(f"FAIL verification: {source}")
                continue

            if destination.exists():
                destination.unlink()
            shutil.move(str(temporary), str(destination))
            print(f"OK   {source} -> {destination}")

            if args.replace_source and not same_file and source.exists():
                source.unlink()
                print(f"DELETE verified source: {source}")

    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
