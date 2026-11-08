// SPDX-License-Identifier: GPL-2.0-only
#ifndef VS680_HDMI_DEV_H
#define VS680_HDMI_DEV_H

#include <stdint.h>

/* HDMI 开发 TA 的 UUID。 */
#define HDMI_DEV_UUID \
    { 0x4156150c, 0xfa24, 0x4acc, \
      { 0x9c, 0x5a, 0x7f, 0x78, 0x87, 0x85, 0x47, 0x3f } }
#define HDMI_DEV_UUID_STRING "4156150c-fa24-4acc-9c5a-7f788785473f"

/* INIT、SHOW、BLEND: VALUE_INPUT、NONE、NONE、NONE。STOP、SERVICE: 全部 NONE。
 * STATUS: MEMREF_OUTPUT、NONE、NONE、NONE，长度恰为 24 字节。 */
enum hdmi_dev_command {
    HDMI_DEV_INIT = 0,
    HDMI_DEV_SHOW = 1,
    HDMI_DEV_STATUS = 2,
    HDMI_DEV_STOP = 3,
    HDMI_DEV_SERVICE = 4,
    HDMI_DEV_BLEND = 5,
    HDMI_DEV_OVERLAY = 6,
    HDMI_DEV_INIT_MODE = 7,
    HDMI_DEV_PREFLIGHT_MODE = 8,
    HDMI_DEV_DDC_SNAPSHOT = 9,
    HDMI_DEV_DDC_READ = 10,
    HDMI_DEV_DDC_OWNER_PREPARE = 11,
    HDMI_DEV_DDC_RECOVER = 12
};

/* 命令 9 返回当前会话 INIT 后的寄存器快照。 */
#define HDMI_DEV_DDC_SNAPSHOT_VERSION 1u
#define HDMI_DEV_DDC_RAW_COUNT 22u
#define HDMI_DEV_DDC_TIMING_UNREADY 1u
#define HDMI_DEV_DDC_IRQ_GATED 2u
#define HDMI_DEV_DDC_AUTO_ACTIVE 4u
#define HDMI_DEV_DDC_HPD_LOW 8u
#define HDMI_DEV_DDC_PARTIAL 16u
#define HDMI_DEV_DDC_SNAPSHOT_ONLY 1u
#define HDMI_DEV_DDC_OWNER_UNVERIFIED 2u
#define HDMI_DEV_DDC_SFR_UNVERIFIED 4u
struct hdmi_dev_ddc_snapshot {
    uint32_t version, size, readiness, valid_mask;
    uint32_t raw[HDMI_DEV_DDC_RAW_COUNT];
    uint32_t selected_high, selected_low, sda_hold, fast_mode, flags, io_result;
};

#define HDMI_DEV_DDC_READ_VERSION 1u
#define HDMI_DEV_DDC_OWNER_HDCP_CLOCK 1u
#define HDMI_DEV_DDC_OWNER_HDCP14 2u
#define HDMI_DEV_DDC_OWNER_HDCP22 4u
#define HDMI_DEV_DDC_OWNER_HPD_EVENT 8u
#define HDMI_DEV_DDC_OWNER_QUARANTINE 16u
#define HDMI_DEV_DDC_OWNER_IO 32u
#define HDMI_DEV_DDC_OWNER_EPOCH 64u
#define HDMI_DEV_DDC_OWNER_GENERATION 128u
/* Fixed32 wire values; negative results preserve the VD enum's int32 value.
 * Owner raw order: MC_CLKDIS, HDCPCFG0, HDCPCFG1, HDCP22REG_CTRL, IH_PHY. */
struct hdmi_dev_ddc_read {
    uint32_t version,size;
    int32_t result,readiness_result,edid_result,cleanup_result;
    uint32_t owner_blockers,owner_valid_mask,owner_raw[5];
    struct hdmi_dev_ddc_snapshot before;
    uint32_t edid_bytes,edid_blocks,edid_expected,edid_flags,run_flags;
    uint32_t last_status,transactions,polls,last_offset,last_slave,last_segment,last_address;
    uint32_t elapsed_us_low,elapsed_us_high;
    int32_t scdc_result[5];
    uint32_t scdc_value[5],checksum_sum[2];
    uint8_t edid[256];
};

/* 显式端口准备：固定 80B 输出，全为固定宽度字段。 */
#define HDMI_DEV_DDC_RECOVERY_VERSION 1u
struct hdmi_dev_ddc_recovery {
    uint32_t version,size;
    int32_t result,original_result;
    uint32_t flags,status,owner_blockers,owner_valid_mask;
    uint32_t selectors[4];
};
#define HDMI_DEV_DDC_PREPARE_VERSION 1u
struct hdmi_dev_ddc_prepare {
    uint32_t version,size;
    int32_t result;
    uint32_t flags,writes,valid_mask,epoch;
    uint32_t before[5],after[5],phy_before,phy_after,ddc_status;
};

