#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
"""生成使用 eMMC Linux 用户空间的 RAM 启动程序。"""
import argparse
import base64
import gzip
import ipaddress
from pathlib import Path
import re
import shlex
import stat
import struct

ROOT = Path(__file__).resolve().parents[2]

def pack(entries):
    result = bytearray()
    for ino, (name, mode, body) in enumerate([*entries, ('TRAILER!!!', 0, b'')], 1):
        encoded = name.encode() + b'\0'
        fields = [ino, mode, 0, 0, 1, 0, len(body), 0, 0, 0, 0, len(encoded), 0]
        result += b'070701' + ''.join(f'{v:08x}' for v in fields).encode() + encoded
        result += bytes(-len(result) % 4)
        result += body
        result += bytes(-len(result) % 4)
    return bytes(result)

def arm64_static(data):
    if len(data) < 64 or data[:6] != b'\x7fELF\x02\x01' or struct.unpack_from('<H', data, 18)[0] != 183:
        raise ValueError('BusyBox 须为 ARM64 ELF')
    offset = struct.unpack_from('<Q', data, 32)[0]
    size, count = struct.unpack_from('<HH', data, 54)
    for i in range(count):
        if struct.unpack_from('<I', data, offset + i * size)[0] == 3:
            raise ValueError('BusyBox 须使用静态链接')

def public_key(path):
    # 校验 OpenSSH 公钥字段并保留算法和编码内容。
    fields = path.read_text().strip().split()
    if len(fields) < 2 or fields[0] not in ('ssh-ed25519', 'ssh-rsa', 'ecdsa-sha2-nistp256'):
        raise ValueError('需要 OpenSSH 公钥文件')
    base64.b64decode(fields[1], validate=True)
    return (' '.join(fields[:2]) + '\n').encode()

def identity(value):
    if not re.fullmatch(r'[0-9a-fA-F]{4}:[0-9a-fA-F]{4}:[0-9a-fA-F]{2}', value):
        raise argparse.ArgumentTypeError('设备格式为 VID:PID:接口号')
    return value.lower()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--busybox', type=Path, required=True)
    parser.add_argument('--authorized-key', type=Path, required=True)
    parser.add_argument('--address', type=ipaddress.IPv4Interface, required=True)
    parser.add_argument('--root-device', default='/dev/mmcblk0p16')
    parser.add_argument('--net-interface', default='eth0')
    parser.add_argument('--modules', type=Path, default=ROOT / 'out/linux-optee-6.12/modules/lib/modules')
    parser.add_argument('--hid-device', action='append', type=identity, default=[])
    parser.add_argument('--peer', type=ipaddress.IPv4Address)
    parser.add_argument('--rate', choices=['auto', '1000', '4000', '8000'], default='8000')
    parser.add_argument('--output', type=Path, default=ROOT / 'out/usbboot-initramfs.cpio.gz')
    args = parser.parse_args()
    if args.hid_device and args.peer is None:
        parser.error('USB 合成器需要 --peer 指定控制电脑地址')
    busybox = args.busybox.read_bytes()
    arm64_static(busybox)
    if not args.modules.is_dir():
        parser.error('内核模块目录不存在')
    entries = []
    directories = set()
    def directory(name):
        parts = Path(name).parts
        for i in range(1, len(parts) + 1):
            item = '/'.join(parts[:i])
            if item not in directories:
                entries.append((item, stat.S_IFDIR | 0o755, b''))
                directories.add(item)
    def file(name, data, mode=0o644):
        if '/' in name:
            directory(name.rsplit('/', 1)[0])
        entries.append((name, stat.S_IFREG | mode, data))
    for name in ['bin', 'dev', 'proc', 'sys', 'newroot', 'lib/modules']:
        directory(name)
    file('bin/busybox', busybox, 0o755)
    for name in ['sh', 'mount', 'mkdir', 'cp']:
        entries.append(('bin/' + name, stat.S_IFLNK | 0o777, b'busybox'))
    file('init', Path(__file__).with_name('ram-init.sh').read_bytes(), 0o755)
    config = {'ROOT_DEVICE': args.root_device, 'NET_INTERFACE': args.net_interface,
              'BOARD_ADDRESS': str(args.address)}
    file('boot.conf', ''.join(f'{k}={shlex.quote(v)}\n' for k, v in config.items()).encode())
    file('authorized-key', public_key(args.authorized_key), 0o600)
    for path in sorted(args.modules.rglob('*')):
        if path.is_symlink():
            continue
        name = 'lib/modules/' + path.relative_to(args.modules).as_posix()
        if path.is_dir():
            directory(name)
        else:
            file(name, path.read_bytes())
    if args.hid_device:
        runtime = {
            'bin/vs680-hid': ROOT / 'build/usb-hid/aarch64-unknown-linux-gnu/release/vs680-hid',
            'bin/control-net': ROOT / 'build/usb-hid/control-net',
            'tools/usb-hid/hid-gadget.sh': ROOT / 'tools/usb-hid/hid-gadget.sh',
            'tools/hid-bridge/supervisor.py': ROOT / 'tools/hid-bridge/supervisor.py',
        }
        for name, path in runtime.items():
            file('hid-runtime/' + name, path.read_bytes(), 0o755)
        prefix = '/run/vs680-hid-runtime'
        command = ['/usr/bin/python3', prefix + '/tools/hid-bridge/supervisor.py',
                   '--binary', prefix + '/bin/vs680-hid', '--rate', args.rate]
        for value in args.hid_device:
            command += ['--device', value]
        network = [prefix + '/bin/control-net', str(args.address.ip), str(args.peer)]
        script = ('#!/bin/sh\n' + shlex.join(command) + ' > /run/hid-relay.log 2>&1 &\n' +
                  shlex.join(network) + ' > /run/hid-control-net.log 2>&1 &\n')
        file('hid-runtime/start.sh', script.encode(), 0o755)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(gzip.compress(pack(entries), mtime=0))
    print(args.output)

if __name__ == '__main__':
    main()
