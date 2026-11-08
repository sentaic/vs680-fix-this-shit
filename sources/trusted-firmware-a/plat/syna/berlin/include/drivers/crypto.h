/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CRYPTO_H_
#define __CRYPTO_H_

#define CPACR_VFP_BITS         (3 << 20)
#define AARCH64_CPTR_TFP       (1 << 10)

#define SHA256_BLOCK_SIZE       64
#define SHA512_BLOCK_SIZE       128
#define MD5_BLOCK_SIZE       	64

static inline int enable_vfp()
{
	unsigned int cpacr_el1;
	__asm__ volatile (	"mrs %0, cpacr_el1\n\t"
				"orr x1, %0, #(3 << 20)\n\t"
				"msr cpacr_el1, x1\n\t"
				"mrs x1, cptr_el3\n\t"
				"mov x2, #(1 << 10)\n\t"
				"bic x1, x1, x2\n\t"
				"msr cptr_el3, x1\n\t"
				"isb\n\t"
				:"=r" (cpacr_el1)
				:
				:"x1", "x2"
			);
	return cpacr_el1;
}

static inline void disable_vfp(unsigned int cpacr_el1)
{

	__asm__ volatile (	"msr cpacr_el1, %0\n\t"
				"mrs x1, cptr_el3\n\t"
				"mov x2, #(1 << 10)\n\t"
				"orr x1, x1, x2\n\t"
				"msr cptr_el3, x1\n\t"
				"isb\n\t"
				:
				:"r" (cpacr_el1)
				:"x1", "x2"
			);
}

static inline void vfp_write_fpexc(unsigned int fpexc)
{
	__asm__ volatile ("msr	fpexc32_el2, %0": : "r" (fpexc));
}

static inline unsigned int vfp_read_fpexc()
{
	unsigned int fpexc;
	__asm__ volatile ("mrs	%0, fpexc32_el2": "=r" (fpexc));
	return fpexc;
}

struct sha512_state {
    unsigned char buf[128];
    unsigned long state[8];
    unsigned long  length;
    unsigned int curlen;
};

struct sha256_state {
    unsigned char buf[64];
    unsigned int state[8];
    unsigned long  length;
    unsigned int curlen;
};

struct md5_state {
    unsigned char buf[64];
    unsigned int state[4];
    unsigned long  length;
    unsigned int curlen;
};

typedef union Hash_state {
	char dummy[1];
	struct sha512_state sha512;
	struct sha256_state sha256;
	struct md5_state    md5;
	void *data;
} hash_state;


int sha256_init(hash_state *);
int sha256_process(hash_state *, const unsigned char *, unsigned long);
int sha256_done(hash_state *, unsigned char *);

int rand(void);
#endif

