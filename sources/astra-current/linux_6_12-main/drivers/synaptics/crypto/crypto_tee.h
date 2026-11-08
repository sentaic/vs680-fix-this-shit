/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __CRYPTO_TEE__
#define __CRYPTO_TEE__

#include <linux/types.h>

#define AES_GCM_IV_SIZE		12
#define AES_GCM_TAG_SIZE	16
#define AES_AAD_MAX_LEN		16
#define ENABLE_CRYPTO_TEST 0
#define TA_CMD_AES_GCM_PROCESS 0xf003
struct aes_gcm_ctx {
	int is_encrypt;
	u8 iv[AES_GCM_IV_SIZE];
	u8 aad[AES_AAD_MAX_LEN];
	size_t aad_len;
	u8 tag[AES_GCM_TAG_SIZE];
};

int aes_hwkey_gcm_init(struct aes_gcm_ctx *ctx, const u8 *iv, const u8 *aad,
						size_t aad_len, const u8 *tag, int is_encrypt);
int aes_hwkey_gcm_process(struct aes_gcm_ctx *ctx, const u8 *in, u8 *out, size_t len);
int aes_hwkey_gcm_get_tag(struct aes_gcm_ctx *ctx, u8 *tag);

#endif
