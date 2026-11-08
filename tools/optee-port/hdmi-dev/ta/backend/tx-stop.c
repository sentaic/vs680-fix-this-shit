/* SPDX-License-Identifier: GPL-2.0 */
#include "tx-stop.h"
#include "mode-720p.h"
#include "avio.h"
#include "Galois_memmap.h"
#include "avio_memmap.h"
#define BCM_BASE (MEMMAP_AVIO_REG_BASE+AVIO_MEMMAP_AVIO_BCM_REG_BASE)
TEE_Result hdmi_tx_require_idle(const struct hdmi_mode_io *io,
    const struct hdmi_avio_registers *registers,struct hdmi_avio_activity *activity)
{
    uint32_t empty,automatic;
    if(!io||!io->read32)return TEE_ERROR_BAD_PARAMETERS;
    TEE_Result result=io->read32(io->opaque,BCM_BASE+RA_AVIO_BCM_AUTOPUSH,&automatic);
    if(!result)result=io->read32(io->opaque,BCM_BASE+RA_AVIO_BCM_EMP_STS,&empty);
    if(result)return result;
    if((automatic&1u)||!(empty&1u))return TEE_ERROR_BUSY;
    result=hdmi_avio_tx_snapshot(registers,activity);
    if(result)return result;
    return hdmi_avio_idle(activity)?TEE_SUCCESS:TEE_ERROR_BUSY;
}
TEE_Result hdmi_tx_stop(const struct hdmi_mode_io *io,struct hdmi_avio_registers *registers,
    struct hdmi_frame_start *start,struct hdmi_avio_activity *activity)
{
    if(!io||!registers||!start||!activity)return TEE_ERROR_BAD_PARAMETERS;
    TEE_Result result;
    if(start->configured) result=hdmi_frame_start_stop(start);
    else result=hdmi_tx_require_idle(io,registers,activity);
    if(result)return result;
    result=hdmi_mode_720p_disable(io);if(result)return result;
    /* 只清本显示拥有者的两个读通道，RX及其他dHub通道保持。 */
    struct hdmi_gfx_dma gfx={.registers=registers,.channel=7u};
    struct hdmi_gfx_dma pip={.registers=registers,.channel=5u};
    result=hdmi_gfx_dma_stop(&gfx);if(result)return result;
    result=hdmi_gfx_dma_stop(&pip);if(result)return result;
    return hdmi_tx_require_idle(io,registers,activity);
}
