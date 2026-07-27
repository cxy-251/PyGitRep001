(() => {
  if (globalThis.__CWA_BRIDGE_INSTALLED__) return;
  globalThis.__CWA_BRIDGE_INSTALLED__ = true;

  const namespace = globalThis.ChromeWorkflowAssistant;

  function currentAdapter() {
    if (namespace.kuaishou.supports()) return namespace.kuaishou;
    if (namespace.boss.supports()) return namespace.boss;
    if (namespace.x.supports()) return namespace.x;
    return null;
  }

  function detect() {
    const adapter = currentAdapter();
    if (adapter) return adapter.detect();
    return {
      site: "unsupported",
      supported: false,
      url: location.href,
      hostname: location.hostname,
      message: "当前页面暂未适配。"
    };
  }

  function execute(action, payload) {
    switch (action) {
      case "detect":
        return detect();
      case "kuaishou.highlight-upload":
        return namespace.kuaishou.supports()
          ? namespace.kuaishou.highlightUpload()
          : { ok: false, reason: "wrong_site" };
      case "kuaishou.apply-settings":
        return namespace.kuaishou.supports()
          ? namespace.kuaishou.applySettings(payload)
          : { ok: false, reason: "wrong_site" };
      case "boss.scan-jobs":
        return namespace.boss.supports()
          ? namespace.boss.scanJobs(payload)
          : { ok: false, reason: "wrong_site" };
      case "boss.fill-message":
        return namespace.boss.supports()
          ? namespace.boss.fillMessage(payload)
          : { ok: false, reason: "wrong_site" };
      case "x.scan-prompts":
        return namespace.x.supports()
          ? namespace.x.scanPrompts(payload)
          : { ok: false, reason: "wrong_site" };
      default:
        return { ok: false, reason: "unsupported_action" };
    }
  }

  chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
    if (
      sender.id !== chrome.runtime.id ||
      message?.source !== "chrome-workflow-assistant" ||
      message?.type !== "CWA_ACTION"
    ) {
      return false;
    }

    try {
      sendResponse(execute(message.action, message.payload || {}));
    } catch (error) {
      sendResponse({
        ok: false,
        reason: "page_action_failed",
        error: error instanceof Error ? error.message : String(error)
      });
    }
    return false;
  });
})();
