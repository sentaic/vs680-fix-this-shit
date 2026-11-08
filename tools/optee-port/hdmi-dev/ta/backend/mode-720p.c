/* SPDX-License-Identifier: GPL-2.0 */
/* 寄存器定义和配置依据 Synaptics / Synopsys 公开驱动。 */
/* Copyright (C) 2025 Synopsys, Inc. */
/* Copyright (C) 2025 Synaptics Incorporated */

#include "mode-720p.h"
#include "pixel-clock.h"
#include <stddef.h>
#include <stdbool.h>

/*
 * 官方来源：
 * https://github.com/synaptics-astra/tee-optee_dev/tree/
 * e397232786bfee0599dc9d51eb3ca2d6db008eda/ta/display
 * 以下相对路径均以该目录为根，芯片选择 common/include/dolphin。
 * HDMI offset已是字节偏移，见 vendor_hal/synopsys/access_ipk.c。
 */

/* ta/vpp/g7/vppdrv/source/BE/hdmi/vendor/synopsys/api/src/core 与 phy。 */
#define FC_AVICONF0             UINT32_C(0x00004064)
#define FC_AVICONF1             UINT32_C(0x00004068)
#define FC_AVICONF2             UINT32_C(0x0000406C)
#define FC_AVICONF3             UINT32_C(0x0000405C)
#define FC_AVIVID               UINT32_C(0x00004070)
#define FC_CH0PREAM             UINT32_C(0x00004050)
#define FC_CH1PREAM             UINT32_C(0x00004054)
#define FC_CH2PREAM             UINT32_C(0x00004058)
#define FC_CTRLDUR              UINT32_C(0x00004044)
#define FC_DATAUTO0             UINT32_C(0x000042CC)
#define FC_DATAUTO1             UINT32_C(0x000042D0)
#define FC_DATAUTO2             UINT32_C(0x000042D4)
#define FC_DATAUTO3             UINT32_C(0x000042DC)
#define FC_EXCTRLDUR            UINT32_C(0x00004048)
#define FC_EXCTRLSPAC           UINT32_C(0x0000404C)
#define FC_GCP                  UINT32_C(0x00004060)
#define FC_HSYNCINDELAY0        UINT32_C(0x00004020)
#define FC_HSYNCINDELAY1        UINT32_C(0x00004024)
#define FC_HSYNCINWIDTH0        UINT32_C(0x00004028)
#define FC_HSYNCINWIDTH1        UINT32_C(0x0000402C)
#define FC_INHACTIV0            UINT32_C(0x00004004)
#define FC_INHACTIV1            UINT32_C(0x00004008)
#define FC_INHBLANK0            UINT32_C(0x0000400C)
#define FC_INHBLANK1            UINT32_C(0x00004010)
#define FC_INVACTIV0            UINT32_C(0x00004014)
#define FC_INVACTIV1            UINT32_C(0x00004018)
#define FC_INVBLANK             UINT32_C(0x0000401C)
#define FC_INVIDCONF            UINT32_C(0x00004000)
#define FC_PACKET_TX_EN         UINT32_C(0x0000438C)
#define FC_PRCONF               UINT32_C(0x00004380)
#define FC_VSYNCINDELAY         UINT32_C(0x00004030)
#define FC_VSYNCINWIDTH         UINT32_C(0x00004034)
#define MC_CLKDIS               UINT32_C(0x00010004)
#define MC_FLOWCTRL             UINT32_C(0x00010010)
#define MC_PHYRSTZ              UINT32_C(0x00010014)
#define MC_SWRSTZREQ            UINT32_C(0x00010008)
#define PHY_CONF0               UINT32_C(0x0000C000)
#define PHY_STAT0               UINT32_C(0x0000C010)
#define TX_BCBDATA0             UINT32_C(0x00000818)
#define TX_BCBDATA1             UINT32_C(0x0000081C)
#define TX_GYDATA0              UINT32_C(0x00000808)
#define TX_GYDATA1              UINT32_C(0x0000080C)
#define TX_INSTUFFING           UINT32_C(0x00000804)
#define TX_INVID0               UINT32_C(0x00000800)
#define TX_RCRDATA0             UINT32_C(0x00000810)
#define TX_RCRDATA1             UINT32_C(0x00000814)
#define VP_CONF                 UINT32_C(0x00002010)
#define VP_PR_CD                UINT32_C(0x00002004)
#define VP_REMAP                UINT32_C(0x0000200C)
#define VP_STUFF                UINT32_C(0x00002008)

