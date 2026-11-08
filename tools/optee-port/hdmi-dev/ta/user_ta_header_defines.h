// SPDX-License-Identifier: GPL-2.0-only
#ifndef VS680_HDMI_DEV_USER_TA_HEADER_DEFINES_H
#define VS680_HDMI_DEV_USER_TA_HEADER_DEFINES_H

#include "../hdmi-dev.h"

#define TA_UUID HDMI_DEV_UUID
/* One live session, unload after its close. DMA storage is owned elsewhere.
 * Cache maintenance is needed if the board backend uses TEE_CacheClean(). */
#define TA_FLAGS (TA_FLAG_SINGLE_INSTANCE | TA_FLAG_CACHE_MAINTENANCE)
#define TA_STACK_SIZE (16 * 1024)
#ifdef HDMI_DEV_SHOW_TIMING_ENABLED
#define TA_DATA_SIZE (64 * 1024)
#else
#define TA_DATA_SIZE (32 * 1024)
#endif
#define TA_VERSION "0.1"
#define TA_DESCRIPTION "VS680 development-only HDMI frontend"

#endif
