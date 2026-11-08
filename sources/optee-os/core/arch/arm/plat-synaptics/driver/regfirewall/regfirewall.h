// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _REG_FIREWALL_H_
#define _REG_FIREWALL_H_
#include <types_ext.h>
#include <stdint.h>

#include "tz_perm.h"

/** set register firewall window by base/mask.
 * for bg2ct, only (64 * 2^n - 1)kB (n=0~13) are supported;
 */
int rf_set_window(int id, uint32_t base, uint32_t mask, int perm);

/** set register firewall window by base/size.
 * for bg2ct, only (64 * 2^n)kB (n=0~13) are supported;
 * for other chips, only 2^n can be supported.
 */
int rf_set_window_by_size(int id, uint32_t base, uint32_t size, int perm);

/** reset/clear the register firewall window
 */
int rf_reset_window(int id);

void rf_buffer_registers(void);

int rf_check_changed(void);

int rf_lock(void);
#endif /* _REG_FIREWALL_H_ */
