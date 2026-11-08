// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include "rx-phy-context.h"

int hrx_phy_init(struct syna_hrx_v4l2_dev *context, bool rate6g, bool full);
void loop_rx_select_legacy_video(void);
/* 检查 PHY 初始化时设置的路由和计时器字段。 */
static int loop_rx_prelink_verify(void)
{
    struct syna_hrx_v4l2_dev *context = &loop_receiver;
    static const struct { u32 reg, mask, value; } fields[] = {
        { HDMI_SCDC_CONFIG, HDMI_SCDC_CONFIG_POWERPROVIDED_MASK,
          1u << HDMI_SCDC_CONFIG_POWERPROVIDED_OFFSET },
        { HDMI_DESCRAND_EN_CONTROL, HDMI_DESCRAND_EN_CONTROL_SCRAMB_EN_MASK,
          1u << HDMI_DESCRAND_EN_CONTROL_SCRAMB_EN_OFFSET },
        { HDMI_CORE_CONFIG, HDMI_CORE_CONFIG_HPD_MASK,
          1u << HDMI_CORE_CONFIG_HPD_OFFSET },
        { HDMI_GLOBAL_TIMER_REF_BASE, HDMI_GLOBAL_TIMER_REF_BASE_MASK,
          250000000 & HDMI_GLOBAL_TIMER_REF_BASE_MASK },
        { HDMI_VMON_CONTROL, HDMI_VMON_CONTROL_SOURCE_SEL_MASK,
          4u << HDMI_VMON_CONTROL_SOURCE_SEL_OFFSET },
        { HDMI_HDCP22_CONFIG,
          HDMI_HDCP22_CONFIG_SWITCH_OVR_VALUE_MASK |
          HDMI_HDCP22_CONFIG_SWITCH_OVR_EN_MASK,
          1u << HDMI_HDCP22_CONFIG_SWITCH_OVR_EN_OFFSET },
    };
    unsigned index;

    for (index = 0; index < ARRAY_SIZE(fields); index++)
        if ((hrx_reg_read(context, fields[index].reg) & fields[index].mask) !=
                fields[index].value)
            return -EIO;
    return 0;
}

/* Controller sequence follows vendor hrx-drv.c. Polling replaces its ISR tasks. */
int loop_rx_start_rate(bool rate6g)
{
    struct syna_hrx_v4l2_dev *context = &loop_receiver;
    unsigned attempt;
    int result;
    u32 ready = HDMI_MAINUNIT_0_INT_STATUS_REGBANK_READY |
                HDMI_MAINUNIT_0_INT_STATUS_APB_IF_READY;
    if (hrx_reg_read(context, HDMI_CORE_ID) != HDMI_CORE_ID_VALUE)
        return -ENODEV;
    hrx_reg_write(context, HDMI_GLOBAL_SWRESET_REQUEST_APB |
        HDMI_GLOBAL_SWRESET_REQUEST_CEC | HDMI_GLOBAL_SWRESET_REQUEST_DATAPATH |
        HDMI_GLOBAL_SWRESET_REQUEST_AUDIO, HDMI_GLOBAL_SWRESET_REQUEST);
    for (attempt = 0; attempt < 50; attempt++) {
        if ((hrx_reg_read(context, HDMI_MAINUNIT_0_INT_STATUS) & ready) == ready)
            break;
        msleep(10);
    }
    if (attempt == 50)
        return -ETIMEDOUT;
    hrx_reg_write(context, 0, HDMI_MAINUNIT_0_INT_MASK_N);
    hrx_reg_write(context, 0, HDMI_MAINUNIT_1_INT_MASK_N);
    hrx_reg_write(context, 0, HDMI_MAINUNIT_2_INT_MASK_N);
    hrx_reg_write(context, 0, HDMI_AVPUNIT_0_INT_MASK_N);
    hrx_reg_write(context, 0, HDMI_AVPUNIT_1_INT_MASK_N);
    hrx_reg_write(context, 0, HDMI_PKT_0_INT_MASK_N);
    hrx_reg_write(context, 0, HDMI_PKT_1_INT_MASK_N);
    hrx_reg_write(context, 0, HDMI_SCDC_INT_MASK_N);
    hrx_reg_write(context, 0, HDMI_HDCP_INT_MASK_N);
    hrx_reg_mask_write(context, 1, HDMI_SCDC_CONFIG,
        HDMI_SCDC_CONFIG_POWERPROVIDED_OFFSET, HDMI_SCDC_CONFIG_POWERPROVIDED_MASK);
    hrx_reg_mask_write(context, 1, HDMI_DESCRAND_EN_CONTROL,
        HDMI_DESCRAND_EN_CONTROL_SCRAMB_EN_OFFSET, HDMI_DESCRAND_EN_CONTROL_SCRAMB_EN_MASK);
    hrx_reg_mask_write(context, 1, HDMI_CORE_CONFIG,
        HDMI_CORE_CONFIG_HPD_OFFSET, HDMI_CORE_CONFIG_HPD_MASK);
    hrx_reg_write(context, HDMI_MAINUNIT_0_INT_CLEAR_TIMER_BASE_LOCKED_MASK,
        HDMI_MAINUNIT_0_INT_CLEAR);
    hrx_reg_write(context, 250000000 & HDMI_GLOBAL_TIMER_REF_BASE_MASK,
        HDMI_GLOBAL_TIMER_REF_BASE);
    for (attempt = 0; attempt < 50; attempt++) {
        if (hrx_reg_read(context, HDMI_CMU_STATUS) & HDMI_CMU_STATUS_TIMER_BASE_LOCKED)
            break;
        msleep(10);
    }
    if (attempt == 50)
        return -ETIMEDOUT;
    hrx_reg_mask_write(context, 4, HDMI_VMON_CONTROL,
        HDMI_VMON_CONTROL_SOURCE_SEL_OFFSET, HDMI_VMON_CONTROL_SOURCE_SEL_MASK);
    result = hrx_phy_init(context, rate6g, true);
    if (result) return result;
    loop_rx_select_legacy_video();
    return loop_rx_prelink_verify();
}

