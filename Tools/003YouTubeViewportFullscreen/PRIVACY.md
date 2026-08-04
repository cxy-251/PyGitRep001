# 隐私政策 / Privacy Policy

最后更新 / Last updated: 2026-08-04

## 中文

本政策适用于 Chrome 扩展“网页视口全屏助手”。

扩展具有两种运行方式：

- YouTube 页面会加载本地专用脚本，用于提供播放器控制栏中的网页全屏按钮；
- 其他普通网页只有在用户点击扩展图标时，才通过 `activeTab` 临时注入本地元素选择脚本。

为了实现功能，扩展会在当前标签页本地读取目标元素及其祖先 DOM、尺寸、可见状态、滚动位置和焦点状态。这些信息仅用于选择目标、调整布局以及退出时恢复页面。

扩展不会：

- 收集、保存或上传浏览历史、账号信息、搜索内容或网页内容；
- 读取 Cookie、认证信息或网络请求；
- 使用分析、广告、跟踪或遥测服务；
- 向开发者或第三方发送数据；
- 加载或执行远程代码。

扩展申请 `activeTab` 和 `scripting` 权限，仅用于响应用户点击，在当前普通 HTTP/HTTPS 标签页中临时注入扩展自带的 JavaScript 和 CSS。扩展没有后台服务器，不持久化网页内容；运行状态在页面刷新或关闭后清除。

联系与支持：

https://github.com/cxy-251/PyGitRep001/issues

YouTube is a trademark of Google LLC. This extension is not affiliated with or endorsed by Google LLC.

---

## English

This policy applies to the Chrome extension “Viewport Fullscreen Assistant”.

The extension has two operating modes:

- A local YouTube-specific script provides the viewport-fullscreen button inside the player controls;
- On other regular webpages, the generic element picker is injected temporarily only after the user clicks the extension action, using the `activeTab` permission.

To provide the feature, the extension locally reads the selected element and ancestor DOM, element dimensions and visibility, scroll position, and focus state. This information is used only to select the target, adjust its layout, and restore the page on exit.

The extension does not:

- Collect, store, or upload browsing history, account information, searches, or webpage content;
- Read cookies, authentication data, or network requests;
- Use analytics, advertising, tracking, or telemetry services;
- Send data to the developer or third parties;
- Load or execute remote code.

The `activeTab` and `scripting` permissions are used only in response to an explicit user action to inject bundled JavaScript and CSS into the current regular HTTP/HTTPS tab. The extension has no backend server and does not persist webpage content. Runtime state is cleared when the page is refreshed or closed.

Contact and support:

https://github.com/cxy-251/PyGitRep001/issues

YouTube is a trademark of Google LLC. This extension is not affiliated with or endorsed by Google LLC.
