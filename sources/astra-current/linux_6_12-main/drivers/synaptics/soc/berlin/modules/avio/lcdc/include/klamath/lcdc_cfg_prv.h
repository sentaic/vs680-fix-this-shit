// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 */
#ifndef lcdc_config_prv_h
#define lcdc_config_prv_h
#include "ctypes.h"
#include "vppGbl.h"
#include "avio.h"
#include "Galois_memmap.h"

//LCDC Frame end interrupts
#define LCDC2_FRAME_END_INTERRUPT BCMQMap_VPP_event5

// VPP/AIO - DHUB CG Disable
#define SYNA_LCDC_AIODHUB_CG_DISABLE(x)
#define SYNA_LCDC_VPPDHUB_CG_DISABLE(x)   \
do { \
      x.uCTRL_VPPDHUB_dyCG_en = 0; \
      x.uCTRL_VPPDHUB_CG_en = 0; \
} \
while (0)

/*
 * Klamath has only one LCDC instance (MIPI), mapped as LCDC2 in the driver
 * for compatibility with MYNA2 encoder mapping. Always return index 1.
 */
#define SYNA_LCDC_GET_DEV_NDX(crtc_id) (1)

// LCDC BCM Dhub handle
#define SYNA_LCDC_BCM_DHUB_HANDLE Dhub_GetDhub2dHandle_ByDhubId(DHUB_ID_VPP_DHUB)

#define SYNA_MEMMAP_AVIO_VPP_GBL_BASE           (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_GBL_REG_BASE)
#define SYNA_MEMMAP_AVIO_VPP_GBL_INTR_CTRL      (SYNA_MEMMAP_AVIO_VPP_GBL_BASE + RA_vppGbl_INTR_CTRL)
#define SYNA_MEMMAP_AVIO_VPP_GBL_LCDC_CTRL      (SYNA_MEMMAP_AVIO_VPP_GBL_BASE + RA_vppGbl_LCDC_CTRL)
#define SYNA_MEMMAP_AVIO_VPP_GBL_LCDC2_CTRL     (SYNA_MEMMAP_AVIO_VPP_GBL_BASE + RA_vppGbl_LCDC2_CTRL)
#define SYNA_MEMMAP_AVIO_VPP_GBL_AVPLLA_CLK_EN  (SYNA_MEMMAP_AVIO_VPP_GBL_BASE + RA_vppGbl_AVPLLA_CLK_EN)
#define SYNA_MEMMAP_AVIO_VPP_GBL_SWPDOWN_CTRL   (SYNA_MEMMAP_AVIO_VPP_GBL_BASE + RA_vppGbl_SWPDWN_CTRL)
#define SYNA_MEMMAP_AVIO_VPP_GBL_CTRL           (SYNA_MEMMAP_AVIO_VPP_GBL_BASE + RA_vppGbl_CTRL)

#define SYNA_MEMMAP_AVIO_VPP_GBL_DPHYTX (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_GBL_REG_BASE + RA_vppGbl_DPHYTX)
#define SYNA_MEMMAP_AVIO_VPP_GBL_MIPI (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_GBL_REG_BASE + RA_vppGbl_MEMMAP_MIPI)

//LCDC BASE address
#define SYNA_MEMMAP_AVIO_VPP_GBL_LCDC2_REG_BASE (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_LCDC2_REG_BASE)
#define SYNA_LCDC_GET_BASE_ADDRESS(x) (x ? SYNA_MEMMAP_AVIO_VPP_GBL_LCDC2_REG_BASE : NULL)

#define RA_avioVppGbl_VPLL0_WRAP      RA_vppGbl_VPLL0_WRAP
#define RA_avioVppGbl_VPPL1_WRAP      RA_vppGbl_VPLL1_WRAP

#define RA_avioVppGbl_LCDC2_CTRL      RA_vppGbl_LCDC2_CTRL

#define avioDhubChMap_vpp_BCM_R       avioDhubChMap_vpp128b_BCM_R

#define AVIO_GBL_BASE_ADDR      (MEMMAP_AVIO_REG_BASE+  AVIO_MEMMAP_AVIO_GBL_REG_BASE)
#define MIPI_SRAM_PWR_BASE_ADDR (AVIO_GBL_BASE_ADDR + RA_vppGbl_MIPI_SRAMPWR)
#define MIPI_SRAM_PWRCTRL_ADDR  (MIPI_SRAM_PWR_BASE_ADDR + RA_SRAMPWR_ctrl)

#define MIPI_CTRL_ADDR (AVIO_GBL_BASE_ADDR +  RA_vppGbl_MIPI_CTRL)

// Typedef added to maintain same datatypes in common files
typedef T32vppGbl_INTR_CTRL     T32avioVppGbl_INTR_CTRL;
typedef T32vppGbl_LCDC_CTRL     T32avioVppGbl_LCDC_CTRL;
typedef T32vppGbl_AVPLLA_CLK_EN T32avioVppGbl_AVPLLA_CLK_EN;
typedef T32vppGbl_SWPDWN_CTRL   T32avioVppGbl_SWPDWN_CTRL;
typedef T32vppGbl_CTRL          T32avioVppGbl_CTRL;

#endif
