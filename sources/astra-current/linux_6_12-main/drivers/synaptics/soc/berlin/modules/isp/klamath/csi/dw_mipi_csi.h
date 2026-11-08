/**
 * @file dw_mipi_csi.h
 * @brief MIPI CSI-2 controller driver
 *
 * Copyright (C) 2014 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */

#ifndef DW_MIPI_CSI_H_
#define DW_MIPI_CSI_H_

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/of_irq.h>
#include <linux/of_graph.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/string.h>
#include <linux/phy/phy.h>
#include <media/v4l2-device.h>
#include <media/v4l2-dv-timings.h>
#include <linux/videodev2.h>
#include <linux/io.h>

#include "snps_dphy_csi2.h"

#define CSI_DEVICE_NAME "dw-mipi-csi"

#define MAX_WIDTH   3280
#define MAX_HEIGHT  1852

#define MIN_WIDTH   640
#define MIN_HEIGHT  480

#define CSI_MAX_ENTITIES    2

enum mipi_csi_pads {
	CSI_PAD_SINK            = 0,
	CSI_PAD_SOURCE          = 1,
	CSI_PADS_NUM            = 2,
};

#define MAX_IPI_NUM 2

/** @short DWC MIPI CSI-2 register addresses*/
enum register_addresses {
	R_CSI2_VERSION = 0x00,
	R_CSI2_N_LANES = 0x04,
	R_CSI2_CTRL_RESETN = 0x08,
	R_CSI2_INTERRUPT = 0x0C,
	R_CSI2_DATA_IDS_1 = 0x10,
	R_CSI2_DATA_IDS_2 = 0x14,
	R_CSI2_INTERRUPT_AP = 0x2c,
	R_CSI2_DATA_IDS_VC_1 = 0x30,
	R_CSI2_DATA_IDS_VC_2 = 0x34,
	R_CSI2_PHY_SHUTDOWNZ = 0x40,
	R_CSI2_PHY_RSTZ  = 0x44,
	R_CSI2_IPI_MODE = 0x80,
	R_CSI2_IPI_VCID = 0x84,
	R_CSI2_IPI_DATA_TYPE = 0x88,
	R_CSI2_IPI_MEM_FLUSH = 0x8C,
	R_CSI2_IPI_HSA_TIME = 0x90,
	R_CSI2_IPI_HBP_TIME = 0x94,
	R_CSI2_IPI_HSD_TIME = 0x98,
	R_CSI2_IPI_HLINE_TIME = 0x9C,
	R_CSI2_IPI_SOFTRSTN = 0xA0,
	R_CSI2_IPI_ADV_FEATURES = 0xAC,
	R_CSI2_IPI_VSA_LINES = 0xB0,
	R_CSI2_IPI_VBP_LINES = 0xB4,
	R_CSI2_IPI_VFP_LINES = 0xB8,
	R_CSI2_IPI_VACTIVE_LINES = 0xBC,
	R_CSI2_VC_EXTENSION = 0xC8,
	R_CSI2_PHY_CAL = 0xCC,
	R_CSI2_INT_PHY_FATAL = 0xe0,
	R_CSI2_MASK_INT_PHY_FATAL = 0xe4,
	R_CSI2_FORCE_INT_PHY_FATAL = 0xe8,
	R_CSI2_INT_PKT_FATAL = 0xf0,
	R_CSI2_MASK_INT_PKT_FATAL = 0xf4,
	R_CSI2_FORCE_INT_PKT_FATAL = 0xf8,
	R_CSI2_INT_PHY = 0x110,
	R_CSI2_MASK_INT_PHY = 0x114,
	R_CSI2_FORCE_INT_PHY = 0x118,
	R_CSI2_INT_LINE = 0x130,
	R_CSI2_MASK_INT_LINE = 0x134,
	R_CSI2_FORCE_INT_LINE = 0x138,
	R_CSI2_INT_IPI = 0x140,
	R_CSI2_MASK_INT_IPI = 0x144,
	R_CSI2_FORCE_INT_IPI = 0x148,
	R_CSI2_IPI2_MODE = 0x200,
	R_CSI2_IPI2_VCID = 0x204,
	R_CSI2_IPI2_DATA_TYPE = 0x208,
	R_CSI2_IPI2_MEM_FLUSH = 0x20C,
	R_CSI2_IPI2_HSA_TIME = 0x210,
	R_CSI2_IPI2_HBP_TIME = 0x214,
	R_CSI2_IPI2_HSD_TIME = 0x218,
	R_CSI2_IPI2_ADV_FEATURES = 0x21C,
	R_CSI2_IPI3_MODE = 0x220,
	R_CSI2_IPI3_VCID = 0x224,
	R_CSI2_IPI3_DATA_TYPE = 0x228,
	R_CSI2_IPI3_MEM_FLUSH = 0x22C,
	R_CSI2_IPI3_HSA_TIME = 0x230,
	R_CSI2_IPI3_HBP_TIME = 0x234,
	R_CSI2_IPI3_HSD_TIME = 0x238,
	R_CSI2_IPI3_ADV_FEATURES = 0x23C,
	R_CSI2_IPI4_MODE = 0x240,
	R_CSI2_IPI4_VCID = 0x244,
	R_CSI2_IPI4_DATA_TYPE = 0x248,
	R_CSI2_IPI4_MEM_FLUSH = 0x24C,
	R_CSI2_IPI4_HSA_TIME = 0x250,
	R_CSI2_IPI4_HBP_TIME = 0x254,
	R_CSI2_IPI4_HSD_TIME = 0x258,
	R_CSI2_IPI4_ADV_FEATURES = 0x25C,
	R_CSI2_INT_BNDRY_FRAME_FATAL = 0x280,
	R_CSI2_INT_MASK_BNDRY_FRAME_FATAL = 0x284,
	R_CSI2_INT_FORCE_BNDRY_FRAME_FATAL = 0x288,
	R_CSI2_INT_SEQ_FRAME_FATAL = 0x290,
	R_CSI2_INT_MASK_SEQ_FRAME_FATAL = 0x294,
	R_CSI2_INT_FORCE_SEQ_FRAME_FATAL = 0x298,
	R_CSI2_INT_CRC_FRAME_FATAL = 0x2A0,
	R_CSI2_INT_MASK_CRC_FRAME_FATAL = 0x2A4,
	R_CSI2_INT_FORCE_CRC_FRAME_FATAL = 0x2A8,
	R_CSI2_INT_PLD_CRC_FATAL = 0x2B0,
	R_CSI2_INT_MASK_PLD_CRC_FATAL = 0x2B4,
	R_CSI2_INT_FORCE_PLD_CRC_FATAL = 0x2B8,
	R_CSI2_INT_DATA_ID = 0x2C0,
	R_CSI2_INT_MASK_DATA_ID = 0x2C4,
	R_CSI2_INT_FORCE_DATA_ID = 0x2C8,
	R_CSI2_INT_ECC_CORRECT = 0x2D0,
	R_CSI2_INT_MASK_ECC_CORRECT = 0x2D4,
	R_CSI2_INT_FORCE_ECC_CORRECT = 0x2D8,
	R_CSI2_SCRAMBLING = 0x300
};

