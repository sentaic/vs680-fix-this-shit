/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_TX_SCDC_ADAPTER_H
#define HDMI_TX_SCDC_ADAPTER_H
#include "scdc-link.h"
/* 平台回调检查 DDC 所有权、时钟、复位和配置状态；每次传输前确认前一事务已结束。MMIO 使用物理字节地址并保持访问顺序。 */
struct hdmi_tx_scdc_platform {
    void *opaque;
    int (*read32)(void *, uint32_t, uint32_t *);
    int (*write32)(void *, uint32_t, uint32_t);
    int (*now_us)(void *, uint64_t *);
    int (*wait_us)(void *, uint32_t, uint32_t);
    int (*verify_source_clock)(void *, uint64_t, bool, uint32_t);
    int (*verify_ddc_ready)(void *, uint32_t);
    /* 恢复 DDC 的复位和配置，并检查空闲状态；恢复失败时保持 faulted。 */
    int (*reset_ddc_verified)(void *, uint32_t);
};
enum hdmi_tx_scdc_adapter_error {
    HDMI_TX_SCDC_EINVAL=-22, HDMI_TX_SCDC_UNSUPPORTED=-95,
    HDMI_TX_SCDC_BUSY=-16, HDMI_TX_SCDC_IO=-5,
    HDMI_TX_SCDC_TIMEOUT=-110, HDMI_TX_SCDC_DISCONNECTED=-67,
    HDMI_TX_SCDC_TIME=-62
};
/* 对象首次使用时清零；传输超时或错误后调用 recover_ddc，再处理 SCDC 配置恢复。 */
struct hdmi_tx_scdc_adapter {
    struct hdmi_tx_scdc_platform platform;
    uint64_t deadline_us, last_us;
    uint32_t bound, active, faulted, last_status;
};
int hdmi_tx_scdc_adapter_bind(struct hdmi_tx_scdc_adapter *,
    const struct hdmi_tx_scdc_platform *, struct hdmi_scdc_io *);
int hdmi_tx_scdc_adapter_recover_ddc(struct hdmi_tx_scdc_adapter *,
    uint32_t timeout_us);
#endif
