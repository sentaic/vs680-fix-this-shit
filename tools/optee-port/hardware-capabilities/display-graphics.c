/* SPDX-License-Identifier: GPL-2.0 */
#include "tee_internal_api.h"
#include "tee_ta_mmap.h"
#include "api_avio_dhub.h"
#include "vpp.h"
#include "vpp_gfx_vop.h"
#include "../../../sources/astra-current/tee-optee_dev/ta/display/ta/vpp/g7/vppdrv/source/DUMMY/DV/include/vpp_gfx_vop_dv_reg.h"
/* 读取四个 VPP 图形寄存器。 */
static TEE_Result loop_display_graphics_command(TEE_Param params[4])
{
    uint32_t base = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_REG_BASE;
    uint32_t gfx = base + RA_Vpp_GFX1;
    uint32_t alpha = base + RA_Vpp_vidpl + RA_vidpl_CPCB0 + RA_CPCB0_OVL1 + RA_OVL_ALPHA_CTRL;
    params[1].value.a = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(gfx + RA_GFX1_CTRL);
    params[1].value.b = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(gfx + RA_GFX1_CTRL1);
    params[2].value.a = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(alpha);
    params[2].value.b = *(volatile uint32_t *)TEE_TA_RegPhyToVirt(gfx + RA_GFX1_gfx_pix);
    return TEE_SUCCESS;
}

static TEE_Result loop_display_mixer_command(TEE_Param params[4])
{
    uint32_t base = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_REG_BASE +
                    RA_Vpp_vidpl + RA_vidpl_CPCB0;
    const uint32_t offsets[9][4] = {
        {RA_CPCB0_OVL1 + RA_OVL_SENSE_CTRL, RA_CPCB0_OVL1 + RA_OVL_LAY,
         RA_CPCB0_OVL1 + RA_OVL_CTRL, RA_CPCB0_OVL1 + RA_OVL_APM},
        {RA_CPCB0_CTRL0, RA_CPCB0_CTRL2, RA_CPCB0_CTRL8, RA_CPCB0_CTRL6},
        {RA_CPCB0_CTRL10, RA_CPCB0_CTRL9, RA_CPCB0_OVL1 + RA_OVL_ALPHA0,
         RA_CPCB0_OVL1 + RA_OVL_ALP_UD1},
        {RA_HDMI_VOP_CFG0, RA_HDMI_VOP_CFG1, RA_HDMI_VOP_CFG4, RA_HDMI_VOP_CFG7},
        {RA_BETG_TG_PRG + RA_TG_PRG_Total, RA_BETG_TG_PRG + RA_TG_PRG_FT,
         RA_BETG_TG_PRG + RA_TG_PRG_HSYNC, RA_BETG_TG_PRG + RA_TG_PRG_VSYNC},
        {RA_BETG_PL0 + RA_TG_PL_X, RA_BETG_PL0 + RA_TG_PL_Y,
         RA_BETG_TG_PRG + RA_TG_PRG_Initial, RA_BETG_TG_PRG + RA_TG_PRG_CTRL1},
        {RA_OCSC_OCSC_RGB2YUV_COEF00, RA_OCSC_OCSC_RGB2YUV_COEF00 + 4,
         RA_OCSC_OCSC_RGB2YUV_COEF00 + 8, RA_OCSC_OCSC_RGB2YUV_OFFSET_0},
        {RA_OCSC_OCSC_RGB2YUV_COEF00 + 12, RA_OCSC_OCSC_RGB2YUV_COEF00 + 16,
         RA_OCSC_OCSC_RGB2YUV_COEF00 + 20, RA_OCSC_OCSC_RGB2YUV_OFFSET_1},
        {RA_OCSC_OCSC_RGB2YUV_COEF00 + 24, RA_OCSC_OCSC_RGB2YUV_COEF00 + 28,
         RA_OCSC_OCSC_RGB2YUV_COEF00 + 32, RA_OCSC_OCSC_SCALE2P}
    };
    uint32_t words[4], group = params[0].value.a;
    if (group >= 9) return TEE_ERROR_BAD_PARAMETERS;
    if (group == 3) base = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_GFX_VOP_REG_BASE +
                           RA_VPP_GFX_VOP_VOP + RA_VOP_HDMI_VOP;
    if (group == 4 || group == 5) base = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_GFX_VOP_REG_BASE +
                           RA_VPP_GFX_VOP_VOP + RA_VOP_DTG + RA_DTG_DISPLAY_TG;
    if (group >= 6) base = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_GFX_VOP_REG_BASE +
                           RA_VPP_GFX_VOP_VOP + RA_VOP_OCSC;
    for (unsigned i = 0; i < 4; i++) {
        volatile uint32_t *view = TEE_TA_RegPhyToVirt(base + offsets[group][i]);
        if (!view) return TEE_ERROR_BAD_STATE;
        words[i] = *view;
    }
    params[1].value.a = words[0]; params[1].value.b = words[1];
    params[2].value.a = words[2]; params[2].value.b = words[3];
    return TEE_SUCCESS;
}