int loop_rx_start(void)
{
    return loop_rx_start_rate(false);
}

int loop_rx_video_request(void)
{
    struct syna_hrx_v4l2_dev *context = &loop_receiver;
    u32 polarity = HDMI_VIDEO_CONFIG2_HSYNC_POL_OVR_MASK |
        HDMI_VIDEO_CONFIG2_HSYNC_POL_OVR_EN_MASK |
        HDMI_VIDEO_CONFIG2_VSYNC_POL_OVR_MASK |
        HDMI_VIDEO_CONFIG2_VSYNC_POL_OVR_EN_MASK;
    hrx_reg_write(context, ~0u, HDMI_PKTEX_BCH_ERRFILT_CONFIG);
    hrx_reg_write(context, ~0u, HDMI_PKTEX_CHKSUM_ERRFILT_CONFIG);
    hrx_reg_mask_write(context, 1, HDMI_VIDEO_CONFIG2,
        HDMI_VIDEO_CONFIG2_HSYNC_POL_OVR_OFFSET, HDMI_VIDEO_CONFIG2_HSYNC_POL_OVR_MASK);
    hrx_reg_mask_write(context, 1, HDMI_VIDEO_CONFIG2,
        HDMI_VIDEO_CONFIG2_HSYNC_POL_OVR_EN_OFFSET, HDMI_VIDEO_CONFIG2_HSYNC_POL_OVR_EN_MASK);
    hrx_reg_mask_write(context, 1, HDMI_VIDEO_CONFIG2,
        HDMI_VIDEO_CONFIG2_VSYNC_POL_OVR_OFFSET, HDMI_VIDEO_CONFIG2_VSYNC_POL_OVR_MASK);
    hrx_reg_mask_write(context, 1, HDMI_VIDEO_CONFIG2,
        HDMI_VIDEO_CONFIG2_VSYNC_POL_OVR_EN_OFFSET, HDMI_VIDEO_CONFIG2_VSYNC_POL_OVR_EN_MASK);
    /* 检查同步极性配置后，请求视频路径复位。 */
    if ((hrx_reg_read(context, HDMI_VIDEO_CONFIG2) & polarity) != polarity)
        return -EIO;
    hrx_reg_write(context, HDMI_GLOBAL_SWRESET_REQUEST_DATAPATH,
        HDMI_GLOBAL_SWRESET_REQUEST);
    return 0;
}

int loop_rx_video_config(void)
{
    int result = loop_rx_video_request();
    if (result) return result;
    /* 为视频路径复位预留 100 ms。 */
    msleep(100);
    return 0;
}

int loop_rx_wait_frames(unsigned frames)
{
    struct syna_hrx_v4l2_dev *context = &loop_receiver;
    u32 previous = hrx_reg_read(context, HDMI_DEFRAMER_CONFIG0);
    u32 event = HDMI_AVPUNIT_1_INT_MASK_N_DEFRAMER_VSYNC_MASK;
    int result = 0;
    unsigned frame;
    if (!frames || frames > 8) return -EINVAL;
    hrx_reg_mask_write(context, 1, HDMI_DEFRAMER_CONFIG0,
        HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_OFFSET,
        HDMI_DEFRAMER_CONFIG0_VS_CNT_THR_MASK);
    for (frame = 0; frame < frames; frame++) {
        unsigned attempt;
        (void)hrx_reg_read(context, HDMI_AVPUNIT_1_INT_STATUS);
        hrx_reg_write(context, event, HDMI_AVPUNIT_1_INT_CLEAR);
        /* VSYNC count clear is a hardware pulse, as in the vendor controller. */
        hrx_reg_write(context, 1, HDMI_DEFRAMER_VSYNC_CNT_CLEAR);
        for (attempt = 0; attempt < 125; attempt++) {
            if (!(hrx_reg_read(context, HDMI_MAINUNIT_STATUS) &
                    HDMI_MAINUNIT_STATUS_TMDSVALID_STABLE)) {
                result = -ENOLCK;
                break;
            }
            if (hrx_reg_read(context, HDMI_AVPUNIT_1_INT_STATUS) & event) break;
            usleep_range(2000, 2500);
        }
        if (result) break;
        if (attempt == 125) { result = -ETIMEDOUT; break; }
    }
    (void)hrx_reg_read(context, HDMI_AVPUNIT_1_INT_STATUS);
    hrx_reg_write(context, event, HDMI_AVPUNIT_1_INT_CLEAR);
    hrx_reg_write(context, previous, HDMI_DEFRAMER_CONFIG0);
    return result;
}
