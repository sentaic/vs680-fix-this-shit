#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-only
set -euo pipefail
root="$(cd "$(dirname "$0")/../../.." && pwd)"
out="$root/build/hdmi"
mkdir -p "$out"
common=(-std=gnu11 -Wall -Wextra -Werror -O2
    -I"$root/sources/optee-client/libteec/include")
for source in hdmi-client hdmi-mode-diagnose pool-holder; do
    aarch64-linux-gnu-gcc "${common[@]}" "$root/tools/optee-port/hdmi-dev/$source.c" \
        "$root/build/optee-client/libteec/libteec.a" -pthread -o "$out/$source"
done
