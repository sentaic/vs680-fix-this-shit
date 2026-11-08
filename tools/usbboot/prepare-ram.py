#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
"""用内核、TF-A、OP-TEE 和 initramfs 生成 USBBOOT RAM 启动文件。"""
import argparse
import gzip
import json
from pathlib import Path
import re
import subprocess
import tempfile
from boot_layout import (KERNEL, RAMDISK, DTB, HANDOFF, SCRATCH, SECURE,
                         boot_parameters, secure_bundle, check_ranges, manifest)

ROOT = Path(__file__).resolve().parents[2]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--initramfs', type=Path, required=True, help='gzip 压缩的 newc initramfs')
    parser.add_argument('--output', type=Path, default=ROOT / 'out/usbboot')
    parser.add_argument('--kernel', type=Path, default=ROOT / 'out/linux-optee-6.12/Image')
    parser.add_argument('--dts', type=Path, default=ROOT / 'boards/vs680.dts')
    parser.add_argument('--memory', type=Path, default=ROOT / 'boards/vs680-memory.json')
    parser.add_argument('--bl31', type=Path, default=ROOT / 'build/atf/berlin/dolphin/release/bl31.bin')
    parser.add_argument('--optee', type=Path, default=ROOT / 'build/optee/core/tee-pager_v2.bin')
    parser.add_argument('--dtc', default=str(ROOT / 'build/linux-optee-6.12/scripts/dtc/dtc'))
    args = parser.parse_args()
    image = args.kernel.read_bytes()
    if image[56:60] != b'ARM\x64':
        parser.error('需要 ARM64 Image')
    ramdisk = gzip.decompress(args.initramfs.read_bytes())
    if not ramdisk.startswith(b'070701'):
        parser.error('需要 newc 格式 initramfs')
    with tempfile.TemporaryDirectory(prefix='vs680-handoff-') as tmp:
        obj = Path(tmp) / 'handoff.o'
        binary = Path(tmp) / 'handoff.bin'
        subprocess.run(['aarch64-linux-gnu-as', '-o', str(obj),
                        str(Path(__file__).with_name('ram-handoff.S'))], check=True)
        subprocess.run(['aarch64-linux-gnu-objcopy', '-O', 'binary', str(obj), str(binary)], check=True)
        handoff = binary.read_bytes()
    secure, copies = secure_bundle(boot_parameters(args.memory),
                                  args.bl31.read_bytes(), args.optee.read_bytes(), handoff)
    dts = args.dts.read_text()
    bootargs = ('console=ttyS0,115200 earlycon=uart8250,mmio32,0xf7fcd000 '
                'rdinit=/init maxcpus=4 loglevel=4 cma=16M net.ifnames=0')
    dts, count = re.subn(r'bootargs = "[^"]*";', f'bootargs = "{bootargs}";', dts)
    if count != 1:
        raise ValueError('设备树须包含一个 bootargs')
    dts += (f'\n/ {{ chosen {{ linux,initrd-start = <0 {RAMDISK:#x}>; '
            f'linux,initrd-end = <0 {RAMDISK + len(ramdisk):#x}>; }}; }};\n')
    dtb = subprocess.run([args.dtc, '-I', 'dts', '-O', 'dtb'], input=dts.encode(),
                         capture_output=True, check=True).stdout
    files = {'Image.gz': gzip.compress(image, mtime=0),
             'ram-smoke.cpio.gz': gzip.compress(ramdisk, mtime=0),
             'ram-smoke.dtb': dtb, 'secure-ram.bin.gz': gzip.compress(secure, mtime=0)}
    check_ranges([('kernel', KERNEL, len(image)), ('initramfs', RAMDISK, len(ramdisk)),
                  ('dtb', DTB, len(dtb)), ('handoff', HANDOFF, len(handoff)),
                  ('secure staging', SECURE, len(secure)),
                  ('USB staging', SCRATCH, max(map(len, files.values())))])
    plan = manifest(files, image, ramdisk, secure, copies)
    args.output.mkdir(parents=True, exist_ok=True)
    for name, data in files.items():
        (args.output / name).write_bytes(data)
    (args.output / 'ram-boot.json').write_text(json.dumps(plan, indent=2) + '\n')
    print(args.output)

if __name__ == '__main__':
    main()
