// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "types_ext.h"
#ifdef CONFIG_GEN2
#include "image_gen2.h"
#else
#include "image_genx.h"
#endif
TEE_Result verify_ta(const TEE_UUID *uuid, const uint8_t *nw, size_t nw_size, void *sw, size_t *sw_ta_size);

#endif /* _IMAGE_H_ */
