// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/pid.h>
#include <linux/sched/signal.h>
#include <linux/ktime.h>
#include <linux/kernel.h>
#include "rx-progress.h"
#include "rx-transaction.h"

int loop_rx_start_rate(bool rate6g);
#define RX_PRELINK_BUDGET_MS 60000
static DEFINE_MUTEX(register_mutex);
static struct pid *owner;
static u64 next_attempt;
static u64 allocated_ms;
static struct loop_rx_progress progress;
static int prelinked, transaction_error;
static u64 maintenance_generation, discarded_attempt;
static struct pid *receipt_owner;
static struct loop_rx_progress discarded_progress;
static int discarded_error;

int loop_rx_maintenance_init(void)
{
    if (maintenance_generation) return -EALREADY;
    maintenance_generation = ktime_get_boottime_ns();
    return maintenance_generation ? 0 : -EAGAIN;
}

void loop_rx_maintenance_detach(void)
{
    if (receipt_owner) put_pid(receipt_owner);
    receipt_owner = NULL;
}


static u64 monotonic_ms(void)
{
    return ktime_get_boottime_ns() / 1000000;
}

int loop_rx_register_acquire(void)
{
    if (!mutex_trylock(&register_mutex)) return -EBUSY;
    if (owner) {
        mutex_unlock(&register_mutex);
        return -EACCES;
    }
    return 0;
}

void loop_rx_register_release(void)
{
    mutex_unlock(&register_mutex);
}

int loop_rx_transaction_prelink_rate(bool rate6g)
{
    int result;
    if (!mutex_trylock(&register_mutex)) return -EBUSY;
    if (owner) { result = -EALREADY; goto unlock; }
    if (next_attempt == U64_MAX) { result = -EOVERFLOW; goto unlock; }
    /* 硬件操作前固定模块引用和进程身份；prelink 失败时继续保留引用。 */
    if (!try_module_get(THIS_MODULE)) { result = -ESHUTDOWN; goto unlock; }
    owner = get_task_pid(current, PIDTYPE_TGID);
    if (!owner) {
        module_put(THIS_MODULE);
        result = -ESRCH;
        goto unlock;
    }
    /* A receipt keeps exact PID identity without retaining this module.
     * The next accepted attempt invalidates it before changing hardware. */
    loop_rx_maintenance_detach();
    discarded_attempt = 0;
    progress = (struct loop_rx_progress){.attempt = ++next_attempt};
    allocated_ms = monotonic_ms();
    prelinked = 0;
    transaction_error = 0;
    result = loop_rx_start_rate(rate6g);
    if (!result && monotonic_ms() - allocated_ms >= RX_PRELINK_BUDGET_MS)
        result = -ETIMEDOUT;
    if (result) {
        transaction_error = result;
        progress.phase = LOOP_RX_FAILED;
        progress.error = result;
    } else prelinked = 1;
    /* prelinked 标记 PHY 配置阶段完成。 */
unlock:
    mutex_unlock(&register_mutex);
    return result;
}

int loop_rx_transaction_prelink(void)
{
    return loop_rx_transaction_prelink_rate(false);
}

static int authorize(u64 attempt)
{
    if (!owner) return -ENOENT;
    if (owner != task_tgid(current)) return -EPERM;
    if (!attempt || attempt != progress.attempt) return -ESTALE;
    return 0;
}

int loop_rx_transaction_begin(u64 attempt)
{
    int result;
    u64 now;
    if (!mutex_trylock(&register_mutex)) return -EBUSY;
    result = authorize(attempt);
    if (result) goto unlock;
    if (transaction_error) { result = transaction_error; goto unlock; }
    if (!prelinked || progress.phase != LOOP_RX_IDLE) {
        result = -EALREADY;
        goto unlock;
    }
    now = monotonic_ms();
    if (now - allocated_ms >= RX_PRELINK_BUDGET_MS) {
        result = transaction_error = -ETIMEDOUT;
        progress.error = result;
        progress.phase = LOOP_RX_FAILED;
        goto unlock;
    }
    /* 沿用 prelink 分配的事务编号。 */
    progress.started_ms = now;
    progress.phase = LOOP_RX_WAIT_LOCK;
    result = 0;
unlock:
    mutex_unlock(&register_mutex);
    return result;
}

int loop_rx_transaction_poll(u64 attempt)
{
    int result;
    if (!mutex_trylock(&register_mutex)) return -EBUSY;
    result = authorize(attempt);
    if (result) goto unlock;
    if (progress.phase == LOOP_RX_IDLE) { result = -EALREADY; goto unlock; }
    /* poll 的处理结果写入缓存状态，返回 0 表示该命令已处理。 */
    if (!transaction_error) {
        result = loop_rx_progress_step(&progress, attempt, monotonic_ms());
        if (result < 0) transaction_error = result;
    }
    result = 0;
unlock:
    mutex_unlock(&register_mutex);
    return result;
}

