/* SPDX-License-Identifier: GPL-2.0 */
/* Each finite DMA command owns one band; missed events cause a timeout. */
static unsigned band_lines, band_queued, band_completed;
static uint32_t band_address;
static unsigned band_count(void) { return band_lines ? 720 / band_lines : 1; }
/* VPP's 128-bit command slots use the same 16-byte unit as channel_cfg. */
static int band_write_cmd(uint32_t address, unsigned bytes)
{
    HDL_hbo *hbo = dhub_hbo(&capture_dhub.dhub);
    unsigned queue = 2 * CHANNEL, slot = 0;
    unsigned used = hbo_queue_query(hbo, queue, 0, &slot);
    unsigned depth = hbo->fifoCtl.depth[queue];
    if (!depth || used >= depth || slot >= depth) return 0;
    SIGN32 words[2];
    dhub_channel_generate_cmd(&capture_dhub.dhub, CHANNEL, address, bytes,
                              0, 0, 0, 1, words);
    uint32_t base = hbo->mem + hbo->base[queue] + slot * 16;
    volatile uint32_t *command = TEE_TA_RegPhyToVirt(base);
    command[0] = words[0]; command[1] = words[1];
    command[2] = 0; command[3] = 0;
    __asm__ volatile("dmb sy" ::: "memory");
    hbo_queue_push(hbo, queue, 1);
    return 1;
}
static void band_enqueue(void)
{
    while (band_queued < band_count()) {
        uint32_t offset = band_queued * band_lines * 3840u;
        if (!band_write_cmd(band_address + offset, band_lines * 3840u))
            break;
        band_queued++;
    }
}
static TEE_Result band_progress(TEE_Param p[4])
{
    if (!active || !band_lines) return TEE_ERROR_BAD_STATE;
    HDL_semaphore *completion = dhub_semaphore(&capture_dhub.dhub);
    /* 通道完成位可能合并多个事件；计数不足时等待超时。 */
    if (semaphore_chk_full(completion, -1) & MASK) {
        /* Pop before clearing: clearing a still-full cell reasserts the latch. */
        semaphore_pop(completion, CHANNEL, 1);
        semaphore_clr_full(completion, CHANNEL);
        __asm__ volatile("dsb sy" ::: "memory");
        band_completed++;
        if (band_completed > band_queued) return TEE_ERROR_BAD_STATE;
    }
    band_enqueue();
    p[1].value.a = band_completed * band_lines;
    p[1].value.b = busy();
    p[2].value.a = band_queued;
    p[2].value.b = band_count();
    return TEE_SUCCESS;
}

/* Keep acknowledgements independent of normal-world pixel processing. */
static TEE_Result band_collect(uint32_t types, TEE_Param p[4])
{
    if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,
            TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE) ||
        p[0].memref.size != sizeof(struct capture_notify))
        return TEE_ERROR_BAD_PARAMETERS;
    if (!active || !band_lines || !input_pending) return TEE_ERROR_BAD_STATE;
    struct capture_notify *notice = p[0].memref.buffer;
    TEE_Time begin, now;
    TEE_GetSystemTime(&begin);
    input_pending = 0;
    if (!capture_sync_enabled) reg_write(RA_HDMIRX_PIPE_tg_ctrl, 1);
    TEE_Result result = TEE_ERROR_TIMEOUT;
    unsigned published = UINT32_MAX, previous_busy = UINT32_MAX;
    do {
        TEE_Param progress[4] = {0};
        result = band_progress(progress);
        if (result) break;
        if (published != progress[1].value.a || previous_busy != progress[1].value.b) {
            __atomic_store_n(&notice->busy, progress[1].value.b, __ATOMIC_RELAXED);
            __atomic_store_n(&notice->queued, progress[2].value.a, __ATOMIC_RELAXED);
            __atomic_store_n(&notice->total, progress[2].value.b, __ATOMIC_RELAXED);
            __atomic_store_n(&notice->rows, progress[1].value.a, __ATOMIC_RELEASE);
            published = progress[1].value.a;
            previous_busy = progress[1].value.b;
        }
        if (progress[1].value.a == 720 && !progress[1].value.b) break;
        TEE_GetSystemTime(&now);
        uint64_t elapsed = (uint64_t)now.seconds * 1000 + now.millis -
                           ((uint64_t)begin.seconds * 1000 + begin.millis);
        if (elapsed >= 200) { result = TEE_ERROR_TIMEOUT; break; }
    } while (1);
    __atomic_store_n(&notice->result, result, __ATOMIC_RELAXED);
    __atomic_store_n(&notice->done, 1, __ATOMIC_RELEASE);
    return result;
}