#define HDMI_DEV_MODE_VERSION 1u
#define HDMI_DEV_MODE_SINK_HDMI 1u
#define HDMI_DEV_MODE_SINK_LISTED 2u
#define HDMI_DEV_MODE_SOURCE_SCDC 4u
#define HDMI_DEV_MODE_SINK_SCDC 8u
struct hdmi_dev_detailed_timing {
    uint64_t pixel_clock_hz;
    uint32_t hdisplay, hsync_start, hsync_end, htotal;
    uint32_t vdisplay, vsync_start, vsync_end, vtotal;
    uint32_t flags, cta_vic;
};
/* INIT_MODE 接收详细时序、源端能力和接收端能力。 */
struct hdmi_dev_mode_request {
    uint32_t version, size, pool_pa, pool_bytes;
    uint32_t main_slots, pip_enabled, vpp_blanking, caps_bits;
    struct hdmi_dev_detailed_timing timing;
    uint64_t source_max_tmds_hz, sink_max_tmds_hz;
    uint32_t reserved[4];
};

#define HDMI_DEV_PREFLIGHT_VERSION 1u
#define HDMI_DEV_PREFLIGHT_PURE 1u
/* 命令 8 校验输入并计算内存布局，原样返回请求内容。 */
struct hdmi_dev_mode_preflight {
    uint32_t version,size;
    struct hdmi_dev_mode_request request;
    uint32_t width,height,stride,image_bytes;
    uint32_t frame_base,frame_slot_bytes,main_slots,total_bytes;
    uint32_t pip_slot_bytes,pip_offset[2],command_bytes;
    uint32_t flags,reserved[3];
};

enum hdmi_dev_pattern {
    HDMI_DEV_PATTERN_COORDINATE = 0,
    HDMI_DEV_PATTERN_SOLID = 1
};
enum hdmi_dev_state {
    HDMI_DEV_UNINITIALIZED = 0,
    HDMI_DEV_READY = 1,
    HDMI_DEV_DISPLAYING = 2,
    HDMI_DEV_FAULT = 3
};
#define HDMI_DEV_WIDTH 1280u
#define HDMI_DEV_HEIGHT 720u
#define HDMI_DEV_REFRESH_MILLIHZ 60000u
#define HDMI_DEV_DMA_QUIESCENT (UINT32_C(1) << 0)
#define HDMI_DEV_POOL_ALIGNMENT UINT32_C(4096)
#define HDMI_DEV_POOL_BASE UINT32_C(0x76400000)
#define HDMI_DEV_POOL_LIMIT UINT32_C(0x76e00000)
#define HDMI_DEV_POOL_BYTES UINT32_C(0x800000)
#define HDMI_DEV_CACHED_POOL_BASE UINT32_C(0x6fc00000)
#define HDMI_DEV_CACHED_POOL_LIMIT UINT32_C(0x71c00000)
#define HDMI_DEV_CACHED_POOL_BYTES UINT32_C(0x2000000)

/* 支持原NC 8MiB或独立缓存32MiB池；当前720p主图使用三个紧密槽。
 * 首次PIP在READY配置后使用前2MiB命令/叠加及两个3MiB主槽。 */
static inline int hdmi_dev_pool_valid(uint32_t pool_pa, uint32_t pool_bytes)
{
    uint32_t begin,limit;
    if (pool_bytes==HDMI_DEV_POOL_BYTES) { begin=HDMI_DEV_POOL_BASE;limit=HDMI_DEV_POOL_LIMIT; }
    else if (pool_bytes==HDMI_DEV_CACHED_POOL_BYTES) {
        begin=HDMI_DEV_CACHED_POOL_BASE;limit=HDMI_DEV_CACHED_POOL_LIMIT;
    } else return 0;
    return pool_pa>=begin && pool_pa<limit && !(pool_pa%HDMI_DEV_POOL_ALIGNMENT) &&
        pool_bytes<=limit-pool_pa;
}

/* OVERLAY 使用两个 MEMREF_INPUT：20 字节几何配置和 RGBA 像素。首次在 READY 配置；DISPLAYING 时可提交同尺寸内容。行跨度按 16 字节对齐，使用空闲叠加槽。 */
#define HDMI_DEV_OVERLAY_OFFSET UINT32_C(0x10000)
#define HDMI_DEV_OVERLAY_SLOT_BYTES UINT32_C(0xe0000)
struct hdmi_dev_overlay {
    uint32_t x, y, width, height, stride;
};
static inline int hdmi_dev_overlay_valid(const struct hdmi_dev_overlay *p,
    uint64_t bytes)
{
    return p && p->width && p->height && p->width <= 2560u &&
        p->height <= 1440u && p->x <= 2560u - p->width &&
        p->y <= 1440u - p->height && !(p->width % 4u) &&
        !(p->stride % 16u) && p->stride >= p->width * 4u &&
        bytes == (uint64_t)p->stride * p->height &&
        bytes <= HDMI_DEV_OVERLAY_SLOT_BYTES;
}

struct hdmi_dev_status {
    uint32_t state;
    uint32_t width;
    uint32_t height;
    uint32_t refresh_millihz;
    uint32_t last_error;
    uint32_t flags;
};

/* INIT 接收 4 KiB 对齐的物理池地址和大小，支持 NC 8 MiB 或缓存 32 MiB；底层 DMA-BUF 由 Linux holder 保留。SHOW 选择坐标图或纯色，纯色参数为 0xRRGGBB。BLEND 的透明度范围为 0..4095。SERVICE 处理帧切换，BUSY 时继续处理已有提交。STOP 完成后设置 DMA_QUIESCENT，再由持有方回收内存。 */

#endif
