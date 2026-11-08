// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "SZC.h"
#include "config.h"
#include "io.h"
#include "string.h"
#include "mz.h"
#include "mem_region_userdata.h"
#include "mem_region.h"
#include "mr_setup.h"
#include "tz_boot_param_value.h"
#include "tz_errcode.h"
#include <assert.h>
#include <trace.h>
#include <mm/core_memprot.h>
#include <initcall.h>
#include <kernel/pm.h>

#define SZC_ZONE_NUM_ZONES_MAX      16
#define SZC_ZONE_NUM_MASTERS_MAX    32

#define SZC_ZONE_BASE               (SZC_BASE + RA_SZC_AllZone)
#define SZC_ZONE(z)                 (SZC_ZONE_BASE + (z) * 28)

struct mem_zone mzs[SZC_ZONE_NUM_ZONES_MAX];
int szc_zone_num;

#define readl io_read32
#define writel(v, a) do {               \
		io_write32(a, v);                    \
	} while(0)

#if defined(PLATFORM_FLAVOR_dolphin) || defined(PLATFORM_FLAVOR_platypus)
static void szc_set_zone_enable_per_master(unsigned int master_index, unsigned int enable)
{
	unsigned int val;
	vaddr_t addr = SZC_ZONE_BASE + RA_AllZone_Zone_enable;

	val = readl(addr);
	if (enable)
	{
		val |= 1 << master_index;
	}
	else
	{
		val &= ~(1 << master_index);
	}
	writel(val, addr);
}
#elif defined(PLATFORM_FLAVOR_myna2) || defined(PLATFORM_FLAVOR_klamath)

static void szc_set_zone_enable(unsigned int enable)
{
	vaddr_t addr = SZC_ZONE_BASE + RA_AllZone_Zone_enable;
	io_write32(addr, enable);
}
#endif

static void szc_set_zone_ctrl(unsigned int zone_index, unsigned int enable, unsigned int ns, unsigned int lock)
{
	vaddr_t addr = SZC_ZONE(zone_index) + RA_SecureZone_Zone_CTRL;

	writel(enable | (ns << 1) | (lock << 2), addr);
}

static int mz_set_zone(struct mem_zone* mz)
{
	int zone_id;
	uint32_t start_addr;
	uint32_t size;
	uint32_t ns;
	vaddr_t base;

	if (!mz)
		return -1;

	zone_id = mz->id;
	start_addr = mz->start;
	size = mz->size;
	ns = mz->ns;
	base = SZC_ZONE(zone_id);

	DMSG("zone=%d, start_addr=0x%08x, size=0x%08x, ns_rd = 0x%x, ns_wr = 0x%x, s_rd = 0x%x, s_wr = 0x%x\n",
			zone_id, start_addr, size, mz->ns_rd, mz->ns_wr,  mz->s_rd, mz->s_wr);
	assert(zone_id >= 0 && zone_id < SZC_ZONE_NUM_ZONES_MAX);
	assert((start_addr & 0x3) == 0);
	assert((size & 0x3) == 0);
	assert(start_addr + size > start_addr);
	assert(ns == 0 || ns == 1);

	//disable mzone
	szc_set_zone_ctrl(zone_id, 0, ns, 0);

	//config zone start and end
	writel(start_addr, (base + RA_SecureZone_REGION_SADDR));
	assert(start_addr == readl(base + RA_SecureZone_REGION_SADDR));

	writel(start_addr + size, (base + RA_SecureZone_REGION_EADDR));
	assert((start_addr + size) == readl(base + RA_SecureZone_REGION_EADDR));

	// S_Read_En
	writel(mz->s_rd, base + RA_SecureZone_Master_CTRL);
	assert(mz->s_rd == readl(base + RA_SecureZone_Master_CTRL));

	// S_Write_En
	writel(mz->s_wr, base + RA_SecureZone_Master_CTRL1);
	assert(mz->s_wr == readl(base + RA_SecureZone_Master_CTRL1));

	// NS_Read_En
	writel(mz->ns_rd, base + RA_SecureZone_Master_CTRL2);
	assert(mz->ns_rd == readl(base + RA_SecureZone_Master_CTRL2));

	// NS_Write_En
	writel(mz->ns_wr, base + RA_SecureZone_Master_CTRL3);
	assert(mz->ns_wr == readl(base + RA_SecureZone_Master_CTRL3));

	return 0;
}

static int mz_enable_zone(int zone_id, uint32_t ns, uint32_t lock)
{

	assert(zone_id >= 0 && zone_id < SZC_ZONE_NUM_ZONES_MAX);
	assert(ns == 0 || ns == 1);
	assert(lock == 0 || lock == 1);

	//enable zone
	szc_set_zone_ctrl(zone_id, 1, ns, lock);

	return 0;
}


