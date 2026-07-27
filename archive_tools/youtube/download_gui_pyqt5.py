"""PyQt5 图形界面的单 URL yt-dlp 视频下载器。"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("downloads"))
    args = parser.parse_args()
    try:
        from PyQt5.QtCore import QThread, pyqtSignal
        from PyQt5.QtWidgets import (
            QApplication, QFileDialog, QLabel, QLineEdit, QMessageBox,
            QPushButton, QVBoxLayout, QWidget,
        )
    except ImportError as exc:
        raise SystemExit("Install PyQt5") from exc

    class Worker(QThread):
        finished_result = pyqtSignal(int, str)

        def __init__(self, command: list[str]) -> None:
            super().__init__()
            self.command = command

        def run(self) -> None:
            result = subprocess.run(self.command, capture_output=True, text=True)
            self.finished_result.emit(
                result.returncode,
                result.stderr.strip() if result.returncode else "Download completed",
            )

    class Window(QWidget):
        def __init__(self) -> None:
            super().__init__()
            self.worker = None
            self.setWindowTitle("yt-dlp Video Downloader")
            self.url = QLineEdit()
            self.cookies = QLineEdit()
            self.output = QLineEdit(str(args.output))
            browse = QPushButton("Browse output")
            self.download = QPushButton("Download")
            layout = QVBoxLayout(self)
            for label, widget in (
                ("Video URL", self.url),
                ("Cookies file (optional)", self.cookies),
                ("Output directory", self.output),
            ):
                layout.addWidget(QLabel(label))
                layout.addWidget(widget)
            layout.addWidget(browse)
            layout.addWidget(self.download)
            browse.clicked.connect(self.choose_output)
            self.download.clicked.connect(self.start)

        def choose_output(self) -> None:
            selected = QFileDialog.getExistingDirectory(self, "Select output directory")
            if selected:
                self.output.setText(selected)

        def start(self) -> None:
            yt_dlp = shutil.which("yt-dlp")
            if yt_dlp is None:
                QMessageBox.critical(self, "Error", "yt-dlp is not available in PATH")
                return
            if not self.url.text().strip():
                QMessageBox.critical(self, "Error", "Video URL is required")
                return
            output = Path(self.output.text()).expanduser().resolve()
            output.mkdir(parents=True, exist_ok=True)
            command = [
                yt_dlp,
                "-f", "bv*+ba/b",
                "--merge-output-format", "mp4",
                "--output", str(output / "%(title).150s [%(id)s].%(ext)s"),
            ]
            if self.cookies.text().strip():
                command += ["--cookies", str(Path(self.cookies.text()).expanduser().resolve())]
            command.append(self.url.text().strip())
            self.download.setEnabled(False)
            self.worker = Worker(command)
            self.worker.finished_result.connect(self.finish)
            self.worker.start()

        def finish(self, code: int, message: str) -> None:
            self.download.setEnabled(True)
            if code == 0:
                QMessageBox.information(self, "Download", message)
            else:
                QMessageBox.critical(self, "Download failed", message or "Unknown error")

    app = QApplication(sys.argv)
    window = Window()
    window.resize(520, 280)
    window.show()
    return app.exec_()


if __name__ == "__main__":
    raise SystemExit(main())
