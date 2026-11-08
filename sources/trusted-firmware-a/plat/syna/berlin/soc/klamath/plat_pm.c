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
#include "klamath_memmap.h"
#include "cpu_wrp.h"
#include "sm.h"
#include "soc.h"
#include "global.h"
#include "nexus_fw_include/mcu_cpu_ss.h"
#include "nexus_fw_include/nexus_klamath_memmap.h"
#include "nexus_fw_include/mcu_gbl_cfg.h"
#include "sm_ipc.h"
#include "synaprot_mailbox.h"

int berlin_set_wakeup_sources(u_register_t x1, u_register_t x2, u_register_t x3, u_register_t x4);
#define RESET_nCPUPORESET_0    0

#define PWRSW_CNTRL1_I         0
#define PWRSW_ACK1_O           1
#define PWRSW_CNTRL2_I         2
#define PWRSW_ACK2_O           3
#define PWRSW_CNTRL_MEM_I      4
#define PWRSW_CNTRL_MEM_O      5
#define nCPU_ISO_MODE_EN       6

#define MEMMAP_CA7_REG_BASE START_ACPU_REG
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
		//INFO("CPU 0x%x try to Power down CPU 0x%x\n", current_cpu, target_cpu);

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
	/*
	 * Set the CPUPWRCTLR.CORE_PWRDN_EN bit to 0
	 * so that CPU will not be powered down after excute WFI instruction
	 */
	arm_set_PWRCTLR(0);

	return PSCI_E_SUCCESS;

}

void berlin_soc_init(void)
{
	mmio_write_32(MEMMAP_CA7_REG_BASE + RA_CPU_REG_CFG, 0x3);
	// GPIO, SPI, SPI, UART non-Secure access allowed
	mmio_write_32(START_SOC_REG + RA_SOC_PERIF_PPC0_CTRL2, 0xffff);

	mmio_write_32(0xf7e10420, 0xffffffff);
	udelay(10);
	mmio_write_32(START_PDMA_REG, 0xffffffff);
	mmio_write_32(START_CHIP_CTRL_REG + RA_Gbl_pdma_dst_req_mask, 0xffffffff);
	mmio_write_32(START_CHIP_CTRL_REG + RA_Gbl_pdma_src_req_mask, 0xffffffff);
	// enable arm sse timer cntctrl
	mmio_write_32(START_SYSCOUNT_CTRL_REG, 0x1);
	mailbox_init();
}

void berlin_suspend_to_sram(unsigned long sec_entrypoint,
				unsigned int power_state)
{
	/*
	 * Mark warm boot for BL31: store a flag in retention memory so that
	 * bl31_entrypoint can detect warm resume and jump to bl31_warm_entrypoint.
	 * Do this before disabling MMU and perform cache maintenance afterwards
	 * to ensure visibility.
	 */
	boot_mode = WARM_BOOT;
	INFO("boot mode: 0x%x.\n", boot_mode);
	/*
	 * disable the mmu before jump to sram entry, or, it will lead
	 * to a permission fault when execute in SRAM, because of WXN and XN
	 * bit effect.
	 */
	disable_mmu_el3();
	arm_set_PWRCTLR(0);
	/* Flush all caches. */
	dcsw_op_all(DCCISW);
	sm_system_warmdown(power_state);
	// should not return here
	while(1);

}

void berlin_resume_from_sram(void)
{
	boot_mode = NORMAL_BOOT;
}

void berlin_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	assert(req_state);

	/* lower affinities use PLAT_MAX_OFF_STATE */
	req_state->pwr_domain_state[MPIDR_AFFLVL0] = PLAT_MAX_OFF_STATE;
	req_state->pwr_domain_state[MPIDR_AFFLVL1] = BERLIN_SUSPEND;
	req_state->pwr_domain_state[MPIDR_AFFLVL2] = BERLIN_SUSPEND;
}

void __dead2 berlin_system_off(void)
{
	INFO("Berlin System Off .......\n");
	sm_system_off();
	while (1);
	ERROR("Berlin System Off: should not be here.\n");
	panic();
}

#if 0
void __dead2 berlin_system_reset(void)
{
	INFO("Berlin System Reset ......\n");
	sm_system_reset();
	while (1);
	ERROR("Berlin System reset: should not be here.\n");
	panic();
}
#else
static inline void reset_soc(void)
{
	unsigned int mcu_gbl_base = MCU_PERIF_REGION(START_MCU_GBL_SEC_S);

	mmio_write_32(mcu_gbl_base + RA_mcu_gbl_cfg_sec_reset_trigger, 1);
}

void __dead2 berlin_system_reset(void)
{
	INFO("Berlin System Reset ......\n");
	reset_soc();
	while (1);
	ERROR("Berlin System reset: should not be here.\n");
	panic();
}

#endif
