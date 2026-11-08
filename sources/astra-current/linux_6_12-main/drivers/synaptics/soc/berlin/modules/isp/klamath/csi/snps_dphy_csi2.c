/**
 * @file snps_dphy_csi2.c
 * @brief Synopsys D-PHY CSI-2 implementation
 *
 * Copyright (C) 2015 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */

#include <linux/delay.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include "snps_dphy_csi2.h"
#include "snps_dphy_gen3.h"

/** @short DPHY GEN 3 configuration */
struct range_dphy range_gen3[] = {
	{  80, 0x00, 0x1CC}, {  90, 0x10, 0x1CC}, { 100, 0x20, 0x1CC},
	{ 110, 0x30, 0x1CC}, { 120, 0x01, 0x1CC}, { 130, 0x11, 0x1CC},
	{ 140, 0x21, 0x1CC}, { 150, 0x31, 0x1CC}, { 160, 0x02, 0x1CC},
	{ 170, 0x12, 0x1CC}, { 180, 0x22, 0x1CC}, { 190, 0x32, 0x1CC},
	{ 205, 0x03, 0x1CC}, { 220, 0x13, 0x1CC}, { 235, 0x23, 0x1CC},
	{ 250, 0x33, 0x1CC}, { 275, 0x04, 0x1CC}, { 300, 0x14, 0x1CC},
	{ 325, 0x25, 0x1CC}, { 350, 0x35, 0x1CC}, { 400, 0x05, 0x1CC},
	{ 450, 0x16, 0x1CC}, { 500, 0x26, 0x1CC}, { 550, 0x37, 0x1CC},
	{ 600, 0x07, 0x1CC}, { 650, 0x18, 0x1CC}, { 700, 0x28, 0x1CC},
	{ 750, 0x39, 0x1CC}, { 800, 0x09, 0x1CC}, { 850, 0x19, 0x1CC},
	{ 900, 0x29, 0x1CC}, { 950, 0x3A, 0x1CC}, {1000, 0x0A, 0x1CC},
	{1050, 0x1A, 0x1CC}, {1100, 0x2A, 0x1CC}, {1150, 0x3B, 0x1CC},
	{1200, 0x0B, 0x1CC}, {1250, 0x1B, 0x1CC}, {1300, 0x2B, 0x1CC},
	{1350, 0x3C, 0x1CC}, {1400, 0x0C, 0x1CC}, {1450, 0x1C, 0x1CC},
	{1500, 0x2C, 0x1CC}, {1550, 0x3D, 0x11D}, {1600, 0x0D, 0x127},
	{1650, 0x1D, 0x130}, {1700, 0x2E, 0x139}, {1750, 0x3E, 0x142},
	{1800, 0x0E, 0x14B}, {1850, 0x1E, 0x155}, {1900, 0x2F, 0x15E},
	{1950, 0x3F, 0x167}, {2000, 0x0F, 0x170}, {2050, 0x40, 0x179},
	{2100, 0x41, 0x183}, {2150, 0x42, 0x18C}, {2200, 0x43, 0x195},
	{2250, 0x44, 0x19E}, {2300, 0x45, 0x1A7}, {2350, 0x46, 0x1B0},
	{2400, 0x47, 0x1BA}, {2450, 0x48, 0x1C3}, {2500, 0x49, 0x1CC}
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

u32 phy_read_part(struct snps_dphy *dev, unsigned int address,
		unsigned char shift, unsigned char width)
{
	return (phy_read(dev, address) >> shift) & ((1 << width) - 1);
}

static void snps_dphy_reset(struct snps_dphy *dev)
{
	phy_write(dev, R_CSI2_DPHY_RSTZ, 0);
	phy_write(dev, R_CSI2_DPHY_RSTZ, 1);
}

int snps_dphy_power_on(struct snps_dphy *dev)
{
	return __set_phy_state(dev, 1);
}

int snps_dphy_init(struct snps_dphy *dev)
{
	snps_dphy_reset(dev);

	phy_write_part(dev, R_CSI2_DPHY_SHUTDOWNZ, 0, 0, 1);

	return 0;
}
