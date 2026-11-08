// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <arm.h>
#include <console.h>
#include <drivers/gic.h>
#include <drivers/serial8250_uart.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/interrupt.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <kernel/tee_time.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <platform_config.h>
#include <stdint.h>
#include <string.h>
#include <tee/entry_fast.h>
#include <tee/entry_std.h>
#include <tee/cache.h>
#include <kernel/tee_common_otp.h>
#include "syna_fastcall_entry.h"
#include "syna_setup.h"

static struct serial8250_uart_data console_data;

register_phys_mem(MEM_AREA_RAM_SEC, TZDRAM_BOOT_PARAM, TZDRAM_BOOT_PARAM_SIZE);

register_phys_mem(MEM_AREA_IO_SEC, 0xf6000000, 0x4000000);

#ifdef BERLIN_DEVICE1_SIZE
register_phys_mem(MEM_AREA_IO_SEC, BERLIN_DEVICE1_BASE, BERLIN_DEVICE1_SIZE);
#endif
#ifdef BERLIN_DEVICE2_SIZE
register_phys_mem(MEM_AREA_IO_SEC, BERLIN_DEVICE2_BASE, BERLIN_DEVICE2_SIZE);
#endif

#ifdef DRAM0_BASE
register_ddr(DRAM0_BASE, DRAM0_SIZE);
#endif
#ifdef DRAM1_BASE
register_ddr(DRAM1_BASE, DRAM1_SIZE);
#endif

void boot_primary_init_intc(void)
{
	gic_init(GICC_BASE, GICD_BASE);
}

void boot_secondary_init_intc(void)
{
	gic_init_per_cpu();
}

void plat_console_init(void)
{
	serial8250_uart_init(&console_data, CONSOLE_UART_BASE,
		CONSOLE_UART_CLK_IN_HZ, CONSOLE_BAUDRATE);

	register_serial_console(&console_data.chip);
}

TEE_Result __weak syna_get_hw_unique_key(size_t key_len __unused, uint8_t *skey __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}

TEE_Result tee_otp_get_hw_unique_key(struct tee_hw_unique_key *hwkey)
{
	TEE_Result ret = TEE_ERROR_GENERIC;
	int key_len = 32;
	static uint8_t syna_hw_unique_key[32] __aligned(CACHELINE_LEN) = {
		0x3C, 0x9C, 0x35, 0xC7, 0x9F, 0xF2, 0x0C, 0x5F,
		0x2A, 0xD1, 0xF4, 0xDE, 0x3A, 0x15, 0x57, 0x25,
		0x2D, 0x13, 0x50, 0xEE, 0xED, 0x9A, 0xB8, 0x16,
		0xEA, 0xB8, 0x63, 0x6B, 0x70, 0x0D, 0x14, 0x97};
	static int already_get_syna_hw_unique_key = 0;

	if (already_get_syna_hw_unique_key) {
		memcpy(&hwkey->data[0], syna_hw_unique_key, HW_UNIQUE_KEY_LENGTH);
		return TEE_SUCCESS;
	}

	ret = syna_get_hw_unique_key(key_len, syna_hw_unique_key);
	if (ret)
		return TEE_ERROR_SECURITY;

	memcpy(&hwkey->data[0], syna_hw_unique_key, HW_UNIQUE_KEY_LENGTH);

	already_get_syna_hw_unique_key = 1;

	DHEXDUMP(syna_hw_unique_key, key_len);

	return TEE_SUCCESS;
}


extern uint64_t __save_boot_regs[8];

void  boot_save_args(unsigned long a0, unsigned long a1,
			   unsigned long a2, unsigned long a3,
			   unsigned long a4)
{
	DMSG("a0 = 0x%lx, a1 = 0x%lx, a2 = 0x%lx, a3 = 0x%lx, a4 = 0x%lx",
			a0, a1, a2, a3, a4);
	__save_boot_regs[0] = a0;
	__save_boot_regs[1] = a1;
	__save_boot_regs[2] = a2;
	__save_boot_regs[3] = a3;
	__save_boot_regs[4] = a4;
}
