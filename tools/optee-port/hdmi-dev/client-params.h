// SPDX-License-Identifier: GPL-2.0-only
#ifndef VS680_HDMI_DEV_CLIENT_PARAMS_H
#define VS680_HDMI_DEV_CLIENT_PARAMS_H

#include <stddef.h>
#include <stdint.h>
#include <tee_client_api.h>
#include "hdmi-dev.h"

_Static_assert(sizeof(struct hdmi_dev_status) == 24, "HDMI status ABI size");
_Static_assert(offsetof(struct hdmi_dev_status, flags) == 20, "HDMI status ABI fields");

static inline int hdmi_dev_init_operation(TEEC_Operation *operation,
                                         uint32_t pool_pa, uint32_t pool_bytes)
{
    if (!operation || !hdmi_dev_pool_valid(pool_pa, pool_bytes))
        return -1;
    *operation = (TEEC_Operation){0};
    operation->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
                                           TEEC_NONE, TEEC_NONE);
    operation->params[0].value.a = pool_pa;
    operation->params[0].value.b = pool_bytes;
    return 0;
}

_Static_assert(sizeof(struct hdmi_dev_mode_request)==112,"mode request ABI");
static inline int hdmi_dev_init_mode_operation(TEEC_Operation *o,
    const struct hdmi_dev_mode_request *q)
{
    if(!o || !q || q->version!=HDMI_DEV_MODE_VERSION || q->size!=sizeof(*q) ||
        !hdmi_dev_pool_valid(q->pool_pa,q->pool_bytes))return -1;
    *o=(TEEC_Operation){0};
    o->paramTypes=TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,TEEC_NONE,TEEC_NONE,TEEC_NONE);
    o->params[0].tmpref.buffer=(void *)q;o->params[0].tmpref.size=sizeof(*q);
    return 0;
}

static inline int hdmi_dev_show_operation(TEEC_Operation *operation,
                                         uint32_t pattern, uint32_t rgb)
{
    if (!operation || pattern > HDMI_DEV_PATTERN_SOLID ||
        rgb > UINT32_C(0xffffff) ||
        (pattern == HDMI_DEV_PATTERN_COORDINATE && rgb != 0))
        return -1;
    *operation = (TEEC_Operation){0};
    operation->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
                                           TEEC_NONE, TEEC_NONE);
    operation->params[0].value.a = pattern;
    operation->params[0].value.b = rgb;
    return 0;
}

static inline int hdmi_dev_blend_operation(TEEC_Operation *operation,
                                            uint32_t rgb, uint32_t alpha_q12)
{
    if (!operation || rgb>UINT32_C(0xffffff) || alpha_q12>4095u) return -1;
    *operation=(TEEC_Operation){0};
    operation->paramTypes=TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,TEEC_NONE,TEEC_NONE,TEEC_NONE);
    operation->params[0].value.a=rgb; operation->params[0].value.b=alpha_q12;
    return 0;
}

static inline void hdmi_dev_service_operation(TEEC_Operation *operation)
{
    *operation = (TEEC_Operation){0};
    operation->paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_NONE, TEEC_NONE);
}

static inline void hdmi_dev_status_operation(TEEC_Operation *operation,
                                             struct hdmi_dev_status *status)
{
    *status = (struct hdmi_dev_status){0};
    *operation = (TEEC_Operation){0};
    operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE,
                                           TEEC_NONE, TEEC_NONE);
    operation->params[0].tmpref.buffer = status;
    operation->params[0].tmpref.size = sizeof(*status);
}

static inline int hdmi_dev_status_valid(const struct hdmi_dev_status *status,
                                        size_t size)
{
    if (!status || size != sizeof(*status) || status->state > HDMI_DEV_FAULT ||
        (status->flags & ~HDMI_DEV_DMA_QUIESCENT))
        return 0;
    if (status->state == HDMI_DEV_READY || status->state == HDMI_DEV_DISPLAYING) {
        if (!status->width || status->width>2560u || !status->height || status->height>1440u ||
            !status->refresh_millihz)
            return 0;
    }
    return status->state != HDMI_DEV_DISPLAYING ||
           !(status->flags & HDMI_DEV_DMA_QUIESCENT);
}

static inline int hdmi_dev_status_stopped(const struct hdmi_dev_status *status)
{
    return (status->state == HDMI_DEV_UNINITIALIZED ||
            status->state == HDMI_DEV_READY) &&
           (status->flags & HDMI_DEV_DMA_QUIESCENT);
}

#endif
