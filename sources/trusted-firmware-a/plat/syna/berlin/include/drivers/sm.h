/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SM_H__
#define __SM_H__

typedef struct {
	void*	queue_base;
	void*	sm_ctrl_base;
}sm_config_t;

void sm_set_config(sm_config_t *config);
void sm_do_system_warmdown(unsigned int power_state);
void sm_do_system_warmup(unsigned int power_state);
void sm_do_system_reset(void);
void sm_do_system_off(void);

#endif
