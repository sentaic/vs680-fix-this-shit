// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef INCLUDE_BCM_MAILBOX_H
#define INCLUDE_BCM_MAILBOX_H

#include "Galois_memmap.h"
#include "global.h"

typedef struct
{
	uint32_t arg0;
	uint32_t arg1;
	uint32_t arg2;
	uint32_t arg3;
	uint32_t arg4;
	uint32_t arg5;
	uint32_t arg6;
	uint32_t arg7;
	uint32_t arg8;
	uint32_t arg9;
	uint32_t arg10;
	uint32_t arg11;
	uint32_t arg12;
	uint32_t arg13;
	uint32_t arg14;
	uint32_t arg15;
	uint32_t command;
} BCM_MAILBOX_COMMAND;

typedef struct
{
	uint32_t return_status;
	uint32_t arg0;
	uint32_t arg1;
	uint32_t arg2;
	uint32_t arg3;
	uint32_t arg4;
	uint32_t arg5;
	uint32_t arg6;
	uint32_t arg7;
	uint32_t arg8;
	uint32_t arg9;
	uint32_t arg10;
	uint32_t arg11;
	uint32_t arg12;
	uint32_t arg13;
	uint32_t arg14;
	uint32_t arg15;
} BCM_MAILBOX_STATUS;

// the command mailbox structure, which appears in the Client's memory space
typedef struct
{
	uint32_t command_parameter0;                  // 0x0 Write Only
	uint32_t command_parameter1;                  // 0x4 Write Only
	uint32_t command_parameter2;                  // 0x8 Write Only
	uint32_t command_parameter3;                  // 0xc Write Only
	uint32_t command_parameter4;                  // 0x10 Write Only
	uint32_t command_parameter5;                  // 0x14 Write Only
	uint32_t command_parameter6;                  // 0x18 Write Only
	uint32_t command_parameter7;                  // 0x1c Write Only
	uint32_t command_parameter8;                  // 0x20 Write Only
	uint32_t command_parameter9;                  // 0x24 Write Only
	uint32_t command_parameter10;                 // 0x28 Write Only
	uint32_t command_parameter11;                 // 0x2c Write Only
	uint32_t command_parameter12;                 // 0x30 Write Only
	uint32_t command_parameter13;                 // 0x34 Write Only
	uint32_t command_parameter14;                 // 0x38 Write Only: linked-list dma host-read pointer
	uint32_t command_parameter15;                 // 0x3c Write Only: linked-list dma host-write pointer
	uint32_t secure_processor_command;            // 0x40 Write Only
	uint32_t reserved_0x44[15];
	uint32_t command_return_status;               // 0x80 Read Only
	uint32_t command_status0;                     // 0x84 Read Only
	uint32_t command_status1;                     // 0x88 Read Only
	uint32_t command_status2;                     // 0x8c Read Only
	uint32_t command_status3;                     // 0x90 Read Only
	uint32_t command_status4;                     // 0x94 Read Only
	uint32_t command_status5;                     // 0x98 Read Only
	uint32_t command_status6;                     // 0x9c Read Only
	uint32_t command_status7;                     // 0xa0 Read Only
	uint32_t command_status8;                     // 0xa4 Read Only
	uint32_t command_status9;                     // 0xa8 Read Only
	uint32_t command_status10;                    // 0xac Read Only
	uint32_t command_status11;                    // 0xb0 Read Only
	uint32_t command_status12;                    // 0xb4 Read Only
	uint32_t command_status13;                    // 0xb8 Read Only
	uint32_t command_status14;                    // 0xbc Read Only
	uint32_t command_status15;                    // 0xc0 Read Only
	uint32_t command_fifo_status;                 // 0xc4 Read Only
	uint32_t host_interrupt_register;             // 0xc8 Write-to-Clear
	uint32_t host_interrupt_mask;                 // 0xcc Write-to-Clear
	uint32_t host_exception_address;              // 0xd0 Write-to-Clear
	uint32_t sp_trust_register;                   // 0xd4 Read Only
	uint32_t wtm_identification;                  // 0xd8 Read Only
	uint32_t wtm_revision;                        // 0xdc Read Only
} BCM_MAILBOX;


