// SPDX-License-Identifier: GPL-2.0
#include <linux/string.h>
#include "rx-phy-context.h"
#include "rx-avi.h"
#include "rx-progress.h"

int loop_rx_video_request(void);
#define RX_PROGRESS_TIMEOUT_MS 1000
#define RX_VSYNC_EVENT HDMI_AVPUNIT_1_INT_MASK_N_DEFRAMER_VSYNC_MASK
#define RX_AVI_EVENT HDMI_PKT_0_INT_STATUS_AVIIF

/* 按 SCDC 时钟比配置 PHY，再等待输入锁定；配置改变后进入下一次采样。 */
static int sync_clock_ratio(int configure)
{
    u32 requested = hrx_reg_read(&loop_receiver, HDMI_SCDC_REGBANK_STATUS1) &
                    HDMI_SCDC_REGBANK_STATUS1_TMDSBITCLKRATIO;
    u32 desired = requested ? HDMI_PHY_CONFIG_TMDS_CLOCK_RATIO_MASK : 0;
    u32 actual = hrx_reg_read(&loop_receiver, HDMI_PHY_CONFIG) &
                 HDMI_PHY_CONFIG_TMDS_CLOCK_RATIO_MASK;
    int changed = actual != desired;

    if (changed) {
        if (!configure) return -ENOLCK;
        hrx_reg_mask_write(&loop_receiver, !!requested, HDMI_PHY_CONFIG,
            HDMI_PHY_CONFIG_TMDS_CLOCK_RATIO_OFFSET,
            HDMI_PHY_CONFIG_TMDS_CLOCK_RATIO_MASK);
        if ((hrx_reg_read(&loop_receiver, HDMI_PHY_CONFIG) &
                HDMI_PHY_CONFIG_TMDS_CLOCK_RATIO_MASK) != desired)
            return -EIO;
    }
    if ((hrx_reg_read(&loop_receiver, HDMI_SCDC_REGBANK_STATUS1) &
            HDMI_SCDC_REGBANK_STATUS1_TMDSBITCLKRATIO) != requested)
        return configure ? 1 : -ENOLCK;
    loop_receiver.is_hdmi2 = !!requested;
    return changed;
}

static int input_locked(void)
{
    return (hrx_reg_read(&loop_receiver, HDMI_MAINUNIT_STATUS) &
            HDMI_MAINUNIT_STATUS_TMDSVALID_STABLE) &&
           (hrx_reg_read(&loop_receiver, HDMI_CMU_STATUS) &
            HDMI_CMU_STATUS_TMDSQP_CK_LOCKED);
}

/* Synopsys interrupt clearing first samples the corresponding status.
 * See Linux snps_hdmirx.c hdmirx_clear_interrupt(); these VS680 banks
 * also place STATUS eight bytes before CLEAR. */
static void clear_event(u32 status, u32 clear, u32 mask)
{
    (void)hrx_reg_read(&loop_receiver, status);
    hrx_reg_write(&loop_receiver, mask, clear);
}

static int finish_progress(struct loop_rx_progress *s, int error)
{
    if (s->threshold_saved) {
        hrx_reg_mask_write(&loop_receiver,
            s->saved_threshold >> HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_OFFSET,
            HDMI_DEFRAMER_CONFIG0, HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_OFFSET,
            HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_MASK);
        if ((hrx_reg_read(&loop_receiver, HDMI_DEFRAMER_CONFIG0) &
                HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_MASK) != s->saved_threshold)
            s->cleanup_error = -EIO;
        else s->threshold_saved = 0;
    }
    s->error = error ? error : s->cleanup_error;
    s->phase = s->error ? LOOP_RX_FAILED : LOOP_RX_VALID;
    return s->error ? s->error : 1;
}

int loop_rx_progress_begin(struct loop_rx_progress *s, u64 attempt, u64 now_ms)
{
    if (!s || !attempt) return -EINVAL;
    if (s->phase == LOOP_RX_WAIT_LOCK || s->phase == LOOP_RX_WAIT_EVENTS)
        return -EBUSY;
    if (s->threshold_saved) return -EBUSY;
    if (attempt <= s->attempt) return -ESTALE;
    *s = (struct loop_rx_progress){.attempt = attempt, .started_ms = now_ms,
                                 .phase = LOOP_RX_WAIT_LOCK};
    return 0;
}

