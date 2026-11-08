// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2019-2025 Synaptics Incorporated */

#ifndef _AVIO_FL_INFO_H_
#define _AVIO_FL_INFO_H_

typedef struct bootloader_info_t  {
	u32 status : 1;     // Successfully displayed logo or not
	u32 devnum : 1;     // Device number (0 or 1)
	u32 hw_partition : 3;   // Partition type (0=DEFAULT, 4=GP1, 5=GP2 etc)
	u32 sw_partition : 5;  // Partition identifier (partition number)
	u32 cpcb0ResId : 8; // CpCb0 resolution -- may be additional bits for depth/format
	u32 cpcb1ResId : 8; // CpCb1 resolution
	u32 reserved : 6;   // Reserved for future use
} BOOTLOADER_INFO;

typedef union  avio_fastlogo_info_u {
	u32 fl_disp_info;
	BOOTLOADER_INFO u;
} avio_fastlogo_info;

avio_fastlogo_info avio_get_fastlogo_status(void);
void avio_set_fastlogo_status(int status);

#endif
