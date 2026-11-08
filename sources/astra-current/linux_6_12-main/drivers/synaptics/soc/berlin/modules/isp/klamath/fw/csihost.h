/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef csihost_h
#define csihost_h (){}
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
#ifndef h_DummyRegH
#define h_DummyRegH (){}
    typedef struct SIE_DummyRegH {
            UNSG32 u_word                                      : 32;
    } SIE_DummyRegH;
     SIGN32 DummyRegH_drvrd(SIE_DummyRegH *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DummyRegH_drvwr(SIE_DummyRegH *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DummyRegH_reset(SIE_DummyRegH *p);
     SIGN32 DummyRegH_cmp  (SIE_DummyRegH *p, SIE_DummyRegH *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DummyRegH_check(p,pie,pfx,hLOG) DummyRegH_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DummyRegH_print(p,    pfx,hLOG) DummyRegH_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_REGH
#define h_REGH (){}
    #define     RA_REGH_DummyH                                 0x0000
    typedef struct SIE_REGH {
              SIE_DummyRegH                                    ie_DummyH[4096];
    } SIE_REGH;
     SIGN32 REGH_drvrd(SIE_REGH *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 REGH_drvwr(SIE_REGH *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void REGH_reset(SIE_REGH *p);
     SIGN32 REGH_cmp  (SIE_REGH *p, SIE_REGH *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define REGH_check(p,pie,pfx,hLOG) REGH_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define REGH_print(p,    pfx,hLOG) REGH_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_GENH
#define h_GENH (){}
    #define     RA_GENH_PHYCTRL                                0x0000
    #define     RA_GENH_PHYCTRL1                               0x0004
    #define     RA_GENH_PHYTESTDIN                             0x0008
    #define     RA_GENH_PHYTESTEN                              0x000C
    #define     RA_GENH_PHYTESTCLK                             0x0010
    #define     RA_GENH_PHYTESTCLR                             0x0014
    #define     RA_GENH_CTRL                                   0x0018
    #define     RA_GENH_PHYSTS                                 0x001C
    #define     RA_GENH_PHYTESTDOUT                            0x0020
    typedef struct SIE_GENH {
    #define   SET32GENH_PHYCTRL_cfg_clk_off(r32,v)             _BFSET_(r32,17,17,v)
    #define   SET32GENH_PHYCTRL_cfgclkfreqrange(r32,v)         _BFSET_(r32,14, 7,v)
    #define   SET32GENH_PHYCTRL_hsfreqrange(r32,v)             _BFSET_(r32, 6, 0,v)
    #define     w32GENH_PHYCTRL                                {\
            UNSG32 uPHYCTRL_hsfreqrange                        :  7;\
            UNSG32 uPHYCTRL_cfgclkfreqrange                    :  8;\
            UNSG32 uPHYCTRL_cont_en                            :  1;\
            UNSG32 uPHYCTRL_biston                             :  1;\
            UNSG32 uPHYCTRL_cfg_clk_off                        :  1;\
            UNSG32 uPHYCTRL_txclkescdiv                        :  3;\
            UNSG32 uPHYCTRL_txclkescen                         :  1;\
            UNSG32 uPHYCTRL_txclkesc_off                       :  1;\
            UNSG32 RSVDx0_b23                                  :  9;\
          }
    union { UNSG32 u32GENH_PHYCTRL;
            struct w32GENH_PHYCTRL;
          };
    #define     w32GENH_PHYCTRL1                               {\
            UNSG32 uPHYCTRL1_shutdownz                         :  1;\
            UNSG32 uPHYCTRL1_rstz                              :  1;\
            UNSG32 uPHYCTRL1_ateen                             :  1;\
            UNSG32 uPHYCTRL1_tpsel                             :  1;\
            UNSG32 RSVDx4_b4                                   : 28;\
          }
    union { UNSG32 u32GENH_PHYCTRL1;
            struct w32GENH_PHYCTRL1;
          };
    #define     w32GENH_PHYTESTDIN                             {\
            UNSG32 uPHYTESTDIN_value                           :  8;\
            UNSG32 RSVDx8_b8                                   : 24;\
          }
    union { UNSG32 u32GENH_PHYTESTDIN;
            struct w32GENH_PHYTESTDIN;
          };
    #define     w32GENH_PHYTESTEN                              {\
            UNSG32 uPHYTESTEN_value                            :  1;\
            UNSG32 RSVDxC_b1                                   : 31;\
          }
    union { UNSG32 u32GENH_PHYTESTEN;
            struct w32GENH_PHYTESTEN;
          };
    #define     w32GENH_PHYTESTCLK                             {\
            UNSG32 uPHYTESTCLK_value                           :  1;\
            UNSG32 RSVDx10_b1                                  : 31;\
          }
    union { UNSG32 u32GENH_PHYTESTCLK;
            struct w32GENH_PHYTESTCLK;
          };
    #define     w32GENH_PHYTESTCLR                             {\
            UNSG32 uPHYTESTCLR_value                           :  1;\
            UNSG32 RSVDx14_b1                                  : 31;\
          }
    union { UNSG32 u32GENH_PHYTESTCLR;
            struct w32GENH_PHYTESTCLR;
          };
    #define     w32GENH_CTRL                                   {\
            UNSG32 uCTRL_pclk_off                              :  1;\
            UNSG32 uCTRL_ipi_clk_off                           :  1;\
            UNSG32 RSVDx18_b2                                  : 30;\
          }
    union { UNSG32 u32GENH_CTRL;
            struct w32GENH_CTRL;
          };
    #define     w32GENH_PHYSTS                                 {\
            UNSG32 uPHYSTS_bistdone                            :  1;\
            UNSG32 uPHYSTS_bistok                              :  1;\
            UNSG32 uPHYSTS_cont_data                           :  7;\
            UNSG32 RSVDx1C_b9                                  : 23;\
          }
    union { UNSG32 u32GENH_PHYSTS;
            struct w32GENH_PHYSTS;
          };
    #define     w32GENH_PHYTESTDOUT                            {\
            UNSG32 uPHYTESTDOUT_value                          :  8;\
            UNSG32 RSVDx20_b8                                  : 24;\
          }
    union { UNSG32 u32GENH_PHYTESTDOUT;
            struct w32GENH_PHYTESTDOUT;
          };
    } SIE_GENH;
    typedef union  T32GENH_PHYCTRL
          { UNSG32 u32;
            struct w32GENH_PHYCTRL;
                 } T32GENH_PHYCTRL;
    typedef union  T32GENH_PHYCTRL1
          { UNSG32 u32;
            struct w32GENH_PHYCTRL1;
                 } T32GENH_PHYCTRL1;
    typedef union  T32GENH_PHYTESTDIN
          { UNSG32 u32;
            struct w32GENH_PHYTESTDIN;
                 } T32GENH_PHYTESTDIN;
    typedef union  T32GENH_PHYTESTEN
          { UNSG32 u32;
            struct w32GENH_PHYTESTEN;
                 } T32GENH_PHYTESTEN;
    typedef union  T32GENH_PHYTESTCLK
          { UNSG32 u32;
            struct w32GENH_PHYTESTCLK;
                 } T32GENH_PHYTESTCLK;
    typedef union  T32GENH_PHYTESTCLR
          { UNSG32 u32;
            struct w32GENH_PHYTESTCLR;
                 } T32GENH_PHYTESTCLR;
    typedef union  T32GENH_CTRL
          { UNSG32 u32;
            struct w32GENH_CTRL;
                 } T32GENH_CTRL;
    typedef union  T32GENH_PHYSTS
          { UNSG32 u32;
            struct w32GENH_PHYSTS;
                 } T32GENH_PHYSTS;
    typedef union  T32GENH_PHYTESTDOUT
          { UNSG32 u32;
            struct w32GENH_PHYTESTDOUT;
                 } T32GENH_PHYTESTDOUT;
    typedef union  TGENH_PHYCTRL
          { UNSG32 u32[1];
            struct {
            struct w32GENH_PHYCTRL;
                   };
                 } TGENH_PHYCTRL;
    typedef union  TGENH_PHYCTRL1
          { UNSG32 u32[1];
            struct {
            struct w32GENH_PHYCTRL1;
                   };
                 } TGENH_PHYCTRL1;
    typedef union  TGENH_PHYTESTDIN
          { UNSG32 u32[1];
            struct {
            struct w32GENH_PHYTESTDIN;
                   };
                 } TGENH_PHYTESTDIN;
    typedef union  TGENH_PHYTESTEN
          { UNSG32 u32[1];
            struct {
            struct w32GENH_PHYTESTEN;
                   };
                 } TGENH_PHYTESTEN;
    typedef union  TGENH_PHYTESTCLK
          { UNSG32 u32[1];
            struct {
            struct w32GENH_PHYTESTCLK;
                   };
                 } TGENH_PHYTESTCLK;
    typedef union  TGENH_PHYTESTCLR
          { UNSG32 u32[1];
            struct {
            struct w32GENH_PHYTESTCLR;
                   };
                 } TGENH_PHYTESTCLR;
    typedef union  TGENH_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32GENH_CTRL;
                   };
                 } TGENH_CTRL;
    typedef union  TGENH_PHYSTS
          { UNSG32 u32[1];
            struct {
            struct w32GENH_PHYSTS;
                   };
                 } TGENH_PHYSTS;
    typedef union  TGENH_PHYTESTDOUT
          { UNSG32 u32[1];
            struct {
            struct w32GENH_PHYTESTDOUT;
                   };
                 } TGENH_PHYTESTDOUT;
     SIGN32 GENH_drvrd(SIE_GENH *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 GENH_drvwr(SIE_GENH *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void GENH_reset(SIE_GENH *p);
     SIGN32 GENH_cmp  (SIE_GENH *p, SIE_GENH *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define GENH_check(p,pie,pfx,hLOG) GENH_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define GENH_print(p,    pfx,hLOG) GENH_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_DPHYRX_PPI
#define h_DPHYRX_PPI (){}
    #define     RA_DPHYRX_PPI_CTRL                             0x0000
    #define     RA_DPHYRX_PPI_STATUS0                          0x0004
    #define     RA_DPHYRX_PPI_STATUS1                          0x0008
    #define     RA_DPHYRX_PPI_STATUS2                          0x000C
    typedef struct SIE_DPHYRX_PPI {
    #define   SET32DPHYRX_PPI_CTRL_forcerxmode_N(r32,v)        _BFSET_(r32,24,23,v)
    #define   SET32DPHYRX_PPI_CTRL_basedir_0(r32,v)            _BFSET_(r32, 0, 0,v)
    #define     w32DPHYRX_PPI_CTRL                             {\
            UNSG32 uCTRL_basedir_0                             :  1;\
            UNSG32 uCTRL_txrequestesc_0                        :  1;\
            UNSG32 uCTRL_txlpdtesc_0                           :  1;\
            UNSG32 uCTRL_txulpsesc_0                           :  1;\
            UNSG32 uCTRL_txulpsexit_0                          :  1;\
            UNSG32 uCTRL_txtriggeresc_0                        :  4;\
            UNSG32 uCTRL_txdataesc_0                           :  8;\
            UNSG32 uCTRL_txvalidesc_0                          :  1;\
            UNSG32 uCTRL_enableclk                             :  1;\
            UNSG32 uCTRL_forcetxstopmode_N                     :  2;\
            UNSG32 uCTRL_turnrequest_0                         :  1;\
            UNSG32 uCTRL_turndisable_0                         :  1;\
            UNSG32 uCTRL_forcerxmode_N                         :  2;\
            UNSG32 uCTRL_enable_N                              :  2;\
            UNSG32 RSVDx0_b27                                  :  5;\
          }
    union { UNSG32 u32DPHYRX_PPI_CTRL;
            struct w32DPHYRX_PPI_CTRL;
          };
    #define     w32DPHYRX_PPI_STATUS0                          {\
            UNSG32 uSTATUS0_ulpsactivenotclk                   :  1;\
            UNSG32 uSTATUS0_rxactivehs_N                       :  2;\
            UNSG32 uSTATUS0_rxsynchs_N                         :  2;\
            UNSG32 uSTATUS0_txreadyesc_0                       :  1;\
            UNSG32 uSTATUS0_rxclkesc_N                         :  2;\
            UNSG32 uSTATUS0_rxlpdtesc_N                        :  2;\
            UNSG32 uSTATUS0_rxtriggeresc_0                     :  4;\
            UNSG32 uSTATUS0_rxtriggeresc_1                     :  4;\
            UNSG32 uSTATUS0_errsoths_N                         :  2;\
            UNSG32 uSTATUS0_errsotsynchs_N                     :  2;\
            UNSG32 uSTATUS0_rxulpsesc_N                        :  2;\
            UNSG32 RSVDx4_b24                                  :  8;\
          }
    union { UNSG32 u32DPHYRX_PPI_STATUS0;
            struct w32DPHYRX_PPI_STATUS0;
          };
    #define     w32DPHYRX_PPI_STATUS1                          {\
            UNSG32 uSTATUS1_rxdataesc_0                        :  8;\
            UNSG32 uSTATUS1_rxdataesc_1                        :  8;\
            UNSG32 uSTATUS1_rxvalidesc_N                       :  2;\
            UNSG32 uSTATUS1_rxvalidhs_N                        :  2;\
            UNSG32 uSTATUS1_erresc_N                           :  2;\
            UNSG32 uSTATUS1_errsyncesc_N                       :  2;\
            UNSG32 uSTATUS1_errcontrol_N                       :  2;\
            UNSG32 uSTATUS1_errcontentionlp0_0                 :  1;\
            UNSG32 uSTATUS1_errcontentionlp1_0                 :  1;\
            UNSG32 uSTATUS1_ulpsactivenot_N                    :  2;\
            UNSG32 uSTATUS1_direction_0                        :  1;\
            UNSG32 RSVDx8_b31                                  :  1;\
          }
    union { UNSG32 u32DPHYRX_PPI_STATUS1;
            struct w32DPHYRX_PPI_STATUS1;
          };
    #define     w32DPHYRX_PPI_STATUS2                          {\
            UNSG32 uSTATUS2_rxdatahs_0                         :  8;\
            UNSG32 uSTATUS2_rxdatahs_1                         :  8;\
            UNSG32 uSTATUS2_rxskewcalhs                        :  1;\
            UNSG32 uSTATUS2_stopstateclk                       :  1;\
            UNSG32 uSTATUS2_rxulpsclknot                       :  1;\
            UNSG32 uSTATUS2_rxclkactivehs                      :  1;\
            UNSG32 uSTATUS2_stopstatedata_N                    :  2;\
            UNSG32 RSVDxC_b22                                  : 10;\
          }
    union { UNSG32 u32DPHYRX_PPI_STATUS2;
            struct w32DPHYRX_PPI_STATUS2;
          };
    } SIE_DPHYRX_PPI;
    typedef union  T32DPHYRX_PPI_CTRL
          { UNSG32 u32;
            struct w32DPHYRX_PPI_CTRL;
                 } T32DPHYRX_PPI_CTRL;
    typedef union  T32DPHYRX_PPI_STATUS0
          { UNSG32 u32;
            struct w32DPHYRX_PPI_STATUS0;
                 } T32DPHYRX_PPI_STATUS0;
    typedef union  T32DPHYRX_PPI_STATUS1
          { UNSG32 u32;
            struct w32DPHYRX_PPI_STATUS1;
                 } T32DPHYRX_PPI_STATUS1;
    typedef union  T32DPHYRX_PPI_STATUS2
          { UNSG32 u32;
            struct w32DPHYRX_PPI_STATUS2;
                 } T32DPHYRX_PPI_STATUS2;
    typedef union  TDPHYRX_PPI_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32DPHYRX_PPI_CTRL;
                   };
                 } TDPHYRX_PPI_CTRL;
    typedef union  TDPHYRX_PPI_STATUS0
          { UNSG32 u32[1];
            struct {
            struct w32DPHYRX_PPI_STATUS0;
                   };
                 } TDPHYRX_PPI_STATUS0;
    typedef union  TDPHYRX_PPI_STATUS1
          { UNSG32 u32[1];
            struct {
            struct w32DPHYRX_PPI_STATUS1;
                   };
                 } TDPHYRX_PPI_STATUS1;
    typedef union  TDPHYRX_PPI_STATUS2
          { UNSG32 u32[1];
            struct {
            struct w32DPHYRX_PPI_STATUS2;
                   };
                 } TDPHYRX_PPI_STATUS2;
     SIGN32 DPHYRX_PPI_drvrd(SIE_DPHYRX_PPI *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DPHYRX_PPI_drvwr(SIE_DPHYRX_PPI *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DPHYRX_PPI_reset(SIE_DPHYRX_PPI *p);
     SIGN32 DPHYRX_PPI_cmp  (SIE_DPHYRX_PPI *p, SIE_DPHYRX_PPI *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DPHYRX_PPI_check(p,pie,pfx,hLOG) DPHYRX_PPI_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DPHYRX_PPI_print(p,    pfx,hLOG) DPHYRX_PPI_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_CSIHOST
#define h_CSIHOST (){}
    #define     RA_CSIHOST_DW                                  0x0000
    #define     RA_CSIHOST_GENH                                0x4000
    #define     RA_CSIHOST_DPHYRX_PPI                          0x4024
    #define     RA_CSIHOST_IRQEN                               0x4034
    #define     RA_CSIHOST_IRQSTS                              0x4038
    #define     RA_CSIHOST_DEBUG_CTRL                          0x403C
    typedef struct SIE_CSIHOST {
              SIE_REGH                                         ie_DW;
              SIE_GENH                                         ie_GENH;
              SIE_DPHYRX_PPI                                   ie_DPHYRX_PPI;
    #define     w32CSIHOST_IRQEN                               {\
            UNSG32 uIRQEN_HCIRQ                                :  1;\
            UNSG32 RSVDx4034_b1                                : 31;\
          }
    union { UNSG32 u32CSIHOST_IRQEN;
            struct w32CSIHOST_IRQEN;
          };
    #define     w32CSIHOST_IRQSTS                              {\
            UNSG32 uIRQSTS_HCIRQ                               :  1;\
            UNSG32 RSVDx4038_b1                                : 31;\
          }
    union { UNSG32 u32CSIHOST_IRQSTS;
            struct w32CSIHOST_IRQSTS;
          };
    #define     w32CSIHOST_DEBUG_CTRL                          {\
            UNSG32 uDEBUG_CTRL_SEL                             :  4;\
            UNSG32 RSVDx403C_b4                                : 28;\
          }
    union { UNSG32 u32CSIHOST_DEBUG_CTRL;
            struct w32CSIHOST_DEBUG_CTRL;
          };
             UNSG8 RSVDx4040                                   [16320];
    } SIE_CSIHOST;
    typedef union  T32CSIHOST_IRQEN
          { UNSG32 u32;
            struct w32CSIHOST_IRQEN;
                 } T32CSIHOST_IRQEN;
    typedef union  T32CSIHOST_IRQSTS
          { UNSG32 u32;
            struct w32CSIHOST_IRQSTS;
                 } T32CSIHOST_IRQSTS;
    typedef union  T32CSIHOST_DEBUG_CTRL
          { UNSG32 u32;
            struct w32CSIHOST_DEBUG_CTRL;
                 } T32CSIHOST_DEBUG_CTRL;
    typedef union  TCSIHOST_IRQEN
          { UNSG32 u32[1];
            struct {
            struct w32CSIHOST_IRQEN;
                   };
                 } TCSIHOST_IRQEN;
    typedef union  TCSIHOST_IRQSTS
          { UNSG32 u32[1];
            struct {
            struct w32CSIHOST_IRQSTS;
                   };
                 } TCSIHOST_IRQSTS;
    typedef union  TCSIHOST_DEBUG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32CSIHOST_DEBUG_CTRL;
                   };
                 } TCSIHOST_DEBUG_CTRL;
     SIGN32 CSIHOST_drvrd(SIE_CSIHOST *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 CSIHOST_drvwr(SIE_CSIHOST *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void CSIHOST_reset(SIE_CSIHOST *p);
     SIGN32 CSIHOST_cmp  (SIE_CSIHOST *p, SIE_CSIHOST *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define CSIHOST_check(p,pie,pfx,hLOG) CSIHOST_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define CSIHOST_print(p,    pfx,hLOG) CSIHOST_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifdef __cplusplus
  }
#endif
#pragma  pack()
#endif
