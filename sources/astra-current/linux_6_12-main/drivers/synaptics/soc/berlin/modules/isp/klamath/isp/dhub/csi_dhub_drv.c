// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/types.h>

#include "camera_isp_driver.h"
#include "api_csi_dhub.h"

/* KLAMATH_KERNEL: Local definitions to replace removed diag_common.h */
#ifndef UNUSED
#define UNUSED(x)           (void)(x)
#endif

HDL_dhub2d CSI_dhubHandle;

/* Total size vip128bDhub_BANK0_SIZE 0x5000 */
/* #define BWT_CASE 1 */

/* DHUB size definitions */
#ifndef BWT_CASE
    #define CSI_IPI0_Y_DHUB_SIZE    (CSI_IPIY_DHUB_SIZE)
    #define CSI_IPI0_C_DHUB_SIZE    (CSI_IPIC_DHUB_SIZE)
    #define CSI_IPI1_Y_DHUB_SIZE    (CSI_IPIY_DHUB_SIZE)
    #define CSI_IPI1_C_DHUB_SIZE    (CSI_IPIC_DHUB_SIZE)
#else
    #define CSI_IPI0_Y_DHUB_SIZE    (0x2600)
    #define CSI_IPI0_C_DHUB_SIZE    (0x0)
    #define CSI_IPI1_Y_DHUB_SIZE    (0x2600)
    #define CSI_IPI1_C_DHUB_SIZE    (0x0)
#endif

/* Note: CSI_BCM_DHUB_SIZE is already defined in api_csi_dhub.h */

/* DHUB base address definitions */
#define CSI_IPI0_Y_DHUB_BASE    (avioDhubTcmMap_vip128bDhub_BANK0_START_ADDR)
#define CSI_IPI0_C_DHUB_BASE    (CSI_IPI0_Y_DHUB_BASE + CSI_IPI0_Y_DHUB_SIZE)
#define CSI_IPI1_Y_DHUB_BASE    (CSI_IPI0_C_DHUB_BASE + CSI_IPI0_C_DHUB_SIZE)
#define CSI_IPI1_C_DHUB_BASE    (CSI_IPI1_Y_DHUB_BASE + CSI_IPI1_Y_DHUB_SIZE)
#define CSI_BCM_DHUB_BASE       (CSI_IPI1_C_DHUB_BASE + CSI_IPI1_C_DHUB_SIZE)

/* Magic numbers as named constants */
#define DHUB_BUFFER_OFFSET_64   (64)
#define DHUB_BUFFER_OFFSET_128  (128)
#define DHUB_QOS_MASK           (0xF)

/* DHUB channel configuration array */
DHUB_channel_config CSI_config[CSI_NUM_OF_CHANNELS] = {
	{
		avioDhubChMap_vip128b_IPI0Y_W,
		CSI_IPI0_Y_DHUB_BASE,
		CSI_IPI0_Y_DHUB_BASE + DHUB_BUFFER_OFFSET_64,
		DHUB_BUFFER_OFFSET_64,
		(CSI_IPI0_Y_DHUB_SIZE - DHUB_BUFFER_OFFSET_64),
		dHubChannel_CFG_MTU_256byte,
		1, 0, 1, DHUB_QOS_MASK, DHUB_QOS_MASK
	},
	{
		avioDhubChMap_vip128b_IPI0C_W,
		CSI_IPI0_C_DHUB_BASE,
#ifndef BWT_CASE
		CSI_IPI0_C_DHUB_BASE + DHUB_BUFFER_OFFSET_64,
		DHUB_BUFFER_OFFSET_64,
		(CSI_IPI0_C_DHUB_SIZE - DHUB_BUFFER_OFFSET_64),
#else
		CSI_IPI0_C_DHUB_BASE,
		0,
		0,
#endif
		dHubChannel_CFG_MTU_256byte,
		1, 0, 1, DHUB_QOS_MASK, DHUB_QOS_MASK
	},
	{
		avioDhubChMap_vip128b_IPI1Y_W,
		CSI_IPI1_Y_DHUB_BASE,
		CSI_IPI1_Y_DHUB_BASE + DHUB_BUFFER_OFFSET_64,
		DHUB_BUFFER_OFFSET_64,
		(CSI_IPI1_Y_DHUB_SIZE - DHUB_BUFFER_OFFSET_64),
		dHubChannel_CFG_MTU_256byte,
		1, 0, 1, DHUB_QOS_MASK, DHUB_QOS_MASK
	},
	{
		avioDhubChMap_vip128b_IPI1C_W,
		CSI_IPI1_C_DHUB_BASE,
#ifndef BWT_CASE
		CSI_IPI1_C_DHUB_BASE + DHUB_BUFFER_OFFSET_64,
		DHUB_BUFFER_OFFSET_64,
		(CSI_IPI1_C_DHUB_SIZE - DHUB_BUFFER_OFFSET_64),
#else
		CSI_IPI1_C_DHUB_BASE,
		0,
		0,
#endif
		dHubChannel_CFG_MTU_256byte,
		1, 0, 1, DHUB_QOS_MASK, DHUB_QOS_MASK
	},
	{
		avioDhubChMap_vip128b_BCM_R,
		CSI_BCM_DHUB_BASE,
		CSI_BCM_DHUB_BASE + DHUB_BUFFER_OFFSET_128,
		DHUB_BUFFER_OFFSET_128,
		(CSI_BCM_DHUB_SIZE - DHUB_BUFFER_OFFSET_128),
		dHubChannel_CFG_MTU_128byte,
		1, 0, 1, DHUB_QOS_MASK, DHUB_QOS_MASK
	}
};

