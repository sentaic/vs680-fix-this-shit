/* SPDX-License-Identifier: GPL-2.0 */
#include "mixer.h"
#include "vpp.h"
#define OVL UINT32_C(0xf7492a5c)
TEE_Result hdmi_mixer_program(const struct hdmi_mixer *m,
    const struct hdmi_mode_io *io, struct hdmi_register_write program[HDMI_MIXER_WRITES])
{
    T32OVL_ALPHA_CTRL alpha={.u32=0};
    uint32_t sense;
    if (!m || !io || !io->read32 || !program || m->background_rgb>0xffffffu ||
        m->alpha_q12>4095u) return TEE_ERROR_BAD_PARAMETERS;
    TEE_Result rc=io->read32(io->opaque,OVL+RA_OVL_ALPHA_CTRL,&alpha.u32);
    if (!rc) rc=io->read32(io->opaque,OVL+RA_OVL_SENSE_CTRL,&sense);
    if (rc) return rc;
    /* VPP_OVL_SetAlpha(Vpp_OVL_PLANE3)：RGB24使用全局透明度。 */
    alpha.uALPHA_CTRL_pl4_alpctrl0=0;
    alpha.uALPHA_CTRL_pl4_alpctrl1=m->alpha_q12;
    /* 正常方向：(1-w)*背景+w*前景，GFX1位为bit2。 */
    sense &= ~UINT32_C(4);
    /* VPP底色的低字节为R；公共参数保持0xRRGGBB。 */
    uint32_t packed=((m->background_rgb&255u)<<16)|(m->background_rgb&0xff00u)|
        ((m->background_rgb>>16)&255u);
    program[0]=(struct hdmi_register_write){packed,OVL+RA_OVL_FIX};
    program[1]=(struct hdmi_register_write){packed,OVL+RA_OVL_FIX0};
    program[2]=(struct hdmi_register_write){alpha.u32,OVL+RA_OVL_ALPHA_CTRL};
    program[3]=(struct hdmi_register_write){sense,OVL+RA_OVL_SENSE_CTRL};
    return TEE_SUCCESS;
}
