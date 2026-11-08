// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 */
 #ifndef __SYNA_LCDC_DRV_H_
 #define __SYNA_LCDC_DRV_H_

#include "avio_type.h"
#include "framequeue.h"
#include "bcmbuf.h"
#include "avioDhub.h"
#include "vpp_vbuf.h"
#include "syna_lcdc_reg.h"
#include "vpp_mem.h"
#include "avio_dhub_drv.h"

#define BCM_BUF_COUNT        0x2
#define BCM_BUFFER_SIZE        (4 * 1024)
#define DMA_CMD_BUFFER_SIZE    (200)

#define INT_FRAME_START 0x1
#define INT_FRAME_DONE  0x2
#define INT_UNDERRUN    0x4

#define LCDC_CMD_SIZE       0xA
#define DSI_CMD_MODE_POS    14
#define DSI_WAIT_TE_POS     15

#define LCDC_DSI_CMD_MODE    (1 << DSI_CMD_MODE_POS)
#define LCDC_WAIT_FOR_TE    (1 << DSI_WAIT_TE_POS)

#define CURR_VBI_BCM_BUF    (&dev->bcmbuf[dev->bufferCurSet].vbi_bcm_buf)
#define CURR_VBI_DMA_CFGQ   (&(dev->bcmbuf[dev->bufferCurSet].vbi_cfgQ[SYNA_DHUB_CFGQ_TYPE_DMA]))
#define CURR_VBI_BCM_CFGQ   (&(dev->bcmbuf[dev->bufferCurSet].vbi_cfgQ[SYNA_DHUB_CFGQ_TYPE_BCM]))

#define SYNA_MEMMAP_AVIO_GBL_BASE    SYNA_MEMMAP_AVIO_VPP_GBL_BASE

// VPP DHUB Handle
#define SYNA_LCDC_VPP_DHUB_HANDLE Dhub_GetDhub2dHandle_ByDhubId(DHUB_ID_VPP_DHUB)

//LCDC and MIPI interrupt enable
#define SYNA_LCDC1_INTR_EN(intr_reg)      intr_reg.uINTR_CTRL_lcdc1_int_en = 1
#define SYNA_LCDC2_INTR_EN(intr_reg)      intr_reg.uINTR_CTRL_lcdc2_int_en = 1
#define SYNA_MIPI_INTR_EN(intr_reg)	  intr_reg.uINTR_CTRL_mipi_int_en = 1

#define SYNA_LCDC_GAMMA_LUT_ENTRRIES 33
#define SYNA_LCDC_BRIGHTNESS_LUT_ENTRIES 3

typedef enum SYNA_LCDC_ERROR_t {
	SYNA_LCDC_OK            = 0x0000,   /**< Success. */
	SYNA_LCDC_EBADPARAM     = 0x0001,   /**< Function parameter error. */
	SYNA_LCDC_ENOMEM        = 0x0002,   /**< Not enough memory. */
	SYNA_LCDC_EUNSUPPORT    = 0x0003,   /**< plane not connected in configuration*/
	SYNA_LCDC_ECMDQFULL     = 0x0004,   /**< Command Queue is full */
	SYNA_LCDC_EFRAMEQFULL   = 0x0005,   /**< Frame Queue is full */
	SYNA_LCDC_EBCMBUFFULL   = 0x0006,   /**< BCM Buffer is full */
	SYNA_LCDC_EVBIBUFFULL   = 0x0007   /**< VBI Buffer is full */
} SYNA_LCDC_ERROR;

typedef enum _SYNA_DHUB_CFGQ_TYPE_ {
	SYNA_DHUB_CFGQ_TYPE_DMA = 0x0,
	SYNA_DHUB_CFGQ_TYPE_BCM = 0x1,
	SYNA_DHUB_CFGQ_TYPE_MAX = 0x2
} SYNA_DHUB_CFGQ_TYPE;

typedef enum _SYNA_LCDC_SETTING_ {
	SYNA_LCDC_GAMMA = 0x1,
	SYNA_LCDC_BRIGHTNESS = 0x2,
} SYNA_LCDC_SETTING;

typedef enum _SYNA_LCDC_BRIGHT_CH_ {
	SYNA_LCDC_BRIGHT_CH_R,
	SYNA_LCDC_BRIGHT_CH_G,
	SYNA_LCDC_BRIGHT_CH_B,
	SYNA_LCDC_BRIGHT_CH_ALL,
	SYNA_LCDC_BRIGHT_CH_MAX
} SYNA_LCDC_BRIGHT_CH;

