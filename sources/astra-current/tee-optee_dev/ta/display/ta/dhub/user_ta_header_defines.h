/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2025 Synaptics Incorporated */

/*
 * The name of this file must not be modified
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include "dhub_cmd.h"
/* To get the TA UUID definition */
#ifdef LOOP_RX_CAPTURE
#define TA_UUID { 0x0b564e41, 0x93da, 0x4fd8, {0xb2,0xf9,0x40,0x98,0x56,0x72,0xd1,0x28} }
#else
#define TA_UUID  TA_DHUB_UUID
#endif

/*
 * TA properties: multi-instance TA, no specific attribute
 * TA_FLAG_EXEC_DDR is meaningless but mandated.
 */
#define TA_FLAGS  TA_FLAG_EXEC_DDR | TA_FLAG_CACHE_MAINTENANCE

/* To be reviewed */
/* Provisioned stack size */
#define TA_STACK_SIZE  (16 * 1024)

/* Provisioned heap size for TEE_Malloc() and friends */
#define TA_DATA_SIZE  (32 * 1024)

/* The gpd.ta.version property */
#define TA_VERSION  "1.0"

/* The gpd.ta.description property */
#define TA_DESCRIPTION	"DHUB Trusted Application"

#endif /* USER_TA_HEADER_DEFINES_H */
