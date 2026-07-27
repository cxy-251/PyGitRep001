(() => {
  "use strict";

  const BUTTON_ID = "yt-webpage-fullscreen-button";
  const ACTIVE_CLASS = "yt-webpage-fullscreen-active";
  const NATIVE_CLASS = "yt-webpage-native-fullscreen";
  const PLAYER_ATTRIBUTE = "data-yt-webpage-fullscreen";

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
  let savedScrollX = 0;
  let savedScrollY = 0;
  let reconcileTimer = 0;

  function isSupportedPage() {
    return location.pathname === "/watch" || location.pathname.startsWith("/shorts/");
  }

  function findPlayer() {
    const players = Array.from(document.querySelectorAll("#movie_player.html5-video-player, #movie_player"));
    return players.find((player) => {
      const rect = player.getBoundingClientRect();
      return rect.width > 0 && rect.height > 0;
    }) ?? players[0] ?? null;
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

  function ensureButton() {
    if (!isSupportedPage()) {
      if (active) exitViewportFullscreen();
      removeButton();
      return;
    }

    const player = findPlayer();
    const controls = findControls(player);
    if (!player || !controls || !controls.isConnected) return;

    if (active && currentPlayer !== player) {
      currentPlayer?.removeAttribute(PLAYER_ATTRIBUTE);
      currentPlayer = player;
      currentPlayer.setAttribute(PLAYER_ATTRIBUTE, "true");
      notifyPlayerResize();
    }

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
        scheduleReconcile(100);
        return;
      }
    }

    updateButtonState();
  }

  function enterViewportFullscreen() {
    const player = findPlayer();
    if (!player || active) return;

    savedScrollX = window.scrollX;
    savedScrollY = window.scrollY;
    currentPlayer = player;
    active = true;

    document.documentElement.classList.add(ACTIVE_CLASS);
    currentPlayer.setAttribute(PLAYER_ATTRIBUTE, "true");
    updateButtonState();
    notifyPlayerResize();
  }

  function exitViewportFullscreen() {
    if (!active) return;

    active = false;
    document.documentElement.classList.remove(ACTIVE_CLASS, NATIVE_CLASS);
    currentPlayer?.removeAttribute(PLAYER_ATTRIBUTE);
    currentPlayer = null;
    updateButtonState();
    notifyPlayerResize();

    requestAnimationFrame(() => {
      requestAnimationFrame(() => window.scrollTo(savedScrollX, savedScrollY));
    });
  }

  function toggleViewportFullscreen() {
    if (active) {
      exitViewportFullscreen();
    } else {
      enterViewportFullscreen();
    }
  }

  function syncNativeFullscreenState() {
    document.documentElement.classList.toggle(NATIVE_CLASS, Boolean(document.fullscreenElement));
    if (active && !document.fullscreenElement) notifyPlayerResize();
    updateButtonState();
  }

  function scheduleReconcile(delay = 100) {
    window.clearTimeout(reconcileTimer);
    reconcileTimer = window.setTimeout(ensureButton, delay);
  }

  document.addEventListener(
    "keydown",
    (event) => {
      if (event.key !== "Escape" || !active || document.fullscreenElement) return;
      event.preventDefault();
      event.stopPropagation();
      exitViewportFullscreen();
    },
    true
  );

  document.addEventListener("fullscreenchange", syncNativeFullscreenState);
  document.addEventListener("yt-navigate-start", () => scheduleReconcile(0));
  document.addEventListener("yt-navigate-finish", () => scheduleReconcile(150));
  window.addEventListener("popstate", () => scheduleReconcile(100));

  const observer = new MutationObserver(() => scheduleReconcile(80));
  observer.observe(document.documentElement, { childList: true, subtree: true });

  window.setInterval(ensureButton, 1000);
  ensureButton();
})();