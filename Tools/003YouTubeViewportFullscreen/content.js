(() => {
  "use strict";

  const BUTTON_CLASS = "yt-vfs-player-button";
  const ENTER_ICON = `
    <svg viewBox="0 0 36 36" aria-hidden="true" focusable="false">
      <path d="M7 7h9v3h-6v6H7V7Zm13 0h9v9h-3v-6h-6V7ZM7 20h3v6h6v3H7v-9Zm19 0h3v9h-9v-3h6v-6Z"/>
    </svg>`;
  const EXIT_ICON = `
    <svg viewBox="0 0 36 36" aria-hidden="true" focusable="false">
      <path d="M7 15v-3h5V7h3v8H7Zm14 0V7h3v5h5v3h-8ZM7 21h8v8h-3v-5H7v-3Zm14 0h8v3h-5v5h-3v-8Z"/>
    </svg>`;

  function isElementVisible(element) {
    const rect = element.getBoundingClientRect();
    const style = getComputedStyle(element);
    return rect.width > 80
      && rect.height > 80
      && style.display !== "none"
      && style.visibility !== "hidden"
      && rect.bottom > 0
      && rect.top < window.innerHeight;
  }

  function visibleArea(element) {
    const rect = element.getBoundingClientRect();
    const width = Math.max(0, Math.min(rect.right, window.innerWidth) - Math.max(rect.left, 0));
    const height = Math.max(0, Math.min(rect.bottom, window.innerHeight) - Math.max(rect.top, 0));
    return width * height;
  }

  function scoreVideo(video) {
    if (!isElementVisible(video)) return -1;
    let score = visibleArea(video);
    if (!video.paused && !video.ended) score += 10_000_000;
    if (video.currentTime > 0) score += 1_000_000;
    if (video.readyState >= HTMLMediaElement.HAVE_CURRENT_DATA) score += 100_000;
    return score;
  }

  class YouTubeAdapter {
    constructor() {
      this.buttonHandler = null;
    }

    getPageKind() {
      if (location.pathname === "/watch") return "watch";
      if (location.pathname.startsWith("/shorts/")) return "shorts";
      return "unsupported";
    }

    getRouteKey() {
      const pageKind = this.getPageKind();
      if (pageKind === "watch") {
        return `watch:${new URL(location.href).searchParams.get("v") ?? location.search}`;
      }
      if (pageKind === "shorts") {
        return `shorts:${location.pathname.split("/")[2] ?? location.pathname}`;
      }
      return `${location.pathname}${location.search}`;
    }

    getPlayerContext() {
      const rankedVideos = Array.from(document.querySelectorAll("video.html5-main-video, video"))
        .map((video) => ({ video, score: scoreVideo(video) }))
        .filter(({ score }) => score >= 0)
        .sort((left, right) => right.score - left.score);

      for (const { video } of rankedVideos) {
        const playerRoot = video.closest("#movie_player") ?? video.parentElement;
        if (!playerRoot) continue;
        const target = video.closest("ytd-player")
          ?? video.closest("ytd-reel-video-renderer")
          ?? playerRoot;
        return { target, playerRoot, video };
      }
      return null;
    }

    async waitForPlayer(timeoutMs = 8_000) {
      const immediate = this.getPlayerContext();
      if (immediate) return immediate;

      return await new Promise((resolve) => {
        let settled = false;
        const finish = (value) => {
          if (settled) return;
          settled = true;
          observer.disconnect();
          window.clearInterval(intervalId);
          window.clearTimeout(timeoutId);
          resolve(value);
        };
        const check = () => {
          const context = this.getPlayerContext();
          if (context) finish(context);
        };
        const observer = new MutationObserver(check);
        observer.observe(document.documentElement, { childList: true, subtree: true });
        const intervalId = window.setInterval(check, 250);
        const timeoutId = window.setTimeout(() => finish(null), timeoutMs);
        check();
      });
    }

    subscribePageChanges(listener) {
      let lastRouteKey = this.getRouteKey();
      let mutationTimer = 0;
      const emit = () => {
        lastRouteKey = this.getRouteKey();
        listener();
      };
      const onNavigation = () => emit();

      document.addEventListener("yt-navigate-start", onNavigation);
      document.addEventListener("yt-navigate-finish", onNavigation);
      window.addEventListener("popstate", onNavigation);

      const isPlayerSurface = (node) => {
        if (!(node instanceof Element)) return false;
        return node.matches("ytd-player, #movie_player, .ytp-right-controls, .ytp-fullscreen-button, ytd-reel-video-renderer")
          || Boolean(node.querySelector("ytd-player, #movie_player, .ytp-right-controls, .ytp-fullscreen-button"));
      };

      const observer = new MutationObserver((mutations) => {
        const relevant = mutations.some((mutation) =>
          [...mutation.addedNodes, ...mutation.removedNodes].some(isPlayerSurface)
        );
        if (!relevant) return;
        window.clearTimeout(mutationTimer);
        mutationTimer = window.setTimeout(listener, 160);
      });
      observer.observe(document.documentElement, { childList: true, subtree: true });

      const routePoller = window.setInterval(() => {
        const currentRouteKey = this.getRouteKey();
        if (currentRouteKey !== lastRouteKey) emit();
      }, 1_000);

      return () => {
        document.removeEventListener("yt-navigate-start", onNavigation);
        document.removeEventListener("yt-navigate-finish", onNavigation);
        window.removeEventListener("popstate", onNavigation);
        observer.disconnect();
        window.clearInterval(routePoller);
        window.clearTimeout(mutationTimer);
      };
    }

    syncPlayerButton(active, onToggle) {
      const context = this.getPlayerContext();
      if (!context) {
        this.removePlayerButtons();
        return;
      }

      const controls = context.playerRoot.querySelector(".ytp-right-controls");
      if (!controls) return;

      let button = controls.querySelector(`.${BUTTON_CLASS}`);
      if (!button) {
        button = document.createElement("button");
        button.type = "button";
        button.className = `ytp-button ${BUTTON_CLASS}`;
        button.addEventListener("click", (event) => {
          event.preventDefault();
          event.stopImmediatePropagation();
          this.buttonHandler?.();
        });
        const nativeFullscreenButton = controls.querySelector(".ytp-fullscreen-button");
        controls.insertBefore(button, nativeFullscreenButton);
      }

      this.buttonHandler = onToggle;
      button.innerHTML = active ? EXIT_ICON : ENTER_ICON;
      button.title = active ? "退出网页全屏（Esc）" : "网页全屏";
      button.setAttribute("aria-label", button.title);
      button.setAttribute("aria-pressed", String(active));
    }

    removePlayerButtons() {
      document.querySelectorAll(`.${BUTTON_CLASS}`).forEach((button) => button.remove());
      this.buttonHandler = null;
    }
  }

  class LayoutController {
    constructor() {
      this.target = null;
      this.ancestors = [];
      this.snapshot = null;
    }

    enter(target) {
      if (!this.snapshot) {
        this.snapshot = {
          scrollX: window.scrollX,
          scrollY: window.scrollY,
          focusedElement: document.activeElement instanceof HTMLElement ? document.activeElement : null
        };
      }
      this.bindTarget(target);
      document.documentElement.classList.add("yt-vfs-active");
      document.documentElement.classList.remove("yt-vfs-native-fullscreen");
      this.notifyResize();
    }

    rebind(target) {
      if (this.target === target) return;
      this.clearTargetMarkers();
      this.bindTarget(target);
      this.notifyResize();
    }

    exit() {
      document.documentElement.classList.remove("yt-vfs-active", "yt-vfs-native-fullscreen");
      this.clearTargetMarkers();
      this.notifyResize();
      const snapshot = this.snapshot;
      this.snapshot = null;
      if (!snapshot) return;

      requestAnimationFrame(() => {
        requestAnimationFrame(() => {
          window.scrollTo(snapshot.scrollX, snapshot.scrollY);
          if (snapshot.focusedElement?.isConnected) {
            snapshot.focusedElement.focus({ preventScroll: true });
          }
        });
      });
    }

    suspendForNativeFullscreen() {
      document.documentElement.classList.add("yt-vfs-native-fullscreen");
      this.notifyResize();
    }

    resumeFromNativeFullscreen() {
      document.documentElement.classList.remove("yt-vfs-native-fullscreen");
      this.notifyResize();
    }

    bindTarget(target) {
      this.target = target;
      target.dataset.ytVfsTarget = "true";
      this.ancestors = [];
      let ancestor = target.parentElement;
      while (ancestor && ancestor !== document.body && ancestor !== document.documentElement) {
        ancestor.dataset.ytVfsAncestor = "true";
        this.ancestors.push(ancestor);
        ancestor = ancestor.parentElement;
      }
    }

    clearTargetMarkers() {
      this.target?.removeAttribute("data-yt-vfs-target");
      for (const ancestor of this.ancestors) {
        ancestor.removeAttribute("data-yt-vfs-ancestor");
      }
      this.target = null;
      this.ancestors = [];
    }

    notifyResize() {
      requestAnimationFrame(() => {
        window.dispatchEvent(new Event("resize"));
        requestAnimationFrame(() => window.dispatchEvent(new Event("resize")));
      });
    }
  }

  class StateController {
    constructor(layout, onChange) {
      this.layout = layout;
      this.onChange = onChange;
      this.state = "idle";
      this.operationId = 0;
    }

    get isActive() {
      return this.state === "entering" || this.state === "active" || this.state === "native-fullscreen";
    }

    async activate(target) {
      const operationId = ++this.operationId;
      if (this.isActive) {
        this.layout.rebind(target);
        if (this.state !== "native-fullscreen") this.setState("active");
        return;
      }
      this.setState("entering");
      this.layout.enter(target);
      await new Promise((resolve) => requestAnimationFrame(resolve));
      if (operationId === this.operationId) this.setState("active");
    }

    async deactivate() {
      const operationId = ++this.operationId;
      if (this.state === "idle" || this.state === "exiting") return;
      this.setState("exiting");
      this.layout.exit();
      await new Promise((resolve) => requestAnimationFrame(resolve));
      if (operationId === this.operationId) this.setState("idle");
    }

    rebind(target) {
      if (this.isActive) this.layout.rebind(target);
    }

    handleNativeFullscreen(isFullscreen) {
      if (isFullscreen && this.isActive) {
        this.layout.suspendForNativeFullscreen();
        this.setState("native-fullscreen");
        return;
      }
      if (!isFullscreen && this.state === "native-fullscreen") {
        this.layout.resumeFromNativeFullscreen();
        this.setState("active");
      }
    }

    setState(state) {
      if (this.state === state) return;
      this.state = state;
      this.onChange(state);
    }
  }

  class InputController {
    constructor(isActive, onExit) {
      this.isActive = isActive;
      this.onExit = onExit;
      this.onKeyDown = (event) => {
        if (event.key !== "Escape" || !this.isActive() || document.fullscreenElement) return;
        event.preventDefault();
        event.stopImmediatePropagation();
        this.onExit();
      };
    }

    start() {
      document.addEventListener("keydown", this.onKeyDown, true);
    }

    stop() {
      document.removeEventListener("keydown", this.onKeyDown, true);
    }
  }

  class ExtensionApp {
    constructor() {
      this.adapter = new YouTubeAdapter();
      this.layout = new LayoutController();
      this.state = new StateController(this.layout, () => this.syncButton());
      this.input = new InputController(() => this.state.isActive, () => void this.deactivate());
      this.unsubscribePageChanges = null;
      this.reconcileTimer = 0;
      this.reconcileGeneration = 0;
      this.onFullscreenChange = () => {
        this.state.handleNativeFullscreen(Boolean(document.fullscreenElement));
        this.syncButton();
      };
      this.onResize = () => {
        if (this.state.isActive) this.scheduleReconcile();
      };
    }

    async start() {
      this.input.start();
      this.unsubscribePageChanges = this.adapter.subscribePageChanges(() => this.scheduleReconcile());
      document.addEventListener("fullscreenchange", this.onFullscreenChange);
      window.addEventListener("resize", this.onResize, { passive: true });
      await this.reconcile();
    }

    stop() {
      this.input.stop();
      this.unsubscribePageChanges?.();
      document.removeEventListener("fullscreenchange", this.onFullscreenChange);
      window.removeEventListener("resize", this.onResize);
      this.adapter.removePlayerButtons();
      void this.state.deactivate();
    }

    scheduleReconcile() {
      window.clearTimeout(this.reconcileTimer);
      this.reconcileTimer = window.setTimeout(() => void this.reconcile(), 180);
    }

    async reconcile() {
      const generation = ++this.reconcileGeneration;
      if (this.adapter.getPageKind() === "unsupported") {
        if (this.state.isActive) await this.state.deactivate();
        if (generation !== this.reconcileGeneration) return;
        this.adapter.removePlayerButtons();
        return;
      }

      const context = await this.adapter.waitForPlayer();
      if (generation !== this.reconcileGeneration) return;
      if (!context) {
        this.adapter.removePlayerButtons();
        return;
      }
      if (this.state.isActive) this.state.rebind(context.target);
      this.syncButton();
    }

    async toggle() {
      if (this.state.isActive) {
        await this.deactivate();
        return;
      }
      const context = await this.adapter.waitForPlayer();
      if (!context) return;
      await this.state.activate(context.target);
      this.syncButton();
    }

    async deactivate() {
      await this.state.deactivate();
      this.syncButton();
    }

    syncButton() {
      if (this.adapter.getPageKind() === "unsupported") {
        this.adapter.removePlayerButtons();
        return;
      }
      this.adapter.syncPlayerButton(this.state.isActive, () => void this.toggle());
    }
  }

  const app = new ExtensionApp();
  void app.start();
  window.addEventListener("pagehide", () => app.stop(), { once: true });
})();
