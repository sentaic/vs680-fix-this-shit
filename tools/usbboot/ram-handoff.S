/* SPDX-License-Identifier: GPL-2.0-only */
/* EL3 cold-boot handoff using the Synaptics tz_boot_arg32 interface. */
.text
.global _start
_start:
    msr daifset, #0xf
    ldr x0, =0x100800
    mov x1, xzr
    mov x2, xzr
    mov x3, xzr
    ldr x4, =0x120000
    br x4
