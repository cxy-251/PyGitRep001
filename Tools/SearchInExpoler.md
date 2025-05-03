# Windows 无法使用英文搜索语法（如 `size:>1GB`）的原因与解决方案

## 🧩 问题描述

在 Windows 文件资源管理器的搜索框中输入如下英文语法：
```plaintext
size:>1GB
```

发现无法正确搜索文件，或无结果返回。但使用中文语法，如：

```plaintext
大小:大
```

却能正常匹配文件。

---

## 📌 问题原因

Windows 文件资源管理器的搜索功能使用 **AQS（Advanced Query Syntax）**，它依赖于系统当前的显示语言。

* 若系统语言是中文（简体/繁体），**资源管理器的搜索语法将使用本地化关键词（如“大小”而非“size”）**。
* `size:>1GB` 等英文语法在中文环境中默认无效。
* AQS 语言映射表并不统一，且无法在资源管理器中直接切换语言。

---

## ✅ 替代方案一：修改系统语言

若确实需要使用英文搜索语法，可以：

1. 打开“设置” → “时间和语言” → “语言和区域”；
2. 将系统语言改为英文；
3. 重启系统。

缺点：

* 会影响整个系统界面语言；
* 不适用于只希望资源管理器支持英文搜索的用户。

---

## ✅ 替代方案二：使用 PowerShell 脚本搜索（推荐）

PowerShell 脚本不依赖 AQS，本质是文件属性级别的搜索，更灵活可靠。

---

## 🛠️ PowerShell 搜索语法详解

### 获取指定大小范围的视频文件

示例：查找大于 1GB 的 MP4/MKV 文件

```powershell
Get-ChildItem -Path "D:\Videos" -Recurse -File |
Where-Object { ($_.Extension -match "\.mp4|\.mkv") -and $_.Length -gt 1GB }
```

### 查找介于 2GB 和 4GB 的视频

```powershell
Get-ChildItem -Path "D:\Videos" -Recurse -File |
Where-Object { $_.Length -ge 2GB -and $_.Length -lt 4GB }
```

### 按文件名模糊搜索

```powershell
Get-ChildItem -Recurse -File |
Where-Object { $_.Name -like "*lecture*" }
```

### 排除某种扩展名的文件（如 MP3）

```powershell
Get-ChildItem -Recurse -File |
Where-Object { $_.Extension -ne ".mp3" }
```

---

## 📄 示例脚本：搜索所有大于 4GB 的视频

```powershell
$targetPath = "D:\YourFolder"
Get-ChildItem -Path $targetPath -Recurse -File |
Where-Object { $_.Length -gt 4GB -and $_.Extension -match '\.mp4|\.mkv' } |
ForEach-Object {
    Write-Output "文件大小: $([Math]::Round($_.Length / 1GB, 2)) GB"
    Write-Output "路径: $($_.FullName)"
    Write-Output ""
}
```

### 预期输出：

```
文件大小: 4.78 GB
路径: D:\YourFolder\Movie\long_movie.mkv
```

---

## ✅ 总结

| 方法         | 是否推荐 | 备注说明          |
| ---------- | ---- | ------------- |
| 修改系统语言     | ❌    | 改动大，不便维护      |
| PowerShell | ✅    | 灵活、强大、跨语言环境通用 |

PowerShell 是在中文系统中处理文件搜索和批量操作的首选方式，推荐学习掌握基本语法用于替代传统资源管理器的搜索功能。

这个文档介绍了你所需要的内容，包括系统无法支持英文语法的原因，以及使用 PowerShell 进行文件搜索的替代方案和语法。


---

# PowerShell 文件搜索语法详解

PowerShell 提供了强大的文件和目录操作能力，可以用于搜索、过滤和管理文件。以下是一些常见的 PowerShell 文件操作命令和语法的详细介绍。

## 🛠️ 常用命令

### 1. `Get-ChildItem`

`Get-ChildItem` 是 PowerShell 中用来列出指定目录及其子目录中文件和文件夹的命令。

#### 基本用法
```powershell
Get-ChildItem -Path "C:\Users\YourName\Documents"
```
此命令会列出 `"C:\Users\YourName\Documents"` 目录下的所有文件和文件夹。

#### 递归列出子目录中的文件
```powershell
Get-ChildItem -Path "C:\Users\YourName\Documents" -Recurse
```
加上 `-Recurse` 参数会递归列出子目录中的文件。

#### 只列出文件（不包括文件夹）
```powershell
Get-ChildItem -Path "C:\Users\YourName\Documents" -File
```
使用 `-File` 参数可以只获取文件，不包括文件夹。