#define BCM_MAILBOX_BASE                    (MEMMAP_BCM_REG_BASE)
#define BCM_COMMAND_PARAMETER0              (BCM_MAILBOX_BASE+0x00)  //Write Only
#define BCM_COMMAND_PARAMETER1              (BCM_MAILBOX_BASE+0x04)  //Write Only
#define BCM_COMMAND_PARAMETER2              (BCM_MAILBOX_BASE+0x08)  //Write Only
#define BCM_COMMAND_PARAMETER3              (BCM_MAILBOX_BASE+0x0c)  //Write Only
#define BCM_COMMAND_PARAMETER4              (BCM_MAILBOX_BASE+0x10)  //Write Only
#define BCM_COMMAND_PARAMETER5              (BCM_MAILBOX_BASE+0x14)  //Write Only
#define BCM_COMMAND_PARAMETER6              (BCM_MAILBOX_BASE+0x18)  //Write Only
#define BCM_COMMAND_PARAMETER7              (BCM_MAILBOX_BASE+0x1c)  //Write Only
#define BCM_COMMAND_PARAMETER8              (BCM_MAILBOX_BASE+0x20)  //Write Only
#define BCM_COMMAND_PARAMETER9              (BCM_MAILBOX_BASE+0x24)  //Write Only
#define BCM_COMMAND_PARAMETER10             (BCM_MAILBOX_BASE+0x28)  //Write Only
#define BCM_COMMAND_PARAMETER11             (BCM_MAILBOX_BASE+0x2c)  //Write Only
#define BCM_COMMAND_PARAMETER12             (BCM_MAILBOX_BASE+0x30)  //Write Only
#define BCM_COMMAND_PARAMETER13             (BCM_MAILBOX_BASE+0x34)  //Write Only
#define BCM_COMMAND_PARAMETER14             (BCM_MAILBOX_BASE+0x38)  //Write Only: linked-list dma host-read pointer
#define BCM_COMMAND_PARAMETER15             (BCM_MAILBOX_BASE+0x3c)  //Write Only: linked-list dma host-write pointer
#define BCM_SECURE_PROCESSOR_COMMAND        (BCM_MAILBOX_BASE+0x40)  //Write Only

#define BCM_COMMAND_RETURN_STATUS           (BCM_MAILBOX_BASE+0x80)  //Read Only
#define BCM_COMMAND_STATUS0                 (BCM_MAILBOX_BASE+0x84)  //Read Only
#define BCM_COMMAND_STATUS1                 (BCM_MAILBOX_BASE+0x88)  //Read Only
#define BCM_COMMAND_STATUS2                 (BCM_MAILBOX_BASE+0x8c)  //Read Only
#define BCM_COMMAND_STATUS3                 (BCM_MAILBOX_BASE+0x90)  //Read Only
#define BCM_COMMAND_STATUS4                 (BCM_MAILBOX_BASE+0x94)  //Read Only
#define BCM_COMMAND_STATUS5                 (BCM_MAILBOX_BASE+0x98)  //Read Only
#define BCM_COMMAND_STATUS6                 (BCM_MAILBOX_BASE+0x9c)  //Read Only
#define BCM_COMMAND_STATUS7                 (BCM_MAILBOX_BASE+0xa0)  //Read Only
#define BCM_COMMAND_STATUS8                 (BCM_MAILBOX_BASE+0xa4)  //Read Only
#define BCM_COMMAND_STATUS9                 (BCM_MAILBOX_BASE+0xa8)  //Read Only
#define BCM_COMMAND_STATUS10                (BCM_MAILBOX_BASE+0xac)  //Read Only
#define BCM_COMMAND_STATUS11                (BCM_MAILBOX_BASE+0xb0)  //Read Only
#define BCM_COMMAND_STATUS12                (BCM_MAILBOX_BASE+0xb4)  //Read Only
#define BCM_COMMAND_STATUS13                (BCM_MAILBOX_BASE+0xb8)  //Read Only
#define BCM_COMMAND_STATUS14                (BCM_MAILBOX_BASE+0xbc)  //Read Only
#define BCM_COMMAND_STATUS15                (BCM_MAILBOX_BASE+0xc0)  //Read Only
#define BCM_COMMAND_FIFO_STATUS             (BCM_MAILBOX_BASE+0xc4)  //Read Only

#define BCM_HOST_INTERRUPT_REGISTER         (BCM_MAILBOX_BASE+0xC8)  //Write to clear
#define BCM_HOST_INTERRUPT_MASK             (BCM_MAILBOX_BASE+0xcc)  //Write-to-Clear
#define BCM_HOST_EXCEPTION_ADDRESS          (BCM_MAILBOX_BASE+0xd0)  //Write-to-Clear
#define BCM_SP_TRUST_REGISTER               (BCM_MAILBOX_BASE+0xd4)  //Read Only
#define BCM_WTM_IDENTIFICATION              (BCM_MAILBOX_BASE+0xd8)  //Read Only
#define BCM_WTM_REVISION                    (BCM_MAILBOX_BASE+0xdc)  //Read Only

#define BCM_CLOCK_CTRL                      (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_bcmClk)

#define BCM_REG(addr)                       (vaddr_t)phys_to_virt_io(addr, MEMMAP_BCM_REG_SIZE)

#define BCM_Reg_Write32(offset, val)        ((*(volatile uint32_t*)(BCM_REG((offset))))=val)
#define BCM_Reg_Read32(offset, holder)      ((*(volatile uint32_t*)(holder))=(*(uint32_t*)(BCM_REG(offset))))

#define BCM_Reg_Write16(offset, val)        ((*(volatile uint16_t*)(BCM_REG(offset)))=val)
#define BCM_Reg_Read16(offset, holder)      ((*(volatile uint16_t*)(holder))=(*(uint16_t*)(BCM_REG(offset))))

int execute_bcm_command(BCM_MAILBOX_COMMAND *pCmd, BCM_MAILBOX_STATUS *pStatus, uint32_t delay);

#endif