#define IH_I2CMPHY_STAT0        UINT32_C(0x00000420)
#define PHY_I2CM_SLAVE          UINT32_C(0x0000c080)
#define PHY_I2CM_ADDRESS        UINT32_C(0x0000c084)
#define PHY_I2CM_DATAO_1        UINT32_C(0x0000c088)
#define PHY_I2CM_DATAO_0        UINT32_C(0x0000c08c)
#define PHY_I2CM_DATAI_1        UINT32_C(0x0000c090)
#define PHY_I2CM_DATAI_0        UINT32_C(0x0000c094)
#define PHY_I2CM_OPERATION      UINT32_C(0x0000c098)
#define PHY_I2CM_INT            UINT32_C(0x0000c09c)
#define PHY_I2CM_CTLINT         UINT32_C(0x0000c0a0)
#define JTAG_PHY_CONFIG         UINT32_C(0x0000c0d0)
#define FC_SCRAMBLER_CTRL       UINT32_C(0x00004384)

#define SNPS_BASE UINT32_C(0xf7440000)
#define HDMITX_WRAP_VID_CTRL1 UINT32_C(0xf74aca0c)
static bool mode_configured;
static bool mode_enabled;
static struct hdmi_mode_io mode_owner;

#define STEP(call) do { TEE_Result step_result = (call); \
    if (step_result != TEE_SUCCESS) return step_result; } while (0)

static TEE_Result mode_check_io(const struct hdmi_mode_io *io)
{
    if (io == NULL || io->read32 == NULL || io->write32 == NULL ||
        io->wait_ms == NULL)
        return TEE_ERROR_BAD_PARAMETERS;
    return TEE_SUCCESS;
}

static TEE_Result mode_write(const struct hdmi_mode_io *io, uint32_t address,
                             uint32_t value)
{
    return io->write32(io->opaque, address, value);
}

static TEE_Result mode_update(const struct hdmi_mode_io *io, uint32_t address,
                              uint32_t mask, uint32_t value)
{
    uint32_t old_value;
    STEP(io->read32(io->opaque, address, &old_value));
    return mode_write(io, address, (old_value & ~mask) | (value & mask));
}

static TEE_Result snps_update(const struct hdmi_mode_io *io, uint32_t offset,
                              uint32_t mask, uint32_t value)
{
    return mode_update(io, SNPS_BASE + offset, mask, value);
}

/* 按 wait_ms 间隔读取硬件状态，直到条件满足或超时。 */
static TEE_Result mode_poll(const struct hdmi_mode_io *io, uint32_t address,
                            uint32_t mask, uint32_t expected, uint32_t tries)
{
    uint32_t value, i;
    for (i = 0; i < tries; ++i) {
        STEP(io->read32(io->opaque, address, &value));
        if ((value & mask) == expected)
            return TEE_SUCCESS;
        if (i + 1u != tries)
            STEP(io->wait_ms(io->opaque, 1));
    }
    return TEE_ERROR_TIMEOUT;
}