#### 只列出文件夹
```powershell
Get-ChildItem -Path "C:\Users\YourName\Documents" -Directory
```
使用 `-Directory` 参数可以只列出文件夹，不包括文件。

### 2. `Where-Object`

`Where-Object` 用于筛选符合特定条件的对象。

#### 基本用法
```powershell
Get-ChildItem -Path "C:\YourFolder" | Where-Object { $_.Extension -eq ".txt" }
```
此命令会列出 `"C:\YourFolder"` 目录下所有 `.txt` 文件。

- `$_` 表示当前对象，`$_.Extension` 是文件的扩展名。

#### 结合文件大小过滤
```powershell
Get-ChildItem -Path "C:\YourFolder" -Recurse | Where-Object { $_.Length -gt 1GB }
```
此命令会列出大于 1GB 的文件。

### 3. `Select-Object`

`Select-Object` 用于从返回的对象中选择指定的属性或字段。

#### 示例
```powershell
Get-ChildItem -Path "C:\YourFolder" | Select-Object Name, Length
```
此命令会列出 `"C:\YourFolder"` 目录下所有文件的名称和大小。

### 4. `Sort-Object`

`Sort-Object` 用于对对象进行排序。

#### 示例
```powershell
Get-ChildItem -Path "C:\YourFolder" | Sort-Object Length
```
此命令会按文件大小对文件进行升序排序。

### 5. `ForEach-Object`

`ForEach-Object` 用于对每个返回的对象执行操作。

#### 示例
```powershell
Get-ChildItem -Path "C:\YourFolder" | ForEach-Object { Write-Output "文件名: $($_.Name)" }
```
此命令会输出目录中每个文件的名称。

---

## 📂 文件搜索示例

### 1. 查找特定类型的文件

```powershell
Get-ChildItem -Path "D:\YourFolder" -Recurse | Where-Object { $_.Extension -eq ".mp4" }
```
此命令会递归查找 `"D:\YourFolder"` 目录下所有的 `.mp4` 文件。

### 2. 查找大于 1GB 的文件

```powershell
Get-ChildItem -Path "D:\YourFolder" -Recurse | Where-Object { $_.Length -gt 1GB }
```
此命令会列出所有大于 1GB 的文件。

### 3. 查找 2GB 到 4GB 之间的文件

```powershell
Get-ChildItem -Path "D:\YourFolder" -Recurse | Where-Object { $_.Length -ge 2GB -and $_.Length -lt 4GB }
```
此命令会列出所有文件大小介于 2GB 和 4GB 之间的文件。

### 4. 查找包含特定关键字的文件

```powershell
Get-ChildItem -Path "D:\YourFolder" -Recurse | Where-Object { $_.Name -like "*report*" }
```
此命令会查找文件名中包含 `report` 的所有文件。

### 5. 排除特定类型的文件（如 MP3 文件）

```powershell
Get-ChildItem -Path "D:\YourFolder" -Recurse | Where-Object { $_.Extension -ne ".mp3" }
```
此命令会列出所有不包含 `.mp3` 扩展名的文件。

---

## 📏 文件大小比较

PowerShell 支持直接使用文件大小的比较操作符进行筛选。

### 常见比较操作符

- `-gt` : 大于 (greater than)
- `-lt` : 小于 (less than)
- `-ge` : 大于或等于 (greater than or equal)
- `-le` : 小于或等于 (less than or equal)
- `-eq` : 等于 (equal)
- `-ne` : 不等于 (not equal)

### 示例：查找大于 4GB 的文件
```powershell
Get-ChildItem -Path "D:\YourFolder" -Recurse | Where-Object { $_.Length -gt 4GB }
```

### 示例：查找小于 500MB 的文件
```powershell
Get-ChildItem -Path "D:\YourFolder" -Recurse | Where-Object { $_.Length -lt 500MB }
```

---

## 📋 实用命令汇总

| 命令                         | 作用                                   |
|------------------------------|----------------------------------------|
| `Get-ChildItem`               | 列出文件和文件夹                      |
| `Where-Object`                | 筛选符合条件的对象                    |
| `Select-Object`               | 选择对象的属性                        |
| `Sort-Object`                 | 对对象排序                            |
| `ForEach-Object`              | 对每个对象执行操作                    |
| `-gt` (greater than)          | 大于                                  |
| `-lt` (less than)             | 小于                                  |
| `-ge` (greater than or equal) | 大于或等于                            |
| `-le` (less than or equal)    | 小于或等于                            |
| `-eq` (equal)                 | 等于                                  |
| `-ne` (not equal)             | 不等于                                |

