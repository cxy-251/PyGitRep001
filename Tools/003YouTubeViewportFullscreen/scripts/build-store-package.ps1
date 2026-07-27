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
$storeAssetsRoot = Join-Path $outputRoot "store-assets"
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
        $backgroundPath = New-RoundedRectanglePath -Rectangle $backgroundRect -Radius ([single]($canvasSize * 0.1875))
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

function Write-SmallPromoTile {
    param(
        [string]$IconPath,
        [string]$Destination
    )

    $bitmap = $null
    $graphics = $null
    $icon = $null
    $backgroundBrush = $null
    $titleBrush = $null
    $subtitleBrush = $null
    $titleFont = $null
    $subtitleFont = $null

    try {
        $bitmap = [System.Drawing.Bitmap]::new(
            440,
            280,
            [System.Drawing.Imaging.PixelFormat]::Format32bppArgb
        )
        $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
        $graphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
        $graphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
        $graphics.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit

        $backgroundBrush = [System.Drawing.SolidBrush]::new(
            [System.Drawing.ColorTranslator]::FromHtml("#202124")
        )
        $graphics.FillRectangle($backgroundBrush, 0, 0, 440, 280)

        $icon = [System.Drawing.Image]::FromFile($IconPath)
        $graphics.DrawImage($icon, 28, 60, 160, 160)

        $titleBrush = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::White)
        $subtitleBrush = [System.Drawing.SolidBrush]::new(
            [System.Drawing.ColorTranslator]::FromHtml("#BDC1C6")
        )
        $titleFont = [System.Drawing.Font]::new(
            "Segoe UI Semibold",
            30,
            [System.Drawing.FontStyle]::Regular,
            [System.Drawing.GraphicsUnit]::Pixel
        )
        $subtitleFont = [System.Drawing.Font]::new(
            "Segoe UI",
            16,
            [System.Drawing.FontStyle]::Regular,
            [System.Drawing.GraphicsUnit]::Pixel
        )

        $graphics.DrawString("VIEWPORT", $titleFont, $titleBrush, 210, 76)
        $graphics.DrawString("FULLSCREEN", $titleFont, $titleBrush, 210, 116)
        $graphics.DrawString("KEEP TABS VISIBLE", $subtitleFont, $subtitleBrush, 212, 172)

        $bitmap.Save($Destination, [System.Drawing.Imaging.ImageFormat]::Png)
    }
    finally {
        if ($subtitleFont) { $subtitleFont.Dispose() }
        if ($titleFont) { $titleFont.Dispose() }
        if ($subtitleBrush) { $subtitleBrush.Dispose() }
        if ($titleBrush) { $titleBrush.Dispose() }
        if ($backgroundBrush) { $backgroundBrush.Dispose() }
        if ($icon) { $icon.Dispose() }
        if ($graphics) { $graphics.Dispose() }
        if ($bitmap) { $bitmap.Dispose() }
    }
}

if (-not (Test-Path $manifestSource)) {
    throw "未找到 manifest.json：$manifestSource"
}

if (Test-Path $stageRoot) {
    Remove-Item $stageRoot -Recurse -Force
}
if (Test-Path $storeAssetsRoot) {
    Remove-Item $storeAssetsRoot -Recurse -Force
}
New-Item $iconsRoot -ItemType Directory -Force | Out-Null
New-Item $storeAssetsRoot -ItemType Directory -Force | Out-Null

foreach ($fileName in @("content.js", "content.css")) {
    $source = Join-Path $extensionRoot $fileName
    if (-not (Test-Path $source)) {
        throw "缺少发布文件：$source"
    }
    Copy-Item $source $stageRoot
}

foreach ($size in @(16, 32, 48, 128)) {
    Write-ViewportIcon -Size $size -Destination (Join-Path $iconsRoot "icon$size.png")
}

$storeIconPath = Join-Path $storeAssetsRoot "store-icon-128.png"
Copy-Item (Join-Path $iconsRoot "icon128.png") $storeIconPath
Write-SmallPromoTile -IconPath $storeIconPath -Destination (Join-Path $storeAssetsRoot "small-promo-440x280.png")

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
Write-Host "商店图标和小型宣传图："
Write-Host $storeAssetsRoot
Write-Host "提交前请先在 chrome://extensions 中加载以下目录测试："
Write-Host $stageRoot
