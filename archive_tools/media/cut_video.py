"""使用 FFmpeg 截取视频片段；默认流复制，也可选择重新编码。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--start", default="0", help="FFmpeg time value, for example 90 or 00:01:30")
    end_group = parser.add_mutually_exclusive_group(required=True)
    end_group.add_argument("--end", help="Absolute end timestamp")
    end_group.add_argument("--duration", help="Clip duration")
    parser.add_argument("--reencode", action="store_true")
    parser.add_argument("--overwrite", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffmpeg = shutil.which("ffmpeg")
    if ffmpeg is None:
        raise SystemExit("ffmpeg must be available in PATH")
    source = args.input.expanduser().resolve()
    output = args.output.expanduser().resolve()
    if not source.is_file():
        raise SystemExit(f"Input does not exist: {source}")
    if output.exists() and not args.overwrite:
        raise SystemExit(f"Output exists; add --overwrite: {output}")
    output.parent.mkdir(parents=True, exist_ok=True)

    command = [ffmpeg, "-hide_banner", "-loglevel", "error"]
    command.append("-y" if args.overwrite else "-n")
    command += ["-ss", args.start, "-i", str(source)]
    if args.end is not None:
        command += ["-to", args.end]
    else:
        command += ["-t", args.duration]
    if args.reencode:
        command += ["-c:v", "libx264", "-c:a", "aac"]
    else:
        command += ["-c", "copy"]
    command.append(str(output))
    return subprocess.run(command).returncode


if __name__ == "__main__":
    raise SystemExit(main())
