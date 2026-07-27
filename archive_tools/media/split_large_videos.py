"""Split large videos into approximate-size parts without deleting originals."""

from __future__ import annotations

import argparse
import math
import shutil
import subprocess
from pathlib import Path

VIDEO_EXTENSIONS = {".mp4", ".mkv", ".mov", ".avi", ".flv", ".webm", ".m4v"}


def require_command(name: str) -> str:
    path = shutil.which(name)
    if path is None:
        raise SystemExit(f"Required command not found in PATH: {name}")
    return path


def get_duration(ffprobe: str, path: Path) -> float:
    result = subprocess.run(
        [
            ffprobe, "-v", "error",
            "-show_entries", "format=duration",
            "-of", "default=noprint_wrappers=1:nokey=1",
            str(path),
        ],
        capture_output=True,
        text=True,
        encoding="utf-8",
    )
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or "ffprobe failed")
    return float(result.stdout.strip())


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--recursive", action="store_true")
    parser.add_argument("--min-size-gb", type=float, default=2.0)
    parser.add_argument("--target-part-gb", type=float, default=2.0)
    parser.add_argument(
        "--apply",
        action="store_true",
        help="Create parts. Without this flag only print the plan.",
    )
    parser.add_argument(
        "--archive-originals",
        action="store_true",
        help="After every part succeeds, move the original into root/long_videos.",
    )
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffmpeg = require_command("ffmpeg")
    ffprobe = require_command("ffprobe")
    root = args.root.expanduser().resolve()

    if not root.is_dir():
        raise SystemExit(f"Directory does not exist: {root}")
    if args.min_size_gb <= 0 or args.target_part_gb <= 0:
        raise SystemExit("Size values must be positive")

    iterator = root.rglob("*") if args.recursive else root.iterdir()
    videos = sorted(
        path for path in iterator
        if path.is_file() and path.suffix.lower() in VIDEO_EXTENSIONS
    )
    archive_root = root / "long_videos"
    failures = 0

    for source in videos:
        size_gb = source.stat().st_size / (1024 ** 3)
        if size_gb < args.min_size_gb:
            continue

        try:
            duration = get_duration(ffprobe, source)
        except Exception as exc:
            failures += 1
            print(f"SKIP {source}: {exc}")
            continue

        part_count = max(2, math.ceil(size_gb / args.target_part_gb))
        segment_duration = duration / part_count
        output_dir = source.parent / f"{source.stem}_parts"
        print(
            f"{'SPLIT' if args.apply else 'PLAN'} {source} "
            f"({size_gb:.2f} GB, {duration / 60:.1f} min) -> {part_count} parts"
        )

        if not args.apply:
            continue

        output_dir.mkdir(parents=True, exist_ok=True)
        created: list[Path] = []
        success = True
        for index in range(part_count):
            start = segment_duration * index
            length = duration - start if index == part_count - 1 else segment_duration
            destination = output_dir / f"{source.stem}_part{index + 1:03d}{source.suffix}"
            result = subprocess.run(
                [
                    ffmpeg,
                    "-hide_banner", "-loglevel", "error",
                    "-y",
                    "-ss", f"{start:.3f}",
                    "-i", str(source),
                    "-t", f"{length:.3f}",
                    "-map", "0",
                    "-c", "copy",
                    str(destination),
                ],
                capture_output=True,
                text=True,
            )
            if result.returncode != 0 or not destination.exists() or destination.stat().st_size == 0:
                success = False
                failures += 1
                print(f"FAIL part {index + 1}: {result.stderr.strip()}")
                break
            created.append(destination)

        if not success:
            for path in created:
                path.unlink(missing_ok=True)
            continue

        print(f"OK   created {len(created)} parts in {output_dir}")
        if args.archive_originals:
            archive_root.mkdir(parents=True, exist_ok=True)
            destination = archive_root / source.name
            if destination.exists():
                failures += 1
                print(f"KEEP original; archive target exists: {destination}")
            else:
                shutil.move(str(source), str(destination))
                print(f"ARCHIVE original -> {destination}")

    if not args.apply:
        print("\nDry run only. Add --apply to split files.")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