/* core/fc_video.c、video.c、video_sampler.c、video_packetizer.c。 */
static TEE_Result mode_hdmi_core(const struct hdmi_mode_io *io)
{
    static const struct { uint32_t reg, mask, value; } fields[] = {
        { FC_INVIDCONF, 0xfb, 0x78 },
        { FC_INHACTIV0, 0xff, 0x00 },
        { FC_INHACTIV1, 0x1f, 0x05 },
        { FC_INHBLANK0, 0xff, 0x72 },
        { FC_INHBLANK1, 0x1f, 0x01 },
        { FC_INVACTIV0, 0xff, 0xd0 },
        { FC_INVACTIV1, 0x1f, 0x02 },
        { FC_INVBLANK, 0xff, 30 },
        { FC_HSYNCINDELAY0, 0xff, 110 },
        { FC_HSYNCINDELAY1, 0x1f, 0 },
        { FC_HSYNCINWIDTH0, 0xff, 40 },
        { FC_HSYNCINWIDTH1, 0x01, 0 },
        { FC_VSYNCINDELAY, 0xff, 5 },
        { FC_VSYNCINWIDTH, 0x3f, 5 },
        { FC_CTRLDUR, 0xff, 12 },
        { FC_EXCTRLDUR, 0xff, 32 },
        { FC_EXCTRLSPAC, 0xff, 1 },
        { FC_CH0PREAM, 0xff, 11 },
        { FC_CH1PREAM, 0x3f, 22 },
        { FC_CH2PREAM, 0x3f, 33 },
        { FC_PRCONF, 0xff, 0x10 },
        { VP_PR_CD, 0xff, 0 },
        { VP_STUFF, 0x27, 0x07 },
        { VP_REMAP, 0x03, 0 },
        { VP_CONF, 0x7f, 0x47 },
        { TX_INVID0, 0x9f, 1 },
        { TX_GYDATA0, 0xff, 0 }, { TX_GYDATA1, 0xff, 0 },
        { TX_RCRDATA0, 0xff, 0 }, { TX_RCRDATA1, 0xff, 0 },
        { TX_BCBDATA0, 0xff, 0 }, { TX_BCBDATA1, 0xff, 0 },
        { TX_INSTUFFING, 0x07, 0x07 },
        /* core/fc_avi.c：RGB、16:9、全范围(Q=2)、VIC 4、无重复。 */
        { FC_AVICONF0, 0xff, 0x40 },
        { FC_AVICONF1, 0xff, 0x28 },
        { FC_AVICONF2, 0xff, 0x08 },
        { FC_AVICONF3, 0x0f, 0 },
        { FC_AVIVID, 0xff, 4 },
        { FC_DATAUTO0, 0x1f, 0 },
        { FC_DATAUTO1, 0x0f, 0 },
        { FC_DATAUTO2, 0xff, 0x10 },
        { FC_DATAUTO3, 0x3f, 0x0c },
        { FC_PACKET_TX_EN, 0xff, 0x06 },
        { FC_GCP, 0x07, 0x02 }
    };
    uint32_t i;
    for (i = 0; i < sizeof(fields) / sizeof(fields[0]); ++i)
        STEP(snps_update(io, fields[i].reg, fields[i].mask, fields[i].value));
    return TEE_SUCCESS;
}

/* phy/phy_i2c.c：bit0为错误，bit1为完成，状态寄存器写1清除。 */
static TEE_Result mode_phy_wait_i2c(const struct hdmi_mode_io *io)
{
    uint32_t status, i;
    for (i = 0; i < 400u; ++i) {
        STEP(io->wait_ms(io->opaque, 2));
        STEP(io->read32(io->opaque, SNPS_BASE + IH_I2CMPHY_STAT0, &status));
        status &= 3u;
        if (status != 0) {
            STEP(mode_write(io, SNPS_BASE + IH_I2CMPHY_STAT0, status));
            if ((status & 1u) != 0)
                return TEE_ERROR_COMMUNICATION;
            return TEE_SUCCESS;
        }
    }
    return TEE_ERROR_TIMEOUT;
}

static TEE_Result mode_phy_write_checked(const struct hdmi_mode_io *io,
                                         uint32_t reg, uint32_t value)
{
    uint32_t high, low;
    /* 清除上次操作遗留的状态，随后每个事务均等实际完成。 */
    STEP(mode_write(io, SNPS_BASE + IH_I2CMPHY_STAT0, 3));
    STEP(mode_write(io, SNPS_BASE + PHY_I2CM_ADDRESS, reg));
    STEP(mode_write(io, SNPS_BASE + PHY_I2CM_DATAO_1, (value >> 8) & 0xff));
    STEP(mode_write(io, SNPS_BASE + PHY_I2CM_DATAO_0, value & 0xff));
    STEP(mode_write(io, SNPS_BASE + PHY_I2CM_OPERATION, 0x10));
    STEP(mode_phy_wait_i2c(io));
    STEP(mode_write(io, SNPS_BASE + IH_I2CMPHY_STAT0, 3));
    STEP(mode_write(io, SNPS_BASE + PHY_I2CM_ADDRESS, reg));
    STEP(mode_write(io, SNPS_BASE + PHY_I2CM_OPERATION, 1));
    STEP(mode_phy_wait_i2c(io));
    STEP(io->read32(io->opaque, SNPS_BASE + PHY_I2CM_DATAI_1, &high));
    STEP(io->read32(io->opaque, SNPS_BASE + PHY_I2CM_DATAI_0, &low));
    if ((((high & 0xffu) << 8) | (low & 0xffu)) != value)
        return TEE_ERROR_COMMUNICATION;
    return TEE_SUCCESS;
}

