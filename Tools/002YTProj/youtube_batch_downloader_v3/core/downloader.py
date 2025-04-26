
import os
import subprocess
import json
import time
import random
import shutil
from pathlib import Path
from urllib.parse import urlparse
from utils.disk_checker import check_disk_space
from utils.stats_collector import StatsCollector
from utils.file_namer import resolve_filename_conflict

class Downloader:
    def __init__(self, config, logger):
        self.config = config
        self.logger = logger
        self.stats = StatsCollector()

    def run(self):
        channels = self.load_channels(self.config.data.get("channel_list_file", "channels.txt"))
        for channel in channels:
            self.logger.info(f"开始处理频道：{channel}")
            folder_name = urlparse(channel).path.strip("/")
            if folder_name.startswith("_"):
                self.logger.info(f"_开头的频道，跳过：{channel}")
                continue
            video_ids = self.get_video_ids(channel)

            # folder_name = url.strip("/").split("@")[-1] # 又加了/videos # .split("/")[0]
            if folder_name.startswith("@"):
                folder_name = folder_name.split("/")[0][1:]  # 移除 @

            for vid in video_ids:
                if not check_disk_space(self.config.data["max_disk_space_gb"], self.logger):
                    return
                self.download_video(vid, folder_name)
                time.sleep(random.uniform(*self.config.data["sleep_interval"]))
        self.logger.info("任务完成")
        self.logger.info(self.stats.summary())

    def load_channels(self, path):
        if not os.path.exists(path):
            self.logger.warning(f"频道文件不存在: {path}")
            return []
        with open(path, encoding="utf-8") as f:
            return [line.strip() for line in f if line.strip()]

    def get_video_ids(self, channel_url):
        cmd = ["yt-dlp", "--flat-playlist", "-J", channel_url]
        if self.config.data.get("cookies_file"):
            cmd += ["--cookies", self.config.data["cookies_file"]]
        try:
            res = subprocess.run(cmd, capture_output=True, text=True, check=True)
            data = json.loads(res.stdout)
            return [f"https://www.youtube.com/watch?v={e['id']}" for e in data.get("entries", [])]
        except Exception as e:
            self.logger.error(f"获取视频失败：{e}")
            return []
# yt-dlp -f bestvideo+bestaudio --merge-output-format mp4 -o "%(upload_date)s - %(title)s.%(ext)s" --download-archive 000downloaded.txt https://www.youtube.com/@ssunbiki
    def download_video(self, url, folder_name):
        target_dir = os.path.join(self.config.data["download_path"], folder_name)
        os.makedirs(target_dir, exist_ok=True)
        archive_path = os.path.join(target_dir, "000downloaded.txt")
        out_template = os.path.join(target_dir, "%(upload_date)s - %(title)s.%(ext)s")
        # final_path = resolve_filename_conflict(out_template)
        final_path = out_template
        cmd = ["yt-dlp", url, "--output", final_path, "--cache-dir", self.config.data["cache_dir"]]
        cmd += ["-f", "bestvideo+bestaudio", "--merge-output-format", "mp4"]
        cmd += ["--download-archive", archive_path]

        cmd += [
            "--add-header", "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36 Edg/123.0.0.0",
            "--add-header", "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8",
            "--add-header", "Referer: https://www.youtube.com/"
        ]
        if self.config.data.get("cookies_file"):
            cmd += ["--cookies", self.config.data["cookies_file"]]
        if self.config.data.get("limit_rate") != "unlimited":
            cmd += ["--limit-rate", self.config.data["limit_rate"]]
        try:
            subprocess.run(cmd, check=True)
            self.logger.info(f"下载成功: {url}")
            self.stats.success += 1
        except subprocess.CalledProcessError:
            self.logger.warning(f"下载失败（跳过）: {url}")
            self.stats.fail += 1
