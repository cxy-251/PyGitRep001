"""从文本文件读取频道或播放列表 URL，并按过滤规则下载 YouTube 视频。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("channels", type=Path, help="One channel or playlist URL per line")
    parser.add_argument("output", type=Path)
    parser.add_argument("--start-line", type=int, default=1)
    parser.add_argument("--end-line", type=int)
    parser.add_argument("--cookies", type=Path)
    parser.add_argument("--cookies-from-browser")
    parser.add_argument("--minimum-views", type=int, default=0)
    parser.add_argument("--minimum-duration", type=int, default=10)
    parser.add_argument("--reject-title", action="append", default=["Live"])
    parser.add_argument("--include-live", action="store_true")
    parser.add_argument("--sleep-min", type=int, default=3)
    parser.add_argument("--sleep-max", type=int, default=7)
    parser.add_argument("--archive-name", default="video-downloaded.txt")
    return parser


def read_urls(path: Path, start: int, end: int | None) -> list[tuple[int, str]]:
    lines = path.read_text(encoding="utf-8").splitlines()
    result = []
    for number, raw in enumerate(lines, start=1):
        value = raw.strip()
        if not value or value.startswith("#") or number < start or (end and number > end):
            continue
        result.append((number, value))
    return result


def main() -> int:
    args = build_parser().parse_args()
    yt_dlp = shutil.which("yt-dlp")
    if yt_dlp is None:
        raise SystemExit("yt-dlp must be available in PATH")
    channels = args.channels.expanduser().resolve()
    if not channels.is_file():
        raise SystemExit(f"Channel list does not exist: {channels}")
    output = args.output.expanduser().resolve()
    output.mkdir(parents=True, exist_ok=True)

    failures = 0
    for line_number, url in read_urls(channels, args.start_line, args.end_line):
        command = [
            yt_dlp,
            "--yes-playlist",
            "--prefer-ffmpeg",
            "-f", "bv*+ba/b",
            "--merge-output-format", "mp4",
            "--embed-metadata",
            "--download-archive", str(output / args.archive_name),
            "--no-post-overwrites",
            "--output", str(output / "%(uploader)s/%(upload_date)s-%(title).80s.%(ext)s"),
            "--output-na-placeholder", "_",
            "--retries", "5",
            "--fragment-retries", "5",
            "--concurrent-fragments", "3",
            "--sleep-interval", str(args.sleep_min),
            "--max-sleep-interval", str(args.sleep_max),
        ]
        filters = [f"duration > {args.minimum_duration}"]
        if args.minimum_views > 0:
            filters.append(f"view_count > {args.minimum_views}")
        if not args.include_live:
            filters.append("is_live = False")
        for condition in filters:
            command += ["--match-filter", condition]
        for title in args.reject_title:
            command += ["--reject-title", title]
        if args.cookies:
            command += ["--cookies", str(args.cookies.expanduser().resolve())]
        if args.cookies_from_browser:
            command += ["--cookies-from-browser", args.cookies_from_browser]
        command.append(url)
        print(f"LINE {line_number}: {url}")
        result = subprocess.run(command)
        if result.returncode != 0:
            failures += 1
            print(f"FAIL line {line_number}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
