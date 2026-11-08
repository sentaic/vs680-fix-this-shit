/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
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
#include <string.h>
#include <assert.h>
#include <berlin_private.h>
#include <drivers/delay_timer.h>
#include <plat/arm/common/plat_arm.h>
#include <bl31/interrupt_mgmt.h>
#include <gicv2_ext.h>

extern unsigned long berlin_percpu_entrypoint[PLATFORM_CORE_COUNT];
//unsigned long berlin_percpu_entrypoint[PLATFORM_CORE_COUNT]
//__attribute__((section(".data"))) = {0,};
unsigned long berlin_warm_entrypoint __attribute__((section(".data"))) = 0;
bakery_lock_t berlin_pm_lock
#if USE_COHERENT_MEM
__attribute__((section("tzfw_coherent_mem")))
#endif
;

#pragma weak berlin_soc_pwr_domain_on
#pragma weak berlin_soc_pwr_domain_off
#pragma weak berlin_soc_pwr_enable_pmu
#pragma weak berlin_soc_pwr_domain_suspend_finish
#pragma weak berlin_soc_init

#pragma weak berlin_cpu_standby
#pragma weak berlin_pwr_domain_on
#pragma weak berlin_pwr_domain_off
#pragma weak berlin_pwr_domain_suspend
#pragma weak berlin_pwr_domain_on_finish
#pragma weak berlin_pwr_domain_suspend_finish
#pragma weak berlin_pwr_domain_pwr_down_wfi
#pragma weak berlin_system_off
#pragma weak berlin_system_reset
#pragma weak berlin_system_reset2
#pragma weak berlin_validate_power_state
#pragma weak berlin_validate_ns_entrypoint
#pragma weak berlin_get_sys_suspend_power_state

#pragma weak berlin_suspend_to_sram
#pragma weak berlin_resume_from_sram

int berlin_soc_pwr_domain_on(unsigned int target_cpu)
{
	return PSCI_E_SUCCESS;
}

int berlin_soc_pwr_domain_off(unsigned int target_cpu)
{
	return PSCI_E_SUCCESS;
}

int berlin_soc_pwr_enable_pmu(unsigned int target_cpu)
{
	return PSCI_E_SUCCESS;
}

int berlin_soc_pwr_domain_suspend_finish(const psci_power_state_t *target_state)
{
	return PSCI_E_NOT_SUPPORTED;
}

void berlin_suspend_to_sram(unsigned long sec_entrypoint,
				unsigned int power_state)
{
}
void berlin_resume_from_sram(void)
{
}

static int wakeup_irqs[MAX_WAKEUP_IRQS];
static int wakeup_irq_num = 0;

static irq_status_t irq_status[MAX_WAKEUP_IRQS];

#define LEVEL_SENSITIVE_MASK	(0x10000)
#define IRQ_NUM(x)				(x & 0xFFFF)
#define IS_LEVLE_SENSITIVE(x)	(x & LEVEL_SENSITIVE_MASK)

static bool valid_wakeup_irq(int irq)
{
	if ((wakeup_irq_num < MAX_WAKEUP_IRQS) && irq > 0 && irq < IRQ_TOTAL_IRQ)
		return true;
	else
		return false;
}

int berlin_set_wakeup_sources(u_register_t x1, u_register_t x2, u_register_t x3, u_register_t x4)
{
	int irq1 = x1, irq2 = x2, irq3 = x3, irq4 = x4;

	INFO("wakeup irq %d, %d, %d, %d.\n", irq1, irq2, irq3, irq4);

	if (valid_wakeup_irq(IRQ_NUM(irq1)))
		wakeup_irqs[wakeup_irq_num++] = irq1 + 32;

	if (valid_wakeup_irq(IRQ_NUM(irq2)))
		wakeup_irqs[wakeup_irq_num++] = irq2 + 32;

	if (valid_wakeup_irq(IRQ_NUM(irq3)))
		wakeup_irqs[wakeup_irq_num++] = irq3 + 32;

	if (valid_wakeup_irq(IRQ_NUM(irq4)))
		wakeup_irqs[wakeup_irq_num++] = irq4 + 32;

	INFO("wakeup irq num %d.\n", wakeup_irq_num);

	return 0;
}


