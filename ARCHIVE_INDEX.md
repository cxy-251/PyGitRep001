# PyGitRep001 功能—代码归档索引

> 归档日期：2026-07-27  
> 本仓库保存过去的工具、一次性脚本、实验和学习记录。项目不再持续开发，代码按原路径保留，主要用于查找旧实现和复用思路。

## 状态说明

| 状态 | 含义 |
|---|---|
| 归档成品 | 当时已完成并实际使用过，现不再主动开发 |
| 一次性脚本 | 为某个具体任务编写，路径和参数通常写死 |
| 实验 | 原型或未完成实现 |
| 失败实验 | 已确认方案不可行或代码无法完成原目标 |
| 学习记录 | 算法、设计模式或工具学习笔记 |
| 生成数据 | 程序输出、下载结果或保存的网页资源，不是源代码 |
| 高风险 | 会覆盖、移动或删除真实文件，运行前必须备份 |

## 浏览器工具

| 功能 | 代码入口 | 状态 | 说明 |
|---|---|---|---|
| YouTube 网页全屏，保留浏览器标签栏和地址栏 | [`Tools/003YouTubeViewportFullscreen/`](Tools/003YouTubeViewportFullscreen/) · [`content.js`](Tools/003YouTubeViewportFullscreen/content.js) | 归档成品 | 仓库中完成度最高的独立工具；Manifest V3 Chrome 扩展 |

## YouTube / yt-dlp

> 这些实现已停止维护。后续实际使用的 yt-dlp 能力已经进入 `lyricMVPlayer`，本仓库仅保存历史实现。

| 功能 | 代码入口 | 状态 | 说明 |
|---|---|---|---|
| 单文件频道批量下载器 | [`youtube_batch_downloader_v2/youtube_batch_downloader.py`](Tools/002YTProj/youtube_batch_downloader_v2/youtube_batch_downloader.py) | 一次性脚本 | v2 单文件版本，包含配置监听、磁盘检查和批量下载 |
| 模块化频道批量下载器入口 | [`youtube_batch_downloader_v3/main.py`](Tools/002YTProj/youtube_batch_downloader_v3/main.py) | 实验 | v3 拆分后的入口 |
| v3 下载核心 | [`core/downloader.py`](Tools/002YTProj/youtube_batch_downloader_v3/core/downloader.py) | 实验 | 下载、重试、Cookie 刷新、分辨率分类；存在重复逻辑和硬编码 |
| v3 配置热更新 | [`utils/config_loader.py`](Tools/002YTProj/youtube_batch_downloader_v3/utils/config_loader.py) | 实验 | 使用 watchdog 监听配置文件 |
| v3 日志 | [`utils/logger.py`](Tools/002YTProj/youtube_batch_downloader_v3/utils/logger.py) | 一次性脚本 | 按日期写日志 |
| v3 磁盘检查 | [`utils/disk_checker.py`](Tools/002YTProj/youtube_batch_downloader_v3/utils/disk_checker.py) | 一次性脚本 | 检查盘符写死为 `G:/` |
| v3 Cookie 导出 | [`utils/reget_cookies.py`](Tools/002YTProj/youtube_batch_downloader_v3/utils/reget_cookies.py) | 一次性脚本 | 从 Firefox 导出 YouTube Cookie |
| v3 缓存清理 | [`utils/cache_cleaner.py`](Tools/002YTProj/youtube_batch_downloader_v3/utils/cache_cleaner.py) | 一次性脚本 | 删除 `.part` 和 `.tmp` |
| v3 下载统计 | [`utils/stats_collector.py`](Tools/002YTProj/youtube_batch_downloader_v3/utils/stats_collector.py) | 实验 | 只统计成功和失败数量 |
| v3 文件名冲突处理 | [`utils/file_namer.py`](Tools/002YTProj/youtube_batch_downloader_v3/utils/file_namer.py) | 实验 | 当前下载器未实际使用 |
| v4 入口占位 | [`youtube_batch_downloader_v4/main.py`](Tools/002YTProj/youtube_batch_downloader_v4/main.py) | 实验 | 与 v3 入口重复，并非完整 v4 |
| 从 Edge 导出 YouTube Cookie | [`LoginYT.py`](Tools/002YTProj/LoginYT.py) | 一次性脚本 | 会请求管理员权限；Cookie 属于敏感数据 |
| 从“稍后观看”提取发布者列表 | [`GetYTList.py`](Tools/002YTProj/GetYTList.py) · [`GetYTList.ipynb`](Tools/002YTProj/GetYTList.ipynb) | 失败实验 | Playwright 登录方案曾被 YouTube 判定不安全 |
| 按频道视频数排序频道列表 | [`sortChannels.py`](Tools/002YTProj/sortChannels.py) | 一次性脚本 | 输入、输出路径写死为本机路径 |
| 单 URL Streamlit 下载界面 | [`StreamlitTestGui.py`](Tools/002YTProj/StreamlitTestGui.py) | 实验 | 简单 GUI 原型 |
| Kivy GUI 测试 | [`KivyTestGuiDontWorkInNotebook.py`](Tools/002YTProj/KivyTestGuiDontWorkInNotebook.py) | 失败实验 | 实际内容是 Todo 示例，与下载器无直接关系 |
| YouTube 视频处理 Notebook | [`dealYTMovie.ipynb`](Tools/002YTProj/dealYTMovie.ipynb) | 一次性脚本 | 历史视频处理流程 |
| YouTube 音乐处理 Notebook | [`dealYTMusic.ipynb`](Tools/002YTProj/dealYTMusic.ipynb) | 一次性脚本 | 历史音乐处理流程 |
| 按频道行号提取并下载音乐 | [`YTExtractor.ipynb`](Tools/005YTProjMerge/YTExtractor.ipynb) | 一次性脚本 | 路径、Cookie、起始行号写死；生成大量音频和元数据 |
| YouTube 媒体提取实验 | [`YTExtractorMV.ipynb`](Tools/005YTProjMerge/YTExtractorMV.ipynb) | 一次性脚本 | 体积较大的 Notebook，保存了运行输出 |

