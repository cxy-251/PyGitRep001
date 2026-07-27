# PyGitRep001 功能—代码总表

> 整理日期：2026-07-27  
> 混合 Notebook、重复下载器和零散实验已按“一个明确功能一个文件”提取；新的浏览器扩展继续独立开发。

## 使用规则

- 先执行 `python <脚本> --help`。
- 带 `--apply`、`--move`、`--overwrite` 或 `--replace-source` 的操作，先使用副本测试。
- FFmpeg、yt-dlp、Playwright、浏览器 Cookie 和网站 DOM 可能随版本变化。
- Cookie、OAuth token 和客户端密钥属于敏感信息，不要提交。

## 浏览器扩展

| 功能 | 代码 | 状态 |
|---|---|---|
| 快手发布辅助、BOSS 职位整理、X 时间线提示词提取 | [`Tools/006ChromeWorkflowAssistant/`](Tools/006ChromeWorkflowAssistant/) | 当前开发，Manifest V3 原型 |
| YouTube 网页全屏并保留浏览器界面 | [`Tools/003YouTubeViewportFullscreen/`](Tools/003YouTubeViewportFullscreen/) | 已完成归档成品 |

## 媒体：检查、转换与整理

| 功能 | 代码 | 说明 |
|---|---|---|
| 统计视频时长、分辨率和文件大小 | [`video_inventory.py`](archive_tools/media/video_inventory.py) | 只读，可输出 JSON |
| 输出逐视频详细 CSV 和错误 CSV | [`video_catalog_csv.py`](archive_tools/media/video_catalog_csv.py) | 包含 FPS、码率、时长、尺寸 |
| 按子目录汇总分辨率和总时长 | [`folder_video_summary.py`](archive_tools/media/folder_video_summary.py) | 输出 CSV |
| 提取音频并增加音量 | [`extract_audio_gain.py`](archive_tools/media/extract_audio_gain.py) | 保留源文件 |
| 安全批量转换 MP3 | [`convert_audio_to_mp3.py`](archive_tools/media/convert_audio_to_mp3.py) | 验证成功后才允许删除源文件 |
| 按分辨率整理视频 | [`organize_videos_by_resolution.py`](archive_tools/media/organize_videos_by_resolution.py) | 默认只预览 |
| 提取竖屏视频并保留相对路径 | [`copy_portrait_videos.py`](archive_tools/media/copy_portrait_videos.py) | 默认复制 |
| 旋转视频 | [`rotate_videos.py`](archive_tools/media/rotate_videos.py) | 生成新文件 |
| 截取视频片段 | [`cut_video.py`](archive_tools/media/cut_video.py) | 默认流复制，可重新编码 |
| 按文件大小分割大视频 | [`split_large_videos.py`](archive_tools/media/split_large_videos.py) | 所有片段成功后才可归档原件 |
| 按目标大小分组合并音频 | [`merge_audio_groups.py`](archive_tools/media/merge_audio_groups.py) | 每个子目录独立处理 |
| 三个视频横向拼接 | [`tile_three_videos.py`](archive_tools/media/tile_three_videos.py) | 默认每路 720×1280 |
| 按子目录顺序合并视频 | [`concat_videos_by_folder.py`](archive_tools/media/concat_videos_by_folder.py) | 统一尺寸和帧率后合并 |

## 文件与目录

| 功能 | 代码 | 说明 |
|---|---|---|
| 随机编号重命名 | [`random_rename.py`](archive_tools/files/random_rename.py) | 两阶段改名并保存映射 |
| 汇总多层目录中的文件 | [`flatten_directory.py`](archive_tools/files/flatten_directory.py) | 自动处理同名冲突 |
| 按数量给文件或目录分组 | [`group_items.py`](archive_tools/files/group_items.py) | 默认只预览 |
| 汇总 APK 并把路径写入文件名 | [`collect_apks.py`](archive_tools/files/collect_apks.py) | 默认复制 |
| 导出文件树 JSON | [`export_file_tree_json.py`](archive_tools/files/export_file_tree_json.py) | 包含文件大小和访问错误 |
| 统计直接子目录文件数 | [`folder_item_counts.py`](archive_tools/files/folder_item_counts.py) | 输出 CSV |
| 清理下载残片 | [`clean_partial_downloads.py`](archive_tools/files/clean_partial_downloads.py) | 清理 `.part`、`.tmp`、`.ytdl`，默认预览 |

## YouTube / yt-dlp

> 正式工作流已迁移到 `lyricMVPlayer`；这里保留可复用的历史工具。

