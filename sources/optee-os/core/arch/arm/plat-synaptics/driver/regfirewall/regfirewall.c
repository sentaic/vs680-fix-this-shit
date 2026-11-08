// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <config.h>
#include <string.h>
#include "regfirewall.h"
#include "io.h"
#include "soc.h"
#include <types_ext.h>
#include <stdint.h>
#include <assert.h>

#pragma weak rf_set_window
#pragma weak rf_lock
#pragma weak rf_buffer_registers

#ifndef MEMMAP_SOC_REG_BASE
#define MEMMAP_SOC_REG_BASE	0xf7ca0000
#endif

#ifdef CONFIG_RF_WINDOW_NUM
#define RF_WINDOW_NUM		CONFIG_RF_WINDOW_NUM
#else
#define RF_WINDOW_NUM		32
#endif

#ifdef RA_SOC_SecureRegionCtrl0
#define RF_WINDOW_START		RA_SOC_SecureRegionCtrl0
#else
#define RF_WINDOW_START		RA_SOC_SecureRegionCtrl
#endif

#define RF_REG_BASE		(MEMMAP_SOC_REG_BASE + RF_WINDOW_START)

#define RF_WINDOW_REG_SIZE	(16)

#define RF_WINDOW_PERM_WRITE	(0x0)
#define RF_WINDOW_PERM_READ	(0x4)
#define RF_WINDOW_BASE		(0x8)
#define RF_WINDOW_MASK		(0xC)

void bcm_add_bg_check_register(uint32_t addr, uint32_t data);

static void rf_writel(uint32_t v, uintptr_t a)
{
	io_write32(a, v);
#ifndef CONFIG_RF_LOCK
	bcm_add_bg_check_register(a, v);
#endif
}

int rf_set_window(int id, uint32_t base, uint32_t mask, int perm)
{
	assert(0 <= id && id < RF_WINDOW_NUM);

	/* set base & mask*/
	rf_writel(base, RF_REG_BASE +
			RF_WINDOW_BASE + id * RF_WINDOW_REG_SIZE);

	rf_writel(mask, RF_REG_BASE +
			RF_WINDOW_MASK + id * RF_WINDOW_REG_SIZE);

	/* set permission */
	rf_writel(TZ_IS_NNW(perm) ? 1 : 0,
			RF_REG_BASE +
			RF_WINDOW_PERM_WRITE + id * RF_WINDOW_REG_SIZE);

	rf_writel(TZ_IS_NNR(perm) ? 1 : 0,
			RF_REG_BASE +
			RF_WINDOW_PERM_READ + id * RF_WINDOW_REG_SIZE);

	return 0;
}

/** set register firwall window.
 * for size, only 2^n can be supported.
 */
int rf_set_window_by_size(int id, uint32_t base, uint32_t size, int perm)
{
	unsigned int mask = size -1;
	if (0 != (size & mask))
		return -1;

	return rf_set_window(id, base, ~mask, perm);
}

int rf_reset_window(int id)
{
	return rf_set_window(id, 0, 0, 0);
}

int rf_lock(void)
{
	/* before bg5ct, there is no register firewall lock */
	return 0;
}

#ifndef CONFIG_RF_LOCK
static char rf_buffer[RF_WINDOW_NUM * RF_WINDOW_REG_SIZE];
static int rf_buffed_flag = 0;
#endif

void rf_buffer_registers(void)
{
#ifndef CONFIG_RF_LOCK
	void *buf = rf_buffer;

	memcpy(buf, (void*)(MEMMAP_SOC_REG_BASE + RF_WINDOW_START),
			RF_WINDOW_NUM * RF_WINDOW_REG_SIZE);
	rf_buffed_flag = 1;
#endif
}

int rf_check_changed(void)
{
#ifndef CONFIG_RF_LOCK
	int i;
	unsigned int tmp;
	int len = RF_WINDOW_NUM * RF_WINDOW_REG_SIZE;
	vaddr_t base = MEMMAP_SOC_REG_BASE + RF_WINDOW_START;
	void *buf = rf_buffer;

	if (!rf_buffed_flag)
		return 0;
	for (i = 0; i < len; i += 4) {
		tmp = io_read32(base + i);
		if (tmp != *(unsigned int*)((vaddr_t)buf + i)) {
			return 1;
		}
	}
#endif
	return 0;
}
