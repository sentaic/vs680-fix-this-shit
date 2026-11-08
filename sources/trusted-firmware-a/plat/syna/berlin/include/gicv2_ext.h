/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GICV2_EXT_H
#define GICV2_EXT_H

typedef struct {
	unsigned int int_type;
	unsigned int priority;
	unsigned int icfg;
	struct {
		unsigned int mode;
		unsigned int target;
	};
} irq_status_t;

void plat_ic_save_interrupt_status(unsigned int id, irq_status_t *is);

void plat_ic_setup_wakeup_interrupt(unsigned int id, unsigned int level);

void plat_ic_resume_wakeup_interrupt(unsigned int id, irq_status_t *is);

#endif
