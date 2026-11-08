// SPDX-License-Identifier: GPL-2.0-only
#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "client-params.h"

static volatile sig_atomic_t stop_requested;
static const struct hdmi_dev_mode_request *active_mode;
static const TEEC_UUID uuid = HDMI_DEV_UUID;

static void request_stop(int signal_number)
{
    (void)signal_number;
    stop_requested = 1;
}

static TEEC_Result invoke(TEEC_Session *session, uint32_t command,
                          TEEC_Operation *operation, const char *name)
{
    uint32_t origin = 0;
    TEEC_Result result = TEEC_InvokeCommand(session, command, operation, &origin);
    printf("%s result=0x%08" PRIx32 " origin=%" PRIu32 "\n", name, result, origin);
    fflush(stdout);
    return result;
}

static TEEC_Result read_status(TEEC_Session *session, struct hdmi_dev_status *status)
{
    TEEC_Operation operation;
    hdmi_dev_status_operation(&operation, status);
    TEEC_Result result = invoke(session, HDMI_DEV_STATUS, &operation, "STATUS");
    if (result != TEEC_SUCCESS)
        return result;
    if (!hdmi_dev_status_valid(status, operation.params[0].tmpref.size)) {
        fprintf(stderr, "STATUS 返回的数据格式错误，长度=%zu\n",
                operation.params[0].tmpref.size);
        return TEEC_ERROR_BAD_FORMAT;
    }
    printf("state=%" PRIu32 " width=%" PRIu32 " height=%" PRIu32
           " refresh_millihz=%" PRIu32 " last_error=0x%08" PRIx32
           " flags=0x%08" PRIx32 "\n",
           status->state, status->width, status->height, status->refresh_millihz,
           status->last_error, status->flags);
    return TEEC_SUCCESS;
}

static TEEC_Result stop(TEEC_Session *session)
{
    TEEC_Operation operation = {0};
    struct hdmi_dev_status status;
    TEEC_Result result = invoke(session, HDMI_DEV_STOP, &operation, "STOP");
    if (result != TEEC_SUCCESS)
        return result;
    result = read_status(session, &status);
    if (result != TEEC_SUCCESS)
        return result;
    if (!hdmi_dev_status_stopped(&status)) {
        fprintf(stderr, "STOP 后未返回停止状态及 DMA_QUIESCENT，保留 DMA 内存\n");
        return TEEC_ERROR_BAD_STATE;
    }
    puts("TA 返回停止状态及 DMA_QUIESCENT");
    return TEEC_SUCCESS;
}

static TEEC_Result open_session(TEEC_Context *context, TEEC_Session *session)
{
    uint32_t origin = 0;
    TEEC_Result result = TEEC_OpenSession(context, session, &uuid,
                                         TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
    printf("OPEN uuid=%s result=0x%08" PRIx32 " origin=%" PRIu32 "\n",
           HDMI_DEV_UUID_STRING, result, origin);
    return result;
}

struct stream_counts {
    uint64_t accepted_submissions;
    uint64_t service_calls;
    uint64_t service_busy;
    uint64_t show_busy;
};

static int monotonic_ms(uint64_t *milliseconds)
{
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
        perror("读取连续提交计时");
        return -1;
    }
    *milliseconds = (uint64_t)now.tv_sec * 1000 + (uint64_t)now.tv_nsec / 1000000;
    return 0;
}

static TEEC_Result stream_invoke(TEEC_Session *session, uint32_t command,
                                TEEC_Operation *operation, const char *name)
{
    uint32_t origin = 0;
    TEEC_Result result = TEEC_InvokeCommand(session, command, operation, &origin);
    if (result != TEEC_SUCCESS && result != TEEC_ERROR_BUSY)
        fprintf(stderr, "%s result=0x%08" PRIx32 " origin=%" PRIu32 "\n",
                name, result, origin);
    return result;
}

static TEEC_Result stream_updates(TEEC_Session *session, uint32_t duration_ms,
                                  struct stream_counts *counts)
{
    uint64_t now;
    uint32_t pending_rgb = UINT32_C(0x00ffff);
    TEEC_Result result = TEEC_SUCCESS;
    if (monotonic_ms(&now))
        return TEEC_ERROR_GENERIC;
    const uint64_t deadline = now + duration_ms;
    while (!stop_requested) {
        TEEC_Operation operation;
        if (monotonic_ms(&now))
            return TEEC_ERROR_GENERIC;
        if (now >= deadline)
            break;
        hdmi_dev_service_operation(&operation);
        ++counts->service_calls;
        result = stream_invoke(session, HDMI_DEV_SERVICE, &operation, "SERVICE");
        if (result == TEEC_ERROR_BUSY) {
            ++counts->service_busy;
        } else {
            if (result != TEEC_SUCCESS)
                return result;
            if (stop_requested)
                break;
            if (monotonic_ms(&now))
                return TEEC_ERROR_GENERIC;
            if (now >= deadline)
                break;
            if (hdmi_dev_show_operation(&operation, HDMI_DEV_PATTERN_SOLID, pending_rgb))
                return TEEC_ERROR_BAD_PARAMETERS;
            result = stream_invoke(session, HDMI_DEV_SHOW, &operation, "SHOW");
            if (result == TEEC_SUCCESS) {
                ++counts->accepted_submissions;
                pending_rgb ^= UINT32_C(0xffffff);
            } else if (result == TEEC_ERROR_BUSY) {
                ++counts->show_busy;
            } else {
                return result;
            }
        }
        /* 让出 CPU 后继续查询显示进度。 */
        struct timespec pause = { .tv_sec = 0, .tv_nsec = 1000000L };
        while (!stop_requested && nanosleep(&pause, &pause) != 0) {
            if (errno != EINTR) {
                perror("连续提交调用间隔");
                return TEEC_ERROR_GENERIC;
            }
        }
    }
    if (!stop_requested && counts->accepted_submissions < 2) {
        fprintf(stderr, "运行区间内没有第二次被接受的提交\n");
        return TEEC_ERROR_BUSY;
    }
    return TEEC_SUCCESS;
}

