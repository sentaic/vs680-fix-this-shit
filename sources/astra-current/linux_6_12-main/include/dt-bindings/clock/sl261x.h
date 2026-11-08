/* SPDX-License-Identifier: (GPL-2.0 OR MIT) */
/*
 * Synaptics SL261X clock tree IDs
 *
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * Author: Jisheng Zhang <jszhang@kernel.org>
 */

/* common clks */
#define CLK_CPUFASTREF		0
#define CLK_MEMFASTREF		1
#define CLK_CFG			2
#define CLK_SYS			3
#define CLK_PERIFSYS		4
#define CLK_APBCORE		5
#define CLK_APBSER		6
#define CLK_ATB			7
#define CLK_HPC			8
#define CLK_EMMC		9
#define CLK_SD0			10
#define CLK_SD1			11
#define CLK_GETHRGMII		12
#define CLK_GETHRGMII1		13
#define CLK_GE0_PTP_REF		14
#define CLK_GE1_PTP_REF		15
#define CLK_USB2TEST		16
#define CLK_USB2TEST480MG0	17
#define CLK_USB2TEST480MG1	18
#define CLK_USB2TEST480MG2	19
#define CLK_USB2TEST100MG0	20
#define CLK_USB2TEST100MG1	21
#define CLK_USB2TEST100MG2	22
#define CLK_USB2TEST100MG3	23
#define CLK_PERIFTEST125MG0	24
#define CLK_PERIFTEST200MG0	25
#define CLK_PERIFTEST200MG1	26
#define CLK_GPU			27
#define CLK_NPU			28
#define CLK_AVIOSYS		29
#define CLK_AIOSYS		30
#define CLK_AVIO_LCDC2SCAN	31
#define CLK_AVIO_IPI		32
#define CLK_AVIO_P		33
#define CLK_AVIO_DPHYRXTXESC	34
#define CLK_AVIOFPLL		35
#define CLK_AVIO_RX_SCANBYTE	36
#define CLK_AVIO_RX_SCANTEST	37

/* gate clks */
#define CLK_USB0CORE		0
#define CLK_SDIOSYS		1
#define CLK_EMMCSYS		2
#define CLK_GPUAXI		3
#define CLK_GETHRGMIISYS	4
#define CLK_SDIO1SYS		5
#define CLK_USB1CORE		6
#define CLK_GETHRGMII1SYS	7
#define CLK_USB0PHYREF		8
#define CLK_USB1PHYREF		9
#define CLK_APBUART0		10
#define CLK_APBUART1		11
#define CLK_APBUART2		12
#define CLK_APBUART3		13
#define CLK_APBI2C0		14
#define CLK_APBI2C1		15
#define CLK_APBSPI0		16
#define CLK_APBSPI1		17
#define CLK_APBSPI2		18
#define CLK_APBSPI3		19
#define CLK_APBGPIO		20
#define CLK_APBTIMERS		21
#define CLK_APBSYSCNT		22
#define CLK_APBWDT		23
#define CLK_APBDMA		24

/* sysmgr clks */
#define CLK_CFG_UART0		0
#define CLK_SER_UART0		1
#define CLK_CFG_UART1		2
#define CLK_CFG_UART2		3
#define CLK_CFG_UART3		4
#define CLK_CFG_GPIO		5
#define CLK_CFG_I2CM0		6
#define CLK_CFG_I2CM1		7
#define CLK_CFG_SPIM		8
#define CLK_CFG_SPIS		9
#define CLK_CFG_I3C		10
#define CLK_CFG_XSPI		11
#define CLK_AXI_XSPI		12
#define CLK_CFG_PVT		13
#define CLK_CFG_ADC		14
#define CLK_CFG_PWM		15
#define CLK_BUS_CAN0		16
#define CLK_BUS_CAN1		17
#define CLK_CFG_PDM		18

#define CLK_SER_UART1		19
#define CLK_SER_UART2		20
#define CLK_SER_UART3		21
#define CLK_DEB_GPIO		22
#define CLK_SER_I2CM0		23
#define CLK_SER_I2CM1		24
#define CLK_SER_SPIM		25
#define CLK_SER_SPIS		26
#define CLK_SER_I3C		27
#define CLK_SER_XSPI		28
#define CLK_HS_PVT		29
#define CLK_CORE_ADC		30
#define CLK_SER_PWM		31
#define CLK_SER_CAN0		32
#define CLK_SER_CAN1		33
#define CLK_SER_PDM		34
#define CLK_REF_CALIB		35
