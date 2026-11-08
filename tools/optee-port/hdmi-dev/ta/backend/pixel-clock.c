/* SPDX-License-Identifier: GPL-2.0+ */
#include "pixel-clock.h"
#include "Galois_memmap.h"
#include "avio_memmap.h"
#include "avioGbl.h"
#define PLL_BASE (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_GBL_BASE + \
                  RA_avioGbl_VPLL_WRAP0 + RA_VPLL_WRAP_VPLL)
#define POWER_CONTROL (MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_GBL_BASE + \
                RA_avioGbl_AVPLL_CTRL0)

static TEE_Result update(const struct hdmi_mode_io *io, uint32_t offset,
                         uint32_t mask, uint32_t value)
{
    uint32_t previous;
    TEE_Result result = io->read32(io->opaque, PLL_BASE + offset, &previous);
    if (result) return result;
    return io->write32(io->opaque, PLL_BASE + offset,
                       (previous & ~mask) | (value & mask));
}

TEE_Result hdmi_pixel_clock_configure(const struct hdmi_mode_io *io, uint32_t rate)
{
    const uint64_t parent = UINT64_C(25000000);
    const uint32_t dm = 5;
    uint32_t power, status = 0, dn, fraction, divider_half;
    uint32_t rdm, rdn, rfrac, rdp;
    uint64_t vco, fixed, actual;
    TEE_Result result;
    if (!io || !io->read32 || !io->write32 || !io->wait_ms)
        return TEE_ERROR_BAD_PARAMETERS;
    /* 当前模式合同为720p60；其他频率在扩展模式时分别接入。 */
    if (rate != UINT32_C(74250000)) return TEE_ERROR_NOT_SUPPORTED;
    divider_half = UINT64_C(6000000000) / ((uint64_t)rate * 2u);
    if (divider_half > 32u) divider_half = 32u;
    if (!divider_half) return TEE_ERROR_BAD_PARAMETERS;
    vco = (uint64_t)rate * divider_half * 2u;
    if (vco < UINT64_C(1200000000)) return TEE_ERROR_BAD_PARAMETERS;
    fixed = (vco << 24) * dm / (parent * 4u);
    dn = fixed >> 24;
    fraction = fixed & UINT32_C(0xffffff);
    if (!dn || dn > 512u) return TEE_ERROR_BAD_PARAMETERS;
    result = io->read32(io->opaque, POWER_CONTROL, &power);
    if (result) return result;
    result = io->write32(io->opaque, POWER_CONTROL, power | 1u);
    if (result) return result;
    /* VS680 linux_5_4 的CTRLA/CTRLB布局：RESET bit0、BYPASS bit1、SSE bit9。 */
    result = update(io, 0, 3u, 3u);
    if (!result) result = update(io, 4, 1u << 9, 0);
    if (!result) result = update(io, 8, 0x1ffu, dm - 1u);
    if (!result) result = update(io, 12, 0x1ffu, dn - 1u);
    if (!result) result = update(io, 16, 0xffffffu, fraction);
    if (!result) result = update(io, 0, 7u << 3, 0);
    if (!result) result = update(io, 20, 0x1fu, divider_half - 1u);
    if (!result) result = update(io, 0, 1u, 0);
    if (!result) result = update(io, 0, 2u, 0);
    if (!result) result = io->write32(io->opaque, POWER_CONTROL, power & ~1u);
    if (result) return result;
    /* 与vs680_pll_update_setting一致：依次解除reset、内部bypass和PD，再查询LOCK。 */
    for (unsigned attempt = 0; attempt < 100u; ++attempt) {
        result = io->read32(io->opaque, PLL_BASE + 28u, &status);
        if (result) return result;
        if (status & 1u) goto verify;
        result = io->wait_ms(io->opaque, 1);
        if (result) return result;
    }
    return TEE_ERROR_BUSY;
verify:
    result = io->read32(io->opaque, PLL_BASE + 8u, &rdm);
    if (!result) result = io->read32(io->opaque, PLL_BASE + 12u, &rdn);
    if (!result) result = io->read32(io->opaque, PLL_BASE + 16u, &rfrac);
    if (!result) result = io->read32(io->opaque, PLL_BASE + 20u, &rdp);
    if (result) return result;
    rdm = (rdm & 0x1ffu) + 1u;
    rdn = (rdn & 0x1ffu) + 1u;
    rfrac &= 0xffffffu;
    rdp = ((rdp & 0x1fu) + 1u) * 2u;
    actual = parent * ((uint64_t)rdn * (1u << 24) + rfrac) * 4u /
             rdm / rdp / (1u << 24);
    if (actual > (uint64_t)rate + 100u || actual + 100u < rate)
        return TEE_ERROR_BAD_STATE;
    return TEE_SUCCESS;
}
