
import logging
from datetime import datetime
import os

def setup_logger(log_dir):
    today = datetime.now().strftime("%Y-%m-%d")
    log_file = os.path.join(log_dir, f"{today}.log")
    logging.basicConfig(
        filename=log_file,
        level=logging.INFO,
        format="[%(asctime)s] %(message)s",
        encoding="utf-8"
    )
    return logging.getLogger()
