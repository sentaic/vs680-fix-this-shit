/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nexus_fw_include/mcu_cpu_ss.h"
#include "nexus_fw_include/nexus_klamath_memmap.h"
#include <lib/mmio.h>
#include <platform_def.h>

#define MCU_SYSCTRL_S_BASE MCU_PERIF_REGION(START_MCU_SYSCTRL_S)

#define APP_CRYPTO_INTF_LOCK_CTRL (MCU_SYSCTRL_S_BASE + RA_MCS_REG_APP_CRYPTO + RA_MCS_INTF_LOCK_INTF_LOCK_CTRL)
#define APP_CRYPTO_INTF_LOCK_STAT (MCU_SYSCTRL_S_BASE + RA_MCS_REG_APP_CRYPTO + RA_MCS_INTF_LOCK_INTF_LOCK_STAT)

#define MID_A55 0x2

int32_t appcrypto_lock(void)
{
	/*
	 * 1. Read MCS_REG_APP_CRYPTO_INTF_LOCK_STAT.locked, if 1, keep trying
	 * 2. Write MCS_REG_APP_CRYPTO_INTF_LOCK_CTRL.lock to 1 to claim lock.
	 * 3. Read MCS_REG_APP_CRYPTO_INTF_LOCK_STAT.locked and .mid. Make sure
	 *    the MID matches itself, indicating it successfully claimed the lock.
	*/
	T32MCS_INTF_LOCK_INTF_LOCK_STAT stat;
	T32MCS_INTF_LOCK_INTF_LOCK_CTRL ctrl;

	do {
		stat.u32 = mmio_read_32(APP_CRYPTO_INTF_LOCK_STAT);
	} while(stat.uINTF_LOCK_STAT_locked);

	ctrl.uINTF_LOCK_CTRL_lock = 1;
	mmio_write_32(APP_CRYPTO_INTF_LOCK_CTRL, ctrl.u32);

	stat.u32 = mmio_read_32(APP_CRYPTO_INTF_LOCK_STAT);
	if (stat.uINTF_LOCK_STAT_locked && (stat.uINTF_LOCK_STAT_mid == MID_A55))
		return 0;

	/* FIXME: go back to loop and wait or return? */
	return -1;
}

int32_t appcrypto_try_lock(void)
{
	/* Read MCS_REG_APP_CRYPTO_INTF_LOCK_STAT.locked, if 1, return */
	T32MCS_INTF_LOCK_INTF_LOCK_STAT stat;
	T32MCS_INTF_LOCK_INTF_LOCK_CTRL ctrl;

	stat.u32 = mmio_read_32(APP_CRYPTO_INTF_LOCK_STAT);
	if (stat.uINTF_LOCK_STAT_locked)
		return -1;

	ctrl.uINTF_LOCK_CTRL_lock = 1;
	mmio_write_32(APP_CRYPTO_INTF_LOCK_CTRL, ctrl.u32);

	stat.u32 = mmio_read_32(APP_CRYPTO_INTF_LOCK_STAT);
	if (stat.uINTF_LOCK_STAT_locked && (stat.uINTF_LOCK_STAT_mid == MID_A55))
		return 0;

	return -1;
}

int32_t appcrypto_unlock(void)
{
	/* Write MCS_REG_APP_CRYPTO_INTF_LOCK_CTRL.lock to 0 to release lock */
	T32MCS_INTF_LOCK_INTF_LOCK_STAT stat;
	T32MCS_INTF_LOCK_INTF_LOCK_CTRL ctrl;

	stat.u32 = mmio_read_32(APP_CRYPTO_INTF_LOCK_STAT);
	if (!stat.uINTF_LOCK_STAT_locked || (stat.uINTF_LOCK_STAT_mid != MID_A55))
		return -1;

	ctrl.u32 = 0;
	mmio_write_32(APP_CRYPTO_INTF_LOCK_CTRL, ctrl.u32);

	return 0;
}