/** @short IPI Data Types */
enum data_type {
	CSI_2_YUV420_8 = 0x18,
	CSI_2_YUV420_10 = 0x19,
	CSI_2_YUV420_8_LEG = 0x1A,
	CSI_2_YUV420_8_SHIFT = 0x1C,
	CSI_2_YUV420_10_SHIFT = 0x1D,
	CSI_2_YUV422_8 = 0x1E,
	CSI_2_YUV422_10 = 0x1F,
	CSI_2_RGB444 = 0x20,
	CSI_2_RGB555 = 0x21,
	CSI_2_RGB565 = 0x22,
	CSI_2_RGB666 = 0x23,
	CSI_2_RGB888 = 0x24,
	CSI_2_RAW6 = 0x28,
	CSI_2_RAW7 = 0x29,
	CSI_2_RAW8 = 0x2A,
	CSI_2_RAW10 = 0x2B,
	CSI_2_RAW12 = 0x2C,
	CSI_2_RAW14 = 0x2D,
	CSI_2_RAW16 = 0x2E,
};

/** @short Interrupt Masks */
enum interrupt_type {
	CSI2_INT_PHY_FATAL = 1 << 0,
	CSI2_INT_PKT_FATAL = 1 << 1,
	CSI2_INT_FRAME_FATAL = 1 << 2,
	CSI2_INT_SEQ_FRAME_FATAL = 1 << 3,
	CSI2_INT_CRC_FRAME_FATAL = 1 << 4,
	CSI2_INT_PLD_CRC_FATAL = 1 << 5,
	CSI2_INT_DATA_ID = 1 << 6,
	CSI2_INT_ECC_CORRECTED = 1 << 7,
	CSI2_INT_PHY = 1 << 16,
	CSI2_INT_LINE = 1 << 17,
	CSI2_INT_IPI = 1 << 18,
	CSI2_INT_IPI2 = 1 << 19,
	CSI2_INT_IPI3 = 1 << 20,
	CSI2_INT_IPI4 = 1 << 21,
};

