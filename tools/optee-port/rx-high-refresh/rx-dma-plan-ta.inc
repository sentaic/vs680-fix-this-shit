/* SPDX-License-Identifier: GPL-2.0 */
#include "rx-dma-plan.c"
/* 调用者先停产出、禁用2ND并确认旧DMA结束，之后才能提交本调用。 */
static uint32_t rx_dma_plan_apply(HDL_dhub2d *hub, uint32_t channel,
                                  uint32_t address, const struct rx_dma_plan *p,
                                  T64b cfg_queue[])
{
    return dhub2nd_channel_cfg(hub, channel, address, p->burst, p->step1,
        p->size1, p->step2, p->size2, 0, 0, 1, 1, cfg_queue);
}
