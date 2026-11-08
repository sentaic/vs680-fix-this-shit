/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 * Copyright (c) 2001-2007, Tom St Denis
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://libtom.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <common/debug.h>
#include "mem4.h"
#include "crypto.h"

#define SHA_MIN(x, y) ( ((x)<(y))?(x):(y) )

#define swab32(x) ((unsigned int)(                         \
	        (((unsigned int)(x) & (unsigned int)0x000000ffUL) << 24) |            \
	        (((unsigned int)(x) & (unsigned int)0x0000ff00UL) <<  8) |            \
	        (((unsigned int)(x) & (unsigned int)0x00ff0000UL) >>  8) |            \
	        (((unsigned int)(x) & (unsigned int)0xff000000UL) >> 24)))

#define swab64(x) ((unsigned long)(                         \
	        (((unsigned long)(x) & (unsigned long)0x00000000000000ffULL) << 56) |   \
	        (((unsigned long)(x) & (unsigned long)0x000000000000ff00ULL) << 40) |   \
	        (((unsigned long)(x) & (unsigned long)0x0000000000ff0000ULL) << 24) |   \
	        (((unsigned long)(x) & (unsigned long)0x00000000ff000000ULL) <<  8) |   \
	        (((unsigned long)(x) & (unsigned long)0x000000ff00000000ULL) >>  8) |   \
	        (((unsigned long)(x) & (unsigned long)0x0000ff0000000000ULL) >> 24) |   \
	        (((unsigned long)(x) & (unsigned long)0x00ff000000000000ULL) >> 40) |   \
	        (((unsigned long)(x) & (unsigned long)0xff00000000000000ULL) >> 56)))

#define FPEXC_EN        (1 << 30)

/* Implemented in assembly */
/* Prototype for assembly function */
void sha256_ce_transform(uint32_t state[8], const void *src,
			 unsigned int block_count);

static int sha256_compress_nblocks(hash_state *md, const unsigned char *buf,
				   int blocks)
{
	unsigned int fpexc;

	fpexc = vfp_read_fpexc();
	vfp_write_fpexc(fpexc | FPEXC_EN);
	sha256_ce_transform(md->sha256.state, buf, blocks);
	vfp_write_fpexc(fpexc & ~FPEXC_EN);
	return 0;
}

static int sha256_compress(hash_state * md, unsigned char *buf)
{
	return sha256_compress_nblocks(md, buf, 1);
}
/**
   Initialize the hash state
   @param md   The hash state you wish to initialize
   @return 0 if successful
*/
int sha256_init(hash_state * md)
{
	assert(md != NULL);

	md->sha256.curlen = 0;
	md->sha256.length = 0;
	md->sha256.state[0] = 0x6A09E667UL;
	md->sha256.state[1] = 0xBB67AE85UL;
	md->sha256.state[2] = 0x3C6EF372UL;
	md->sha256.state[3] = 0xA54FF53AUL;
	md->sha256.state[4] = 0x510E527FUL;
	md->sha256.state[5] = 0x9B05688CUL;
	md->sha256.state[6] = 0x1F83D9ABUL;
	md->sha256.state[7] = 0x5BE0CD19UL;

	return 0;
}