| 功能 | 代码 | 说明 |
|---|---|---|
| 通用频道列表音频/视频下载 | [`download_channel_media.py`](archive_tools/youtube/download_channel_media.py) | 通用归档入口 |
| 专用频道视频下载 | [`download_video_channels.py`](archive_tools/youtube/download_video_channels.py) | 支持播放量、时长、直播和标题过滤 |
| YouTube Music 播放列表下载 MP3 | [`download_music_playlists.py`](archive_tools/youtube/download_music_playlists.py) | 元数据、封面、歌词和下载档案 |
| 列出频道或播放列表 URL | [`list_playlist_urls.py`](archive_tools/youtube/list_playlist_urls.py) | 支持普通 YouTube 与 Music URL |
| 按频道视频数排序 | [`sort_channels_by_video_count.py`](archive_tools/youtube/sort_channels_by_video_count.py) | 输出排序后的频道列表 |
| 查询频道订阅数 | [`channel_subscriber_count.py`](archive_tools/youtube/channel_subscriber_count.py) | 使用 yt-dlp 元数据 |
| 导出浏览器 YouTube Cookie | [`export_browser_cookies.py`](archive_tools/youtube/export_browser_cookies.py) | 不申请管理员权限 |
| 从播放列表页面提取发布者 | [`watch_later_creators.py`](archive_tools/youtube/watch_later_creators.py) | Playwright 登录配置，DOM 可能变化 |
| 用 YouTube Data API 导出播放列表频道 | [`export_playlist_creators_api.py`](archive_tools/youtube/export_playlist_creators_api.py) | 需要 OAuth 客户端密钥 |
| Tkinter 下载界面 | [`download_gui_tk.py`](archive_tools/youtube/download_gui_tk.py) | 标准库 GUI |
| PyQt5 下载界面 | [`download_gui_pyqt5.py`](archive_tools/youtube/download_gui_pyqt5.py) | 后台线程执行下载 |
| Streamlit 下载界面 | [`download_gui_streamlit.py`](archive_tools/youtube/download_gui_streamlit.py) | 使用 `streamlit run` 启动 |

## 文本、代码生成与系统检查

| 功能 | 代码 | 说明 |
|---|---|---|
| 电话号码格式化与粗略校验 | [`phone_number.py`](archive_tools/text/phone_number.py) | `format`、`validate` 子命令 |
| 生成 C++ enum / if-else / switch / mapping / dispatch | [`cpp_branch_codegen.py`](archive_tools/codegen/cpp_branch_codegen.py) | 整合旧 TDD 假代码 Notebook 的有效部分 |
| 生成 Python 表驱动处理函数和测试 | [`python_handler_codegen.py`](archive_tools/codegen/python_handler_codegen.py) | 可输出到文件 |
| 检查磁盘可用空间 | [`check_disk_space.py`](archive_tools/system/check_disk_space.py) | 低于阈值返回非零退出码 |
| 检测 HTTP 代理连通性 | [`check_http_proxy.py`](archive_tools/network/check_http_proxy.py) | 输出状态码和耗时 |

## GUI 与演示工具

| 功能 | 代码 | 说明 |
|---|---|---|
| UTF-8 文本编辑器 | [`text_editor.py`](archive_tools/gui/text_editor.py) | Tkinter |
| 摄像头查看器 | [`webcam_viewer.py`](archive_tools/gui/webcam_viewer.py) | OpenCV，按 `q` 或 `Esc` 退出 |
| 随机折线图演示 | [`random_plot_demo.py`](archive_tools/gui/random_plot_demo.py) | Matplotlib |
| 本地音乐播放器 | [`music_player_pyqt5.py`](archive_tools/gui/music_player_pyqt5.py) | PyQt5 Multimedia |
| 待办事项示例 | [`todo_kivy.py`](archive_tools/gui/todo_kivy.py) | Kivy 进程内示例 |

## 设置备忘与学习内容

| 内容 | 代码 | 说明 |
|---|---|---|
| Windows Files 应用安装备忘 | [`windows_setup.md`](archive_notes/windows_setup.md) | 从旧环境 Notebook 提取 |
| Windows 资源管理器搜索 | [`SearchInExpoler.md`](Tools/SearchInExpoler.md) | 历史备忘 |
| 设计模式 | [`CodeDesigner/designerModel.ipynb`](CodeDesigner/designerModel.ipynb) | 学习记录 |
| 数据结构与算法 | [`DataStructuresAndAlgorithms/`](DataStructuresAndAlgorithms/) | 学习记录 |
| LeetCode 单题 | [`LeetCode/`](LeetCode/) | 学习记录 |
| Git 操作历史 | [`git_Log.ipynb`](git_Log.ipynb) | 早期学习记录，含旧本机信息 |

## 生成数据

| 内容 | 路径 | 说明 |
|---|---|---|
| YouTube 音乐、封面和元数据 | [`Tools/005YTProjMerge/music2/`](Tools/005YTProjMerge/music2/) | 保留的历史生成数据 |
| 快手网页静态资源 | [`Tools/004Kwai/`](Tools/004Kwai/) | 保存页面产生的 HTML、JS、CSS 和图片 |
| 本地下载目录 | [`downloads/`](downloads/) | 新内容由 `.gitignore` 忽略 |

## 已删除的旧容器

下列内容的有效功能已提取，因此不再作为当前分支入口：多功能媒体 Notebook、YouTube 视频/音乐/Watch Later 实验 Notebook、v1–v4 重复批量下载器、电话号码与 TDD 代码生成混合 Notebook、GUI 杂项 Notebook，以及根目录临时测试和无关文章。这些文件仍可从 Git 历史恢复。
