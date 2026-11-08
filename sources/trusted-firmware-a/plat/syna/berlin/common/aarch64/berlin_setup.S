
/*
 * Copyright (c) 2026, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <platform_def.h>

	.globl	plat_secondary_cold_boot_setup
	.globl	plat_is_my_cpu_primary
	.globl	bl31_warm_entrypoint
	.globl	plat_warm_boot_setup

func plat_warm_boot_setup
	bl	plat_is_my_cpu_primary
	cbnz	w0, bl31_warm_entrypoint
	/* This is a cold boot on a secondary CPU */
	bl	plat_secondary_cold_boot_setup
endfunc plat_warm_boot_setup