static void secure_mem_zone_init(struct mem_zone *zone, int id, const char *name, uint32_t start, uint32_t size)
{
	int i;

	if (NULL == zone)
		return;

	assert(id >= 0 && id < SZC_ZONE_NUM_ZONES_MAX);
	assert((start & 0x3) == 0);
	assert((size & 0x3) == 0);
	assert(start + size > start);

	zone->id = id;
	zone->start =  start;
	zone->size =  size;
	strcpy(zone->name, name);
	for (i = 0; i < mem_zone_num; i++) {
		if (0 == strcmp(name, mem_zone_param[i].name)) {
			zone->s_rd = mem_zone_param[i].s_rd;
			zone->s_wr = mem_zone_param[i].s_wr;
			zone->ns_rd = mem_zone_param[i].ns_rd;
			zone->ns_wr = mem_zone_param[i].ns_wr;
			zone->ns = mem_zone_param[i].ns;
			break;
		}
	}

	if (i == mem_zone_num) {
		EMSG("There is no zone parameter for %s.\n", name);
		assert(0);
	}
}

static int generate_securezone_by_mr(void)
{
	const struct mem_region * mr;
	int mr_num;
	int i, zone_num = 0;
	uint32_t mr_start, mr_end = 0;

	mr_num = mr_get_region(&mr);
	assert(mr_num > 0 && mr != NULL);

	memset(mzs, 0, sizeof(mzs));

	mr_start = mr[0].base;
	mr_end = mr[0].base + mr[0].size;

	for (i = 0; i < mr_num; i++) {
		// Calculate the end of DDR region
		if(!TEE_MR_USER_IS_REGISTER(&mr[i]) && (mr_start < mr[i].base)) {
			mr_start = mr[i].base;
			mr_end = mr_start + mr[i].size;
		}
		if (TEE_MR_ACPU_ZONE(&mr[i])) {
			assert(zone_num < SZC_ZONE_NUM_ZONES_MAX);
			DMSG("region[%d]@0x%x, size = 0x%x.\n", i, mr[i].base, mr[i].size);
			secure_mem_zone_init(mzs + zone_num, zone_num, mr[i].name, mr[i].base, mr[i].size);
			zone_num++;
		}
	}
#if 0
	// Add the rest ddr region to rollback zone if max ddr region is less than 4G
	// The address needs to 4byte align so the max DDR address should be 0xfffffffc
	if (mr_end < 0xfffffffc) {
		secure_mem_zone_init(mzs + zone_num, zone_num, "rollback", mr_end,
							0xfffffffc - mr_end);
		zone_num++;
	}
#endif
	return zone_num;
}

static TEE_Result szc_configure_setup(void)
{
	int i = 0;

	szc_zone_num = generate_securezone_by_mr();
	assert(szc_zone_num >= 0 && szc_zone_num <= SZC_ZONE_NUM_ZONES_MAX);

	DMSG("zone num: %d\n", szc_zone_num);

	for (i = 0; i < szc_zone_num; i++) {
		DMSG("zone_id=%d, start = 0x%x, size = 0x%x, ns = 0x%x\n",
			mzs[i].id, mzs[i].start, mzs[i].size, mzs[i].ns);

		mz_set_zone(mzs + i);

		//enable zone and lock it
		mz_enable_zone(mzs[i].id, mzs[i].ns, 1);
	}

#if defined(PLATFORM_FLAVOR_dolphin) || defined(PLATFORM_FLAVOR_platypus)
	// enable SZC for each master
	for (i = 0; i < SZC_ZONE_NUM_MASTERS_MAX; i++)
		szc_set_zone_enable_per_master(i, 1);
#elif defined(PLATFORM_FLAVOR_myna2) || defined(PLATFORM_FLAVOR_klamath)
	// enable All SZC zone
	szc_set_zone_enable(0xffffffff);
#else
#error
#endif

	return TEE_SUCCESS;
}

static TEE_Result
pm_enter_resume(enum pm_op op, uint32_t pm_hint __unused,
		const struct pm_callback_handle *pm_handle __unused)
{
	DMSG("op = 0x%x.\n", op);
	if (op == PM_OP_RESUME)
		return szc_configure_setup();

	return TEE_SUCCESS;
}

static TEE_Result mz_setup(void)
{
	register_pm_driver_cb(pm_enter_resume, NULL, "syna_mz");

	szc_configure_setup();

	return TEE_SUCCESS;
}


int szc_get_zones(void *mz, size_t *size)
{
	uint32_t zone_sizes;

	zone_sizes = szc_zone_num * sizeof(struct mem_zone);

	if (*size < zone_sizes)
		return TZ_ERROR_SHORT_BUFFER;

	memcpy(mz, mzs, zone_sizes);

	*size =  zone_sizes;

	return TZ_SUCCESS;
}

int szc_get_zones_cnt(void)
{
	return szc_zone_num;
}

driver_init(mz_setup);

