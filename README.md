[English](README.en.md)

使用 dev key 加载的 OP-TEE + Linux 6.12，适用于 VS680 SoC。

**注意：受原厂启动固件的签名和加载限制，当前方案依赖 USBBOOT 引导，必须依赖外部设备提供启动文件**

建议用鳄鱼夹夹住 USBBOOT 按键，或短接。

## 当前进度

简单适配。

## 编译和启动

基础编译：

```sh
bash tools/optee-port/build.sh
uv run --no-project python tools/linux-lts/build.py
```

USB 合成器：

```sh
bash tools/usb-hid/build.sh
```

RX (不完整)：

```sh
uv run --no-project python tools/linux-lts/build-media.py
bash tools/optee-port/rx-high-refresh/build-client.sh
```

构建需要 GNU、make 和 uv；USB 合成器需要 Rust 工具链。Python 环境通过 uv 管理。

RAM 启动程序默认将 `/dev/mmcblk0p16` 以 `ro,noload` 挂载。

完整流程大致：

```text
USBBOOT 模式
    → 电脑下载原厂启动固件和 RAM U-Boot
    → 加载 TF-A、OP-TEE、Linux 和启动程序
    → 关闭 BCM USB 下载接口
    → 进入 Linux
```

---

题外话：你为什么不用 rk 呢？
