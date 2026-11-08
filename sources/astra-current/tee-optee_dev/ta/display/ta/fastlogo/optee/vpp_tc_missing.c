/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2025 Synaptics Incorporated */

#include "vpp_tc_missing.h"
#include <stdint.h>
#include <float.h>

#if DBL_MANT_DIG != 53 || DBL_MAX_EXP != 1024
#error "Display math requires IEEE 754 binary64"
#endif

int vprintf(const char *format, va_list arg)
{
    return 0;
}

double fabs(double a)
{
    union { double value; uint64_t bits; } number = { .value = a };

    number.bits &= UINT64_C(0x7fffffffffffffff);
    return number.value;
}

float sin(float a)
{
#if CONFIG_VPP_ENABLE_HUE_BYPASS == 1
    a = 0.0;
#endif
    return a;
}

float cos(float a)
{
#if CONFIG_VPP_ENABLE_HUE_BYPASS == 1
    a = 1.0;
#endif
    return a;
}

double ceil(double a)
{
    union { double value; uint64_t bits; } number = { .value = a };
    uint64_t fraction_mask;
    int exponent = (int)((number.bits >> 52) & 0x7ff) - 1023;

    /* 整数、无穷和 NaN 原样返回。 */
    if (exponent >= 52)
        return a;
    if (exponent < 0) {
        if ((number.bits & UINT64_C(0x7fffffffffffffff)) == 0)
            return a;
        if (number.bits >> 63) {
            number.bits = UINT64_C(0x8000000000000000);
            return number.value;
        }
        return 1.0;
    }

    fraction_mask = (UINT64_C(1) << (52 - exponent)) - 1;
    if ((number.bits & fraction_mask) == 0)
        return a;
    if (!(number.bits >> 63))
        number.bits += fraction_mask;
    number.bits &= ~fraction_mask;
    return number.value;
}
