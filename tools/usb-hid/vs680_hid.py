# SPDX-License-Identifier: GPL-2.0-only
"""UDP 控制客户端，自动恢复连接并按会话管理动作句柄。"""
import threading
import time

from hid_client import Client, CommandError, Message, Op


class Action(int):
    def __new__(cls, value, epoch):
        item=int.__new__(cls,value)
        item.epoch=epoch
        return item


class Hid:
    """网络故障时命令返回 None；connected 和 last_error 提供连接状态。"""
    def __init__(self, host, port=5681, timeout=0.25):
        if timeout<=0:
            raise ValueError('命令超时须大于零')
        self.host, self.port, self.timeout = host, port, timeout
        self._state = threading.Lock()
        self._call = threading.Lock()
        self._stop = threading.Event()
        self._wake = threading.Event()
        self._ready = threading.Event()
        self._client = None
        self._clock_offset = 0
        self.last_error = None
        self._worker = threading.Thread(target=self._connect_loop, daemon=True)
        self._worker.start()

    @property
    def connected(self):
        return self._ready.is_set()

    @property
    def epoch(self):
        with self._state:
            return self._client.epoch if self._client else None

    def wait_connected(self, timeout=2):
        return self._ready.wait(timeout)

    def now_ns(self):
        return time.monotonic_ns()+self._clock_offset

    def _connect_loop(self):
        backoff=0.1
        while not self._stop.is_set():
            try:
                client=Client(host=self.host,port=self.port,timeout=self.timeout)
            except (OSError,RuntimeError) as error:
                self.last_error=error
                self._wake.wait(backoff)
                self._wake.clear()
                backoff=min(backoff*2,2)
                continue
            with self._state:
                cancelled=self._stop.is_set()
                if not cancelled:
                    self._client=client
                    self._clock_offset=client.sock.clock_offset_ns
                    self.last_error=None
                    self._ready.set()
            if cancelled:
                client.close()
                break
            backoff=0.1
            client.failed.wait()
            with self._state:
                self._ready.clear()
                self._client=None
            with self._call:
                client.close()


    def __getattr__(self, name):
        if name.startswith('_') or not callable(getattr(Client,name,None)):
            raise AttributeError(name)
        def invoke(*args, **kwargs):
            if self._stop.is_set():
                return None
            with self._call:
                with self._state:
                    client=self._client
                if client is None:
                    return None
                values=(*args,*kwargs.values())
                if any(isinstance(value,Action) and value.epoch!=client.epoch for value in values):
                    return None
                args=tuple(int(v) if isinstance(v,Action) else v for v in args)
                kwargs={k:int(v) if isinstance(v,Action) else v for k,v in kwargs.items()}
                try:
                    result=getattr(client,name)(*args,**kwargs)
                except CommandError as error:
                    if error.status not in (6,7,8,10,11):
                        raise
                    self.last_error=error
                    return None
                except OSError as error:
                    self.last_error=error
                    with self._state:
                        self._ready.clear()
                        if self._client is client:
                            self._client=None
                    client.failed.set()
                    return None
                if name in ('force_button','force_key','scale','trigger','batch'):
                    return Action(result,client.epoch)
                return result
        return invoke

    def close(self):
        self._stop.set()
        self._wake.set()
        with self._state:
            client=self._client
        if client:
            client.failed.set()
        self._worker.join()

    def __enter__(self):
        self.wait_connected()
        return self

    def __exit__(self, *_):
        self.close()
