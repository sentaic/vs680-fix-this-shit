
/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APPCRYPTO_LOCK_H
#define _APPCRYPTO_LOCK_H

int32_t appcrypto_lock(void);
int32_t appcrypto_try_lock(void);
int32_t appcrypto_unlock(void);
#endif // _APPCRYPTO_LOCK_H
