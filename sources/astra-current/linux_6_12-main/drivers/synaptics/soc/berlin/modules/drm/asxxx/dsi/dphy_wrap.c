/*
* INTERNAL USE ONLY
*
* Unpublished Work Copyright (C) 2013-2017 Synaptics Incorporated.
* All rights reserved.
*
* This file contains information that is proprietary to Synaptics
* Incorporated ("Synaptics"). The holder of this file shall treat all
* information contained herein as confidential, shall use the
* information only for its intended purpose, and shall not duplicate,
* disclose, or disseminate any of this information in any manner
* unless Synaptics has otherwise provided express, written
* permission.
*
* Use of the materials may require a license of intellectual property
* from a third party or from Synaptics. This file conveys no express
* or implied licenses to any intellectual property rights belonging
* to Synaptics.
*/
#include "includes.h"
#include "dsih_core.h"
#include "DPHYTX_release.h"

void mipi_dphy_BiuCtrlPHYEn(dphy_t *phy, int en);
void mipi_dphy_shutdown(dphy_t *phy, int shutdown);
void mipi_dphy_resetz(dphy_t *phy, int resetz);
void mipi_dphy_enable_lanes(dphy_t *phy, int lanes);
void mipi_dphy_EnableClkBIU(dphy_t *phy, int en);
void mipi_dphy_CfgClkFreqRange(dphy_t *phy, int range);
void mipi_dphy_pll_shadow_control_en(dphy_t *phy, int en);
void mipi_dphy_pll_clksel(dphy_t *phy, int clksel);
void mipi_dphy_stopstate_wait(dphy_t *phy, int lanes);


void mipi_dphy_BiuCtrlPHYEn(dphy_t *phy, int en)
{
	uint32_t ctrl;

	ioread32(phy->base + R_DPHYTX_DPHY_CTL0, &ctrl);
	MIPI_FIELD_SET(ctrl, MIPI_DPHY_BIUCTRLPHYEN, (en & 0x1));
	iowrite32(phy->base + R_DPHYTX_DPHY_CTL0, ctrl);

	return;
}

void mipi_dphy_shutdown(dphy_t *phy, int shutdown)
{
	uint32_t ctrl;

	ioread32(phy->base + R_DPHYTX_DPHY_CTL1, &ctrl);
	MIPI_FIELD_SET(ctrl, MIPI_DPHY_SHUTDOWNZ, (shutdown & 0x1));
	iowrite32(phy->base + R_DPHYTX_DPHY_CTL1, ctrl);
	return;
}

void mipi_dphy_resetz(dphy_t *phy, int resetz)
{
	uint32_t ctrl;

	ioread32(phy->base + R_DPHYTX_DPHY_CTL1, &ctrl);
        MIPI_FIELD_SET(ctrl, MIPI_DPHY_RSTZ, (resetz & 0x1));
	iowrite32(phy->base + R_DPHYTX_DPHY_CTL1, ctrl);
	return;
}

void mipi_dphy_enable_lanes(dphy_t *phy, int lanes)
{
	uint32_t ctrl;

	ioread32(phy->base + R_DPHYTX_DPHY_CTL1, &ctrl);

	ctrl &= MIPI_FIELD_CLR_MASK(MIPI_DPHY_CTL_ENABLE);

        MIPI_FIELD_SET(ctrl, MIPI_DPHY_CTL_ENABLE, (lanes & 0xF));

	iowrite32(phy->base + R_DPHYTX_DPHY_CTL1, ctrl);

	return;
}

void mipi_dphy_EnableClkBIU(dphy_t *phy, int en)
{
	uint32_t ctrl;

	ioread32(phy->base + R_DPHYTX_DPHY_CTL1, &ctrl);
        MIPI_FIELD_SET(ctrl, MIPI_DPHY_ENABLECLK_BIU, (en & 0x1));
	iowrite32(phy->base + R_DPHYTX_DPHY_CTL1, ctrl);
	return;
}

void mipi_dphy_CfgClkFreqRange(dphy_t *phy, int range)
{
	uint32_t ctrl;

	ioread32(phy->base + R_DPHYTX_DPHY_CTL1, &ctrl);
        MIPI_FIELD_SET(ctrl, MIPI_DPHY_ENABLECLK_FREQUENCY_RANGE, (range & 0x3F));
	iowrite32(phy->base + R_DPHYTX_DPHY_CTL1, ctrl);
	return;
}

void mipi_dphy_pll_shadow_control_en(dphy_t *phy, int en)
{
	uint32_t ctrl;

	ioread32(phy->base + R_DPHYTX_DPHY_PLL2, &ctrl);
        MIPI_FIELD_SET(ctrl, MIPI_DPHY_PLL2_PLL_SHADOW_CONTROL, (en & 0x1));
	iowrite32(phy->base + R_DPHYTX_DPHY_PLL2, ctrl);
}

void mipi_dphy_pll_clksel(dphy_t *phy, int clksel)
{
	uint32_t ctrl;

	ioread32(phy->base + R_DPHYTX_DPHY_PLL2, &ctrl);
        MIPI_FIELD_SET(ctrl, MIPI_PLL2_CLKSEL, (clksel & 0x3));
	iowrite32(phy->base + R_DPHYTX_DPHY_PLL2, ctrl);
}

void mipi_dphy_stopstate_wait(dphy_t *phy, int lanes)
{
	uint32_t rb0, cond;
	unsigned int wait = 0xF00;

	lanes = (1<<lanes)-1;
	cond = 0;
        MIPI_FIELD_SET(cond, MIPI_DPHY_RB0_STOPSTATECLK, 1);
        MIPI_FIELD_SET(cond, MIPI_RB0_STOPSTATEDATA, (lanes & 0xF));

	do {
		ioread32(phy->base + R_DPHYTX_DPHY_RB0, &rb0);
		rb0 = (rb0 & 0x3D000);
		if (!wait)
			break;
		wait = wait - 1;
	} while (rb0 != cond);
}