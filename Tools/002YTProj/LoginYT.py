# import undetected_chromedriver as uc
# from selenium.webdriver.common.by import By
# from selenium.webdriver.chrome.options import Options
# import time

# def manual_login():
#     # 配置 Chrome Options
#     chrome_options = Options()
#     chrome_options.add_argument("--start-maximized")  # 启动时最大化窗口
#     chrome_options.add_argument("--disable-blink-features=AutomationControlled")  # 避免被检测为自动化工具
#     # 添加代理配置，如果需要
#     # chrome_options.add_argument("--proxy-server=http://127.0.0.1:10809")  # 代理服务器

#     # 启动浏览器并登录
#     driver = uc.Chrome(options=chrome_options)
#     driver.get("https://www.youtube.com")

#     print("✅ 请登录到 YouTube，并且手动关闭浏览器")
#     input("👉 登录完成后，按回车继续（关闭浏览器也可以）")

#     # 确保浏览器登录成功后关闭
#     driver.quit()

# manual_login()




# from playwright.sync_api import sync_playwright

# def manual_login():
#     user_data_dir = "youtube_login_profile"  # 目录名可以随便取
#     with sync_playwright() as p:
#         # ⚠️ 打开一个有持久化数据的浏览器（这样才会保存你登录后的信息）
#         browser = p.chromium.launch_persistent_context(user_data_dir, headless=False)
#         page = browser.new_page()
#         page.goto("https://www.youtube.com")
#         print("✅ 请在弹出的页面中登录你的 YouTube 账号，然后手动关闭浏览器")
#         input("👉 登录完成后，按回车继续（关闭浏览器也可以）")

#         browser.close()

# manual_login()



import time
import undetected_chromedriver as uc
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager

# 设置ChromeOptions来避免被检测到
options = uc.ChromeOptions()

# 使用 undetected_chromedriver 启动 Chrome 浏览器
driver = uc.Chrome(service=Service(ChromeDriverManager().install()), options=options)

# 访问 YouTube 登录页面
driver.get("https://accounts.google.com/ServiceLogin?service=youtube")

# 等待页面加载
time.sleep(2)

# 输入 Google 帐号（用户名）
email_input = driver.find_element(By.ID, "identifierId")
email_input.send_keys("your_email@gmail.com")  # 在此处替换为你的 Gmail 地址
email_input.send_keys(Keys.RETURN)

# 等待页面加载
time.sleep(2)

# 输入密码
password_input = driver.find_element(By.NAME, "password")
password_input.send_keys("your_password")  # 在此处替换为你的密码
password_input.send_keys(Keys.RETURN)

# 等待登录完成，最大等待时间为 10 秒
time.sleep(10)

# 登录后，获取页面源或执行其他操作
print(driver.current_url)  # 打印当前页面的 URL，验证是否成功登录

# 在此处可以添加你后续的操作，获取视频或其他数据
# driver.get("https://www.youtube.com/playlist?list=WL")  # 例如获取播放列表

# 完成任务后退出浏览器
driver.quit()






