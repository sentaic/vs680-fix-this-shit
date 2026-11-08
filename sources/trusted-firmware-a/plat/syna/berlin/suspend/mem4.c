/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
/*
 * Fill @count bytes of memory pointed to by @dst with @val
 * make sure the count is multiple of 4
 * it's for zsp sram write of it's limitation
 */
void *memset4(void *dst, int val, int count)
{
	unsigned int *ptr = dst;
	assert((count & 3) == 0);
	count >>= 2;

	while (count--)
		*ptr++ = val;

	return dst;
}

/*
 * Copy @len bytes from @src to @dst
 * make sure the len is multiple of 4
 * it's for zsp sram write of it's limitation
 */
void *memcpy4(void *dst, const void *src, int len)
{
	const unsigned int *s = src;
	unsigned int *d = dst;

	assert ((len & 3) == 0);
	len >>= 2;

	while (len--)
		*d++ = *s++;

	return dst;
}
