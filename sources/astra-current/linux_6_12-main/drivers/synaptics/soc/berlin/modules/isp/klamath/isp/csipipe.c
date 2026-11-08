// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
/// @file csipipe.c

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <media/v4l2-device.h>

#include "isp_ctrl.h"
#include "camera_isp_driver.h"
#include "csiPipe.h"
#include "csipipe_pvt.h"
#include "csipipe_api.h"
#include "api_csi_dhub.h"
#include "hal_dhub.h"
#include "demosaic.h"
#include "fvf.h"
#include "wb.h"
#include "csc_common.h"
#include "global.h"
#include "intr_handler.h"
#include "vipGbl.h"
#include "avio.h"
#include "avioDhub.h"
#include "cam_bcmbuf.h"
#ifdef HANDLE_CSIHOST_IRQ
#include <csihost.h>
#endif

/* Path specific Defines */

#define BYTE_LEN					8
#define INVD_INPUT					100
#define DUMMY_REG_BCM				0x32C /* Test point reg - 0xF745832C */
#define PIPE1_QUEUE					BCM_SCHED_Q0
#define PIPE2_QUEUE					BCM_SCHED_Q1
#define CSI_ALIGN(a, b)				(((a + b - 1) / b) * b)


#define IS_IMGRESTODH_ACTIVE(a)		(a >= 1)
#define IS_FMT_RAW16(i)				  (i == CAM_PIXFMT_RAW16)
#define IS_VALID_INPUT(a)			(a != INVD_INPUT)

#define ADD_TO_MODULE_LIST(a, b)	(a |= (1<<b))
#define IS_MODULE_ENABLED(a, b)		(a & (1<<b))
#define IS_HW_CAPTURE_MODE(mode)	((mode == SINGLE_CAPTURE_HW) ||\
									(mode == NTH_CAPTURE_HW))
#define COMP_UV			0
#define COMP_Y			1
#define BCM_BUFFER_SIZE 400
#define FINAL_CFGQ_SIZE 64
#define PIPE_CFGQ_SIZE	200
#define Y_FRAME			0
#define UV_FRAME		1

#define FMT_BAYER16   1
#define FMT_GRAYSCALE 0
#define IMGRES_OPRN_BINNING 1
#define IMGRES_OPRN_MAX 2

#define START_2DDMA(dhubID, dmaID, start_addr, stride, width, height, cfgQ) \
	dhub2d_channel_cfg((HDL_dhub2d *)dhubID, dmaID, start_addr, stride, \
		width, height, 1, 0, 0, 0, 1, 1, cfgQ)
#ifdef BCM_ENABLE
#define START_2NDDMA(dhubID, dmaID, start_addr, burst, step1, size1, step2, size2, cfgQ) \
	isp_dhub2nd_channel_cfg((HDL_dhub2d *)dhubID, dmaID, start_addr, burst, \
		step1, size1, step2, size2, 0, 0, 1, 1, cfgQ)
#else
#define START_2NDDMA(dhubID, dmaID, start_addr, burst, step1, size1, step2, size2, cfgQ) \
	isp_dhub2nd_channel_cfg((HDL_dhub2d *)dhubID, dmaID, start_addr, burst, \
		step1, size1, step2, size2, 0, 0, 1, 1, NULL)
#endif

#define CLEAR_2DDMA(dhubID, dmaID, bcmbuf) \
	do { \
		dhub2d_channel_clear_seq(dhubID, dmaID); \
		dhub2d_channel_start_seq(dhubID, dmaID); \
		dhub2d_channel_clear_seq_bcm(dhubID, dmaID, bcmbuf); \
	} while (0)
#define CLEAR_2NDDMA(hdl, dmaID) \
	do { \
		dhub2nd_channel_clear_seq(hdl, dmaID); \
	} while (0)

typedef struct MODULE_STATUS_s {
	uint8_t imgres_en;
	uint8_t imgres_byp;
	uint8_t seq_en;
	uint8_t crop_en;
	uint8_t fvf_en;
	uint8_t wb_en;
	uint8_t dmsc_en;
	uint8_t csc_en;
	uint8_t dns1_en;
	uint8_t dns2_en;
	uint8_t wr_c_en;
} MODULE_STATUS_t;

extern HDL_dhub2d CSI_dhubHandle;
static char intr_name[MAX_INTR][12] = {"ch0", "ch1", "ch2", "ch3",
				"ch4", "ch5", "ch6", "ch7",
				"ch8", "ch9", "ch10", "ch11",
				"ch12", "ch13", "ch14", "ch15",
				"CSI2HST", "H2DH0eof", "IPI0TGeof", "H2DH1eof",
				"IPI1TGeof", "VID_MUTE", "IPI0eof", "IPI1eof",
				"IPI0BUF1OF", "IPI0BUF0OF", "IPI1BUF1OF", "IPI1BUF0OF",
				"FVF_ERR", "IIF_EOF", "RSVD", "BCM_INVD"};

/* CSI IPI always outputs Raw 16, If ISP's final output is Raw 8,
 * ImageRes module is convert it back to Raw8
 */
static uint32_t CSI_PIPE_GetIPIFormat(uint32_t fmt)
{
	uint32_t op_fmt = CAM_PIXFMT_RAW16;

	switch (fmt) {
	case MEDIA_BUS_FMT_YUV8_1X24:
		op_fmt = CAM_PIXFMT_YUV444;
		break;
	case MEDIA_BUS_FMT_SBGGR8_1X8:
	case MEDIA_BUS_FMT_SGRBG8_1X8:
	case MEDIA_BUS_FMT_SRGGB8_1X8:
	case MEDIA_BUS_FMT_SGRBG10_1X10:
	case MEDIA_BUS_FMT_SRGGB10_1X10:
	case MEDIA_BUS_FMT_SBGGR10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
		op_fmt = CAM_PIXFMT_RAW16;
		break;
	case MEDIA_BUS_FMT_RGB565_2X8_LE:
	case MEDIA_BUS_FMT_RGB565_2X8_BE:
		op_fmt = CAM_PIXFMT_RGB565;
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
		op_fmt = CAM_PIXFMT_RGB888;
		break;
	case MEDIA_BUS_FMT_UYVY8_1X16:
		op_fmt = CAM_PIXFMT_YUV422P;
		break;
	case MEDIA_BUS_FMT_YUYV8_1_5X8:
		op_fmt = CAM_PIXFMT_YUV420SP;
		break;
	default:
		pr_warn("Unsupported format[0x%x], considering RAW16 out\n", fmt);
		break;
	}
	return op_fmt;
}

static uint32_t CSI_PIPE_GetCamFormat(uint32_t fmt)
{
	uint32_t op_fmt = CAM_PIXFMT_RAW16;

	switch (fmt) {
	case MEDIA_BUS_FMT_YUV8_1X24:
		op_fmt = CAM_PIXFMT_YUV444;
		break;
	case MEDIA_BUS_FMT_SBGGR8_1X8:
	case MEDIA_BUS_FMT_SGRBG8_1X8:
	case MEDIA_BUS_FMT_SRGGB8_1X8:
		op_fmt = CAM_PIXFMT_RAW8;
		break;
	case MEDIA_BUS_FMT_SGRBG10_1X10:
	case MEDIA_BUS_FMT_SRGGB10_1X10:
	case MEDIA_BUS_FMT_SBGGR10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
		op_fmt = CAM_PIXFMT_RAW16;
		break;
	case MEDIA_BUS_FMT_RGB565_2X8_LE:
	case MEDIA_BUS_FMT_RGB565_2X8_BE:
		op_fmt = CAM_PIXFMT_RGB565;
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
		op_fmt = CAM_PIXFMT_RGB888;
		break;
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_YUYV8_1X16:
		op_fmt = CAM_PIXFMT_YUV422P;
		break;
	case MEDIA_BUS_FMT_YUYV8_1_5X8:
		op_fmt = CAM_PIXFMT_YUV420SP;
		break;
	case MEDIA_BUS_FMT_YUYV8_2X8:
		op_fmt = CAM_PIXFMT_YUV422SP;
		break;
	default:
		pr_warn("Unsupported format[0x%x], considering RAW16 out\n", fmt);
		break;
	}
	return op_fmt;
}

static int CSI_PIPE_CheckCompatibility(uint32_t ipi_fmt, uint32_t op_fmt)
{
	int res = 0;

	switch (ipi_fmt) {
	case CAM_PIXFMT_RAW8:
	case CAM_PIXFMT_RAW16:
		// All output formats are supported
		break;
	case CAM_PIXFMT_RGB888:
	case CAM_PIXFMT_RGB565:
		if (op_fmt < CAM_PIXFMT_RGB888) { // To send out RGB soft bypass CSC
			pr_warn("Only Output formats till RGB888 supported\n");
			res = -1;
		}
		break;
	case CAM_PIXFMT_YUV444:
		if (op_fmt < CAM_PIXFMT_YUV444) {
			pr_warn("Only Output formats till YUV444 supported\n");
			res = -1;
		}
		break;
	case CAM_PIXFMT_YUV422SP:
		if (op_fmt < CAM_PIXFMT_YUV422SP) {
			pr_warn("Only Output formats YUV422/YUV420 supported\n");
			res = -1;
		}
		break;
	case CAM_PIXFMT_YUV422P:
		if (op_fmt != CAM_PIXFMT_YUV422P && op_fmt != CAM_PIXFMT_YUV422SP &&
		    op_fmt != CAM_PIXFMT_YUV420SP) {
			pr_warn("Only Output formats YUV422P/YUV422SP/YUV420 supported\n");
			res = -1;
		}
		break;
	case CAM_PIXFMT_YUV420SP:
		if (op_fmt < CAM_PIXFMT_YUV420SP) {
			pr_warn("Only Output format YUV420 supported\n");
			res = -1;
		}
		break;
	default:
		pr_warn("Unsupported format\n");
		res = -1;
		break;
	}
	return res;
}

static uint8_t CSI_PIPE_GetNextIndex(uint8_t index, uint8_t max)
{
	index++;
	if (index >= max)
		return 0; // circular index, reset to 0

	return index;
}

static void CSI_PIPE_AlignBCM(struct camera_isp_dev *dev, struct BCMBUF *pbcmbuf)
{
	uint32_t size;
	UINT64 *start = pbcmbuf->head;

	size = (UINT64)pbcmbuf->writer - (UINT64)*start;

	// entry number should be even, one entry size=8(reg+val)
	// If entry number not even, then write a dummy register
	if (size % (2*8)) {
		// During debug last debug point is not available
		CAM_HAL_WriteReg(dev, pbcmbuf, DUMMY_REG_BCM, 0);
	}
}

static void CSI_PIPE_PrintPipeline(uint32_t module_list)
{
	pr_debug("================================== Pipe ==================================\n");
	/* Set source */
	if (IS_MODULE_ENABLED(module_list, MODULE_IIF))
		pr_cont(" IIF-->");
	else if (IS_MODULE_ENABLED(module_list, MODULE_IPI0))
		pr_cont(" IPI0-->");
	else if (IS_MODULE_ENABLED(module_list, MODULE_IPI1))
		pr_cont(" IPI1-->");

	/* Set Pipeline */
	if (IS_MODULE_ENABLED(module_list, MODULE_IMGRES))
		pr_cont("IMGRES-->");
	if (IS_MODULE_ENABLED(module_list, MODULE_FVF))
		pr_cont("FVF-->");
	if (IS_MODULE_ENABLED(module_list, MODULE_WB))
		pr_cont("WB-->");
	if (IS_MODULE_ENABLED(module_list, MODULE_DEMOSAIC))
		pr_cont("DMSC-->");
	if (IS_MODULE_ENABLED(module_list, MODULE_CSC))
		pr_cont("CSC-->");
	if (IS_MODULE_ENABLED(module_list, MODULE_DNS444_422))
		pr_cont("DNS444_422-->");
	if (IS_MODULE_ENABLED(module_list, MODULE_DNS422_420))
		pr_cont("DNS422_420-->");
	if (IS_MODULE_ENABLED(module_list, MODULE_DHUB))
		pr_cont("DHUB\n");
	pr_debug("==========================================================================\n");
}

