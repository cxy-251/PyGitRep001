"""Archived yt-dlp helper for downloading channel audio or video from a URL list."""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import Any


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("channel_list", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--mode", choices=("audio", "video"), default="audio")
    parser.add_argument("--start-line", type=int, default=1)
    parser.add_argument("--end-line", type=int)
    parser.add_argument("--cookies", type=Path)
    parser.add_argument("--cookies-from-browser", choices=("firefox", "chrome", "edge"))
    parser.add_argument("--archive", type=Path)
    return parser


def build_options(args: argparse.Namespace, line_number: int) -> dict[str, Any]:
    output_root = args.output.expanduser().resolve()
    options: dict[str, Any] = {
        "ignoreerrors": True,
        "continuedl": True,
        "retries": 3,
        "fragment_retries": 3,
        "socket_timeout": 30,
        "sleep_interval": 3,
        "max_sleep_interval": 10,
        "overwrites": False,
        "windowsfilenames": True,
    }

    if args.cookies:
        options["cookiefile"] = str(args.cookies.expanduser().resolve())
    elif args.cookies_from_browser:
        options["cookiesfrombrowser"] = (args.cookies_from_browser,)

    if args.archive:
        options["download_archive"] = str(args.archive.expanduser().resolve())

    if args.mode == "audio":
        options.update({
            "format": "bestaudio/best",
            "outtmpl": str(
                output_root
                / f"line_num_{line_number}"
                / "%(uploader)s"
                / "%(upload_date)s-%(title)s-%(id)s.%(ext)s"
            ),
            "writethumbnail": True,
            "writeinfojson": True,
            "postprocessors": [
                {
                    "key": "FFmpegExtractAudio",
                    "preferredcodec": "mp3",
                    "preferredquality": "0",
                },
                {"key": "EmbedThumbnail"},
                {"key": "FFmpegMetadata"},
            ],
        })
    else:
        options.update({
            "format": "bestvideo+bestaudio/best",
            "merge_output_format": "mp4",
            "outtmpl": str(
                output_root
                / f"line_num_{line_number}"
                / "%(uploader)s"
                / "%(upload_date)s-%(title)s-%(id)s.%(ext)s"
            ),
            "writeinfojson": True,
        })
    return options


def main() -> int:
    args = build_parser().parse_args()
    if args.start_line < 1:
        raise SystemExit("--start-line must be at least 1")
    if args.cookies and args.cookies_from_browser:
        raise SystemExit("Use either --cookies or --cookies-from-browser, not both")

    try:
        from yt_dlp import YoutubeDL
    except ImportError as exc:
        raise SystemExit("Install dependency: python -m pip install yt-dlp") from exc

    lines = [
        line.strip()
        for line in args.channel_list.expanduser().read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    ]
    end_line = args.end_line or len(lines)
    selected = [
        (number, lines[number - 1])
        for number in range(args.start_line, min(end_line, len(lines)) + 1)
    ]

    failures = 0
    for line_number, url in selected:
        print(f"[{line_number}/{len(lines)}] {url}")
        options = build_options(args, line_number)
        try:
            with YoutubeDL(options) as downloader:
                result = downloader.download([url])
            if result != 0:
                failures += 1
        except Exception as exc:
            failures += 1
            print(f"FAIL {url}: {exc}")

    print(f"Completed: {len(selected) - failures}; failed: {failures}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
