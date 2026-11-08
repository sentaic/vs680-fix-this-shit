// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __INTR_HANDLER_H__
#define __INTR_HANDLER_H__
#include "api_dhub.h"

#define VIP_DHUB_INTR_HANDLER_MAX 32

typedef int (*VIP_DHUB_INTR_HANDLER)(UNSG32 intrNum, void *pArgs);
typedef void* VIP_DHUB_CONTEXT_HANDLE;

typedef struct VIP_DHUB_CONTEXT_INFO_s {
    VIP_DHUB_INTR_HANDLER pIntrHandler[VIP_DHUB_INTR_HANDLER_MAX];
    void *pIntrHandlerArgs[VIP_DHUB_INTR_HANDLER_MAX];
} VIP_DHUB_CONTEXT_INFO;

void VIP_DhubIntrDeRegisterHandler(void *handle, int intr);
void VIP_DhubIntrRegisterHandler(void *handle, int intr, void *pArgs,
                                 VIP_DHUB_INTR_HANDLER intrHandler);
VIP_DHUB_CONTEXT_HANDLE VIP_IntrHandleInit(struct device *dev, int irq_num);
void VIP_IntrHandleExit(void *handle);
struct CSI_PL_CTX_s *VIP_GetIntrCtx(void *handle, int intr);

#endif
