
import json
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from threading import Thread

class ConfigLoader:
    def __init__(self, path, logger):
        self.path = path
        self.logger = logger
        self.data = self.load()
        self.start_watcher()

    def load(self):
        try:
            with open(self.path, encoding="utf-8") as f:
                return json.load(f)
        except Exception as e:
            self.logger.error(f"加载配置失败: {e}")
            return {}

    def reload(self):
        self.data = self.load()
        self.logger.info("配置已重新加载")

    def start_watcher(self):
        class Handler(FileSystemEventHandler):
            def __init__(self, outer):
                self.outer = outer
            def on_modified(self, event):
                if event.src_path.endswith(self.outer.path):
                    self.outer.reload()

        observer = Observer()
        observer.schedule(Handler(self), path="config", recursive=False)
        Thread(target=observer.start, daemon=True).start()
