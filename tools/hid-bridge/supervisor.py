#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
"""按 USB 身份启动键鼠转发，设备变化由内核事件驱动。"""
import argparse
import os
from pathlib import Path
import selectors
import re
import threading
import signal
import socket
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
USB = ROOT / 'tools/usb-hid'
BINARY = ROOT / 'build/usb-hid/aarch64-unknown-linux-gnu/release/vs680-hid'

def identity(value):
    match = re.fullmatch(r"([0-9a-fA-F]{4}):([0-9a-fA-F]{4}):([0-9a-fA-F]{2})", value)
    if match is None:
        raise argparse.ArgumentTypeError("设备格式为 VID:PID:接口号")
    return tuple(part.lower() for part in match.groups())


def devices(selected):
    found = []
    for vid, pid, interface in selected:
        matches = []
        for node in Path('/sys/class/hidraw').glob('hidraw*'):
            try:
                values = dict(line.split('=', 1) for line in (node/'device/uevent').read_text().splitlines())
                usb = (node/'device').resolve().parent
                if values.get('HID_ID', '').lower() == '0003:0000'+vid+':0000'+pid and (usb/'bInterfaceNumber').read_text().strip()==interface:
                    matches.append('/dev/'+node.name)
            except (FileNotFoundError, ValueError):
                continue
        if len(matches)!=1:
            return None
        found += ['--device', matches[0]]
    return found


def relay_log(stream):
    for line in stream:
        match=re.match(r'interface=(\d+) read_reports=',line)
        if match:
            Path('/run/hid-stats-'+match[1]+'.txt').write_text(line)
        else:
            print(line,end='',flush=True)


def restart_delay(previous, lifetime):
    return 0 if lifetime >= 30 else min(30, max(1, previous * 2))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--device', action='append', type=identity, required=True)
    parser.add_argument('--binary', type=Path, default=BINARY)
    parser.add_argument('--rate', choices=['auto', '1000', '4000', '8000'], default='8000')
    args = parser.parse_args()
    net = socket.socket(socket.AF_NETLINK, socket.SOCK_DGRAM, 15)
    net.bind((os.getpid(), 1))
    reader, writer = os.pipe2(os.O_NONBLOCK | os.O_CLOEXEC)
    signal.set_wakeup_fd(writer)
    signal.signal(signal.SIGCHLD, lambda *_: None)
    selector = selectors.DefaultSelector()
    selector.register(net, selectors.EVENT_READ)
    selector.register(reader, selectors.EVENT_READ)
    child = None
    attempted = None
    delay = 0
    restart_at = 0
    started = 0
    while True:
        paths = devices(args.device)
        now = time.monotonic()
        if child and child.poll() is not None:
            delay = restart_delay(delay, now - started)
            restart_at = now + delay
            print('relay_exit='+str(child.returncode)+' restart_delay='+str(delay), flush=True)
            attempted = None
            child = None
        if paths is not None and child is None and paths != attempted and now >= restart_at:
            attempted = paths
            started = now
            child = subprocess.Popen(['bash', str(USB/'hid-gadget.sh'), *paths, '--rate', args.rate], env=dict(os.environ, VS680_HID_BIN=str(args.binary.resolve())), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
            threading.Thread(target=relay_log,args=(child.stdout,),daemon=True).start()
        timeout = max(0, restart_at - time.monotonic()) if child is None and paths is not None and paths != attempted else None
        for event, _ in selector.select(timeout):
            if event.fileobj is net:
                message=net.recv(8192)
                if b'SUBSYSTEM=hidraw' in message:
                    attempted = None
            else:
                os.read(reader, 4096)


if __name__ == '__main__':
    main()
