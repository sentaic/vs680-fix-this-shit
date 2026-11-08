/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_mailbox.h - BCM XBAR Component related structure definition
//
//*****************************************************************************
#ifndef __BCM_DRV_MAILBOX_H__
#define __BCM_DRV_MAILBOX_H__

#include "bcm_reg.h"
#include "bcm_struct.h"
#include "bcm_drv_com.h"
#include "bcm_external.h"
// the command mailbox, which appears in the Client's memory space
typedef struct _MAILBOX
{
   uint32_t primitive_command_parameter0;                  // 0x0 Write Only
   uint32_t primitive_command_parameter1;                  // 0x4 Write Only
   uint32_t primitive_command_parameter2;                  // 0x8 Write Only
   uint32_t primitive_command_parameter3;                  // 0xc Write Only
   uint32_t primitive_command_parameter4;                  // 0x10 Write Only
   uint32_t primitive_command_parameter5;                  // 0x14 Write Only
   uint32_t primitive_command_parameter6;                  // 0x18 Write Only
   uint32_t primitive_command_parameter7;                  // 0x1c Write Only
   uint32_t primitive_command_parameter8;                  // 0x20 Write Only
   uint32_t primitive_command_parameter9;                  // 0x24 Write Only
   uint32_t primitive_command_parameter10;                 // 0x28 Write Only
   uint32_t primitive_command_parameter11;                 // 0x2c Write Only
   uint32_t primitive_command_parameter12;                 // 0x30 Write Only
   uint32_t primitive_command_parameter13;                 // 0x34 Write Only
   uint32_t primitive_command_parameter14;                 // 0x38 Write Only: linked-list dma host-read pointer
   uint32_t primitive_command_parameter15;                 // 0x3c Write Only: linked-list dma host-write pointer
   uint32_t secure_processor_command;                      // 0x40 Write Only
   uint8_t  reserved_0x44[60];
   uint32_t command_return_status;                         // 0x80 Read Only
   uint32_t command_status_0;                              // 0x84 Read Only
   uint32_t command_status_1;                              // 0x88 Read Only
   uint32_t command_status_2;                              // 0x8c Read Only
   uint32_t command_status_3;                              // 0x90 Read Only
   uint32_t command_status_4;                              // 0x94 Read Only
   uint32_t command_status_5;                              // 0x98 Read Only
   uint32_t command_status_6;                              // 0x9c Read Only
   uint32_t command_status_7;                              // 0xa0 Read Only
   uint32_t command_status_8;                              // 0xa4 Read Only
   uint32_t command_status_9;                              // 0xa8 Read Only
   uint32_t command_status_10;                             // 0xac Read Only
   uint32_t command_status_11;                             // 0xb0 Read Only
   uint32_t command_status_12;                             // 0xb4 Read Only
   uint32_t command_status_13;                             // 0xb8 Read Only
   uint32_t command_status_14;                             // 0xbc Read Only
   uint32_t command_status_15;                             // 0xc0 Read Only
   uint32_t command_fifo_status;                           // 0xc4 Read Only
   uint32_t host_interrupt_register;                       // 0xc8 Write-to-Clear
   uint32_t host_interrupt_mask;                           // 0xcc Write-to-Clear
   uint32_t host_exception_address;                        // 0xd0 Write-to-Clear
   uint32_t sp_trust_register;                             // 0xd4 Read Only
   uint32_t wtm_identification;                            // 0xd8 Read Only
   uint32_t wtm_revision;                                  // 0xdc Read Only
} MAILBOX;

#define MAILBOX_SZ_IN_BYTE       (sizeof(MAILBOX))
#define MAILBOX_SZ_IN_WORD       (MAILBOX_SZ_IN_BYTE/sizeof(uint32_t))

typedef enum _tagCmdStatusType
{
    MB_CMD_INIT_DONE  = 0,
    MB_CMD_READY      = 1,
    MB_CMD_PRE_PROC   = 2,
    MB_CMD_PROC       = 3,
    MB_CMD_POST_PROC  = 4,
    MB_CMD_LAST
}MAILBOX_STATUS;


typedef struct _tagCmdProcessorCtx
{
    uint32_t mb_status;
    uint32_t processed_primitive;
    uint32_t last_primitive;
    uint32_t last_primitive_status;

    MAILBOX mb;

}CMD_PROC_CTX;


typedef STATUS (*DISPATCH_FUNC)(uint32_t, ...);       // the handler, with variable arguments

typedef struct _PRIM_DISPATCH
{
   DISPATCH_FUNC func;
   uint8_t  num_args;                  // the number of arguments
   uint8_t  caps;                      // capabilities flags (see above)
   U16 cew;
   uint32_t cmd_id;
} PRIM_DISPATCH;



#endif //#ifndef __BCM_DRV_MAILBOX_H__
