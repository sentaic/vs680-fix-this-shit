/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_GFX_DMA_H
#define HDMI_DEV_GFX_DMA_H
#include "avio-hardware.h"
#include "dma-pool.h"
#include "register-command.h"
#include "mode-io.h"
#define HDMI_GFX_PROGRAM_WRITES 9u
struct hdmi_gfx_dma {
    struct hdmi_avio_registers *registers;
    unsigned channel;
    uint32_t mtu_bytes;
    uint64_t sequence;
    uint64_t active;
    struct hdmi_register_write program[HDMI_GFX_PROGRAM_WRITES];
    bool configured;
};
/* 仅支持已核实的GFX1通道7及PIP通道5；每个实例独占一个读取通道。 */
/* Select the mature row MTU using actual visible row bytes before arm.
 * A 256-byte multiple retains MTU256; other rows use the official MTU128
 * fallback. Whole-row descriptors retain the selected MTU for identity checks. */
TEE_Result hdmi_gfx_dma_init_geometry(struct hdmi_gfx_dma *dma,
    struct hdmi_avio_registers *registers, uint32_t line_bytes);
TEE_Result hdmi_gfx_dma_init_pip_geometry(struct hdmi_gfx_dma *dma,
    struct hdmi_avio_registers *registers, uint32_t line_bytes);
TEE_Result hdmi_gfx_dma_init_pip(struct hdmi_gfx_dma *dma,
    struct hdmi_avio_registers *registers);
TEE_Result hdmi_gfx_dma_submit_region(struct hdmi_gfx_dma *dma,
    struct hdmi_dma_pool *pool, uint32_t offset, uint32_t stride,
    uint32_t line_bytes, uint32_t height, uint64_t *submission);
/* 编码下一份命令表；换表完成前保留旧表及旧像素。 */
TEE_Result hdmi_gfx_dma_prepare_repeat(struct hdmi_gfx_dma *dma,
    struct hdmi_dma_pool *pool, uint32_t offset, uint32_t stride,
    uint32_t line_bytes, uint32_t height, uint64_t *submission);
TEE_Result hdmi_gfx_dma_init(struct hdmi_gfx_dma *dma,
                             struct hdmi_avio_registers *registers);
/* 准备 DMA 描述符，由 VBI 命令表启动。 */
TEE_Result hdmi_gfx_dma_submit(struct hdmi_gfx_dma *dma,
                               struct hdmi_dma_pool *pool, unsigned slot,
                               uint64_t *submission);
/* 读完成表示 DMA 已读取源内存。 */
TEE_Result hdmi_gfx_dma_complete(struct hdmi_gfx_dma *dma, uint64_t *submission);
/* 新表已送达实际读取通道后，才能开始旧源的完成与回收检查。 */
TEE_Result hdmi_gfx_dma_source_matches(struct hdmi_gfx_dma *dma,
    const struct hdmi_mode_io *io);
TEE_Result hdmi_gfx_dma_stop(struct hdmi_gfx_dma *dma);
#endif