---

## 📝 总结

PowerShell 是强大的脚本工具，能够灵活地处理文件系统中的各类操作，提供比 Windows 资源管理器更精确的搜索能力。通过结合 `Get-ChildItem`、`Where-Object` 和 `Select-Object` 等命令，用户可以根据文件的属性进行复杂的搜索和操作。

这个 Markdown 文档介绍了 PowerShell 中用于搜索和处理文件的常用命令和语法，包括如何筛选、排序、搜索特定文件等操作。希望对你有帮助！


---

# PowerShell 高级文件搜索与处理用法

本文档整理了一些 PowerShell 中更高级的文件搜索、过滤和批处理操作方法，适合需要进行高效文件管理的用户使用。

---

## 🧠 进阶技巧一：结合正则表达式匹配文件名

```powershell
Get-ChildItem -Recurse | Where-Object { $_.Name -match '^IMG_\d{4}\.jpg$' }
```
- 匹配形如 IMG_1234.jpg 的图片文件。

---

## 🧠 进阶技巧二：多个条件组合过滤

```powershell
Get-ChildItem -Recurse | Where-Object { $_.Extension -eq ".mp4" -and $_.Length -gt 2GB }
```
- 获取所有 `.mp4` 且大于 2GB 的视频文件。

---

## 🧠 进阶技巧三：筛选指定时间范围的文件

```powershell
Get-ChildItem -Recurse | Where-Object {
    $_.LastWriteTime -gt (Get-Date).AddDays(-7)
}
```
- 获取最近 7 天修改过的文件。

---

## 🧠 进阶技巧四：使用 Filter 提高性能

```powershell
Get-ChildItem -Path "D:\Videos" -Filter "*.mp4" -Recurse
```
- `-Filter` 比 `Where-Object` 快，适用于简单匹配。

---

## 🧠 进阶技巧五：组合管道生成报表

```powershell
Get-ChildItem -Recurse | Where-Object { $_.Extension -eq ".log" } |
Select-Object Name, Length, LastWriteTime |
Export-Csv "log_report.csv" -NoTypeInformation
```
- 导出所有 `.log` 文件的信息到 CSV 文件，适合做日志统计。

---

## 🧠 进阶技巧六：使用 `Measure-Object` 做统计

```powershell
Get-ChildItem -Recurse | Measure-Object -Property Length -Sum
```
- 统计当前目录及子目录中所有文件的总大小（字节）。

---

## 🧠 进阶技巧七：查找重复文件（按大小和哈希）

```powershell
Get-ChildItem -Recurse -File | Group-Object Length | Where-Object { $_.Count -gt 1 }
```
- 先按大小分组，找出疑似重复文件（可以再结合哈希做精确判断）。

---

## 🧠 进阶技巧八：批量重命名文件

```powershell
Get-ChildItem -Recurse -Filter "*.txt" | Rename-Item -NewName { $_.Name -replace "old", "new" }
```
- 把所有 txt 文件名中的 "old" 替换为 "new"。

---

## 🧠 进阶技巧九：根据文件内容查找

```powershell
Select-String -Path "*.log" -Pattern "ERROR"
```
- 在所有 `.log` 文件中搜索包含 “ERROR” 的行。

---

## 🧠 进阶技巧十：查找空文件夹

```powershell
Get-ChildItem -Directory -Recurse | Where-Object { ($_ | Get-ChildItem).Count -eq 0 }
```
- 查找没有任何内容的空目录。

---

## 📚 推荐组合命令模板

### 查找所有最近 30 天内创建的 PDF 文件大于 1MB 的：

```powershell
Get-ChildItem -Recurse -Filter "*.pdf" | Where-Object {
    $_.CreationTime -gt (Get-Date).AddDays(-30) -and $_.Length -gt 1MB
}
```

### 查找扩展名为 `.mov` 或 `.mp4` 且大小超过 3GB 的视频：

```powershell
Get-ChildItem -Recurse | Where-Object {
    ($_.Extension -eq ".mov" -or $_.Extension -eq ".mp4") -and $_.Length -gt 3GB
}
```

---

## ✅ 总结

通过合理使用 PowerShell 的对象管道、过滤器、正则表达式和条件逻辑，可以构建出高效、强大的文件搜索与处理命令组合，大大提升日常文件管理效率。


---

# PowerShell 使用详解：从文件检索到系统自动化

PowerShell 是 Windows 上功能强大的命令行工具与脚本语言，适合用于系统管理、文件操作、网络任务等自动化流程。本文档将全面介绍 PowerShell 的语法、用途及高级用法。

