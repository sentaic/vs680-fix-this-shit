// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/printk.h>
#include <asm/string.h>

#include "avio.h"
#include "api_dhub.h"
#include "isp_shm.h"
#include "camera_isp_driver.h"
#include "isp_err.h"
#include "klamath_memmap.h"
#include "cam_bcmbuf.h"
#include "hal_dhub.h"

#define UNUSED(x)                ((void)(x))

extern HDL_dhub2d CSI_dhubHandle;

/***************************************************************
 * FUNCTION: allocate register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 *       : size - size of the buffer to allocate
 *       :      - (should be a multiple of 4)
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
INT  CAM_BCMBUF_Create(struct BCMBUF *pbcmbuf, int size)
{
	if (size <= 0)
		return (ISP_EBADPARAM);

	/* allocate memory for the buffer */
	if (isp_shm_allocate(SHM_NONSECURE, size, 32, &pbcmbuf->handle,
		SHM_NONSECURE_CONTIG) != ISP_OK)
		return ISP_ENOMEM;

	pbcmbuf->size = size;
	isp_shm_get_virtual_address(pbcmbuf->handle, 0, (void *)&pbcmbuf->head);
	isp_shm_get_physical_address(pbcmbuf->handle, 0, (void *)&pbcmbuf->phy_addr);
	pr_debug("bcmbuf addr: 0x%lx phy_addr: 0x%lx\n",
       (unsigned long)pbcmbuf->head, (unsigned long)pbcmbuf->phy_addr);

	return ISP_OK;
}

/***************************************************************
 * FUNCTION: free register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
INT  CAM_BCMBUF_Destroy(struct BCMBUF *pbcmbuf)
{
	/* allocate memory for the buffer */
	if (!pbcmbuf->head)
		return (ISP_EBADCALL);

	isp_shm_release(pbcmbuf->handle);

	pbcmbuf->head = NULL;

	return ISP_OK;
}


/***************************************************************
 * FUNCTION: reset a register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
INT  CAM_BCMBUF_Reset(struct BCMBUF *pbcmbuf)
{

	/* TODO This looks incorrect, pointer arithmetic */
	pbcmbuf->tail = pbcmbuf->head + pbcmbuf->size;

	/*set pointers to the head*/
	pbcmbuf->writer = pbcmbuf->head;
	pbcmbuf->subID = -1; /* total */

	return ISP_OK;
}

/*********************************************************
 * FUNCTION: Select sub register programming buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *         subID - CPCB_1, CPCB_2, CPCB_3 or total
 ********************************************************/
void  CAM_BCMBUF_Select(struct BCMBUF *pbcmbuf, INT subID)
{
	/* reset read/write pointer of the buffer */
	pbcmbuf->writer = pbcmbuf->head;
	pbcmbuf->subID = subID;
	return;
}

/*********************************************************
 * FUNCTION: write register address (4 bytes) and value (4 bytes) to the buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *               address - address of the register to be set
 *               value - the value to be written into the register
 * RETURN: 1 - succeed
 *               0 - register programming buffer is full
 ********************************************************/
INT CAM_BCMBUF_Write(struct camera_isp_dev *isp_dev, struct BCMBUF *pbcmbuf,
		   UINT32 address, UINT32 value)
{
	UINT64 *end;
	UINT64 val = 0;

	if (!pbcmbuf){
		HAL_ISP_CORE_REG_WRITE32(isp_dev, address, value);
		return ISP_OK;
	}

	/*if not enough space for storing another 8 bytes, wrap around happens*/
	end = pbcmbuf->tail;

	if (pbcmbuf->writer == end){
		/*the buffer is full, no space for wrap around*/
		pr_info("BCMBUF_Write failed (0x%x 0x%x)\r\n", address, value);
		return ISP_EBCMBUFFULL;
	}

	val = address + START_AVIO_CSIPIPE;
	val = (val << 32) | (value);
	*pbcmbuf->writer = val;
	pbcmbuf->writer++;

	return ISP_OK;
}

