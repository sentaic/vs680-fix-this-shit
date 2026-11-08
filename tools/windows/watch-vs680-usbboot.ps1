# SPDX-License-Identifier: GPL-2.0-only
[CmdletBinding()]
param(
    [Parameter(Mandatory)][string]$Images,
    [Parameter(Mandatory)][string]$BootImages,
    [Parameter(Mandatory)][string]$LibUsb,
    [string]$LogDirectory = (Join-Path $PSScriptRoot '../../out/usbboot-log'),
    [switch]$Once
)
$ErrorActionPreference = 'Stop'
$Images = (Resolve-Path -LiteralPath $Images).Path
$BootImages = (Resolve-Path -LiteralPath $BootImages).Path
$LibUsb = (Resolve-Path -LiteralPath $LibUsb).Path
$LogDirectory = [IO.Path]::GetFullPath($LogDirectory)
[IO.Directory]::CreateDirectory($LogDirectory) | Out-Null
$mutex = [Threading.Mutex]::new($false, 'Local\VS680-USBBOOT')
if (-not $mutex.WaitOne(0)) { $mutex.Dispose(); exit 0 }
$events = $null
try {
    $attempted = $false
    if (-not $Once) {
        $events = Register-CimIndicationEvent -Query 'SELECT * FROM Win32_DeviceChangeEvent' `
            -SourceIdentifier VS680UsbBootChange
    }
    do {
        $present = @(Get-PnpDevice -PresentOnly |
            Where-Object { $_.InstanceId -like 'USB\VID_06CB&PID_00B1\*' }).Count -gt 0
        if (-not $present) { $attempted = $false }
        if ($present -and -not $attempted) {
            $attempted = $true
            try {
                & (Join-Path $PSScriptRoot 'boot-vs680-optee-ram.ps1') `
                    -Images $Images -BootImages $BootImages -LibUsb $LibUsb `
                    -LogDirectory $LogDirectory *> (Join-Path $LogDirectory 'console.raw')
                "$(Get-Date -Format o) RAM 启动命令已发送" |
                    Set-Content -LiteralPath (Join-Path $LogDirectory 'status.txt')
            } catch {
                "$(Get-Date -Format o) $($_.Exception.Message)" |
                    Set-Content -LiteralPath (Join-Path $LogDirectory 'status.txt')
                if ($Once) { throw }
            }
        }
        if ($Once) { break }
        $null = Wait-Event -SourceIdentifier VS680UsbBootChange
        Get-Event -SourceIdentifier VS680UsbBootChange | Remove-Event
    } while ($true)
} finally {
    if ($events) { Unregister-Event -SourceIdentifier VS680UsbBootChange }
    $mutex.ReleaseMutex()
    $mutex.Dispose()
}
