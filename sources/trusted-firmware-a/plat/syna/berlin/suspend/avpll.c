/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <lib/mmio.h>
#include "avpll_regs.h"

#define BFM_HOST_Bus_Write32(offset, val)		\
			mmio_write_32((unsigned long)offset, val)

#define BFM_HOST_Bus_Read32(offset, val)		\
			val = mmio_read_32((unsigned long)offset)

struct avpll_clk_module {
	unsigned int reg_offset;
	int avpll_enable;
};

struct avpll_control{
	unsigned int reg_offset;
	int power_enable;
};

static struct avpll_clk_module AVPLL_module[AVPLL_CLK_MODULE_NUM];
static struct avpll_control AVPLL_control[AVPLL_CONTROL_NUM];

void avpll_init(void)
{
	unsigned int i;

	/*AVPLL power related*/
	AVPLL_control[0].reg_offset = RA_avPll_ctrlPLL;
	AVPLL_control[1].reg_offset = RA_avPll_C1 + RA_avpllCh_ctrl1;
	AVPLL_control[2].reg_offset = RA_avPll_C2 + RA_avpllCh_ctrl1;
	AVPLL_control[3].reg_offset = RA_avPll_C3 + RA_avpllCh_ctrl1;
	AVPLL_control[4].reg_offset = RA_avPll_C4 + RA_avpllCh_ctrl1;
	AVPLL_control[5].reg_offset = RA_avPll_C5 + RA_avpllCh_ctrl1;
	AVPLL_control[6].reg_offset = RA_avPll_C6 + RA_avpllCh_ctrl1;
	AVPLL_control[7].reg_offset = RA_avPll_C7 + RA_avpllCh_ctrl1;
	AVPLL_control[8].reg_offset = RA_avPll_C8 + RA_avpllCh8_ctrl1;

	for (i = 0; i <AVPLL_CONTROL_NUM;i++) {
		AVPLL_control[i].power_enable = 0;
	}

	/*AVPLL module related*/
	AVPLL_module[CPU_FAST_REF_CLK].reg_offset = RA_Gbl_cpufastRefClk;
	AVPLL_module[MEM_FAST_REF_CLK].reg_offset  = RA_Gbl_memfastRefClk;
	AVPLL_module[CFG_CLK].reg_offset  = RA_Gbl_cfgClk;
	AVPLL_module[PERIF_SYS_CLK].reg_offset  = RA_Gbl_perifSysClk;
	#ifdef HB_CLK
	AVPLL_module[HB_CLK].reg_offset  = RA_Gbl_hbClk;
	#endif
	AVPLL_module[ATB_CLK].reg_offset  = RA_Gbl_atbClk;
	AVPLL_module[DECODER_CLK].reg_offset  = RA_Gbl_decoderClk;
	AVPLL_module[DECODER_M3_CLK].reg_offset  = RA_Gbl_decoderM3Clk;
	#ifdef DECODER_PCUBE_CLK
	AVPLL_module[DECODER_PCUBE_CLK].reg_offset  = RA_Gbl_decoderPCubeClk;
	#endif
	AVPLL_module[ENCODER_CLK].reg_offset  = RA_Gbl_encoderClk;
	AVPLL_module[OVP_CORE_CLK].reg_offset  = RA_Gbl_ovpCoreClk;
	#ifdef GFX2D_CORE_CLK
	AVPLL_module[GFX2D_CORE_CLK].reg_offset  = RA_Gbl_gfx2DCoreClk;
	#endif
	AVPLL_module[GFX3D_CORE_CLK].reg_offset  = RA_Gbl_gfx3DCoreClk;
	#ifdef GFX3D_SHADER_CLK
	AVPLL_module[GFX3D_SHADER_CLK].reg_offset  = RA_Gbl_gfx3DShClk;
	#endif
	AVPLL_module[GFX3D_SYS_CLK].reg_offset  = RA_Gbl_gfx3DSysClk;
	#ifdef GFX2D_SYS_CLK
	AVPLL_module[GFX2D_SYS_CLK].reg_offset  = RA_Gbl_gfx2DSysClk;
	#endif
	AVPLL_module[AVIO_SYS_CLK].reg_offset  = RA_Gbl_avioSysClk;
	AVPLL_module[VPP_SYS_CLK].reg_offset  = RA_Gbl_vppSysClk;
	AVPLL_module[EDDC_CLK].reg_offset  = RA_Gbl_eddcClk;
	AVPLL_module[AVIO_BIU_CLK].reg_offset  = RA_Gbl_avioBiuClk;
	AVPLL_module[ZSP_CLK].reg_offset  = RA_Gbl_zspClk;
	AVPLL_module[TSP_CLK].reg_offset  = RA_Gbl_tspClk;
	AVPLL_module[TSP_REF_CLK].reg_offset  = RA_Gbl_tspRefClk;
	AVPLL_module[NDS_CLK].reg_offset  = RA_Gbl_ndsClk;
	AVPLL_module[NOCS_CLK].reg_offset  = RA_Gbl_nocsClk;
	AVPLL_module[APB_CORE_CLK].reg_offset  = RA_Gbl_apbCoreClk;
	AVPLL_module[EMMC_CLK].reg_offset  = RA_Gbl_emmcClk;
	AVPLL_module[SD0_CLK].reg_offset  = RA_Gbl_sd0Clk;
	AVPLL_module[SD1_CLK].reg_offset  = RA_Gbl_sd1Clk;
	AVPLL_module[DLL_MASTER_REF_CLK].reg_offset  = RA_Gbl_dllMstRefClk;
	AVPLL_module[GMAC_SMARTCARD_CLK].reg_offset  = RA_Gbl_gethRgmiiClk;
	#ifdef GMAC_SMARTCARD_SYS_CLK
	AVPLL_module[GMAC_SMARTCARD_SYS_CLK].reg_offset = RA_Gbl_gethRgmiiSysClk;
	#endif
	AVPLL_module[USIM0_CLK].reg_offset  = RA_Gbl_usim0Clk;
	AVPLL_module[PCIE_TEST_CLK].reg_offset  = RA_Gbl_pcieTestClk;
	AVPLL_module[USB2_TEST_CLK].reg_offset  = RA_Gbl_usb2TestClk;
	AVPLL_module[USB3_TEST_CLK].reg_offset  = RA_Gbl_usb3TestClk;
	AVPLL_module[USB3_CORE_CLK].reg_offset  = RA_Gbl_usb3CoreClk;
	AVPLL_module[NFC_ECC_CLK].reg_offset  = RA_Gbl_nfcEccClk;
	AVPLL_module[BCM_CLK].reg_offset  = RA_Gbl_bcmClk;

	for (i = 0; i < AVPLL_CLK_MODULE_NUM; i++) {
		AVPLL_module[i].avpll_enable = 0;
	}

}

