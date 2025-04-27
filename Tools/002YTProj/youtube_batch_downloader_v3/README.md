
---

# 📄 `README.md` （for youtube_batch_downloader_v3）

<!-- ```markdown -->
# YouTube Batch Downloader v3

一个高效、稳定、可扩展的 YouTube 批量下载器。  
使用 yt-dlp + Python 多模块架构，支持自动更新 cookies、断点续传、磁盘空间保护、错误处理与日志记录。

---

## ✨ 核心特性

- 批量下载频道下所有视频（支持频道主页链接）
- 支持自定义下载路径、限速、缓存目录、日志目录
- 实时读取配置文件（无需重启）
- 支持 cookies 文件，绕过登录验证、反机器人检查
- 遇到 DRM 保护视频时，自动切换兼容下载策略
- 下载出错自动重试（重新提取 cookies）
- 下载失败、异常退出时详细日志记录
- 定时清理无用缓存
- 睡眠间隔控制（防止频繁请求被封IP）
- 磁盘空间监控（低于指定容量时自动停止）
- 数据统计（记录成功、失败、总时长、总大小等）

---

## 📂 项目结构

```plaintext
youtube_batch_downloader_v3/
├── main.py               # 主程序入口
├── config/
│   ├── config.json       # 配置文件，存放隐私文件路径
├── core/
│   ├── downloader.py     # 文件名的功能
├── utils/
│   ├── logger.py         # 日志管理模块
│   ├── config_loader.py  # 配置文件动态加载模块
│   ├── disk_checker.py   # 磁盘空间监测模块
│   ├── reget_cookies.py  # 自动提取 cookies 模块
│   └── ...               # 其他工具模块
└── README.md             # 使用说明（本文件）

ConfigPrivate/Rep001Tools002YTProjV3/隐私文件
├── channels.txt          # 频道列表（每行一个频道链接）
├── cookies.txt           # 存放cookies，注意格式Netscape
├── cache/                # yt-dlp 缓存目录
└── logs/                 # 日志输出目录
```

---

## ⚙️ 配置文件说明（`config.json`）

```json
{
  "channel_list_file": "路径/to/channels.txt",
  "max_disk_space_gb": 20,
  "download_path": "G:/YTDowmLoad/YT-dlp",
  "limit_rate": "unlimited",
  "cookies_file": "路径/to/cookies.txt",
  "sleep_interval": [5, 10],
  "cache_dir": "路径/to/cache",
  "log_dir": "路径/to/logs"
}
```

| 字段 | 说明 |
|:---|:---|
| `channel_list_file` | 频道列表文件路径（每行一个频道主页链接） |
| `max_disk_space_gb` | 剩余磁盘空间小于此值（单位GB）时停止下载 |
| `download_path` | 视频保存目录 |
| `limit_rate` | 下载限速（如 `"500K"`、`"2M"`，或 `"unlimited"`不限速） |
| `cookies_file` | cookies 文件路径（Netscape格式） |
| `sleep_interval` | 每个视频之间随机睡眠秒数区间 `[最小值, 最大值]` |
| `cache_dir` | yt-dlp 缓存目录 |
| `log_dir` | 日志输出目录 |

---

## 🛠 使用方法

### 1. 安装依赖

确保本地安装了：
- Python 3.8+
- [yt-dlp](https://github.com/yt-dlp/yt-dlp)
- `browser-cookie3` （用于自动提取cookies）

安装 Python 第三方依赖：

```bash
pip install yt-dlp browser-cookie3
```

### 2. 准备频道列表

在 `channels.txt` 中列出你要批量下载的 YouTube 频道主页链接，每行一个。

示例：
```
https://www.youtube.com/@examplechannel
https://www.youtube.com/@anotherchannel
```

### 3. 准备 cookies

- 推荐使用浏览器（如 Firefox）自动提取。
- 可使用自带功能：`reget_cookies.py`，运行即可生成 cookies 文件。

或者手动导出，格式为 **Netscape HTTP Cookie File 格式**。

### 4. 配置文件调整

根据你的实际路径，修改 `config.json`。  
注意路径必须用 `/` 或 `\\`（Windows）。

### 5. 运行主程序

```bash
python main.py
```

程序会自动执行：
- 读取频道
- 获取视频列表
- 检查磁盘空间
- 下载视频
- 重新获取cookies
- 日志记录
- 错误重试

运行期间，日志会记录到 `logs/` 目录。

---

## 📈 数据统计

程序运行完后，会在日志中总结：
- 总视频数
- 成功下载数
- 失败下载数
- 下载总大小
- 总耗时

方便后续统计和排查问题。

---

## 🔥 注意事项

- **Cookies定期刷新：** 如果下载失败率高，请重新提取浏览器 cookies。
- **磁盘保护机制：** 剩余空间小于配置值时，会自动停止下载，防止磁盘爆满。
- **异常退出保护：** 出错时程序不会崩溃，会自动跳过并记录日志。
- **睡眠间隔建议：** 防止 IP 被封禁，务必启用下载间隔（建议5-10秒）。

---

## 🛡️ 隐私声明

- 本项目不存储、不上传你的 cookies 或个人数据。
- 所有敏感信息（如 cookies、邮箱信息）均需手动维护在本地。
- 配置文件路径仅引用，不读取隐私内容本身。

---

## 📜 License

MIT License.

---
<!-- ``` -->

---

# ✅ 这个 README.md 具备：
- 专业正式感
- 详细但不啰嗦
- 不涉及敏感信息
- 结构清晰，适合维护
- 适合直接放到 github、或本地文档管理

---

# 📌 下一步提醒：
- 如果你满意这份 Readme，我们就直接开启 **V4版本开发**。
- V4会做：**完整UX界面 + 详细版V4 README + 更加模块化+守护进程**。

---
