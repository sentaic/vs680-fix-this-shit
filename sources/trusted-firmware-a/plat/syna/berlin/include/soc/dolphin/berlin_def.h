/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BERLIN_DEF_H__
#define __BERLIN_DEF_H__

#include <platform_def.h>

//#define CONFIG_FPGA 1

#define BERLIN_TZDRAM_BASE				0x00120000
#define BERLIN_TZDRAM_SIZE				0x00040000

#define BERLIN_DEVICE0_BASE				0xF6000000
#define BERLIN_DEVICE0_SIZE				0x04000000

#define BERLIN_CLUSTER_COUNT				1
#define BERLIN_MAX_CPUS_PER_CLUSTER			4
#define BERLIN_PRIMARY_CPU					0x0

#ifdef CONFIG_FPGA
#define BERLIN_SYSCNT_FREQ				250000
#else
#define BERLIN_SYSCNT_FREQ				25000000
#endif

#define BERLIN_PSTATEID_STANDBY				1
#define BERLIN_PSTATEID_POWERDOWN			3
#define BERLIN_PSTATEID_MASK				0xf

#define BERLIN_ACTIVE_STANDBY				(PLAT_MAX_OFF_STATE - 1)
#define BERLIN_SUSPEND						PLAT_MAX_OFF_STATE

#define BERLIN_RESUME						0x00000001
#define BERLIN_RESUME_CPU_ON				0x00000002

/* arch timer */
#define TCTL_BASE					0xF7921000

/* uart */
#define UART_0_BASE					0xF7FCD000
#define BERLIN_UART_CLOCK			0
#define BERLIN_UART_BAUDRATE		115200

/* GIC */
#define GIC_DIST_BASE					0xF7901000
#define GIC_CPU_BASE					0xF7902000
#define BERLIN_G0_IRQS					42
#define BERLIN_IRQ_SEC_SGI_0		8
#define BERLIN_IRQ_SEC_SGI_1		9
#define BERLIN_IRQ_SEC_SGI_2		10
#define BERLIN_IRQ_SEC_SGI_3		11
#define BERLIN_IRQ_SEC_SGI_4		12
#define BERLIN_IRQ_SEC_SGI_5		13
#define BERLIN_IRQ_SEC_SGI_6		14
#define BERLIN_IRQ_SEC_SGI_7		15

/* Reset vector */
#define CFG64_REG_BASE					0xF7920800
#define RVBARADDR0						3

#define MEMMAP_CHIP_CTRL_REG_BASE	0xF7EA0000
#define RA_Gbl_ResetTrigger			0x0600
#define RA_Gbl_perifStickyResetN	0x0688

#define SOC_RESET_CTRL_BASE		(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger)
#define SOC_REST_PERIF_BASE		(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_perifStickyResetN)

//PCIE
#define MEMMAP_PCIE_REG_BASE		0xF7E40000
#define RA_PCIE_AXI_LIMIT_LOWER		0xA050
#define RA_PCIE_AXI_LIMIT_UPPER		0xA054
#define RA_PCIE_MAC_CTRL			0xA01C

#define PCIE_LOWER_BOUNDARY_ADDR	(MEMMAP_PCIE_REG_BASE + RA_PCIE_AXI_LIMIT_LOWER)
#define PCIE_UPPER_BOUNDARY_ADDR	(MEMMAP_PCIE_REG_BASE + RA_PCIE_AXI_LIMIT_UPPER)
#define PCIE_MAC_CTRL				(MEMMAP_PCIE_REG_BASE + RA_PCIE_MAC_CTRL)
#define LSb32PCIE_MAC_CTRL_ADDR_FILTER_FLAG		8

#define SM_SEND_MSGQ			0xF7F9F400
#define SMREG_BASE				0xF7FE2000 //SOC_SM_MEMMAP_SMREG_BASE
#define RA_smSysCtl_SM_BOOT_STATUS	0x38

#define GBL_TSPCLK					0xF7EA0790

#define SUSPEND_SRAM_BASE		0xF9000000
#define SRAM_BACKUP_SIZE			0x20000

#define DDR_REG_BASE					0xF7CB0000
#define AVIO_REG_BASE					0xF7400000

#define CPU_WRP_PLL_REG_BASE			0xF7922000
#define MC6CTRL_MEMPLL_BASE				0xF7940038
#define GBL_SYSPLL0_BASE				0xF7EA0200
#define GBL_SYSPLL1_BASE				0xF7EA0220
#define GBL_SYSPLL2_BASE				0xF7EA0240

//fix me
#define MEMMAP_TSI_REG_BASE				0xF7CC0000
#define RA_TSSS_KILOPASS_OTP			0x24000
#define RA_ShadowReg_NAGRA_OTP_Field	0x100
#define RA_NAGRA_OTP_TEE_CERT			0x4C
#define RA_NAGRA_OTP_CHIP_TYPE			0xE8
#define KILOPASS_OTP_BASE		(MEMMAP_TSI_REG_BASE + 			\
					RA_TSSS_KILOPASS_OTP + 			\
					RA_ShadowReg_NAGRA_OTP_Field)
#define TEE_PROTECT_SHIFT_OFFSET 	10
#define CERT_PROTECT_SHIFT_OFFSET	8

/*******************************************************************************
 *Low power state Macro
 ******************************************************************************/
#define MAX_WAKEUP_IRQS			8
#define IRQ_TOTAL_IRQ			0x7A
#define IRQ_sm2socSwInt			0x13
//#define CFG_SUSPEND_WAKEUP_INTR IRQ_sm2socSwInt
#define BCM_KERNEL_STORE_HASH		0x32

#endif /* __BERLIN_DEF_H__ */
