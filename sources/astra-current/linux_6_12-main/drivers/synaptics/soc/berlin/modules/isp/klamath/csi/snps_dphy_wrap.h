/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _SNPS_DPHY_WRAP_H_
#define _SNPS_DPHY_WRAP_H_
void csi2_dphy_setclkfreqrange(struct snps_dphy *dev, int cfgclk);
void csi2_dphy_forcerxmode(struct snps_dphy *dev, int rx);
void csi2_dphy_setphy_cfgclkoff(struct snps_dphy *dev, int off);
void csi2_dphy_sethsfreqrange(struct snps_dphy *dev, int hsfreq);
void csi2_dphy_basedir(struct snps_dphy *dev, int en);
void csi2_dphy_setphy_ppienableclk(struct snps_dphy *dev, int en);
void csi2_dphy_stopstate_wait(struct snps_dphy *dev, unsigned int lanes);
void csi2_dphy_setppictrl(void *base_address, int csi, int active, int multich);
#endif
