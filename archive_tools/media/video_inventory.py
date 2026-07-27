"""Inspect video duration and resolution without modifying files."""

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
from collections import Counter
from pathlib import Path
from typing import Any

VIDEO_EXTENSIONS = {".mp4", ".mkv", ".mov", ".avi", ".flv", ".webm", ".m4v"}


def require_command(name: str) -> str:
    path = shutil.which(name)
    if path is None:
        raise SystemExit(f"Required command not found in PATH: {name}")
    return path


def iter_videos(root: Path, recursive: bool) -> list[Path]:
    paths = root.rglob("*") if recursive else root.iterdir()
    return sorted(
        path for path in paths
        if path.is_file() and path.suffix.lower() in VIDEO_EXTENSIONS
    )


def probe_video(ffprobe: str, path: Path) -> dict[str, Any]:
    command = [
        ffprobe,
        "-v", "error",
        "-show_entries", "format=duration:stream=codec_type,width,height",
        "-of", "json",
        str(path),
    ]
    result = subprocess.run(command, capture_output=True, text=True, encoding="utf-8")
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or "ffprobe failed")

    payload = json.loads(result.stdout)
    duration = float(payload.get("format", {}).get("duration") or 0)
    video_stream = next(
        (stream for stream in payload.get("streams", []) if stream.get("codec_type") == "video"),
        {},
    )
    width = int(video_stream.get("width") or 0)
    height = int(video_stream.get("height") or 0)
    return {
        "path": str(path),
        "duration_seconds": round(duration, 3),
        "width": width,
        "height": height,
        "resolution": f"{width}x{height}" if width and height else "unknown",
        "size_bytes": path.stat().st_size,
    }


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("path", type=Path, help="Video file or directory")
    parser.add_argument("--recursive", action="store_true", help="Scan subdirectories")
    parser.add_argument("--json", type=Path, dest="json_path", help="Write detailed JSON report")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffprobe = require_command("ffprobe")
    target = args.path.expanduser().resolve()

    if target.is_file():
        files = [target]
    elif target.is_dir():
        files = iter_videos(target, args.recursive)
    else:
        raise SystemExit(f"Path does not exist: {target}")

    records: list[dict[str, Any]] = []
    for path in files:
        try:
            record = probe_video(ffprobe, path)
            records.append(record)
            print(
                f"{record['resolution']:>12}  "
                f"{record['duration_seconds'] / 60:8.2f} min  {path}"
            )
        except Exception as exc:
            print(f"SKIP {path}: {exc}")

    total_seconds = sum(record["duration_seconds"] for record in records)
    resolutions = Counter(record["resolution"] for record in records)
    print(f"\nFiles: {len(records)}")
    print(f"Total duration: {total_seconds / 3600:.2f} hours")
    print("Resolutions:")
    for resolution, count in resolutions.most_common():
        print(f"  {resolution}: {count}")

    if args.json_path:
        output = args.json_path.expanduser().resolve()
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_text(
            json.dumps(records, ensure_ascii=False, indent=2),
            encoding="utf-8",
        )
        print(f"JSON report: {output}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
