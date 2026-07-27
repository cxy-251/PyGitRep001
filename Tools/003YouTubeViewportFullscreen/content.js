(() => {
  "use strict";

  const BUTTON_ID = "yt-webpage-fullscreen-button";
  const BACKDROP_ID = "yt-webpage-fullscreen-backdrop";
  const ACTIVE_CLASS = "yt-webpage-fullscreen-active";
  const NATIVE_CLASS = "yt-webpage-native-fullscreen";
  const PLAYER_ATTRIBUTE = "data-yt-webpage-fullscreen-player";
  const ANCESTOR_ATTRIBUTE = "data-yt-webpage-fullscreen-ancestor";
  const LEGACY_SIBLING_ATTRIBUTE = "data-yt-webpage-fullscreen-sibling";

  const ENTER_ICON = `
    <svg viewBox="0 0 36 36" aria-hidden="true" focusable="false">
      <path d="M7 7h9v3h-6v6H7V7Zm13 0h9v9h-3v-6h-6V7ZM7 20h3v6h6v3H7v-9Zm19 0h3v9h-9v-3h6v-6Z"/>
    </svg>`;

  const EXIT_ICON = `
    <svg viewBox="0 0 36 36" aria-hidden="true" focusable="false">
      <path d="M7 15v-3h5V7h3v8H7Zm14 0V7h3v5h5v3h-8ZM7 21h8v8h-3v-5H7v-3Zm14 0h8v3h-5v5h-3v-8Z"/>
    </svg>`;

  let active = false;
  let currentPlayer = null;
  let boundPlayer = null;
  let boundControlsRoot = null;
  let savedScrollX = 0;
  let savedScrollY = 0;
  let savedFocus = null;
  let reconcileTimer = 0;
  let resizeTimer = 0;
  let healthTimer = 0;
  let missingPlayerSince = 0;
  let discoveryObserver = null;
  let controlsObserver = null;
  let markedAncestors = [];

  function getPageKind() {
    if (location.pathname === "/watch") return "watch";
    if (location.pathname.startsWith("/shorts/")) return "shorts";
    return "unsupported";
  }

  function visibleArea(element) {
    const rect = element.getBoundingClientRect();
    const width = Math.max(0, Math.min(rect.right, window.innerWidth) - Math.max(rect.left, 0));
    const height = Math.max(0, Math.min(rect.bottom, window.innerHeight) - Math.max(rect.top, 0));
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
    let score = visibleArea(player);

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
      const distance = Math.hypot(
        centerX - window.innerWidth / 2,
        centerY - window.innerHeight / 2
      );
      score -= distance * 1_000;
    }

    return score;
  }

  function findPlayer() {
    const pageKind = getPageKind();
    if (pageKind === "unsupported") return null;

    if (active && isRenderedPlayer(currentPlayer)) {
      return currentPlayer;
    }

    const selector = pageKind === "watch"
      ? "ytd-watch-flexy:not([hidden]) #movie_player, ytd-player #movie_player, #movie_player"
      : "ytd-reel-video-renderer #movie_player, #shorts-container #movie_player, #movie_player";

    return Array.from(document.querySelectorAll(selector))
      .map((player) => ({ player, score: scorePlayer(player, pageKind) }))
      .filter(({ score }) => Number.isFinite(score))
      .sort((left, right) => right.score - left.score)[0]?.player ?? null;
  }

  function findControls(player) {
    return player?.querySelector(".ytp-right-controls") ?? null;
  }

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

  function clearLayoutMarkers({ removeBackdrop = true } = {}) {
    currentPlayer?.removeAttribute(PLAYER_ATTRIBUTE);

    for (const element of markedAncestors) {
      element.removeAttribute(ANCESTOR_ATTRIBUTE);
    }
    markedAncestors = [];

    if (removeBackdrop) document.getElementById(BACKDROP_ID)?.remove();
  }

  function getAncestorPath(player) {
    const path = [];
    let parent = player?.parentElement ?? null;

    while (parent && parent !== document.documentElement) {
      path.push(parent);
      parent = parent.parentElement;
    }

    return path;
  }

  function layoutPathIsCurrent(player) {
    const path = getAncestorPath(player);
    return currentPlayer === player
      && player?.getAttribute(PLAYER_ATTRIBUTE) === "true"
      && path.length === markedAncestors.length
      && path.every((element, index) => (
        element === markedAncestors[index]
        && element.getAttribute(ANCESTOR_ATTRIBUTE) === "true"
      ));
  }

  function applyLayoutMarkers(player) {
    clearLayoutMarkers({ removeBackdrop: false });
    currentPlayer = player;
    currentPlayer.setAttribute(PLAYER_ATTRIBUTE, "true");

    markedAncestors = getAncestorPath(player);
    for (const element of markedAncestors) {
      element.setAttribute(ANCESTOR_ATTRIBUTE, "true");
    }

    ensureBackdrop();
  }

  function notifyPlayerResize() {
    requestAnimationFrame(() => {
      window.dispatchEvent(new Event("resize"));
      requestAnimationFrame(() => window.dispatchEvent(new Event("resize")));
    });
  }

  function updateButtonState() {
    const button = document.getElementById(BUTTON_ID);
    if (!button) return;

    button.innerHTML = active ? EXIT_ICON : ENTER_ICON;
    button.title = active ? "退出网页全屏（Esc）" : "网页全屏";
    button.setAttribute("aria-label", button.title);
    button.setAttribute("aria-pressed", String(active));
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

  function removeButton() {
    document.getElementById(BUTTON_ID)?.remove();
  }

  function createButton(nativeButton) {
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

  function ensureButtonForPlayer(player) {
    const controls = findControls(player);
    const nativeButton = controls?.querySelector(".ytp-fullscreen-button") ?? null;
    const parent = nativeButton?.parentElement ?? null;
    if (!controls || !nativeButton || !parent || !parent.isConnected) return false;

    let button = document.getElementById(BUTTON_ID);
    if (!button || button.parentElement !== parent) {
      button?.remove();
      button = createButton(nativeButton);
    }

    try {
      if (button.nextElementSibling !== nativeButton) {
        parent.insertBefore(button, nativeButton);
      }
    } catch (error) {
      console.warn("[YouTube 网页全屏] 播放器控件正在重建，稍后重试。", error);
      button.remove();
      scheduleReconcile(120);
      return false;
    }

    syncButtonMetrics(button, nativeButton);
    updateButtonState();
    return true;
  }

  function stopDiscoveryObserver() {
    discoveryObserver?.disconnect();
    discoveryObserver = null;
  }

  function startDiscoveryObserver() {
    if (discoveryObserver || getPageKind() === "unsupported") return;

    const root = document.body ?? document.documentElement;
    discoveryObserver = new MutationObserver(() => scheduleReconcile(120));
    discoveryObserver.observe(root, { childList: true, subtree: true });
  }

  function bindPlayer(player) {
    const controls = findControls(player);
    const controlsRoot = player.querySelector(".ytp-chrome-bottom") ?? controls;

    if (
      boundPlayer === player
      && boundControlsRoot === controlsRoot
      && controlsRoot?.isConnected
    ) {
      ensureButtonForPlayer(player);
      return;
    }

    controlsObserver?.disconnect();
    controlsObserver = null;
    boundPlayer = player;
    boundControlsRoot = controlsRoot;

    if (controlsRoot) {
      controlsObserver = new MutationObserver(() => scheduleReconcile(80));
      controlsObserver.observe(controlsRoot, { childList: true, subtree: true });
    }

    ensureButtonForPlayer(player);
  }

  function clearPlayerBinding() {
    controlsObserver?.disconnect();
    controlsObserver = null;
    boundPlayer = null;
    boundControlsRoot = null;
  }

  function reconcile() {
    if (getPageKind() === "unsupported") {
      missingPlayerSince = 0;
      stopDiscoveryObserver();
      clearPlayerBinding();
      removeButton();
      if (active) exitViewportFullscreen();
      return;
    }

    const player = findPlayer();
    if (!player) {
      if (!missingPlayerSince) missingPlayerSince = Date.now();
      clearPlayerBinding();
      removeButton();
      startDiscoveryObserver();

      if (active && Date.now() - missingPlayerSince > 1_500) {
        exitViewportFullscreen();
      }
      return;
    }

    missingPlayerSince = 0;
    stopDiscoveryObserver();

    if (active && !layoutPathIsCurrent(player)) {
      applyLayoutMarkers(player);
      notifyPlayerResize();
    }

    bindPlayer(player);
  }

  function enterViewportFullscreen() {
    const player = findPlayer();
    if (!player || active) return;

    savedScrollX = window.scrollX;
    savedScrollY = window.scrollY;
    savedFocus = document.activeElement instanceof HTMLElement ? document.activeElement : null;
    active = true;

    applyLayoutMarkers(player);
    document.documentElement.classList.add(ACTIVE_CLASS);
    document.documentElement.classList.toggle(NATIVE_CLASS, Boolean(document.fullscreenElement));
    bindPlayer(player);
    updateButtonState();
    notifyPlayerResize();
  }

  function exitViewportFullscreen() {
    if (!active) return;

    active = false;
    document.documentElement.classList.remove(ACTIVE_CLASS, NATIVE_CLASS);
    clearLayoutMarkers();
    currentPlayer = null;
    updateButtonState();
    notifyPlayerResize();

    requestAnimationFrame(() => {
      requestAnimationFrame(() => {
        window.scrollTo(savedScrollX, savedScrollY);
        if (savedFocus?.isConnected) {
          savedFocus.focus({ preventScroll: true });
        }
        savedFocus = null;
      });
    });
  }

  function toggleViewportFullscreen() {
    if (active) {
      exitViewportFullscreen();
    } else {
      enterViewportFullscreen();
    }
  }

  function hasOpenPlayerPopup() {
    if (!currentPlayer) return false;

    return Array.from(
      currentPlayer.querySelectorAll(".ytp-popup, .ytp-contextmenu")
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
    document.documentElement.classList.toggle(NATIVE_CLASS, Boolean(document.fullscreenElement));

    if (active && !document.fullscreenElement) {
      const player = findPlayer();
      if (player && !layoutPathIsCurrent(player)) applyLayoutMarkers(player);
      notifyPlayerResize();
    }

    updateButtonState();
  }

  function scheduleReconcile(delay = 100) {
    window.clearTimeout(reconcileTimer);
    reconcileTimer = window.setTimeout(reconcile, delay);
  }

  function scheduleViewportRefresh() {
    window.clearTimeout(resizeTimer);
    resizeTimer = window.setTimeout(() => {
      const player = findPlayer();
      if (active && player) {
        applyLayoutMarkers(player);
        notifyPlayerResize();
      }
      reconcile();
    }, 100);
  }

  function startHealthCheck() {
    window.clearInterval(healthTimer);
    healthTimer = window.setInterval(() => {
      if (document.visibilityState === "visible") reconcile();
    }, 5_000);
  }

  document.addEventListener(
    "keydown",
    (event) => {
      if (
        event.key !== "Escape"
        || !active
        || document.fullscreenElement
        || hasOpenPlayerPopup()
      ) {
        return;
      }

      event.preventDefault();
      event.stopPropagation();
      exitViewportFullscreen();
    },
    true
  );

  document.addEventListener("fullscreenchange", syncNativeFullscreenState);
  document.addEventListener("yt-navigate-start", () => {
    stopDiscoveryObserver();
    clearPlayerBinding();
    scheduleReconcile(0);
  });
  document.addEventListener("yt-navigate-finish", () => scheduleReconcile(150));
  document.addEventListener("visibilitychange", () => {
    if (document.visibilityState === "visible") scheduleReconcile(0);
  });

  window.addEventListener("resize", (event) => {
    if (event.isTrusted) scheduleViewportRefresh();
  });
  window.addEventListener("popstate", () => scheduleReconcile(100));
  window.addEventListener(
    "pagehide",
    () => {
      window.clearTimeout(reconcileTimer);
      window.clearTimeout(resizeTimer);
      window.clearInterval(healthTimer);
      stopDiscoveryObserver();
      clearPlayerBinding();
      if (active) exitViewportFullscreen();
      removeButton();
      clearLegacyMarkers();
    },
    { once: true }
  );

  clearLegacyMarkers();
  startHealthCheck();
  reconcile();
})();