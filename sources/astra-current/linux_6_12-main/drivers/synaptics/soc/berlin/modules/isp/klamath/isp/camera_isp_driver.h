/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2021 - 2023 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __CAMERA_ISP_DRIVER_H__
#define __CAMERA_ISP_DRIVER_H__

#include <linux/list.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-mc.h>
#include <media/v4l2-ctrls.h>
#include <media/videobuf2-dma-contig.h>
#include <linux/wait.h>
#include <linux/spinlock.h>

#include "wb.h"

#define MAX_PL 2
#define BCM_ENABLE

typedef void *CSIPIPE_HANDLE;

/* Interrupt queue between ISR and process thread */
#define ISP_INTR_Q_SIZE 64

/* regmap is done with 0xF7458000, we just need these offsets to read/write regs */
#define CSIPIPE_OFFSET    0
#define VIP_GBL_OFFSET    0x8000
#define VIP_BCMQ_OFFSET   0xC000

#define HAL_ISP_CORE_REG_WRITE32(dev, addr, val) \
	do { \
		uint32_t _readback_val; \
		void __iomem *_full_addr = dev->core_base_addr + addr; \
		writel(val, _full_addr); \
		_readback_val = readl(_full_addr); \
		if ((_readback_val != (val)) && (_readback_val != 0)) { \
			pr_info("addr=0x%px, written=0x%lX, read=0x%X\n", \
				_full_addr, (unsigned long)val, _readback_val); \
		} \
		pr_debug("ISPWC 0X%lX = 0x%lx\n", \
			(unsigned long)addr, (unsigned long)val); \
	} while (0)
#define HAL_ISP_CORE_REG_READ32(dev, addr)     readl(dev->core_base_addr + addr)

/* mapped address is set in the 'ra' value, hence no need of passing the dhub base address again*/
#define HAL_ISP_DHUB_REG_WRITE32(addr, val) \
	do { \
		writel(val, (void *)addr); \
		pr_debug("ISPWD 0X%lX = 0x%lx\n", \
			(unsigned long)addr, (unsigned long)val); \
	} while (0)

#define HAL_ISP_DHUB_REG_READ32(addr, val) \
	do { \
		(val) = readl((void *)addr); \
		pr_debug("ISPRD 0X%lX = 0x%lx\n", \
			(unsigned long)addr, (unsigned long)val); \
	} while (0)

#define CAM_HAL_WriteReg            CAM_BCMBUF_Write
#define CAM_HAL_ReadReg(dev, addr)       HAL_ISP_CORE_REG_READ32(dev, addr)

#define GET_BIT_MASK(N_BIT)     ((1<<N_BIT) - 1)
#define SET_BIT(VARIABLE, VALUE, BIT_POS, N_BIT) \
	(VARIABLE = (VARIABLE & (~(GET_BIT_MASK(N_BIT) << BIT_POS))) | \
		((VALUE & GET_BIT_MASK(N_BIT)) << BIT_POS))

#define CAMERA_ISP_NAME "camera-isp-subdev"

/* ISP dimension constraints */
#define CAMERA_ISP_WIDTH_ALIGN  16
#define CAMERA_ISP_HEIGHT_ALIGN 8
#define CAMERA_ISP_WIDTH_MIN    32
#define CAMERA_ISP_HEIGHT_MIN   16
#define CAMERA_ISP_WIDTH_MAX    4096
#define CAMERA_ISP_HEIGHT_MAX   3072

/* Default format dimensions */
#define CAMERA_ISP_DEFAULT_WIDTH  1920
#define CAMERA_ISP_DEFAULT_HEIGHT 1080

/* Index of the first resolution in a cached_modes row */
#define CAMERA_ISP_FIRST_RES_IDX  0

/* ISP pad definitions */
enum camera_isp_pad_id {
	CAMERA_ISP_PAD_SINK = 0,            /* Input from sensor */
	CAMERA_ISP_PAD_SOURCE_PATH0,        /* Path 0 output to video device 0 */
	CAMERA_ISP_PAD_SOURCE_PATH1,        /* Path 1 output to video device 1 */
	CAMERA_ISP_PAD_NR,
};

/* Sensor mode structure */
struct sensor_mode {
	u32 width;
	u32 height;
	u32 code;
};

/* Media bus format structure */
struct camera_isp_mbus_fmt {
	uint32_t code;
};

struct camera_isp_pad_data {
	struct v4l2_rect r;
	u32  target;
	bool is_cropping_enable;
};

/* ISP device structure */
struct camera_isp_dev {
	struct device *dev;
	struct v4l2_subdev sd;
	struct media_pad pads[CAMERA_ISP_PAD_NR];
	uint32_t id;
	struct clk **isp_clks;

	/* Format information for each- pad */
	struct v4l2_mbus_framefmt formats[CAMERA_ISP_PAD_NR];
	struct camera_isp_pad_data pad_data[CAMERA_ISP_PAD_NR];

	/* Runtime state */
	uint8_t streaming;
	int active_pipe_id;
	struct mutex lock;
	struct v4l2_async_notifier notifier;
	struct v4l2_ctrl_handler ctrl_handler;

	void __iomem *core_base_addr;
	void __iomem *dhub_base_addr;
	CSIPIPE_HANDLE pipe[MAX_PL];
	bool pipeline_ready[MAX_PL];
	WB_CONFIG_t wb_config;
	void *intr_handle;
	int irq_num;

	/* Interrupt handling infrastructure */
	wait_queue_head_t wq;
	spinlock_t isr_lock;
	int pending_intr_count;
	struct task_struct *intr_thread;

	/* ring buffer for intr numbers */
	unsigned int intr_q_head;
	unsigned int intr_q_tail;
	unsigned int intr_q[ISP_INTR_Q_SIZE];

	/* Cached sensor modes: 2D array [code_idx][res_idx],
	 * allocated in enumerate_sensor_modes()
	 */
	struct sensor_mode **cached_modes;
	int *num_resolutions;
	int num_cached_codes;

	/* Scaling information from resolution selection */
	u32 scale_factor;

	bool bypass_isp_enabled;

	/* DT caps for sensor modes (0 disables the cap) */
	u32 max_sensor_width;
	u32 max_sensor_height;
};

/* IOCTL definitions */
#define CAMERA_ISP_IOC_QUERYCAP     _IOR('V', 0, struct v4l2_capability)

/* Function declarations */
extern struct camera_isp_mbus_fmt camera_isp_mp_fmts[];
extern struct camera_isp_mbus_fmt camera_isp_sp_fmts[];

#endif /* __CAMERA_ISP_DRIVER_H__ */
