// SPDX-License-Identifier: GPL-2.0-only
#ifndef VS680_HDMI_DEV_BACKEND_H
#define VS680_HDMI_DEV_BACKEND_H

#include <stddef.h>
#include <tee_internal_api.h>
#include "../hdmi-dev.h"

/* 帧的 CPU 映射；底层缓冲由 Linux holder 持有。 */
struct hdmi_dev_frame {
    void *mapped;
    size_t capacity;
    uint32_t width, height, stride;
};

/* Linux holder 持有帧和命令内存，TA 保存映射及显示状态。硬件停止后再回收底层缓冲。 */

/* 初始化 720p60 RGB24 输出并检查物理池范围。成功进入 READY；中途失败保留缓冲并记录错误，后续由 stop 清理。 */
TEE_Result hdmi_dev_backend_init(uint32_t pool_pa, uint32_t pool_bytes);
TEE_Result hdmi_dev_backend_preflight_mode(const struct hdmi_dev_mode_request *,
    struct hdmi_dev_mode_preflight *);
TEE_Result hdmi_dev_backend_init_mode(const struct hdmi_dev_mode_request *request);

/* 借出一个已空闲的帧槽，供调用方写入后 present；已有待提交帧或暂无空闲槽时返回 BUSY。 */
TEE_Result hdmi_dev_backend_begin_frame(struct hdmi_dev_frame *frame);

/* 清理帧和命令的缓存并提交显示，成功进入 DISPLAYING；失败时保留已有缓冲。 */
TEE_Result hdmi_dev_backend_present(void);

/* 处理 VBI 和 DMA 完成事件、切换帧槽；没有新图时继续显示当前槽。 */
TEE_Result hdmi_dev_backend_service(void);

/* 在下一次VBI表中应用底色和GFX1全局透明度，0透明、4095不透明。 */
TEE_Result hdmi_dev_backend_blend(uint32_t background_rgb, uint32_t alpha_q12);

/* 读取状态、尺寸、刷新率和最近错误；DMA_QUIESCENT 表示硬件已停止访问提交的内存。 */
TEE_Result hdmi_dev_backend_overlay(const struct hdmi_dev_overlay *geometry,
    const void *pixels, size_t bytes);
TEE_Result hdmi_dev_backend_status(struct hdmi_dev_status *status);
TEE_Result hdmi_dev_backend_ddc_snapshot(struct hdmi_dev_ddc_snapshot *);
TEE_Result hdmi_dev_backend_ddc_read(struct hdmi_dev_ddc_read *);

/* 停止硬件并确认 DMA 结束，然后更新状态；停止失败时保留缓冲供后续恢复。 */
TEE_Result hdmi_dev_backend_stop(void);

TEE_Result hdmi_dev_backend_ddc_prepare(struct hdmi_dev_ddc_prepare *);
TEE_Result hdmi_dev_backend_ddc_recover(struct hdmi_dev_ddc_recovery *);
#endif
