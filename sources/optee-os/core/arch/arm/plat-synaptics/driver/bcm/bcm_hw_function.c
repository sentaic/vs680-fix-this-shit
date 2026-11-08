// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <stdint.h>
#include <string.h>
#include <mm/core_memprot.h>
#include <util.h>
#include <tee/cache.h>
#include <rng_support.h>
#include <kernel/boot.h>
#include <kernel/tee_common_otp.h>

#include "bcm_mailbox.h"
#include "bcm_cmd.h"
#include "bcm_execute_cmd.h"
#include "bcm_hw_function.h"
#include "bcm_random.h"
#include "bcm_primitive.h"

#include "syna_setup.h"

static int bcm_hwrand(uint8_t *randBuffer, uint32_t randLength)
{
	BCM_MAILBOX_COMMAND cmd = { 0 };
	BCM_MAILBOX_STATUS status;

	cmd.arg0 = (uint32_t)virt_to_phys(randBuffer);
	cmd.arg1 = randLength;
	cmd.command = BCM_GET_RANDOM;

	return sp_exec_cmd(&cmd, sizeof(BCM_MAILBOX_COMMAND),
			&status, sizeof(BCM_MAILBOX_STATUS));
}

int hwrand(uint8_t *randBuffer, uint32_t randLength)
{
	int ret, len;
	uint8_t *tmp = NULL;
	int cache_line = 64; //FXIME //plat_cache_line_len();

	if (randLength > 1024) {
		EMSG("Invaild random length = %d.\n", randLength);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	len = ROUNDUP(randLength, cache_line);
	tmp = memalign(cache_line, len);
	if (tmp == NULL)
		return TEE_ERROR_OUT_OF_MEMORY;

	cache_operation(TEE_CACHEFLUSH, (void *)tmp, len);
	ret = bcm_hwrand(tmp, randLength);
	if (ret)
		return ret;

	/* invalidate dcache after bcm create random number */
	cache_operation(TEE_CACHEINVALIDATE, (void *)tmp, len);
	memcpy(randBuffer, tmp, randLength);
	free(tmp);
	return 0;
}

TEE_Result hw_get_random_bytes(void *buf, size_t len)
{
	int ret = 0;

	ret = hwrand(buf, len);

	if (ret) {
		EMSG("fail to get random: ret = %d.\n", ret);
		return TEE_ERROR_ACCESS_CONFLICT;
	} else
		return TEE_SUCCESS;
}

typedef struct _bg_check_item_t {
	uint32_t addr;
	uint32_t data;
} bg_check_item_t;

#define BG_CHECK_ITEM_COUNT_MAX (180)
#define CACHE_LINE_SHIFT	6
#define CACHE_LINE_SIZE		(1 << CACHE_LINE_SHIFT)
#define CACHE_LINE_ALIGN(x)	(((uint32_t)(x)) & (~(CACHE_LINE_SIZE - 1)))
#define CACHE_LINE_ALIGN_UP(x)	((((uint32_t)(x)) + CACHE_LINE_SIZE - 1) & (~(CACHE_LINE_SIZE - 1)))

static bg_check_item_t _g_bg_item_list[BG_CHECK_ITEM_COUNT_MAX];
static uint32_t _g_bg_item_count = 0;
static uint32_t _g_bg_item_enabled = 0;

void bg_check_init(void)
{
	memset(_g_bg_item_list, 0, sizeof(_g_bg_item_list));
	_g_bg_item_count = 0;
	_g_bg_item_enabled = 0;
}

void bcm_add_bg_check_register(uint32_t addr, uint32_t data)
{
	uint32_t i;

	if (_g_bg_item_enabled)
		return;

	for (i = 0; i < _g_bg_item_count; i++) {
		if (_g_bg_item_list[i].addr == addr) {
			_g_bg_item_list[i].data = data;
			return;
		}
	}

	assert(_g_bg_item_count + 1 <= BG_CHECK_ITEM_COUNT_MAX);

	_g_bg_item_list[_g_bg_item_count].addr = addr;
	_g_bg_item_list[_g_bg_item_count].data = data;
	_g_bg_item_count++;
}

int bcm_enable_bg_checking(void)
{
	BCM_MAILBOX_COMMAND cmd;
	BCM_MAILBOX_STATUS status = {0};

	if (_g_bg_item_enabled)
		return 0;

	assert(_g_bg_item_count <= BG_CHECK_ITEM_COUNT_MAX);
	assert(_g_bg_item_enabled == 0);

	//flush_dcache_range((void *)CACHE_LINE_ALIGN(_g_bg_item_list),
	//			 (void *)CACHE_LINE_ALIGN_UP(_g_bg_item_list + BG_CHECK_ITEM_COUNT_MAX));

	cmd.arg0 = (uint32_t)virt_to_phys((void *)_g_bg_item_list);
	cmd.arg1 = _g_bg_item_count;
	cmd.command = BCM_ENABLE_BG_CHECKING;

	_g_bg_item_enabled = 1;

	return sp_exec_cmd(&cmd, sizeof(BCM_MAILBOX_COMMAND),
			&status, sizeof(BCM_MAILBOX_STATUS));
}


/* get the device unique HMAC key for rpmb */
TEE_Result syna_get_hw_unique_key(size_t len, uint8_t* key)
{
	int result = TEE_ERROR_GENERIC;
	BCM_MAILBOX_COMMAND cmd = {0};
	BCM_MAILBOX_STATUS status = {0};
	uint8_t *gkey;
	uint32_t gkey_len;

	if (!key || len > 32)
		return TEE_ERROR_BAD_PARAMETERS;

	gkey_len = ROUNDUP(len, CACHELINE_LEN);

	gkey = memalign(CACHELINE_LEN, gkey_len);
	if (gkey == NULL)
		return TEE_ERROR_OUT_OF_MEMORY;

	memset(&cmd, 0, sizeof(cmd));
	cmd.arg0 = (unsigned long)virt_to_phys((void*)gkey);
	cmd.arg1 = len;
	cmd.command = BCM_EXTN_GENERATE_RPMB_AUTH_KEY;

	cache_operation(TEE_CACHEFLUSH, gkey, gkey_len);
	result = sp_exec_cmd(&cmd, sizeof(BCM_MAILBOX_COMMAND),
			&status, sizeof(BCM_MAILBOX_STATUS));
	if (result) {
		memset(gkey, 0, gkey_len);
		cache_operation(TEE_CACHEFLUSH, gkey, gkey_len);
		free(gkey);
		return result;
	}
	cache_operation(TEE_CACHEINVALIDATE, gkey, gkey_len);

	memcpy(key, gkey, len);

	memset(gkey, 0, gkey_len);
	cache_operation(TEE_CACHEFLUSH, gkey, gkey_len);
	free(gkey);

	return TEE_SUCCESS;
}

#if defined(CFG_RPMB_FS)
static int otp_authkey_programed =0;

/* r/w the rpmb otp bit, rw = 1 for read, 0 for write*/
static int rw_rpmb_authkey_otp(int *flag, int rw)
{
	int result = TEE_ERROR_GENERIC;
	BCM_MAILBOX_COMMAND cmd = {0};
	BCM_MAILBOX_STATUS status = {0};
	int *buflag;
	uint32_t buflag_len;
	uint32_t cacheline_len = CACHELINE_LEN;

	memset(&cmd, 0, sizeof(cmd));
	/* write OTP */
	if (!rw) {
		cmd.command = DRM_OTP_UTILS_SET_RPMB;
		result = sp_exec_cmd(&cmd, sizeof(BCM_MAILBOX_COMMAND),
				&status, sizeof(BCM_MAILBOX_STATUS));
	} else {
		buflag_len = ROUNDUP(4, cacheline_len);
		buflag = memalign(cacheline_len, buflag_len);
		if (buflag == NULL)
			return TEE_ERROR_OUT_OF_MEMORY;

		cmd.arg0 = (unsigned long)virt_to_phys(buflag);
		cmd.command = DRM_OTP_UTILS_GET_RPMB;
		cache_operation(TEE_CACHEFLUSH, buflag, buflag_len);
		result = sp_exec_cmd(&cmd, sizeof(BCM_MAILBOX_COMMAND),
				&status, sizeof(BCM_MAILBOX_STATUS));
		cache_operation(TEE_CACHEFLUSH, buflag, buflag_len);
		*flag = *buflag;
		free(buflag);
	}


	return result;
}

bool plat_rpmb_key_is_ready(void)
{
	TEE_Result ret = TEE_ERROR_GENERIC;
	struct tee_hw_unique_key hwkey = {0};

	if (tee_otp_get_hw_unique_key(&hwkey))
		return false;

	ret = rw_rpmb_authkey_otp(&otp_authkey_programed, 1);
	if (ret != TEE_SUCCESS){
		EMSG("fail to get otp_authkey_programed status, ret = 0x%x.\n", ret);
		return false;
	}
	DMSG("otp_authkey_programed: %s.\n", otp_authkey_programed? "true":"false");

	if (otp_authkey_programed)
		return true;

	/* if the OTP isn't programmed, program it */
	ret = rw_rpmb_authkey_otp(NULL, 0);
	if (ret != TEE_SUCCESS) {
		EMSG("rpmb program key flag otp error %x\n", ret);
		return false;
	}
	otp_authkey_programed = 1;

	return true;
}
#endif

unsigned long plat_get_aslr_seed(void)
{
	unsigned int seed = 0;
	int ret;

	ret = bcm_get_random_without_wait((uint32_t *)&seed, sizeof(seed));
	if (ret != 0) {
		EMSG("Fail to get ASLR seed, ret = 0x%x.\n", ret);
	} else {
		DMSG("ASLR seed : 0x%x.\n", seed);
	}

	return seed;
}

TEE_Result syna_exec_cmd(uint32_t cmd_id, const void *param,
		uint32_t param_len, const void *result, uint32_t result_len)
{
	BCM_MAILBOX_COMMAND cmd = {0};
#ifdef CFG_TA_PERM_CHECK
	bool allow = false;
	struct ts_session *s = ts_get_current_session();
	const TEE_UUID *uuid = &s->ctx->uuid;
#endif
	if (!param || (param_len > sizeof(BCM_MAILBOX_COMMAND)) ||
		!result || (result_len < sizeof(BCM_MAILBOX_STATUS))) {
		EMSG("param and result must not be NULL, param_len and result_len must be vaild\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}


#ifdef CFG_TA_PERM_CHECK
	if (ta_perm_check_bcm_pi(uuid, cmd_id, &allow) != 0 || allow == false) {
		EMSG("Don't allow to send PI 0x%x\n", cmd_id);
		return TEE_ERROR_ACCESS_DENIED;
	}
#endif
	memcpy(&cmd, param, param_len);
	cmd.command = cmd_id;

	/* call secure processor to execute command */
	return sp_exec_cmd(&cmd, sizeof(cmd), (BCM_MAILBOX_STATUS *)result, result_len);

}

TEE_Result syna_exec_cmd_ex(uint32_t cmd_id, const void *param,
		uint32_t param_len, const void *result, uint32_t result_len, uint32_t delay)
{
	BCM_MAILBOX_COMMAND cmd = {0};

	if (!param || (param_len > sizeof(BCM_MAILBOX_COMMAND)) ||
		!result || (result_len < sizeof(BCM_MAILBOX_STATUS))) {
		EMSG("param and result must not be NULL, param_len and result_len must be vaild\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (delay > 100) {
		EMSG("delay number is too large\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

#ifdef CFG_TA_PERM_CHECK
	struct ts_session *s = ts_get_current_session();
	const TEE_UUID *uuid = &s->ctx->uuid;
	bool allow = false;
	if (ta_perm_check_bcm_pi(uuid, cmd_id, &allow) != 0 || allow == false) {
		EMSG("Don't allow to send PI 0x%x\n", cmd_id);
		return TEE_ERROR_ACCESS_DENIED;
	}
#endif
	memcpy(&cmd, param, param_len);
	cmd.command = cmd_id;

	/* call secure processor to execute command */
	return sp_exec_cmd_ex(&cmd, sizeof(cmd), (BCM_MAILBOX_STATUS *)result, result_len, delay);
}
