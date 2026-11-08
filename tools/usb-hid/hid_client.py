# SPDX-License-Identifier: GPL-2.0-only
"""板上 Linux CBOR 控制 SDK；高频共享内存接口由Rust SDK提供。"""
from __future__ import annotations

import collections
import enum
import select
import socket
import struct
import threading
import time
from dataclasses import dataclass, field

from pathlib import Path
import sys

# 优先加载分发目录内的 Python 依赖。
_bundled = Path(__file__).resolve().parent / "python-deps"
if _bundled.is_dir():
    sys.path.insert(0, str(_bundled))
import cbor2

VERSION = 2
MAX_MESSAGE = 192
MAX_PACKET = MAX_MESSAGE * 65 + 8


class Op(enum.IntEnum):
    HELLO = 1
    PING = 2
    SUBSCRIBE = 3
    PHYSICAL = 4
    OUTPUT = 5
    CAPABILITIES = 6
    TAKEOVER = 7
    RELEASE = 8
    RAW = 9
    KEY = 10
    BUTTON = 11
    MOVE = 12
    SCROLL = 13
    SCALE = 14
    ROTATE = 15
    VELOCITY = 16
    OFFSET = 17
    FORCE_KEY = 18
    FORCE_BUTTON = 19
    CANCEL = 20
    CANCEL_ALL = 21
    TRIGGER = 22
    REMOVE_TRIGGER = 23
    ACTION = 24
    DECISION = 25
    KEYS = 26
    BUTTONS = 27
    BATCH = 28
    GOODBYE = 29
    VARIABLE = 30
    PHYSICAL_STATE = 32
    COMPLETED_STATE = 33
    INPUT = 0x8001
    GAP = 0x8002
    RESPONSE = 0x8003
    MOTION_INPUT = 0x8004


@dataclass
class Message:
    op: Op
    seq: int = 0
    at: int = 0
    expires: int = 0
    id: int = 0
    args: list[int] = field(default_factory=lambda: [0, 0, 0, 0])
    target: int = 0
    flags: int = 1
    data: bytes = b""

    def to_object(self) -> dict:
        if len(self.args) != 4 or len(self.data) > 64:
            raise ValueError("命令参数或报告长度错误")
        fields = [self.seq, self.at, self.expires, self.id]
        if any(type(v) is not int or not 0 <= v <= 0xffffffffffffffff for v in fields):
            raise ValueError("时间、序号或ID超出u64范围")
        if any(type(v) is not int or not -0x80000000 <= v <= 0x7fffffff for v in self.args):
            raise ValueError("参数超出i32范围")
        if any(type(v) is not int or not 0 <= v <= 0xffffffff for v in (self.target, self.flags)):
            raise ValueError("目标或标志超出u32范围")
        obj = {0: VERSION, 1: int(Op(self.op))}
        for key, value in zip((2, 3, 4, 5), fields):
            if value:
                obj[key] = value
        if self.args != [0, 0, 0, 0]:
            obj[6] = self.args
        if self.target:
            obj[7] = self.target
        if self.flags != 1:
            obj[8] = self.flags
        data = bytes(self.data).ljust(64, b"\0")
        if self.op == Op.TRIGGER:
            if any(data[16:]):
                raise ValueError("触发动作参数之后必须为零")
            obj[10] = list(struct.unpack("<4i", data[:16]))
        else:
            if self.op in (Op.RAW, Op.INPUT):
                length = self.args[0]
            elif self.op == Op.DECISION and self.args[0] == 1:
                length = self.args[1]
            elif self.op == Op.KEYS:
                length = 32
            elif self.op == Op.RESPONSE and 1 <= self.args[1] <= 64:
                length = 64
            else:
                length = 64 if any(data) else 0
            if not 0 <= length <= 64 or any(data[length:]):
                raise ValueError("报告长度或尾部填充错误")
            if length:
                obj[9] = data[:length]
        return obj

    def pack(self) -> bytes:
        if self.op == Op.BATCH:
            raise ValueError("宏必须通过batch整体编码")
        encoded = cbor2.dumps(self.to_object(), canonical=True)
        if len(encoded) > MAX_MESSAGE:
            raise ValueError("CBOR消息超过容量")
        return encoded

    @classmethod
    def unpack(cls, data: bytes) -> "Message":
        if not 0 < len(data) <= MAX_MESSAGE:
            raise ValueError("CBOR消息长度错误")
        obj = cbor2.loads(data)
        if type(obj) is not dict or not 2 <= len(obj) <= 11:
            raise ValueError("需要CBOR映射")
        if any(type(k) is not int or k not in range(11) for k in obj):
            raise ValueError("未知CBOR字段")
        if type(obj.get(0)) is not int or obj[0] != VERSION or type(obj.get(1)) is not int:
            raise ValueError("协议版本或操作码错误")
        op = Op(obj[1])
        if op == Op.BATCH:
            raise ValueError("响应不能包含宏")
        values = [obj.get(k, 0) for k in (2, 3, 4, 5)]
        if any(type(v) is not int or not 0 <= v <= 0xffffffffffffffff for v in values):
            raise ValueError("无符号字段错误")
        args = obj.get(6, [0, 0, 0, 0])
        if type(args) is not list or len(args) != 4 or any(type(v) is not int or not -0x80000000 <= v <= 0x7fffffff for v in args):
            raise ValueError("参数数组错误")
        target, flags = obj.get(7, 0), obj.get(8, 1)
        if any(type(v) is not int or not 0 <= v <= 0xffffffff for v in (target, flags)):
            raise ValueError("目标或标志错误")
        payload = obj.get(9, b"")
        if type(payload) is not bytes or len(payload) > 64:
            raise ValueError("辅助数据必须是有界字节串")
        if op == Op.TRIGGER:
            effect = obj.get(10)
            if 9 in obj or type(effect) is not list or len(effect) != 4 or any(type(v) is not int or not -0x80000000 <= v <= 0x7fffffff for v in effect):
                raise ValueError("触发动作参数错误")
            payload = struct.pack("<4i", *effect)
        elif 10 in obj:
            raise ValueError("意外的触发参数")
        if op in (Op.RAW, Op.INPUT) and len(payload) != args[0]:
            raise ValueError("原始报告长度不一致")
        if op == Op.DECISION and args[0] == 1 and len(payload) != args[1]:
            raise ValueError("替换报告长度不一致")
        if op == Op.KEYS and len(payload) != 32:
            raise ValueError("键盘位图必须是32字节")
        # 标准规范编码对比会拒绝重复键、非最短整数、无定长容器及尾随数据。
        if cbor2.dumps(obj, canonical=True) != data:
            raise ValueError("CBOR必须使用规范编码且不得有重复键或尾随数据")
        return cls(op, *values, list(args), target, flags, payload.ljust(64, b"\0"))

