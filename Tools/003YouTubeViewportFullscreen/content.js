(() => {
  "use strict";

  const BUTTON_ID = "yt-webpage-fullscreen-button";
  const ACTIVE_CLASS = "yt-webpage-fullscreen-active";
  const NATIVE_CLASS = "yt-webpage-native-fullscreen";
  const PLAYER_ATTRIBUTE = "data-yt-webpage-fullscreen-player";
  const ANCESTOR_ATTRIBUTE = "data-yt-webpage-fullscreen-ancestor";
  const SIBLING_ATTRIBUTE = "data-yt-webpage-fullscreen-sibling";

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
  let savedScrollX = 0;
  let savedScrollY = 0;
  let savedFocus = null;
  let reconcileTimer = 0;
  let discoveryObserver = null;
  let playerObserver = null;
  let healthTimer = 0;
  let markedAncestors = [];
  let hiddenSiblings = [];

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

  function scorePlayer(player, pageKind) {
    if (!(player instanceof HTMLElement) || !player.isConnected) return -Infinity;

    const rect = player.getBoundingClientRect();
    if (rect.width <= 0 || rect.height <= 0) return -Infinity;

    const video = player.querySelector("video.html5-main-video, video");
    let score = visibleArea(player);

    if (video) {
      if (!video.paused && !video.ended) score += 1_000_000_000;
      if (video.currentTime > 0) score += 10_000_000;
      if (video.readyState >= HTMLMediaElement.HAVE_CURRENT_DATA) score += 1_000_000;
    }

    if (pageKind === "shorts") {
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

    const selector = pageKind === "watch"
      ? "ytd-watch-flexy #movie_player, ytd-player #movie_player, #movie_player"
      : "ytd-reel-video-renderer #movie_player, #shorts-container #movie_player, #movie_player";

    return Array.from(document.querySelectorAll(selector))
      .map((player) => ({ player, score: scorePlayer(player, pageKind) }))
      .sort((left, right) => right.score - left.score)[0]?.player ?? null;
  }

  function findControls(player) {
    return player?.querySelector(".ytp-right-controls") ?? null;
  }

  function findDirectChildAnchor(parent, descendant) {
    if (!descendant || !parent.contains(descendant)) return null;

    let anchor = descendant;
    while (anchor.parentElement && anchor.parentElement !== parent) {
      anchor = anchor.parentElement;
    }

    return anchor.parentElement === parent ? anchor : null;
  }

  function isHideableSibling(element) {
    return !["SCRIPT", "STYLE", "LINK", "META"].includes(element.tagName);
  }

  function clearLayoutMarkers() {
    currentPlayer?.removeAttribute(PLAYER_ATTRIBUTE);

    for (const element of markedAncestors) {
      element.removeAttribute(ANCESTOR_ATTRIBUTE);
    }

    for (const element of hiddenSiblings) {
      element.removeAttribute(SIBLING_ATTRIBUTE);
    }

    markedAncestors = [];
    hiddenSiblings = [];
  }

  function applyLayoutMarkers(player) {
    clearLayoutMarkers();
    currentPlayer = player;
    currentPlayer.setAttribute(PLAYER_ATTRIBUTE, "true");

    let branch = player;
    let parent = player.parentElement;

    while (parent && parent !== document.documentElement) {
      parent.setAttribute(ANCESTOR_ATTRIBUTE, "true");
      markedAncestors.push(parent);

      for (const child of parent.children) {
        if (child === branch || !isHideableSibling(child)) continue;
        child.setAttribute(SIBLING_ATTRIBUTE, "true");
        hiddenSiblings.push(child);
      }

      branch = parent;
      parent = parent.parentElement;
    }
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

  function removeButton() {
    document.getElementById(BUTTON_ID)?.remove();
  }

  function insertButton(controls, button) {
    const nativeFullscreenButton = controls.querySelector(".ytp-fullscreen-button");
    const anchor = findDirectChildAnchor(controls, nativeFullscreenButton);

    if (anchor?.parentNode === controls) {
      controls.insertBefore(button, anchor);
      return;
    }

    controls.appendChild(button);
  }

  function ensureButtonForPlayer(player) {
    const controls = findControls(player);
    if (!controls || !controls.isConnected) return false;

    let button = document.getElementById(BUTTON_ID);
    if (!button || !controls.contains(button)) {
      button?.remove();
      button = document.createElement("button");
      button.id = BUTTON_ID;
      button.type = "button";
      button.className = "ytp-button";
      button.addEventListener("click", (event) => {
        event.preventDefault();
        event.stopPropagation();
        toggleViewportFullscreen();
      });

      try {
        insertButton(controls, button);
      } catch (error) {
        console.warn("[YouTube 网页全屏] 播放器控件正在重建，稍后重试。", error);
        button.remove();
        scheduleReconcile(120);
        return false;
      }
    }

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
    if (boundPlayer === player && player.isConnected) {
      ensureButtonForPlayer(player);
      return;
    }

    playerObserver?.disconnect();
    boundPlayer = player;

    playerObserver = new MutationObserver(() => scheduleReconcile(80));
    playerObserver.observe(player, { childList: true, subtree: true });

    if (active && currentPlayer !== player) {
      applyLayoutMarkers(player);
      notifyPlayerResize();
    }

    ensureButtonForPlayer(player);
  }

  function clearPlayerBinding() {
    playerObserver?.disconnect();
    playerObserver = null;
    boundPlayer = null;
  }

  function reconcile() {
    if (getPageKind() === "unsupported") {
      stopDiscoveryObserver();
      clearPlayerBinding();
      removeButton();
      if (active) exitViewportFullscreen();
      return;
    }

    const player = findPlayer();
    if (!player) {
      clearPlayerBinding();
      removeButton();
      startDiscoveryObserver();
      return;
    }

    stopDiscoveryObserver();
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
    if (active && !document.fullscreenElement) notifyPlayerResize();
    updateButtonState();
  }

  function scheduleReconcile(delay = 100) {
    window.clearTimeout(reconcileTimer);
    reconcileTimer = window.setTimeout(reconcile, delay);
  }

  function startHealthCheck() {
    window.clearInterval(healthTimer);
    healthTimer = window.setInterval(() => {
      if (document.visibilityState === "visible") reconcile();
    }, 3_000);
  }

  document.addEventListener(
    "keydown",
    (event) => {
      if (
        event.key !== "Escape" ||
        !active ||
        document.fullscreenElement ||
        hasOpenPlayerPopup()
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
  window.addEventListener("popstate", () => scheduleReconcile(100));

  window.addEventListener(
    "pagehide",
    () => {
      window.clearTimeout(reconcileTimer);
      window.clearInterval(healthTimer);
      stopDiscoveryObserver();
      clearPlayerBinding();
      if (active) exitViewportFullscreen();
      removeButton();
    },
    { once: true }
  );

  startHealthCheck();
  reconcile();
})();
