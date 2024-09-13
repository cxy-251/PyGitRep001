' Set ws = CreateObject("Wscript.Shell") ws.run "C:\01Necessity\ToolSoftwares\WebDav\alist-windows-amd64\alist.exe server",vbhide
Set ws = CreateObject("Wscript.Shell")
ws.Run "C:\01Necessity\ToolSoftwares\WebDav\alist-windows-amd64\alist.exe server", 0
ws.Run "cmd /c jupyter lab --config='C:\Users\14398\.jupyter\jupyter_lab_config_iPad.py' --no-browser", 0
