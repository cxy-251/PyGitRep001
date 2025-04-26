
import os

def clean_cache(cache_dir, logger):
    count = 0
    for f in os.listdir(cache_dir):
        if f.endswith(".part") or f.endswith(".tmp"):
            try:
                os.remove(os.path.join(cache_dir, f))
                count += 1
            except:
                continue
    logger.info(f"清理缓存文件：{count} 个")
