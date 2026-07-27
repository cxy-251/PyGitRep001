(() => {
  const namespace = (globalThis.ChromeWorkflowAssistant ||= {});
  if (namespace.boss) return;
  const shared = namespace.shared;

  const CARD_SELECTORS = [
    "li.job-card-wrapper",
    ".job-card-box",
    "ul.rec-job-list > li",
    ".job-list-box li",
    "[class*='job-card']"
  ];

  function supports() {
    return /(^|\.)zhipin\.com$/i.test(location.hostname);
  }

  function readFirst(root, selectors) {
    const element = shared.firstVisible(selectors, root);
    return shared.normalizeText(element?.textContent);
  }

  function scanJobs(payload = {}) {
    const keywords = String(payload.keywords || "")
      .split(/[,，\n]/)
      .map((item) => item.trim().toLowerCase())
      .filter(Boolean);
    const cards = shared.allVisible(CARD_SELECTORS);
    const extracted = cards.map((card) => {
      const link = card.querySelector("a.job-name,a[href*='/job_detail/'],a[href*='/job_detail']");
      const title = readFirst(card, [".job-name", "[class*='job-name']", "[class*='job-title']"]);
      const company = readFirst(card, [".company-name", "[class*='company-name']", "[class*='company']"]);
      const salary = readFirst(card, [".salary", "[class*='salary']"]);
      const locationText = readFirst(card, [".job-area", "[class*='job-area']", "[class*='location']"]);
      const tags = shared.allVisible([".tag-list li", "[class*='tag']"], card)
        .map((element) => shared.normalizeText(element.textContent))
        .filter(Boolean)
        .slice(0, 12);
      return {
        title,
        company,
        salary,
        location: locationText,
        tags,
        url: link?.href || ""
      };
    }).filter((job) => job.title || job.company);

    const jobs = shared.uniqueBy(
      extracted,
      (job) => job.url || `${job.title}|${job.company}|${job.salary}`
    );
    const filtered = keywords.length
      ? jobs.filter((job) => {
          const haystack = `${job.title} ${job.company} ${job.salary} ${job.location} ${job.tags.join(" ")}`.toLowerCase();
          return keywords.some((keyword) => haystack.includes(keyword));
        })
      : jobs;

    return {
      ok: true,
      scanned: jobs.length,
      jobs: filtered,
      risk: shared.visibleRiskSignal(),
      autoScrolled: false,
      messagesSent: 0
    };
  }

  function findChatInput() {
    return shared.firstVisible([
      ".chat-conversation textarea",
      ".chat-panel textarea",
      "[class*='chat'] textarea[placeholder*='消息']",
      "[class*='chat'] textarea[placeholder*='输入']",
      "[class*='chat'] [contenteditable='true']",
      "textarea[placeholder*='沟通']",
      "textarea[placeholder*='消息']"
    ]);
  }

  function fillMessage(payload) {
    const message = typeof payload.message === "string" ? payload.message.trim() : "";
    if (!message) return { ok: false, reason: "empty_message" };
    const input = findChatInput();
    if (!input) return { ok: false, reason: "chat_input_not_found" };
    const ok = shared.setControlValue(input, message);
    shared.highlight(input, 2500);
    return {
      ok,
      messageFilled: ok,
      sendButtonClicked: false,
      note: "消息已填入；请人工检查并点击发送。"
    };
  }

  function detect() {
    const text = shared.normalizeText(document.body?.innerText);
    return {
      site: "boss",
      supported: supports(),
      loginRequired: /登录|扫码登录/.test(text) && !document.querySelector(".nav-figure,.user-nav,[class*='user-nav']"),
      risk: shared.visibleRiskSignal(),
      capabilities: {
        jobCards: shared.allVisible(CARD_SELECTORS).length,
        chatInput: Boolean(findChatInput())
      }
    };
  }

  namespace.boss = { supports, detect, scanJobs, fillMessage };
})();
