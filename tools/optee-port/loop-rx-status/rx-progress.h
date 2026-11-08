/* SPDX-License-Identifier: GPL-2.0 */
#ifndef VS680_RX_PROGRESS_H
#define VS680_RX_PROGRESS_H
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
typedef uint32_t u32;
typedef uint64_t u64;
#endif

enum loop_rx_phase { LOOP_RX_IDLE, LOOP_RX_WAIT_LOCK, LOOP_RX_WAIT_EVENTS,
                     LOOP_RX_VALID, LOOP_RX_FAILED };
struct loop_rx_progress {
    u64 attempt, started_ms;
    enum loop_rx_phase phase;
    u32 saved_threshold, horizontal, vertical;
    u32 avi_header, avi_payload[4];
    u32 packet_seen, avp_seen, packet_mask, avp_mask;
    unsigned frames, avi_valid, threshold_saved, video_requested;
    int error, cleanup_error;
};
/* 调用方在整个事务期间持有 RX 锁。now_ms 使用内核单调时钟；begin 初始化状态，step 读取硬件，VALID 表示输入信息包和帧同步检查通过。 */
int loop_rx_progress_begin(struct loop_rx_progress *state, u64 attempt, u64 now_ms);
int loop_rx_progress_step(struct loop_rx_progress *state, u64 attempt, u64 now_ms);
#endif
