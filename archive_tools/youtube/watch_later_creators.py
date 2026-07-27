"""通过 Playwright 登录配置读取 YouTube 播放列表中可见视频的发布者。"""

from __future__ import annotations

import argparse
import csv
import time
from pathlib import Path
from urllib.parse import urljoin


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--playlist-url", default="https://www.youtube.com/playlist?list=WL")
    parser.add_argument("--profile-dir", type=Path, default=Path("youtube_profile"))
    parser.add_argument("--output", type=Path, default=Path("youtube_creators.csv"))
    parser.add_argument("--max-scrolls", type=int, default=200)
    parser.add_argument("--delay", type=float, default=1.0)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    try:
        from playwright.sync_api import sync_playwright
    except ImportError as exc:
        raise SystemExit("Install playwright and its Chromium browser first") from exc

    rows: set[tuple[str, str]] = set()
    with sync_playwright() as playwright:
        context = playwright.chromium.launch_persistent_context(
            str(args.profile_dir.expanduser().resolve()), headless=False
        )
        page = context.pages[0] if context.pages else context.new_page()
        page.goto(args.playlist_url, wait_until="domcontentloaded")
        input("Log in and wait for the playlist to load, then press Enter...")

        previous_height = -1
        stable_rounds = 0
        for _ in range(args.max_scrolls):
            height = page.evaluate("document.documentElement.scrollHeight")
            page.evaluate("window.scrollTo(0, document.documentElement.scrollHeight)")
            time.sleep(args.delay)
            if height == previous_height:
                stable_rounds += 1
                if stable_rounds >= 3:
                    break
            else:
                stable_rounds = 0
            previous_height = height

        for renderer in page.query_selector_all("ytd-playlist-video-renderer"):
            candidates = renderer.query_selector_all(
                "ytd-channel-name a[href], #channel-name a[href], a[href^='/@'], a[href^='/channel/']"
            )
            for anchor in candidates:
                href = anchor.get_attribute("href")
                name = anchor.inner_text().strip()
                if href and name:
                    rows.add((name, urljoin("https://www.youtube.com", href)))
                    break
        context.close()

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8-sig") as file:
        writer = csv.writer(file)
        writer.writerow(["creator_name", "creator_url"])
        writer.writerows(sorted(rows))
    print(f"Wrote {len(rows)} creators to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
