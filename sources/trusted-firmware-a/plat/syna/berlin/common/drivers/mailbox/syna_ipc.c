/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <lib/mmio.h>
#include "klamath_memmap.h"
#include "ipc.h"

#define IPC_REG_BASE        START_IPC_REG
#define IPC_ME_BASE         (IPC_REG_BASE + RA_IPC_IPC_CSR_P0)

#define IPC_BASE(id)        (((id)==0)?(IPC_REG_BASE + RA_IPC_IPC_CSR_P0):(IPC_REG_BASE + RA_IPC_IPC_CSR_P1))

#define IPC_MBX0            (RA_IPC_CSR_IPC_MBX0)
#define IPC_MBX1            (RA_IPC_CSR_IPC_MBX1)
#define IPC_MBX2            (RA_IPC_CSR_IPC_MBX2)
#define IPC_MBX3            (RA_IPC_CSR_IPC_MBX3)

#define MAILBOX_OFFSET(mailbox_id)	(IPC_MBX0 + (mailbox_id)*(IPC_MBX1-IPC_MBX0))

#define COMM_PORT 1
static void soft_udelay(unsigned int us)
{
	volatile unsigned int i = 0;

	for (i = 0; i < (us * 1000); i++);
}

int ipc_lock_set(int id)
{
	mmio_write_32(IPC_BASE(COMM_PORT) + RA_IPC_CSR_IPC_LOCK_SET, 1 << id);
	return 0;
}

int ipc_lock_clear(int id)
{
	mmio_write_32(IPC_BASE(COMM_PORT) + RA_IPC_CSR_IPC_LOCK_CLR, 1 << id);
	return 0;
}

int ipc_lock_status(int id)
{
	unsigned int val = 0;
	unsigned int addr = IPC_BASE(COMM_PORT) + RA_IPC_CSR_IPC_LOCK_STATUS;

	val = mmio_read_32(addr);
	return (val & (1 << id)) ? 1 : 0;
}


int ipc_mailbox_set_msg(unsigned int msg, int mailbox_id)
{
    unsigned int ipc_base = IPC_BASE(COMM_PORT);

    mmio_write_32(ipc_base + MAILBOX_OFFSET(mailbox_id), msg);

    return 0;
}

int ipc_mailbox_wait_msg_done(int mailbox_id, int timeout_us)
{
    unsigned int ipc_base = IPC_BASE(COMM_PORT);

    while(timeout_us >= 0)
    {
        unsigned int msg;
        msg = mmio_read_32(ipc_base + MAILBOX_OFFSET(mailbox_id));
        if (msg == 0)
        {
			INFO("IPC command done\n");
            return 0;
        }
        soft_udelay(10);
        timeout_us -= 1;
    }
	INFO("IPC timedout: base @0x%x, msg = 0x%x, irq status = 0x%x, MSK_INT = 0x%x, INT_MSK = 0x%x.\n",
		ipc_base, mmio_read_32(ipc_base + MAILBOX_OFFSET(mailbox_id)),
		mmio_read_32(ipc_base + RA_IPC_CSR_IPC_INT_STATUS),
		mmio_read_32(ipc_base + RA_IPC_CSR_IPC_MASK_INT),
		mmio_read_32(ipc_base + RA_IPC_CSR_IPC_INT_MASK));
    return -1;
}

int ipc_mailbox_get_msg(int mailbox_id, unsigned int* p_msg)
{
    unsigned int ipc_base = IPC_BASE(COMM_PORT);

    if (p_msg)
    {
        *p_msg = mmio_read_32(ipc_base + MAILBOX_OFFSET(mailbox_id));
    }
    mmio_write_32(ipc_base + MAILBOX_OFFSET(mailbox_id), 0);

    return 0;
}
