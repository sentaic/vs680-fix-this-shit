/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <plat/common/platform.h>
#include <platform_def.h>
#include <berlin_private.h>

/*******************************************************************************
 * The Berlin power domain tree has a single system level power domain i.e. a
 * single root node. The first entry in the power domain descriptor specifies
 * the number of power domains at the highest power level.
 ******************************************************************************/
const unsigned char berlin_power_domain_tree_desc[] = {
	/* No of root nodes */
	1,
	/* No of clusters */
	BERLIN_CLUSTER_COUNT,
	/* No of CPU cores - cluster0 */
	BERLIN_MAX_CPUS_PER_CLUSTER
};
