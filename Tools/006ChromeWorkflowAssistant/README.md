# 网页工作流助手

Chrome Manifest V3 扩展原型，用真实、已登录的 Chrome 标签页替代 Electron CDP 与 Safari AppleScript 高频注入。

## 当前能力

### 快手

- 检测上传页、编辑页和风险提示；
- 高亮视频上传区域；
- 用户在网页中人工选择视频文件；
- 填入作品描述、合集文本、同城开关和可识别的定时发布时间输入框；
- 不点击最终发布。

### BOSS 直聘

- 扫描当前页面已经加载的职位卡片；
- 按关键词筛选；
- 生成沟通草稿；
- 把草稿填入当前聊天输入框；
- 不自动滚动，不批量联系，不点击发送。

### X

- 扫描当前页面已经加载且可见的推文；
- 根据 `prompt`、提示词、模型参数和视觉描述等信号筛选；
- 本地去重保存；
- 导出 JSON 或 Markdown；
- 不自动滚动，不调用 X 私有接口。

## 权限模型

`manifest.json` 只申请：

```text
activeTab
scripting
sidePanel
storage
```

扩展没有：

```text
<all_urls>
host_permissions
debugger
cookies
webRequest
远程代码
```

用户点击扩展图标后，Chrome 仅为当时的活动标签页授予临时访问权。切换页面或重新导航后，需要再次点击扩展图标。

## 架构

```text
Chrome action click
  -> Side Panel
  -> Service Worker
  -> chrome.scripting.executeScript
  -> isolated content bridge
  -> kuaishou / boss / x adapter
  -> structured result
  -> Side Panel local state
```

目录：

```text
manifest.json
service-worker.js
content/
  shared.js
  kuaishou.js
  boss.js
  x.js
  bridge.js
sidepanel/
  index.html
  styles.css
  app.js
docs/
  PORTING_FROM_CRAWLWEBELECTRON.md
SECURITY.md
```

## 安装

1. 打开 `chrome://extensions`；
2. 开启“开发者模式”；
3. 点击“加载已解压的扩展程序”；
4. 选择 `Tools/006ChromeWorkflowAssistant`；
5. 打开目标网站并完成登录；
6. 点击扩展图标，侧边栏会自动打开。

## 使用顺序

快手：

```text
打开创作者上传页
-> 点击扩展图标
-> 定位上传区域
-> 在网页中人工选择视频
-> 等待进入编辑页
-> 重新检测
-> 填写侧边栏字段
-> 填入当前编辑页
-> 人工核对并发布
```

BOSS：

```text
打开职位搜索或聊天页面
-> 点击扩展图标
-> 扫描当前页面职位
-> 选择职位
-> 修改沟通模板
-> 填入聊天框
-> 人工检查并发送
```

X：

```text
打开时间线
-> 页面正常滚动到需要的位置
-> 点击扩展图标
-> 扫描当前可见时间线
-> 重复滚动和扫描
-> 导出 JSON 或 Markdown
```

## 限制

- Chrome 扩展不能可靠、合规地静默选择任意本地视频文件，因此快手文件选择保留人工步骤。
- 网站 DOM 不是稳定 API，选择器可能随页面更新失效。
- 扩展运行在当前用户会话中，不能消除网站风控，也不提供反检测或绕过机制。
- 当前版本没有自动发布、自动发送、自动滚动、定时任务和后台批处理。
- BOSS 与快手写入动作均由侧边栏中的明确用户点击触发。

迁移设计见 [`docs/PORTING_FROM_CRAWLWEBELECTRON.md`](docs/PORTING_FROM_CRAWLWEBELECTRON.md)，安全边界见 [`SECURITY.md`](SECURITY.md)。