static void CSI_PIPE_ClkControl(CSI_PL_CTX_t *ctx, MODULE_STATUS_t *mod)
{
	uint32_t clk_en;

	clk_en = (mod->imgres_en << LSb32HOST2DHUB_CLKEN_CTRL_imageRes_clken) |
			 (mod->imgres_byp << LSb32HOST2DHUB_CLKEN_CTRL_imageRes_bypass_clken) |
			 (mod->crop_en << LSb32HOST2DHUB_CLKEN_CTRL_image_crop_clken) |
			 (mod->wb_en << LSb32HOST2DHUB_CLKEN_CTRL_wb_clken) |
			 (mod->dmsc_en << LSb32HOST2DHUB_CLKEN_CTRL_demosaic_clken) |
			 (mod->csc_en << LSb32HOST2DHUB_CLKEN_CTRL_csc_clken) |
			 (mod->dns1_en << LSb32HOST2DHUB_CLKEN_CTRL_dns444to422_clken) |
			 (mod->dns2_en << LSb32HOST2DHUB_CLKEN_CTRL_dns444to420_clken) |
			 (mod->wr_c_en << LSb32HOST2DHUB_CLKEN_CTRL_WrClient_C_clken) |
			 (mod->fvf_en << LSb32HOST2DHUB_CLKEN_CTRL_FVF_clken);
#ifdef CSC_OF_DEBUG_EN
	clk_en |= (1 << LSb32HOST2DHUB_CLKEN_CTRL_fifo_read_on_TG_ACTIVE_en);
#endif
	CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_CLKEN_CTRL, clk_en);
}

/**
 * Config write client
 * @param[in] ctx - pipeline context
 * @param[in] id - Wrient Client id 0=C, 1=Y
 * @param[out] void
 */
static void CSI_PIPE_write_client_clear(CSI_PL_CTX_t *ctx, uint8_t id, uint8_t immdt)
{
	uint32_t val;
	uint32_t base_addr = ctx->pipe_base_addr;
	struct BCMBUF *p_bcmbuf;

	if (immdt || !ctx->p_curr_bcmq)
		p_bcmbuf = NULL;
	else
		p_bcmbuf = &ctx->p_curr_bcmq->bcmBuf;

	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_CTRL2);
	if (id == COMP_Y) {
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL2_WrClient_clear_Y,
			bHOST2DHUB_CTRL2_WrClient_clear_Y);
	} else {
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL2_WrClient_clear_C,
			bHOST2DHUB_CTRL2_WrClient_clear_C);
	}
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL2, val);
	if (id == COMP_Y) {
		SET_BIT(val, 0, LSb32HOST2DHUB_CTRL2_WrClient_clear_Y,
			bHOST2DHUB_CTRL2_WrClient_clear_Y);
	} else {
		SET_BIT(val, 0, LSb32HOST2DHUB_CTRL2_WrClient_clear_C,
			bHOST2DHUB_CTRL2_WrClient_clear_C);
	}
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL2, val);
}

/**
 * Config write client
 * @param[in] ctx - pipeline context
 * @param[in] id - Wrient Client id 0=C, 1=Y
 * @param[out] void
 */
static void CSI_PIPE_write_client_config(CSI_PL_CTX_t *ctx, uint8_t id, uint8_t sp_en)
{
	struct BCMBUF *p_bcmbuf = NULL;
	uint32_t val = 0;
	uint32_t frame_size_px;
	uint32_t line_length_bits;
	uint32_t stride_len __maybe_unused;
	uint32_t width;
	uint32_t height;
	uint32_t scale __maybe_unused = ctx->crop.scale;
	uint32_t base_addr = id ? ctx->pipe_base_addr + RA_HOST2DHUB_WrClient_Y :
		ctx->pipe_base_addr + RA_HOST2DHUB_WrClient_C;

	if (ctx->p_curr_bcmq != NULL)
		p_bcmbuf = &ctx->p_curr_bcmq->bcmBuf;

	width = ctx->yuv420_dir_op ? ctx->op_wt/2 : ctx->op_wt;
	height = ctx->op_ht;
	if (sp_en)
		height = height / 2;

	if (IS_FMT_RAW16(ctx->op_fmt) && (ctx->src != SRC_INTF_IIF))
		frame_size_px = width * height / 3;
	else
		frame_size_px = width * height;
	line_length_bits = width * ctx->op_bpp;
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_WriteClient_pix,
						frame_size_px & MSK32WriteClient_pix_tot);
	/// If line-size is not in multiple of 64bits, configure below registers
	/// so that client can start every line at 64bits aligned address in DRAM
	val = 0;
	if (line_length_bits % 128 != 0) {
		SET_BIT(val, 1, LSb32WriteClient_NonStdRes_enable, bWriteClient_NonStdRes_enable);
		SET_BIT(val, width, LSb32WriteClient_NonStdRes_pixlineTot,
			bWriteClient_NonStdRes_pixlineTot);
	}
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_WriteClient_NonStdRes, val);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_WriteClient_pack, ctx->pack_sel);
}

/**
 * Config write client
 * @param[in] ctx - pipeline context
 * @param[in] id - Wrient Client id 0=C, 1=Y
 * @param[out] void
 */
static void CSI_PIPE_write_client_setinput(CSI_PL_CTX_t *ctx, uint8_t id)
{
	uint32_t val;
	uint32_t base_addr = ctx->pipe_base_addr;
	struct BCMBUF *p_bcmbuf = NULL;
	//void (*get_fmt)

	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_CTRL2);
	if (id == COMP_Y) {
		SET_BIT(val, ctx->y_wr_ip, LSb32HOST2DHUB_CTRL2_wrClient_Y_input_ctrl,
			bHOST2DHUB_CTRL2_wrClient_Y_input_ctrl);
	} else {
		SET_BIT(val, ctx->c_wr_ip, LSb32HOST2DHUB_CTRL2_wrClient_C_input_ctrl,
			bHOST2DHUB_CTRL2_wrClient_C_input_ctrl);
	}
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL2, val);
}

/**
 * Start write client
 * @param[in] ctx - pipeline context
 * @param[in] id - Wrient Client id 0=C, 1=Y
 * @param[out] void
 */
static void CSI_PIPE_write_client_start(CSI_PL_CTX_t *ctx, uint8_t id)
{
	uint32_t val;
	uint32_t base_addr = ctx->pipe_base_addr;
	/* Use immediate writes when BCM not enabled or queue not ready */
	struct BCMBUF *p_bcmbuf =
		(ctx->bcm_enable && ctx->p_curr_bcmq) ? &ctx->p_curr_bcmq->bcmBuf : NULL;

	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_CTRL2);
	if (id == COMP_Y) {
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL2_WrClient_start_Y,
			bHOST2DHUB_CTRL2_WrClient_start_Y);
	} else {
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL2_WrClient_start_C,
			bHOST2DHUB_CTRL2_WrClient_start_C);
	}
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL2, val);
	if (id == COMP_Y) {
		SET_BIT(val, 0, LSb32HOST2DHUB_CTRL2_WrClient_start_Y,
			bHOST2DHUB_CTRL2_WrClient_start_Y);
	} else {
		SET_BIT(val, 0, LSb32HOST2DHUB_CTRL2_WrClient_start_C,
			bHOST2DHUB_CTRL2_WrClient_start_C);
	}
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL2, val);

}

/**
 * Reset Timing Generator
 * @param[in] ctx - pipeline context
 * @param[out] void
 */
static void CSI_PIPE_tg_reset(CSI_PL_CTX_t *ctx)
{
	struct BCMBUF *p_bcmbuf =
		(ctx->bcm_enable && ctx->p_curr_bcmq) ? &ctx->p_curr_bcmq->bcmBuf : NULL;
	uint32_t val;

	// TG reset
	val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr + RA_HOST2DHUB_TG_CTRL);
	SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_tg_clear, bHOST2DHUB_TG_CTRL_tg_clear);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, ctx->pipe_base_addr + RA_HOST2DHUB_TG_CTRL, val);
	SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_tg_clear, bHOST2DHUB_TG_CTRL_tg_clear);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, ctx->pipe_base_addr + RA_HOST2DHUB_TG_CTRL, val);
}

/**
 * Disable Timing Generator
 * @param[in] ctx - pipeline context
 * @param[out] void
 */
static void __maybe_unused CSI_PIPE_tg_disable(CSI_PL_CTX_t *ctx)
{
	int base_addr;

	base_addr = ctx->pipe_base_addr + RA_HOST2DHUB_CSCDNS + RA_CSCDNS_TG;
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_TG_SIZE, 0);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_TG_HS, 0);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_TG_HB, 0);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_TG_VS0, 0);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_TG_VB0, 0);
}

/**
 * Configure Timing Generator
 * @param[in] ctx - pipeline context
 * @param[out] void
 */
static void CSI_PIPE_tg_config(CSI_PL_CTX_t *ctx)
{
	uint32_t val = 0;
	struct BCMBUF *p_bcmbuf = NULL;
	uint32_t base_addr;
	unsigned char ipi_index __maybe_unused = 0;
	uint32_t vs_fe, vs_be, vtotal, vres, hs_fe, hs_be, htotal, hres, hbp __maybe_unused;
	uint32_t vb_fe, vb_be, hb_fe, hb_be;

	hres = ctx->yuv420_dir_op ? ctx->op_wt/2 : ctx->op_wt;
	base_addr = ctx->pipe_base_addr + RA_HOST2DHUB_CSCDNS + RA_CSCDNS_TG;
	hb_fe = hres + DUMMY_TG_HB_FE_OFF;
	hb_be = DUMMY_TG_HB_BE;
	hs_fe = DUMMY_TG_HS_FE;
	hs_be = hres + DUMMY_TG_HS_BE_OFF;

	vres = ctx->op_ht;
	vb_fe = vres + DUMMY_TG_VB_FE_OFF;
	vb_be = DUMMY_TG_VB_BE;
	vs_fe = DUMMY_TG_VS_FE;
	vs_be = vres + DUMMY_TG_VS_BE_OFF;
	htotal = hres + DUMMY_TG_SIZE_H_BLANK;
	vtotal = vres + DUMMY_TG_SIZE_V_BLANK;

	// TRACE(LOG_LEVEL_ERROR,"TG:(%d %d %d %d %d) x (%d %d %d %d %d)\r\n",
	// hb_fe, hs_fe, hs_be, hb_be, htotal, vb_fe, vs_fe, vs_be, vb_be, vtotal);
	//Total number of pixels in a line including blank pixels
	SET_BIT(val, htotal, LSb32TG_SIZE_X, bTG_SIZE_X);
	SET_BIT(val, vtotal, LSb32TG_SIZE_Y, bTG_SIZE_Y);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_TG_SIZE, val);
	//Total number of lines in a frame including blank lines
	val = 0;
	SET_BIT(val, hs_fe, LSb32TG_HS_FE, bTG_HS_FE);
	SET_BIT(val, hs_be, LSb32TG_HS_BE, bTG_HS_BE);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_TG_HS, val);

	val = 0;
	SET_BIT(val, hb_fe, LSb32TG_HB_FE, bTG_HB_FE);
	SET_BIT(val, hb_be, LSb32TG_HB_BE, bTG_HB_BE);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_TG_HB, val);

	val = 0;
	SET_BIT(val, vs_fe, LSb32TG_VS0_FE, bTG_VS0_FE);
	SET_BIT(val, vs_be, LSb32TG_VS0_BE, bTG_VS0_BE);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_TG_VS0, val);
	val = 0;
	SET_BIT(val, vb_fe, LSb32TG_VB0_FE, bTG_VB0_FE);
	SET_BIT(val, vb_be, LSb32TG_VB0_BE, bTG_VB0_BE);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_TG_VB0, val);
}

/**
 * Start timing generator
 * @param[in] ctx - pipeline context
 * @param[out] void
 */
static void CSI_PIPE_tg_start(CSI_PL_CTX_t *ctx)
{
	uint32_t val;
	/* Use immediate writes if BCM queue is not available */
	struct BCMBUF *p_bcmbuf =
		(ctx->bcm_enable && ctx->p_curr_bcmq) ? &ctx->p_curr_bcmq->bcmBuf : NULL;
	uint32_t base_addr = ctx->pipe_base_addr;

	// Host to Dhub
	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_TG_CTRL);
	SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_tg_start, bHOST2DHUB_TG_CTRL_tg_start);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_TG_CTRL, val);
	SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_tg_start, bHOST2DHUB_TG_CTRL_tg_start);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_TG_CTRL, val);
}

