// SPDX-License-Identifier: GPL-2.0-only
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <tee_internal_api.h>
#include "backend.h"
#include "video-pattern.h"
#include "show-timing.h"
#include "switch-timing.h"

#define NO_PARAMS TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, \
                                 TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE)
#define INIT_PARAMS TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT, \
                                   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, \
                                   TEE_PARAM_TYPE_NONE)
#define SHOW_PARAMS TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT, \
                                   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, \
                                   TEE_PARAM_TYPE_NONE)
#define STATUS_PARAMS TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT, \
                                     TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, \
                                     TEE_PARAM_TYPE_NONE)

_Static_assert(sizeof(struct hdmi_dev_status) == 24, "HDMI status ABI size");
_Static_assert(offsetof(struct hdmi_dev_status, flags) == 20,
               "HDMI status ABI fields");

/* 从后端读取硬件状态。 */
static TEE_Result read_status(struct hdmi_dev_status *status)
{
    TEE_Result result;

    memset(status, 0, sizeof(*status));
    result = hdmi_dev_backend_status(status);
    if (result != TEE_SUCCESS)
        return result;
    if (status->state > HDMI_DEV_FAULT ||
        (status->flags & ~HDMI_DEV_DMA_QUIESCENT))
        return TEE_ERROR_BAD_STATE;
    if (status->state == HDMI_DEV_READY ||
        status->state == HDMI_DEV_DISPLAYING) {
        if (!status->width || status->width>2560 || !status->height || status->height>1440 || !status->refresh_millihz)
            return TEE_ERROR_BAD_STATE;
    }
    if (status->state == HDMI_DEV_DISPLAYING &&
        (status->flags & HDMI_DEV_DMA_QUIESCENT))
        return TEE_ERROR_BAD_STATE;
    return TEE_SUCCESS;
}

static TEE_Result initialize(uint32_t pool_pa, uint32_t pool_bytes)
{
    struct hdmi_dev_status status;
    TEE_Result result;

    if (!hdmi_dev_pool_valid(pool_pa, pool_bytes))
        return TEE_ERROR_BAD_PARAMETERS;
    result = hdmi_dev_backend_init(pool_pa, pool_bytes);

    if (result != TEE_SUCCESS)
        return result;
    result = read_status(&status);
    if (result != TEE_SUCCESS)
        return result;
    return status.state == HDMI_DEV_READY ? TEE_SUCCESS : TEE_ERROR_BAD_STATE;
}

static TEE_Result show(uint32_t pattern, uint32_t rgb)
{
    struct hdmi_dev_frame frame = { 0 };
    struct hdmi_dev_status status;
    TEE_Result result;
    hdmi_show_timing_begin(pattern,rgb);
    /* Validate before any backend call or write. */
    if (pattern > HDMI_DEV_PATTERN_SOLID || rgb > UINT32_C(0xffffff) ||
        (pattern == HDMI_DEV_PATTERN_COORDINATE && rgb != 0))
        return hdmi_show_timing_end(TEE_ERROR_BAD_PARAMETERS);
    result = hdmi_dev_backend_begin_frame(&frame);
    hdmi_show_timing_mark(HDMI_SHOW_BORROWED);
    if (result != TEE_SUCCESS) return hdmi_show_timing_end(result);
    int filled=hdmi_dev_fill_pattern(frame.mapped, frame.capacity, frame.width,
        frame.height, frame.stride, pattern, rgb);
    hdmi_show_timing_mark(HDMI_SHOW_FILLED);
    if (filled) return hdmi_show_timing_end(TEE_ERROR_BAD_STATE);
    result = hdmi_dev_backend_present();
    hdmi_show_timing_mark(HDMI_SHOW_PRESENTED);
    if (result != TEE_SUCCESS) return hdmi_show_timing_end(result);
    result = read_status(&status);
    hdmi_show_timing_mark(HDMI_SHOW_STATUS_READ);
    if (result != TEE_SUCCESS) return hdmi_show_timing_end(result);
    return hdmi_show_timing_end(status.state == HDMI_DEV_DISPLAYING ?
        TEE_SUCCESS : TEE_ERROR_BAD_STATE);
}

