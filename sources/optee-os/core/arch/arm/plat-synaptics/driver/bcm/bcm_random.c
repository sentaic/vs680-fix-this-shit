// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <stdint.h>
#include <string.h>
#include <util.h>
#include "bcmREG.h"
#include "bcm_mailbox.h"
#include "bcm_primitive.h"
#include "bcm_random.h"

#define BCM_MAILBOX				MEMMAP_BCM_REG_BASE
#define BCM_STATUS_BOOTSTRAP_IN_PROGRESS	(1<<9)
#define BCM_STATUS_BCM_READY			(1<<8)

int bcm_get_random_without_wait(uint32_t *random, uint32_t random_len)
{
	uint32_t status;
	volatile NOT_MAILBOX *mb = (NOT_MAILBOX *) BCM_MAILBOX;

	status = mb->command_fifo_status;
	if (0 == (status & BCM_STATUS_BOOTSTRAP_IN_PROGRESS)) {
		/* wait for bcm ready */
		while (0 == (status & BCM_STATUS_BCM_READY)) {
			if (status != mb->command_fifo_status)
				status = mb->command_fifo_status;
		}
	}

	mb->primitive_command_parameter0 =
		(unsigned int)((uintptr_t)random);
	mb->primitive_command_parameter1 =
			(unsigned int)(random_len);
	mb->secure_processor_command = DRM_GET_RANDOM;

	/* wait for "command complete" */
	while ( !(mb->host_interrupt_register & 0x1) );

	/* Do a full reset to BCM HOST interrupt */
	mb->host_interrupt_register = 0x70001;
	status = mb->command_return_status;

	return status;
}

