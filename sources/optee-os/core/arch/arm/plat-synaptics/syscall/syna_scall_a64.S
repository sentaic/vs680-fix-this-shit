/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "tee_syscall_numbers.h"
#include "trace_levels.h"
#include <arm64.h>
#include <arm64_macros.S>
#include <asm.S>
#include <generated/asm-defines.h>
#include <kernel/thread.h>
#include <tee_api_defines.h>

#define SC_REC_X0		(8 * 0)
#define SC_REC_X1		(8 * 1)
#define SC_REC_X19		(8 * 2)
#define SC_REC_X30		(8 * 3)
#define SC_REC_SIZE		(SC_REC_X30 + 8)

/*
 * uint32_t syna_scall_do_call(struct thread_scall_regs *regs, syscall_t func);
 *
 * Called from syscall_syna_entry()
 */
FUNC syna_scall_do_call , :
	sub	sp, sp, #SC_REC_SIZE
	stp	x0, x1, [sp, #SC_REC_X0]
	stp	x19, x30, [sp, #SC_REC_X19]
	mov	x19, sp

	/* Load the first 8 arguments to function */
	ldr	x9, [x19, #SC_REC_X0]
	load_xregs x9, THREAD_SCALL_REG_X0, 0, 8

	/* Call the syscall function */
	ldr	x16, [x19, #SC_REC_X1]
	blr	x16

	mov	sp, x19
	ldp	x19, x30, [sp, #SC_REC_X19]
	add	sp, sp, #SC_REC_SIZE
	ret
END_FUNC syna_scall_do_call

BTI(emit_aarch64_feature_1_and     GNU_PROPERTY_AARCH64_FEATURE_1_BTI)
