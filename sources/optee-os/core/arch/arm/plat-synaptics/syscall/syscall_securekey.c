// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <trace.h>
#include "tee_api_syna.h"
#include "svc_syna.h"
#ifdef CFG_BCM
#include "bcm_mailbox.h"
#include "bcm_primitive.h"
#include "bcm_execute_cmd.h"
#include <malloc.h>
#include <string.h>
#include "svc_syna.h"
#include <tee/cache.h>
#include <types_ext.h>
#include <kernel/tee_ta_manager.h>
#include <platform_config.h>
#include <util.h>
#include <mm/core_memprot.h>

TEE_Result syscall_syna_get_securekey(void *buff)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	BCM_MAILBOX_COMMAND cmd = {0};
	BCM_MAILBOX_STATUS status = {0};
	struct SecureKeyParam* buffer = NULL;
	paddr_t phy_addr;
	size_t len;
	struct ts_session *s = ts_get_current_session();
	const TEE_UUID *uuid = NULL;
	struct SecureKeyParam* sk = (struct SecureKeyParam *)buff;

	if (!s || !sk)
		return TEE_ERROR_BAD_PARAMETERS;

	uuid = &s->ctx->uuid;

	len = ROUNDUP(sizeof(struct SecureKeyParam), CACHELINE_LEN);
	buffer = (struct SecureKeyParam *)memalign(CACHELINE_LEN, len);
	if (buffer == NULL) {
		EMSG("malloc fail\n");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	/* use the TA's uuid */
	memcpy(buffer->uuID, uuid, UUID_SIZE);

	memcpy(buffer->appID, sk->appID, APPID_SIZE);
	buffer->appVariant = sk->appVariant;

	cache_operation(TEE_CACHEFLUSH, buffer, sizeof(struct SecureKeyParam));

	phy_addr = virt_to_phys(buffer);

	memset(&cmd, 0, sizeof(BCM_MAILBOX_COMMAND));

	cmd.arg0 = (uint32_t)phy_addr;						/*phy_addr of uuID*/
	cmd.arg1 = (uint32_t)(phy_addr + UUID_SIZE);				/*phy_addr of appID*/
	cmd.arg2 = (uint32_t)buffer->appVariant;
	cmd.arg3 = (uint32_t)(phy_addr + UUID_SIZE + APPID_SIZE + 4);		/*phy_addr of secureKey*/
	cmd.command = DRM_GENERATE_SECUREKEY;

	result = sp_exec_cmd(&cmd, sizeof(cmd), &status, sizeof(status));
	if (result)
		goto out;

	cache_operation(TEE_CACHEINVALIDATE, buffer, sizeof(struct SecureKeyParam));


	memcpy(sk->secureKey, buffer->secureKey, APP_UNIQUE_KEY_SIZE);

out:
	if (buffer)
		free(buffer);
	return result;
}

TEE_Result syscall_syna_get_securekey2(void * buff)
{
	TEE_Result result = TEE_ERROR_GENERIC;
	BCM_MAILBOX_COMMAND cmd = {0};
	BCM_MAILBOX_STATUS status = {0};
	struct SecureKeyParam* buffer;
	paddr_t phy_addr;
	size_t len;
	struct ts_session *s = ts_get_current_session();
	const TEE_UUID *uuid = NULL;
	struct SecureKeyParam* sk = (struct SecureKeyParam *)buff;

	if (!s || !sk)
		return TEE_ERROR_BAD_PARAMETERS;

	uuid = &s->ctx->uuid;

	len = ROUNDUP(sizeof(struct SecureKeyParam), CACHELINE_LEN);
	buffer = (struct SecureKeyParam *)memalign(CACHELINE_LEN, len);
	if (buffer == NULL) {
		EMSG("malloc fail\n");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	/* use the TA's uuid */
	memcpy(buffer->uuID, uuid, UUID_SIZE);

	memcpy(buffer->appID, sk->appID, APPID_SIZE);
	buffer->appVariant = sk->appVariant;

	cache_operation(TEE_CACHEFLUSH, buffer, sizeof(struct SecureKeyParam));

	phy_addr = virt_to_phys(buffer);

	memset(&cmd, 0, sizeof(BCM_MAILBOX_COMMAND));

	cmd.arg0 = (uint32_t)phy_addr;						/*phy_addr of uuID*/
	cmd.arg1 = (uint32_t)(phy_addr + UUID_SIZE);				/*phy_addr of appID*/
	cmd.arg2 = (uint32_t)buffer->appVariant;
	cmd.arg3 = (uint32_t)(phy_addr + UUID_SIZE + APPID_SIZE + 4);		/*phy_addr of secureKey*/
	cmd.command = DRM_GENERATE_SECUREKEY_V2;

	result = sp_exec_cmd(&cmd, sizeof(cmd), &status, sizeof(status));
	if (result)
		goto out;

	cache_operation(TEE_CACHEINVALIDATE, buffer, sizeof(struct SecureKeyParam));

	memcpy(sk->secureKey, buffer->secureKey, APP_UNIQUE_KEY_SIZE);

out:
	if (buffer)
		free(buffer);
	return result;
}
#else
TEE_Result syscall_syna_get_securekey(void *buff __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}
TEE_Result syscall_syna_get_securekey2(void * buff __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}
#endif
