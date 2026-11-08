/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HDMI_DEV_SCDC_LINK_H
#define HDMI_DEV_SCDC_LINK_H

#include <stdbool.h>
#include <stdint.h>

/* VS680/Synopsys HDMI 的 SCDC 配置组件。 */
enum hdmi_scdc_result {
    HDMI_SCDC_OK = 0,
    HDMI_SCDC_BAD_ARGUMENT,
    HDMI_SCDC_CAPABILITIES_REQUIRED,
    HDMI_SCDC_UNSUPPORTED,
    HDMI_SCDC_RATE_UNSUPPORTED,
    HDMI_SCDC_OUTPUT_ACTIVE,
    HDMI_SCDC_DISCONNECTED,
    HDMI_SCDC_IO_ERROR,
    HDMI_SCDC_READBACK_MISMATCH,
    HDMI_SCDC_VERSION_UNSUPPORTED,
    HDMI_SCDC_TIME_INVALID,
    HDMI_SCDC_RESUME_WINDOW_EXPIRED
};

enum hdmi_scdc_support {
    HDMI_SCDC_UNKNOWN = 0,
    HDMI_SCDC_NOT_SUPPORTED,
    HDMI_SCDC_SUPPORTED
};

/* 输入来自源端规格和接收端 EDID，未知项保留 UNKNOWN/0。source_scdc_v1 表示控制器的 SCDC v1 能力；RGB8、零像素重复时，TMDS 字符率等于像素时钟。 */
struct hdmi_scdc_caps {
    uint64_t source_max_tmds_hz, sink_max_tmds_hz;
    enum hdmi_scdc_support source_scdc_v1;
    enum hdmi_scdc_support source_scrambling;
    enum hdmi_scdc_support source_ratio_40;
    enum hdmi_scdc_support sink_hdmi;
    enum hdmi_scdc_support sink_scdc;
    enum hdmi_scdc_support sink_mode;
};

#define HDMI_SCDC_DDC_ADDRESS UINT8_C(0x54) /* 7 位 I2C 地址。 */
#define HDMI_SCDC_IO_TIMEOUT_US UINT32_C(20000)
#define HDMI_SCDC_RESUME_MIN_US UINT64_C(1000)
#define HDMI_SCDC_RESUME_MAX_US UINT64_C(100000)

/* 回调成功返回 0，错误返回非零。DDC 读使用偏移写入加 repeated START，写使用偏移和值。回调在 timeout_us 内返回，MMIO 按物理字节地址访问；verify_source_clock 检查 VPLL、PHY 和时钟比配置。 */
struct hdmi_scdc_io {
    void *opaque;
    int (*read32)(void *, uint32_t address, uint32_t *value);
    int (*write32)(void *, uint32_t address, uint32_t value);
    int (*ddc_readb)(void *, uint8_t slave, uint8_t offset, uint8_t *value,
                     uint32_t timeout_us);
    int (*ddc_writeb)(void *, uint8_t slave, uint8_t offset, uint8_t value,
                      uint32_t timeout_us);
    int (*verify_source_clock)(void *, uint64_t tmds_character_hz,
                                bool ratio_40, uint32_t timeout_us);
    int (*now_us)(void *, uint64_t *value);
    int (*wait_us)(void *, uint32_t duration_us, uint32_t timeout_us);
};

enum hdmi_scdc_stage {
    HDMI_SCDC_STAGE_VALIDATE = 0,
    HDMI_SCDC_STAGE_PREFLIGHT,
    HDMI_SCDC_STAGE_SOURCE_VERSION,
    HDMI_SCDC_STAGE_SINK_SCRAMBLING,
    HDMI_SCDC_STAGE_SOURCE_SCRAMBLING,
    HDMI_SCDC_STAGE_SINK_RATIO,
    HDMI_SCDC_STAGE_SETTLE,
    HDMI_SCDC_STAGE_CONFIGURED,
    HDMI_SCDC_STAGE_RECOVERY,
    HDMI_SCDC_STAGE_RECOVERED
};

#define HDMI_SCDC_CHANGED_VERSION UINT32_C(1)
#define HDMI_SCDC_CHANGED_SINK_CONFIG UINT32_C(2)
#define HDMI_SCDC_CHANGED_KEEPOUT UINT32_C(4)
#define HDMI_SCDC_CHANGED_RESET UINT32_C(8)
#define HDMI_SCDC_CHANGED_SCRAMBLER UINT32_C(16)

struct hdmi_scdc_report {
    enum hdmi_scdc_stage stage;
    int callback_error;
    uint32_t last_address; /* MMIO 物理地址或 SCDC 字节偏移 */
    uint32_t writes_attempted, writes_completed;
    uint32_t changes_attempted; /* 包括返回失败的写，调用者据此处理部分配置 */
    uint8_t sink_version, sink_config_before, sink_config_after;
    bool sink_config_before_valid, sink_config_after_valid;
    bool source_clock_verified;
    bool configuration_matches; /* 配置读回一致。 */
    bool resume_window_valid;
    uint64_t ratio_write_started_us, ratio_write_finished_us;
    uint64_t resume_not_before_us, resume_before_us;
};

/* 先检查停止状态、连接及版本，再配置 Source Version、接收端加扰、本地复位和加扰、接收端时钟比，最后读回。比率写入后在 1..100 ms 内恢复 TMDS；函数返回启动窗口，HPD 变化后重新配置。 */
enum hdmi_scdc_result hdmi_scdc_configure_stopped_rgb8(
    uint64_t tmds_character_hz, const struct hdmi_scdc_caps *caps,
    const struct hdmi_scdc_io *io, struct hdmi_scdc_report *report);

/* 停止状态下恢复本地加扰及接收端配置，保留其余位；失败时返回部分写入记录。恢复后重新执行模式和 SCDC 配置。 */
enum hdmi_scdc_result hdmi_scdc_recover_stopped(
    const struct hdmi_scdc_caps *caps, const struct hdmi_scdc_io *io,
    struct hdmi_scdc_report *report);

/* 纯时间检查。launch_budget_us 是调用者对实际恢复 TMDS 所需时间的上界。
 * 仍需独占路径、当前连接、全部模式/PHY 条件；返回 true 仅表示时间充足。
 */
bool hdmi_scdc_resume_window_contains(const struct hdmi_scdc_report *report,
                                    uint64_t now_us, uint32_t launch_budget_us);

#endif
