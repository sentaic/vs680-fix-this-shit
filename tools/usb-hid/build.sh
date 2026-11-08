#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-only
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../.." && pwd)"
out="$root/build/usb-hid"
cargo build --manifest-path "$here/Cargo.toml" --target-dir "$out" \
    --target aarch64-unknown-linux-gnu --release --locked \
    --config 'target.aarch64-unknown-linux-gnu.linker="aarch64-linux-gnu-gcc"' \
    --lib --bin vs680-hid --bin hidctl
aarch64-linux-gnu-gcc -O2 -Wall -Wextra -Werror \
    "$root/tools/hid-bridge/control_net.c" -o "$out/control-net"
printf '%s\n' "$out/aarch64-unknown-linux-gnu/release/vs680-hid" "$out/control-net"
