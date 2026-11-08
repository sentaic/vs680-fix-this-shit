// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _CAM_BCMBUF_H_
#define _CAM_BCMBUF_H_

#include <linux/types.h>
#include "camera_isp_driver.h"

/* Type compatibility definitions */
typedef u32 UINT32;
typedef u64 UINT64;
typedef s32 INT32;
typedef int INT;

struct DHUB_CFGQ {
	UINT64 handle;
	INT32 shm_offset;
	UINT64 *addr;
	INT len;
	UINT64 *phy_addr;
};

/* structure of a buffer descriptor */
/* a buffer descriptor contains the pointers of the entire buffer and sub-buffers */
struct BCMBUF {
	UINT64 *head;
	UINT64 *tail;
	UINT64 *writer;
	int size;
	UINT64 handle;
	INT32 shm_offset;
	int subID;
	UINT64 *phy_addr;
};

/******* register programming buffer APIs **************/
/***************************************************************
 * FUNCTION: allocate register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 *       : size - size of the buffer to allocate
 *       :      - (should be a multiple of 4)
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
int CAM_BCMBUF_Create(struct BCMBUF *pbcmbuf, int size);

/***************************************************************
 * FUNCTION: free register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
int CAM_BCMBUF_Destroy(struct BCMBUF *pbcmbuf);

/***************************************************************
 * FUNCTION: reset a register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
int CAM_BCMBUF_Reset(struct BCMBUF *pbcmbuf);

/*********************************************************
 * FUNCTION: selest BCM sub-buffer to use
 * PARAMS: *buf - pointer to the buffer descriptor
 *         subID - DV_1, DV_2, DV_3
 ********************************************************/
void CAM_BCMBUF_Select(struct BCMBUF *pbcmbuf, int subID);

/*********************************************************
 * FUNCTION: write register address (4bytes) and value (4bytes) to the buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *               address - address of the register to be set
 *               value - the value to be written into the register
 * RETURN: 1 - succeed
 *               0 - register programming buffer is full
 ********************************************************/
int CAM_BCMBUF_Write(struct camera_isp_dev *isp_dev, struct BCMBUF *pbcmbuf,
		unsigned int address, unsigned int value);

/*********************************************************
 * FUNCTION: write a block of data to BCM buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *               *pdata - pointer to the data
 *               length - the length of the data to be written to BCM buffer
 * RETURN: 1 - succeed
 *               0 - register programming buffer is full
 ********************************************************/
 INT  CAM_BCMBUF_WriteBlock(struct BCMBUF *pbcmbuf, UINT64 *pdata, UINT32 length);

/*********************************************************************
 * FUNCTION: do the hardware transmission
 * PARAMS: block - 0: return without waiting for transaction finishing
 *                 1: return after waiting for transaction finishing
 ********************************************************************/
void CAM_BCMBUF_HardwareTrans(struct camera_isp_dev *isp_dev, struct BCMBUF *pbcmbuf, int block);

void CAM_BCMDHUB_HardwareTrans(struct camera_isp_dev *isp_dev, struct DHUB_CFGQ *cfgQ, int block);

/*********************************************************************
 * FUNCTION: do the hardware transaction
 * PARAMS: pdata - pointer to the data block
 *               length - data length for transaction
 ********************************************************************/
void CAM_BCMBUF_HardwareTransBlock(const unsigned int *pdata, unsigned int length, int block);

/*******************************************************************************
 * FUNCTION: commit cfgQ which contains BCM DHUB programming info to interrupt service routine
 * PARAMS: *cfgQ - cfgQ
 *         cpcbID - cpcb ID which this cmdQ belongs to
 *         intrType - interrupt type which this cmdQ belongs to:
 *                    0 - VBI, 1 - VDE
 * NOTE: this API is only called from VBI/VDE ISR.
 *******************************************************************************/
int CAM_BCMDHUB_CFGQ_Commit(struct camera_isp_dev *isp_dev, struct DHUB_CFGQ *cfgQ,
			   int cpcbID, int intrType);

/*******************************************************************************
 * FUNCTION: commit cfgQ which contains DMA dhub programming info to VBI interrupt service routine
 * PARAMS: *cfgQ - cfgQ
 *         cpcbID - cpcb ID which this cmdQ belongs to
 * NOTE: this API is only called from VBI/VDE ISR.
 *******************************************************************************/
int CAM_DMADHUB_CFGQ_Commit(struct camera_isp_dev *isp_dev, struct DHUB_CFGQ *cfgQ, int cpcbID);

/*********************************************************************
 * FUNCTION: send a BCM BUF info info to a BCM cfgQ
 * PARAMS: *pbcmbuf - pointer to the BCMBUF
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 ********************************************************************/
int CAM_BCMBUF_To_CFGQ(struct camera_isp_dev *isp_dev, struct BCMBUF *pbcmbuf,
			 struct DHUB_CFGQ *cfgQ);

/*********************************************************************
 * FUNCTION: send a raw BCM BUF info to a BCM cfgQ
 * PARAMS: pdata - pointer to the data block
 *         length - data length for transaction
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 ********************************************************************/
void CAM_BCMBUF_Raw_To_CFGQ(struct camera_isp_dev *isp_dev, const UINT32 *pdata,
			     UINT32 length, struct DHUB_CFGQ *cfgQ);

/*********************************************************************
 * FUNCTION: send a BCM cfgQ info to a BCM cfgQ
 * PARAMS: src_cfgQ - pointer to the source BCM cfgQ
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 ********************************************************************/
int CAM_CFGQ_To_CFGQ(struct camera_isp_dev *isp_dev, struct DHUB_CFGQ *src_cfgQ,
		    struct DHUB_CFGQ *cfgQ);

int CAM_BCMDHUB_AutoPush(struct camera_isp_dev *isp_dev, int intr, int enable);

void CAM_BCMBUF_Commit_To_CFGQ(struct camera_isp_dev *isp_dev, int intrType, UINT32 phy_start,
			     UINT32 size, struct DHUB_CFGQ *cfgQ);
INT CAM_CFGQ_Create(struct DHUB_CFGQ *pCfgQ, int size);
INT CAM_CFGQ_Destroy(struct DHUB_CFGQ *pCfgQ);
INT CAM_CFGQ_To_BCMBUF(struct DHUB_CFGQ *pCfgQ, struct BCMBUF *pbcmbuf);
INT CAM_BCMBUF_Raw_LogPrint(UINT64 *pdata, UINT32 length);
INT CAM_BCMBUF_To_Raw(struct BCMBUF *pbcmbuf, UINT64 **start, INT32 *size);
int CAM_BCMBUF_LogPrint(struct BCMBUF *pbcmbuf);
int CAM_CFGQ_LogPrint(struct DHUB_CFGQ *pCfgQ);

#endif

