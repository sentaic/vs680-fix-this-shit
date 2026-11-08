/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BCM_REG_H__
#define __BCM_REG_H__

// the command mailbox registers which appears in the Client's memory space
#ifndef MEMMAP_BCM_REG_BASE
#define MEMMAP_BCM_REG_BASE                 0xF7930000
#endif

#define BCM_MAILBOX_BASE                    (MEMMAP_BCM_REG_BASE)
#define BCM_PRIMITIVE_COMMAND_PARAMETER0    (BCM_MAILBOX_BASE+0x00)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER1    (BCM_MAILBOX_BASE+0x04)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER2    (BCM_MAILBOX_BASE+0x08)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER3    (BCM_MAILBOX_BASE+0x0c)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER4    (BCM_MAILBOX_BASE+0x10)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER5    (BCM_MAILBOX_BASE+0x14)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER6    (BCM_MAILBOX_BASE+0x18)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER7    (BCM_MAILBOX_BASE+0x1c)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER8    (BCM_MAILBOX_BASE+0x20)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER9    (BCM_MAILBOX_BASE+0x24)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER10   (BCM_MAILBOX_BASE+0x28)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER11   (BCM_MAILBOX_BASE+0x2c)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER12   (BCM_MAILBOX_BASE+0x30)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER13   (BCM_MAILBOX_BASE+0x34)  //Write Only
#define BCM_PRIMITIVE_COMMAND_PARAMETER14   (BCM_MAILBOX_BASE+0x38)  //Write Only: linked-list dma host-read pointer
#define BCM_PRIMITIVE_COMMAND_PARAMETER15   (BCM_MAILBOX_BASE+0x3c)  //Write Only: linked-list dma host-write pointer
#define BCM_SECURE_PROCESSOR_COMMAND        (BCM_MAILBOX_BASE+0x40)  //Write Only

#define BCM_COMMAND_RETURN_STATUS           (BCM_MAILBOX_BASE+0x80)  //Read Only
#define BCM_COMMAND_STATUS_0                (BCM_MAILBOX_BASE+0x84)  //Read Only
#define BCM_COMMAND_STATUS_1                (BCM_MAILBOX_BASE+0x88)  //Read Only
#define BCM_COMMAND_STATUS_2                (BCM_MAILBOX_BASE+0x8c)  //Read Only
#define BCM_COMMAND_STATUS_3                (BCM_MAILBOX_BASE+0x90)  //Read Only
#define BCM_COMMAND_STATUS_4                (BCM_MAILBOX_BASE+0x94)  //Read Only
#define BCM_COMMAND_STATUS_5                (BCM_MAILBOX_BASE+0x98)  //Read Only
#define BCM_COMMAND_STATUS_6                (BCM_MAILBOX_BASE+0x9c)  //Read Only
#define BCM_COMMAND_STATUS_7                (BCM_MAILBOX_BASE+0xa0)  //Read Only
#define BCM_COMMAND_STATUS_8                (BCM_MAILBOX_BASE+0xa4)  //Read Only
#define BCM_COMMAND_STATUS_9                (BCM_MAILBOX_BASE+0xa8)  //Read Only
#define BCM_COMMAND_STATUS_10               (BCM_MAILBOX_BASE+0xac)  //Read Only
#define BCM_COMMAND_STATUS_11               (BCM_MAILBOX_BASE+0xb0)  //Read Only
#define BCM_COMMAND_STATUS_12               (BCM_MAILBOX_BASE+0xb4)  //Read Only
#define BCM_COMMAND_STATUS_13               (BCM_MAILBOX_BASE+0xb8)  //Read Only
#define BCM_COMMAND_STATUS_14               (BCM_MAILBOX_BASE+0xbc)  //Read Only
#define BCM_COMMAND_STATUS_15               (BCM_MAILBOX_BASE+0xc0)  //Read Only
#define BCM_COMMAND_FIFO_STATUS             (BCM_MAILBOX_BASE+0xc4)  //Read Only

#define BCM_HOST_INTERRUPT_REGISTER         (BCM_MAILBOX_BASE+0xC8)  //Write to clear
#define BCM_HOST_INTERRUPT_MASK             (BCM_MAILBOX_BASE+0xcc)  //Write-to-Clear
#define BCM_HOST_EXCEPTION_ADDRESS          (BCM_MAILBOX_BASE+0xd0)  //Write-to-Clear
#define BCM_SP_TRUST_REGISTER               (BCM_MAILBOX_BASE+0xd4)  //Read Only
#define BCM_WTM_IDENTIFICATION              (BCM_MAILBOX_BASE+0xd8)  //Read Only
#define BCM_WTM_REVISION                    (BCM_MAILBOX_BASE+0xdc)  //Read Only

#endif
