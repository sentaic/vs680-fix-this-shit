// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2022 Synaptics Incorporated
 */

#ifndef __M2M_WRAPPER_H__
#define __M2M_WRAPPER_H__
#include <linux/device.h>
#if !IS_ENABLED(CONFIG_OPTEE)
#include "tee_client_api.h"
#endif
#include "uapi/m2m.h"

#if IS_ENABLED(CONFIG_OPTEE)
typedef uint32_t TEEC_Session;
#endif

int m2m_wrapper_init(struct device *dev);
void m2m_wrapper_exit(void);
int m2m_wrapper_open_session(TEEC_Session *teec_sess, unsigned int crypto_mode);
void m2m_wrapper_close_session(TEEC_Session *teec_sess);
int m2m_wrapper_set_scheme(TEEC_Session *teec_sess, unsigned int scheme);
int m2m_wrapper_set_patternMode(TEEC_Session *teec_sess,
				struct m2m_pattern_mode *pattern);
int m2m_wrapper_set_residueMode(TEEC_Session *teec_sess,
				unsigned int residueMode);
int m2m_wrapper_set_ivPolicy(TEEC_Session *teec_sess, unsigned int iv_policy);
int m2m_wrapper_config(TEEC_Session *teec_sess, struct m2m_key *key_config);
int m2m_wrapper_update(TEEC_Session *teec_sess,
				unsigned int inputPhyAddr, size_t inputSize, size_t inputOffset,
				unsigned int outputPhyAddr, size_t outputSize, size_t outputOffset);
int m2m_wrapper_routine(TEEC_Session *teec_sess, unsigned int *cmdFinishNum);
int m2m_wrapper_set_IVPolicy(TEEC_Session *teec_sess, unsigned int iv_policy);

#endif
