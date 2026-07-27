const DEFAULT_SETTINGS = {
  kuaishou: {
    caption: "",
    collectionName: "",
    nearbyMode: "",
    scheduledPublishTime: ""
  },
  boss: {
    keywords: "",
    template: "您好，我关注到「{title}」岗位，想进一步了解该岗位的核心工作、技术栈和团队方向。"
  },
  x: {
    includeAll: false
  }
};

const state = {
  site: "unsupported",
  bossJobs: [],
  selectedBossJob: null,
  xItems: []
};

const element = (id) => document.getElementById(id);

function log(message, data) {
  const timestamp = new Date().toLocaleTimeString();
  const detail = data === undefined ? "" : `\n${JSON.stringify(data, null, 2)}`;
  element("log-output").textContent = `[${timestamp}] ${message}${detail}`;
}

async function send(message) {
  const response = await chrome.runtime.sendMessage(message);
  if (!response?.ok) {
    throw new Error(response?.error || "扩展操作失败");
  }
  return response;
}

async function runAction(action, payload = {}) {
  const response = await send({ type: "CWA_RUN", action, payload });
  return response.response;
}

function showSite(site) {
  for (const id of ["unsupported-section", "kuaishou-section", "boss-section", "x-section"]) {
    element(id).classList.add("hidden");
  }
  const sectionId = site === "kuaishou"
    ? "kuaishou-section"
    : site === "boss"
      ? "boss-section"
      : site === "x"
        ? "x-section"
        : "unsupported-section";
  element(sectionId).classList.remove("hidden");
}

async function refreshContext() {
  element("page-status").textContent = "检测中";
  try {
    const response = await send({ type: "CWA_CONTEXT" });
    const detection = response.response || {};
    state.site = detection.site || "unsupported";
    element("site-name").textContent = state.site === "kuaishou"
      ? "快手"
      : state.site === "boss"
        ? "BOSS 直聘"
        : state.site === "x"
          ? "X"
          : "未适配";
    element("page-status").textContent = detection.pageType || (detection.supported ? "可用" : "未适配");
    element("permission-hint").textContent = detection.risk
      ? `页面出现风险提示：${detection.risk}`
      : "扩展仅能访问本次点击扩展图标时的活动标签页。";
    showSite(state.site);
    log("页面检测完成", detection);
  } catch (error) {
    state.site = "unsupported";
    showSite(state.site);
    element("site-name").textContent = "无法访问";
    element("page-status").textContent = "请重新点击扩展图标";
    element("permission-hint").textContent = error.message;
    log("页面检测失败", { error: error.message });
  }
}

function replaceTemplate(template, job) {
  const values = {
    title: job?.title || "该",
    company: job?.company || "贵公司",
    salary: job?.salary || "",
    location: job?.location || ""
  };
  return template.replace(/\{(title|company|salary|location)\}/g, (_match, key) => values[key]);
}

function currentBossMessage() {
  return replaceTemplate(element("boss-template").value.trim(), state.selectedBossJob);
}

function renderBossJobs() {
  const container = element("boss-jobs");
  container.replaceChildren();
  for (const job of state.bossJobs) {
    const item = document.createElement("article");
    item.className = "result-item";
    if (job === state.selectedBossJob) item.classList.add("selected");

    const title = document.createElement("strong");
    title.textContent = job.title || "未命名职位";
    const meta = document.createElement("small");
    meta.textContent = [job.company, job.salary, job.location].filter(Boolean).join(" · ");
    const tags = document.createElement("p");
    tags.textContent = (job.tags || []).join(" / ");
    const button = document.createElement("button");
    button.className = "secondary";
    button.textContent = "选择职位";
    button.addEventListener("click", () => {
      state.selectedBossJob = job;
      renderBossJobs();
      log("已选择职位", job);
    });

    item.append(title, meta, tags, button);
    container.append(item);
  }
}

function xItemKey(item) {
  return item.url || `${item.author}|${item.createdAt}|${item.text}`;
}

function mergeXItems(items) {
  const map = new Map(state.xItems.map((item) => [xItemKey(item), item]));
  for (const item of items) map.set(xItemKey(item), item);
  state.xItems = Array.from(map.values()).slice(-1000);
}

function renderXItems() {
  element("x-summary").textContent = `已保存 ${state.xItems.length} 条（最多保留 1000 条）`;
  const container = element("x-items");
  container.replaceChildren();
  for (const item of state.xItems.slice().reverse()) {
    const article = document.createElement("article");
    article.className = "result-item";
    const title = document.createElement("strong");
    title.textContent = [item.author || "未知作者", item.score ? `评分 ${item.score}` : ""].filter(Boolean).join(" · ");
    const meta = document.createElement("small");
    meta.textContent = item.createdAt || item.url || "";
    const text = document.createElement("p");
    text.textContent = item.text;
    article.append(title, meta, text);
    container.append(article);
  }
}

function downloadText(filename, content, type) {
  const url = URL.createObjectURL(new Blob([content], { type }));
  const anchor = document.createElement("a");
  anchor.href = url;
  anchor.download = filename;
  anchor.click();
  setTimeout(() => URL.revokeObjectURL(url), 1000);
}

