[CmdletBinding()]
param(
    [string]$OutputDirectory = (Join-Path $PSScriptRoot "..\.store-build")
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

if ($PSVersionTable.PSEdition -eq "Core" -and -not $IsWindows) {
    throw "此脚本使用 Windows System.Drawing，请在 Windows PowerShell 或 Windows 上的 PowerShell 7 中运行。"
}

Add-Type -AssemblyName System.Drawing

$extensionRoot = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot ".."))
$outputRoot = [System.IO.Path]::GetFullPath($OutputDirectory)
$stageRoot = Join-Path $outputRoot "package"
$iconsRoot = Join-Path $stageRoot "icons"
$manifestSource = Join-Path $extensionRoot "manifest.json"

function New-RoundedRectanglePath {
    param(
        [System.Drawing.RectangleF]$Rectangle,
        [single]$Radius
    )

    $path = [System.Drawing.Drawing2D.GraphicsPath]::new()
    $diameter = $Radius * 2
    $arc = [System.Drawing.RectangleF]::new(
        $Rectangle.X,
        $Rectangle.Y,
        $diameter,
        $diameter
    )

    $path.AddArc($arc, 180, 90)
    $arc.X = $Rectangle.Right - $diameter
    $path.AddArc($arc, 270, 90)
    $arc.Y = $Rectangle.Bottom - $diameter
    $path.AddArc($arc, 0, 90)
    $arc.X = $Rectangle.Left
    $path.AddArc($arc, 90, 90)
    $path.CloseFigure()

    return $path
}

function Write-ViewportIcon {
    param(
        [int]$Size,
        [string]$Destination
    )

    $scale = 4
    $canvasSize = $Size * $scale
    $largeBitmap = $null
    $largeGraphics = $null
    $smallBitmap = $null
    $smallGraphics = $null
    $backgroundBrush = $null
    $backgroundPath = $null
    $cornerPen = $null

    try {
        $largeBitmap = [System.Drawing.Bitmap]::new(
            $canvasSize,
            $canvasSize,
            [System.Drawing.Imaging.PixelFormat]::Format32bppArgb
        )
        $largeGraphics = [System.Drawing.Graphics]::FromImage($largeBitmap)
        $largeGraphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
        $largeGraphics.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
        $largeGraphics.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
        $largeGraphics.Clear([System.Drawing.Color]::Transparent)

        # 128px 图标中保留约 16px 透明边距；其他尺寸按比例缩放。
        $padding = [single]($canvasSize * 0.125)
        $backgroundRect = [System.Drawing.RectangleF]::new(
            $padding,
            $padding,
            $canvasSize - (2 * $padding),
            $canvasSize - (2 * $padding)
        )
        $backgroundPath = New-RoundedRectanglePath \
            -Rectangle $backgroundRect \
            -Radius ([single]($canvasSize * 0.1875))
        $backgroundColor = [System.Drawing.ColorTranslator]::FromHtml("#202124")
        $backgroundBrush = [System.Drawing.SolidBrush]::new($backgroundColor)
        $largeGraphics.FillPath($backgroundBrush, $backgroundPath)

        $cornerPen = [System.Drawing.Pen]::new(
            [System.Drawing.Color]::White,
            [single]($canvasSize * 0.07)
        )
        $cornerPen.StartCap = [System.Drawing.Drawing2D.LineCap]::Round
        $cornerPen.EndCap = [System.Drawing.Drawing2D.LineCap]::Round
        $cornerPen.LineJoin = [System.Drawing.Drawing2D.LineJoin]::Round

        $innerStart = [single]($canvasSize * 0.305)
        $innerEnd = [single]($canvasSize * 0.695)
        $arm = [single]($canvasSize * 0.09)

        $largeGraphics.DrawLines($cornerPen, [System.Drawing.PointF[]]@(
            [System.Drawing.PointF]::new($innerStart + $arm, $innerStart),
            [System.Drawing.PointF]::new($innerStart, $innerStart),
            [System.Drawing.PointF]::new($innerStart, $innerStart + $arm)
        ))
        $largeGraphics.DrawLines($cornerPen, [System.Drawing.PointF[]]@(
            [System.Drawing.PointF]::new($innerEnd - $arm, $innerStart),
            [System.Drawing.PointF]::new($innerEnd, $innerStart),
            [System.Drawing.PointF]::new($innerEnd, $innerStart + $arm)
        ))
        $largeGraphics.DrawLines($cornerPen, [System.Drawing.PointF[]]@(
            [System.Drawing.PointF]::new($innerStart, $innerEnd - $arm),
            [System.Drawing.PointF]::new($innerStart, $innerEnd),
            [System.Drawing.PointF]::new($innerStart + $arm, $innerEnd)
        ))
        $largeGraphics.DrawLines($cornerPen, [System.Drawing.PointF[]]@(
            [System.Drawing.PointF]::new($innerEnd, $innerEnd - $arm),
            [System.Drawing.PointF]::new($innerEnd, $innerEnd),
            [System.Drawing.PointF]::new($innerEnd - $arm, $innerEnd)
        ))

        $smallBitmap = [System.Drawing.Bitmap]::new(
            $Size,
            $Size,
            [System.Drawing.Imaging.PixelFormat]::Format32bppArgb
        )
        $smallGraphics = [System.Drawing.Graphics]::FromImage($smallBitmap)
        $smallGraphics.CompositingMode = [System.Drawing.Drawing2D.CompositingMode]::SourceCopy
        $smallGraphics.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
        $smallGraphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
        $smallGraphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
        $smallGraphics.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
        $smallGraphics.DrawImage($largeBitmap, 0, 0, $Size, $Size)
        $smallBitmap.Save($Destination, [System.Drawing.Imaging.ImageFormat]::Png)
    }
    finally {
        if ($cornerPen) { $cornerPen.Dispose() }
        if ($backgroundBrush) { $backgroundBrush.Dispose() }
        if ($backgroundPath) { $backgroundPath.Dispose() }
        if ($smallGraphics) { $smallGraphics.Dispose() }
        if ($smallBitmap) { $smallBitmap.Dispose() }
        if ($largeGraphics) { $largeGraphics.Dispose() }
        if ($largeBitmap) { $largeBitmap.Dispose() }
    }
}

