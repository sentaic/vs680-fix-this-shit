// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _PROTOR_H_
#define _PROTOR_H_

#include "tz_perm.h"

#define PROT_ID_INVALID			(-1)

int prot_reset(int id);
int prot_set(int id, int perm);
void prot_dump(void);
void prot_buffer_registers(void);
int prot_check_changed(void);

#endif /* _PROTOR_H_ */