/**
 * csi_dhub_get_channel_info() - Get the DHUB configuration of requested channel
 * @dhub_handle: Pointer to 2D dhubHandle
 * @channel_id: Channel ID of the dhub (0 to CSI_NUM_OF_CHANNELS-1)
 * @cfg: Configuration structure to be updated
 *
 * Return: 0 on success, negative error code on failure
 *   -EFAULT - Invalid configuration pointer or handle
 *   -EINVAL - Invalid channel ID
 *   -ENODEV - Invalid handle
 */
int csi_dhub_get_channel_info(HDL_dhub2d *dhub_handle, int channel_id, T32dHubChannel_CFG *cfg)
{
	DHUB_channel_config *dhub_config;

	/* Input parameter validation */
	if (!dhub_handle || !cfg)
		return -EFAULT;

	if (channel_id < 0 || channel_id >= CSI_NUM_OF_CHANNELS)
		return -EINVAL;

	/* Get the DHUB config array from the received handle */
	if (dhub_handle == &CSI_dhubHandle) {
		dhub_config = CSI_config;
	} else {
		return -ENODEV;
	}

	/* Update the MTU, QOS and self loop parameters */
	cfg->uCFG_MTU = dhub_config[channel_id].chanMtuSize;
	cfg->uCFG_QoS = dhub_config[channel_id].chanQos;
	cfg->uCFG_selfLoop = dhub_config[channel_id].chanSelfLoop;

	return 0;
}

/******************************************************************************************************************
 *    Function: DhubInitialization
 *    Description: Initialize DHUB .
 *    Parameter : cpuId ------------- cpu ID
 *             dHubBaseAddr -------------  dHub Base address.
 *             hboSramAddr ----- Sram Address for HBO.
 *             pdhubHandle ----- pointer to 2D dhubHandle
 *             dhub_config ----- configuration of AG
 *             numOfChans     ----- number of channels
 *    Return:        void
******************************************************************************************************************/
static void DhubInitialization(SIGN32 cpuId, UNSG64 dHubBaseAddr, UNSG64 hboSramAddr, HDL_dhub2d *pdhubHandle, DHUB_channel_config *dhub_config, SIGN32 numOfChans, DHUB_TYPE dHubType)
{
	HDL_semaphore *pSemHandle;
	SIGN32 i;
	SIGN32 chanId;
	SIGN32 cmdDiv = 8;
	UNUSED(cpuId);

	if (dHubType == DHUB_TYPE_128BIT)
		cmdDiv = 16;
	else
		cmdDiv = 8;

	/* Initialize HDL_dhub with a $dHub BIU instance. */
	dhub2d_hdl(hboSramAddr,			/* Base address of dHub.HBO SRAM */
		   dHubBaseAddr,		/* Base address of a BIU instance of $dHub */
		   pdhubHandle			/* Handle to HDL_dhub2d */
		  );
	/* set up semaphore to trigger cmd done interrupt
	 * note that this set of semaphores are different from the HBO semaphores
	 * the ID must match the dhub ID because they are hardwired.
	 */
	pSemHandle = isp_dhub_semaphore(&pdhubHandle->dhub);

	for (i = 0; i < numOfChans; i++) {
		/* Configurate a dHub channel
		 * note that in this function, it also configured right HBO channels(cmdQ and dataQ) and semaphores
		 */
		chanId = dhub_config[i].chanId;

		AVIO_DHUB_INITCHANNELAXQOS(pdhubHandle, dhub_config, chanId, i, 0);

		dhub_channel_cfg(
			&pdhubHandle->dhub,			/* Handle to HDL_dhub */
			chanId,					/* Channel ID in $dHubReg */
			dhub_config[i].chanCmdBase,		/* Channel FIFO base address (byte address) for cmdQ */
			dhub_config[i].chanDataBase,		/* Channel FIFO base address (byte address) for dataQ */
			dhub_config[i].chanCmdSize/cmdDiv,	/* Channel FIFO depth for cmdQ, in 64b word */
			dhub_config[i].chanDataSize/cmdDiv,	/* Channel FIFO depth for dataQ, in 64b word */
			dhub_config[i].chanMtuSize,		/* See 'dHubChannel.CFG.MTU', 0/1/2 for 8/32/128 bytes */
			dhub_config[i].chanQos,			/* See 'dHubChannel.CFG.QoS' */
			dhub_config[i].chanSelfLoop,		/* See 'dHubChannel.CFG.selfLoop' */
			dhub_config[i].chanEnable,		/* 0 to disable, 1 to enable */
			0					/* Pass NULL to directly init dHub, or
							 * Pass non-zero to receive programming sequence
							 * in (adr,data) pairs
							 */
			);
		/* setup interrupt for channel chanId */
		/* configure the semaphore depth to be 1 */
		semaphore_cfg(pSemHandle, chanId, 1, 0);
#if 0
		/* enable interrupt from this semaphore */
		semaphore_intr_enable(
			pSemHandle,	/* semaphore handler */
			chanId,
			0,		/* empty */
			1,		/* full */
			0,		/* almost_empty */
			0,		/* almost_full */
			cpuId		/* 0~2, depending on which CPU the interrupt is enabled for. */
			);
#endif
	}
}

