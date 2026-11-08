/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __TZ_H_
#define __TZ_H_
void tos_suspend(void *, uint32_t);
void tos_resume(void *);
void tos_save_secure_boot_param(void *, void *, void *, int);
void tos_reset_secure_rf_param(unsigned int, int);
#endif
