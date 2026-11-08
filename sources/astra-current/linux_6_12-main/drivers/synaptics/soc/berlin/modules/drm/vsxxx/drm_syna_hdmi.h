// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2024 Synaptics Incorporated
 *
 */

#if !defined(__DRM_SYNA_HDMI_H__)
#define __DRM_SYNA_HDMI_H__

#include "drm_syna_drv.h"
#if IS_ENABLED(CONFIG_CEC_CORE)
#include <media/cec-notifier.h>
#endif

#define HDMITX_CORE_CONFIG_ENTRIES 2
#define to_syna_conn_hdmi(conn) container_of(conn, struct syna_conn_hdmi, base)

struct syna_hdmi_mode_map {
	const char *name;
	int id;
	u32 sinkcap_bitmask;
};

typedef union SYNA_HDMITX_CONF_T {
	UINT8 hdmiTxConfig[HDMITX_CORE_CONFIG_ENTRIES];
	struct {
		UINT8 hpdHandlingEnabled;
		UINT8 fixedModeSet;
	} hdmiTxConfigFields;
} SYNA_HDMITX_CONF;

struct syna_conn_hdmi {
	struct drm_connector base;
	struct syna_drm_private *drm_priv;

	struct task_struct *hpd_monitor_task;
	SYNA_HDMITX_CONF syna_hdmi_conf;

	UINT8 count_format_supported;
	UINT8 *supported_formats;

	struct dentry *debugfs_res_node;
	struct dentry *debugfs_hpd_node;
#if IS_ENABLED(CONFIG_CEC_CORE)
	struct cec_notifier *cec;
#endif
};
#endif
