// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __M2M_KERNEL_COMPATIBILITY_WRAP_H__
#define __M2M_KERNEL_COMPATIBILITY_WRAP_H__

#include <linux/version.h>
#include <linux/compiler.h>

#include "kernel_compatibility.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 19, 0))
#define m2m_create_dev_class(CLS_NAME) class_create(CLS_NAME)
#else
#define m2m_create_dev_class(CLS_NAME) class_create(THIS_MODULE, CLS_NAME)
#endif

#endif
