// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _SCATTER_LIST_H_
#define _SCATTER_LIST_H_

typedef struct {
	unsigned int addr; // physical address
	unsigned int npage; // number of scatter
} scatter_item_t;

typedef struct {
	unsigned int num; // number of element in the table
	scatter_item_t s_item[0]; // point to the first element of scatter_item
} scatter_list_t;

#define SIZE_OF_SCATTERLIST(num) (sizeof(unsigned int) + (num * sizeof(scatter_item_t)))

#endif /* _SCATTER_LIST_H_ */
