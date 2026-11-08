// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _HAL_SOC_H_
#define _HAL_SOC_H_

#include "stdint.h"

/** get phsical protor id by logical id.
 *
 * @param id	logical id.
 *
 * @return physical protor id.
 */
int prot_get_id(int id);

/** initialize soc.
 */
void soc_init(void);

void otp_init(void);

void pcie_init(void);

/** clear TSP, ZSP sram
 */
void clear_sram(void);

/* reset chip by write soc reset register */
void reset_soc(void);

int soc_get_otp_raw_data(uint32_t row, uint8_t *buffer);
int soc_get_otp_prop(uint32_t index, void* dst, int dst_len);

#endif /* _HAL_SOC_H_ */
