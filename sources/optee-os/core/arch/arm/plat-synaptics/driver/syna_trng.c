/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nexus_fw_include/nexus_trng_regs.h"
#include "nexus_fw_include/nexus_klamath_memmap.h"
#include "plat_def.h"
#include <io.h>
#include <util.h>
#include <types_ext.h>
#include <tee_api_types.h>
#include <string.h>
#include <stdint.h>
#include <mm/core_memprot.h>
#include <rng_support.h>
#include <kernel/mutex.h>

static struct mutex trng_mutex = MUTEX_INITIALIZER;

#define BASE_ADDR_TRNG     (vaddr_t)phys_to_virt_io(MCU_DMA_CRYPT_REGION(START_MCU_TRNG_S), SIZE_MCU_TRNG_S)

#define  ELP_SP80090C_DRBG_SEED_WIDTH                       384
#define  pDATA_WIDTH                                        32
#define  pSEED_DATA_WIDTH                                   (ELP_SP80090C_DRBG_SEED_WIDTH)
#define  pNUM_SEEDx_REGISTERS                               (pSEED_DATA_WIDTH/pDATA_WIDTH)

#define RNG_DEFAULT_COOKIE          (0xABADF0CC)
#define RNG_TIMEOUT_COUNT           (1000)

#define MAX_RNG_LEN 48

static TEE_Result trng_get_random(uint8_t *data, size_t size)
{
    volatile int32_t count = 0;
    volatile uint32_t  stat, busy, ctrl, seed, smode;
    size_t i = 0;

    if (data == NULL)
        return TEE_ERROR_BAD_PARAMETERS;

    /* get maximum 48B each time */
    if (size > MAX_RNG_LEN)
		return TEE_ERROR_NOT_SUPPORTED;

    count = 0; stat = 0; busy = 0;
    do {
        stat = io_read32(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_STAT);
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
            seed = io_read32(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_SEED0 + (count*4));
            if (seed == 0)
                return TEE_ERROR_COMMUNICATION;
        }
        data[i] = *((uint8_t *)(&seed) + (i % 4));
    }

    count++;
    for (; count < 12; count++) {
        seed = io_read32(BASE_ADDR_TRNG + REG_ADDR__TRNG_REGS_SEED0 + (count*4));
    }


    return TEE_SUCCESS;
}

TEE_Result hw_get_random_bytes(void *buf, size_t len)
{
	TEE_Result ret = 0;
	size_t rng_len = MAX_RNG_LEN;
	size_t tmp_len = 0;

	do {

		if (len - tmp_len <= MAX_RNG_LEN)
			rng_len = len - tmp_len;
		else
			rng_len = MAX_RNG_LEN;

		mutex_lock(&trng_mutex);
		ret = trng_get_random((uint8_t *)((uintptr_t)buf + tmp_len), rng_len);
		mutex_unlock(&trng_mutex);

		if (ret != TEE_SUCCESS) {
			EMSG("fail to get random: ret = %d.\n", ret);
			ret = TEE_ERROR_ACCESS_CONFLICT;
			goto out;
		}
		tmp_len += rng_len;
	} while (tmp_len < len);

	ret = TEE_SUCCESS;
out:
	if (ret != TEE_SUCCESS) {
		memset(buf, 0, len);
	}
	return ret;
}
