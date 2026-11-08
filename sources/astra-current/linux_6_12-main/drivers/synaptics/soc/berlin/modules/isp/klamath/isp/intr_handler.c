// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/interrupt.h>
#include <linux/slab.h>

#include "intr_handler.h"
#include "camera_isp_driver.h"
#include "api_csi_dhub.h"


extern HDL_dhub2d CSI_dhubHandle;

/* Forward declarations to avoid heavy includes here */
struct CSI_PL_CTX_s;
typedef struct CSI_PL_CTX_s CSI_PL_CTX_t;
struct device;

static irqreturn_t VIP_IrqHandler(int irq, void *param)
{
	int instat, intrID;
	HDL_semaphore *pSemHandle = isp_dhub_semaphore(&CSI_dhubHandle.dhub);
	VIP_DHUB_CONTEXT_INFO *pCtxInfo = (VIP_DHUB_CONTEXT_INFO *)param;

	/* check avio0 interrupt status to figure out which CPCB interrupt */
	intrID = 0;
	instat = isp_semaphore_chk_full(pSemHandle, -1);
	while (instat) {
		if (instat & (1 << intrID)) {
			semaphore_pop(pSemHandle, intrID, 1);
			semaphore_clr_full(pSemHandle, intrID);
			instat &= ~(1 << intrID);
			if (pCtxInfo->pIntrHandler[intrID]) {
				(*pCtxInfo->pIntrHandler[intrID])(intrID, pCtxInfo->pIntrHandlerArgs[intrID]);
			}
		}
		intrID++;
	}
	if (instat != 0) {
		pr_err("Unhandled interrupt: 0x%x\n", instat);
		semaphore_pop(pSemHandle, instat, 1);
		semaphore_clr_full(pSemHandle, instat);
	}

	return IRQ_HANDLED;
}

static void VIP_DhubIntrEnable(int intr, int enable)
{
	HDL_semaphore *pSemHandle;
	HDL_dhub2d *pdhubHandle;

	pdhubHandle = &CSI_dhubHandle;
	pSemHandle = isp_dhub_semaphore(&pdhubHandle->dhub);
	semaphore_cfg(pSemHandle, intr, 1, 0);
	semaphore_clr_full(pSemHandle, intr);
	semaphore_intr_enable(pSemHandle, intr, 0, enable, 0, 0, 0);
}

void VIP_DhubIntrRegisterHandler(void *handle, int intr, void *pArgs,
				VIP_DHUB_INTR_HANDLER intrHandler) {

	int intrNum = intr;
	VIP_DHUB_CONTEXT_INFO *pCtxInfo = (VIP_DHUB_CONTEXT_INFO *)handle;

	if (pCtxInfo && (intrNum < VIP_DHUB_INTR_HANDLER_MAX)) {
		pCtxInfo->pIntrHandler[intrNum] = intrHandler;
		pCtxInfo->pIntrHandlerArgs[intrNum] = pArgs;
	}
	VIP_DhubIntrEnable(intr, 1);
}

void VIP_DhubIntrDeRegisterHandler(void *handle, int intr) {

	int intrNum = intr;
	VIP_DHUB_CONTEXT_INFO *pCtxInfo = (VIP_DHUB_CONTEXT_INFO *)handle;

	if (pCtxInfo && (intrNum < VIP_DHUB_INTR_HANDLER_MAX)) {
		pCtxInfo->pIntrHandler[intrNum] = NULL;
		pCtxInfo->pIntrHandlerArgs[intrNum] = NULL;
	}
	VIP_DhubIntrEnable(intr, 0);
}

VIP_DHUB_CONTEXT_HANDLE VIP_IntrHandleInit(struct device *dev, int irq_num)
{
	//int gic_intr = IRQ_dHubIntrAvio2_0;
	VIP_DHUB_CONTEXT_INFO *pCtxInfo = kzalloc(sizeof(VIP_DHUB_CONTEXT_INFO), GFP_KERNEL);
	int ret;
	if (!pCtxInfo)
		return NULL;

	ret = devm_request_irq(dev, irq_num, VIP_IrqHandler, 0, "vip_dhub_irq", pCtxInfo);
	if (ret) {
		pr_err("Failed to request IRQ %d: %d\n", irq_num, ret);
		kfree(pCtxInfo);
		return NULL;
	}
	return (VIP_DHUB_CONTEXT_HANDLE *)pCtxInfo;
}

void VIP_IntrHandleExit(void *handle)
{
	VIP_DHUB_CONTEXT_INFO *pCtxInfo = (VIP_DHUB_CONTEXT_INFO *)handle;
	kfree(pCtxInfo);
}

/*
 * Retrieve interrupt context for a given intr number from the ISP intr handle.
 */
struct CSI_PL_CTX_s *VIP_GetIntrCtx(void *handle, int intr)
{
	VIP_DHUB_CONTEXT_INFO *pCtxInfo = (VIP_DHUB_CONTEXT_INFO *)handle;
	struct CSI_PL_CTX_s *ctx = NULL;

	pr_debug("%s: ENTER\n", __func__);

	if (!pCtxInfo)
		goto out;

	if (intr < 0 || intr >= VIP_DHUB_INTR_HANDLER_MAX)
		goto out;

	ctx = (struct CSI_PL_CTX_s *)pCtxInfo->pIntrHandlerArgs[intr];

out:
	pr_debug("%s: EXIT\n", __func__);
	return ctx;
}
