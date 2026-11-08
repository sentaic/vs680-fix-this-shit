/*
 * Copyright (c) 2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <platform_def.h>
#include <lib/psci/psci.h>

extern const unsigned char berlin_power_domain_tree_desc[];

/*******************************************************************************
 * This function returns the Berlin default topology tree information.
 ******************************************************************************/
const unsigned char *plat_get_power_domain_tree_desc(void)
{
	return berlin_power_domain_tree_desc;
}

/*******************************************************************************
 * This function implements a part of the critical interface between the psci
 * generic layer and the platform that allows the former to query the platform
 * to convert an MPIDR to a unique linear index. An error code (-1) is returned
 * in case the MPIDR is invalid.
 ******************************************************************************/
int plat_core_pos_by_mpidr(u_register_t mpidr)
{
	unsigned int cluster_id, cpu_id;
	uint64_t valid_mask;

#if ARM_PLAT_MT
	valid_mask = ~(MPIDR_AFFLVL_MASK |
			(MPIDR_AFFLVL_MASK << MPIDR_AFF1_SHIFT) |
			(MPIDR_AFFLVL_MASK << MPIDR_AFF2_SHIFT));
	cluster_id = (mpidr >> MPIDR_AFF2_SHIFT) & MPIDR_AFFLVL_MASK;
	cpu_id = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
#else
	valid_mask = ~(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK);
	cluster_id = (unsigned int) ((mpidr >> MPIDR_AFF1_SHIFT) &
						MPIDR_AFFLVL_MASK);
	cpu_id = (unsigned int) ((mpidr >> MPIDR_AFF0_SHIFT) &
						MPIDR_AFFLVL_MASK);
#endif

	mpidr &= MPIDR_AFFINITY_MASK;

	if ((mpidr & valid_mask) != 0U)
		return -1;

	if (cluster_id >= BERLIN_CLUSTER_COUNT)
		return -1;

	/*
	 * Validate cpu_id by checking whether it represents a CPU in
	 * one of the two clusters present on the platform.
	 */
	if (cpu_id >= BERLIN_MAX_CPUS_PER_CLUSTER)
		return -1;

	return (cpu_id + (cluster_id * BERLIN_MAX_CPUS_PER_CLUSTER));
}
