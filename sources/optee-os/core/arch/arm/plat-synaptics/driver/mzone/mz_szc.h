// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _MZ_SZC_REG_H_
#define _MZ_SZC_REG_H_

#include "MC6Ctrl.h"
#include "Galois_memmap.h"

#define MZ_REGION_NUM		  (8)
#define MZ_REGION_REG_BASE      (MEMMAP_MCTRLSS_REG_BASE + RA_MC6Ctrl_Region0)
#define MZ_REGION_REG_SIZE      (0x10)

#define MZ_ACPU_ACCESS_ENABLE   (2)
#define MZ_ACPU_ACCESS_MASK     ((MZ_ACPU_ACCESS_ENABLE << LSb32Secure_Zone_REGION_MST_CFG_S_Write_En) |  \
                                 (MZ_ACPU_ACCESS_ENABLE << LSb32Secure_Zone_REGION_MST_CFG_S_Read_En) |   \
                                 (MZ_ACPU_ACCESS_ENABLE << LSb32Secure_Zone_REGION_MST_CFG_NS_Write_En) | \
                                 (MZ_ACPU_ACCESS_ENABLE << LSb32Secure_Zone_REGION_MST_CFG_NS_Read_En))

#endif /* _MC_TZC_REG_H_ */
