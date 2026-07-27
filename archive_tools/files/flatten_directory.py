"""Move descendant files into one root directory; dry-run unless --apply is supplied."""

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


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--apply", action="store_true")
    parser.add_argument("--remove-empty", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    root = args.root.expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Directory does not exist: {root}")

    files = sorted(
        path for path in root.rglob("*")
        if path.is_file() and path.parent != root
    )
    for source in files:
        destination = unique_destination(root / source.name)
        print(f"{'MOVE' if args.apply else 'PLAN'} {source} -> {destination}")
        if args.apply:
            shutil.move(str(source), str(destination))

    if args.apply and args.remove_empty:
        directories = sorted(
            (path for path in root.rglob("*") if path.is_dir()),
            key=lambda path: len(path.parts),
            reverse=True,
        )
        for directory in directories:
            try:
                directory.rmdir()
            except OSError:
                pass

    if not args.apply:
        print("\nDry run only. Add --apply to move files.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
