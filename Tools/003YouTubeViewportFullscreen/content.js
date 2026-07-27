(() => {
  "use strict";

  // ---------- 常量 ----------

  const BUTTON_ID = "yt-webpage-fullscreen-button";
  const BACKDROP_ID = "yt-webpage-fullscreen-backdrop";
  const ACTIVE_CLASS = "yt-webpage-fullscreen-active";
  const NATIVE_CLASS = "yt-webpage-native-fullscreen";
  const PLAYER_ATTRIBUTE = "data-yt-webpage-fullscreen-player";
  const ANCESTOR_ATTRIBUTE = "data-yt-webpage-fullscreen-ancestor";
  const LEGACY_SIBLING_ATTRIBUTE = "data-yt-webpage-fullscreen-sibling";

  const TIMING = Object.freeze({
    discoveryDelay: 120,
    controlsDelay: 80,
    navigationDelay: 150,
    popstateDelay: 100,
    viewportDelay: 100,
    missingPlayerGrace: 1_500,
    healthInterval: 5_000
  });

  const ENTER_ICON = `
    <svg viewBox="0 0 36 36" aria-hidden="true" focusable="false">
      <path d="M7 7h9v3h-6v6H7V7Zm13 0h9v9h-3v-6h-6V7ZM7 20h3v6h6v3H7v-9Zm19 0h3v9h-9v-3h6v-6Z"/>
    </svg>`;

  const EXIT_ICON = `
    <svg viewBox="0 0 36 36" aria-hidden="true" focusable="false">
      <path d="M7 15v-3h5V7h3v8H7Zm14 0V7h3v5h5v3h-8ZM7 21h8v8h-3v-5H7v-3Zm14 0h8v3h-5v5h-3v-8Z"/>
    </svg>`;

  // ---------- 运行状态 ----------

  const state = {
    viewport: {
      active: false,
      player: null,
      ancestors: [],
      savedScrollX: 0,
      savedScrollY: 0,
      savedFocus: null
    },
    binding: {
      player: null,
      controlsRoot: null
    },
    observers: {
      discovery: null,
      controls: null
    },
    timers: {
      runtimeSync: 0,
      viewportRefresh: 0,
      healthCheck: 0
    },
    missingPlayerSince: 0
  };

  // ---------- 页面与播放器识别 ----------

  function getPageKind() {
    if (location.pathname === "/watch") return "watch";
    if (location.pathname.startsWith("/shorts/")) return "shorts";
    return "unsupported";
  }

  function getVisibleArea(element) {
    const rect = element.getBoundingClientRect();
    const width = Math.max(
      0,
      Math.min(rect.right, window.innerWidth) - Math.max(rect.left, 0)
    );
    const height = Math.max(
      0,
      Math.min(rect.bottom, window.innerHeight) - Math.max(rect.top, 0)
    );
    return width * height;
  }

  function isRenderedPlayer(player) {
    if (!(player instanceof HTMLElement) || !player.isConnected) return false;

    const rect = player.getBoundingClientRect();
    if (rect.width <= 1 || rect.height <= 1) return false;

    const style = getComputedStyle(player);
    return style.display !== "none"
      && style.visibility !== "hidden"
      && Number(style.opacity || 1) > 0
      && Boolean(player.querySelector("video.html5-main-video, video"));
  }

  function scorePlayer(player, pageKind) {
    if (!isRenderedPlayer(player)) return -Infinity;

    const rect = player.getBoundingClientRect();
    const video = player.querySelector("video.html5-main-video, video");
    let score = getVisibleArea(player);

    if (video) {
      if (!video.paused && !video.ended) score += 1_000_000_000;
      if (video.currentTime > 0) score += 10_000_000;
      if (video.readyState >= HTMLMediaElement.HAVE_CURRENT_DATA) score += 1_000_000;
    }

    if (pageKind === "watch") {
      const watchPage = player.closest("ytd-watch-flexy");
      if (watchPage && !watchPage.hasAttribute("hidden")) score += 100_000_000;
    }

    if (pageKind === "shorts") {
      const renderer = player.closest("ytd-reel-video-renderer");
      if (renderer?.matches("[is-active], [active]")) score += 500_000_000;
      if (renderer?.getAttribute("aria-hidden") === "false") score += 100_000_000;

      const centerX = rect.left + rect.width / 2;
      const centerY = rect.top + rect.height / 2;
      const distanceFromViewportCenter = Math.hypot(
        centerX - window.innerWidth / 2,
        centerY - window.innerHeight / 2
      );
      score -= distanceFromViewportCenter * 1_000;
    }

    return score;
  }

  function findBestPlayer() {
    const pageKind = getPageKind();
    if (pageKind === "unsupported") return null;

    // 网页全屏期间优先保留当前播放器，避免响应式重排时切换到备用播放器。
    if (state.viewport.active && isRenderedPlayer(state.viewport.player)) {
      return state.viewport.player;
    }

    const selector = pageKind === "watch"
      ? "ytd-watch-flexy:not([hidden]) #movie_player, ytd-player #movie_player, #movie_player"
      : "ytd-reel-video-renderer #movie_player, #shorts-container #movie_player, #movie_player";

    return Array.from(document.querySelectorAll(selector))
      .map((player) => ({ player, score: scorePlayer(player, pageKind) }))
      .filter(({ score }) => Number.isFinite(score))
      .sort((left, right) => right.score - left.score)[0]?.player ?? null;
  }

  function findRightControls(player) {
    return player?.querySelector(".ytp-right-controls") ?? null;
  }

  // ---------- 网页全屏布局 ----------

  function clearLegacyMarkers() {
    document.querySelectorAll(`[${LEGACY_SIBLING_ATTRIBUTE}]`).forEach((element) => {
      element.removeAttribute(LEGACY_SIBLING_ATTRIBUTE);
    });
  }

  function ensureBackdrop() {
    let backdrop = document.getElementById(BACKDROP_ID);
    if (!backdrop) {
      backdrop = document.createElement("div");
      backdrop.id = BACKDROP_ID;
      backdrop.setAttribute("aria-hidden", "true");
    }

    const host = document.body ?? document.documentElement;
    if (backdrop.parentElement !== host) host.appendChild(backdrop);
    return backdrop;
  }

  function getPlayerAncestorPath(player) {
    const path = [];
    let parent = player?.parentElement ?? null;

    while (parent && parent !== document.documentElement) {
      path.push(parent);
      parent = parent.parentElement;
    }

    return path;
  }

  function isViewportLayoutCurrent(player) {
    const path = getPlayerAncestorPath(player);
    return state.viewport.player === player
      && player?.getAttribute(PLAYER_ATTRIBUTE) === "true"
      && path.length === state.viewport.ancestors.length
      && path.every((element, index) => (
        element === state.viewport.ancestors[index]
        && element.getAttribute(ANCESTOR_ATTRIBUTE) === "true"
      ));
  }

  function clearViewportLayout({ removeBackdrop = true } = {}) {
    state.viewport.player?.removeAttribute(PLAYER_ATTRIBUTE);

    for (const element of state.viewport.ancestors) {
      element.removeAttribute(ANCESTOR_ATTRIBUTE);
    }
    state.viewport.ancestors = [];

    if (removeBackdrop) document.getElementById(BACKDROP_ID)?.remove();
  }

  function applyViewportLayout(player) {
    clearViewportLayout({ removeBackdrop: false });
    state.viewport.player = player;
    state.viewport.player.setAttribute(PLAYER_ATTRIBUTE, "true");

    state.viewport.ancestors = getPlayerAncestorPath(player);
    for (const element of state.viewport.ancestors) {
      element.setAttribute(ANCESTOR_ATTRIBUTE, "true");
    }

    ensureBackdrop();
  }

  function requestPlayerResize() {
    requestAnimationFrame(() => {
      window.dispatchEvent(new Event("resize"));
      requestAnimationFrame(() => window.dispatchEvent(new Event("resize")));
    });
  }

  // ---------- 播放器按钮 ----------

  function updateViewportButton() {
    const button = document.getElementById(BUTTON_ID);
    if (!button) return;

    button.innerHTML = state.viewport.active ? EXIT_ICON : ENTER_ICON;
    button.title = state.viewport.active ? "退出网页全屏（Esc）" : "网页全屏";
    button.setAttribute("aria-label", button.title);
    button.setAttribute("aria-pressed", String(state.viewport.active));
  }

  function syncButtonMetrics(button, nativeButton) {
    const rect = nativeButton.getBoundingClientRect();
    if (rect.width > 0) {
      button.style.setProperty("--yt-vfs-button-width", `${rect.width}px`);
    }
    if (rect.height > 0) {
      button.style.setProperty("--yt-vfs-button-height", `${rect.height}px`);
    }
  }

  function removeViewportButton() {
    document.getElementById(BUTTON_ID)?.remove();
  }

  function createViewportButton(nativeButton) {
    const button = document.createElement("button");
    const nativeClasses = Array.from(nativeButton.classList)
      .filter((className) => className !== "ytp-fullscreen-button");

    button.id = BUTTON_ID;
    button.type = "button";
    button.className = nativeClasses.join(" ");
    button.classList.add("ytp-button");
    button.addEventListener("click", (event) => {
      event.preventDefault();
      event.stopPropagation();
      toggleViewportFullscreen();
    });

    return button;
  }

  function ensureViewportButton(player) {
    const controls = findRightControls(player);
    const nativeButton = controls?.querySelector(".ytp-fullscreen-button") ?? null;
    const parent = nativeButton?.parentElement ?? null;
    if (!controls || !nativeButton || !parent || !parent.isConnected) return false;

    let button = document.getElementById(BUTTON_ID);
    if (!button || button.parentElement !== parent) {
      button?.remove();
      button = createViewportButton(nativeButton);
    }

    try {
      if (button.nextElementSibling !== nativeButton) {
        parent.insertBefore(button, nativeButton);
      }
    } catch (error) {
      console.warn("[YouTube 网页全屏] 播放器控件正在重建，稍后重试。", error);
      button.remove();
      scheduleRuntimeSync(TIMING.discoveryDelay);
      return false;
    }

    syncButtonMetrics(button, nativeButton);
    updateViewportButton();
    return true;
  }

  // ---------- DOM 观察器 ----------

  function stopDiscoveryObserver() {
    state.observers.discovery?.disconnect();
    state.observers.discovery = null;
  }

  function startDiscoveryObserver() {
    if (state.observers.discovery || getPageKind() === "unsupported") return;

    const root = document.body ?? document.documentElement;
    state.observers.discovery = new MutationObserver(() => {
      scheduleRuntimeSync(TIMING.discoveryDelay);
    });
    state.observers.discovery.observe(root, { childList: true, subtree: true });
  }

  function stopControlsObserver() {
    state.observers.controls?.disconnect();
    state.observers.controls = null;
    state.binding.player = null;
    state.binding.controlsRoot = null;
  }

  function observePlayerControls(player) {
    const controls = findRightControls(player);
    const controlsRoot = player.querySelector(".ytp-chrome-bottom") ?? controls;

    if (
      state.binding.player === player
      && state.binding.controlsRoot === controlsRoot
      && controlsRoot?.isConnected
    ) {
      ensureViewportButton(player);
      return;
    }

    stopControlsObserver();
    state.binding.player = player;
    state.binding.controlsRoot = controlsRoot;

    if (controlsRoot) {
      state.observers.controls = new MutationObserver(() => {
        scheduleRuntimeSync(TIMING.controlsDelay);
      });
      state.observers.controls.observe(controlsRoot, { childList: true, subtree: true });
    }

    ensureViewportButton(player);
  }

  // ---------- 运行状态同步 ----------

  function resetRuntime({ exitViewport = false, removeButton = false } = {}) {
    state.missingPlayerSince = 0;
    stopDiscoveryObserver();
    stopControlsObserver();

    if (exitViewport && state.viewport.active) exitViewportFullscreen();
    if (removeButton) removeViewportButton();
  }

  function syncRuntimeState() {
    if (getPageKind() === "unsupported") {
      resetRuntime({ exitViewport: true, removeButton: true });
      return;
    }

    const player = findBestPlayer();
    if (!player) {
      if (!state.missingPlayerSince) state.missingPlayerSince = Date.now();
      stopControlsObserver();
      removeViewportButton();
      startDiscoveryObserver();

      // YouTube 切片时播放器会短暂消失，留出重建时间后再退出网页全屏。
      if (
        state.viewport.active
        && Date.now() - state.missingPlayerSince > TIMING.missingPlayerGrace
      ) {
        exitViewportFullscreen();
      }
      return;
    }

    state.missingPlayerSince = 0;
    stopDiscoveryObserver();

    if (state.viewport.active && !isViewportLayoutCurrent(player)) {
      applyViewportLayout(player);
      requestPlayerResize();
    }

    observePlayerControls(player);
  }

  function scheduleRuntimeSync(delay = TIMING.popstateDelay) {
    window.clearTimeout(state.timers.runtimeSync);
    state.timers.runtimeSync = window.setTimeout(syncRuntimeState, delay);
  }

  function scheduleViewportLayoutRefresh() {
    window.clearTimeout(state.timers.viewportRefresh);
    state.timers.viewportRefresh = window.setTimeout(() => {
      const player = findBestPlayer();
      if (state.viewport.active && player) {
        applyViewportLayout(player);
        requestPlayerResize();
      }
      syncRuntimeState();
    }, TIMING.viewportDelay);
  }

  function startHealthCheck() {
    window.clearInterval(state.timers.healthCheck);
    state.timers.healthCheck = window.setInterval(() => {
      if (document.visibilityState === "visible") syncRuntimeState();
    }, TIMING.healthInterval);
  }

  function clearTimers() {
    window.clearTimeout(state.timers.runtimeSync);
    window.clearTimeout(state.timers.viewportRefresh);
    window.clearInterval(state.timers.healthCheck);
    state.timers.runtimeSync = 0;
    state.timers.viewportRefresh = 0;
    state.timers.healthCheck = 0;
  }

  // ---------- 网页全屏状态 ----------

  function enterViewportFullscreen() {
    const player = findBestPlayer();
    if (!player || state.viewport.active) return;

    state.viewport.savedScrollX = window.scrollX;
    state.viewport.savedScrollY = window.scrollY;
    state.viewport.savedFocus = document.activeElement instanceof HTMLElement
      ? document.activeElement
      : null;
    state.viewport.active = true;

    applyViewportLayout(player);
    document.documentElement.classList.add(ACTIVE_CLASS);
    document.documentElement.classList.toggle(
      NATIVE_CLASS,
      Boolean(document.fullscreenElement)
    );
    observePlayerControls(player);
    updateViewportButton();
    requestPlayerResize();
  }

  function exitViewportFullscreen() {
    if (!state.viewport.active) return;

    state.viewport.active = false;
    document.documentElement.classList.remove(ACTIVE_CLASS, NATIVE_CLASS);
    clearViewportLayout();
    state.viewport.player = null;
    updateViewportButton();
    requestPlayerResize();

    requestAnimationFrame(() => {
      requestAnimationFrame(() => {
        window.scrollTo(state.viewport.savedScrollX, state.viewport.savedScrollY);
        if (state.viewport.savedFocus?.isConnected) {
          state.viewport.savedFocus.focus({ preventScroll: true });
        }
        state.viewport.savedFocus = null;
      });
    });
  }

  function toggleViewportFullscreen() {
    if (state.viewport.active) {
      exitViewportFullscreen();
    } else {
      enterViewportFullscreen();
    }
  }

  function hasOpenPlayerPopup() {
    const player = state.viewport.player;
    if (!player) return false;

    return Array.from(
      player.querySelectorAll(".ytp-popup, .ytp-contextmenu")
    ).some((popup) => {
      const style = getComputedStyle(popup);
      const rect = popup.getBoundingClientRect();
      return style.display !== "none"
        && style.visibility !== "hidden"
        && Number(style.opacity || 1) > 0
        && rect.width > 0
        && rect.height > 0;
    });
  }

  function syncNativeFullscreenState() {
    document.documentElement.classList.toggle(
      NATIVE_CLASS,
      Boolean(document.fullscreenElement)
    );

    if (state.viewport.active && !document.fullscreenElement) {
      const player = findBestPlayer();
      if (player && !isViewportLayoutCurrent(player)) applyViewportLayout(player);
      requestPlayerResize();
    }

    updateViewportButton();
  }

  // ---------- 事件与生命周期 ----------

  function handleKeydown(event) {
    if (
      event.key !== "Escape"
      || !state.viewport.active
      || document.fullscreenElement
      || hasOpenPlayerPopup()
    ) {
      return;
    }

    event.preventDefault();
    event.stopPropagation();
    exitViewportFullscreen();
  }

  function handleNavigationStart() {
    stopDiscoveryObserver();
    stopControlsObserver();
    scheduleRuntimeSync(0);
  }

  function handleVisibilityChange() {
    if (document.visibilityState === "visible") scheduleRuntimeSync(0);
  }

  function handleWindowResize(event) {
    // requestPlayerResize() 派发的是合成事件，只处理用户真实调整窗口产生的 resize。
    if (event.isTrusted) scheduleViewportLayoutRefresh();
  }

  function handlePageHide() {
    clearTimers();
    stopDiscoveryObserver();
    stopControlsObserver();
    if (state.viewport.active) exitViewportFullscreen();
    removeViewportButton();
    clearLegacyMarkers();
  }

  function registerEventListeners() {
    document.addEventListener("keydown", handleKeydown, true);
    document.addEventListener("fullscreenchange", syncNativeFullscreenState);
    document.addEventListener("yt-navigate-start", handleNavigationStart);
    document.addEventListener("yt-navigate-finish", () => {
      scheduleRuntimeSync(TIMING.navigationDelay);
    });
    document.addEventListener("visibilitychange", handleVisibilityChange);

    window.addEventListener("resize", handleWindowResize);
    window.addEventListener("popstate", () => {
      scheduleRuntimeSync(TIMING.popstateDelay);
    });
    window.addEventListener("pagehide", handlePageHide, { once: true });
  }

  // ---------- 启动 ----------

  function initialize() {
    clearLegacyMarkers();
    registerEventListeners();
    startHealthCheck();
    syncRuntimeState();
  }

  initialize();
})();