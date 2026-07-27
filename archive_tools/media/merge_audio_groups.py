"""Merge audio files in each subdirectory into size-based MP3 groups."""

from __future__ import annotations

import argparse
import shutil
import subprocess
import tempfile
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

AUDIO_EXTENSIONS = {".mp3", ".m4a", ".flac", ".wav", ".ogg", ".aac", ".opus", ".wma"}


def safe_name(text: str, limit: int = 120) -> str:
    cleaned = "".join(char if char not in '<>:"/\\|?*' else "_" for char in text)
    return cleaned.strip(" .")[:limit] or "audio"


def group_by_size(files: list[Path], target_bytes: int) -> list[list[Path]]:
    groups: list[list[Path]] = []
    current: list[Path] = []
    current_size = 0
    for path in sorted(files, key=lambda item: (item.stat().st_size, item.name.lower())):
        current.append(path)
        current_size += path.stat().st_size
        if current_size >= target_bytes:
            groups.append(current)
            current = []
            current_size = 0
    if current:
        groups.append(current)
    return groups


def convert_and_merge(
    ffmpeg: str,
    files: list[Path],
    destination: Path,
    bitrate: str,
) -> tuple[bool, str]:
    with tempfile.TemporaryDirectory(prefix="audio-merge-") as temporary:
        temp_root = Path(temporary)
        converted: list[Path] = []

        for index, source in enumerate(files):
            target = temp_root / f"{index:04d}.mp3"
            result = subprocess.run(
                [
                    ffmpeg, "-hide_banner", "-loglevel", "error",
                    "-y", "-i", str(source),
                    "-vn", "-c:a", "libmp3lame", "-b:a", bitrate,
                    str(target),
                ],
                capture_output=True,
                text=True,
            )
            if result.returncode != 0 or not target.exists():
                return False, f"convert failed for {source}: {result.stderr.strip()}"
            converted.append(target)

        list_file = temp_root / "concat.txt"
        list_file.write_text(
            "".join(f"file '{path.as_posix()}'\n" for path in converted),
            encoding="utf-8",
        )
        destination.parent.mkdir(parents=True, exist_ok=True)
        result = subprocess.run(
            [
                ffmpeg, "-hide_banner", "-loglevel", "error",
                "-y", "-f", "concat", "-safe", "0",
                "-i", str(list_file),
                "-c:a", "libmp3lame", "-b:a", bitrate,
                str(destination),
            ],
            capture_output=True,
            text=True,
        )
        if result.returncode != 0 or not destination.exists():
            destination.unlink(missing_ok=True)
            return False, result.stderr.strip() or "concat failed"
        return True, ""


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--output", type=Path)
    parser.add_argument("--target-mb", type=float, default=25.0)
    parser.add_argument("--bitrate", default="320k")
    parser.add_argument("--workers", type=int, default=2)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffmpeg = shutil.which("ffmpeg")
    if ffmpeg is None:
        raise SystemExit("ffmpeg was not found in PATH")

    root = args.root.expanduser().resolve()
    output_root = (args.output or root.with_name(f"{root.name}_merged")).expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Directory does not exist: {root}")
    if args.target_mb <= 0 or args.workers <= 0:
        raise SystemExit("--target-mb and --workers must be positive")

    jobs: list[tuple[list[Path], Path]] = []
    target_bytes = int(args.target_mb * 1024 * 1024)
    directories = [root, *sorted(path for path in root.rglob("*") if path.is_dir())]
    for directory in directories:
        files = [
            path for path in directory.iterdir()
            if path.is_file() and path.suffix.lower() in AUDIO_EXTENSIONS
        ]
        for index, group in enumerate(group_by_size(files, target_bytes), start=1):
            relative = directory.relative_to(root)
            name = safe_name(directory.name)
            destination = output_root / relative / f"{name}_group{index:03d}.mp3"
            jobs.append((group, destination))

    failures = 0
    with ThreadPoolExecutor(max_workers=args.workers) as executor:
        future_map = {
            executor.submit(convert_and_merge, ffmpeg, files, destination, args.bitrate):
                destination
            for files, destination in jobs
        }
        for future in as_completed(future_map):
            destination = future_map[future]
            try:
                success, message = future.result()
            except Exception as exc:
                success, message = False, str(exc)
            if success:
                print(f"OK   {destination}")
            else:
                failures += 1
                print(f"FAIL {destination}: {message}")

    print(f"Completed: {len(jobs) - failures}; failed: {failures}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
