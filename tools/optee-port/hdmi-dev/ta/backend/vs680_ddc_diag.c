/* SPDX-License-Identifier: GPL-2.0-only */
#include "vs680_ddc_diag.h"
#include <string.h>

#define VD_DONE 2u
#define VD_ERROR 1u
#define VD_TERMINAL (VD_DONE | VD_ERROR)
#define VD_HPD 2u
#define VD_FAST 8u
#define VD_AUTO_MASK 0x31u

const uint32_t vd_snapshot_offsets[VD_SNAPSHOT_COUNT] = {
    VD_DIV, VD_SS_HIGH1, VD_SS_HIGH0, VD_SS_LOW1, VD_SS_LOW0,
    VD_FS_HIGH1, VD_FS_HIGH0, VD_FS_LOW1, VD_FS_LOW0, VD_SDA_HOLD,
    VD_INT, VD_CTLINT, VD_IH_MUTE, VD_SCDC_UPDATE, VD_SOFTRSTZ,
    VD_OPERATION, VD_IH_STATUS, VD_PHY_STATUS,
    VD_SLAVE, VD_ADDRESS, VD_SEGADDR, VD_SEGPTR
};
const uint8_t vd_scdc_addresses[VD_SCDC_READS] = { 0x01, 0x20, 0x21, 0x40, 0x41 };

struct run {
    const struct vd_ops *o;
    const struct vd_limits *l;
    struct vd_report *r;
    uint64_t start, previous, deadline;
    int selectors_dirty, fatal;
};

static int rd(struct run *s, uint32_t reg, uint32_t *v)
{
    s->r->last_mmio_offset = reg;
    if (s->o->read32(s->o->ctx, reg, v)) {
        s->fatal = 1;
        if (s->selectors_dirty || s->r->operation_may_be_active) s->r->recovery_required = 1;
        return VD_MMIO_ERROR;
    }
    return VD_OK;
}
static int wr(struct run *s, uint32_t reg, uint32_t v)
{
    s->r->last_mmio_offset = reg;
    if (s->o->write32(s->o->ctx, reg, v)) {
        /* 在调用写回调之前记录写入已尝试，供错误清理使用。 */
        s->r->recovery_required = 1;
        s->fatal = 1;
        return VD_MMIO_ERROR;
    }
    return VD_OK;
}
static int tick(struct run *s, uint64_t *v)
{
    if (s->o->now_us(s->o->ctx, v) || *v < s->previous) {
        s->fatal = 1;
        return VD_CLOCK_ERROR;
    }
    s->previous = *v;
    s->r->elapsed_us = *v - s->start;
    return VD_OK;
}
static int available(struct run *s)
{
    uint64_t now;
    int rc = tick(s, &now);
    if (rc) return rc;
    if (now >= s->deadline) { s->fatal = 1; return VD_TOTAL_TIMEOUT; }
    return VD_OK;
}
static uint16_t pair(const struct vd_snapshot *p, unsigned hi, unsigned lo)
{
    return (uint16_t)(((p->raw[hi] & 0xffu) << 8) | (p->raw[lo] & 0xffu));
}
static int snapshot(struct run *s)
{
    struct vd_snapshot *p = &s->r->before;
    unsigned i;
    int rc;
    for (i = 0; i < VD_SNAPSHOT_COUNT; ++i) {
        if ((rc = available(s)) != VD_OK) return rc;
        if ((rc = rd(s, vd_snapshot_offsets[i], &p->raw[i])) != VD_OK) return rc;
        p->valid_mask |= (uint32_t)1u << i;
    }
    p->ss_high = pair(p, VD_S_SS_HIGH1, VD_S_SS_HIGH0);
    p->ss_low = pair(p, VD_S_SS_LOW1, VD_S_SS_LOW0);
    p->fs_high = pair(p, VD_S_FS_HIGH1, VD_S_FS_HIGH0);
    p->fs_low = pair(p, VD_S_FS_LOW1, VD_S_FS_LOW0);
    p->sda_hold = (uint8_t)p->raw[VD_S_SDA_HOLD];
    p->fast_mode = !!(p->raw[VD_S_DIV] & VD_FAST);
    if (!p->sda_hold || (p->fast_mode ? (!p->fs_high || !p->fs_low)
                                                    : (!p->ss_high || !p->ss_low)))
        return VD_TIMING_UNREADY;
    p->nonzero_sanity_only = 1;
    if ((p->raw[VD_S_INT] & 0x04u) || (p->raw[VD_S_CTLINT] & 0x44u))
        return VD_IRQ_GATED;
    if (p->raw[VD_S_SCDC_UPDATE] & VD_AUTO_MASK) return VD_AUTO_DDC_ACTIVE;
    if (!(p->raw[VD_S_PHY_STATUS] & VD_HPD)) return VD_HPD_LOW;
    return VD_OK;
}
static int clear_initial_status(struct run *s)
{
    uint32_t observed = s->r->before.raw[VD_S_IH_STATUS] & VD_TERMINAL;
    uint32_t v;
    int rc;
    /* 清除旧的 DONE/ERROR 事件，保留 SCDC_READREQ；W1C 位直接写入掩码。 */
    if (observed) {
        if ((rc = wr(s, VD_IH_STATUS, observed)) != VD_OK) return rc;
        s->r->initial_status_cleared = (uint8_t)observed;
    }
    if ((rc = rd(s, VD_IH_STATUS, &v)) != VD_OK) return rc;
    s->r->last_status = (uint8_t)v;
    if (v & VD_TERMINAL) {
        s->r->recovery_required = 1;
        s->fatal = 1;
        return VD_STALE_STATUS;
    }
    return VD_OK;
}