static int arm_events(struct loop_rx_progress *s)
{
    u32 avi_baseline, vsync_baseline;
    s->saved_threshold = hrx_reg_read(&loop_receiver, HDMI_DEFRAMER_CONFIG0) &
                         HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_MASK;
    s->threshold_saved = 1;
    hrx_reg_mask_write(&loop_receiver, 1, HDMI_DEFRAMER_CONFIG0,
        HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_OFFSET,
        HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_MASK);
    if ((hrx_reg_read(&loop_receiver, HDMI_DEFRAMER_CONFIG0) &
            HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_MASK) !=
            (1u << HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_OFFSET))
        return finish_progress(s, -EIO);
    /* 清除本事务使用的 AVI 和 VSYNC 事件，从新的信息包开始计数。 */
    clear_event(HDMI_PKT_0_INT_STATUS, HDMI_PKT_0_INT_CLEAR, RX_AVI_EVENT);
    clear_event(HDMI_AVPUNIT_1_INT_STATUS, HDMI_AVPUNIT_1_INT_CLEAR, RX_VSYNC_EVENT);
    hrx_reg_write(&loop_receiver, 1, HDMI_DEFRAMER_VSYNC_CNT_CLEAR);
    avi_baseline = hrx_reg_read(&loop_receiver, HDMI_PKT_0_INT_STATUS);
    vsync_baseline = hrx_reg_read(&loop_receiver, HDMI_AVPUNIT_1_INT_STATUS);
    if ((avi_baseline & RX_AVI_EVENT) || (vsync_baseline & RX_VSYNC_EVENT)) {
        pr_warn("loop-rx: event baseline attempt=%llu packet=%#x avp=%#x\n",
                (unsigned long long)s->attempt, avi_baseline, vsync_baseline);
        return finish_progress(s, -EAGAIN);
    }
    s->packet_mask = hrx_reg_read(&loop_receiver, HDMI_PKT_0_INT_MASK_N);
    s->avp_mask = hrx_reg_read(&loop_receiver, HDMI_AVPUNIT_1_INT_MASK_N);
    s->phase = LOOP_RX_WAIT_EVENTS;
    return 0;
}

