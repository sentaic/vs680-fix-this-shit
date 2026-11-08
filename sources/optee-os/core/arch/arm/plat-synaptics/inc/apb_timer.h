// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef APB_TIMER_H
#define APB_TIMER_H

enum timer_mode {
	TIMER_MODE_FREE_RUNNING,
	TIMER_MODE_PERIODIC,
	TIMER_MODE_ONESHOT,
	TIMER_MODE_MAX
};

/* start timer in the specified mode, and umask interrupt */
void timer_start(uint32_t base, enum timer_mode mode, uint32_t period);
void timer_stop(uint32_t base);
/* read current counter, start from 0 */
uint32_t timer_read_count(uint32_t base);

/* check whether there is a pending interupt */
bool timer_check_interrupt(uint32_t base);
void timer_clear_interrupt(uint32_t base);
void timer_enable_interrupt(uint32_t base);
void timer_disable_interrupt(uint32_t base);

TEE_Result secure_timer_setup(void);
TEE_Result secure_timer_stop(void);

#endif /* APB_TIMER_H */
