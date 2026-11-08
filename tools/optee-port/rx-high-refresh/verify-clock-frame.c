/* SPDX-License-Identifier: GPL-2.0-only */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <tee_client_api.h>
#include "rx-capture-config.h"
#include "rx-timing-snapshot.h"
#include "clock-frame-420.h"

struct heap_allocation { uint64_t length; uint32_t fd, flags; uint64_t heap_flags; };
struct pinned_buffer { uint32_t fd, reserved; uint64_t address, length; };
static const TEEC_UUID uuid = {0x0b564e41,0x93da,0x4fd8,{0xb2,0xf9,0x40,0x98,0x56,0x72,0xd1,0x28}};
static unsigned clock_cell = 8;

static uint64_t monotonic_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint64_t)t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

static TEEC_Result command(TEEC_Session *session, unsigned number, unsigned a,
                           unsigned b, unsigned *busy, unsigned *full)
{
    TEEC_Operation op = {0};
    uint32_t origin = 0;
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
                                    TEEC_VALUE_OUTPUT, TEEC_NONE);
    op.params[0].value.a = a;
    op.params[0].value.b = b;
    TEEC_Result result = TEEC_InvokeCommand(session, number, &op, &origin);
    *busy = op.params[1].value.a;
    *full = op.params[1].value.b;
    if (number != 0x7f10 || result || (*full && !*busy)) {
        printf("command=%#x result=%#x origin=%u busy=%#x full=%#x\n",
               number, result, origin, *busy, *full);
        fflush(stdout);
    }
    return result;
}

