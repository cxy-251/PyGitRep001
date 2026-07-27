"""一个最小 Tkinter 文本编辑器，支持打开和保存 UTF-8 文本。"""

from __future__ import annotations

import argparse
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, messagebox


class TextEditor:
    def __init__(self, root: tk.Tk, initial: Path | None = None) -> None:
        self.root = root
        self.path: Path | None = None
        root.title("Simple Text Editor")
        self.text = tk.Text(root, wrap="word", undo=True)
        self.text.pack(expand=True, fill="both")

        menu = tk.Menu(root)
        file_menu = tk.Menu(menu, tearoff=False)
        file_menu.add_command(label="Open", command=self.open_file)
        file_menu.add_command(label="Save", command=self.save)
        file_menu.add_command(label="Save As", command=self.save_as)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=root.destroy)
        menu.add_cascade(label="File", menu=file_menu)
        root.config(menu=menu)
        root.bind("<Control-s>", lambda _event: self.save())
        if initial:
            self.load(initial)

    def load(self, path: Path) -> None:
        try:
            content = path.read_text(encoding="utf-8")
        except Exception as exc:
            messagebox.showerror("Open failed", str(exc))
            return
        self.path = path
        self.text.delete("1.0", tk.END)
        self.text.insert("1.0", content)
        self.root.title(f"Simple Text Editor — {path.name}")

    def open_file(self) -> None:
        selected = filedialog.askopenfilename(filetypes=[("Text files", "*.txt"), ("All files", "*")])
        if selected:
            self.load(Path(selected))

    def save(self) -> None:
        if self.path is None:
            self.save_as()
            return
        try:
            self.path.write_text(self.text.get("1.0", "end-1c"), encoding="utf-8")
        except Exception as exc:
            messagebox.showerror("Save failed", str(exc))

    def save_as(self) -> None:
        selected = filedialog.asksaveasfilename(
            defaultextension=".txt",
            filetypes=[("Text files", "*.txt"), ("All files", "*")],
        )
        if selected:
            self.path = Path(selected)
            self.save()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("file", nargs="?", type=Path)
    args = parser.parse_args()
    root = tk.Tk()
    root.geometry("900x650")
    TextEditor(root, args.file)
    root.mainloop()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
