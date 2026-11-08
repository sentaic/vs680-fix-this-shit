/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef vipGbl_h
#define vipGbl_h (){}
#include "ctypes.h"
#pragma pack(1)
#ifdef __cplusplus
  extern "C" {
#endif
#ifndef _DOCC_H_BITOPS_
#define _DOCC_H_BITOPS_ (){}
    #define _bSETMASK_(b)                                      ((b)<32 ? (1<<((b)&31)) : 0)
    #define _NSETMASK_(msb,lsb)                                (_bSETMASK_((msb)+1)-_bSETMASK_(lsb))
    #define _bCLRMASK_(b)                                      (~_bSETMASK_(b))
    #define _NCLRMASK_(msb,lsb)                                (~_NSETMASK_(msb,lsb))
    #define _BFGET_(r,msb,lsb)                                 (_NSETMASK_((msb)-(lsb),0)&((r)>>(lsb)))
    #define _BFSET_(r,msb,lsb,v)                               do{ (r)&=_NCLRMASK_(msb,lsb); (r)|=_NSETMASK_(msb,lsb)&((v)<<(lsb)); }while(0)
#endif
#ifndef h_SRAMPWR
#define h_SRAMPWR (){}
    #define     RA_SRAMPWR_ctrl                                0x0000
    #define   LSb32SRAMPWR_ctrl_SD                                0
    #define       bSRAMPWR_ctrl_SD                             1
    #define        SRAMPWR_ctrl_SD_ON                                       0x0
    #define        SRAMPWR_ctrl_SD_SHUTDWN                                  0x1
    #define   LSb32SRAMPWR_ctrl_DSLP                              1
    #define       bSRAMPWR_ctrl_DSLP                           1
    #define        SRAMPWR_ctrl_DSLP_ON                                     0x0
    #define        SRAMPWR_ctrl_DSLP_DEEPSLP                                0x1
    #define   LSb32SRAMPWR_ctrl_SLP                               2
    #define       bSRAMPWR_ctrl_SLP                            1
    #define        SRAMPWR_ctrl_SLP_ON                                      0x0
    #define        SRAMPWR_ctrl_SLP_SLEEP                                   0x1
    typedef struct SIE_SRAMPWR {
    #define     w32SRAMPWR_ctrl                                {\
            UNSG32 uctrl_SD                                    :  1;\
            UNSG32 uctrl_DSLP                                  :  1;\
            UNSG32 uctrl_SLP                                   :  1;\
            UNSG32 RSVDx0_b3                                   : 29;\
          }
    union { UNSG32 u32SRAMPWR_ctrl;
            struct w32SRAMPWR_ctrl;
          };
    } SIE_SRAMPWR;
    typedef union  T32SRAMPWR_ctrl
          { UNSG32 u32;
            struct w32SRAMPWR_ctrl;
                 } T32SRAMPWR_ctrl;
    typedef union  TSRAMPWR_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32SRAMPWR_ctrl;
                   };
                 } TSRAMPWR_ctrl;
     SIGN32 SRAMPWR_drvrd(SIE_SRAMPWR *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 SRAMPWR_drvwr(SIE_SRAMPWR *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void SRAMPWR_reset(SIE_SRAMPWR *p);
     SIGN32 SRAMPWR_cmp  (SIE_SRAMPWR *p, SIE_SRAMPWR *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define SRAMPWR_check(p,pie,pfx,hLOG) SRAMPWR_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define SRAMPWR_print(p,    pfx,hLOG) SRAMPWR_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_SRAMRWTC
#define h_SRAMRWTC (){}
    #define     RA_SRAMRWTC_ctrl0                              0x0000
    #define   LSb32SRAMRWTC_ctrl0_RF1P                            0
    #define       bSRAMRWTC_ctrl0_RF1P                         4
    #define   LSb32SRAMRWTC_ctrl0_UHDRF1P                         4
    #define       bSRAMRWTC_ctrl0_UHDRF1P                      4
    #define   LSb32SRAMRWTC_ctrl0_RF2P                            8
    #define       bSRAMRWTC_ctrl0_RF2P                         8
    #define   LSb32SRAMRWTC_ctrl0_UHDRF2P                         16
    #define       bSRAMRWTC_ctrl0_UHDRF2P                      8
    #define   LSb32SRAMRWTC_ctrl0_UHDRF2P_ULVT                    24
    #define       bSRAMRWTC_ctrl0_UHDRF2P_ULVT                 8
    #define     RA_SRAMRWTC_ctrl1                              0x0004
    #define   LSb32SRAMRWTC_ctrl1_SHDMBSR1P                       0
    #define       bSRAMRWTC_ctrl1_SHDMBSR1P                    4
    #define   LSb32SRAMRWTC_ctrl1_SHDSBSR1P                       4
    #define       bSRAMRWTC_ctrl1_SHDSBSR1P                    4
    #define   LSb32SRAMRWTC_ctrl1_SHCMBSR1P_SSEG                  8
    #define       bSRAMRWTC_ctrl1_SHCMBSR1P_SSEG               4
    #define   LSb32SRAMRWTC_ctrl1_SHCMBSR1P_USEG                  12
    #define       bSRAMRWTC_ctrl1_SHCMBSR1P_USEG               4
    #define   LSb32SRAMRWTC_ctrl1_SHCSBSR1P                       16
    #define       bSRAMRWTC_ctrl1_SHCSBSR1P                    4
    #define   LSb32SRAMRWTC_ctrl1_SHCSBSR1P_CUSTM                 20
    #define       bSRAMRWTC_ctrl1_SHCSBSR1P_CUSTM              4
    #define   LSb32SRAMRWTC_ctrl1_SPSRAM_WT0                      24
    #define       bSRAMRWTC_ctrl1_SPSRAM_WT0                   4
    #define   LSb32SRAMRWTC_ctrl1_SPSRAM_WT1                      28
    #define       bSRAMRWTC_ctrl1_SPSRAM_WT1                   4
    #define     RA_SRAMRWTC_ctrl2                              0x0008
    #define   LSb32SRAMRWTC_ctrl2_L1CACHE                         0
    #define       bSRAMRWTC_ctrl2_L1CACHE                      4
    #define   LSb32SRAMRWTC_ctrl2_DPSR2P                          4
    #define       bSRAMRWTC_ctrl2_DPSR2P                       4
    #define   LSb32SRAMRWTC_ctrl2_ROM                             8
    #define       bSRAMRWTC_ctrl2_ROM                          8
    typedef struct SIE_SRAMRWTC {
    #define     w32SRAMRWTC_ctrl0                              {\
            UNSG32 uctrl0_RF1P                                 :  4;\
            UNSG32 uctrl0_UHDRF1P                              :  4;\
            UNSG32 uctrl0_RF2P                                 :  8;\
            UNSG32 uctrl0_UHDRF2P                              :  8;\
            UNSG32 uctrl0_UHDRF2P_ULVT                         :  8;\
          }
    union { UNSG32 u32SRAMRWTC_ctrl0;
            struct w32SRAMRWTC_ctrl0;
          };
    #define     w32SRAMRWTC_ctrl1                              {\
            UNSG32 uctrl1_SHDMBSR1P                            :  4;\
            UNSG32 uctrl1_SHDSBSR1P                            :  4;\
            UNSG32 uctrl1_SHCMBSR1P_SSEG                       :  4;\
            UNSG32 uctrl1_SHCMBSR1P_USEG                       :  4;\
            UNSG32 uctrl1_SHCSBSR1P                            :  4;\
            UNSG32 uctrl1_SHCSBSR1P_CUSTM                      :  4;\
            UNSG32 uctrl1_SPSRAM_WT0                           :  4;\
            UNSG32 uctrl1_SPSRAM_WT1                           :  4;\
          }
    union { UNSG32 u32SRAMRWTC_ctrl1;
            struct w32SRAMRWTC_ctrl1;
          };
    #define     w32SRAMRWTC_ctrl2                              {\
            UNSG32 uctrl2_L1CACHE                              :  4;\
            UNSG32 uctrl2_DPSR2P                               :  4;\
            UNSG32 uctrl2_ROM                                  :  8;\
            UNSG32 RSVDx8_b16                                  : 16;\
          }
    union { UNSG32 u32SRAMRWTC_ctrl2;
            struct w32SRAMRWTC_ctrl2;
          };
    } SIE_SRAMRWTC;
    typedef union  T32SRAMRWTC_ctrl0
          { UNSG32 u32;
            struct w32SRAMRWTC_ctrl0;
                 } T32SRAMRWTC_ctrl0;
    typedef union  T32SRAMRWTC_ctrl1
          { UNSG32 u32;
            struct w32SRAMRWTC_ctrl1;
                 } T32SRAMRWTC_ctrl1;
    typedef union  T32SRAMRWTC_ctrl2
          { UNSG32 u32;
            struct w32SRAMRWTC_ctrl2;
                 } T32SRAMRWTC_ctrl2;
    typedef union  TSRAMRWTC_ctrl0
          { UNSG32 u32[1];
            struct {
            struct w32SRAMRWTC_ctrl0;
                   };
                 } TSRAMRWTC_ctrl0;
    typedef union  TSRAMRWTC_ctrl1
          { UNSG32 u32[1];
            struct {
            struct w32SRAMRWTC_ctrl1;
                   };
                 } TSRAMRWTC_ctrl1;
    typedef union  TSRAMRWTC_ctrl2
          { UNSG32 u32[1];
            struct {
            struct w32SRAMRWTC_ctrl2;
                   };
                 } TSRAMRWTC_ctrl2;
     SIGN32 SRAMRWTC_drvrd(SIE_SRAMRWTC *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 SRAMRWTC_drvwr(SIE_SRAMRWTC *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void SRAMRWTC_reset(SIE_SRAMRWTC *p);
     SIGN32 SRAMRWTC_cmp  (SIE_SRAMRWTC *p, SIE_SRAMRWTC *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define SRAMRWTC_check(p,pie,pfx,hLOG) SRAMRWTC_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define SRAMRWTC_print(p,    pfx,hLOG) SRAMRWTC_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_vipGbl
#define h_vipGbl (){}
    #define     RA_vipGbl_CTRL                                 0x0000
    #define   LSb32vipGbl_CTRL_VIPDHUB_dyCG_en                    0
    #define       bvipGbl_CTRL_VIPDHUB_dyCG_en                 1
    #define   LSb32vipGbl_CTRL_VIPDHUB_CG_en                      1
    #define       bvipGbl_CTRL_VIPDHUB_CG_en                   1
    #define   LSb32vipGbl_CTRL_BCM_FIFO_FLUSH                     2
    #define       bvipGbl_CTRL_BCM_FIFO_FLUSH                  1
    #define   LSb32vipGbl_CTRL_BCMQ_FIFO_FLUSH                    3
    #define       bvipGbl_CTRL_BCMQ_FIFO_FLUSH                 1
    #define     RA_vipGbl_VIP128bDHUB_SRAMPWR                  0x0004
    #define     RA_vipGbl_MIPICSIRX_SRAMPWR                    0x0008
    #define     RA_vipGbl_CSIRXPIPE_SRAMPWR                    0x000C
    #define     RA_vipGbl_INTR_CTRL                            0x0010
    #define   LSb32vipGbl_INTR_CTRL_csi_hc_int_en                 0
    #define       bvipGbl_INTR_CTRL_csi_hc_int_en              1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr0_int_en           1
    #define       bvipGbl_INTR_CTRL_csiPipeIntr0_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr1_int_en           2
    #define       bvipGbl_INTR_CTRL_csiPipeIntr1_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr2_int_en           3
    #define       bvipGbl_INTR_CTRL_csiPipeIntr2_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr3_int_en           4
    #define       bvipGbl_INTR_CTRL_csiPipeIntr3_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr4_int_en           5
    #define       bvipGbl_INTR_CTRL_csiPipeIntr4_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr5_int_en           6
    #define       bvipGbl_INTR_CTRL_csiPipeIntr5_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr6_int_en           7
    #define       bvipGbl_INTR_CTRL_csiPipeIntr6_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr7_int_en           8
    #define       bvipGbl_INTR_CTRL_csiPipeIntr7_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr8_int_en           9
    #define       bvipGbl_INTR_CTRL_csiPipeIntr8_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr9_int_en           10
    #define       bvipGbl_INTR_CTRL_csiPipeIntr9_int_en        1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr10_int_en          11
    #define       bvipGbl_INTR_CTRL_csiPipeIntr10_int_en       1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr11_int_en          12
    #define       bvipGbl_INTR_CTRL_csiPipeIntr11_int_en       1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr12_int_en          13
    #define       bvipGbl_INTR_CTRL_csiPipeIntr12_int_en       1
    #define   LSb32vipGbl_INTR_CTRL_bcmInvalidReq_int_en          14
    #define       bvipGbl_INTR_CTRL_bcmInvalidReq_int_en       1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr13_int_en          15
    #define       bvipGbl_INTR_CTRL_csiPipeIntr13_int_en       1
    #define   LSb32vipGbl_INTR_CTRL_csiPipeIntr14_int_en          16
    #define       bvipGbl_INTR_CTRL_csiPipeIntr14_int_en       1
    #define   LSb32vipGbl_INTR_CTRL_buf1_0Overflow_intr_mux_sel_host2dhub1    17
    #define       bvipGbl_INTR_CTRL_buf1_0Overflow_intr_mux_sel_host2dhub1 1
    #define   LSb32vipGbl_INTR_CTRL_buf1_0Overflow_intr_mux_sel_host2dhub2    18
    #define       bvipGbl_INTR_CTRL_buf1_0Overflow_intr_mux_sel_host2dhub2 1
    #define     RA_vipGbl_SWRST_CTRL                           0x0014
    #define   LSb32vipGbl_SWRST_CTRL_apbCSIHostSyncRstn           0
    #define       bvipGbl_SWRST_CTRL_apbCSIHostSyncRstn        1
    #define   LSb32vipGbl_SWRST_CTRL_ipiSyncRstn                  1
    #define       bvipGbl_SWRST_CTRL_ipiSyncRstn               1
    #define   LSb32vipGbl_SWRST_CTRL_imgDigIfSyncRstn             2
    #define       bvipGbl_SWRST_CTRL_imgDigIfSyncRstn          1
    #define   LSb32vipGbl_SWRST_CTRL_biu_ipiSyncRstn              3
    #define       bvipGbl_SWRST_CTRL_biu_ipiSyncRstn           1
    #define   LSb32vipGbl_SWRST_CTRL_iif_ipiSyncRstn              4
    #define       bvipGbl_SWRST_CTRL_iif_ipiSyncRstn           1
    #define   LSb32vipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn            5
    #define       bvipGbl_SWRST_CTRL_h2dh1_ipiSyncRstn         1
    #define   LSb32vipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn            6
    #define       bvipGbl_SWRST_CTRL_h2dh2_ipiSyncRstn         1
    #define   LSb32vipGbl_SWRST_CTRL_iif_ipi_framefilt_SyncRstn    7
    #define       bvipGbl_SWRST_CTRL_iif_ipi_framefilt_SyncRstn 1
    #define     RA_vipGbl_CG_CTRL                              0x0018
    #define   LSb32vipGbl_CG_CTRL_csi_rx_cfgClk_cg_en             0
    #define       bvipGbl_CG_CTRL_csi_rx_cfgClk_cg_en          1
    #define   LSb32vipGbl_CG_CTRL_csi_rx_apbClk_cg_en             1
    #define       bvipGbl_CG_CTRL_csi_rx_apbClk_cg_en          1
    #define   LSb32vipGbl_CG_CTRL_csi_rx_dphy_cfg_clk_cg_en       2
    #define       bvipGbl_CG_CTRL_csi_rx_dphy_cfg_clk_cg_en    1
    #define   LSb32vipGbl_CG_CTRL_csi_rx_ipi_clk_cg_en            3
    #define       bvipGbl_CG_CTRL_csi_rx_ipi_clk_cg_en         1
    typedef struct SIE_vipGbl {
    #define     w32vipGbl_CTRL                                 {\
            UNSG32 uCTRL_VIPDHUB_dyCG_en                       :  1;\
            UNSG32 uCTRL_VIPDHUB_CG_en                         :  1;\
            UNSG32 uCTRL_BCM_FIFO_FLUSH                        :  1;\
            UNSG32 uCTRL_BCMQ_FIFO_FLUSH                       :  1;\
            UNSG32 RSVDx0_b4                                   : 28;\
          }
    union { UNSG32 u32vipGbl_CTRL;
            struct w32vipGbl_CTRL;
          };
              SIE_SRAMPWR                                      ie_VIP128bDHUB_SRAMPWR;
              SIE_SRAMPWR                                      ie_MIPICSIRX_SRAMPWR;
              SIE_SRAMPWR                                      ie_CSIRXPIPE_SRAMPWR;
    #define     w32vipGbl_INTR_CTRL                            {\
            UNSG32 uINTR_CTRL_csi_hc_int_en                    :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr0_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr1_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr2_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr3_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr4_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr5_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr6_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr7_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr8_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr9_int_en              :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr10_int_en             :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr11_int_en             :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr12_int_en             :  1;\
            UNSG32 uINTR_CTRL_bcmInvalidReq_int_en             :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr13_int_en             :  1;\
            UNSG32 uINTR_CTRL_csiPipeIntr14_int_en             :  1;\
            UNSG32 uINTR_CTRL_buf1_0Overflow_intr_mux_sel_host2dhub1 :  1;\
            UNSG32 uINTR_CTRL_buf1_0Overflow_intr_mux_sel_host2dhub2 :  1;\
            UNSG32 RSVDx10_b19                                 : 13;\
          }
    union { UNSG32 u32vipGbl_INTR_CTRL;
            struct w32vipGbl_INTR_CTRL;
          };
    #define     w32vipGbl_SWRST_CTRL                           {\
            UNSG32 uSWRST_CTRL_apbCSIHostSyncRstn              :  1;\
            UNSG32 uSWRST_CTRL_ipiSyncRstn                     :  1;\
            UNSG32 uSWRST_CTRL_imgDigIfSyncRstn                :  1;\
            UNSG32 uSWRST_CTRL_biu_ipiSyncRstn                 :  1;\
            UNSG32 uSWRST_CTRL_iif_ipiSyncRstn                 :  1;\
            UNSG32 uSWRST_CTRL_h2dh1_ipiSyncRstn               :  1;\
            UNSG32 uSWRST_CTRL_h2dh2_ipiSyncRstn               :  1;\
            UNSG32 uSWRST_CTRL_iif_ipi_framefilt_SyncRstn      :  1;\
            UNSG32 RSVDx14_b8                                  : 24;\
          }
    union { UNSG32 u32vipGbl_SWRST_CTRL;
            struct w32vipGbl_SWRST_CTRL;
          };
    #define     w32vipGbl_CG_CTRL                              {\
            UNSG32 uCG_CTRL_csi_rx_cfgClk_cg_en                :  1;\
            UNSG32 uCG_CTRL_csi_rx_apbClk_cg_en                :  1;\
            UNSG32 uCG_CTRL_csi_rx_dphy_cfg_clk_cg_en          :  1;\
            UNSG32 uCG_CTRL_csi_rx_ipi_clk_cg_en               :  1;\
            UNSG32 RSVDx18_b4                                  : 28;\
          }
    union { UNSG32 u32vipGbl_CG_CTRL;
            struct w32vipGbl_CG_CTRL;
          };
    } SIE_vipGbl;
    typedef union  T32vipGbl_CTRL
          { UNSG32 u32;
            struct w32vipGbl_CTRL;
                 } T32vipGbl_CTRL;
    typedef union  T32vipGbl_INTR_CTRL
          { UNSG32 u32;
            struct w32vipGbl_INTR_CTRL;
                 } T32vipGbl_INTR_CTRL;
    typedef union  T32vipGbl_SWRST_CTRL
          { UNSG32 u32;
            struct w32vipGbl_SWRST_CTRL;
                 } T32vipGbl_SWRST_CTRL;
    typedef union  T32vipGbl_CG_CTRL
          { UNSG32 u32;
            struct w32vipGbl_CG_CTRL;
                 } T32vipGbl_CG_CTRL;
    typedef union  TvipGbl_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vipGbl_CTRL;
                   };
                 } TvipGbl_CTRL;
    typedef union  TvipGbl_INTR_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vipGbl_INTR_CTRL;
                   };
                 } TvipGbl_INTR_CTRL;
    typedef union  TvipGbl_SWRST_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vipGbl_SWRST_CTRL;
                   };
                 } TvipGbl_SWRST_CTRL;
    typedef union  TvipGbl_CG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vipGbl_CG_CTRL;
                   };
                 } TvipGbl_CG_CTRL;
     SIGN32 vipGbl_drvrd(SIE_vipGbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 vipGbl_drvwr(SIE_vipGbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void vipGbl_reset(SIE_vipGbl *p);
     SIGN32 vipGbl_cmp  (SIE_vipGbl *p, SIE_vipGbl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define vipGbl_check(p,pie,pfx,hLOG) vipGbl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define vipGbl_print(p,    pfx,hLOG) vipGbl_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifdef __cplusplus
  }
#endif
#pragma  pack()
#endif
