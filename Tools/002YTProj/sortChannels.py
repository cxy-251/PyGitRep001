import subprocess
import time

input_file = r"C:\02Programmer\02Proj\PyVSCode\ConfigPrivate\Rep001Tools002YTProjV3\channels.txt"
output_file = r"C:\02Programmer\02Proj\PyVSCode\ConfigPrivate\Rep001Tools002YTProjV3\channels_sorted.txt"

def get_video_count(channel_url):
    try:
        result = subprocess.run(
            ["yt-dlp", "--flat-playlist", "--print", "%(id)s", channel_url],
            capture_output=True, text=True, timeout=120
        )
        video_ids = result.stdout.strip().split("\n")
        if video_ids == ['']:
            return 0
        return len(video_ids)
    except Exception as e:
        print(f"出错了，无法处理: {channel_url}")
        print(e)
        return float('inf')  # 出错的频道放到最后

def main():
    with open(input_file, "r", encoding="utf-8") as f:
        channels = [line.strip() for line in f if line.strip()]

    channel_video_counts = []

    print("开始统计频道视频数...")

    for idx, channel in enumerate(channels, 1):
        print(f"[{idx}/{len(channels)}] 正在处理: {channel}")
        count = get_video_count(channel)
        print(f"频道视频数: {count}")
        channel_video_counts.append((count, channel))
        time.sleep(1)  # 适当休息，防止被限速

    # 排序
    channel_video_counts.sort()

    # 写入新文件
    with open(output_file, "w", encoding="utf-8") as f:
        for count, channel in channel_video_counts:
            f.write(f"{channel}\n")

    print(f"✅ 排序完成，已保存到: {output_file}")

if __name__ == "__main__":
    main()
