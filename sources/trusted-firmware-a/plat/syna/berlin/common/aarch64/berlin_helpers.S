
/*
 * Copyright (c) 2014-2022, Arm Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <platform_def.h>

	.globl	plat_get_my_entrypoint
	.globl	plat_secondary_cold_boot_setup
	.globl	platform_mem_init
	.globl	plat_report_exception
	.globl	plat_is_my_cpu_primary
	.globl	plat_my_core_pos
	.globl	plat_crash_console_init
	.globl	plat_crash_console_putc
	.globl	plat_crash_console_flush

	.globl berlin_percpu_entrypoint

	/* -----------------------------------------------------
	 * void plat_secondary_cold_boot_setup (void);
	 *
	 * It is responsible for placing the executing secondary
	 * CPU in a platform-specific state until the primary
	 * CPU performs the necessary actions to bring it out of
	 * that state and allow entry into the OS.
	 * This function must not return.
	 * -----------------------------------------------------
	 */
func plat_secondary_cold_boot_setup
1:
#if ARM_PLAT_MT
	mrs x0, S3_0_C15_C2_7
	orr	x0, x0, #0x1
	msr	S3_0_C15_C2_7, x0
	isb
#endif
	wfi
	b	1b
endfunc plat_secondary_cold_boot_setup

	/* -----------------------------------------------------
	 * unsigned long plat_get_my_entrypoint (void);
	 *
	 * The function is responsible for distinguishing
	 * between a warm and cold reset for the current CPU
	 * using platform-specific means. If it's a warm reset,
	 * then it returns the warm reset entrypoint point
	 * provided to plat_setup_psci_ops() during BL31
	 * initialization. If it's a cold reset then this
	 * function must return zero.
	 * -----------------------------------------------------
	 */
func plat_get_my_entrypoint

	/* -----------------------------------------------------
	 * A per-cpu berlin_percpu_entrypoint is maintained to
	 * save the entrypoint address where cpu execution
	 * should start from.
	 *
	 * On a cold boot entrypoint are 0s for all CPUs. The
	 * entrypoint value can be changed in PSCI commands,
	 * e.g. on a CPU_ON event, it would change to the PSCI
	 * entrypoint.
	 *
	 * NOTE that berlin_percpu_entrypoint should always be
	 * allocated in '.data' section otherwise it would contain
	 * unknown value in BSS section on cold boot up before
	 * primary CPU zero it out
	 * -----------------------------------------------------
	 */

	bl	plat_my_core_pos
	ldr	x10, =berlin_percpu_entrypoint
	lsl	x0, x0, #3
	ldr	x0, [x10, x0]
	ret
endfunc plat_get_my_entrypoint
	/* -----------------------------------------------------
	 * void platform_mem_init (void);
	 *
	 * This function is called before any access to data is
	 * made by the firmware, in order to carry out any
	 * essential memory initialization.
	 * -----------------------------------------------------
	 */
func platform_mem_init
	ret
endfunc platform_mem_init

	/* -----------------------------------------------------
	 * void plat_report_exception(unsigned int type)
	 *
	 * Function to report an unhandled exception
	 * with platform-specific means.
	 * -----------------------------------------------------
	 */
func plat_report_exception
	ret
endfunc plat_report_exception

	/* -----------------------------------------------------
	 * unsigned int plat_is_my_cpu_primary(void);
	 *
	 * This function checks if this is the Primary CPU
	 * -----------------------------------------------------
	 */
func plat_is_my_cpu_primary
	mrs	x0, mpidr_el1
	mov_imm	x1, MPIDR_AFFINITY_MASK
	and	x0, x0, x1
	cmp	x0, #BERLIN_PRIMARY_CPU
	cset	x0, eq
	ret
endfunc plat_is_my_cpu_primary

	/* -----------------------------------------------------
	 * unsigned int plat_my_core_pos(void);
	 *
	 * Result: CorePos = CoreId + (ClusterId << 2)
	 * -----------------------------------------------------
	 */
func plat_my_core_pos
	mrs	x0, mpidr_el1
#if ARM_PLAT_MT
	mov x3, x0
#else
	/* shift MPIDR to left to make it
	 * look as if in a multi-threaded implementation.
	 */
	lsl	x3, x0, #MPIDR_AFFINITY_BITS
#endif

	/* Extract individual affinity fields from MPIDR */
	ubfx	x0, x3, #MPIDR_AFF0_SHIFT, #MPIDR_AFFINITY_BITS
	ubfx	x1, x3, #MPIDR_AFF1_SHIFT, #MPIDR_AFFINITY_BITS
	ubfx	x2, x3, #MPIDR_AFF2_SHIFT, #MPIDR_AFFINITY_BITS

	/* Compute linear position */
	mov	x4, #BERLIN_MAX_CPUS_PER_CLUSTER
	madd	x0, x2, x4, x1
	ret
endfunc plat_my_core_pos

	/* -----------------------------------------------------
	 * int plat_crash_console_init(void)
	 *
	 * Function to initialize the crash console
	 * without a C Runtime to print crash report.
	 * -----------------------------------------------------
	 */
func plat_crash_console_init
#ifdef SYNA_MP
	mov_imm	x0, UART_0_BASE
	mov_imm	x1, BERLIN_UART_CLOCK
	mov_imm	x2, BERLIN_UART_BAUDRATE
	b	console_16550_core_init
#endif
endfunc plat_crash_console_init

	/* -----------------------------------------------------
	 * int plat_crash_console_putc(int c)
	 *
	 * Function to print a character on the crash
	 * console without a C Runtime.
	 * Clobber list : x1, x2
	 * -----------------------------------------------------
	 */
func plat_crash_console_putc
#ifdef SYNA_MP
	mov_imm	x1, UART_0_BASE
	b	console_16550_core_putc
#endif
endfunc plat_crash_console_putc

func plat_crash_console_flush
	mov	w0, #0
	ret
endfunc plat_crash_console_flush

	.data
	.align 3
berlin_percpu_entrypoint:
	.quad 0
	.quad 0
	.quad 0
	.quad 0
