/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2025, Synaptics Incorporated.
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <otp_ta.h>

#define TA_UUID				TA_OTP_UUID

#define TA_FLAGS			TA_FLAG_EXEC_DDR

#define TA_STACK_SIZE			(2 * 1024)

#define TA_DATA_SIZE			(32 * 1024)

#define TA_VERSION	"1.0"

#define TA_DESCRIPTION	"Synaptics OTP Trusted Application"

#endif /* USER_TA_HEADER_DEFINES_H */
