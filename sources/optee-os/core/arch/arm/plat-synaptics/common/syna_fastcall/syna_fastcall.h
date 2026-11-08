// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef SYNA_FASTCALL_H
#define SYNA_FASTCALL_H

void fastcall_enter_boot_stage(struct thread_smc_args *args);

void fastcall_get_mem_region_count(struct thread_smc_args *args);

void fastcall_get_mem_region_list(struct thread_smc_args *args);

void fastcall_verify_image(struct thread_smc_args *args);
#endif
