/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nexus_fw_include/nexus_trng_regs.h"
#include "nexus_fw_include/nexus_klamath_memmap.h"
#include <lib/mmio.h>
#include <platform_def.h>
#include "appcrypto_lock.h"

#define BASE_ADDR_TRNG     MCU_DMA_CRYPT_REGION(START_MCU_TRNG_S)

#define  ELP_SP80090C_DRBG_SEED_WIDTH                       384
#define  pDATA_WIDTH                                        32
#define  pSEED_DATA_WIDTH                                   (ELP_SP80090C_DRBG_SEED_WIDTH)
#define  pNUM_SEEDx_REGISTERS                               (pSEED_DATA_WIDTH/pDATA_WIDTH)

#define RNG_DEFAULT_COOKIE          (0xABADF0CC)
#define RNG_TIMEOUT_COUNT           (1000)

void appcrypto_get_random(uint8_t *data, uint32_t size)
{
    volatile int32_t count = 0;
    volatile uint32_t  stat, busy, ctrl, seed, smode;
    int i = 0;

    if (data == NULL)
        return;

    /* get maximum 48B each time */
    if (size > 48)
		return;// STATUS_UNSUPPORTED_PARAMETER;

    count = 0; stat = 0; busy = 0;
    do {
        stat = mmio_read_32(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_STAT);
        busy = stat & (1U << REG_FIELD_POS__TRNG_REGS_STAT_BUSY);
    } while ( busy && ++count < RNG_TIMEOUT_COUNT );

    smode = *((volatile uint32_t *)(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_SMODE));
    smode &= ~(1U << REG_FIELD_POS__TRNG_REGS_SMODE_MISSION_MODE);
    *((volatile uint32_t *)(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_SMODE)) = smode;

    count = 0; stat = 0; busy = 0;
    do {
        stat = *((volatile uint32_t *)(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_STAT));
        busy = stat & (1U << REG_FIELD_POS__TRNG_REGS_STAT_BUSY);
    } while ( busy && ++count < RNG_TIMEOUT_COUNT  );

    ctrl = ENUM__TRNG_REGS_CTRL_CMD__GEN_NOISE << REG_FIELD_POS__TRNG_REGS_CTRL_CMD;
    *((volatile uint32_t *)(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_CTRL)) = ctrl;

    count = 0; stat = 0;
    do {
        stat = *((volatile uint32_t *)(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_ISTAT));
        if (stat & (1 << REG_FIELD_POS__TRNG_REGS_ISTAT_DONE)) {
            *((volatile uint32_t *)(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_ISTAT)) = 1 << REG_FIELD_POS__TRNG_REGS_ISTAT_DONE;
            break;
        }
    } while (++count < RNG_TIMEOUT_COUNT);

    count = 0;
    for (i = 0; i < size; i++) {
        if (!(i % 4)) {
            count = i / 4;
            seed = mmio_read_32(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_SEED0 + (count*4));
            if (seed == 0)
                return ;
        }
        data[i] = *((uint8_t *)(&seed) + (i % 4));
    }

    count++;
    for (; count < 12; count++) {
        seed = mmio_read_32(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_SEED0 + (count*4));
    }


    return;
}

void hw_get_random_bytes(uint8_t *data, uint32_t size)
{
	appcrypto_lock();
	appcrypto_get_random(data, size);
	appcrypto_unlock();
}
