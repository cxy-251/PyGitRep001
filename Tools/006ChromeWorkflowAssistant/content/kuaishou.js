(() => {
  const namespace = (globalThis.ChromeWorkflowAssistant ||= {});
  if (namespace.kuaishou) return;
  const shared = namespace.shared;

  const CAPTION_SELECTORS = [
    "textarea[placeholder*='作品描述']",
    "textarea[placeholder*='描述']",
    "[contenteditable='true'][data-placeholder*='作品描述']",
    "[contenteditable='true'][data-placeholder*='描述']",
    "[contenteditable='true'][aria-label*='描述']"
  ];

  const COLLECTION_SELECTORS = [
    "input[placeholder*='合集']",
    "input[aria-label*='合集']",
    "[role='combobox'][aria-label*='合集']"
  ];

  const SCHEDULE_SELECTORS = [
    "input[type='datetime-local']",
    "input[placeholder*='发布时间']",
    "input[placeholder*='日期']"
  ];

  function supports() {
    return /(^|\.)kuaishou\.com$/i.test(location.hostname);
  }

  function detect() {
    const text = shared.normalizeText(document.body?.innerText);
    const fileInput = document.querySelector("input[type='file'][accept*='video'],input[type='file']");
    const caption = shared.firstVisible(CAPTION_SELECTORS);
    const publishButton = shared.allVisible(["button", "[role='button']"])
      .find((element) => /发布作品|确认发布|立即发布|发布/.test(shared.normalizeText(element.textContent)));
    const loginRequired = /扫码登录|密码登录|登录后继续/.test(text) && !caption && !fileInput;
    const pageType = caption
      ? "editor"
      : fileInput
        ? "upload"
        : /创作者服务平台|内容管理|作品管理/.test(text)
          ? "creator-center"
          : "unknown";

    return {
      site: "kuaishou",
      supported: supports(),
      pageType,
      loginRequired,
      risk: shared.visibleRiskSignal(),
      capabilities: {
        fileInput: Boolean(fileInput),
        caption: Boolean(caption),
        collection: Boolean(shared.firstVisible(COLLECTION_SELECTORS) || shared.findNearLabel(["合集"], "input,[role='combobox']")),
        nearby: Boolean(shared.findNearLabel(["同城页展示", "同城", "附近"], "input[type='checkbox'],[role='checkbox'],button[role='switch']")),
        schedule: Boolean(shared.firstVisible(SCHEDULE_SELECTORS) || shared.findNearLabel(["发布时间", "定时发布"], "input")),
        publishButton: Boolean(publishButton)
      }
    };
  }

  function highlightUpload() {
    const input = document.querySelector("input[type='file'][accept*='video'],input[type='file']");
    if (!input) return { ok: false, reason: "file_input_not_found", detection: detect() };
    const target = input.closest("label,[class*='upload'],[class*='Upload']") || input.parentElement || input;
    shared.highlight(target);
    return {
      ok: true,
      message: "已定位上传区域。请在页面中人工选择视频文件；扩展不会静默读取本地文件。"
    };
  }

  function applySettings(payload) {
    const results = {};

    if (typeof payload.caption === "string" && payload.caption.trim()) {
      const control = shared.firstVisible(CAPTION_SELECTORS) || shared.findNearLabel(["作品描述", "描述"], "textarea,[contenteditable='true']");
      results.caption = control
        ? { ok: shared.setControlValue(control, payload.caption.trim()) }
        : { ok: false, reason: "caption_control_not_found" };
    }

    if (typeof payload.collectionName === "string" && payload.collectionName.trim()) {
      const control = shared.firstVisible(COLLECTION_SELECTORS) || shared.findNearLabel(["合集"], "input,[role='combobox']");
      results.collectionName = control
        ? {
            ok: shared.setControlValue(control, payload.collectionName.trim()),
            note: "仅填入合集文本；候选选择仍需人工确认。"
          }
        : { ok: false, reason: "collection_control_not_found" };
    }

    if (typeof payload.showInNearby === "boolean") {
      results.showInNearby = shared.clickCheckboxForLabel(
        ["同城页展示", "同城", "附近"],
        payload.showInNearby
      );
    }

    if (typeof payload.scheduledPublishTime === "string" && payload.scheduledPublishTime) {
      const control = shared.firstVisible(SCHEDULE_SELECTORS) || shared.findNearLabel(["发布时间", "定时发布"], "input");
      results.scheduledPublishTime = control
        ? {
            ok: shared.setControlValue(control, payload.scheduledPublishTime),
            note: "自定义日期组件可能仍需人工确认。"
          }
        : { ok: false, reason: "schedule_control_not_found" };
    }

    return {
      ok: Object.values(results).some((item) => item?.ok),
      results,
      detection: detect(),
      finalPublishClicked: false
    };
  }

  namespace.kuaishou = { supports, detect, highlightUpload, applySettings };
})();
