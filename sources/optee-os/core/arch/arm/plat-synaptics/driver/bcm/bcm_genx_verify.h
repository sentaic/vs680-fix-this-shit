// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _BCM_GENX_VERIFY_H_
#define _BCM_GENX_VERIFY_H_

/** set anti-rollback parameters.
 *
 * the parameters will be used when verify image.
 *
 * @param comm_ver	common version.
 * @param cust_ver	customer version.
 * @param recovery	in recovery stage?
 */
void set_antirollback_param(int comm_ver, int cust_ver, bool recovery);

/** get the permission flags in encrypted image header.
 *
 * @retval 32bits permission flags.
 */
unsigned int image_get_permissions(const void *image);

/** do decryption and signature verification.
 *
 * @note dst can overlap src, in other word, dst can be same as src.
 *
 * @param src	encrypted image buffer, must be 32bytes aligned
 * @param dst	decrypted image buffer, must be 32bytes aligned
 * @param size	size of src image.
 *
 * @retval 0	fail
 * @retval >0	size of the decrypted data
 *
 */

int verify_ta_image(const void *src, unsigned int src_size ,
		void *pta, unsigned int ta_size,
		void *pta_all_permission, unsigned int all_permission_size,
		void *pta_permission, unsigned int subset_permission_size);

#endif /* _BCM_GENX_VERIFY_H_ */
