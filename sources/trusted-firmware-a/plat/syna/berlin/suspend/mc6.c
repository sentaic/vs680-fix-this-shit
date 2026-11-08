/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <assert.h>
#include <lib/mmio.h>
#include "mc6.h"
#include "mc6_regs.h"

#define RD_MC6_REG(offset , data)	\
		data = mmio_read_32((unsigned long)((char*)base + offset))

#define WR_MC6_REG(offset , value)	\
		mmio_write_32((unsigned long)((char*)base + offset), value)

static unsigned int dram_config_5_bak;
static unsigned int phy_control_3_bak;

/*
 * DDR enter self-refresh
 */
static void ddr_enter_self_refresh(void *base)
{
	unsigned int data;

	// halt scheduler & turn on dfc
	RD_MC6_REG(MC6_MC_Control_0, data);
	data |= 0x3;
	WR_MC6_REG(MC6_MC_Control_0, data);

	// turn off dll
	RD_MC6_REG(MC6_DRAM_Config_2, data);
	data |= 0x10;
	WR_MC6_REG(MC6_DRAM_Config_2, data);

	// turn off odt
	RD_MC6_REG(MC6_DRAM_Config_5_CS0, data);
	dram_config_5_bak = data;
	data &= 0xfcf8ffff;
	WR_MC6_REG(MC6_DRAM_Config_5_CS0, data);

	// mrs1
	WR_MC6_REG(MC6_USER_COMMAND_2, 0x11000200);

	// overwrite adcm drv to 0xff before SRE [Shun 20180414]
	RD_MC6_REG(MC6_PHY_Control_3, data);
	phy_control_3_bak = data;
	data |= 0xff << 20;
	WR_MC6_REG(MC6_PHY_Control_3, data);

	// check sr status
	WR_MC6_REG(MC6_USER_COMMAND_0, 0x11000040);
	do
	{
		RD_MC6_REG(MC6_DRAM_STATUS, data);
	}while(!(data & 0x4));

}

/*
 * DDR exit self-refresh
 */
static void ddr_exit_self_refresh(void *base)
{
	unsigned int data;

	WR_MC6_REG(MC6_PHY_Control_9, 0x20000000);
	WR_MC6_REG(MC6_PHY_Control_9, 0x40000000);
	WR_MC6_REG(MC6_PHY_Control_9, 0x80000000);

	// exit sr
	WR_MC6_REG(MC6_USER_COMMAND_0, 0x11000080);
	do
	{
		RD_MC6_REG(MC6_DRAM_STATUS, data);
	}while((data & 0x4));

	// recover adcm drv
	WR_MC6_REG(MC6_PHY_Control_3, phy_control_3_bak);

	// enable dll
	RD_MC6_REG(MC6_DRAM_Config_2, data);
	data &= ~0x10;
	WR_MC6_REG(MC6_DRAM_Config_2, data);

	// turn on odt
	WR_MC6_REG(MC6_DRAM_Config_5_CS0, dram_config_5_bak);

	// dll reset
	RD_MC6_REG(MC6_DRAM_Config_3, data);
	data |= 0x1;
	WR_MC6_REG(MC6_DRAM_Config_3, data);

	// mrw2
	WR_MC6_REG(MC6_USER_COMMAND_2, 0x11000400);

	// mrw1
	WR_MC6_REG(MC6_USER_COMMAND_2, 0x11000200);

	// mrw0
	WR_MC6_REG(MC6_USER_COMMAND_2, 0x11000100);

	// zq long
	WR_MC6_REG(MC6_USER_COMMAND_0, 0x11001000);

	// start scheduler & leave dfc
	RD_MC6_REG(MC6_MC_Control_0, data);
	data &= ~0x3;
	WR_MC6_REG(MC6_MC_Control_0, data);
}

/*
 * Power down the PHY of MC6
 */
static void mc_phy_power_down(void *base)
{
	unsigned int data;

	RD_MC6_REG(MC6_PHY_Control_13, data);
	data |= 0x3F3FFFF;
	WR_MC6_REG(MC6_PHY_Control_13, data);

	RD_MC6_REG(MC6_PHY_Control_14, data);
	data |= 0x3FFFFF;
	WR_MC6_REG(MC6_PHY_Control_14, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B0, data);
	data |= 0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B0, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B1, data);
	data |= 0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B1, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B2, data);
	data |= 0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B2, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B3, data);
	data |= 0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B3, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B4, data);
	data |= 0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B4, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B5, data);
	data |= 0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B5, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B6, data);
	data |= 0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B6, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B7, data);
	data |= 0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B7, data);
}

/*
 * Power up the PHY of MC6
 */
static void mc_phy_power_up(void* base)
{
	unsigned int data;

	RD_MC6_REG(MC6_PHY_Control_13, data);
	data &= ~0x3F3FFFF;
	WR_MC6_REG(MC6_PHY_Control_13, data);

	RD_MC6_REG(MC6_PHY_Control_14, data);
	data &= ~0x3FFFFF;
	WR_MC6_REG(MC6_PHY_Control_14, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B0, data);
	data &= ~0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B0, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B1, data);
	data &= ~0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B1, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B2, data);
	data &= ~0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B2, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B3, data);
	data &= ~0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B3, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B4, data);
	data &= ~0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B4, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B5, data);
	data &= ~0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B5, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B6, data);
	data &= ~0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B6, data);

	RD_MC6_REG(MC6_PHY_Data_Byte_Control_B7, data);
	data &= ~0x700;
	WR_MC6_REG(MC6_PHY_Data_Byte_Control_B7, data);
}

static mem_config_t mem_config;

void mem_set_config(mem_config_t *config)
{
	assert(config != NULL);

	memcpy(&mem_config, config, sizeof(mem_config_t));

	assert(mem_config.mc6_base != NULL);
}

void mem_suspend(void)
{
	ddr_enter_self_refresh(mem_config.mc6_base);
	mc_phy_power_down(mem_config.mc6_base);
}

void mem_resume(void)
{
	mc_phy_power_up(mem_config.mc6_base);
	ddr_exit_self_refresh(mem_config.mc6_base);
}
