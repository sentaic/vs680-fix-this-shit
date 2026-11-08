/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2025, Synaptics Incorporated.
 */

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <string.h>
#include <otp_ta.h>
#include "tee_api_syna.h"

TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	DMSG("Goodbye!\n");
}
#define REE_SegID 0x834137E4

static TEE_Result otp_get_db_enabled(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");
	TEE_Result ret = TEE_SUCCESS;
	uint8_t buf[8] = {0};
	uint8_t dolby_en = 0;
	uint32_t oem_audio_customer_id;


	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	ret = TEE_GetOtpRawData(1, buf);
	if (ret != TEE_SUCCESS) {
		EMSG("TEE_GetOtpRawData fail, return 0x%x\n", ret);
		return ret;
	}
	dolby_en = (buf[1] >> 4) & 0x1 ? 0 : 1;

	DMSG("dolby_en = 0x%x\n", dolby_en);

	memset(buf, 0, sizeof(buf));
	ret = TEE_GetOtpRawData(68, buf);
	if (ret != TEE_SUCCESS) {
		EMSG("TEE_GetOtpRawData fail, return 0x%x\n", ret);
		return ret;
	}

	oem_audio_customer_id = buf[7] << 24 | buf[6] << 16 | buf[5] << 8 | buf[4];
	DMSG("oem_audio_customer_id = 0x%x\n]\n", oem_audio_customer_id);

	params[0].value.a = dolby_en && (oem_audio_customer_id == REE_SegID);

	return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	switch (cmd_id) {
	case TA_OTP_GET_DB_ENABLED:
		return otp_get_db_enabled(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
