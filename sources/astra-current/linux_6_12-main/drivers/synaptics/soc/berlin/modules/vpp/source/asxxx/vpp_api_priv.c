// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 */

#include "vpp_api.h"
#include "hal_vpp_wrap.h"
#include "avio_fl_info.h"

#define PIXEL_CLOCK_MULTIPLIER	4
#define PIXEL_CLOCK_RATE(PIX_CLK)	(PIX_CLK * PIXEL_CLOCK_MULTIPLIER * 1000)

static int VPP_Configure_Lcdc(SYNA_LCDC_NUM lcdcid, SYNA_LCDC_CONFIG *lcdcparams)
{
	if (!lcdcparams)
		return 0;

	/* Set the Pixel Clock Before the TG configuration */
	if (lcdcid == SYNA_LCDC_1)
		VPP_Clock_Set_Rate(PIXEL_CLOCK_RATE(lcdcparams->pixclock));
	else
		VPP_Clock_Set_Rate_Ext(PIXEL_CLOCK_RATE(lcdcparams->pixclock));

	return wrap_MV_VPP_LoadConfigTable(VOUT_TFT, lcdcid, lcdcparams);
}

int wrap_VPP_Init_Recovery(VPP_MEM_LIST *shm_list,
			int is_ampless_boot, vpp_config_params vpp_config_param)
{
	int res = -1;
	VPP_MIPI_CONFIG_PARAMS *pResCfg = vpp_config_param.mipi_resinfo_params;
	avio_fastlogo_info display_info = avio_get_fastlogo_status();

	if (display_info.u.status)
		return 0;

	/* Always assume ampless boot and NTZ version flow for ASxxx family.
	 * Configure LCDC-0 and enable interrupt
	 */
	if ((res = VPP_Configure_Lcdc(SYNA_LCDC_1, vpp_config_param.lcdc_config_params))) {
		pr_err("Failed Load For LCDC0\n");
		goto exit;
	}

	/* Configure LCDC-1 and enable interrupt */
	if (pResCfg && (res = VPP_Configure_Lcdc(SYNA_LCDC_2, vpp_config_param.mipi_lcdc_config_params)))
		pr_err("Failed Load CFG For LCDC1\n");

exit:
	return res;
}