void berlin_cfg_wakeup_sources(void)
{
	int i = 0;
	for (i = 0; i < wakeup_irq_num; i++) {
		INFO("######cfg wakeup irq = 0x%x#######\n", wakeup_irqs[i]);
		plat_ic_save_interrupt_status(IRQ_NUM(wakeup_irqs[i]), &irq_status[i]);
		plat_ic_setup_wakeup_interrupt(IRQ_NUM(wakeup_irqs[i]), IS_LEVLE_SENSITIVE(wakeup_irqs[i]));
	}
}

void berlin_resume_wakeup_sources(void)
{
	int i = 0;
	unsigned int id = 0;

	/* clear all pending interrupt */
	do {
		id = plat_ic_get_pending_interrupt_id();

		if (id != INTR_ID_UNAVAILABLE)
			plat_ic_clear_interrupt_pending(id);
		else
			break;
	} while (1);

	for (i = 0; i < wakeup_irq_num; i++)
		plat_ic_resume_wakeup_interrupt(IRQ_NUM(wakeup_irqs[i]), &irq_status[i]);
}

void berlin_program_percpu_entrypoint(unsigned long target_cpu,
				unsigned long sec_entrypoint)
{
	unsigned long *ep;

	ep = &berlin_percpu_entrypoint[target_cpu];
	*ep = sec_entrypoint;
	flush_dcache_range((unsigned long)ep, sizeof(*ep));
}

void berlin_program_rvbar(unsigned long target_cpu,
				unsigned long entrypoint)
{
	unsigned long addr;

	addr = CFG64_REG_BASE + (target_cpu << RVBARADDR0);
	mmio_write_64(addr, entrypoint);
}

static void berlin_power_down_common(void)
{
	unsigned long target_cpu = plat_my_core_pos();

	/* Prevent interrupts from spuriously waking up this cpu */
	plat_arm_gic_cpuif_disable();

	berlin_soc_pwr_enable_pmu(target_cpu);

	/*
	 * the following doesn't really control the signals
	 * since we have enabled PMU. It's to sync the PWRSW
	 * bits to power down
	 */
	berlin_soc_pwr_domain_off(target_cpu);

	dsbsy();
	wfi();
}

void berlin_soc_init(void)
{
}

void berlin_init(void)
{
	int i;

	bakery_lock_init(&berlin_pm_lock);
	berlin_soc_init();

	/* Power off CPU cores except primary one, those secondary cores are
	 * later to be awaken by Linux kernel through PSCI CPU_ON call.
	 */
	for (i = 0; i < PLATFORM_CORE_COUNT; i++) {
		if (i == BERLIN_PRIMARY_CPU) {
			continue;
		}
		berlin_soc_pwr_domain_off(i);
	}

#ifdef CFG_SUSPEND_WAKEUP_INTR
	berlin_set_wakeup_sources(CFG_SUSPEND_WAKEUP_INTR, -1, -1, -1);
#endif

#ifdef CFG_UART_WAKEUP_INTR
	berlin_set_wakeup_sources(CFG_UART_WAKEUP_INTR | LEVEL_SENSITIVE_MASK, -1, -1, -1);
#endif
}


/*******************************************************************************
 * Handler called when an affinity instance is about to enter standby.
 ******************************************************************************/
void berlin_cpu_standby(plat_local_state_t cpu_state)
{
	unsigned int scr;

	scr = read_scr_el3();
	/* Enable PhysicalIRQ bit for NS world to wake the CPU */
	write_scr_el3(scr | SCR_IRQ_BIT);
	isb();
	dsb();
	wfi();

	/*
	 * Restore SCR to the original value, synchronisation of scr_el3 is
	 * done by eret while el3_exit to save some execution cycles.
	 */
	write_scr_el3(scr);
}

/*******************************************************************************
 * Handler called when an affinity instance is about to be turned on. The
 * level and mpidr determine the affinity instance.
 ******************************************************************************/
int berlin_pwr_domain_on(u_register_t mpidr)
{
	unsigned long target_cpu;

	target_cpu = plat_core_pos_by_mpidr(mpidr);
	if (target_cpu == -1) {
		return PSCI_E_INVALID_PARAMS;
	}

	/*
	 * Setup reset vector register to contain entrypoint where CPU
	 * execution starts from when it next powers up
	 */
	berlin_program_rvbar(target_cpu, berlin_warm_entrypoint);

	berlin_soc_pwr_domain_on(target_cpu);

	return PSCI_E_SUCCESS;
}

/*******************************************************************************
 * Handler called when a power domain is about to be turned off. The
 * target_state encodes the power state that each level should transition to.
 ******************************************************************************/
void berlin_pwr_domain_off(const psci_power_state_t *target_state)
{
	assert(target_state);
}

