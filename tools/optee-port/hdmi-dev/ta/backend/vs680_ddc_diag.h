/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef VS680_DDC_DIAG_H
#define VS680_DDC_DIAG_H

#include <stddef.h>
#include <stdint.h>

/* DDC 诊断的数据结构和回调接口。 */
#define VD_EDID_BLOCK_SIZE 128u
#define VD_EDID_MAX_BLOCKS 256u
#define VD_SCDC_READS 5u
/* 诊断程序使用的软件上限。 */
#define VD_MAX_TOTAL_TIMEOUT_US UINT64_C(60000000)
#define VD_MAX_TRANSACTION_TIMEOUT_US 100000u
#define VD_MAX_POLLS_PER_TRANSACTION 10000u

/* Synopsys TX 寄存器的字节偏移。 */
enum vd_register {
    VD_IH_STATUS = 0x00414, VD_IH_MUTE = 0x00614,
    VD_PHY_STATUS = 0x0c010,
    VD_SLAVE = 0x1f800, VD_ADDRESS = 0x1f804,
    VD_DATAI = 0x1f80c, VD_OPERATION = 0x1f810,
    VD_INT = 0x1f814, VD_CTLINT = 0x1f818, VD_DIV = 0x1f81c,
    VD_SEGADDR = 0x1f820, VD_SOFTRSTZ = 0x1f824, VD_SEGPTR = 0x1f828,
    VD_SS_HIGH1 = 0x1f82c, VD_SS_HIGH0 = 0x1f830,
    VD_SS_LOW1 = 0x1f834, VD_SS_LOW0 = 0x1f838,
    VD_FS_HIGH1 = 0x1f83c, VD_FS_HIGH0 = 0x1f840,
    VD_FS_LOW1 = 0x1f844, VD_FS_LOW0 = 0x1f848,
    VD_SDA_HOLD = 0x1f84c, VD_SCDC_UPDATE = 0x1f850,
    VD_READ_BUFFER0 = 0x1f880
};

enum vd_result {
    VD_OK = 0,
    VD_NOT_ATTEMPTED = -1, VD_BAD_ARGUMENT = -2, VD_OWNER_UNAVAILABLE = -3,
    VD_MMIO_ERROR = -4, VD_CLOCK_ERROR = -5, VD_WAIT_ERROR = -6,
    VD_TIMING_UNREADY = -7, VD_IRQ_GATED = -8, VD_AUTO_DDC_ACTIVE = -9,
    VD_HPD_LOW = -10, VD_TRANSACTION_TIMEOUT = -11, VD_TOTAL_TIMEOUT = -12,
    VD_POLL_LIMIT = -13, VD_I2C_ERROR = -14, VD_EDID_HEADER = -15,
    VD_EDID_CHECKSUM = -16, VD_EDID_LIMIT = -17, VD_RESTORE_ERROR = -18,
    VD_OWNER_FINISH_ERROR = -19, VD_STALE_STATUS = -20
};

/* 回调按顺序访问 32 位 MMIO，成功返回 0，错误返回非零。enter 获取当前显示/DDC 所有权并检查初始化与事务状态；失败时释放已取得的锁。finish 收尾，recovery_required=1 时保留故障状态，交由拥有者恢复。 */
struct vd_ops {
    void *ctx;
    int (*enter)(void *ctx, uint64_t absolute_deadline_us);
    int (*finish)(void *ctx, int recovery_required);
    int (*read32)(void *ctx, uint32_t byte_offset, uint32_t *value);
    int (*write32)(void *ctx, uint32_t byte_offset, uint32_t value);
    int (*now_us)(void *ctx, uint64_t *monotonic_us);
    int (*wait_us)(void *ctx, uint32_t delay_us);
};

struct vd_limits {
    uint64_t total_timeout_us;       /* Includes enter and all sink reads. */
    uint32_t transaction_timeout_us;
    uint32_t poll_interval_us;
    uint32_t max_polls_per_transaction; /* Hard bound if clock stops. */
    uint16_t max_edid_blocks;        /* 1..256, includes the base block. */
};

/* 当前版本的寄存器快照顺序。 */
enum vd_snapshot_index {
    VD_S_DIV, VD_S_SS_HIGH1, VD_S_SS_HIGH0, VD_S_SS_LOW1, VD_S_SS_LOW0,
    VD_S_FS_HIGH1, VD_S_FS_HIGH0, VD_S_FS_LOW1, VD_S_FS_LOW0, VD_S_SDA_HOLD,
    VD_S_INT, VD_S_CTLINT, VD_S_IH_MUTE, VD_S_SCDC_UPDATE, VD_S_SOFTRSTZ,
    VD_S_OPERATION, VD_S_IH_STATUS, VD_S_PHY_STATUS,
    VD_S_SLAVE, VD_S_ADDRESS, VD_S_SEGADDR, VD_S_SEGPTR, VD_SNAPSHOT_COUNT
};
struct vd_snapshot {
    uint32_t raw[VD_SNAPSHOT_COUNT];
    uint32_t valid_mask;             /* A failed read leaves remaining fields invalid. */
    uint16_t ss_high, ss_low, fs_high, fs_low;
    uint8_t sda_hold, fast_mode;
    uint8_t nonzero_sanity_only;     /* 配置检查结果。 */
};
struct vd_scdc_byte {
    uint8_t address, value;
    int result;
};
struct vd_report {
    int result, readiness_result, edid_result, cleanup_result;
    struct vd_snapshot before;
    struct vd_scdc_byte scdc[VD_SCDC_READS]; /* 读取 SCDC 的 01、20、21、40、41 寄存器。 */
    size_t edid_bytes;              /* Successfully read chunks, including a partial block. */
    uint16_t edid_blocks_read, edid_expected_blocks;
    uint8_t edid_checksum_sum[VD_EDID_MAX_BLOCKS]; /* 按字节求和并取模 256。 */
    uint8_t edid_header_valid, edid_complete, edid_valid, edid_limited;
    uint8_t recovery_required, selectors_restored, operation_may_be_active;
    uint8_t initial_status_cleared, last_status;
    uint32_t transactions_started, polls;
    uint32_t last_mmio_offset;
    uint8_t last_slave, last_segment, last_address;
    uint64_t elapsed_us;
};

extern const uint32_t vd_snapshot_offsets[VD_SNAPSHOT_COUNT];
extern const uint8_t vd_scdc_addresses[VD_SCDC_READS];

/* 调用方提供至少 max_edid_blocks×128 字节的缓冲。分别记录 EDID 和 SCDC 读取结果；VD_OK 表示请求的读取全部完成。 */
int vd_run(const struct vd_ops *ops, const struct vd_limits *limits,
           uint8_t *edid, size_t edid_capacity, struct vd_report *report);
const char *vd_result_name(int result);

#endif