static inline void CAM_Callback(CSI_PL_CTX_t *ctx)
{
	struct camera_vb2_buffer *curr_buf;
	unsigned long flags;

	spin_lock_irqsave(&ctx->buf.lock, flags);
	curr_buf = ctx->buf.curr;

	if (curr_buf) {
		if (curr_buf->vb.vb2_buf.state == VB2_BUF_STATE_ACTIVE) {
			curr_buf->vb.sequence = curr_buf->sequence;
			curr_buf->vb.vb2_buf.timestamp = ktime_get_boottime_ns();
			curr_buf->vb.field = V4L2_FIELD_NONE;
			vb2_buffer_done(&curr_buf->vb.vb2_buf, VB2_BUF_STATE_DONE);
		}
	} else {
		pr_err("frame drop detected !!\n");
	}

	spin_unlock_irqrestore(&ctx->buf.lock, flags);
}

//#define STATIC_FRAME_ADDRESS	  1
static void CSI_PIPE_GetFrame(CSI_PL_CTX_t *ctx, uint32_t addr[])
{
	unsigned long flags;

	spin_lock_irqsave(&ctx->buf.lock, flags);
	if (!list_empty(&ctx->buf.queue)) {
		ctx->buf.curr = ctx->buf.next;
		ctx->buf.next = list_first_entry(&ctx->buf.queue, struct camera_vb2_buffer, list);
		pr_debug("Remove addr: 0x%x ctx: %d\n", ctx->buf.next->planes[0].dma_addr, ctx->id);
		list_del(&ctx->buf.next->list);
	} else {
		pr_debug("%s queue empty\n", __func__);
	}
	spin_unlock_irqrestore(&ctx->buf.lock, flags);

	addr[Y_FRAME] = ctx->buf.next->planes[Y_FRAME].dma_addr;
	addr[UV_FRAME] = ctx->buf.next->planes[UV_FRAME].dma_addr;
}

static void CSI_PIPE_DhubStart(CSI_PL_CTX_t *ctx)
{
	uint32_t width_byte;
	uint32_t stride_len;
	uint32_t height;
	uint32_t dma_id;
	uint32_t pipe = ctx->id;
	uint32_t *cfgQ_len = 0;
	uint64_t *cfgQ = NULL;
	uint32_t addr[2];
	uint32_t uv_offset = 0;
	uint32_t stride_align = 16;
	uint32_t bytesperpixel = ctx->op_bpp / BYTE_LEN;

	if (stride_align % bytesperpixel != 0)
		stride_align = stride_align * bytesperpixel;

	width_byte =  ((ctx->op_wt * ctx->op_bpp)) / BYTE_LEN;
	stride_len = CSI_ALIGN(width_byte, stride_align);

	if (IS_VALID_INPUT(ctx->y_wr_ip)) {
		dma_id = (pipe == 0) ? avioDhubChMap_vip128b_IPI0Y_W : avioDhubChMap_vip128b_IPI1Y_W;
		CSI_PIPE_GetFrame(ctx, addr);
		if (ctx->bcm_enable && (ctx->p_curr_bcmq != NULL)) {
			cfgQ = ctx->p_curr_bcmq->dhub_cfgQ.addr;
			cfgQ_len = &ctx->p_curr_bcmq->dhub_cfgQ.len;
			*cfgQ_len += START_2NDDMA(&CSI_dhubHandle, dma_id, addr[Y_FRAME], width_byte, width_byte,
							1, stride_len, ctx->op_ht, (T64b *)cfgQ);
		} else {
			START_2NDDMA(&CSI_dhubHandle, dma_id, addr[Y_FRAME], width_byte, width_byte,
							1, stride_len, ctx->op_ht, NULL);
		}
	}

	if (IS_VALID_INPUT(ctx->c_wr_ip)) {
		uv_offset = ctx->op_wt * ctx->op_ht * (ctx->op_bpp / 8); //YUV420SP & YUV422SP
		height = (ctx->op_fmt == CAM_PIXFMT_YUV420SP) ? (ctx->op_ht / 2) : ctx->op_ht;
		dma_id = (pipe == 0) ? avioDhubChMap_vip128b_IPI0C_W : avioDhubChMap_vip128b_IPI1C_W;
		if (ctx->bcm_enable && (ctx->p_curr_bcmq != NULL)) {
			cfgQ = ((ctx->p_curr_bcmq->dhub_cfgQ.addr) + (*cfgQ_len));
			*cfgQ_len += START_2NDDMA(&CSI_dhubHandle, dma_id, addr[UV_FRAME], width_byte, width_byte,
							1, stride_len, height, (T64b *)cfgQ);
		} else {
			START_2NDDMA(&CSI_dhubHandle, dma_id, addr[UV_FRAME], width_byte, width_byte,
							1, stride_len, height, NULL);
		}
	}

	if (ctx->bcm_enable && (ctx->p_curr_bcmq != NULL))
		CAM_CFGQ_To_BCMBUF(&ctx->p_curr_bcmq->dhub_cfgQ, &ctx->p_curr_bcmq->dhub_bcmBuf);
}

static void CSI_PIPE_FlushPipe(CSI_PL_CTX_t *ctx)
{
	uint32_t val;
	uint32_t base_addr = ctx->pipe_base_addr;
	/* Use immediate writes when BCM not enabled or queue not ready */
	struct BCMBUF *p_bcmbuf = (ctx->bcm_enable && ctx->p_curr_bcmq) ? &ctx->p_curr_bcmq->bcmBuf : NULL;

	if (IS_MODULE_ENABLED(ctx->enabled_modules, MODULE_CSC)) {
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_CTRL1);
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_fifo_flush, bHOST2DHUB_CTRL1_fifo_flush);
		CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL1, val);
		SET_BIT(val, 0, LSb32HOST2DHUB_CTRL1_fifo_flush, bHOST2DHUB_CTRL1_fifo_flush);
		CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL1, val);
	}

	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_CTRL2);
	SET_BIT(val, 1, LSb32HOST2DHUB_CTRL2_input_fifo_flush, bHOST2DHUB_CTRL2_input_fifo_flush);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL2, val);
	SET_BIT(val, 0, LSb32HOST2DHUB_CTRL2_input_fifo_flush, bHOST2DHUB_CTRL2_input_fifo_flush);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_HOST2DHUB_CTRL2, val);
}

static void CSI_PIPE_FVF_Config(CSI_PL_CTX_t *ctx, int en)
{
	FVF_CONFIG_t config;
	struct isp_ctrl ctrl;

	config.enable = en;
	config.frame_width = ctx->op_wt;
	config.frame_height = ctx->op_ht;
	config.min_frame_gap = 0;
	config.min_line_gap = 0;
	config.max_frame_duration_enable = 0;
	config.max_frame_duration_value = 0;
	config.error_mask = 0x1ff; // FVF_ERR_ENABLE
	ctrl.cfg = &config;
	ctrl.handler = ctx;
	if (en) {
		ctrl.id = CID_FVF_CONFIG;
		ctrl.cfg = &config;
		fvf_s_ctrl(&ctrl);
		ctrl.id = CID_FVF_EN_ERRS;
		fvf_s_ctrl(&ctrl);
	}
	ctrl.id = CID_FVF_ENABLE;
	fvf_s_ctrl(&ctrl);
}

static void CSI_PIPE_Reset(CSI_PL_CTX_t *ctx)
{
	uint32_t reset;

	reset = HAL_ISP_CORE_REG_READ32(ctx->dev, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL);
	// Assert reset
	if (ctx->id == 0) { //CSIPipe1
		SET_BIT(reset, 0, LSb32vipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn,
			bvipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn);
	} else {
		SET_BIT(reset, 0, LSb32vipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn,
			bvipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn);
	}

	CAM_HAL_WriteReg(ctx->dev, NULL, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL, reset);
	// De-assert reset
	if (ctx->id == 0) { //CSIPipe1
		SET_BIT(reset, 1, LSb32vipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn,
			bvipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn);
	} else {
		SET_BIT(reset, 1, LSb32vipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn,
			bvipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn);
	}

	CAM_HAL_WriteReg(ctx->dev, NULL, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL, reset);
}

static void CSI_PIPE_StopPipeline(CSIPIPE_HANDLE handle)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;
	uint32_t base_addr = ctx->pipe_base_addr;
	uint32_t reset;
	uint32_t val;
	int dma_id;

	/* Gate EOF immediately in teardown */
	ctx->tg_en = 0;

	// TODO: Program essential stop registers H2DH enable, Wrclient etc.
	val = HAL_ISP_CORE_REG_READ32(ctx->dev,
		base_addr + RA_HOST2DHUB_IMAGERESWRAP + RA_IMAGERESWRAP_CTRL);
	SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_enable, bIMAGERESWRAP_CTRL_enable);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_HOST2DHUB_IMAGERESWRAP +
		RA_IMAGERESWRAP_CTRL, 0);
	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_CTRL6);
	SET_BIT(val, 0, LSb32HOST2DHUB_CTRL6_image_crop_en, bHOST2DHUB_CTRL6_image_crop_en);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_HOST2DHUB_CTRL6, val);
	if (ctx->fvf_en)
		CSI_PIPE_FVF_Config(ctx, 0);

	reset = HAL_ISP_CORE_REG_READ32(ctx->dev, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL);
	// Assert reset
	if (ctx->id == 0) { //CSIPipe1
		SET_BIT(reset, 0, LSb32vipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn,
			bvipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn);
	} else {
		SET_BIT(reset, 0, LSb32vipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn,
			bvipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn);
	}
	CAM_HAL_WriteReg(ctx->dev, NULL, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL, reset);
	/* Small delay after reset assertion to ensure hardware state settles */
	udelay(1);
	// De-assert reset
	if (ctx->id == 0) { //CSIPipe1
		SET_BIT(reset, 1, LSb32vipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn,
			bvipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn);
	} else {
		SET_BIT(reset, 1, LSb32vipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn,
			bvipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn);
	}
	CAM_HAL_WriteReg(ctx->dev, NULL, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL, reset);
	if (IS_VALID_INPUT(ctx->y_wr_ip)) {
		dma_id = (ctx->id == 0) ?
			avioDhubChMap_vip128b_IPI0Y_W : avioDhubChMap_vip128b_IPI1Y_W;
		CLEAR_2NDDMA(&CSI_dhubHandle, dma_id);
	}
	if (IS_VALID_INPUT(ctx->c_wr_ip)) {
		dma_id = (ctx->id == 0) ?
			avioDhubChMap_vip128b_IPI0C_W : avioDhubChMap_vip128b_IPI1C_W;
		CLEAR_2NDDMA(&CSI_dhubHandle, dma_id);
	}
	/* Flush Pipe - Direct Write Start */
	if (IS_IMGRESTODH_ACTIVE(ctx->crop.scale)) {
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr +
				RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_CTRL);
		SET_BIT(val, 3, LSb32IMAGERES_CTRL_fifoFlush, bIMAGERES_CTRL_fifoFlush);
		CAM_HAL_WriteReg(ctx->dev, NULL, base_addr +
			RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_CTRL, val);
		SET_BIT(val, 0,  LSb32IMAGERES_CTRL_fifoFlush, bIMAGERES_CTRL_fifoFlush);
		CAM_HAL_WriteReg(ctx->dev, NULL, base_addr +
			RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_CTRL, val);
	}
	if (IS_MODULE_ENABLED(ctx->enabled_modules, MODULE_CSC)) {
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_CTRL1);
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_fifo_flush, bHOST2DHUB_CTRL1_fifo_flush);
		CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_HOST2DHUB_CTRL1, val);
		SET_BIT(val, 0, LSb32HOST2DHUB_CTRL1_fifo_flush, bHOST2DHUB_CTRL1_fifo_flush);
		CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_HOST2DHUB_CTRL1, val);
	}

	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_HOST2DHUB_CTRL2);
	SET_BIT(val, 1, LSb32HOST2DHUB_CTRL2_input_fifo_flush,
		bHOST2DHUB_CTRL2_input_fifo_flush);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_HOST2DHUB_CTRL2, val);
	SET_BIT(val, 0, LSb32HOST2DHUB_CTRL2_input_fifo_flush,
		bHOST2DHUB_CTRL2_input_fifo_flush);
	CAM_HAL_WriteReg(ctx->dev, NULL, base_addr + RA_HOST2DHUB_CTRL2, val);
	/* Flush Pipe - Direct Write End */
	if (IS_VALID_INPUT(ctx->y_wr_ip)) {
		CSI_PIPE_write_client_clear(ctx, 1, 1);
		dma_id = (ctx->id == 0) ?
			avioDhubChMap_vip128b_IPI0Y_W : avioDhubChMap_vip128b_IPI1Y_W;
		csi_dhub_channel_clear(&CSI_dhubHandle.dhub, dma_id, NULL);
		csi_dhub_channel_flush(&CSI_dhubHandle.dhub, dma_id, NULL);
	}
	if (IS_VALID_INPUT(ctx->c_wr_ip)) {
		CSI_PIPE_write_client_clear(ctx, 0, 1);
		dma_id = (ctx->id == 0) ?
			avioDhubChMap_vip128b_IPI0C_W : avioDhubChMap_vip128b_IPI1C_W;
		csi_dhub_channel_clear(&CSI_dhubHandle.dhub, dma_id, NULL);
		csi_dhub_channel_flush(&CSI_dhubHandle.dhub, dma_id, NULL);
	}
	CAM_HAL_WriteReg(ctx->dev, NULL,
		ctx->pipe_base_addr + RA_HOST2DHUB_CLKEN_CTRL, 0x0);

	if (ctx->bcm_enable) {
		if (ctx->id)
			BCM_SCHED_QueueFlush(ctx->dev, PIPE2_QUEUE);
		else
			BCM_SCHED_QueueFlush(ctx->dev, PIPE1_QUEUE);
	}

	//TODO: Disable IIF clock if not required
	// Reset Variables of context
	ctx->p_curr_bcmq = NULL;
}

