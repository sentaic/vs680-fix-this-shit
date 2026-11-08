// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _BCM_RANDOM_H_
#define _BCM_RANDOM_H_

/** get bcm random value without TEE wait
 *
 * @param random	output random buffer pointer
 * @param random_len	input params for get random value's length
 *
 * @retval 0	success
 * @retval !0	fail
 *
 */
int bcm_get_random_without_wait(uint32_t *random, uint32_t random_len);

#endif /* _BCM_RANDOM_H_ */