/* 使用 phy_316.c 的 74.25 MHz、8 位参数，以及 BE_HDMITX_init 的低速模拟参数；校准后保持 TX 断电，输出启动时再上电。 */
static TEE_Result mode_phy_configure(const struct hdmi_mode_io *io)
{
    static const struct { uint32_t reg, value; } settings[] = {
        { 0x04, 0x0000 }, { 0x06, 0x0002 }, { 0x10, 0x1142 },
        { 0x11, 0x0214 }, { 0x19, 0x0007 }, { 0x0e, 0x0180 },
        { 0x09, 0x8188 }
    };
    uint32_t i;
    STEP(snps_update(io, MC_PHYRSTZ, 1, 1));
    STEP(snps_update(io, PHY_CONF0, 0x08, 0));
    STEP(snps_update(io, PHY_CONF0, 0x10, 0x10));
    STEP(snps_update(io, PHY_CONF0, 0x20, 0x20));
    /* phy_initialize: DE正极性，普通接口。 */
    STEP(snps_update(io, PHY_CONF0, 0x03, 0x02));
    /* TX 初始化时启用 HPD 和接收端检测。 */
    STEP(snps_update(io, PHY_CONF0, 0x04, 0x04));
    STEP(snps_update(io, MC_PHYRSTZ, 1, 0));
    STEP(mode_write(io, SNPS_BASE + JTAG_PHY_CONFIG, 0x10));
    STEP(snps_update(io, PHY_I2CM_SLAVE, 0x7f, 0x69));
    STEP(snps_update(io, PHY_I2CM_INT, 0x04, 0));
    STEP(snps_update(io, PHY_I2CM_CTLINT, 0x44, 0));
    for (i = 0; i < sizeof(settings) / sizeof(settings[0]); ++i)
        STEP(mode_phy_write_checked(io, settings[i].reg, settings[i].value));
    /* 等待 PHY 校准，随后由输出启动流程设置 TXPWRON。 */
    STEP(io->wait_ms(io->opaque, 1));
    return snps_update(io, FC_SCRAMBLER_CTRL, 1, 0);
}

#define VPP_GFX1_CTRL          UINT32_C(0xf7491000)
#define VPP_GFX1_SCL_BYPASS    UINT32_C(0xf7491004)
#define VPP_GFX1_SCL           UINT32_C(0xf7491008)
#define VPP_GFX1_READ_WORDS    UINT32_C(0xf749100c)
#define VPP_GFX1_PIXELS        UINT32_C(0xf7491010)
#define VPP_GFX1_CTRL1         UINT32_C(0xf7491018)
#define VPP_GFX1_DUMMY_TG      UINT32_C(0xf7491800)
#define VPP_CPCB_CTRL0         UINT32_C(0xf7492a38)
#define VPP_CPCB_CTRL2         UINT32_C(0xf7492a3c)
#define VPP_CPCB_GFX1_PIXELS   UINT32_C(0xf7492a48)
#define VPP_CPCB_OVL           UINT32_C(0xf7492a5c)
#define VPP_CPCB_BETG          UINT32_C(0xf7492a90)
#define VPP_CPCB_TG            UINT32_C(0xf7492b20)
#define VPP_VOP_BETG           UINT32_C(0xf7496014)
#define VPP_VOP_TG             UINT32_C(0xf74960a4)
#define VPP_CPCB_CLOCK         UINT32_C(0xf74960ec)
#define VPP_DISPLAY_TG        UINT32_C(0xf7480c84)

/* FE/source/vpp_fe_dlr.c:FE_DLR_ClearChannel，保留同寄存器的其他位。 */
static TEE_Result mode_gfx1_clear(const struct hdmi_mode_io *io)
{
    uint32_t value;
    STEP(io->read32(io->opaque, VPP_GFX1_SCL, &value));
    /* 两个start是脉冲；清理时保持为0。 */
    value &= ~UINT32_C(0x0f);
    STEP(mode_write(io, VPP_GFX1_SCL, value | 0x0a));
    return mode_write(io, VPP_GFX1_SCL, value);
}

