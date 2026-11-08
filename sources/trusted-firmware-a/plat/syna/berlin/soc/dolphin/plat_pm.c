/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <lib/bakery_lock.h>
#include <lib/mmio.h>
#include <drivers/arm/gicv2.h>
#include <string.h>
#include <assert.h>
#include <berlin_private.h>
#include <drivers/delay_timer.h>
#include "cpu_wrp.h"
#include "sm.h"
#include "Galois_memmap.h"
#include "cpu_wrp.h"

#define RESET_nCPUPORESET_0	0
#define PWRSW_CNTRL1_I         0
#define PWRSW_CNTRL2_I         1
#define PWRSW_ACK1_O           2
#define PWRSW_ACK2_O           3
#define nCPU_ISO_MODE_EN       4

#define REBOOT_PARAM_ADDR               0xF7F9EC00//(SOC_SM_MEMMAP_ITCM_BASE + SOC_SM_MEMMAP_ITCM_SIZE - 0x1400)

int berlin_soc_pwr_domain_on(unsigned int target_cpu)
{
	unsigned int val;

	/* disable CPMU */
	mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 0);

	/* assert reset */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu);
	val &= ~(1 << RESET_nCPUPORESET_0);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu, val);

	/* PWRSW_CNTRL1_I */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val |= (1 << PWRSW_CNTRL1_I);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	udelay(100);

	/* PWRSW_CNTRL2_I */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val |= (1 << PWRSW_CNTRL2_I);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);
	/* turn off isolation cells */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val |= (1 << nCPU_ISO_MODE_EN);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);
	udelay(100);

	/* release reset */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu);
	val |= (1 << RESET_nCPUPORESET_0);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu, val);

	return PSCI_E_SUCCESS;
}


int berlin_soc_pwr_domain_off(unsigned int target_cpu)
{
	unsigned int val;

	/* turn on isolation cells */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val &= ~(1 << nCPU_ISO_MODE_EN);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	/* PWRSW_CNTRL1_I */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val &= ~(1 << PWRSW_CNTRL1_I);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	/* PWRSW_CNTRL2_I */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val &= ~(1<< PWRSW_CNTRL2_I);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	return PSCI_E_SUCCESS;
}

int berlin_soc_pwr_enable_pmu(unsigned int target_cpu)
{
	/* enable PMU */
	mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 1);

	return PSCI_E_SUCCESS;
}


int berlin_soc_pwr_domain_suspend_finish(const psci_power_state_t *target_state)
{
	unsigned long target_cpu;
	unsigned int val;

	/* Get the linear id for this cpu */
	target_cpu = plat_my_core_pos();

	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	/* PWRSW_CNTRL1_I */
	val |= (1 << PWRSW_CNTRL1_I);

	/* PWRSW_CNTRL2_I */
	val |= (1 << PWRSW_CNTRL2_I);
	/* turn off isolation cells */
	val |= (1 << nCPU_ISO_MODE_EN);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	/* disable PMU */
	mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 0);

	return PSCI_E_SUCCESS;

}

void berlin_soc_init(void)
{
	sm_config_t sm_config;
	int i;

	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CFG, 0xf);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CpuPmuPwrSwDly, 250);
	for (i = 1; i < PLATFORM_CORE_COUNT; i++) {
		unsigned int val;
		uintptr_t addr;
		/* assert reset */
		addr = MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0  + i * 4;
		val = mmio_read_32(addr);
		val &= ~(1 << RESET_nCPUPORESET_0);
		mmio_write_32(addr, val);
	}

	sm_config.queue_base = (void *)SM_SEND_MSGQ;
	sm_config.sm_ctrl_base = (void *)SMREG_BASE;
	sm_set_config(&sm_config);
}

void __dead2 berlin_system_off(void)
{
	INFO("Berlin System Off .......\n");
	sm_do_system_off();
	while (1);
	ERROR("Berlin System Off: should not be here.\n");
	panic();
}

void __dead2 berlin_system_reset(void)
{
	INFO("Berlin System Reset ......\n");
	sm_do_system_reset();
	while (1);
	ERROR("Berlin System reset: should not be here.\n");
	panic();
}


int berlin_system_reset2(int is_vendor,
			int reset_type, u_register_t cookie)
{
	INFO("Berlin System Reset2: reset_type = 0x%x ......\n", reset_type);

	//fix me
	mmio_write_32(REBOOT_PARAM_ADDR, reset_type);

	sm_do_system_reset();
	while (1);
	/*
	 * berlin_system_reset cannot return (it is a __dead function),
	 * but berlin_system_reset2 has to return some value, even in
	 * this case.
	 */
	ERROR("Berlin System reset2: should not be here.\n");

	return 0;
}

extern unsigned char *nagra_boot_p;
extern unsigned int suspend_entry[];
extern unsigned int suspend_size;
static unsigned char sram_backup[SRAM_BACKUP_SIZE];

typedef void (*suspend_handler_t)(unsigned long, unsigned int, void *);

void berlin_suspend_to_sram(unsigned long sec_entrypoint,
				unsigned int power_state)
{
	suspend_handler_t suspend_func;

	/* Save the MPT sram to ddr */
	memcpy(sram_backup, (void *)SUSPEND_SRAM_BASE, SRAM_BACKUP_SIZE);

	/*
	 * Copy code to SRAM, and jump to it later
	 */
	memcpy((void *)SUSPEND_SRAM_BASE,
		(const void *)suspend_entry, suspend_size);

	flush_dcache_range(SUSPEND_SRAM_BASE, suspend_size);

	/*
	 * disable the mmu before jump to sram entry, or, it will lead
	 * to a permission fault when execute in SRAM, because of WXN and XN
	 * bit effect.
	 */
	disable_mmu_el3();

	/* Jump to SRAM base */
	suspend_func = (suspend_handler_t)SUSPEND_SRAM_BASE;
	suspend_func(sec_entrypoint, power_state, nagra_boot_p);
}

void berlin_resume_from_sram(void)
{
	/* Restore the sram from ddr */
	memcpy((void *)SUSPEND_SRAM_BASE, sram_backup, SRAM_BACKUP_SIZE);
}

void berlin_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	assert(req_state);

	/* lower affinities use PLAT_MAX_OFF_STATE */
	req_state->pwr_domain_state[MPIDR_AFFLVL0] = PLAT_MAX_OFF_STATE;
	req_state->pwr_domain_state[MPIDR_AFFLVL1] = BERLIN_SUSPEND;
	req_state->pwr_domain_state[MPIDR_AFFLVL2] = BERLIN_SUSPEND;
}
