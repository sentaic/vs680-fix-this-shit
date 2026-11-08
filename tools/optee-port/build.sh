#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-only
set -euo pipefail
root="$(cd "$(dirname "$0")/../.." && pwd)"
if [[ ! -x "$root/.venv/bin/python" ]]; then
    uv venv "$root/.venv"
fi
uv pip sync --python "$root/.venv/bin/python" "$root/tools/optee-port/requirements.txt"
export PATH="$root/.venv/bin:$PATH"
jobs="${JOBS:-4}"
out="$root/build/optee"
kit="$out/export-ta_arm64"
ta_source="$root/sources/astra-current/tee-optee_dev/ta/display/ta"
common=(PLATFORM=synaptics PLATFORM_FLAVOR=dolphin
    CROSS_COMPILE64=aarch64-linux-gnu- CROSS_COMPILE32=arm-linux-gnueabihf-
    ta-targets=ta_arm64 O="$out" CFG_SECURE_DATA_PATH=n CFG_SYNA_TA=n
    CFG_SYNA_BOOT_PARAM_MAX_SIZE=0x1c00 CFG_REE_FS=n CFG_REE_FS_TA=n
    CFG_RPMB_FS=n CFG_RPMB_WRITE_KEY=n CFG_TEE_CORE_LOG_LEVEL=3
    CFG_CORE_HEAP_SIZE=0xA00000 CFG_TEE_RAM_VA_SIZE=0x2000000
    CFG_TZDRAM_START=0x00160000 CFG_TZDRAM_SIZE=0x03EA0000
    CFG_WITH_SOFTWARE_PRNG=n)
make -C "$root/sources/optee-os" -j"$jobs" "${common[@]}" CFG_EARLY_TA=n ta_dev_kit
make -C "$root/tools/optee-port/ta" -j"$jobs" TA_DEV_KIT_DIR="$kit" \
    CROSS_COMPILE=aarch64-linux-gnu- O="$root/build/ta/smoke"
make -C "$ta_source/dhub" -f Makefile.op -j"$jobs" TA_DEV_KIT_DIR="$kit" \
    CROSS_COMPILE=aarch64-linux-gnu- O="$root/build/ta/dhub" CFG_SYNA_PTA=n
make -C "$ta_source/dhub" -f Makefile.op -j"$jobs" TA_DEV_KIT_DIR="$kit" \
    CROSS_COMPILE=aarch64-linux-gnu- O="$root/build/ta/capture" CFG_SYNA_PTA=n \
    CFG_LOOP_RX_CAPTURE=y TA_UUID=0b564e41-93da-4fd8-b2f9-40985672d128
make -C "$ta_source/fastlogo" -f Makefile.op -j"$jobs" TA_DEV_KIT_DIR="$kit" \
    CROSS_COMPILE=aarch64-linux-gnu- O="$root/build/ta/fastlogo" \
    CFG_SYNA_PTA=n SoC_Ver=dolphin
make -C "$root/tools/optee-port/hdmi-dev/ta" -j"$jobs" TA_DEV_KIT_DIR="$kit" \
    CROSS_COMPILE=aarch64-linux-gnu- O="$root/build/ta/hdmi" CFG_HDMI_SHOW_TIMING=n
early=(
    "$root/build/ta/hdmi/4156150c-fa24-4acc-9c5a-7f788785473f.stripped.elf"
    "$root/build/ta/smoke/68680001-1011-4000-8000-000000000001.stripped.elf"
    "$root/build/ta/capture/0b564e41-93da-4fd8-b2f9-40985672d128.stripped.elf"
    "$root/build/ta/dhub/1316a183-894d-43fe-9893-bb946ae103f3.stripped.elf"
    "$root/build/ta/fastlogo/1316a183-894d-43fe-9893-bb946ae103f5.stripped.elf"
)
make -C "$root/sources/optee-os" -j"$jobs" "${common[@]}" \
    CFG_EARLY_TA=y "EARLY_TA_PATHS=${early[*]}" all
make -C "$root/sources/trusted-firmware-a" -j"$jobs" \
    BUILD_BASE="$root/build/atf" DEBUG=0 CROSS_COMPILE=aarch64-linux-gnu- \
    PLAT=berlin TARGET_SOC=dolphin SPD=opteed SYNA_GENX=1 bl31
make -C "$root/sources/optee-client" -j"$jobs" CROSS_COMPILE=aarch64-linux-gnu- \
    O="$root/build/optee-client" build-libteec
