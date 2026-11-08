/* SPDX-License-Identifier: MIT */
#ifndef VS680_PHY_RANGE_H
#define VS680_PHY_RANGE_H
#include <stddef.h>
#include <stdint.h>

/* 根据输入时钟和位深选择 PHY 参数。 */
enum vs680_encoding { VS680_RGB, VS680_YUV444, VS680_YUV422, VS680_YUV420 };
enum vs680_profile_evidence {
    VS680_DOCUMENTED_RANGE, /* Same PHY/revision documentation establishes bounds. */
    VS680_VALIDATED_RANGE,  /* Board validation establishes bounds; record conditions. */
    VS680_EXPERIMENTAL_RANGE
};
enum vs680_select_status {
    VS680_SELECT_OK = 0,
    VS680_SELECT_INVALID = -1,
    VS680_SELECT_CLOCK_LIMIT = -2,
    VS680_SELECT_NO_PROFILE = -3, /* 缺少所需配置。 */
    VS680_SELECT_EXPERIMENT_DISABLED = -4,
    VS680_SELECT_AMBIGUOUS = -5
};
struct vs680_phy_request {
    uint64_t pixel_clock_hz; /* 目标或生成的时钟频率。 */
    enum vs680_encoding encoding;
    unsigned int bpc;       /* 显式位深：8、10、12 或 16。 */
    unsigned int repetition; /* Vendor convention: multiplier is repetition + 1. */
};
struct vs680_phy_limits {
    uint64_t hw_pixel_clock_max_hz;
    uint64_t hw_tmds_char_max_hz;
    uint64_t sink_tmds_char_max_hz;
};
struct vs680_ratio { uint64_t numerator; unsigned int denominator; };
struct vs680_phy_profile {
    const char *id;
    const char *evidence; /* 配置来源的版本或验证记录。 */
    enum vs680_profile_evidence status;
    uint64_t phy_ref_min_hz, phy_ref_max_hz;
    unsigned int min_inclusive, max_inclusive;
    unsigned int effective_bpc; /* YUV422 uses 8 for this PHY's lookup. */
    unsigned int repetition;
    unsigned int encoding_mask; /* 1u << enum vs680_encoding */
    uint16_t opmode_pllcfg, pllcurrctrl, plldivctrl;
};
struct vs680_phy_selection {
    const struct vs680_phy_profile *profile;
    struct vs680_ratio phy_ref_hz, tmds_char_hz;
    unsigned int effective_bpc;
    unsigned int high_tmds_ratio_required; /* 超过 340 MHz 时需要 SCDC。 */
};
int vs680_phy_select(const struct vs680_phy_request *request,
                     const struct vs680_phy_limits *limits,
                     const struct vs680_phy_profile *profiles, size_t count,
                     unsigned int allow_experimental,
                     struct vs680_phy_selection *selection);

/* 高频段实验参数取自 475.2、495、505.25、552.75 和 594 MHz 的公开条目。 */
extern const struct vs680_phy_profile vs680_phy316_high8_candidate;
#endif
