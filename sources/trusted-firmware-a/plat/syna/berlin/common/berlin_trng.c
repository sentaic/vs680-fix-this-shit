/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arm_acle.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <lib/mmio.h>
#include <lib/utils_def.h>
#include <platform_def.h>

#include <lib/smccc.h>
#include <services/trng_svc.h>
#include <smccc_helpers.h>

#include <plat/common/platform.h>

extern void hw_get_random_bytes(uint8_t *data, uint32_t size);

DEFINE_SVC_UUID2(_plat_trng_uuid,
	0x1316a183, 0x894d, 0x43fe, 0x98, 0x93,
	0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x43
);
uuid_t plat_trng_uuid;

static uint8_t trng_buf[64]
    __attribute__((aligned(CACHE_WRITEBACK_GRANULE)));

bool plat_get_entropy(uint64_t *out)
{
	memset(trng_buf, 0, sizeof(trng_buf));
	hw_get_random_bytes(trng_buf, sizeof(*out));

	if (out)
		memcpy(out, trng_buf, sizeof(*out));

	return true;
}

void plat_entropy_setup(void)
{
	uint64_t dummy;

	plat_trng_uuid = _plat_trng_uuid;

	/* Initialise the entropy source and trigger RNG generation */
	plat_get_entropy(&dummy);
	INFO("random dummy value = 0x%lx", dummy);
}
