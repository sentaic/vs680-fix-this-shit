/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <plat/common/platform.h>
#include <drivers/arm/gicv2.h>
#include <bl31/interrupt_mgmt.h>
#include <platform_def.h>
#include <drivers/arm/gic_common.h>
#include <drivers/arm/gicv2.h>
#include <lib/mmio.h>
#include <gicv2_ext.h>

extern unsigned int gicd_read_ipriorityr(uintptr_t base, unsigned int id);

static unsigned int plat_ic_get_interrupt_priority(unsigned int id)
{
	unsigned int ipriorityr = 0;
	unsigned int priority = 0;

	ipriorityr = gicd_read_ipriorityr(PLAT_ARM_GICD_BASE, id);

	priority = ipriorityr >> (id % 4 * 8) & GIC_PRI_MASK;

	return priority;
}

static inline uint8_t gicd_get_itargetsr(uintptr_t base, unsigned int id)
{
	return mmio_read_8(base + GICD_ITARGETSR + id);
}

static inline void gicd_set_itargetsr(uintptr_t base, unsigned int id,
		unsigned int target)
{
	uint8_t val = target & GIC_TARGET_CPU_MASK;

	mmio_write_8(base + GICD_ITARGETSR + id, val);
}

static uint8_t plat_ic_get_interrupt_target(unsigned int id)
{
	return gicd_get_itargetsr(PLAT_ARM_GICD_BASE, id);
}

static void plat_ic_set_spi_target(unsigned int id, unsigned int routing_mode,
							unsigned int target)
{
	switch (routing_mode) {
	case INTR_ROUTING_MODE_PE:
		gicd_set_itargetsr(PLAT_ARM_GICD_BASE, id, target);
		break;
	case INTR_ROUTING_MODE_ANY:
		gicd_set_itargetsr(PLAT_ARM_GICD_BASE, id, GIC_TARGET_CPU_MASK);
		break;
	default:
		break;
	}
}
void plat_ic_set_cfg(unsigned int id, unsigned int cfg)
{
	gicv2_interrupt_set_cfg(id, cfg);
}

static unsigned int plat_ic_get_cfg(unsigned int id)
{
	/* Interrupt configuration is a 2-bit field */
	unsigned int bit_num = id & ((1U << ICFGR_SHIFT) - 1U);
	unsigned int bit_shift = bit_num << 1;
	unsigned int n = id >> ICFGR_SHIFT;
	uint32_t reg_val = mmio_read_32(PLAT_ARM_GICD_BASE + GICD_ICFGR + (n << 2));

	return (reg_val & (GIC_CFG_MASK << bit_shift)) >> bit_shift;
}

void plat_ic_save_interrupt_status(unsigned int id, irq_status_t *is)
{
	uint8_t itargetsr = 0;

	is->int_type = plat_ic_get_interrupt_type(id);
	is->priority = plat_ic_get_interrupt_priority(id);
	is->icfg = plat_ic_get_cfg(id);
	itargetsr = plat_ic_get_interrupt_target(id);
	if (itargetsr == GIC_TARGET_CPU_MASK)
		is->mode = INTR_ROUTING_MODE_ANY;
	else {
		is->mode = INTR_ROUTING_MODE_PE;
		is->target = itargetsr;
	}
}

void plat_ic_setup_wakeup_interrupt(unsigned int id, unsigned int level)
{
#if GICV2_G0_FOR_EL3
	plat_ic_set_interrupt_type(id, INTR_TYPE_EL3);
#else
	plat_ic_set_interrupt_type(id, INTR_TYPE_S_EL1);
#endif
	plat_ic_set_interrupt_priority(id, PLAT_RAS_PRI);
	plat_ic_clear_interrupt_pending(id);
	plat_ic_set_spi_routing(id, INTR_ROUTING_MODE_PE, read_mpidr());
	if (!level)
		plat_ic_set_cfg(id, GIC_INTR_CFG_EDGE);
	plat_ic_enable_interrupt(id);
}

void plat_ic_resume_wakeup_interrupt(unsigned int id, irq_status_t *is)
{
	plat_ic_disable_interrupt(id);
	plat_ic_clear_interrupt_pending(id);
	plat_ic_set_interrupt_type(id, is->int_type);
	plat_ic_set_interrupt_priority(id, is->priority);
	plat_ic_set_spi_target(id, is->mode, is->target);
	plat_ic_set_cfg(id, is->icfg);
}