/* FE_DLR_SetClientSize、SetPlaneSize、SetPlanePackSel、SetDummyTG_gfx1。 */
static TEE_Result mode_gfx1_geometry(const struct hdmi_mode_io *io)
{
    STEP(mode_gfx1_clear(io));
    STEP(mode_update(io, VPP_GFX1_CTRL, 0x06, 3u << 1));
    STEP(mode_update(io, VPP_GFX1_CTRL1, 0x0ca0, 0x00a0));
    STEP(mode_write(io, VPP_GFX1_READ_WORDS, 172800));
    STEP(mode_write(io, VPP_GFX1_PIXELS, 921600));
    /* 前端dummyTG采用官方20/6偏移，独立于HDMI总长。 */
    STEP(mode_write(io, VPP_GFX1_DUMMY_TG + 0x04, (1300u << 16) | 726u));
    STEP(mode_write(io, VPP_GFX1_DUMMY_TG + 0x0c, (7u << 16) | 1288u));
    STEP(mode_write(io, VPP_GFX1_DUMMY_TG + 0x10, (7u << 16) | 1288u));
    STEP(mode_write(io, VPP_GFX1_DUMMY_TG + 0x20, (2u << 16) | 723u));
    STEP(mode_write(io, VPP_GFX1_DUMMY_TG + 0x24, (2u << 16) | 723u));
    /* MISC/source/vpp_clkGate.c:VPP_ClkGate_GFXScalar，1:1旁路。 */
    STEP(mode_update(io, VPP_GFX1_SCL_BYPASS, 1, 1));
    return mode_update(io, VPP_GFX1_CTRL, UINT32_C(1) << 23, 0);
}

/* 720p 使用完整消隐；TG_PRG_FT 按官方 8 位字段编码像素数，921600 编码为 0，高 24 位保留。 */
static TEE_Result mode_tg_geometry(const struct hdmi_mode_io *io,
                                  uint32_t base, int display)
{
    STEP(mode_update(io, base + 0x08, 0x01ffffff, (1649u << 12) | 749u));
    STEP(mode_update(io, base + 0x0c, 0x01ffffff, 1641));
    STEP(mode_update(io, base + 0x1c, 0xff, 921600u & 0xffu));
    STEP(mode_update(io, base, UINT32_MAX, (727u << 10) | (15u << 2)));
    STEP(mode_update(io, base + 0x04, 0x7ff, 0));
    STEP(mode_update(io, base + 0x10, 0x03ffffff, 1390u | (1430u << 13)));
    STEP(mode_update(io, base + 0x14, 0x00ffffff, 725u | (730u << 12)));
    if (display)
        STEP(mode_update(io, base + 0x18, 0x03ffffff, 1390u | (1390u << 13)));
    return mode_update(io, base + 0x20, 0x1fff, 1390);
}

static TEE_Result mode_plane_window(const struct hdmi_mode_io *io,
                                    uint32_t base, uint32_t offset)
{
    STEP(mode_update(io, base + offset, 0x03ffffff, 1280u << 13));
    return mode_update(io, base + offset + 4, 0x00ffffff, 720u << 12);
}

/* CPCB_TG_SetPlaneWindow、CPCB_OVL_SetAlpha、SetAlphaPolarity、SetAlphaMode。 */
static TEE_Result mode_cpcb_geometry(const struct hdmi_mode_io *io)
{
    uint32_t i;
    static const uint32_t bases[] = { VPP_CPCB_BETG, VPP_VOP_BETG };
    STEP(mode_update(io, VPP_CPCB_CTRL2, 0xff, 0x40));
    STEP(mode_write(io, VPP_CPCB_CLOCK, 0x04));
    STEP(mode_tg_geometry(io, VPP_CPCB_TG, 0));
    STEP(mode_tg_geometry(io, VPP_VOP_TG, 0));
    STEP(mode_tg_geometry(io, VPP_DISPLAY_TG, 1));
    for (i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        STEP(mode_plane_window(io, bases[i], 0x18));
        STEP(mode_plane_window(io, bases[i], 0x58));
        STEP(mode_plane_window(io, bases[i], 0x38));
    }
    /* 公共输出的BASE、BASE_BORDER、OVL窗口；复位已清零这些窗口。 */
    STEP(mode_plane_window(io, VPP_VOP_BETG, 0));
    STEP(mode_plane_window(io, VPP_VOP_BETG, 0x20));
    STEP(mode_plane_window(io, VPP_VOP_BETG, 0x40));
    STEP(mode_plane_window(io, VPP_CPCB_BETG, 0));
    STEP(mode_plane_window(io, VPP_CPCB_BETG, 0x20));
    STEP(mode_plane_window(io, VPP_CPCB_BETG, 0x40));
    STEP(mode_plane_window(io, VPP_DISPLAY_TG - 0x90, 0));
    STEP(mode_update(io, UINT32_C(0xf7480bf0), 2, 0));
    STEP(mode_write(io, VPP_CPCB_GFX1_PIXELS, 921600));
    STEP(mode_update(io, VPP_CPCB_OVL + 0x2c, 3, 2));
    STEP(mode_update(io, VPP_CPCB_OVL + 0x14, 0xffff, 0x210));
    STEP(mode_update(io, VPP_CPCB_OVL + 4, 0x0fffc000, 0x0fff0000));
    STEP(mode_update(io, VPP_CPCB_OVL, 4, 0));
    return mode_update(io, VPP_CPCB_OVL + 0x30, 4, 0);
}

