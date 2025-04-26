
import os
import json
import subprocess
import shutil
import time
import random
import logging
from pathlib import Path
from datetime import datetime
from threading import Thread
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

CONFIG_FILE_PATH = "config.json"
CHANNEL_LIST_FILE = "channels.txt"
LOG_FILE_DIR = "logs"
CACHE_DIR = "cache"

os.makedirs(LOG_FILE_DIR, exist_ok=True)
os.makedirs(CACHE_DIR, exist_ok=True)

# 日志设置（按日期归档）
today_str = datetime.now().strftime("%Y-%m-%d")
log_path = os.path.join(LOG_FILE_DIR, f"{today_str}.log")
logging.basicConfig(filename=log_path, level=logging.INFO, format='[%(asctime)s] %(message)s', encoding='utf-8')

def log(msg): 
    print(f"[{datetime.now():%Y-%m-%d %H:%M:%S}] {msg}")
    logging.info(msg)

# 配置加载与监听
class Config:
    def __init__(self, path):
        self.path = path
        self.data = self.load()
    
    def load(self):
        try:
            with open(self.path, 'r', encoding='utf-8') as f:
                return json.load(f)
        except Exception as e:
            log(f"[配置错误] 无法读取配置: {e}")
            return {}
    
    def reload(self):
        self.data = self.load()
        log("配置已重新加载")

class ConfigHandler(FileSystemEventHandler):
    def __init__(self, config):
        self.config = config
    def on_modified(self, event):
        if event.src_path.endswith(CONFIG_FILE_PATH):
            self.config.reload()

# 初始化配置与监听
config = Config(CONFIG_FILE_PATH)
observer = Observer()
observer.schedule(ConfigHandler(config), path='.', recursive=False)
observer_thread = Thread(target=observer.start, daemon=True)
observer_thread.start()

# 读取频道链接表
def load_channels():
    try:
        with open(CHANNEL_LIST_FILE, 'r', encoding='utf-8') as f:
            return [line.strip() for line in f if line.strip()]
    except FileNotFoundError:
        log(f"[错误] 未找到频道表 {CHANNEL_LIST_FILE}")
        return []

# 检查磁盘空间
def check_disk_space(min_free_gb):
    total, used, free = shutil.disk_usage("/")
    free_gb = free // (2**30)
    if free_gb <= min_free_gb:
        log(f"[警告] 磁盘剩余 {free_gb}GB，低于限制 {min_free_gb}GB，停止下载")
        return False
    return True

# 清理无用缓存
def clean_cache():
    removed = 0
    for file in os.listdir(CACHE_DIR):
        if file.endswith('.part') or file.endswith('.tmp'):
            try:
                os.remove(os.path.join(CACHE_DIR, file))
                removed += 1
            except:
                continue
    log(f"[清理] 已移除无用缓存文件: {removed} 个")

# 视频下载
def download_video(video_url):
    if not check_disk_space(config.data.get("max_disk_space_gb", 20)):
        return False
    cookies_file = config.data.get("cookies_file")
    limit_rate = config.data.get("limit_rate", "0")
    sleep_range = config.data.get("sleep_interval", [5, 10])
    out_template = os.path.join(config.data.get("download_path", "."), "%(title).200s.%(ext)s")

    command = ["yt-dlp", video_url, "--output", out_template, "--cache-dir", CACHE_DIR]
    if cookies_file:
        command += ["--cookies", cookies_file]
    if limit_rate and limit_rate != "unlimited":
        command += ["--limit-rate", limit_rate]
    
    try:
        subprocess.run(command, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        log(f"[成功] 下载完成：{video_url}")
        return True
    except subprocess.CalledProcessError:
        log(f"[失败] yt-dlp 错误（跳过该视频）：{video_url}")
    except Exception as e:
        log(f"[异常] 下载失败 {video_url}: {e}")
    
    return False

# 批量处理频道
def process_channels():
    stats = {"channels": 0, "videos": 0, "success": 0}
    channels = load_channels()
    stats["channels"] = len(channels)
    
    for url in channels:
        log(f"[频道] 开始处理：{url}")
        command = ["yt-dlp", "--flat-playlist", "-J", url]
        if config.data.get("cookies_file"):
            command += ["--cookies", config.data["cookies_file"]]
        
        try:
            result = subprocess.run(command, capture_output=True, text=True, check=True)
            info = json.loads(result.stdout)
            entries = info.get("entries", [])
            stats["videos"] += len(entries)
            for entry in entries:
                video_url = f"https://www.youtube.com/watch?v={entry['id']}"
                if download_video(video_url):
                    stats["success"] += 1
                time.sleep(random.uniform(*config.data.get("sleep_interval", [5, 10])))
        except Exception as e:
            log(f"[错误] 频道处理失败：{url} | {e}")
    
    log(f"[统计] 频道数: {stats['channels']} | 视频数: {stats['videos']} | 成功: {stats['success']}")
    return stats

if __name__ == "__main__":
    clean_cache()
    stats = process_channels()
    observer.stop()
