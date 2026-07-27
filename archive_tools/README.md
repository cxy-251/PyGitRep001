# Extracted Archive Tools

This directory contains the reusable parts extracted from large historical notebooks.
The original projects are no longer maintained. Each script is intentionally standalone,
uses command-line arguments instead of hard-coded local paths, and avoids destructive
behavior unless an explicit flag such as `--apply`, `--move`, or `--replace-source` is used.

## Categories

- `media/`: FFmpeg and FFprobe utilities.
- `files/`: file organization and inventory utilities.
- `youtube/`: archived yt-dlp and browser-cookie helpers.
- `tts/`: text-to-speech helpers.
- `web/`: archived web-scraping prototypes.

## Main extraction map

- `Tools/000DealMV/dealMV.ipynb`, `DealManyVideos.ipynb`, `DealLongVideo.ipynb`
  → `media/` and `files/`.
- `Tools/003DealMP3/covertToMp3.ipynb`
  → `media/convert_audio_to_mp3.py`.
- `Tools/005YTProjMerge/YTExtractor*.ipynb`
  → `youtube/download_channel_media.py`.
- `Tools/001AIGenReanVoice/GenRealVoice.ipynb`
  → `tts/edge_tts_to_mp3.py`.
- `Tools/004Kwai/KwaiExtractor.ipynb`
  → `web/kuaishou_profile_urls.py`.
- `Tools/genFileTreeJson.ipynb`
  → `files/export_file_tree_json.py`.
- `Tools/littleTrick.ipynb`
  → `files/group_items.py` and `files/collect_apks.py`.
- `Tools/002YTProj/LoginYT.py` and `sortChannels.py`
  → the corresponding `youtube/` scripts.

The removed source files remain recoverable from Git history.

## Safety rules

1. Run scripts without `--apply` first when a dry-run mode is available.
2. Use copied test data before moving, renaming, grouping, or replacing files.
3. Treat exported cookie files as password-equivalent secrets.
4. FFmpeg, yt-dlp, Playwright, website DOM selectors, and browser cookie formats may
   have changed since these tools were originally written.
5. These scripts are archived reference implementations, not actively supported products.

## Dependencies

Most file tools use only the Python standard library.

- Media tools: `ffmpeg` and `ffprobe` in `PATH`.
- YouTube download tools: `yt-dlp`.
- Cookie export: `browser-cookie3`.
- Edge TTS: `edge-tts`.
- Kuaishou scraper: `playwright` plus an installed Chromium browser.

Use `python <script> --help` for the exact arguments of each tool.
