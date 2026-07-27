(() => {
  const namespace = (globalThis.ChromeWorkflowAssistant ||= {});
  if (namespace.shared) return;

  function normalizeText(value) {
    return String(value || "").replace(/\s+/g, " ").trim();
  }

  function isVisible(element) {
    if (!(element instanceof Element)) return false;
    const style = getComputedStyle(element);
    const rect = element.getBoundingClientRect();
    return (
      style.display !== "none" &&
      style.visibility !== "hidden" &&
      Number(style.opacity || 1) !== 0 &&
      rect.width > 0 &&
      rect.height > 0
    );
  }

  function firstVisible(selectors, root = document) {
    for (const selector of selectors) {
      const match = Array.from(root.querySelectorAll(selector)).find(isVisible);
      if (match) return match;
    }
    return null;
  }

  function allVisible(selectors, root = document) {
    const found = [];
    const seen = new Set();
    for (const selector of selectors) {
      for (const element of root.querySelectorAll(selector)) {
        if (!seen.has(element) && isVisible(element)) {
          seen.add(element);
          found.push(element);
        }
      }
    }
    return found;
  }

  function nativeValueSetter(element) {
    const prototypes = [
      HTMLInputElement.prototype,
      HTMLTextAreaElement.prototype,
      HTMLSelectElement.prototype
    ];
    for (const prototype of prototypes) {
      if (element instanceof prototype.constructor) {
        return Object.getOwnPropertyDescriptor(prototype, "value")?.set || null;
      }
    }
    return null;
  }

  function emitEditEvents(element) {
    element.dispatchEvent(new InputEvent("input", { bubbles: true, composed: true }));
    element.dispatchEvent(new Event("change", { bubbles: true, composed: true }));
  }

  function setControlValue(element, value) {
    if (!element) return false;
    element.focus();
    if (element instanceof HTMLInputElement || element instanceof HTMLTextAreaElement || element instanceof HTMLSelectElement) {
      const setter = nativeValueSetter(element);
      if (setter) setter.call(element, String(value));
      else element.value = String(value);
      emitEditEvents(element);
      return true;
    }
    if (element instanceof HTMLElement && element.isContentEditable) {
      const selection = getSelection();
      const range = document.createRange();
      range.selectNodeContents(element);
      selection?.removeAllRanges();
      selection?.addRange(range);
      const inserted = document.execCommand("insertText", false, String(value));
      selection?.removeAllRanges();
      if (!inserted) element.textContent = String(value);
      emitEditEvents(element);
      return true;
    }
    return false;
  }

  function textMatches(element, terms) {
    const text = normalizeText(element.textContent).toLowerCase();
    return terms.some((term) => text.includes(term.toLowerCase()));
  }

  function findNearLabel(terms, controlSelector) {
    const labels = allVisible(["label", "span", "div", "p"])
      .filter((element) => textMatches(element, terms))
      .sort((a, b) => normalizeText(a.textContent).length - normalizeText(b.textContent).length);

    for (const label of labels.slice(0, 80)) {
      const direct = label.matches(controlSelector) ? label : label.querySelector(controlSelector);
      if (direct && isVisible(direct)) return direct;

      let parent = label.parentElement;
      for (let depth = 0; parent && depth < 4; depth += 1, parent = parent.parentElement) {
        const candidate = Array.from(parent.querySelectorAll(controlSelector)).find(isVisible);
        if (candidate) return candidate;
      }
    }
    return null;
  }

  function clickCheckboxForLabel(terms, desired) {
    const control = findNearLabel(terms, "input[type='checkbox'],[role='checkbox'],button[role='switch']");
    if (!control) return { ok: false, reason: "control_not_found" };
    const current = control instanceof HTMLInputElement
      ? control.checked
      : control.getAttribute("aria-checked") === "true";
    if (current !== Boolean(desired)) control.click();
    return { ok: true, changed: current !== Boolean(desired), current: Boolean(desired) };
  }

  function highlight(element, duration = 4000) {
    if (!element) return false;
    const target = isVisible(element) ? element : element.parentElement || element;
    target.scrollIntoView({ behavior: "smooth", block: "center" });
    const previous = target.style.outline;
    const previousOffset = target.style.outlineOffset;
    target.style.outline = "3px solid #f59e0b";
    target.style.outlineOffset = "4px";
    setTimeout(() => {
      target.style.outline = previous;
      target.style.outlineOffset = previousOffset;
    }, duration);
    return true;
  }

  function visibleRiskSignal() {
    const riskPattern = /验证码|访问异常|账号异常|安全验证|操作频繁|暂时封禁|verify|captcha|unusual activity/i;
    const selectors = [
      "[role='dialog']",
      "[class*='captcha']",
      "[class*='verify']",
      "[class*='security']",
      "[class*='risk']",
      ".geetest_panel",
      ".captcha"
    ];
    const node = allVisible(selectors).find((element) => riskPattern.test(normalizeText(element.textContent)));
    return node ? normalizeText(node.textContent).slice(0, 300) : "";
  }

  function uniqueBy(items, keyFactory) {
    const result = [];
    const seen = new Set();
    for (const item of items) {
      const key = keyFactory(item);
      if (!key || seen.has(key)) continue;
      seen.add(key);
      result.push(item);
    }
    return result;
  }

  namespace.shared = {
    normalizeText,
    isVisible,
    firstVisible,
    allVisible,
    setControlValue,
    findNearLabel,
    clickCheckboxForLabel,
    highlight,
    visibleRiskSignal,
    uniqueBy
  };
})();
