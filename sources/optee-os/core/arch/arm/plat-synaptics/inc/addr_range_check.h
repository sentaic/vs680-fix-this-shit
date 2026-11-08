// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _ADDR_RANGE_CHECK_H_
#define _ADDR_RANGE_CHECK_H_

/* size can't be zero and both MACRO include round up check of size */
#define ADDR_REGION_IS_OVERLAP(addr1, size1, addr2, size2) \
	(!(size1 && size2 && (((addr1 < addr2) && (size1 <= (addr2 - addr1)) && ((addr1 + size1) <= addr2)) || \
	((addr1 >= (addr2 + size2)) && (size2 <= (addr1 - addr2))))))

#define ADDR_REGION_IN_RANGE(addr1, size1, addr2, size2) \
	(size1 && size2 && (addr1 >= addr2) && (addr1 < (addr2 + size2)) && (size1 <= size2) && \
	((addr1 + size1) > addr2) && ((addr1 + size1) <= (addr2 + size2)))

#define ADDR_IN_RANGE(addr1, addr2, size2) \
	((addr1 >= addr2) && (addr1 < (addr2 + size2)))

#endif
