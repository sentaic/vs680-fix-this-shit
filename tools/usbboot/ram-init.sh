#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-only
PATH=/bin:/sbin
export PATH
. /boot.conf
mount -t devtmpfs devtmpfs /dev
mount -t proc proc /proc
mount -t sysfs sysfs /sys
mkdir -p /dev/pts /newroot /sys/kernel/config /sys/kernel/debug
mount -t devpts -o mode=0620,ptmxmode=0666 devpts /dev/pts
mount -t configfs configfs /sys/kernel/config
mount -t debugfs debugfs /sys/kernel/debug
if ! mount -t ext4 -o ro,noload "$ROOT_DEVICE" /newroot; then
    echo "根文件系统挂载失败：$ROOT_DEVICE"
    exec /bin/busybox sh
fi
for name in dev proc sys; do mount --bind "/$name" "/newroot/$name"; done
for name in dev/pts sys/kernel/config sys/kernel/debug; do
    mount --bind "/$name" "/newroot/$name"
done
mount --bind /lib/modules /newroot/lib/modules
mount -t tmpfs -o mode=0755 tmpfs /newroot/run
mount -t tmpfs -o mode=1777 tmpfs /newroot/tmp
mkdir -p /newroot/run/sshd /newroot/run/lock
cp /authorized-key /newroot/run/sshd/authorized_keys
/bin/busybox chroot /newroot /sbin/ip link set "$NET_INTERFACE" up
/bin/busybox chroot /newroot /sbin/ip addr add "$BOARD_ADDRESS" dev "$NET_INTERFACE"
/bin/busybox chroot /newroot /usr/sbin/sshd -D -e -o UsePAM=no \
    -o AuthorizedKeysFile=/run/sshd/authorized_keys -o PermitRootLogin=prohibit-password \
    -o PasswordAuthentication=no -E /tmp/sshd-access.log </dev/null >/dev/null 2>&1 &
/bin/busybox watchdog -F -t 5 -T 30 /dev/watchdog0 >/newroot/run/watchdog.log 2>&1 &
if [ -d /hid-runtime ]; then
    mkdir -p /newroot/run/vs680-hid-runtime
    mount --bind /hid-runtime /newroot/run/vs680-hid-runtime
    /bin/busybox chroot /newroot /bin/sh /run/vs680-hid-runtime/start.sh
fi
exec /bin/busybox tail -f /dev/null
