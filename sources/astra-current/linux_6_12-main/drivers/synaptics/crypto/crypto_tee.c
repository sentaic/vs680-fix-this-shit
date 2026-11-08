// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/tee_drv.h>
#include <linux/uuid.h>
#include "crypto_tee.h"

/* -------------------------------------------------------------------------
 * 1. TEE Crypto Core Implementation
 * -------------------------------------------------------------------------
 */

/* TA UUID */
static const uuid_t ta_crypto_uuid =
	UUID_INIT(0x1316a183, 0x894d, 0x43fe, 0x98, 0x93, 0xbb, 0x94,
		0x6a, 0xe1, 0x04, 0x20);

static int match_tee_device(struct tee_ioctl_version_data *ver, const void *data)
{
	return ver->impl_id == TEE_IMPL_ID_OPTEE;
}

int aes_hwkey_gcm_init(struct aes_gcm_ctx *ctx, const u8 *iv, const u8 *aad,
				size_t aad_len, const u8 *tag, int is_encrypt)
{
	if (!ctx || !iv || !aad)
		return -EINVAL;

	memset(ctx, 0, sizeof(*ctx));
	ctx->is_encrypt = is_encrypt;
	memcpy(ctx->iv, iv, AES_GCM_IV_SIZE);

	ctx->aad_len = aad_len > AES_AAD_MAX_LEN ? AES_AAD_MAX_LEN : aad_len;
	memcpy(ctx->aad, aad, ctx->aad_len);

	/* If decrypting, pre-store the Tag to be verified by the TA */
	if (!is_encrypt && tag)
		memcpy(ctx->tag, tag, AES_GCM_TAG_SIZE);

	return 0;
}
EXPORT_SYMBOL(aes_hwkey_gcm_init);

int aes_hwkey_gcm_process(struct aes_gcm_ctx *ctx, const u8 *in, u8 *out, size_t len)
{
	struct tee_context *tee_ctx = NULL;
	struct tee_ioctl_open_session_arg sess_arg;
	struct tee_param param[4];
	struct tee_shm *shm_payload = NULL, *shm_iv = NULL, *shm_aad = NULL;
	struct tee_ioctl_invoke_arg inv_arg;
	int rc = 0;

	/* A. Open Context & Session */
	tee_ctx = tee_client_open_context(NULL, match_tee_device, NULL, NULL);
	if (IS_ERR(tee_ctx))
		return PTR_ERR(tee_ctx);

	memset(&sess_arg, 0, sizeof(sess_arg));
	export_uuid(sess_arg.uuid, &ta_crypto_uuid);
	sess_arg.clnt_login = TEE_IOCTL_LOGIN_PUBLIC;

	rc = tee_client_open_session(tee_ctx, &sess_arg, NULL);
	if (rc < 0)
		goto out_ctx;

	/* B. distribute Payload SHM (Payload len + 16 bytes Tag) */
	shm_payload = tee_shm_alloc_kernel_buf(tee_ctx, len + AES_GCM_TAG_SIZE);
	if (IS_ERR(shm_payload)) {
		rc = PTR_ERR(shm_payload);
		goto out_sess;
	}

	memcpy(tee_shm_get_va(shm_payload, 0), in, len);

	if (!ctx->is_encrypt) {
		/* Put the verification tag after the ciphertext. */
		memcpy((u8 *)tee_shm_get_va(shm_payload, 0) + len,
		       ctx->tag, AES_GCM_TAG_SIZE);
	}

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT;
	param[0].u.value.a = ctx->is_encrypt;

	param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT;
	param[1].u.memref.shm = shm_payload;
	param[1].u.memref.size = len + AES_GCM_TAG_SIZE;

	/* IV SHM */
	shm_iv = tee_shm_alloc_kernel_buf(tee_ctx, AES_GCM_IV_SIZE);
	if (IS_ERR(shm_iv)) {
		rc = PTR_ERR(shm_iv);
		goto out_shm_payload;
	}
	memcpy(tee_shm_get_va(shm_iv, 0), ctx->iv, AES_GCM_IV_SIZE);
	param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT;
	param[2].u.memref.shm = shm_iv;
	param[2].u.memref.size = AES_GCM_IV_SIZE;

	/* AAD SHM */
	shm_aad = tee_shm_alloc_kernel_buf(tee_ctx, ctx->aad_len);
	if (IS_ERR(shm_aad)) {
		rc = PTR_ERR(shm_aad);
		goto out_shm_iv;
	}
	memcpy(tee_shm_get_va(shm_aad, 0), ctx->aad, ctx->aad_len);
	param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT;
	param[3].u.memref.shm = shm_aad;
	param[3].u.memref.size = ctx->aad_len;

	/* D. Invoke TA */
	memset(&inv_arg, 0, sizeof(inv_arg));
	inv_arg.func = TA_CMD_AES_GCM_PROCESS;
	inv_arg.session = sess_arg.session;
	inv_arg.num_params = 4;

	rc = tee_client_invoke_func(tee_ctx, &inv_arg, param);

	/* E. Result */
	if (rc == 0 && inv_arg.ret == 0) {
		memcpy(out, tee_shm_get_va(shm_payload, 0), len);
		if (ctx->is_encrypt) {
			/* Get Tag from the tail */
			memcpy(ctx->tag, (u8 *)tee_shm_get_va(shm_payload, 0) + len,
			       AES_GCM_TAG_SIZE);
		}
	} else {
		/* 0xffff3071 = MAC Mismatch */
		rc = (inv_arg.ret == 0xffff3071) ? -EBADMSG : -EINVAL;
	}

	/* F. buf release */
	tee_shm_free(shm_aad);
out_shm_iv:
	tee_shm_free(shm_iv);
out_shm_payload:
	tee_shm_free(shm_payload);
out_sess:
	tee_client_close_session(tee_ctx, sess_arg.session);
out_ctx:
	tee_client_close_context(tee_ctx);
	return rc;
}
EXPORT_SYMBOL(aes_hwkey_gcm_process);

