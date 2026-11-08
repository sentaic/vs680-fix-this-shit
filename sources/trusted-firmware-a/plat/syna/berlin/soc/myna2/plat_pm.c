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
#include "Galois_memmap.h"
#include "cpu_wrp.h"
#include "wdt.h"
#include "sm.h"

extern bakery_lock_t berlin_pm_lock;

int berlin_set_wakeup_sources(u_register_t x1, u_register_t x2, u_register_t x3, u_register_t x4);
#define RESET_nCPUPORESET_0    0

#define PWRSW_CNTRL1_I         0
#define PWRSW_ACK1_O           1
#define PWRSW_CNTRL2_I         2
#define PWRSW_ACK2_O           3
#define PWRSW_CNTRL_MEM_I      4
#define PWRSW_CNTRL_MEM_O      5
#define nCPU_ISO_MODE_EN       6

#define CPU_PCH(n)      (*(volatile unsigned int*)((uintptr_t)(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CPU0 + (n)*(RA_CPU_REG_CPU1-RA_CPU_REG_CPU0))))
/*
 void arm_set_PWRCTLR(unsigned int value);
 */
static void arm_set_PWRCTLR(unsigned int value)
{
	asm volatile("msr	S3_0_C15_C2_7, %[value]\n"
			: : [value] "r" (value) : "memory", "cc");
}

int berlin_soc_pwr_domain_on(unsigned int target_cpu)
{
#if 1
	unsigned int val;
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CFG);
	val |= 1 << target_cpu;
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CFG, val);

	// back to power-off manual mode before disable PMU if it is power-off by PMU
	/* assert reset */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu);
	val &= ~(1 << RESET_nCPUPORESET_0);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu, val);

	/* turn on isolation cells, PWRSW_CNTRL1_I, PWRSW_CNTRL2_I,  PWRSW_CNTRL_MEM_I*/
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val &= ~((1 << nCPU_ISO_MODE_EN) | (1 << PWRSW_CNTRL1_I) | (1<< PWRSW_CNTRL2_I) | (1<< PWRSW_CNTRL_MEM_I));
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	// manual mode
	// Set the PSTATE to ON but no set request and not wait ack
	CPU_PCH(target_cpu) =  0; // write 0 to clear
	//arm_DMB();
	CPU_PCH(target_cpu) = (0x8 << LSb32CPU_PCH_PCH_STATE);

	/* disable CPMU */
	mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 0);

	/* PWRSW_CNTRL1_I */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val |= (1 << PWRSW_CNTRL1_I);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	do {
		val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
		val &= (1 << PWRSW_ACK1_O);
	} while (!val);

	udelay(10);

	/* PWRSW_CNTRL2_I */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val |= (1 << PWRSW_CNTRL2_I);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	do {
		val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
		val &= (1 << PWRSW_ACK2_O);
	} while (!val);

	/* PWRSW_CNTRL_MEM_I */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val |= (1 << PWRSW_CNTRL_MEM_I);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);

	do {
		val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
		val &= (1 << PWRSW_CNTRL_MEM_O);
	} while (!val);

	/* turn off isolation cells */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val |= (1 << nCPU_ISO_MODE_EN);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);
	udelay(100);

	/* release reset */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu);
	val |= (1 << RESET_nCPUPORESET_0);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu, val);

#else
    INFO("%s(), line %d: target_cpu = 0x%x.\n", __func__, __LINE__, target_cpu);
	mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 1);
    INFO("%s(), line %d: reset vect = 0x%x.\n", __func__, __LINE__, mmio_read_32(CFG64_REG_BASE + (target_cpu << RVBARADDR0)));

	gicv2_raise_sgi(ARM_IRQ_SEC_SGI_0, 0xf);
	udelay(20000);
	INFO("%s(), line %d: target_cpu = 0x%x.\n", __func__, __LINE__, target_cpu);
#endif
	return PSCI_E_SUCCESS;
}

/* 1.	CA-55 power down sequence (below steps are copied from A55 TRM)
 * A4.8 Power down sequence
 * The Cortex-A55 core uses the following power down sequence.
 * To power down a core, perform the following programming sequence:
 * 1.	Save all architectural state.
 * 2.	Configure the GIC distributor to disable or reroute interrupts away from this core.
 * 3.	Set the CPUPWRCTLR.CORE_PWRDN_EN bit to 1 to indicate to the power controller that a powerdown is requested.
 * 4.	Execute an Instruction Synchronization Barrier (ISB) instruction.
 * 5.	Execute a WFI instruction.
 * After executing WFI and then receiving a powerdown request from the power controller, the hardware performs the following:
 * • Disabling and flushing of caches (L1 and L2).
 * • Removal of the core from coherency.
 */

