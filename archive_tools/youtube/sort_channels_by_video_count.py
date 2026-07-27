"""Sort channel URLs by the number of videos reported by yt-dlp."""

from __future__ import annotations

import argparse
import shutil
import subprocess
import time
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--cookies", type=Path)
    parser.add_argument("--delay", type=float, default=1.0)
    parser.add_argument("--timeout", type=int, default=180)
    parser.add_argument("--descending", action="store_true")
    return parser


def count_videos(yt_dlp: str, url: str, cookies: Path | None, timeout: int) -> int | None:
    command = [yt_dlp, "--flat-playlist", "--print", "%(id)s", url]
    if cookies:
        command += ["--cookies", str(cookies)]
    try:
        result = subprocess.run(
            command,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=timeout,
        )
    except subprocess.TimeoutExpired:
        return None
    if result.returncode != 0:
        return None
    return len([line for line in result.stdout.splitlines() if line.strip()])


def main() -> int:
    args = build_parser().parse_args()
    yt_dlp = shutil.which("yt-dlp")
    if yt_dlp is None:
        raise SystemExit("yt-dlp was not found in PATH")

    input_path = args.input.expanduser().resolve()
    output_path = args.output.expanduser().resolve()
    channels = [
        line.strip()
        for line in input_path.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    ]

    records: list[tuple[int | None, str]] = []
    for index, channel in enumerate(channels, start=1):
        count = count_videos(yt_dlp, channel, args.cookies, args.timeout)
        label = "ERROR" if count is None else str(count)
        print(f"[{index}/{len(channels)}] {label:>8}  {channel}")
        records.append((count, channel))
        if args.delay > 0:
            time.sleep(args.delay)

    valid = [(count, url) for count, url in records if count is not None]
    invalid = [url for count, url in records if count is None]
    valid.sort(key=lambda item: item[0], reverse=args.descending)

    lines = [url for _, url in valid]
    if invalid:
        lines.append("# Failed to count")
        lines.extend(invalid)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"Saved: {output_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
