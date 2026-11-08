// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include "mem_region.h"
#include "macros.h"
#include "szc.h"

/* A dummy sets of security settings */

/* memory region setting */
const struct mem_region mem_region_param[0];
const int mem_region_num = ARRAY_SIZE(mem_region_param);

/* memory zone setting */
const struct mem_zone mem_zone_param[0];
const int mem_zone_num = ARRAY_SIZE(mem_zone_param);

/* register firewall setting */
const struct rf_window rf_window_param[0];
const int rf_window_num = ARRAY_SIZE(rf_window_param);

/* master setting */
const struct master_setting master_setting_param[0];
const int master_setting_num = ARRAY_SIZE(master_setting_param);

/* register region settings */
const struct reg_region reg_region_param[0];
const int reg_region_param_num = ARRAY_SIZE(reg_region_param);

/* szc rules setting */
const uint32_t  szc_rule_param[0];
const int szc_rule_num = ARRAY_SIZE(szc_rule_param);
