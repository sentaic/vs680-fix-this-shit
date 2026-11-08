// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2025, Synaptics Incorporated.
 */

#ifndef _PROT_ID_H_
#define _PROT_ID_H_

/* refer to h_protOR in soc.h */

enum prot_id {
	PROT_ID_CPU,
	PROT_ID_AIO,
	PROT_ID_VIO,
	PROT_ID_CIO,
	PROT_ID_GPU,
	PROT_ID_NPU,
	PROT_ID_MTEST,
	PROT_ID_GE0,
	PROT_ID_GE1,
	PROT_ID_EMMC,
	PROT_ID_SDIO0,
	PROT_ID_SDIO1,
	PROT_ID_USB0,
	PROT_ID_USB1,
	PROT_ID_PDMA,
	PROT_ID_MAX
};

#endif /* _PROT_ID_H_ */