static void CSI_PIPE_EOF_Routine(CSI_PL_CTX_t *ctx)
{
	/* Ensure we have a current BCM queue when BCM is enabled */
	if (ctx->bcm_enable) {
		uint8_t index = CSI_PIPE_GetNextIndex(ctx->curr_bcm_index,
				BCM_BUF_RING_NUM);
		ctx->p_curr_bcmq = &ctx->p_bcmq[index];
		CAM_BCMBUF_Reset(&ctx->p_curr_bcmq->bcmBuf);
		CAM_BCMBUF_Reset(&ctx->p_curr_bcmq->dhub_bcmBuf);
		ctx->p_curr_bcmq->dhub_cfgQ.len = 0;
		ctx->p_curr_bcmq->final_cfgQ.len = 0;
		ctx->curr_bcm_index = index;
	}

	CSI_PIPE_FlushPipe(ctx);
	if (ctx->tg_en) {
		CSI_PIPE_tg_reset(ctx);
		CSI_PIPE_tg_start(ctx);
	}

	/* Use immediate register writes when BCM is disabled to avoid NULL p_bcmbuf */
	if (IS_VALID_INPUT(ctx->y_wr_ip)) {
		//CSI_PIPE_write_client_clear(ctx, COMP_Y, 0);
		CSI_PIPE_write_client_clear(ctx, COMP_Y,
			(ctx->bcm_enable && ctx->p_curr_bcmq) ? 0 : 1);
		CSI_PIPE_write_client_start(ctx, COMP_Y);
	}
	if (IS_VALID_INPUT(ctx->c_wr_ip)) {
		//CSI_PIPE_write_client_clear(ctx, COMP_UV, 0);
		CSI_PIPE_write_client_clear(ctx, COMP_UV,
			(ctx->bcm_enable && ctx->p_curr_bcmq) ? 0 : 1);
		CSI_PIPE_write_client_start(ctx, COMP_UV);
	}
	CSI_PIPE_DhubStart(ctx);

	if (ctx->bcm_enable  && ctx->p_curr_bcmq) {
		//commit buffer
		CSI_PIPE_AlignBCM(ctx->dev, &ctx->p_curr_bcmq->bcmBuf);
		CSI_PIPE_AlignBCM(ctx->dev, &ctx->p_curr_bcmq->dhub_bcmBuf);
		//CAM_BCMBUF_LogPrint(&ctx->p_curr_bcmq->bcmBuf);
		//CAM_BCMBUF_LogPrint(&ctx->p_curr_bcmq->dhub_bcmBuf);
		CAM_BCMBUF_To_CFGQ(ctx->dev, &ctx->p_curr_bcmq->bcmBuf,
			&ctx->p_curr_bcmq->final_cfgQ);
		CAM_BCMBUF_To_CFGQ(ctx->dev, &ctx->p_curr_bcmq->dhub_bcmBuf,
			&ctx->p_curr_bcmq->final_cfgQ);
		//CAM_CFGQ_LogPrint(&ctx->p_curr_bcmq->final_cfgQ);
		if (ctx->id) {
			CAM_BCMDHUB_CFGQ_Commit(ctx->dev, &ctx->p_curr_bcmq->final_cfgQ,
				PIPE2_QUEUE, 0);
		} else {
			CAM_BCMDHUB_CFGQ_Commit(ctx->dev, &ctx->p_curr_bcmq->final_cfgQ,
				PIPE1_QUEUE, 0);
		}
	}
}

#ifdef HANDLE_CSIHOST_IRQ
static int CSI_HOST_Irq_Handler(uint32_t intrNum, void *pArgs)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *) pArgs;

	pr_err("CSIHost Interrupt handler invoked on error IntrNum[%d] Status[0x%x]\n",
			intrNum, CAM_HAL_ReadReg(START_AVIO_CSIHOST + RA_CSIHOST_IRQSTS));
	CAM_HAL_WriteReg(ctx->dev, NULL, START_AVIO_CSIHOST + RA_CSIHOST_IRQSTS, 1);
	dw_mipi_csi_status(0);
}
#endif

/*
 * Kernel thread to process pending interrupts signaled by the IRQ handler.
 */
static int process_interrupt(void *arg)
{
	struct camera_isp_dev *isp_dev = (struct camera_isp_dev *)arg;

	pr_debug("%s: thread start (isp_dev=%p)\n", __func__, isp_dev);

	while (!kthread_should_stop()) {
		wait_event_interruptible_timeout(isp_dev->wq,
			isp_dev->pending_intr_count > 0,
			msecs_to_jiffies(1000));

		if (isp_dev->pending_intr_count > 0) {
			struct CSI_PL_CTX_s *ctx;
			unsigned long flags;
			unsigned int intr = 0;
			int popped = 0;

			/* Pop one intr number from queue and decrement pending count together */
			spin_lock_irqsave(&isp_dev->isr_lock, flags);
			if (isp_dev->intr_q_head != isp_dev->intr_q_tail) {
				intr = isp_dev->intr_q[isp_dev->intr_q_head];
				isp_dev->intr_q_head = (isp_dev->intr_q_head + 1) % ISP_INTR_Q_SIZE;
				isp_dev->pending_intr_count--;
				popped = 1;
			}
			spin_unlock_irqrestore(&isp_dev->isr_lock, flags);

			if (!popped) {
				pr_err("%s: pending>0 but queue empty (race)\n", __func__);
				continue;
			}

			pr_debug("%s: popped intr=0x%x\n", __func__, intr);

			/* Retrieve ctx for this intr */
			ctx = VIP_GetIntrCtx(isp_dev->intr_handle, intr);
			pr_debug("%s: VIP_GetIntrCtx(intr=0x%x) -> ctx=%p\n", __func__, intr, ctx);
			if (!ctx)
				continue;

			/* Apply original ISR switch logic in thread context */
			switch (intr) {
			case avioDhubSemMap_vip128b_vip_intr1:
			case avioDhubSemMap_vip128b_vip_intr3:
				break;
			case avioDhubSemMap_vip128b_CH0_intr: /* Dhub EOF interrupt */
			case avioDhubSemMap_vip128b_CH2_intr: /* Dhub EOF interrupt */
				if (!ctx->tg_en) {
					ctx->frame_cnt++;
					CAM_Callback((CSI_PL_CTX_t *)ctx);
					CSI_PIPE_EOF_Routine((CSI_PL_CTX_t *)ctx);
				}
				break;
			case avioDhubSemMap_vip128b_vip_intr2:
			case avioDhubSemMap_vip128b_vip_intr4:
				if (ctx->tg_en) {
					ctx->frame_cnt++;
					CAM_Callback((CSI_PL_CTX_t *)ctx);
					CSI_PIPE_EOF_Routine((CSI_PL_CTX_t *)ctx);
				}
				break;
			case avioDhubSemMap_vip128b_vip_intr6:
			case avioDhubSemMap_vip128b_vip_intr7:
				break;
			default:
				break;
			}
		} else {
			/* Woke up but no pending interrupt (likely timeout) */
		}
		usleep_range(2, 5);
	}

	pr_debug("%s: thread exit\n", __func__);
	return 0;
}

/**
 * Enable CPU interrupt
 * @param[in] ctx - pipeline context
 */
static int CSI_PIPE_Irq_Handler(uint32_t intrNum, void *pArgs)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)pArgs;
	struct camera_isp_dev *isp_dev;
	unsigned long flags;

	if (!ctx || !atomic_read(&ctx->active))
		return 0;

	isp_dev = (struct camera_isp_dev *)ctx->parent;
	if (!isp_dev)
		return 0;

#ifdef DEBUG_INTR
	static int count;

	if (intrNum == 0 && count++ % 30 == 0)
		pr_err("%s intrNum 0x%x\n", __func__, intrNum);
#endif
	/* Record count per intr for stats */
	ctx->intr_cnt[intrNum]++;

	/* Queue work to thread */
	spin_lock_irqsave(&isp_dev->isr_lock, flags);
	/* push intrNum into ring buffer */
	{
		unsigned int next = (isp_dev->intr_q_tail + 1) % ISP_INTR_Q_SIZE;

		if (next == isp_dev->intr_q_head) {
			/* overflow, drop and log */
			pr_err("%s: intr queue overflow, dropping intr=0x%x\n", __func__, intrNum);
		} else {
			isp_dev->intr_q[isp_dev->intr_q_tail] = intrNum;
			isp_dev->intr_q_tail = next;
			isp_dev->pending_intr_count++;
		}
	}
	spin_unlock_irqrestore(&isp_dev->isr_lock, flags);
	wake_up_interruptible(&isp_dev->wq);

	return 0;
}

static void CSI_PIPE_RegisterIrq(CSI_PL_CTX_t *ctx, int pipe)
{
	struct camera_isp_dev *isp_dev = ctx->parent;
	// TODO: Enable interrupt only as required
	if (pipe == 0) {
		VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_vip_intr1,
			ctx, CSI_PIPE_Irq_Handler);
		VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_vip_intr2,
			ctx, CSI_PIPE_Irq_Handler);
		VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_vip_intr6,
			ctx, CSI_PIPE_Irq_Handler);
		VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_CH0_intr,
			ctx, CSI_PIPE_Irq_Handler);
	} else {
		VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_vip_intr3,
			ctx, CSI_PIPE_Irq_Handler);
		VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_vip_intr4,
			ctx, CSI_PIPE_Irq_Handler);
		VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_vip_intr7,
			ctx, CSI_PIPE_Irq_Handler);
		VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_CH2_intr,
			ctx, CSI_PIPE_Irq_Handler);
	}
	VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_vip_intr5,
		ctx, CSI_PIPE_Irq_Handler);
#ifdef HANDLE_CSIHOST_IRQ
	VIP_DhubIntrRegisterHandler(isp_dev->intr_handle, avioDhubSemMap_vip128b_vip_intr0,
		ctx, CSI_HOST_Irq_Handler);
#endif
}

