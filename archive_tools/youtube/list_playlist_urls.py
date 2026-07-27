"""使用 yt-dlp 列出频道或播放列表中的视频 URL。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("url")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--cookies", type=Path)
    parser.add_argument("--cookies-from-browser")
    parser.add_argument("--include-live", action="store_true")
    parser.add_argument("--music", action="store_true", help="Use music.youtube.com watch URLs")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    yt_dlp = shutil.which("yt-dlp")
    if yt_dlp is None:
        raise SystemExit("yt-dlp must be available in PATH")
    command = [yt_dlp, "--flat-playlist", "--print", "%(id)s"]
    if args.cookies:
        command += ["--cookies", str(args.cookies.expanduser().resolve())]
    if args.cookies_from_browser:
        command += ["--cookies-from-browser", args.cookies_from_browser]
    if not args.include_live:
        command += ["--match-filter", "is_live = False"]
    command.append(args.url)

    result = subprocess.run(
        command, capture_output=True, text=True, encoding="utf-8", errors="replace"
    )
    if result.returncode != 0:
        print(result.stderr.strip())
        return result.returncode
    host = "https://music.youtube.com/watch?v=" if args.music else "https://www.youtube.com/watch?v="
    urls = [line.strip() for line in result.stdout.splitlines() if line.strip()]
    normalized = [line if line.startswith("http") else host + line for line in urls]
    text = "\n".join(normalized) + ("\n" if normalized else "")
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text, encoding="utf-8")
    else:
        print(text, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
