
import os
import json
import subprocess
import shutil
import time
import logging
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from datetime import datetime

# 配置文件路径
CONFIG_FILE_PATH = "config.json"
LOG_FILE = "download_log.txt"

# 初始加载配置
def load_config():
    try:
        with open(CONFIG_FILE_PATH, 'r', encoding='utf-8') as f:
            config = json.load(f)
        return config
    except Exception as e:
        log(f"× 错误加载配置文件: {e}")
        return {}

# 配置文件修改事件处理器
class ConfigFileHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path == CONFIG_FILE_PATH:
            log("配置文件已修改，重新加载配置...")
            global config
            config = load_config()

# 日志记录
def log(message: str):
    timestamp = datetime.now().strftime("[%Y-%m-%d %H:%M:%S]")
    print(f"{timestamp} {message}")
    try:
        with open(LOG_FILE, "a", encoding="utf-8") as log_file:
            log_file.write(f"{timestamp} {message}")
    except Exception as e:
        print(f"[日志写入失败] {e}")

# 磁盘空间检查
def check_disk_space():
    total, used, free = shutil.disk_usage("/")
    free_gb = free // (2**30)  # 转换为 GB
    if free_gb <= config["max_disk_space_gb"]:
        log(f"磁盘空间不足 {free_gb}GB，下载已暂停。")
        return False
    return True

# 错误码处理
def handle_error(error_code, url):
    if error_code in config["error_codes"]:
        log(f"【错误】{config['error_codes'][error_code]}：{url}")
    else:
        log(f"【未知错误】视频下载失败，错误码 {error_code}: {url}")

# 下载视频
def download_video(url):
    if not check_disk_space():
        return

    try:
        log(f"开始下载: {url}")
        command = ['yt-dlp', url, '--output', '%(title)s.%(ext)s', '--limit-rate', config['limit_rate']]
        subprocess.run(command, check=True)
        log(f"下载完成: {url}")
    except subprocess.CalledProcessError as e:
        handle_error("timeout", url)
    except FileNotFoundError:
        handle_error("404", url)
    except Exception as e:
        log(f"下载发生意外错误：{e}")

# 配置文件加载
config = load_config()

# 启动监听配置文件变化
def start_config_listener():
    event_handler = ConfigFileHandler()
    observer = Observer()
    observer.schedule(event_handler, path='.', recursive=False)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

# 启动监听
start_config_listener()