int CSI_PIPE_Init(struct camera_isp_dev *isp_dev)
{
	int clock;
	int reset;

	// Disable IIF clock by default
	clock = HAL_ISP_CORE_REG_READ32(isp_dev, CSIPIPE_OFFSET + RA_CSIPIPE_CTRL);
	SET_BIT(clock, 0, LSb32CSIPIPE_CTRL_IIF_clken, bCSIPIPE_CTRL_IIF_clken);
	CAM_HAL_WriteReg(isp_dev, NULL, CSIPIPE_OFFSET + RA_CSIPIPE_CTRL, clock);

	/* Reset CSIPipe(include IIF) & CSIHost registers */
	reset = HAL_ISP_CORE_REG_READ32(isp_dev, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL);
	// Assert reset
	SET_BIT(reset, 0, LSb32vipGbl_SWRST_CTRL_biu_ipiSyncRstn, bvipGbl_SWRST_CTRL_biu_ipiSyncRstn);
	SET_BIT(reset, 0, LSb32vipGbl_SWRST_CTRL_apbCSIHostSyncRstn,
		bvipGbl_SWRST_CTRL_apbCSIHostSyncRstn);
	CAM_HAL_WriteReg(isp_dev, NULL, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL, reset);
	// De-assert reset
	SET_BIT(reset, 1, LSb32vipGbl_SWRST_CTRL_biu_ipiSyncRstn, bvipGbl_SWRST_CTRL_biu_ipiSyncRstn);
	SET_BIT(reset, 1, LSb32vipGbl_SWRST_CTRL_apbCSIHostSyncRstn,
		bvipGbl_SWRST_CTRL_apbCSIHostSyncRstn);
	CAM_HAL_WriteReg(isp_dev, NULL, VIP_GBL_OFFSET + RA_vipGbl_SWRST_CTRL, reset);

	csi_dhub_init(isp_dev, 0); //cpuId=0
	CAM_HAL_WriteReg(isp_dev, NULL, VIP_GBL_OFFSET + RA_vipGbl_INTR_CTRL, 0x3fff);
	isp_dev->intr_handle = VIP_IntrHandleInit(isp_dev->dev, isp_dev->irq_num);

	/* Initialize interrupt processing thread infrastructure */
	init_waitqueue_head(&isp_dev->wq);
	spin_lock_init(&isp_dev->isr_lock);
	isp_dev->pending_intr_count = 0;
	isp_dev->intr_q_head = 0;
	isp_dev->intr_q_tail = 0;
	isp_dev->intr_thread = kthread_run(process_interrupt, isp_dev, "isp_intr");
	if (IS_ERR(isp_dev->intr_thread)) {
		pr_err("Failed to start isp_intr thread\n");
		isp_dev->intr_thread = NULL;
	}

	return 0;
}

void CSI_PIPE_Exit(struct camera_isp_dev *isp_dev)
{
	/* Stop interrupt processing thread */
	if (isp_dev->intr_thread) {
		kthread_stop(isp_dev->intr_thread);
		isp_dev->intr_thread = NULL;
	}

	VIP_IntrHandleExit(isp_dev->intr_handle);
	csi_dhub_exit(0);
}

CSIPIPE_HANDLE CSI_PIPE_Create(struct camera_isp_dev *isp_dev, int pipe)
{
	CSI_PL_CTX_t *ctx;
	struct BCMBUF *pBcmBuf;
	struct DHUB_CFGQ *pCfgQ;
	int val;
	int i;

	ctx = kzalloc(sizeof(CSI_PL_CTX_t), GFP_KERNEL);
	ctx->parent = isp_dev;
	ctx->dev = isp_dev;
	ctx->pipe_base_addr = pipe == 0 ? CSIPIPE_OFFSET + RA_CSIPIPE_HOST2DHUB1 :
		CSIPIPE_OFFSET + RA_CSIPIPE_HOST2DHUB2;
	ctx->id = pipe;
#ifdef BCM_ENABLE
	ctx->bcm_enable = 1;
#else
	ctx->bcm_enable = 0;
#endif

	for (i = 0; i < BCM_BUF_RING_NUM; i++) {
		pBcmBuf = &ctx->p_bcmq[i].bcmBuf;
		CAM_BCMBUF_Create(pBcmBuf, BCM_BUFFER_SIZE);
		CAM_BCMBUF_Reset(pBcmBuf);

		pBcmBuf = &ctx->p_bcmq[i].dhub_bcmBuf;
		CAM_BCMBUF_Create(pBcmBuf, BCM_BUFFER_SIZE);
		CAM_BCMBUF_Reset(pBcmBuf);

		pCfgQ = &ctx->p_bcmq[i].final_cfgQ;
		CAM_CFGQ_Create(pCfgQ, FINAL_CFGQ_SIZE);
		pCfgQ->len = 0;

		pCfgQ = &ctx->p_bcmq[i].dhub_cfgQ;
		CAM_CFGQ_Create(pCfgQ, PIPE_CFGQ_SIZE);
		pCfgQ->len = 0;
	}
	// Disable Pipe by default
	val = HAL_ISP_CORE_REG_READ32(isp_dev,
		ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP + RA_IMAGERESWRAP_CTRL);
	SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_enable, bIMAGERESWRAP_CTRL_enable);
	CAM_HAL_WriteReg(isp_dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP +
					RA_IMAGERESWRAP_CTRL, val);
	INIT_LIST_HEAD(&ctx->buf.queue);
	spin_lock_init(&ctx->buf.lock);
	atomic_set(&ctx->active, 1);
	return (CSIPIPE_HANDLE)ctx;
}

void CSI_PIPE_Destroy(CSIPIPE_HANDLE handle)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;
	struct camera_isp_dev *isp_dev = ctx->dev;
	struct BCMBUF *pBcmBuf;
	struct DHUB_CFGQ *pCfgQ;
	int i;

	// TODO: Enable interrupt only as required
	if (ctx->id == 0) {
		VIP_DhubIntrDeRegisterHandler(isp_dev->intr_handle,
			avioDhubSemMap_vip128b_vip_intr1);
		VIP_DhubIntrDeRegisterHandler(isp_dev->intr_handle,
			avioDhubSemMap_vip128b_vip_intr2);
		VIP_DhubIntrDeRegisterHandler(isp_dev->intr_handle,
			avioDhubSemMap_vip128b_vip_intr6);
		VIP_DhubIntrDeRegisterHandler(isp_dev->intr_handle,
			avioDhubSemMap_vip128b_CH0_intr);
	} else {
		VIP_DhubIntrDeRegisterHandler(isp_dev->intr_handle,
			avioDhubSemMap_vip128b_vip_intr3);
		VIP_DhubIntrDeRegisterHandler(isp_dev->intr_handle,
			avioDhubSemMap_vip128b_vip_intr4);
		VIP_DhubIntrDeRegisterHandler(isp_dev->intr_handle,
			avioDhubSemMap_vip128b_vip_intr7);
		VIP_DhubIntrDeRegisterHandler(isp_dev->intr_handle,
			avioDhubSemMap_vip128b_CH2_intr);
	}

	for (i = 0; i < BCM_BUF_RING_NUM; i++) {
		pBcmBuf = &ctx->p_bcmq[i].bcmBuf;
		CAM_BCMBUF_Destroy(pBcmBuf);

		pBcmBuf = &ctx->p_bcmq[i].dhub_bcmBuf;
		CAM_BCMBUF_Destroy(pBcmBuf);

		pCfgQ = &ctx->p_bcmq[i].final_cfgQ;
		CAM_CFGQ_Destroy(pCfgQ);

		pCfgQ = &ctx->p_bcmq[i].dhub_cfgQ;
		CAM_CFGQ_Destroy(pCfgQ);
	}

	kfree(ctx);
}

void CSI_PIPE_Set_Fmt(CSIPIPE_HANDLE handle, struct v4l2_mbus_framefmt *format)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;

	ctx->op_fmt = CSI_PIPE_GetCamFormat(format->code);

	ctx->swizzle_ctrl = 0;
	ctx->op_wt = format->width;
	ctx->op_ht = format->height;
}

void CSI_PIPE_Set_Input_Fmt(CSIPIPE_HANDLE handle, uint32_t width, uint32_t height)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;

	ctx->hres = width;
	ctx->vres = height;
}

int CSI_PIPE_Config(CSIPIPE_HANDLE handle, uint32_t mbus_code, uint32_t scale_factor)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;

	// Source property
	ctx->src = SRC_INTF_IPI0;
	ctx->src_fmt = mbus_code;
	ctx->inp_comp_order = 1;

	// Module enable control
	ctx->fvf_en = 0; // Fix : Keep it 0 to fix the restart issue
	ctx->op_through_ipi = 0;

	// Capture properties
	ctx->capture_mode = 0;
	ctx->capture_frame_interval = 0;
	ctx->skip_frame_num = 0;


	// Use pre-calculated scale factor from resolution selection algorithm
	if (scale_factor <= 1) {
		// Normal mode: 1:1 (no scaling)
		ctx->crop.scale = 0;
	} else {
		// Scaling mode: downscale from input to output
		ctx->crop.scale = scale_factor;
	}
	ctx->crop.imgres_oprn = 0;
	// For binning sacling factor should be power of 2
	if ((scale_factor != 0) &&
			((scale_factor & (scale_factor - 1)) == 0))
		ctx->crop.imgres_oprn = 1;

	pr_debug("[SCALING] CSI_PIPE_Config: input=%dx%d output=%dx%d "
		"crop(%d,%d)-(%d,%d) scale=%d oprn=%d\n",
		ctx->hres, ctx->vres, ctx->op_wt, ctx->op_ht,
		ctx->crop.x_st, ctx->crop.y_st, ctx->crop.x_end, ctx->crop.y_end,
		ctx->crop.scale, ctx->crop.imgres_oprn);

	return 0;
}

static void CSI_PIPE_ImgRes_Config(CSIPIPE_HANDLE handle)
{
	uint32_t val = 0;
	struct BCMBUF *p_bcmbuf = NULL;
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;
	uint32_t base_addr = ctx->pipe_base_addr;
	uint32_t scale = ctx->crop.scale;

	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr + RA_IMAGERESWRAP_IMAGERES
			+ RA_IMAGERES_CTRL);
	SET_BIT(val, 3, LSb32IMAGERES_CTRL_fifoFlush, bIMAGERES_CTRL_fifoFlush);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_IMAGERESWRAP_IMAGERES +
		RA_IMAGERES_CTRL, val);
	SET_BIT(val, 0,  LSb32IMAGERES_CTRL_fifoFlush, bIMAGERES_CTRL_fifoFlush);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_IMAGERESWRAP_IMAGERES +
		RA_IMAGERES_CTRL, val);

	if (ctx->inp_comp_order != FMT_GRAYSCALE) { // Bayer pattern
		SET_BIT(val, 1, LSb32IMAGERES_CTRL_imgType, bIMAGERES_CTRL_imgType);
	} else {
		SET_BIT(val, 0, LSb32IMAGERES_CTRL_imgType, bIMAGERES_CTRL_imgType);
	}
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr + RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_CTRL, val);

	/// Input resolution settings
	val =  0;
	SET_BIT(val, ctx->hres, LSb32IMAGERES_IMGINSIZE_imgInHSize,
		bIMAGERES_IMGINSIZE_imgInHSize);
	SET_BIT(val, ctx->vres, LSb32IMAGERES_IMGINSIZE_imgInVSize,
		bIMAGERES_IMGINSIZE_imgInVSize);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr +
		RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_IMGINSIZE, val);

	/// Output resolution settings
	val =  0;
	SET_BIT(val, ctx->op_wt, LSb32IMAGERES_IMGOUTSIZE_imgOutHSize,
		bIMAGERES_IMGOUTSIZE_imgOutHSize);
	SET_BIT(val, ctx->op_ht, LSb32IMAGERES_IMGOUTSIZE_imgOutVSize,
		bIMAGERES_IMGOUTSIZE_imgOutVSize);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr +
		RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_IMGOUTSIZE, val);

	/// Crop settings
	val = 0;
	SET_BIT(val, ctx->crop.x_st, LSb32IMAGERES_CROPSTART_cropHStart,
		bIMAGERES_CROPSTART_cropHStart);
	SET_BIT(val, ctx->crop.y_st, LSb32IMAGERES_CROPSTART_cropVStart,
		bIMAGERES_CROPSTART_cropVStart);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr +
		RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_CROPSTART, val);

	val = 0;
	SET_BIT(val, ctx->crop.x_end, LSb32IMAGERES_CROPEND_cropHEnd,
		bIMAGERES_CROPEND_cropHEnd);
	SET_BIT(val, ctx->crop.y_end, LSb32IMAGERES_CROPEND_cropVEnd,
		bIMAGERES_CROPEND_cropVEnd);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr +
		RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_CROPEND, val);

	/// Scale settings
	val = HAL_ISP_CORE_REG_READ32(ctx->dev, base_addr +
		RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_CTRL);
	ctx->crop.imgres_oprn = (ctx->crop.imgres_oprn < IMGRES_OPRN_MAX) ?
								ctx->crop.imgres_oprn : IMGRES_OPRN_BINNING;
	pr_info("ImgRes %s mode Opn\n", ctx->crop.imgres_oprn ?
			"Scaling" : "Cropping");
	SET_BIT(val, ctx->crop.imgres_oprn, LSb32IMAGERES_CTRL_imgResOpr,
		bIMAGERES_CTRL_imgResOpr);
	SET_BIT(val, (scale - 1), LSb32IMAGERES_CTRL_ratio,
		bIMAGERES_CTRL_ratio);
	CAM_HAL_WriteReg(ctx->dev, p_bcmbuf, base_addr +
		RA_IMAGERESWRAP_IMAGERES + RA_IMAGERES_CTRL, val);
	// ImgRes - END

}

