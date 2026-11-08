/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_DMA_CREDIT_H
#define RX_DMA_CREDIT_H
#include <stdint.h>
#define RX_DMA_CREDIT_COMMAND 0x7f2du
#define RX_DMA_CREDIT_ABI 3u
#define RX_DMA_WRITE_READ_CREDIT_COMMAND 0x7f30u
#define RX_DMA_WRITE_READ_PATTERN 0x83000000u
#define RX_DMA_CREDIT_FIFO_WORDS 64u
#define RX_DMA_CREDIT_WAITED 1u
#define RX_DMA_CREDIT_COMPLETED 2u
#define RX_DMA_CREDIT_FIFO_PRESENT 4u
#define RX_DMA_CREDIT_WRITE_COMPLETED 8u
struct rx_dma_credit_state {
    uint32_t busy, pending, two_d, completion;
    uint32_t commands, data, data_pointer;
    uint32_t ready_producer, ready_consumer;
};
struct rx_dma_credit_result {
    uint32_t abi, result, cleanup_result, armed, restored, observations;
    uint32_t vpp_initial, aio_initial, vpp_queued, aio_queued;
    uint32_t bcm_empty, bcm_autopush, words_valid;
    /* Written table configuration and HAL write count; CFG/ARQOS are write-only. */
    uint32_t normal_cfg, config_writes, write_busy, write_completed;
    struct rx_dma_credit_state initial, empty_ready, released, final;
    uint32_t words[RX_DMA_CREDIT_FIFO_WORDS];
    uint32_t ready_empty, ready_full, ready_mask, writer_config_writes, rx_restored;
    struct rx_dma_credit_state writer_final;
};
#endif
