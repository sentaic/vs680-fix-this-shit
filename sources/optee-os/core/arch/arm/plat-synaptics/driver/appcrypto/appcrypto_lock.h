// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2025, Synaptics Incorporated.
 */
#ifndef _APPCRYPTO_LOCK_H
#define _APPCRYPTO_LOCK_H

#include <stdint.h>

/* Claim the app crypto interface lock. Returns 0 on success, -1 on failure. */
int32_t appcrypto_lock(void);

/* Try to claim the lock without waiting. Returns 0 on success, -1 if busy. */
int32_t appcrypto_try_lock(void);

/* Release the app crypto interface lock. Returns 0 on success, -1 on failure. */
int32_t appcrypto_unlock(void);

#endif /* _APPCRYPTO_LOCK_H */
