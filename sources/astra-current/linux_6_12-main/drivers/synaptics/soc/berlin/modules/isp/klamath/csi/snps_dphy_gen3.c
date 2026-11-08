/*
 * Synopsys MIPI D-PHY GEN 3 driver
 *
 * Copyright (C) 2015 Synopsys
 * Author: Ramiro Oliveira <roliveir@synopsys.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "snps_dphy_csi2.h"
#include "snps_dphy_wrap.h"
#include "snps_dphy_gen3.h"
#include "csihost.h"
#include "klamath_memmap.h"

#define EQ_REG_LANE0 0x509
#define EQ_REG_LANE1 0x709
#define MSB_OFFSET   0

#define PHY_SHUTDOWNZ   0x40

/* Note: There is no CSI1, legacy code */
#define MIPI_CSI0_BASE  0
#define MIPI_DPHY_BASE  PHY_SHUTDOWNZ /*Base address for PHY_SHUTDOWNZ*/

#define MIPI_PHYCTRL0_BASE (MIPI_CSI0_BASE + RA_CSIHOST_GENH)

#define MIPI_PHYATE0_BASE  (MIPI_CSI0_BASE + RA_CSIHOST_GENH)

#define COMPATIBITLTY_MODE_WAIT_COUNT 0x2F00

struct mipi_csi_wrapper mipi_gbl_info[DW_MIPI_DEV_MAX] = {
	{MIPI_CSI0_BASE, (MIPI_CSI0_BASE + MIPI_DPHY_BASE), MIPI_PHYCTRL0_BASE,
	 MIPI_PHYATE0_BASE, 0x1},
};

static void phy_write_part(struct snps_dphy *dev,  unsigned long address,
		unsigned long data, unsigned char shift, unsigned char width)
{
	u32 mask = (1 << width) - 1;
	u32 temp = phy_read(dev, address);

	temp &= ~(mask << shift);
	temp |= (data & mask) << shift;
	phy_write(dev, address, temp);
}

void snps_dphy_te_write(struct snps_dphy *dev,
		u8 address, u8 *data, u8 data_length)
{
	unsigned int i = 0;

	if (data != 0) {

		phy_write(dev, R_CSI2_DPHY_TST_CTRL0, 0);
		phy_write(dev, R_CSI2_DPHY_TST_CTRL1, 0);
		/* set TESTEN input high  */
		phy_write_part(dev, R_CSI2_DPHY_TST_CTRL1, 1, 16, 1);
		/*
		 * set the TESTCLK input high in preparation to latch in the desired
		 * test mode
		 */
		phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 1, 1, 1);
		/* set the desired test code in the input 8-bit bus TESTDIN[7:0] */

		phy_write_part(dev, R_CSI2_DPHY_TST_CTRL1, address, 0, 8);
		/*
		 * drive the TESTCLK input low; the falling edge captures the chosen
		 * test code into the transceiver
		 */
		phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 0, 1, 1);
		/* set TESTEN input low to disable further test mode code latching  */
		phy_write_part(dev, R_CSI2_DPHY_TST_CTRL1, 0, 16, 1);
		/* start writing MSB first */
		for (i = data_length; i > 0; i--) {
			/* set TESTDIN[7:0] to the desired test data appropriate to the
			 * chosen test mode
			 */
			phy_write(dev, R_CSI2_DPHY_TST_CTRL1, data[i - 1]);
			/* pulse TESTCLK high to capture this test data into the macrocell;
			 * repeat these two steps as necessary
			 */
			phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 1, 1, 1);

			phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 0, 1, 1);
		}

		if (dev->lanes != LANES_8)
			return; /* Send codes for second DPHY */

		/*
		 * set the TESTCLK input high in preparation to latch in the desired
		 * test mode
		 */
		phy_write_part(dev, R_CSI2_DPHY2_TST_CTRL0, 1, 1, 1);
		/* set the desired test code in the input 8-bit bus TESTDIN[7:0] */

		phy_write(dev, R_CSI2_DPHY2_TST_CTRL1, address);
		/* set TESTEN input high  */
		phy_write_part(dev, R_CSI2_DPHY2_TST_CTRL1, 1, 16, 1);
		/*
		 * drive the TESTCLK input low; the falling edge captures the chosen
		 * test code into the transceiver
		 */
		phy_write_part(dev, R_CSI2_DPHY2_TST_CTRL0, 0, 1, 1);
		/* set TESTEN input low to disable further test mode code latching  */
		phy_write_part(dev, R_CSI2_DPHY2_TST_CTRL1, 0, 16, 1);
		/* start writing MSB first */
		for (i = data_length; i > 0; i--) {
			/*
			 * set TESTDIN[7:0] to the desired test data appropriate to the
			 * chosen test mode
			 */
			phy_write(dev, R_CSI2_DPHY2_TST_CTRL1, data[i - 1]);
			/*
			 * pulse TESTCLK high to capture this test data into the macrocell;
			 * repeat these two steps as necessary
			 */
			phy_write_part(dev, R_CSI2_DPHY2_TST_CTRL0, 1, 1, 1);
			phy_write_part(dev, R_CSI2_DPHY2_TST_CTRL0, 0, 1, 1);
		}
	}
}