/*********************************************************
 * FUNCTION: write a block of data to BCM buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *               *pdata - pointer to the data
 *               length - the length of the data to be written to BCM buffer
 * RETURN: 1 - succeed
 *               0 - register programming buffer is full
 ********************************************************/
INT  CAM_BCMBUF_WriteBlock(struct BCMBUF *pbcmbuf, UINT64 *pdata, UINT32 length)
{
	UINT64 *end;

	end = pbcmbuf->tail;

	if (pbcmbuf->writer > end - (length >> 3)) {
		/*the buffer is full*/
		pr_err("%s BCMBUF is full!!\n", __func__);
		return ISP_EBCMBUFFULL;
	}

	/*save the data to BCM buffer*/
	memcpy(pbcmbuf->writer, pdata, length);

	pbcmbuf->writer += (length >> 3);

	return ISP_OK;
}

/*********************************************************************
 * FUNCTION: do the hardware transaction
 * PARAMS: *buf - pointer to the buffer descriptor
 ********************************************************************/
void  CAM_BCMBUF_HardwareTrans(struct camera_isp_dev *isp_dev, struct BCMBUF *pbcmbuf, INT block)
{
	HDL_semaphore *pSemHandle;
	HDL_dhub2d *pDhubHandle;
	UINT64 *start;
	int status;
	int dhubID, size;
	INT32 shm_offset;
	unsigned int bcm_sched_cmd[2];

	start = pbcmbuf->head;
	shm_offset = 0;

	size = pbcmbuf->writer-start;

	pr_info("BCMBUF_HardwareTrans (buf: 0x%lx start: 0x%lx size: 0x%x)\r\n",
        (unsigned long)pbcmbuf, (unsigned long)start, size);
	if (size <= 0)
		return;

	shm_offset = shm_offset*4;
	/* flush data in D$ */
	isp_shm_clean_cache(pbcmbuf->handle, shm_offset, size);
	/* get non-cache physical address for DMA */
	isp_shm_get_physical_address(pbcmbuf->handle, shm_offset, (void *)start);

	/* start BCM engine */
	dhubID = avioDhubChMap_vip128b_BCM_R;
	pDhubHandle = &CSI_dhubHandle;

	pSemHandle = isp_dhub_semaphore(&(pDhubHandle->dhub));
	if (block) {
		/* clear possible BCM previous interrupt */
		status = isp_semaphore_chk_full(pSemHandle, dhubID);
		if (status) {
			semaphore_pop(pSemHandle, dhubID, 1);
			semaphore_clr_full(pSemHandle, dhubID);
		}
	}

	/* clear BCM interrupt */
	status = isp_semaphore_chk_full(pSemHandle, dhubID);
	while (status) {
		semaphore_pop(pSemHandle, dhubID, 1);
		semaphore_clr_full(pSemHandle, dhubID);
		status = isp_semaphore_chk_full(pSemHandle, dhubID);
	}

	dhub_channel_generate_cmd(&(pDhubHandle->dhub), dhubID, (UINT64)start, (INT)size,
				0, 0, 0, 1, (SIGN32 *)bcm_sched_cmd);
	while (!BCM_SCHED_PushCmd(isp_dev, BCM_SCHED_Q12, bcm_sched_cmd, NULL))
		;

	if (block){
		/* check BCM interrupt */
		pSemHandle = isp_dhub_semaphore(&(pDhubHandle->dhub));
		status = isp_semaphore_chk_full(pSemHandle, dhubID);
		while (!status) {
			status = isp_semaphore_chk_full(pSemHandle, dhubID);
		}

		/* clear BCM interrupt */
		semaphore_pop(pSemHandle, dhubID, 1);
		semaphore_clr_full(pSemHandle, dhubID);
	}
	return;
}

/*********************************************************************
 * FUNCTION: send a BCM BUF info to a BCM cfgQ
 * PARAMS: *pbcmbuf - pointer to the struct BCMBUF
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 ********************************************************************/
int CAM_BCMBUF_To_CFGQ(struct camera_isp_dev *isp_dev, struct BCMBUF *pbcmbuf,
			struct DHUB_CFGQ *cfgQ)
{
	UINT64 *start,*phy_start;
	INT32 size, shm_offset;
	unsigned int bcm_sched_cmd[2];
	int index;

