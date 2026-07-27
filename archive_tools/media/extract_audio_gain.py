"""Extract audio from media files and apply an optional gain filter."""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path

MEDIA_EXTENSIONS = {
    ".mp4", ".mkv", ".mov", ".avi", ".flv", ".webm",
    ".m4a", ".aac", ".flac", ".wav", ".ogg", ".opus",
}


def require_ffmpeg() -> str:
    path = shutil.which("ffmpeg")
    if path is None:
        raise SystemExit("ffmpeg was not found in PATH")
    return path


def iter_media(path: Path, recursive: bool) -> list[Path]:
    if path.is_file():
        return [path]
    iterator = path.rglob("*") if recursive else path.iterdir()
    return sorted(
        item for item in iterator
        if item.is_file() and item.suffix.lower() in MEDIA_EXTENSIONS
    )


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path, help="Input file or directory")
    parser.add_argument("output", type=Path, help="Output directory")
    parser.add_argument("--gain-db", type=float, default=15.0)
    parser.add_argument("--bitrate", default="192k")
    parser.add_argument("--recursive", action="store_true")
    parser.add_argument("--overwrite", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffmpeg = require_ffmpeg()
    source = args.input.expanduser().resolve()
    output_root = args.output.expanduser().resolve()

    if not source.exists():
        raise SystemExit(f"Input does not exist: {source}")

    base = source.parent if source.is_file() else source
    files = iter_media(source, args.recursive)
    failures = 0

    for media in files:
        relative_parent = media.parent.relative_to(base)
        destination_dir = output_root / relative_parent
        destination_dir.mkdir(parents=True, exist_ok=True)
        destination = destination_dir / f"{media.stem}.mp3"

        if destination.exists() and not args.overwrite:
            print(f"SKIP exists: {destination}")
            continue

        command = [
            ffmpeg,
            "-hide_banner",
            "-loglevel", "error",
            "-y" if args.overwrite else "-n",
            "-i", str(media),
            "-vn",
            "-af", f"volume={args.gain_db}dB",
            "-c:a", "libmp3lame",
            "-b:a", args.bitrate,
            str(destination),
        ]
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode != 0 or not destination.exists():
            failures += 1
            destination.unlink(missing_ok=True)
            print(f"FAIL {media}: {result.stderr.strip()}")
            continue

        print(f"OK   {media} -> {destination}")

    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
