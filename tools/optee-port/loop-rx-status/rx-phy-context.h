// SPDX-License-Identifier: GPL-2.0
#ifndef LOOP_RX_PHY_CONTEXT_H
#define LOOP_RX_PHY_CONTEXT_H
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/errno.h>
typedef u32 UNSG32;
typedef u16 UNSG16;
typedef u8 UNSG8;
#include "hdmirx.h"
#define HRX_DRV_H
struct syna_hrx_v4l2_dev {
    void __iomem *receiver;
    void __iomem *wrapper;
    u32 phy_cfg_clk;
    bool phy_reset, is_hdmi2;
};
extern struct syna_hrx_v4l2_dev loop_receiver;
#define HRX_DRV_INFO 0
#define HRX_DRV_ERROR 1
#define HRX_DRV_DEBUG 2
#define HRX_LOG(level, format, ...) pr_info("loop-rx: " format, ##__VA_ARGS__)
/* Base address of the wrapper registers used by the PHY. */
#define HDMIRX_WRAP_REG_BASE 0
#define HRX_REG_WORD32_READ(offset, value) (*(value) = readl(loop_receiver.wrapper + (offset)))
#define HRX_REG_WORD32_WRITE(offset, value) writel((value), loop_receiver.wrapper + (offset))
static inline u32 hrx_reg_read(struct syna_hrx_v4l2_dev *context, u32 reg)
{ return readl(context->receiver + reg); }
static inline void hrx_reg_write(struct syna_hrx_v4l2_dev *context, u32 value, u32 reg)
{ writel(value, context->receiver + reg); }
static inline u32 hrx_reg_mask_read(struct syna_hrx_v4l2_dev *context,
                                   u32 reg, u32 shift, u32 mask)
{ return (hrx_reg_read(context, reg) & mask) >> shift; }
static inline void hrx_reg_mask_write(struct syna_hrx_v4l2_dev *context,
                                      u32 value, u32 reg, u32 shift, u32 mask)
{
    u32 original = hrx_reg_read(context, reg);
    hrx_reg_write(context, (original & ~mask) | ((value << shift) & mask), reg);
}
static inline bool hrx_is_hdmi2(struct syna_hrx_v4l2_dev *context)
{
    return !!(hrx_reg_read(context, HDMI_SCDC_REGBANK_STATUS1) &
              HDMI_SCDC_REGBANK_STATUS1_TMDSBITCLKRATIO);
}
#define HRX_REG_H
#endif