/* 发起一次读取。 */
static int read_sink(struct run *s, uint8_t slave, uint8_t segment,
                     uint8_t address, unsigned count, uint8_t *data)
{
    uint64_t now, tx_start;
    uint32_t v, op, i, delay;
    int rc;
    if ((rc = available(s)) != VD_OK) return rc;
    if ((rc = rd(s, VD_PHY_STATUS, &v)) != VD_OK) return rc;
    if (!(v & VD_HPD)) { s->fatal = 1; return VD_HPD_LOW; }
    if ((rc = rd(s, VD_IH_STATUS, &v)) != VD_OK) return rc;
    s->r->last_status = (uint8_t)v;
    if (v & VD_TERMINAL) {
        /* A new terminal event between our commands violates quiescence. */
        s->r->recovery_required = 1;
        s->fatal = 1;
        return VD_STALE_STATUS;
    }
    s->r->last_slave = slave;
    s->r->last_segment = segment;
    s->r->last_address = address;
    s->selectors_dirty = 1;
    /* Preserve the reserved bits of SLAVE, as official dev_write_mask does. */
    v = (s->r->before.raw[VD_S_SLAVE] & ~0x7fu) | slave;
    if ((rc = wr(s, VD_SLAVE, v)) != VD_OK) return rc;
    if ((rc = wr(s, VD_ADDRESS, address)) != VD_OK) return rc;
    if ((rc = wr(s, VD_SEGADDR, slave == 0x50 ? 0x30u : 0u)) != VD_OK) return rc;
    if ((rc = wr(s, VD_SEGPTR, segment)) != VD_OK) return rc;
    /* Check total time again: selector callbacks consume time too. */
    if ((rc = available(s)) != VD_OK) return rc;
    tx_start = s->previous;
    op = count == 8u ? (segment ? 8u : 4u) : (segment ? 2u : 1u);
    s->r->operation_may_be_active = 1;
    ++s->r->transactions_started;
    if ((rc = wr(s, VD_OPERATION, op)) != VD_OK) goto uncertain;
    for (i = 0; i < s->l->max_polls_per_transaction; ++i) {
        if ((rc = tick(s, &now)) != VD_OK) goto uncertain;
        /* 超时后也取得最后一次状态，避免漏掉等待期间到来的完成。 */
        if ((rc = rd(s, VD_PHY_STATUS, &v)) != VD_OK) goto uncertain;
        if (!(v & VD_HPD)) { rc = VD_HPD_LOW; goto uncertain; }
        if ((rc = rd(s, VD_IH_STATUS, &v)) != VD_OK) goto uncertain;
        ++s->r->polls;
        s->r->last_status = (uint8_t)v;
        /* MMIO 返回后重新采样时间，再判断截止时间和下一次等待。 */
        if ((rc = tick(s, &now)) != VD_OK) goto uncertain;
        int expired = now >= s->deadline ? VD_TOTAL_TIMEOUT :
            now - tx_start >= s->l->transaction_timeout_us ? VD_TRANSACTION_TIMEOUT : VD_OK;
        if (v & VD_TERMINAL) {
            /* ERROR 和 DONE 同时出现时，按 ERROR 处理。 */
            if ((rc = wr(s, VD_IH_STATUS, v & VD_TERMINAL)) != VD_OK) goto uncertain;
            s->r->operation_may_be_active = 0;
            if (v & VD_ERROR) {
                /* ERROR 后将事务交给恢复流程处理。 */
                s->r->operation_may_be_active = 1;
                rc = VD_I2C_ERROR; goto uncertain;
            }
            if (expired) {
                /* DONE 超时后恢复选择寄存器，并返回超时错误。 */
                s->fatal = 1;
                return expired;
            }
            for (i = 0; i < count; ++i) {
                rc = rd(s, count == 8u ? VD_READ_BUFFER0 + 4u*i : VD_DATAI, &v);
                if (rc) return rc;
                data[i] = (uint8_t)v;
            }
            /* Catch a disconnect during buffer reads before trusting a block. */
            if ((rc = rd(s, VD_PHY_STATUS, &v)) != VD_OK) return rc;
            if (!(v & VD_HPD)) { s->fatal = 1; return VD_HPD_LOW; }
            if ((rc = available(s)) != VD_OK) return rc;
            if (s->previous - tx_start >= s->l->transaction_timeout_us) {
                s->fatal = 1; /* Completed, so restore is safe; stop further reads. */
                return VD_TRANSACTION_TIMEOUT;
            }
            return VD_OK;
        }
        if (expired) { rc = expired; goto uncertain; }
        if (i + 1u == s->l->max_polls_per_transaction) break;
        delay = s->l->poll_interval_us;
        if ((uint64_t)delay > s->deadline - now) delay = (uint32_t)(s->deadline - now);
        if ((uint64_t)delay > s->l->transaction_timeout_us - (now - tx_start))
            delay = (uint32_t)(s->l->transaction_timeout_us - (now - tx_start));
        if (s->o->wait_us(s->o->ctx, delay)) { rc = VD_WAIT_ERROR; goto uncertain; }
    }
    rc = VD_POLL_LIMIT;
uncertain:
    s->r->recovery_required = 1;
    s->fatal = 1;
    return rc;
}