/* 关闭显示和前端入口，外层后端随后终止 dHub 访问。 */
static TEE_Result mode_output_off(const struct hdmi_mode_io *io)
{
    STEP(snps_update(io, FC_GCP, 3, 2));
    STEP(snps_update(io, PHY_CONF0, 8, 0));
    STEP(snps_update(io, PHY_CONF0, 0x10, 0x10));
    STEP(mode_poll(io, SNPS_BASE + PHY_STAT0, 1, 0, 101));
    STEP(snps_update(io, MC_CLKDIS, 0x5f, 0x5f));
    STEP(mode_update(io, VPP_CPCB_CTRL2, 0xff, 0x40));
    return mode_gfx1_clear(io);
}

/* 按 RES_RESET 设置 active=0、total=1，并检查 Display TG 的 H/V Total 清零；dHub 停止状态由外层检查。 */
static TEE_Result mode_tg_reset(const struct hdmi_mode_io *io)
{
    static const uint32_t tg[] = {
        VPP_CPCB_TG, VPP_VOP_TG, VPP_DISPLAY_TG
    };
    static const uint32_t windows[] = {
        VPP_CPCB_BETG, VPP_VOP_BETG, VPP_DISPLAY_TG - 0x90
    };
    uint32_t i, j, value;
    /* CPCB_TG.c:VOP_TG_Enable(0)，clear为脉冲，start保持0。 */
    STEP(io->read32(io->opaque, UINT32_C(0xf7480a94), &value));
    value &= ~UINT32_C(3);
    STEP(mode_write(io, UINT32_C(0xf7480a94), value | 2));
    STEP(mode_write(io, UINT32_C(0xf7480a94), value));
    for (i = 0; i < sizeof(tg) / sizeof(tg[0]); ++i) {
        STEP(mode_update(io, tg[i] + 8, 0x01ffffff,
                         i == 2 ? 0 : (30u << 12) | 6u));
        STEP(mode_update(io, tg[i] + 12, 0x01ffffff, 0));
        STEP(mode_update(io, tg[i] + 28, 0xff, 0));
        STEP(mode_write(io, tg[i], (7u << 10) | (15u << 2)));
        STEP(mode_update(io, tg[i] + 4, 0x7ff, 0));
        STEP(mode_update(io, tg[i] + 16, 0x03ffffff, 0));
        STEP(mode_update(io, tg[i] + 20, 0x00ffffff, 0));
        STEP(mode_update(io, tg[i] + 32, 0x1fff, 0));
    }
    STEP(mode_update(io, VPP_DISPLAY_TG + 24, 0x03ffffff, 0));
    STEP(mode_update(io, UINT32_C(0xf7480bf0), 2, 0));
    for (i = 0; i < sizeof(windows) / sizeof(windows[0]); ++i) {
        /* PL0..PL8及四个crop窗口；这些窗口均为TG_PL_X/Y。 */
        for (j = 0; j < 13u; ++j) {
            STEP(mode_update(io, windows[i] + j * 8, 0x03ffffff, 0));
            STEP(mode_update(io, windows[i] + j * 8 + 4, 0x00ffffff, 0));
        }
    }
    STEP(mode_write(io, VPP_CPCB_GFX1_PIXELS, 0));
    STEP(mode_write(io, UINT32_C(0xf7480a98), 0));
    return mode_poll(io, VPP_DISPLAY_TG + 8, 0x01ffffff, 0, 101);
}

/*
 * FE_DLR_SetBitmap的ORDER_RGBA恒等映射；三字节内存顺序为R/G/B。
 * ARGB参考表会将24位输入移成0/R/G，丢掉第三分量。
 */
