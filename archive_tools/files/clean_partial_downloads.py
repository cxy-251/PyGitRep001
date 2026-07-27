"""查找并清理残留的 .part、.tmp 和 .ytdl 文件；默认只预览。"""

from __future__ import annotations

import argparse
from pathlib import Path

DEFAULT_SUFFIXES = (".part", ".tmp", ".ytdl")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--suffix", action="append", dest="suffixes")
    parser.add_argument("--apply", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    root = args.root.expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Root directory does not exist: {root}")
    suffixes = tuple(args.suffixes or DEFAULT_SUFFIXES)
    matches = sorted(
        path for path in root.rglob("*")
        if path.is_file() and any(path.name.lower().endswith(s.lower()) for s in suffixes)
    )
    for path in matches:
        if args.apply:
            path.unlink()
            print(f"DELETE {path}")
        else:
            print(f"WOULD DELETE {path}")
    print(f"Matched {len(matches)} files; apply={args.apply}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
