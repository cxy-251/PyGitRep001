# PyGitRep001

个人历史代码归档仓库。

仓库中原先混合了完整项目、一次性脚本、Notebook 实验、学习记录和程序生成数据。现在采用两层结构：

1. [`archive_tools/`](./archive_tools/)：从旧 Notebook 和旧项目中提取出的可复用小工具；
2. [`ARCHIVE_INDEX.md`](./ARCHIVE_INDEX.md)：按“功能 → 代码”查找工具的总表。

## 当前定位

- 不再继续维护旧项目版本。
- YouTube / yt-dlp 的正式能力已经迁移到 `lyricMVPlayer`。
- 旧代码只保留仍有历史价值、且尚未完成提取的部分。
- 已提取的大型混合 Notebook 会从当前分支删除；它们仍可从 Git 历史恢复。
- `Tools/005YTProjMerge/music2/` 继续作为历史生成数据保留。

## 使用方式

先打开 [`ARCHIVE_INDEX.md`](./ARCHIVE_INDEX.md)，按功能进入对应的小脚本。每个脚本都支持：

```powershell
python .\path\to\tool.py --help
```

涉及重命名、移动、分组或替换源文件的工具，默认只预览，必须显式传入 `--apply`、`--move` 或 `--replace-source` 才会修改文件。

## 归档说明

这些工具主要用于回顾旧实现和复用思路，不保证适配当前网站、依赖或操作系统环境。Cookie 文件属于敏感信息，不应提交到仓库。