/*******************************************************************************
 * Handler called when called when a power domain is about to be suspended. The
 * target_state encodes the power state that each level should transition to.
 ******************************************************************************/
void berlin_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	plat_local_state_t pwrstate;

	assert(target_state);
	/*
	 * berlin currently supports retention only at cpu level. Just return
	 * as nothing is to be done for retention.
	 */
	if (target_state->pwr_domain_state[MPIDR_AFFLVL0] ==
			PLAT_MAX_RET_STATE)
		return;

	pwrstate = target_state->pwr_domain_state[MPIDR_AFFLVL1];

	switch (pwrstate) {
	case BERLIN_ACTIVE_STANDBY:
		break;
	case BERLIN_SUSPEND:
		berlin_program_percpu_entrypoint(plat_my_core_pos(),
				berlin_warm_entrypoint);
		break;
	default:
		berlin_program_rvbar(plat_my_core_pos(), berlin_warm_entrypoint);
		berlin_power_down_common();
		break;
	}
}

/*******************************************************************************
 * Handler called when a power domain has just been powered on after
 * being turned off earlier. The target_state encodes the low power state that
 * each level has woken up from.
 ******************************************************************************/
void berlin_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	assert(target_state);

	/* Enable the gic cpu interface */
	plat_arm_gic_pcpu_init();

	plat_arm_gic_cpuif_enable();

	/* Clear the RVBAR for this cpu. */
	berlin_program_rvbar(plat_my_core_pos(), 0);
}

/*******************************************************************************
 * Handler called when a power domain has just been powered on after
 * having been suspended earlier. The target_state encodes the low power state
 * that each level has woken up from.
 ******************************************************************************/
void berlin_pwr_domain_suspend_finish(
				const psci_power_state_t *target_state)
{
	unsigned long target_cpu;
	plat_local_state_t pwrstate;

	assert(target_state);

	/* Return as nothing is to be done on waking up from retention. */
	if (target_state->pwr_domain_state[MPIDR_AFFLVL0] ==
			PLAT_MAX_RET_STATE)
		return;

	pwrstate = target_state->pwr_domain_state[MPIDR_AFFLVL1];

	/* Get the linear id for this cpu */
	target_cpu = plat_my_core_pos();

	berlin_soc_pwr_domain_suspend_finish(target_state);

	/* Enable the gic cpu interface */
	plat_arm_gic_pcpu_init();

	plat_arm_gic_cpuif_enable();

	switch (pwrstate) {
	case BERLIN_ACTIVE_STANDBY:
		berlin_resume_from_sram();
		berlin_resume_wakeup_sources();
		break;
	case BERLIN_SUSPEND:
		berlin_program_percpu_entrypoint(target_cpu, 0);
		bl31_platform_setup();
		berlin_resume_from_sram();
		berlin_resume_wakeup_sources();
		berlin_context_resume();
		break;
	default:
		berlin_program_rvbar(target_cpu, 0);
		break;
	}
}

void __dead2 berlin_pwr_domain_pwr_down_wfi(
				const psci_power_state_t *target_state)
{
	plat_local_state_t pwrstate;

	assert(target_state);

	pwrstate = target_state->pwr_domain_state[MPIDR_AFFLVL1];

	switch (pwrstate) {
	case BERLIN_ACTIVE_STANDBY:
		berlin_cfg_wakeup_sources();
		berlin_suspend_to_sram(berlin_warm_entrypoint, 1);
		break;
	case BERLIN_SUSPEND:
		berlin_cfg_wakeup_sources();
		berlin_suspend_to_sram(berlin_warm_entrypoint, 0);
		break;
	default:
		berlin_power_down_common();
		break;
	}

	while (1)
		;
}

/*******************************************************************************
 * Handler called when the system wants to be powered off
 ******************************************************************************/
void __dead2 berlin_system_off(void)
{
	ERROR("Berlin System Off: operation not handled.\n");
	panic();
}

/*******************************************************************************
 * Handler called when the system wants to be restarted.
 ******************************************************************************/
void __dead2 berlin_system_reset(void)
{
	ERROR("Berlin System Reset: operation not handled.\n");
	panic();
}

/*******************************************************************************
 * Handler called when the system wants to be restarted.
 ******************************************************************************/

int berlin_system_reset2(int is_vendor,
			int reset_type, u_register_t cookie)

{
	ERROR("Berlin System Reset2: operation not handled.\n");
	return PSCI_E_NOT_SUPPORTED;
}

