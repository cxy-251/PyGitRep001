"""生成 Python 表驱动处理函数及 pytest 参数化测试。"""

from __future__ import annotations

import argparse
from pathlib import Path


def generate(function_name: str, count: int, default: str) -> str:
    lines = [
        f"def {function_name}(code: int) -> str:",
        f"    responses = {{i: f'Response {{i}}' for i in range({count})}}",
        f"    return responses.get(code, {default!r})",
        "",
        "",
        f"TEST_CASES = [(i, f'Response {{i}}') for i in range({count})]",
        f"TEST_CASES.append(({count}, {default!r}))",
        "",
        "",
        "def test_generated_handler():",
        f"    for code, expected in TEST_CASES:",
        f"        assert {function_name}(code) == expected",
    ]
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--name", default="handle_code")
    parser.add_argument("--count", type=int, default=100)
    parser.add_argument("--default", default="Unknown")
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    if args.count <= 0 or not args.name.isidentifier():
        raise SystemExit("count must be positive and name must be a valid Python identifier")
    text = generate(args.name, args.count, args.default)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text + "\n", encoding="utf-8")
    else:
        print(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
