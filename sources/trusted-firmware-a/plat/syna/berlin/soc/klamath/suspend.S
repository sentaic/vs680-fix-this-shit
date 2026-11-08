/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

	.globl suspend_entry
	.globl suspend_size
suspend_entry:
#ifndef SUSPEND_BIN
#if DEBUG
	.incbin "build/berlin/klamath/debug/suspend.bin"
#else
	.incbin "build/berlin/klamath/release/suspend.bin"
#endif
#else
       .incbin SUSPEND_BIN
#endif

suspend_size:
	.word   . - suspend_entry
