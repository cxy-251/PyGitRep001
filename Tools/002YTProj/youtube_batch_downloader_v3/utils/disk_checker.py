
import shutil

def check_disk_space(limit_gb, logger):
    total, used, free = shutil.disk_usage("G:/")
    free_gb = free // (2**30)
    if free_gb <= limit_gb:
        logger.warning(f"磁盘剩余空间不足：{free_gb} GB")
        return False
    return True
