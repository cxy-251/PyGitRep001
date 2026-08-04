const TOGGLE_MESSAGE = "viewport-fullscreen:toggle-picker";
const INJECTED_FILES = Object.freeze({
  css: ["generic.css"],
  js: ["generic.js"]
});

function isInjectableUrl(url = "") {
  return url.startsWith("http://") || url.startsWith("https://");
}

async function sendToggle(tabId) {
  return chrome.tabs.sendMessage(tabId, { type: TOGGLE_MESSAGE });
}

async function ensureGenericRuntime(tabId) {
  try {
    return await sendToggle(tabId);
  } catch (_error) {
    await chrome.scripting.insertCSS({
      target: { tabId },
      files: INJECTED_FILES.css
    });
    await chrome.scripting.executeScript({
      target: { tabId },
      files: INJECTED_FILES.js
    });
    return sendToggle(tabId);
  }
}

chrome.action.onClicked.addListener((tab) => {
  if (!tab.id || !isInjectableUrl(tab.url)) return;
  ensureGenericRuntime(tab.id).catch((error) => {
    console.warn("[网页视口全屏助手] 无法在当前页面运行。", error);
  });
});
