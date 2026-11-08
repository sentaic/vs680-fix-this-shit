// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <arm64_macros.S>
#include <arm.h>
#include <asm.S>

/*
 *1. store the input parameter x0~x7
 *2. randomazition of optee address (do later)
 *3. setup mr,rr setting
 *4. bl to _start
*/

.globl	syna_entrypoint
FUNC syna_entrypoint , :
	ldr x20, =__save_boot_regs

	stp x6, x7, [x20, #0x30]
	stp x4, x5, [x20, #0x20]
	stp x2, x3, [x20, #0x10]
	stp x0, x1, [x20, #0x0]

	b	_start
END_FUNC syna_entrypoint

DATA __save_boot_regs , :
	.long	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
END_DATA __save_boot_regs
