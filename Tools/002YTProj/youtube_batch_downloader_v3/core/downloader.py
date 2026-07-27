# -*- coding: utf-8 -*-

import os
import shutil
import msvcrt
import subprocess
import json
import time
import random
import chardet
from urllib.parse import urlparse
from utils.disk_checker import check_disk_space
from utils.stats_collector import StatsCollector
from utils.reget_cookies import reget_cookies
from utils.file_namer import resolve_filename_conflict

RETRY_LIMITS = 3
class Downloader:
    def __init__(self, config, logger):
        self.config = config
        self.logger = logger
        self.stats = StatsCollector()
        self.reget_cookies_nums = 0
        self.retry_reget_cookies_times = 0
        self.use_safty_cmd_DRM = 0
        self.cant_deal_DRM = False

    def run(self):
        channels = self.load_channels(self.config.data.get("channel_list_file", "channels.txt"))
        for channel in channels:
            self.logger.info(f"开始处理频道: {channel}")
            folder_name = urlparse(channel).path.strip("/") or "load_channel_failed"
            if folder_name.startswith("_"):
                self.logger.info(f"_开头的频道, 跳过: {channel}")
                continue
            video_ids = self.get_video_ids(channel)

            # folder_name = url.strip("/").split("@")[-1] # 又加了/videos # .split("/")[0]
            if folder_name.startswith("@"):
                folder_name = folder_name.split("/")[0][1:]  # 移除 @

            for vid in video_ids:
                if not check_disk_space(self.config.data["max_disk_space_gb"], self.logger):
                    self.logger.warning("磁盘空间不足, 任务中止")
                    # self.logger.info(self.stats.summary())
                    return
                res = self.download_video(vid, folder_name)
                time.sleep(random.uniform(*self.config.data["sleep_interval"]))
                while not res:
                    if self.retry_reget_cookies_times >= RETRY_LIMITS or self.cant_deal_DRM:
                        self.logger.warning(f"达到最大重试次数3, 跳过下载: {vid}")
                        self.retry_reget_cookies_times = 0
                        self.cant_deal_DRM = False
                        break
                    self.reget_cookies_nums += 1
                    reget_cookies(self.config, self.logger)
                    self.logger.warning(f"下载失败, （重新获取cookies重试）:{vid}, 重新获取cookies次数{self.reget_cookies_nums}")
                    res = self.download_video(vid, folder_name)
                    if not res:
                        self.retry_reget_cookies_times += 1
                    time.sleep(random.uniform(*self.config.data["sleep_interval"]))
                # TODO::shutilMoveVideoByWxH(folder_name/ABC/WxH)
        self.logger.info("任务完成")
        self.logger.info(self.stats.summary())

    def load_channels(self, path):
        if not os.path.exists(path):
            self.logger.warning(f"频道文件不存在: {path}")
            return []
        with open(path, encoding="utf-8") as f:
            return [line.strip() for line in f if line.strip()]
        
    @staticmethod
    def is_file_in_use(file_path):
        
        try:
            with open(file_path, 'r+b') as f:
                # msvcrt.locking(f.fileno(), msvcrt.LK_NBLCK, os.path.getsize(file_path))
                # OverflowError: Python int too large to convert to C long
                # # 根本原因：msvcrt.locking() 的第 3 个参数 nbytes 是一个 C long 类型（在 32 位系统上最大为 2,147,483,647，即约 2GB），而你试图加锁的文件大小可能远超这个限制，比如超过了 2GB。
                # msvcrt.locking(f.fileno(), msvcrt.LK_UNLCK, os.path.getsize(file_path))
                
                msvcrt.locking(f.fileno(), msvcrt.LK_NBLCK, 1)  # 只锁 1 个字节即可
                msvcrt.locking(f.fileno(), msvcrt.LK_UNLCK, 1)
                return False  # 能加锁又解锁, 说明没被占用
        except IOError:
            return True  # 加锁失败, 说明文件被占用
        # """检查文件是否正在被使用"""
        # try:
        #     with open(file_path, "rb") as f:
        #         pass  # 能正常打开文件, 说明没有被占用
        #     return False
        # except IOError:
        #     return True  # 文件正在被占用
    
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
    def download_video(self, url, folder_name) -> bool:
        target_dir = os.path.join(self.config.data["download_path"], folder_name)
        os.makedirs(target_dir, exist_ok=True)
        archive_path = os.path.join(target_dir, "000downloaded.txt")
        out_template = os.path.join(target_dir, "%(upload_date)s - %(title)s.%(ext)s")
        # final_path = resolve_filename_conflict(out_template)
        final_path = out_template
        cmd = ["yt-dlp", url, "--output", final_path, "--cache-dir", self.config.data["cache_dir"]]
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

        cmd2 = cmd.copy()
        cmd += ["-f", "bestvideo+bestaudio", "--merge-output-format", "mp4"]
        
        # 获取文件名
        cmd_check_filename = ["yt-dlp", url, "--output", final_path,] + ["--cookies", self.config.data["cookies_file"]] + ["--print", "filename"] + ["--merge-output-format", "mp4"]
        result = subprocess.run(cmd_check_filename, capture_output=True, 
                                # encoding='utf-8',
                                text=True)
                                # text=False) # 以字节流返回输出，避免直接按 utf-8 解码 # 字节流返回后用对应的编码方式解码, 还是不对
                                # encoding='utf-8', 
                                # errors='replace'  # 无法解码的字符会被替代
                                # ) # 依旧乱码
        
        # # 使用 chardet 检测编码
        # detected_encoding = chardet.detect(result.stdout)['encoding']
        # self.logger.info(f"检测到的编码: {detected_encoding}")

        # # 根据检测到的编码解码输出
        # stdout_raw = result.stdout.decode(detected_encoding)
        # self.logger.info(f"原始输出：{repr(stdout_raw)}")  # 打印原始输出，确认是否包含韩文字符

        # filename = os.path.basename(stdout_raw)
        # self.logger.info(f"文件名：{repr(filename)}")
        
        filename = os.path.basename(result.stdout.strip())
        self.logger.info(f"准备下载filename: {filename}")
        # filename = re.search(r'[^\\]+\.webm', stdout.strip()).group(0)
        try:
            subprocess.run(cmd, check=True, capture_output=True)
            self.logger.info(f"下载成功: {url}")
            self.stats.success += 1

            needBeMovedFilepath = os.path.join(target_dir, filename)

            if not os.path.exists(needBeMovedFilepath):
                self.logger.info(f"移动文件跳过（文件没找到）{needBeMovedFilepath}")
                return True  # 文件不存在, 视为文件下载过了 # 或者文件路径不存在, 文件名编码有问题
            
            # 检查文件是否正在被使用
            if Downloader.is_file_in_use(needBeMovedFilepath):
                self.logger.info(f"移动文件跳过（文件正在使用）{filename} \n {needBeMovedFilepath} \n {result}")
                return True

            # 获取视频分辨率, 限制 5 秒超时
            cmd_resolution = [
                "ffprobe", "-v", "error", "-select_streams", "v:0",
                "-show_entries", "stream=width,height", "-of", "csv=p=0", needBeMovedFilepath
            ]
            try:
                result = subprocess.run(cmd_resolution, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=5)
                width, height = map(int, result.stdout.strip().split(","))

                # 在当前子文件夹中创建ABC文件夹
                abc_folder = os.path.join(target_dir, 'ABC')
                os.makedirs(abc_folder, exist_ok=True)

                # 创建分辨率文件夹并放入ABC子文件夹中
                resolution_folder = os.path.join(abc_folder, f"{width}x{height}")
                os.makedirs(resolution_folder, exist_ok=True)

                # 移动文件到目标文件夹
                shutil.move(needBeMovedFilepath, os.path.join(resolution_folder, filename))
                self.logger.info(f"移动文件成功{filename}")
                # print(f"已移动: {filename} → {resolution_folder}/")

            except subprocess.TimeoutExpired:
                print(f"跳过（FFprobe 超时）: {filename}")

            except ValueError:
                print(f"跳过（无法获取分辨率）: {filename}")

            return True
        except subprocess.CalledProcessError as e:
            stderr = e.stderr or b""
            if b"DRM" in e.stderr or b"Protected" in stderr:
                self.logger.warning(f"检测到DRM保护, 使用安全格式重试...: {url}, and reget_cookies")
                cmd2 += ["-f", "bestvideo+bestaudio", "--merge-output-format", "mp4"]
                # cmd2 += ["--extractor-args", "youtube:player-client=web;web_ver=3"] // Your account may have the SSAP (server-side ads) experiment which interferes with yt-dlp.
                # Some tv client https formats have been skipped as they are DRM protected. Your account may have an experiment that applies DRM to all videos on the tv client.
                # 开个会员就好了
                self.use_safty_cmd_DRM += 1
                # reget_cookies(self.config, self.logger)
                try:
                    subprocess.run(cmd2, check=True)
                    self.logger.info(f"使用安全格式下载成功: {url}, 遇到DRM保护, 但是下载成功的次数：{self.use_safty_cmd_DRM}")
                    self.stats.success += 1
                    
                    
                            
                    # # 获取文件名
                    # cmd_check_filename = ["yt-dlp", url, "--output", final_path,] + ["--cookies", self.config.data["cookies_file"]] + ["--print", "filename"] + ["--merge-output-format", "mp4"]
                    # result = subprocess.run(cmd_check_filename, capture_output=True, 
                    #                         # encoding='utf-8')
                    #                         text=True)
                    # filename = os.path.basename(result.stdout.strip())
                    
                    needBeMovedFilepath = os.path.join(target_dir, filename)

                    if not os.path.exists(needBeMovedFilepath):
                        self.logger.info(f"移动文件跳过（文件下载过了）{needBeMovedFilepath}")
                        return True  # 文件不存在, 视为文件下载过了
                    
                    # 检查文件是否正在被使用
                    if Downloader.is_file_in_use(needBeMovedFilepath):
                        self.logger.info(f"移动文件跳过（文件正在使用）{filename} \n {needBeMovedFilepath} \n {result}")
                        return True

                    # 获取视频分辨率, 限制 5 秒超时
                    cmd_resolution = [
                        "ffprobe", "-v", "error", "-select_streams", "v:0",
                        "-show_entries", "stream=width,height", "-of", "csv=p=0", needBeMovedFilepath
                    ]
                    try:
                        result = subprocess.run(cmd_resolution, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=5)
                        width, height = map(int, result.stdout.strip().split(","))

                        # 在当前子文件夹中创建ABC文件夹
                        abc_folder = os.path.join(target_dir, 'ABC')
                        os.makedirs(abc_folder, exist_ok=True)

                        # 创建分辨率文件夹并放入ABC子文件夹中
                        resolution_folder = os.path.join(abc_folder, f"{width}x{height}")
                        os.makedirs(resolution_folder, exist_ok=True)

                        # 移动文件到目标文件夹
                        shutil.move(needBeMovedFilepath, os.path.join(resolution_folder, filename))
                        self.logger.info(f"移动文件成功{filename}")
                        # print(f"已移动: {filename} → {resolution_folder}/")

                    except subprocess.TimeoutExpired:
                        print(f"跳过（FFprobe 超时）: {filename}")

                    except ValueError:
                        print(f"跳过（无法获取分辨率）: {filename}")
                    
                    
                    
                    
                    
                    
                    
                    return True
                except subprocess.CalledProcessError:
                    self.logger.warning(f"使用安全格式重试仍然失败: {url}")
                    self.stats.fail += 1
                    self.cant_deal_DRM = True
                    return False

            self.logger.warning(f"下载失败: {url}")
            self.stats.fail += 1
        # wait_for_user_action():
            # while True:
            #     choice = input("处理失败, 请修复 cookies 后输入是否继续？(y/n): ").strip().lower()
            #     if choice == 'y':
            #         print("继续执行程序...")
            #         break
            #     elif choice == 'n':
            #         print("你自己按ctrl+c退出程序。")
            #     else:
            #         print("无效输入, 请输入 y 或 n。")
            # # （继续循环或重试）
            # return False
        # get cookies from browser
            # 从 Firefox 获取 cookies
            # cookies = browser_cookie3.firefox(domain_name='youtube.com')

            # # 格式化为 Netscape 格式（yt-dlp 支持的格式）
            # cookies_txt_path = self.config.data["cookies_file"]

            # with open(cookies_txt_path, 'w', encoding='utf-8') as f:
            #     f.write("# Netscape HTTP Cookie File\n")
            #     for cookie in cookies:
            #         domain = cookie.domain if cookie.domain.startswith('.') else '.' + cookie.domain
            #         path = cookie.path or '/'
            #         secure = "TRUE" if cookie.secure else "FALSE"
            #         expires = int(time.time()) + 3600 * 24 * 30  # 设置过期时间为 30 天
            #         f.write(f"{domain}\tTRUE\t{path}\t{secure}\t{expires}\t{cookie.name}\t{cookie.value}\n")

            # self.logger.warning(f"✅ Cookies 已提取并保存到 {cookies_txt_path}")
            return False
