"""Archive scraper: collect visible Kuaishou short-video URLs from a profile page."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from urllib.parse import urljoin


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("profile_url")
    parser.add_argument("-o", "--output", type=Path, default=Path("kuaishou_videos.json"))
    parser.add_argument("--max-scrolls", type=int, default=30)
    parser.add_argument("--stable-rounds", type=int, default=3)
    parser.add_argument("--wait-ms", type=int, default=1000)
    parser.add_argument("--headless", action="store_true")
    parser.add_argument("--user-data-dir", type=Path)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    try:
        from playwright.sync_api import sync_playwright
    except ImportError as exc:
        raise SystemExit(
            "Install dependencies: python -m pip install playwright && playwright install chromium"
        ) from exc

    collected: set[str] = set()
    stable = 0

    with sync_playwright() as playwright:
        if args.user_data_dir:
            context = playwright.chromium.launch_persistent_context(
                str(args.user_data_dir.expanduser().resolve()),
                headless=args.headless,
            )
            browser = None
        else:
            browser = playwright.chromium.launch(headless=args.headless)
            context = browser.new_context()

        page = context.new_page()
        page.goto(args.profile_url, wait_until="domcontentloaded", timeout=60_000)

        for _ in range(args.max_scrolls):
            for element in page.locator("a[href*='/short-video/']").all():
                href = element.get_attribute("href")
                if href:
                    collected.add(urljoin("https://www.kuaishou.com", href))
            previous = len(collected)
            page.mouse.wheel(0, 2400)
            page.wait_for_timeout(args.wait_ms)

            for element in page.locator("a[href*='/short-video/']").all():
                href = element.get_attribute("href")
                if href:
                    collected.add(urljoin("https://www.kuaishou.com", href))

            if len(collected) == previous:
                stable += 1
                if stable >= args.stable_rounds:
                    break
            else:
                stable = 0

        context.close()
        if browser is not None:
            browser.close()

    output = args.output.expanduser().resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(
        json.dumps(sorted(collected), ensure_ascii=False, indent=2),
        encoding="utf-8",
    )
    print(f"Saved {len(collected)} URLs: {output}")
    print("This archived scraper depends on Kuaishou DOM selectors and may require updates.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