static int read_edid(struct run *s, uint8_t *edid)
{
    static const uint8_t header[8] = { 0, 255, 255, 255, 255, 255, 255, 0 };
    unsigned block, chunk, j, target = 1;
    int rc, semantic = VD_OK;
    for (block = 0; block < target; ++block) {
        uint8_t sum = 0;
        uint8_t *p = edid + (size_t)block * VD_EDID_BLOCK_SIZE;
        for (chunk = 0; chunk < VD_EDID_BLOCK_SIZE; chunk += 8u) {
            rc = read_sink(s, 0x50, (uint8_t)(block / 2u),
                           (uint8_t)((block % 2u)*128u + chunk), 8, p + chunk);
            if (rc) return rc;
            s->r->edid_bytes += 8;
        }
        for (j = 0; j < VD_EDID_BLOCK_SIZE; ++j) sum = (uint8_t)(sum + p[j]);
        s->r->edid_checksum_sum[block] = sum;
        ++s->r->edid_blocks_read;
        if (block == 0) {
            s->r->edid_header_valid = memcmp(p, header, sizeof header) == 0;
            if (!s->r->edid_header_valid) return VD_EDID_HEADER;
            if (sum) return VD_EDID_CHECKSUM; /* 检查 EDID 扩展块数量。 */
            s->r->edid_expected_blocks = (uint16_t)(1u + p[126]);
            target = s->r->edid_expected_blocks;
            if (target > s->l->max_edid_blocks) {
                target = s->l->max_edid_blocks;
                s->r->edid_limited = 1;
            }
        } else if (sum) semantic = VD_EDID_CHECKSUM;
    }
    s->r->edid_complete = s->r->edid_blocks_read == s->r->edid_expected_blocks;
    if (semantic) return semantic;
    if (s->r->edid_limited) return VD_EDID_LIMIT;
    s->r->edid_valid = 1;
    return VD_OK;
}

static int restore_selectors(struct run *s)
{
    static const unsigned fields[4] = { VD_S_SLAVE, VD_S_ADDRESS, VD_S_SEGADDR, VD_S_SEGPTR };
    unsigned i;
    if (!s->selectors_dirty) return VD_OK;
    if (s->r->recovery_required || s->r->operation_may_be_active) return VD_NOT_ATTEMPTED;
    /* 空闲后通过四次写入恢复选择寄存器。 */
    for (i = 0; i < 4; ++i) {
        unsigned f = fields[i];
        if (wr(s, vd_snapshot_offsets[f], s->r->before.raw[f])) return VD_RESTORE_ERROR;
    }
    s->r->selectors_restored = 1;
    return VD_OK;
}