int aes_hwkey_gcm_get_tag(struct aes_gcm_ctx *ctx, u8 *tag)
{
	if (!ctx || !tag)
		return -EINVAL;

	if (ctx->is_encrypt) {
		memcpy(tag, ctx->tag, 16);
		return 0;
	}
	return -EINVAL;
}
EXPORT_SYMBOL(aes_hwkey_gcm_get_tag);


/* -------------------------------------------------------------------------
 * 2. Debugfs Test Interface (Wrapped with #ifdef)
 * -------------------------------------------------------------------------
 */
#ifdef ENABLE_CRYPTO_TEST

static struct dentry *debug_dir;

static u32 test_is_encrypt = 1;
static u8 test_iv[AES_GCM_IV_SIZE] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0x0c
};
static u8 test_aad[AES_AAD_MAX_LEN] = {
	0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22
};
static u8 test_tag[AES_GCM_TAG_SIZE] = {0};
static u8 test_payload[256];
static u8 test_out[256];
static size_t test_payload_len;
static size_t test_aad_len = 8;

static ssize_t hex_write_helper(const char __user *buf, size_t count,
				u8 *dest, size_t max, size_t *actual)
{
	char *k_buf;
	char *data;
	size_t hex_len;

	if (count < 2 || count > 1024)
		return -EINVAL;

	k_buf = kmalloc(count + 1, GFP_KERNEL);
	if (!k_buf)
		return -ENOMEM;

	if (copy_from_user(k_buf, buf, count)) {
		kfree(k_buf);
		return -EFAULT;
	}
	k_buf[count] = '\0';

	data = strim(k_buf);
	hex_len = strlen(data);

	if (hex_len % 2 != 0 || (hex_len / 2) > max) {
		pr_err("test: Hex string error (len:%zu, max:%zu)\n",
		       hex_len, max);
		kfree(k_buf);
		return -EINVAL;
	}

	*actual = hex_len / 2;

	if (hex2bin(dest, data, *actual)) {
		pr_err("test: Invalid hex character detected!\n");
		kfree(k_buf);
		return -EINVAL;
	}

	pr_info("test: Successfully converted %zu bytes from hex\n", *actual);
	kfree(k_buf);
	return count;
}