---

## 📁 一、文件与文件夹操作

### 1. 创建、复制、移动、删除文件/文件夹

```powershell
New-Item -Path "C:\Logs" -ItemType Directory         # 创建文件夹
Copy-Item -Path "file.txt" -Destination "C:\Backup"  # 复制文件
Move-Item -Path "file.txt" -Destination "D:\Docs"    # 移动文件
Remove-Item -Path "C:\OldLogs\*.log"                # 删除日志文件
```

### 2. 读取与写入文件内容

```powershell
Get-Content "log.txt"             # 读取文件内容
Set-Content "output.txt" "Hello"  # 写入内容（覆盖）
Add-Content "output.txt" "World"  # 追加内容
```

---

## 🖥️ 二、系统管理

### 1. 服务管理

```powershell
Get-Service                        # 获取所有服务
Get-Service "Spooler"              # 获取指定服务
Start-Service "Spooler"            # 启动服务
Stop-Service "Spooler"             # 停止服务
Restart-Service "Spooler"          # 重启服务
```

### 2. 进程管理

```powershell
Get-Process                        # 查看进程
Stop-Process -Name "notepad"       # 杀掉进程
```

---

## 🌐 三、网络管理

### 1. 查看 IP 地址和网络接口

```powershell
Get-NetIPAddress
Get-NetAdapter
```

### 2. 测试连接和下载

```powershell
Test-Connection google.com        # ping 测试
Invoke-WebRequest -Uri http://example.com -OutFile example.html
```

### 3. PowerShell Remoting（远程会话）

```powershell
Enter-PSSession -ComputerName server01
```

---

## 📦 四、包与软件管理

### 1. 安装与使用模块

```powershell
Install-Module -Name Az           # 安装 Azure 模块
Import-Module Az
Get-Module -ListAvailable
```

### 2. 使用 Winget/Chocolatey 安装软件

```powershell
winget install git
choco install vscode
```

---

## 📅 五、任务调度与自动化

### 1. 创建定时任务（需结合 Task Scheduler）

```powershell
schtasks /create /sc daily /tn "BackupTask" /tr "powershell.exe -File backup.ps1" /st 23:00
```

### 2. 自动化脚本示例：按日期备份

```powershell
$date = Get-Date -Format "yyyyMMdd"
Copy-Item "C:\Data" -Destination "D:\Backup\Data_$date" -Recurse
```

---

## 📜 六、脚本结构与错误处理

### 1. 条件判断

```powershell
if ($true) { "执行了" } else { "没执行" }
```

### 2. 循环结构

```powershell
foreach ($file in Get-ChildItem *.txt) {
    Write-Output $file.Name
}
```

### 3. 错误捕获

```powershell
try {
    Get-Item "nonexistent.txt"
} catch {
    Write-Warning "文件不存在"
}
```

---

## 📝 七、日志、事件与监控

### 1. 查看系统事件日志

```powershell
Get-EventLog -LogName System -Newest 10
```

### 2. 实时查看日志更新

```powershell
Get-Content "log.txt" -Wait
```

---

## 🧰 八、注册表操作

```powershell
Get-ItemProperty "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
Set-ItemProperty -Path ... -Name ... -Value ...
```

---

## 💡 九、用户交互与 GUI 元素（进阶）

### 弹出输入框

```powershell
Add-Type -AssemblyName Microsoft.VisualBasic
[Microsoft.VisualBasic.Interaction]::InputBox("请输入名称", "提示")
```

---

## 💡 十、使用 PowerShell 替代资源管理器的搜索

若资源管理器不支持英文 AQS 语法，可用 PowerShell 实现强大搜索：

```powershell
Get-ChildItem -Path C:\Videos -Recurse -Include *.mp4 -File | Where-Object { $_.Length -gt 2GB }
```

> ✅ 示例结果：列出所有大于 2GB 的 MP4 视频文件。

---

## 🔚 总结：PowerShell 的适用范围

| 任务类别         | 示例 |
|------------------|------|
| 文件管理         | 创建、读取、移动、过滤 |
| 系统服务管理     | 启动服务、终止进程     |
| 网络操作         | Ping、远程访问、下载网页 |
| 注册表操作       | 修改系统参数           |
| 软件安装与更新   | 使用 Winget / Chocolatey |
| 日志与系统事件   | 监控日志、查询错误     |
| 自动化与调度     | 定时执行备份等脚本     |
| 图形界面交互     | 弹窗、输入框、消息框   |

PowerShell 远不止命令行那般简单，它是系统管理员与高级用户的强力助手。