static void CSI_PIPE_Wb_Config(CSI_PL_CTX_t *ctx, int input_sel)
{
	WB_CONFIG_t config;
	struct isp_ctrl ctrl;

	config.input_sel = input_sel;
	ctrl.id = CID_WB_CONFIG;
	ctrl.cfg = &config;
	ctrl.handler = ctx;
	wb_s_ctrl(&ctrl);
}

static void CSI_PIPE_Demosaic_Config(CSI_PL_CTX_t *ctx)
{
	DEMOSAIC_CONFIG_t config;
	struct isp_ctrl ctrl;

	config.enable = 1;
	config.demosaic_mode = ctx->inp_comp_order;
	config.width = ctx->op_wt;
	config.height = ctx->op_ht;
	config.swizzle_ctrl = ctx->swizzle_ctrl;
	if (ctx->wb_en)
		config.input_sel = 0; // whitebalance
	else
		config.input_sel = 1; // whitebalance bypassed
	ctrl.id = CID_DEMOSAIC_CONFIG;
	ctrl.cfg = &config;
	ctrl.handler = ctx;
	demosaic_s_ctrl(&ctrl);
	ctrl.id = CID_DEMOSAIC_ENABLE;
	demosaic_s_ctrl(&ctrl);
}

static void CSI_PIPE_Csc_Config(CSI_PL_CTX_t *ctx, uint32_t input, uint32_t mode, int en)
{
	CSC_CONFIG_t config;
	struct isp_ctrl ctrl;

	config.enable = en;
	config.mode = mode;

	config.input_sel = input;
	ctrl.cfg = &config;
	ctrl.handler = ctx;
	ctrl.id = CID_CSC_CONFIG;
	csc_s_ctrl(&ctrl);
	if (en)
		ctrl.id = CID_CSC_ENABLE;
	else
		ctrl.id = CID_CSC_DISABLE;
	csc_s_ctrl(&ctrl);
}

static void CSI_PIPE_DNS444to422_Config(CSI_PL_CTX_t *ctx)
{
	CSC_CONFIG_t config;
	struct isp_ctrl ctrl;

	config.enable = 1;
	ctrl.id = CID_CSC_DNS_444_TO_422;
	ctrl.cfg = &config;
	ctrl.handler = ctx;
	csc_s_ctrl(&ctrl);
}

static void CSI_PIPE_DNS422to420_Config(CSI_PL_CTX_t *ctx, uint8_t zero_ld)
{
	CSC_CONFIG_t config;
	struct isp_ctrl ctrl;

	config.enable = 1;
	config.zero_line_delay = zero_ld;

	ctrl.id = CID_CSC_DNS_422_TO_420;
	ctrl.cfg = &config;
	ctrl.handler = ctx;
	csc_s_ctrl(&ctrl);
}

static inline bool is_packed_mode(int fmt)
{
	return fmt == CAM_PIXFMT_YUV444 || fmt == CAM_PIXFMT_RGB888;
}

static void CSI_PIPE_TG_Config(CSI_PL_CTX_t *ctx, int ipi_out_fmt, int csc_fifo_en)
{
	uint32_t val;

	if (csc_fifo_en) {
		/* CLK control Programming */
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_TG_CTRL);
		SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_clken_ctrl2,
			bHOST2DHUB_TG_CTRL_clken_ctrl2);
		SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_clken_ctrl3,
			bHOST2DHUB_TG_CTRL_clken_ctrl3);
		SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_clken_ctrl4,
			bHOST2DHUB_TG_CTRL_clken_ctrl4);
		SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_clken_ctrl5,
			bHOST2DHUB_TG_CTRL_clken_ctrl5);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_TG_CTRL, val);
		// TG clock controls
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
			RA_HOST2DHUB_TG_CTRL);
		SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_clken_ctrl0,
			bHOST2DHUB_TG_CTRL_clken_ctrl0);
		if (IS_VALID_INPUT(ctx->c_wr_ip)) {
			SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_clken_ctrl1,
				bHOST2DHUB_TG_CTRL_clken_ctrl1);
		} else {
			SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_clken_ctrl1,
				bHOST2DHUB_TG_CTRL_clken_ctrl1);
		}
		SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_fifo_ctrlEn,
			bHOST2DHUB_TG_CTRL_fifo_ctrlEn);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_TG_CTRL, val);
		/* End of CLK control programming */
		// TG Configuration
		CSI_PIPE_tg_reset(ctx);
		CSI_PIPE_tg_config(ctx);
		CSI_PIPE_tg_start(ctx);
		ctx->tg_en = 1;
	} else {
		/* CLK control Programming */
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_TG_CTRL);
		SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_clken_ctrl2,
			bHOST2DHUB_TG_CTRL_clken_ctrl2);
		SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_clken_ctrl3,
			bHOST2DHUB_TG_CTRL_clken_ctrl3);
		if (IS_VALID_INPUT(ctx->c_wr_ip)) {
			SET_BIT(val, 0, LSb32HOST2DHUB_TG_CTRL_clken_ctrl4,
				bHOST2DHUB_TG_CTRL_clken_ctrl4);
		} else {
			SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_clken_ctrl4,
				bHOST2DHUB_TG_CTRL_clken_ctrl4);
		}
		SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_clken_ctrl5,
			bHOST2DHUB_TG_CTRL_clken_ctrl5);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_TG_CTRL, val);

		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_TG_CTRL);
		SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_clken_ctrl0,
				bHOST2DHUB_TG_CTRL_clken_ctrl0);
		SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_clken_ctrl1,
				bHOST2DHUB_TG_CTRL_clken_ctrl1);
		SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_fifo_ctrlEn,
			bHOST2DHUB_TG_CTRL_fifo_ctrlEn);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_TG_CTRL, val);
		ctx->tg_en = 0;
		/* End of CLK control programming */
	}
}

