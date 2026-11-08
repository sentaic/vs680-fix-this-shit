#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
"""为 Linux 6.12 构建 HDMI RX、取帧缓冲和引脚模块。"""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'tools/optee-port'
OUT = ROOT / 'build/media-optee-6.12'

def module(name, objects):
    stage = OUT / name
    stage.mkdir(parents=True, exist_ok=True)
    lines = ['ccflags-y += -I' + str(ROOT / 'sources/astra-current/linux_5_15-modules/drivers/hrx-v4l2')]
    for target, files in objects.items():
        lines.append('obj-m += ' + target + '.o')
        lines.append(target + '-y := ' + ' '.join('unit-' + f.stem + '.o' for f in files))
        for path in files:
            wrapper = stage / ('unit-' + path.name)
            body = '#include "' + str(path) + '"'+chr(10)
            if not wrapper.exists() or wrapper.read_text() != body:
                wrapper.write_text(body)
    (stage / 'Makefile').write_text(chr(10).join(lines)+chr(10))
    subprocess.run(['make','-C',str(ROOT/'sources/astra-current/linux_6_12-main'),
        'O='+str(ROOT/'build/linux-optee-6.12'),'M='+str(stage), 'ARCH=arm64',
        'CROSS_COMPILE=aarch64-linux-gnu-','LOCALVERSION=','-j4','modules'],check=True)

if __name__ == '__main__':
    names = ['loop-rx-status','rx-phy','rx-controller','rx-avi','rx-progress',
        'rx-transaction','rx-transaction-sysfs','rx-input','rx-pattern','rx-video-path']
    module('rx', {'rx-loop':[SOURCE/'loop-rx-status'/str(n+'.c') for n in names],
        'capture-buffer':[SOURCE/'loop-rx-status/capture-buffer.c']})
    module('pins', {'loop-pinctrl':[SOURCE/'loop-pinctrl/loop-pinctrl.c',
        SOURCE/'loop-pinctrl/loop-edid.c']})