static TEE_Result mode_rgb_route(const struct hdmi_mode_io *io)
{
    static const uint8_t bitmap[32] = {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
        16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    };
    static const uint32_t coeff[9] = {
        0, 0x2000, 0, 0x2000, 0, 0, 0, 0, 0x2000
    };
    uint32_t i, j, value;
    /* dolphin/vpp.h的GFX RGB通路reset默认旁路；SCL改为1:1硬旁路。 */
    STEP(mode_update(io, VPP_GFX1_CTRL, 0x7fe0000f, 0x6e000007));
    STEP(mode_update(io, VPP_GFX1_CTRL1, 0x3fa0, 0x10a0));
    STEP(mode_update(io, UINT32_C(0xf7491014), 1, 0));
    for (i = 0; i < 8u; ++i) {
        value = 0;
        for (j = 0; j < 5u && i * 5u + j < 32u; ++j)
            value |= (uint32_t)bitmap[i * 5u + j] << (j * 6u);
        STEP(mode_write(io, UINT32_C(0xf7491030) + i * 4u, value));
    }
    /* PLANE_GFX1对应门控字段GFX2(bit1)，OVL输入bit9=0选择GFX1。 */
    STEP(mode_update(io, UINT32_C(0xf7490000), 0x202, 2));
    STEP(mode_update(io, UINT32_C(0xf749101c), 1, 1));
    STEP(mode_update(io, UINT32_C(0xf7496010), 0x3f, 0x20));
    STEP(mode_update(io, UINT32_C(0xf7490004), 0x1f, 0x1f));
    STEP(mode_update(io, VPP_CPCB_CTRL0, 0x7f, 0));
    /* VPP_BE_Reset清LVDS选择；VOP_CTRL恢复公开HDMI默认使能字段。 */
    STEP(mode_update(io, UINT32_C(0xf7480a7c), 3, 0));
    STEP(mode_update(io, UINT32_C(0xf7480a00), 0x622, 0x222));
    /* BE_SetVoutInput:primary progressive=0，12→8 rounding=1，MSB align=1。 */
    STEP(mode_update(io, UINT32_C(0xf7480a80), 0x007fffff, 0x00140800));
    STEP(mode_update(io, UINT32_C(0xf7480a8c), 0x7ff, 0x48f));
    /* DUMMY/DV/VPP_DV_DitherCfg：保留OCSC启用，EDR/dither/Clamp旁路。 */
    STEP(mode_update(io, UINT32_C(0xf7480a90), 0xff, 0x7e));
    STEP(mode_update(io, UINT32_C(0xf7480a9c), 0x1ff0, 0x320));
    STEP(mode_write(io, UINT32_C(0xf7480a98), 921600));
    STEP(mode_update(io, UINT32_C(0xf7480aac), 0x0f, 0x0d));
    /* vpp_dv_ocsc.c：RGB参考矩阵；Q13单位增益精确为8192，避免截断减1。 */
    STEP(mode_write(io, UINT32_C(0xf7480b48), 0x46050001));
    for (i = 0; i < 9u; ++i)
        STEP(mode_write(io, UINT32_C(0xf7480b94) + i * 4u, coeff[i]));
    STEP(mode_update(io, UINT32_C(0xf7480bb8), 0x7c00, 0x3400));
    STEP(mode_write(io, UINT32_C(0xf7480bbc), 0));
    STEP(mode_write(io, UINT32_C(0xf7480bc0), 0));
    return mode_write(io, UINT32_C(0xf7480bd8), 0x10000);
}

static TEE_Result mode_expect(const struct hdmi_mode_io *io, uint32_t address,
                              uint32_t mask, uint32_t expected)
{
    uint32_t value;
    STEP(io->read32(io->opaque, address, &value));
    return (value & mask) == expected ? TEE_SUCCESS : TEE_ERROR_BAD_STATE;
}

TEE_Result hdmi_mode_720p_configure(const struct hdmi_mode_io *io)
{
    mode_configured = false;
    mode_enabled = false;
    STEP(mode_check_io(io));
    /* 复位后检查 CFG7 默认字段。 */
    STEP(mode_expect(io, UINT32_C(0xf7480a9c), 0x1fff, 0x324));
    STEP(mode_output_off(io));
    /* 由 pixel-clock.c 配置 PLL，成功后继续设置路由。 */
    STEP(hdmi_pixel_clock_configure(io, HDMI_MODE_720P_PIXEL_CLOCK_HZ));
    /* vpp_clk.c:MV_VPP_Clk_Ctrl。wrapper源选择/输出使能独立于PLL分频。 */
    STEP(mode_update(io, UINT32_C(0xf749c000), 0x1fff, 0x820));
    /* MV_VPP_Clk_Ctrl的公共输出时钟使能，AVIO reset后该字段为0。 */
    STEP(mode_update(io, UINT32_C(0xf749c0ec), 0xff, 0x4d));
    STEP(mode_gfx1_geometry(io));
    STEP(mode_cpcb_geometry(io));
    STEP(mode_rgb_route(io));
    STEP(mode_hdmi_core(io));
    /* vpp_be_hdmitx.c:hdmi_tx_ConfigHdmiTxWrapperModule，RGB分量0/2。 */
    STEP(mode_update(io, HDMITX_WRAP_VID_CTRL1, 0x00330000, 0x00200000));
    /* main_controller_reg.h定义bit0=1经过CSC，RGB直通明确清0。 */
    STEP(snps_update(io, MC_FLOWCTRL, 1, 0));
    STEP(mode_phy_configure(io));
    STEP(mode_expect(io, VPP_CPCB_TG + 8, 0x01ffffff, (1649u << 12) | 749u));
    STEP(mode_expect(io, VPP_VOP_TG + 8, 0x01ffffff, (1649u << 12) | 749u));
    STEP(mode_expect(io, VPP_DISPLAY_TG + 8, 0x01ffffff, (1649u << 12) | 749u));
    STEP(mode_expect(io, VPP_GFX1_READ_WORDS, UINT32_MAX, 172800));
    STEP(mode_expect(io, SNPS_BASE + FC_AVICONF2, 0x0c, 8));
    mode_owner = *io;
    mode_configured = true;
    return TEE_SUCCESS;
}

