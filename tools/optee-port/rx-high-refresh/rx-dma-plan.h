/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_DMA_PLAN_H
#define RX_DMA_PLAN_H
#include <stdint.h>
struct rx_dma_plan {
    uint32_t burst, step1, size1, step2, size2, channel_mtu;
    uint64_t transferred_bytes;
};
int rx_dma_plan_make(uint32_t row_bytes, uint32_t stride, uint32_t height,
                     struct rx_dma_plan *out);
#endif
