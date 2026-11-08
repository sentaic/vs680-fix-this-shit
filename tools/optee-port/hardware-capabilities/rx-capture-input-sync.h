/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_CAPTURE_INPUT_SYNC_H
#define RX_CAPTURE_INPUT_SYNC_H
#include <stdint.h>
#include <tee_internal_api.h>
struct rx_capture_input_sync_status {
    uint32_t polls, input_status, event_status, elapsed_ms;
};
/* 调用方持有管线，已排队 DMA，并保持 TG 清零及输入写屏蔽。成功时启动 WR/OTG/TG，失败时保持停止；此函数与 DMA 准备在同一次 TA 调用中执行。 */
TEE_Result rx_capture_input_sync_arm(struct rx_capture_input_sync_status *status);
#endif
