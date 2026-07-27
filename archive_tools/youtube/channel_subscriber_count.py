"""使用 yt-dlp 打印频道名称和订阅数。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("url")
    parser.add_argument("--cookies", type=Path)
    parser.add_argument("--cookies-from-browser")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    yt_dlp = shutil.which("yt-dlp")
    if yt_dlp is None:
        raise SystemExit("yt-dlp must be available in PATH")
    command = [
        yt_dlp, "--playlist-end", "1",
        "--print", "%(uploader)s\t%(uploader_subscriber_count)s",
    ]
    if args.cookies:
        command += ["--cookies", str(args.cookies.expanduser().resolve())]
    if args.cookies_from_browser:
        command += ["--cookies-from-browser", args.cookies_from_browser]
    command.append(args.url)
    return subprocess.run(command).returncode


if __name__ == "__main__":
    raise SystemExit(main())
