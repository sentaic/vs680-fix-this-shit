/* SPDX-License-Identifier: GPL-2.0 */
/* Internal TA adapter; command-pool mapping is owned by the existing 0x7f35. */
#include "rx-vde-exclusive.inc"
static struct rx_vde_exclusive stream_exclusive_vde;
static int stream_exclusive_granted;
static uint32_t stream_exclusive_aio_enable_issued;
static int stream_exclusive_vde_verified(void)
{
    /* 该连续采集模式当前返回能力值 0。 */
    return 0;
}
static int stream_exclusive_next_supported(void)
{ return stream_exclusive_vde_verified(); }
static int stream_exclusive_enabled(void)
{ return stream_exclusive_granted && stream_exclusive_vde_verified(); }
static int stream_exclusive_open_allowed(void)
{ return !rx_exclusive_aio_owned; }
static TEE_Result stream_exclusive_prepare(uint64_t epoch)
{
    if (!stream_exclusive_vde_verified()) return TEE_ERROR_NOT_SUPPORTED;
    if (stream_exclusive_granted || rx_exclusive_aio_owned || stream_first_frame_started || active || busy() ||
        !rx_command_pool_bound(epoch) || !capture_stream.opened ||
        capture_stream.epoch != epoch) return TEE_ERROR_BAD_STATE;
    /* 复用 0x7f36 的 AIO 初始化。 */
    stream_exclusive_aio_enable_issued=0;
    TEE_Result result=rx_aio_exclusive_prepare(epoch);
    if (rx_exclusive_aio_owned) stream_exclusive_granted=1;
    if (result) return result;
    stream_exclusive_aio_enable_issued=1;
    HDL_dhub2d *aio=rx_aio_exclusive_handle(epoch);
    if (!aio) return TEE_ERROR_BAD_STATE;
    void *mapped;uint32_t pa;
    result=rx_command_pool_table(epoch,0,RX_VDE_POOL_REQUIRED,&mapped,&pa);
    if (result) return result;
    return rx_vde_exclusive_bind(&stream_exclusive_vde,aio,mapped,pa,
        rx_command_pool.bytes,rx_command_pool.mem_id,epoch,stream_exclusive_aio_enable_issued);
}
static TEE_Result stream_exclusive_start_next(const struct rx_stream_reservation *r)
{
    if (!stream_exclusive_enabled() || !stream_exclusive_vde.bound)
        return TEE_ERROR_NOT_SUPPORTED;
    if (!capture_geometry_matches(&capture_geometry) ||
        !(reg_read(RA_HDMIRX_PIPE_tg_ctrl)&1u)) return TEE_ERROR_BAD_STATE;
    struct rx_dma_plan dma;
    if (rx_dma_plan_make(capture_layout.stride,capture_layout.stride,capture_layout.height,&dma))
        return TEE_ERROR_BAD_PARAMETERS;
    TEE_Result result=rx_vde_exclusive_build(&stream_exclusive_vde,&capture_dhub,
        (uint32_t)r->address,&dma,r->token);
    if (!result) result=rx_vde_exclusive_publish(&stream_exclusive_vde,&capture_dhub,
                                                &rx_command_pool.dma_armed,&stream_disable_issued);
    /* 发布失败且尚未 armed 时，清除 prepared 状态。 */
    if (result && !stream_exclusive_vde.armed) stream_exclusive_vde.prepared=0;
    return result;
}
static TEE_Result stream_exclusive_read_finish(void)
{
    if (!stream_exclusive_vde.armed) return TEE_SUCCESS;
    return rx_vde_exclusive_complete(&stream_exclusive_vde,&capture_dhub,
                                      &rx_command_pool.dma_armed);
}
static TEE_Result stream_exclusive_stop(void)
{
    if (!stream_exclusive_granted) return TEE_SUCCESS;
    if (!stream_exclusive_vde.bound)
        return rx_command_pool.dma_armed ? TEE_ERROR_BUSY : TEE_SUCCESS;
    /* Finish nested readers before the old STOP disables the VDE producer. */
    return rx_vde_exclusive_stop(&stream_exclusive_vde,&capture_dhub,
                                 &rx_command_pool.dma_armed);
}
static TEE_Result stream_exclusive_release(void)
{
    if (!stream_exclusive_granted) return TEE_SUCCESS;
    if (stream_exclusive_vde.bound || rx_command_pool.dma_armed) return TEE_ERROR_BUSY;
    /* RX STOP 完成后释放 AIO；释放函数同时检查 VPP 停止状态。 */
    TEE_Result result=rx_aio_exclusive_release(capture_stream.epoch);
    if (result) return result;
    stream_exclusive_granted=0; stream_exclusive_aio_enable_issued=0;
    stream_exclusive_vde=(struct rx_vde_exclusive){0};
    return TEE_SUCCESS;
}
static int stream_exclusive_can_close(void)
{
    return !stream_exclusive_granted && !stream_exclusive_vde.bound &&
        !rx_exclusive_aio_owned && !rx_command_pool.dma_armed;
}
