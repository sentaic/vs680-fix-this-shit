/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_TX_MODE_ADAPTER_H
#define HDMI_TX_MODE_ADAPTER_H
#include "../backend.h"
#include "display-mode.h"
#include "vpp-timing.h"
#include "pixel-clock-plan.h"
#include "avio-hardware.h"
#include "tx-ddc-runtime.h"
struct hdmi_tx_mode_adapter {
    struct hdmi_mode_io io;
    struct hdmi_avio_registers *registers;
    struct hdmi_display_timing timing;
    struct hdmi_display_link_caps caps;
    struct hdmi_display_report display;
    struct hdmi_vpp_timing_plan vpp_plan;
    struct hdmi_vpp_timing_report vpp_report;
    struct hdmi_vpll_report clock,clock_verified;
    struct hdmi_display_report phy_verified;
    struct hdmi_tx_ddc_runtime ddc;
    struct hdmi_scdc_io scdc_io;
    struct hdmi_scdc_report scdc;
    struct hdmi_scdc_caps scdc_caps;
    enum hdmi_vpp_blanking blanking;
    uint32_t refresh_millihz;
    bool configured, requires_scdc,link_changed,link_failed;
};
TEE_Result hdmi_tx_mode_validate(const struct hdmi_dev_mode_request *,
                                 struct hdmi_tx_mode_adapter *);
TEE_Result hdmi_tx_mode_prepare(struct hdmi_tx_mode_adapter *,
    const struct hdmi_mode_io *, struct hdmi_avio_registers *);
/* 接收端信息检查完成后，显式选择对应的配置入口。 */
TEE_Result hdmi_tx_mode_prepare_current_ddc(struct hdmi_tx_mode_adapter *,
    const struct hdmi_mode_io *, struct hdmi_avio_registers *,
    int (*verify_owner)(void *), void *owner_opaque);
TEE_Result hdmi_tx_mode_stop_link(struct hdmi_tx_mode_adapter *);
TEE_Result hdmi_tx_mode_enable(struct hdmi_tx_mode_adapter *);
#endif
