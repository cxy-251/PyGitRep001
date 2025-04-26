
import os
import time

def resolve_filename_conflict(template):
    base = template.replace("%(title).200s.%(ext)s", "{}.%(ext)s")
    i = 0
    while True:
        suffix = time.strftime("%Y%m%d_%H%M%S") if i == 0 else f"{time.strftime('%Y%m%d_%H%M%S')}_{i}"
        path = base.format(suffix)
        if not os.path.exists(path):
            return base.format(suffix)
        i += 1
