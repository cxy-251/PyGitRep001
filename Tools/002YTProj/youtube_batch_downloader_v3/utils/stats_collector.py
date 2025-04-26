
class StatsCollector:
    def __init__(self):
        self.success = 0
        self.fail = 0
    def summary(self):
        return f"下载成功：{self.success} | 失败：{self.fail}"
