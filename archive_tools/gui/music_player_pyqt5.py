"""一个最小 PyQt5 本地音频播放器。"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("file", nargs="?", type=Path)
    args = parser.parse_args()
    try:
        from PyQt5.QtCore import QUrl, Qt
        from PyQt5.QtMultimedia import QMediaContent, QMediaPlayer
        from PyQt5.QtWidgets import (
            QApplication, QFileDialog, QLabel, QPushButton, QSlider,
            QVBoxLayout, QWidget,
        )
    except ImportError as exc:
        raise SystemExit("Install PyQt5 with multimedia support") from exc

    class Player(QWidget):
        def __init__(self) -> None:
            super().__init__()
            self.setWindowTitle("Simple Music Player")
            self.player = QMediaPlayer(self)
            self.label = QLabel("No file selected")
            open_button = QPushButton("Open")
            play_button = QPushButton("Play")
            pause_button = QPushButton("Pause")
            stop_button = QPushButton("Stop")
            volume = QSlider(Qt.Horizontal)
            volume.setRange(0, 100)
            volume.setValue(70)

            layout = QVBoxLayout(self)
            for widget in (self.label, open_button, play_button, pause_button, stop_button, volume):
                layout.addWidget(widget)
            open_button.clicked.connect(self.open_file)
            play_button.clicked.connect(self.player.play)
            pause_button.clicked.connect(self.player.pause)
            stop_button.clicked.connect(self.player.stop)
            volume.valueChanged.connect(self.player.setVolume)
            if args.file:
                self.load(args.file)

        def load(self, path: Path) -> None:
            resolved = path.expanduser().resolve()
            if resolved.is_file():
                self.player.setMedia(QMediaContent(QUrl.fromLocalFile(str(resolved))))
                self.label.setText(resolved.name)

        def open_file(self) -> None:
            selected, _ = QFileDialog.getOpenFileName(
                self, "Open audio", "", "Audio (*.mp3 *.wav *.ogg *.m4a *.flac)"
            )
            if selected:
                self.load(Path(selected))
                self.player.play()

    app = QApplication(sys.argv)
    window = Player()
    window.resize(420, 240)
    window.show()
    return app.exec_()


if __name__ == "__main__":
    raise SystemExit(main())