	start = pbcmbuf->head;
	shm_offset = 0;
	phy_start = pbcmbuf->phy_addr;

	size = (UINT64)pbcmbuf->writer - (UINT64)start;

	if (size <= 0)
		return ISP_EBADPARAM;
	shm_offset = shm_offset*4;
	isp_shm_clean_cache(pbcmbuf->handle, shm_offset,  size);

	dhub_channel_generate_cmd(&(CSI_dhubHandle.dhub),
			avioDhubChMap_vip128b_BCM_R, (INT)(UINT64)phy_start,
			(INT)size, 0, 0, 0, 1, (SIGN32 *)bcm_sched_cmd);

	while (!(index = BCM_SCHED_PushCmd(isp_dev, BCM_SCHED_Q13,
					bcm_sched_cmd, (cfgQ->addr + cfgQ->len))))
		usleep_range(2, 5);

	cfgQ->len += index;
	return ISP_OK;
}

/*********************************************************************
 * FUNCTION: send a raw BCM BUF info to a BCM cfgQ
 * PARAMS: pdata - pointer to the data block
 *         length - data length for transaction
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 ********************************************************************/
void CAM_BCMBUF_Raw_To_CFGQ(struct camera_isp_dev *isp_dev, const UINT32 *pdata,
			UINT32 length, struct DHUB_CFGQ *cfgQ)
{
	unsigned int bcm_sched_cmd[2];

	dhub_channel_generate_cmd(&(CSI_dhubHandle.dhub), avioDhubChMap_vip128b_BCM_R,
		(UINT64)pdata, (INT)length, 0, 0, 0, 1, (SIGN32 *)bcm_sched_cmd);
	while (!BCM_SCHED_PushCmd(isp_dev, BCM_SCHED_Q13, bcm_sched_cmd,
			(cfgQ->addr + cfgQ->len * 2)))
		;
	cfgQ->len += 2;
}

/*********************************************************************
 * FUNCTION: send a BCM cfgQ info to a BCM cfgQ
 * PARAMS: src_cfgQ - pointer to the source BCM cfgQ
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 ********************************************************************/
int CAM_CFGQ_To_CFGQ(struct camera_isp_dev *isp_dev, struct DHUB_CFGQ *src_cfgQ,
			struct DHUB_CFGQ *cfgQ)
{
	unsigned int bcm_sched_cmd[2];

	if (src_cfgQ->len <= 0)
		return ISP_EBADPARAM;
	isp_shm_clean_cache(src_cfgQ->handle, 0, src_cfgQ->len*8);

	dhub_channel_generate_cmd(&(CSI_dhubHandle.dhub), avioDhubChMap_vip128b_BCM_R,
			     (UINT64)src_cfgQ->phy_addr, (INT)src_cfgQ->len * 8,
			     0, 0, 0, 1, (SIGN32 *)bcm_sched_cmd);
	while (!BCM_SCHED_PushCmd(isp_dev, BCM_SCHED_Q13, bcm_sched_cmd,
			(cfgQ->addr + cfgQ->len * 2)))
		;
	cfgQ->len += 2;

	return 0;
}

/*******************************************************************************
 * FUNCTION: commit cfgQ which contains BCM DHUB programming info to interrupt service routine
 * PARAMS: *cfgQ - cfgQ
 *         cpcbID - cpcb ID which this cmdQ belongs to
 *         intrType - interrupt type which this cmdQ belongs to: 0 - VBI, 1 - VDE
 * NOTE: this API is only called from VBI/VDE ISR.
 *******************************************************************************/
