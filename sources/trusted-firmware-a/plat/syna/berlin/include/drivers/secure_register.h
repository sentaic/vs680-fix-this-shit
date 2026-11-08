/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SECURE_REGISTERS_H
#define SECURE_REGISTERS_H

/* secure register op */
#define SYNA_SREGISTER_READ				0xF1
#define SYNA_SREGISTER_WRITE			0xF2

/* status response */
#define SYNA_SECURE_REGISTER_OK			0x0
#define SYNA_SECURE_REGISTER_REJECTED	0x1

struct secure_register {
	uint32_t reg;
	uint32_t mask;
};

enum secure_register_id {
	SEM_INTR_ENABLE_1	=	0,
	SEM_INTR_ENABLE_2	=	1,
	SEM_INTR_ENABLE_3	=	2,
	SEM_CHK_FULL		=	3,
	SEM_POP			=	4,
	OVP_INTSTATUS		=	5,
	TSB_INTR_ENABLE_1 = 6,
	TSB_INTR_ENABLE_2 = 7,
	TSB_INTR_ENABLE_3 = 8,
	TSB_SEM_POP       = 9,
	TSB_SEM_FULL      = 10,
	FWR_INTR_ENABLE_1 = 11,
	FWR_INTR_ENABLE_2 = 12,
	FWR_INTR_ENABLE_3 = 13,
	FWR_SEM_POP       = 14,
	FWR_SEM_FULL      = 15,
	ISP_INT_STATUS    = 16,
	ISP_CORE_TOP_CTRL = 17,
	ISP_ISP_INTR_STS  = 18,
	ISP_ISP_INTR_CLR  = 19,
	ISP_MI_INTR_STS   = 20,
	ISP_MI_INTR_CLR   = 21,
	NSK_CORE_INTSTATUS	=	22,
	NSK_SOC_INTSTATUS	=	23,
	NSK_SOC_INTCLEAR	= 	24,
	DSP0_INTR_STATUS	= 	25,
	DSP1_INTR_STATUS	= 	26,
	IFCP_I2H_CTRL_STATUS	= 	27,
	TZ_SECUREREG_MAX,
};

uint32_t secure_register_op(uint32_t op, uint32_t index, uint32_t *value);

#endif /* SECURE_REGISTERS_H */
