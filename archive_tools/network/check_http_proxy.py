"""通过指定 HTTP 代理请求 URL，并报告状态码和耗时。"""

from __future__ import annotations

import argparse
import time
import urllib.request


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--proxy", default="http://127.0.0.1:10809")
    parser.add_argument("--url", default="https://www.google.com/generate_204")
    parser.add_argument("--timeout", type=float, default=15.0)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    opener = urllib.request.build_opener(
        urllib.request.ProxyHandler({"http": args.proxy, "https": args.proxy})
    )
    request = urllib.request.Request(args.url, headers={"User-Agent": "archive-proxy-check/1.0"})
    started = time.perf_counter()
    try:
        with opener.open(request, timeout=args.timeout) as response:
            elapsed = time.perf_counter() - started
            print(f"status={response.status}")
            print(f"elapsed_seconds={elapsed:.3f}")
            print(f"url={response.geturl()}")
            return 0 if 200 <= response.status < 400 else 1
    except Exception as exc:
        elapsed = time.perf_counter() - started
        print(f"error={exc}")
        print(f"elapsed_seconds={elapsed:.3f}")
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