async function loadSettings() {
  const stored = await chrome.storage.local.get({
    cwaSettings: DEFAULT_SETTINGS,
    cwaXItems: []
  });
  const settings = {
    kuaishou: { ...DEFAULT_SETTINGS.kuaishou, ...(stored.cwaSettings?.kuaishou || {}) },
    boss: { ...DEFAULT_SETTINGS.boss, ...(stored.cwaSettings?.boss || {}) },
    x: { ...DEFAULT_SETTINGS.x, ...(stored.cwaSettings?.x || {}) }
  };
  element("kuaishou-caption").value = settings.kuaishou.caption;
  element("kuaishou-collection").value = settings.kuaishou.collectionName;
  element("kuaishou-nearby-mode").value = settings.kuaishou.nearbyMode;
  element("kuaishou-time").value = settings.kuaishou.scheduledPublishTime;
  element("boss-keywords").value = settings.boss.keywords;
  element("boss-template").value = settings.boss.template;
  element("x-include-all").checked = settings.x.includeAll;
  state.xItems = Array.isArray(stored.cwaXItems) ? stored.cwaXItems.slice(-1000) : [];
  renderXItems();
}

async function saveSettings() {
  const cwaSettings = {
    kuaishou: {
      caption: element("kuaishou-caption").value,
      collectionName: element("kuaishou-collection").value,
      nearbyMode: element("kuaishou-nearby-mode").value,
      scheduledPublishTime: element("kuaishou-time").value
    },
    boss: {
      keywords: element("boss-keywords").value,
      template: element("boss-template").value
    },
    x: {
      includeAll: element("x-include-all").checked
    }
  };
  await chrome.storage.local.set({ cwaSettings });
}

function bindEvents() {
  element("refresh-context").addEventListener("click", refreshContext);

  element("kuaishou-highlight").addEventListener("click", async () => {
    try {
      const result = await runAction("kuaishou.highlight-upload");
      log("快手上传区域定位", result);
    } catch (error) {
      log("定位失败", { error: error.message });
    }
  });

  element("kuaishou-apply").addEventListener("click", async () => {
    await saveSettings();
    const payload = {
      caption: element("kuaishou-caption").value,
      collectionName: element("kuaishou-collection").value,
      scheduledPublishTime: element("kuaishou-time").value
    };
    const nearbyMode = element("kuaishou-nearby-mode").value;
    if (nearbyMode !== "") payload.showInNearby = nearbyMode === "true";
    try {
      const result = await runAction("kuaishou.apply-settings", payload);
      log("快手字段写入完成", result);
    } catch (error) {
      log("快手字段写入失败", { error: error.message });
    }
  });

  element("boss-scan").addEventListener("click", async () => {
    await saveSettings();
    try {
      const result = await runAction("boss.scan-jobs", {
        keywords: element("boss-keywords").value
      });
      state.bossJobs = result.jobs || [];
      state.selectedBossJob = state.bossJobs[0] || null;
      element("boss-summary").textContent = `扫描 ${result.scanned || 0} 条，保留 ${state.bossJobs.length} 条`;
      renderBossJobs();
      log("BOSS 职位扫描完成", result);
    } catch (error) {
      log("BOSS 扫描失败", { error: error.message });
    }
  });

  element("boss-copy").addEventListener("click", async () => {
    try {
      const message = currentBossMessage();
      await navigator.clipboard.writeText(message);
      log("沟通草稿已复制", { message });
    } catch (error) {
      log("复制失败", { error: error.message });
    }
  });

  element("boss-fill").addEventListener("click", async () => {
    await saveSettings();
    const message = currentBossMessage();
    try {
      const result = await runAction("boss.fill-message", { message });
      log("沟通草稿已填入", result);
    } catch (error) {
      log("填入聊天框失败", { error: error.message });
    }
  });

  element("x-scan").addEventListener("click", async () => {
    await saveSettings();
    try {
      const result = await runAction("x.scan-prompts", {
        includeAll: element("x-include-all").checked
      });
      mergeXItems(result.items || []);
      await chrome.storage.local.set({ cwaXItems: state.xItems });
      renderXItems();
      log("X 时间线扫描完成", result);
    } catch (error) {
      log("X 扫描失败", { error: error.message });
    }
  });

  element("x-export-json").addEventListener("click", () => {
    downloadText("x-prompts.json", JSON.stringify(state.xItems, null, 2), "application/json");
  });

  element("x-export-md").addEventListener("click", () => {
    const markdown = state.xItems.map((item, index) => [
      `## ${index + 1}. ${item.author || "未知作者"}`,
      item.url ? `来源：${item.url}` : "",
      item.createdAt ? `时间：${item.createdAt}` : "",
      "",
      item.text,
      ""
    ].join("\n")).join("\n\n");
    downloadText("x-prompts.md", markdown, "text/markdown");
  });

  element("x-clear").addEventListener("click", async () => {
    state.xItems = [];
    await chrome.storage.local.set({ cwaXItems: [] });
    renderXItems();
    log("已清空 X 提取结果");
  });
}

async function initialize() {
  bindEvents();
  await loadSettings();
  await refreshContext();
}

initialize().catch((error) => log("扩展初始化失败", { error: error.message }));
