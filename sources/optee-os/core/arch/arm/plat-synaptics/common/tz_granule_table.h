// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _TZ_GRANULE_TABLE_H_
#define _TZ_GRANULE_TABLE_H_
#include "scatter_list.h"

int mark_pages_in_granuletable(scatter_list_t *s_list);
int clear_pages_in_granuletable(scatter_list_t *s_list);
int try_mark_pages_in_granuletable(scatter_list_t *s_list);

#endif /* _TZ_GRANULE_TABLE_H_ */
