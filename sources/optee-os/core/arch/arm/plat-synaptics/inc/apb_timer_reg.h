// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef APB_TIMER_REG_H
#define APB_TIMER_REG_H

/* timer definitions. register offsets */
#define TMR_LOAD_COUNT			0x00
#define TMR_CURRENT_VALUE		0x04
#define TMR_CONTROL			0x08
#define TMR_EOI				0x0c
#define TMR_INT_STATUS			0x10

#define TMRS_INT_STATUS			0xa0
#define TMRS_EOI			0xa4
#define TMRS_RAW_INT_STATUS		0xa8
#define TMRS_COMP_VERSION		0xac
#define TMRS_REG_SIZE			0x14

/* register bits */
#define TMR_CONTROL_ENABLE		(1<<0)
#define TMR_CONTROL_MODE_PERIODIC	(1<<1)	/*1: periodic 0:free running */
#define TMR_CONTROL_INT_MASK		(1<<2)

#define TMR_INT_STATUS_PENDING		(1<<0)

#endif /* APB_TIMER_REG_H */
