# SPDX-License-Identifier: GPL-2.0-only
[CmdletBinding()]
param(
    [string]$Images,
    [string]$BootImages,
    [string]$LibUsb,
    [switch]$Remove
)
$ErrorActionPreference = 'Stop'
$name = 'VS680UsbBoot'
$key = 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Run'
if ($Remove) {
    Remove-ItemProperty -LiteralPath $key -Name $name -ErrorAction SilentlyContinue
    Write-Output '已取消登录启动；当前监视进程会在退出登录时结束'
    return
}
if (-not $Images -or -not $BootImages -or -not $LibUsb) {
    throw '需要 -Images、-BootImages 和 -LibUsb'
}
$Images = (Resolve-Path -LiteralPath $Images).Path
$BootImages = (Resolve-Path -LiteralPath $BootImages).Path
$LibUsb = (Resolve-Path -LiteralPath $LibUsb).Path
if (-not (Test-Path -LiteralPath (Join-Path $Images 'ram-boot.json'))) {
    throw '启动文件目录缺少 ram-boot.json'
}
$null = Get-Command uv -ErrorAction Stop
$engine = (Get-Command pwsh -ErrorAction Stop).Source
$watch = Join-Path $PSScriptRoot 'watch-vs680-usbboot.ps1'
function Quote-Literal([string]$value) { return "'" + $value.Replace("'", "''") + "'" }
$command = '& ' + (Quote-Literal $watch) + ' -Images ' + (Quote-Literal $Images) +
    ' -BootImages ' + (Quote-Literal $BootImages) + ' -LibUsb ' + (Quote-Literal $LibUsb)
$encoded = [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes($command))
$arguments = @('-NoProfile', '-NonInteractive', '-ExecutionPolicy', 'Bypass',
    '-WindowStyle', 'Hidden', '-EncodedCommand', $encoded)
New-Item -Path $key -Force | Out-Null
Set-ItemProperty -LiteralPath $key -Name $name -Value ('"{0}" {1}' -f $engine, ($arguments -join ' '))
Start-Process -FilePath $engine -ArgumentList $arguments -WindowStyle Hidden
Write-Output '已启用当前用户登录后自动下载并启动 VS680'
