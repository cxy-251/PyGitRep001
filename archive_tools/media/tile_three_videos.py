"""把三个视频缩放后并排合成为一个横向视频。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("videos", nargs=3, type=Path)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--tile-width", type=int, default=720)
    parser.add_argument("--tile-height", type=int, default=1280)
    parser.add_argument("--fps", type=int, default=30)
    parser.add_argument("--audio-from", type=int, choices=(0, 1, 2, 3), default=0,
                        help="0 disables audio; 1-3 selects an input video's audio")
    parser.add_argument("--overwrite", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffmpeg = shutil.which("ffmpeg")
    if ffmpeg is None:
        raise SystemExit("ffmpeg must be available in PATH")
    inputs = [path.expanduser().resolve() for path in args.videos]
    missing = [str(path) for path in inputs if not path.is_file()]
    if missing:
        raise SystemExit("Missing input files: " + ", ".join(missing))

    output = args.output.expanduser().resolve()
    if output.exists() and not args.overwrite:
        raise SystemExit(f"Output exists; add --overwrite: {output}")
    output.parent.mkdir(parents=True, exist_ok=True)

    filters = []
    for index in range(3):
        filters.append(
            f"[{index}:v]scale={args.tile_width}:{args.tile_height}:"
            f"force_original_aspect_ratio=decrease,"
            f"pad={args.tile_width}:{args.tile_height}:(ow-iw)/2:(oh-ih)/2,"
            f"fps={args.fps},setsar=1[v{index}]"
        )
    filters.append("[v0][v1][v2]hstack=inputs=3[vout]")

    command = [ffmpeg, "-hide_banner", "-loglevel", "error"]
    if args.overwrite:
        command.append("-y")
    else:
        command.append("-n")
    for path in inputs:
        command += ["-i", str(path)]
    command += ["-filter_complex", ";".join(filters), "-map", "[vout]"]
    if args.audio_from:
        command += ["-map", f"{args.audio_from - 1}:a?", "-c:a", "aac", "-b:a", "128k"]
    else:
        command += ["-an"]
    command += ["-c:v", "libx264", "-preset", "medium", "-crf", "20", "-r", str(args.fps), str(output)]

    result = subprocess.run(command)
    return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
