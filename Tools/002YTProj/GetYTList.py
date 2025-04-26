from playwright.sync_api import sync_playwright
import os
import pandas as pd
import time

user_data_dir = "./youtube_profile"

def scroll_to_bottom(page, step=1000, delay=1):
    prev_height = None
    while True:
        current_height = page.evaluate(
            "(step) => { window.scrollBy(0, step); return document.documentElement.scrollHeight; }", step
        )
        if prev_height == current_height:
            break
        prev_height = current_height
        time.sleep(delay)

def scrape_watch_later_with_login():
    with sync_playwright() as p:
        browser = p.chromium.launch_persistent_context(user_data_dir, headless=False)
        page = browser.new_page()

        a = input("👀 请确保已登录 YouTube，按回车继续，这种浏览器被youtube检测出来不安全，不可行")

        playlist_url = "https://www.youtube.com/playlist?list=WL"
        page.goto(playlist_url)

        print("👀 请确保已登录 YouTube，页面加载完后按 Ctrl+C 退出脚本重跑")

        scroll_to_bottom(page)
        print("✅ 页面滚动完成，开始提取")

        elements = page.query_selector_all("ytd-playlist-video-renderer")
        data = []

        for el in elements:
            try:
                creator_elem = el.query_selector("a.yt-simple-endpoint.style-scope.yt-formatted-string")
                creator_url = creator_elem.get_attribute("href")
                creator_name = creator_elem.inner_text()

                if creator_url:
                    full_url = "https://www.youtube.com" + creator_url
                    data.append({
                        "发布者名称": creator_name.strip(),
                        "发布者主页": full_url
                    })
            except Exception as e:
                print(f"⚠️ 跳过一项: {e}")

        df = pd.DataFrame(data).drop_duplicates()
        df.to_csv("稍后观看_发布者列表.csv", index=False, encoding="utf-8-sig")
        print(f"📁 完成：共 {len(df)} 项，已保存 CSV")

        browser.close()

scrape_watch_later_with_login()