static ssize_t p_write(struct file *f, const char __user *b, size_t c, loff_t *p)
{
	return hex_write_helper(b, c, test_payload, 256, &test_payload_len);
}

static ssize_t i_write(struct file *f, const char __user *b, size_t c, loff_t *p)
{
	size_t d;

	return hex_write_helper(b, c, test_iv, AES_GCM_IV_SIZE, &d);
}

static ssize_t a_write(struct file *f, const char __user *b, size_t c, loff_t *p)
{
	return hex_write_helper(b, c, test_aad, AES_AAD_MAX_LEN, &test_aad_len);
}

static ssize_t t_write(struct file *f, const char __user *b, size_t c, loff_t *p)
{
	size_t d;

	return hex_write_helper(b, c, test_tag, AES_GCM_TAG_SIZE, &d);
}

static const struct file_operations f_p = { .write = p_write, .open = simple_open };
static const struct file_operations f_i = { .write = i_write, .open = simple_open };
static const struct file_operations f_a = { .write = a_write, .open = simple_open };
static const struct file_operations f_t = { .write = t_write, .open = simple_open };

static int run_crypto_test(void *data, u64 val)
{
	struct aes_gcm_ctx ctx;
	int ret;

	pr_info("=== [Test] Mode: %s, Data Len: %zu ===\n",
		test_is_encrypt ? "ENC" : "DEC", test_payload_len);

	aes_hwkey_gcm_init(&ctx, test_iv, test_aad, test_aad_len,
				test_is_encrypt ? NULL : test_tag, test_is_encrypt);

	memset(test_out, 0, sizeof(test_out));
	ret = aes_hwkey_gcm_process(&ctx, test_payload, test_out, test_payload_len);

	if (ret == 0) {
		print_hex_dump(KERN_INFO, "Result Data: ", DUMP_PREFIX_OFFSET,
					16, 1, test_out, test_payload_len, true);
		if (test_is_encrypt) {
			aes_hwkey_gcm_get_tag(&ctx, test_tag);
			print_hex_dump(KERN_INFO, "Generated Tag: ", DUMP_PREFIX_OFFSET,
						16, 1, test_tag, 16, false);
		}
	} else {
		pr_err("=== FAILED: %d ===\n", ret);
	}
	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(run_ops, NULL, run_crypto_test, "%llu\n");

#endif /* ENABLE_CRYPTO_TEST */


/* -------------------------------------------------------------------------
 * 3. Module Init & Exit
 * -------------------------------------------------------------------------
 */

static int __init crypto_tee_init(void)
{
#ifdef ENABLE_CRYPTO_TEST
	debug_dir = debugfs_create_dir("test", NULL);
	if (!debug_dir)
		return -ENOMEM;

	debugfs_create_u32("is_encrypt", 0644, debug_dir, &test_is_encrypt);
	debugfs_create_file("payload", 0644, debug_dir, NULL, &f_p);
	debugfs_create_file("iv", 0644, debug_dir, NULL, &f_i);
	debugfs_create_file("aad", 0644, debug_dir, NULL, &f_a);
	debugfs_create_file("tag", 0644, debug_dir, NULL, &f_t);
	debugfs_create_file("run", 0200, debug_dir, NULL, &run_ops);
#endif /* ENABLE_CRYPTO_TEST */

	return 0;
}

static void __exit crypto_tee_exit(void)
{
#ifdef ENABLE_CRYPTO_TEST
	debugfs_remove_recursive(debug_dir);
#endif /* ENABLE_CRYPTO_TEST */
}

module_init(crypto_tee_init);
module_exit(crypto_tee_exit);

MODULE_DESCRIPTION("OP-TEE Crypto Provider");
MODULE_LICENSE("GPL");