typedef struct syna_lcdc_panel_t {
	unsigned char intf_type;     /*DPI TFT or CPU, DSI Video or CMD*/
	unsigned int bits_per_pixel; /*16, 18 or 24-bits per pixel*/
	unsigned char mode;      /*refer lcdc_output.xlsx*/
	unsigned char ext_te;    /*External tearing Effect*/
	unsigned int te_delay;
	unsigned char iclk;      /*lpclock polarity*/
	unsigned char rgb_swap;
	unsigned char rotation;      /*0, 90, 180, 270*/

	/*htotal = hsync_len + left_margin + xres + right_margin*/
	unsigned int hsync_len;
	unsigned int right_margin; //FP
	unsigned int xres;
	unsigned int left_margin;  //BP

	/*vtotal = vsync_len + upper_margin + yres + lower_margin*/
	unsigned int vsync_len;
	unsigned int lower_margin; //FP
	unsigned int yres;
	unsigned int upper_margin; //BP

	unsigned int refresh;
	/*htotal * vtotal * refresh*/
	unsigned int pixclock;

	/*htotal + hskip*/
	unsigned int hskip;
	/*vtotal + vskip*/
	unsigned int vskip;
} SYNA_LCDC_PANEL;

typedef struct _SYNA_BCMBUF_DATA_T_ {
	BCMBUF    vbi_bcm_buf;
	DHUB_CFGQ vbi_cfgQ[SYNA_DHUB_CFGQ_TYPE_MAX];   //O : DMA, 1: BCM
	VPP_MEM   vpp_mem_handle[SYNA_DHUB_CFGQ_TYPE_MAX];
} SYNA_BCMBUF_DATA;

struct syna_lcdc_dev {
	unsigned char lcdcID;     /*to identify the LCDC1 and LCDC2*/
	unsigned char bcm_enable; /* Use BCM engine or not for programming registers */
	unsigned char bcm_autopush_en;
	unsigned char en_intr_handler;
	unsigned int core_addr;
	struct syna_lcdc_panel_t *panel;

	VPP_VBUF *curr_frame; /*Slot for new frame*/
	VPP_VBUF *prev_frame;  /*Slot for current display frame*/
	int dmaRID;      /*DMA Read channel ID*/
	int dhubID;      /*DHUB channel handle*/
	int intrID;      /*Main interrupt*/
	int intrNo;      /*GIC interrupt number*/

	unsigned int interrupts;
	unsigned int underruns;
	unsigned int bufferCurSet;

	SYNA_BCMBUF_DATA bcmbuf[BCM_BUF_COUNT];

	FRAMEQUEUE inputq;     // input frame buffer queue of this plane
	FRAMEQUEUE outputq;    // recycle frame buffer queue of this plane

	bool     isTGConfig;
	unsigned int m_srcfmt;
	unsigned int m_order;
	unsigned int m_content_width;
	unsigned int m_content_height;
	unsigned int m_bits_per_pixel;
	unsigned char u8Gamma[SYNA_LCDC_GAMMA_LUT_ENTRRIES]; //GAMMA Table
	unsigned int update_flags;
	bool b_gamma_en;
	uint64_t brightness[SYNA_LCDC_BRIGHT_CH_MAX];

	VPP_MEM_LIST   *vpp_mem_list;
	int is_first_frame;
};

unsigned int syna_lcdc_read(struct syna_lcdc_dev *dev, unsigned long addr);
void syna_lcdc_write(struct syna_lcdc_dev *dev, unsigned long addr, unsigned int val);
int syna_lcdc_pushframe(int planeID, void *pnew);
int syna_lcdc_dlr_create(struct syna_lcdc_dev *dev, int num);
void syna_lcdc_dlr_handler(struct syna_lcdc_dev *dev);
void syna_bcmbuf_flip(struct syna_lcdc_dev *dev);
void syna_bcmbuf_submit(struct syna_lcdc_dev *dev, int use_vbi);
void syna_lcdc_irq(int intrMask);
int syna_lcdc_waitVsync(int lcdcId);
void syna_lcdc_releaseVsync(int lcdcId);
void syna_lcdc_hw_config(int lcdcID, SYNA_LCDC_PANEL *panel);
void syna_lcdc_dlr_destroy(struct syna_lcdc_dev *dev);
int syna_lcdc_suspend (int enable);
void syna_lcdc_cfg_setbitmap(struct syna_lcdc_dev *dev, int src_fmt, int order);
void syna_lcdc_cfg_dlr_fifoflush(struct syna_lcdc_dev *dev);
void syna_lcdc_cfg_dlr_init(struct syna_lcdc_dev *dev, int num);
void syna_lcdc_cfg_wrap_interrupt_enable(void);
int syna_lcdc_update_gamma_table(int lcdcID, const void *data,
				 unsigned int length);
int syna_lcdc_update_brightness(int lcdcID, int channel, uint64_t val);
int syna_lcdc_get_brightness(int lcdcID, int channel, uint64_t *val);
#endif