/*******************************************************************************
 * Handler called to check the validity of the power state parameter.
 ******************************************************************************/
int32_t berlin_validate_power_state(unsigned int power_state,
				psci_power_state_t *req_state)
{
	int pstate = psci_get_pstate_type(power_state);
	int pwrlvl = psci_get_pstate_pwrlvl(power_state);
	int stateid = psci_get_pstate_id(power_state);

	assert(req_state);

	if (pwrlvl > MPIDR_AFFLVL1)
		return PSCI_E_INVALID_PARAMS;

	/* Sanity check the requested state */
	if (pstate == PSTATE_TYPE_STANDBY) {
		/*
		 * Set the CPU local state as retention and ignore the higher
		 * levels. This allows the generic PSCI layer to invoke
		 * plat_psci_ops 'cpu_standby' hook and the compatibility
		 * layer invokes the 'affinst_standby' handler with the
		 * correct power_state parameter thus preserving the correct
		 * behavior.
		 */
		req_state->pwr_domain_state[MPIDR_AFFLVL0] = PLAT_MAX_RET_STATE;
	} else {
		req_state->pwr_domain_state[MPIDR_AFFLVL0] = PLAT_MAX_OFF_STATE;

		if (pwrlvl == MPIDR_AFFLVL1) {
			switch (stateid & BERLIN_PSTATEID_MASK) {
			case BERLIN_PSTATEID_STANDBY:
				req_state->pwr_domain_state[pwrlvl] = BERLIN_ACTIVE_STANDBY;
				break;
			case BERLIN_PSTATEID_POWERDOWN:
				req_state->pwr_domain_state[pwrlvl] = BERLIN_SUSPEND;
				break;
			default:
				assert(0);
			}
		}
	}

	return PSCI_E_SUCCESS;
}

/*******************************************************************************
 * Platform handler called to check the validity of the non secure entrypoint.
 ******************************************************************************/
int berlin_validate_ns_entrypoint(uintptr_t entrypoint)
{
	return PSCI_E_SUCCESS;
}

/*******************************************************************************
 * This handler is called by the PSCI implementation during the `SYSTEM_SUSPEND`
 * call to get the `power_state` parameter. This allows the platform to encode
 * the appropriate State-ID field within the `power_state` parameter which can
 * be utilized in `pwr_domain_suspend()` to suspend to system affinity level.
 ******************************************************************************/
void berlin_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	assert(req_state);

	/* lower affinities use PLAT_MAX_OFF_STATE */
	req_state->pwr_domain_state[MPIDR_AFFLVL0] = PLAT_MAX_OFF_STATE;
	req_state->pwr_domain_state[MPIDR_AFFLVL1] = BERLIN_ACTIVE_STANDBY;
	req_state->pwr_domain_state[MPIDR_AFFLVL2] = BERLIN_ACTIVE_STANDBY;
}

/*******************************************************************************
 * Export the platform handlers to enable psci to invoke them
 ******************************************************************************/
const plat_psci_ops_t berlin_plat_psci_ops = {
	.cpu_standby			= berlin_cpu_standby,
	.pwr_domain_on			= berlin_pwr_domain_on,
	.pwr_domain_off			= berlin_pwr_domain_off,
	.pwr_domain_suspend		= berlin_pwr_domain_suspend,
	.pwr_domain_on_finish		= berlin_pwr_domain_on_finish,
	.pwr_domain_suspend_finish	= berlin_pwr_domain_suspend_finish,
	.pwr_domain_pwr_down_wfi	= berlin_pwr_domain_pwr_down_wfi,
	.system_off			= berlin_system_off,
	.system_reset			= berlin_system_reset,
	.system_reset2			= berlin_system_reset2,
	.validate_power_state		= berlin_validate_power_state,
	.validate_ns_entrypoint		= berlin_validate_ns_entrypoint,
	.get_sys_suspend_power_state	= berlin_get_sys_suspend_power_state,
};

/*******************************************************************************
 * Export the platform specific power ops and initialize Power Controller
 ******************************************************************************/
int plat_setup_psci_ops(uintptr_t warm_entrypoint,
				const plat_psci_ops_t **psci_ops)
{
	berlin_warm_entrypoint = warm_entrypoint;
	flush_dcache_range((unsigned long)&berlin_warm_entrypoint,
			sizeof(unsigned long));

	/*
	 * Initialize PSCI ops struct
	 */
	*psci_ops = &berlin_plat_psci_ops;

	return 0;
}
