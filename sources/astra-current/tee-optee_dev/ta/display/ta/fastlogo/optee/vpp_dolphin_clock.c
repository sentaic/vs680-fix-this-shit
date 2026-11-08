// SPDX-License-Identifier: GPL-2.0+
/* VS680 PLL sequence and coefficient encoding from linux_5_4 vs680-pll.c. */
#include <stdint.h>
#include <tee_api_defines.h>
#include <trace.h>
#include <tee_internal_api.h>
#include "OSAL_api.h"
#include <galois_io.h>
#include <Galois_memmap.h>
#include <avio_memmap.h>
#include <avioGbl.h>
#include "vpp_dolphin_clock.h"
#undef TEE_Wait

#define PLL_BASE (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_GBL_BASE + RA_avioGbl_VPLL_WRAP0 + RA_VPLL_WRAP_VPLL)
#define PLL_BYPASS (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_GBL_BASE + RA_avioGbl_AVPLL_CTRL0)
static uint32_t read_register(uint32_t offset)
{
    uint32_t value;
    GA_REG_WORD32_READ(PLL_BASE + offset, &value);
    return value;
}
static void update_register(uint32_t offset, uint32_t mask, uint32_t value)
{
    uint32_t previous = read_register(offset);
    GA_REG_WORD32_WRITE(PLL_BASE + offset, (previous & ~mask) | value);
}

int vpp_dolphin_set_pixel_clock(unsigned int rate)
{
    const uint64_t parent = 25000000;
    uint32_t dm = 5;
    if (rate < 20000000 || rate > 3000000000u)
        return TEE_ERROR_BAD_PARAMETERS;
    uint32_t divider_half = 6000000000ull / ((uint64_t)rate * 2);
    if (divider_half > 32) divider_half = 32;
    if (!divider_half) return TEE_ERROR_BAD_PARAMETERS;
    uint64_t vco = (uint64_t)rate * divider_half * 2;
    if (vco < 1200000000ull) return TEE_ERROR_BAD_PARAMETERS;
    uint64_t fixed = (vco << 24) * dm / (parent * 4);
    uint32_t dn = fixed >> 24;
    uint32_t fraction = fixed & 0xffffff;
    if (!dn || dn > 512) return TEE_ERROR_BAD_PARAMETERS;
    uint32_t bypass;
    GA_REG_WORD32_READ(PLL_BYPASS, &bypass);
    GA_REG_WORD32_WRITE(PLL_BYPASS, bypass | 1u);
    update_register(0, 3, 3);              /* internal bypass and reset */
    update_register(4, 1u << 9, 0);        /* disable spread spectrum */
    update_register(8, 0x1ff, dm - 1);
    update_register(12, 0x1ff, dn - 1);
    update_register(16, 0xffffff, fraction);
    update_register(0, 7u << 3, 0);        /* 5MHz PFD range */
    update_register(20, 0x1f, divider_half - 1);
    update_register(0, 1, 0);
    update_register(0, 2, 0);
    GA_REG_WORD32_WRITE(PLL_BYPASS, bypass & ~1u);
    /* TEE_Wait has millisecond resolution; exceeds the vendor 120us wait. */
    TEE_Wait(1);
    uint32_t status = read_register(28);
    if (!(status & 1)) {
        GA_REG_WORD32_WRITE(PLL_BYPASS, bypass | 1u);
        EMSG("VPLL0 lock absent, status=%#x; output kept bypassed", status);
        return TEE_ERROR_BAD_STATE;
    }
    uint32_t rdm = (read_register(8) & 0x1ff) + 1;
    uint32_t rdn = (read_register(12) & 0x1ff) + 1;
    uint32_t rfrac = read_register(16) & 0xffffff;
    uint32_t rdp = ((read_register(20) & 0x1f) + 1) * 2;
    uint64_t actual = (parent * ((uint64_t)rdn * (1u << 24) + rfrac) * 4) / rdm / rdp / (1u << 24);
    return actual > rate + 100 || actual + 100 < rate ? TEE_ERROR_BAD_STATE : 0;
}