void CSI_PIPE_Start(CSIPIPE_HANDLE handle)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;
	uint32_t capture_mode = ctx->capture_mode;
	uint32_t module_list = 0;
	MODULE_STATUS_t mod = {0};
	uint32_t bcm_mux_event;
	uint32_t seq_crop = 0;
	uint32_t ipi_out_fmt;
	uint32_t skip_frame;
	uint8_t zero_ld = 0;
	uint32_t scale = 0;
	int32_t frame_num;
	int csc_input = 1; // by default set as IPI input
	uint32_t val = 0;
	uint8_t sp420_en = 0;
	int mode;
	int res;
	struct camera_isp_dev *isp_dev = ctx->dev;
	unsigned long flags;
	int crop_is_full;

	// Init WrClient variables with invalid value
	ctx->y_wr_ip = INVD_INPUT;
	ctx->c_wr_ip = INVD_INPUT;
	ipi_out_fmt = CSI_PIPE_GetIPIFormat(ctx->src_fmt);

	pr_debug("%s: id: %d ip fmt: 0x%x, op fmt: 0x%x\n", __func__,
			ctx->id, ipi_out_fmt, ctx->op_fmt);
	scale = ctx->crop.scale;
	ctx->op_bpp = 8;
	if (!ctx->op_through_ipi)
		mod.imgres_byp = 1;
	mod.wb_en = ctx->wb_en;
	crop_is_full = (ctx->crop.x_st == 0 && ctx->crop.y_st == 0 &&
			   (ctx->crop.x_end + 1) == ctx->hres &&
			   (ctx->crop.y_end + 1) == ctx->vres);
	/*
	 * Only scaling
	 * Or both cropping and scaling
	 */
	if (IS_IMGRESTODH_ACTIVE(ctx->crop.scale)) {
		mod.imgres_en = 1;
		mod.imgres_byp = 0;
		ctx->op_wt = (ctx->crop.x_end + 1 - ctx->crop.x_st) / scale;
		ctx->op_ht = (ctx->crop.y_end + 1 - ctx->crop.y_st) / scale;
	}
	/* Only cropping */
	else if (!crop_is_full) {
		ctx->op_wt = ctx->crop.x_end + 1 - ctx->crop.x_st;
		if (is_packed_mode(ipi_out_fmt)) {
			ctx->op_wt = ctx->op_wt * 3;
			mod.seq_en = 1;
		}
		ctx->op_ht = ctx->crop.y_end + 1 - ctx->crop.y_st;
	}
	/* Full frame: no cropping and no scaling */
	else {
		ctx->op_wt = ctx->hres;
		ctx->op_ht = ctx->vres;
		/* COLOR48 mode: IPI emits 3 component-clocks per pixel.
		 * Only enable seq path when output is also packed (RGB888/YUV444).
		 * DNS outputs (NV12/NV16/UYVY) consume triplets natively; seq path
		 * would triple op_wt and stall DHUB waiting for 3x more bytes.
		 */
		if (is_packed_mode(ipi_out_fmt) &&
		    (ctx->op_fmt == CAM_PIXFMT_RGB888 ||
		     ctx->op_fmt == CAM_PIXFMT_YUV444)) {
			ctx->op_wt = ctx->hres * 3;
			mod.seq_en = 1;
		}
	}
	/* When resolution is not multiple of 3 IPI will
	 * insert padding pixels - handle for RAW
	 */
	if ((ctx->src != SRC_INTF_IIF) && (CAM_PIXFMT_RAW16 == ipi_out_fmt) &&
		 (ctx->hres % 3 != 0)) {
		ctx->hres = CSI_ALIGN(ctx->hres, 3);
		if (!ctx->crop.x_end) {
			ctx->crop.x_st = 0;
			ctx->crop.x_end = ctx->op_wt - 1;
		}
		if (!ctx->crop.y_end) {
			ctx->crop.y_st = 0;
			ctx->crop.y_end = ctx->op_ht - 1;
		}
	}
	res = CSI_PIPE_CheckCompatibility(ipi_out_fmt, ctx->op_fmt);
	if (res != 0) {
		pr_err("Compatibility check failed ipi_fmt[0x%x] out_fmt[0x%x], Exit\n",
			   ipi_out_fmt, ctx->op_fmt);
		return;
	}
	if ((ctx->hres != ctx->op_wt) || (ctx->vres != ctx->op_ht) || mod.seq_en)
		mod.crop_en = 1;

	CSI_PIPE_Reset(ctx);
	switch (ctx->op_fmt) {
	case CAM_PIXFMT_RAW8:
		ctx->y_wr_ip = 5;
		ctx->pack_sel = 0;
		break;
	case CAM_PIXFMT_RAW16:
		ctx->y_wr_ip = 6;
		ctx->pack_sel = 3;
		ctx->op_bpp = 16;
		if (ctx->src == SRC_INTF_IIF)
			ctx->op_bpp = 48;

		ctx->fvf_en = 0;
		mod.imgres_byp = 0;
		break;
	case CAM_PIXFMT_RGB888:
		if ((ipi_out_fmt == CAM_PIXFMT_RGB888) ||
			(ipi_out_fmt == CAM_PIXFMT_RGB565)) {
			mod.csc_en = 1; // byapss csc
			ctx->y_wr_ip = 3;
			ctx->fvf_en = 0;
		} else {
			mod.dmsc_en = 1;
			ctx->y_wr_ip = 4;
		}
		ctx->pack_sel = 2;
		ctx->op_bpp = 24;
		if (mod.seq_en) {
			mod.csc_en = 0;
			ctx->y_wr_ip = 5;
			ctx->pack_sel = 0;
			ctx->op_bpp = 8;
		}
		break;
	case CAM_PIXFMT_RGB565:
		if ((ipi_out_fmt == CAM_PIXFMT_RGB888) ||
			(ipi_out_fmt == CAM_PIXFMT_RGB565)) {
			mod.csc_en = 1; //bypass csc
			ctx->y_wr_ip = 3;
			ctx->fvf_en = 0;
		} else {
			mod.dmsc_en = 1;
			ctx->y_wr_ip = 4;
		}
		ctx->pack_sel = 1; // 16bits
		ctx->op_bpp = 16;
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1);
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_enable_565_write,
					bHOST2DHUB_CTRL1_enable_565_write);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_CTRL1, val);
		break;
	case CAM_PIXFMT_YUV444:
		if (ipi_out_fmt < CAM_PIXFMT_RGB888)
			mod.dmsc_en = 1;
		else
			ctx->fvf_en = 0;

		mod.csc_en = 1;
		ctx->pack_sel = 2;
		ctx->y_wr_ip = 3;
		ctx->op_bpp = 24;
		if (mod.seq_en) {
			mod.csc_en = 0;
			ctx->y_wr_ip = 5;
			ctx->pack_sel = 0;
			ctx->op_bpp = 8;
		}
		break;
	case CAM_PIXFMT_YUV422SP:
		if (ipi_out_fmt < CAM_PIXFMT_RGB888)
			mod.dmsc_en = 1;
		else
			ctx->fvf_en = 0;

		if (ipi_out_fmt < CAM_PIXFMT_YUV444)
			mod.csc_en = 1;

		if (ctx->op_through_ipi) {
			val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
					RA_HOST2DHUB_CTRL1);
			SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_pix_toggle_en_444to422,
					bHOST2DHUB_CTRL1_pix_toggle_en_444to422);
			CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
					RA_HOST2DHUB_CTRL1, val);
			ctx->y_wr_ip = 9;
			ctx->c_wr_ip = 2;
		} else {
			mod.dns1_en = 1;
			ctx->y_wr_ip = 2;
			ctx->c_wr_ip = 1;
		}
		ctx->pack_sel = 0;
		ctx->op_bpp = 8;
		break;
	case CAM_PIXFMT_YUV422P:
		if (ipi_out_fmt < CAM_PIXFMT_RGB888)
			mod.dmsc_en = 1;
		else
			ctx->fvf_en = 0;

		if (ipi_out_fmt < CAM_PIXFMT_YUV444)
			mod.csc_en = 1;
		if (ctx->op_through_ipi) {
			val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
					RA_HOST2DHUB_CTRL1);
			SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_pix_toggle_en_444to422,
					bHOST2DHUB_CTRL1_pix_toggle_en_444to422);
			CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
					RA_HOST2DHUB_CTRL1, val);
			ctx->y_wr_ip = 8;
			ctx->pack_sel = 1;
		} else {
			mod.dns1_en = 1;
			ctx->pack_sel = 1;
			ctx->y_wr_ip = 1;
		}
		ctx->op_bpp = 16;
		break;
	case CAM_PIXFMT_YUV420SP:
		if (ipi_out_fmt < CAM_PIXFMT_RGB888)
			mod.dmsc_en = 1;
		if (ipi_out_fmt < CAM_PIXFMT_YUV444)
			mod.csc_en = 1;
		if (ipi_out_fmt < CAM_PIXFMT_YUV422SP)
			mod.dns1_en = 1;
		if (ipi_out_fmt == CAM_PIXFMT_YUV420SP) {
			ctx->pack_sel = 1;
			ctx->y_wr_ip = 10;
			ctx->c_wr_ip = 3;
			ctx->yuv420_dir_op = 1;
			csc_input = 2;
			val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
					RA_HOST2DHUB_CTRL1);
			SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl2,
				bHOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl2);
			SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl3,
				bHOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl3);
			SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_init_val_422to420,
				bHOST2DHUB_CTRL1_init_val_422to420);
			SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_line_toggle_en_422to420,
				bHOST2DHUB_CTRL1_line_toggle_en_422to420);
			SET_BIT(val, 2, LSb32HOST2DHUB_CTRL1_CSC_FIFO_wr_ctrl,
				bHOST2DHUB_CTRL1_CSC_FIFO_wr_ctrl);
			CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1, val);
		} else {
			mod.dns2_en = 1;
			ctx->pack_sel = 0;
			ctx->y_wr_ip = 0;
			ctx->c_wr_ip = 0;
		}
		ctx->op_bpp = 8;
		sp420_en = 1;
		break;
	default:
		pr_err("Output format[%d] not supported\n", ctx->op_fmt);
		return;
		break;
	}

	mod.fvf_en = ctx->fvf_en;
	if (IS_VALID_INPUT(ctx->c_wr_ip))
		mod.wr_c_en = 1;
	CSI_PIPE_RegisterIrq(ctx, ctx->id);
	CSI_PIPE_ClkControl(ctx, &mod);

	if (IS_VALID_INPUT(ctx->y_wr_ip)) {
		CSI_PIPE_write_client_clear(ctx, COMP_Y, 1);
		CSI_PIPE_write_client_config(ctx, COMP_Y, 0);
		CSI_PIPE_write_client_setinput(ctx, COMP_Y);
		CSI_PIPE_write_client_start(ctx, COMP_Y);
	}
	if (IS_VALID_INPUT(ctx->c_wr_ip)) {
		CSI_PIPE_write_client_clear(ctx, COMP_UV, 1);
		CSI_PIPE_write_client_config(ctx, COMP_UV, sp420_en);
		CSI_PIPE_write_client_setinput(ctx, COMP_UV);
		CSI_PIPE_write_client_start(ctx, COMP_UV);
	}

	if (ctx->src == SRC_INTF_IPI0) {
		int bit_pos = LSb32CSIPIPE_CTRL_HC0_ipi_halt_source + ctx->id;

		val = HAL_ISP_CORE_REG_READ32(ctx->dev, CSIPIPE_OFFSET +
				RA_CSIPIPE_CTRL);
		SET_BIT(val, 1, bit_pos, 1);
		CAM_HAL_WriteReg(ctx->dev, NULL, CSIPIPE_OFFSET +
				RA_CSIPIPE_CTRL, val);
	} else if (ctx->src == SRC_INTF_IPI1) {
		int bit_pos = LSb32CSIPIPE_CTRL_HC1_ipi_halt_source + ctx->id;

		val = HAL_ISP_CORE_REG_READ32(ctx->dev, CSIPIPE_OFFSET + RA_CSIPIPE_CTRL);
		SET_BIT(val, 1, bit_pos, 1);
		CAM_HAL_WriteReg(ctx->dev, NULL, CSIPIPE_OFFSET +
				RA_CSIPIPE_CTRL, val);
	}
	if (mod.imgres_en) {
		val = 0;
		SET_BIT(val, 1, LSb32IMAGERESWRAP_CTRL_ovflowClr1,
			bIMAGERESWRAP_CTRL_ovflowClr0);
		SET_BIT(val, 1, LSb32IMAGERESWRAP_CTRL_ovflowClr1,
			bIMAGERESWRAP_CTRL_ovflowClr1);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP +
			RA_IMAGERESWRAP_CTRL, val);
	} else {
		val = 0;
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_WB + RA_WB_CTRL1);
		SET_BIT(val, 1, LSb32WB_CTRL1_input_sel,
			bWB_CTRL1_input_sel);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_WB + RA_WB_CTRL1, val);
	}
	if (ctx->op_through_ipi) {
		// Enable backpressure from CSCFifo when IPI direct data is send
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1);
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_enable_halt_back,
				bHOST2DHUB_CTRL1_enable_halt_back);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1, val);
	} else {
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1);
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_enable_halt_front,
				bHOST2DHUB_CTRL1_enable_halt_front);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1, val);
	}

	val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
			RA_HOST2DHUB_IMAGERESWRAP + RA_IMAGERESWRAP_CTRL);
	SET_BIT(val, 1, LSb32IMAGERESWRAP_CTRL_enable, bIMAGERESWRAP_CTRL_enable);
	SET_BIT(val, ctx->src, LSb32IMAGERESWRAP_CTRL_input_sel,
			bIMAGERESWRAP_CTRL_input_sel);
	SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_WrClient_rdy_en,
			bIMAGERESWRAP_CTRL_WrClient_rdy_en);
	SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_lps_pixReq_en,
			bIMAGERESWRAP_CTRL_lps_pixReq_en);
	CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP +
			RA_IMAGERESWRAP_CTRL, val);

	/* Configure oneshot, Nth frame, software Capture - START */
	skip_frame = ctx->skip_frame_num ? (ctx->skip_frame_num - 1) : 0;
	frame_num = ctx->capture_frame_interval ? (ctx->capture_frame_interval - 1) : 0;
	val = HAL_ISP_CORE_REG_READ32(ctx->dev,
			ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP + RA_IMAGERESWRAP_CTRL);
	SET_BIT(val, skip_frame, LSb32IMAGERESWRAP_CTRL_nframes_skip,
			bIMAGERESWRAP_CTRL_nframes_skip);
	CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP +
			RA_IMAGERESWRAP_CTRL, val);
	if (capture_mode == SINGLE_CAPTURE_SW || capture_mode == SINGLE_CAPTURE_HW) {
		int sw_mode = capture_mode == SINGLE_CAPTURE_SW ? 1 : 0;

		SET_BIT(val, sw_mode, LSb32IMAGERESWRAP_CTRL_capture_sw,
			bIMAGERESWRAP_CTRL_capture_sw);
		SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_n_eof, bIMAGERESWRAP_CTRL_n_eof);
		SET_BIT(val, 1, LSb32IMAGERESWRAP_CTRL_one_shot_capture_on,
			bIMAGERESWRAP_CTRL_one_shot_capture_on);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP +
			RA_IMAGERESWRAP_CTRL, val);
	} else if (capture_mode == NTH_CAPTURE_SW || capture_mode == NTH_CAPTURE_HW) {
		int sw_mode = capture_mode == NTH_CAPTURE_SW ? 1 : 0;

		SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_one_shot_capture_on,
			bIMAGERESWRAP_CTRL_one_shot_capture_on);
		SET_BIT(val, sw_mode, LSb32IMAGERESWRAP_CTRL_capture_sw,
			bIMAGERESWRAP_CTRL_capture_sw);
		SET_BIT(val, frame_num, LSb32IMAGERESWRAP_CTRL_n_eof,
			bIMAGERESWRAP_CTRL_n_eof);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP +
			RA_IMAGERESWRAP_CTRL, val);
	} else {
		SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_capture_sw,
			bIMAGERESWRAP_CTRL_capture_sw);
		SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_one_shot_capture_on,
			bIMAGERESWRAP_CTRL_one_shot_capture_on);
		SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_n_eof, bIMAGERESWRAP_CTRL_n_eof);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP +
			RA_IMAGERESWRAP_CTRL, val);
	}
	/* Configure oneshot, Nth frame, software Capture - END */

	if (ctx->src == SRC_INTF_IIF) { //IIF
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_TG_CTRL);
		SET_BIT(val, 1, LSb32HOST2DHUB_TG_CTRL_zero_line_delay_en,
			bHOST2DHUB_TG_CTRL_zero_line_delay_en);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_TG_CTRL, val);

		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL5);
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL5_iif_mode, bHOST2DHUB_CTRL5_iif_mode);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL5, val);
		ADD_TO_MODULE_LIST(module_list, MODULE_IIF);
		zero_ld = 1;
	} else if (ctx->src == SRC_INTF_IPI0) {
		ADD_TO_MODULE_LIST(module_list, MODULE_IPI0);
	} else if (ctx->src == SRC_INTF_IPI1) {
		ADD_TO_MODULE_LIST(module_list, MODULE_IPI1);
	}

	if (!mod.imgres_en) {
		int wt, ht;

		if ((ipi_out_fmt == CAM_PIXFMT_RGB888) ||
			(ipi_out_fmt == CAM_PIXFMT_RGB565) ||
			(ipi_out_fmt == CAM_PIXFMT_YUV444)) {
			wt = ctx->hres * 3;
		} else {
			wt = ctx->hres;
		}
		ht = ctx->vres;
		seq_crop = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
						RA_HOST2DHUB_CTRL4);
		SET_BIT(seq_crop, wt, LSb32HOST2DHUB_CTRL4_image_width,
			bHOST2DHUB_CTRL4_image_width);
		SET_BIT(seq_crop, ht, LSb32HOST2DHUB_CTRL4_image_height,
			bHOST2DHUB_CTRL4_image_height);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_CTRL4, seq_crop);

		if ((ctx->hres != ctx->op_wt) || (ctx->vres != ctx->op_ht) || mod.seq_en) {
			int crp_x_st, crp_x_end;

			seq_crop = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
						RA_HOST2DHUB_CTRL6);
			SET_BIT(seq_crop, 1, LSb32HOST2DHUB_CTRL6_image_crop_en,
				bHOST2DHUB_CTRL6_image_crop_en);
			if ((ipi_out_fmt == CAM_PIXFMT_RGB888) ||
				(ipi_out_fmt == CAM_PIXFMT_RGB565) ||
				(ipi_out_fmt == CAM_PIXFMT_YUV444)) {
				/* x_end should be the last position of pixel, hence end pixel
				 * value should be multiplied by 3 then reduce 1 to find position
				 * of pixel. Since x_end input is already reduced by 1, need to add
				 * 1, then multiply and reduce 1
				 */
				crp_x_st = (ctx->crop.x_st * 3);
				crp_x_end = ((ctx->crop.x_end + 1) * 3) - 1;
			} else {
				crp_x_st = ctx->crop.x_st;
				crp_x_end = ctx->crop.x_end;
			}
			SET_BIT(seq_crop, crp_x_st, LSb32HOST2DHUB_CTRL6_crop_left,
				bHOST2DHUB_CTRL6_crop_left);
			SET_BIT(seq_crop, crp_x_end, LSb32HOST2DHUB_CTRL6_crop_right,
				bHOST2DHUB_CTRL6_crop_right);
			if ((ctx->src == SRC_INTF_IIF) && !IS_HW_CAPTURE_MODE(ctx->capture_mode)) {
				SET_BIT(seq_crop, 1, LSb32HOST2DHUB_CTRL6_image_crop_in_sel,
					bHOST2DHUB_CTRL6_image_crop_in_sel);
			} else {
				SET_BIT(seq_crop, 0, LSb32HOST2DHUB_CTRL6_image_crop_in_sel,
					bHOST2DHUB_CTRL6_image_crop_in_sel);
			}
			CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL6, seq_crop);

			seq_crop = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
						RA_HOST2DHUB_CTRL7);
			SET_BIT(seq_crop, ctx->crop.y_st, LSb32HOST2DHUB_CTRL7_crop_top,
				bHOST2DHUB_CTRL7_crop_top);
			SET_BIT(seq_crop, ctx->crop.y_end, LSb32HOST2DHUB_CTRL7_crop_bot,
				bHOST2DHUB_CTRL7_crop_bot);
			CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL7, seq_crop);
		}
	}
	// FVF
	if (ctx->fvf_en) {
		CSI_PIPE_FVF_Config(ctx, 1);
		ADD_TO_MODULE_LIST(module_list, MODULE_FVF);
	} else {
		val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1);
		SET_BIT(val, 1, LSb32HOST2DHUB_CTRL1_FVF_bypass,
			bHOST2DHUB_CTRL1_FVF_bypass);
		CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
			RA_HOST2DHUB_CTRL1, val);
	}
	// WhiteBalance
	if (ctx->wb_en) {
		ADD_TO_MODULE_LIST(module_list, MODULE_WB);
	} else {
		if (mod.imgres_en)
			CSI_PIPE_Wb_Config(ctx, 0); //imgres=0
		else
			CSI_PIPE_Wb_Config(ctx, 1); //imgres_bypass=1
	}
	// DEMOSAIC
	if (mod.dmsc_en) {
		CSI_PIPE_Demosaic_Config(ctx);
		ADD_TO_MODULE_LIST(module_list, MODULE_DEMOSAIC);
		csc_input = 0; // demosaic input
	}
	if ((ctx->op_fmt == CAM_PIXFMT_RGB888) ||
		(ctx->op_fmt == CAM_PIXFMT_RGB565) ||
		((ctx->op_fmt == CAM_PIXFMT_YUV444) &&
		(ipi_out_fmt == CAM_PIXFMT_YUV444))) {
		mode = CSC_BYPASS_MODE;
	} else {
		mode = CSC_RGB_TO_YUV_601;
	}
	if (mod.csc_en) {
		if (ipi_out_fmt >= CAM_PIXFMT_RGB888) {
			val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
					RA_HOST2DHUB_CTRL1);
			SET_BIT(val, 0, LSb32HOST2DHUB_CTRL1_CSC_IPI_swap_ctrl,
				bHOST2DHUB_CTRL1_CSC_IPI_swap_ctrl);
			SET_BIT(val, 0, LSb32HOST2DHUB_CTRL1_CSC_IPI_swap_ctrl_lvl2,
				bHOST2DHUB_CTRL1_CSC_IPI_swap_ctrl_lvl2);
			CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1, val);
		}
		CSI_PIPE_Csc_Config(ctx, csc_input, mode, 1);
		ADD_TO_MODULE_LIST(module_list, MODULE_CSC);
	} else {
		if ((ipi_out_fmt == CAM_PIXFMT_YUV444) && (ctx->op_through_ipi)) {
			val = HAL_ISP_CORE_REG_READ32(ctx->dev, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1);
			SET_BIT(val, 0, LSb32HOST2DHUB_CTRL1_CSC_IPI_swap_ctrl,
				bHOST2DHUB_CTRL1_CSC_IPI_swap_ctrl);
			SET_BIT(val, 2, LSb32HOST2DHUB_CTRL1_CSC_IPI_swap_ctrl_lvl2,
				bHOST2DHUB_CTRL1_CSC_IPI_swap_ctrl_lvl2);
			CAM_HAL_WriteReg(ctx->dev, NULL, ctx->pipe_base_addr +
				RA_HOST2DHUB_CTRL1, val);
		}
		CSI_PIPE_Csc_Config(ctx, csc_input, mode, 0);
	}
	CSI_PIPE_TG_Config(ctx, ipi_out_fmt,
		((mod.csc_en | mod.dns1_en | mod.dns2_en | ctx->op_through_ipi) & !mod.seq_en));

	// DNS_444_422
	if (mod.dns1_en) {
		CSI_PIPE_DNS444to422_Config(ctx);
		ADD_TO_MODULE_LIST(module_list, MODULE_DNS444_422);
	}

	// DNS_422_420
	if (mod.dns2_en) {
		CSI_PIPE_DNS422to420_Config(ctx, zero_ld);
		ADD_TO_MODULE_LIST(module_list, MODULE_DNS422_420);
	}

	if (mod.imgres_en) {
		CSI_PIPE_ImgRes_Config(handle);
		ADD_TO_MODULE_LIST(module_list, MODULE_IMGRES);
	}
	int dma_id;

	if (IS_VALID_INPUT(ctx->y_wr_ip)) {
		dma_id = (ctx->id == 0) ? avioDhubChMap_vip128b_IPI0Y_W :
					avioDhubChMap_vip128b_IPI1Y_W;
		csi_dhub_channel_clear(&CSI_dhubHandle.dhub, dma_id, NULL);
	}
	if (IS_VALID_INPUT(ctx->c_wr_ip)) {
		dma_id = (ctx->id == 0) ? avioDhubChMap_vip128b_IPI0C_W :
			avioDhubChMap_vip128b_IPI1C_W;
		csi_dhub_channel_clear(&CSI_dhubHandle.dhub, dma_id, NULL);
	}
	if (IS_HW_CAPTURE_MODE(ctx->capture_mode)  && ctx->bcm_enable) {
		// No need to start Dhub initially for HW mode capture
		bcm_mux_event = ctx->id ? BCMQMap_VIP_event5 : BCMQMap_VIP_event3;
	} else {
		if (ctx->tg_en) { // Use TG EOF
			bcm_mux_event = ctx->id ? BCMQMap_VIP_event6 : BCMQMap_VIP_event4;
		} else { // Use Dhub EOF
			if (mod.seq_en) { // use CSI eof for RGB/YUV crop cases
				bcm_mux_event =  (ctx->src == SRC_INTF_IPI1)
									? BCMQMap_VIP_event0 :
									BCMQMap_VIP_event1;
			} else {
				bcm_mux_event = ctx->id ? BCMQMap_VIP_event28 :
									BCMQMap_VIP_event26;
			}
		}
		CSI_PIPE_DhubStart(ctx);
	}

	ADD_TO_MODULE_LIST(module_list, MODULE_DHUB);
	ctx->enabled_modules = module_list;

	if (ctx->bcm_enable) {
		if (ctx->id) {
			BCM_SCHED_QueueFlush(ctx->dev, PIPE2_QUEUE);
			BCM_SchedSetMux(ctx->dev, PIPE2_QUEUE, bcm_mux_event);
		} else {
			BCM_SCHED_QueueFlush(ctx->dev, PIPE1_QUEUE);
			BCM_SchedSetMux(ctx->dev, PIPE1_QUEUE, bcm_mux_event);
		}
		CSI_PIPE_EOF_Routine(ctx);
	}
	CSI_PIPE_PrintPipeline(module_list);
	spin_lock_irqsave(&isp_dev->isr_lock, flags);
	isp_dev->streaming |= (1 << ctx->id);
	spin_unlock_irqrestore(&isp_dev->isr_lock, flags);
}