static TEEC_Result cycle(TEEC_Context *context, uint32_t pool_pa, uint32_t pool_bytes,
                         uint32_t pattern, uint32_t rgb, uint32_t hold_ms, int streaming)
{
    struct stream_counts counts = {0};
    TEEC_Session session;
    TEEC_Operation operation = {0};
    struct hdmi_dev_status status;
    TEEC_Result result = open_session(context, &session);
    TEEC_Result stop_result;
    if (result != TEEC_SUCCESS)
        return result;
    if ((active_mode ? hdmi_dev_init_mode_operation(&operation,active_mode) :
        hdmi_dev_init_operation(&operation, pool_pa, pool_bytes)) != 0) {
        TEEC_CloseSession(&session);
        return TEEC_ERROR_BAD_PARAMETERS;
    }
    printf("INIT pool_pa=0x%08" PRIx32 " pool_bytes=%" PRIu32 "\n",
           pool_pa, pool_bytes);
    result = invoke(&session, active_mode ? HDMI_DEV_INIT_MODE : HDMI_DEV_INIT,
                    &operation, active_mode ? "INIT_MODE" : "INIT");
    if (result != TEEC_SUCCESS || stop_requested)
        goto finish;
    if (hdmi_dev_show_operation(&operation, pattern, rgb) != 0) {
        result = TEEC_ERROR_BAD_PARAMETERS;
        goto finish;
    }
    result = invoke(&session, HDMI_DEV_SHOW, &operation, "SHOW");
    if (result != TEEC_SUCCESS)
        goto finish;
    if (streaming)
        ++counts.accepted_submissions;
    result = read_status(&session, &status);
    if (result != TEEC_SUCCESS)
        goto finish;
    if (status.state != HDMI_DEV_DISPLAYING) {
        result = TEEC_ERROR_BAD_STATE;
        fprintf(stderr, "SHOW 后 TA 未进入 DISPLAYING\n");
        goto finish;
    }
    puts("TA 已接受 RGB24 测试图；实际像素与刷新率待验");
    if (streaming) {
        result = stream_updates(&session, hold_ms, &counts);
        if (result == TEEC_SUCCESS && !stop_requested) {
            result = read_status(&session, &status);
            if (result == TEEC_SUCCESS && status.state != HDMI_DEV_DISPLAYING)
                result = TEEC_ERROR_BAD_STATE;
        }
        goto finish;
    }
    struct timespec remaining = { .tv_sec = hold_ms / 1000,
                                 .tv_nsec = (long)(hold_ms % 1000) * 1000000L };
    while (!stop_requested && nanosleep(&remaining, &remaining) != 0) {
        if (errno != EINTR) {
            perror("保持测试图的停留时间");
            result = TEEC_ERROR_GENERIC;
            break;
        }
    }
finish:
    if (streaming)
        printf("stream accepted_submissions=%" PRIu64 " service_calls=%" PRIu64
               " service_busy=%" PRIu64 " show_busy=%" PRIu64 "；实际显示帧数及帧率未验\n",
               counts.accepted_submissions, counts.service_calls, counts.service_busy, counts.show_busy);
    /* INIT 可能在配置中途失败，随后调用 STOP 清理。 */
    stop_result = stop(&session);
    if (stop_result != TEEC_SUCCESS) {
        fprintf(stderr, "STOP 失败: 0x%08" PRIx32
                "；持有程序继续保留物理池，完成 STOP 后再回收\n", stop_result);
        if (result == TEEC_SUCCESS)
            result = stop_result;
    }
    TEEC_CloseSession(&session);
    puts("CLOSE 已关闭会话");
    if (result == TEEC_SUCCESS && stop_requested)
        result = TEEC_ERROR_CANCEL;
    return result;
}

static int number(const char *text, unsigned base, uint32_t maximum, uint32_t *value)
{
    char *end;
    if (!text[0])
        return -1;
    for (const char *p = text; *p; ++p) {
        unsigned digit;
        if (*p >= '0' && *p <= '9') digit = (unsigned)(*p - '0');
        else if (*p >= 'a' && *p <= 'f') digit = (unsigned)(*p - 'a') + 10;
        else if (*p >= 'A' && *p <= 'F') digit = (unsigned)(*p - 'A') + 10;
        else return -1;
        if (digit >= base) return -1;
    }
    errno = 0;
    unsigned long parsed = strtoul(text, &end, base);
    if (errno || *end || parsed > maximum)
        return -1;
    *value = (uint32_t)parsed;
    return 0;
}

