/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#include <mm/generic_ram_layout.h>
#include <util.h>

/* Make stacks aligned to data cache line length */
#define CACHELINE_LEN		64
#define STACK_ALIGNMENT		CACHELINE_LEN

#define TZDRAM_BOOT_PARAM			0x100000
#define TZDRAM_BOOT_PARAM_SIZE		0x020000

#define TEE_RAM_START		TZDRAM_BASE

#define DRAM0_BASE	(TZDRAM_BASE + TZDRAM_SIZE)
#define DRAM0_SIZE	(0x80000000 - DRAM0_BASE)

#define DRAM1_BASE	(0x80000000)
#define DRAM1_SIZE	(0xE0000000- DRAM1_BASE)

/* GICv2 */
#define MEMMAP_GIC_REG_BASE	0xF7900000
#define GIC_BASE		MEMMAP_GIC_REG_BASE

#define MEMMAP_GIC_CPU_REG_BASE			0xF7902000
#define MEMMAP_GIC_DIST_REG_BASE		0xF7901000

#define GICD_BASE		MEMMAP_GIC_DIST_REG_BASE
#define GICC_BASE		MEMMAP_GIC_CPU_REG_BASE

#define GIC_IID_SPI(n)			(32 + (n))

#if defined(PLATFORM_FLAVOR_platypus)
/* UART */
#define CONSOLE_UART_BASE	0xf7fcd000
#define MEMMAP_MPT_REG_BASE 0xF9000000
#define MEMMAP_MPT_REG_SIZE 0x40000
#define SZC_BASE			(vaddr_t)phys_to_virt_io(MEMMAP_MPT_REG_BASE, 0x40000)

#define MEMMAP_SOC_REG_BASE  0xF7CA0000
#define MEMMAP_SOC_REG_SIZE  0x10000
#define MEMMAP_SOC_REG_VA_BASE	(vaddr_t)phys_to_virt_io(MEMMAP_SOC_REG_BASE, MEMMAP_SOC_REG_SIZE)

#define MAX_SZC_NUM 4
#define SZC_RULE_NUM_RULES_MAX      16
#define SZC_RULE_NUM_MASTERS_MAX    32
#define SZC_ZONE_NUM_ZONES_MAX      16
#define SZC_ZONE_NUM_MASTERS_MAX    32
#define CONFIG_MPT_RULE_NUM 2

#define CONFIG_SZC_ICTL_IRQ		GIC_IID_SPI(IRQ_szc_intr)

#elif  defined(PLATFORM_FLAVOR_dolphin)
#define CONSOLE_UART_BASE	0xf7fcd000
#define MEMMAP_MPT_REG_BASE 0xF9000000
#define MEMMAP_MPT_REG_SIZE 0x40000
#define SZC_BASE			(vaddr_t)phys_to_virt_io(MEMMAP_MPT_REG_BASE, MEMMAP_MPT_REG_SIZE)

#define MEMMAP_SOC_REG_BASE  0xF7CA0000
#define MEMMAP_SOC_REG_SIZE  0x10000
#define MEMMAP_SOC_REG_VA_BASE	(vaddr_t)phys_to_virt_io(MEMMAP_SOC_REG_BASE, MEMMAP_SOC_REG_SIZE)

#define MAX_SZC_NUM 5
#define SZC_RULE_NUM_RULES_MAX      16
#define SZC_RULE_NUM_MASTERS_MAX    32
#define SZC_ZONE_NUM_ZONES_MAX      16
#define SZC_ZONE_NUM_MASTERS_MAX    32
#define CONFIG_MPT_RULE_NUM 2

#define CONFIG_SZC_ICTL_IRQ		GIC_IID_SPI(IRQ_szc_intr)

// MC
#define CONFIG_AXIERRMON_NUM	5
#define CONFIG_AXIERRMON_IRQ	GIC_IID_SPI(IRQ_AxiErrMonIntr)
#define MC_REGS_POISONCFG		0x36c
#define MC_REGS_POISONSTAT		0x370

//(RA_mc_wrap_AxiErrorMon_Mst1 - RA_mc_wrap_AxiErrorMon_Mst0)
#define AxiErrorMon_OFFSET		0x18

#elif  defined(PLATFORM_FLAVOR_myna2)
#define CONSOLE_UART_BASE	0xF7E81400

#define MEMMAP_SOC_REG_BASE  0xF7CA0000
#define MEMMAP_SOC_REG_SIZE  0x10000
#define MEMMAP_SOC_REG_VA_BASE	(vaddr_t)phys_to_virt_io(MEMMAP_SOC_REG_BASE, MEMMAP_SOC_REG_SIZE)

//#define  MEMMAP_MPTS_REG_BASE 0xF9000000
#define MEMMAP_MPT_REG_BASE 0xF9000000
#define MEMMAP_MPT_REG_SIZE 0x40000

#define SZC_BASE			(vaddr_t)phys_to_virt_io(MEMMAP_MPT_REG_BASE, MEMMAP_MPT_REG_SIZE)

#define CONFIG_SZC_ICTL_IRQ		GIC_IID_SPI(IRQ_szc_intr)

#elif  defined(PLATFORM_FLAVOR_klamath)

#define CONSOLE_UART_BASE	0xe5030000

#ifdef GICD_BASE
#undef GICD_BASE
#define GICD_BASE		0xF7E59000 //START_GIC_REG + 0x1000
#endif
#ifdef GICC_BASE
#undef GICC_BASE
#define GICC_BASE		0xF7E5A000
#endif

#define BERLIN_DEVICE1_BASE                            0xe1000000
#define BERLIN_DEVICE1_SIZE                            0x00200000
#define BERLIN_DEVICE2_BASE                            0xe3000000
#define BERLIN_DEVICE2_SIZE                            0x04000000

#define START_SOC_REG  0xF7E20000
#define SIZE_SOC_REG   0x2000

#define MEMMAP_SOC_REG_BASE START_SOC_REG
#define MEMMAP_SOC_REG_SIZE SIZE_SOC_REG
#define MEMMAP_SOC_REG_VA_BASE	(vaddr_t)phys_to_virt_io(MEMMAP_SOC_REG_BASE, MEMMAP_SOC_REG_SIZE)

#define MEMMAP_MPT_REG_BASE 0xF9000000
#define MEMMAP_MPT_REG_SIZE 0x40000
#define SZC_BASE			(vaddr_t)phys_to_virt_io(MEMMAP_MPT_REG_BASE, MEMMAP_MPT_REG_SIZE)

#define MAX_SZC_NUM 2
#define SZC_ZONE_NUM_ZONES_MAX      16
#define SZC_ZONE_NUM_MASTERS_MAX    32

#else
#error "Unknown platform flavor"
#endif

#ifdef CONFIG_QEMU
#define CONSOLE_BAUDRATE		57600
#define CONSOLE_UART_CLK_IN_HZ	24000000
#else
#define CONSOLE_BAUDRATE		115200
#define CONSOLE_UART_CLK_IN_HZ	25000000
#endif

/* SoC Timer: clock 200MHz */
#define CONFIG_TIMER_CLOCK		(200 * 1000000)
#define CONFIG_TIMER1_ICTL_IRQ	GIC_IID_SPI(IRQ_apbPerif_timers_1_Intr0_irq)

#ifdef CFG_WITH_LPAE
#define MAX_XLAT_TABLES		24
#endif

#endif /*PLATFORM_CONFIG_H*/
