/* SPDX-License-Identifier: GPL-2.0 */
#include "mode-access.h"
#include <tee_api_syna.h>
#include "Galois_memmap.h"

#define AVIO_BYTES UINT32_C(0xc0000)
#define CHIP_BYTES UINT32_C(4096)

static TEE_Result word_address(struct hdmi_mode_access *access, uint32_t physical,
                               volatile uint32_t **word)
{
    uint32_t base, bytes;
    volatile uint32_t **view;
    void *mapped = NULL;
    TEE_Result result;
    if (!access || !word || physical % sizeof(uint32_t))
        return TEE_ERROR_BAD_PARAMETERS;
    if (physical >= MEMMAP_AVIO_REG_BASE &&
        physical - MEMMAP_AVIO_REG_BASE < AVIO_BYTES) {
        base = MEMMAP_AVIO_REG_BASE;
        bytes = AVIO_BYTES;
        view = &access->avio;
    } else if (physical >= MEMMAP_CHIP_CTRL_REG_BASE &&
               physical - MEMMAP_CHIP_CTRL_REG_BASE < CHIP_BYTES) {
        base = MEMMAP_CHIP_CTRL_REG_BASE;
        bytes = CHIP_BYTES;
        view = &access->chip;
    } else {
        return TEE_ERROR_NOT_SUPPORTED;
    }
    if (!*view) {
        result = TEE_Map_Register(base, bytes, 0, 0, &mapped);
        if (result) return result;
        if (!mapped) return TEE_ERROR_BAD_STATE;
        *view = mapped;
    }
    *word = *view + (physical - base) / sizeof(uint32_t);
    return TEE_SUCCESS;
}

static TEE_Result mode_read(void *opaque, uint32_t physical, uint32_t *value)
{
    volatile uint32_t *word;
    TEE_Result result;
    if (!value) return TEE_ERROR_BAD_PARAMETERS;
    *value = 0;
    result = word_address(opaque, physical, &word);
    if (result) return result;
    __asm__ volatile("dsb sy" ::: "memory");
    *value = *word;
    __asm__ volatile("dsb sy" ::: "memory");
    return TEE_SUCCESS;
}

static TEE_Result mode_write(void *opaque, uint32_t physical, uint32_t value)
{
    volatile uint32_t *word;
    TEE_Result result = word_address(opaque, physical, &word);
    if (result) return result;
    *word = value;
    __asm__ volatile("dsb sy" ::: "memory");
    return TEE_SUCCESS;
}

static TEE_Result mode_wait(void *opaque, uint32_t milliseconds)
{
    if (!opaque || !milliseconds || milliseconds > 1000u)
        return TEE_ERROR_BAD_PARAMETERS;
    return TEE_Wait(milliseconds);
}

TEE_Result hdmi_mode_access_bind(struct hdmi_mode_access *access,
                                 struct hdmi_mode_io *io)
{
    if (!access || !io) return TEE_ERROR_BAD_PARAMETERS;
    *io = (struct hdmi_mode_io){
        .opaque = access,
        .read32 = mode_read,
        .write32 = mode_write,
        .wait_ms = mode_wait,
    };
    return TEE_SUCCESS;
}