int CAM_BCMDHUB_CFGQ_Commit(struct camera_isp_dev *isp_dev, struct DHUB_CFGQ *cfgQ,
			int cpcbID, int intrType)
{
	unsigned int sched_qid;
	unsigned int bcm_sched_cmd[2];

	if (cfgQ->len <= 0)
		return ISP_EBADPARAM;

	if (cpcbID == 0) {
		  sched_qid = BCM_SCHED_Q0;
	} else {
		  sched_qid = BCM_SCHED_Q1;
	}
	isp_shm_clean_cache(cfgQ->handle, 0, cfgQ->len*8);

	dhub_channel_generate_cmd(&(CSI_dhubHandle.dhub), avioDhubChMap_vip128b_BCM_R,
			     (UINT64)cfgQ->phy_addr, (INT)cfgQ->len*8,
			     0, 0, 0, 1, (SIGN32 *)bcm_sched_cmd);
	while( !BCM_SCHED_PushCmd(isp_dev, sched_qid, bcm_sched_cmd, NULL));

	return ISP_OK;
}

void CAM_BCMDHUB_HardwareTrans(struct camera_isp_dev *isp_dev, struct DHUB_CFGQ *cfgQ, int block)
{
	HDL_dhub2d *pDhubHandle;
	HDL_semaphore *pSemHandle;
	unsigned int bcm_sched_cmd[2];
	int dhubID;
	int status;

	if (cfgQ->len <= 0)
		return;

	/* start BCM engine */
	dhubID = avioDhubChMap_vip128b_BCM_R;
	pDhubHandle = &CSI_dhubHandle;
	pSemHandle = isp_dhub_semaphore(&(pDhubHandle->dhub));
	if (block) {
		/* clear possible BCM previous interrupt */
		status = isp_semaphore_chk_full(pSemHandle, dhubID);
		if (status) {
			semaphore_pop(pSemHandle, dhubID, 1);
			semaphore_clr_full(pSemHandle, dhubID);
		}
	}

	/* clear BCM interrupt */
	status = isp_semaphore_chk_full(pSemHandle, dhubID);
	while (status) {
		semaphore_pop(pSemHandle, dhubID, 1);
		semaphore_clr_full(pSemHandle, dhubID);
		status = isp_semaphore_chk_full(pSemHandle, dhubID);
	}

	isp_shm_clean_cache(cfgQ->handle, 0, cfgQ->len*8);
	dhub_channel_generate_cmd(&(pDhubHandle->dhub), dhubID, (UINT64)cfgQ->phy_addr,
			(INT)cfgQ->len * 8, 0, 0, 0, 1, (SIGN32 *)bcm_sched_cmd);
	while (!BCM_SCHED_PushCmd(isp_dev, BCM_SCHED_Q12, bcm_sched_cmd, NULL))
		;

	if (block){
		/* check BCM interrupt */
		pSemHandle = isp_dhub_semaphore(&(pDhubHandle->dhub));
		status = isp_semaphore_chk_full(pSemHandle, dhubID);
		while (!status) {
			status = isp_semaphore_chk_full(pSemHandle, dhubID);
		}

		/* clear BCM interrupt */
		semaphore_pop(pSemHandle, dhubID, 1);
		semaphore_clr_full(pSemHandle, dhubID);
	}
}

int CAM_BCMDHUB_AutoPush(struct camera_isp_dev *isp_dev, int intrType, int enable)
{
	unsigned int sched_qid/*, trig_event*/;

	if (intrType == 0) { // VBI interrupt Queue
		sched_qid = BCM_SCHED_Q0;
	} else {
		sched_qid = BCM_SCHED_Q1;
	}
	BCM_SCHED_AutoPushCmd(isp_dev, sched_qid, enable);
	return ISP_OK;
}


/**
 *
 * @param[in] intrType - interrupt type which this cmdQ belongs to
 * @param[in] phy_start -
 * @param[in] size - size of the buffer to allocate
 * @param[in] cfgQ - target BCM cfgQ
 * @param[out] void
 */
void  CAM_BCMBUF_Commit_To_CFGQ(struct camera_isp_dev *isp_dev, int intrType,
		UINT32 phy_start, UINT32 size, struct DHUB_CFGQ *cfgQ)
{
	int index;
	unsigned int bcm_sched_cmd[2];

	UNUSED(intrType);
	dhub_channel_generate_cmd(&(CSI_dhubHandle.dhub), avioDhubChMap_vip128b_BCM_R,
		(INT)phy_start, (INT)size, 0, 0, 0, 1, bcm_sched_cmd);
	while (!(index = BCM_SCHED_PushCmd(isp_dev, BCM_SCHED_Q13, bcm_sched_cmd,
                                  cfgQ->addr + cfgQ->len * 2)))
		;

	cfgQ->len += index;
}