## 音视频处理

| 功能 | 代码入口 | 状态 | 说明 |
|---|---|---|---|
| 长视频处理 | [`DealLongVideo.ipynb`](Tools/000DealMV/DealLongVideo.ipynb) | 一次性脚本 | 为特定媒体任务编写 |
| 批量视频处理 | [`DealManyVideos.ipynb`](Tools/000DealMV/DealManyVideos.ipynb) | 一次性脚本 | 批处理实验 |
| 视频与音频杂项处理 | [`dealMV.ipynb`](Tools/000DealMV/dealMV.ipynb) | 高风险 | 包含随机重命名、旋转、合并、目录打散和移动文件等互不相关操作 |
| 批量转换音频为 MP3 | [`covertToMp3.ipynb`](Tools/003DealMP3/covertToMp3.ipynb) | 高风险 | FFmpeg 失败时仍可能删除源文件；不要直接对唯一原件运行 |
| FFmpeg 截取 MP4 命令备忘 | [`ffmpegCutMP4.txt`](Tools/ffmpegCutMP4.txt) | 一次性脚本 | 命令片段 |
| Edge TTS 与 ChatTTS 实验 | [`GenRealVoice.ipynb`](Tools/001AIGenReanVoice/GenRealVoice.ipynb) | 实验 | Edge TTS 示例可用；ChatTTS 部分保存了失败堆栈和本机模型路径 |

## 网页抓取

| 功能 | 代码入口 | 状态 | 说明 |
|---|---|---|---|
| 快手主页链接和视频抓取 | [`KwaiExtractor.ipynb`](Tools/004Kwai/KwaiExtractor.ipynb) | 实验 | Playwright 原型，选择器、用户主页和 Edge 路径写死 |
| 保存的快手页面 | [`Tools/004Kwai/`](Tools/004Kwai/) | 生成数据 | HTML、CSS、JS、图片和 SVG 是浏览器保存的网页资源 |

