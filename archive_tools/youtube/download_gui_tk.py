"""Tkinter 图形界面的单 URL yt-dlp 视频下载器。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import threading
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, messagebox


class DownloaderApp:
    def __init__(self, root: tk.Tk, initial_output: Path) -> None:
        self.root = root
        root.title("yt-dlp Video Downloader")
        root.geometry("620x260")

        self.url = tk.StringVar()
        self.cookies = tk.StringVar()
        self.output = tk.StringVar(value=str(initial_output))

        self._row("Video URL", self.url, 0)
        self._row("Cookies file (optional)", self.cookies, 1)
        self._row("Output directory", self.output, 2, browse=True)

        self.button = tk.Button(root, text="Download", command=self.start)
        self.button.grid(row=3, column=1, padx=8, pady=18, sticky="ew")
        self.status = tk.StringVar(value="Ready")
        tk.Label(root, textvariable=self.status, anchor="w").grid(
            row=4, column=0, columnspan=3, padx=8, sticky="ew"
        )
        root.grid_columnconfigure(1, weight=1)

    def _row(self, label: str, variable: tk.StringVar, row: int, browse: bool = False) -> None:
        tk.Label(self.root, text=label).grid(row=row, column=0, padx=8, pady=8, sticky="w")
        tk.Entry(self.root, textvariable=variable).grid(
            row=row, column=1, padx=8, pady=8, sticky="ew"
        )
        if browse:
            tk.Button(self.root, text="Browse", command=self.choose_output).grid(
                row=row, column=2, padx=8, pady=8
            )

    def choose_output(self) -> None:
        selected = filedialog.askdirectory()
        if selected:
            self.output.set(selected)

    def start(self) -> None:
        if not self.url.get().strip():
            messagebox.showerror("Error", "Video URL is required")
            return
        self.button.config(state=tk.DISABLED)
        self.status.set("Downloading...")
        threading.Thread(target=self.download, daemon=True).start()

    def download(self) -> None:
        yt_dlp = shutil.which("yt-dlp")
        if yt_dlp is None:
            self.finish(False, "yt-dlp is not available in PATH")
            return
        output = Path(self.output.get()).expanduser().resolve()
        output.mkdir(parents=True, exist_ok=True)
        command = [
            yt_dlp,
            "-f", "bv*+ba/b",
            "--merge-output-format", "mp4",
            "--output", str(output / "%(title).150s [%(id)s].%(ext)s"),
        ]
        cookie_value = self.cookies.get().strip()
        if cookie_value:
            command += ["--cookies", str(Path(cookie_value).expanduser().resolve())]
        command.append(self.url.get().strip())
        result = subprocess.run(command, capture_output=True, text=True)
        message = result.stderr.strip() if result.returncode else "Download completed"
        self.finish(result.returncode == 0, message)

    def finish(self, success: bool, message: str) -> None:
        def update() -> None:
            self.button.config(state=tk.NORMAL)
            self.status.set("Ready" if success else "Failed")
            (messagebox.showinfo if success else messagebox.showerror)(
                "Download", message or "Unknown error"
            )
        self.root.after(0, update)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("downloads"))
    args = parser.parse_args()
    root = tk.Tk()
    DownloaderApp(root, args.output)
    root.mainloop()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
