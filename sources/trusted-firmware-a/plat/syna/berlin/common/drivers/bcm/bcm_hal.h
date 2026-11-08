/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BCM_HAL_H__
#define __BCM_HAL_H__

typedef struct _BCM_MAILBOX_COMMAND
{
   unsigned int arg0;
   unsigned int arg1;
   unsigned int arg2;
   unsigned int arg3;
   unsigned int arg4;
   unsigned int arg5;
   unsigned int arg6;
   unsigned int arg7;
   unsigned int arg8;
   unsigned int arg9;
   unsigned int arg10;
   unsigned int arg11;
   unsigned int arg12;
   unsigned int arg13;
   unsigned int arg14;
   unsigned int arg15;
   unsigned int command;
} BCM_MAILBOX_COMMAND;

typedef struct _BCM_MAILBOX_STATUS
{
   unsigned int return_status;
   unsigned int arg0;
   unsigned int arg1;
   unsigned int arg2;
   unsigned int arg3;
   unsigned int arg4;
   unsigned int arg5;
   unsigned int arg6;
   unsigned int arg7;
   unsigned int arg8;
   unsigned int arg9;
   unsigned int arg10;
   unsigned int arg11;
   unsigned int arg12;
   unsigned int arg13;
   unsigned int arg14;
   unsigned int arg15;
} BCM_MAILBOX_STATUS;

// the command mailbox structure, which appears in the Client's memory space
typedef struct _BCM_MAILBOX
{
   unsigned int command_parameter0;                  // 0x0 Write Only
   unsigned int command_parameter1;                  // 0x4 Write Only
   unsigned int command_parameter2;                  // 0x8 Write Only
   unsigned int command_parameter3;                  // 0xc Write Only
   unsigned int command_parameter4;                  // 0x10 Write Only
   unsigned int command_parameter5;                  // 0x14 Write Only
   unsigned int command_parameter6;                  // 0x18 Write Only
   unsigned int command_parameter7;                  // 0x1c Write Only
   unsigned int command_parameter8;                  // 0x20 Write Only
   unsigned int command_parameter9;                  // 0x24 Write Only
   unsigned int command_parameter10;                 // 0x28 Write Only
   unsigned int command_parameter11;                 // 0x2c Write Only
   unsigned int command_parameter12;                 // 0x30 Write Only
   unsigned int command_parameter13;                 // 0x34 Write Only
   unsigned int command_parameter14;                 // 0x38 Write Only: linked-list dma host-read pointer
   unsigned int command_parameter15;                 // 0x3c Write Only: linked-list dma host-write pointer
   unsigned int secure_processor_command;            // 0x40 Write Only
   unsigned int reserved_0x44[15];
   unsigned int command_return_status;               // 0x80 Read Only
   unsigned int command_status0;                     // 0x84 Read Only
   unsigned int command_status1;                     // 0x88 Read Only
   unsigned int command_status2;                     // 0x8c Read Only
   unsigned int command_status3;                     // 0x90 Read Only
   unsigned int command_status4;                     // 0x94 Read Only
   unsigned int command_status5;                     // 0x98 Read Only
   unsigned int command_status6;                     // 0x9c Read Only
   unsigned int command_status7;                     // 0xa0 Read Only
   unsigned int command_status8;                     // 0xa4 Read Only
   unsigned int command_status9;                     // 0xa8 Read Only
   unsigned int command_status10;                    // 0xac Read Only
   unsigned int command_status11;                    // 0xb0 Read Only
   unsigned int command_status12;                    // 0xb4 Read Only
   unsigned int command_status13;                    // 0xb8 Read Only
   unsigned int command_status14;                    // 0xbc Read Only
   unsigned int command_status15;                    // 0xc0 Read Only
   unsigned int command_fifo_status;                 // 0xc4 Read Only
   unsigned int host_interrupt_register;             // 0xc8 Write-to-Clear
   unsigned int host_interrupt_mask;                 // 0xcc Write-to-Clear
   unsigned int host_exception_address;              // 0xd0 Write-to-Clear
   unsigned int sp_trust_register;                   // 0xd4 Read Only
   unsigned int wtm_identification;                  // 0xd8 Read Only
   unsigned int wtm_revision;                        // 0xdc Read Only
} _BCM_MAILBOX;

int bcm_exec_func(const BCM_MAILBOX_COMMAND* cmd);
void bcm_clear_command(BCM_MAILBOX_COMMAND* cmd);
#endif
