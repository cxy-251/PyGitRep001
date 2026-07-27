
import json
from core.downloader import Downloader
from utils.logger import setup_logger
from utils.config_loader import ConfigLoader
from utils.cache_cleaner import clean_cache
from utils.reget_cookies import reget_cookies

if __name__ == "__main__":
    # 临时加载 config，获取 log_dir
    with open("config/config.json", "r", encoding="utf-8") as f:
        config_raw = json.load(f)
    log_dir = config_raw.get("log_dir", "log")  # 默认 log 目录
    cache_dir = config_raw.get("cache_dir", "cache")  # 默认 cache 目录
    print(f"日志目录: {log_dir}\n缓存目录: {cache_dir}")

    logger = setup_logger(log_dir)
    config = ConfigLoader("config/config.json", logger)
    clean_cache(cache_dir, logger)

    reget_cookies(config, logger)
    downloader = Downloader(config, logger)
    downloader.run()
