"""Randomly rename files to numbered names; dry-run unless --apply is supplied."""

from __future__ import annotations

import argparse
import json
import random
import uuid
from datetime import datetime
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("directory", type=Path)
    parser.add_argument("--digits", type=int, default=3)
    parser.add_argument("--prefix", default="")
    parser.add_argument("--seed", type=int)
    parser.add_argument("--apply", action="store_true")
    parser.add_argument("--mapping", type=Path, help="Mapping JSON path")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    directory = args.directory.expanduser().resolve()
    if not directory.is_dir():
        raise SystemExit(f"Directory does not exist: {directory}")
    if args.digits < 1:
        raise SystemExit("--digits must be at least 1")

    files = sorted(path for path in directory.iterdir() if path.is_file())
    maximum = (10 ** args.digits) - 1
    if len(files) > maximum:
        raise SystemExit(f"{len(files)} files exceed the {maximum} available numbers")

    generator = random.Random(args.seed)
    numbers = generator.sample(range(1, maximum + 1), len(files))
    plan: list[tuple[Path, Path, Path]] = []
    target_names: set[str] = set()

    for source, number in zip(files, numbers):
        destination = directory / f"{args.prefix}{number:0{args.digits}d}{source.suffix.lower()}"
        if destination.name in target_names:
            raise RuntimeError(f"Internal target collision: {destination}")
        target_names.add(destination.name)
        temporary = directory / f".rename-{uuid.uuid4().hex}{source.suffix}"
        plan.append((source, temporary, destination))
        print(f"{'RENAME' if args.apply else 'PLAN'} {source.name} -> {destination.name}")

    if not args.apply:
        print("\nDry run only. Add --apply to rename files.")
        return 0

    moved_to_temp: list[tuple[Path, Path, Path]] = []
    moved_to_final: list[tuple[Path, Path, Path]] = []
    try:
        for source, temporary, destination in plan:
            source.rename(temporary)
            moved_to_temp.append((source, temporary, destination))

        for source, temporary, destination in moved_to_temp:
            if destination.exists():
                raise FileExistsError(f"Target already exists: {destination}")
            temporary.rename(destination)
            moved_to_final.append((source, temporary, destination))
    except Exception:
        for source, _, destination in reversed(moved_to_final):
            if destination.exists() and not source.exists():
                destination.rename(source)
        for source, temporary, _ in reversed(moved_to_temp):
            if temporary.exists() and not source.exists():
                temporary.rename(source)
        raise

    mapping = [{"from": source.name, "to": destination.name} for source, _, destination in plan]
    mapping_path = args.mapping or directory / (
        f"rename-map-{datetime.now().strftime('%Y%m%d-%H%M%S')}.json"
    )
    mapping_path.write_text(json.dumps(mapping, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"Mapping saved: {mapping_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
