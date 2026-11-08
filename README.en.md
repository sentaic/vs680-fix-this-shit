[中文](README.md)

OP-TEE + Linux 6.12 loaded with a dev key, for the VS680 SoC.

**Note: Due to signature and loading restrictions in the vendor boot firmware, this setup boots through USBBOOT and requires an external device to supply the boot files.**

Use an alligator clip to hold down the USBBOOT button, or short the button contacts.

## Current status

Basic port.

## Build and boot

Base build:

```sh
bash tools/optee-port/build.sh
uv run --no-project python tools/linux-lts/build.py
```

USB input synthesizer:

```sh
bash tools/usb-hid/build.sh
```

RX (incomplete):

```sh
uv run --no-project python tools/linux-lts/build-media.py
bash tools/optee-port/rx-high-refresh/build-client.sh
```

The build needs GNU tools, make and uv; the USB input synthesizer also needs a Rust toolchain. Python environments are managed with uv.

By default, the RAM boot script mounts `/dev/mmcblk0p16` with `ro,noload`.

Rough boot flow:

```text
USBBOOT mode
    → Computer downloads the vendor boot firmware and RAM U-Boot
    → Load TF-A, OP-TEE, Linux and the startup program
    → Shut down the BCM USB download interface
    → Enter Linux
```

---

By the way, why not just use rk?