int snps_dphy_te_read(struct snps_dphy *dev, unsigned int addr)
{
	uint8_t ret;

	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 0, 0, 1);
	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL1, 1, 16, 1);
	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 1, 1, 1);

	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL1, addr, 0, 8);

	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 0, 1, 1);

	phy_write_part(dev, R_CSI2_DPHY2_TST_CTRL1, 0, 16, 1);

	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL1, 0x00, 0, 8);

	ret = phy_read_part(dev, R_CSI2_DPHY_TST_CTRL1, 8, 8);

	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL1, 0, 16, 1);

	return ret;
}


static void snps_dphy_testport_write(struct snps_dphy *dev, u8 addr, u8 data, u8 length)
{
	snps_dphy_te_write(dev, addr, &data, length);
}

static int snps_dphy_configure(struct snps_dphy *dev)
{
	u32 input_freq = dev->dphy_freq;
	u32 lanes = dev->max_lanes;
	unsigned char range = 0; /* ranges iterator */
	unsigned char osc_freq_target_msb, osc_freq_target_lsb;
	unsigned int timeout = COMPATIBITLTY_MODE_WAIT_COUNT;
	unsigned int isDDlComplete1 = 0;
	unsigned int isDDlComplete2 = 0;
	unsigned int temp = 0;
	unsigned int comp_mode = 0;
	int val;

	comp_mode = dev->comp_en;

	for (range = 0;
			(range < ARRAY_SIZE(range_gen3) - 1) &&
			((input_freq / 1000) > range_gen3[range].freq); range++) {
		;
	}
	csi2_dphy_setphy_cfgclkoff(dev, 1); /* switch off config clock */

	phy_write_part(dev, R_CSI2_DPHY_RSTZ, 0, 0, 1);
	phy_write_part(dev, R_CSI2_DPHY_SHUTDOWNZ, 0, 0, 1);

	/* phy test clear */
	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 1, 0, 1);
	/* FIXME: delay 15ns */
	phy_write_part(dev, R_CSI2_DPHY_TST_CTRL0, 0, 0, 1);

	/* program hsfreqrange */
	snps_dphy_testport_write(dev, 0x1, 0x20, 1);
	snps_dphy_testport_write(dev, 0x2, range_gen3[range].hsfregrange, 1);
	csi2_dphy_sethsfreqrange(dev, range_gen3[range].hsfregrange);

	/*0x1AB = 6*/
	snps_dphy_testport_write(dev, 0x0, 0x1, 1);
	snps_dphy_testport_write(dev, 0xAB, 0x6, 1);

	/*0x1AC = 1*/
	snps_dphy_testport_write(dev, 0x0, 0x1, 1);
	snps_dphy_testport_write(dev, 0xAC, 0x4B, 1);
	snps_dphy_testport_write(dev, 0x0, 0, 1);

	snps_dphy_testport_write(dev, 0x0, 0x3, 1);
	snps_dphy_testport_write(dev, 0x7, 0x80, 1);
	snps_dphy_testport_write(dev, 0x0, 0x0, 1);

	/*Compatibility mode*/
	if ((comp_mode) || ((input_freq/1000) > 1500)) {
		osc_freq_target_lsb = range_gen3[range].osc_freq_target & 0xFF;
		osc_freq_target_msb = (range_gen3[range].osc_freq_target>>8) & 0xF;
		snps_dphy_testport_write(dev, 0xe2, osc_freq_target_lsb, 1);
		snps_dphy_testport_write(dev, 0xe3, osc_freq_target_msb, 1);
		val = snps_dphy_te_read(dev, 0xe4);
		val |= 0x1;
		snps_dphy_testport_write(dev, 0xe4, val, 1);
	} else {
		snps_dphy_testport_write(dev, 0x0, 0x6, 1);
		snps_dphy_testport_write(dev, 0x7, 0x3F, 1);
		snps_dphy_testport_write(dev, 0x0, 0x8, 1);
		snps_dphy_testport_write(dev, 0x7, 0x3F, 1);
		snps_dphy_testport_write(dev, 0x0, 0xA, 1);
		snps_dphy_testport_write(dev, 0x7, 0x3F, 1);
		snps_dphy_testport_write(dev, 0x0, 0xC, 1);
		snps_dphy_testport_write(dev, 0x7, 0x3F, 1);
	}
	snps_dphy_testport_write(dev, 0x0, 0, 1);
	/*deskew_numedges_rw[7:0] = 0x43*/
	if ((input_freq/1000) > 1500)
		snps_dphy_testport_write(dev, 0xA, 0x43, 1);
	else
		snps_dphy_testport_write(dev, 0x8, 0x38, 1);/*deskew_polarity_rw signal[bit 5] = 1*/

	snps_dphy_testport_write(dev, 0x0, 0x0, 1);

	/*set cfgclk = 25MHz, (25-17) * 4*/
	csi2_dphy_setclkfreqrange(dev, 0x20);
	csi2_dphy_setphy_cfgclkoff(dev, 0); /* switch on config clock */
	csi2_dphy_basedir(dev, 0x1);
	csi2_dphy_forcerxmode(dev, 0x3);
	csi2_dphy_setphy_ppienableclk(dev, 1);

	//FIXME: Set n_lanes before calling this function
	/*read with timeout*/
	/*Compatibility mode*/
	if (comp_mode) {
		do {
			snps_dphy_testport_write(dev, 0x0, 0x5, 1);
			isDDlComplete1 = snps_dphy_te_read(dev, 0xe0);
			snps_dphy_testport_write(dev, 0x0, 0x7, 1);
			isDDlComplete2 = snps_dphy_te_read(dev, 0xe0);
			if (timeout)
				timeout = timeout - 1;
			else
				break;
		} while (!((isDDlComplete1 & (1<<2)) || (isDDlComplete2 & (1<<2))));

		if (!timeout)
			pr_err("compatibility_mode_wait timeout\n");
		/* Enable Compatibility Mode - Start */
		snps_dphy_testport_write(dev, 0x0, 0, 1);
		temp = snps_dphy_te_read(dev, 0xe0);
		temp |= (1 << 7);
		snps_dphy_testport_write(dev, 0x0, 0, 1);
		snps_dphy_testport_write(dev, 0xe0, temp, 1);
		snps_dphy_testport_write(dev, 0x0, 0, 1);
		temp = snps_dphy_te_read(dev, 0xe1);
		temp |= (1 << 0);
		snps_dphy_testport_write(dev, 0x0, 0, 1);
		snps_dphy_testport_write(dev, 0xe1, temp, 1);
		pr_err("compatibility_mode enabled\n");
		/* Enable Compatibility Mode - End */
	}

	phy_write_part(dev, R_CSI2_DPHY_SHUTDOWNZ, 1, 0, 1);
	phy_write_part(dev, R_CSI2_DPHY_RSTZ, 1, 0, 1);

	/* Check stop state clock and data before releasing rxmode */
	csi2_dphy_stopstate_wait(dev, lanes);
	csi2_dphy_forcerxmode(dev, 0x0);
	/* csi2_dphy_skewcalib_wait(dev); */
	return 0;
}

