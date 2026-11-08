# SPDX-License-Identifier: GPL-2.0-only
[CmdletBinding()]
param(
    [Parameter(Mandatory)][string]$Images,
    [Parameter(Mandatory)][string]$LibUsb,
    [string]$BootImages,
    [string]$LogDirectory = (Join-Path $PSScriptRoot '../../out/usbboot-log'),
    [switch]$SkipBootstrap
)
$ErrorActionPreference = 'Stop'
$Images = (Resolve-Path -LiteralPath $Images).Path
$LibUsb = (Resolve-Path -LiteralPath $LibUsb).Path
$LogDirectory = [IO.Path]::GetFullPath($LogDirectory)
[IO.Directory]::CreateDirectory($LogDirectory) | Out-Null
$console = Join-Path $PSScriptRoot 'vs680-usb-console.py'
$manifest = Get-Content -LiteralPath (Join-Path $Images 'ram-boot.json') -Raw | ConvertFrom-Json
if (-not $SkipBootstrap) {
    if (-not $BootImages) { throw '需要 -BootImages 指定 Gen2 前级镜像目录' }
    $BootImages = (Resolve-Path -LiteralPath $BootImages).Path
    & uv run --no-project python $console --libusb $LibUsb --images $BootImages `
        --bootstrap --wait-seconds 30 --command '#skip' --seconds 12 --until-prompt `
        --log (Join-Path $LogDirectory 'bootstrap.raw')
    if ($LASTEXITCODE -ne 0) { throw 'USBBOOT 前级加载失败' }
}
$stepNumber = 0
foreach ($step in $manifest.steps) {
    if ($step.command.Length -gt 255) { throw 'USB 命令超过 255 字节' }
    $stepNumber++
    $log = Join-Path $LogDirectory "ram-$stepNumber.raw"
    & uv run --no-project python $console --libusb $LibUsb --images $Images `
        --command $step.command --seconds 30 --until-prompt --log $log
    if ($LASTEXITCODE -ne 0) { throw "RAM 加载步骤 $stepNumber 失败" }
    $reply = [Text.Encoding]::ASCII.GetString([IO.File]::ReadAllBytes($log))
    if ($reply.Contains('FDT_ERR_') -or $reply.Contains('Unknown command') -or $reply.Contains('Usage:')) {
        throw "U-Boot 命令失败，步骤 $stepNumber"
    }
    $actual = @([regex]::Matches($reply, '==> ([0-9a-fA-F]{8})') |
        ForEach-Object { $_.Groups[1].Value.ToLowerInvariant() })
    $expected = @($step.expected_crc)
    if ($actual.Count -ne $expected.Count) { throw "CRC 返回数量错误，步骤 $stepNumber" }
    for ($i = 0; $i -lt $actual.Count; $i++) {
        if ($actual[$i] -ne $expected[$i]) { throw "RAM 数据 CRC 不匹配，步骤 $stepNumber，结果 $i" }
    }
    if (-not $reply.EndsWith('=> ')) { throw "缺少 U-Boot 提示符，步骤 $stepNumber" }
}
# USB 控制台切回串口，释放 BCM 下载接口后交给 Linux。
$handoff = "setenv stdout serial; setenv stderr serial; bcmusboff; $($manifest.handoff)"
if ($handoff.Length -gt 255) { throw '启动交接命令超过 255 字节' }
& uv run --no-project python $console --libusb $LibUsb --images $Images `
    --command $handoff --seconds 8 --log (Join-Path $LogDirectory 'handoff.raw')
if ($LASTEXITCODE -ne 0) { throw 'RAM 启动交接失败' }
Write-Output 'RAM 启动命令已发送'
