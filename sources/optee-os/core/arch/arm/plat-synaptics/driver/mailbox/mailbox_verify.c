/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "plat_def.h"
#include <io.h>
#include <util.h>
#include <types_ext.h>
#include <tee_api_types.h>
#include <string.h>
#include <stdint.h>
#include <mm/core_memprot.h>
#include <initcall.h>
#include <tee/cache.h>
#include <mm/core_mmu.h>
#include <mm/mobj.h>
#include <crypto/crypto.h>

#include "klamath_memmap.h"
#include "synaprot_fw_include/mailbox.h"
#include "synaprot_mailbox.h"
#include "nexus_fw_include/mcu_cpu_ss.h"
#include "nexus_fw_include/nexus_klamath_memmap.h"
#include "nexus_fw_include/mcu_gbl_cfg.h"
#include "image.h"
#include "appcrypto_lock.h"
#include "bcm_drv_smc.h"
#include "bcm_drv_hash.h"

#define IMAGE_ID_LINUX	(0x00020018)
#define KEY_LEN_AES256  (32)
#define MCU_SECCTRL_S_BASE (vaddr_t)phys_to_virt_io(MCU_PERIF_REGION(START_MCU_SECCTRL_S), SIZE_MCU_SECCTRL_S)

static int is_iv_set(uint8_t *iv, uint32_t len)
{
	uint32_t i = 0;

	for (i = 0; i < len; i++)
		if (iv[i] != 0)
			return 1;

	return 0;
}

