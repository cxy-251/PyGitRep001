# PyGitRep001

个人历史工具与代码归档仓库。

旧仓库中的大型 Notebook、重复项目和一次性实验已经拆成独立小工具。当前主要入口：

- [`ARCHIVE_INDEX.md`](./ARCHIVE_INDEX.md)：按“功能 → 代码”查找；
- [`archive_tools/`](./archive_tools/)：提取后的独立脚本；
- [`Tools/003YouTubeViewportFullscreen/`](./Tools/003YouTubeViewportFullscreen/)：已完成的 Chrome 扩展。

## 当前定位

- 仓库不再持续开发旧项目版本；
- yt-dlp 正式能力已经迁移到 `lyricMVPlayer`；
- 一个脚本只保留一个明确功能；
- 破坏性操作默认预览，必须显式启用；
- 被替代的混合 Notebook 和重复下载器已从当前分支删除，但可从 Git 历史恢复；
- `Tools/005YTProjMerge/music2/` 继续作为历史生成数据保留。

## 使用

```powershell
python .\archive_tools\<分类>\<工具>.py --help
```

完整功能列表见 [`ARCHIVE_INDEX.md`](./ARCHIVE_INDEX.md)。
