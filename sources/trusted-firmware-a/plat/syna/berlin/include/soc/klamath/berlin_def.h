/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BERLIN_DEF_H__
#define __BERLIN_DEF_H__

#include <platform_def.h>
/* Enable the dynamic translation tables library. */
#define PLAT_XLAT_TABLES_DYNAMIC	1
#define MAX_XLAT_TABLES		6

#define BERLIN_TZDRAM_BASE				0x00120000
#define BERLIN_TZDRAM_SIZE				0x00040000

#define BERLIN_DEVICE0_BASE				0xF6000000
#define BERLIN_DEVICE0_SIZE				0x04000000
#define BERLIN_DEVICE1_BASE				0xe1000000
#define BERLIN_DEVICE1_SIZE				0x06000000

#define BERLIN_CLUSTER_COUNT				1
#define BERLIN_MAX_CPUS_PER_CLUSTER			2
#define BERLIN_PRIMARY_CPU					0x0

#define BERLIN_SYSCNT_FREQ				25000000

#define BERLIN_PSTATEID_STANDBY				1
#define BERLIN_PSTATEID_POWERDOWN			3
#define BERLIN_PSTATEID_MASK				0xf

#define BERLIN_ACTIVE_STANDBY				(PLAT_MAX_OFF_STATE - 1)
#define BERLIN_SUSPEND					PLAT_MAX_OFF_STATE

#define BERLIN_RESUME						0x00000001
#define BERLIN_RESUME_CPU_ON				0x00000002


/* arch timer */
//MEMMAP_CA7_REG_BASE + RA_CPU_WRP_TIMER_REG+RA_TIMER_REG_CPUTimerEn
#define TCTL_BASE					0xF7E31000

/* uart */
#define UART_0_BASE                  0xe5030000
#define BERLIN_UART_CLOCK            25000000
#define BERLIN_UART_BAUDRATE		115200

/* GIC */
//MEMMAP_GIC_REG_BASE 
#define GIC_DIST_BASE					0xF7E59000
#define GIC_CPU_BASE					0xF7E5A000
#define BERLIN_G0_IRQS					42
#define BERLIN_IRQ_SEC_SGI_0		8
#define BERLIN_IRQ_SEC_SGI_1		9
#define BERLIN_IRQ_SEC_SGI_2		10
#define BERLIN_IRQ_SEC_SGI_3		11
#define BERLIN_IRQ_SEC_SGI_4		12
#define BERLIN_IRQ_SEC_SGI_5		13
#define BERLIN_IRQ_SEC_SGI_6		14
#define BERLIN_IRQ_SEC_SGI_7		15

#define MCU_PERIF_REGION(addr)	((addr) - 0x58000000 + 0xE4000000)
#define MCU_DMA_CRYPT_REGION(addr)	((addr) - 0x50000000 + 0xE3000000)

/* Reset vector */
//START_ACPU_REG + RA_CPU_WRP_CFG64_REG
#define CFG64_REG_BASE					0xF7E30800
#define RVBARADDR0						3

#define START_WD0_REG                  0xF7F30000
#define CONFIG_DW_WDT_BASE             START_WD0_REG
#define RA_Gbl_WDTSysRstMask            0x0330

#define MEMMAP_CHIP_CTRL_REG_BASE	0xF7E10000 // START_CHIP_CTRL_REG
#define SOC_RESET_CTRL_BASE			0xF7E10320 //(START_CHIP_CTRL_REG + RA_Gbl_ResetTrigger)

#define RA_Gbl_perifStickyResetN	0x0360
#define SOC_REST_PERIF_BASE		(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_perifStickyResetN)
#define MAX_PERIF_ID	4

#define SUSPEND_SRAM_BASE		0xF9000000 //START_MPTS_REG
#define SRAM_BACKUP_SIZE			0x10000

#define DDR_REG_BASE					0xF7CB0000
#define AVIO_REG_BASE					0xF7400000

//MEMMAP_CA7_REG_BASE + RA_CPU_WRP_PLL_REG
#define MEMMAP_CA7_REG_BASE       START_ACPU_REG

#define CPU_WRP_PLL_REG_BASE           (START_ACPU_REG + RA_CPU_WRP_PLL_REG)
//(MEMMAP_MCTRLSS_REG_BASE + RA_MC6Ctrl_memPll)
#define MC6CTRL_MEMPLL_BASE             0xF7940038

#define GBL_SYSPLL0_BASE				0xF7EA0200
#define GBL_SYSPLL1_BASE				0xF7EA0220
#define SYSPLL_CTRL_BASE				GBL_SYSPLL0_BASE	//(MEMMAP_CHIP_CTRL_REG_BASE + GBL_SYSPLL0_BASE)
#define CPUPLL_CTRL_BASE				0xF7922000			//(MEMMAP_CA7_REG_BASE + RA_CPU_WRP_PLL_REG)
#define MEMPLL_CTRL_BASE				0xF7940008			//(MEMMAP_MCTRLSS_REG_BASE + RA_mc_wrap_memPll)


/*******************************************************************************
 * Platform memory map related constants
 ******************************************************************************/
#define DRAM_BASE				(0x0)
#define DRAM_SIZE				(0x80000000)

/*******************************************************************************
 *Low power state Macro
 ******************************************************************************/
#define CONFIG_GENX				1
#define MAX_WAKEUP_IRQS 8
#define IRQ_TOTAL_IRQ 0x6D
// Hash calculate
//#define BCM_CALCULATE_HASH		1
#define BCM_KERNEL_STORE_HASH		0x32

#endif /* __BERLIN_DEF_H__ */