static TEE_Result decrypt_sw_image(const uint8_t *skey, uint32_t skey_len,
						uint8_t *iv, uint32_t iv_len,
						uint8_t *input, uint32_t input_len,
						void * output, uint32_t output_len)
{
	TEE_Result ret = TEE_ERROR_GENERIC;

	if (!input || !output) {
		EMSG("invaild input@0x%p, output@0x%p",	input, output);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	cache_operation(TEE_CACHEFLUSH, input, input_len);

	appcrypto_lock();

	ret = bcm_drv_aes(input, output, input_len, skey, skey_len * 8,
				iv, iv_len, SMC_AES_CBC, SMC_AES_DECRYPTION, &output_len);

	if (ret || !output_len) {
		EMSG("Fail to descrypt, ret = 0x%x, output_len = 0x%x", ret, output_len);
		goto out;
	}

	ret = TEE_SUCCESS;
	cache_operation(TEE_CACHEINVALIDATE, output, output_len);

out:
	appcrypto_unlock();

	return ret;
}

static TEE_Result appcrypto_hash(void *digest, uint32_t *digest_len, void * msg, uint32_t msg_size_byte)
{
	TEE_Result ret = TEE_ERROR_GENERIC;

	if (!msg) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	appcrypto_lock();

	ret = bcm_drv_hash(HASH_MODE_SHA512, msg, msg_size_byte, (uint32_t *)digest, digest_len);
	if (ret != TEE_SUCCESS) {
		EMSG("bcm_drv_hash fail: ret = 0x%x", ret);
		goto out;
	}

	ret = TEE_SUCCESS;

out:
	appcrypto_unlock();
	return ret;

}
static void dump_hash(uint8_t *digest, uint32_t digest_len)
{
	uint32_t i;

	IMSG("DUMP HASH: digest_len = 0x%x.\n", digest_len);
	for (i = 0; i < digest_len; i ++) {
		IMSG("digest [%d] = 0x%x.\n", i, digest[i]);
	}
	IMSG("DUMP Done");
}

static TEE_Result mailbox_verify_image(void *src, void *dst, size_t size)
{
	TEE_Result ret = TEE_ERROR_GENERIC;
	struct boot_image_header *header = src;
	uint32_t body_size = 0;
	uint8_t context[] __aligned(CACHELINE_LEN) = "SL2610-IMAGE-ENCRYPTION";
	uint32_t label[4] __aligned(CACHELINE_LEN);
	uint8_t skey[KEY_LEN_AES256] __aligned(CACHELINE_LEN) = {0};

	if (!header || header->length < sizeof(*header) || header->length > size
			|| !is_iv_set(header->iv, sizeof(header->iv))) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	body_size = header->length - sizeof(*header);

	label[0] = header->img_id;
	label[1] = header->img_format_ver;
	label[2] = header->prod_img_flag;
	label[3] = header->segid;
	ret = mailbox_derive_keys(ROOT_KEY_ID_CCGK,
				context, sizeof(context) - 1,
				(uint8_t *)label, sizeof(label),
				skey, KEY_LEN_AES256 * 8);
	if (ret != TEE_SUCCESS) {
		EMSG("Fail to derive key, ret = 0x%x.\n", ret);
		goto out;
	}

	ret = decrypt_sw_image(skey, KEY_LEN_AES256, header->iv, 16,
							  ((uint8_t *)header + sizeof(*header)), body_size,
							  dst, body_size);
	if (ret != TEE_SUCCESS) {
		EMSG("Fail to decrypt sw image, ret = 0x%x.\n", ret);
		goto out;
	}

	ret = TEE_SUCCESS;
out:
	return ret;
}

static struct mobj *get_param_mobj(paddr_t pa, size_t len)
{
	int result = TEE_ERROR_GENERIC;
	struct mobj *mobj = NULL;
	paddr_t *pages = NULL;
	int i;
	void *va;
	int num_pages;

	num_pages = (len + SMALL_PAGE_SIZE - 1) / SMALL_PAGE_SIZE;

	if (core_pbuf_is(CORE_MEM_NSEC_SHM, pa, len)) {
		va = phys_to_virt(pa, MEM_AREA_NSEC_SHM, 1);
		if (!va) {
			EMSG("Fail to transfer va to pa address.\n");
			result = TEE_ERROR_BAD_PARAMETERS;
			goto out;
		}
		mobj = mobj_shm_alloc(pa, len, 0);
	} else {
		if (pa & SMALL_PAGE_MASK) {
			EMSG("Command buffer spans across page boundary pa = 0x%08" PRIxPA, pa);
			result = TEE_ERROR_BAD_PARAMETERS;
			goto out;
		}
		pages = malloc(sizeof(paddr_t) *num_pages);
		if (!pages) {
			result = TEE_ERROR_OUT_OF_MEMORY;
			goto out;
		}
		memset(pages, 0, sizeof(paddr_t) *num_pages);
		for (i = 0; i < num_pages; i++) {
			pages[i] = pa + i * SMALL_PAGE_SIZE;
		}
		mobj = mobj_mapped_shm_alloc(pages, num_pages, 0, 0);
		if (!mobj) {
			result = TEE_ERROR_OUT_OF_MEMORY;
			EMSG(" Fail to map shm pa = 0x%08" PRIxPA " len = 0x%" PRIxPASZ, pa, len);
		}
	}
	result = TEE_SUCCESS;

out:
	if (pages) {
		memset(pages, 0, sizeof(paddr_t) *num_pages);
		free(pages);
	}

	if (result == TEE_SUCCESS)
		return mobj;

	if (mobj) {
		mobj_dec_map(mobj);
		mobj_put(mobj);
	}
	return NULL;
}

static TEE_Result verify_syna_image(paddr_t src, unsigned int size, paddr_t dst, unsigned int *dst_size __unused)
{
	TEE_Result ret = TEE_ERROR_GENERIC;
	struct boot_image_header *header = NULL;

	struct mobj *src_mobj = NULL;
	struct mobj *dst_mobj = NULL;

	void *src_va = NULL;
	void *dst_va = NULL;

	paddr_t pa_src = src & ~SMALL_PAGE_MASK;
	paddr_t pa_dst = dst & ~SMALL_PAGE_MASK;

	uint32_t src_off = src - pa_src;
	uint32_t dst_off = dst - pa_dst;

	uint32_t body_size = 0;
	uint8_t hash_msg[TEE_SHA512_HASH_SIZE] __aligned(CACHELINE_LEN) = {0};
	uint32_t digest_len = sizeof(hash_msg);

	bool in_place = (pa_src == pa_dst);

	DMSG("verify_syna_image: size = 0x%x, src=%"PRIxPA" dst=%"PRIxPA, size, src, dst);

	/* -----------------------------
	 * 1. Map source
	 * ----------------------------- */
	src_mobj = get_param_mobj(pa_src, size + src_off);
	if (!src_mobj) {
		EMSG("Fail to get mobj for src @ 0x%08" PRIxPA " size = 0x%x", pa_src, size);
		ret = TEE_ERROR_COMMUNICATION;
		goto out;
	}

	src_va = mobj_get_va(src_mobj, src_off, size);
	if (!src_va) {
		EMSG("Fail to get va address.\n");
		ret = TEE_ERROR_COMMUNICATION;
		goto out;
	}

	/* -----------------------------
	 * 2. Map destination (ONLY if needed)
	 * ----------------------------- */
	if (in_place) {
		dst_mobj = src_mobj;
		mobj_get(dst_mobj);   // reference bump
	} else {
		dst_mobj = get_param_mobj(pa_dst, size + dst_off);
		if (!dst_mobj) {
		    EMSG("dst mobj fail");
		    ret = TEE_ERROR_COMMUNICATION;
		    goto out;
		}
	}

	dst_va = mobj_get_va(dst_mobj, dst_off, size);
	if (!dst_va) {
		EMSG("Fail to get dst va address.\n");
		ret = TEE_ERROR_COMMUNICATION;
		goto out;
	}

	/* -----------------------------
	 * 3. Parse header
	 * ----------------------------- */
	header = memalign(CACHELINE_LEN, sizeof(*header));
	if (!header) {
		ret = TEE_ERROR_SHORT_BUFFER;
		goto out;
	}

	memcpy(header, src_va, sizeof(*header));

	if (header->length < sizeof(*header) || header->length > size) {
		EMSG("Invalid image length");
		ret = TEE_ERROR_SECURITY;
		goto out;
	}

	body_size = header->length - sizeof(*header);

	/* -----------------------------
	 * 4. Verify header signature
	 * ----------------------------- */
	digest_len = TEE_SHA512_HASH_SIZE;
	memset(hash_msg, 0, sizeof(hash_msg));
	ret = appcrypto_hash(hash_msg, &digest_len, src_va,
					sizeof(*header) - sizeof(header->signature));
	if (ret != TEE_SUCCESS) {
		EMSG("appcrypto_hash fail: ret = 0x%x", ret);
		goto out;
	}

	cache_operation(TEE_CACHEFLUSH, hash_msg, sizeof(hash_msg));
	cache_operation(TEE_CACHEFLUSH, header, sizeof(*header));

	ret = mailbox_verify_signature(header->img_id, ALG_ECC_P521_SHA512,
					virt_to_phys(header->signature), virt_to_phys(hash_msg));
	if (ret != TEE_SUCCESS) {
		dump_hash(hash_msg, sizeof(hash_msg));
		EMSG("Fail to verify header signature: ret = 0x%x", ret);
		goto out;
	}

	/* -----------------------------
	 * 5. Handle image copy / decrypt
	 * ----------------------------- */
	if (!is_iv_set(header->iv, sizeof(header->iv))) {
		DMSG("Clear image copy");
		memcpy(dst_va, (uint8_t *)src_va + sizeof(*header), body_size);
	} else {
		DMSG("Encrypted image");

		ret = mailbox_verify_image(src_va, dst_va, size);
		if (ret)
		    goto out;
	}

	/* -----------------------------
	 * 6. Verify body hash
	 * ----------------------------- */
	digest_len = TEE_SHA512_HASH_SIZE;
	memset(hash_msg, 0, sizeof(hash_msg));
	ret = appcrypto_hash(hash_msg, &digest_len,
				dst_va, body_size);
	if (ret != TEE_SUCCESS) {
		EMSG("appcrypto_hash fail: ret = 0x%x", ret);
		goto out;
	}

	if (memcmp(hash_msg, header->checksum, digest_len)) {
		dump_hash(header->checksum, sizeof(header->checksum));
		EMSG("Body hash mismatch");
		dump_hash(hash_msg, digest_len);
		ret = TEE_ERROR_CORRUPT_OBJECT;
		goto out;
	}


	ret = TEE_SUCCESS;

out:
	if (src_mobj) {
		mobj_dec_map(src_mobj);
		mobj_put(src_mobj);
	}

	if (dst_mobj && dst_mobj != src_mobj) {
		mobj_dec_map(dst_mobj);
		mobj_put(dst_mobj);
	}

	if (header) {
		memset(header, 0, sizeof(*header));
		free(header);
	}
	return ret;
}

int verify_image(paddr_t src, unsigned int size, paddr_t dst, unsigned int *dst_size,
		unsigned int image_type __unused, unsigned int multi_keys __unused)
{
	int ret = TEE_ERROR_GENERIC;

	if (!src || !size) {
		EMSG("src can not be NULL\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	ret = verify_syna_image(src, size, dst, dst_size);

	return ret;
}
