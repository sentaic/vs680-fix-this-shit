/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_TX_STOP_H
#define HDMI_TX_STOP_H
#include "frame-start.h"
TEE_Result hdmi_tx_require_idle(const struct hdmi_mode_io *,
    const struct hdmi_avio_registers *,struct hdmi_avio_activity *);
TEE_Result hdmi_tx_stop(const struct hdmi_mode_io *,struct hdmi_avio_registers *,
    struct hdmi_frame_start *,struct hdmi_avio_activity *);
#endif
