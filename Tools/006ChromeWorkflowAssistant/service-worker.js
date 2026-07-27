const CONTENT_FILES = [
  "content/shared.js",
  "content/kuaishou.js",
  "content/boss.js",
  "content/x.js",
  "content/bridge.js"
];

const ALLOWED_ACTIONS = new Set([
  "detect",
  "kuaishou.highlight-upload",
  "kuaishou.apply-settings",
  "boss.scan-jobs",
  "boss.fill-message",
  "x.scan-prompts"
]);

async function configureSidePanel() {
  await chrome.sidePanel.setPanelBehavior({ openPanelOnActionClick: true });
}

chrome.runtime.onInstalled.addListener(() => {
  configureSidePanel().catch(console.error);
});

chrome.runtime.onStartup.addListener(() => {
  configureSidePanel().catch(console.error);
});

function isTrustedSidePanel(sender) {
  return (
    sender.id === chrome.runtime.id &&
    typeof sender.url === "string" &&
    sender.url.startsWith(chrome.runtime.getURL("sidepanel/"))
  );
}

async function getActiveTab() {
  const [tab] = await chrome.tabs.query({ active: true, lastFocusedWindow: true });
  if (!tab?.id) {
    throw new Error("没有找到当前活动标签页");
  }
  return tab;
}

async function ensureContentBridge(tabId) {
  await chrome.scripting.executeScript({
    target: { tabId },
    files: CONTENT_FILES
  });
}

async function sendToActiveTab(action, payload = {}) {
  if (!ALLOWED_ACTIONS.has(action)) {
    throw new Error(`不支持的页面动作：${action}`);
  }

  const tab = await getActiveTab();
  await ensureContentBridge(tab.id);
  const response = await chrome.tabs.sendMessage(tab.id, {
    source: "chrome-workflow-assistant",
    type: "CWA_ACTION",
    action,
    payload
  });

  return {
    tab: {
      id: tab.id,
      title: tab.title || "",
      url: tab.url || ""
    },
    response
  };
}

chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  if (!isTrustedSidePanel(sender)) {
    return false;
  }

  const run = async () => {
    if (message?.type === "CWA_CONTEXT") {
      return sendToActiveTab("detect");
    }

    if (message?.type === "CWA_RUN") {
      return sendToActiveTab(message.action, message.payload || {});
    }

    throw new Error("未知扩展消息");
  };

  run()
    .then((result) => sendResponse({ ok: true, ...result }))
    .catch((error) => {
      sendResponse({
        ok: false,
        error: error instanceof Error ? error.message : String(error)
      });
    });

  return true;
});
