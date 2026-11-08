// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _SZC_MST_H_
#define _SZC_MST_H_

//  refer to DIAG: BerlinBG7\code\modules\secure_tzc\include\secure_szc.h
#define MST_ID_ACPU0      0
#define MST_ID_ACPU1      1
#define MST_ID_ACPU2      2
#define MST_ID_ACPU3      3
#define MST_ID_SCPU_DMA   6
#define MST_ID_SCPU_DIR   5
#define MST_ID_SCPU_CPU   4
#define MST_ID_ISP_ST0    8
#define MST_ID_ISP_ST1    9
#define MST_ID_ISP_BCM    10
#define MST_ID_NPU        11
#define MST_ID_IFCP       12
#define MST_ID_OVP        15
#define MST_ID_V4G        16
#define MST_ID_H1         17
#define MST_ID_AHBMON     25
#define MST_ID_TSP        28
#define MST_ID_BC         29
#define MST_ID_CORESIGHT  30
#define MST_ID_PB         31
#ifdef CONFIG_VS680_Z1
#define MST_ID_AVIO       7
#define MST_ID_GFX        18
#define MST_ID_GE         19
#define MST_ID_EMMC       20
#define MST_ID_SDIO       21
#define MST_ID_USB2       22
#define MST_ID_USB3       23
#define MST_ID_PCIE       24
#define MST_ID_MTEST      26
#define MST_ID_JTAG2AHB   27
#else
#define MST_ID_VIO        7
#define MST_ID_GFXDATA    18
#define MST_ID_GFXFW      19
#define MST_ID_AIO        20
#define MST_ID_HDCP       21
#define MST_ID_PERIF      24  //MTEST JTAG2HAB GE EMMC SDIO USB2 USB3 PCIE will be mapped to this ID before reaching SZC
#define MST_ID_GFXPM      26
#define MST_ID_GFXCODE    27
#endif
#define MST_ID_MAX        32

/* refer to MasterGroup in soc.h, these master IDs are for SZC only */
enum {
	RF_MST_ACPU0,
	RF_MST_ACPU1,
	RF_MST_ACPU2,
	RF_MST_ACPU3,
	RF_MST_SCPU_CPU,
	RF_MST_SCPU_DIR,
	RF_MST_SCPU_DMA,
	RF_MST_AVIO,
	RF_MST_ISP_STRM0,
	RF_MST_ISP_STRM1,
	RF_MST_ISP_BCM,
	RF_MST_NPU,
	RF_MST_IFCP,
	RF_MST_RFU0,
	RF_MST_RFU1,
	RF_MST_OVP,
	RF_MST_V4G,
	RF_MST_H1,
	RF_MST_GFX,
	RF_MST_GE,
	RF_MST_EMMC,
	RF_MST_SDIO,
	RF_MST_USB2,
	RF_MST_USB3,
	RF_MST_PCIE,
	RF_MST_AHBMON,
	RF_MST_MTEST,
	RF_MST_BSCAN2AHB,
	RF_MST_TSP,
	RF_MST_BC,
	RF_MST_CORESIGHT,
	RF_MST_PB,
	RF_MST_MAX,
};

enum {
	RF_GROUP0,
	RF_GROUP1,
	RF_GROUP2,
	RF_GROUP3,
	RF_GROUP4,
	RF_GROUP5,
	RF_GROUP6,
	RF_GROUP7,
	RF_GROUP8,
	RF_GROUP9,
	RF_GROUP10,
	RF_GROUP11,
	RF_GROUP12,
	RF_GROUP_MAX,
};
#endif /* _SZC_MST_H_ */
