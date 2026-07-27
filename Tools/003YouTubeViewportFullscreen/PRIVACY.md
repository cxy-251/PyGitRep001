# 隐私政策 / Privacy Policy

最后更新 / Last updated: 2026-07-27

## 中文

### 适用范围

本政策适用于 Chrome 扩展“网页全屏助手 for YouTube™”。

### 本地处理的数据

为了定位当前播放器并实现网页全屏，扩展会在用户打开 YouTube 页面时，仅在浏览器本地读取：

- 当前页面路径，用于判断普通视频页或 Shorts 页面；
- 页面中的播放器和控制栏 DOM 结构；
- 播放器是否可见、是否正在播放以及原生全屏状态。

这些信息只用于在当前标签页中实现扩展功能。

### 不收集、不传输

扩展不会：

- 收集、保存或上传浏览历史、账号信息、搜索内容、视频标题或观看记录；
- 使用分析、广告、跟踪或遥测服务；
- 向开发者或任何第三方发送页面内容或播放器状态；
- 出售或共享用户数据。

扩展没有后台服务器，也不发起第三方网络请求。

### 权限范围

扩展只在 `https://www.youtube.com/*` 页面注入本地脚本和样式，用于增加网页全屏按钮并调整当前播放器布局。扩展不申请额外的 Chrome API 权限。

### 数据保留

扩展不建立用户数据库，也不持久化上述页面或播放器信息。网页全屏状态仅存在于当前标签页运行期间，页面关闭或刷新后即被清除。

### 政策更新

功能或数据处理方式发生实质变化时，本政策会同步更新，并修改顶部日期。

### 联系与支持

请通过仓库 Issues 提交问题：

https://github.com/cxy-251/PyGitRep001/issues

### 商标声明

YouTube is a trademark of Google LLC. This extension is not affiliated with or endorsed by Google LLC.

---

## English

### Scope

This policy applies to the Chrome extension “Viewport Fullscreen for YouTube™”.

### Data processed locally

To locate the active player and provide viewport fullscreen, the extension reads the following information locally in the browser while the user is on YouTube:

- The current page path, to distinguish regular watch pages from Shorts pages;
- The player and control-bar DOM structure;
- Whether the player is visible or playing, and whether native fullscreen is active.

This information is used only to provide the extension’s functionality in the current tab.

### No collection or transmission

The extension does not:

- Collect, store, or upload browsing history, account information, searches, video titles, or watch history;
- Use analytics, advertising, tracking, or telemetry services;
- Send page content or player state to the developer or any third party;
- Sell or share user data.

The extension has no backend server and makes no third-party network requests.

### Permission scope

The extension injects local scripts and styles only on `https://www.youtube.com/*` to add the viewport-fullscreen control and adjust the current player layout. It requests no additional Chrome API permissions.

### Data retention

The extension does not maintain a user database or persist page or player information. Runtime state exists only in the current tab and is cleared when the page is refreshed or closed.

### Policy changes

If the extension’s functionality or data practices materially change, this policy and the date above will be updated.

### Contact and support

Please use the repository issue tracker:

https://github.com/cxy-251/PyGitRep001/issues

### Trademark notice

YouTube is a trademark of Google LLC. This extension is not affiliated with or endorsed by Google LLC.
