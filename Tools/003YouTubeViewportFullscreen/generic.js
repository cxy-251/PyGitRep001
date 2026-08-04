(() => {
  "use strict";

  if (globalThis.__viewportFullscreenGenericInstalled) return;
  globalThis.__viewportFullscreenGenericInstalled = true;

  const MESSAGE_TYPE = "viewport-fullscreen:toggle-picker";
  const ACTIVE_CLASS = "vfs-generic-active";
  const PICKING_CLASS = "vfs-generic-picking";
  const TARGET_ATTRIBUTE = "data-vfs-generic-target";
  const ANCESTOR_ATTRIBUTE = "data-vfs-generic-ancestor";
  const BACKDROP_ID = "vfs-generic-backdrop";
  const OVERLAY_ID = "vfs-generic-picker-overlay";
  const HINT_ID = "vfs-generic-picker-hint";

  const state = {
    active: false,
    picking: false,
    target: null,
    candidate: null,
    ancestors: [],
    savedScrollX: 0,
    savedScrollY: 0,
    savedFocus: null,
    overlay: null,
    hint: null,
    observer: null,
    missingTargetTimer: 0
  };

  function isElement(value) {
    return value instanceof Element;
  }

  function isExtensionUi(element) {
    return Boolean(element?.closest?.(`#${OVERLAY_ID}, #${HINT_ID}, #${BACKDROP_ID}`));
  }

  function isRendered(element) {
    if (!isElement(element) || !element.isConnected) return false;
    const rect = element.getBoundingClientRect();
    if (rect.width < 2 || rect.height < 2) return false;
    const style = getComputedStyle(element);
    return style.display !== "none"
      && style.visibility !== "hidden"
      && Number(style.opacity || 1) > 0;
  }

  function visibleArea(element) {
    const rect = element.getBoundingClientRect();
    const width = Math.max(0, Math.min(rect.right, innerWidth) - Math.max(rect.left, 0));
    const height = Math.max(0, Math.min(rect.bottom, innerHeight) - Math.max(rect.top, 0));
    return width * height;
  }

  function normalizeTarget(rawTarget) {
    let element = rawTarget instanceof Element ? rawTarget : rawTarget?.parentElement;
    if (!element || isExtensionUi(element)) return null;

    const directMedia = element.closest("video, iframe, canvas, img, picture, object, embed, svg");
    if (directMedia && isRendered(directMedia)) return directMedia;

    while (
      element.parentElement
      && element !== document.body
      && element !== document.documentElement
    ) {
      const rect = element.getBoundingClientRect();
      const style = getComputedStyle(element);
      const tooSmall = rect.width < 120 || rect.height < 80;
      const inlineLike = style.display === "inline" || style.display === "contents";
      if (!tooSmall && !inlineLike) break;
      element = element.parentElement;
    }

    if (element === document.body || element === document.documentElement) {
      const preferred = ["main", "article", "[role='main']"]
        .flatMap((selector) => Array.from(document.querySelectorAll(selector)))
        .filter(isRendered)
        .sort((left, right) => visibleArea(right) - visibleArea(left))[0];
      return preferred || null;
    }

    return isRendered(element) ? element : null;
  }

  function describeElement(element) {
    if (!element) return "";
    const tag = element.tagName.toLowerCase();
    const id = element.id ? `#${element.id}` : "";
    const classes = Array.from(element.classList || []).slice(0, 2);
    const classText = classes.length ? `.${classes.join(".")}` : "";
    const rect = element.getBoundingClientRect();
    return `${tag}${id}${classText} · ${Math.round(rect.width)}×${Math.round(rect.height)}`;
  }

  function ensurePickerUi() {
    if (!state.overlay) {
      state.overlay = document.createElement("div");
      state.overlay.id = OVERLAY_ID;
      state.overlay.setAttribute("aria-hidden", "true");
      (document.body || document.documentElement).appendChild(state.overlay);
    }

    if (!state.hint) {
      state.hint = document.createElement("div");
      state.hint.id = HINT_ID;
      state.hint.textContent = "点击要铺满视口的区域 · ↑ 选择父级 · Enter 确认 · Esc 取消";
      (document.body || document.documentElement).appendChild(state.hint);
    }
  }

  function removePickerUi() {
    state.overlay?.remove();
    state.hint?.remove();
    state.overlay = null;
    state.hint = null;
  }

  function updatePickerOverlay(candidate) {
    state.candidate = candidate;
    if (!state.overlay || !candidate || !isRendered(candidate)) {
      if (state.overlay) state.overlay.hidden = true;
      return;
    }

    const rect = candidate.getBoundingClientRect();
    state.overlay.hidden = false;
    state.overlay.style.left = `${Math.max(0, rect.left)}px`;
    state.overlay.style.top = `${Math.max(0, rect.top)}px`;
    state.overlay.style.width = `${Math.max(0, Math.min(rect.right, innerWidth) - Math.max(0, rect.left))}px`;
    state.overlay.style.height = `${Math.max(0, Math.min(rect.bottom, innerHeight) - Math.max(0, rect.top))}px`;
    state.overlay.dataset.label = describeElement(candidate);
  }

  function onPickerPointerMove(event) {
    updatePickerOverlay(normalizeTarget(event.composedPath?.()[0] || event.target));
  }

  function onPickerClick(event) {
    if (!state.picking) return;
    const candidate = state.candidate
      || normalizeTarget(event.composedPath?.()[0] || event.target);
    if (!candidate) return;
    event.preventDefault();
    event.stopImmediatePropagation();
    enterViewportFullscreen(candidate);
  }

  function onPickerKeydown(event) {
    if (!state.picking) return;

    if (event.key === "Escape") {
      event.preventDefault();
      event.stopImmediatePropagation();
      cancelPicker();
      return;
    }

    if (event.key === "ArrowUp" && state.candidate?.parentElement) {
      const parent = normalizeTarget(state.candidate.parentElement);
      if (parent) updatePickerOverlay(parent);
      event.preventDefault();
      event.stopImmediatePropagation();
      return;
    }

    if (event.key === "Enter" && state.candidate) {
      event.preventDefault();
      event.stopImmediatePropagation();
      enterViewportFullscreen(state.candidate);
    }
  }

  function startPicker() {
    if (state.active || state.picking) return;
    state.picking = true;
    document.documentElement.classList.add(PICKING_CLASS);
    ensurePickerUi();
    document.addEventListener("pointermove", onPickerPointerMove, true);
    document.addEventListener("click", onPickerClick, true);
    document.addEventListener("keydown", onPickerKeydown, true);
  }

  function cancelPicker() {
    if (!state.picking) return;
    state.picking = false;
    state.candidate = null;
    document.documentElement.classList.remove(PICKING_CLASS);
    document.removeEventListener("pointermove", onPickerPointerMove, true);
    document.removeEventListener("click", onPickerClick, true);
    document.removeEventListener("keydown", onPickerKeydown, true);
    removePickerUi();
  }

  function getAncestorPath(target) {
    const path = [];
    let parent = target?.parentElement || null;
    while (
      parent
      && parent !== document.body
      && parent !== document.documentElement
    ) {
      path.push(parent);
      parent = parent.parentElement;
    }
    return path;
  }

  function ensureBackdrop() {
    let backdrop = document.getElementById(BACKDROP_ID);
    if (!backdrop) {
      backdrop = document.createElement("div");
      backdrop.id = BACKDROP_ID;
      backdrop.setAttribute("aria-hidden", "true");
      (document.body || document.documentElement).appendChild(backdrop);
    }
    return backdrop;
  }

  function requestLayoutRefresh() {
    requestAnimationFrame(() => {
      window.dispatchEvent(new Event("resize"));
      requestAnimationFrame(() => window.dispatchEvent(new Event("resize")));
    });
  }

  function stopTargetObserver() {
    state.observer?.disconnect();
    state.observer = null;
    clearTimeout(state.missingTargetTimer);
    state.missingTargetTimer = 0;
  }

  function startTargetObserver() {
    stopTargetObserver();
    const root = document.body || document.documentElement;
    state.observer = new MutationObserver(() => {
      if (!state.active || state.target?.isConnected) return;
      if (state.missingTargetTimer) return;
      state.missingTargetTimer = window.setTimeout(() => {
        state.missingTargetTimer = 0;
        if (state.active && !state.target?.isConnected) exitViewportFullscreen();
      }, 300);
    });
    state.observer.observe(root, { childList: true, subtree: true });
  }

  function enterViewportFullscreen(target) {
    if (!target || !isRendered(target)) return;
    cancelPicker();

    state.savedScrollX = window.scrollX;
    state.savedScrollY = window.scrollY;
    state.savedFocus = document.activeElement instanceof HTMLElement
      ? document.activeElement
      : null;
    state.target = target;
    state.ancestors = getAncestorPath(target);
    state.active = true;

    target.setAttribute(TARGET_ATTRIBUTE, "true");
    for (const ancestor of state.ancestors) {
      ancestor.setAttribute(ANCESTOR_ATTRIBUTE, "true");
    }

    ensureBackdrop();
    document.documentElement.classList.add(ACTIVE_CLASS);
    startTargetObserver();
    requestLayoutRefresh();
  }

  function exitViewportFullscreen() {
    if (!state.active) return;

    state.active = false;
    stopTargetObserver();
    document.documentElement.classList.remove(ACTIVE_CLASS);
    state.target?.removeAttribute(TARGET_ATTRIBUTE);
    for (const ancestor of state.ancestors) {
      ancestor.removeAttribute(ANCESTOR_ATTRIBUTE);
    }
    document.getElementById(BACKDROP_ID)?.remove();

    const savedFocus = state.savedFocus;
    const savedScrollX = state.savedScrollX;
    const savedScrollY = state.savedScrollY;
    state.target = null;
    state.ancestors = [];
    state.savedFocus = null;
    requestLayoutRefresh();

    requestAnimationFrame(() => {
      requestAnimationFrame(() => {
        window.scrollTo(savedScrollX, savedScrollY);
        if (savedFocus?.isConnected) savedFocus.focus({ preventScroll: true });
      });
    });
  }

  function togglePicker() {
    if (state.active) {
      exitViewportFullscreen();
      return "exited";
    }
    if (state.picking) {
      cancelPicker();
      return "cancelled";
    }
    startPicker();
    return "picking";
  }

  function onGlobalKeydown(event) {
    if (event.key !== "Escape" || !state.active) return;
    event.preventDefault();
    event.stopImmediatePropagation();
    exitViewportFullscreen();
  }

  chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
    if (sender.id !== chrome.runtime.id || message?.type !== MESSAGE_TYPE) return false;
    sendResponse({ ok: true, state: togglePicker() });
    return false;
  });

  document.addEventListener("keydown", onGlobalKeydown, true);
  window.addEventListener("pagehide", () => {
    cancelPicker();
    exitViewportFullscreen();
  }, { once: true });
})();
