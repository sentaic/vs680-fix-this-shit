#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-only
set -euo pipefail
npu_dir="$(cd "$(dirname "$0")" && pwd)"
workspace="$(cd "$npu_dir/../../.." && pwd)"
npu_build="${VS680_NPU_BUILD:-$workspace/build/npu-optee-6.12}"
driver="$workspace/sources/astra-current/synap-vsi_npu_driver"
kernel="${VS680_KERNEL_SOURCE:-$workspace/sources/astra-current/linux_6_12-main}"
headers="${VS680_KERNEL_BUILD:-$workspace/build/linux-optee-6.12}"
stage="$npu_build/driver"
mkdir -p "$stage/src"
# 将编译单元放进构建目录，目标文件在此生成。
for source in "$driver/kernel/src/"*.c "$driver/kernel/src/"*.h; do
  target="$stage/src/$(basename "$source")"
  if ! cmp -s "$source" "$target"; then cp "$source" "$target"; fi
done
if ! cmp -s "$driver/lib/ebg_file/ebg_file.c" "$stage/ebg_file.c"; then
  cp "$driver/lib/ebg_file/ebg_file.c" "$stage/"
fi
cat > "$stage/Makefile" <<EOF
obj-m += synap-optee.o
synap-optee-y := src/synap_kernel.o src/synap_kernel_drv.o src/synap_kernel_stats.o src/synap_mem.o src/synap_profile.o src/synap_kernel_ca_optee.o ebg_file.o
ccflags-y += -DCONFIG_OPTEE=1 -I$stage/src -I$driver/kernel/inc -I$driver/lib/ta_interface -I$driver/lib/ebg_file -I$kernel/drivers/staging/android/ion -include $npu_dir/linux54-compat.h
EOF
make -C "$kernel" O="$headers" ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- \
  M="$stage" -j "${JOBS:-2}" modules