## 文件与系统小工具

| 功能 | 代码入口 | 状态 | 说明 |
|---|---|---|---|
| 生成文件树 JSON | [`genFileTreeJson.ipynb`](Tools/genFileTreeJson.ipynb) | 一次性脚本 | 文件目录扫描工具 |
| Windows 资源管理器搜索备忘 | [`SearchInExpoler.md`](Tools/SearchInExpoler.md) | 学习记录 | 搜索语法和使用记录 |
| 零散 Python 技巧 | [`littleTrick.ipynb`](Tools/littleTrick.ipynb) | 学习记录 | 多个无关代码片段的集合 |
| TDD 假代码生成实验 | [`GenTDDFakeCode.ipynb`](Tools/GenTDDFakeCode.ipynb) | 实验 | 代码生成尝试 |

## 学习记录

| 内容 | 代码入口 | 状态 | 说明 |
|---|---|---|---|
| 设计模式示例 | [`CodeDesigner/designerModel.ipynb`](CodeDesigner/designerModel.ipynb) | 学习记录 | 抽象工厂、生成器等模式示例；不是完整应用 |
| 递归与数据结构 | [`DataStructuresAndAlgorithms/Recursion.ipynb`](DataStructuresAndAlgorithms/Recursion.ipynb) | 学习记录 | 表达式求值、进制转换、分形树等 |
| LeetCode Notebook | [`DataStructuresAndAlgorithms/LeetCode.ipynb`](DataStructuresAndAlgorithms/LeetCode.ipynb) | 学习记录 | 集中式刷题笔记 |
| LeetCode 单题代码 | [`LeetCode/`](LeetCode/) | 学习记录 | 多数文件由 LeetCode 插件生成，部分依赖 LeetCode 运行环境提供类型 |
| Git 命令和仓库内部结构 | [`git_Log.ipynb`](git_Log.ipynb) | 学习记录 | 保存了早期终端输出、本机路径、邮箱和 SSH 指纹等历史信息 |

## 根目录临时文件

| 内容 | 路径 | 状态 | 说明 |
|---|---|---|---|
| C++ 测试程序 | [`test.cpp`](test.cpp) | 实验 | 最小控制台测试 |
| 编译产物 | [`test.exe`](test.exe) | 生成数据 | Windows 可执行文件，不是源代码 |
| Notebook 测试 | [`test.ipynb`](test.ipynb) · [`test2.ipynb`](test2.ipynb) | 实验 | 根目录草稿 |
| 下载目录 | [`downloads/`](downloads/) | 生成数据 | 运行脚本产生的本地文件 |

## 大型生成数据

| 数据 | 路径 | 说明 |
|---|---|---|
| YouTube 音乐、封面和元数据 | [`Tools/005YTProjMerge/music2/`](Tools/005YTProjMerge/music2/) | 主要由 MP3、WebP 和 `info.json` 组成；约 420 MB，不是可维护源代码 |
| 快手保存网页资源 | [`Tools/004Kwai/`](Tools/004Kwai/) | 保存的网页及依赖资源 |
| TTS 示例音频 | [`Tools/001AIGenReanVoice/`](Tools/001AIGenReanVoice/) | Notebook 输出示例 |

## 使用归档代码前

1. 先阅读对应文件，不要直接“全部运行” Notebook。
2. 修改写死的 Windows 路径、Cookie 路径、盘符和输出目录。
3. 对重命名、移动、覆盖、删除和转码脚本先复制测试数据。
4. Cookie、账号数据和私人配置不要提交到仓库。
5. yt-dlp、Playwright、ChatTTS 和网页 DOM 均可能已经变化，归档代码不保证在当前版本直接运行。
