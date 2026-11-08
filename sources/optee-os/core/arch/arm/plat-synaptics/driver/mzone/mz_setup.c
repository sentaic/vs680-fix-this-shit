// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "config.h"
#include "io.h"
#include "mz.h"
#include "mz_setup.h"
#include "mr_setup.h"
#include "log.h"
#include "tz_boot_param.h"
#include "tz_boot_param_value.h"

void mz_setup(void)
{
	int i;
	mz_ops_t *mz_ops;

	platform_setup_mz_ops(&mz_ops);

	/* The default MZ is TZ_SRW_NRW, so only set the secure MZ */
	for (i = 0; i < secure_memzone_num; i++) {
		if (mz_ops->set_zone)
			mz_ops->set_zone(0, i, secure_memzone_info[i].start, secure_memzone_info[i].size,
					TZ_SRW_NNA);

		if (mz_ops->enable_zone)
			mz_ops->enable_zone(0, i, TZ_SRW_NNA);

	}

	if (mz_ops->lock)
		mz_ops->lock(0);
}