void avpll_power_down(void *base)
{
	unsigned int value, i;

	BFM_HOST_Bus_Read32(base + AVIO_MEMMAP_AVIO_GBL_BASE +
			RA_avioGbl_AVPLLA +
			AVPLL_control[0].reg_offset, value);

	if (value & MSK32avPll_ctrlPLL_PU) {
		AVPLL_control[0].power_enable = 1;
		value &= (~MSK32avPll_ctrlPLL_PU);
		BFM_HOST_Bus_Write32(base + AVIO_MEMMAP_AVIO_GBL_BASE +
					RA_avioGbl_AVPLLA +
					AVPLL_control[0].reg_offset, value);
	} else {
		AVPLL_control[0].power_enable = 0;
	}

	for (i = 1; i <= 8; i++) {
		BFM_HOST_Bus_Read32(base + AVIO_MEMMAP_AVIO_GBL_BASE +
				RA_avioGbl_AVPLLA +
				AVPLL_control[i].reg_offset, value);

		if (value & MSK32avpllCh_ctrl_PU) {
			AVPLL_control[i].power_enable = 1;
			value &= (~MSK32avpllCh_ctrl_PU);
			BFM_HOST_Bus_Write32(base + AVIO_MEMMAP_AVIO_GBL_BASE +
					RA_avioGbl_AVPLLA +
					AVPLL_control[i].reg_offset, value);
		} else {
			AVPLL_control[i].power_enable = 0;
		}
	}
}

void avpll_power_up(void *base)
{
	unsigned int value, i;

	for (i = 1; i <= 8; i++) {
		BFM_HOST_Bus_Read32(base +
				AVIO_MEMMAP_AVIO_GBL_BASE +
				RA_avioGbl_AVPLLA +
				AVPLL_control[i].reg_offset, value);

		if (AVPLL_control[i].power_enable) {
			AVPLL_control[i].power_enable = 0;
			value |= MSK32avpllCh_ctrl_PU;
			BFM_HOST_Bus_Write32(base +
					AVIO_MEMMAP_AVIO_GBL_BASE +
					RA_avioGbl_AVPLLA +
					AVPLL_control[i].reg_offset, value);
		}
	}

	BFM_HOST_Bus_Read32(base + AVIO_MEMMAP_AVIO_GBL_BASE +
				RA_avioGbl_AVPLLA +
				AVPLL_control[0].reg_offset, value);

	if (AVPLL_control[0].power_enable) {
		AVPLL_control[0].power_enable = 0;
		value |= MSK32avPll_ctrlPLL_PU;
		BFM_HOST_Bus_Write32(base + AVIO_MEMMAP_AVIO_GBL_BASE +
				RA_avioGbl_AVPLLA +
				AVPLL_control[0].reg_offset, value);
	}

}

void avpll_suspend(void *power_base)
{
	avpll_init();
	avpll_power_down(power_base);
}

void avpll_resume(void *power_base)
{
	avpll_power_up(power_base);
}
