"""Rotate video files with FFmpeg and keep the source files unchanged."""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path

VIDEO_EXTENSIONS = {".mp4", ".mkv", ".mov", ".avi", ".webm", ".m4v"}
FILTERS = {
    "cw": "transpose=1",
    "ccw": "transpose=2",
    "180": "hflip,vflip",
}


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path, help="Video file or directory")
    parser.add_argument("--direction", choices=FILTERS, default="ccw")
    parser.add_argument("--recursive", action="store_true")
    parser.add_argument("--suffix", default="_rotated")
    parser.add_argument("--overwrite", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffmpeg = shutil.which("ffmpeg")
    if ffmpeg is None:
        raise SystemExit("ffmpeg was not found in PATH")

    target = args.input.expanduser().resolve()
    if target.is_file():
        files = [target]
    elif target.is_dir():
        iterator = target.rglob("*") if args.recursive else target.iterdir()
        files = sorted(
            path for path in iterator
            if path.is_file() and path.suffix.lower() in VIDEO_EXTENSIONS
        )
    else:
        raise SystemExit(f"Input does not exist: {target}")

    failures = 0
    for source in files:
        destination = source.with_name(f"{source.stem}{args.suffix}{source.suffix}")
        if destination.exists() and not args.overwrite:
            print(f"SKIP exists: {destination}")
            continue

        command = [
            ffmpeg,
            "-hide_banner", "-loglevel", "error",
            "-y" if args.overwrite else "-n",
            "-i", str(source),
            "-vf", FILTERS[args.direction],
            "-c:a", "copy",
            str(destination),
        ]
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode != 0 or not destination.exists():
            failures += 1
            destination.unlink(missing_ok=True)
            print(f"FAIL {source}: {result.stderr.strip()}")
        else:
            print(f"OK   {source} -> {destination}")

    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
