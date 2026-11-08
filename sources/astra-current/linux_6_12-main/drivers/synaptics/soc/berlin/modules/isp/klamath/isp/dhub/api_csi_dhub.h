/* SPDX-License-Identifier: GPL-2.0  */
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __API_AVIO_DHUB_H__
#define __API_AVIO_DHUB_H__
#include "klamath_memmap.h"
#include "api_dhub.h"

/* KLAMATH_KERNEL: Local constants extracted from removed avioDhub.h */
#define avioDhubTcmMap_vip128bDhub_BANK0_START_ADDR 0x0
#define avioDhubTcmMap_vip128bDhub_BANK0_SIZE       0x2800
#define avioDhubTcmMap_vip128bDhub_BANK1_START_ADDR 0x2800
#define avioDhubTcmMap_vip128bDhub_BANK1_SIZE       0x2800

/* KLAMATH_KERNEL: Register address constants */
#define RA_vip128bDhub_dHub0                        0x8000
#define RA_vip128bDhub_tcm0                         0x0000

/* KLAMATH_KERNEL: Channel mapping constants */
#define avioDhubChMap_vip128b_IPI0Y_W               0x0
#define avioDhubChMap_vip128b_IPI0C_W               0x1
#define avioDhubChMap_vip128b_IPI1Y_W               0x2
#define avioDhubChMap_vip128b_IPI1C_W               0x3
#define avioDhubChMap_vip128b_BCM_R                 0x4

/* KLAMATH_KERNEL: MTU size constants */
#define dHubChannel_CFG_MTU_8byte                   0x0
#define dHubChannel_CFG_MTU_16byte                  0x1
#define dHubChannel_CFG_MTU_32byte                  0x2
#define dHubChannel_CFG_MTU_64byte                  0x3
#define dHubChannel_CFG_MTU_128byte                 0x4
#define dHubChannel_CFG_MTU_256byte                 0x5
#define dHubChannel_CFG_MTU_512byte                 0x6
#define dHubChannel_CFG_MTU_1024byte                0x7
#define dHubChannel_CFG_MTU_2048byte                0x8
#define dHubChannel_CFG_MTU_4096byte                0x9

/* KLAMATH_KERNEL: T32dHubChannel_CFG type definition moved to api_dhub.h */

typedef int (*CSIPIPE_DHUB_INTR_HANDLER)(UNSG32 intrNum, void *pArgs);

#define CSI_NUM_OF_CHANNELS     (5)

#define CSI_DHUB_BASE           (RA_vip128bDhub_dHub0)
#define CSI_HBO_SRAM_BASE       (RA_vip128bDhub_tcm0)

#define CSI_DHUB_BANK0_START_ADDR      avioDhubTcmMap_vip128bDhub_BANK0_START_ADDR
#define CSI_IPIY_DHUB_SIZE             (0x1400)
#define CSI_IPIC_DHUB_SIZE             (0x1200)
#define CSI_BCM_DHUB_SIZE              (avioDhubTcmMap_vip128bDhub_BANK0_SIZE \
			+ avioDhubTcmMap_vip128bDhub_BANK1_SIZE \
			- ((CSI_IPIY_DHUB_SIZE + CSI_IPIC_DHUB_SIZE) * 2))

#define AVIO_DHUB_DESC_OVRDQOS       1
#define AVIO_DHUB_DESC_QOSSEL        1

#define AVIO_DHUB_INITCHANNELAXQOS(pdhubHandle, dhub_config, chanId, i, cfgQ) \
	dhub_channel_axqos(                 \
			&pdhubHandle->dhub,         \
			chanId,                     \
			dhub_config[i].chanAxQosLO, \
			dhub_config[i].chanAxQosHI, \
			dhub_config[i].chanAxQosLO, \
			dhub_config[i].chanAxQosHI, \
			cfgQ                        \
			)

typedef struct DHUB_channel_config {
	SIGN32 chanId;
	UNSG32 chanCmdBase;
	UNSG32 chanDataBase;
	SIGN32 chanCmdSize;
	SIGN32 chanDataSize;
	SIGN32 chanMtuSize;
	SIGN32 chanQos;
	SIGN32 chanSelfLoop;
	SIGN32 chanEnable;
	UNSG32 chanAxQosLO;
	UNSG32 chanAxQosHI;
} DHUB_channel_config;

typedef enum {
	DHUB_TYPE_64BIT = 0,
	DHUB_TYPE_128BIT
} DHUB_TYPE;

extern HDL_dhub2d CSI_dhubHandle;
extern DHUB_channel_config  CSI_config[];

int getCsiDhubChannelInfo(HDL_dhub2d *pdhubHandle, SIGN32 IChannel, T32dHubChannel_CFG *cfg);
UNSG32  dhub_channel_axqos(
	void        *hdl,               /*! Handle to HDL_dhub !*/
	SIGN32      id,                 /*! Channel ID in $dHubReg !*/
	UNSG32      awQosLO,            /*! AWQOS value when low priority !*/
	UNSG32      awQosHI,            /*! AWQOS value when high priority !*/
	UNSG32      arQosLO,            /*! ARQOS value when low priority !*/
	UNSG32      arQosHI,            /*! ARQOS value when high priority !*/
	T64b        cfgQ[]              /*!
					 * Pass NULL to directly init dHub, or
					 * Pass non-zero to receive programming sequence
					 * in (adr,data) pairs
					 */
	);

int csi_dhub_get_channel_info(HDL_dhub2d *dhub_handle, int channel_id, T32dHubChannel_CFG *cfg);
void csi_dhub_channel_clear(void *hdl, int channel_id, T64b cfg_queue[]);
void csi_dhub_channel_flush(void *hdl, int channel_id, T64b cfg_queue[]);
int csi_dhub_init(struct camera_isp_dev *isp_dev, int cpu_id);
void csi_dhub_exit(int cpu_id);

#endif //__API_AVIO_DHUB_H__
