// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <initcall.h>
#include <trace.h>
#include "tee_api_syna.h"
#include <string.h>
#include <kernel/mutex.h>
#include <kernel/tee_ta_manager.h>
#include "svc_syna.h"

static struct mutex sa_mutex = MUTEX_INITIALIZER;
static struct SecurityAttribute sa[MAX_ENTRY_NUM];

/* white list of TAs */
const TEE_UUID WhiteList[MAX_ENTRY_NUM] = {
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x0e}},	/* TA_WIDEVINE_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x0b}},	/* TA_PR25_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x0d}},	/* TA_PR40_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x0c}},	/* TA_PR32_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x07}},	/* TA_VMX_ULTRA_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x37}},	/* TA_IRDETO_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x34}},	/* TA_SYNAMEDIA_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x33}}, /* TA_SYNAMEDIA_HDI_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x03, 0xff}},	/* TA_NOCSBCM_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x03, 0xfd}},	/* TA_CERT_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x03, 0xfe}}, /* TA_KEYGEN_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x01}},	/* TA_KM_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x05}},	/* TA_CCL_UUID */
	{0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x03}},	/* TA_TALTS_UUID */
};

static void sa_mutex_lock(void)
{
	mutex_lock(&sa_mutex);
}

static void sa_mutex_unlock(void)
{
	mutex_unlock(&sa_mutex);
}

static TEE_Result init_sa_entry(void)
{
	int i;
	for (i = 0; i < MAX_ENTRY_NUM; i++) {
		memset(&sa[i], 0, sizeof(struct SecurityAttribute));
		sa[i].index = 0;
		sa[i].sessionId = -1;
		sa[i].hdcpVersion = -1;
		sa[i].ContentStreamType = -1;
		memcpy(&sa[i].uuid, &WhiteList[i], sizeof(TEE_UUID));
	}

	return TEE_SUCCESS;
}

TEE_Result syscall_syna_get_sa_entry(void* sa_entry, uint32_t keySessionId)
{
	int i;
	int ret = TEE_ERROR_GENERIC;
	struct SecurityAttribute* sessionEntry = (struct SecurityAttribute *)sa_entry;

	if (!sessionEntry)
		return TEE_ERROR_BAD_PARAMETERS;
#if 0
	if (!virt_in_usr(sessionEntry, sizeof(*sessionEntry))) {
		EMSG("sessionEntry can't exceed user space\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
#endif
	if ((int32_t)keySessionId == -1) {
		memset(sessionEntry, 0, sizeof(struct SecurityAttribute));
		sessionEntry->index = 0;
		sessionEntry->sessionId = -1;
		sessionEntry->hdcpVersion = -1;
		sessionEntry->ContentStreamType = -1;
		return TEE_SUCCESS;
	}

	sa_mutex_lock();

	for (i = 0; i < MAX_ENTRY_NUM; i++) {
		if (sa[i].sessionId == (int32_t)keySessionId) {
			memcpy(sessionEntry, &sa[i], sizeof(struct SecurityAttribute));
			ret = TEE_SUCCESS;
			goto out;
		}
	}
	ret = TEE_ERROR_ITEM_NOT_FOUND;

out:
	sa_mutex_unlock();
	return ret;
}

TEE_Result syscall_syna_set_sa_entry(void *buff)
{
	int i, j;
	int ret = TEE_ERROR_GENERIC;
	struct ts_session *s = ts_get_current_session();
	const TEE_UUID *uuid = NULL;
	struct SecurityAttribute *sessionEntry = (struct SecurityAttribute *)buff;

	if (!s)
		return TEE_ERROR_BAD_PARAMETERS;

	uuid = &s->ctx->uuid;

	if (!sessionEntry)
		return TEE_ERROR_BAD_PARAMETERS;
#if 0
	if (!virt_in_usr(sessionEntry, sizeof (*sessionEntry))) {
		EMSG("sessionEntry can't exceed user space\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
#endif
	sa_mutex_lock();

	if (sessionEntry->sessionId == -1) {
		for (i = 0; i < MAX_ENTRY_NUM; i++) {
			if (!memcmp(&sa[i].uuid, uuid, sizeof(TEE_UUID))) {
				memset(&sa[i], 0, sizeof(struct SecurityAttribute));
				sa[i].index = 0;
				sa[i].sessionId = -1;
				sa[i].hdcpVersion = -1;
				sa[i].ContentStreamType = -1;
				memcpy(&sa[i].uuid, uuid, sizeof(TEE_UUID));
				ret = TEE_SUCCESS;
				goto out;
			}
		}
		EMSG("reset SA failed\n");
		ret = TEE_ERROR_ITEM_NOT_FOUND;
		goto out;
	}

	for (i = 0; i < MAX_ENTRY_NUM; i++) {
		if (!memcmp(&sa[i].uuid, uuid, sizeof(TEE_UUID))) {
			memcpy(&sa[i], sessionEntry, sizeof(struct SecurityAttribute));
			sa[i].receivedUsageRules = 1;

			/* set other SAs that have same hdcpversion to deactive */
			for (j = 0; j < MAX_ENTRY_NUM; j++) {
				if (j != i && sa[i].hdcpVersion == sa[j].hdcpVersion )
					sa[j].receivedUsageRules= -1;
			}

			ret = TEE_SUCCESS;
			goto out;
		}
	}

	EMSG("set SA failed\n");

	ret = TEE_ERROR_ITEM_NOT_FOUND;
out:
	sa_mutex_unlock();
	return ret;;

}

TEE_Result syscall_syna_get_active_entries(int *num)
{
	int saNum = 0;

	sa_mutex_lock();
	for (int i = 0; i < MAX_ENTRY_NUM; i++)
		if (sa[i].receivedUsageRules == 1)
			saNum++;
	sa_mutex_unlock();

	if (!num)
		*num = saNum;
	return TEE_SUCCESS;
}

TEE_Result syscall_syna_get_active_sa_entry(void *buf)
{
	int highestHDCPIndex = -1;
	int highestHDCPVersion = -1;
	int ret = TEE_ERROR_GENERIC;
	struct SecurityAttribute *sessionEntry = (struct SecurityAttribute *)buf;

	if (!sessionEntry)
		return TEE_ERROR_BAD_PARAMETERS;
#if 0
	if (!virt_in_usr(sessionEntry, sizeof(*sessionEntry))) {
		EMSG("sessionEntry can't exceed user space\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
#endif
	sa_mutex_lock();

	for (int i = 0; i < MAX_ENTRY_NUM; i++) {
		if (sa[i].receivedUsageRules == 1) {
			if (sa[i].hdcpVersion > highestHDCPVersion) {
				highestHDCPVersion = sa[i].hdcpVersion;
				highestHDCPIndex = i;
			}
		}
	}

	if (highestHDCPVersion != -1) {
		memcpy(sessionEntry, &sa[highestHDCPIndex], sizeof(struct SecurityAttribute));
		ret = TEE_SUCCESS;
		goto out;
	}

	ret = TEE_ERROR_ITEM_NOT_FOUND;
out:
	sa_mutex_unlock();

	return ret;
}

service_init(init_sa_entry);

