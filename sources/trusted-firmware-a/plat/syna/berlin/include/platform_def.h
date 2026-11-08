/*
 * Copyright (c) 2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PLATFORM_DEF_H__
#define __PLATFORM_DEF_H__

#include <arch.h>
#include <plat/common/common_def.h>
#include <berlin_def.h>

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/

/* Size of cacheable stacks */
#define PLATFORM_STACK_SIZE		0x400

#define PLAT_MAX_PWR_LVL		MPIDR_AFFLVL2
#define PLATFORM_CORE_COUNT		(BERLIN_CLUSTER_COUNT *		\
						BERLIN_MAX_CPUS_PER_CLUSTER)
#define PLAT_NUM_PWR_DOMAINS		(PLATFORM_CORE_COUNT +		\
						BERLIN_CLUSTER_COUNT + 1)

/*******************************************************************************
 * Platform power states
 ******************************************************************************/
#define PLAT_MAX_RET_STATE		1
#define PLAT_MAX_OFF_STATE		3

/*******************************************************************************
 * BL31 specific defines.
 ******************************************************************************/
#define BL31_BASE			BERLIN_TZDRAM_BASE
#define BL31_LIMIT			(BERLIN_TZDRAM_BASE + BERLIN_TZDRAM_SIZE)

/*******************************************************************************
 * BL32 specific defines.
 ******************************************************************************/
/*
 * End of BL3-1 follows BL3-2
 */
#define BL32_BASE			BL31_LIMIT
#define BL32_BOOT_PARAM		0x00101000

#define BL33_BASE			0x4100000
#define DTB_BASE			0x09000000
/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#define PLAT_PHY_ADDR_SPACE_SIZE        (1ull << 32)
#define PLAT_VIRT_ADDR_SPACE_SIZE       (1ull << 32)

#ifndef MAX_XLAT_TABLES
#define MAX_XLAT_TABLES			3
#endif

#define MAX_MMAP_REGIONS		16

/*******************************************************************************
 * Some data must be aligned on the biggest cache line size in the platform.
 * This is known only to the platform as it might have a combination of
 * integrated and external caches.
 ******************************************************************************/
#define CACHE_WRITEBACK_SHIFT		6
#define CACHE_WRITEBACK_GRANULE		(1 << CACHE_WRITEBACK_SHIFT)

/*
 * GIC related constants to cater for GICv2
 */
#define PLAT_ARM_GICD_BASE		GIC_DIST_BASE
#define PLAT_ARM_GICC_BASE		GIC_CPU_BASE

#define ARM_IRQ_SEC_PHY_TIMER		29

#define PLAT_RAS_PRI			U(0x10)

#define ARM_IRQ_SEC_SGI_0		8
#define ARM_IRQ_SEC_SGI_1		9
#define ARM_IRQ_SEC_SGI_2		10
#define ARM_IRQ_SEC_SGI_3		11
#define ARM_IRQ_SEC_SGI_4		12
#define ARM_IRQ_SEC_SGI_5		13
#define ARM_IRQ_SEC_SGI_6		14
#define ARM_IRQ_SEC_SGI_7		15

/*
 * Define a list of Group 1 Secure and Group 0 interrupt properties as per GICv3
 * terminology. On a GICv2 system or mode, the lists will be merged and treated
 * as Group 0 interrupts.
 */
#define ARM_G1S_IRQ_PROPS(grp) \
			INTR_PROP_DESC(ARM_IRQ_SEC_PHY_TIMER, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_LEVEL), \
			INTR_PROP_DESC(ARM_IRQ_SEC_SGI_1, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_EDGE), \
			INTR_PROP_DESC(ARM_IRQ_SEC_SGI_2, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_EDGE), \
			INTR_PROP_DESC(ARM_IRQ_SEC_SGI_3, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_EDGE), \
			INTR_PROP_DESC(ARM_IRQ_SEC_SGI_4, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_EDGE), \
			INTR_PROP_DESC(ARM_IRQ_SEC_SGI_5, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_EDGE), \
			INTR_PROP_DESC(ARM_IRQ_SEC_SGI_7, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_EDGE)

#define ARM_G0_IRQ_PROPS(grp) \
			INTR_PROP_DESC(ARM_IRQ_SEC_SGI_0, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_EDGE), \
			INTR_PROP_DESC(ARM_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY, (grp), \
					GIC_INTR_CFG_EDGE)

#define PLAT_ARM_G1S_IRQ_PROPS(grp) \
		ARM_G1S_IRQ_PROPS(grp)

#define PLAT_ARM_G0_IRQ_PROPS(grp)	\
		ARM_G0_IRQ_PROPS(grp)

#endif /* __PLATFORM_DEF_H__ */
