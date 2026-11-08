# SPDX-License-Identifier: GPL-2.0-only
"""生成 Synaptics 启动参数和 RAM 加载命令。"""
import json
import struct
import zlib

KERNEL = 0x04C00000
RAMDISK = 0x0CA00000
DTB = 0x15A00000
HANDOFF = 0x06000000
SCRATCH = 0x07000000
SECURE = 0x08000000
BOOT_ARGS = 0x00100800
BOOT_PARAMS = 0x00101000
BL31 = 0x00120000
OPTEE = 0x00160000

def boot_parameters(path):
    rows = json.loads(path.read_text())
    if not 1 <= len(rows) <= 16:
        raise ValueError('启动内存区域数须为 1..16')
    regions = bytearray()
    for row in rows:
        name = row['name'].encode('ascii')
        base, size, attr = (int(row[k], 0) for k in ('base', 'size', 'attr'))
        userdata = [int(v, 0) for v in row['userdata']]
        if not 0 < len(name) < 32 or size <= 0 or base + size > 0x100000000 or len(userdata) != 4:
            raise ValueError('启动内存区域格式错误')
        # 对应公开 mem_region.h 的 name、base、size、attr、reserved 和 userdata。
        regions += struct.pack('<32s8I', name, base, size, attr, 0, *userdata)
    vendors = [('SYNAPTIC', 0)] + [(f'VENDOR{i}', 0x564E4430 + i) for i in range(1, 6)]
    vendor_info = struct.pack('<2I', 0, 128)
    vendor_info += b''.join(struct.pack('<16sI', n.encode(), v) for n, v in vendors)
    body = b''
    for ident, data in [(0x80000002, regions), (0x80000009, vendor_info),
                        (0x8000000C, struct.pack('<I', 0))]:
        body += struct.pack('<4I', 16 + len(data), ident, 0, 0) + data
    return struct.pack('<4I', 0x545A2A42, 16 + len(body), 0, 0) + body

def secure_bundle(parameters, bl31, optee, handoff):
    args = struct.pack('<8I', 0, BOOT_PARAMS, OPTEE, KERNEL, DTB, 0, 0, 0)
    parts = [(BOOT_ARGS, args), (BOOT_PARAMS, parameters), (BL31, bl31),
             (OPTEE, optee), (HANDOFF, handoff)]
    check_ranges([(hex(address), address, len(data)) for address, data in parts])
    if len(parameters) > 0x1C00 or len(bl31) > OPTEE - BL31 or len(optee) > 0x04100000 - OPTEE:
        raise ValueError('安全侧组件超出预留地址范围')
    bundle = bytearray()
    copies = []
    for address, data in parts:
        if not data:
            raise ValueError(f'空启动组件：{address:#x}')
        copies.append((SECURE + len(bundle), address, len(data)))
        bundle += data
        bundle += bytes(-len(bundle) % 16)
    return bytes(bundle), copies

def check_ranges(ranges):
    for i, (name, start, size) in enumerate(ranges):
        if start < 0 or size <= 0 or start + size > 0x80000000:
            raise ValueError(f'RAM 地址范围错误：{name}')
        for other, address, length in ranges[i + 1:]:
            if max(start, address) < min(start + size, address + length):
                raise ValueError(f'RAM 地址重叠：{name}、{other}')

def crc(data):
    return f'{zlib.crc32(data):08x}'

def manifest(files, image, ramdisk, secure, copies):
    steps = []
    def load(name, plain=None, address=None):
        data = files[name]
        command = f'usbload {name} {SCRATCH:x}; crc32 {SCRATCH:x} {len(data):x}'
        expected = [crc(data)]
        if plain is not None:
            command += f'; unzip {SCRATCH:x} {address:x}; crc32 {address:x} {len(plain):x}'
            expected.append(crc(plain))
        steps.append({'command': command, 'expected_crc': expected})
    load('secure-ram.bin.gz', secure, SECURE)
    steps.append({'command': '; '.join(f'cp.b {src:x} {dst:x} {size:x}' for src, dst, size in copies),
                  'expected_crc': []})
    load('Image.gz', image, KERNEL)
    load('ram-smoke.cpio.gz', ramdisk, RAMDISK)
    load('ram-smoke.dtb')
    dtb = files['ram-smoke.dtb']
    steps.append({'command': f'cp.b {SCRATCH:x} {DTB:x} {len(dtb):x}; crc32 {DTB:x} {len(dtb):x}',
                  'expected_crc': [crc(dtb)]})
    steps.append({'command': '; '.join(f'crc32 {dst:x} {size:x}' for _, dst, size in copies),
                  'expected_crc': [crc(secure[src - SECURE:src - SECURE + size]) for src, _, size in copies]})
    if any(len(step['command'].encode()) > 255 for step in steps):
        raise ValueError('USB 命令超过 255 字节')
    return {'steps': steps, 'handoff': f'dcache off; icache off; go {HANDOFF:x}',
            'disable_bcm_usb': True}
