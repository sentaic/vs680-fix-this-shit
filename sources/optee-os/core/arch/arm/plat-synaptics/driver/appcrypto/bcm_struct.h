#ifndef REG_STRUCT_H
#define REG_STRUCT_H

// Structure for each register (requires definition of uint8_t, U16, and uint32_t)

struct biu_CMD_PARAM0
{
  union {
#define biu_CMD_PARAM0_address 0x00000000
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM1
{
  union {
#define biu_CMD_PARAM1_address 0x00000004
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM2
{
  union {
#define biu_CMD_PARAM2_address 0x00000008
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM3
{
  union {
#define biu_CMD_PARAM3_address 0x0000000C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM4
{
  union {
#define biu_CMD_PARAM4_address 0x00000010
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM5
{
  union {
#define biu_CMD_PARAM5_address 0x00000014
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM6
{
  union {
#define biu_CMD_PARAM6_address 0x00000018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM7
{
  union {
#define biu_CMD_PARAM7_address 0x0000001C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM8
{
  union {
#define biu_CMD_PARAM8_address 0x00000020
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM9
{
  union {
#define biu_CMD_PARAM9_address 0x00000024
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM10
{
  union {
#define biu_CMD_PARAM10_address 0x00000028
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM11
{
  union {
#define biu_CMD_PARAM11_address 0x0000002C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM12
{
  union {
#define biu_CMD_PARAM12_address 0x00000030
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM13
{
  union {
#define biu_CMD_PARAM13_address 0x00000034
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM14
{
  union {
#define biu_CMD_PARAM14_address 0x00000038
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_PARAM15
{
  union {
#define biu_CMD_PARAM15_address 0x0000003C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD
{
  union {
#define biu_CMD_address 0x00000040
    uint32_t value;
    struct {
      uint32_t CMD                                                                    : 16; // [15: 0]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct biu___reserved__17
{
  union {
#define biu___reserved__17_address 0x00000044
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__18
{
  union {
#define biu___reserved__18_address 0x00000048
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__19
{
  union {
#define biu___reserved__19_address 0x00000050
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__20
{
  union {
#define biu___reserved__20_address 0x00000058
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__21
{
  union {
#define biu___reserved__21_address 0x00000060
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__22
{
  union {
#define biu___reserved__22_address 0x00000068
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__23
{
  union {
#define biu___reserved__23_address 0x00000070
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__24
{
  union {
#define biu___reserved__24_address 0x00000078
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_RET_STATUS
{
  union {
#define biu_CMD_RET_STATUS_address 0x00000080
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS0
{
  union {
#define biu_CMD_STATUS0_address 0x00000084
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS1
{
  union {
#define biu_CMD_STATUS1_address 0x00000088
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS2
{
  union {
#define biu_CMD_STATUS2_address 0x0000008C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS3
{
  union {
#define biu_CMD_STATUS3_address 0x00000090
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS4
{
  union {
#define biu_CMD_STATUS4_address 0x00000094
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS5
{
  union {
#define biu_CMD_STATUS5_address 0x00000098
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS6
{
  union {
#define biu_CMD_STATUS6_address 0x0000009C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS7
{
  union {
#define biu_CMD_STATUS7_address 0x000000A0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS8
{
  union {
#define biu_CMD_STATUS8_address 0x000000A4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS9
{
  union {
#define biu_CMD_STATUS9_address 0x000000A8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS10
{
  union {
#define biu_CMD_STATUS10_address 0x000000AC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS11
{
  union {
#define biu_CMD_STATUS11_address 0x000000B0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS12
{
  union {
#define biu_CMD_STATUS12_address 0x000000B4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS13
{
  union {
#define biu_CMD_STATUS13_address 0x000000B8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS14
{
  union {
#define biu_CMD_STATUS14_address 0x000000BC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_STATUS15
{
  union {
#define biu_CMD_STATUS15_address 0x000000C0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CMD_FIFO_STATUS
{
  union {
#define biu_CMD_FIFO_STATUS_address 0x000000C4
    uint32_t value;
    struct {
      uint32_t CMD_CNTR                                                               :  3; // [ 2: 0]
      uint32_t __reserved__2                                                          :  5; // [ 7: 3]
      uint32_t CMD_STATUS                                                             :  8; // [15: 8]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct biu_HST_INTERRUPT_RST
{
  union {
#define biu_HST_INTERRUPT_RST_address 0x000000C8
    uint32_t value;
    struct {
      uint32_t SP_CMD_CMPLT                                                           :  1; // [ 0: 0]
      uint32_t __reserved__4                                                          : 15; // [15: 1]
      uint32_t HST_ADDR_RANGE                                                         :  1; // [16:16]
      uint32_t HST_QUEUE_FULL_ACCESS                                                  :  1; // [17:17]
      uint32_t HST_QUEUE_FULL                                                         :  1; // [18:18]
      uint32_t __reserved__0                                                          : 13; // [31:19]
    } b;
  } u;
};

struct biu_HST_INTERRUPT_MASK
{
  union {
#define biu_HST_INTERRUPT_MASK_address 0x000000CC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_HST_EXCEPT_ADDR
{
  union {
#define biu_HST_EXCEPT_ADDR_address 0x000000D0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_HST_TRUST
{
  union {
#define biu_HST_TRUST_address 0x000000D4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_WTM_ID
{
  union {
#define biu_WTM_ID_address 0x000000D8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_WTM_REV
{
  union {
#define biu_WTM_REV_address 0x000000DC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_CONTEXT_STATUS
{
  union {
#define biu_CONTEXT_STATUS_address 0x000000E0
    uint32_t value;
    struct {
      uint32_t CONTEXT_STATUS                                                         : 32; // [31: 0]
    } b;
  } u;
};

struct biu___reserved__50
{
  union {
#define biu___reserved__50_address 0x000000E4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__51
{
  union {
#define biu___reserved__51_address 0x000000E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__52
{
  union {
#define biu___reserved__52_address 0x000000F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__53
{
  union {
#define biu___reserved__53_address 0x000000F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__54
{
  union {
#define biu___reserved__54_address 0x00000100
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__55
{
  union {
#define biu___reserved__55_address 0x00000108
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__56
{
  union {
#define biu___reserved__56_address 0x00000110
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__57
{
  union {
#define biu___reserved__57_address 0x00000118
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__58
{
  union {
#define biu___reserved__58_address 0x00000120
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__59
{
  union {
#define biu___reserved__59_address 0x00000128
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__60
{
  union {
#define biu___reserved__60_address 0x00000130
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__61
{
  union {
#define biu___reserved__61_address 0x00000138
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__62
{
  union {
#define biu___reserved__62_address 0x00000140
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__63
{
  union {
#define biu___reserved__63_address 0x00000148
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__64
{
  union {
#define biu___reserved__64_address 0x00000150
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__65
{
  union {
#define biu___reserved__65_address 0x00000158
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__66
{
  union {
#define biu___reserved__66_address 0x00000160
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__67
{
  union {
#define biu___reserved__67_address 0x00000168
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__68
{
  union {
#define biu___reserved__68_address 0x00000170
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__69
{
  union {
#define biu___reserved__69_address 0x00000178
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__70
{
  union {
#define biu___reserved__70_address 0x00000180
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__71
{
  union {
#define biu___reserved__71_address 0x00000188
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__72
{
  union {
#define biu___reserved__72_address 0x00000190
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__73
{
  union {
#define biu___reserved__73_address 0x00000198
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__74
{
  union {
#define biu___reserved__74_address 0x000001A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__75
{
  union {
#define biu___reserved__75_address 0x000001A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__76
{
  union {
#define biu___reserved__76_address 0x000001B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__77
{
  union {
#define biu___reserved__77_address 0x000001B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__78
{
  union {
#define biu___reserved__78_address 0x000001C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__79
{
  union {
#define biu___reserved__79_address 0x000001C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__80
{
  union {
#define biu___reserved__80_address 0x000001D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__81
{
  union {
#define biu___reserved__81_address 0x000001D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__82
{
  union {
#define biu___reserved__82_address 0x000001E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__83
{
  union {
#define biu___reserved__83_address 0x000001E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__84
{
  union {
#define biu___reserved__84_address 0x000001F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__85
{
  union {
#define biu___reserved__85_address 0x000001F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__86
{
  union {
#define biu___reserved__86_address 0x00000200
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__87
{
  union {
#define biu___reserved__87_address 0x00000208
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu_SP_INTERRUPT_SET
{
  union {
#define biu_SP_INTERRUPT_SET_address 0x00000210
    uint32_t value;
    struct {
      uint32_t ENG_INT                                                                :  1; // [ 0: 0]
      uint32_t CMD_REG_OCCUPIED                                                       :  1; // [ 1: 1]
      uint32_t SYS_ADDR_RANGE                                                         :  1; // [ 2: 2]
      uint32_t MEMC_ADDR_RANGE                                                        :  1; // [ 3: 3]
      uint32_t MEMC_ERR_SBE                                                           :  1; // [ 4: 4]
      uint32_t MEMC_ERR_DBE                                                           :  1; // [ 5: 5]
      uint32_t MEMC_ERR_PAR                                                           :  1; // [ 6: 6]
      uint32_t WTM_ID_ERR                                                             :  1; // [ 7: 7]
      uint32_t __reserved__6                                                          :  1; // [ 8: 8]
      uint32_t HST_ADDR_RANGE                                                         :  1; // [ 9: 9]
      uint32_t TRUST_REG_RD                                                           :  1; // [10:10]
      uint32_t ENG_SLVERR                                                             :  1; // [11:11]
      uint32_t SOC_PWR_MONITOR                                                        :  1; // [12:12]
      uint32_t SOC_SIDE_CHANNEL                                                       :  1; // [13:13]
      uint32_t __reserved__0                                                          : 18; // [31:14]
    } b;
  } u;
};

struct biu_SP_SRAM_CTRL
{
  union {
#define biu_SP_SRAM_CTRL_address 0x00000214
    uint32_t value;
    struct {
      uint32_t MEMC_REMAP                                                             :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct biu_SP_INTERRUPT_RST
{
  union {
#define biu_SP_INTERRUPT_RST_address 0x00000218
    uint32_t value;
    struct {
      uint32_t ENG_INT                                                                :  1; // [ 0: 0]
      uint32_t CMD_REG_OCCUPIED                                                       :  1; // [ 1: 1]
      uint32_t SYS_ADDR_RANGE                                                         :  1; // [ 2: 2]
      uint32_t MEMC_ADDR_RANGE                                                        :  1; // [ 3: 3]
      uint32_t MEMC_ERR_SBE                                                           :  1; // [ 4: 4]
      uint32_t MEMC_ERR_DBE                                                           :  1; // [ 5: 5]
      uint32_t MEMC_ERR_PAR                                                           :  1; // [ 6: 6]
      uint32_t WTM_ID_ERR                                                             :  1; // [ 7: 7]
      uint32_t __reserved__6                                                          :  1; // [ 8: 8]
      uint32_t HST_ADDR_RANGE                                                         :  1; // [ 9: 9]
      uint32_t TRUST_REG_RD                                                           :  1; // [10:10]
      uint32_t ENG_SLVERR                                                             :  1; // [11:11]
      uint32_t SOC_PWR_MONITOR                                                        :  1; // [12:12]
      uint32_t SOC_SIDE_CHANNEL                                                       :  1; // [13:13]
      uint32_t __reserved__0                                                          : 18; // [31:14]
    } b;
  } u;
};

struct biu_SP_INTERRUPT_MASK
{
  union {
#define biu_SP_INTERRUPT_MASK_address 0x0000021C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_SP_CONTROL
{
  union {
#define biu_SP_CONTROL_address 0x00000220
    uint32_t value;
    struct {
      uint32_t CMD_REG_OCCUPIED                                                       :  1; // [ 0: 0]
      uint32_t SP_INT_TO_HST                                                          :  1; // [ 1: 1]
      uint32_t MASK_SP_INT                                                            :  1; // [ 2: 2]
      uint32_t SEC_ACCESS_CTRL                                                        :  1; // [ 3: 3]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct biu_SP_EXCEPT_ADDR
{
  union {
#define biu_SP_EXCEPT_ADDR_address 0x00000224
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_SP_EXCEPT_DATA
{
  union {
#define biu_SP_EXCEPT_DATA_address 0x00000228
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_SP_EXCEPT_PAR
{
  union {
#define biu_SP_EXCEPT_PAR_address 0x0000022C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__96
{
  union {
#define biu___reserved__96_address 0x00000230
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu_HST_INTERRUPT_SET
{
  union {
#define biu_HST_INTERRUPT_SET_address 0x00000234
    uint32_t value;
    struct {
      uint32_t SP_CMD_CMPLT                                                           :  1; // [ 0: 0]
      uint32_t __reserved__4                                                          : 15; // [15: 1]
      uint32_t HST_ADDR_RANGE                                                         :  1; // [16:16]
      uint32_t HST_QUEUE_FULL_ACCESS                                                  :  1; // [17:17]
      uint32_t HST_QUEUE_FULL                                                         :  1; // [18:18]
      uint32_t __reserved__0                                                          : 13; // [31:19]
    } b;
  } u;
};

struct biu___reserved__98
{
  union {
#define biu___reserved__98_address 0x00000238
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu___reserved__99
{
  union {
#define biu___reserved__99_address 0x00000240
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct biu_SP_TL
{
  union {
#define biu_SP_TL_address 0x00000248
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct biu
{
  struct biu_CMD_PARAM0                                     CMD_PARAM0          ; //offset 0 (0)
  struct biu_CMD_PARAM1                                     CMD_PARAM1          ; //offset 4 (4)
  struct biu_CMD_PARAM2                                     CMD_PARAM2          ; //offset 8 (8)
  struct biu_CMD_PARAM3                                     CMD_PARAM3          ; //offset C (12)
  struct biu_CMD_PARAM4                                     CMD_PARAM4          ; //offset 10 (16)
  struct biu_CMD_PARAM5                                     CMD_PARAM5          ; //offset 14 (20)
  struct biu_CMD_PARAM6                                     CMD_PARAM6          ; //offset 18 (24)
  struct biu_CMD_PARAM7                                     CMD_PARAM7          ; //offset 1C (28)
  struct biu_CMD_PARAM8                                     CMD_PARAM8          ; //offset 20 (32)
  struct biu_CMD_PARAM9                                     CMD_PARAM9          ; //offset 24 (36)
  struct biu_CMD_PARAM10                                    CMD_PARAM10         ; //offset 28 (40)
  struct biu_CMD_PARAM11                                    CMD_PARAM11         ; //offset 2C (44)
  struct biu_CMD_PARAM12                                    CMD_PARAM12         ; //offset 30 (48)
  struct biu_CMD_PARAM13                                    CMD_PARAM13         ; //offset 34 (52)
  struct biu_CMD_PARAM14                                    CMD_PARAM14         ; //offset 38 (56)
  struct biu_CMD_PARAM15                                    CMD_PARAM15         ; //offset 3C (60)
  struct biu_CMD                                            CMD                 ; //offset 40 (64)
         uint32_t                                                __reserved__17      ; //offset 44 (68)
         uint64_t                                                __reserved__18      ; //offset 48 (72)
         uint64_t                                                __reserved__19      ; //offset 50 (80)
         uint64_t                                                __reserved__20      ; //offset 58 (88)
         uint64_t                                                __reserved__21      ; //offset 60 (96)
         uint64_t                                                __reserved__22      ; //offset 68 (104)
         uint64_t                                                __reserved__23      ; //offset 70 (112)
         uint64_t                                                __reserved__24      ; //offset 78 (120)
  struct biu_CMD_RET_STATUS                                 CMD_RET_STATUS      ; //offset 80 (128)
  struct biu_CMD_STATUS0                                    CMD_STATUS0         ; //offset 84 (132)
  struct biu_CMD_STATUS1                                    CMD_STATUS1         ; //offset 88 (136)
  struct biu_CMD_STATUS2                                    CMD_STATUS2         ; //offset 8C (140)
  struct biu_CMD_STATUS3                                    CMD_STATUS3         ; //offset 90 (144)
  struct biu_CMD_STATUS4                                    CMD_STATUS4         ; //offset 94 (148)
  struct biu_CMD_STATUS5                                    CMD_STATUS5         ; //offset 98 (152)
  struct biu_CMD_STATUS6                                    CMD_STATUS6         ; //offset 9C (156)
  struct biu_CMD_STATUS7                                    CMD_STATUS7         ; //offset A0 (160)
  struct biu_CMD_STATUS8                                    CMD_STATUS8         ; //offset A4 (164)
  struct biu_CMD_STATUS9                                    CMD_STATUS9         ; //offset A8 (168)
  struct biu_CMD_STATUS10                                   CMD_STATUS10        ; //offset AC (172)
  struct biu_CMD_STATUS11                                   CMD_STATUS11        ; //offset B0 (176)
  struct biu_CMD_STATUS12                                   CMD_STATUS12        ; //offset B4 (180)
  struct biu_CMD_STATUS13                                   CMD_STATUS13        ; //offset B8 (184)
  struct biu_CMD_STATUS14                                   CMD_STATUS14        ; //offset BC (188)
  struct biu_CMD_STATUS15                                   CMD_STATUS15        ; //offset C0 (192)
  struct biu_CMD_FIFO_STATUS                                CMD_FIFO_STATUS     ; //offset C4 (196)
  struct biu_HST_INTERRUPT_RST                              HST_INTERRUPT_RST   ; //offset C8 (200)
  struct biu_HST_INTERRUPT_MASK                             HST_INTERRUPT_MASK  ; //offset CC (204)
  struct biu_HST_EXCEPT_ADDR                                HST_EXCEPT_ADDR     ; //offset D0 (208)
  struct biu_HST_TRUST                                      HST_TRUST           ; //offset D4 (212)
  struct biu_WTM_ID                                         WTM_ID              ; //offset D8 (216)
  struct biu_WTM_REV                                        WTM_REV             ; //offset DC (220)
  struct biu_CONTEXT_STATUS                                 CONTEXT_STATUS      ; //offset E0 (224)
         uint32_t                                                __reserved__50      ; //offset E4 (228)
         uint64_t                                                __reserved__51      ; //offset E8 (232)
         uint64_t                                                __reserved__52      ; //offset F0 (240)
         uint64_t                                                __reserved__53      ; //offset F8 (248)
         uint64_t                                                __reserved__54      ; //offset 100 (256)
         uint64_t                                                __reserved__55      ; //offset 108 (264)
         uint64_t                                                __reserved__56      ; //offset 110 (272)
         uint64_t                                                __reserved__57      ; //offset 118 (280)
         uint64_t                                                __reserved__58      ; //offset 120 (288)
         uint64_t                                                __reserved__59      ; //offset 128 (296)
         uint64_t                                                __reserved__60      ; //offset 130 (304)
         uint64_t                                                __reserved__61      ; //offset 138 (312)
         uint64_t                                                __reserved__62      ; //offset 140 (320)
         uint64_t                                                __reserved__63      ; //offset 148 (328)
         uint64_t                                                __reserved__64      ; //offset 150 (336)
         uint64_t                                                __reserved__65      ; //offset 158 (344)
         uint64_t                                                __reserved__66      ; //offset 160 (352)
         uint64_t                                                __reserved__67      ; //offset 168 (360)
         uint64_t                                                __reserved__68      ; //offset 170 (368)
         uint64_t                                                __reserved__69      ; //offset 178 (376)
         uint64_t                                                __reserved__70      ; //offset 180 (384)
         uint64_t                                                __reserved__71      ; //offset 188 (392)
         uint64_t                                                __reserved__72      ; //offset 190 (400)
         uint64_t                                                __reserved__73      ; //offset 198 (408)
         uint64_t                                                __reserved__74      ; //offset 1A0 (416)
         uint64_t                                                __reserved__75      ; //offset 1A8 (424)
         uint64_t                                                __reserved__76      ; //offset 1B0 (432)
         uint64_t                                                __reserved__77      ; //offset 1B8 (440)
         uint64_t                                                __reserved__78      ; //offset 1C0 (448)
         uint64_t                                                __reserved__79      ; //offset 1C8 (456)
         uint64_t                                                __reserved__80      ; //offset 1D0 (464)
         uint64_t                                                __reserved__81      ; //offset 1D8 (472)
         uint64_t                                                __reserved__82      ; //offset 1E0 (480)
         uint64_t                                                __reserved__83      ; //offset 1E8 (488)
         uint64_t                                                __reserved__84      ; //offset 1F0 (496)
         uint64_t                                                __reserved__85      ; //offset 1F8 (504)
         uint64_t                                                __reserved__86      ; //offset 200 (512)
         uint64_t                                                __reserved__87      ; //offset 208 (520)
  struct biu_SP_INTERRUPT_SET                               SP_INTERRUPT_SET    ; //offset 210 (528)
  struct biu_SP_SRAM_CTRL                                   SP_SRAM_CTRL        ; //offset 214 (532)
  struct biu_SP_INTERRUPT_RST                               SP_INTERRUPT_RST    ; //offset 218 (536)
  struct biu_SP_INTERRUPT_MASK                              SP_INTERRUPT_MASK   ; //offset 21C (540)
  struct biu_SP_CONTROL                                     SP_CONTROL          ; //offset 220 (544)
  struct biu_SP_EXCEPT_ADDR                                 SP_EXCEPT_ADDR      ; //offset 224 (548)
  struct biu_SP_EXCEPT_DATA                                 SP_EXCEPT_DATA      ; //offset 228 (552)
  struct biu_SP_EXCEPT_PAR                                  SP_EXCEPT_PAR       ; //offset 22C (556)
         uint32_t                                                __reserved__96      ; //offset 230 (560)
  struct biu_HST_INTERRUPT_SET                              HST_INTERRUPT_SET   ; //offset 234 (564)
         uint64_t                                                __reserved__98      ; //offset 238 (568)
         uint64_t                                                __reserved__99      ; //offset 240 (576)
  struct biu_SP_TL                                          SP_TL               ; //offset 248 (584)
}; //biu


struct apb_adec_ADEC_CTRL
{
  union {
#define apb_adec_ADEC_CTRL_address 0x00000400
    uint32_t value;
    struct {
      uint32_t ACC_ENG0_RST                                                           :  1; // [ 0: 0]
      uint32_t ACC_ENG1_RST                                                           :  1; // [ 1: 1]
      uint32_t ACC_ENG2_RST                                                           :  1; // [ 2: 2]
      uint32_t ACC_ENG3_RST                                                           :  1; // [ 3: 3]
      uint32_t ACC_ENG4_RST                                                           :  1; // [ 4: 4]
      uint32_t ACC_ENG5_RST                                                           :  1; // [ 5: 5]
      uint32_t ACC_ENG6_RST                                                           :  1; // [ 6: 6]
      uint32_t ACC_ENG7_RST                                                           :  1; // [ 7: 7]
      uint32_t ACC_ENG8_RST                                                           :  1; // [ 8: 8]
      uint32_t ACC_ENG9_RST                                                           :  1; // [ 9: 9]
      uint32_t ACC_ENG10_RST                                                          :  1; // [10:10]
      uint32_t ACC_ENG11_RST                                                          :  1; // [11:11]
      uint32_t ACC_ENG12_RST                                                          :  1; // [12:12]
      uint32_t ACC_ENG13_RST                                                          :  1; // [13:13]
      uint32_t ACC_ENG14_RST                                                          :  1; // [14:14]
      uint32_t ACC_ENG15_RST                                                          :  1; // [15:15]
      uint32_t ACC_ENG0_CLK_EN                                                        :  1; // [16:16]
      uint32_t ACC_ENG1_CLK_EN                                                        :  1; // [17:17]
      uint32_t ACC_ENG2_CLK_EN                                                        :  1; // [18:18]
      uint32_t ACC_ENG3_CLK_EN                                                        :  1; // [19:19]
      uint32_t ACC_ENG4_CLK_EN                                                        :  1; // [20:20]
      uint32_t ACC_ENG5_CLK_EN                                                        :  1; // [21:21]
      uint32_t ACC_ENG6_CLK_EN                                                        :  1; // [22:22]
      uint32_t ACC_ENG7_CLK_EN                                                        :  1; // [23:23]
      uint32_t ACC_ENG8_CLK_EN                                                        :  1; // [24:24]
      uint32_t ACC_ENG9_CLK_EN                                                        :  1; // [25:25]
      uint32_t ACC_ENG10_CLK_EN                                                       :  1; // [26:26]
      uint32_t ACC_ENG11_CLK_EN                                                       :  1; // [27:27]
      uint32_t ACC_ENG12_CLK_EN                                                       :  1; // [28:28]
      uint32_t ACC_ENG13_CLK_EN                                                       :  1; // [29:29]
      uint32_t ACC_ENG14_CLK_EN                                                       :  1; // [30:30]
      uint32_t ACC_ENG15_CLK_EN                                                       :  1; // [31:31]
    } b;
  } u;
};

struct apb_adec_ADEC_CTRL2
{
  union {
#define apb_adec_ADEC_CTRL2_address 0x00000404
    uint32_t value;
    struct {
      uint32_t ACC_ENG16_RST                                                          :  1; // [ 0: 0]
      uint32_t ACC_ENG17_RST                                                          :  1; // [ 1: 1]
      uint32_t ACC_ENG18_RST                                                          :  1; // [ 2: 2]
      uint32_t ACC_ENG19_RST                                                          :  1; // [ 3: 3]
      uint32_t ACC_ENG20_RST                                                          :  1; // [ 4: 4]
      uint32_t __reserved__6                                                          : 11; // [15: 5]
      uint32_t ACC_ENG16_CLK_EN                                                       :  1; // [16:16]
      uint32_t ACC_ENG17_CLK_EN                                                       :  1; // [17:17]
      uint32_t ACC_ENG18_CLK_EN                                                       :  1; // [18:18]
      uint32_t ACC_ENG19_CLK_EN                                                       :  1; // [19:19]
      uint32_t ACC_ENG20_CLK_EN                                                       :  1; // [20:20]
      uint32_t __reserved__0                                                          : 11; // [31:21]
    } b;
  } u;
};

struct apb_adec_AXI_SL_CTRL
{
  union {
#define apb_adec_AXI_SL_CTRL_address 0x00000408
    uint32_t value;
    struct {
      uint32_t IN_AXI_SL_BYTE_SWP                                                     :  1; // [ 0: 0]
      uint32_t IN_AXI_SL_WORD_SWP                                                     :  1; // [ 1: 1]
      uint32_t OUT_AXI_SL_BYTE_SWP                                                    :  1; // [ 2: 2]
      uint32_t OUT_AXI_SL_WORD_SWP                                                    :  1; // [ 3: 3]
      uint32_t AXI_SL_BUS_SZ                                                          :  1; // [ 4: 4]
      uint32_t AXI_SL_RD_QENA                                                         :  1; // [ 5: 5]
      uint32_t AXI_SL_WR_POST_RESP_ENA                                                :  1; // [ 6: 6]
      uint32_t __reserved__7                                                          :  1; // [ 7: 7]
      uint32_t AXI_SL_WR_PROT                                                         :  3; // [10: 8]
      uint32_t __reserved__5                                                          :  1; // [11:11]
      uint32_t AXI_SL_RD_PROT                                                         :  3; // [14:12]
      uint32_t AHB_WR_SEQ_ALLOW                                                       :  1; // [15:15]
      uint32_t AXI_DATA_PAR_EVEN                                                      :  1; // [16:16]
      uint32_t AXI_DATA_PAR_EN                                                        :  1; // [17:17]
      uint32_t __reserved__0                                                          : 14; // [31:18]
    } b;
  } u;
};

struct apb_adec_ADEC_INT
{
  union {
#define apb_adec_ADEC_INT_address 0x0000040C
    uint32_t value;
    struct {
      uint32_t ACC_ENG0_INT                                                           :  1; // [ 0: 0]
      uint32_t ACC_ENG1_INT                                                           :  1; // [ 1: 1]
      uint32_t ACC_ENG2_INT                                                           :  1; // [ 2: 2]
      uint32_t ACC_ENG3_INT                                                           :  1; // [ 3: 3]
      uint32_t ACC_ENG4_INT                                                           :  1; // [ 4: 4]
      uint32_t ACC_ENG5_INT                                                           :  1; // [ 5: 5]
      uint32_t ACC_ENG6_INT                                                           :  1; // [ 6: 6]
      uint32_t ACC_ENG7_INT                                                           :  1; // [ 7: 7]
      uint32_t ACC_ENG8_INT                                                           :  1; // [ 8: 8]
      uint32_t ACC_ENG9_INT                                                           :  1; // [ 9: 9]
      uint32_t ACC_ENG10_INT                                                          :  1; // [10:10]
      uint32_t AXI_SL_APB_ERR                                                         :  1; // [11:11]
      uint32_t AXI_SL_BUS_ERR                                                         :  1; // [12:12]
      uint32_t APB_ACC_ERR                                                            :  1; // [13:13]
      uint32_t USER_INT0                                                              :  1; // [14:14]
      uint32_t USER_INT1                                                              :  1; // [15:15]
      uint32_t USER_INT2                                                              :  1; // [16:16]
      uint32_t USER_INT3                                                              :  1; // [17:17]
      uint32_t ACC_ENG11_INT                                                          :  1; // [18:18]
      uint32_t ACC_ENG12_INT                                                          :  1; // [19:19]
      uint32_t ACC_ENG13_INT                                                          :  1; // [20:20]
      uint32_t ACC_ENG14_INT                                                          :  1; // [21:21]
      uint32_t ACC_ENG15_INT                                                          :  1; // [22:22]
      uint32_t ACC_ENG16_INT                                                          :  1; // [23:23]
      uint32_t ACC_ENG17_INT                                                          :  1; // [24:24]
      uint32_t ACC_ENG18_INT                                                          :  1; // [25:25]
      uint32_t ACC_ENG19_INT                                                          :  1; // [26:26]
      uint32_t ACC_ENG20_INT                                                          :  1; // [27:27]
      uint32_t AXI_SL_AXI_PAR_ERR                                                     :  1; // [28:28]
      uint32_t __reserved__0                                                          :  3; // [31:29]
    } b;
  } u;
};

struct apb_adec_ADEC_INT_MSK
{
  union {
#define apb_adec_ADEC_INT_MSK_address 0x00000410
    uint32_t value;
    struct {
      uint32_t ACC_ENG0_INT_MSK                                                       :  1; // [ 0: 0]
      uint32_t ACC_ENG1_INT_MSK                                                       :  1; // [ 1: 1]
      uint32_t ACC_ENG2_INT_MSK                                                       :  1; // [ 2: 2]
      uint32_t ACC_ENG3_INT_MSK                                                       :  1; // [ 3: 3]
      uint32_t ACC_ENG4_INT_MSK                                                       :  1; // [ 4: 4]
      uint32_t ACC_ENG5_INT_MSK                                                       :  1; // [ 5: 5]
      uint32_t ACC_ENG6_INT_MSK                                                       :  1; // [ 6: 6]
      uint32_t ACC_ENG7_INT_MSK                                                       :  1; // [ 7: 7]
      uint32_t ACC_ENG8_INT_MSK                                                       :  1; // [ 8: 8]
      uint32_t ACC_ENG9_INT_MSK                                                       :  1; // [ 9: 9]
      uint32_t ACC_ENG10_INT_MSK                                                      :  1; // [10:10]
      uint32_t AXI_SL_APB_ERR_MSK                                                     :  1; // [11:11]
      uint32_t AXI_SL_BUS_ERR_MSK                                                     :  1; // [12:12]
      uint32_t APB_ACC_ERR_MSK                                                        :  1; // [13:13]
      uint32_t USER_INT0_MSK                                                          :  1; // [14:14]
      uint32_t USER_INT1_MSK                                                          :  1; // [15:15]
      uint32_t USER_INT2_MSK                                                          :  1; // [16:16]
      uint32_t USER_INT3_MSK                                                          :  1; // [17:17]
      uint32_t ACC_ENG11_INT_MSK                                                      :  1; // [18:18]
      uint32_t ACC_ENG12_INT_MSK                                                      :  1; // [19:19]
      uint32_t ACC_ENG13_INT_MSK                                                      :  1; // [20:20]
      uint32_t ACC_ENG14_INT_MSK                                                      :  1; // [21:21]
      uint32_t ACC_ENG15_INT_MSK                                                      :  1; // [22:22]
      uint32_t ACC_ENG16_INT_MSK                                                      :  1; // [23:23]
      uint32_t ACC_ENG17_INT_MSK                                                      :  1; // [24:24]
      uint32_t ACC_ENG18_INT_MSK                                                      :  1; // [25:25]
      uint32_t ACC_ENG19_INT_MSK                                                      :  1; // [26:26]
      uint32_t ACC_ENG20_INT_MSK                                                      :  1; // [27:27]
      uint32_t AXI_SL_AXI_PAR_ERR_MSK                                                 :  1; // [28:28]
      uint32_t __reserved__0                                                          :  3; // [31:29]
    } b;
  } u;
};

struct apb_adec_ACC_ERR_ADR
{
  union {
#define apb_adec_ACC_ERR_ADR_address 0x00000414
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct apb_adec_MP_FIFO_ERR_ADR
{
  union {
#define apb_adec_MP_FIFO_ERR_ADR_address 0x00000418
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct apb_adec
{
  struct apb_adec_ADEC_CTRL                                 ADEC_CTRL           ; //offset 0 (0)
  struct apb_adec_ADEC_CTRL2                                ADEC_CTRL2          ; //offset 4 (4)
  struct apb_adec_AXI_SL_CTRL                               AXI_SL_CTRL         ; //offset 8 (8)
  struct apb_adec_ADEC_INT                                  ADEC_INT            ; //offset C (12)
  struct apb_adec_ADEC_INT_MSK                              ADEC_INT_MSK        ; //offset 10 (16)
  struct apb_adec_ACC_ERR_ADR                               ACC_ERR_ADR         ; //offset 14 (20)
  struct apb_adec_MP_FIFO_ERR_ADR                           MP_FIFO_ERR_ADR     ; //offset 18 (24)
}; //apb_adec


struct dma_IN_DMA_CTRL
{
  union {
#define dma_IN_DMA_CTRL_address 0x00000800
    uint32_t value;
    struct {
      uint32_t IN_DMA_ENA                                                             :  1; // [ 0: 0]
      uint32_t IN_PROT_MODE                                                           :  3; // [ 3: 1]
      uint32_t IN_RID                                                                 :  4; // [ 7: 4]
      uint32_t IN_BURST_LEN                                                           :  5; // [12: 8]
      uint32_t IN_BURST_TYPE                                                          :  2; // [14:13]
      uint32_t IN_BUS_SZ                                                              :  1; // [15:15]
      uint32_t IN_REQ_PAUSE                                                           :  1; // [16:16]
      uint32_t IN_REQ_ALGN64                                                          :  1; // [17:17]
      uint32_t IN_ORDER_DIS                                                           :  1; // [18:18]
      uint32_t __reserved__6                                                          :  1; // [19:19]
      uint32_t IN_RID_EXT                                                             :  4; // [23:20]
      uint32_t IN_CACHE_MODE                                                          :  4; // [27:24]
      uint32_t IN_DIS_ERR_CODE                                                        :  1; // [28:28]
      uint32_t IN_DIS_LL_ERR_CODE                                                     :  1; // [29:29]
      uint32_t IN_DIS_REC_ERR_CODE                                                    :  1; // [30:30]
      uint32_t __reserved__0                                                          :  1; // [31:31]
    } b;
  } u;
};

struct dma_IN_DMA_STATUS
{
  union {
#define dma_IN_DMA_STATUS_address 0x00000804
    uint32_t value;
    struct {
      uint32_t RD_CYCLE_ERR                                                           :  2; // [ 1: 0]
      uint32_t PAR_ERR                                                                :  1; // [ 2: 2]
      uint32_t LL_ERR_CODE                                                            :  2; // [ 4: 3]
      uint32_t PAUSE_CMPL                                                             :  1; // [ 5: 5]
      uint32_t READ_PENDING                                                           :  1; // [ 6: 6]
      uint32_t __reserved__0                                                          : 25; // [31: 7]
    } b;
  } u;
};

struct dma_IN_SRC_ADR
{
  union {
#define dma_IN_SRC_ADR_address 0x00000808
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_IN_XFER_CNTR
{
  union {
#define dma_IN_XFER_CNTR_address 0x0000080C
    uint32_t value;
    struct {
      uint32_t XFER_CNTR                                                              : 28; // [27: 0]
      uint32_t __reserved__0                                                          :  4; // [31:28]
    } b;
  } u;
};

struct dma_IN_NX_LL_ADR
{
  union {
#define dma_IN_NX_LL_ADR_address 0x00000810
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_IN_DMA_INT1
{
  union {
#define dma_IN_DMA_INT1_address 0x00000814
    uint32_t value;
    struct {
      uint32_t IN_DMA_DONE                                                            :  1; // [ 0: 0]
      uint32_t IN_DMA_BUS_ERR                                                         :  1; // [ 1: 1]
      uint32_t IN_LL_ERR                                                              :  1; // [ 2: 2]
      uint32_t IN_PAR_ERR                                                             :  1; // [ 3: 3]
      uint32_t IN_DMA_PAUSE_CMPL                                                      :  1; // [ 4: 4]
      uint32_t AXI_DATA_PAR_ERR                                                       :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct dma_IN_DMA_INT1_MSK
{
  union {
#define dma_IN_DMA_INT1_MSK_address 0x00000818
    uint32_t value;
    struct {
      uint32_t IN_DMA_DONE_MSK                                                        :  1; // [ 0: 0]
      uint32_t IN_DMA_BUS_ERR_MSK                                                     :  1; // [ 1: 1]
      uint32_t IN_LL_ERR_MSK                                                          :  1; // [ 2: 2]
      uint32_t IN_PAR_ERR_MSK                                                         :  1; // [ 3: 3]
      uint32_t IN_DMA_PAUSE_CMPL_MSK                                                  :  1; // [ 4: 4]
      uint32_t AXI_DATA_PAR_ERR_MSK                                                   :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct dma_OUT_DMA_CTRL
{
  union {
#define dma_OUT_DMA_CTRL_address 0x0000081C
    uint32_t value;
    struct {
      uint32_t OUT_DMA_ENA                                                            :  1; // [ 0: 0]
      uint32_t OUT_PROT_MODE                                                          :  3; // [ 3: 1]
      uint32_t OUT_WID                                                                :  4; // [ 7: 4]
      uint32_t OUT_BURST_LEN                                                          :  5; // [12: 8]
      uint32_t OUT_BURST_TYPE                                                         :  2; // [14:13]
      uint32_t OUT_BUS_SZ                                                             :  1; // [15:15]
      uint32_t OUT_REQ_PAUSE                                                          :  1; // [16:16]
      uint32_t OUT_REQ_ALGN64                                                         :  1; // [17:17]
      uint32_t __reserved__6                                                          :  2; // [19:18]
      uint32_t OUT_WID_EXT                                                            :  4; // [23:20]
      uint32_t OUT_CACHE_MODE                                                         :  4; // [27:24]
      uint32_t OUT_DIS_ERR_CODE                                                       :  1; // [28:28]
      uint32_t OUT_DIS_LL_ERR_CODE                                                    :  1; // [29:29]
      uint32_t OUT_DIS_XMIT_ERR_CODE                                                  :  1; // [30:30]
      uint32_t __reserved__0                                                          :  1; // [31:31]
    } b;
  } u;
};

struct dma_OUT_DMA_STATUS
{
  union {
#define dma_OUT_DMA_STATUS_address 0x00000820
    uint32_t value;
    struct {
      uint32_t WR_CYCLE_ERR                                                           :  2; // [ 1: 0]
      uint32_t PAR_ERR                                                                :  1; // [ 2: 2]
      uint32_t LL_ERR_CODE                                                            :  2; // [ 4: 3]
      uint32_t PAUSE_CMPL                                                             :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct dma_OUT_DEST_ADR
{
  union {
#define dma_OUT_DEST_ADR_address 0x00000824
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_OUT_XFER_CNTR
{
  union {
#define dma_OUT_XFER_CNTR_address 0x00000828
    uint32_t value;
    struct {
      uint32_t XFER_CNTR                                                              : 28; // [27: 0]
      uint32_t __reserved__0                                                          :  4; // [31:28]
    } b;
  } u;
};

struct dma_OUT_NX_LL_ADR
{
  union {
#define dma_OUT_NX_LL_ADR_address 0x0000082C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_OUT_DMA_INT1
{
  union {
#define dma_OUT_DMA_INT1_address 0x00000830
    uint32_t value;
    struct {
      uint32_t OUT_DMA_DONE                                                           :  1; // [ 0: 0]
      uint32_t OUT_DMA_BUS_ERR                                                        :  1; // [ 1: 1]
      uint32_t OUT_LL_ERR                                                             :  1; // [ 2: 2]
      uint32_t OUT_PAR_ERR                                                            :  1; // [ 3: 3]
      uint32_t OUT_DMA_PAUSE_CMPL                                                     :  1; // [ 4: 4]
      uint32_t AXI_DATA_PAR_ERR                                                       :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct dma_OUT_DMA_INT1_MSK
{
  union {
#define dma_OUT_DMA_INT1_MSK_address 0x00000834
    uint32_t value;
    struct {
      uint32_t OUT_DMA_DONE_MSK                                                       :  1; // [ 0: 0]
      uint32_t OUT_DMA_BUS_ERR_MSK                                                    :  1; // [ 1: 1]
      uint32_t OUT_LL_ERR_MSK                                                         :  1; // [ 2: 2]
      uint32_t OUT_PAR_ERR_MSK                                                        :  1; // [ 3: 3]
      uint32_t OUT_DMA_PAUSE_CMPL_MSK                                                 :  1; // [ 4: 4]
      uint32_t DATA_PAR_ERR_MSK                                                       :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct dma_AXI_CTRL
{
  union {
#define dma_AXI_CTRL_address 0x00000838
    uint32_t value;
    struct {
      uint32_t IN_AXI_MS_BYTE_SWP                                                     :  1; // [ 0: 0]
      uint32_t IN_AXI_MS_WRD_SWP                                                      :  1; // [ 1: 1]
      uint32_t OUT_AXI_MS_BYTE_SWP                                                    :  1; // [ 2: 2]
      uint32_t OUT_AXI_MS_WRD_SWP                                                     :  1; // [ 3: 3]
      uint32_t DMA_LOOP_BACK_EN                                                       :  1; // [ 4: 4]
      uint32_t LINK_LIST_SWAP_DIS                                                     :  1; // [ 5: 5]
      uint32_t AXI_RDATA_PAR_EVEN                                                     :  1; // [ 6: 6]
      uint32_t AXI_WDATA_PAR_EVEN                                                     :  1; // [ 7: 7]
      uint32_t AXI_DATA_PAR_ENA                                                       :  1; // [ 8: 8]
      uint32_t __reserved__2                                                          :  7; // [15: 9]
      uint32_t AXI_XMIT_RESP_MAX                                                      :  3; // [18:16]
      uint32_t __reserved__0                                                          : 13; // [31:19]
    } b;
  } u;
};

struct dma_IF_RCOUNT
{
  union {
#define dma_IF_RCOUNT_address 0x0000083C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_IF_RD_PTR_ERR
{
  union {
#define dma_IF_RD_PTR_ERR_address 0x00000840
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_OF_SPACE
{
  union {
#define dma_OF_SPACE_address 0x00000844
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_OF_RD_PTR_ERR
{
  union {
#define dma_OF_RD_PTR_ERR_address 0x00000848
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_IN_SRC_ADR2
{
  union {
#define dma_IN_SRC_ADR2_address 0x0000084C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_IN_NX_LL_ADR2
{
  union {
#define dma_IN_NX_LL_ADR2_address 0x00000850
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_OUT_DEST_ADR2
{
  union {
#define dma_OUT_DEST_ADR2_address 0x00000854
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_OUT_NX_LL_ADR2
{
  union {
#define dma_OUT_NX_LL_ADR2_address 0x00000858
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__23
{
  union {
#define dma___reserved__23_address 0x0000085C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma_IN_DMA_CTRL2
{
  union {
#define dma_IN_DMA_CTRL2_address 0x00000860
    uint32_t value;
    struct {
      uint32_t IN_BURST_LEN_EXT                                                       :  4; // [ 3: 0]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct dma_OUT_DMA_CTRL2
{
  union {
#define dma_OUT_DMA_CTRL2_address 0x00000864
    uint32_t value;
    struct {
      uint32_t OUT_BURST_LEN_EXT                                                      :  4; // [ 3: 0]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct dma___reserved__26
{
  union {
#define dma___reserved__26_address 0x00000868
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__27
{
  union {
#define dma___reserved__27_address 0x00000870
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__28
{
  union {
#define dma___reserved__28_address 0x00000878
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__29
{
  union {
#define dma___reserved__29_address 0x00000880
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__30
{
  union {
#define dma___reserved__30_address 0x00000888
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__31
{
  union {
#define dma___reserved__31_address 0x00000890
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__32
{
  union {
#define dma___reserved__32_address 0x00000898
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__33
{
  union {
#define dma___reserved__33_address 0x000008A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__34
{
  union {
#define dma___reserved__34_address 0x000008A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__35
{
  union {
#define dma___reserved__35_address 0x000008B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__36
{
  union {
#define dma___reserved__36_address 0x000008B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__37
{
  union {
#define dma___reserved__37_address 0x000008C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__38
{
  union {
#define dma___reserved__38_address 0x000008C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__39
{
  union {
#define dma___reserved__39_address 0x000008D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__40
{
  union {
#define dma___reserved__40_address 0x000008D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__41
{
  union {
#define dma___reserved__41_address 0x000008E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__42
{
  union {
#define dma___reserved__42_address 0x000008E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__43
{
  union {
#define dma___reserved__43_address 0x000008F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__44
{
  union {
#define dma___reserved__44_address 0x000008F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma_IF_RAM
{
  union {
#define dma_IF_RAM_address 0x00000900
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__46
{
  union {
#define dma___reserved__46_address 0x00000904
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__47
{
  union {
#define dma___reserved__47_address 0x00000908
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__48
{
  union {
#define dma___reserved__48_address 0x00000910
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__49
{
  union {
#define dma___reserved__49_address 0x00000918
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__50
{
  union {
#define dma___reserved__50_address 0x00000920
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__51
{
  union {
#define dma___reserved__51_address 0x00000928
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__52
{
  union {
#define dma___reserved__52_address 0x00000930
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__53
{
  union {
#define dma___reserved__53_address 0x00000938
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__54
{
  union {
#define dma___reserved__54_address 0x00000940
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__55
{
  union {
#define dma___reserved__55_address 0x00000948
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__56
{
  union {
#define dma___reserved__56_address 0x00000950
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__57
{
  union {
#define dma___reserved__57_address 0x00000958
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__58
{
  union {
#define dma___reserved__58_address 0x00000960
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__59
{
  union {
#define dma___reserved__59_address 0x00000968
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__60
{
  union {
#define dma___reserved__60_address 0x00000970
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__61
{
  union {
#define dma___reserved__61_address 0x00000978
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__62
{
  union {
#define dma___reserved__62_address 0x00000980
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__63
{
  union {
#define dma___reserved__63_address 0x00000988
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__64
{
  union {
#define dma___reserved__64_address 0x00000990
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__65
{
  union {
#define dma___reserved__65_address 0x00000998
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__66
{
  union {
#define dma___reserved__66_address 0x000009A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__67
{
  union {
#define dma___reserved__67_address 0x000009A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__68
{
  union {
#define dma___reserved__68_address 0x000009B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__69
{
  union {
#define dma___reserved__69_address 0x000009B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__70
{
  union {
#define dma___reserved__70_address 0x000009C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__71
{
  union {
#define dma___reserved__71_address 0x000009C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__72
{
  union {
#define dma___reserved__72_address 0x000009D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__73
{
  union {
#define dma___reserved__73_address 0x000009D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__74
{
  union {
#define dma___reserved__74_address 0x000009E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__75
{
  union {
#define dma___reserved__75_address 0x000009E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__76
{
  union {
#define dma___reserved__76_address 0x000009F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__77
{
  union {
#define dma___reserved__77_address 0x000009F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__78
{
  union {
#define dma___reserved__78_address 0x00000A00
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__79
{
  union {
#define dma___reserved__79_address 0x00000A08
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__80
{
  union {
#define dma___reserved__80_address 0x00000A10
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__81
{
  union {
#define dma___reserved__81_address 0x00000A18
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__82
{
  union {
#define dma___reserved__82_address 0x00000A20
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__83
{
  union {
#define dma___reserved__83_address 0x00000A28
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__84
{
  union {
#define dma___reserved__84_address 0x00000A30
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__85
{
  union {
#define dma___reserved__85_address 0x00000A38
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__86
{
  union {
#define dma___reserved__86_address 0x00000A40
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__87
{
  union {
#define dma___reserved__87_address 0x00000A48
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__88
{
  union {
#define dma___reserved__88_address 0x00000A50
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__89
{
  union {
#define dma___reserved__89_address 0x00000A58
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__90
{
  union {
#define dma___reserved__90_address 0x00000A60
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__91
{
  union {
#define dma___reserved__91_address 0x00000A68
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__92
{
  union {
#define dma___reserved__92_address 0x00000A70
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__93
{
  union {
#define dma___reserved__93_address 0x00000A78
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__94
{
  union {
#define dma___reserved__94_address 0x00000A80
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__95
{
  union {
#define dma___reserved__95_address 0x00000A88
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__96
{
  union {
#define dma___reserved__96_address 0x00000A90
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__97
{
  union {
#define dma___reserved__97_address 0x00000A98
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__98
{
  union {
#define dma___reserved__98_address 0x00000AA0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__99
{
  union {
#define dma___reserved__99_address 0x00000AA8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__100
{
  union {
#define dma___reserved__100_address 0x00000AB0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__101
{
  union {
#define dma___reserved__101_address 0x00000AB8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__102
{
  union {
#define dma___reserved__102_address 0x00000AC0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__103
{
  union {
#define dma___reserved__103_address 0x00000AC8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__104
{
  union {
#define dma___reserved__104_address 0x00000AD0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__105
{
  union {
#define dma___reserved__105_address 0x00000AD8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__106
{
  union {
#define dma___reserved__106_address 0x00000AE0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__107
{
  union {
#define dma___reserved__107_address 0x00000AE8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__108
{
  union {
#define dma___reserved__108_address 0x00000AF0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma___reserved__109
{
  union {
#define dma___reserved__109_address 0x00000AF8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct dma_OF_RAM
{
  union {
#define dma_OF_RAM_address 0x00000B00
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct dma
{
  struct dma_IN_DMA_CTRL                                    IN_DMA_CTRL         ; //offset 0 (0)
  struct dma_IN_DMA_STATUS                                  IN_DMA_STATUS       ; //offset 4 (4)
  struct dma_IN_SRC_ADR                                     IN_SRC_ADR          ; //offset 8 (8)
  struct dma_IN_XFER_CNTR                                   IN_XFER_CNTR        ; //offset C (12)
  struct dma_IN_NX_LL_ADR                                   IN_NX_LL_ADR        ; //offset 10 (16)
  struct dma_IN_DMA_INT1                                    IN_DMA_INT1         ; //offset 14 (20)
  struct dma_IN_DMA_INT1_MSK                                IN_DMA_INT1_MSK     ; //offset 18 (24)
  struct dma_OUT_DMA_CTRL                                   OUT_DMA_CTRL        ; //offset 1C (28)
  struct dma_OUT_DMA_STATUS                                 OUT_DMA_STATUS      ; //offset 20 (32)
  struct dma_OUT_DEST_ADR                                   OUT_DEST_ADR        ; //offset 24 (36)
  struct dma_OUT_XFER_CNTR                                  OUT_XFER_CNTR       ; //offset 28 (40)
  struct dma_OUT_NX_LL_ADR                                  OUT_NX_LL_ADR       ; //offset 2C (44)
  struct dma_OUT_DMA_INT1                                   OUT_DMA_INT1        ; //offset 30 (48)
  struct dma_OUT_DMA_INT1_MSK                               OUT_DMA_INT1_MSK    ; //offset 34 (52)
  struct dma_AXI_CTRL                                       AXI_CTRL            ; //offset 38 (56)
  struct dma_IF_RCOUNT                                      IF_RCOUNT           ; //offset 3C (60)
  struct dma_IF_RD_PTR_ERR                                  IF_RD_PTR_ERR       ; //offset 40 (64)
  struct dma_OF_SPACE                                       OF_SPACE            ; //offset 44 (68)
  struct dma_OF_RD_PTR_ERR                                  OF_RD_PTR_ERR       ; //offset 48 (72)
         uint32_t                                                __reserved__19      ; //offset 4C (76)
         uint32_t                                                __reserved__20      ; //offset 50 (80)
         uint32_t                                                __reserved__21      ; //offset 54 (84)
         uint32_t                                                __reserved__22      ; //offset 58 (88)
         uint32_t                                                __reserved__23      ; //offset 5C (92)
  struct dma_IN_DMA_CTRL2                                   IN_DMA_CTRL2        ; //offset 60 (96)
  struct dma_OUT_DMA_CTRL2                                  OUT_DMA_CTRL2       ; //offset 64 (100)
         uint64_t                                                __reserved__26      ; //offset 68 (104)
         uint64_t                                                __reserved__27      ; //offset 70 (112)
         uint64_t                                                __reserved__28      ; //offset 78 (120)
         uint64_t                                                __reserved__29      ; //offset 80 (128)
         uint64_t                                                __reserved__30      ; //offset 88 (136)
         uint64_t                                                __reserved__31      ; //offset 90 (144)
         uint64_t                                                __reserved__32      ; //offset 98 (152)
         uint64_t                                                __reserved__33      ; //offset A0 (160)
         uint64_t                                                __reserved__34      ; //offset A8 (168)
         uint64_t                                                __reserved__35      ; //offset B0 (176)
         uint64_t                                                __reserved__36      ; //offset B8 (184)
         uint64_t                                                __reserved__37      ; //offset C0 (192)
         uint64_t                                                __reserved__38      ; //offset C8 (200)
         uint64_t                                                __reserved__39      ; //offset D0 (208)
         uint64_t                                                __reserved__40      ; //offset D8 (216)
         uint64_t                                                __reserved__41      ; //offset E0 (224)
         uint64_t                                                __reserved__42      ; //offset E8 (232)
         uint64_t                                                __reserved__43      ; //offset F0 (240)
         uint64_t                                                __reserved__44      ; //offset F8 (248)
  struct dma_IF_RAM                                         IF_RAM              ; //offset 100 (256)
         uint32_t                                                __reserved__46      ; //offset 104 (260)
         uint64_t                                                __reserved__47      ; //offset 108 (264)
         uint64_t                                                __reserved__48      ; //offset 110 (272)
         uint64_t                                                __reserved__49      ; //offset 118 (280)
         uint64_t                                                __reserved__50      ; //offset 120 (288)
         uint64_t                                                __reserved__51      ; //offset 128 (296)
         uint64_t                                                __reserved__52      ; //offset 130 (304)
         uint64_t                                                __reserved__53      ; //offset 138 (312)
         uint64_t                                                __reserved__54      ; //offset 140 (320)
         uint64_t                                                __reserved__55      ; //offset 148 (328)
         uint64_t                                                __reserved__56      ; //offset 150 (336)
         uint64_t                                                __reserved__57      ; //offset 158 (344)
         uint64_t                                                __reserved__58      ; //offset 160 (352)
         uint64_t                                                __reserved__59      ; //offset 168 (360)
         uint64_t                                                __reserved__60      ; //offset 170 (368)
         uint64_t                                                __reserved__61      ; //offset 178 (376)
         uint64_t                                                __reserved__62      ; //offset 180 (384)
         uint64_t                                                __reserved__63      ; //offset 188 (392)
         uint64_t                                                __reserved__64      ; //offset 190 (400)
         uint64_t                                                __reserved__65      ; //offset 198 (408)
         uint64_t                                                __reserved__66      ; //offset 1A0 (416)
         uint64_t                                                __reserved__67      ; //offset 1A8 (424)
         uint64_t                                                __reserved__68      ; //offset 1B0 (432)
         uint64_t                                                __reserved__69      ; //offset 1B8 (440)
         uint64_t                                                __reserved__70      ; //offset 1C0 (448)
         uint64_t                                                __reserved__71      ; //offset 1C8 (456)
         uint64_t                                                __reserved__72      ; //offset 1D0 (464)
         uint64_t                                                __reserved__73      ; //offset 1D8 (472)
         uint64_t                                                __reserved__74      ; //offset 1E0 (480)
         uint64_t                                                __reserved__75      ; //offset 1E8 (488)
         uint64_t                                                __reserved__76      ; //offset 1F0 (496)
         uint64_t                                                __reserved__77      ; //offset 1F8 (504)
         uint64_t                                                __reserved__78      ; //offset 200 (512)
         uint64_t                                                __reserved__79      ; //offset 208 (520)
         uint64_t                                                __reserved__80      ; //offset 210 (528)
         uint64_t                                                __reserved__81      ; //offset 218 (536)
         uint64_t                                                __reserved__82      ; //offset 220 (544)
         uint64_t                                                __reserved__83      ; //offset 228 (552)
         uint64_t                                                __reserved__84      ; //offset 230 (560)
         uint64_t                                                __reserved__85      ; //offset 238 (568)
         uint64_t                                                __reserved__86      ; //offset 240 (576)
         uint64_t                                                __reserved__87      ; //offset 248 (584)
         uint64_t                                                __reserved__88      ; //offset 250 (592)
         uint64_t                                                __reserved__89      ; //offset 258 (600)
         uint64_t                                                __reserved__90      ; //offset 260 (608)
         uint64_t                                                __reserved__91      ; //offset 268 (616)
         uint64_t                                                __reserved__92      ; //offset 270 (624)
         uint64_t                                                __reserved__93      ; //offset 278 (632)
         uint64_t                                                __reserved__94      ; //offset 280 (640)
         uint64_t                                                __reserved__95      ; //offset 288 (648)
         uint64_t                                                __reserved__96      ; //offset 290 (656)
         uint64_t                                                __reserved__97      ; //offset 298 (664)
         uint64_t                                                __reserved__98      ; //offset 2A0 (672)
         uint64_t                                                __reserved__99      ; //offset 2A8 (680)
         uint64_t                                                __reserved__100     ; //offset 2B0 (688)
         uint64_t                                                __reserved__101     ; //offset 2B8 (696)
         uint64_t                                                __reserved__102     ; //offset 2C0 (704)
         uint64_t                                                __reserved__103     ; //offset 2C8 (712)
         uint64_t                                                __reserved__104     ; //offset 2D0 (720)
         uint64_t                                                __reserved__105     ; //offset 2D8 (728)
         uint64_t                                                __reserved__106     ; //offset 2E0 (736)
         uint64_t                                                __reserved__107     ; //offset 2E8 (744)
         uint64_t                                                __reserved__108     ; //offset 2F0 (752)
         uint64_t                                                __reserved__109     ; //offset 2F8 (760)
  struct dma_OF_RAM                                         OF_RAM              ; //offset 300 (768)
}; //dma


struct mahb_IN_DMA_CTRL
{
  union {
#define mahb_IN_DMA_CTRL_address 0x00000800
    uint32_t value;
    struct {
      uint32_t IN_DMA_ENA                                                             :  1; // [ 0: 0]
      uint32_t IN_PROT_MODE                                                           :  4; // [ 4: 1]
      uint32_t IN_BURST_LEN                                                           :  5; // [ 9: 5]
      uint32_t IN_USE_LL                                                              :  1; // [10:10]
      uint32_t IN_LOCK                                                                :  1; // [11:11]
      uint32_t IN_REQ_PAUSE                                                           :  1; // [12:12]
      uint32_t __reserved__0                                                          : 19; // [31:13]
    } b;
  } u;
};

struct mahb_IN_SRC_ADR
{
  union {
#define mahb_IN_SRC_ADR_address 0x00000804
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb_IN_XFER_CNTR
{
  union {
#define mahb_IN_XFER_CNTR_address 0x00000808
    uint32_t value;
    struct {
      uint32_t IN_XFER_CNTR                                                           : 28; // [27: 0]
      uint32_t __reserved__0                                                          :  4; // [31:28]
    } b;
  } u;
};

struct mahb_IN_DMA_INT1
{
  union {
#define mahb_IN_DMA_INT1_address 0x0000080C
    uint32_t value;
    struct {
      uint32_t IN_DMA_DONE                                                            :  1; // [ 0: 0]
      uint32_t IN_DMA_BUS_ERR                                                         :  1; // [ 1: 1]
      uint32_t IN_PAR_ERR                                                             :  1; // [ 2: 2]
      uint32_t IN_PAUSE_CMPL                                                          :  1; // [ 3: 3]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct mahb_IN_DMA_INT1_MSK
{
  union {
#define mahb_IN_DMA_INT1_MSK_address 0x00000810
    uint32_t value;
    struct {
      uint32_t IN_DMA_DONE_MSK                                                        :  1; // [ 0: 0]
      uint32_t IN_DMA_BUS_ERR_MSK                                                     :  1; // [ 1: 1]
      uint32_t IN_PAR_ERR_MSK                                                         :  1; // [ 2: 2]
      uint32_t IN_PAUSE_CMPL_MSK                                                      :  1; // [ 3: 3]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct mahb___reserved__5
{
  union {
#define mahb___reserved__5_address 0x00000814
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__6
{
  union {
#define mahb___reserved__6_address 0x00000818
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb_OUT_DMA_CTRL
{
  union {
#define mahb_OUT_DMA_CTRL_address 0x00000820
    uint32_t value;
    struct {
      uint32_t OUT_DMA_ENA                                                            :  1; // [ 0: 0]
      uint32_t OUT_PROT_MODE                                                          :  4; // [ 4: 1]
      uint32_t OUT_BURST_LEN                                                          :  5; // [ 9: 5]
      uint32_t OUT_USE_LL                                                             :  1; // [10:10]
      uint32_t OUT_LOCK                                                               :  1; // [11:11]
      uint32_t OUT_REQ_PAUSE                                                          :  1; // [12:12]
      uint32_t __reserved__0                                                          : 19; // [31:13]
    } b;
  } u;
};

struct mahb_OUT_DEST_ADR
{
  union {
#define mahb_OUT_DEST_ADR_address 0x00000824
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb_OUT_XFER_CNTR
{
  union {
#define mahb_OUT_XFER_CNTR_address 0x00000828
    uint32_t value;
    struct {
      uint32_t OUT_XFER_CNTR                                                          : 28; // [27: 0]
      uint32_t __reserved__0                                                          :  4; // [31:28]
    } b;
  } u;
};

struct mahb_OUT_DMA_INT1
{
  union {
#define mahb_OUT_DMA_INT1_address 0x0000082C
    uint32_t value;
    struct {
      uint32_t OUT_DMA_DONE                                                           :  1; // [ 0: 0]
      uint32_t OUT_DMA_BUS_ERR                                                        :  1; // [ 1: 1]
      uint32_t OUT_PAR_ERR                                                            :  1; // [ 2: 2]
      uint32_t OUT_PAUSE_CMPL                                                         :  1; // [ 3: 3]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct mahb_OUT_DMA_INT1_MSK
{
  union {
#define mahb_OUT_DMA_INT1_MSK_address 0x00000830
    uint32_t value;
    struct {
      uint32_t OUT_DMA_DONE_MSK                                                       :  1; // [ 0: 0]
      uint32_t OUT_DMA_BUS_ERR_MSK                                                    :  1; // [ 1: 1]
      uint32_t OUT_PAR_ERR_MSK                                                        :  1; // [ 2: 2]
      uint32_t OUT_PAUSE_CMPL_MSK                                                     :  1; // [ 3: 3]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct mahb___reserved__12
{
  union {
#define mahb___reserved__12_address 0x00000834
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__13
{
  union {
#define mahb___reserved__13_address 0x00000838
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__14
{
  union {
#define mahb___reserved__14_address 0x00000840
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__15
{
  union {
#define mahb___reserved__15_address 0x00000848
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__16
{
  union {
#define mahb___reserved__16_address 0x00000850
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__17
{
  union {
#define mahb___reserved__17_address 0x00000858
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__18
{
  union {
#define mahb___reserved__18_address 0x00000860
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__19
{
  union {
#define mahb___reserved__19_address 0x00000868
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__20
{
  union {
#define mahb___reserved__20_address 0x00000870
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__21
{
  union {
#define mahb___reserved__21_address 0x00000878
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__22
{
  union {
#define mahb___reserved__22_address 0x00000880
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__23
{
  union {
#define mahb___reserved__23_address 0x00000888
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__24
{
  union {
#define mahb___reserved__24_address 0x00000890
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__25
{
  union {
#define mahb___reserved__25_address 0x00000898
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__26
{
  union {
#define mahb___reserved__26_address 0x000008A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__27
{
  union {
#define mahb___reserved__27_address 0x000008A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__28
{
  union {
#define mahb___reserved__28_address 0x000008B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__29
{
  union {
#define mahb___reserved__29_address 0x000008B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__30
{
  union {
#define mahb___reserved__30_address 0x000008C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__31
{
  union {
#define mahb___reserved__31_address 0x000008C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__32
{
  union {
#define mahb___reserved__32_address 0x000008D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__33
{
  union {
#define mahb___reserved__33_address 0x000008D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__34
{
  union {
#define mahb___reserved__34_address 0x000008E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__35
{
  union {
#define mahb___reserved__35_address 0x000008E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__36
{
  union {
#define mahb___reserved__36_address 0x000008F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb___reserved__37
{
  union {
#define mahb___reserved__37_address 0x000008F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct mahb_IF_RAM_WR_PTR
{
  union {
#define mahb_IF_RAM_WR_PTR_address 0x00000900
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb_IF_RAM_RD_PTR
{
  union {
#define mahb_IF_RAM_RD_PTR_address 0x00000904
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb_IF_RAM_DATA
{
  union {
#define mahb_IF_RAM_DATA_address 0x00000908
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb_OF_RAM_WR_PTR
{
  union {
#define mahb_OF_RAM_WR_PTR_address 0x0000090C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb_OF_RAM_RD_PTR
{
  union {
#define mahb_OF_RAM_RD_PTR_address 0x00000910
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb_OF_RAM_DATA
{
  union {
#define mahb_OF_RAM_DATA_address 0x00000914
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mahb
{
  struct mahb_IN_DMA_CTRL                                   IN_DMA_CTRL         ; //offset 0 (0)
  struct mahb_IN_SRC_ADR                                    IN_SRC_ADR          ; //offset 4 (4)
  struct mahb_IN_XFER_CNTR                                  IN_XFER_CNTR        ; //offset 8 (8)
  struct mahb_IN_DMA_INT1                                   IN_DMA_INT1         ; //offset C (12)
  struct mahb_IN_DMA_INT1_MSK                               IN_DMA_INT1_MSK     ; //offset 10 (16)
         uint32_t                                                __reserved__5       ; //offset 14 (20)
         uint64_t                                                __reserved__6       ; //offset 18 (24)
  struct mahb_OUT_DMA_CTRL                                  OUT_DMA_CTRL        ; //offset 20 (32)
  struct mahb_OUT_DEST_ADR                                  OUT_DEST_ADR        ; //offset 24 (36)
  struct mahb_OUT_XFER_CNTR                                 OUT_XFER_CNTR       ; //offset 28 (40)
  struct mahb_OUT_DMA_INT1                                  OUT_DMA_INT1        ; //offset 2C (44)
  struct mahb_OUT_DMA_INT1_MSK                              OUT_DMA_INT1_MSK    ; //offset 30 (48)
         uint32_t                                                __reserved__12      ; //offset 34 (52)
         uint64_t                                                __reserved__13      ; //offset 38 (56)
         uint64_t                                                __reserved__14      ; //offset 40 (64)
         uint64_t                                                __reserved__15      ; //offset 48 (72)
         uint64_t                                                __reserved__16      ; //offset 50 (80)
         uint64_t                                                __reserved__17      ; //offset 58 (88)
         uint64_t                                                __reserved__18      ; //offset 60 (96)
         uint64_t                                                __reserved__19      ; //offset 68 (104)
         uint64_t                                                __reserved__20      ; //offset 70 (112)
         uint64_t                                                __reserved__21      ; //offset 78 (120)
         uint64_t                                                __reserved__22      ; //offset 80 (128)
         uint64_t                                                __reserved__23      ; //offset 88 (136)
         uint64_t                                                __reserved__24      ; //offset 90 (144)
         uint64_t                                                __reserved__25      ; //offset 98 (152)
         uint64_t                                                __reserved__26      ; //offset A0 (160)
         uint64_t                                                __reserved__27      ; //offset A8 (168)
         uint64_t                                                __reserved__28      ; //offset B0 (176)
         uint64_t                                                __reserved__29      ; //offset B8 (184)
         uint64_t                                                __reserved__30      ; //offset C0 (192)
         uint64_t                                                __reserved__31      ; //offset C8 (200)
         uint64_t                                                __reserved__32      ; //offset D0 (208)
         uint64_t                                                __reserved__33      ; //offset D8 (216)
         uint64_t                                                __reserved__34      ; //offset E0 (224)
         uint64_t                                                __reserved__35      ; //offset E8 (232)
         uint64_t                                                __reserved__36      ; //offset F0 (240)
         uint64_t                                                __reserved__37      ; //offset F8 (248)
  struct mahb_IF_RAM_WR_PTR                                 IF_RAM_WR_PTR       ; //offset 100 (256)
  struct mahb_IF_RAM_RD_PTR                                 IF_RAM_RD_PTR       ; //offset 104 (260)
  struct mahb_IF_RAM_DATA                                   IF_RAM_DATA         ; //offset 108 (264)
  struct mahb_OF_RAM_WR_PTR                                 OF_RAM_WR_PTR       ; //offset 10C (268)
  struct mahb_OF_RAM_RD_PTR                                 OF_RAM_RD_PTR       ; //offset 110 (272)
  struct mahb_OF_RAM_DATA                                   OF_RAM_DATA         ; //offset 114 (276)
}; //mahb


struct accel_bus_reg_accel_bus_ctrl
{
  union {
#define accel_bus_reg_accel_bus_ctrl_address 0x00000C00
    uint32_t value;
    struct {
      uint32_t rd_cb_ctrl                                                             :  1; // [ 0: 0]
      uint32_t __reserved__6                                                          :  1; // [ 1: 1]
      uint32_t wr_cb_ctrl                                                             :  1; // [ 2: 2]
      uint32_t __reserved__4                                                          :  1; // [ 3: 3]
      uint32_t GroupA_Select                                                          :  3; // [ 6: 4]
      uint32_t __reserved__2                                                          :  1; // [ 7: 7]
      uint32_t GroupB_Select                                                          :  3; // [10: 8]
      uint32_t __reserved__0                                                          : 21; // [31:11]
    } b;
  } u;
};

struct accel_bus_reg
{
  struct accel_bus_reg_accel_bus_ctrl                       accel_bus_ctrl      ; //offset 0 (0)
}; //accel_bus_reg


struct abus_ABUS_CFG
{
  union {
#define abus_ABUS_CFG_address 0x00000C00
    uint32_t value;
    struct {
      uint32_t RdCrossBar_Sel                                                         :  1; // [ 0: 0]
      uint32_t __reserved__8                                                          :  1; // [ 1: 1]
      uint32_t WrCrossBar_Sel                                                         :  1; // [ 2: 2]
      uint32_t __reserved__6                                                          :  1; // [ 3: 3]
      uint32_t GroupA_Sel                                                             :  2; // [ 5: 4]
      uint32_t __reserved__4                                                          :  2; // [ 7: 6]
      uint32_t GroupB_Sel                                                             :  2; // [ 9: 8]
      uint32_t __reserved__2                                                          :  2; // [11:10]
      uint32_t GroupC_Sel                                                             :  2; // [13:12]
      uint32_t __reserved__0                                                          : 18; // [31:14]
    } b;
  } u;
};

struct abus
{
  struct abus_ABUS_CFG                                      ABUS_CFG            ; //offset 0 (0)
}; //abus


struct crypt_AES_CFG_REG
{
  union {
#define crypt_AES_CFG_REG_address 0x00001000
    uint32_t value;
    struct {
      uint32_t Decrypt                                                                :  1; // [ 0: 0]
      uint32_t KeyLen                                                                 :  2; // [ 2: 1]
      uint32_t Mode                                                                   :  3; // [ 5: 3]
      uint32_t RKEY                                                                   :  1; // [ 6: 6]
      uint32_t __reserved__4                                                          :  1; // [ 7: 7]
      uint32_t Modular                                                                :  7; // [14: 8]
      uint32_t __reserved__2                                                          :  1; // [15:15]
      uint32_t Cts_mode                                                               :  1; // [16:16]
      uint32_t __reserved__0                                                          : 15; // [31:17]
    } b;
  } u;
};

struct crypt_AES_CTRL_REG
{
  union {
#define crypt_AES_CTRL_REG_address 0x00001004
    uint32_t value;
    struct {
      uint32_t Reset                                                                  :  1; // [ 0: 0]
      uint32_t Resume                                                                 :  1; // [ 1: 1]
      uint32_t OutputBlock                                                            :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct crypt_AES_CMD_REG
{
  union {
#define crypt_AES_CMD_REG_address 0x00001008
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct crypt_AES_STATUS_REG
{
  union {
#define crypt_AES_STATUS_REG_address 0x0000100C
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t State                                                                  :  4; // [ 4: 1]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct crypt_AES_INTRPT_REG
{
  union {
#define crypt_AES_INTRPT_REG_address 0x00001010
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t Error1                                                                 :  1; // [ 1: 1]
      uint32_t Error2                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct crypt_AES_INTRPT_SRC_EN_REG
{
  union {
#define crypt_AES_INTRPT_SRC_EN_REG_address 0x00001014
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t Error1                                                                 :  1; // [ 1: 1]
      uint32_t Error2                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct crypt_AES_STREAM_SIZE
{
  union {
#define crypt_AES_STREAM_SIZE_address 0x00001018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_DES_CFG_REG
{
  union {
#define crypt_DES_CFG_REG_address 0x0000101C
    uint32_t value;
    struct {
      uint32_t Decrypt                                                                :  1; // [ 0: 0]
      uint32_t Eng_mode                                                               :  1; // [ 1: 1]
      uint32_t Op_mode                                                                :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct crypt_DES_CTRL_REG
{
  union {
#define crypt_DES_CTRL_REG_address 0x00001020
    uint32_t value;
    struct {
      uint32_t Reset                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct crypt_DES_CMD_REG
{
  union {
#define crypt_DES_CMD_REG_address 0x00001024
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct crypt_DES_STATUS_REG
{
  union {
#define crypt_DES_STATUS_REG_address 0x00001028
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t State                                                                  :  4; // [ 4: 1]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct crypt_DES_INTRPT_REG
{
  union {
#define crypt_DES_INTRPT_REG_address 0x0000102C
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t Error1                                                                 :  1; // [ 1: 1]
      uint32_t Error2                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct crypt_DES_INTRPT_SRC_EN_REG
{
  union {
#define crypt_DES_INTRPT_SRC_EN_REG_address 0x00001030
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t Error1                                                                 :  1; // [ 1: 1]
      uint32_t Error2                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct crypt_DES_STREAM_SIZE
{
  union {
#define crypt_DES_STREAM_SIZE_address 0x00001034
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_RC4_CFG_REG
{
  union {
#define crypt_RC4_CFG_REG_address 0x00001038
    uint32_t value;
    struct {
      uint32_t KeyLen                                                                 :  5; // [ 4: 0]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct crypt_RC4_CTRL_REG
{
  union {
#define crypt_RC4_CTRL_REG_address 0x0000103C
    uint32_t value;
    struct {
      uint32_t Reset                                                                  :  1; // [ 0: 0]
      uint32_t Resume                                                                 :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct crypt_RC4_CMD_REG
{
  union {
#define crypt_RC4_CMD_REG_address 0x00001040
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct crypt_RC4_STATUS_REG
{
  union {
#define crypt_RC4_STATUS_REG_address 0x00001044
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t State                                                                  :  4; // [ 4: 1]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct crypt_RC4_INTRPT_REG
{
  union {
#define crypt_RC4_INTRPT_REG_address 0x00001048
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct crypt_RC4_INTRPT_SRC_EN_REG
{
  union {
#define crypt_RC4_INTRPT_SRC_EN_REG_address 0x0000104C
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct crypt_RC4_STREAM_SIZE
{
  union {
#define crypt_RC4_STREAM_SIZE_address 0x00001050
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_RC4_INDEX
{
  union {
#define crypt_RC4_INDEX_address 0x00001054
    uint32_t value;
    struct {
      uint32_t Index_j                                                                :  8; // [ 7: 0]
      uint32_t Index_i                                                                :  8; // [15: 8]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct crypt_KEY2_W0
{
  union {
#define crypt_KEY2_W0_address 0x00001058
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY2_W1
{
  union {
#define crypt_KEY2_W1_address 0x0000105C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY2_W2
{
  union {
#define crypt_KEY2_W2_address 0x00001060
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY2_W3
{
  union {
#define crypt_KEY2_W3_address 0x00001064
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY2_W4
{
  union {
#define crypt_KEY2_W4_address 0x00001068
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY2_W5
{
  union {
#define crypt_KEY2_W5_address 0x0000106C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY2_W6
{
  union {
#define crypt_KEY2_W6_address 0x00001070
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY2_W7
{
  union {
#define crypt_KEY2_W7_address 0x00001074
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY1_W0
{
  union {
#define crypt_KEY1_W0_address 0x00001078
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY1_W1
{
  union {
#define crypt_KEY1_W1_address 0x0000107C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY1_W2
{
  union {
#define crypt_KEY1_W2_address 0x00001080
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY1_W3
{
  union {
#define crypt_KEY1_W3_address 0x00001084
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY1_W4
{
  union {
#define crypt_KEY1_W4_address 0x00001088
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY1_W5
{
  union {
#define crypt_KEY1_W5_address 0x0000108C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY1_W6
{
  union {
#define crypt_KEY1_W6_address 0x00001090
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_KEY1_W7
{
  union {
#define crypt_KEY1_W7_address 0x00001094
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_IV_W0
{
  union {
#define crypt_IV_W0_address 0x00001098
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_IV_W1
{
  union {
#define crypt_IV_W1_address 0x0000109C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_IV_W2
{
  union {
#define crypt_IV_W2_address 0x000010A0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_IV_W3
{
  union {
#define crypt_IV_W3_address 0x000010A4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct crypt_ENGINE_SELECT_REG
{
  union {
#define crypt_ENGINE_SELECT_REG_address 0x000010A8
    uint32_t value;
    struct {
      uint32_t Select                                                                 :  2; // [ 1: 0]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct crypt
{
  struct crypt_AES_CFG_REG                                  AES_CFG_REG         ; //offset 0 (0)
  struct crypt_AES_CTRL_REG                                 AES_CTRL_REG        ; //offset 4 (4)
  struct crypt_AES_CMD_REG                                  AES_CMD_REG         ; //offset 8 (8)
  struct crypt_AES_STATUS_REG                               AES_STATUS_REG      ; //offset C (12)
  struct crypt_AES_INTRPT_REG                               AES_INTRPT_REG      ; //offset 10 (16)
  struct crypt_AES_INTRPT_SRC_EN_REG                        AES_INTRPT_SRC_EN_REG; //offset 14 (20)
  struct crypt_AES_STREAM_SIZE                              AES_STREAM_SIZE     ; //offset 18 (24)
  struct crypt_DES_CFG_REG                                  DES_CFG_REG         ; //offset 1C (28)
  struct crypt_DES_CTRL_REG                                 DES_CTRL_REG        ; //offset 20 (32)
  struct crypt_DES_CMD_REG                                  DES_CMD_REG         ; //offset 24 (36)
  struct crypt_DES_STATUS_REG                               DES_STATUS_REG      ; //offset 28 (40)
  struct crypt_DES_INTRPT_REG                               DES_INTRPT_REG      ; //offset 2C (44)
  struct crypt_DES_INTRPT_SRC_EN_REG                        DES_INTRPT_SRC_EN_REG; //offset 30 (48)
  struct crypt_DES_STREAM_SIZE                              DES_STREAM_SIZE     ; //offset 34 (52)
  struct crypt_RC4_CFG_REG                                  RC4_CFG_REG         ; //offset 38 (56)
  struct crypt_RC4_CTRL_REG                                 RC4_CTRL_REG        ; //offset 3C (60)
  struct crypt_RC4_CMD_REG                                  RC4_CMD_REG         ; //offset 40 (64)
  struct crypt_RC4_STATUS_REG                               RC4_STATUS_REG      ; //offset 44 (68)
  struct crypt_RC4_INTRPT_REG                               RC4_INTRPT_REG      ; //offset 48 (72)
  struct crypt_RC4_INTRPT_SRC_EN_REG                        RC4_INTRPT_SRC_EN_REG; //offset 4C (76)
  struct crypt_RC4_STREAM_SIZE                              RC4_STREAM_SIZE     ; //offset 50 (80)
  struct crypt_RC4_INDEX                                    RC4_INDEX           ; //offset 54 (84)
  struct crypt_KEY2_W0                                      KEY2_W0             ; //offset 58 (88)
  struct crypt_KEY2_W1                                      KEY2_W1             ; //offset 5C (92)
  struct crypt_KEY2_W2                                      KEY2_W2             ; //offset 60 (96)
  struct crypt_KEY2_W3                                      KEY2_W3             ; //offset 64 (100)
  struct crypt_KEY2_W4                                      KEY2_W4             ; //offset 68 (104)
  struct crypt_KEY2_W5                                      KEY2_W5             ; //offset 6C (108)
  struct crypt_KEY2_W6                                      KEY2_W6             ; //offset 70 (112)
  struct crypt_KEY2_W7                                      KEY2_W7             ; //offset 74 (116)
  struct crypt_KEY1_W0                                      KEY1_W0             ; //offset 78 (120)
  struct crypt_KEY1_W1                                      KEY1_W1             ; //offset 7C (124)
  struct crypt_KEY1_W2                                      KEY1_W2             ; //offset 80 (128)
  struct crypt_KEY1_W3                                      KEY1_W3             ; //offset 84 (132)
  struct crypt_KEY1_W4                                      KEY1_W4             ; //offset 88 (136)
  struct crypt_KEY1_W5                                      KEY1_W5             ; //offset 8C (140)
  struct crypt_KEY1_W6                                      KEY1_W6             ; //offset 90 (144)
  struct crypt_KEY1_W7                                      KEY1_W7             ; //offset 94 (148)
  struct crypt_IV_W0                                        IV_W0               ; //offset 98 (152)
  struct crypt_IV_W1                                        IV_W1               ; //offset 9C (156)
  struct crypt_IV_W2                                        IV_W2               ; //offset A0 (160)
  struct crypt_IV_W3                                        IV_W3               ; //offset A4 (164)
  struct crypt_ENGINE_SELECT_REG                            ENGINE_SELECT_REG   ; //offset A8 (168)
}; //crypt


struct aes_AES_CFG_REG
{
  union {
#define aes_AES_CFG_REG_address 0x00001000
    uint32_t value;
    struct {
      uint32_t Decrypt                                                                :  1; // [ 0: 0]
      uint32_t KeyLen                                                                 :  2; // [ 2: 1]
      uint32_t OP_Mode                                                                :  3; // [ 5: 3]
      uint32_t RKEK                                                                   :  1; // [ 6: 6]
      uint32_t __reserved__9                                                          :  1; // [ 7: 7]
      uint32_t Modular                                                                :  7; // [14: 8]
      uint32_t __reserved__7                                                          :  1; // [15:15]
      uint32_t Cts_mode                                                               :  1; // [16:16]
      uint32_t __reserved__5                                                          :  7; // [23:17]
      uint32_t IV_Mask                                                                :  3; // [26:24]
      uint32_t __reserved__3                                                          :  1; // [27:27]
      uint32_t IV_Mode                                                                :  2; // [29:28]
      uint32_t __reserved__1                                                          :  1; // [30:30]
      uint32_t IV_mode_enable                                                         :  1; // [31:31]
    } b;
  } u;
};

struct aes_AES_CTRL_REG
{
  union {
#define aes_AES_CTRL_REG_address 0x00001004
    uint32_t value;
    struct {
      uint32_t Reset                                                                  :  1; // [ 0: 0]
      uint32_t Resume                                                                 :  1; // [ 1: 1]
      uint32_t BlockOutput                                                            :  1; // [ 2: 2]
      uint32_t __reserved__3                                                          :  1; // [ 3: 3]
      uint32_t Interleave                                                             :  1; // [ 4: 4]
      uint32_t ByteEndian                                                             :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct aes_AES_CMD_REG
{
  union {
#define aes_AES_CMD_REG_address 0x00001008
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct aes_AES_STATUS_REG
{
  union {
#define aes_AES_STATUS_REG_address 0x0000100C
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t State                                                                  :  4; // [ 4: 1]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct aes_AES_INTRPT_REG
{
  union {
#define aes_AES_INTRPT_REG_address 0x00001010
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t Error1                                                                 :  1; // [ 1: 1]
      uint32_t Error2                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct aes_AES_INTRPT_SRC_EN_REG
{
  union {
#define aes_AES_INTRPT_SRC_EN_REG_address 0x00001014
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t Error1                                                                 :  1; // [ 1: 1]
      uint32_t Error2                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct aes_AES_STREAM_SIZE
{
  union {
#define aes_AES_STREAM_SIZE_address 0x00001018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes___reserved__7
{
  union {
#define aes___reserved__7_address 0x0000101C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes___reserved__8
{
  union {
#define aes___reserved__8_address 0x00001020
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes___reserved__9
{
  union {
#define aes___reserved__9_address 0x00001028
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes___reserved__10
{
  union {
#define aes___reserved__10_address 0x00001030
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes___reserved__11
{
  union {
#define aes___reserved__11_address 0x00001038
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes___reserved__12
{
  union {
#define aes___reserved__12_address 0x00001040
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes___reserved__13
{
  union {
#define aes___reserved__13_address 0x00001048
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes___reserved__14
{
  union {
#define aes___reserved__14_address 0x00001050
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_KEY2_W0
{
  union {
#define aes_KEY2_W0_address 0x00001058
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY2_W1
{
  union {
#define aes_KEY2_W1_address 0x0000105C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY2_W2
{
  union {
#define aes_KEY2_W2_address 0x00001060
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY2_W3
{
  union {
#define aes_KEY2_W3_address 0x00001064
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY2_W4
{
  union {
#define aes_KEY2_W4_address 0x00001068
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY2_W5
{
  union {
#define aes_KEY2_W5_address 0x0000106C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY2_W6
{
  union {
#define aes_KEY2_W6_address 0x00001070
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY2_W7
{
  union {
#define aes_KEY2_W7_address 0x00001074
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY1_W0
{
  union {
#define aes_KEY1_W0_address 0x00001078
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY1_W1
{
  union {
#define aes_KEY1_W1_address 0x0000107C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY1_W2
{
  union {
#define aes_KEY1_W2_address 0x00001080
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY1_W3
{
  union {
#define aes_KEY1_W3_address 0x00001084
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY1_W4
{
  union {
#define aes_KEY1_W4_address 0x00001088
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY1_W5
{
  union {
#define aes_KEY1_W5_address 0x0000108C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY1_W6
{
  union {
#define aes_KEY1_W6_address 0x00001090
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_KEY1_W7
{
  union {
#define aes_KEY1_W7_address 0x00001094
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_IV_W0
{
  union {
#define aes_IV_W0_address 0x00001098
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_IV_W1
{
  union {
#define aes_IV_W1_address 0x0000109C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_IV_W2
{
  union {
#define aes_IV_W2_address 0x000010A0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_IV_W3
{
  union {
#define aes_IV_W3_address 0x000010A4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ENGINE_SELECT_REG
{
  union {
#define aes_ENGINE_SELECT_REG_address 0x000010A8
    uint32_t value;
    struct {
      uint32_t Select                                                                 :  2; // [ 1: 0]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct aes_AES_SECTOR_SIZE
{
  union {
#define aes_AES_SECTOR_SIZE_address 0x000010AC
    uint32_t value;
    struct {
      uint32_t SectorCnt                                                              : 16; // [15: 0]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct aes_LBA_W0
{
  union {
#define aes_LBA_W0_address 0x000010B0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_LBA_W1
{
  union {
#define aes_LBA_W1_address 0x000010B4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes
{
  struct aes_AES_CFG_REG                                    AES_CFG_REG         ; //offset 0 (0)
  struct aes_AES_CTRL_REG                                   AES_CTRL_REG        ; //offset 4 (4)
  struct aes_AES_CMD_REG                                    AES_CMD_REG         ; //offset 8 (8)
  struct aes_AES_STATUS_REG                                 AES_STATUS_REG      ; //offset C (12)
  struct aes_AES_INTRPT_REG                                 AES_INTRPT_REG      ; //offset 10 (16)
  struct aes_AES_INTRPT_SRC_EN_REG                          AES_INTRPT_SRC_EN_REG; //offset 14 (20)
  struct aes_AES_STREAM_SIZE                                AES_STREAM_SIZE     ; //offset 18 (24)
         uint32_t                                                __reserved__7       ; //offset 1C (28)
         uint64_t                                                __reserved__8       ; //offset 20 (32)
         uint64_t                                                __reserved__9       ; //offset 28 (40)
         uint64_t                                                __reserved__10      ; //offset 30 (48)
         uint64_t                                                __reserved__11      ; //offset 38 (56)
         uint64_t                                                __reserved__12      ; //offset 40 (64)
         uint64_t                                                __reserved__13      ; //offset 48 (72)
         uint64_t                                                __reserved__14      ; //offset 50 (80)
  struct aes_KEY2_W0                                        KEY2_W0             ; //offset 58 (88)
  struct aes_KEY2_W1                                        KEY2_W1             ; //offset 5C (92)
  struct aes_KEY2_W2                                        KEY2_W2             ; //offset 60 (96)
  struct aes_KEY2_W3                                        KEY2_W3             ; //offset 64 (100)
  struct aes_KEY2_W4                                        KEY2_W4             ; //offset 68 (104)
  struct aes_KEY2_W5                                        KEY2_W5             ; //offset 6C (108)
  struct aes_KEY2_W6                                        KEY2_W6             ; //offset 70 (112)
  struct aes_KEY2_W7                                        KEY2_W7             ; //offset 74 (116)
  struct aes_KEY1_W0                                        KEY1_W0             ; //offset 78 (120)
  struct aes_KEY1_W1                                        KEY1_W1             ; //offset 7C (124)
  struct aes_KEY1_W2                                        KEY1_W2             ; //offset 80 (128)
  struct aes_KEY1_W3                                        KEY1_W3             ; //offset 84 (132)
  struct aes_KEY1_W4                                        KEY1_W4             ; //offset 88 (136)
  struct aes_KEY1_W5                                        KEY1_W5             ; //offset 8C (140)
  struct aes_KEY1_W6                                        KEY1_W6             ; //offset 90 (144)
  struct aes_KEY1_W7                                        KEY1_W7             ; //offset 94 (148)
  struct aes_IV_W0                                          IV_W0               ; //offset 98 (152)
  struct aes_IV_W1                                          IV_W1               ; //offset 9C (156)
  struct aes_IV_W2                                          IV_W2               ; //offset A0 (160)
  struct aes_IV_W3                                          IV_W3               ; //offset A4 (164)
  struct aes_ENGINE_SELECT_REG                              ENGINE_SELECT_REG   ; //offset A8 (168)
  struct aes_AES_SECTOR_SIZE                                AES_SECTOR_SIZE     ; //offset AC (172)
  struct aes_LBA_W0                                         LBA_W0              ; //offset B0 (176)
  struct aes_LBA_W1                                         LBA_W1              ; //offset B4 (180)
}; //aes


struct smc_AES_CFG
{
  union {
#define smc_AES_CFG_address 0x00001000
    uint32_t value;
    struct {
      uint32_t Decrypt                                                                :  1; // [ 0: 0]
      uint32_t KeyLen                                                                 :  2; // [ 2: 1]
      uint32_t __reserved__7                                                          :  1; // [ 3: 3]
      uint32_t OpMode                                                                 :  3; // [ 6: 4]
      uint32_t __reserved__5                                                          :  1; // [ 7: 7]
      uint32_t CtrMod                                                                 :  7; // [14: 8]
      uint32_t __reserved__3                                                          :  1; // [15:15]
      uint32_t Rkek                                                                   :  1; // [16:16]
      uint32_t __reserved__1                                                          : 14; // [30:17]
      uint32_t GcmIvSrc                                                               :  1; // [31:31]
    } b;
  } u;
};

struct smc_AES_CTRL
{
  union {
#define smc_AES_CTRL_address 0x00001004
    uint32_t value;
    struct {
      uint32_t __reserved__3                                                          :  1; // [ 0: 0]
      uint32_t Resume                                                                 :  1; // [ 1: 1]
      uint32_t OutputDis                                                              :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct smc_AES_CMD
{
  union {
#define smc_AES_CMD_address 0x00001008
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t Reset                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct smc_AES_STATUS
{
  union {
#define smc_AES_STATUS_address 0x0000100C
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__4                                                          :  3; // [ 3: 1]
      uint32_t CoreState                                                              :  4; // [ 7: 4]
      uint32_t __reserved__2                                                          :  4; // [11: 8]
      uint32_t EngState                                                               :  3; // [14:12]
      uint32_t __reserved__0                                                          : 17; // [31:15]
    } b;
  } u;
};

struct smc_AES_INTP
{
  union {
#define smc_AES_INTP_address 0x00001010
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t LenErr                                                                 :  1; // [ 1: 1]
      uint32_t TagErr                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__2                                                          :  1; // [ 3: 3]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_AES_INTP_EN
{
  union {
#define smc_AES_INTP_EN_address 0x00001014
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t LenErr                                                                 :  1; // [ 1: 1]
      uint32_t TagErr                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__2                                                          :  1; // [ 3: 3]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_AES_STR_SIZE
{
  union {
#define smc_AES_STR_SIZE_address 0x00001018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_AES_AAD_SIZE
{
  union {
#define smc_AES_AAD_SIZE_address 0x0000101C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_AES_IV_SIZE
{
  union {
#define smc_AES_IV_SIZE_address 0x00001020
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_AES_TAG_SIZE
{
  union {
#define smc_AES_TAG_SIZE_address 0x00001024
    uint32_t value;
    struct {
      uint32_t Tsize                                                                  :  5; // [ 4: 0]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_RSVD0
{
  union {
#define smc_RSVD0_address 0x00001028
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__11
{
  union {
#define smc___reserved__11_address 0x0000102C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__12
{
  union {
#define smc___reserved__12_address 0x00001030
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__13
{
  union {
#define smc___reserved__13_address 0x00001038
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc_DES_CFG
{
  union {
#define smc_DES_CFG_address 0x00001040
    uint32_t value;
    struct {
      uint32_t Decrypt                                                                :  1; // [ 0: 0]
      uint32_t EngMode                                                                :  1; // [ 1: 1]
      uint32_t OpMode                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct smc_RSVD_DES
{
  union {
#define smc_RSVD_DES_address 0x00001044
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_DES_CMD
{
  union {
#define smc_DES_CMD_address 0x00001048
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t Reset                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct smc_DES_STATUS
{
  union {
#define smc_DES_STATUS_address 0x0000104C
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t CoreState                                                              :  4; // [ 4: 1]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_DES_INTP
{
  union {
#define smc_DES_INTP_address 0x00001050
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t LenErr                                                                 :  1; // [ 1: 1]
      uint32_t __reserved__2                                                          :  2; // [ 3: 2]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_DES_INTP_EN
{
  union {
#define smc_DES_INTP_EN_address 0x00001054
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t LenErr                                                                 :  1; // [ 1: 1]
      uint32_t __reserved__2                                                          :  2; // [ 3: 2]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_DES_STR_SIZE
{
  union {
#define smc_DES_STR_SIZE_address 0x00001058
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_RSVD1
{
  union {
#define smc_RSVD1_address 0x0000105C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__22
{
  union {
#define smc___reserved__22_address 0x00001060
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__23
{
  union {
#define smc___reserved__23_address 0x00001068
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__24
{
  union {
#define smc___reserved__24_address 0x00001070
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__25
{
  union {
#define smc___reserved__25_address 0x00001078
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc_RC4_CFG
{
  union {
#define smc_RC4_CFG_address 0x00001080
    uint32_t value;
    struct {
      uint32_t KeyLen                                                                 :  5; // [ 4: 0]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_RC4_CTRL
{
  union {
#define smc_RC4_CTRL_address 0x00001084
    uint32_t value;
    struct {
      uint32_t __reserved__2                                                          :  1; // [ 0: 0]
      uint32_t Resume                                                                 :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct smc_RC4_CMD
{
  union {
#define smc_RC4_CMD_address 0x00001088
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t Reset                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct smc_RC4_STATUS
{
  union {
#define smc_RC4_STATUS_address 0x0000108C
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t CoreState                                                              :  4; // [ 4: 1]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_RC4_INTP
{
  union {
#define smc_RC4_INTP_address 0x00001090
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__2                                                          :  3; // [ 3: 1]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_RC4_INTP_EN
{
  union {
#define smc_RC4_INTP_EN_address 0x00001094
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__2                                                          :  3; // [ 3: 1]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct smc_RC4_STR_SIZE
{
  union {
#define smc_RC4_STR_SIZE_address 0x00001098
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_RC4_INDEX
{
  union {
#define smc_RC4_INDEX_address 0x0000109C
    uint32_t value;
    struct {
      uint32_t Index_J                                                                :  8; // [ 7: 0]
      uint32_t Index_I                                                                :  8; // [15: 8]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct smc___reserved__34
{
  union {
#define smc___reserved__34_address 0x000010A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__35
{
  union {
#define smc___reserved__35_address 0x000010A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__36
{
  union {
#define smc___reserved__36_address 0x000010B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__37
{
  union {
#define smc___reserved__37_address 0x000010B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__38
{
  union {
#define smc___reserved__38_address 0x000010C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__39
{
  union {
#define smc___reserved__39_address 0x000010C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__40
{
  union {
#define smc___reserved__40_address 0x000010D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__41
{
  union {
#define smc___reserved__41_address 0x000010D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__42
{
  union {
#define smc___reserved__42_address 0x000010E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__43
{
  union {
#define smc___reserved__43_address 0x000010E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__44
{
  union {
#define smc___reserved__44_address 0x000010F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__45
{
  union {
#define smc___reserved__45_address 0x000010F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc_KEY2_DW0
{
  union {
#define smc_KEY2_DW0_address 0x00001100
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY2_DW1
{
  union {
#define smc_KEY2_DW1_address 0x00001104
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY2_DW2
{
  union {
#define smc_KEY2_DW2_address 0x00001108
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY2_DW3
{
  union {
#define smc_KEY2_DW3_address 0x0000110C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY2_DW4
{
  union {
#define smc_KEY2_DW4_address 0x00001110
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY2_DW5
{
  union {
#define smc_KEY2_DW5_address 0x00001114
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY2_DW6
{
  union {
#define smc_KEY2_DW6_address 0x00001118
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY2_DW7
{
  union {
#define smc_KEY2_DW7_address 0x0000111C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY1_DW0
{
  union {
#define smc_KEY1_DW0_address 0x00001120
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY1_DW1
{
  union {
#define smc_KEY1_DW1_address 0x00001124
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY1_DW2
{
  union {
#define smc_KEY1_DW2_address 0x00001128
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY1_DW3
{
  union {
#define smc_KEY1_DW3_address 0x0000112C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY1_DW4
{
  union {
#define smc_KEY1_DW4_address 0x00001130
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY1_DW5
{
  union {
#define smc_KEY1_DW5_address 0x00001134
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY1_DW6
{
  union {
#define smc_KEY1_DW6_address 0x00001138
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_KEY1_DW7
{
  union {
#define smc_KEY1_DW7_address 0x0000113C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_IV_DW0
{
  union {
#define smc_IV_DW0_address 0x00001140
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_IV_DW1
{
  union {
#define smc_IV_DW1_address 0x00001144
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_IV_DW2
{
  union {
#define smc_IV_DW2_address 0x00001148
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_IV_DW3
{
  union {
#define smc_IV_DW3_address 0x0000114C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc_RSVD2
{
  union {
#define smc_RSVD2_address 0x00001150
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__67
{
  union {
#define smc___reserved__67_address 0x00001154
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__68
{
  union {
#define smc___reserved__68_address 0x00001158
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__69
{
  union {
#define smc___reserved__69_address 0x00001160
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__70
{
  union {
#define smc___reserved__70_address 0x00001168
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__71
{
  union {
#define smc___reserved__71_address 0x00001170
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc___reserved__72
{
  union {
#define smc___reserved__72_address 0x00001178
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct smc_ENG_CFG
{
  union {
#define smc_ENG_CFG_address 0x00001180
    uint32_t value;
    struct {
      uint32_t Select                                                                 :  2; // [ 1: 0]
      uint32_t __reserved__3                                                          : 22; // [23: 2]
      uint32_t DataSwap                                                               :  1; // [24:24]
      uint32_t __reserved__1                                                          :  6; // [30:25]
      uint32_t ExtCtrlEn                                                              :  1; // [31:31]
    } b;
  } u;
};

struct smc_RSVD3
{
  union {
#define smc_RSVD3_address 0x00001184
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct smc
{
  struct smc_AES_CFG                                        AES_CFG             ; //offset 0 (0)
  struct smc_AES_CTRL                                       AES_CTRL            ; //offset 4 (4)
  struct smc_AES_CMD                                        AES_CMD             ; //offset 8 (8)
  struct smc_AES_STATUS                                     AES_STATUS          ; //offset C (12)
  struct smc_AES_INTP                                       AES_INTP            ; //offset 10 (16)
  struct smc_AES_INTP_EN                                    AES_INTP_EN         ; //offset 14 (20)
  struct smc_AES_STR_SIZE                                   AES_STR_SIZE        ; //offset 18 (24)
  struct smc_AES_AAD_SIZE                                   AES_AAD_SIZE        ; //offset 1C (28)
  struct smc_AES_IV_SIZE                                    AES_IV_SIZE         ; //offset 20 (32)
  struct smc_AES_TAG_SIZE                                   AES_TAG_SIZE        ; //offset 24 (36)
         uint32_t                                                __reserved__10      ; //offset 28 (40)
         uint32_t                                                __reserved__11      ; //offset 2C (44)
         uint64_t                                                __reserved__12      ; //offset 30 (48)
         uint64_t                                                __reserved__13      ; //offset 38 (56)
  struct smc_DES_CFG                                        DES_CFG             ; //offset 40 (64)
         uint32_t                                                __reserved__15      ; //offset 44 (68)
  struct smc_DES_CMD                                        DES_CMD             ; //offset 48 (72)
  struct smc_DES_STATUS                                     DES_STATUS          ; //offset 4C (76)
  struct smc_DES_INTP                                       DES_INTP            ; //offset 50 (80)
  struct smc_DES_INTP_EN                                    DES_INTP_EN         ; //offset 54 (84)
  struct smc_DES_STR_SIZE                                   DES_STR_SIZE        ; //offset 58 (88)
         uint32_t                                                __reserved__21      ; //offset 5C (92)
         uint64_t                                                __reserved__22      ; //offset 60 (96)
         uint64_t                                                __reserved__23      ; //offset 68 (104)
         uint64_t                                                __reserved__24      ; //offset 70 (112)
         uint64_t                                                __reserved__25      ; //offset 78 (120)
  struct smc_RC4_CFG                                        RC4_CFG             ; //offset 80 (128)
  struct smc_RC4_CTRL                                       RC4_CTRL            ; //offset 84 (132)
  struct smc_RC4_CMD                                        RC4_CMD             ; //offset 88 (136)
  struct smc_RC4_STATUS                                     RC4_STATUS          ; //offset 8C (140)
  struct smc_RC4_INTP                                       RC4_INTP            ; //offset 90 (144)
  struct smc_RC4_INTP_EN                                    RC4_INTP_EN         ; //offset 94 (148)
  struct smc_RC4_STR_SIZE                                   RC4_STR_SIZE        ; //offset 98 (152)
  struct smc_RC4_INDEX                                      RC4_INDEX           ; //offset 9C (156)
         uint64_t                                                __reserved__34      ; //offset A0 (160)
         uint64_t                                                __reserved__35      ; //offset A8 (168)
         uint64_t                                                __reserved__36      ; //offset B0 (176)
         uint64_t                                                __reserved__37      ; //offset B8 (184)
         uint64_t                                                __reserved__38      ; //offset C0 (192)
         uint64_t                                                __reserved__39      ; //offset C8 (200)
         uint64_t                                                __reserved__40      ; //offset D0 (208)
         uint64_t                                                __reserved__41      ; //offset D8 (216)
         uint64_t                                                __reserved__42      ; //offset E0 (224)
         uint64_t                                                __reserved__43      ; //offset E8 (232)
         uint64_t                                                __reserved__44      ; //offset F0 (240)
         uint64_t                                                __reserved__45      ; //offset F8 (248)
  struct smc_KEY2_DW0                                       KEY2_DW0            ; //offset 100 (256)
  struct smc_KEY2_DW1                                       KEY2_DW1            ; //offset 104 (260)
  struct smc_KEY2_DW2                                       KEY2_DW2            ; //offset 108 (264)
  struct smc_KEY2_DW3                                       KEY2_DW3            ; //offset 10C (268)
  struct smc_KEY2_DW4                                       KEY2_DW4            ; //offset 110 (272)
  struct smc_KEY2_DW5                                       KEY2_DW5            ; //offset 114 (276)
  struct smc_KEY2_DW6                                       KEY2_DW6            ; //offset 118 (280)
  struct smc_KEY2_DW7                                       KEY2_DW7            ; //offset 11C (284)
  struct smc_KEY1_DW0                                       KEY1_DW0            ; //offset 120 (288)
  struct smc_KEY1_DW1                                       KEY1_DW1            ; //offset 124 (292)
  struct smc_KEY1_DW2                                       KEY1_DW2            ; //offset 128 (296)
  struct smc_KEY1_DW3                                       KEY1_DW3            ; //offset 12C (300)
  struct smc_KEY1_DW4                                       KEY1_DW4            ; //offset 130 (304)
  struct smc_KEY1_DW5                                       KEY1_DW5            ; //offset 134 (308)
  struct smc_KEY1_DW6                                       KEY1_DW6            ; //offset 138 (312)
  struct smc_KEY1_DW7                                       KEY1_DW7            ; //offset 13C (316)
  struct smc_IV_DW0                                         IV_DW0              ; //offset 140 (320)
  struct smc_IV_DW1                                         IV_DW1              ; //offset 144 (324)
  struct smc_IV_DW2                                         IV_DW2              ; //offset 148 (328)
  struct smc_IV_DW3                                         IV_DW3              ; //offset 14C (332)
         uint32_t                                                __reserved__66      ; //offset 150 (336)
         uint32_t                                                __reserved__67      ; //offset 154 (340)
         uint64_t                                                __reserved__68      ; //offset 158 (344)
         uint64_t                                                __reserved__69      ; //offset 160 (352)
         uint64_t                                                __reserved__70      ; //offset 168 (360)
         uint64_t                                                __reserved__71      ; //offset 170 (368)
         uint64_t                                                __reserved__72      ; //offset 178 (376)
  struct smc_ENG_CFG                                        ENG_CFG             ; //offset 180 (384)
         uint32_t                                                __reserved__74      ; //offset 184 (388)
}; //smc


struct aes_ccm_AES_CFG
{
  union {
#define aes_ccm_AES_CFG_address 0x00001000
    uint32_t value;
    struct {
      uint32_t Decrypt                                                                :  1; // [ 0: 0]
      uint32_t KeyLen                                                                 :  2; // [ 2: 1]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct aes_ccm_AES_CTRL
{
  union {
#define aes_ccm_AES_CTRL_address 0x00001004
    uint32_t value;
    struct {
      uint32_t __reserved__4                                                          :  3; // [ 2: 0]
      uint32_t MacDis                                                                 :  1; // [ 3: 3]
      uint32_t __reserved__2                                                          :  1; // [ 4: 4]
      uint32_t ByteSwap                                                               :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct aes_ccm_AES_CMD
{
  union {
#define aes_ccm_AES_CMD_address 0x00001008
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t Reset                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct aes_ccm_AES_STATUS
{
  union {
#define aes_ccm_AES_STATUS_address 0x0000100C
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__2                                                          :  3; // [ 3: 1]
      uint32_t CoreState                                                              :  3; // [ 6: 4]
      uint32_t __reserved__0                                                          : 25; // [31: 7]
    } b;
  } u;
};

struct aes_ccm_AES_INTP
{
  union {
#define aes_ccm_AES_INTP_address 0x00001010
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__4                                                          :  1; // [ 1: 1]
      uint32_t MacErr                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__2                                                          :  1; // [ 3: 3]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct aes_ccm_AES_INTP_EN
{
  union {
#define aes_ccm_AES_INTP_EN_address 0x00001014
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__4                                                          :  1; // [ 1: 1]
      uint32_t MacErr                                                                 :  1; // [ 2: 2]
      uint32_t __reserved__2                                                          :  1; // [ 3: 3]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct aes_ccm_RSVD
{
  union {
#define aes_ccm_RSVD_address 0x00001018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__7
{
  union {
#define aes_ccm___reserved__7_address 0x0000101C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__8
{
  union {
#define aes_ccm___reserved__8_address 0x00001020
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__9
{
  union {
#define aes_ccm___reserved__9_address 0x00001028
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__10
{
  union {
#define aes_ccm___reserved__10_address 0x00001030
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__11
{
  union {
#define aes_ccm___reserved__11_address 0x00001038
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__12
{
  union {
#define aes_ccm___reserved__12_address 0x00001040
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__13
{
  union {
#define aes_ccm___reserved__13_address 0x00001048
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__14
{
  union {
#define aes_ccm___reserved__14_address 0x00001050
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__15
{
  union {
#define aes_ccm___reserved__15_address 0x00001058
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__16
{
  union {
#define aes_ccm___reserved__16_address 0x00001060
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__17
{
  union {
#define aes_ccm___reserved__17_address 0x00001068
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__18
{
  union {
#define aes_ccm___reserved__18_address 0x00001070
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__19
{
  union {
#define aes_ccm___reserved__19_address 0x00001078
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__20
{
  union {
#define aes_ccm___reserved__20_address 0x00001080
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__21
{
  union {
#define aes_ccm___reserved__21_address 0x00001088
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__22
{
  union {
#define aes_ccm___reserved__22_address 0x00001090
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__23
{
  union {
#define aes_ccm___reserved__23_address 0x00001098
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__24
{
  union {
#define aes_ccm___reserved__24_address 0x000010A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__25
{
  union {
#define aes_ccm___reserved__25_address 0x000010A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__26
{
  union {
#define aes_ccm___reserved__26_address 0x000010B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__27
{
  union {
#define aes_ccm___reserved__27_address 0x000010B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__28
{
  union {
#define aes_ccm___reserved__28_address 0x000010C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__29
{
  union {
#define aes_ccm___reserved__29_address 0x000010C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__30
{
  union {
#define aes_ccm___reserved__30_address 0x000010D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__31
{
  union {
#define aes_ccm___reserved__31_address 0x000010D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__32
{
  union {
#define aes_ccm___reserved__32_address 0x000010E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__33
{
  union {
#define aes_ccm___reserved__33_address 0x000010E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__34
{
  union {
#define aes_ccm___reserved__34_address 0x000010F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__35
{
  union {
#define aes_ccm___reserved__35_address 0x000010F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__36
{
  union {
#define aes_ccm___reserved__36_address 0x00001100
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__37
{
  union {
#define aes_ccm___reserved__37_address 0x00001108
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__38
{
  union {
#define aes_ccm___reserved__38_address 0x00001110
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm___reserved__39
{
  union {
#define aes_ccm___reserved__39_address 0x00001118
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_KEY_DW0
{
  union {
#define aes_ccm_KEY_DW0_address 0x00001120
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_KEY_DW1
{
  union {
#define aes_ccm_KEY_DW1_address 0x00001124
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_KEY_DW2
{
  union {
#define aes_ccm_KEY_DW2_address 0x00001128
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_KEY_DW3
{
  union {
#define aes_ccm_KEY_DW3_address 0x0000112C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_KEY_DW4
{
  union {
#define aes_ccm_KEY_DW4_address 0x00001130
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_KEY_DW5
{
  union {
#define aes_ccm_KEY_DW5_address 0x00001134
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_KEY_DW6
{
  union {
#define aes_ccm_KEY_DW6_address 0x00001138
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_KEY_DW7
{
  union {
#define aes_ccm_KEY_DW7_address 0x0000113C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_MAC_DW0
{
  union {
#define aes_ccm_MAC_DW0_address 0x00001140
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_MAC_DW1
{
  union {
#define aes_ccm_MAC_DW1_address 0x00001144
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_MAC_DW2
{
  union {
#define aes_ccm_MAC_DW2_address 0x00001148
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm_MAC_DW3
{
  union {
#define aes_ccm_MAC_DW3_address 0x0000114C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct aes_ccm
{
  struct aes_ccm_AES_CFG                                    AES_CFG             ; //offset 0 (0)
  struct aes_ccm_AES_CTRL                                   AES_CTRL            ; //offset 4 (4)
  struct aes_ccm_AES_CMD                                    AES_CMD             ; //offset 8 (8)
  struct aes_ccm_AES_STATUS                                 AES_STATUS          ; //offset C (12)
  struct aes_ccm_AES_INTP                                   AES_INTP            ; //offset 10 (16)
  struct aes_ccm_AES_INTP_EN                                AES_INTP_EN         ; //offset 14 (20)
         uint32_t                                                __reserved__6       ; //offset 18 (24)
         uint32_t                                                __reserved__7       ; //offset 1C (28)
         uint64_t                                                __reserved__8       ; //offset 20 (32)
         uint64_t                                                __reserved__9       ; //offset 28 (40)
         uint64_t                                                __reserved__10      ; //offset 30 (48)
         uint64_t                                                __reserved__11      ; //offset 38 (56)
         uint64_t                                                __reserved__12      ; //offset 40 (64)
         uint64_t                                                __reserved__13      ; //offset 48 (72)
         uint64_t                                                __reserved__14      ; //offset 50 (80)
         uint64_t                                                __reserved__15      ; //offset 58 (88)
         uint64_t                                                __reserved__16      ; //offset 60 (96)
         uint64_t                                                __reserved__17      ; //offset 68 (104)
         uint64_t                                                __reserved__18      ; //offset 70 (112)
         uint64_t                                                __reserved__19      ; //offset 78 (120)
         uint64_t                                                __reserved__20      ; //offset 80 (128)
         uint64_t                                                __reserved__21      ; //offset 88 (136)
         uint64_t                                                __reserved__22      ; //offset 90 (144)
         uint64_t                                                __reserved__23      ; //offset 98 (152)
         uint64_t                                                __reserved__24      ; //offset A0 (160)
         uint64_t                                                __reserved__25      ; //offset A8 (168)
         uint64_t                                                __reserved__26      ; //offset B0 (176)
         uint64_t                                                __reserved__27      ; //offset B8 (184)
         uint64_t                                                __reserved__28      ; //offset C0 (192)
         uint64_t                                                __reserved__29      ; //offset C8 (200)
         uint64_t                                                __reserved__30      ; //offset D0 (208)
         uint64_t                                                __reserved__31      ; //offset D8 (216)
         uint64_t                                                __reserved__32      ; //offset E0 (224)
         uint64_t                                                __reserved__33      ; //offset E8 (232)
         uint64_t                                                __reserved__34      ; //offset F0 (240)
         uint64_t                                                __reserved__35      ; //offset F8 (248)
         uint64_t                                                __reserved__36      ; //offset 100 (256)
         uint64_t                                                __reserved__37      ; //offset 108 (264)
         uint64_t                                                __reserved__38      ; //offset 110 (272)
         uint64_t                                                __reserved__39      ; //offset 118 (280)
  struct aes_ccm_KEY_DW0                                    KEY_DW0             ; //offset 120 (288)
  struct aes_ccm_KEY_DW1                                    KEY_DW1             ; //offset 124 (292)
  struct aes_ccm_KEY_DW2                                    KEY_DW2             ; //offset 128 (296)
  struct aes_ccm_KEY_DW3                                    KEY_DW3             ; //offset 12C (300)
  struct aes_ccm_KEY_DW4                                    KEY_DW4             ; //offset 130 (304)
  struct aes_ccm_KEY_DW5                                    KEY_DW5             ; //offset 134 (308)
  struct aes_ccm_KEY_DW6                                    KEY_DW6             ; //offset 138 (312)
  struct aes_ccm_KEY_DW7                                    KEY_DW7             ; //offset 13C (316)
  struct aes_ccm_MAC_DW0                                    MAC_DW0             ; //offset 140 (320)
  struct aes_ccm_MAC_DW1                                    MAC_DW1             ; //offset 144 (324)
  struct aes_ccm_MAC_DW2                                    MAC_DW2             ; //offset 148 (328)
  struct aes_ccm_MAC_DW3                                    MAC_DW3             ; //offset 14C (332)
}; //aes_ccm


struct ecp_ECP_CFG_REG
{
  union {
#define ecp_ECP_CFG_REG_address 0x00001400
    uint32_t value;
    struct {
      uint32_t PFIELD_SEL                                                             :  2; // [ 1: 0]
      uint32_t OP_MODE                                                                :  3; // [ 4: 2]
      uint32_t USE_OTP_EC521_DK                                                       :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct ecp_ECP_CTRL_REG
{
  union {
#define ecp_ECP_CTRL_REG_address 0x00001404
    uint32_t value;
    struct {
      uint32_t RESET                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ecp_ECP_CMD_REG
{
  union {
#define ecp_ECP_CMD_REG_address 0x00001408
    uint32_t value;
    struct {
      uint32_t CMD_START                                                              :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ecp_ECP_INTRPT_REG
{
  union {
#define ecp_ECP_INTRPT_REG_address 0x0000140C
    uint32_t value;
    struct {
      uint32_t CMD_DONE_INT                                                           :  1; // [ 0: 0]
      uint32_t INVALID_MODE                                                           :  1; // [ 1: 1]
      uint32_t ZERO_KEY                                                               :  1; // [ 2: 2]
      uint32_t ZERO_OUTPUT                                                            :  1; // [ 3: 3]
      uint32_t CAL_ZERO_INV                                                           :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct ecp_ECP_STATUS_REG
{
  union {
#define ecp_ECP_STATUS_REG_address 0x00001410
    uint32_t value;
    struct {
      uint32_t BUSY                                                                   :  1; // [ 0: 0]
      uint32_t MFSM_STATE                                                             :  2; // [ 2: 1]
      uint32_t SFSM_MALU_STATE                                                        :  5; // [ 7: 3]
      uint32_t SFSM_MEMW_STATE                                                        :  2; // [ 9: 8]
      uint32_t SFSM_MEMR_STATE                                                        :  2; // [11:10]
      uint32_t SFSM_ADD_STATE                                                         :  2; // [13:12]
      uint32_t SFSM_MUL_STATE                                                         :  3; // [16:14]
      uint32_t MEM_ERROR                                                              :  1; // [17:17]
      uint32_t __reserved__0                                                          : 14; // [31:18]
    } b;
  } u;
};

struct ecp_ECP_INTRPT_MASK_REG
{
  union {
#define ecp_ECP_INTRPT_MASK_REG_address 0x00001414
    uint32_t value;
    struct {
      uint32_t CMD_DONE_INT                                                           :  1; // [ 0: 0]
      uint32_t INVALID_MODE                                                           :  1; // [ 1: 1]
      uint32_t ZERO_KEY                                                               :  1; // [ 2: 2]
      uint32_t ZERO_OUTPUT                                                            :  1; // [ 3: 3]
      uint32_t CAL_ZERO_INV                                                           :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct ecp_ECP_SRAM
{
  union {
#define ecp_ECP_SRAM_address 0x00001418
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ecp
{
  struct ecp_ECP_CFG_REG                                    ECP_CFG_REG         ; //offset 0 (0)
  struct ecp_ECP_CTRL_REG                                   ECP_CTRL_REG        ; //offset 4 (4)
  struct ecp_ECP_CMD_REG                                    ECP_CMD_REG         ; //offset 8 (8)
  struct ecp_ECP_INTRPT_REG                                 ECP_INTRPT_REG      ; //offset C (12)
  struct ecp_ECP_STATUS_REG                                 ECP_STATUS_REG      ; //offset 10 (16)
  struct ecp_ECP_INTRPT_MASK_REG                            ECP_INTRPT_MASK_REG ; //offset 14 (20)
  struct ecp_ECP_SRAM                                       ECP_SRAM            ; //offset 18 (24)
}; //ecp


struct ecrsa_ecp_ECP_CFG_REG
{
  union {
#define ecrsa_ecp_ECP_CFG_REG_address 0x00001400
    uint32_t value;
    struct {
      uint32_t FIELD_MODE                                                             :  2; // [ 1: 0]
      uint32_t OP_MODE                                                                :  3; // [ 4: 2]
      uint32_t USE_OTP_EC521_DK                                                       :  1; // [ 5: 5]
      uint32_t __reserved__2                                                          :  2; // [ 7: 6]
      uint32_t USE_DEF_PRIME                                                          :  1; // [ 8: 8]
      uint32_t __reserved__0                                                          : 23; // [31: 9]
    } b;
  } u;
};

struct ecrsa_ecp_ECP_CTRL_REG
{
  union {
#define ecrsa_ecp_ECP_CTRL_REG_address 0x00001404
    uint32_t value;
    struct {
      uint32_t RESET                                                                  :  1; // [ 0: 0]
      uint32_t ENG_SEL                                                                :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct ecrsa_ecp_ECP_CMD_REG
{
  union {
#define ecrsa_ecp_ECP_CMD_REG_address 0x00001408
    uint32_t value;
    struct {
      uint32_t START                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ecrsa_ecp_ECP_INTRPT_REG
{
  union {
#define ecrsa_ecp_ECP_INTRPT_REG_address 0x0000140C
    uint32_t value;
    struct {
      uint32_t DONE                                                                   :  1; // [ 0: 0]
      uint32_t MERR                                                                   :  1; // [ 1: 1]
      uint32_t ZKEY                                                                   :  1; // [ 2: 2]
      uint32_t ZOUT                                                                   :  1; // [ 3: 3]
      uint32_t ZINV                                                                   :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct ecrsa_ecp_ECP_STATUS_REG
{
  union {
#define ecrsa_ecp_ECP_STATUS_REG_address 0x00001410
    uint32_t value;
    struct {
      uint32_t BUSY                                                                   :  1; // [ 0: 0]
      uint32_t PUSM                                                                   :  3; // [ 3: 1]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct ecrsa_ecp_ECP_INTRPT_MASK_REG
{
  union {
#define ecrsa_ecp_ECP_INTRPT_MASK_REG_address 0x00001414
    uint32_t value;
    struct {
      uint32_t DONE                                                                   :  1; // [ 0: 0]
      uint32_t MERR                                                                   :  1; // [ 1: 1]
      uint32_t ZKEY                                                                   :  1; // [ 2: 2]
      uint32_t ZOUT                                                                   :  1; // [ 3: 3]
      uint32_t ZINV                                                                   :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct ecrsa_ecp_ECP_SRAM
{
  union {
#define ecrsa_ecp_ECP_SRAM_address 0x00001418
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ecrsa_ecp
{
  struct ecrsa_ecp_ECP_CFG_REG                              ECP_CFG_REG         ; //offset 0 (0)
  struct ecrsa_ecp_ECP_CTRL_REG                             ECP_CTRL_REG        ; //offset 4 (4)
  struct ecrsa_ecp_ECP_CMD_REG                              ECP_CMD_REG         ; //offset 8 (8)
  struct ecrsa_ecp_ECP_INTRPT_REG                           ECP_INTRPT_REG      ; //offset C (12)
  struct ecrsa_ecp_ECP_STATUS_REG                           ECP_STATUS_REG      ; //offset 10 (16)
  struct ecrsa_ecp_ECP_INTRPT_MASK_REG                      ECP_INTRPT_MASK_REG ; //offset 14 (20)
  struct ecrsa_ecp_ECP_SRAM                                 ECP_SRAM            ; //offset 18 (24)
}; //ecrsa_ecp


struct amc_AMC_CFG
{
  union {
#define amc_AMC_CFG_address 0x00001400
    uint32_t value;
    struct {
      uint32_t OpMode                                                                 :  5; // [ 4: 0]
      uint32_t __reserved__3                                                          : 23; // [27: 5]
      uint32_t SecureMode                                                             :  1; // [28:28]
      uint32_t Rkek                                                                   :  1; // [29:29]
      uint32_t __reserved__0                                                          :  2; // [31:30]
    } b;
  } u;
};

struct amc_RSVD0
{
  union {
#define amc_RSVD0_address 0x00001404
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct amc_AMC_CMD
{
  union {
#define amc_AMC_CMD_address 0x00001408
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t Reset                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct amc_AMC_STATUS
{
  union {
#define amc_AMC_STATUS_address 0x0000140C
    uint32_t value;
    struct {
      uint32_t Busy                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__2                                                          :  3; // [ 3: 1]
      uint32_t CpuState                                                               :  3; // [ 6: 4]
      uint32_t __reserved__0                                                          : 25; // [31: 7]
    } b;
  } u;
};

struct amc_AMC_INTP
{
  union {
#define amc_AMC_INTP_address 0x00001410
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t OpErr                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__2                                                          :  2; // [ 3: 2]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct amc_AMC_INTP_EN
{
  union {
#define amc_AMC_INTP_EN_address 0x00001414
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t OpErr                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__2                                                          :  2; // [ 3: 2]
      uint32_t TxErr                                                                  :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct amc_AMC_SIZE
{
  union {
#define amc_AMC_SIZE_address 0x00001418
    uint32_t value;
    struct {
      uint32_t BitWidth                                                               : 14; // [13: 0]
      uint32_t __reserved__0                                                          : 18; // [31:14]
    } b;
  } u;
};

struct amc_RSVD1
{
  union {
#define amc_RSVD1_address 0x0000141C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__8
{
  union {
#define amc___reserved__8_address 0x00001420
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__9
{
  union {
#define amc___reserved__9_address 0x00001428
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__10
{
  union {
#define amc___reserved__10_address 0x00001430
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__11
{
  union {
#define amc___reserved__11_address 0x00001438
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__12
{
  union {
#define amc___reserved__12_address 0x00001440
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__13
{
  union {
#define amc___reserved__13_address 0x00001448
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__14
{
  union {
#define amc___reserved__14_address 0x00001450
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__15
{
  union {
#define amc___reserved__15_address 0x00001458
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__16
{
  union {
#define amc___reserved__16_address 0x00001460
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__17
{
  union {
#define amc___reserved__17_address 0x00001468
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__18
{
  union {
#define amc___reserved__18_address 0x00001470
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__19
{
  union {
#define amc___reserved__19_address 0x00001478
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__20
{
  union {
#define amc___reserved__20_address 0x00001480
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__21
{
  union {
#define amc___reserved__21_address 0x00001488
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__22
{
  union {
#define amc___reserved__22_address 0x00001490
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__23
{
  union {
#define amc___reserved__23_address 0x00001498
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__24
{
  union {
#define amc___reserved__24_address 0x000014A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__25
{
  union {
#define amc___reserved__25_address 0x000014A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__26
{
  union {
#define amc___reserved__26_address 0x000014B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__27
{
  union {
#define amc___reserved__27_address 0x000014B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__28
{
  union {
#define amc___reserved__28_address 0x000014C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__29
{
  union {
#define amc___reserved__29_address 0x000014C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__30
{
  union {
#define amc___reserved__30_address 0x000014D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__31
{
  union {
#define amc___reserved__31_address 0x000014D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__32
{
  union {
#define amc___reserved__32_address 0x000014E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__33
{
  union {
#define amc___reserved__33_address 0x000014E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__34
{
  union {
#define amc___reserved__34_address 0x000014F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__35
{
  union {
#define amc___reserved__35_address 0x000014F8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct amc_AMC_ACC_ADDR
{
  union {
#define amc_AMC_ACC_ADDR_address 0x000014FC
    uint32_t value;
    struct {
      uint32_t Offset                                                                 :  8; // [ 7: 0]
      uint32_t __reserved__0                                                          : 24; // [31: 8]
    } b;
  } u;
};

struct amc_AMC_ACC_ZMP_DATA
{
  union {
#define amc_AMC_ACC_ZMP_DATA_address 0x00001500
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__38
{
  union {
#define amc___reserved__38_address 0x00001504
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__39
{
  union {
#define amc___reserved__39_address 0x00001508
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__40
{
  union {
#define amc___reserved__40_address 0x00001510
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__41
{
  union {
#define amc___reserved__41_address 0x00001518
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__42
{
  union {
#define amc___reserved__42_address 0x00001520
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__43
{
  union {
#define amc___reserved__43_address 0x00001528
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__44
{
  union {
#define amc___reserved__44_address 0x00001530
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__45
{
  union {
#define amc___reserved__45_address 0x00001538
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__46
{
  union {
#define amc___reserved__46_address 0x00001540
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__47
{
  union {
#define amc___reserved__47_address 0x00001548
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__48
{
  union {
#define amc___reserved__48_address 0x00001550
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__49
{
  union {
#define amc___reserved__49_address 0x00001558
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__50
{
  union {
#define amc___reserved__50_address 0x00001560
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__51
{
  union {
#define amc___reserved__51_address 0x00001568
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__52
{
  union {
#define amc___reserved__52_address 0x00001570
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__53
{
  union {
#define amc___reserved__53_address 0x00001578
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__54
{
  union {
#define amc___reserved__54_address 0x00001580
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__55
{
  union {
#define amc___reserved__55_address 0x00001588
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__56
{
  union {
#define amc___reserved__56_address 0x00001590
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__57
{
  union {
#define amc___reserved__57_address 0x00001598
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__58
{
  union {
#define amc___reserved__58_address 0x000015A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__59
{
  union {
#define amc___reserved__59_address 0x000015A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__60
{
  union {
#define amc___reserved__60_address 0x000015B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__61
{
  union {
#define amc___reserved__61_address 0x000015B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__62
{
  union {
#define amc___reserved__62_address 0x000015C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__63
{
  union {
#define amc___reserved__63_address 0x000015C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__64
{
  union {
#define amc___reserved__64_address 0x000015D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__65
{
  union {
#define amc___reserved__65_address 0x000015D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__66
{
  union {
#define amc___reserved__66_address 0x000015E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__67
{
  union {
#define amc___reserved__67_address 0x000015E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__68
{
  union {
#define amc___reserved__68_address 0x000015F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc___reserved__69
{
  union {
#define amc___reserved__69_address 0x000015F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct amc_AMC_ACC_ECP_DATA
{
  union {
#define amc_AMC_ACC_ECP_DATA_address 0x00001600
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct amc
{
  struct amc_AMC_CFG                                        AMC_CFG             ; //offset 0 (0)
         uint32_t                                                __reserved__1       ; //offset 4 (4)
  struct amc_AMC_CMD                                        AMC_CMD             ; //offset 8 (8)
  struct amc_AMC_STATUS                                     AMC_STATUS          ; //offset C (12)
  struct amc_AMC_INTP                                       AMC_INTP            ; //offset 10 (16)
  struct amc_AMC_INTP_EN                                    AMC_INTP_EN         ; //offset 14 (20)
  struct amc_AMC_SIZE                                       AMC_SIZE            ; //offset 18 (24)
         uint32_t                                                __reserved__7       ; //offset 1C (28)
         uint64_t                                                __reserved__8       ; //offset 20 (32)
         uint64_t                                                __reserved__9       ; //offset 28 (40)
         uint64_t                                                __reserved__10      ; //offset 30 (48)
         uint64_t                                                __reserved__11      ; //offset 38 (56)
         uint64_t                                                __reserved__12      ; //offset 40 (64)
         uint64_t                                                __reserved__13      ; //offset 48 (72)
         uint64_t                                                __reserved__14      ; //offset 50 (80)
         uint64_t                                                __reserved__15      ; //offset 58 (88)
         uint64_t                                                __reserved__16      ; //offset 60 (96)
         uint64_t                                                __reserved__17      ; //offset 68 (104)
         uint64_t                                                __reserved__18      ; //offset 70 (112)
         uint64_t                                                __reserved__19      ; //offset 78 (120)
         uint64_t                                                __reserved__20      ; //offset 80 (128)
         uint64_t                                                __reserved__21      ; //offset 88 (136)
         uint64_t                                                __reserved__22      ; //offset 90 (144)
         uint64_t                                                __reserved__23      ; //offset 98 (152)
         uint64_t                                                __reserved__24      ; //offset A0 (160)
         uint64_t                                                __reserved__25      ; //offset A8 (168)
         uint64_t                                                __reserved__26      ; //offset B0 (176)
         uint64_t                                                __reserved__27      ; //offset B8 (184)
         uint64_t                                                __reserved__28      ; //offset C0 (192)
         uint64_t                                                __reserved__29      ; //offset C8 (200)
         uint64_t                                                __reserved__30      ; //offset D0 (208)
         uint64_t                                                __reserved__31      ; //offset D8 (216)
         uint64_t                                                __reserved__32      ; //offset E0 (224)
         uint64_t                                                __reserved__33      ; //offset E8 (232)
         uint64_t                                                __reserved__34      ; //offset F0 (240)
         uint32_t                                                __reserved__35      ; //offset F8 (248)
  struct amc_AMC_ACC_ADDR                                   AMC_ACC_ADDR        ; //offset FC (252)
  struct amc_AMC_ACC_ZMP_DATA                               AMC_ACC_ZMP_DATA    ; //offset 100 (256)
         uint32_t                                                __reserved__38      ; //offset 104 (260)
         uint64_t                                                __reserved__39      ; //offset 108 (264)
         uint64_t                                                __reserved__40      ; //offset 110 (272)
         uint64_t                                                __reserved__41      ; //offset 118 (280)
         uint64_t                                                __reserved__42      ; //offset 120 (288)
         uint64_t                                                __reserved__43      ; //offset 128 (296)
         uint64_t                                                __reserved__44      ; //offset 130 (304)
         uint64_t                                                __reserved__45      ; //offset 138 (312)
         uint64_t                                                __reserved__46      ; //offset 140 (320)
         uint64_t                                                __reserved__47      ; //offset 148 (328)
         uint64_t                                                __reserved__48      ; //offset 150 (336)
         uint64_t                                                __reserved__49      ; //offset 158 (344)
         uint64_t                                                __reserved__50      ; //offset 160 (352)
         uint64_t                                                __reserved__51      ; //offset 168 (360)
         uint64_t                                                __reserved__52      ; //offset 170 (368)
         uint64_t                                                __reserved__53      ; //offset 178 (376)
         uint64_t                                                __reserved__54      ; //offset 180 (384)
         uint64_t                                                __reserved__55      ; //offset 188 (392)
         uint64_t                                                __reserved__56      ; //offset 190 (400)
         uint64_t                                                __reserved__57      ; //offset 198 (408)
         uint64_t                                                __reserved__58      ; //offset 1A0 (416)
         uint64_t                                                __reserved__59      ; //offset 1A8 (424)
         uint64_t                                                __reserved__60      ; //offset 1B0 (432)
         uint64_t                                                __reserved__61      ; //offset 1B8 (440)
         uint64_t                                                __reserved__62      ; //offset 1C0 (448)
         uint64_t                                                __reserved__63      ; //offset 1C8 (456)
         uint64_t                                                __reserved__64      ; //offset 1D0 (464)
         uint64_t                                                __reserved__65      ; //offset 1D8 (472)
         uint64_t                                                __reserved__66      ; //offset 1E0 (480)
         uint64_t                                                __reserved__67      ; //offset 1E8 (488)
         uint64_t                                                __reserved__68      ; //offset 1F0 (496)
         uint64_t                                                __reserved__69      ; //offset 1F8 (504)
  struct amc_AMC_ACC_ECP_DATA                               AMC_ACC_ECP_DATA    ; //offset 200 (512)
}; //amc


struct hash_Hash_Configure
{
  union {
#define hash_Hash_Configure_address 0x00001800
    uint32_t value;
    struct {
      uint32_t alg_select                                                             :  3; // [ 2: 0]
      uint32_t hash_mode                                                              :  1; // [ 3: 3]
      uint32_t output_data_mode                                                       :  1; // [ 4: 4]
      uint32_t byte_swap                                                              :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct hash_Hash_control
{
  union {
#define hash_Hash_control_address 0x00001804
    uint32_t value;
    struct {
      uint32_t hash_op_mode                                                           :  2; // [ 1: 0]
      uint32_t hardware_padded_required                                               :  1; // [ 2: 2]
      uint32_t reset                                                                  :  1; // [ 3: 3]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct hash_Hash_Command
{
  union {
#define hash_Hash_Command_address 0x00001808
    uint32_t value;
    struct {
      uint32_t com_start                                                              :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct hash_Hash_Status
{
  union {
#define hash_Hash_Status_address 0x0000180C
    uint32_t value;
    struct {
      uint32_t hash_done                                                              :  1; // [ 0: 0]
      uint32_t hash_busy                                                              :  1; // [ 1: 1]
      uint32_t xfer_sm_busy                                                           :  1; // [ 2: 2]
      uint32_t HMAC_sm_busy                                                           :  1; // [ 3: 3]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct hash_incoming_segment_size
{
  union {
#define hash_incoming_segment_size_address 0x00001810
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash___reserved__5
{
  union {
#define hash___reserved__5_address 0x00001814
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_total_message_size_low
{
  union {
#define hash_total_message_size_low_address 0x00001818
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_total_message_size_high
{
  union {
#define hash_total_message_size_high_address 0x0000181C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_A
{
  union {
#define hash_digest_context_A_address 0x00001820
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_B
{
  union {
#define hash_digest_context_B_address 0x00001824
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_C
{
  union {
#define hash_digest_context_C_address 0x00001828
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_D
{
  union {
#define hash_digest_context_D_address 0x0000182C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_E
{
  union {
#define hash_digest_context_E_address 0x00001830
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_F
{
  union {
#define hash_digest_context_F_address 0x00001834
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_G
{
  union {
#define hash_digest_context_G_address 0x00001838
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_H
{
  union {
#define hash_digest_context_H_address 0x0000183C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_A_HIGH
{
  union {
#define hash_digest_context_A_HIGH_address 0x00001840
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_B_HIGH
{
  union {
#define hash_digest_context_B_HIGH_address 0x00001844
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_C_HIGH
{
  union {
#define hash_digest_context_C_HIGH_address 0x00001848
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_D_HIGH
{
  union {
#define hash_digest_context_D_HIGH_address 0x0000184C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_E_HIGH
{
  union {
#define hash_digest_context_E_HIGH_address 0x00001850
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_F_HIGH
{
  union {
#define hash_digest_context_F_HIGH_address 0x00001854
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_G_HIGH
{
  union {
#define hash_digest_context_G_HIGH_address 0x00001858
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_digest_context_H_HIGH
{
  union {
#define hash_digest_context_H_HIGH_address 0x0000185C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_A
{
  union {
#define hash_contexto_A_address 0x00001860
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_B
{
  union {
#define hash_contexto_B_address 0x00001864
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_C
{
  union {
#define hash_contexto_C_address 0x00001868
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_D
{
  union {
#define hash_contexto_D_address 0x0000186C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_E
{
  union {
#define hash_contexto_E_address 0x00001870
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_F
{
  union {
#define hash_contexto_F_address 0x00001874
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_G
{
  union {
#define hash_contexto_G_address 0x00001878
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_H
{
  union {
#define hash_contexto_H_address 0x0000187C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_A_HIGH
{
  union {
#define hash_contexto_A_HIGH_address 0x00001880
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_B_HIGH
{
  union {
#define hash_contexto_B_HIGH_address 0x00001884
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_C_HIGH
{
  union {
#define hash_contexto_C_HIGH_address 0x00001888
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_D_HIGH
{
  union {
#define hash_contexto_D_HIGH_address 0x0000188C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_E_HIGH
{
  union {
#define hash_contexto_E_HIGH_address 0x00001890
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_F_HIGH
{
  union {
#define hash_contexto_F_HIGH_address 0x00001894
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_G_HIGH
{
  union {
#define hash_contexto_G_HIGH_address 0x00001898
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_contexto_H_HIGH
{
  union {
#define hash_contexto_H_HIGH_address 0x0000189C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_len
{
  union {
#define hash_key_len_address 0x000018A0
    uint32_t value;
    struct {
      uint32_t len                                                                    :  8; // [ 7: 0]
      uint32_t __reserved__0                                                          : 24; // [31: 8]
    } b;
  } u;
};

struct hash_key_0
{
  union {
#define hash_key_0_address 0x000018A4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_1
{
  union {
#define hash_key_1_address 0x000018A8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_2
{
  union {
#define hash_key_2_address 0x000018AC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_3
{
  union {
#define hash_key_3_address 0x000018B0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_4
{
  union {
#define hash_key_4_address 0x000018B4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_5
{
  union {
#define hash_key_5_address 0x000018B8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_6
{
  union {
#define hash_key_6_address 0x000018BC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_7
{
  union {
#define hash_key_7_address 0x000018C0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_8
{
  union {
#define hash_key_8_address 0x000018C4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_9
{
  union {
#define hash_key_9_address 0x000018C8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_10
{
  union {
#define hash_key_10_address 0x000018CC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_11
{
  union {
#define hash_key_11_address 0x000018D0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_12
{
  union {
#define hash_key_12_address 0x000018D4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_13
{
  union {
#define hash_key_13_address 0x000018D8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_14
{
  union {
#define hash_key_14_address 0x000018DC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_15
{
  union {
#define hash_key_15_address 0x000018E0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_16
{
  union {
#define hash_key_16_address 0x000018E4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_17
{
  union {
#define hash_key_17_address 0x000018E8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_18
{
  union {
#define hash_key_18_address 0x000018EC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_19
{
  union {
#define hash_key_19_address 0x000018F0
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_20
{
  union {
#define hash_key_20_address 0x000018F4
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_21
{
  union {
#define hash_key_21_address 0x000018F8
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_22
{
  union {
#define hash_key_22_address 0x000018FC
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_23
{
  union {
#define hash_key_23_address 0x00001900
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_24
{
  union {
#define hash_key_24_address 0x00001904
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_25
{
  union {
#define hash_key_25_address 0x00001908
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_26
{
  union {
#define hash_key_26_address 0x0000190C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_27
{
  union {
#define hash_key_27_address 0x00001910
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_28
{
  union {
#define hash_key_28_address 0x00001914
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_29
{
  union {
#define hash_key_29_address 0x00001918
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_30
{
  union {
#define hash_key_30_address 0x0000191C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash_key_31
{
  union {
#define hash_key_31_address 0x00001920
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct hash
{
  struct hash_Hash_Configure                                Hash_Configure      ; //offset 0 (0)
  struct hash_Hash_control                                  Hash_control        ; //offset 4 (4)
  struct hash_Hash_Command                                  Hash_Command        ; //offset 8 (8)
  struct hash_Hash_Status                                   Hash_Status         ; //offset C (12)
  struct hash_incoming_segment_size                         incoming_segment_size; //offset 10 (16)
         uint32_t                                                __reserved__5       ; //offset 14 (20)
  struct hash_total_message_size_low                        total_message_size_low; //offset 18 (24)
  struct hash_total_message_size_high                       total_message_size_high; //offset 1C (28)
  struct hash_digest_context_A                              digest_context_A    ; //offset 20 (32)
  struct hash_digest_context_B                              digest_context_B    ; //offset 24 (36)
  struct hash_digest_context_C                              digest_context_C    ; //offset 28 (40)
  struct hash_digest_context_D                              digest_context_D    ; //offset 2C (44)
  struct hash_digest_context_E                              digest_context_E    ; //offset 30 (48)
  struct hash_digest_context_F                              digest_context_F    ; //offset 34 (52)
  struct hash_digest_context_G                              digest_context_G    ; //offset 38 (56)
  struct hash_digest_context_H                              digest_context_H    ; //offset 3C (60)
  struct hash_digest_context_A_HIGH                         digest_context_A_HIGH; //offset 40 (64)
  struct hash_digest_context_B_HIGH                         digest_context_B_HIGH; //offset 44 (68)
  struct hash_digest_context_C_HIGH                         digest_context_C_HIGH; //offset 48 (72)
  struct hash_digest_context_D_HIGH                         digest_context_D_HIGH; //offset 4C (76)
  struct hash_digest_context_E_HIGH                         digest_context_E_HIGH; //offset 50 (80)
  struct hash_digest_context_F_HIGH                         digest_context_F_HIGH; //offset 54 (84)
  struct hash_digest_context_G_HIGH                         digest_context_G_HIGH; //offset 58 (88)
  struct hash_digest_context_H_HIGH                         digest_context_H_HIGH; //offset 5C (92)
  struct hash_contexto_A                                    contexto_A          ; //offset 60 (96)
  struct hash_contexto_B                                    contexto_B          ; //offset 64 (100)
  struct hash_contexto_C                                    contexto_C          ; //offset 68 (104)
  struct hash_contexto_D                                    contexto_D          ; //offset 6C (108)
  struct hash_contexto_E                                    contexto_E          ; //offset 70 (112)
  struct hash_contexto_F                                    contexto_F          ; //offset 74 (116)
  struct hash_contexto_G                                    contexto_G          ; //offset 78 (120)
  struct hash_contexto_H                                    contexto_H          ; //offset 7C (124)
  struct hash_contexto_A_HIGH                               contexto_A_HIGH     ; //offset 80 (128)
  struct hash_contexto_B_HIGH                               contexto_B_HIGH     ; //offset 84 (132)
  struct hash_contexto_C_HIGH                               contexto_C_HIGH     ; //offset 88 (136)
  struct hash_contexto_D_HIGH                               contexto_D_HIGH     ; //offset 8C (140)
  struct hash_contexto_E_HIGH                               contexto_E_HIGH     ; //offset 90 (144)
  struct hash_contexto_F_HIGH                               contexto_F_HIGH     ; //offset 94 (148)
  struct hash_contexto_G_HIGH                               contexto_G_HIGH     ; //offset 98 (152)
  struct hash_contexto_H_HIGH                               contexto_H_HIGH     ; //offset 9C (156)
  struct hash_key_len                                       key_len             ; //offset A0 (160)
  struct hash_key_0                                         key_0               ; //offset A4 (164)
  struct hash_key_1                                         key_1               ; //offset A8 (168)
  struct hash_key_2                                         key_2               ; //offset AC (172)
  struct hash_key_3                                         key_3               ; //offset B0 (176)
  struct hash_key_4                                         key_4               ; //offset B4 (180)
  struct hash_key_5                                         key_5               ; //offset B8 (184)
  struct hash_key_6                                         key_6               ; //offset BC (188)
  struct hash_key_7                                         key_7               ; //offset C0 (192)
  struct hash_key_8                                         key_8               ; //offset C4 (196)
  struct hash_key_9                                         key_9               ; //offset C8 (200)
  struct hash_key_10                                        key_10              ; //offset CC (204)
  struct hash_key_11                                        key_11              ; //offset D0 (208)
  struct hash_key_12                                        key_12              ; //offset D4 (212)
  struct hash_key_13                                        key_13              ; //offset D8 (216)
  struct hash_key_14                                        key_14              ; //offset DC (220)
  struct hash_key_15                                        key_15              ; //offset E0 (224)
  struct hash_key_16                                        key_16              ; //offset E4 (228)
  struct hash_key_17                                        key_17              ; //offset E8 (232)
  struct hash_key_18                                        key_18              ; //offset EC (236)
  struct hash_key_19                                        key_19              ; //offset F0 (240)
  struct hash_key_20                                        key_20              ; //offset F4 (244)
  struct hash_key_21                                        key_21              ; //offset F8 (248)
  struct hash_key_22                                        key_22              ; //offset FC (252)
  struct hash_key_23                                        key_23              ; //offset 100 (256)
  struct hash_key_24                                        key_24              ; //offset 104 (260)
  struct hash_key_25                                        key_25              ; //offset 108 (264)
  struct hash_key_26                                        key_26              ; //offset 10C (268)
  struct hash_key_27                                        key_27              ; //offset 110 (272)
  struct hash_key_28                                        key_28              ; //offset 114 (276)
  struct hash_key_29                                        key_29              ; //offset 118 (280)
  struct hash_key_30                                        key_30              ; //offset 11C (284)
  struct hash_key_31                                        key_31              ; //offset 120 (288)
}; //hash


struct scratch_pad_SP_CTRL
{
  union {
#define scratch_pad_SP_CTRL_address 0x00001C00
    uint32_t value;
    struct {
      uint32_t SP_RESET                                                               :  1; // [ 0: 0]
      uint32_t SP_ACC_ERR_ENG_INT_EN                                                  :  1; // [ 1: 1]
      uint32_t SP_ACC_ERR_CPU_INT_EN                                                  :  1; // [ 2: 2]
      uint32_t SP_ACC_ERR_INJECT                                                      :  1; // [ 3: 3]
      uint32_t SP_ACCESS_SELECT                                                       :  2; // [ 5: 4]
      uint32_t __reserved__2                                                          :  1; // [ 6: 6]
      uint32_t SP_REV_MODE_SELECT                                                     :  1; // [ 7: 7]
      uint32_t __reserved__0                                                          : 24; // [31: 8]
    } b;
  } u;
};

struct scratch_pad_SP_STATUS
{
  union {
#define scratch_pad_SP_STATUS_address 0x00001C04
    uint32_t value;
    struct {
      uint32_t SP_ACCESS_ERROR                                                        :  1; // [ 0: 0]
      uint32_t SP_EMPTY                                                               :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct scratch_pad_SP_CPU_WR_ADDR
{
  union {
#define scratch_pad_SP_CPU_WR_ADDR_address 0x00001C08
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct scratch_pad_SP_CPU_WR_DATA
{
  union {
#define scratch_pad_SP_CPU_WR_DATA_address 0x00001C0C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct scratch_pad_SP_CPU_RD_ADDR
{
  union {
#define scratch_pad_SP_CPU_RD_ADDR_address 0x00001C10
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct scratch_pad_SP_CPU_RD_DATA
{
  union {
#define scratch_pad_SP_CPU_RD_DATA_address 0x00001C14
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct scratch_pad_SP_SET_WR_ADDR
{
  union {
#define scratch_pad_SP_SET_WR_ADDR_address 0x00001C18
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct scratch_pad_SP_SET_RD_ADDR
{
  union {
#define scratch_pad_SP_SET_RD_ADDR_address 0x00001C1C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct scratch_pad
{
  struct scratch_pad_SP_CTRL                                SP_CTRL             ; //offset 0 (0)
  struct scratch_pad_SP_STATUS                              SP_STATUS           ; //offset 4 (4)
  struct scratch_pad_SP_CPU_WR_ADDR                         SP_CPU_WR_ADDR      ; //offset 8 (8)
  struct scratch_pad_SP_CPU_WR_DATA                         SP_CPU_WR_DATA      ; //offset C (12)
  struct scratch_pad_SP_CPU_RD_ADDR                         SP_CPU_RD_ADDR      ; //offset 10 (16)
  struct scratch_pad_SP_CPU_RD_DATA                         SP_CPU_RD_DATA      ; //offset 14 (20)
  struct scratch_pad_SP_SET_WR_ADDR                         SP_SET_WR_ADDR      ; //offset 18 (24)
  struct scratch_pad_SP_SET_RD_ADDR                         SP_SET_RD_ADDR      ; //offset 1C (28)
}; //scratch_pad


struct spad_SPAD_CFG
{
  union {
#define spad_SPAD_CFG_address 0x00001C00
    uint32_t value;
    struct {
      uint32_t AccMode                                                                :  3; // [ 2: 0]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct spad_SPAD_CTRL
{
  union {
#define spad_SPAD_CTRL_address 0x00001C04
    uint32_t value;
    struct {
      uint32_t FrcPerr                                                                :  2; // [ 1: 0]
      uint32_t __reserved__2                                                          :  2; // [ 3: 2]
      uint32_t IgnPerr                                                                :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct spad_SPAD_CMD
{
  union {
#define spad_SPAD_CMD_address 0x00001C08
    uint32_t value;
    struct {
      uint32_t ZeroStart                                                              :  1; // [ 0: 0]
      uint32_t Reset                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct spad_SPAD_ENG_STATUS
{
  union {
#define spad_SPAD_ENG_STATUS_address 0x00001C0C
    uint32_t value;
    struct {
      uint32_t ZeroDone                                                               :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct spad_SPAD_INTP
{
  union {
#define spad_SPAD_INTP_address 0x00001C10
    uint32_t value;
    struct {
      uint32_t Perr                                                                   :  2; // [ 1: 0]
      uint32_t __reserved__2                                                          :  2; // [ 3: 2]
      uint32_t Merr                                                                   :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct spad_SPAD_INTP_EN
{
  union {
#define spad_SPAD_INTP_EN_address 0x00001C14
    uint32_t value;
    struct {
      uint32_t Perr                                                                   :  2; // [ 1: 0]
      uint32_t __reserved__2                                                          :  2; // [ 3: 2]
      uint32_t Merr                                                                   :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct spad_RSVD0
{
  union {
#define spad_RSVD0_address 0x00001C18
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad___reserved__7
{
  union {
#define spad___reserved__7_address 0x00001C1C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad_SPAD_FCT_WPTR
{
  union {
#define spad_SPAD_FCT_WPTR_address 0x00001C20
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad_SPAD_FCT_RPTR
{
  union {
#define spad_SPAD_FCT_RPTR_address 0x00001C24
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad_RSVD1
{
  union {
#define spad_RSVD1_address 0x00001C28
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad___reserved__11
{
  union {
#define spad___reserved__11_address 0x00001C2C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad_SPAD_FCT_STATUS0
{
  union {
#define spad_SPAD_FCT_STATUS0_address 0x00001C30
    uint32_t value;
    struct {
      uint32_t Empty                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__2                                                          :  3; // [ 3: 1]
      uint32_t Full                                                                   :  1; // [ 4: 4]
      uint32_t __reserved__0                                                          : 27; // [31: 5]
    } b;
  } u;
};

struct spad_SPAD_FCT_STATUS1
{
  union {
#define spad_SPAD_FCT_STATUS1_address 0x00001C34
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad_SPAD_FCT_STATUS2
{
  union {
#define spad_SPAD_FCT_STATUS2_address 0x00001C38
    uint32_t value;
    struct {
      uint32_t Bnum                                                                   :  3; // [ 2: 0]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct spad_RSVD2
{
  union {
#define spad_RSVD2_address 0x00001C3C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad___reserved__16
{
  union {
#define spad___reserved__16_address 0x00001C40
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct spad___reserved__17
{
  union {
#define spad___reserved__17_address 0x00001C48
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct spad_SPAD_ACC_ADDR
{
  union {
#define spad_SPAD_ACC_ADDR_address 0x00001C50
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad_SPAD_ACC_DATA
{
  union {
#define spad_SPAD_ACC_DATA_address 0x00001C54
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct spad
{
  struct spad_SPAD_CFG                                      SPAD_CFG            ; //offset 0 (0)
  struct spad_SPAD_CTRL                                     SPAD_CTRL           ; //offset 4 (4)
  struct spad_SPAD_CMD                                      SPAD_CMD            ; //offset 8 (8)
  struct spad_SPAD_ENG_STATUS                               SPAD_ENG_STATUS     ; //offset C (12)
  struct spad_SPAD_INTP                                     SPAD_INTP           ; //offset 10 (16)
  struct spad_SPAD_INTP_EN                                  SPAD_INTP_EN        ; //offset 14 (20)
         uint32_t                                                __reserved__6       ; //offset 18 (24)
         uint32_t                                                __reserved__7       ; //offset 1C (28)
  struct spad_SPAD_FCT_WPTR                                 SPAD_FCT_WPTR       ; //offset 20 (32)
  struct spad_SPAD_FCT_RPTR                                 SPAD_FCT_RPTR       ; //offset 24 (36)
         uint32_t                                                __reserved__10      ; //offset 28 (40)
         uint32_t                                                __reserved__11      ; //offset 2C (44)
  struct spad_SPAD_FCT_STATUS0                              SPAD_FCT_STATUS0    ; //offset 30 (48)
  struct spad_SPAD_FCT_STATUS1                              SPAD_FCT_STATUS1    ; //offset 34 (52)
  struct spad_SPAD_FCT_STATUS2                              SPAD_FCT_STATUS2    ; //offset 38 (56)
         uint32_t                                                __reserved__15      ; //offset 3C (60)
         uint64_t                                                __reserved__16      ; //offset 40 (64)
         uint64_t                                                __reserved__17      ; //offset 48 (72)
  struct spad_SPAD_ACC_ADDR                                 SPAD_ACC_ADDR       ; //offset 50 (80)
  struct spad_SPAD_ACC_DATA                                 SPAD_ACC_DATA       ; //offset 54 (84)
}; //spad


struct zmodp_ZMODP_CFG_REG
{
  union {
#define zmodp_ZMODP_CFG_REG_address 0x00002000
    uint32_t value;
    struct {
      uint32_t OP_MODE                                                                :  3; // [ 2: 0]
      uint32_t MOD_SIZE                                                               :  3; // [ 5: 3]
      uint32_t NUM_COEFS                                                              :  7; // [12: 6]
      uint32_t CODE_MODE                                                              :  1; // [13:13]
      uint32_t __reserved__2                                                          :  1; // [14:14]
      uint32_t USE_OTP_EC521_DK                                                       :  1; // [15:15]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct zmodp_ZMODP_CTRL_REG
{
  union {
#define zmodp_ZMODP_CTRL_REG_address 0x00002004
    uint32_t value;
    struct {
      uint32_t RESET                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct zmodp_ZMODP_CMD_REG
{
  union {
#define zmodp_ZMODP_CMD_REG_address 0x00002008
    uint32_t value;
    struct {
      uint32_t ZERORIZE_START                                                         :  1; // [ 0: 0]
      uint32_t CMD_START                                                              :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct zmodp_ZMODP_STATUS_REG
{
  union {
#define zmodp_ZMODP_STATUS_REG_address 0x0000200C
    uint32_t value;
    struct {
      uint32_t BUSY                                                                   :  1; // [ 0: 0]
      uint32_t MFSM                                                                   :  5; // [ 5: 1]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct zmodp_ZMODP_INTRPT_SRC_REG
{
  union {
#define zmodp_ZMODP_INTRPT_SRC_REG_address 0x00002010
    uint32_t value;
    struct {
      uint32_t CMD_DONE_INT                                                           :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct zmodp_ZMODP_INTRPT_SRC_EN_REG
{
  union {
#define zmodp_ZMODP_INTRPT_SRC_EN_REG_address 0x00002014
    uint32_t value;
    struct {
      uint32_t CMD_DONE_EN                                                            :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct zmodp_ZMODP_MEM
{
  union {
#define zmodp_ZMODP_MEM_address 0x00002018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct zmodp
{
  struct zmodp_ZMODP_CFG_REG                                ZMODP_CFG_REG       ; //offset 0 (0)
  struct zmodp_ZMODP_CTRL_REG                               ZMODP_CTRL_REG      ; //offset 4 (4)
  struct zmodp_ZMODP_CMD_REG                                ZMODP_CMD_REG       ; //offset 8 (8)
  struct zmodp_ZMODP_STATUS_REG                             ZMODP_STATUS_REG    ; //offset C (12)
  struct zmodp_ZMODP_INTRPT_SRC_REG                         ZMODP_INTRPT_SRC_REG; //offset 10 (16)
  struct zmodp_ZMODP_INTRPT_SRC_EN_REG                      ZMODP_INTRPT_SRC_EN_REG; //offset 14 (20)
  struct zmodp_ZMODP_MEM                                    ZMODP_MEM           ; //offset 18 (24)
}; //zmodp


struct ecrsa_zmp_ZMP_CFG_REG
{
  union {
#define ecrsa_zmp_ZMP_CFG_REG_address 0x00002000
    uint32_t value;
    struct {
      uint32_t OP_MODE                                                                :  3; // [ 2: 0]
      uint32_t MOD_SIZE                                                               :  3; // [ 5: 3]
      uint32_t NUM_COEFS                                                              :  7; // [12: 6]
      uint32_t CODE_MODE                                                              :  1; // [13:13]
      uint32_t __reserved__2                                                          :  1; // [14:14]
      uint32_t USE_OTP_EC521_DK                                                       :  1; // [15:15]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct ecrsa_zmp_ZMP_CTRL_REG
{
  union {
#define ecrsa_zmp_ZMP_CTRL_REG_address 0x00002004
    uint32_t value;
    struct {
      uint32_t RESET                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ecrsa_zmp_ZMP_CMD_REG
{
  union {
#define ecrsa_zmp_ZMP_CMD_REG_address 0x00002008
    uint32_t value;
    struct {
      uint32_t ZERORIZE_START                                                         :  1; // [ 0: 0]
      uint32_t START                                                                  :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct ecrsa_zmp_ZMP_STATUS_REG
{
  union {
#define ecrsa_zmp_ZMP_STATUS_REG_address 0x0000200C
    uint32_t value;
    struct {
      uint32_t BUSY                                                                   :  1; // [ 0: 0]
      uint32_t PUSM                                                                   :  3; // [ 3: 1]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct ecrsa_zmp_ZMP_INTRPT_REG
{
  union {
#define ecrsa_zmp_ZMP_INTRPT_REG_address 0x00002010
    uint32_t value;
    struct {
      uint32_t DONE                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ecrsa_zmp_ZMP_INTRPT_EN_REG
{
  union {
#define ecrsa_zmp_ZMP_INTRPT_EN_REG_address 0x00002014
    uint32_t value;
    struct {
      uint32_t DONE                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ecrsa_zmp_ZMP_SRAM
{
  union {
#define ecrsa_zmp_ZMP_SRAM_address 0x00002018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ecrsa_zmp
{
  struct ecrsa_zmp_ZMP_CFG_REG                              ZMP_CFG_REG         ; //offset 0 (0)
  struct ecrsa_zmp_ZMP_CTRL_REG                             ZMP_CTRL_REG        ; //offset 4 (4)
  struct ecrsa_zmp_ZMP_CMD_REG                              ZMP_CMD_REG         ; //offset 8 (8)
  struct ecrsa_zmp_ZMP_STATUS_REG                           ZMP_STATUS_REG      ; //offset C (12)
  struct ecrsa_zmp_ZMP_INTRPT_REG                           ZMP_INTRPT_REG      ; //offset 10 (16)
  struct ecrsa_zmp_ZMP_INTRPT_EN_REG                        ZMP_INTRPT_EN_REG   ; //offset 14 (20)
  struct ecrsa_zmp_ZMP_SRAM                                 ZMP_SRAM            ; //offset 18 (24)
}; //ecrsa_zmp


struct mct_MCT_CTRL_REG
{
  union {
#define mct_MCT_CTRL_REG_address 0x00002400
    uint32_t value;
    struct {
      uint32_t RESET                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct mct_MCT_CMD_REG
{
  union {
#define mct_MCT_CMD_REG_address 0x00002404
    uint32_t value;
    struct {
      uint32_t CMD_START                                                              :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct mct_MCT_STATUS_REG
{
  union {
#define mct_MCT_STATUS_REG_address 0x00002408
    uint32_t value;
    struct {
      uint32_t BUSY                                                                   :  1; // [ 0: 0]
      uint32_t MFSM_STATE                                                             :  2; // [ 2: 1]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct mct_MCT_INTRPT_REG
{
  union {
#define mct_MCT_INTRPT_REG_address 0x0000240C
    uint32_t value;
    struct {
      uint32_t CMD_DONE_INT                                                           :  1; // [ 0: 0]
      uint32_t EXCEED_RANGE                                                           :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct mct_MCT_INTRPT_MASK_REG
{
  union {
#define mct_MCT_INTRPT_MASK_REG_address 0x00002410
    uint32_t value;
    struct {
      uint32_t CMD_DONE_INT                                                           :  1; // [ 0: 0]
      uint32_t EXCEED_RANGE                                                           :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct mct_MCT_VAL_W0
{
  union {
#define mct_MCT_VAL_W0_address 0x00002414
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mct_MCT_VAL_W1
{
  union {
#define mct_MCT_VAL_W1_address 0x00002418
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mct_MCT_VAL_W2
{
  union {
#define mct_MCT_VAL_W2_address 0x0000241C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mct_MCT_VAL_W3
{
  union {
#define mct_MCT_VAL_W3_address 0x00002420
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mct_MCT_VAL_W4
{
  union {
#define mct_MCT_VAL_W4_address 0x00002424
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct mct
{
  struct mct_MCT_CTRL_REG                                   MCT_CTRL_REG        ; //offset 0 (0)
  struct mct_MCT_CMD_REG                                    MCT_CMD_REG         ; //offset 4 (4)
  struct mct_MCT_STATUS_REG                                 MCT_STATUS_REG      ; //offset 8 (8)
  struct mct_MCT_INTRPT_REG                                 MCT_INTRPT_REG      ; //offset C (12)
  struct mct_MCT_INTRPT_MASK_REG                            MCT_INTRPT_MASK_REG ; //offset 10 (16)
  struct mct_MCT_VAL_W0                                     MCT_VAL_W0          ; //offset 14 (20)
  struct mct_MCT_VAL_W1                                     MCT_VAL_W1          ; //offset 18 (24)
  struct mct_MCT_VAL_W2                                     MCT_VAL_W2          ; //offset 1C (28)
  struct mct_MCT_VAL_W3                                     MCT_VAL_W3          ; //offset 20 (32)
  struct mct_MCT_VAL_W4                                     MCT_VAL_W4          ; //offset 24 (36)
}; //mct


struct otp_efuse_CONTROL
{
  union {
#define otp_efuse_CONTROL_address 0x00002800
    uint32_t value;
    struct {
      uint32_t START                                                                  :  1; // [ 0: 0]
      uint32_t WRITE                                                                  :  1; // [ 1: 1]
      uint32_t READ                                                                   :  1; // [ 2: 2]
      uint32_t OTP_SEL                                                                :  3; // [ 5: 3]
      uint32_t TEST_MODE                                                              :  1; // [ 6: 6]
      uint32_t READ_BACK                                                              :  1; // [ 7: 7]
      uint32_t __reserved__0                                                          : 24; // [31: 8]
    } b;
  } u;
};

struct otp_efuse_INT
{
  union {
#define otp_efuse_INT_address 0x00002804
    uint32_t value;
    struct {
      uint32_t DONE                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct otp_efuse_INT_MASK
{
  union {
#define otp_efuse_INT_MASK_address 0x00002808
    uint32_t value;
    struct {
      uint32_t DONE_MASK                                                              :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct otp_efuse___reserved__3
{
  union {
#define otp_efuse___reserved__3_address 0x0000280C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP0
{
  union {
#define otp_efuse_OTP0_address 0x00002810
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP1
{
  union {
#define otp_efuse_OTP1_address 0x00002814
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP2
{
  union {
#define otp_efuse_OTP2_address 0x00002818
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP3
{
  union {
#define otp_efuse_OTP3_address 0x0000281C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP4
{
  union {
#define otp_efuse_OTP4_address 0x00002820
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP5
{
  union {
#define otp_efuse_OTP5_address 0x00002824
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP6
{
  union {
#define otp_efuse_OTP6_address 0x00002828
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP7
{
  union {
#define otp_efuse_OTP7_address 0x0000282C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_OTP8
{
  union {
#define otp_efuse_OTP8_address 0x00002830
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_TEST_CTRL
{
  union {
#define otp_efuse_TEST_CTRL_address 0x00002834
    uint32_t value;
    struct {
      uint32_t SCLK                                                                   :  1; // [ 0: 0]
      uint32_t RSTB                                                                   :  1; // [ 1: 1]
      uint32_t CSB                                                                    :  1; // [ 2: 2]
      uint32_t WPROT                                                                  :  1; // [ 3: 3]
      uint32_t PGM                                                                    :  1; // [ 4: 4]
      uint32_t PCODE                                                                  :  1; // [ 5: 5]
      uint32_t PCLK                                                                   :  1; // [ 6: 6]
      uint32_t __reserved__0                                                          : 25; // [31: 7]
    } b;
  } u;
};

struct otp_efuse_TEST
{
  union {
#define otp_efuse_TEST_address 0x00002838
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_USER0
{
  union {
#define otp_efuse_USER0_address 0x0000283C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse___reserved__16
{
  union {
#define otp_efuse___reserved__16_address 0x00002840
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_MATCH_CODE
{
  union {
#define otp_efuse_MATCH_CODE_address 0x00002844
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct otp_efuse_VOLT_CTRL
{
  union {
#define otp_efuse_VOLT_CTRL_address 0x00002848
    uint32_t value;
    struct {
      uint32_t VOLTAGE_CTRL                                                           :  2; // [ 1: 0]
      uint32_t __reserved__2                                                          :  6; // [ 7: 2]
      uint32_t VTR_ON                                                                 :  1; // [ 8: 8]
      uint32_t __reserved__0                                                          : 23; // [31: 9]
    } b;
  } u;
};

struct otp_efuse
{
  struct otp_efuse_CONTROL                                  CONTROL             ; //offset 0 (0)
  struct otp_efuse_INT                                      INT                 ; //offset 4 (4)
  struct otp_efuse_INT_MASK                                 INT_MASK            ; //offset 8 (8)
         uint32_t                                                __reserved__3       ; //offset C (12)
  struct otp_efuse_OTP0                                     OTP0                ; //offset 10 (16)
  struct otp_efuse_OTP1                                     OTP1                ; //offset 14 (20)
  struct otp_efuse_OTP2                                     OTP2                ; //offset 18 (24)
  struct otp_efuse_OTP3                                     OTP3                ; //offset 1C (28)
  struct otp_efuse_OTP4                                     OTP4                ; //offset 20 (32)
  struct otp_efuse_OTP5                                     OTP5                ; //offset 24 (36)
  struct otp_efuse_OTP6                                     OTP6                ; //offset 28 (40)
  struct otp_efuse_OTP7                                     OTP7                ; //offset 2C (44)
  struct otp_efuse_OTP8                                     OTP8                ; //offset 30 (48)
  struct otp_efuse_TEST_CTRL                                TEST_CTRL           ; //offset 34 (52)
  struct otp_efuse_TEST                                     TEST                ; //offset 38 (56)
  struct otp_efuse_USER0                                    USER0               ; //offset 3C (60)
         uint32_t                                                __reserved__16      ; //offset 40 (64)
  struct otp_efuse_MATCH_CODE                               MATCH_CODE          ; //offset 44 (68)
  struct otp_efuse_VOLT_CTRL                                VOLT_CTRL           ; //offset 48 (72)
}; //otp_efuse


struct ebg_EBG_CTRL_REG
{
  union {
#define ebg_EBG_CTRL_REG_address 0x00002C00
    uint32_t value;
    struct {
      uint32_t __reserved__11                                                         :  6; // [ 5: 0]
      uint32_t PP_ENA                                                                 :  1; // [ 6: 6]
      uint32_t RNG_SLOW_OSC_ENA                                                       :  1; // [ 7: 7]
      uint32_t RNG_FAST_OSC_ENA                                                       :  1; // [ 8: 8]
      uint32_t RNG_PU_BIAS                                                            :  1; // [ 9: 9]
      uint32_t RNG_RSTN                                                               :  1; // [10:10]
      uint32_t RNG_CLK_SEL                                                            :  1; // [11:11]
      uint32_t ENTROPY_REQ                                                            :  1; // [12:12]
      uint32_t ENTROPY_SEL                                                            :  2; // [14:13]
      uint32_t ENTROPY_MODE                                                           :  1; // [15:15]
      uint32_t RNG_SLOW_DIV_VAL                                                       :  8; // [23:16]
      uint32_t DS_RATIO                                                               :  8; // [31:24]
    } b;
  } u;
};

struct ebg_EBG_ENTROPY_VALUE_REG
{
  union {
#define ebg_EBG_ENTROPY_VALUE_REG_address 0x00002C04
    uint32_t value;
    struct {
      uint32_t ENTROPY_VALUE                                                          : 16; // [15: 0]
      uint32_t __reserved__1                                                          : 15; // [30:16]
      uint32_t ENTROPY_RDY                                                            :  1; // [31:31]
    } b;
  } u;
};

struct ebg
{
  struct ebg_EBG_CTRL_REG                                   EBG_CTRL_REG        ; //offset 0 (0)
  struct ebg_EBG_ENTROPY_VALUE_REG                          EBG_ENTROPY_VALUE_REG; //offset 4 (4)
}; //ebg


struct rctrl_MEM_CTRL_CORE
{
  union {
#define rctrl_MEM_CTRL_CORE_address 0x00003000
    uint32_t value;
    struct {
      uint32_t RTC_0                                                                  :  2; // [ 1: 0]
      uint32_t WTC_0                                                                  :  2; // [ 3: 2]
      uint32_t PDWN_0                                                                 :  1; // [ 4: 4]
      uint32_t RTC_1                                                                  :  2; // [ 6: 5]
      uint32_t WTC_1                                                                  :  2; // [ 8: 7]
      uint32_t PDWN_1                                                                 :  1; // [ 9: 9]
      uint32_t RTC_2                                                                  :  2; // [11:10]
      uint32_t WTC_2                                                                  :  2; // [13:12]
      uint32_t PDWN_2                                                                 :  1; // [14:14]
      uint32_t RTC_3                                                                  :  2; // [16:15]
      uint32_t WTC_3                                                                  :  2; // [18:17]
      uint32_t PDWN_3                                                                 :  1; // [19:19]
      uint32_t RTC_4                                                                  :  2; // [21:20]
      uint32_t WTC_4                                                                  :  2; // [23:22]
      uint32_t PDWN_4                                                                 :  1; // [24:24]
      uint32_t RTC_5                                                                  :  2; // [26:25]
      uint32_t WTC_5                                                                  :  2; // [28:27]
      uint32_t PDWN_5                                                                 :  1; // [29:29]
      uint32_t __reserved__0                                                          :  2; // [31:30]
    } b;
  } u;
};

struct rctrl_BCM_MON_MOD_SEL
{
  union {
#define rctrl_BCM_MON_MOD_SEL_address 0x00003004
    uint32_t value;
    struct {
      uint32_t MOD_SEL                                                                :  8; // [ 7: 0]
      uint32_t __reserved__0                                                          : 24; // [31: 8]
    } b;
  } u;
};

struct rctrl_BCM_MON_SIG_SEL
{
  union {
#define rctrl_BCM_MON_SIG_SEL_address 0x00003008
    uint32_t value;
    struct {
      uint32_t SIG_SEL                                                                :  8; // [ 7: 0]
      uint32_t MON_DIS                                                                :  1; // [ 8: 8]
      uint32_t __reserved__0                                                          : 23; // [31: 9]
    } b;
  } u;
};

struct rctrl_BCM_MONITOR
{
  union {
#define rctrl_BCM_MONITOR_address 0x0000300C
    uint32_t value;
    struct {
      uint32_t BCM_MONITOR                                                            : 16; // [15: 0]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct rctrl_MEM_CTRL_PROC
{
  union {
#define rctrl_MEM_CTRL_PROC_address 0x00003010
    uint32_t value;
    struct {
      uint32_t RTC_0                                                                  :  2; // [ 1: 0]
      uint32_t WTC_0                                                                  :  2; // [ 3: 2]
      uint32_t PDWN_0                                                                 :  1; // [ 4: 4]
      uint32_t RTC_1                                                                  :  2; // [ 6: 5]
      uint32_t WTC_1                                                                  :  2; // [ 8: 7]
      uint32_t PDWN_1                                                                 :  1; // [ 9: 9]
      uint32_t RTC_ROM                                                                :  3; // [12:10]
      uint32_t RTC_REF_ROM                                                            :  2; // [14:13]
      uint32_t PDWN_ROM                                                               :  1; // [15:15]
      uint32_t __reserved__0                                                          : 16; // [31:16]
    } b;
  } u;
};

struct rctrl_PWR_CTRL
{
  union {
#define rctrl_PWR_CTRL_address 0x00003014
    uint32_t value;
    struct {
      uint32_t PWR_EVENT                                                              :  1; // [ 0: 0]
      uint32_t PWR_CTRL                                                               :  7; // [ 7: 1]
      uint32_t __reserved__0                                                          : 24; // [31: 8]
    } b;
  } u;
};

struct rctrl___reserved__6
{
  union {
#define rctrl___reserved__6_address 0x00003018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct rctrl___reserved__7
{
  union {
#define rctrl___reserved__7_address 0x0000301C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct rctrl_LKG_CTRL_CORE
{
  union {
#define rctrl_LKG_CTRL_CORE_address 0x00003020
    uint32_t value;
    struct {
      uint32_t MEM_0                                                                  :  2; // [ 1: 0]
      uint32_t MEM_1                                                                  :  2; // [ 3: 2]
      uint32_t MEM_2                                                                  :  2; // [ 5: 4]
      uint32_t MEM_3                                                                  :  2; // [ 7: 6]
      uint32_t MEM_4                                                                  :  2; // [ 9: 8]
      uint32_t MEM_5                                                                  :  2; // [11:10]
      uint32_t __reserved__0                                                          : 20; // [31:12]
    } b;
  } u;
};

struct rctrl_LKG_CTRL_PROC
{
  union {
#define rctrl_LKG_CTRL_PROC_address 0x00003024
    uint32_t value;
    struct {
      uint32_t SRAM_0                                                                 :  2; // [ 1: 0]
      uint32_t SRAM_1                                                                 :  2; // [ 3: 2]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct rctrl_BIST_CTRL_CORE
{
  union {
#define rctrl_BIST_CTRL_CORE_address 0x00003028
    uint32_t value;
    struct {
      uint32_t EXT_CTRL                                                               :  1; // [ 0: 0]
      uint32_t EXT_CLK_DISABLE                                                        :  1; // [ 1: 1]
      uint32_t EXT_NWE                                                                :  1; // [ 2: 2]
      uint32_t EXT_NRE                                                                :  1; // [ 3: 3]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct rctrl_BIST_CTRL_PROC
{
  union {
#define rctrl_BIST_CTRL_PROC_address 0x0000302C
    uint32_t value;
    struct {
      uint32_t M0_EXT_CTRL                                                            :  1; // [ 0: 0]
      uint32_t M0_EXT_CLK_DISABLE                                                     :  1; // [ 1: 1]
      uint32_t M0_EXT_NWRT                                                            :  1; // [ 2: 2]
      uint32_t M0_EXT_NCE                                                             :  1; // [ 3: 3]
      uint32_t __reserved__5                                                          : 12; // [15: 4]
      uint32_t M1_EXT_CTRL                                                            :  1; // [16:16]
      uint32_t M1_EXT_CLK_DISABLE                                                     :  1; // [17:17]
      uint32_t M1_EXT_NWRT                                                            :  1; // [18:18]
      uint32_t M1_EXT_NCE                                                             :  1; // [19:19]
      uint32_t __reserved__0                                                          : 12; // [31:20]
    } b;
  } u;
};

struct rctrl
{
  struct rctrl_MEM_CTRL_CORE                                MEM_CTRL_CORE       ; //offset 0 (0)
  struct rctrl_BCM_MON_MOD_SEL                              BCM_MON_MOD_SEL     ; //offset 4 (4)
  struct rctrl_BCM_MON_SIG_SEL                              BCM_MON_SIG_SEL     ; //offset 8 (8)
  struct rctrl_BCM_MONITOR                                  BCM_MONITOR         ; //offset C (12)
  struct rctrl_MEM_CTRL_PROC                                MEM_CTRL_PROC       ; //offset 10 (16)
  struct rctrl_PWR_CTRL                                     PWR_CTRL            ; //offset 14 (20)
         uint32_t                                                __reserved__6       ; //offset 18 (24)
         uint32_t                                                __reserved__7       ; //offset 1C (28)
  struct rctrl_LKG_CTRL_CORE                                LKG_CTRL_CORE       ; //offset 20 (32)
  struct rctrl_LKG_CTRL_PROC                                LKG_CTRL_PROC       ; //offset 24 (36)
  struct rctrl_BIST_CTRL_CORE                               BIST_CTRL_CORE      ; //offset 28 (40)
  struct rctrl_BIST_CTRL_PROC                               BIST_CTRL_PROC      ; //offset 2C (44)
}; //rctrl


struct sahb_AHB_SLV_CTRL
{
  union {
#define sahb_AHB_SLV_CTRL_address 0x00003400
    uint32_t value;
    struct {
      uint32_t AHB_SLV_EN                                                             :  1; // [ 0: 0]
      uint32_t IN_AHB_SL_BYTE_SWP                                                     :  1; // [ 1: 1]
      uint32_t OUT_AHB_SL_BYTE_SWP                                                    :  1; // [ 2: 2]
      uint32_t AHB_SL_WR_PROT                                                         :  1; // [ 3: 3]
      uint32_t AHB_SL_RD_PROT                                                         :  1; // [ 4: 4]
      uint32_t PAR_ERR_MSK                                                            :  1; // [ 5: 5]
      uint32_t __reserved__0                                                          : 26; // [31: 6]
    } b;
  } u;
};

struct sahb_AHB_SLV_INT
{
  union {
#define sahb_AHB_SLV_INT_address 0x00003404
    uint32_t value;
    struct {
      uint32_t AHB_SL_BUS_ERR                                                         :  1; // [ 0: 0]
      uint32_t APB_SLV_ERR                                                            :  1; // [ 1: 1]
      uint32_t APB_PAR_ERR                                                            :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct sahb_AHB_SLV_INT_MSK
{
  union {
#define sahb_AHB_SLV_INT_MSK_address 0x00003408
    uint32_t value;
    struct {
      uint32_t AHB_SL_APB_ERR_MSK                                                     :  1; // [ 0: 0]
      uint32_t AHB_SL_BUS_ERR_MSK                                                     :  1; // [ 1: 1]
      uint32_t APB_PAR_ERR_MSK                                                        :  1; // [ 2: 2]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct sahb_APB_ERR_ADDR
{
  union {
#define sahb_APB_ERR_ADDR_address 0x0000340C
    uint32_t value;
    struct {
      uint32_t APB_ERR_ADDR                                                           : 19; // [18: 0]
      uint32_t __reserved__0                                                          : 13; // [31:19]
    } b;
  } u;
};

struct sahb
{
  struct sahb_AHB_SLV_CTRL                                  AHB_SLV_CTRL        ; //offset 0 (0)
  struct sahb_AHB_SLV_INT                                   AHB_SLV_INT         ; //offset 4 (4)
  struct sahb_AHB_SLV_INT_MSK                               AHB_SLV_INT_MSK     ; //offset 8 (8)
  struct sahb_APB_ERR_ADDR                                  APB_ERR_ADDR        ; //offset C (12)
}; //sahb


struct apb_APB_ACCESS
{
  union {
#define apb_APB_ACCESS_address 0x00003800
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__1
{
  union {
#define apb___reserved__1_address 0x00003804
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__2
{
  union {
#define apb___reserved__2_address 0x00003808
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__3
{
  union {
#define apb___reserved__3_address 0x00003810
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__4
{
  union {
#define apb___reserved__4_address 0x00003818
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__5
{
  union {
#define apb___reserved__5_address 0x00003820
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__6
{
  union {
#define apb___reserved__6_address 0x00003828
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__7
{
  union {
#define apb___reserved__7_address 0x00003830
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__8
{
  union {
#define apb___reserved__8_address 0x00003838
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__9
{
  union {
#define apb___reserved__9_address 0x00003840
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__10
{
  union {
#define apb___reserved__10_address 0x00003848
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__11
{
  union {
#define apb___reserved__11_address 0x00003850
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__12
{
  union {
#define apb___reserved__12_address 0x00003858
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__13
{
  union {
#define apb___reserved__13_address 0x00003860
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__14
{
  union {
#define apb___reserved__14_address 0x00003868
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__15
{
  union {
#define apb___reserved__15_address 0x00003870
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__16
{
  union {
#define apb___reserved__16_address 0x00003878
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__17
{
  union {
#define apb___reserved__17_address 0x00003880
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__18
{
  union {
#define apb___reserved__18_address 0x00003888
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__19
{
  union {
#define apb___reserved__19_address 0x00003890
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__20
{
  union {
#define apb___reserved__20_address 0x00003898
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__21
{
  union {
#define apb___reserved__21_address 0x000038A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__22
{
  union {
#define apb___reserved__22_address 0x000038A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__23
{
  union {
#define apb___reserved__23_address 0x000038B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__24
{
  union {
#define apb___reserved__24_address 0x000038B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__25
{
  union {
#define apb___reserved__25_address 0x000038C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__26
{
  union {
#define apb___reserved__26_address 0x000038C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__27
{
  union {
#define apb___reserved__27_address 0x000038D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__28
{
  union {
#define apb___reserved__28_address 0x000038D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__29
{
  union {
#define apb___reserved__29_address 0x000038E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__30
{
  union {
#define apb___reserved__30_address 0x000038E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__31
{
  union {
#define apb___reserved__31_address 0x000038F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__32
{
  union {
#define apb___reserved__32_address 0x000038F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__33
{
  union {
#define apb___reserved__33_address 0x00003900
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__34
{
  union {
#define apb___reserved__34_address 0x00003908
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__35
{
  union {
#define apb___reserved__35_address 0x00003910
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__36
{
  union {
#define apb___reserved__36_address 0x00003918
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__37
{
  union {
#define apb___reserved__37_address 0x00003920
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__38
{
  union {
#define apb___reserved__38_address 0x00003928
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__39
{
  union {
#define apb___reserved__39_address 0x00003930
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__40
{
  union {
#define apb___reserved__40_address 0x00003938
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__41
{
  union {
#define apb___reserved__41_address 0x00003940
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__42
{
  union {
#define apb___reserved__42_address 0x00003948
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__43
{
  union {
#define apb___reserved__43_address 0x00003950
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__44
{
  union {
#define apb___reserved__44_address 0x00003958
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__45
{
  union {
#define apb___reserved__45_address 0x00003960
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__46
{
  union {
#define apb___reserved__46_address 0x00003968
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__47
{
  union {
#define apb___reserved__47_address 0x00003970
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__48
{
  union {
#define apb___reserved__48_address 0x00003978
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__49
{
  union {
#define apb___reserved__49_address 0x00003980
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__50
{
  union {
#define apb___reserved__50_address 0x00003988
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__51
{
  union {
#define apb___reserved__51_address 0x00003990
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__52
{
  union {
#define apb___reserved__52_address 0x00003998
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__53
{
  union {
#define apb___reserved__53_address 0x000039A0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__54
{
  union {
#define apb___reserved__54_address 0x000039A8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__55
{
  union {
#define apb___reserved__55_address 0x000039B0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__56
{
  union {
#define apb___reserved__56_address 0x000039B8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__57
{
  union {
#define apb___reserved__57_address 0x000039C0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__58
{
  union {
#define apb___reserved__58_address 0x000039C8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__59
{
  union {
#define apb___reserved__59_address 0x000039D0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__60
{
  union {
#define apb___reserved__60_address 0x000039D8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__61
{
  union {
#define apb___reserved__61_address 0x000039E0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__62
{
  union {
#define apb___reserved__62_address 0x000039E8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__63
{
  union {
#define apb___reserved__63_address 0x000039F0
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb___reserved__64
{
  union {
#define apb___reserved__64_address 0x000039F8
    uint64_t value;
    struct {
      uint64_t __nofield__;
    } b;
  } u;
};

struct apb_APB_BASE
{
  union {
#define apb_APB_BASE_address 0x00003A00
    uint32_t value;
    struct {
      uint32_t APB_BASE                                                               : 30; // [29: 0]
      uint32_t __reserved__0                                                          :  2; // [31:30]
    } b;
  } u;
};

struct apb_APB_INTF
{
  union {
#define apb_APB_INTF_address 0x00003A04
    uint32_t value;
    struct {
      uint32_t APB_INTF                                                               :  2; // [ 1: 0]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct apb_APB_INT_STATUS
{
  union {
#define apb_APB_INT_STATUS_address 0x00003A08
    uint32_t value;
    struct {
      uint32_t APB_INT_STATUS                                                         :  8; // [ 7: 0]
      uint32_t __reserved__0                                                          : 24; // [31: 8]
    } b;
  } u;
};

struct apb_APB_INT_MASK
{
  union {
#define apb_APB_INT_MASK_address 0x00003A0C
    uint32_t value;
    struct {
      uint32_t APB_INT_MASK                                                           :  8; // [ 7: 0]
      uint32_t __reserved__0                                                          : 24; // [31: 8]
    } b;
  } u;
};

struct apb_APB_RST
{
  union {
#define apb_APB_RST_address 0x00003A10
    uint32_t value;
    struct {
      uint32_t APB_RST                                                                :  4; // [ 3: 0]
      uint32_t __reserved__0                                                          : 28; // [31: 4]
    } b;
  } u;
};

struct apb
{
  struct apb_APB_ACCESS                                     APB_ACCESS          ; //offset 0 (0)
         uint32_t                                                __reserved__1       ; //offset 4 (4)
         uint64_t                                                __reserved__2       ; //offset 8 (8)
         uint64_t                                                __reserved__3       ; //offset 10 (16)
         uint64_t                                                __reserved__4       ; //offset 18 (24)
         uint64_t                                                __reserved__5       ; //offset 20 (32)
         uint64_t                                                __reserved__6       ; //offset 28 (40)
         uint64_t                                                __reserved__7       ; //offset 30 (48)
         uint64_t                                                __reserved__8       ; //offset 38 (56)
         uint64_t                                                __reserved__9       ; //offset 40 (64)
         uint64_t                                                __reserved__10      ; //offset 48 (72)
         uint64_t                                                __reserved__11      ; //offset 50 (80)
         uint64_t                                                __reserved__12      ; //offset 58 (88)
         uint64_t                                                __reserved__13      ; //offset 60 (96)
         uint64_t                                                __reserved__14      ; //offset 68 (104)
         uint64_t                                                __reserved__15      ; //offset 70 (112)
         uint64_t                                                __reserved__16      ; //offset 78 (120)
         uint64_t                                                __reserved__17      ; //offset 80 (128)
         uint64_t                                                __reserved__18      ; //offset 88 (136)
         uint64_t                                                __reserved__19      ; //offset 90 (144)
         uint64_t                                                __reserved__20      ; //offset 98 (152)
         uint64_t                                                __reserved__21      ; //offset A0 (160)
         uint64_t                                                __reserved__22      ; //offset A8 (168)
         uint64_t                                                __reserved__23      ; //offset B0 (176)
         uint64_t                                                __reserved__24      ; //offset B8 (184)
         uint64_t                                                __reserved__25      ; //offset C0 (192)
         uint64_t                                                __reserved__26      ; //offset C8 (200)
         uint64_t                                                __reserved__27      ; //offset D0 (208)
         uint64_t                                                __reserved__28      ; //offset D8 (216)
         uint64_t                                                __reserved__29      ; //offset E0 (224)
         uint64_t                                                __reserved__30      ; //offset E8 (232)
         uint64_t                                                __reserved__31      ; //offset F0 (240)
         uint64_t                                                __reserved__32      ; //offset F8 (248)
         uint64_t                                                __reserved__33      ; //offset 100 (256)
         uint64_t                                                __reserved__34      ; //offset 108 (264)
         uint64_t                                                __reserved__35      ; //offset 110 (272)
         uint64_t                                                __reserved__36      ; //offset 118 (280)
         uint64_t                                                __reserved__37      ; //offset 120 (288)
         uint64_t                                                __reserved__38      ; //offset 128 (296)
         uint64_t                                                __reserved__39      ; //offset 130 (304)
         uint64_t                                                __reserved__40      ; //offset 138 (312)
         uint64_t                                                __reserved__41      ; //offset 140 (320)
         uint64_t                                                __reserved__42      ; //offset 148 (328)
         uint64_t                                                __reserved__43      ; //offset 150 (336)
         uint64_t                                                __reserved__44      ; //offset 158 (344)
         uint64_t                                                __reserved__45      ; //offset 160 (352)
         uint64_t                                                __reserved__46      ; //offset 168 (360)
         uint64_t                                                __reserved__47      ; //offset 170 (368)
         uint64_t                                                __reserved__48      ; //offset 178 (376)
         uint64_t                                                __reserved__49      ; //offset 180 (384)
         uint64_t                                                __reserved__50      ; //offset 188 (392)
         uint64_t                                                __reserved__51      ; //offset 190 (400)
         uint64_t                                                __reserved__52      ; //offset 198 (408)
         uint64_t                                                __reserved__53      ; //offset 1A0 (416)
         uint64_t                                                __reserved__54      ; //offset 1A8 (424)
         uint64_t                                                __reserved__55      ; //offset 1B0 (432)
         uint64_t                                                __reserved__56      ; //offset 1B8 (440)
         uint64_t                                                __reserved__57      ; //offset 1C0 (448)
         uint64_t                                                __reserved__58      ; //offset 1C8 (456)
         uint64_t                                                __reserved__59      ; //offset 1D0 (464)
         uint64_t                                                __reserved__60      ; //offset 1D8 (472)
         uint64_t                                                __reserved__61      ; //offset 1E0 (480)
         uint64_t                                                __reserved__62      ; //offset 1E8 (488)
         uint64_t                                                __reserved__63      ; //offset 1F0 (496)
         uint64_t                                                __reserved__64      ; //offset 1F8 (504)
  struct apb_APB_BASE                                       APB_BASE            ; //offset 200 (512)
  struct apb_APB_INTF                                       APB_INTF            ; //offset 204 (516)
  struct apb_APB_INT_STATUS                                 APB_INT_STATUS      ; //offset 208 (520)
  struct apb_APB_INT_MASK                                   APB_INT_MASK        ; //offset 20C (524)
  struct apb_APB_RST                                        APB_RST             ; //offset 210 (528)
}; //apb


struct ghash_GHASH_CFG
{
  union {
#define ghash_GHASH_CFG_address 0x00005000
    uint32_t value;
    struct {
      uint32_t DisByp                                                                 :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ghash_GHASH_CTRL
{
  union {
#define ghash_GHASH_CTRL_address 0x00005004
    uint32_t value;
    struct {
      uint32_t Reset                                                                  :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ghash_GHASH_CMD
{
  union {
#define ghash_GHASH_CMD_address 0x00005008
    uint32_t value;
    struct {
      uint32_t Start                                                                  :  1; // [ 0: 0]
      uint32_t Enable                                                                 :  1; // [ 1: 1]
      uint32_t __reserved__0                                                          : 30; // [31: 2]
    } b;
  } u;
};

struct ghash_GHASH_STATUS
{
  union {
#define ghash_GHASH_STATUS_address 0x0000500C
    uint32_t value;
    struct {
      uint32_t GHASH_Busy                                                             :  1; // [ 0: 0]
      uint32_t GHASH_State                                                            :  2; // [ 2: 1]
      uint32_t __reserved__0                                                          : 29; // [31: 3]
    } b;
  } u;
};

struct ghash_GHASH_INTRPT
{
  union {
#define ghash_GHASH_INTRPT_address 0x00005010
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ghash_GHASH_INTRPT_SRC_EN
{
  union {
#define ghash_GHASH_INTRPT_SRC_EN_address 0x00005014
    uint32_t value;
    struct {
      uint32_t Done                                                                   :  1; // [ 0: 0]
      uint32_t __reserved__0                                                          : 31; // [31: 1]
    } b;
  } u;
};

struct ghash_GHASH_STREAM_SIZE
{
  union {
#define ghash_GHASH_STREAM_SIZE_address 0x00005018
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_RSVD
{
  union {
#define ghash_RSVD_address 0x0000501C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_KEY_W0
{
  union {
#define ghash_KEY_W0_address 0x00005020
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_KEY_W1
{
  union {
#define ghash_KEY_W1_address 0x00005024
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_KEY_W2
{
  union {
#define ghash_KEY_W2_address 0x00005028
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_KEY_W3
{
  union {
#define ghash_KEY_W3_address 0x0000502C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_ANS_W0
{
  union {
#define ghash_ANS_W0_address 0x00005030
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_ANS_W1
{
  union {
#define ghash_ANS_W1_address 0x00005034
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_ANS_W2
{
  union {
#define ghash_ANS_W2_address 0x00005038
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash_ANS_W3
{
  union {
#define ghash_ANS_W3_address 0x0000503C
    uint32_t value;
    struct {
      uint32_t __nofield__;
    } b;
  } u;
};

struct ghash
{
  struct ghash_GHASH_CFG                                    GHASH_CFG           ; //offset 0 (0)
  struct ghash_GHASH_CTRL                                   GHASH_CTRL          ; //offset 4 (4)
  struct ghash_GHASH_CMD                                    GHASH_CMD           ; //offset 8 (8)
  struct ghash_GHASH_STATUS                                 GHASH_STATUS        ; //offset C (12)
  struct ghash_GHASH_INTRPT                                 GHASH_INTRPT        ; //offset 10 (16)
  struct ghash_GHASH_INTRPT_SRC_EN                          GHASH_INTRPT_SRC_EN ; //offset 14 (20)
  struct ghash_GHASH_STREAM_SIZE                            GHASH_STREAM_SIZE   ; //offset 18 (24)
         uint32_t                                                __reserved__7       ; //offset 1C (28)
  struct ghash_KEY_W0                                       KEY_W0              ; //offset 20 (32)
  struct ghash_KEY_W1                                       KEY_W1              ; //offset 24 (36)
  struct ghash_KEY_W2                                       KEY_W2              ; //offset 28 (40)
  struct ghash_KEY_W3                                       KEY_W3              ; //offset 2C (44)
  struct ghash_ANS_W0                                       ANS_W0              ; //offset 30 (48)
  struct ghash_ANS_W1                                       ANS_W1              ; //offset 34 (52)
  struct ghash_ANS_W2                                       ANS_W2              ; //offset 38 (56)
  struct ghash_ANS_W3                                       ANS_W3              ; //offset 3C (60)
}; //ghash


#endif //REG_STRUCT_H