if (-not (Test-Path $manifestSource)) {
    throw "未找到 manifest.json：$manifestSource"
}

if (Test-Path $stageRoot) {
    Remove-Item $stageRoot -Recurse -Force
}
New-Item $iconsRoot -ItemType Directory -Force | Out-Null

foreach ($fileName in @("content.js", "content.css")) {
    $source = Join-Path $extensionRoot $fileName
    if (-not (Test-Path $source)) {
        throw "缺少发布文件：$source"
    }
    Copy-Item $source $stageRoot
}

foreach ($size in @(16, 32, 48, 128)) {
    Write-ViewportIcon \
        -Size $size \
        -Destination (Join-Path $iconsRoot "icon$size.png")
}

$manifest = Get-Content $manifestSource -Raw | ConvertFrom-Json
$manifest | Add-Member -NotePropertyName "icons" -NotePropertyValue ([ordered]@{
    "16" = "icons/icon16.png"
    "32" = "icons/icon32.png"
    "48" = "icons/icon48.png"
    "128" = "icons/icon128.png"
}) -Force

$manifestJson = $manifest | ConvertTo-Json -Depth 10
$manifestDestination = Join-Path $stageRoot "manifest.json"
[System.IO.File]::WriteAllText(
    $manifestDestination,
    $manifestJson + [Environment]::NewLine,
    [System.Text.UTF8Encoding]::new($false)
)

$zipName = "viewport-fullscreen-for-youtube-v$($manifest.version).zip"
$zipPath = Join-Path $outputRoot $zipName
if (Test-Path $zipPath) {
    Remove-Item $zipPath -Force
}

Compress-Archive -Path (Join-Path $stageRoot "*") -DestinationPath $zipPath -Force

Write-Host "Chrome Web Store 发布包已生成："
Write-Host $zipPath
Write-Host "提交前请先在 chrome://extensions 中加载以下目录测试："
Write-Host $stageRoot
