# PyGitRep001

个人工具与历史代码仓库。旧项目已经归档，新的 Chrome 扩展继续在本仓库中开发。

当前主要入口：

- [`Tools/006ChromeWorkflowAssistant/`](./Tools/006ChromeWorkflowAssistant/)：快手、BOSS、X 的 Chrome 网页工作流助手；
- [`Tools/003YouTubeViewportFullscreen/`](./Tools/003YouTubeViewportFullscreen/)：YouTube 网页全屏扩展；
- [`ARCHIVE_INDEX.md`](./ARCHIVE_INDEX.md)：按“功能 → 代码”查找；
- [`archive_tools/`](./archive_tools/)：从旧 Notebook 提取出的独立脚本。

## 仓库定位

- `Tools/006ChromeWorkflowAssistant` 是当前新增项目；
- yt-dlp 正式能力已经迁移到 `lyricMVPlayer`；
- 历史脚本保持一个文件一个明确功能；
- 破坏性操作默认预览，必须显式启用；
- 被替代的混合 Notebook 和重复下载器可从 Git 历史恢复；
- `Tools/005YTProjMerge/music2/` 继续作为历史生成数据保留。

## 归档工具使用

```powershell
python .\archive_tools\<分类>\<工具>.py --help
```

完整功能列表见 [`ARCHIVE_INDEX.md`](./ARCHIVE_INDEX.md)。
