/* SPDX-License-Identifier: MIT */
#include "vs680_phy_range.h"
#include <limits.h>
#include <string.h>

const struct vs680_phy_profile vs680_phy316_high8_candidate = {
    "phy316-high8-experimental-475200000-594000000",
    "Synaptics tee-optee_dev e397232786bfee0599dc9d51eb3ca2d6db008eda "
    "phy_316.c:294-299; equal discrete rows only, continuous interval unvalidated",
    VS680_EXPERIMENTAL_RANGE,
    475200000, 594000000, 1, 1, 8, 0,
    (1u << VS680_RGB) | (1u << VS680_YUV444),
    0x0640, 0x3080, 0x0005
};

/* Exact ratio-to-integer comparison without multiplying the bound. */
static int compare(struct vs680_ratio r, uint64_t bound)
{
    uint64_t whole = r.numerator / r.denominator;
    if (whole < bound) return -1;
    if (whole > bound) return 1;
    return r.numerator % r.denominator ? 1 : 0;
}

int vs680_phy_select(const struct vs680_phy_request *q,
                     const struct vs680_phy_limits *l,
                     const struct vs680_phy_profile *p, size_t count,
                     unsigned int allow_experimental,
                     struct vs680_phy_selection *out)
{
    struct vs680_ratio ref, tmds;
    const struct vs680_phy_profile *match = NULL;
    unsigned int depth, multiplier, denied = 0;
    size_t i;
    if (out) memset(out, 0, sizeof(*out));
    if (!q || !l || !p || !count || !out || !q->pixel_clock_hz ||
        q->encoding < VS680_RGB || q->encoding > VS680_YUV420 ||
        (q->bpc != 8 && q->bpc != 10 && q->bpc != 12 && q->bpc != 16) ||
        (q->encoding == VS680_YUV422 && q->bpc == 16) ||
        q->repetition > 9 || !l->hw_pixel_clock_max_hz ||
        !l->hw_tmds_char_max_hz || !l->sink_tmds_char_max_hz)
        return VS680_SELECT_INVALID;
    if (q->pixel_clock_hz > l->hw_pixel_clock_max_hz)
        return VS680_SELECT_CLOCK_LIMIT;
    depth = q->encoding == VS680_YUV422 ? 8 : q->bpc;
    multiplier = (q->repetition + 1) * depth;
    if (q->pixel_clock_hz > UINT64_MAX / multiplier)
        return VS680_SELECT_INVALID;
    ref.numerator = q->pixel_clock_hz;
    ref.denominator = q->encoding == VS680_YUV420 ? 2 : 1;
    tmds.numerator = q->pixel_clock_hz * multiplier;
    tmds.denominator = ref.denominator * 8;
    if (compare(tmds, l->hw_tmds_char_max_hz) > 0 ||
        compare(tmds, l->sink_tmds_char_max_hz) > 0)
        return VS680_SELECT_CLOCK_LIMIT;
    for (i = 0; i < count; ++i) {
        int lo, hi;
        if (!p[i].id || !p[i].evidence || !p[i].phy_ref_min_hz ||
            p[i].phy_ref_min_hz > p[i].phy_ref_max_hz ||
            p[i].status < VS680_DOCUMENTED_RANGE ||
            p[i].status > VS680_EXPERIMENTAL_RANGE)
            return VS680_SELECT_INVALID;
        if (p[i].effective_bpc != depth || p[i].repetition != q->repetition ||
            !(p[i].encoding_mask & (1u << q->encoding))) continue;
        lo = compare(ref, p[i].phy_ref_min_hz);
        hi = compare(ref, p[i].phy_ref_max_hz);
        if (lo < 0 || (lo == 0 && !p[i].min_inclusive) ||
            hi > 0 || (hi == 0 && !p[i].max_inclusive)) continue;
        if (p[i].status == VS680_EXPERIMENTAL_RANGE && !allow_experimental) {
            denied = 1;
            continue;
        }
        /* 检查频率区间是否重叠。 */
        if (match) return VS680_SELECT_AMBIGUOUS;
        match = &p[i];
    }
    if (!match) return denied ? VS680_SELECT_EXPERIMENT_DISABLED : VS680_SELECT_NO_PROFILE;
    out->profile = match;
    out->phy_ref_hz = ref;
    out->tmds_char_hz = tmds;
    out->effective_bpc = depth;
    out->high_tmds_ratio_required = compare(tmds, 340000000) > 0;
    return VS680_SELECT_OK;
}
