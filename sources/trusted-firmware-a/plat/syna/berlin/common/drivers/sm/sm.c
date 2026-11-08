/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <string.h>
#include <assert.h>
#include <lib/mmio.h>
#include "sm.h"

#ifndef RA_smSysCtl_SM_BOOT_STATUS
#define RA_smSysCtl_SM_BOOT_STATUS	0x34
#endif

#define SM_MSG_SIZE                     32
#define SM_MSGQ_TOTAL_SIZE              512
#define SM_MSGQ_HEADER_SIZE             SM_MSG_SIZE
#define SM_MSGQ_SIZE                    (SM_MSGQ_TOTAL_SIZE - SM_MSGQ_HEADER_SIZE )
#define MV_SM_ID_POWER                  5
#define MV_SM_POWER_SUSPEND_REQUEST     2
#define MV_SM_ACTIVE_2_SUSPEND_RESP     0xA0
#define MV_SM_SUSPEND_2_ACTIVE_RESP     6
#define MV_SM_POWER_STANDBY_REQUEST     1
#define MV_SM_ACTIVE_2_NORMALSTANDBY_RESP	4
#define MV_SM_POWER_SYS_RESET           0xFF
#define SM_MSG_BODY_SIZE        	(SM_MSG_SIZE - sizeof(short) * 2)

#define MV_SOC_STATE_ACTIVE_2_SUSPEND	0xA0
#define MV_SOC_STATE_SUSPEND_REQUEST	0xA1


#define MV_SM_POWER_SUSPEND_TYPE_WFI	0x0F1
#define MV_SM_POWER_SUSPEND_TYPE_PWROFF	0x0FF

#define SM_Q_PUSH( pSM_Q ) {                                    	\
	                pSM_Q->m_iWrite += SM_MSG_SIZE;                 \
	                if( pSM_Q->m_iWrite >= SM_MSGQ_SIZE )           \
	                        pSM_Q->m_iWrite -= SM_MSGQ_SIZE;        \
	                pSM_Q->m_iWriteTotal += SM_MSG_SIZE; }

#define SM_Q_POP( pSM_Q ) {                                     	\
	                pSM_Q->m_iRead += SM_MSG_SIZE;                  \
	                if( pSM_Q->m_iRead >= SM_MSGQ_SIZE )            \
	                        pSM_Q->m_iRead -= SM_MSGQ_SIZE;         \
	                pSM_Q->m_iReadTotal += SM_MSG_SIZE; }

typedef struct {

	short   m_iModuleID;
	short   m_iMsgLen;
	char    m_pucMsgBody[SM_MSG_BODY_SIZE];
} MV_SM_Message;

typedef struct {
	int     m_iWrite;
	int     m_iRead;
	int     m_iWriteTotal;
	int     m_iReadTotal;
	char    m_Padding[SM_MSGQ_HEADER_SIZE - sizeof(int) * 4];
	char    m_Queue[SM_MSGQ_SIZE];
} MV_SM_MsgQ;

static sm_config_t sm_config = {
	.queue_base = (void*)(0x04004000-512*3 - 0x04000000 + 0xF7FA0000),
	.sm_ctrl_base = (void*)0xF7FE2000,
};

void sm_set_config(sm_config_t *config)
{
	assert(config != NULL);

	memcpy(&sm_config, config, sizeof(sm_config_t));

	assert(sm_config.queue_base != NULL);
}

static int sm_send_msg(MV_SM_Message *m)
{
	int ret = 0;
	MV_SM_Message *p;
	MV_SM_MsgQ *sm_queue;

	sm_queue = (MV_SM_MsgQ*)sm_config.queue_base;
	p = (MV_SM_Message*)(&(sm_queue->m_Queue[sm_queue->m_iWrite]));
	memcpy(p, m, sizeof(*p));
	SM_Q_PUSH(sm_queue);
	dsb();

	return ret;
}

static inline int sm_get_boot_status(void)
{
	return mmio_read_32((unsigned long)sm_config.sm_ctrl_base +
						RA_smSysCtl_SM_BOOT_STATUS);
}

static inline void sm_wait_for_complete(unsigned int status)
{
	while (sm_get_boot_status() != status);
}

void sm_do_system_warmdown(unsigned int power_state)
{
	MV_SM_Message m;
	unsigned int *msg = (unsigned int*)m.m_pucMsgBody;

	m.m_iModuleID = MV_SM_ID_POWER;
	m.m_iMsgLen = 12;
	*msg = MV_SM_POWER_SUSPEND_REQUEST;

	if (power_state) {
		msg[2] = MV_SM_POWER_SUSPEND_TYPE_WFI;
	} else {
		msg[2] = MV_SM_POWER_SUSPEND_TYPE_PWROFF;
	}
	sm_send_msg(&m);
	sm_wait_for_complete(MV_SOC_STATE_SUSPEND_REQUEST);

	m.m_iModuleID = MV_SM_ID_POWER;
	m.m_iMsgLen = 4;
	*msg = MV_SM_ACTIVE_2_SUSPEND_RESP;
	sm_send_msg(&m);

	sm_wait_for_complete(MV_SOC_STATE_ACTIVE_2_SUSPEND);
}

void sm_do_system_warmup(unsigned int power_state)
{
	MV_SM_Message m;
	void *msg = m.m_pucMsgBody;

	m.m_iModuleID = MV_SM_ID_POWER;
	m.m_iMsgLen = 4;
	*(unsigned int*)msg = MV_SM_SUSPEND_2_ACTIVE_RESP;
	sm_send_msg(&m);
}

void sm_do_system_reset(void)
{
	MV_SM_Message m;
	void *msg = m.m_pucMsgBody;

	m.m_iModuleID = MV_SM_ID_POWER;
	m.m_iMsgLen = 4;
	*(unsigned int*)msg = MV_SM_POWER_SYS_RESET;
	sm_send_msg(&m);
}

void sm_do_system_off(void)
{
	MV_SM_Message m;
	void *msg = m.m_pucMsgBody;

	m.m_iModuleID = MV_SM_ID_POWER;
	m.m_iMsgLen = 4;
	*(unsigned int *)msg = MV_SM_POWER_STANDBY_REQUEST;
	sm_send_msg(&m);

	m.m_iModuleID = MV_SM_ID_POWER;
	m.m_iMsgLen = 4;
	*(unsigned int *)msg = MV_SM_ACTIVE_2_NORMALSTANDBY_RESP;
	sm_send_msg(&m);
}
