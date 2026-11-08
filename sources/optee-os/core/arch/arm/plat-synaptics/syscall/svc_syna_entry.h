// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef SVC_SYNA_ENTRY_H
#define SVC_SYNA_ENTRY_H

TEE_Result syscall_syna_entry(size_t num, uint64_t x0,
				uint64_t x1, uint64_t x2, uint64_t x3,
				uint64_t x4, uint64_t x5, uint64_t x6);

#endif /*SVC_SYNA_ENTRY_H*/
