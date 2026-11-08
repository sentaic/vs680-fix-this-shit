// SPDX-License-Identifier: GPL-2.0
#include "rx-phy-context.h"

static bool saved;
static u32 config14, config22;

void loop_rx_vendor_video_restore(void)
{
    if (!saved) return;
    hrx_reg_write(&loop_receiver, config14, HDMI_HDCP14_CONFIG);
    hrx_reg_write(&loop_receiver, config22, HDMI_HDCP22_CONFIG);
    saved = false;
}

void loop_rx_select_legacy_video(void)
{
    struct syna_hrx_v4l2_dev *context = &loop_receiver;
    /* Vendor hrx_set_hdcp_version(HRX_HDCP_VERSION_14), without authentication. */
    hrx_reg_mask_write(context, 0, HDMI_HDCP22_CONFIG,
        HDMI_HDCP22_CONFIG_SWITCH_OVR_VALUE_OFFSET,
        HDMI_HDCP22_CONFIG_SWITCH_OVR_VALUE_MASK);
    hrx_reg_mask_write(context, 1, HDMI_HDCP22_CONFIG,
        HDMI_HDCP22_CONFIG_SWITCH_OVR_EN_OFFSET,
        HDMI_HDCP22_CONFIG_SWITCH_OVR_EN_MASK);
}

void loop_rx_vendor_video_enable(bool initialize)
{
    struct syna_hrx_v4l2_dev *context = &loop_receiver;
    if (!saved) {
        config14 = hrx_reg_read(context, HDMI_HDCP14_CONFIG);
        config22 = hrx_reg_read(context, HDMI_HDCP22_CONFIG);
        saved = true;
    }
    loop_rx_select_legacy_video();
    if (!initialize) return;
    /* 设置公开驱动初始化序列中的 HDCP 配置字段。 */
    hrx_reg_mask_write(context, 0, HDMI_HDCP14_CONFIG,
        HDMI_HDCP14_CONFIG_KEY_DECRYPT_EN_OFFSET,
        HDMI_HDCP14_CONFIG_KEY_DECRYPT_EN_MASK);
    hrx_reg_mask_write(context, 0, HDMI_HDCP14_CONFIG,
        HDMI_HDCP14_CONFIG_EESS_OESS_SEL_OFFSET,
        HDMI_HDCP14_CONFIG_EESS_OESS_SEL_MASK);
    hrx_reg_mask_write(context, 1, HDMI_HDCP14_CONFIG,
        HDMI_HDCP14_CONFIG_HDMI_RSVD_OFFSET,
        HDMI_HDCP14_CONFIG_HDMI_RSVD_MASK);
    hrx_reg_mask_write(context, 0, HDMI_HDCP14_CONFIG,
        HDMI_HDCP14_CONFIG_FEATURES_1DOT1_OFFSET,
        HDMI_HDCP14_CONFIG_FEATURES_1DOT1_MASK);
    hrx_reg_mask_write(context, 0xc, HDMI_HDCP14_CONFIG,
        HDMI_HDCP14_CONFIG_EESS_CTL_THR_OFFSET,
        HDMI_HDCP14_CONFIG_EESS_CTL_THR_MASK);
}