static TEE_Result resume_window(const struct hdmi_mode_resume_window *w)
{
    uint64_t now;
    if(!w)return TEE_SUCCESS;
    if(!w->now_us || w->not_before_us>=w->before_us ||
        w->now_us(w->opaque,&now))return TEE_ERROR_BAD_STATE;
    /* 为毫秒计时精度和最后一段 MMIO 操作预留 1 ms。 */
    if(now<w->not_before_us || now>w->before_us || w->before_us-now<=1000u)
        return TEE_ERROR_BUSY;
    return TEE_SUCCESS;
}
static TEE_Result mode_enable_common(const struct hdmi_mode_io *io, uint32_t vsync_width,
                                    const struct hdmi_mode_resume_window *window)
{
    STEP(mode_check_io(io));
    if (!vsync_width || vsync_width>0x3f || mode_enabled || io->opaque != mode_owner.opaque ||
        io->read32 != mode_owner.read32 || io->write32 != mode_owner.write32 ||
        io->wait_ms != mode_owner.wait_ms)
        return TEE_ERROR_BAD_STATE;
    /* 启动前清除可启动标志；启动中途失败时，通过 disable/configure 重新准备。 */
    mode_configured = false;
    STEP(mode_expect(io, UINT32_C(0xf749c020), 1, 1));
    STEP(snps_update(io, MC_CLKDIS, 0x5f, 0x4c));
    STEP(snps_update(io, PHY_CONF0, 0x10, 0));
    STEP(resume_window(window));
    STEP(snps_update(io, PHY_CONF0, 8, 8));
    STEP(resume_window(window));
    STEP(mode_poll(io, SNPS_BASE + PHY_STAT0, 1, 1, 2001));
    STEP(mode_write(io, SNPS_BASE + MC_SWRSTZREQ, 0));
    STEP(snps_update(io, FC_VSYNCINWIDTH, 0x3f, vsync_width));
    STEP(mode_update(io, VPP_CPCB_CTRL2, 0x1f, 8));
    /* FE/VOP/CPCB与unmute由frame-start在 CPCB0 VBI 上执行。 */
    mode_enabled = true;
    return TEE_SUCCESS;
}

TEE_Result hdmi_mode_720p_enable(const struct hdmi_mode_io *io)
{
    if (!mode_configured) return TEE_ERROR_BAD_STATE;
    return mode_enable_common(io,5,NULL);
}
/* 显示路由关闭后，由调用方写入并检查详细时序。 */
TEE_Result hdmi_mode_enable_prepared(const struct hdmi_mode_io *io, uint32_t vsync_width)
{
    if (mode_configured) return TEE_ERROR_BAD_STATE;
    return mode_enable_common(io,vsync_width,NULL);
}

TEE_Result hdmi_mode_720p_disable(const struct hdmi_mode_io *io)
{
    mode_configured = false;
    mode_enabled = false;
    STEP(mode_check_io(io));
    STEP(mode_output_off(io));
    return mode_tg_reset(io);
}

TEE_Result hdmi_mode_enable_prepared_window(const struct hdmi_mode_io *io,
    uint32_t vsync_width,const struct hdmi_mode_resume_window *window)
{
    if(mode_configured || !window)return TEE_ERROR_BAD_STATE;
    return mode_enable_common(io,vsync_width,window);
}
