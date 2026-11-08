// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _BCM_HW_FUNCTION_H_
#define _BCM_HW_FUNCTION_H_

int hwrand(uint8_t *randBuffer, uint32_t randLength);
int bcm_enable_bg_checking(void);
void bg_check_init(void);
void bcm_add_bg_check_register(uint32_t addr, uint32_t data);

#endif