class CommandError(RuntimeError):
    def __init__(self, op, status):
        self.status=status
        super().__init__('命令'+op.name+'失败，状态='+str(status))


class _NetworkSocket:
    """每条 CBOR 消息通过一个 UDP 数据报发送一次。"""
    def __init__(self,address,timeout=2):
        self.raw=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.raw.connect(address)
        self.raw.settimeout(timeout)
        self.clock_offset_ns=0

    def settimeout(self,value):
        self.raw.settimeout(value)

    def fileno(self):
        return self.raw.fileno()

    def send(self,data,flags=0):
        return self.raw.send(data)

    def recv(self,capacity):
        return self.raw.recv(capacity)

    def close(self):
        self.raw.close()


class Client:
    """一个实例由一个调用线程使用；独立心跳线程维持租约。"""
    def __init__(self, path: str = "/run/vs680-hid/control.sock", *, host: str | None = None, port: int = 5681, timeout: float = 2):
        if host is None:
            self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
            self.sock.settimeout(2)
            self.sock.connect(path)
        else:
            self.sock = _NetworkSocket((host, port), timeout)
        self.timeout = timeout
        self.failed = threading.Event()
        self.lock = threading.Lock()
        self.sequence = 0
        self.pending: collections.deque[Message] = collections.deque()
        self.stop = threading.Event()
        self.thread: threading.Thread | None = None
        try:
            started=time.monotonic_ns()
            self._send(Message(Op.HELLO, args=[2000, 0, 0, 0], flags=0).pack())
            response = Message.unpack(self.sock.recv(MAX_MESSAGE+1))
            if response.op != Op.RESPONSE or response.args[0] != 0:
                raise RuntimeError(f"控制连接被拒绝：{response.args[0]}")
            if host is not None:
                self.sock.clock_offset_ns=response.at-(started+time.monotonic_ns())//2
            self.epoch = response.id
            self.sock.settimeout(None if host is None else timeout)
            self.thread = threading.Thread(target=self._heartbeat,
                                           args=(response.args[1] / 3000,), daemon=True)
            self.thread.start()
        except BaseException:
            self.sock.close()
            raise

    def _send(self, data: bytes):
        with self.lock:
            if self.sock.send(data, getattr(socket, "MSG_DONTWAIT", 0)) != len(data):
                raise OSError("消息没有完整发送")

    def _heartbeat(self, interval: float):
        while not self.stop.wait(max(interval, .05)):
            try:
                self._send(Message(Op.PING, flags=0).pack())
            except OSError:
                self.failed.set()
                return

    def submit(self, message: Message) -> int:
        self.sequence += 1
        message.seq = self.sequence
        self._send(message.pack())
        return message.seq

    def wait(self, seq: int, timeout: float | None = None) -> Message:
        timeout = self.timeout if timeout is None else timeout
        end = time.monotonic() + timeout
        while True:
            for i, message in enumerate(self.pending):
                if message.op == Op.RESPONSE and message.seq == seq:
                    del self.pending[i]
                    return message
            remaining = end - time.monotonic()
            if remaining <= 0 or not select.select([self.sock], [], [], remaining)[0]:
                raise TimeoutError("等待控制响应超时")
            data = self.sock.recv(MAX_MESSAGE+1)
            if not data:
                raise ConnectionError("控制会话已断开")
            if len(self.pending) >= 1024:
                raise BufferError("观察事件积压，请及时poll消费")
            self.pending.append(Message.unpack(data))

    def request(self, op: Op, *, target=0, args=None, data=b"", ident=0, at=0,
                expires=0, check=True) -> Message:
        message = Message(op, target=target, args=args or [0, 0, 0, 0], data=data,
                          id=ident, at=at, expires=expires)
        result = self.wait(self.submit(message))
        if check and result.args[0] not in (0, 1, 2):
            raise CommandError(op,result.args[0])
        return result

    def poll(self, timeout=.1) -> list[Message]:
        out = list(self.pending)
        self.pending.clear()
        if not out and select.select([self.sock], [], [], timeout)[0]:
            data = self.sock.recv(MAX_MESSAGE+1)
            if not data:
                raise ConnectionError("控制会话已断开")
            out.append(Message.unpack(data))
        return out

    def subscribe(self, enabled=True):
        return self.request(Op.SUBSCRIBE, args=[int(enabled), 0, 0, 0])

    def capabilities(self, target=0):
        result = self.request(Op.CAPABILITIES, target=target)
        total = int.from_bytes(result.data[2:4], "little")
        lengths = {}
        for offset in range(0, total, 30):
            if offset:
                result = self.request(Op.CAPABILITIES, target=target, args=[offset, 0, 0, 0])
            for i in range(min(30, total-offset)):
                lengths[result.data[4+2*i]] = result.data[5+2*i]
        return {"keyboard_id": result.data[0] if result.args[1] & 1 else None,
                "mouse_id": result.data[1] if result.args[1] & 2 else None,
                "report_lengths": lengths, "interfaces": result.args[3]}

    def snapshot(self, target: int, report_id: int, output=False):
        result = self.request(Op.OUTPUT if output else Op.PHYSICAL, target=target,
                              args=[report_id, 0, 0, 0], check=False)
        return None if result.args[0] == 9 else result.data[:result.args[1]]

    def takeover(self, target=0, *, external=False, xy=False, budget_us=10000, pass_on_timeout=False):
        if xy and not external:
            raise ValueError("二维输入模式需要external=True")
        return self.request(Op.TAKEOVER, target=target,
                            args=[3 if xy else 2 if external else 0, budget_us, int(pass_on_timeout), 0])

    def release(self, target=0):
        return self.request(Op.RELEASE, target=target)

    def now_ns(self):
        """返回板端单调时间估计，用于远程定时动作；误差包含连接往返不对称。"""
        return time.monotonic_ns()+getattr(self.sock,'clock_offset_ns',0)


    def force_button(self, target: int, button: int, down: bool, duration_ms=0, priority=0):
        return self.request(Op.FORCE_BUTTON,target=target,
                            args=[button,int(down),duration_ms,priority]).id

    def force_key(self, target: int, usage: int, down: bool, duration_ms=0, priority=0):
        return self.request(Op.FORCE_KEY, target=target,
                            args=[usage, int(down), duration_ms, priority]).id

    def scale(self, target: int, x: float, y: float | None = None, duration_ms=0):
        import math
        y = x if y is None else y
        if not all(math.isfinite(v) and abs(v) <= 64 for v in (x, y)):
            raise ValueError("缩放系数必须在-64..64内")
        return self.request(Op.SCALE, target=target,
                            args=[round(x * 65536), round(y * 65536), duration_ms, 0]).id

    def trigger(self, source: int, usage_page: int, usage: int, edge: int,
                effect: Op, effect_args: list[int], target: int):
        """edge为1按下、2松开、3持续按住；当前支持Force、Scale、Rotate、Velocity、Offset。"""
        return self.request(Op.TRIGGER, target=target,
                            args=[source, usage_page << 16 | usage, edge, int(effect)],
                            data=struct.pack("<4i", *effect_args)).id

    def batch(self, steps: list[Message], ident=0):
        if not 1 <= len(steps) <= 64:
            raise ValueError("宏必须有1..64步")
        self.sequence += 1
        header = Message(Op.BATCH, seq=self.sequence, id=ident)
        if any(step.op == Op.BATCH for step in steps):
            raise ValueError("不允许嵌套宏")
        obj = header.to_object()
        obj[11] = [step.to_object() for step in steps]
        packet = cbor2.dumps(obj, canonical=True)
        if len(packet) > MAX_PACKET:
            raise ValueError("宏超过消息容量")
        self._send(packet)
        result = self.wait(header.seq)
        if result.args[0] != 1:
            raise CommandError(Op.BATCH,result.args[0])
        return result.id

    def cancel(self, ident=0):
        return self.request(Op.CANCEL if ident else Op.CANCEL_ALL, ident=ident)

    def close(self):
        if self.stop.is_set():
            return
        self.stop.set()
        if self.thread:
            self.thread.join(timeout=1)
        try:
            self._send(Message(Op.GOODBYE, flags=0).pack())
        except OSError:
            pass
        self.sock.close()

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.close()
