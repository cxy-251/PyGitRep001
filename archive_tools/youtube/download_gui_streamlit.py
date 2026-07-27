"""Streamlit 图形界面的单 URL yt-dlp 下载器。"""

from __future__ import annotations

import shutil
import subprocess
from pathlib import Path

try:
    import streamlit as st
except ImportError as exc:
    raise SystemExit("Install streamlit, then run: streamlit run download_gui_streamlit.py") from exc

st.title("yt-dlp Video Downloader")
url = st.text_input("Video URL")
output_value = st.text_input("Output directory", "downloads")
cookies_value = st.text_input("Cookies file (optional)")

if st.button("Download"):
    yt_dlp = shutil.which("yt-dlp")
    if yt_dlp is None:
        st.error("yt-dlp is not available in PATH")
    elif not url.strip():
        st.warning("Video URL is required")
    else:
        output = Path(output_value).expanduser().resolve()
        output.mkdir(parents=True, exist_ok=True)
        command = [
            yt_dlp,
            "-f", "bv*+ba/b",
            "--merge-output-format", "mp4",
            "--output", str(output / "%(title).150s [%(id)s].%(ext)s"),
        ]
        if cookies_value.strip():
            command += ["--cookies", str(Path(cookies_value).expanduser().resolve())]
        command.append(url.strip())
        with st.spinner("Downloading..."):
            result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode == 0:
            st.success("Download completed")
            if result.stdout:
                st.code(result.stdout)
        else:
            st.error("Download failed")
            st.code(result.stderr or "No error output")
