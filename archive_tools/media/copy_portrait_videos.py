"""Copy portrait videos to a new root while preserving relative directories."""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path

VIDEO_EXTENSIONS = {".mp4", ".mkv", ".mov", ".avi", ".flv", ".webm", ".m4v"}


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


def is_within(path: Path, parent: Path) -> bool:
    try:
        path.relative_to(parent)
        return True
    except ValueError:
        return False


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--move", action="store_true", help="Move instead of copy")
    parser.add_argument("--apply", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffprobe = shutil.which("ffprobe")
    if ffprobe is None:
        raise SystemExit("ffprobe was not found in PATH")

    source_root = args.input.expanduser().resolve()
    output_root = args.output.expanduser().resolve()
    if not source_root.is_dir():
        raise SystemExit(f"Directory does not exist: {source_root}")
    if source_root == output_root:
        raise SystemExit("Input and output directories must be different")

    sources = [
        path for path in source_root.rglob("*")
        if path.is_file()
        and path.suffix.lower() in VIDEO_EXTENSIONS
        and not is_within(path, output_root)
    ]

    failures = 0
    for source in sorted(sources):
        try:
            width, height = get_resolution(ffprobe, source)
        except Exception as exc:
            failures += 1
            print(f"SKIP {source}: {exc}")
            continue
        if height <= width:
            continue

        destination = output_root / source.relative_to(source_root)
        action = "MOVE" if args.move else "COPY"
        print(f"{action if args.apply else 'PLAN'} {source} -> {destination}")
        if args.apply:
            destination.parent.mkdir(parents=True, exist_ok=True)
            if destination.exists():
                print(f"SKIP target exists: {destination}")
                continue
            if args.move:
                shutil.move(str(source), str(destination))
            else:
                shutil.copy2(source, destination)

    if not args.apply:
        print("\nDry run only. Add --apply to copy or move portrait videos.")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
