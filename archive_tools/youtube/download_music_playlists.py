"""从文本文件读取 YouTube Music 播放列表并下载带元数据的 MP3。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("playlists", type=Path, help="One playlist/channel URL per line")
    parser.add_argument("output", type=Path)
    parser.add_argument("--start-line", type=int, default=1)
    parser.add_argument("--end-line", type=int)
    parser.add_argument("--cookies", type=Path)
    parser.add_argument("--cookies-from-browser")
    parser.add_argument("--minimum-views", type=int, default=0)
    parser.add_argument("--with-lyrics", action="store_true")
    parser.add_argument("--write-info-json", action="store_true")
    parser.add_argument("--archive-name", default="music-downloaded.txt")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    yt_dlp = shutil.which("yt-dlp")
    if yt_dlp is None:
        raise SystemExit("yt-dlp must be available in PATH")
    source = args.playlists.expanduser().resolve()
    if not source.is_file():
        raise SystemExit(f"Playlist list does not exist: {source}")
    output = args.output.expanduser().resolve()
    output.mkdir(parents=True, exist_ok=True)
    lines = source.read_text(encoding="utf-8").splitlines()
    failures = 0

    for number, raw in enumerate(lines, start=1):
        url = raw.strip()
        if not url or url.startswith("#") or number < args.start_line or (
            args.end_line and number > args.end_line
        ):
            continue
        command = [
            yt_dlp,
            "--yes-playlist",
            "--extract-audio",
            "--audio-format", "mp3",
            "--audio-quality", "0",
            "--prefer-ffmpeg",
            "-f", "ba/best",
            "--embed-metadata",
            "--embed-thumbnail",
            "--add-metadata",
            "--replace-in-metadata", "artist", ",", " _",
            "--download-archive", str(output / args.archive_name),
            "--reject-title", "Live",
            "--no-post-overwrites",
            "--output", str(output / "%(artist).40s/%(album)s/%(title)s.%(ext)s"),
            "--retries", "5",
            "--fragment-retries", "5",
            "--concurrent-fragments", "3",
        ]
        if args.minimum_views > 0:
            command += ["--match-filter", f"view_count > {args.minimum_views}"]
        if args.with_lyrics:
            command += ["--write-subs", "--embed-subs", "--sub-format", "lrc"]
        if args.write_info_json:
            command.append("--write-info-json")
        if args.cookies:
            command += ["--cookies", str(args.cookies.expanduser().resolve())]
        if args.cookies_from_browser:
            command += ["--cookies-from-browser", args.cookies_from_browser]
        command.append(url)
        print(f"LINE {number}: {url}")
        result = subprocess.run(command)
        if result.returncode != 0:
            failures += 1
            print(f"FAIL line {number}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