int loop_rx_transaction_release(u64 attempt)
{
    int result;
    struct pid *released_owner;
    if (!mutex_trylock(&register_mutex)) return -EBUSY;
    result = authorize(attempt);
    if (result) goto unlock;
    if (transaction_error || progress.phase != LOOP_RX_VALID ||
        progress.error || progress.cleanup_error || progress.threshold_saved ||
        progress.frames != 3 || !progress.avi_valid) {
        result = -EACCES;
        goto unlock;
    }
    /* 释放事务持有的进程和模块引用。 */
    released_owner = owner;
    owner = NULL;
    progress = (struct loop_rx_progress){0};
    prelinked = transaction_error = 0;
    allocated_ms = 0;
    put_pid(released_owner);
    module_put(THIS_MODULE);
    result = 0;
unlock:
    mutex_unlock(&register_mutex);
    return result;
}

ssize_t loop_rx_transaction_status(char *buffer)
{
    ssize_t result;
    const char *phase = "idle";
    if (!mutex_trylock(&register_mutex)) return -EBUSY;
    if (owner && owner != task_tgid(current)) { result = -EPERM; goto unlock; }
    if (owner) {
        if (progress.phase == LOOP_RX_WAIT_LOCK) phase = "wait_lock";
        else if (progress.phase == LOOP_RX_WAIT_EVENTS) phase = "wait_events";
        else if (progress.phase == LOOP_RX_VALID) phase = "valid";
        else if (progress.phase == LOOP_RX_FAILED) phase = "failed";
        else phase = "prelink";
    }
    /* 返回缓存的事务状态。 */
    result = scnprintf(buffer, PAGE_SIZE,
        "RX_TXN_V2 owner=%d attempt=%llu phase=%s error=%d cleanup_error=%d frames=%u avi=%u\n",
        owner ? pid_vnr(owner) : 0, (unsigned long long)progress.attempt, phase,
        transaction_error ? -transaction_error : 0,
        progress.cleanup_error ? -progress.cleanup_error : 0,
        progress.frames, progress.avi_valid);
unlock:
    mutex_unlock(&register_mutex);
    return result;
}

int loop_rx_discard_failed(u64 generation, u64 attempt)
{
    int result;
    if (!mutex_trylock(&register_mutex)) return -EBUSY;
    if (!generation || generation != maintenance_generation || !attempt) {
        result = -ESTALE;
        goto unlock;
    }
    if (!owner && receipt_owner) {
        result = attempt != discarded_attempt ? -ESTALE :
                 receipt_owner == task_tgid(current) ? 0 : -EPERM;
        goto unlock;
    }
    result = authorize(attempt);
    if (result) goto unlock;
    if (!prelinked || progress.phase != LOOP_RX_FAILED || !transaction_error ||
        progress.cleanup_error || progress.threshold_saved) {
        result = -EACCES;
        goto unlock;
    }
    /* 保存失败记录并释放配置事务的所有权。 */
    discarded_attempt = attempt;
    discarded_progress = progress;
    discarded_error = transaction_error;
    receipt_owner = owner;
    owner = NULL;
    progress = (struct loop_rx_progress){0};
    prelinked = transaction_error = 0;
    allocated_ms = 0;
    module_put(THIS_MODULE);
    result = 0;
unlock:
    mutex_unlock(&register_mutex);
    return result;
}

ssize_t loop_rx_maintenance_status(char *buffer)
{
    const struct loop_rx_progress *s;
    const char *phase;
    int error;
    ssize_t result;
    if (!mutex_trylock(&register_mutex)) return -EBUSY;
    if ((owner && owner != task_tgid(current)) ||
        (!owner && receipt_owner && receipt_owner != task_tgid(current))) {
        result = -EPERM;
        goto unlock;
    }
    s = !owner && discarded_attempt ? &discarded_progress : &progress;
    error = !owner && discarded_attempt ? discarded_error : transaction_error;
    phase = s->phase == LOOP_RX_FAILED ? "failed" :
            s->phase == LOOP_RX_VALID ? "valid" :
            s->phase == LOOP_RX_WAIT_LOCK ? "wait_lock" :
            s->phase == LOOP_RX_WAIT_EVENTS ? "wait_events" :
            owner ? "prelink" : "idle";
    result = scnprintf(buffer, PAGE_SIZE,
        "RX_MAINT_V1 generation=%llu attempt=%llu scope=rx-config phase=%s error=%d cleanup_error=%d pending_restore=%u last_discard_receipt=%llu\n",
        (unsigned long long)maintenance_generation,
        (unsigned long long)s->attempt, phase, error ? -error : 0,
        s->cleanup_error ? -s->cleanup_error : 0, s->threshold_saved,
        (unsigned long long)discarded_attempt);
unlock:
    mutex_unlock(&register_mutex);
    return result;
}
