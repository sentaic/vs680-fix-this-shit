#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-only
# 创建 FunctionFS 键鼠设备并管理转发进程。
set -euo pipefail
script_dir=$(cd -- "$(dirname -- "$0")" && pwd)
binary=${VS680_HID_BIN:-$script_dir/../../build/usb-hid/aarch64-unknown-linux-gnu/release/vs680-hid}
gadget=/sys/kernel/config/usb_gadget/vs680-kvm
ffs=/run/vs680-kvm/ffs

if [[ ${1:-} == --help || $# == 0 ]]; then
    cat <<'HELP'
Usage: sudo ./hid-gadget.sh --device /dev/hidrawN [--device /dev/hidrawM] [--rate auto|1000|4000|8000] [--duration SECONDS]
Use the built vs680-hid executable with --list to find physical interfaces.
Set the OTG controller to device mode before starting.
VS680_UDC selects a controller when more than one exists.
VS680_HID_BIN can override the executable path.
Ctrl+C stops the relay and removes its USB gadget.
HELP
    exit 0
fi
[[ $EUID == 0 ]] || { echo "Run on the VS680 board as root" >&2; exit 1; }
[[ -x $binary ]] || { echo "Missing executable: $binary" >&2; exit 1; }
for arg in "$@"; do
    case "$arg" in --udc|--ffs|--list) echo "Use the launcher's UDC/FunctionFS configuration" >&2; exit 2;; esac
done
# Use the selected physical USB device VID/PID and HID class descriptors.
primary=
arguments=("$@")
for ((i=0; i<${#arguments[@]}-1; i++)); do
    if [[ ${arguments[i]} == --device ]]; then primary=${arguments[i+1]}; break; fi
done
[[ -n $primary ]] || { echo "Select a physical HID interface" >&2; exit 2; }
primary=$(readlink -f -- "$primary")
physical_id=$(sed -n 's/^HID_ID=//p' "/sys/class/hidraw/${primary##*/}/device/uevent")
[[ $physical_id =~ ^0003:0000([[:xdigit:]]{4}):0000([[:xdigit:]]{4})$ ]] || { echo "Expected a USB HID identity" >&2; exit 2; }
vendor=${BASH_REMATCH[1]}
product=${BASH_REMATCH[2]}
exec 9>/run/lock/vs680-kvm.lock
flock -n 9 || { echo "Another demo owns the gadget" >&2; exit 1; }
mountpoint -q /sys/kernel/config || mount -t configfs none /sys/kernel/config
[[ ! -e $gadget ]] || { echo "$gadget already exists; inspect its owner before removing it" >&2; exit 1; }
[[ ! -e /run/vs680-kvm ]] || { echo "/run/vs680-kvm already exists; inspect the previous session" >&2; exit 1; }
shopt -s nullglob
for bound in /sys/kernel/config/usb_gadget/*/UDC; do
    [[ -z $(cat "$bound") ]] || { echo "Another gadget is bound: $bound" >&2; exit 1; }
done
controllers=(/sys/class/udc/*)
udc=${VS680_UDC:-}
if [[ -z $udc ]]; then
    [[ ${#controllers[@]} == 1 ]] || { echo "Need one UDC (or VS680_UDC). Check OTG device mode and cable." >&2; exit 1; }
    udc=$(basename "${controllers[0]}")
fi
[[ $udc != */* && -d /sys/class/udc/$udc ]] || { echo "Invalid UDC: $udc" >&2; exit 1; }
created=0
mounted=0
child=
cleanup() {
    status=$?
    trap - EXIT INT TERM
    if [[ $created == 1 ]]; then
        printf '\n' > "$gadget/UDC" 2>/dev/null || true
    fi
    if [[ -n $child ]]; then
        kill -TERM "$child" 2>/dev/null || true
        for _ in {1..20}; do
            kill -0 "$child" 2>/dev/null || break
            sleep 0.1
        done
        kill -KILL "$child" 2>/dev/null || true
        wait "$child" 2>/dev/null || true
    fi
    if [[ $mounted == 1 ]]; then umount "$ffs" || true; fi
    if [[ $created == 1 ]]; then
        [[ ! -L $gadget/configs/c.1/ffs.vs680relay ]] || unlink "$gadget/configs/c.1/ffs.vs680relay"
        rmdir "$gadget/functions/ffs.vs680relay" "$gadget/configs/c.1/strings/0x409" \
            "$gadget/configs/c.1" "$gadget/strings/0x409" "$gadget" 2>/dev/null || true
    fi
    rmdir "$ffs" /run/vs680-kvm 2>/dev/null || true
    exit "$status"
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
mkdir "$gadget"
created=1
echo "0x$vendor" > "$gadget/idVendor"
echo "0x$product" > "$gadget/idProduct"
echo 0x0200 > "$gadget/bcdUSB"
echo 0x0400 > "$gadget/bcdDevice"
# 控制器提供 max_speed 属性时，将设备速度设为高速 USB2。
if [[ -e $gadget/max_speed ]]; then echo high-speed > "$gadget/max_speed"; fi
mkdir "$gadget/strings/0x409"
echo USBINPUT-0004 > "$gadget/strings/0x409/serialnumber"
echo "Generic" > "$gadget/strings/0x409/manufacturer"
echo "USB Keyboard and Mouse" > "$gadget/strings/0x409/product"
mkdir -p "$gadget/configs/c.1/strings/0x409"
echo "Keyboard and Mouse" > "$gadget/configs/c.1/strings/0x409/configuration"
echo 0xC0 > "$gadget/configs/c.1/bmAttributes"
echo 2 > "$gadget/configs/c.1/MaxPower"
mkdir "$gadget/functions/ffs.vs680relay"
ln -s "$gadget/functions/ffs.vs680relay" "$gadget/configs/c.1/ffs.vs680relay"
mkdir -p "$ffs"
mount -t functionfs vs680relay "$ffs"
mounted=1
# 避免glibc为每个线程预留大块分配区后再被mlockall锁定。
MALLOC_ARENA_MAX=1 "$binary" --ffs "$ffs" --udc "$udc" "$@" &
child=$!
wait "$child"
child=
