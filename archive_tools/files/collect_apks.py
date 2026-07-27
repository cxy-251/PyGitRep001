"""Collect APK files into one directory and encode the relative path in each name."""

from __future__ import annotations

import argparse
import shutil
from pathlib import Path


def unique_destination(path: Path) -> Path:
    if not path.exists():
        return path
    index = 1
    while True:
        candidate = path.with_name(f"{path.stem}_{index}{path.suffix}")
        if not candidate.exists():
            return candidate
        index += 1


def is_within(path: Path, parent: Path) -> bool:
    try:
        path.relative_to(parent)
        return True
    except ValueError:
        return False


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--move", action="store_true", help="Move instead of copy")
    parser.add_argument("--apply", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    root = args.root.expanduser().resolve()
    output = args.output.expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Directory does not exist: {root}")
    if root == output:
        raise SystemExit("Input and output directories must be different")

    apks = sorted(
        path for path in root.rglob("*.apk")
        if path.is_file() and not is_within(path, output)
    )
    for source in apks:
        relative = source.relative_to(root)
        encoded_name = "_".join(relative.parts)
        destination = unique_destination(output / encoded_name)
        action = "MOVE" if args.move else "COPY"
        print(f"{action if args.apply else 'PLAN'} {source} -> {destination}")
        if args.apply:
            output.mkdir(parents=True, exist_ok=True)
            if args.move:
                shutil.move(str(source), str(destination))
            else:
                shutil.copy2(source, destination)

    if not args.apply:
        print("\nDry run only. Add --apply to copy or move APK files.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
