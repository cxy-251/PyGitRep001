# PyGitRep001 功能—代码总表

> 整理日期：2026-07-27  
> 主入口均指向从旧 Notebook 和项目中提取后的独立小工具。旧的大型混合 Notebook 已在提取后删除，可通过 Git 历史恢复。

## 状态

| 状态 | 含义 |
|---|---|
| 可复用归档工具 | 已拆成独立脚本，参数化并增加基本安全检查 |
| 归档成品 | 当时完成并实际使用过，现不再主动开发 |
| 历史快照 | 保留原项目结构，仅供查看，不再作为推荐入口 |
| 生成数据 | 程序输出或下载结果，不是源代码 |
| 学习记录 | 算法、设计模式或工具学习内容 |

## 媒体工具

| 功能 | 代码 | 状态 | 说明 |
|---|---|---|---|
| 统计视频时长、分辨率和文件大小 | [`video_inventory.py`](archive_tools/media/video_inventory.py) | 可复用归档工具 | 只读；可输出 JSON |
| 提取音频并增加音量 | [`extract_audio_gain.py`](archive_tools/media/extract_audio_gain.py) | 可复用归档工具 | 保留原媒体文件 |
| 安全批量转换 MP3 | [`convert_audio_to_mp3.py`](archive_tools/media/convert_audio_to_mp3.py) | 可复用归档工具 | 转换和验证成功后才允许 `--replace-source` |
| 按分辨率归档视频 | [`organize_videos_by_resolution.py`](archive_tools/media/organize_videos_by_resolution.py) | 可复用归档工具 | 默认只预览；`--apply` 后移动 |
| 提取竖屏视频并保留相对目录 | [`copy_portrait_videos.py`](archive_tools/media/copy_portrait_videos.py) | 可复用归档工具 | 默认只预览；默认复制 |
| 旋转视频 | [`rotate_videos.py`](archive_tools/media/rotate_videos.py) | 可复用归档工具 | 生成新文件，不删除原件 |
| 分割大型视频 | [`split_large_videos.py`](archive_tools/media/split_large_videos.py) | 可复用归档工具 | 默认只预览；所有分段成功后才可归档原件 |
| 按目标大小分组合并音频 | [`merge_audio_groups.py`](archive_tools/media/merge_audio_groups.py) | 可复用归档工具 | 每个子目录独立分组，使用临时目录转码 |
| FFmpeg 截取 MP4 命令备忘 | [`ffmpegCutMP4.txt`](Tools/ffmpegCutMP4.txt) | 历史快照 | 原始命令片段 |

## 文件整理工具

| 功能 | 代码 | 状态 | 说明 |
|---|---|---|---|
| 随机编号重命名 | [`random_rename.py`](archive_tools/files/random_rename.py) | 可复用归档工具 | 默认只预览；两阶段改名避免重名冲突，并保存映射 JSON |
| 把多层目录文件汇总到根目录 | [`flatten_directory.py`](archive_tools/files/flatten_directory.py) | 可复用归档工具 | 默认只预览；自动解决同名冲突 |
| 按数量把文件或目录分组 | [`group_items.py`](archive_tools/files/group_items.py) | 可复用归档工具 | 支持文件和目录；默认只预览 |
| 汇总 APK 并把相对路径写入文件名 | [`collect_apks.py`](archive_tools/files/collect_apks.py) | 可复用归档工具 | 默认复制；移动需要 `--move --apply` |
| 导出文件树 JSON | [`export_file_tree_json.py`](archive_tools/files/export_file_tree_json.py) | 可复用归档工具 | 递归生成目录树、文件大小和错误信息 |

## YouTube / yt-dlp 归档工具

> 正式 yt-dlp 工作流已迁移到 `lyricMVPlayer`；这里仅保留独立、可查阅的历史工具。

| 功能 | 代码 | 状态 | 说明 |
|---|---|---|---|
| 从频道列表下载音频或视频 | [`download_channel_media.py`](archive_tools/youtube/download_channel_media.py) | 可复用归档工具 | 支持行号范围、音频/视频模式、Cookie 文件或浏览器 Cookie |
| 按频道视频数量排序 URL | [`sort_channels_by_video_count.py`](archive_tools/youtube/sort_channels_by_video_count.py) | 可复用归档工具 | 使用 yt-dlp 平铺播放列表统计 |
| 从本地浏览器导出 YouTube Cookie | [`export_browser_cookies.py`](archive_tools/youtube/export_browser_cookies.py) | 可复用归档工具 | 不申请管理员权限；输出文件按密码级敏感信息处理 |
| 旧批量下载器 v2/v3/v4 | [`Tools/002YTProj/`](Tools/002YTProj/) | 历史快照 | 原项目停止维护；存在硬编码、重复逻辑和失败实验 |

## TTS 与网页抓取

| 功能 | 代码 | 状态 | 说明 |
|---|---|---|---|
| 使用 Edge TTS 生成 MP3 | [`edge_tts_to_mp3.py`](archive_tools/tts/edge_tts_to_mp3.py) | 可复用归档工具 | 支持直接文本或文本文件 |
| 抓取快手主页可见短视频 URL | [`kuaishou_profile_urls.py`](archive_tools/web/kuaishou_profile_urls.py) | 可复用归档工具 | Playwright 历史实现；DOM 变化后可能需要修改选择器 |
| 保存的快手网页和静态资源 | [`Tools/004Kwai/`](Tools/004Kwai/) | 生成数据 | 旧网页快照，不是推荐代码入口 |

## 独立成品

| 功能 | 代码 | 状态 | 说明 |
|---|---|---|---|
| YouTube 网页全屏并保留浏览器界面 | [`Tools/003YouTubeViewportFullscreen/`](Tools/003YouTubeViewportFullscreen/) | 归档成品 | Manifest V3 Chrome 扩展 |

## 学习记录

| 内容 | 代码 | 状态 |
|---|---|---|
| 设计模式 | [`CodeDesigner/designerModel.ipynb`](CodeDesigner/designerModel.ipynb) | 学习记录 |
| 递归与数据结构 | [`DataStructuresAndAlgorithms/Recursion.ipynb`](DataStructuresAndAlgorithms/Recursion.ipynb) | 学习记录 |
| LeetCode Notebook | [`DataStructuresAndAlgorithms/LeetCode.ipynb`](DataStructuresAndAlgorithms/LeetCode.ipynb) | 学习记录 |
| LeetCode 单题 | [`LeetCode/`](LeetCode/) | 学习记录 |
| Git 操作历史 | [`git_Log.ipynb`](git_Log.ipynb) | 学习记录 |

## 大型生成数据

| 内容 | 路径 | 说明 |
|---|---|---|
| YouTube 音乐、封面和元数据 | [`Tools/005YTProjMerge/music2/`](Tools/005YTProjMerge/music2/) | 继续保留；不是源码入口 |
| 快手网页资源 | [`Tools/004Kwai/`](Tools/004Kwai/) | 保存页面产生的 HTML、CSS、JS 和图片 |
| 下载目录 | [`downloads/`](downloads/) | 本地运行生成内容 |

## 运行前规则

1. 先执行 `python <脚本> --help`。
2. 有预览模式的工具先不加 `--apply`。
3. 对重命名、移动、分组和替换源文件操作使用副本测试。
4. Cookie、账号和私人配置不要提交。
5. FFmpeg、yt-dlp、Playwright 和网站 DOM 均可能变化，本仓库不做持续兼容。
