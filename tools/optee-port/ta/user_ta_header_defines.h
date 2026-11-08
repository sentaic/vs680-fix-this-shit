// SPDX-License-Identifier: GPL-2.0-only
#ifndef VS680_SMOKE_TA_HEADER_H
#define VS680_SMOKE_TA_HEADER_H
#define TA_UUID { 0x68680001, 0x1011, 0x4000, { 0x80, 0, 0, 0, 0, 0, 0, 1 } }
#define TA_FLAGS (TA_FLAG_SINGLE_INSTANCE | TA_FLAG_MULTI_SESSION)
#define TA_STACK_SIZE 2048
#define TA_DATA_SIZE 16384
#define TA_VERSION "1.0"
#define TA_DESCRIPTION "VS680 OP-TEE arithmetic verification"
#endif
