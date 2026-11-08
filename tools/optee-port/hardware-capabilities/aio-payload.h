/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LOOP_AIO_PAYLOAD_H
#define LOOP_AIO_PAYLOAD_H
#include "tee_internal_api.h"
TEE_Result loop_aio_payload_check(uint32_t types, TEE_Param params[4], int capture_active);
#endif