/**
   Process a block of memory though the hash
   @param md     The hash state
   @param in     The data to hash
   @param inlen  The length of the data (octets)
   @return 0 if successful
*/
int sha256_process(hash_state * md, const unsigned char *in, unsigned long inlen)
{
	unsigned long n, blocks;
	int err;

	assert(md != NULL);
	assert(in != NULL);
	if (md->sha256.curlen > sizeof(md->sha256.buf)) {
		return -1;
	}
	while (inlen > 0) {
		if (md->sha256.curlen == 0 && inlen >= SHA256_BLOCK_SIZE) {
			blocks = inlen / SHA256_BLOCK_SIZE;
			if ((err = sha256_compress_nblocks(md, (unsigned char *)in, blocks)) != 0) {
				return err;
			}
			md->sha256.length 	+= blocks * SHA256_BLOCK_SIZE * 8;
			in             		+= blocks * SHA256_BLOCK_SIZE;
			inlen          		-= blocks* SHA256_BLOCK_SIZE;
		} else {
			n = SHA_MIN(inlen, (SHA256_BLOCK_SIZE - md->sha256.curlen));
			memcpy(md->sha256.buf + md->sha256.curlen, in, n);
			md->sha256.curlen 	+= n;
			in             		+= n;
			inlen          		-= n;
			if (md->sha256.curlen == SHA256_BLOCK_SIZE) {
				if ((err = sha256_compress(md, md->sha256.buf)) != 0) {
					return err;
				}
				md->sha256.length += 8*SHA256_BLOCK_SIZE;
				md->sha256.curlen = 0;
			}
		}
	}
	return 0;
}

/**
   Terminate the hash to get the digest
   @param md  The hash state
   @param out [out] The destination of the hash (32 bytes)
   @return 0 if successful
*/
int sha256_done(hash_state * md, unsigned char *out)
{
	int i;
	int *buftmp;
	unsigned long _be_temp;

	assert(md  != NULL);
	assert(out != NULL);

	if (md->sha256.curlen >= sizeof(md->sha256.buf)) {
		return -1;
	}


	/* increase the length of the message */
	md->sha256.length += md->sha256.curlen * 8;

	/* append the '1' bit */
	//md->sha256.buf[md->sha256.curlen++] = (unsigned char)0x80;
	buftmp = (int*)(&md->sha256.buf[md->sha256.curlen]);
	*buftmp = 0x80;
	md->sha256.curlen += 4;
	/* if the length is currently above 56 bytes we append zeros
	 * then compress.  Then we can fall back to padding zeros and length
	 * encoding like normal.
	 */
	if (md->sha256.curlen > 56) {
	//	while (md->sha256.curlen < 64) {
	//		md->sha256.buf[md->sha256.curlen++] = (unsigned char)0;
	//	}
	//	sha256_compress(md, md->sha256.buf);
	//	md->sha256.curlen = 0;
	}

	/* pad upto 56 bytes of zeroes */
	if (md->sha256.curlen < 56) {
		//md->sha256.buf[md->sha256.curlen++] = (unsigned char)0;
		memset4(&md->sha256.buf[md->sha256.curlen], 0, 56 - md->sha256.curlen);
		md->sha256.curlen += (56 - md->sha256.curlen);
	}

	/* store length */
	_be_temp = swab64(md->sha256.length);
	memcpy4(md->sha256.buf + 56, &_be_temp, 8);
	sha256_compress(md, md->sha256.buf);

	/* copy output */
	for (i = 0; i < 8; i++) {
		*(unsigned int*)(out + 4*i) = swab32(md->sha256.state[i]);
	}
	return 0;
}

/*
 * For test purpose
 */
#if 0
hash_state *md = (hash_state*)0x800000;
int  sha256_test(void)
{
	static const struct {
		const char *msg;
		unsigned char hash[32];
	} tests[] = {
		{ "abc",
			{ 0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
				0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
				0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
				0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad }
		},
		{ "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
			{ 0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8,
				0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39,
				0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67,
				0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1 }
		},
	};

	int i;
	unsigned char tmp[32];
	unsigned int save_vfp;

	save_vfp = enable_vfp();

	for (i = 0; i < (int)(sizeof(tests) / sizeof(tests[0])); i++) {
		sha256_init(md);
		sha256_process(md, (unsigned char*)tests[i].msg, (unsigned long)strlen(tests[i].msg));
		sha256_done(md, tmp);
		__asm__ volatile ("b .");
		if (memcmp(tmp, tests[i].hash, 32) != 0) {
			INFO("SHA256 TEST FAIL, i = %d\n", i);
		}
	}
	disable_vfp(save_vfp);
	return 0;
}
#endif
