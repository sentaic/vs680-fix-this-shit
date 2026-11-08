// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _MZ_MPT_H_
#define _MZ_MPT_H_

void mpt_suspend(void);

void mpt_resume(void);

int mpt_configure_region(unsigned int start_addr, unsigned int size, unsigned int rule);

int mpt_get_rule(unsigned int start_addr, unsigned int size, unsigned int *rule);

unsigned int mpt_get_granularity(void);

unsigned int mpt_get_num_ctrlbit(void);

unsigned int mpt_get_num_rule(void);

void mpt_cache_flush(void);

void mpt_cache_enable(void);

void mpt_cache_disable(void);
#endif /* _MZ_MPT_H_ */
