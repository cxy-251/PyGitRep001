"""Export YouTube cookies from a local browser to Netscape cookie format."""

from __future__ import annotations

import argparse
import time
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--browser", choices=("firefox", "chrome", "edge"), default="firefox")
    parser.add_argument("-o", "--output", type=Path, default=Path("cookies.txt"))
    return parser


def main() -> int:
    args = build_parser().parse_args()
    try:
        import browser_cookie3
    except ImportError as exc:
        raise SystemExit("Install dependency: python -m pip install browser-cookie3") from exc

    loader = getattr(browser_cookie3, args.browser)
    cookies = loader(domain_name="youtube.com")
    output = args.output.expanduser().resolve()
    output.parent.mkdir(parents=True, exist_ok=True)

    lines = ["# Netscape HTTP Cookie File\n"]
    now_plus_30_days = int(time.time()) + (30 * 24 * 3600)
    for cookie in cookies:
        domain = cookie.domain or ".youtube.com"
        include_subdomains = "TRUE" if domain.startswith(".") else "FALSE"
        path = cookie.path or "/"
        secure = "TRUE" if cookie.secure else "FALSE"
        expires = int(cookie.expires or now_plus_30_days)
        lines.append(
            f"{domain}\t{include_subdomains}\t{path}\t{secure}\t"
            f"{expires}\t{cookie.name}\t{cookie.value}\n"
        )

    output.write_text("".join(lines), encoding="utf-8")
    print(f"Saved {len(lines) - 1} cookies: {output}")
    print("Treat this file as a password-equivalent secret.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
