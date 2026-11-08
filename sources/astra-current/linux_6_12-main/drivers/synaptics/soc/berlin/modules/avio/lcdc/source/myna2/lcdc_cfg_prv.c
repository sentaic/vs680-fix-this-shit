// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 */
#include "lcdc.h"
#include "syna_lcdc_dev.h"
#include "lcdc_cfg_prv.h"
#include "avio_io.h"
#include "syna_lcdc_drv.h"

//BGRA output
static unsigned char syna_lcdc_bitmap_table[SYNA_LCDC_PIXFMT_MAX][SYNA_LCDC_PIXORDER_MAX][32] = {
	{//ARGB32 input
		{ //ARGB
			24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23,
			8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4 ,5, 6, 7,
		},
		{ //ABGR
			8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31, 0, 1, 2, 3, 4 ,5, 6, 7,
		},
		{ //RGBA
			16, 17, 18, 19, 20, 21, 22, 23, 8, 9, 10, 11, 12, 13, 14, 15,
			0, 1, 2, 3, 4 ,5, 6, 7, 24, 25, 26, 27, 28, 29, 30, 31,
		},
		{ //BGRA
			0, 1, 2, 3, 4 ,5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		}
	},
	{ //ARGB32_PM
		{ //ARGB
			24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23,
			8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4 ,5, 6, 7,
		},
		{ //ABGR
			8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
			25, 26, 27, 28, 29, 30, 31, 0, 1, 2, 3, 4 ,5, 6, 7,
		},
		{ //RGBA
			16, 17, 18, 19, 20, 21, 22, 23, 8, 9, 10, 11, 12, 13, 14, 15,
			0, 1, 2, 3, 4 ,5, 6, 7, 24, 25, 26, 27, 28, 29, 30, 31,
		},
		{ //BGRA
			0, 1, 2, 3, 4 ,5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		}
	},
	{ //RGB565
		{ //RGB
			11, 12, 13, 14, 15, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4,
			27, 28, 29, 30, 31, 21, 22, 23, 24, 25, 26, 16, 17, 18, 19, 20,
		},
		{ //BGR
			0, 1, 2, 3, 4 ,5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
			17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		},
		{//RGB
			11, 12, 13, 14, 15, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4,
			27, 28, 29, 30, 31, 21, 22, 23, 24, 25, 26, 16, 17, 18, 19, 20,
		},
		{ //BGR
			0, 1, 2, 3, 4 ,5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		}
	},
	{ //RGB888
		{ //RGB
			24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23,
			8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4 ,5, 6, 7,
		},
		{ //BGR
			24, 25, 26, 27, 28, 29, 30, 31, 0, 1, 2, 3, 4, 5, 6, 7,
			8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
		},
		{ //RGB
			24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23,
			8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4 ,5, 6, 7,
		},
		{ //BGR
			24, 25, 26, 27, 28, 29, 30, 31, 0, 1, 2, 3, 4, 5, 6, 7,
			8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
		}
	}
};


void syna_lcdc_cfg_setbitmap(struct syna_lcdc_dev *dev,
				int src_fmt,
				int order)
{
	unsigned char *bitmap_table = NULL;
	T32LCDC_CTRL4 ctrl4;
	T32BITMAP32_SEL sel;
	T32BITMAP32_SEL5 sel5;
	int index = 0;

	ctrl4.u32 = syna_lcdc_read(dev, RA_LCDC_CTRL4);
	ctrl4.uCTRL4_bitmap32_en = 1;
	syna_lcdc_write(dev, RA_LCDC_CTRL4, ctrl4.u32);

	bitmap_table = &syna_lcdc_bitmap_table[src_fmt][order][0];
	do {
		sel.uSEL_BIT_POS0 = bitmap_table[index + 0];
		sel.uSEL_BIT_POS1 = bitmap_table[index + 1];
		sel.uSEL_BIT_POS2 = bitmap_table[index + 2];
		sel.uSEL_BIT_POS3 = bitmap_table[index + 3];
		sel.uSEL_BIT_POS4 = bitmap_table[index + 4];
		sel.uSEL_BIT_POS5 = bitmap_table[index + 5];
		syna_lcdc_write(dev, RA_LCDC_bitmap32_ctrl + 4 * (index / 6), sel.u32);
		index += 6;
	} while(index < 30);

	sel5.uSEL_BIT_POS30 = bitmap_table[index + 0];
	sel5.uSEL_BIT_POS31 = bitmap_table[index + 1];
	syna_lcdc_write(dev, RA_LCDC_bitmap32_ctrl + 4 * (index / 6), sel5.u32);
}

__attribute__((weak)) void syna_lcdc_cfg_dlr_fifoflush(struct syna_lcdc_dev *dev)
{

}

void syna_lcdc_cfg_dlr_init(struct syna_lcdc_dev *dev, int num)
{
	dev->prev_frame = NULL;
	dev->curr_frame = NULL;

	/**
	  * Refer AVIO_BCM.pdf intrNo details
	  */
	dev->lcdcID = num;
	if (num == SYNA_LCDC_1) {
		dev->dmaRID = avioDhubChMap_vpp128b_LCDC1_R;
		dev->intrID = avioDhubSemMap_vpp128b_vpp_inr0;
		dev->intrNo = LCDC1_FRAME_END_INTERRUPT; //LCDC1 Frame end interrupt
	} else {
		dev->dmaRID = avioDhubChMap_vpp128b_LCDC2_R;
		dev->intrID = avioDhubSemMap_vpp128b_vpp_inr1;
		dev->intrNo = LCDC2_FRAME_END_INTERRUPT; //LCDC2 Frame end interrupt
	}
}

void syna_lcdc_cfg_wrap_interrupt_enable(void)
{
	unsigned int addr;
	T32avioGbl_INTR_CTRL lcdc_intr;

	addr = SYNA_MEMMAP_AVIO_VPP_GBL_INTR_CTRL;
	GA_REG_WORD32_READ(addr, &lcdc_intr.u32);
	SYNA_LCDC1_INTR_EN(lcdc_intr);
	SYNA_LCDC2_INTR_EN(lcdc_intr);
	SYNA_MIPI_INTR_EN(lcdc_intr);
	GA_REG_WORD32_WRITE(addr, lcdc_intr.u32);
}
