/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include <common/debug.h>
#include <lib/mmio.h>
#include "secure_register.h"
#include "secure_register_setting.h"

#define readl(a)		mmio_read_32((uintptr_t)(a))
#define writel(v, a)	mmio_write_32((uintptr_t)(a), v)

uint32_t secure_register_op(uint32_t op, uint32_t index, uint32_t *value)
{
	uint32_t reg_addr = 0;
	uint32_t mask = 0;
	uint32_t reg_value = 0;

	if (index >= TZ_SECUREREG_MAX)
		return SYNA_SECURE_REGISTER_REJECTED;

	reg_addr = sec_regs[index].reg;
	mask = sec_regs[index].mask;
	if (!reg_addr) {
		ERROR("bad address @0x%x reg.\n", index);
		return SYNA_SECURE_REGISTER_REJECTED;
	}

	if (!mask) {
		ERROR("bad mask @0x%x.\n", mask);
		return SYNA_SECURE_REGISTER_REJECTED;
	}

	if (op == SYNA_SREGISTER_READ)
		*value = (readl(reg_addr) & mask);
	else if (op == SYNA_SREGISTER_WRITE) {
		reg_value = readl(reg_addr);
		reg_value &= ~mask;
		reg_value |= (*value & mask);
		writel(reg_value, reg_addr);
	} else
		return SYNA_SECURE_REGISTER_REJECTED;

	return SYNA_SECURE_REGISTER_OK;
}
