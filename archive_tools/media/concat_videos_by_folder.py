"""按文件名顺序合并每个直接子目录中的视频，并统一尺寸、帧率和编码。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import tempfile
from pathlib import Path

VIDEO_EXTENSIONS = {".mp4", ".mkv", ".avi", ".mov", ".flv", ".webm", ".m4v"}


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input_root", type=Path)
    parser.add_argument("output_root", type=Path)
    parser.add_argument("--width", type=int, default=720)
    parser.add_argument("--height", type=int, default=1280)
    parser.add_argument("--fps", type=int, default=30)
    parser.add_argument("--keep-audio", action="store_true")
    parser.add_argument("--overwrite", action="store_true")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    ffmpeg = shutil.which("ffmpeg")
    if ffmpeg is None:
        raise SystemExit("ffmpeg must be available in PATH")
    input_root = args.input_root.expanduser().resolve()
    output_root = args.output_root.expanduser().resolve()
    if not input_root.is_dir():
        raise SystemExit(f"Input root does not exist: {input_root}")
    output_root.mkdir(parents=True, exist_ok=True)
    failures = 0

    for folder in sorted(path for path in input_root.iterdir() if path.is_dir()):
        videos = sorted(
            path for path in folder.iterdir()
            if path.is_file() and path.suffix.lower() in VIDEO_EXTENSIONS
        )
        if not videos:
            continue
        output = output_root / f"{folder.name}_merged.mp4"
        if output.exists() and not args.overwrite:
            print(f"SKIP exists: {output}")
            continue

        with tempfile.TemporaryDirectory(prefix="concat-normalized-") as temp:
            temp_root = Path(temp)
            normalized: list[Path] = []
            for index, source in enumerate(videos):
                target = temp_root / f"{index:06d}.mp4"
                command = [
                    ffmpeg, "-hide_banner", "-loglevel", "error", "-y",
                    "-i", str(source),
                    "-vf",
                    f"scale={args.width}:{args.height}:force_original_aspect_ratio=decrease,"
                    f"pad={args.width}:{args.height}:(ow-iw)/2:(oh-ih)/2,"
                    f"fps={args.fps},setsar=1",
                    "-c:v", "libx264", "-preset", "fast", "-crf", "21",
                ]
                if args.keep_audio:
                    command += ["-c:a", "aac", "-b:a", "128k", "-ar", "44100"]
                else:
                    command += ["-an"]
                command.append(str(target))
                result = subprocess.run(command)
                if result.returncode != 0:
                    failures += 1
                    print(f"FAIL normalize: {source}")
                    normalized = []
                    break
                normalized.append(target)

            if not normalized:
                continue
            list_file = temp_root / "files.txt"
            list_file.write_text(
                "".join(f"file '{path.as_posix()}'\n" for path in normalized),
                encoding="utf-8",
            )
            command = [
                ffmpeg, "-hide_banner", "-loglevel", "error",
                "-y" if args.overwrite else "-n",
                "-f", "concat", "-safe", "0", "-i", str(list_file),
                "-c", "copy", str(output),
            ]
            result = subprocess.run(command)
            if result.returncode != 0:
                failures += 1
                print(f"FAIL concat: {folder}")
            else:
                print(f"OK   {folder} -> {output}")

    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
