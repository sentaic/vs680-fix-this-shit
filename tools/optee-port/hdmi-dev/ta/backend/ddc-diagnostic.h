/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_DDC_DIAGNOSTIC_H
#define HDMI_DEV_DDC_DIAGNOSTIC_H
#include <stdbool.h>
#include "mode-io.h"
#include "vs680_ddc_diag.h"
#include "../../hdmi-dev.h"
/* 从当前 TX 映射读取 DDC 寄存器快照。 */
TEE_Result hdmi_ddc_snapshot(const struct hdmi_mode_io *,
                            struct hdmi_dev_ddc_snapshot *);
struct hdmi_ddc_owner {
    bool initialized,entered,quarantined;
    uint32_t phy_event,port_epoch;
    bool retained_valid;
    struct vd_report retained;
};
TEE_Result hdmi_ddc_read_current(const struct hdmi_mode_io *,
    struct hdmi_ddc_owner *,struct hdmi_dev_ddc_read *);
TEE_Result hdmi_ddc_prepare_owner(const struct hdmi_mode_io *,
    struct hdmi_ddc_owner *, struct hdmi_dev_ddc_prepare *);
TEE_Result hdmi_ddc_recover_current(const struct hdmi_mode_io *,
    struct hdmi_ddc_owner *,struct hdmi_dev_ddc_recovery *);
#endif