/**
 * csi_dhub_channel_clear() - Clear corresponding DHUB channel
 * @hdl: Handle to HDL_dhub
 * @channel_id: Channel ID in dHubReg
 * @cfg_queue: Pass NULL to directly init dhub, or pass non-zero to receive
 *             programming sequence in (addr, data) pairs
 *
 * This function performs a complete channel clear sequence following the
 * hardware specification requirements.
 */
void csi_dhub_channel_clear(void *hdl, int channel_id, T64b cfg_queue[])
{
	u32 cmd_id, data_id;
	HDL_dhub *dhub;
	HDL_hbo *hbo;

	/* Input validation */
	if (!hdl || channel_id < 0)
		return;

	cmd_id = dhub_id2hbo_cmdQ(channel_id);
	data_id = dhub_id2hbo_data(channel_id);
	dhub = (HDL_dhub *)hdl;
	hbo = &(dhub->hbo);

	/* 1. Software stops the command queue in HBO */
	hbo_queue_enable(hbo, cmd_id, 0, cfg_queue);
	/* 2. Software stops the channel in dHub by writing zero to dHubChannel.START.EN */
	dhub_channel_enable(dhub, channel_id, 0, cfg_queue);
	/* 3. Software clears the channel in dHub by writing one to dHubChannel.CLEAR.EN */
	dhub_channel_clear(dhub, channel_id);
	/* 4. Software waits for the register bits dHubChannel.PENDING.ST and dHubChannel.BUSY.ST to be 0 */
	dhub_channel_clear_done(dhub, channel_id);
	/* 5. Software stops and clears the data queue */
	hbo_queue_enable(hbo, data_id, 0, cfg_queue);
	hbo_queue_clear(hbo, data_id);
	/* 6. Software wait for the corresponding busy bit to be 0 */
	hbo_queue_clear_done(hbo, data_id);
	/* 7. Software stops and clears the command queue */
	hbo_queue_enable(hbo, cmd_id, 0, cfg_queue);
	hbo_queue_clear(hbo, cmd_id);
	/* 8. Software wait for the corresponding busy bit to be 0 */
	hbo_queue_clear_done(hbo, cmd_id);
	/* 9. Software enable dHub and HBO */
	dhub_channel_enable(dhub, channel_id, 1, cfg_queue);
	hbo_queue_enable(hbo, cmd_id, 1, cfg_queue);
	hbo_queue_enable(hbo, data_id, 1, cfg_queue);
}

/**
 * csi_dhub_channel_flush() - Flush corresponding DHUB channel
 * @hdl: Handle to HDL_dhub
 * @channel_id: Channel ID in dHubReg
 * @cfg_queue: Pass NULL to directly init dhub, or pass non-zero to receive
 *             programming sequence in (addr, data) pairs
 *
 * This function performs a complete channel flush sequence following the
 * hardware specification requirements.
 */
