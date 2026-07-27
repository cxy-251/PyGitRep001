(() => {
  const namespace = (globalThis.ChromeWorkflowAssistant ||= {});
  if (namespace.x) return;
  const shared = namespace.shared;

  function supports() {
    return location.hostname === "x.com" || location.hostname === "twitter.com";
  }

  function promptScore(text) {
    let score = 0;
    if (/\bprompt\b|提示词|negative prompt|system prompt|midjourney|stable diffusion|flux|sora|veo|runway/i.test(text)) score += 3;
    if (/--ar\b|--stylize\b|steps\s*[:=]|cfg\s*[:=]|seed\s*[:=]|镜头|光线|构图|材质|风格/i.test(text)) score += 2;
    if (/```|\{[^}]{20,}\}|\[[^\]]{20,}\]/s.test(text)) score += 1;
    if (text.length >= 160) score += 1;
    return score;
  }

  function findAuthor(article) {
    const userName = article.querySelector("[data-testid='User-Name']");
    const links = Array.from(userName?.querySelectorAll("a[href^='/']") || []);
    const profileLink = links.find((anchor) => {
      const href = anchor.getAttribute("href") || "";
      return /^\/[A-Za-z0-9_]+$/.test(href) && !/^\/(home|explore|notifications|messages|i)$/.test(href);
    });
    const handle = links
      .map((anchor) => shared.normalizeText(anchor.textContent))
      .find((text) => text.startsWith("@"));
    return {
      author: handle || shared.normalizeText(profileLink?.textContent),
      authorUrl: profileLink?.href || ""
    };
  }

  function scanPrompts(payload = {}) {
    const includeAll = Boolean(payload.includeAll);
    const articles = Array.from(document.querySelectorAll("article[data-testid='tweet']"))
      .filter(shared.isVisible);
    const items = [];

    for (const article of articles) {
      const textNodes = Array.from(article.querySelectorAll("[data-testid='tweetText']"));
      const text = shared.normalizeText(textNodes.map((node) => node.innerText || node.textContent).join("\n"));
      if (!text) continue;
      const time = article.querySelector("time");
      const statusLink = time?.closest("a[href*='/status/']") || article.querySelector("a[href*='/status/']");
      const author = findAuthor(article);
      const score = promptScore(text);
      if (!includeAll && score < 2) continue;
      items.push({
        text,
        score,
        author: author.author,
        authorUrl: author.authorUrl,
        createdAt: time?.getAttribute("datetime") || "",
        url: statusLink?.href || ""
      });
    }

    return {
      ok: true,
      scanned: articles.length,
      items: shared.uniqueBy(items, (item) => item.url || item.text),
      autoScrolled: false,
      readOnly: true
    };
  }

  function detect() {
    return {
      site: "x",
      supported: supports(),
      risk: shared.visibleRiskSignal(),
      capabilities: {
        visibleTweets: Array.from(document.querySelectorAll("article[data-testid='tweet']")).filter(shared.isVisible).length
      }
    };
  }

  namespace.x = { supports, detect, scanPrompts };
})();
