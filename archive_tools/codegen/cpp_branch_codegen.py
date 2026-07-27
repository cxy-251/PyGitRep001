"""生成 C++ enum、if/else、switch、映射表或分组 dispatch 代码。"""

from __future__ import annotations

import argparse
from pathlib import Path


def enum_code(total: int, enum_name: str, item_prefix: str, start_value: int, per_line: int) -> str:
    entries = []
    for index in range(1, total + 1):
        value = f" = {start_value}" if index == 1 else ""
        entries.append(f"{item_prefix}{index}{value}")
    lines = [f"enum class {enum_name} {{"]
    for offset in range(0, len(entries), per_line):
        lines.append("    " + ", ".join(entries[offset:offset + per_line]) + ",")
    lines.append("};")
    return "\n".join(lines)


def if_else_code(total: int, group_size: int, start_id: int, return_expression: str) -> str:
    blocks = []
    for group_index, offset in enumerate(range(0, total, group_size)):
        values = range(start_id + offset, start_id + min(offset + group_size, total))
        condition = " || ".join(f"id == {value}" for value in values)
        prefix = "if" if group_index == 0 else "else if"
        blocks.append(f"{prefix} ({condition}) {{\n    return {return_expression};\n}}")
    blocks.append("else {\n    return -1;\n}")
    return " ".join(blocks)


def switch_code(total: int, group_size: int, enum_name: str, item_prefix: str) -> str:
    functions = []
    for group_index, offset in enumerate(range(0, total, group_size), start=1):
        end = min(offset + group_size, total)
        lines = [
            f"int HandleGroup{group_index}(int value) {{",
            "    switch (value) {",
        ]
        for index in range(offset + 1, end + 1):
            lines.append(f"        case int({enum_name}::{item_prefix}{index}): return value;")
        lines += ["        default: return -1;", "    }", "}"]
        functions.append("\n".join(lines))
    return "\n\n".join(functions)


def mapping_code(total: int, table_size: int, enum_name: str, item_prefix: str) -> str:
    tables = []
    for table_index, offset in enumerate(range(0, total, table_size), start=1):
        end = min(offset + table_size, total)
        name = f"formIdMap{table_index}"
        lines = [
            f"static const std::unordered_map<{enum_name}, {enum_name}> {name} = {{"
        ]
        for index in range(offset + 1, end + 1):
            item = f"{enum_name}::{item_prefix}{index}"
            lines.append(f"    {{{item}, {item}}},")
        lines.append("};")
        tables.append("\n".join(lines))
    return "\n\n".join(tables)


def dispatch_code(total: int, group_size: int, enum_name: str, item_prefix: str) -> str:
    functions = switch_code(total, group_size, enum_name, item_prefix)
    mapping = ["static const std::unordered_map<int, int(*)(int)> handlerMap = {"]
    for index in range(1, total + 1):
        group = ((index - 1) // group_size) + 1
        mapping.append(
            f"    {{int({enum_name}::{item_prefix}{index}), HandleGroup{group}}},"
        )
    mapping.append("};")
    mapping += [
        "",
        "int Dispatch(int value) {",
        "    const auto it = handlerMap.find(value);",
        "    return it == handlerMap.end() ? -1 : it->second(value);",
        "}",
    ]
    return functions + "\n\n" + "\n".join(mapping)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("kind", choices=("enum", "if-else", "switch", "mapping", "dispatch"))
    parser.add_argument("--total", type=int, required=True)
    parser.add_argument("--group-size", type=int, default=50)
    parser.add_argument("--start-id", type=int, default=1000)
    parser.add_argument("--enum-name", default="TestFormNum")
    parser.add_argument("--item-prefix", default="Num")
    parser.add_argument("--per-line", type=int, default=6)
    parser.add_argument("--return-expression", default="id")
    parser.add_argument("--output", type=Path)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    if args.total <= 0 or args.group_size <= 0 or args.per_line <= 0:
        raise SystemExit("total, group-size and per-line must be positive")
    if args.kind == "enum":
        text = enum_code(
            args.total, args.enum_name, args.item_prefix, args.start_id, args.per_line
        )
    elif args.kind == "if-else":
        text = if_else_code(
            args.total, args.group_size, args.start_id, args.return_expression
        )
    elif args.kind == "switch":
        text = switch_code(args.total, args.group_size, args.enum_name, args.item_prefix)
    elif args.kind == "mapping":
        text = mapping_code(args.total, args.group_size, args.enum_name, args.item_prefix)
    else:
        text = dispatch_code(args.total, args.group_size, args.enum_name, args.item_prefix)

    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text + "\n", encoding="utf-8")
    else:
        print(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
