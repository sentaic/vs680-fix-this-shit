// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2018-2020 Synaptics Incorporated */

#include <linux/module.h>
#include <sound/soc.h>

#include "berlin_pcm.h"
#include "aio_hal.h"
#include "berlin_util.h"

int berlin_set_pll(void *aio_handle, u32 apll_id, u32 clk_rate)
{
	if (apll_id >= AIO_APLL_NUM) {
		pr_err("apll%d not supported", apll_id);
		return 0;
	}

	if (aio_get_clk_rate(aio_handle, apll_id) != clk_rate) {
		aio_clk_enable(aio_handle, apll_id, false);
		aio_set_clk_rate(aio_handle, apll_id, clk_rate);
		aio_clk_enable(aio_handle, apll_id, true);
		pr_info("set apll%d to %u\n", apll_id, clk_rate);
	} else {
		pr_info("apll%d already set to %u\n", apll_id, clk_rate);
	}

	return 0;
}
EXPORT_SYMBOL(berlin_set_pll);

u32 berlin_get_bclk_div(u32 mclk, u32 bclk)
{
	u32 bclk_div;

	bclk_div = mclk/(bclk);
	bclk_div = ilog2(bclk_div);
	pr_debug("%s bclk_div %d mclk %d bclk %d", __func__, bclk_div, mclk, bclk);
	return bclk_div;
}
EXPORT_SYMBOL(berlin_get_bclk_div);

/*
 * Get the channel resolution (number of valid bits in a half period of FSYNC)
 */
u32 berlin_get_sample_resolution(u32 word_w)
{
	u32 dfm;

	switch (word_w) {
	case 16:
		dfm = AIO_16DFM;
		break;
	case 24:
		dfm = AIO_24DFM;
		break;
	case 32:
	default:
		dfm = AIO_32DFM;
		break;
	}

	return dfm;
}
EXPORT_SYMBOL(berlin_get_sample_resolution);

/*
 * Get the half period of FSYNC (sampling rate) in terms of number of bit-clocks
 */
u32 berlin_get_sample_period_in_bclk(u32 word_s)
{
	u32 cfm;

	switch (word_s) {
	case 16:
		cfm = AIO_16CFM;
		break;
	case 24:
		cfm = AIO_24CFM;
		break;
	case 32:
	default:
		cfm = AIO_32CFM;
		break;
	}

	return cfm;
}
EXPORT_SYMBOL(berlin_get_sample_period_in_bclk);

u32 berlin_get_cfm(u32 word_s)
{
	u32 cfm;

	switch (word_s) {
	case 16:
		cfm = AIO_16CFM;
		break;
	case 24:
		cfm = AIO_24CFM;
		break;
	case 32:
	default:
		cfm = AIO_32CFM;
		break;
	}

	return cfm;
}
EXPORT_SYMBOL(berlin_get_cfm);

