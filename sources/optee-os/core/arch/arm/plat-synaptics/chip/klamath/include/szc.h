/*
 * Copyright (c) 2013-2020 Synaptics International Ltd. and its affiliates.
 * All rights reserved.
 *
 * SYNAPTICS CONFIDENTIAL
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Synaptics International Ltd or its
 * suppliers or licensors. Title to the Material remains with Synaptics
 * International Ltd or its suppliers and licensors. The Material contains
 * trade secrets and proprietary and confidential information of Synaptics or
 * its suppliers and licensors.
 * The Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Synaptics's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Synaptics in writing.
 */
#ifndef _SZC_REG_H_
#define _SZC_REG_H_
#include "szc_mst.h"
#include "config.h"

#define RULE_S_RD	1
#define RULE_S_WR	(1 << 1)
#define RULE_NS_RD	(1 << 2)
#define RULE_NS_WR	(1 << 3)

#define MST_RULE_SRW_NRW	(RULE_S_RD | RULE_S_WR | RULE_NS_RD | RULE_NS_WR)
#define MST_RULE_SRW_NRO	(RULE_S_RD | RULE_S_WR | RULE_NS_RD)
#define MST_RULE_SRW_NNA	(RULE_S_RD | RULE_S_WR)
#define MST_RULE_SRO_NRW	(RULE_S_RD | RULE_NS_RD | RULE_NS_WR)
#define MST_RULE_SWO_NNA	(RULE_S_WR)
#define MST_RULE_SNA_NRW	(RULE_NS_RD | RULE_NS_WR)
#define MST_RULE_SNA_NNA	(0)

#define RULE_M_ID		(0xff000000)
#define RULE_M_MST		(0x00ff0000)
#define RULE_M_PERM		(0x000000ff)
#define RULE_S_ID		(24)
#define RULE_S_MST		(16)
#define RULE_S_PERM		(0)

#define RULE_GET_ID(attr)			(((attr) & RULE_M_ID) >> RULE_S_ID)
#define RULE_GET_MST(attr)			(((attr) & RULE_M_MST) >> RULE_S_MST)
#define RULE_GET_PERM(attr)			(((attr) & RULE_M_PERM) >> RULE_S_PERM)
#define RULE_ATTR(id, master, perm)	((((id) << RULE_S_ID) & RULE_M_ID) | \
					(((master) << RULE_S_MST)  & RULE_M_MST) | \
					((perm) << RULE_S_PERM))

void szc_set_rule_master_attr(unsigned int rule_index, unsigned int master_index, unsigned int s_rd, unsigned int s_wr, unsigned int ns_rd, unsigned int ns_wr);
void szc_set_rule_lock(unsigned int lock_mask);
void szc_set_rule_sel(unsigned int sel);
void szc_set_rule_enable(unsigned int enable);
void szc_set_rule_enable_per_master(unsigned int master_index, unsigned int enable);
void szc_set_rule_index_sel(unsigned int index);
int szc_get_rule_num(void);

int szc_get_zones(void *mz, size_t *size);
int szc_get_zones_cnt(void);
#endif /* _SZC_REG_H_ */
