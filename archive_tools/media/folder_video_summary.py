"""按直接子目录汇总视频数量、分辨率及各分辨率总时长。"""

from __future__ import annotations

import argparse
import csv
import json
import shutil
import subprocess
from pathlib import Path

VIDEO_EXTENSIONS = {".mp4", ".mkv", ".avi", ".mov", ".flv", ".webm", ".m4v"}


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--output", type=Path, default=Path("folder_video_summary.csv"))
    return parser


def probe(path: Path, ffprobe: str) -> tuple[str, float]:
    result = subprocess.run(
        [
            ffprobe, "-v", "error", "-select_streams", "v:0",
            "-show_entries", "stream=width,height:format=duration",
            "-of", "json", str(path),
        ],
        capture_output=True, text=True, encoding="utf-8", errors="replace",
    )
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or "ffprobe failed")
    data = json.loads(result.stdout)
    stream = (data.get("streams") or [None])[0]
    if not stream:
        raise RuntimeError("no video stream")
    resolution = f"{int(stream.get('width') or 0)}x{int(stream.get('height') or 0)}"
    duration = float((data.get("format") or {}).get("duration") or 0)
    return resolution, duration


def main() -> int:
    args = build_parser().parse_args()
    ffprobe = shutil.which("ffprobe")
    if ffprobe is None:
        raise SystemExit("ffprobe must be available in PATH")
    root = args.root.expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Root directory does not exist: {root}")

    rows: list[dict[str, object]] = []
    failures = 0
    for folder in sorted(path for path in root.iterdir() if path.is_dir()):
        totals: dict[str, float] = {}
        count = 0
        for path in sorted(folder.rglob("*")):
            if not path.is_file() or path.suffix.lower() not in VIDEO_EXTENSIONS:
                continue
            count += 1
            try:
                resolution, duration = probe(path, ffprobe)
                totals[resolution] = totals.get(resolution, 0.0) + duration
            except Exception as exc:
                failures += 1
                print(f"FAIL {path}: {exc}")
        rounded = {key: round(value, 2) for key, value in sorted(totals.items())}
        rows.append(
            {
                "folder": folder.name,
                "video_count": count,
                "resolutions": json.dumps(list(rounded), ensure_ascii=False),
                "duration_by_resolution_seconds": json.dumps(rounded, ensure_ascii=False),
                "total_duration_seconds": round(sum(rounded.values()), 2),
            }
        )
        print(f"{folder.name}: videos={count}, resolutions={list(rounded)}")

    fields = [
        "folder", "video_count", "resolutions",
        "duration_by_resolution_seconds", "total_duration_seconds",
    ]
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8-sig") as file:
        writer = csv.DictWriter(file, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
