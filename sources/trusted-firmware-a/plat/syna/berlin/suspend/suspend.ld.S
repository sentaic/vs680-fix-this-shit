/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <platform_def.h>

OUTPUT_FORMAT(PLATFORM_LINKER_FORMAT)
OUTPUT_ARCH(PLATFORM_LINKER_ARCH)
ENTRY(suspend_entrypoint)

MEMORY {
    RAM (rwx): ORIGIN = SUSPEND_SRAM_BASE, LENGTH = SRAM_BACKUP_SIZE
}

SECTIONS
{
    . = SUSPEND_SRAM_BASE;
    ASSERT(. == ALIGN(4096),
           "TSPREG_FIGODTCM_BASE address is not aligned on a page boundary.")

    ro . : {
	__RO_START__ = .;
    __TEXT_START__ = .;
        *suspend_entrypoint.o(.text*)
        *(.text*)
        *(.rodata*)
        /*
         * Keep the .got section in the RO section as it is patched
         * prior to enabling the MMU and having the .got in RO is better for
         * security. GOT is a table of addresses so ensure 8-byte alignment.
         */
        . = ALIGN(8);
        __GOT_START__ = .;
        *(.got)
        __GOT_END__ = .;

	__RO_END_UNALIGNED__ = .;
	. = NEXT(4096);
	__RO_END__ = .;
    } >RAM

    .stack (NOLOAD) : {
        __STACKS_START__ = .;
	*(tzfw_normal_stacks)
        __STACKS_END__ = .;
    } >RAM

    /*
     * The .bss section gets initialised to 0 at runtime.
     * Its base address must be 16-byte aligned.
     */
    .bss : ALIGN(16) {
        __BSS_START__ = .;
        *(.bss*)
        *(COMMON)
        __BSS_END__ = .;
    } >RAM
    __BSS_SIZE__ = SIZEOF(.bss);

    .data . : {
	__DATA_START__ = .;
        *(.data*)
	__DATA_END__ = .;
    } >RAM

    /*
     * .rela.dyn needs to come after .data for the read-elf utility to parse
     * this section correctly. Ensure 8-byte alignment so that the fields of
     * RELA data structure are aligned.
     */
    . = ALIGN(8);
    __RELA_START__ = .;
    .rela.dyn . : {
    } >RAM
    __RELA_END__ = .;
    __SUSPEND_END__ = .;

    ASSERT(. <= SUSPEND_SRAM_BASE + SRAM_BACKUP_SIZE,
           "Suspend image has exceeded its limit.")
}
