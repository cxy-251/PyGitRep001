Set WshShell = WScript.CreateObject("WScript.Shell")

' ' 打开 Steam
' WshShell.Run "steam:", 0, false
' WScript.Sleep 3000 ' 等待 3 秒

' 打开 Steam 游戏 Farmer Against Potatoes Idle
' WshShell.Run "steam://rungameid/1535560", 1, False
' WScript.Sleep 5000 ' 等待 5 秒

' 打开 Microsoft Edge
WshShell.Run "microsoft-edge:", 9, false
WScript.Sleep 1000 ' 等待 1 秒

' 打开 windows terminal preview
WshShell.Run "wt", 1, false
WScript.Sleep 1000 ' 等待 1 秒

' 打开 VSCode
' WshShell.Run "cmd /c code", 0, false
' WScript.Sleep 1000 ' 等待 1 秒

' 打开文件资源管理器
WshShell.Run "explorer C:\02Programmer\02Proj", 9, false
WScript.Sleep 1000 ' 等待 1 秒

' 打开 设置蓝牙耳机连接
WshShell.Run "ms-settings:bluetooth", 9, false
WScript.Sleep 1000 ' 等待 1 秒

' 打开 OpenAI
' WshShell.Run "C:\01Necessity\ToolSoftwares\StartUpLink\OpenAI.lnk", 9, false
' WScript.Sleep 1000 ' 等待 1 秒

''''''''''''''''''''''''''''''''''''路径不能包含空格
' 打开 Google Trans
' WshShell.Run "C:\01Necessity\ToolSoftwares\StartUpLink\GoogleTrans.lnk", 9, false
' WScript.Sleep 1000 ' 等待 1 秒

' 打开 MuMuMultiPlayer
' WshShell.Run "C:\03Entertainment\MuMu\MuMuPlayer-12.0\shell\MuMuMultiPlayer.exe", 1, false
' WScript.Sleep 1000 ' 等待 1 秒

