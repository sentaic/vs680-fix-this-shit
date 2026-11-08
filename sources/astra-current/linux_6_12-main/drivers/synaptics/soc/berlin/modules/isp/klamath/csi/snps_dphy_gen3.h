/* SPDX-License-Identifier: GPL-2.0 */
/**
 * @file snps_dphy_gen3.h
 * @brief Synopsys D-PHY GEN3 header
 *
 * Copyright (C) 2015 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */

#ifndef __SNPS_DPHY_GEN3__
#define __SNPS_DPHY_GEN3__

struct mipi_csi_wrapper {
	unsigned int csi2_base;
	unsigned int dphy_base;
	unsigned int phyctrl_base;
	unsigned int phyate_base;
	unsigned int irq_num;
};

#define  DW_MIPI_DEV_MAX  1

int snps_dphy_te_read(struct snps_dphy *dev, unsigned int addr);
void snps_dphy_te_write(struct snps_dphy *dev, u8 address, u8 *data, u8 data_length);
void snps_dphy_seteq(struct snps_dphy *dev, int eq);
int __set_phy_state(struct snps_dphy *state, unsigned int on);

#endif
