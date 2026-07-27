"""一个最小 Kivy 待办事项示例；项目只保存在当前进程内。"""

from __future__ import annotations

import argparse


def main() -> int:
    argparse.ArgumentParser(description=__doc__).parse_args()
    try:
        from kivy.app import App
        from kivy.uix.boxlayout import BoxLayout
        from kivy.uix.button import Button
        from kivy.uix.label import Label
        from kivy.uix.textinput import TextInput
    except ImportError as exc:
        raise SystemExit("Install kivy") from exc

    class TodoApp(App):
        def build(self):
            self.layout = BoxLayout(orientation="vertical")
            self.input = TextInput(hint_text="Enter task", multiline=False, size_hint_y=None, height=48)
            button = Button(text="Add Task", size_hint_y=None, height=48)
            button.bind(on_press=self.add_task)
            self.layout.add_widget(self.input)
            self.layout.add_widget(button)
            return self.layout

        def add_task(self, _instance) -> None:
            value = self.input.text.strip()
            if value:
                self.layout.add_widget(Label(text=value, size_hint_y=None, height=36), index=1)
                self.input.text = ""

    TodoApp().run()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
