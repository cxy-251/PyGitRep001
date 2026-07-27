"""检查指定路径所在磁盘的可用空间，并用退出码表示是否低于阈值。"""

from __future__ import annotations

import argparse
import shutil
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("path", type=Path)
    parser.add_argument("--minimum-free-gb", type=float, required=True)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    path = args.path.expanduser().resolve()
    existing = path
    while not existing.exists() and existing != existing.parent:
        existing = existing.parent
    if not existing.exists():
        raise SystemExit(f"No existing parent found for: {path}")
    usage = shutil.disk_usage(existing)
    free_gb = usage.free / (1024 ** 3)
    total_gb = usage.total / (1024 ** 3)
    print(f"path={existing}")
    print(f"total_gb={total_gb:.2f}")
    print(f"free_gb={free_gb:.2f}")
    print(f"minimum_free_gb={args.minimum_free_gb:.2f}")
    return 0 if free_gb >= args.minimum_free_gb else 2


if __name__ == "__main__":
    raise SystemExit(main())