static void usage(const char *program)
{
    fprintf(stderr, "用法: %s status\n"
            "      %s run POOL_PA_HEX POOL_BYTES_DEC [coordinate|RRGGBB] [hold-ms] [cycles]\n"
            "      %s stream POOL_PA_HEX POOL_BYTES_DEC [duration-ms]\n"
            "池地址按十六进制(可带 0x)，长度固定为十进制 8388608；独立持有程序须已保留该池\n"
            "默认坐标图、停留 1000ms、1 次完整会话；cycles 范围 1..100\n"
            "stream 默认 1000ms，范围 1..60000ms；红色与青色交替请求，SERVICE 推进硬件处理\n"
            "运行前用 pool-holder 分配并导入显示池\n",
            program, program, program);
}

int main(int argc, char **argv)
{
    uint32_t pattern = HDMI_DEV_PATTERN_COORDINATE, rgb = 0, hold_ms = 1000, cycles = 1;
    uint32_t pool_pa = 0, pool_bytes = 0;
    struct hdmi_dev_mode_request mode={0};
    int mode_run=argc>=2 && !strcmp(argv[1],"mode-run");
    int status_only = argc == 2 && !strcmp(argv[1], "status");
    int streaming = argc >= 2 && !strcmp(argv[1], "stream");
    if(mode_run) {
        if(argc<3 || argc>4)goto bad_arguments;
        FILE *f=fopen(argv[2],"rb");if(!f)goto bad_arguments;
        size_t n=fread(&mode,1,sizeof(mode),f);int extra=fgetc(f);
        int io_error=ferror(f);fclose(f);
        if(n!=sizeof(mode) || extra!=EOF || io_error ||
            hdmi_dev_init_mode_operation(&(TEEC_Operation){0},&mode))goto bad_arguments;
        active_mode=&mode;pool_pa=mode.pool_pa;pool_bytes=mode.pool_bytes;
        if(argc>3 && number(argv[3],10,60000,&hold_ms))goto bad_arguments;
    } else if (!status_only) {
        if (argc < 4 || argc > (streaming ? 5 : 7) ||
            (!streaming && strcmp(argv[1], "run")))
            goto bad_arguments;
        const char *address = argv[2];
        if (address[0] == '0' && (address[1] == 'x' || address[1] == 'X'))
            address += 2;
        if (number(address, 16, UINT32_MAX, &pool_pa) ||
            number(argv[3], 10, UINT32_MAX, &pool_bytes) ||
            !hdmi_dev_pool_valid(pool_pa, pool_bytes))
            goto bad_arguments;
        if (streaming) {
            if (argc > 4 && (number(argv[4], 10, 60000, &hold_ms) || !hold_ms))
                goto bad_arguments;
            pattern = HDMI_DEV_PATTERN_SOLID;
            rgb = UINT32_C(0xff0000);
        } else {
            if (argc > 4 && strcmp(argv[4], "coordinate")) {
                if (strlen(argv[4]) != 6 || number(argv[4], 16, 0xffffffu, &rgb))
                    goto bad_arguments;
                pattern = HDMI_DEV_PATTERN_SOLID;
            }
            if (argc > 5 && number(argv[5], 10, 60000, &hold_ms))
                goto bad_arguments;
            if (argc > 6 && (number(argv[6], 10, 100, &cycles) || !cycles))
                goto bad_arguments;
        }
    }
    struct sigaction action = { .sa_handler = request_stop };
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGINT, &action, NULL) || sigaction(SIGTERM, &action, NULL)) {
        perror("设置信号处理");
        return 1;
    }
    TEEC_Context context;
    TEEC_Result result = TEEC_InitializeContext(NULL, &context);
    if (result != TEEC_SUCCESS) {
        fprintf(stderr, "TEEC_InitializeContext result=0x%08" PRIx32 "\n", result);
        return 1;
    }
    if (status_only) {
        TEEC_Session session;
        struct hdmi_dev_status status;
        result = open_session(&context, &session);
        if (result == TEEC_SUCCESS) {
            result = read_status(&session, &status);
            TEEC_CloseSession(&session);
        }
    } else {
        for (uint32_t i = 0; i < cycles && !stop_requested; ++i) {
            printf("会话 %" PRIu32 "/%" PRIu32 "\n", i + 1, cycles);
            result = cycle(&context, pool_pa, pool_bytes, pattern, rgb, hold_ms, streaming);
            if (result != TEEC_SUCCESS)
                break;
        }
    }
    TEEC_FinalizeContext(&context);
    return result == TEEC_SUCCESS && !stop_requested ? 0 : 1;
bad_arguments:
    fprintf(stderr,"mode-run REQUEST.bin [hold_ms] (explicit timing/capabilities/pool)\n");
    usage(argv[0]);
    return 2;
}