static uint32_t crc32(uint32_t value)
{
    uint32_t crc = UINT32_MAX;
    for (unsigned byte = 0; byte < 4; ++byte) {
        crc ^= value & 255;
        value >>= 8;
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    return ~crc;
}

static int verify_pixels(const uint8_t *pixels, const struct rx_capture_layout *layout)
{
    unsigned valid = 0, invalid = 0, different = 0;
    uint32_t first = 0;
    /* 测试源重复绘制 12x12 格时间码，红蓝分量之和用于判定码点。 */
    unsigned tile = clock_cell * 12;
    for (unsigned top = 0; top + tile <= layout->height; top += tile) {
        for (unsigned left = 0; left + tile <= layout->width; left += tile) {
            uint64_t word = 0;
            int border_ok = 1;
            for (unsigned y = 0; y < 12; ++y) {
                for (unsigned x = 0; x < 12; ++x) {
                    unsigned at = (top + y * clock_cell + clock_cell / 2) * layout->stride +
                                  (left + x * clock_cell + clock_cell / 2) * 3;
                    int high = pixels[at] + pixels[at + 2] > 250;
                    if (!x || !y || x == 11 || y == 11) border_ok &= high;
                    else if (x == 1 || y == 1 || x == 10 || y == 10) border_ok &= !high;
                    else if (high) word |= UINT64_C(1) << ((y - 2) * 8 + x - 2);
                }
            }
            word ^= UINT64_C(0xa6d39b704c28e15f);
            uint32_t stamp = (uint32_t)word;
            if (!border_ok || (uint32_t)(word >> 32) != crc32(stamp)) {
                ++invalid;
                continue;
            }
            if (!valid) first = stamp;
            else different += stamp != first;
            ++valid;
        }
    }
    unsigned levels[256] = {0}, colors = 0, rgb_errors = 0, bgr_errors = 0;
    uint64_t pattern = (((uint64_t)crc32(first) << 32) | first) ^ UINT64_C(0xa6d39b704c28e15f);
    for (unsigned y = 0; y < layout->height; ++y) {
        for (unsigned x = 0; x < layout->width; ++x) {
            unsigned tx = (x / clock_cell) % 12, ty = (y / clock_cell) % 12;
            int high;
            if (!tx || !ty || tx == 11 || ty == 11) high = 1;
            else if (tx == 1 || ty == 1 || tx == 10 || ty == 10) high = 0;
            else high = (int)((pattern >> ((ty - 2) * 8 + tx - 2)) & 1);
            const uint8_t expected[3] = {high ? 240 : 16, high ? 142 : 114, high ? 255 : 0};
            const uint8_t *p = pixels + y * layout->stride + x * 3;
            rgb_errors += p[0] != expected[0] || p[1] != expected[1] || p[2] != expected[2];
            bgr_errors += p[0] != expected[2] || p[1] != expected[1] || p[2] != expected[0];
            levels[p[1]]++;
        }
    }
    for (unsigned i = 0; i < 256; ++i) colors += levels[i] != 0;
    printf("clock_symbols_valid=%u invalid=%u differing_timestamps=%u first_timestamp=%u green_levels=%u rgb_pixel_errors=%u bgr_pixel_errors=%u first24=",
           valid, invalid, different, first, colors, rgb_errors, bgr_errors);
    for (unsigned i = 0; i < 24; ++i) printf("%02x", pixels[i]);
    puts("");
    return valid && !invalid && !different && (!rgb_errors || !bgr_errors);
}

int main(int argc, char **argv)
{
    const char *heap = argc > 1 ? argv[1] : "/dev/dma_heap/reserved";
    unsigned packed = argc > 2 ? (unsigned)strtoul(argv[2], NULL, 0) : 0;
    clock_cell = argc > 3 ? (unsigned)strtoul(argv[3], NULL, 0) : 8;
    TEEC_Context context;
    TEEC_Session session;
    uint32_t origin = 0;
    unsigned busy = 0, full = 0;
    int result = 1, session_open = 0, holder = -1, allocation_fd = -1;
    int pinned = 0, started = 0;
    uint8_t *pixels = MAP_FAILED;
    struct rx_timing_snapshot input;
    struct rx_capture_layout layout;
    struct rx_capture_config config = {0};
    if (rx_timing_snapshot_read_transport(NULL, &input)) {
        fputs("Stable receiver timing is unavailable\n", stderr);
        return 1;
    }
    unsigned encoding = (input.avi_payload[0] >> 13) & 7u;
    unsigned source_width = input.width * (encoding == 3u ? 2u : 1u);
    if (!clock_cell || clock_cell > source_width / 12 || clock_cell > input.height / 12 ||
        (encoding == 3u && (clock_cell < 2u || (clock_cell & 1u)))) {
        fputs("Clock symbol does not fit the source frame\n", stderr);
        return 1;
    }
    config.version = RX_CAPTURE_CONFIG_VERSION;
    config.size = sizeof(config);
    config.width = input.width; config.height = input.height;
    config.stride = input.width * 3; config.format = RX_CAPTURE_FORMAT_RGB888;
    if (encoding == 3u)
        config.format = RX_CAPTURE_FORMAT_HDMI420_8;
    config.video = input.video; config.vmon7 = input.vmon7;
    config.hfront = input.hfront; config.hsync_raw = input.hsync_raw;
    config.hback = input.hback; config.htotal = input.htotal;
    config.vfront = input.vfront; config.vsync_raw = input.vsync_raw;
    config.vback = input.vback; config.vtotal = input.vtotal;
    config.avi_header = input.avi_header;
    memcpy(config.avi_payload, input.avi_payload, sizeof(config.avi_payload));
    config.gcp_header = input.gcp_header; config.gcp_payload = input.gcp_pb0;
    config.avmute = input.avmute;
    if (rx_capture_layout_validate(&config, &layout)) return 1;
    if (TEEC_InitializeContext(NULL, &context)) return 1;
    TEEC_Result code = TEEC_OpenSession(&context, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
    printf("open=%#x origin=%u dimensions=%ux%u\n", code, origin, layout.width, layout.height);
    if (code) goto done;
    session_open = 1;
    TEEC_Operation op = {0};
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = &config;
    op.params[0].tmpref.size = sizeof(config);
    code = TEEC_InvokeCommand(&session, RX_CAPTURE_CONFIG_COMMAND, &op, &origin);
    printf("configure=%#x marker=%#x\n", code, config.reserved[0]);
    if (code || config.reserved[0] != 0x52584331) goto done;
    if (command(&session, 0x7f1e, packed, 0, &busy, &full) ||
        command(&session, 0x7f22, 1, 0, &busy, &full) ||
        command(&session, 0x7f2f, 1, 0, &busy, &full) ||
        command(&session, 0x7f28, 0, 0, &busy, &full)) goto done;
    int fd = open(heap, O_RDWR | O_CLOEXEC);
    if (fd < 0) { perror(heap); goto done; }
    struct heap_allocation allocation = {.length = layout.allocation_bytes, .flags = O_RDWR | O_CLOEXEC};
    int allocated = ioctl(fd, _IOWR('H', 0, struct heap_allocation), &allocation);
    close(fd);
    if (allocated) { perror("DMA heap allocation"); goto done; }
    allocation_fd = allocation.fd;
    holder = open("/dev/vs680-capture-buffer", O_RDWR | O_CLOEXEC);
    struct pinned_buffer region = {.fd = allocation.fd};
    if (holder < 0 || ioctl(holder, _IOWR('B', 0, struct pinned_buffer), &region)) {
        perror("DMA buffer import"); goto done;
    }
    pinned = 1;
    printf("buffer_address=%#llx length=%llu heap=%s\n",
           (unsigned long long)region.address, (unsigned long long)region.length, heap);
    if (rx_capture_buffer_validate(&layout, region.address, region.length)) goto done;
    pixels = mmap(NULL, layout.allocation_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, allocation_fd, 0);
    if (pixels == MAP_FAILED) { perror("DMA mmap"); goto done; }
    uint64_t sync = 2;
    if (ioctl(allocation_fd, _IOW('b', 0, uint64_t), &sync)) goto done;
    memset(pixels, 0xa5, layout.allocation_bytes);
    sync = 6;
    if (ioctl(allocation_fd, _IOW('b', 0, uint64_t), &sync)) goto done;
    code = command(&session, 0x7f11, region.address, region.length, &busy, &full);
    started = code != TEEC_ERROR_BUSY;
    if (code) goto done;
    if (command(&session, 0x7f2e, 0, 0, &busy, &full) ||
        command(&session, 0x7f26, 0, 0, &busy, &full)) goto done;
    uint64_t deadline = monotonic_ms() + 3000;
    do {
        code = command(&session, 0x7f10, 0, 0, &busy, &full);
        if (code || (full && !busy)) break;
    } while (monotonic_ms() < deadline);
    if (code || !full || busy) goto done;
    if (command(&session, 0x7f12, 0, 0, &busy, &full) || busy) goto done;
    started = 0;
    sync = 1;
    if (ioctl(allocation_fd, _IOW('b', 0, uint64_t), &sync)) goto done;
    if (config.format == RX_CAPTURE_FORMAT_HDMI420_8) {
        struct clock_frame_420_result checked = {0};
        result = clock_frame_420_check(pixels, layout.width, layout.height,
                                      layout.stride, clock_cell, &checked) ? 0 : 1;
        printf("wire_encoding=3 logical_width=%u logical_height=%u clock_symbols_valid=%u invalid=%u differing_timestamps=%u first_timestamp=%u",
            layout.width * 2u, layout.height, checked.valid, checked.invalid,
            checked.different, checked.timestamp);
        for (unsigned plane = 0; plane < 3; plane++) {
            printf(" plane%u_samples=%u plane%u_errors=%u first_error%u=%u,%u,%u,%u",
                plane, checked.samples[plane], plane, checked.errors[plane], plane,
                checked.first_error[plane][0], checked.first_error[plane][1],
                checked.first_error[plane][2], checked.first_error[plane][3]);
        }
        puts("");
    } else {
        result = verify_pixels(pixels, &layout) ? 0 : 1;
        printf("wire_encoding=0 logical_width=%u logical_height=%u\n", layout.width, layout.height);
    }
    sync = 5;
    if (ioctl(allocation_fd, _IOW('b', 0, uint64_t), &sync)) result = 1;
done:
    if (started && (command(&session, 0x7f12, 0, 0, &busy, &full) || busy)) {
        fputs("DMA stop failed; imported allocation remains held by the kernel\n", stderr);
        pinned = 0;
        result = 1;
    }
    if (pixels != MAP_FAILED) munmap(pixels, layout.allocation_bytes);
    if (pinned && ioctl(holder, _IO('B', 1))) { perror("DMA buffer release"); result = 1; }
    if (holder >= 0) close(holder);
    if (allocation_fd >= 0) close(allocation_fd);
    if (session_open) TEEC_CloseSession(&session);
    TEEC_FinalizeContext(&context);
    return result;
}