void CSI_PIPE_Stop(CSIPIPE_HANDLE handle)
{
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;
	struct camera_isp_dev *isp_dev = ctx->dev;
	unsigned long flags;

	/* Mark context as inactive to prevent interrupt handler from accessing it */
	atomic_set(&ctx->active, 0);

	spin_lock_irqsave(&isp_dev->isr_lock, flags);
	isp_dev->streaming &= ~(1 << ctx->id);
	if (!isp_dev->streaming)
		isp_dev->pending_intr_count = 0;
	spin_unlock_irqrestore(&isp_dev->isr_lock, flags);

	/* Reset frame counter when stopping pipeline */
	ctx->frame_cnt = 0;

	CSI_PIPE_StopPipeline(handle);
	INIT_LIST_HEAD(&ctx->buf.queue);
}

void CSI_PIPE_Mute(CSIPIPE_HANDLE handle, int mute)
{
	int val;
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;

	val = HAL_ISP_CORE_REG_READ32(ctx->dev, CSIPIPE_OFFSET +
			RA_CSIPIPE_VIDEO_MUTE);
	if (ctx->src == SRC_INTF_IIF) { // IIF
		SET_BIT(val, mute, LSb32CSIPIPE_VIDEO_MUTE_SW_MUTE_H2,
			bCSIPIPE_VIDEO_MUTE_SW_MUTE_H2);
	} else if (ctx->src == SRC_INTF_IPI0) { // IPI0
		SET_BIT(val, mute, LSb32CSIPIPE_VIDEO_MUTE_SW_MUTE_H0,
			bCSIPIPE_VIDEO_MUTE_SW_MUTE_H0);
	} else if (ctx->src == SRC_INTF_IPI1) { //IPI1
		SET_BIT(val, mute, LSb32CSIPIPE_VIDEO_MUTE_SW_MUTE_H1,
			bCSIPIPE_VIDEO_MUTE_SW_MUTE_H1);
	} else {
		pr_warn("Source does not support Mute function\n");
		return;
	}
	CAM_HAL_WriteReg(ctx->dev, NULL, CSIPIPE_OFFSET +
		RA_CSIPIPE_VIDEO_MUTE, val);
	pr_info("%s CSIPipe[%d]:\n", mute ? "Mute" : "Unmute", ctx->id);
}

void CSI_PIPE_Capture(CSIPIPE_HANDLE handle)
{
	int val;
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;
	uint32_t addr = ctx->pipe_base_addr + RA_HOST2DHUB_IMAGERESWRAP +
						RA_IMAGERESWRAP_CTRL;
	val = HAL_ISP_CORE_REG_READ32(ctx->dev, addr);
	SET_BIT(val, 1, LSb32IMAGERESWRAP_CTRL_one_shot_capture,
		bIMAGERESWRAP_CTRL_one_shot_capture);
	// Immediate Program
	CAM_HAL_WriteReg(ctx->dev, NULL, addr, val);
	SET_BIT(val, 0, LSb32IMAGERESWRAP_CTRL_one_shot_capture,
		bIMAGERESWRAP_CTRL_one_shot_capture);
	CAM_HAL_WriteReg(ctx->dev, NULL, addr, val);
	pr_info("Capture frame on CSIPipe[%d]\n", ctx->id);
}

void CSI_PIPE_Status(CSIPIPE_HANDLE handle)
{
	int i;
	struct isp_ctrl ctrl;
	CSI_PL_CTX_t *ctx = (CSI_PL_CTX_t *)handle;

	pr_info("Frame Count = %d\n", ctx->frame_cnt);
	pr_info("CSIPipe[%d] Interrupt Status:\n", ctx->id);
	for (i = 0; i < MAX_INTR; i++) {
		if (ctx->intr_cnt[i])
			pr_info("%s ID[%d] Count = %d\n", intr_name[i], i, ctx->intr_cnt[i]);
	}
	/* Print FVF Status */
	ctrl.id = CID_FVF_G_STATUS;
	ctrl.handler = ctx;
	fvf_s_ctrl(&ctrl);
}
