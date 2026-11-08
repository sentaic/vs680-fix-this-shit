/* SPDX-License-Identifier: GPL-2.0 */
#include <stdint.h>
#include <tee_api_syna.h>

static TEE_Result display_ddc_pins(TEE_Param params[4], int select_ddc)
{
    static void *mapping;
    /* VS680 A0 global.h and pinctrl-vs680.c: AVIO mux and DDC pad controls. */
    if (!mapping) {
        TEE_Result result = TEE_Map_Register((paddr_t)0xf7ea8000, 0x1000, 0, 0,
            &mapping);
        if (result) return result;
    }
    volatile uint32_t *mux = (volatile uint32_t *)((uint8_t *)mapping + 0x404);
    volatile uint32_t *scl = (volatile uint32_t *)((uint8_t *)mapping + 0xc40);
    volatile uint32_t *sda = (volatile uint32_t *)((uint8_t *)mapping + 0xc44);
    if (select_ddc) {
        const uint32_t mask = (7u << 18) | (7u << 21);
        *mux = (*mux & ~mask) | (1u << 18) | (1u << 21);
    }
    params[0].value.a = *mux;
    params[0].value.b = *scl;
    params[1].value.a = *sda;
    if (select_ddc && (((params[0].value.a >> 18) & 7u) != 1u ||
                       ((params[0].value.a >> 21) & 7u) != 1u))
        return TEE_ERROR_BAD_STATE;
    /* The platform maps register pages permanently for this TA's lifetime. */
    return TEE_SUCCESS;
}