void snps_dphy_seteq(struct snps_dphy *dev, int eq)
{
	u8 data, addr;

	/* Lane0 */
	addr = (EQ_REG_LANE0 >> 8) & 0xf;
	snps_dphy_te_write(dev, MSB_OFFSET, &addr, 1);
	addr = EQ_REG_LANE0 & 0xff;
	data = snps_dphy_te_read(dev, addr);

	data = (data & ~0x7) | eq;
	addr = (EQ_REG_LANE0 >> 8) & 0xf;
	snps_dphy_te_write(dev, MSB_OFFSET, &addr, 1);
	addr = EQ_REG_LANE0 & 0xff;
	snps_dphy_te_write(dev, addr, &data, 1);
	data = snps_dphy_te_read(dev, addr);
	pr_err("Lane0 EQ = 0x%x\n", data);

	/* Lane1 */
	addr = (EQ_REG_LANE1 >> 8) & 0xf;
	snps_dphy_te_write(dev, MSB_OFFSET, &addr, 1);
	addr = EQ_REG_LANE1 & 0xff;
	data = snps_dphy_te_read(dev, addr);

	data = (data & ~0x7) | eq;
	addr = (EQ_REG_LANE1 >> 8) & 0xf;
	snps_dphy_te_write(dev, MSB_OFFSET, &addr, 1);
	addr = EQ_REG_LANE1 & 0xff;
	snps_dphy_te_write(dev, addr, &data, 1);
	pr_err("Lane1 EQ = 0x%x\n", data);
}

int __set_phy_state(struct snps_dphy *state, unsigned int on)
{
	if (on) {
		snps_dphy_configure(state);
	} else {
		phy_write_part(state, R_CSI2_DPHY_SHUTDOWNZ, 0, 0, 1);
		phy_write_part(state, R_CSI2_DPHY_TST_CTRL0, 0, 1, 1);

		if (state->lanes == LANES_8)
			phy_write_part(state, R_CSI2_DPHY2_TST_CTRL0, 0, 1, 1);
	}
	return 0;
}

int snps_dphy_probe(struct snps_dphy *state, int index)
{
	state->base_address = state->mipi_base + mipi_gbl_info[index].dphy_base;
	state->phyctrl_base = (uint64_t) state->mipi_base + mipi_gbl_info[index].phyctrl_base;
	state->phyate_base = (uint64_t) state->mipi_base + mipi_gbl_info[index].phyate_base;
	if (IS_ERR(state->base_address)) {
		pr_err("Error requesting base address\n");
		return PTR_ERR(state->base_address);
	}
	/* TODO Get this value from subdev ctrl */
	state->dphy_freq = 280000; /* OV5647 VGA */
	state->max_lanes = 2;
	state->ref_clk = 25000;
	if (state->max_lanes == 4)
		state->lanes = LANES_4;
	else
		state->lanes = LANES_8;
	return 0;
}
