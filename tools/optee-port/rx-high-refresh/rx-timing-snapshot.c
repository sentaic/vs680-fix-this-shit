/* SPDX-License-Identifier: GPL-2.0 */
#include "rx-timing-snapshot.h"
#include "rx-sync-measurement.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

struct field { const char *name; size_t offset; unsigned decimal; };
#define FIELD(name, member, decimal) {name, offsetof(struct rx_timing_snapshot, member), decimal}
static const struct field fields[] = {
    FIELD("lock_before", lock_before, 0), FIELD("lock_after", lock_after, 0),
    FIELD("deframer", deframer, 0), FIELD("video", video, 0),
    FIELD("hactive", width, 1), FIELD("htotal", htotal, 1),
    FIELD("hfront", hfront, 1), FIELD("hsync_raw", hsync_raw, 1),
    FIELD("hback", hback, 1), FIELD("vactive", height, 1),
    FIELD("vtotal", vtotal, 1), FIELD("vfront", vfront, 1),
    FIELD("vsync_raw", vsync_raw, 1), FIELD("vback", vback, 1),
    FIELD("vmon7", vmon7, 0), FIELD("avi_header", avi_header, 0),
    FIELD("avi_pb", avi_payload, 2), FIELD("gcp_header", gcp_header, 0),
    FIELD("gcp_pb0", gcp_pb0, 0), FIELD("avmute", avmute, 0)
};
#undef FIELD
#define FIELD_COUNT (sizeof(fields) / sizeof(fields[0]))

