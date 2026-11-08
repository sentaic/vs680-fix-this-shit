// SPDX-License-Identifier: GPL-2.0
#include <linux/bitfield.h>
#include "rx-phy-context.h"

/*
 * Register layout:
 * https://github.com/synaptics-astra/linux_5_15-modules/blob/
 * 75932239d32198cacf2a442f80a25b31bb004854/drivers/hrx-v4l2/hdmirx.h
 */
struct pattern_config {
    u32 control;
    u32 format;
    u32 horizontal;
    u32 hsync;
    u32 vertical;
    u32 vsync;
};

static struct pattern_config original_pattern;
static bool pattern_active;

static int pattern_dimensions(u32 active, u32 total, u32 *dimensions)
{
    if (!active || total <= active)
        return -EINVAL;

    *dimensions = FIELD_PREP(GENMASK(15, 0), active) |
                  FIELD_PREP(GENMASK(31, 16), total - active);
    return 0;
}

static void pattern_read(struct pattern_config *config)
{
    config->control = hrx_reg_read(&loop_receiver, HDMI_PSVG_CONFIG0);
    config->format = hrx_reg_read(&loop_receiver, HDMI_PSVG_CONFIG1);
    config->horizontal = hrx_reg_read(&loop_receiver, HDMI_PSVG_HAHB_CONFIG);
    config->hsync = hrx_reg_read(&loop_receiver, HDMI_PSVG_HDHW_CONFIG);
    config->vertical = hrx_reg_read(&loop_receiver, HDMI_PSVG_VAVB_CONFIG);
    config->vsync = hrx_reg_read(&loop_receiver, HDMI_PSVG_VDVW_CONFIG);
}

static void pattern_apply(const struct pattern_config *config)
{
    u32 control = hrx_reg_read(&loop_receiver, HDMI_PSVG_CONFIG0);

    hrx_reg_write(&loop_receiver, control & ~HDMI_PSVG_CONFIG0_EN_MASK,
                  HDMI_PSVG_CONFIG0);
    hrx_reg_write(&loop_receiver, config->horizontal, HDMI_PSVG_HAHB_CONFIG);
    hrx_reg_write(&loop_receiver, config->hsync, HDMI_PSVG_HDHW_CONFIG);
    hrx_reg_write(&loop_receiver, config->vertical, HDMI_PSVG_VAVB_CONFIG);
    hrx_reg_write(&loop_receiver, config->vsync, HDMI_PSVG_VDVW_CONFIG);
    hrx_reg_write(&loop_receiver, config->format, HDMI_PSVG_CONFIG1);
    hrx_reg_write(&loop_receiver, config->control, HDMI_PSVG_CONFIG0);
}

int loop_rx_pattern_enable(void)
{
    struct pattern_config config;
    u32 measured;
    int result;

    if (pattern_active)
        return -EBUSY;
    if (!(hrx_reg_read(&loop_receiver, HDMI_MAINUNIT_STATUS) &
          HDMI_MAINUNIT_STATUS_TMDSVALID_STABLE))
        return -ENOLCK;

    measured = hrx_reg_read(&loop_receiver, HDMI_VMON_STATUS3);
    result = pattern_dimensions(
        FIELD_GET(HDMI_VMON_STATUS3_HACTIVE_MASK, measured),
        FIELD_GET(HDMI_VMON_STATUS3_HTOTAL_MASK, measured), &config.horizontal);
    if (result)
        return result;
    measured = hrx_reg_read(&loop_receiver, HDMI_VMON_STATUS6);
    result = pattern_dimensions(
        FIELD_GET(HDMI_VMON_STATUS6_VACTIVE_MASK, measured),
        FIELD_GET(HDMI_VMON_STATUS6_VTOTAL_MASK, measured), &config.vertical);
    if (result)
        return result;

    config.hsync = hrx_reg_read(&loop_receiver, HDMI_VMON_STATUS1);
    config.vsync = hrx_reg_read(&loop_receiver, HDMI_VMON_STATUS4);
    config.format = FIELD_PREP(HDMI_PSVG_CONFIG1_PATT_MODE_QST_MASK, 0);
    config.control =
        FIELD_PREP(HDMI_PSVG_CONFIG0_CONNECTION_SEL_QST_MASK, 3) |
        FIELD_PREP(HDMI_PSVG_CONFIG0_COLORDEPTH_QST_MASK, 4) |
        HDMI_PSVG_CONFIG0_HS_POL_QST_MASK |
        HDMI_PSVG_CONFIG0_VS_POL_QST_MASK |
        HDMI_PSVG_CONFIG0_DE_POL_QST_MASK |
        HDMI_PSVG_CONFIG0_EN_MASK;

    pattern_read(&original_pattern);
    pattern_apply(&config);
    pattern_active = true;
    return 0;
}

void loop_rx_pattern_restore(void)
{
    if (pattern_active) {
        pattern_apply(&original_pattern);
        pattern_active = false;
    }
}
