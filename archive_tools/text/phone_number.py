"""格式化或粗略校验中国大陆 11 位手机号码。"""

from __future__ import annotations

import argparse
import re

DIGITS_PATTERN = re.compile(r"\D+")
MAINLAND_MOBILE_PATTERN = re.compile(r"^1[3-9]\d{9}$")


def normalize(value: str) -> str:
    return DIGITS_PATTERN.sub("", value)


def format_number(value: str) -> str:
    digits = normalize(value)[:11]
    parts = [digits[:3], digits[3:7], digits[7:11]]
    return " ".join(part for part in parts if part)


def is_valid_mainland_mobile(value: str) -> bool:
    return MAINLAND_MOBILE_PATTERN.fullmatch(normalize(value)) is not None


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    format_parser = subparsers.add_parser("format")
    format_parser.add_argument("value")
    validate_parser = subparsers.add_parser("validate")
    validate_parser.add_argument("value")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    if args.command == "format":
        print(format_number(args.value))
        return 0
    valid = is_valid_mainland_mobile(args.value)
    print("valid" if valid else "invalid")
    return 0 if valid else 1


if __name__ == "__main__":
    raise SystemExit(main())
