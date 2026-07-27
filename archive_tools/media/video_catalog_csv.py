"""扫描视频并输出详细 CSV；无法读取的文件写入单独错误表。"""

from __future__ import annotations

import argparse
import csv
import json
import shutil
import subprocess
from fractions import Fraction
from pathlib import Path

VIDEO_EXTENSIONS = {".mp4", ".mkv", ".avi", ".mov", ".flv", ".webm", ".m4v"}


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--output", type=Path, default=Path("video_catalog.csv"))
    parser.add_argument("--errors", type=Path, default=Path("video_catalog_errors.csv"))
    parser.add_argument("--no-recursive", action="store_true")
    return parser


def probe(path: Path, ffprobe: str) -> dict[str, object]:
    result = subprocess.run(
        [
            ffprobe, "-v", "error",
            "-select_streams", "v:0",
            "-show_entries", "stream=width,height,r_frame_rate,bit_rate:format=duration,bit_rate",
            "-of", "json",
            str(path),
        ],
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace",
    )
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or "ffprobe failed")
    data = json.loads(result.stdout)
    streams = data.get("streams") or []
    if not streams:
        raise RuntimeError("no video stream")
    stream = streams[0]
    fmt = data.get("format") or {}
    fps_raw = str(stream.get("r_frame_rate") or "0")
    try:
        fps = float(Fraction(fps_raw))
    except (ValueError, ZeroDivisionError):
        fps = 0.0
    bitrate = stream.get("bit_rate") or fmt.get("bit_rate") or 0
    return {
        "width": int(stream.get("width") or 0),
        "height": int(stream.get("height") or 0),
        "fps": round(fps, 3),
        "duration_seconds": round(float(fmt.get("duration") or 0), 3),
        "bitrate_kbps": round(int(bitrate) / 1000) if str(bitrate).isdigit() else 0,
    }


def main() -> int:
    args = build_parser().parse_args()
    ffprobe = shutil.which("ffprobe")
    if ffprobe is None:
        raise SystemExit("ffprobe must be available in PATH")

    root = args.root.expanduser().resolve()
    if not root.is_dir():
        raise SystemExit(f"Root directory does not exist: {root}")

    iterator = root.glob("*") if args.no_recursive else root.rglob("*")
    videos = sorted(p for p in iterator if p.is_file() and p.suffix.lower() in VIDEO_EXTENSIONS)
    fields = [
        "relative_path", "parent", "filename", "width", "height", "resolution",
        "fps", "duration_seconds", "bitrate_kbps", "size_bytes", "usage_count",
    ]
    error_fields = ["relative_path", "error"]

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.errors.parent.mkdir(parents=True, exist_ok=True)
    failures = 0
    with args.output.open("w", newline="", encoding="utf-8-sig") as out_file, \
         args.errors.open("w", newline="", encoding="utf-8-sig") as error_file:
        writer = csv.DictWriter(out_file, fieldnames=fields)
        error_writer = csv.DictWriter(error_file, fieldnames=error_fields)
        writer.writeheader()
        error_writer.writeheader()

        for path in videos:
            relative = path.relative_to(root)
            try:
                info = probe(path, ffprobe)
                row = {
                    "relative_path": str(relative),
                    "parent": str(relative.parent),
                    "filename": path.name,
                    **info,
                    "resolution": f"{info['width']}x{info['height']}",
                    "size_bytes": path.stat().st_size,
                    "usage_count": 0,
                }
                writer.writerow(row)
                print(f"OK   {relative} {row['resolution']} {row['duration_seconds']}s")
            except Exception as exc:
                failures += 1
                error_writer.writerow({"relative_path": str(relative), "error": str(exc)})
                print(f"FAIL {relative}: {exc}")

    print(f"Scanned {len(videos)} videos; failures={failures}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
