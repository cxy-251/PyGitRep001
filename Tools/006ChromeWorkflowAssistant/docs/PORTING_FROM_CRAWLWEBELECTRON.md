# 从 crawlWebElectron 迁移到 Chrome 扩展

## 原实现

`crawlWebElectron` 中存在三类运行方式：

```text
快手：Electron WebContents + DOM/CDP + 本地 HTTP API
BOSS：AppleScript 控制真实 Safari + 注入 JavaScript
X：AppleScript 控制真实 Safari + 注入 JavaScript
```

这些实现已经证明业务流程可行，也暴露了几个问题：跨进程状态复杂、页面轮询频繁、选择器诊断分散、自动化窗口容易干扰人工浏览，以及 DevTools/CDP 路径更容易触发站点异常检测。

## Chrome 版本的统一模型

```text
真实 Chrome 登录会话
-> 用户点击扩展图标
-> activeTab 临时授权
-> Side Panel 发送明确动作
-> Content Script 在页面内读取或写入
-> 返回结构化结果
-> 本地保存和导出
```

不再维护 Electron 浏览器壳、Safari 专用窗口、AppleScript 桥接、本地 HTTP API、任务调度器和跨进程数据库。

## 快手映射

| Electron 能力 | Chrome 扩展处理 |
|---|---|
| 打开上传页 | 用户自行打开 |
| CDP 设置本地视频文件 | 保留人工文件选择 |
| DOM 页面识别 | `kuaishou.detect()` |
| DOM 字段写入 | `kuaishou.applySettings()` |
| 草稿冲突检测 | 当前版本只报告页面类型和能力，不自动处理草稿 |
| 最终发布 | 永远人工确认 |
| 本地 API 批处理 | 当前版本不迁移 |

文件上传是最重要的边界。为了继续使用纯扩展和最小权限，不引入 `chrome.debugger`、Native Messaging 或本地守护进程。

## BOSS 映射

| Safari RPA 能力 | Chrome 扩展处理 |
|---|---|
| 专用 Safari 工作区窗口 | 用户当前 Chrome 标签页 |
| 搜索结果 DOM 读取 | `boss.scanJobs()` |
| 规则筛选 | 侧边栏关键词筛选 |
| 自动进入详情和批量沟通 | 当前版本不迁移 |
| 生成沟通内容 | 侧边栏模板替换 |
| 写入聊天框 | `boss.fillMessage()` |
| 点击发送 | 永远人工确认 |
| 每日额度和计划任务 | 当前版本不迁移 |

BOSS 流程从“自动沟通系统”改成“职位审阅和沟通草稿助手”。这样保留最有价值的页面整理能力，同时避免批量发送、静默沟通和不确定重试。

## X 映射

| Safari RPA 能力 | Chrome 扩展处理 |
|---|---|
| 时间线滚动 | 用户人工滚动 |
| 当前页面推文读取 | `x.scanPrompts()` |
| 原始记录去重 | Side Panel + `chrome.storage.local` |
| 提示词初筛 | 本地规则评分 |
| JSONL/CSV 报告 | JSON / Markdown 导出 |
| 本地 LLM 清洗 | 当前版本不迁移 |
| 详情页自动打开和重试 | 当前版本不迁移 |

X 是最适合纯扩展的场景：整个第一阶段可以保持只读，不需要网络拦截或账号写操作。

## 后续演进顺序

1. 先在三个网站分别维护稳定的页面能力检测和结构化错误结果；
2. 把选择器从代码提取成版本化页面配置；
3. 增加页面诊断面板，显示每个定位器的匹配情况；
4. 增加可导入、导出的本地配置；
5. 只有在明确需要时，再评估受审查的本地 Native Messaging 辅助；
6. 不重新引入 CDP、隐藏自动化、验证码绕过或大规模后台写入。
