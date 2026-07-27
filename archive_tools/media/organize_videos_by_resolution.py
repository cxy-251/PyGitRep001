"""Plan or apply moving videos into WIDTHxHEIGHT directories."""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path

VIDEO_EXTENSIONS = {".mp4", ".mkv", ".mov", ".avi", ".flv", ".webm", ".m4v"}


def require_ffprobe() -> str:
    path = shutil.which("ffprobe")
    if path is None:
        raise SystemExit("ffprobe was not found in PATH")
    return path


def get_resolution(ffprobe: str, path: Path) -> tuple[int, int]:
    result = subprocess.run(
        [
            ffprobe, "-v", "error",
            "-select_streams", "v:0",
            "-show_entries", "stream=width,height",
            "-of", "csv=p=0",
            str(path),
        ],
        capture_output=True,
        text=True,
        encoding="utf-8",
    )
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or "ffprobe failed")
    width, height = map(int, result.stdout.strip().split(","))
    return width, height


def unique_destination(path: Path) -> Path:
    if not path.exists():
        return path
    index = 1
    while True:
        candidate = path.with_name(f"{path.stem}_{index}{path.suffix}")
        if not candidate.exists():
            return candidate
        index += 1


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path, help="Directory containing videos")
    parser.add_argument("--output", type=Path, help="Destination root; defaults to input")
    parser.add_argument("--recursive", action="store_true")
    parser.add_argument(
        "--apply",
        action="store_true",
        help="Actually move files. Without this flag only print the plan.",
    )
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffprobe = require_ffprobe()
    source_root = args.input.expanduser().resolve()
    output_root = (args.output or source_root).expanduser().resolve()

    if not source_root.is_dir():
        raise SystemExit(f"Input directory does not exist: {source_root}")

    iterator = source_root.rglob("*") if args.recursive else source_root.iterdir()
    videos = sorted(
        path for path in iterator
        if path.is_file() and path.suffix.lower() in VIDEO_EXTENSIONS
    )

    failures = 0
    for video in videos:
        try:
            width, height = get_resolution(ffprobe, video)
        except Exception as exc:
            failures += 1
            print(f"SKIP {video}: {exc}")
            continue

        destination_dir = output_root / f"{width}x{height}"
        if video.parent == destination_dir:
            continue
        destination = unique_destination(destination_dir / video.name)
        print(f"{'MOVE' if args.apply else 'PLAN'} {video} -> {destination}")

        if args.apply:
            destination_dir.mkdir(parents=True, exist_ok=True)
            shutil.move(str(video), str(destination))

    if not args.apply:
        print("\nDry run only. Add --apply to move files.")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
