# Chrome Web Store 上架资料

本文档用于准备商店页面和审核字段。当前版本建议先免费发布，不包含支付、账号、广告或远程服务。

## 产品名称

中文：

`网页全屏助手 for YouTube™`

英文：

`Viewport Fullscreen for YouTube™`

短名称：

`网页全屏助手`

## 简短说明

中文：

`让 YouTube 播放器铺满当前网页视口，同时保留 Chrome 标签栏和地址栏。`

英文：

`Fill the current browser viewport with the YouTube player while keeping Chrome tabs and the address bar visible.`

## 详细说明（中文）

网页全屏助手在 YouTube 播放器控制栏中增加一个网页全屏按钮。点击后，播放器会铺满当前网页可视区域，同时保留 Chrome 标签栏、地址栏和窗口边框。

主要功能：

- 支持普通视频、播放列表、直播、广告阶段和 Shorts；
- 再次点击按钮或按 `Esc` 退出并恢复原滚动位置；
- 保留字幕、设置菜单、画质菜单和 YouTube 原生全屏；
- 窗口收窄、放宽、最大化或还原时自动重新适配；
- 不收集数据、不加载远程代码、不显示广告。

本扩展只完成一个明确功能：在保留浏览器界面的情况下，让当前 YouTube 播放器占满网页视口。

YouTube is a trademark of Google LLC. This extension is not affiliated with or endorsed by Google LLC.

## Detailed description (English)

Viewport Fullscreen adds a viewport-fullscreen button to the YouTube player controls. It expands the active player to fill the browser viewport while keeping Chrome tabs, the address bar, and the browser window visible.

Features:

- Supports regular videos, playlists, live streams, ad playback, and Shorts;
- Exit with the same button or `Esc`, restoring the previous scroll position;
- Keeps captions, player menus, quality settings, and native fullscreen available;
- Reflows when the browser window is resized, maximized, or restored;
- No data collection, remote code, analytics, or advertising.

The extension has one narrow purpose: fill the current page viewport with the active YouTube player without hiding the browser interface.

YouTube is a trademark of Google LLC. This extension is not affiliated with or endorsed by Google LLC.

## 隐私实践字段

### Single purpose

`在 YouTube 视频页面中增加网页全屏控制，使当前播放器铺满网页视口，同时保留浏览器标签栏和地址栏。`

### Host access justification

`扩展需要在 https://www.youtube.com/* 上运行本地 content script，以定位当前播放器、插入网页全屏按钮并调整播放器布局。页面结构和播放器状态仅在当前标签页本地处理，不会被保存或传输。`

### Remote code

选择：`No, I am not using remote code.`

说明：

`所有 JavaScript 和 CSS 均包含在扩展安装包中；扩展不下载、生成或执行远程代码。`

### 数据披露

按照当前代码行为填写：

- 不收集或传输个人身份信息；
- 不收集或传输健康、财务、认证或通信信息；
- 不收集或传输位置、浏览历史或网站内容；
- 不使用分析、广告、跟踪或遥测；
- 不出售或共享用户数据。

扩展只在本机运行时处理当前 YouTube 页面的 DOM 和播放器状态。详细说明见 `PRIVACY.md`。

## 图片准备

打包脚本会生成：

- `store-assets/store-icon-128.png`：商店图标；
- `store-assets/small-promo-440x280.png`：小型宣传图。

至少还需要人工截取一张真实运行截图。建议准备 3–5 张 `1280×800` PNG：

1. 普通视频页面，标出控制栏中的网页全屏按钮；
2. 网页全屏状态，清楚显示 Chrome 标签栏和地址栏仍然存在；
3. 窗口收窄后的网页全屏状态；
4. Shorts 网页全屏状态；
5. 网页全屏与 YouTube 原生全屏的区别。

截图应使用真实扩展界面，不使用 YouTube 官方 Logo 作为扩展品牌图标，不写“官方”“最佳”“第一”等无法证明的宣传语。

## 发布步骤

仅在需要发布到 Chrome Web Store 时，在扩展目录中执行：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\scripts\build-store-package.ps1
```

该命令使用 Windows 自带的 Windows PowerShell，不要求安装 PowerShell 7。脚本会：

1. 生成 16、32、48、128 像素 PNG 图标；
2. 生成 128×128 商店图标和 440×280 小型宣传图；
3. 创建仅包含发布文件的临时目录；
4. 在发布包的 `manifest.json` 中加入图标配置；
5. 输出可上传的 ZIP 文件。

生成位置：

- 发布 ZIP：`.store-build/viewport-fullscreen-for-youtube-v<版本号>.zip`
- 解压测试目录：`.store-build/package/`
- 商店图片：`.store-build/store-assets/`

提交前检查：

- 扩展版本号已递增；
- ZIP 在 `chrome://extensions` 中可正常加载；
- 至少完成普通视频、Shorts、窗口缩放、`Esc` 和原生全屏往返测试；
- 已准备至少一张真实运行截图；
- 商店标题、说明、截图和隐私字段与当前版本行为一致；
- 隐私政策 URL 使用公开可访问地址：
  `https://github.com/cxy-251/PyGitRep001/blob/main/Tools/003YouTubeViewportFullscreen/PRIVACY.md`；
- 支持 URL 使用：`https://github.com/cxy-251/PyGitRep001/issues`。
