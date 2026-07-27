# Extracted Archive Tools

这里保存从旧 Notebook、重复项目和一次性脚本中提取出的独立工具。

## 目录

- `media/`：FFmpeg、FFprobe、视频清单、音频转换和拼接；
- `files/`：重命名、分组、目录整理、文件树和残片清理；
- `youtube/`：yt-dlp、YouTube Data API、Cookie 和下载 GUI；
- `text/`：字符串格式化与校验；
- `codegen/`：C++ 和 Python 代码生成；
- `system/`：磁盘等系统检查；
- `network/`：代理与网络检查；
- `gui/`：从旧 GUI 实验中提取出的可运行示例；
- `tts/`：文字转语音；
- `web/`：网页抓取历史实现。

## 设计原则

1. 不再写死本机盘符、Cookie 路径和输出目录；
2. 一个文件只完成一个明确功能；
3. 重命名、移动、删除和替换默认不执行；
4. 外部程序失败时检查退出码；
5. Cookie、OAuth token 和客户端密钥不进入 Git；
6. 每个命令行工具提供 `--help`。

## 依赖

- 媒体：`ffmpeg`、`ffprobe`；
- YouTube：`yt-dlp`；
- 浏览器 Cookie：`browser-cookie3`；
- Playwright 抓取：`playwright` 与 Chromium；
- YouTube Data API：`google-api-python-client`、`google-auth-oauthlib`；
- TTS：`edge-tts`；
- GUI 示例：按脚本分别使用 Tkinter、PyQt5、Kivy、OpenCV 或 Matplotlib。

完整索引见 [`../ARCHIVE_INDEX.md`](../ARCHIVE_INDEX.md)。

被替代的旧文件仍可从 Git 历史恢复。
