/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_AVIO_HARDWARE_H
#define HDMI_DEV_AVIO_HARDWARE_H
#include <tee_internal_api.h>

struct hdmi_avio_registers {
    volatile uint32_t *chip;
    volatile uint32_t *vpp;
    volatile uint32_t *aio;
};
struct hdmi_avio_activity {
    uint32_t reset_issued;
    uint32_t vpp_busy;
    uint32_t vpp_pending;
    uint32_t vpp_2d_busy;
    uint32_t aio_busy;
    uint32_t aio_pending;
    uint32_t aio_2d_busy;
    uint32_t queued_domains;
};
TEE_Result hdmi_avio_map(struct hdmi_avio_registers *registers);
TEE_Result hdmi_avio_snapshot(const struct hdmi_avio_registers *registers,
                             struct hdmi_avio_activity *activity);
/* 调用者须独占整个 AVIO；复位期间保留所有 DMA 内存。 */
TEE_Result hdmi_avio_reset(struct hdmi_avio_registers *registers,
                          struct hdmi_avio_activity *activity);
TEE_Result hdmi_avio_tx_snapshot(const struct hdmi_avio_registers *,
                                struct hdmi_avio_activity *);
bool hdmi_avio_idle(const struct hdmi_avio_activity *activity);
#endif
