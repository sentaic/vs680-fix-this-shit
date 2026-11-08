// SPDX-License-Identifier: GPL-2.0-only
#include <tee_internal_api.h>
#include <stdint.h>

TEE_Result TA_CreateEntryPoint(void)
{
    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t types, TEE_Param params[4], void **session)
{
    (void)params;
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
                                 TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE))
        return TEE_ERROR_BAD_PARAMETERS;
    *session = NULL;
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *session)
{
    (void)session;
}

TEE_Result TA_InvokeCommandEntryPoint(void *session, uint32_t command,
                                     uint32_t types, TEE_Param params[4])
{
    (void)session;
    if (command != 0)
        return TEE_ERROR_NOT_SUPPORTED;
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT, TEE_PARAM_TYPE_NONE,
                                 TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE))
        return TEE_ERROR_BAD_PARAMETERS;
    if (params[0].value.a == UINT32_MAX)
        return TEE_ERROR_OVERFLOW;
    params[0].value.a++;
    params[0].value.b = 0x68680405;
    return TEE_SUCCESS;
}