/**
 *
 * @param[in] pCfgQ -
 * @param[in] size - size of the buffer to allocate
 * @param[out] return 1 - succeed / 0 - failure
 */
INT CAM_CFGQ_Create(struct DHUB_CFGQ *pCfgQ, int size)
{
	//Allocate CfgQ
	if (isp_shm_allocate(SHM_NONSECURE, size, 32, &pCfgQ->handle,
			SHM_NONSECURE_CONTIG) != ISP_OK)
		return ISP_ENOMEM;

	pCfgQ->len = 0;
	isp_shm_get_virtual_address(pCfgQ->handle, 0, (void *)&pCfgQ->addr);
	isp_shm_get_physical_address(pCfgQ->handle, 0, (void *)&pCfgQ->phy_addr);
	pr_debug("cfgq addr: 0x%lx phy_addr: 0x%lx\n",
       (unsigned long)pCfgQ->addr, (unsigned long)pCfgQ->phy_addr);

	return ISP_OK;
}

/**
 *
 * @param[in] pCfgQ -
 * @param[out] return 1 - succeed / 0 - failure
 */
INT CAM_CFGQ_Destroy(struct DHUB_CFGQ *pCfgQ)
{
	if (!pCfgQ->addr)
		return (ISP_EBADCALL);

	isp_shm_release(pCfgQ->handle);

	return ISP_OK;
}

/**
 *
 * @param[in] pCfgQ -
 * @param[in] pbcmbuf - cpcb ID which this cmdQ belongs to
 * @param[out] return
 */
INT CAM_CFGQ_To_BCMBUF(struct DHUB_CFGQ *pCfgQ, struct BCMBUF *pbcmbuf)
{
	if (pCfgQ->len <= 0) {
		pr_info( "CAM_CFGQ_To_struct BCMBUF: Invalid length\r\n");
		return 0;
	}
	return CAM_BCMBUF_WriteBlock(pbcmbuf, pCfgQ->addr, pCfgQ->len*8);
}

INT CAM_BCMBUF_Raw_LogPrint(UINT64 *pdata, UINT32 length)
{
	int i;
	UINT64 addr, val;
	UINT64 *regValPair = pdata;

	if ((!pdata) || (length < 8))
		return ISP_EBADPARAM;

	for (i = 0; i < length; i += 8) {
		addr  = (*regValPair & 0xFFFFFFFF00000000) >> 32;
		val = (*regValPair & 0xFFFFFFFF);
		regValPair++;
		pr_err("0x%08X = 0x%08X\n", (unsigned int)addr, (unsigned int)val);
	}

	return ISP_OK;
}

INT CAM_BCMBUF_To_Raw(struct BCMBUF *pbcmbuf, UINT64 **start, INT32 *size)
{
	if (!pbcmbuf || !start || !size)
		return -1;

	*start = pbcmbuf->head;

	*size = (UINT64)pbcmbuf->writer - (UINT64)*start;
	printk(KERN_INFO "struct BCMBUF(%p) => start : %p, size:%d\n", pbcmbuf, *start, *size);
	return 0;
}

int  CAM_BCMBUF_LogPrint(struct BCMBUF *pbcmbuf)
{
	UINT64 *start;
	INT32 size;
	INT retVal;

	if ( 0 == (retVal = CAM_BCMBUF_To_Raw(pbcmbuf, &start, &size)) ) {
		retVal = CAM_BCMBUF_Raw_LogPrint(start, size);
	}
	return retVal;
}

int CAM_CFGQ_LogPrint(struct DHUB_CFGQ *pCfgQ)
{
	pr_info("CFGQ (%p) => start : %p, size:%d\n", pCfgQ, pCfgQ->addr, pCfgQ->len*8);

	return CAM_BCMBUF_Raw_LogPrint(pCfgQ->addr, pCfgQ->len*8);
}
