/*
 * Copyright (c) 2026, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef PERIF_RESET_ID_H
#define PERIF_RESET_ID_H

#define PCIE_RESET 1

enum syna_perif_id {
	USB3PHYRST_ID,
	PCIE0RST_ID,
	USB0PHYRST_ID,
	USB0CORERST_ID,
	USB0MAHBRST_ID,
	PCIEPHYRST_ID,
	ISPGRST_ID,
	MAX_PERIF_ID
};

#endif /* PERIF_RESET_ID_H */