static TEE_Result stop(void)
{
    struct hdmi_dev_status status;
    TEE_Result result;

    /* 初始化中途失败时也允许 STOP。 */
    result = hdmi_dev_backend_stop();
    if (result != TEE_SUCCESS)
        return result;
    result = read_status(&status);
    if (result != TEE_SUCCESS)
        return result;
    if ((status.state != HDMI_DEV_UNINITIALIZED &&
         status.state != HDMI_DEV_READY) ||
        !(status.flags & HDMI_DEV_DMA_QUIESCENT))
        return TEE_ERROR_BAD_STATE;
    return TEE_SUCCESS;
}

TEE_Result TA_CreateEntryPoint(void)
{
    /* 硬件配置由 INIT 命令执行。 */
    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
    /* DMA 缓冲由 Linux holder 持有。 */
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types, TEE_Param params[4],
                                    void **session_context)
{
    (void)params;
    if (param_types != NO_PARAMS || !session_context)
        return TEE_ERROR_BAD_PARAMETERS;
    *session_context = NULL;
    /* OP-TEE SINGLE_INSTANCE without MULTI_SESSION enforces one owner. */
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *session_context)
{
    (void)session_context;
    /* 会话关闭后保留现有硬件状态，停止由 STOP 命令执行。 */
}