/** @short DWC MIPI CSI-2 output types*/
enum output_type {
	DISABLED = 0,
	IPI_OUT = 1,
	IDI_OUT = 2,
	BOTH_OUT = 3
};

/** @short IPI output types*/
enum ipi_output_type {
	CAMERA_TIMING = 0,
	AUTO_TIMING = 1
};

/* IPI color components */
enum color_mode {
	COLOR48 = 0,
	COLOR16 = 1
};

/* IPI cut through */
enum cut_through {
	CTINACTIVE = 0,
	CTACTIVE = 1
};

/**
 * @short Format template
 */
struct mipi_fmt {
	const char *name;
	uint32_t code;
	u8 depth;
};

struct csi_hw {

	uint32_t num_lanes;
	uint32_t output_type;   //IPI = 0; IDI = 1; BOTH = 2
	/*IPI Info */

	uint32_t ipi_mode;
	uint32_t ipi_color_mode;
	uint32_t ipi_auto_flush;
	uint32_t ipi_cut_through_en;
	uint32_t virtual_ch;
	uint32_t data_type;
	uint32_t v4l2_data_type;

	uint32_t hsa;
	uint32_t hbp;
	uint32_t hsd;
	uint32_t htotal;
	uint32_t hactive;

	uint32_t vsa;
	uint32_t vbp;
	uint32_t vfp;
	uint32_t vactive;
};

/**
 * @short Structure to embed device driver information
 */
struct mipi_csi_dev {
	struct v4l2_subdev sd;
	struct video_device vdev;
	struct clk **csi_clks;

	struct mutex lock;
	spinlock_t slock;
	struct media_pad pads[CSI_PADS_NUM];
	struct platform_device *pdev;
	u8 index;

	/** Store current format */
	const struct mipi_fmt *fmt;
	struct v4l2_mbus_framefmt format;

	/** Device Tree Information */
	void __iomem *base_address;
	uint32_t ctrl_irq_number;

	struct csi_hw hw[2]; // currently 2 IPIs supported
	struct snps_dphy phy;
	struct v4l2_async_notifier notifier;
	struct device *dev;
};

void dw_mipi_csi_set_ipi_fmt(struct mipi_csi_dev *dev, int ipi, uint32_t code);
void dw_mipi_csi_intr_enable(struct mipi_csi_dev *dev, int en);
void dw_mipi_csi_configure(struct mipi_csi_dev *dev);

#endif              /* DW_MIPI_CSI */