void csi_dhub_channel_flush(void *hdl, int channel_id, T64b cfg_queue[])
{
	u32 cmd_id, data_id;
	HDL_dhub *dhub;
	HDL_hbo *hbo;

	/* Input validation */
	if (!hdl || channel_id < 0)
		return;

	cmd_id = dhub_id2hbo_cmdQ(channel_id);
	data_id = dhub_id2hbo_data(channel_id);
	dhub = (HDL_dhub *)hdl;
	hbo = &(dhub->hbo);

	/* 1. Software stops the command queue in HBO */
	hbo_queue_enable(hbo, cmd_id, 0, cfg_queue);
	/* 2. Software stops the channel in dHub by writing zero to dHubChannel.START.EN */
	dhub_channel_enable(dhub, channel_id, 0, cfg_queue);
	/* 3. Software clears the channel in dHub by writing one to dHubChannel.FLUSH.EN */
	dhub_channel_flush(dhub, channel_id);
	/* 4. Software waits for the register bits dHubChannel.PENDING.ST and dHubChannel.BUSY.ST to be 0 */
	dhub_channel_clear_done(dhub, channel_id);
	/* 5. Software stops and clears the command queue */
	hbo_queue_enable(hbo, cmd_id, 0, cfg_queue);
	hbo_queue_clear(hbo, cmd_id);
	/* 6. Software wait for the corresponding busy bit to be 0 */
	hbo_queue_clear_done(hbo, cmd_id);
	/* 7. Software enable dHub and HBO */
	dhub_channel_enable(dhub, channel_id, 1, cfg_queue);
	hbo_queue_enable(hbo, cmd_id, 1, cfg_queue);
	hbo_queue_enable(hbo, data_id, 1, cfg_queue);
}

/* Mutex to protect initialization state */
static DEFINE_MUTEX(csi_dhub_mutex);
static bool initialized;

/**
 * csi_dhub_init() - Initialize CSI DHUB with required configuration
 * @cpu_id: CPU ID for initialization
 *
 * This function ensures DHUB is initialized only once using proper locking.
 * It's thread-safe and can be called multiple times safely.
 *
 * Return: 0 on success, negative error code on failure
 */
int csi_dhub_init(struct camera_isp_dev *isp_dev, int cpu_id)
{
	int ret = 0;

	mutex_lock(&csi_dhub_mutex);

	if (initialized) {
		mutex_unlock(&csi_dhub_mutex);
		return 0; /* Already initialized */
	}

	/* De-Assert IMGP Global reset - otherwise cannot access IMGP register space */
	/* IMGP_GLB_RestCtrl(IMGP_RESET_CTRL_ENABLE); */

	/* Initialize the DHUB content with required details */
	/* Since we are using IMGP_DHUB_128bBCM_TYPE_64b, MEMMAP_IMGP_DUMMY_REG_BASE is not valid */
	/* IMGP_InitDhubContext(IMGP_DHUB_128bBCM_TYPE_64b, MEMMAP_IMGP_BCM_REG_BASE, MEMMAP_IMGP_DUMMY_REG_BASE); */

	/* Initialize DHUB */
	DhubInitialization(cpu_id, (UNSG64)(isp_dev->dhub_base_addr + CSI_DHUB_BASE),
			(UNSG64)isp_dev->dhub_base_addr + CSI_HBO_SRAM_BASE,
			&CSI_dhubHandle, CSI_config, CSI_NUM_OF_CHANNELS,
			DHUB_TYPE_128BIT);

	/* Register dhub interrupt handler */
	/* IMGP_DhubRegisterHandler(cpu_id, 1); */

	initialized = true;

	mutex_unlock(&csi_dhub_mutex);
	return ret;
}

/**
 * csi_dhub_exit() - Cleanup and exit CSI DHUB
 * @cpu_id: CPU ID for cleanup (currently unused but kept for API consistency)
 *
 * This function safely resets the initialization state to allow re-initialization.
 * It's thread-safe and properly synchronized.
 */
void csi_dhub_exit(int cpu_id)
{
	mutex_lock(&csi_dhub_mutex);

	if (!initialized) {
		mutex_unlock(&csi_dhub_mutex);
		return; /* Already cleaned up */
	}

	/* Additional cleanup could be added here if needed */
	/* For example: interrupt handler deregistration, resource cleanup, etc. */

	/* Reset initialization flag to allow re-initialization */
	initialized = false;

	mutex_unlock(&csi_dhub_mutex);
}