int loop_rx_progress_step(struct loop_rx_progress *s, u64 attempt, u64 now_ms)
{
    u32 avi, vsync, horizontal, vertical;
    int result, first_avi = 0;
    if (!s || !attempt) return -EINVAL;
    if (attempt != s->attempt) return -ESTALE;
    /* 重复查询直接返回已完成事务的结果。 */
    if (s->phase == LOOP_RX_VALID) return 1;
    if (s->phase == LOOP_RX_FAILED) return s->error;
    if (s->phase != LOOP_RX_WAIT_LOCK && s->phase != LOOP_RX_WAIT_EVENTS)
        return -EINVAL;
    if (now_ms < s->started_ms) return finish_progress(s, -EINVAL);
    if (now_ms - s->started_ms >= RX_PROGRESS_TIMEOUT_MS) {
        /* 超时时输出本事务已经采集的状态。 */
        pr_warn("loop-rx: event timeout attempt=%llu frames=%u avi=%u packet_seen=%#x avp_seen=%#x packet_mask=%#x avp_mask=%#x horizontal=%#x vertical=%#x\n",
                (unsigned long long)s->attempt, s->frames, s->avi_valid,
                s->packet_seen, s->avp_seen, s->packet_mask, s->avp_mask,
                s->horizontal, s->vertical);
        return finish_progress(s, -ETIMEDOUT);
    }
    result = sync_clock_ratio(s->phase == LOOP_RX_WAIT_LOCK);
    if (result < 0) return finish_progress(s, result);
    if (result) return 0;
    /* 先请求视频路径复位，再等待复位后的输入锁定。 */
    if (s->phase == LOOP_RX_WAIT_LOCK && !s->video_requested) {
        result = loop_rx_video_request();
        if (result) return finish_progress(s, result);
        s->video_requested = 1;
        return 0;
    }
    if (!input_locked()) {
        if (s->phase == LOOP_RX_WAIT_LOCK) return 0;
        pr_warn("loop-rx: input lock lost attempt=%llu frames=%u avi=%u\n",
                (unsigned long long)s->attempt, s->frames, s->avi_valid);
        return finish_progress(s, -ENOLCK);
    }
    if (s->phase == LOOP_RX_WAIT_LOCK) {
        horizontal = hrx_reg_read(&loop_receiver, HDMI_VMON_STATUS3);
        vertical = hrx_reg_read(&loop_receiver, HDMI_VMON_STATUS6);
        if (!(horizontal & 65535) || !(vertical & 65535) ||
            (horizontal & 65535) >= (horizontal >> 16) ||
            (vertical & 65535) >= (vertical >> 16)) return 0;
        return arm_events(s);
    }
    if ((hrx_reg_read(&loop_receiver, HDMI_DEFRAMER_CONFIG0) &
            HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_MASK) !=
            (1u << HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_OFFSET))
        return finish_progress(s, -EIO);
    avi = hrx_reg_read(&loop_receiver, HDMI_PKT_0_INT_STATUS);
    vsync = hrx_reg_read(&loop_receiver, HDMI_AVPUNIT_1_INT_STATUS);
    s->packet_seen |= avi;
    s->avp_seen |= vsync;
    avi &= RX_AVI_EVENT;
    vsync &= RX_VSYNC_EVENT;
    if (avi) {
        struct loop_rx_avi_packet packet;
        result = loop_rx_avi_read_packet(&packet);
        if (result && (result != -EAGAIN || s->avi_valid))
            return finish_progress(s, result);
        if (!result) {
            if (s->avi_valid && (s->avi_header != packet.header ||
                    memcmp(s->avi_payload, packet.payload, sizeof(s->avi_payload))))
                return finish_progress(s, -ENOLCK);
            first_avi = !s->avi_valid;
            s->avi_header = packet.header;
            memcpy(s->avi_payload, packet.payload, sizeof(s->avi_payload));
            s->avi_valid = 1;
        }
        clear_event(HDMI_PKT_0_INT_STATUS, HDMI_PKT_0_INT_CLEAR, RX_AVI_EVENT);
        if (hrx_reg_read(&loop_receiver, HDMI_PKT_0_INT_STATUS) & RX_AVI_EVENT)
            return finish_progress(s, -EAGAIN);
    }
    if (vsync) {
        horizontal = hrx_reg_read(&loop_receiver, HDMI_VMON_STATUS3);
        vertical = hrx_reg_read(&loop_receiver, HDMI_VMON_STATUS6);
        if (!(horizontal & 65535) || !(vertical & 65535) ||
            (horizontal & 65535) >= (horizontal >> 16) ||
            (vertical & 65535) >= (vertical >> 16)) {
            pr_warn("loop-rx: invalid event geometry attempt=%llu horizontal=%#x vertical=%#x\n",
                    (unsigned long long)s->attempt, horizontal, vertical);
            return finish_progress(s, -ENOLCK);
        }
        if (s->frames && (horizontal != s->horizontal || vertical != s->vertical))
            return finish_progress(s, -ENOLCK);
        s->horizontal = horizontal;
        s->vertical = vertical;
        clear_event(HDMI_AVPUNIT_1_INT_STATUS, HDMI_AVPUNIT_1_INT_CLEAR, RX_VSYNC_EVENT);
        hrx_reg_write(&loop_receiver, 1, HDMI_DEFRAMER_VSYNC_CNT_CLEAR);
        if (hrx_reg_read(&loop_receiver, HDMI_AVPUNIT_1_INT_STATUS) & RX_VSYNC_EVENT)
            return finish_progress(s, -EAGAIN);
        /* 与首个 AVI 同时锁存的 VSYNC 可能更早，帧数从后续 VSYNC 开始累加。 */
        if (s->avi_valid && !first_avi && s->frames < 3) s->frames++;
    }
    if (!input_locked()) return finish_progress(s, -ENOLCK);
    if (s->avi_valid && s->frames == 3) return finish_progress(s, 0);
    return 0;
}
