// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
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
void szc_set_rule_master(unsigned int rule);

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
