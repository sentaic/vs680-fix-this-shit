# SPDX-License-Identifier: GPL-2.0-only
"""通过 libusb 下载 VS680 Gen2 前级文件并执行 RAM U-Boot 命令。"""
import argparse
import ctypes as C
from pathlib import Path
import struct
import time
import queue
import threading


def at_console_prompt(tail):
    tail = tail.rstrip(b'\0\r\n')
    return tail == b'=> ' or tail.endswith((b'\r=> ', b'\n=> '))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--images', type=Path, required=True)
    parser.add_argument('--libusb', type=Path, required=True)
    parser.add_argument('--command', default='version')
    parser.add_argument('--seconds', type=int, default=15)
    parser.add_argument('--wait-seconds', type=int, default=0, help='Wait this long for the board to enumerate before opening it')
    parser.add_argument('--until-prompt', action='store_true', help='Finish after the command returns to the U-Boot prompt')
    parser.add_argument('--log', type=Path, required=True)
    parser.add_argument('--bootstrap', action='store_true', help='Load the selected vendor RAM images when the device is in ROM')
    parser.add_argument('--serve-only', action='store_true', help='Serve file requests initiated from the serial console')
    args = parser.parse_args()
    if len(args.command.rstrip('\n').encode()) > 255:
        parser.error('This RAM U-Boot console accepts commands up to 255 bytes; split longer operations')
    dll = C.CDLL(str(args.libusb.resolve()))
    signatures = {
        'libusb_init': ([C.POINTER(C.c_void_p)], C.c_int),
        'libusb_exit': ([C.c_void_p], None),
        'libusb_open_device_with_vid_pid': ([C.c_void_p, C.c_uint16, C.c_uint16], C.c_void_p),
        'libusb_claim_interface': ([C.c_void_p, C.c_int], C.c_int),
        'libusb_release_interface': ([C.c_void_p, C.c_int], C.c_int),
        'libusb_clear_halt': ([C.c_void_p, C.c_ubyte], C.c_int),
        'libusb_close': ([C.c_void_p], None),
        'libusb_get_device': ([C.c_void_p], C.c_void_p),
        'libusb_get_bus_number': ([C.c_void_p], C.c_uint8),
        'libusb_get_device_address': ([C.c_void_p], C.c_uint8),
        'libusb_get_device_descriptor': ([C.c_void_p, C.c_void_p], C.c_int),
        'libusb_bulk_transfer': ([C.c_void_p, C.c_ubyte, C.c_void_p, C.c_int, C.POINTER(C.c_int), C.c_uint], C.c_int),
        'libusb_interrupt_transfer': ([C.c_void_p, C.c_ubyte, C.c_void_p, C.c_int, C.POINTER(C.c_int), C.c_uint], C.c_int),
    }
    for name, (inputs, output) in signatures.items():
        fn = getattr(dll, name)
        fn.argtypes, fn.restype = inputs, output
    context = C.c_void_p()
    if dll.libusb_init(C.byref(context)):
        raise RuntimeError('libusb initialization failed')
    deadline = time.monotonic() + args.wait_seconds
    handle = None
    while True:
        handle = dll.libusb_open_device_with_vid_pid(context, 0x06cb, 0x00b1)
        if handle or time.monotonic() >= deadline:
            break
        time.sleep(0.15)
    if not handle:
        dll.libusb_exit(context)
        raise RuntimeError('Cannot open VS680 USBBOOT; check ownership and elevation')
    args.log.parent.mkdir(parents=True, exist_ok=True)
    log = args.log.open('wb', buffering=0)
    reader = None
    stop_reader = threading.Event()

    def emit(data):
        log.write(data)
        print(data.decode(errors='replace'), end='', flush=True)

    def send(endpoint, data):
        buf = C.create_string_buffer(data)
        actual = C.c_int()
        fn = dll.libusb_bulk_transfer if endpoint == 1 else dll.libusb_interrupt_transfer
        ret = fn(handle, endpoint, buf, len(data), C.byref(actual), 10000)
        if ret == -9 and actual.value == 0:
            emit(f'USB endpoint {endpoint:#x} stalled; clearing halt and retrying this packet once\n'.encode())
            cleared = dll.libusb_clear_halt(handle, endpoint)
            if cleared == 0:
                time.sleep(0.02)
                ret = fn(handle, endpoint, buf, len(data), C.byref(actual), 10000)
        if ret or actual.value != len(data):
            raise RuntimeError(f'USB endpoint {endpoint:#x}: result={ret}, transferred={actual.value}/{len(data)}')

    def subclass():
        descriptor = C.create_string_buffer(18)
        ret = dll.libusb_get_device_descriptor(dll.libusb_get_device(handle), descriptor)
        if ret:
            raise RuntimeError(f'USB descriptor failed: {ret}')
        return descriptor.raw[5]

    try:
        ret = dll.libusb_claim_interface(handle, 0)
        if ret:
            raise RuntimeError(f'USB claim failed: {ret}')
        emit(b'USB interface opened\n')
        command_sent = not args.bootstrap and not args.serve_only
        device_subclass = subclass()
        emit(f'USB device subclass={device_subclass:#04x}\n'.encode())
        rom_sent = False
        if device_subclass == 0xff:
            if not args.bootstrap:
                raise RuntimeError('Device is in ROM; --bootstrap and matching vendor RAM images are required')
            data = (args.images / 'bcm_erom.bin.usb').read_bytes()
            for offset in range(0, len(data), 1048576):
                send(1, data[offset:offset + 1048576])
            emit(f'ROM EROM sent: {len(data)} bytes\n'.encode())
            rom_sent = True
            dll.libusb_release_interface(handle, 0)
            dll.libusb_close(handle)
            handle = None
            deadline = time.monotonic() + 20
            while time.monotonic() < deadline:
                time.sleep(0.2)
                handle = dll.libusb_open_device_with_vid_pid(context, 0x06cb, 0x00b1)
                if handle:
                    if subclass() != 0xff:
                        break
                    dll.libusb_close(handle)
                    handle = None
            if not handle:
                raise RuntimeError('EROM device did not reconnect')
            ret = dll.libusb_claim_interface(handle, 0)
            if ret:
                raise RuntimeError(f'EROM USB claim failed: {ret}')
        received = queue.Queue()

        def receive_packets():
            while not stop_reader.is_set():
                buf = C.create_string_buffer(4096)
                actual = C.c_int()
                result = dll.libusb_interrupt_transfer(handle, 0x82, buf, len(buf), C.byref(actual), 200)
                if result != -7:
                    received.put((result, buf.raw[:actual.value]))
                if result not in (0, -7):
                    break

        reader = threading.Thread(target=receive_packets, daemon=True)
        reader.start()
        time.sleep(0.02)
        if command_sent:
            send(2, (args.command.rstrip('\n') + '\n').encode())
        last_size = 0
        completed = False
        console_tail = b''
        pending = b''
        boot_started = rom_sent or not (args.bootstrap and args.serve_only)
        initial_wait = args.seconds if boot_started else max(args.seconds, args.wait_seconds)
        end = time.monotonic() + initial_wait
        while time.monotonic() < end:
            try:
                ret, packet = received.get(timeout=0.2)
            except queue.Empty:
                continue
            if ret:
                if packet:
                    emit(packet)
                emit(f'USB receive result={ret}\n'.encode())
                break
            pending += packet
            console_tail = (console_tail + packet)[-64:]
            sent_at_this_prompt = False
            if not args.serve_only and not command_sent and at_console_prompt(console_tail):
                send(2, (args.command.rstrip('\n') + '\n').encode())
                command_sent = True
                sent_at_this_prompt = True
                console_tail = b''
            marker = b'i*m*g*r*q*'
            while marker in pending:
                before, request = pending.split(marker, 1)
                emit(before)
                if not request:
                    pending = marker + request
                    break
                kind = request[0]
                if kind in (0xfe, 0xff):
                    if b'\0' not in request[1:]:
                        pending = marker + request
                        break
                    filename, pending = request[1:].split(b'\0', 1)
                    name = filename.decode('ascii')
                    if not name:
                        raise RuntimeError(f'BCM requested an empty image name; packet={request[:48].hex()}')
                else:
                    pending = request[1:]
                    name = {2: 'sysinit.img', 3: 'bootloader.img', 5: 'drm_erom.img'}.get(kind, f'{kind:02x}_IMAGE')
                if not boot_started:
                    boot_started = True
                    end = time.monotonic() + args.seconds
                if name == '07_IMAGE':
                    data = struct.pack('<I', last_size)
                elif name == '06_IMAGE':
                    data = b'1-2\0'
                elif name == 'u-boot-auto-cmd.txt':
                    data = b'#skip\n'
                    last_size = len(data)
                else:
                    root = args.images.resolve()
                    path = (root / name).resolve()
                    if not path.is_relative_to(root):
                        raise RuntimeError(f'Request outside image directory: {name}')
                    data = path.read_bytes()
                    last_size = len(data)
                emit(f'USB requested {name}: {len(data)} bytes\n'.encode())
                send(1, struct.pack('<II', len(data), 0))
                time.sleep(0.01)
                for offset in range(0, len(data), 1048576):
                    send(1, data[offset:offset + 1048576])
                emit(f'USB sent {name}: {len(data)} bytes\n'.encode())
                end = time.monotonic() + args.seconds
            if marker not in pending:
                at_prompt = ((command_sent or args.serve_only) and not sent_at_this_prompt and
                             at_console_prompt(console_tail))
                # Keep a possible split request prefix for the next packet.
                keep = 0
                for n in range(1, len(marker)):
                    if pending.endswith(marker[:n]):
                        keep = n
                if len(pending) > keep:
                    emit(pending[:-keep] if keep else pending)
                pending = pending[-keep:] if keep else b''
                if args.until_prompt and at_prompt:
                    completed = True
                    break
        if pending:
            emit(pending)
        if args.until_prompt and not completed:
            raise RuntimeError('U-Boot did not return a prompt after the command')
    finally:
        stop_reader.set()
        if reader:
            reader.join()
        if handle:
            dll.libusb_release_interface(handle, 0)
            dll.libusb_close(handle)
        dll.libusb_exit(context)
        log.close()


if __name__ == '__main__':
    main()