static int digit(unsigned char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
static int number(const char *text, size_t length, unsigned decimal, uint32_t *out)
{
    unsigned base = 10;
    uint32_t value = 0;
    if (!length) return -EINVAL;
    if (!decimal && length >= 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        base = 16; text += 2; length -= 2;
    }
    if (!length) return -EINVAL;
    for (size_t i = 0; i < length; i++) {
        int n = digit((unsigned char)text[i]);
        if (n < 0 || (unsigned)n >= base) return -EINVAL;
        if (value > (UINT32_MAX - (unsigned)n) / base) return -ERANGE;
        value = value * base + (unsigned)n;
    }
    *out = value;
    return 0;
}
static int payload(const char *text, size_t length, uint32_t out[4])
{
    if (length != 35) return -EPROTO;
    for (unsigned word = 0; word < 4; word++) {
        uint32_t value = 0;
        for (unsigned i = 0; i < 8; i++) {
            int n = digit((unsigned char)text[word * 9 + i]);
            if (n < 0) return -EINVAL;
            value = value * 16 + (unsigned)n;
        }
        if (word < 3 && text[word * 9 + 8] != ',') return -EPROTO;
        out[word] = value;
    }
    /* The register exposes padding beyond the 14 received AVI bytes. */
    out[3] &= 0xffff;
    return 0;
}
static int white(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }
static int validate(struct rx_timing_snapshot *s,
                    const struct rx_input_clock_measurement *clock, int transport)
{
    if (!(s->lock_before & 2u) || !(s->lock_after & 2u)) return -ENOLCK;
    if (s->lock_before != s->lock_after) return -EAGAIN;
    if ((s->deframer & 0x70u) != 0x10u || (s->deframer & ~0x71u))
        return -EOPNOTSUPP;
    /* hrx-v4l2 hdmirx.h: CD_CURRENT[3:0], PP_CURRENT[6:4]. */
    if ((s->video & 15u) != 4u || (s->video & 0x70u) || (s->video & ~0xff7fu))
        return -EOPNOTSUPP;
    /* 当前解析器接收 VMON7 为零的逐行输入。 */
    if (s->vmon7) return -EOPNOTSUPP;
    if (s->avmute != 0 && s->avmute != 0x10u) return -EOPNOTSUPP;
    s->avi_header &= 0xffff;
    if ((s->avi_header & 255u) != 2u) return -EOPNOTSUPP;
    if ((s->avi_header >> 8) != 13u) return -EPROTO;
    unsigned sum = 0x82u + 2u + 13u;
    for (unsigned i = 0; i < 14; i++)
        sum += (s->avi_payload[i / 4] >> ((i % 4) * 8)) & 255u;
    if (sum & 255u) return -EBADMSG;
    /* PB1 Y[6:5] and PB5 pixel repetition[3:0]; PB0 is the checksum. */
    unsigned encoding = (s->avi_payload[0] >> 13) & 7u;
    if ((encoding && !(transport && encoding == 3u)) || (s->avi_payload[1] & (15u << 8)))
        return -EOPNOTSUPP;
    if (!s->width || !s->height || s->width > 65535u || s->height > 65535u ||
        s->htotal > 65535u || s->vtotal > 65535u ||
        s->width >= s->htotal || s->height >= s->vtotal)
        return -EINVAL;
    struct rx_sync_measurement hsync = {0}, vsync = {0};
    int hr = rx_sync_measurement_normalize(s->width, s->htotal,
        s->hfront, s->hsync_raw, s->hback, &hsync);
    int vr = rx_sync_measurement_normalize(s->height, s->vtotal,
        s->vfront, s->vsync_raw, s->vback, &vsync);
    if (hr == RX_SYNC_MEASUREMENT_INVALID || vr == RX_SYNC_MEASUREMENT_INVALID)
        return -EINVAL;
    if (hr == RX_SYNC_MEASUREMENT_UNNORMALIZED || vr == RX_SYNC_MEASUREMENT_UNNORMALIZED)
        return -EOPNOTSUPP;
    s->hsync = hsync.pulse_width; s->vsync = vsync.pulse_width;
    s->hsync_positive = hsync.positive; s->vsync_positive = vsync.positive;
    s->progressive = 1;
    s->rgb24_bytes = (uint64_t)s->width * s->height * 3u;
    if (clock) {
        if (clock->source == RX_INPUT_CLOCK_UNKNOWN) {
            if (clock->pixel_clock_hz) return -EINVAL;
        } else if (clock->source == RX_INPUT_CLOCK_MEASURED_INPUT_PIXEL_CLOCK) {
            if (!clock->pixel_clock_hz || clock->pixel_clock_hz > UINT64_MAX / 1000u)
                return -ERANGE;
            s->clock_known = 1;
            s->pixel_clock_hz = clock->pixel_clock_hz;
            s->refresh_millihz = clock->pixel_clock_hz * 1000u /
                                ((uint64_t)s->htotal * s->vtotal);
        } else return -EOPNOTSUPP;
    }
    return 0;
}
static int parse(const char *text, size_t length,
    const struct rx_input_clock_measurement *clock, struct rx_timing_snapshot *out, int transport)
{
    struct rx_timing_snapshot s = {0};
    uint32_t seen = 0;
    size_t position = 0;
    if (!text || !out || !length) return -EINVAL;
    if (length > RX_TIMING_MAX_TEXT) return -EOVERFLOW;
    if (memchr(text, 0, length)) return -EINVAL;
    while (position < length) {
        while (position < length && white(text[position])) position++;
        if (position == length) break;
        size_t begin = position;
        while (position < length && !white(text[position])) position++;
        size_t end = position, equal = begin;
        while (equal < end && text[equal] != '=') equal++;
        if (equal == begin || equal == end) return -EINVAL;
        unsigned field;
        for (field = 0; field < FIELD_COUNT; field++)
            if (strlen(fields[field].name) == equal - begin &&
                !memcmp(text + begin, fields[field].name, equal - begin)) break;
        if (field == FIELD_COUNT) return -EOPNOTSUPP;
        if (seen & (1u << field)) return -EINVAL;
        seen |= 1u << field;
        uint32_t *destination = (uint32_t *)((unsigned char *)&s + fields[field].offset);
        int result = fields[field].decimal == 2 ?
            payload(text + equal + 1, end - equal - 1, destination) :
            number(text + equal + 1, end - equal - 1, fields[field].decimal, destination);
        if (result) return result;
    }
    if (seen != (1u << FIELD_COUNT) - 1u) return -EPROTO;
    int result = validate(&s, clock, transport);
    if (result) return result;
    *out = s;
    return 0;
}
int rx_timing_snapshot_parse(const char *text, size_t length,
    const struct rx_input_clock_measurement *clock, struct rx_timing_snapshot *out)
{
    return parse(text, length, clock, out, 0);
}
static int read_input(const char *path,
    const struct rx_input_clock_measurement *clock, struct rx_timing_snapshot *out, int transport)
{
    char text[RX_TIMING_MAX_TEXT + 1];
    if (!out) return -EINVAL;
    FILE *file = fopen(path ? path : RX_TIMING_INPUT_PATH, "r");
    if (!file) return -errno;
    size_t length = fread(text, 1, sizeof(text), file);
    int result = ferror(file) ? -(errno ? errno : EIO) : 0;
    if (fclose(file) && !result) result = -errno;
    if (result) return result;
    return parse(text, length, clock, out, transport);
}
int rx_timing_snapshot_read(const char *path,
    const struct rx_input_clock_measurement *clock, struct rx_timing_snapshot *out)
{
    return read_input(path, clock, out, 0);
}
int rx_timing_snapshot_read_transport(const char *path, struct rx_timing_snapshot *out)
{
    return read_input(path, NULL, out, 1);
}
int rx_timing_snapshot_same_input(const struct rx_timing_snapshot *a,
    const struct rx_timing_snapshot *b)
{
    if (!a || !b) return -EINVAL;
#define SAME(member) if (a->member != b->member) return 0
    SAME(lock_before); SAME(lock_after); SAME(deframer); SAME(video);
    SAME(width); SAME(height); SAME(htotal); SAME(vtotal);
    SAME(hfront); SAME(hsync_raw); SAME(hback); SAME(vfront); SAME(vsync_raw); SAME(vback);
    SAME(vmon7); SAME(avi_header); SAME(gcp_header); SAME(gcp_pb0); SAME(avmute);
    SAME(hsync); SAME(vsync); SAME(hsync_positive); SAME(vsync_positive); SAME(progressive);
    SAME(clock_known); SAME(pixel_clock_hz); SAME(rgb24_bytes); SAME(refresh_millihz);
    for (unsigned i = 0; i < 4; i++) SAME(avi_payload[i]);
#undef SAME
    return 1;
}
