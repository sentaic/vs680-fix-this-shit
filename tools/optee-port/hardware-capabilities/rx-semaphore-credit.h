/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RX_SEMAPHORE_CREDIT_H
#define RX_SEMAPHORE_CREDIT_H
#include <stdint.h>
#define RX_SEMAPHORE_CREDIT_COMMAND 0x7f2cu
#define RX_SEMAPHORE_CREDIT_STEPS 4u
/* Reset, push one, push one, pop one. Preserve the raw query response. */
struct rx_semaphore_credit_sample {
    uint32_t producer, consumer;
};
struct rx_semaphore_credit {
    struct rx_semaphore_credit_sample steps[RX_SEMAPHORE_CREDIT_STEPS];
};
#endif
