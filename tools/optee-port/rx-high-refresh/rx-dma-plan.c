/* SPDX-License-Identifier: GPL-2.0 */
#include "rx-dma-plan.h"
int rx_dma_plan_make(uint32_t row_bytes, uint32_t stride, uint32_t height,
                     struct rx_dma_plan *out)
{
    if (!out || !row_bytes || row_bytes > 65535u || (row_bytes & 15u) ||
        stride < row_bytes || stride > 0xffffffu || !height || height > 0xffffffu)
        return -1;
    struct rx_dma_plan p = {0};
    /* 官方hrx-vip-isr.c:14-15,139-186：256/64/整行三种编码。 */
    if (!(row_bytes & 255u)) { p.burst = 256; p.channel_mtu = 9; }
    else if (!(row_bytes & 63u)) { p.burst = 64; p.channel_mtu = 7; }
    else { p.burst = row_bytes; p.channel_mtu = 4; }
    p.step1 = p.burst; p.size1 = row_bytes / p.burst;
    p.step2 = stride; p.size2 = height;
    p.transferred_bytes = (uint64_t)p.burst * p.size1 * p.size2;
    *out = p;
    return 0;
}