int vd_run(const struct vd_ops *ops, const struct vd_limits *limits,
           uint8_t *edid, size_t edid_capacity, struct vd_report *report)
{
    struct run s;
    uint64_t now;
    unsigned i;
    int rc, result = VD_OK;
    if (!report) return VD_BAD_ARGUMENT;
    memset(report, 0, sizeof *report);
    report->readiness_result = report->edid_result = report->cleanup_result = VD_NOT_ATTEMPTED;
    for (i = 0; i < VD_SCDC_READS; ++i) {
        report->scdc[i].address = vd_scdc_addresses[i];
        report->scdc[i].result = VD_NOT_ATTEMPTED;
    }
    if (!ops || !limits || !edid || !ops->enter || !ops->finish || !ops->read32 ||
        !ops->write32 || !ops->now_us || !ops->wait_us || !limits->total_timeout_us ||
        limits->total_timeout_us > VD_MAX_TOTAL_TIMEOUT_US ||
        !limits->transaction_timeout_us || limits->transaction_timeout_us > VD_MAX_TRANSACTION_TIMEOUT_US ||
        !limits->poll_interval_us || limits->poll_interval_us > limits->transaction_timeout_us ||
        !limits->max_polls_per_transaction ||
        limits->max_polls_per_transaction > VD_MAX_POLLS_PER_TRANSACTION || !limits->max_edid_blocks ||
        limits->max_edid_blocks > VD_EDID_MAX_BLOCKS ||
        edid_capacity / VD_EDID_BLOCK_SIZE < limits->max_edid_blocks) {
        report->result = VD_BAD_ARGUMENT;
        return report->result;
    }
    memset(&s, 0, sizeof s);
    s.o = ops; s.l = limits; s.r = report;
    if (ops->now_us(ops->ctx, &s.start)) {
        report->result = VD_CLOCK_ERROR; return report->result;
    }
    if (UINT64_MAX - s.start < limits->total_timeout_us) {
        report->result = VD_BAD_ARGUMENT; return report->result;
    }
    s.previous = s.start;
    s.deadline = s.start + limits->total_timeout_us;
    if (ops->enter(ops->ctx, s.deadline)) {
        report->result = VD_OWNER_UNAVAILABLE; return report->result;
    }
    rc = snapshot(&s);
    report->readiness_result = rc;
    if (rc) { result = rc; goto finish; }
    if ((rc = clear_initial_status(&s)) != VD_OK) { result = rc; goto finish; }
    report->edid_result = read_edid(&s, edid);
    result = report->edid_result;
    /* EDID 内容校验失败后仍可尝试 SCDC 读取；总线错误则结束读取流程。 */
    for (i = 0; i < VD_SCDC_READS && !s.fatal; ++i) {
        rc = read_sink(&s, 0x54, 0, vd_scdc_addresses[i], 1, &report->scdc[i].value);
        report->scdc[i].result = rc;
        if (!result && rc) result = rc;
        if (rc == VD_TOTAL_TIMEOUT) break;
    }
finish:
    report->cleanup_result = restore_selectors(&s);
    if (report->cleanup_result != VD_OK && report->cleanup_result != VD_NOT_ATTEMPTED && !result)
        result = report->cleanup_result;
    if (tick(&s, &now) != VD_OK && !result) result = VD_CLOCK_ERROR;
    if (ops->finish(ops->ctx, report->recovery_required)) {
        if (!result) result = VD_OWNER_FINISH_ERROR;
        report->cleanup_result = VD_OWNER_FINISH_ERROR;
    }
    report->result = result;
    return result;
}

const char *vd_result_name(int result)
{
    switch (result) {
#define CASE(n) case n: return #n
    CASE(VD_OK); CASE(VD_NOT_ATTEMPTED); CASE(VD_BAD_ARGUMENT);
    CASE(VD_OWNER_UNAVAILABLE); CASE(VD_MMIO_ERROR); CASE(VD_CLOCK_ERROR);
    CASE(VD_WAIT_ERROR); CASE(VD_TIMING_UNREADY); CASE(VD_IRQ_GATED);
    CASE(VD_AUTO_DDC_ACTIVE); CASE(VD_HPD_LOW); CASE(VD_TRANSACTION_TIMEOUT);
    CASE(VD_TOTAL_TIMEOUT); CASE(VD_POLL_LIMIT); CASE(VD_I2C_ERROR);
    CASE(VD_EDID_HEADER); CASE(VD_EDID_CHECKSUM); CASE(VD_EDID_LIMIT);
    CASE(VD_RESTORE_ERROR); CASE(VD_OWNER_FINISH_ERROR); CASE(VD_STALE_STATUS);
#undef CASE
    default: return "VD_UNKNOWN_RESULT";
    }
}
