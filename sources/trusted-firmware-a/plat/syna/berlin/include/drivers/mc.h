/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MC6_H__
#define __MC6_H__

typedef struct {
	void* mc6_base;
}mem_config_t;

void mem_set_config(mem_config_t *config);
void mem_suspend(void);
void mem_resume(void);

#endif
