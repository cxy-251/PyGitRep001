"""统计根目录下每个直接子目录的文件数量，并输出 CSV。"""

from __future__ import annotations

import argparse
import csv
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--output", type=Path, default=Path("folder_item_counts.csv"))
    parser.add_argument("--recursive", action="store_true", help="Count files recursively")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    root = args.root.expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Root directory does not exist: {root}")

    rows: list[tuple[str, int]] = []
    for folder in sorted(path for path in root.iterdir() if path.is_dir()):
        iterator = folder.rglob("*") if args.recursive else folder.iterdir()
        count = sum(1 for path in iterator if path.is_file())
        rows.append((folder.name, count))
        print(f"{folder.name}: {count}")

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8-sig") as file:
        writer = csv.writer(file)
        writer.writerow(["folder", "file_count"])
        writer.writerows(rows)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