int berlin_soc_pwr_domain_off(unsigned int target_cpu)
{

	unsigned int current_cpu;
	current_cpu = plat_my_core_pos();

	if (current_cpu != target_cpu) {
		// INFO("CPU 0x%x try to Power down CPU 0x%x\n", current_cpu, target_cpu);

		/* Power down by other CPU
		 * enable CPMU, then sencondary CPUs can be powered down by CPMU
		 * The CPU should Set the CPUPWRCTLR.CORE_PWRDN_EN bit to 1 then in WFI mode
		 */
		mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 1);

	} else {
	/* Power down by itself with CPMU
	 * 1. enable PMU
	 * 2. Set the CPUPWRCTLR.CORE_PWRDN_EN bit to 1 to indicate
	 *    to the power controller that a powerdown is requested
	 * 3. execute ISB instruction
	 * 4. execute WFI instruction
	 */
		//mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 1);
		arm_set_PWRCTLR(1);
		isb();
		//wfi();
	}
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
#if 0
	unsigned int target_cpu;
	target_cpu = plat_my_core_pos();

	/*
	 * disable PMU so that CPU will not be powered down
	 * after excute WFI instruction
	 */
	mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 0);
#else
	/*
	 * Set the CPUPWRCTLR.CORE_PWRDN_EN bit to 0
	 * so that CPU will not be powered down after excute WFI instruction
	 */
	arm_set_PWRCTLR(0);
#endif
	return PSCI_E_SUCCESS;

}

void berlin_soc_init(void)
{
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CFG, 0xf);
}

#if 0
static int cpu_pch_wait_active(int core, int active, int timeout_ms)
{
	while(1)
	{
        unsigned int value = CPU_PCH(core);

		if (((value & MSK32CPU_PCH_PCH_ACTIVE)>>LSb32CPU_PCH_PCH_ACTIVE) == active)
		{
			return 0;
		}
		if (timeout_ms <= 0)
		{
			ERROR("wait PCH_%d active:0x%x timeout, CPU_PCH:0x%x\n", core, active, value);
			return -1;
		}
		udelay(1000);
		timeout_ms--;
	}
}

static int cpu_pch_req_state(int core, int state, int timeout_ms)
{

 //   unsigned int value;

    state &= MSK32CPU_PCH_PCH_STATE>>LSb32CPU_PCH_PCH_STATE;

    CPU_PCH(core) = 0; // write 0 to clear
   // arm_DMB();
    CPU_PCH(core) = (state << LSb32CPU_PCH_PCH_STATE); // set state first
    //arm_DMB();
    CPU_PCH(core) = (state << LSb32CPU_PCH_PCH_STATE) | MSK32CPU_PCH_PCH_REQ; // set request

	while(1)
	{
        unsigned int value = CPU_PCH(core);

        if (value & MSK32CPU_PCH_PCH_ACCEPT)
        {
            return 1;
        }

        if (value & MSK32CPU_PCH_PCH_DENY)
        {
            INFO("set PCH_%d state:%d denied, CPU_PCH:0x%x\n", core, state, value);

            // retry
            CPU_PCH(core) = (state << LSb32CPU_PCH_PCH_STATE); // set state first
            //arm_DMB();
            CPU_PCH(core) = (state << LSb32CPU_PCH_PCH_STATE) | MSK32CPU_PCH_PCH_REQ; // set request
        }

        if (timeout_ms <= 0)
        {
            INFO("wait PCH_%d ack timeout, CPU_PCH:0x%x\n", core, value);
            return -1;
        }
        udelay(1000);
        timeout_ms--;
    }
}

int berlin_soc_manual_domain_off(unsigned int target_cpu)
{
#if 1
	unsigned int val;
	int ret;

	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CFG);
	val |= 1 << target_cpu;
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CFG, val);

	CPU_PCH(target_cpu) = 0;

	/* disable CPMU for manual mode */
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 0);
	// make sure the following steps are done by the request CPU
	//arm_set_PWRCTLR(1);
	//isb();
	// wfi

	// After core executes WFI it will change PACTIVE value to 0x0, CPMU will wait for this and then change PSTATE to 0x0 and assert PREQ.
	ret = cpu_pch_wait_active(target_cpu, 0, 10);
	if (ret != 0) {
		// return -1;
	}

	ret = cpu_pch_req_state(target_cpu, 0, 10);
	if (ret != 1) {
		// return -1;
	}

	/* assert reset */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu);
	val &= ~(1 << RESET_nCPUPORESET_0);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_RESET_0 + 4*target_cpu, val);
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

	/* PWRSW_CNTRL_MEM_I */
	val = mmio_read_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu);
	val &= ~(1<< PWRSW_CNTRL_MEM_I);
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_PWRSW_0 +  4*target_cpu, val);
#else
	/* enable PMU, then sencondary CPUs can be powered down by CPMU
	 * The CPU should Set the CPUPWRCTLR.CORE_PWRDN_EN bit to 1 then in WFI mode
	 */
	mmio_write_32(MEMMAP_CA7_REG_BASE +  RA_CPU_REG_Cpu0ExtPmu + 4*target_cpu, 1);
#endif


	return PSCI_E_SUCCESS;
}
#endif

void __dead2 berlin_system_reset(void)
{
	INFO("Berlin System Reset ......\n");
	reset_soc();
	while (1);
	ERROR("Berlin System reset: should not be here.\n");
	panic();
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

	assert(suspend_size <= SRAM_BACKUP_SIZE);
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
	arm_set_PWRCTLR(0);

	/* Flush all caches. */
	dcsw_op_all(DCCISW);

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
	req_state->pwr_domain_state[MPIDR_AFFLVL1] = BERLIN_ACTIVE_STANDBY;
	req_state->pwr_domain_state[MPIDR_AFFLVL2] = BERLIN_ACTIVE_STANDBY;
}
