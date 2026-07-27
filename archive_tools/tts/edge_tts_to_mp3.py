"""Generate an MP3 with Microsoft Edge TTS."""

from __future__ import annotations

import argparse
import asyncio
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--text")
    source.add_argument("--text-file", type=Path)
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--voice", default="zh-CN-XiaoxiaoNeural")
    parser.add_argument("--rate", default="+0%")
    parser.add_argument("--volume", default="+0%")
    return parser


async def synthesize(text: str, output: Path, voice: str, rate: str, volume: str) -> None:
    try:
        import edge_tts
    except ImportError as exc:
        raise SystemExit("Install dependency: python -m pip install edge-tts") from exc

    output.parent.mkdir(parents=True, exist_ok=True)
    communicator = edge_tts.Communicate(
        text=text,
        voice=voice,
        rate=rate,
        volume=volume,
    )
    await communicator.save(str(output))


def main() -> int:
    args = build_parser().parse_args()
    if args.text_file:
        text = args.text_file.expanduser().read_text(encoding="utf-8")
    else:
        text = args.text or ""
    if not text.strip():
        raise SystemExit("Text is empty")

    output = args.output.expanduser().resolve()
    asyncio.run(synthesize(text, output, args.voice, args.rate, args.volume))
    print(f"Saved: {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
