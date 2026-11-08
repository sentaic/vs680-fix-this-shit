/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_TX_DDC_RUNTIME_H
#define HDMI_TX_DDC_RUNTIME_H
#include "mode-io.h"
#include "tx-scdc-adapter.h"
/* 对象首次使用时清零，绑定前由调用方获取 TX/DDC 所有权并收尾旧事务。 */
struct hdmi_tx_ddc_runtime {
    struct hdmi_mode_io io;
    struct hdmi_tx_scdc_adapter adapter;
    int (*verify_source_clock)(void *, uint64_t, bool, uint32_t);
    void *source_opaque;
    int (*query_sfr)(void *, uint64_t *);
    void *sfr_opaque;
    uint64_t sfr_hz;
    int (*verify_owner)(void *);
    void *owner_opaque;
    uint32_t current_config_mode;
    uint32_t current_config[13];
    uint64_t last_time_us;
    uint32_t time_valid, initialized, reset_issued, config_issued, failed;
    TEE_Result last_result;
};
/* 兼容入口直接返回 NOT_SUPPORTED；需要时钟查询的调用使用 bind_with_sfr。 */
TEE_Result hdmi_tx_ddc_runtime_bind(struct hdmi_tx_ddc_runtime *,
    const struct hdmi_mode_io *,
    int (*verify_source_clock)(void *, uint64_t, bool, uint32_t),
    void *source_opaque, struct hdmi_scdc_io *out);
/* query_sfr 查询当前 SFR 时钟，现有实现接受 25 MHz；绑定、就绪检查和每次写入前重新查询。 */
TEE_Result hdmi_tx_ddc_runtime_bind_with_sfr(struct hdmi_tx_ddc_runtime *,
    const struct hdmi_mode_io *,
    int (*verify_source_clock)(void *, uint64_t, bool, uint32_t),
    void *source_opaque, int (*query_sfr)(void *, uint64_t *),
    void *sfr_opaque, struct hdmi_scdc_io *out);
/* 当前 DDC 事务收尾后，通过两次一致采样绑定现有时序；verify_owner 在每次访问前检查所有权。 */
TEE_Result hdmi_tx_ddc_runtime_bind_current_config(struct hdmi_tx_ddc_runtime *,
    const struct hdmi_mode_io *,
    int (*verify_source_clock)(void *, uint64_t, bool, uint32_t),
    void *source_opaque, int (*verify_owner)(void *), void *owner_opaque,
    struct hdmi_scdc_io *out);
/* 在空闲且正常的状态下重设配置，其他状态返回 NOT_SUPPORTED 并保留所有权。 */
TEE_Result hdmi_tx_ddc_runtime_reset(struct hdmi_tx_ddc_runtime *,
                                    uint32_t timeout_us);
#endif
