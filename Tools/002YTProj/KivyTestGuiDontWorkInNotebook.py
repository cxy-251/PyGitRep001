from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.textinput import TextInput
from kivy.uix.boxlayout import BoxLayout

class TodoApp(App):
    def build(self):
        layout = BoxLayout(orientation='vertical')

        self.task_input = TextInput(hint_text="Enter task")
        layout.add_widget(self.task_input)

        self.add_button = Button(text="Add Task")
        self.add_button.bind(on_press=self.add_task)
        layout.add_widget(self.add_button)

        return layout

    def add_task(self, instance):
        task = self.task_input.text
        print(f"Adding task: {task}")

if __name__ == '__main__':
    TodoApp().run()
