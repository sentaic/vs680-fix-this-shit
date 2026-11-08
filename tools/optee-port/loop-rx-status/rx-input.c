// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/string.h>
#include "rx-phy-context.h"

struct rx_input_sample {
    u32 before, after;
    u32 timing[7];
    u32 video, deframer, avi, payload[4];
    u32 gcp, gcp_payload, mute;
};

/* Packet headers precede payload reads to use the vendor latch order. */
static void read_input_sample(struct rx_input_sample *sample)
{
    static const u32 timing_regs[] = {
        HDMI_VMON_STATUS1, HDMI_VMON_STATUS2, HDMI_VMON_STATUS3,
        HDMI_VMON_STATUS4, HDMI_VMON_STATUS5, HDMI_VMON_STATUS6,
        HDMI_VMON_STATUS7,
    };
    struct syna_hrx_v4l2_dev *ctx = &loop_receiver;
    unsigned i;
    sample->before = hrx_reg_read(ctx, HDMI_MAINUNIT_STATUS);
    for (i = 0; i < ARRAY_SIZE(timing_regs); i++)
        sample->timing[i] = hrx_reg_read(ctx, timing_regs[i]);
    sample->video = hrx_reg_read(ctx, HDMI_VIDEO_STATUS);
    sample->deframer = hrx_reg_read(ctx, HDMI_DEFRAMER_STATUS);
    sample->avi = hrx_reg_read(ctx, HDMI_PKTDEC_AVIIF_PH);
    for (i = 0; i < ARRAY_SIZE(sample->payload); i++)
        sample->payload[i] = hrx_reg_read(ctx, HDMI_PKTDEC_AVIIF_PB(i));
    sample->gcp = hrx_reg_read(ctx, HDMI_PKTDEC_GCP_PH);
    sample->gcp_payload = hrx_reg_read(ctx, HDMI_PKTDEC_GCP_PB(0));
    sample->mute = hrx_reg_read(ctx, HDMI_PKTDEC_GCP_AVMUTE_STATUS);
    sample->after = hrx_reg_read(ctx, HDMI_MAINUNIT_STATUS);
}

static bool sample_locked(const struct rx_input_sample *sample)
{
    u32 mask = HDMI_MAINUNIT_STATUS_TMDSVALID_STABLE;
    return (sample->before & mask) && (sample->after & mask);
}

ssize_t loop_rx_input(char *buffer)
{
    struct rx_input_sample first = {0}, second = {0};
    const u32 *t = second.timing;
    unsigned width, height, total_width, total_height;
    read_input_sample(&first);
    read_input_sample(&second);
    /* Cable presence is read by the pinctrl module; unlocked input is ENOLCK. */
    if (!sample_locked(&first) || !sample_locked(&second)) return -ENOLCK;
    if (memcmp(first.timing, second.timing, sizeof(first.timing)) ||
        first.video != second.video || first.avi != second.avi ||
        memcmp(first.payload, second.payload, sizeof(first.payload)) ||
        first.gcp != second.gcp || first.gcp_payload != second.gcp_payload ||
        first.mute != second.mute)
        return -ENOLCK;
    width = t[2] & 0xffff; total_width = t[2] >> 16;
    height = t[5] & 0xffff; total_height = t[5] >> 16;
    if (!width || !height || width >= total_width || height >= total_height)
        return -ENOLCK;
    /* Report the sync and interlace fields sampled from the receiver. */
    return scnprintf(buffer, PAGE_SIZE,
        "lock_before=%#x lock_after=%#x deframer=%#x video=%#x "
        "hactive=%u htotal=%u hfront=%u hsync_raw=%u hback=%u "
        "vactive=%u vtotal=%u vfront=%u vsync_raw=%u vback=%u vmon7=%#x "
        "avi_header=%#x avi_pb=%08x,%08x,%08x,%08x "
        "gcp_header=%#x gcp_pb0=%#x avmute=%#x\n",
        second.before, second.after, second.deframer, second.video,
        width, total_width, t[0] & 0xffff, t[0] >> 16, t[1] & 0xffff,
        height, total_height, t[3] & 0xffff, t[3] >> 16, t[4] & 0xffff, t[6],
        second.avi, second.payload[0], second.payload[1], second.payload[2],
        second.payload[3], second.gcp, second.gcp_payload, second.mute);
}
