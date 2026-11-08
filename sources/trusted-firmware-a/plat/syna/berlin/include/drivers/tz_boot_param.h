/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TZ_BOOT_PARAM_H_
#define _TZ_BOOT_PARAM_H_

#include <lib/utils.h>

/*
 * setup & retrieve back boot param
 *
 * note: bellow functions are not thread safe.
 */

/*
 * boot parameter definition is platform specified.
 * different platform may have different boot parameters.
 */
#define	TZ_BOOT_PARAM_USER_START	0x00000000
#define	TZ_BOOT_PARAM_SYS_START		0x80000000
#define	TZ_BOOT_PARAM_CMDLINE		0x80000001
#define	TZ_BOOT_PARAM_MEMREGION		0x80000002
#define	TZ_BOOT_PARAM_MEMZONE		0x80000003
#define	TZ_BOOT_PARAM_REGFIREWALL	0x80000004
#define	TZ_BOOT_PARAM_MASTER		0x80000005
#define	TZ_BOOT_PARAM_ANTIROLLBACK	0x80000006
#define	TZ_BOOT_PARAM_TRUST_STORAGE	0x80000007
#define	TZ_BOOT_PARAM_REG_REGION	0x80000008
#define	TZ_BOOT_PARAM_MAX		0x80000009

#define TZ_BOOT_PARAM_MAGIC_NUM		(0x545a2a42)	/* 'TZ*B' */
#define TZ_BOOT_PARAM_VERSION		(0)

#define TZ_BOOT_VERIFY_REGION		0x8000000A

/*
 * boot parameter format
 *
 * +----------------------------+
 * | tz_boot_param_header	|
 * +----------------------------+
 * | tz_boot_param 0		|
 * +----------------------------+
 * | tz_boot_param 1		|
 * +----------------------------+
 * | ...			|
 * +----------------------------+
 */
struct tz_boot_param_header {
	uint32_t	magic_num;	/* 'TZ*B' */
	uint32_t	size;		/* size of whole boot parameters */
	uint32_t	version;
	uint32_t	reserved;
};

struct tz_boot_param {
	uint32_t	size;		/* current boot param size, including header */
	uint32_t	id;
	uint32_t	reserved[2];	/* must be 0 */
	uint32_t	data[];
};

struct region {
	void *base;
	int size;
};

struct tz_verify_region {
	unsigned int nums;
	struct region regs[4];
};

/* memory region */
struct syna_mem_region {
	char			name[32];
	uint32_t		base;	/* base address */
	uint32_t		size;
	uint32_t		attr;	/* attributes */
	uint32_t		reserved;

	/* bellow fields are user defined data.
	 * it's to support multiple platforms by unified structure.
	 * different platform can use them for different purpose.
	 */
	uint32_t		userdata[4];
};

int mr_setup(void);

const struct syna_mem_region *mr_find_region_by_name(const char *name);

/** initialize the boot parameter structure.
 *
 * @param addr		address of boot parameter
 * @param reset		whether to reset the boot parameters.
 * 			when create the boot parameter, then must be true.
 * 			when parse the boot parameter, must be false.
 */
int tz_boot_param_init(void *addr);

/** get boot parameter data by ID.
 *
 * @param id		id of the parameter, see tz_boot_param_id.
 *
 * @return pointer the to new boot parameter structure.
 */
struct tz_boot_param *tz_boot_param_get(uint32_t id);

#endif /* _TZ_BOOT_PARAM_H_ */
