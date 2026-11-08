/**
 * @file snps_dphy_csi2.h
 * @brief Synopsys D-PHY CSI-2 header
 *
 * Copyright (C) 2015 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */

#ifndef __SNPS_DPHY_CSI2_H__
#define __SNPS_DPHY_CSI2_H__

#include <linux/delay.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>

/** @short DPHY interface register bank*/
#define R_CSI2_DPHY_SHUTDOWNZ   0x0
#define R_CSI2_DPHY_RSTZ    0x4
#define R_CSI2_DPHY_RX      0x8
#define R_CSI2_DPHY_STOPSTATE   0xC
#define R_CSI2_DPHY_TST_CTRL0   0x10
#define R_CSI2_DPHY_TST_CTRL1   0x14
#define R_CSI2_DPHY2_TST_CTRL0  0x18
#define R_CSI2_DPHY2_TST_CTRL1  0x1C

enum tst_ctrl0 {
	PHY_TESTCLR     = 0,
	PHY_TESTCLK     = 1,
};

enum tst_ctrl1 {
	PHY_TESTDIN     = 0,
	PHY_TESTDOUT    = 8,
	PHY_TESTEN  = 16,
};

#define GET_BIT_MASK(N_BIT)     ((1<<N_BIT) - 1)
#define GET_BIT(VALUE, BIT_POS, N_BIT)  ((VALUE & (GET_BIT_MASK(N_BIT) << BIT_POS)) >> BIT_POS)
#define SET_BIT(VARIABLE, VALUE, BIT_POS, N_BIT) \
	(VARIABLE = (VARIABLE & (~(GET_BIT_MASK(N_BIT) << BIT_POS))) | \
	 ((VALUE & GET_BIT_MASK(N_BIT)) << BIT_POS))

#define phy_write(dev, addr, data) do { \
	writel(data, dev->base_address + (addr)); \
	pr_debug("REGW 0X%lX = 0x%lx\n", (unsigned long)(addr), (unsigned long)(data)); \
} while (0)
#define phy_read(dev, addr) readl(dev->base_address + (addr))

enum n_lanes {
	LANES_4 = 0,
	LANES_8 = 1,
};


/** @short DPHY struct configuration */

struct snps_dphy {
	spinlock_t  slock;
	struct phy  *phy;
	uint32_t    dphy_freq;
	uint32_t    ref_clk;
	uint32_t    lanes;
	uint32_t    max_lanes;
	uint32_t    compat_mode;
	uint64_t    phyctrl_base;
	uint64_t    phyate_base;
	uint32_t    comp_en;
	void __iomem    *base_address;
	void __iomem    *gen3_regbank1;
	void __iomem    *gen3_regbank2;
	void *mipi_base;
};

struct range_dphy {
	unsigned long freq; /* upper margin of frequency range */
	unsigned char hsfregrange;
	unsigned int osc_freq_target;
};

int snps_dphy_power_on(struct snps_dphy *dphy);
int snps_dphy_init(struct snps_dphy *dphy);
int snps_dphy_probe(struct snps_dphy *state, int index);
void gen3_if1_write(struct snps_dphy *dev, unsigned int address, unsigned int data);
u32 phy_read_part(struct snps_dphy *dev, unsigned int address,
		unsigned char shift, unsigned char width);
void gen3_if2_write(struct snps_dphy *dev, unsigned int address, unsigned int data);
void dphy1_test_clr(struct snps_dphy *dev);

extern struct range_dphy range_gen3[65];
#endif
