"""Export a directory tree to JSON."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any


def build_tree(path: Path, include_hidden: bool) -> dict[str, Any]:
    if path.is_symlink():
        return {
            "name": path.name,
            "type": "symlink",
            "target": str(path.readlink()),
        }

    node: dict[str, Any] = {
        "name": path.name or str(path),
        "type": "directory" if path.is_dir() else "file",
    }
    if path.is_file():
        node["size_bytes"] = path.stat().st_size
        return node

    children: list[dict[str, Any]] = []
    try:
        entries = sorted(path.iterdir(), key=lambda item: (not item.is_dir(), item.name.lower()))
    except PermissionError:
        node["error"] = "permission denied"
        node["children"] = children
        return node

    for entry in entries:
        if not include_hidden and entry.name.startswith("."):
            continue
        try:
            children.append(build_tree(entry, include_hidden))
        except OSError as exc:
            children.append({"name": entry.name, "type": "unknown", "error": str(exc)})
    node["children"] = children
    return node


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("-o", "--output", type=Path, default=Path("file_tree.json"))
    parser.add_argument("--include-hidden", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    root = args.root.expanduser().resolve()
    if not root.exists() and not root.is_symlink():
        raise SystemExit(f"Path does not exist: {root}")

    output = args.output.expanduser().resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(
        json.dumps(build_tree(root, args.include_hidden), ensure_ascii=False, indent=2),
        encoding="utf-8",
    )
    print(f"Saved: {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
