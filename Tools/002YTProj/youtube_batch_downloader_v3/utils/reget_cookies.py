
import time
import browser_cookie3

def reget_cookies(config, logger):
    # 从 Firefox 获取 cookies
    cookies = browser_cookie3.firefox(domain_name='youtube.com')

    # 格式化为 Netscape 格式（yt-dlp 支持的格式）
    cookies_txt_path = config.data["cookies_file"]

    with open(cookies_txt_path, 'w', encoding='utf-8') as f:
        f.write("# Netscape HTTP Cookie File\n")
        for cookie in cookies:
            domain = cookie.domain if cookie.domain.startswith('.') else '.' + cookie.domain
            path = cookie.path or '/'
            secure = "TRUE" if cookie.secure else "FALSE"
            expires = int(time.time()) + 3600 * 24 * 30  # 设置过期时间为 30 天
            f.write(f"{domain}\tTRUE\t{path}\t{secure}\t{expires}\t{cookie.name}\t{cookie.value}\n")

    logger.warning(f"✅ Cookies 已提取并保存到 {cookies_txt_path}")