TEE_Result TA_InvokeCommandEntryPoint(void *session_context, uint32_t command,
                                      uint32_t param_types, TEE_Param params[4])
{
    struct hdmi_dev_status status;
    TEE_Result result;

    (void)session_context;
    switch (command) {
#ifdef HDMI_DEV_SHOW_TIMING_ENABLED
    case HDMI_SHOW_TIMING_READ:
    case HDMI_SWITCH_TIMING_READ:
        if (param_types != STATUS_PARAMS || !params) return TEE_ERROR_BAD_PARAMETERS;
        result=read_status(&status);
        if (result) return result;
        if (status.state!=HDMI_DEV_UNINITIALIZED ||
            !(status.flags&HDMI_DEV_DMA_QUIESCENT)) return TEE_ERROR_BAD_STATE;
        if (command==HDMI_SWITCH_TIMING_READ)
            return hdmi_switch_timing_read(params[0].memref.buffer,&params[0].memref.size);
        return hdmi_show_timing_read(params[0].memref.buffer,&params[0].memref.size);
#endif
    case HDMI_DEV_DDC_RECOVER: {
        struct hdmi_dev_ddc_recovery reply;
        if(param_types!=STATUS_PARAMS||!params||!params[0].memref.buffer)
            return TEE_ERROR_BAD_PARAMETERS;
        if(params[0].memref.size<sizeof(reply)) {
            params[0].memref.size=sizeof(reply);return TEE_ERROR_SHORT_BUFFER;
        }
        result=hdmi_dev_backend_ddc_recover(&reply);
        if(result)return result;
        memcpy(params[0].memref.buffer,&reply,sizeof(reply));
        params[0].memref.size=sizeof(reply);return TEE_SUCCESS;
    }
    case HDMI_DEV_DDC_OWNER_PREPARE: {
        struct hdmi_dev_ddc_prepare reply;
        if(param_types!=STATUS_PARAMS||!params||!params[0].memref.buffer)
            return TEE_ERROR_BAD_PARAMETERS;
        if(params[0].memref.size<sizeof(reply)){
            params[0].memref.size=sizeof(reply);return TEE_ERROR_SHORT_BUFFER;
        }
        result=hdmi_dev_backend_ddc_prepare(&reply);
        if(result)return result;
        memcpy(params[0].memref.buffer,&reply,sizeof(reply));
        params[0].memref.size=sizeof(reply);return TEE_SUCCESS;
    }
    case HDMI_DEV_DDC_READ: {
        struct hdmi_dev_ddc_read reply;
        if(param_types!=STATUS_PARAMS||!params||!params[0].memref.buffer)
            return TEE_ERROR_BAD_PARAMETERS;
        if(params[0].memref.size<sizeof(reply)) {
            params[0].memref.size=sizeof(reply);return TEE_ERROR_SHORT_BUFFER;
        }
        result=hdmi_dev_backend_ddc_read(&reply);
        if(result)return result;
        memcpy(params[0].memref.buffer,&reply,sizeof(reply));
        params[0].memref.size=sizeof(reply);return TEE_SUCCESS;
    }
    case HDMI_DEV_DDC_SNAPSHOT: {
        struct hdmi_dev_ddc_snapshot reply;
        if(param_types!=STATUS_PARAMS||!params||!params[0].memref.buffer)
            return TEE_ERROR_BAD_PARAMETERS;
        if(params[0].memref.size<sizeof(reply)) {
            params[0].memref.size=sizeof(reply);return TEE_ERROR_SHORT_BUFFER;
        }
        result=hdmi_dev_backend_ddc_snapshot(&reply);
        if(result)return result;
        memcpy(params[0].memref.buffer,&reply,sizeof(reply));
        params[0].memref.size=sizeof(reply);return TEE_SUCCESS;
    }
    case HDMI_DEV_INIT:
        if (param_types != INIT_PARAMS || !params)
            return TEE_ERROR_BAD_PARAMETERS;
        return initialize(params[0].value.a, params[0].value.b);
    case HDMI_DEV_PREFLIGHT_MODE: {
        struct hdmi_dev_mode_request q;
        struct hdmi_dev_mode_preflight reply;
        if(!params || param_types!=TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
                TEE_PARAM_TYPE_MEMREF_OUTPUT,TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE) ||
            !params[0].memref.buffer || params[0].memref.size!=sizeof(q) ||
            !params[1].memref.buffer)return TEE_ERROR_BAD_PARAMETERS;
        if(params[1].memref.size<sizeof(reply)) {
            params[1].memref.size=sizeof(reply);return TEE_ERROR_SHORT_BUFFER;
        }
        memcpy(&q,params[0].memref.buffer,sizeof(q));
        result=hdmi_dev_backend_preflight_mode(&q,&reply);
        if(result)return result;
        memcpy(params[1].memref.buffer,&reply,sizeof(reply));
        params[1].memref.size=sizeof(reply);return TEE_SUCCESS;
    }
    case HDMI_DEV_INIT_MODE: {
        struct hdmi_dev_mode_request q;
        if (!params || param_types!=TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
                TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE) ||
            !params[0].memref.buffer || params[0].memref.size!=sizeof(q))
            return TEE_ERROR_BAD_PARAMETERS;
        memcpy(&q,params[0].memref.buffer,sizeof(q));
        result=hdmi_dev_backend_init_mode(&q);
        if(result)return result;
        result=read_status(&status);
        if(result)return result;
        return status.state==HDMI_DEV_READY ? TEE_SUCCESS : TEE_ERROR_BAD_STATE;
    }
    case HDMI_DEV_SHOW:
        if (param_types != SHOW_PARAMS || !params)
            return TEE_ERROR_BAD_PARAMETERS;
        return show(params[0].value.a, params[0].value.b);
    case HDMI_DEV_BLEND:
        if (param_types != SHOW_PARAMS || !params ||
            params[0].value.a > UINT32_C(0xffffff) || params[0].value.b > 4095u)
            return TEE_ERROR_BAD_PARAMETERS;
        return hdmi_dev_backend_blend(params[0].value.a, params[0].value.b);
    case HDMI_DEV_OVERLAY: {
        struct hdmi_dev_overlay geometry;
        if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
                TEE_PARAM_TYPE_MEMREF_INPUT,TEE_PARAM_TYPE_NONE,TEE_PARAM_TYPE_NONE) ||
            !params || !params[0].memref.buffer || !params[1].memref.buffer ||
            params[0].memref.size != sizeof(geometry)) return TEE_ERROR_BAD_PARAMETERS;
        memcpy(&geometry,params[0].memref.buffer,sizeof(geometry));
        if (!hdmi_dev_overlay_valid(&geometry,params[1].memref.size))
            return TEE_ERROR_BAD_PARAMETERS;
        return hdmi_dev_backend_overlay(&geometry,params[1].memref.buffer,
            params[1].memref.size);
    }
    case HDMI_DEV_STATUS:
        if (param_types != STATUS_PARAMS || !params ||
            !params[0].memref.buffer ||
            params[0].memref.size != sizeof(status))
            return TEE_ERROR_BAD_PARAMETERS;
        result = read_status(&status);
        if (result != TEE_SUCCESS)
            return result;
        /* 使用 memcpy 返回经过校验的状态，支持非对齐缓冲。 */
        memcpy(params[0].memref.buffer, &status, sizeof(status));
        return TEE_SUCCESS;
    case HDMI_DEV_STOP:
        if (param_types != NO_PARAMS)
            return TEE_ERROR_BAD_PARAMETERS;
        return stop();
    case HDMI_DEV_SERVICE:
        if (param_types != NO_PARAMS)
            return TEE_ERROR_BAD_PARAMETERS;
        /* 处理一次后端显示进度。 */
        return hdmi_dev_backend_service();
    default:
        return TEE_ERROR_NOT_SUPPORTED;
    }
}
