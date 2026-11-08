#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-only
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../../.." && pwd)"
out="$root/build/rx-pixel-check"
mkdir -p "$out"
aarch64-linux-gnu-gcc -std=gnu11 -O2 -Wall -Wextra -Werror -static \
    -I"$root/sources/optee-client/libteec/include" -I"$here" \
    "$here/verify-clock-frame.c" "$here/rx-timing-snapshot.c" \
    "$here/rx-capture-config.c" "$here/clock-frame-420.c" \
    "$root/build/optee-client/libteec/libteec.a" -pthread -o "$out/verify-clock-frame"
printf '%s\n' "$out/verify-clock-frame"
