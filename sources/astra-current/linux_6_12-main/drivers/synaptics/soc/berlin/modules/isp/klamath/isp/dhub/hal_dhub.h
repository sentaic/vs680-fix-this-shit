/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __HAL_DHUB_H__
#define __HAL_DHUB_H__

#include "cam_bcmbuf.h"
#include "api_dhub.h"
#include "camera_isp_driver.h"

void dhub2nd_channel_clear_seq(void *hdl, SIGN32 id);
void dhub2nd_channel_start_seq(void *hdl, SIGN32 id);
void dhub_channel_clear_bcmbuf(void *hdl, SIGN32 id, struct BCMBUF *pbcmbuf);
void hbo_queue_clear_bcmbuf(void *hdl, SIGN32 id, struct BCMBUF *pbcmbuf);
void dhub2d_channel_clear_bcmbuf(void *hdl, SIGN32 id, struct BCMBUF *pbcmbuf);
void dhub2d_channel_clear_seq_bcm(void *hdl, SIGN32 id, struct BCMBUF *pbcmbuf);
void dhub_channel_clear_seq(void *hdl, SIGN32 id, struct BCMBUF *pbcmbuf);
void dhub2nd_channel_clear_seq_bcm(void *hdl, SIGN32 id, struct BCMBUF *pbcmbuf);
int dhub_wait_complete(int ch, struct HDL_semaphore *pSemHandle, int timeout);
void dhub2nd_channel_clear(void *hdl, SIGN32 id);
UNSG32  dhub_channel_axqos_isp(void *hdl, SIGN32 id, UNSG32 awQosLO,
		UNSG32 awQosHI, UNSG32 arQosLO, UNSG32 arQosHI, T64b cfgQ[]);
UNSG32 dhub2nd_channel_enable(void *hdl, SIGN32 id,
		SIGN32 enable, T64b cfgQ[]);
UNSG32 dhub_channel_enable_InverseScan_vppBcm(void *hdl, SIGN32 id,
		SIGN32 iMode, UNSG32 pbcmbuf);
UNSG32 dhub_channel_enable_bcmbuf(void *hdl, SIGN32 id,
		SIGN32 enable, struct BCMBUF *pbcmbuf);
UNSG32 hbo_queue_enable_bcmbuf(void *hdl, SIGN32 id,
		SIGN32 enable, struct BCMBUF *pbcmbuf);
void dhub2d_channel_enable_bcmbuf(void *hdl, SIGN32 id,
		SIGN32 enable, struct BCMBUF *pbcmbuf);
void dhub2nd_channel_clear_bcmbuf(void  *hdl, SIGN32 id,
		struct BCMBUF *pbcmbuf);

/* BCM scheduler function prototypes */
void BCM_SCHED_Open(void);
void BCM_SCHED_Close(void);
void BCM_SCHED_GetEmptySts(struct camera_isp_dev *isp_dev, UNSG32 QID, UNSG32 *EmptySts);
void BCM_SCHED_GetFullSts(struct camera_isp_dev *isp_dev, UNSG32 QID, UNSG32 *FullSts);
void BCM_SCHED_Flush(struct camera_isp_dev *isp_dev, UNSG32 mask);
void BCM_SCHED_QueueFlush(struct camera_isp_dev *isp_dev, UNSG32 QID);
int BCM_SCHED_PushCmd(struct camera_isp_dev *isp_dev, UNSG32 QID, UNSG32 *pCmd, UINT64 *cfgQ);
int BCM_SCHED_AutoPushCmd(struct camera_isp_dev *isp_dev, UNSG32 QID, UNSG8 uchEnable);
void BCM_SchedSetMux(struct camera_isp_dev *isp_dev, UNSG32 QID, UNSG32 TrigEvent);

/* BCM buffer function prototypes */
void bcm_buffer_write(void *pBcmBuf, UNSG32 regAddr, UNSG32 regVal);
int bcm_buffer_dummy_write(void *pBcmBuf, UNSG32 len);
int raw_bcm_buffer_dummy_write(UINT64 *rawBcmBufferBase, UNSG32 len);

/* Additional missing function prototypes */
void *isp_dhub_semaphore(void *hdl);
UNSG32 isp_semaphore_chk_full(void *hdl, SIGN32 id);
UNSG32 isp_dhub2nd_channel_cfg(void *hdl, SIGN32 id, UNSG32 addr, SIGN32 burst, SIGN32 step1, SIGN32 size1, SIGN32 step2,
		SIGN32 size2, SIGN32 chkSemId, SIGN32 updSemId, SIGN32 interrupt, SIGN32 enable, T64b cfgQ[]);
void ModInc(UNSG32 *p, UNSG32 inc, UNSG32 depth);
void dhub_channel_generate_cmd(void *hdl, SIGN32 id, UINT64 addr, SIGN32 size, SIGN32 semOnMTU, SIGN32 chkSemId,
		SIGN32 updSemId, SIGN32 interrupt, SIGN32 *pData);

#endif //__HAL_DHUB_H__
