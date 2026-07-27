"""Group files or directories into numbered folders; dry-run unless --apply is supplied."""

from __future__ import annotations

import argparse
import re
import shutil
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--size", type=int, default=10, help="Items per group")
    parser.add_argument("--kind", choices=("files", "directories"), default="files")
    parser.add_argument("--prefix", default="Group")
    parser.add_argument("--apply", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    root = args.root.expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Directory does not exist: {root}")
    if args.size <= 0:
        raise SystemExit("--size must be positive")

    group_pattern = re.compile(re.escape(args.prefix) + r"\d+$")
    if args.kind == "files":
        items = sorted(path for path in root.iterdir() if path.is_file())
    else:
        items = sorted(
            path for path in root.iterdir()
            if path.is_dir() and not group_pattern.fullmatch(path.name)
        )

    plan: list[tuple[Path, Path]] = []
    for index, item in enumerate(items):
        group_number = (index // args.size) + 1
        destination = root / f"{args.prefix}{group_number}" / item.name
        if destination.exists():
            raise FileExistsError(f"Target exists before execution: {destination}")
        plan.append((item, destination))
        print(f"{'MOVE' if args.apply else 'PLAN'} {item} -> {destination}")

    if not args.apply:
        print("\nDry run only. Add --apply to move items.")
        return 0

    for item, destination in plan:
        destination.parent.mkdir(exist_ok=True)
        shutil.move(str(item), str(destination))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
