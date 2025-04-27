import streamlit as st
import subprocess
import os

# 设置 Streamlit 界面
st.title("YouTube 视频下载器")

# 输入框：输入视频 URL
url = st.text_input("请输入 YouTube 视频 URL:")

# 下载按钮：点击后下载视频
if st.button("下载视频"):
    if url:
        try:
            # 创建保存下载文件的目录
            download_dir = './downloads'
            if not os.path.exists(download_dir):
                os.makedirs(download_dir)
                
            # 设置yt-dlp命令行
            cookies_path = "cookies.txt"  # 修改为你的 cookies 文件路径
            command = [
                'yt-dlp', 
                '--format', 'bestvideo+bestaudio/best',  # 下载最好的视频和音频
                '--output', f'{download_dir}/%(title)s.%(ext)s',  # 下载路径
                '--cookies', cookies_path,  # 使用 cookies 文件
                url  # 视频 URL
            ]
            
            # 使用 subprocess 执行命令
            st.text("正在下载视频，请稍候...")
            subprocess.run(command, check=True)  # 执行命令行命令
            
            # 下载完成
            st.success("视频下载完成！")
        except subprocess.CalledProcessError as e:
            st.error(f"下载失败: {e}")
    else:
        st.warning("请输入有效的 URL")



# import streamlit as st
# import pandas as pd

# def predict(data):
#     # 模拟一个简单的预测模型
#     return data['Age'] * 2

# def main():
#     st.title("Simple Model Prediction")
#     data = st.file_uploader("Upload CSV", type=["csv"])
#     if data:
#         df = pd.read_csv(data)
#         st.write(df.head())
        
#         result = predict(df)
#         st.write("Prediction Results", result)

# if __name__ == "__main__":
#     main()
