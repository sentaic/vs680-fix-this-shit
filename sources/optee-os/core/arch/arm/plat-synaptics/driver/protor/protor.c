// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <config.h>
#include <string.h>
#include "protor.h"
#include "io.h"
#include "soc.h"
#include "prot_id.h"
#include "tz_boot_param_value.h"
#include <trace.h>
#include <assert.h>
#include <mm/core_memprot.h>


#define PROTOR_REG_BASE 	(MEMMAP_SOC_REG_VA_BASE + RA_SOC_protOR)

#define PROTOR_PROT_MASK	0x0000000f

#define PROTOR_WRITE_OVERRIDE	0x00000001
#define PROTOR_WRITE_VALUE	0x00000002
#define PROTOR_READ_OVERRIDE	0x00000004
#define PROTOR_RED_VALUE	0x00000008
#define PROTOR_LOCK_VALUE	0x00000010

#define PROTOR_REG_NUM		PROT_ID_MAX

#ifdef CFG_BCM
void bcm_add_bg_check_register(uintptr_t addr, uint32_t data);
#else
static void bcm_add_bg_check_register(uintptr_t __unused addr, uint32_t __unused data)
{
	return;
}
#endif

#define pt_writel(v, a) do {               \
		io_write32(a, v);                    \
		bcm_add_bg_check_register(a, v); \
	} while(0)

int prot_reset(int id)
{
	vaddr_t addr = PROTOR_REG_BASE + id * 4;
	io_write32(addr, 0);
	return 0;
}

int prot_set(int id, int perm)
{
	unsigned int val = perm & PROTOR_PROT_MASK;
	vaddr_t addr = PROTOR_REG_BASE + id * 4;

	val |= PROTOR_LOCK_VALUE;

	pt_writel(val, addr);
	assert(val == io_read32(addr));

	return 0;
}

static unsigned int prot_buffer[32];
static int prot_buffed_flag = 0;

void prot_buffer_registers(void)
{
	   void* addr = (void *)(PROTOR_REG_BASE);
       memcpy(prot_buffer, addr, PROTOR_REG_NUM << 2);
       prot_buffed_flag = 1;
}

int prot_check_changed(void)
{
	int i;
	unsigned int tmp;
	vaddr_t base = PROTOR_REG_BASE;
	void *buf = prot_buffer;
	int id;

	if (!prot_buffed_flag)
		return 0;

	for (i = 0; i < master_setting_num; i++) {
		id = master_setting_param[i].id;
		tmp = io_read32(base + id * 4);
		if (tmp != *(unsigned int*)((vaddr_t)buf + id * 4)) {
			return 1;
		}
	}
	return 0;
}

void prot_dump(void)
{
#if 0
	// disable temporary
	regdmp(PROTOR_REG_BASE, PROTOR_REG_NUM);
#endif
}
