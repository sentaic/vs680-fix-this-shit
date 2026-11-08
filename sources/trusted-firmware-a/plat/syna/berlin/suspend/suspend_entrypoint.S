/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>

        .globl  suspend_entrypoint
		.globl	plat_panic_handler

func suspend_entrypoint
	/* --------------------------------------------
	 * Save the input parameters
	 * x0	:resume entrypoint
	 * x1	:power state
	 * ...
	 *---------------------------------------------
	 */
	mov	x19, x0
	mov	x20, x1
	mov	x21, x2
	mov	x22, x3

	/* ---------------------------------------------
	 * Zero out BSS sections.
	 * ---------------------------------------------
	 */
	ldr	x0, =__BSS_START__
	ldr	x1, =__BSS_SIZE__
	bl	zeromem

	/* ---------------------------------------------
	 * Setup the stack.
	 * ---------------------------------------------
	 */
	mrs	x0, mpidr_el1
	bl	plat_set_my_stack

	/* ---------------------------------------------
	 * Perform platform specific early arch. setup
	 * ---------------------------------------------
	 */
	bl	suspend_early_setup

	/* ---------------------------------------------
	 * Jump to main function.
	 * ---------------------------------------------
	 */
	mov	x0, x19
	mov	x1, x20
	mov	x2, x21
	mov	x3, x22
	bl	suspend_main

	/* ---------------------------------------------
	 * x0 contains the return address
	 * ---------------------------------------------
	 */
	ret	x0
endfunc suspend_entrypoint

func plat_panic_handler
	ret
endfunc plat_panic_handler
