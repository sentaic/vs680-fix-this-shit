// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "snps_dphy_csi2.h"
#include "snps_dphy_wrap.h"
#include "csihost.h"

#define STOP_STATE_WAIT_COUNT  0x2F00

enum ISP_PHY_REGS {
	RA_ispPhyCtrl_CTRL = 0,
	RA_ispPhyCtrl_PPI_CTRL = 0x24,
	RA_ispPhy_PPI_STATUS0 = 0x28,
	RA_ispPhy_PPI_STATUS1 = 0x2C,
	RA_ispPhy_PPI_STATUS2 = 0x30,
};

void csi2_dphy_setclkfreqrange(struct snps_dphy *dev, int cfgclk)
{
	T32GENH_PHYCTRL ctrl;

	ctrl.u32 = ioread32((void *)dev->phyctrl_base + RA_ispPhyCtrl_CTRL);
	SET32GENH_PHYCTRL_cfgclkfreqrange(ctrl.u32, cfgclk);
	iowrite32(ctrl.u32, (void *)dev->phyctrl_base + RA_ispPhyCtrl_CTRL);
}

void csi2_dphy_sethsfreqrange(struct snps_dphy *dev, int hsfreq)
{
	T32GENH_PHYCTRL ctrl;

	ctrl.u32 = ioread32((void *)dev->phyctrl_base + RA_ispPhyCtrl_CTRL);
	SET32GENH_PHYCTRL_hsfreqrange(ctrl.u32, hsfreq);
	iowrite32(ctrl.u32, (void *)dev->phyctrl_base + RA_ispPhyCtrl_CTRL);
}

void csi2_dphy_basedir(struct snps_dphy *dev, int en)
{
	T32DPHYRX_PPI_CTRL ctrl;

	ctrl.u32 = ioread32((void *)dev->phyctrl_base + RA_ispPhyCtrl_PPI_CTRL);
	SET32DPHYRX_PPI_CTRL_basedir_0(ctrl.u32, en);
	iowrite32(ctrl.u32, (void *)dev->phyctrl_base + RA_ispPhyCtrl_PPI_CTRL);
}

void csi2_dphy_forcerxmode(struct snps_dphy *dev, int rx)
{
	T32DPHYRX_PPI_CTRL ctrl;

	ctrl.u32 = ioread32((void *)dev->phyctrl_base + RA_ispPhyCtrl_PPI_CTRL);
	SET32DPHYRX_PPI_CTRL_forcerxmode_N(ctrl.u32, rx);
	iowrite32(ctrl.u32, (void *)dev->phyctrl_base + RA_ispPhyCtrl_PPI_CTRL);
}

void csi2_dphy_setphy_cfgclkoff(struct snps_dphy *dev, int off)
{
	T32GENH_PHYCTRL ctrl;

	ctrl.u32 = ioread32((void *)dev->phyctrl_base + RA_ispPhyCtrl_CTRL);
	SET32GENH_PHYCTRL_cfg_clk_off(ctrl.u32, off);
	iowrite32(ctrl.u32, (void *)dev->phyctrl_base + RA_ispPhyCtrl_CTRL);
}

void csi2_dphy_setphy_ppienableclk(struct snps_dphy *dev, int en)
{
	uint32_t ctrl;

	ctrl = ioread32((void *)(dev->phyctrl_base + RA_ispPhyCtrl_PPI_CTRL));
	SET_BIT(ctrl, en, 18, 1); /* enableclk */
	SET_BIT(ctrl, en, 25, 1); /* enable_N_lane0 */
	SET_BIT(ctrl, en, 26, 1); /* enable_N lane1 */
	iowrite32(ctrl, (void *)(dev->phyctrl_base + RA_ispPhyCtrl_PPI_CTRL));
}

void csi2_dphy_stopstate_wait(struct snps_dphy *dev, unsigned int lanes)
{
	TDPHYRX_PPI_STATUS2 status;
	unsigned int timeout = STOP_STATE_WAIT_COUNT;
	unsigned int lane_mask = ((1<<lanes) - 1);

	do {
		status.u32[0] = ioread32((void *)dev->phyate_base + RA_ispPhy_PPI_STATUS2);
		if (timeout)
			timeout = timeout - 1;
		else
			break;
	} while (!status.uSTATUS2_stopstateclk);

	if (!timeout)
		pr_info("state clock wait timeout:0x%x\r\n", status.u32[0]);
	timeout = STOP_STATE_WAIT_COUNT;

	do {
		status.u32[0] = ioread32((void *)dev->phyate_base + RA_ispPhy_PPI_STATUS2);
		if (timeout)
			timeout = timeout - 1;
		else
			break;
	} while (!(status.uSTATUS2_stopstatedata_N & lane_mask));

	if (!timeout)
		pr_info("state clock data timeout:0x%x\r\n", status.u32[0]);

	pr_debug("base: 0x%llx status: 0x%x\r\n", dev->phyate_base, status.u32[0]);
}
