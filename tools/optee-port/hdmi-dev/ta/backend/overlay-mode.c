/* SPDX-License-Identifier: GPL-2.0 */
#include "overlay.h"
#include "vpp.h"
#define VPP UINT32_C(0xf7490000)
#define GFX0 (VPP+RA_Vpp_GFX0)
#define CPCB UINT32_C(0xf7492a38)
#define CPCB_TG UINT32_C(0xf7492a90)
#define VOP_TG UINT32_C(0xf7496014)
#define STEP(call) do { TEE_Result rc_=(call); if (rc_) return rc_; } while (0)
static TEE_Result update(const struct hdmi_mode_io *io,
    uint32_t address, uint32_t mask, uint32_t value)
{
    uint32_t old;
    STEP(io->read32(io->opaque,address,&old));
    return io->write32(io->opaque,address,(old&~mask)|(value&mask));
}
static TEE_Result window(const struct hdmi_mode_io *io,uint32_t base,
    uint32_t offset,const struct hdmi_dev_overlay *g)
{
    STEP(update(io,base+offset,0x03ffffff,g->x|((g->x+g->width)<<13)));
    return update(io,base+offset+4,0x00ffffff,g->y|((g->y+g->height)<<12));
}
TEE_Result hdmi_overlay_configure_geometry(struct hdmi_overlay *o,
    struct hdmi_avio_registers *registers,const struct hdmi_mode_io *io,
    const struct hdmi_dev_overlay *g,uint32_t canvas_w,uint32_t canvas_h)
{
    if (!canvas_w || canvas_w>2560u || !canvas_h || canvas_h>1440u ||
        !g || g->width>canvas_w || g->height>canvas_h ||
        g->x>canvas_w-g->width || g->y>canvas_h-g->height || !o || !g || !registers || !io || !io->read32 || !io->write32 ||
        !hdmi_dev_overlay_valid(g,(uint64_t)g->stride*g->height))
        return TEE_ERROR_BAD_PARAMETERS;
    if (o->configured) return TEE_ERROR_BAD_STATE;
    STEP(hdmi_gfx_dma_init_pip_geometry(&o->dma,registers,g->width*4u));
    /* FE_DLR_SetDataFmt：Dolphin A0的ARGB32解码值为6，PIP packSel为2。 */
    STEP(update(io,GFX0+RA_GFX0_CTRL,0xffffffff,0x3c000005));
    STEP(update(io,GFX0+RA_GFX0_CTRL0,0x003c7fff,(6u<<2)|(1u<<8)|(8u<<11)));
    /* 释放 CTRL1.clken_st_clr 后开始正常取数；csc_out_sel=1 选择 CSC 输出，保留 doViWrap_Out_Sel。 */
    STEP(update(io,GFX0+RA_GFX0_CTRL1,0x3f7f7fff,
        (1u<<5)|(1u<<9)|(1u<<20)|(1u<<22)|(1u<<24)));
    /* 保留 HDR wrapper 的 alphaRange 转换；LUT 旁路关闭色调映射。 */
    STEP(update(io,GFX0+RA_GFX0_LUT_BYPASS,1,1));
    STEP(update(io,GFX0+RA_GFX0_SCL_BYPASS,1,1));
    STEP(io->write32(io->opaque,GFX0+RA_GFX0_Rd_word0,g->width*g->height/4u));
    STEP(io->write32(io->opaque,GFX0+RA_GFX0_gfx_pix,g->width*g->height));
    uint32_t tg=GFX0+RA_GFX0_pipTG;
    STEP(io->write32(io->opaque,tg+RA_TG_SIZE,((g->width+20u)<<16)|(g->height+6u)));
    STEP(io->write32(io->opaque,tg+RA_TG_HB,(7u<<16)|(g->width+8u)));
    STEP(io->write32(io->opaque,tg+RA_TG_HB_CR,(7u<<16)|(g->width+8u)));
    STEP(io->write32(io->opaque,tg+RA_TG_VB0,(2u<<16)|(g->height+3u)));
    STEP(io->write32(io->opaque,tg+RA_TG_VB0_CR,(2u<<16)|(g->height+3u)));
    /* ORDER_RGBA恒等表，内存第4字节直接提供透明度。 */
    for (unsigned i=0;i<8;++i) {
        uint32_t word=0;
        for (unsigned j=0;j<5 && i*5+j<32;++j) word|=(i*5+j)<<(j*6);
        STEP(io->write32(io->opaque,GFX0+RA_GFX0_BITMAP40+i*4,word));
    }
    STEP(window(io,CPCB_TG,0x10,g));
    STEP(window(io,CPCB_TG,0x50,g));
    STEP(window(io,CPCB_TG,0x30,g));
    /* VPP_CPCB_SetOutputResolution：VOP侧接收整幅混合结果，使用完整输出窗口。 */
    const struct hdmi_dev_overlay output={0,0,canvas_w,canvas_h,0};
    STEP(window(io,VOP_TG,0x10,&output));
    STEP(window(io,VOP_TG,0x50,&output));
    STEP(window(io,VOP_TG,0x30,&output));
    STEP(io->write32(io->opaque,CPCB+RA_CPCB0_CTRL5,g->width*g->height));
    /* 按 VPP_SCL_SetGfxSclparams 配置小于输出宽度的图层，逐行产生完成事件。 */
    T32CPCB0_CTRL8 line={.u32=0};
    STEP(io->read32(io->opaque,CPCB+RA_CPCB0_CTRL8,&line.u32));
    line.uCTRL8_pixLineTot_gfx0=g->width < canvas_w ? g->width : 0x64;
    line.uCTRL8_lineDn_Gfx0_En=g->width < canvas_w;
    STEP(io->write32(io->opaque,CPCB+RA_CPCB0_CTRL8,line.u32));
    /* PIP在GFX1之上；MSB逐像素透明度，普通方向、非预乘。 */
    uint32_t ovl=CPCB+RA_CPCB0_OVL1;
    STEP(update(io,ovl+RA_OVL_LAY,0xffff,0x120));
    STEP(update(io,ovl+RA_OVL_ALPHA_CTRL,0x3fff,1u|(4095u<<2)));
    STEP(update(io,ovl+RA_OVL_SENSE_CTRL,2,0));
    STEP(update(io,ovl+RA_OVL_APM,2,0));
    /* VPP_ClkGate_Plane(enable=0)：图层工作，允许其源时钟。 */
    STEP(update(io,VPP+RA_Vpp_CTRL,0x101,1));
    hdmi_frame_slots_init(&o->slots);o->geometry=*g;o->configured=true;
    return TEE_SUCCESS;
}

TEE_Result hdmi_overlay_configure(struct hdmi_overlay *o,
    struct hdmi_avio_registers *registers,const struct hdmi_mode_io *io,
    const struct hdmi_dev_overlay *g)
{ return hdmi_overlay_configure_geometry(o,registers,io,g,HDMI_DEV_WIDTH,HDMI_DEV_HEIGHT); }
