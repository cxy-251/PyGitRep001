# YouTube 网页全屏

这是一个可直接加载的 Chrome Manifest V3 扩展。它在 YouTube 原播放器控制栏中、系统全屏按钮左侧增加“网页全屏”按钮；点击后播放器铺满当前网页内容区，Chrome 标签栏、地址栏和窗口边框保持可见；按 `Esc` 或再次点击按钮退出并恢复原页面位置。

## 安装

打开 `chrome://extensions`，开启“开发者模式”，点击“加载已解压的扩展程序”，选择当前目录。打开 YouTube 视频后，播放器右下角会出现网页全屏按钮。

## 支持范围

支持普通视频、播放列表、直播、广告阶段、YouTube 站内换片和 Shorts；保持 YouTube 原播放器、字幕、控制栏、清晰度菜单和系统全屏功能。网页全屏期间仍可点击 YouTube 原生全屏按钮进入系统全屏，退出系统全屏后会恢复网页全屏状态。

扩展不解析视频地址、不读取网络请求、不访问第三方服务，不申请额外权限，只在 `https://www.youtube.com/*` 注入 `content.js` 和 `content.css`。

## 维护说明

YouTube DOM 不是公开稳定接口。当前实现以 `ytd-player`、`#movie_player`、`.ytp-right-controls` 和 `.ytp-fullscreen-button` 为主要定位点，并通过活动视频评分、YouTube 站内导航事件和 `MutationObserver` 处理播放器重建；YouTube 大规模调整播放器结构时需要更新选择器。
