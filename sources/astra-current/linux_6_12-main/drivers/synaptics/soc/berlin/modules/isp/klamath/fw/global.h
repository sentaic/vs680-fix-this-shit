/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2025 Synaptics Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef global_h
#define global_h (){}
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
#ifndef h_abipll
#define h_abipll (){}
    #define     RA_abipll_ctrlA                                0x0000
    #define     RA_abipll_ctrlB                                0x0004
    #define     RA_abipll_ctrlC                                0x0008
    #define     RA_abipll_ctrlD                                0x000C
    #define     RA_abipll_ctrlE                                0x0010
    #define     RA_abipll_ctrlF                                0x0014
    #define     RA_abipll_ctrlG                                0x0018
    #define     RA_abipll_status                               0x001C
    typedef struct SIE_abipll {
    #define   SET32abipll_ctrlA_RESET(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16abipll_ctrlA_RESET(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32abipll_ctrlA_BYPASS(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   SET16abipll_ctrlA_BYPASS(r16,v)                  _BFSET_(r16, 1, 1,v)
    #define   SET32abipll_ctrlA_NEWDIV(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   SET16abipll_ctrlA_NEWDIV(r16,v)                  _BFSET_(r16, 2, 2,v)
    #define   SET32abipll_ctrlA_RANGE(r32,v)                   _BFSET_(r32, 5, 3,v)
    #define   SET16abipll_ctrlA_RANGE(r16,v)                   _BFSET_(r16, 5, 3,v)
    #define     w32abipll_ctrlA                                {\
            UNSG32 uctrlA_RESET                                :  1;\
            UNSG32 uctrlA_BYPASS                               :  1;\
            UNSG32 uctrlA_NEWDIV                               :  1;\
            UNSG32 uctrlA_RANGE                                :  3;\
            UNSG32 RSVDx0_b6                                   : 26;\
          }
    union { UNSG32 u32abipll_ctrlA;
            struct w32abipll_ctrlA;
          };
    #define   SET32abipll_ctrlB_SSMF(r32,v)                    _BFSET_(r32, 3, 0,v)
    #define   SET16abipll_ctrlB_SSMF(r16,v)                    _BFSET_(r16, 3, 0,v)
    #define   SET32abipll_ctrlB_SSMD(r32,v)                    _BFSET_(r32, 6, 4,v)
    #define   SET16abipll_ctrlB_SSMD(r16,v)                    _BFSET_(r16, 6, 4,v)
    #define   SET32abipll_ctrlB_SSE_RSVD(r32,v)                _BFSET_(r32, 7, 7,v)
    #define   SET16abipll_ctrlB_SSE_RSVD(r16,v)                _BFSET_(r16, 7, 7,v)
    #define   SET32abipll_ctrlB_SSE(r32,v)                     _BFSET_(r32, 8, 8,v)
    #define   SET16abipll_ctrlB_SSE(r16,v)                     _BFSET_(r16, 8, 8,v)
    #define   SET32abipll_ctrlB_SSDS(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16abipll_ctrlB_SSDS(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32abipll_ctrlB                                {\
            UNSG32 uctrlB_SSMF                                 :  4;\
            UNSG32 uctrlB_SSMD                                 :  3;\
            UNSG32 uctrlB_SSE_RSVD                             :  1;\
            UNSG32 uctrlB_SSE                                  :  1;\
            UNSG32 uctrlB_SSDS                                 :  1;\
            UNSG32 RSVDx4_b10                                  : 22;\
          }
    union { UNSG32 u32abipll_ctrlB;
            struct w32abipll_ctrlB;
          };
    #define   SET32abipll_ctrlC_DIVR(r32,v)                    _BFSET_(r32, 5, 0,v)
    #define   SET16abipll_ctrlC_DIVR(r16,v)                    _BFSET_(r16, 5, 0,v)
    #define     w32abipll_ctrlC                                {\
            UNSG32 uctrlC_DIVR                                 :  6;\
            UNSG32 RSVDx8_b6                                   : 26;\
          }
    union { UNSG32 u32abipll_ctrlC;
            struct w32abipll_ctrlC;
          };
    #define   SET32abipll_ctrlD_DIVFI(r32,v)                   _BFSET_(r32, 8, 0,v)
    #define   SET16abipll_ctrlD_DIVFI(r16,v)                   _BFSET_(r16, 8, 0,v)
    #define     w32abipll_ctrlD                                {\
            UNSG32 uctrlD_DIVFI                                :  9;\
            UNSG32 RSVDxC_b9                                   : 23;\
          }
    union { UNSG32 u32abipll_ctrlD;
            struct w32abipll_ctrlD;
          };
    #define   SET32abipll_ctrlE_DIVFF(r32,v)                   _BFSET_(r32,23, 0,v)
    #define     w32abipll_ctrlE                                {\
            UNSG32 uctrlE_DIVFF                                : 24;\
            UNSG32 RSVDx10_b24                                 :  8;\
          }
    union { UNSG32 u32abipll_ctrlE;
            struct w32abipll_ctrlE;
          };
    #define   SET32abipll_ctrlF_DIVQ(r32,v)                    _BFSET_(r32, 4, 0,v)
    #define   SET16abipll_ctrlF_DIVQ(r16,v)                    _BFSET_(r16, 4, 0,v)
    #define     w32abipll_ctrlF                                {\
            UNSG32 uctrlF_DIVQ                                 :  5;\
            UNSG32 RSVDx14_b5                                  : 27;\
          }
    union { UNSG32 u32abipll_ctrlF;
            struct w32abipll_ctrlF;
          };
    #define   SET32abipll_ctrlG_DIVQF(r32,v)                   _BFSET_(r32, 2, 0,v)
    #define   SET16abipll_ctrlG_DIVQF(r16,v)                   _BFSET_(r16, 2, 0,v)
    #define     w32abipll_ctrlG                                {\
            UNSG32 uctrlG_DIVQF                                :  3;\
            UNSG32 RSVDx18_b3                                  : 29;\
          }
    union { UNSG32 u32abipll_ctrlG;
            struct w32abipll_ctrlG;
          };
    #define   SET32abipll_status_LOCK(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16abipll_status_LOCK(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32abipll_status_DIVACK(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   SET16abipll_status_DIVACK(r16,v)                 _BFSET_(r16, 1, 1,v)
    #define     w32abipll_status                               {\
            UNSG32 ustatus_LOCK                                :  1;\
            UNSG32 ustatus_DIVACK                              :  1;\
            UNSG32 RSVDx1C_b2                                  : 30;\
          }
    union { UNSG32 u32abipll_status;
            struct w32abipll_status;
          };
    } SIE_abipll;
    typedef union  T32abipll_ctrlA
          { UNSG32 u32;
            struct w32abipll_ctrlA;
                 } T32abipll_ctrlA;
    typedef union  T32abipll_ctrlB
          { UNSG32 u32;
            struct w32abipll_ctrlB;
                 } T32abipll_ctrlB;
    typedef union  T32abipll_ctrlC
          { UNSG32 u32;
            struct w32abipll_ctrlC;
                 } T32abipll_ctrlC;
    typedef union  T32abipll_ctrlD
          { UNSG32 u32;
            struct w32abipll_ctrlD;
                 } T32abipll_ctrlD;
    typedef union  T32abipll_ctrlE
          { UNSG32 u32;
            struct w32abipll_ctrlE;
                 } T32abipll_ctrlE;
    typedef union  T32abipll_ctrlF
          { UNSG32 u32;
            struct w32abipll_ctrlF;
                 } T32abipll_ctrlF;
    typedef union  T32abipll_ctrlG
          { UNSG32 u32;
            struct w32abipll_ctrlG;
                 } T32abipll_ctrlG;
    typedef union  T32abipll_status
          { UNSG32 u32;
            struct w32abipll_status;
                 } T32abipll_status;
    typedef union  Tabipll_ctrlA
          { UNSG32 u32[1];
            struct {
            struct w32abipll_ctrlA;
                   };
                 } Tabipll_ctrlA;
    typedef union  Tabipll_ctrlB
          { UNSG32 u32[1];
            struct {
            struct w32abipll_ctrlB;
                   };
                 } Tabipll_ctrlB;
    typedef union  Tabipll_ctrlC
          { UNSG32 u32[1];
            struct {
            struct w32abipll_ctrlC;
                   };
                 } Tabipll_ctrlC;
    typedef union  Tabipll_ctrlD
          { UNSG32 u32[1];
            struct {
            struct w32abipll_ctrlD;
                   };
                 } Tabipll_ctrlD;
    typedef union  Tabipll_ctrlE
          { UNSG32 u32[1];
            struct {
            struct w32abipll_ctrlE;
                   };
                 } Tabipll_ctrlE;
    typedef union  Tabipll_ctrlF
          { UNSG32 u32[1];
            struct {
            struct w32abipll_ctrlF;
                   };
                 } Tabipll_ctrlF;
    typedef union  Tabipll_ctrlG
          { UNSG32 u32[1];
            struct {
            struct w32abipll_ctrlG;
                   };
                 } Tabipll_ctrlG;
    typedef union  Tabipll_status
          { UNSG32 u32[1];
            struct {
            struct w32abipll_status;
                   };
                 } Tabipll_status;
     SIGN32 abipll_drvrd(SIE_abipll *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 abipll_drvwr(SIE_abipll *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void abipll_reset(SIE_abipll *p);
     SIGN32 abipll_cmp  (SIE_abipll *p, SIE_abipll *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define abipll_check(p,pie,pfx,hLOG) abipll_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define abipll_print(p,    pfx,hLOG) abipll_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_pwrOff
#define h_pwrOff (){}
    #define     RA_pwrOff_ctrl                                 0x0000
    #define        pwrOff_ctrl_iso_eN_enable                                0x0
    #define        pwrOff_ctrl_iso_eN_disable                               0x1
    #define        pwrOff_ctrl_pwrSwitchCtrl_PWROFF                         0x0
    #define        pwrOff_ctrl_pwrSwitchCtrl_PWRON                          0x3
    #define        pwrOff_ctrl_pwrDomainRstN_enable                         0x0
    #define        pwrOff_ctrl_pwrDomainRstN_disable                        0x1
    #define     RA_pwrOff_status                               0x0004
    typedef struct SIE_pwrOff {
    #define   SET32pwrOff_ctrl_iso_eN(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16pwrOff_ctrl_iso_eN(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32pwrOff_ctrl_pwrSwitchCtrl(r32,v)            _BFSET_(r32, 2, 1,v)
    #define   SET16pwrOff_ctrl_pwrSwitchCtrl(r16,v)            _BFSET_(r16, 2, 1,v)
    #define   SET32pwrOff_ctrl_pwrDomainRstN(r32,v)            _BFSET_(r32, 3, 3,v)
    #define   SET16pwrOff_ctrl_pwrDomainRstN(r16,v)            _BFSET_(r16, 3, 3,v)
    #define     w32pwrOff_ctrl                                 {\
            UNSG32 uctrl_iso_eN                                :  1;\
            UNSG32 uctrl_pwrSwitchCtrl                         :  2;\
            UNSG32 uctrl_pwrDomainRstN                         :  1;\
            UNSG32 RSVDx0_b4                                   : 28;\
          }
    union { UNSG32 u32pwrOff_ctrl;
            struct w32pwrOff_ctrl;
          };
    #define   SET32pwrOff_status_pwrStatus(r32,v)              _BFSET_(r32, 1, 0,v)
    #define   SET16pwrOff_status_pwrStatus(r16,v)              _BFSET_(r16, 1, 0,v)
    #define     w32pwrOff_status                               {\
            UNSG32 ustatus_pwrStatus                           :  2;\
            UNSG32 RSVDx4_b2                                   : 30;\
          }
    union { UNSG32 u32pwrOff_status;
            struct w32pwrOff_status;
          };
    } SIE_pwrOff;
    typedef union  T32pwrOff_ctrl
          { UNSG32 u32;
            struct w32pwrOff_ctrl;
                 } T32pwrOff_ctrl;
    typedef union  T32pwrOff_status
          { UNSG32 u32;
            struct w32pwrOff_status;
                 } T32pwrOff_status;
    typedef union  TpwrOff_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32pwrOff_ctrl;
                   };
                 } TpwrOff_ctrl;
    typedef union  TpwrOff_status
          { UNSG32 u32[1];
            struct {
            struct w32pwrOff_status;
                   };
                 } TpwrOff_status;
     SIGN32 pwrOff_drvrd(SIE_pwrOff *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 pwrOff_drvwr(SIE_pwrOff *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void pwrOff_reset(SIE_pwrOff *p);
     SIGN32 pwrOff_cmp  (SIE_pwrOff *p, SIE_pwrOff *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define pwrOff_check(p,pie,pfx,hLOG) pwrOff_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define pwrOff_print(p,    pfx,hLOG) pwrOff_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_pwrOn
#define h_pwrOn (){}
    #define     RA_pwrOn_ctrl                                  0x0000
    #define        pwrOn_ctrl_iso_eN_enable                                 0x0
    #define        pwrOn_ctrl_iso_eN_disable                                0x1
    #define     RA_pwrOn_status                                0x0004
    typedef struct SIE_pwrOn {
    #define   SET32pwrOn_ctrl_iso_eN(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16pwrOn_ctrl_iso_eN(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32pwrOn_ctrl_pwrSwitchCtrl(r32,v)             _BFSET_(r32, 2, 1,v)
    #define   SET16pwrOn_ctrl_pwrSwitchCtrl(r16,v)             _BFSET_(r16, 2, 1,v)
    #define   SET32pwrOn_ctrl_pwrDomainRstN(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   SET16pwrOn_ctrl_pwrDomainRstN(r16,v)             _BFSET_(r16, 3, 3,v)
    #define     w32pwrOn_ctrl                                  {\
            UNSG32 uctrl_iso_eN                                :  1;\
            UNSG32 uctrl_pwrSwitchCtrl                         :  2;\
            UNSG32 uctrl_pwrDomainRstN                         :  1;\
            UNSG32 RSVDx0_b4                                   : 28;\
          }
    union { UNSG32 u32pwrOn_ctrl;
            struct w32pwrOn_ctrl;
          };
    #define   SET32pwrOn_status_pwrStatus(r32,v)               _BFSET_(r32, 1, 0,v)
    #define   SET16pwrOn_status_pwrStatus(r16,v)               _BFSET_(r16, 1, 0,v)
    #define     w32pwrOn_status                                {\
            UNSG32 ustatus_pwrStatus                           :  2;\
            UNSG32 RSVDx4_b2                                   : 30;\
          }
    union { UNSG32 u32pwrOn_status;
            struct w32pwrOn_status;
          };
    } SIE_pwrOn;
    typedef union  T32pwrOn_ctrl
          { UNSG32 u32;
            struct w32pwrOn_ctrl;
                 } T32pwrOn_ctrl;
    typedef union  T32pwrOn_status
          { UNSG32 u32;
            struct w32pwrOn_status;
                 } T32pwrOn_status;
    typedef union  TpwrOn_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32pwrOn_ctrl;
                   };
                 } TpwrOn_ctrl;
    typedef union  TpwrOn_status
          { UNSG32 u32[1];
            struct {
            struct w32pwrOn_status;
                   };
                 } TpwrOn_status;
     SIGN32 pwrOn_drvrd(SIE_pwrOn *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 pwrOn_drvwr(SIE_pwrOn *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void pwrOn_reset(SIE_pwrOn *p);
     SIGN32 pwrOn_cmp  (SIE_pwrOn *p, SIE_pwrOn *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define pwrOn_check(p,pie,pfx,hLOG) pwrOn_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define pwrOn_print(p,    pfx,hLOG) pwrOn_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_pwrOn_iso
#define h_pwrOn_iso (){}
    #define     RA_pwrOn_iso_ctrl                              0x0000
    #define        pwrOn_iso_ctrl_iso_eN_enable                             0x0
    #define        pwrOn_iso_ctrl_iso_eN_disable                            0x1
    #define     RA_pwrOn_iso_status                            0x0004
    typedef struct SIE_pwrOn_iso {
    #define   SET32pwrOn_iso_ctrl_iso_eN(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   SET16pwrOn_iso_ctrl_iso_eN(r16,v)                _BFSET_(r16, 0, 0,v)
    #define   SET32pwrOn_iso_ctrl_pwrSwitchCtrl(r32,v)         _BFSET_(r32, 2, 1,v)
    #define   SET16pwrOn_iso_ctrl_pwrSwitchCtrl(r16,v)         _BFSET_(r16, 2, 1,v)
    #define   SET32pwrOn_iso_ctrl_pwrDomainRstN(r32,v)         _BFSET_(r32, 3, 3,v)
    #define   SET16pwrOn_iso_ctrl_pwrDomainRstN(r16,v)         _BFSET_(r16, 3, 3,v)
    #define     w32pwrOn_iso_ctrl                              {\
            UNSG32 uctrl_iso_eN                                :  1;\
            UNSG32 uctrl_pwrSwitchCtrl                         :  2;\
            UNSG32 uctrl_pwrDomainRstN                         :  1;\
            UNSG32 RSVDx0_b4                                   : 28;\
          }
    union { UNSG32 u32pwrOn_iso_ctrl;
            struct w32pwrOn_iso_ctrl;
          };
    #define   SET32pwrOn_iso_status_pwrStatus(r32,v)           _BFSET_(r32, 1, 0,v)
    #define   SET16pwrOn_iso_status_pwrStatus(r16,v)           _BFSET_(r16, 1, 0,v)
    #define   SET32pwrOn_iso_status_IP_IDLE(r32,v)             _BFSET_(r32, 2, 2,v)
    #define   SET16pwrOn_iso_status_IP_IDLE(r16,v)             _BFSET_(r16, 2, 2,v)
    #define     w32pwrOn_iso_status                            {\
            UNSG32 ustatus_pwrStatus                           :  2;\
            UNSG32 ustatus_IP_IDLE                             :  1;\
            UNSG32 RSVDx4_b3                                   : 29;\
          }
    union { UNSG32 u32pwrOn_iso_status;
            struct w32pwrOn_iso_status;
          };
    } SIE_pwrOn_iso;
    typedef union  T32pwrOn_iso_ctrl
          { UNSG32 u32;
            struct w32pwrOn_iso_ctrl;
                 } T32pwrOn_iso_ctrl;
    typedef union  T32pwrOn_iso_status
          { UNSG32 u32;
            struct w32pwrOn_iso_status;
                 } T32pwrOn_iso_status;
    typedef union  TpwrOn_iso_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32pwrOn_iso_ctrl;
                   };
                 } TpwrOn_iso_ctrl;
    typedef union  TpwrOn_iso_status
          { UNSG32 u32[1];
            struct {
            struct w32pwrOn_iso_status;
                   };
                 } TpwrOn_iso_status;
     SIGN32 pwrOn_iso_drvrd(SIE_pwrOn_iso *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 pwrOn_iso_drvwr(SIE_pwrOn_iso *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void pwrOn_iso_reset(SIE_pwrOn_iso *p);
     SIGN32 pwrOn_iso_cmp  (SIE_pwrOn_iso *p, SIE_pwrOn_iso *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define pwrOn_iso_check(p,pie,pfx,hLOG) pwrOn_iso_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define pwrOn_iso_print(p,    pfx,hLOG) pwrOn_iso_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_SRAMPWR
#define h_SRAMPWR (){}
    #define     RA_SRAMPWR_ctrl                                0x0000
    #define        SRAMPWR_ctrl_SD_ON                                       0x0
    #define        SRAMPWR_ctrl_SD_SHUTDWN                                  0x1
    #define        SRAMPWR_ctrl_DSLP_ON                                     0x0
    #define        SRAMPWR_ctrl_DSLP_DEEPSLP                                0x1
    #define        SRAMPWR_ctrl_SLP_ON                                      0x0
    #define        SRAMPWR_ctrl_SLP_SLEEP                                   0x1
    typedef struct SIE_SRAMPWR {
    #define   SET32SRAMPWR_ctrl_SD(r32,v)                      _BFSET_(r32, 0, 0,v)
    #define   SET16SRAMPWR_ctrl_SD(r16,v)                      _BFSET_(r16, 0, 0,v)
    #define   SET32SRAMPWR_ctrl_DSLP(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16SRAMPWR_ctrl_DSLP(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32SRAMPWR_ctrl_SLP(r32,v)                     _BFSET_(r32, 2, 2,v)
    #define   SET16SRAMPWR_ctrl_SLP(r16,v)                     _BFSET_(r16, 2, 2,v)
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
    #define     RA_SRAMRWTC_ctrl1                              0x0004
    #define     RA_SRAMRWTC_ctrl2                              0x0008
    typedef struct SIE_SRAMRWTC {
    #define   SET32SRAMRWTC_ctrl0_RF1P(r32,v)                  _BFSET_(r32, 3, 0,v)
    #define   SET16SRAMRWTC_ctrl0_RF1P(r16,v)                  _BFSET_(r16, 3, 0,v)
    #define   SET32SRAMRWTC_ctrl0_UHDRF1P(r32,v)               _BFSET_(r32, 7, 4,v)
    #define   SET16SRAMRWTC_ctrl0_UHDRF1P(r16,v)               _BFSET_(r16, 7, 4,v)
    #define   SET32SRAMRWTC_ctrl0_RF2P(r32,v)                  _BFSET_(r32,15, 8,v)
    #define   SET16SRAMRWTC_ctrl0_RF2P(r16,v)                  _BFSET_(r16,15, 8,v)
    #define   SET32SRAMRWTC_ctrl0_UHDRF2P(r32,v)               _BFSET_(r32,23,16,v)
    #define   SET16SRAMRWTC_ctrl0_UHDRF2P(r16,v)               _BFSET_(r16, 7, 0,v)
    #define   SET32SRAMRWTC_ctrl0_UHDRF2P_ULVT(r32,v)          _BFSET_(r32,31,24,v)
    #define   SET16SRAMRWTC_ctrl0_UHDRF2P_ULVT(r16,v)          _BFSET_(r16,15, 8,v)
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
    #define   SET32SRAMRWTC_ctrl1_SHDMBSR1P(r32,v)             _BFSET_(r32, 3, 0,v)
    #define   SET16SRAMRWTC_ctrl1_SHDMBSR1P(r16,v)             _BFSET_(r16, 3, 0,v)
    #define   SET32SRAMRWTC_ctrl1_SHDSBSR1P(r32,v)             _BFSET_(r32, 7, 4,v)
    #define   SET16SRAMRWTC_ctrl1_SHDSBSR1P(r16,v)             _BFSET_(r16, 7, 4,v)
    #define   SET32SRAMRWTC_ctrl1_SHCMBSR1P_SSEG(r32,v)        _BFSET_(r32,11, 8,v)
    #define   SET16SRAMRWTC_ctrl1_SHCMBSR1P_SSEG(r16,v)        _BFSET_(r16,11, 8,v)
    #define   SET32SRAMRWTC_ctrl1_SHCMBSR1P_USEG(r32,v)        _BFSET_(r32,15,12,v)
    #define   SET16SRAMRWTC_ctrl1_SHCMBSR1P_USEG(r16,v)        _BFSET_(r16,15,12,v)
    #define   SET32SRAMRWTC_ctrl1_SHCSBSR1P(r32,v)             _BFSET_(r32,19,16,v)
    #define   SET16SRAMRWTC_ctrl1_SHCSBSR1P(r16,v)             _BFSET_(r16, 3, 0,v)
    #define   SET32SRAMRWTC_ctrl1_SHCSBSR1P_CUSTM(r32,v)       _BFSET_(r32,23,20,v)
    #define   SET16SRAMRWTC_ctrl1_SHCSBSR1P_CUSTM(r16,v)       _BFSET_(r16, 7, 4,v)
    #define   SET32SRAMRWTC_ctrl1_SPSRAM_WT0(r32,v)            _BFSET_(r32,27,24,v)
    #define   SET16SRAMRWTC_ctrl1_SPSRAM_WT0(r16,v)            _BFSET_(r16,11, 8,v)
    #define   SET32SRAMRWTC_ctrl1_SPSRAM_WT1(r32,v)            _BFSET_(r32,31,28,v)
    #define   SET16SRAMRWTC_ctrl1_SPSRAM_WT1(r16,v)            _BFSET_(r16,15,12,v)
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
    #define   SET32SRAMRWTC_ctrl2_L1CACHE(r32,v)               _BFSET_(r32, 3, 0,v)
    #define   SET16SRAMRWTC_ctrl2_L1CACHE(r16,v)               _BFSET_(r16, 3, 0,v)
    #define   SET32SRAMRWTC_ctrl2_DPSR2P(r32,v)                _BFSET_(r32, 7, 4,v)
    #define   SET16SRAMRWTC_ctrl2_DPSR2P(r16,v)                _BFSET_(r16, 7, 4,v)
    #define   SET32SRAMRWTC_ctrl2_ROM(r32,v)                   _BFSET_(r32,15, 8,v)
    #define   SET16SRAMRWTC_ctrl2_ROM(r16,v)                   _BFSET_(r16,15, 8,v)
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
#ifndef h_KPINT
#define h_KPINT (){}
    #define     RA_KPINT_KPINT_CFG                             0x0000
    #define     RA_KPINT_KPINT_COL                             0x0004
    #define     RA_KPINT_KPINT_ROW                             0x0008
    typedef struct SIE_KPINT {
    #define   SET32KPINT_KPINT_CFG_KPINT_COL_EN(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   SET16KPINT_KPINT_CFG_KPINT_COL_EN(r16,v)         _BFSET_(r16, 7, 0,v)
    #define   SET32KPINT_KPINT_CFG_KPINT_ROW_EN(r32,v)         _BFSET_(r32,17, 8,v)
    #define     w32KPINT_KPINT_CFG                             {\
            UNSG32 uKPINT_CFG_KPINT_COL_EN                     :  8;\
            UNSG32 uKPINT_CFG_KPINT_ROW_EN                     : 10;\
            UNSG32 RSVDx0_b18                                  : 14;\
          }
    union { UNSG32 u32KPINT_KPINT_CFG;
            struct w32KPINT_KPINT_CFG;
          };
    #define   SET32KPINT_KPINT_COL_COL_ACTIVATE(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   SET16KPINT_KPINT_COL_COL_ACTIVATE(r16,v)         _BFSET_(r16, 7, 0,v)
    #define     w32KPINT_KPINT_COL                             {\
            UNSG32 uKPINT_COL_COL_ACTIVATE                     :  8;\
            UNSG32 RSVDx4_b8                                   : 24;\
          }
    union { UNSG32 u32KPINT_KPINT_COL;
            struct w32KPINT_KPINT_COL;
          };
    #define   SET32KPINT_KPINT_ROW_KEY_PRESSED(r32,v)          _BFSET_(r32, 9, 0,v)
    #define   SET16KPINT_KPINT_ROW_KEY_PRESSED(r16,v)          _BFSET_(r16, 9, 0,v)
    #define     w32KPINT_KPINT_ROW                             {\
            UNSG32 uKPINT_ROW_KEY_PRESSED                      : 10;\
            UNSG32 RSVDx8_b10                                  : 22;\
          }
    union { UNSG32 u32KPINT_KPINT_ROW;
            struct w32KPINT_KPINT_ROW;
          };
    } SIE_KPINT;
    typedef union  T32KPINT_KPINT_CFG
          { UNSG32 u32;
            struct w32KPINT_KPINT_CFG;
                 } T32KPINT_KPINT_CFG;
    typedef union  T32KPINT_KPINT_COL
          { UNSG32 u32;
            struct w32KPINT_KPINT_COL;
                 } T32KPINT_KPINT_COL;
    typedef union  T32KPINT_KPINT_ROW
          { UNSG32 u32;
            struct w32KPINT_KPINT_ROW;
                 } T32KPINT_KPINT_ROW;
    typedef union  TKPINT_KPINT_CFG
          { UNSG32 u32[1];
            struct {
            struct w32KPINT_KPINT_CFG;
                   };
                 } TKPINT_KPINT_CFG;
    typedef union  TKPINT_KPINT_COL
          { UNSG32 u32[1];
            struct {
            struct w32KPINT_KPINT_COL;
                   };
                 } TKPINT_KPINT_COL;
    typedef union  TKPINT_KPINT_ROW
          { UNSG32 u32[1];
            struct {
            struct w32KPINT_KPINT_ROW;
                   };
                 } TKPINT_KPINT_ROW;
     SIGN32 KPINT_drvrd(SIE_KPINT *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 KPINT_drvwr(SIE_KPINT *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void KPINT_reset(SIE_KPINT *p);
     SIGN32 KPINT_cmp  (SIE_KPINT *p, SIE_KPINT *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define KPINT_check(p,pie,pfx,hLOG) KPINT_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define KPINT_print(p,    pfx,hLOG) KPINT_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD1
#define h_clkD1 (){}
    #define     RA_clkD1_ctrl                                  0x0000
    #define        clkD1_ctrl_ClkEn_enable                                  0x1
    #define        clkD1_ctrl_ClkEn_disable                                 0x0
    #define        clkD1_ctrl_ClkPllSel_CLKSRC0                             0x0
    #define        clkD1_ctrl_ClkPllSel_CLKSRC1                             0x1
    #define        clkD1_ctrl_ClkPllSel_CLKSRC2                             0x2
    #define        clkD1_ctrl_ClkPllSel_CLKSRC3                             0x3
    #define        clkD1_ctrl_ClkPllSel_CLKSRC4                             0x4
    #define        clkD1_ctrl_ClkPllSwitch_SYSPLL                           0x0
    #define        clkD1_ctrl_ClkPllSwitch_ALTPLL                           0x1
    #define        clkD1_ctrl_ClkSwitch_SrcClk                              0x0
    #define        clkD1_ctrl_ClkSwitch_DivClk                              0x1
    #define        clkD1_ctrl_ClkD3Switch_NonDiv3Clk                        0x0
    #define        clkD1_ctrl_ClkD3Switch_Div3Clk                           0x1
    #define        clkD1_ctrl_ClkSel_d2                                     0x1
    #define        clkD1_ctrl_ClkSel_d4                                     0x2
    #define        clkD1_ctrl_ClkSel_d6                                     0x3
    #define        clkD1_ctrl_ClkSel_d8                                     0x4
    #define        clkD1_ctrl_ClkSel_d12                                    0x5
    typedef struct SIE_clkD1 {
    #define   SET32clkD1_ctrl_ClkEn(r32,v)                     _BFSET_(r32, 0, 0,v)
    #define   SET16clkD1_ctrl_ClkEn(r16,v)                     _BFSET_(r16, 0, 0,v)
    #define   SET32clkD1_ctrl_ClkPllSel(r32,v)                 _BFSET_(r32, 3, 1,v)
    #define   SET16clkD1_ctrl_ClkPllSel(r16,v)                 _BFSET_(r16, 3, 1,v)
    #define   SET32clkD1_ctrl_ClkPllSwitch(r32,v)              _BFSET_(r32, 4, 4,v)
    #define   SET16clkD1_ctrl_ClkPllSwitch(r16,v)              _BFSET_(r16, 4, 4,v)
    #define   SET32clkD1_ctrl_ClkSwitch(r32,v)                 _BFSET_(r32, 5, 5,v)
    #define   SET16clkD1_ctrl_ClkSwitch(r16,v)                 _BFSET_(r16, 5, 5,v)
    #define   SET32clkD1_ctrl_ClkD3Switch(r32,v)               _BFSET_(r32, 6, 6,v)
    #define   SET16clkD1_ctrl_ClkD3Switch(r16,v)               _BFSET_(r16, 6, 6,v)
    #define   SET32clkD1_ctrl_ClkSel(r32,v)                    _BFSET_(r32, 9, 7,v)
    #define   SET16clkD1_ctrl_ClkSel(r16,v)                    _BFSET_(r16, 9, 7,v)
    #define     w32clkD1_ctrl                                  {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD1_ctrl;
            struct w32clkD1_ctrl;
          };
    } SIE_clkD1;
    typedef union  T32clkD1_ctrl
          { UNSG32 u32;
            struct w32clkD1_ctrl;
                 } T32clkD1_ctrl;
    typedef union  TclkD1_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD1_ctrl;
                   };
                 } TclkD1_ctrl;
     SIGN32 clkD1_drvrd(SIE_clkD1 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD1_drvwr(SIE_clkD1 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD1_reset(SIE_clkD1 *p);
     SIGN32 clkD1_cmp  (SIE_clkD1 *p, SIE_clkD1 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD1_check(p,pie,pfx,hLOG) clkD1_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD1_print(p,    pfx,hLOG) clkD1_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD2
#define h_clkD2 (){}
    #define     RA_clkD2_ctrl                                  0x0000
    #define        clkD2_ctrl_ClkEn_enable                                  0x1
    #define        clkD2_ctrl_ClkEn_disable                                 0x0
    #define        clkD2_ctrl_ClkPllSel_CLKSRC0                             0x0
    #define        clkD2_ctrl_ClkPllSel_CLKSRC1                             0x1
    #define        clkD2_ctrl_ClkPllSel_CLKSRC2                             0x2
    #define        clkD2_ctrl_ClkPllSel_CLKSRC3                             0x3
    #define        clkD2_ctrl_ClkPllSel_CLKSRC4                             0x4
    #define        clkD2_ctrl_ClkPllSwitch_SYSPLL                           0x0
    #define        clkD2_ctrl_ClkPllSwitch_ALTPLL                           0x1
    #define        clkD2_ctrl_ClkSwitch_SrcClk                              0x0
    #define        clkD2_ctrl_ClkSwitch_DivClk                              0x1
    #define        clkD2_ctrl_ClkD3Switch_NonDiv3Clk                        0x0
    #define        clkD2_ctrl_ClkD3Switch_Div3Clk                           0x1
    #define        clkD2_ctrl_ClkSel_d2                                     0x1
    #define        clkD2_ctrl_ClkSel_d4                                     0x2
    #define        clkD2_ctrl_ClkSel_d6                                     0x3
    #define        clkD2_ctrl_ClkSel_d8                                     0x4
    #define        clkD2_ctrl_ClkSel_d12                                    0x5
    #define        clkD2_ctrl_ClkSel_d24                                    0x6
    #define        clkD2_ctrl_ClkSel_d48                                    0x7
    typedef struct SIE_clkD2 {
    #define   SET32clkD2_ctrl_ClkEn(r32,v)                     _BFSET_(r32, 0, 0,v)
    #define   SET16clkD2_ctrl_ClkEn(r16,v)                     _BFSET_(r16, 0, 0,v)
    #define   SET32clkD2_ctrl_ClkPllSel(r32,v)                 _BFSET_(r32, 3, 1,v)
    #define   SET16clkD2_ctrl_ClkPllSel(r16,v)                 _BFSET_(r16, 3, 1,v)
    #define   SET32clkD2_ctrl_ClkPllSwitch(r32,v)              _BFSET_(r32, 4, 4,v)
    #define   SET16clkD2_ctrl_ClkPllSwitch(r16,v)              _BFSET_(r16, 4, 4,v)
    #define   SET32clkD2_ctrl_ClkSwitch(r32,v)                 _BFSET_(r32, 5, 5,v)
    #define   SET16clkD2_ctrl_ClkSwitch(r16,v)                 _BFSET_(r16, 5, 5,v)
    #define   SET32clkD2_ctrl_ClkD3Switch(r32,v)               _BFSET_(r32, 6, 6,v)
    #define   SET16clkD2_ctrl_ClkD3Switch(r16,v)               _BFSET_(r16, 6, 6,v)
    #define   SET32clkD2_ctrl_ClkSel(r32,v)                    _BFSET_(r32, 9, 7,v)
    #define   SET16clkD2_ctrl_ClkSel(r16,v)                    _BFSET_(r16, 9, 7,v)
    #define     w32clkD2_ctrl                                  {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD2_ctrl;
            struct w32clkD2_ctrl;
          };
    } SIE_clkD2;
    typedef union  T32clkD2_ctrl
          { UNSG32 u32;
            struct w32clkD2_ctrl;
                 } T32clkD2_ctrl;
    typedef union  TclkD2_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD2_ctrl;
                   };
                 } TclkD2_ctrl;
     SIGN32 clkD2_drvrd(SIE_clkD2 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD2_drvwr(SIE_clkD2 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD2_reset(SIE_clkD2 *p);
     SIGN32 clkD2_cmp  (SIE_clkD2 *p, SIE_clkD2 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD2_check(p,pie,pfx,hLOG) clkD2_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD2_print(p,    pfx,hLOG) clkD2_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD4
#define h_clkD4 (){}
    #define     RA_clkD4_ctrl                                  0x0000
    #define        clkD4_ctrl_ClkEn_enable                                  0x1
    #define        clkD4_ctrl_ClkEn_disable                                 0x0
    #define        clkD4_ctrl_ClkPllSel_CLKSRC0                             0x0
    #define        clkD4_ctrl_ClkPllSel_CLKSRC1                             0x1
    #define        clkD4_ctrl_ClkPllSel_CLKSRC2                             0x2
    #define        clkD4_ctrl_ClkPllSel_CLKSRC3                             0x3
    #define        clkD4_ctrl_ClkPllSel_CLKSRC4                             0x4
    #define        clkD4_ctrl_ClkPllSwitch_SYSPLL                           0x0
    #define        clkD4_ctrl_ClkPllSwitch_ALTPLL                           0x1
    #define        clkD4_ctrl_ClkSwitch_SrcClk                              0x0
    #define        clkD4_ctrl_ClkSwitch_DivClk                              0x1
    #define        clkD4_ctrl_ClkD3Switch_NonDiv3Clk                        0x0
    #define        clkD4_ctrl_ClkD3Switch_Div3Clk                           0x1
    #define        clkD4_ctrl_ClkSel_d2                                     0x1
    #define        clkD4_ctrl_ClkSel_d4                                     0x2
    #define        clkD4_ctrl_ClkSel_d6                                     0x3
    #define        clkD4_ctrl_ClkSel_d8                                     0x4
    #define        clkD4_ctrl_ClkSel_d12                                    0x5
    typedef struct SIE_clkD4 {
    #define   SET32clkD4_ctrl_ClkEn(r32,v)                     _BFSET_(r32, 0, 0,v)
    #define   SET16clkD4_ctrl_ClkEn(r16,v)                     _BFSET_(r16, 0, 0,v)
    #define   SET32clkD4_ctrl_ClkPllSel(r32,v)                 _BFSET_(r32, 3, 1,v)
    #define   SET16clkD4_ctrl_ClkPllSel(r16,v)                 _BFSET_(r16, 3, 1,v)
    #define   SET32clkD4_ctrl_ClkPllSwitch(r32,v)              _BFSET_(r32, 4, 4,v)
    #define   SET16clkD4_ctrl_ClkPllSwitch(r16,v)              _BFSET_(r16, 4, 4,v)
    #define   SET32clkD4_ctrl_ClkSwitch(r32,v)                 _BFSET_(r32, 5, 5,v)
    #define   SET16clkD4_ctrl_ClkSwitch(r16,v)                 _BFSET_(r16, 5, 5,v)
    #define   SET32clkD4_ctrl_ClkD3Switch(r32,v)               _BFSET_(r32, 6, 6,v)
    #define   SET16clkD4_ctrl_ClkD3Switch(r16,v)               _BFSET_(r16, 6, 6,v)
    #define   SET32clkD4_ctrl_ClkSel(r32,v)                    _BFSET_(r32, 9, 7,v)
    #define   SET16clkD4_ctrl_ClkSel(r16,v)                    _BFSET_(r16, 9, 7,v)
    #define     w32clkD4_ctrl                                  {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD4_ctrl;
            struct w32clkD4_ctrl;
          };
    } SIE_clkD4;
    typedef union  T32clkD4_ctrl
          { UNSG32 u32;
            struct w32clkD4_ctrl;
                 } T32clkD4_ctrl;
    typedef union  TclkD4_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD4_ctrl;
                   };
                 } TclkD4_ctrl;
     SIGN32 clkD4_drvrd(SIE_clkD4 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD4_drvwr(SIE_clkD4 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD4_reset(SIE_clkD4 *p);
     SIGN32 clkD4_cmp  (SIE_clkD4 *p, SIE_clkD4 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD4_check(p,pie,pfx,hLOG) clkD4_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD4_print(p,    pfx,hLOG) clkD4_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD6
#define h_clkD6 (){}
    #define     RA_clkD6_ctrl                                  0x0000
    #define        clkD6_ctrl_ClkEn_enable                                  0x1
    #define        clkD6_ctrl_ClkEn_disable                                 0x0
    #define        clkD6_ctrl_ClkPllSel_CLKSRC0                             0x0
    #define        clkD6_ctrl_ClkPllSel_CLKSRC1                             0x1
    #define        clkD6_ctrl_ClkPllSel_CLKSRC2                             0x2
    #define        clkD6_ctrl_ClkPllSel_CLKSRC3                             0x3
    #define        clkD6_ctrl_ClkPllSel_CLKSRC4                             0x4
    #define        clkD6_ctrl_ClkPllSwitch_SYSPLL                           0x0
    #define        clkD6_ctrl_ClkPllSwitch_ALTPLL                           0x1
    #define        clkD6_ctrl_ClkSwitch_SrcClk                              0x0
    #define        clkD6_ctrl_ClkSwitch_DivClk                              0x1
    #define        clkD6_ctrl_ClkD3Switch_NonDiv3Clk                        0x0
    #define        clkD6_ctrl_ClkD3Switch_Div3Clk                           0x1
    #define        clkD6_ctrl_ClkSel_d2                                     0x1
    #define        clkD6_ctrl_ClkSel_d4                                     0x2
    #define        clkD6_ctrl_ClkSel_d6                                     0x3
    #define        clkD6_ctrl_ClkSel_d8                                     0x4
    #define        clkD6_ctrl_ClkSel_d12                                    0x5
    typedef struct SIE_clkD6 {
    #define   SET32clkD6_ctrl_ClkEn(r32,v)                     _BFSET_(r32, 0, 0,v)
    #define   SET16clkD6_ctrl_ClkEn(r16,v)                     _BFSET_(r16, 0, 0,v)
    #define   SET32clkD6_ctrl_ClkPllSel(r32,v)                 _BFSET_(r32, 3, 1,v)
    #define   SET16clkD6_ctrl_ClkPllSel(r16,v)                 _BFSET_(r16, 3, 1,v)
    #define   SET32clkD6_ctrl_ClkPllSwitch(r32,v)              _BFSET_(r32, 4, 4,v)
    #define   SET16clkD6_ctrl_ClkPllSwitch(r16,v)              _BFSET_(r16, 4, 4,v)
    #define   SET32clkD6_ctrl_ClkSwitch(r32,v)                 _BFSET_(r32, 5, 5,v)
    #define   SET16clkD6_ctrl_ClkSwitch(r16,v)                 _BFSET_(r16, 5, 5,v)
    #define   SET32clkD6_ctrl_ClkD3Switch(r32,v)               _BFSET_(r32, 6, 6,v)
    #define   SET16clkD6_ctrl_ClkD3Switch(r16,v)               _BFSET_(r16, 6, 6,v)
    #define   SET32clkD6_ctrl_ClkSel(r32,v)                    _BFSET_(r32, 9, 7,v)
    #define   SET16clkD6_ctrl_ClkSel(r16,v)                    _BFSET_(r16, 9, 7,v)
    #define     w32clkD6_ctrl                                  {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD6_ctrl;
            struct w32clkD6_ctrl;
          };
    } SIE_clkD6;
    typedef union  T32clkD6_ctrl
          { UNSG32 u32;
            struct w32clkD6_ctrl;
                 } T32clkD6_ctrl;
    typedef union  TclkD6_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD6_ctrl;
                   };
                 } TclkD6_ctrl;
     SIGN32 clkD6_drvrd(SIE_clkD6 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD6_drvwr(SIE_clkD6 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD6_reset(SIE_clkD6 *p);
     SIGN32 clkD6_cmp  (SIE_clkD6 *p, SIE_clkD6 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD6_check(p,pie,pfx,hLOG) clkD6_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD6_print(p,    pfx,hLOG) clkD6_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD8
#define h_clkD8 (){}
    #define     RA_clkD8_ctrl                                  0x0000
    #define        clkD8_ctrl_ClkEn_enable                                  0x1
    #define        clkD8_ctrl_ClkEn_disable                                 0x0
    #define        clkD8_ctrl_ClkPllSel_CLKSRC0                             0x0
    #define        clkD8_ctrl_ClkPllSel_CLKSRC1                             0x1
    #define        clkD8_ctrl_ClkPllSel_CLKSRC2                             0x2
    #define        clkD8_ctrl_ClkPllSel_CLKSRC3                             0x3
    #define        clkD8_ctrl_ClkPllSel_CLKSRC4                             0x4
    #define        clkD8_ctrl_ClkPllSwitch_SYSPLL                           0x0
    #define        clkD8_ctrl_ClkPllSwitch_ALTPLL                           0x1
    #define        clkD8_ctrl_ClkSwitch_SrcClk                              0x0
    #define        clkD8_ctrl_ClkSwitch_DivClk                              0x1
    #define        clkD8_ctrl_ClkD3Switch_NonDiv3Clk                        0x0
    #define        clkD8_ctrl_ClkD3Switch_Div3Clk                           0x1
    #define        clkD8_ctrl_ClkSel_d2                                     0x1
    #define        clkD8_ctrl_ClkSel_d4                                     0x2
    #define        clkD8_ctrl_ClkSel_d6                                     0x3
    #define        clkD8_ctrl_ClkSel_d8                                     0x4
    #define        clkD8_ctrl_ClkSel_d12                                    0x5
    typedef struct SIE_clkD8 {
    #define   SET32clkD8_ctrl_ClkEn(r32,v)                     _BFSET_(r32, 0, 0,v)
    #define   SET16clkD8_ctrl_ClkEn(r16,v)                     _BFSET_(r16, 0, 0,v)
    #define   SET32clkD8_ctrl_ClkPllSel(r32,v)                 _BFSET_(r32, 3, 1,v)
    #define   SET16clkD8_ctrl_ClkPllSel(r16,v)                 _BFSET_(r16, 3, 1,v)
    #define   SET32clkD8_ctrl_ClkPllSwitch(r32,v)              _BFSET_(r32, 4, 4,v)
    #define   SET16clkD8_ctrl_ClkPllSwitch(r16,v)              _BFSET_(r16, 4, 4,v)
    #define   SET32clkD8_ctrl_ClkSwitch(r32,v)                 _BFSET_(r32, 5, 5,v)
    #define   SET16clkD8_ctrl_ClkSwitch(r16,v)                 _BFSET_(r16, 5, 5,v)
    #define   SET32clkD8_ctrl_ClkD3Switch(r32,v)               _BFSET_(r32, 6, 6,v)
    #define   SET16clkD8_ctrl_ClkD3Switch(r16,v)               _BFSET_(r16, 6, 6,v)
    #define   SET32clkD8_ctrl_ClkSel(r32,v)                    _BFSET_(r32, 9, 7,v)
    #define   SET16clkD8_ctrl_ClkSel(r16,v)                    _BFSET_(r16, 9, 7,v)
    #define     w32clkD8_ctrl                                  {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD8_ctrl;
            struct w32clkD8_ctrl;
          };
    } SIE_clkD8;
    typedef union  T32clkD8_ctrl
          { UNSG32 u32;
            struct w32clkD8_ctrl;
                 } T32clkD8_ctrl;
    typedef union  TclkD8_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD8_ctrl;
                   };
                 } TclkD8_ctrl;
     SIGN32 clkD8_drvrd(SIE_clkD8 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD8_drvwr(SIE_clkD8 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD8_reset(SIE_clkD8 *p);
     SIGN32 clkD8_cmp  (SIE_clkD8 *p, SIE_clkD8 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD8_check(p,pie,pfx,hLOG) clkD8_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD8_print(p,    pfx,hLOG) clkD8_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD12
#define h_clkD12 (){}
    #define     RA_clkD12_ctrl                                 0x0000
    #define        clkD12_ctrl_ClkEn_enable                                 0x1
    #define        clkD12_ctrl_ClkEn_disable                                0x0
    #define        clkD12_ctrl_ClkPllSel_CLKSRC0                            0x0
    #define        clkD12_ctrl_ClkPllSel_CLKSRC1                            0x1
    #define        clkD12_ctrl_ClkPllSel_CLKSRC2                            0x2
    #define        clkD12_ctrl_ClkPllSel_CLKSRC3                            0x3
    #define        clkD12_ctrl_ClkPllSel_CLKSRC4                            0x4
    #define        clkD12_ctrl_ClkPllSwitch_SYSPLL                          0x0
    #define        clkD12_ctrl_ClkPllSwitch_ALTPLL                          0x1
    #define        clkD12_ctrl_ClkSwitch_SrcClk                             0x0
    #define        clkD12_ctrl_ClkSwitch_DivClk                             0x1
    #define        clkD12_ctrl_ClkD3Switch_NonDiv3Clk                       0x0
    #define        clkD12_ctrl_ClkD3Switch_Div3Clk                          0x1
    #define        clkD12_ctrl_ClkSel_d2                                    0x1
    #define        clkD12_ctrl_ClkSel_d4                                    0x2
    #define        clkD12_ctrl_ClkSel_d6                                    0x3
    #define        clkD12_ctrl_ClkSel_d8                                    0x4
    #define        clkD12_ctrl_ClkSel_d12                                   0x5
    typedef struct SIE_clkD12 {
    #define   SET32clkD12_ctrl_ClkEn(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16clkD12_ctrl_ClkEn(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32clkD12_ctrl_ClkPllSel(r32,v)                _BFSET_(r32, 3, 1,v)
    #define   SET16clkD12_ctrl_ClkPllSel(r16,v)                _BFSET_(r16, 3, 1,v)
    #define   SET32clkD12_ctrl_ClkPllSwitch(r32,v)             _BFSET_(r32, 4, 4,v)
    #define   SET16clkD12_ctrl_ClkPllSwitch(r16,v)             _BFSET_(r16, 4, 4,v)
    #define   SET32clkD12_ctrl_ClkSwitch(r32,v)                _BFSET_(r32, 5, 5,v)
    #define   SET16clkD12_ctrl_ClkSwitch(r16,v)                _BFSET_(r16, 5, 5,v)
    #define   SET32clkD12_ctrl_ClkD3Switch(r32,v)              _BFSET_(r32, 6, 6,v)
    #define   SET16clkD12_ctrl_ClkD3Switch(r16,v)              _BFSET_(r16, 6, 6,v)
    #define   SET32clkD12_ctrl_ClkSel(r32,v)                   _BFSET_(r32, 9, 7,v)
    #define   SET16clkD12_ctrl_ClkSel(r16,v)                   _BFSET_(r16, 9, 7,v)
    #define     w32clkD12_ctrl                                 {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD12_ctrl;
            struct w32clkD12_ctrl;
          };
    } SIE_clkD12;
    typedef union  T32clkD12_ctrl
          { UNSG32 u32;
            struct w32clkD12_ctrl;
                 } T32clkD12_ctrl;
    typedef union  TclkD12_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD12_ctrl;
                   };
                 } TclkD12_ctrl;
     SIGN32 clkD12_drvrd(SIE_clkD12 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD12_drvwr(SIE_clkD12 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD12_reset(SIE_clkD12 *p);
     SIGN32 clkD12_cmp  (SIE_clkD12 *p, SIE_clkD12 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD12_check(p,pie,pfx,hLOG) clkD12_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD12_print(p,    pfx,hLOG) clkD12_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD1_ENOFF
#define h_clkD1_ENOFF (){}
    #define     RA_clkD1_ENOFF_ctrl                            0x0000
    #define        clkD1_ENOFF_ctrl_ClkEn_enable                            0x1
    #define        clkD1_ENOFF_ctrl_ClkEn_disable                           0x0
    #define        clkD1_ENOFF_ctrl_ClkPllSel_CLKSRC0                       0x0
    #define        clkD1_ENOFF_ctrl_ClkPllSel_CLKSRC1                       0x1
    #define        clkD1_ENOFF_ctrl_ClkPllSel_CLKSRC2                       0x2
    #define        clkD1_ENOFF_ctrl_ClkPllSel_CLKSRC3                       0x3
    #define        clkD1_ENOFF_ctrl_ClkPllSel_CLKSRC4                       0x4
    #define        clkD1_ENOFF_ctrl_ClkPllSwitch_SYSPLL                     0x0
    #define        clkD1_ENOFF_ctrl_ClkPllSwitch_ALTPLL                     0x1
    #define        clkD1_ENOFF_ctrl_ClkSwitch_SrcClk                        0x0
    #define        clkD1_ENOFF_ctrl_ClkSwitch_DivClk                        0x1
    #define        clkD1_ENOFF_ctrl_ClkD3Switch_NonDiv3Clk                  0x0
    #define        clkD1_ENOFF_ctrl_ClkD3Switch_Div3Clk                     0x1
    #define        clkD1_ENOFF_ctrl_ClkSel_d2                               0x1
    #define        clkD1_ENOFF_ctrl_ClkSel_d4                               0x2
    #define        clkD1_ENOFF_ctrl_ClkSel_d6                               0x3
    #define        clkD1_ENOFF_ctrl_ClkSel_d8                               0x4
    #define        clkD1_ENOFF_ctrl_ClkSel_d12                              0x5
    typedef struct SIE_clkD1_ENOFF {
    #define   SET32clkD1_ENOFF_ctrl_ClkEn(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   SET16clkD1_ENOFF_ctrl_ClkEn(r16,v)               _BFSET_(r16, 0, 0,v)
    #define   SET32clkD1_ENOFF_ctrl_ClkPllSel(r32,v)           _BFSET_(r32, 3, 1,v)
    #define   SET16clkD1_ENOFF_ctrl_ClkPllSel(r16,v)           _BFSET_(r16, 3, 1,v)
    #define   SET32clkD1_ENOFF_ctrl_ClkPllSwitch(r32,v)        _BFSET_(r32, 4, 4,v)
    #define   SET16clkD1_ENOFF_ctrl_ClkPllSwitch(r16,v)        _BFSET_(r16, 4, 4,v)
    #define   SET32clkD1_ENOFF_ctrl_ClkSwitch(r32,v)           _BFSET_(r32, 5, 5,v)
    #define   SET16clkD1_ENOFF_ctrl_ClkSwitch(r16,v)           _BFSET_(r16, 5, 5,v)
    #define   SET32clkD1_ENOFF_ctrl_ClkD3Switch(r32,v)         _BFSET_(r32, 6, 6,v)
    #define   SET16clkD1_ENOFF_ctrl_ClkD3Switch(r16,v)         _BFSET_(r16, 6, 6,v)
    #define   SET32clkD1_ENOFF_ctrl_ClkSel(r32,v)              _BFSET_(r32, 9, 7,v)
    #define   SET16clkD1_ENOFF_ctrl_ClkSel(r16,v)              _BFSET_(r16, 9, 7,v)
    #define     w32clkD1_ENOFF_ctrl                            {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD1_ENOFF_ctrl;
            struct w32clkD1_ENOFF_ctrl;
          };
    } SIE_clkD1_ENOFF;
    typedef union  T32clkD1_ENOFF_ctrl
          { UNSG32 u32;
            struct w32clkD1_ENOFF_ctrl;
                 } T32clkD1_ENOFF_ctrl;
    typedef union  TclkD1_ENOFF_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD1_ENOFF_ctrl;
                   };
                 } TclkD1_ENOFF_ctrl;
     SIGN32 clkD1_ENOFF_drvrd(SIE_clkD1_ENOFF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD1_ENOFF_drvwr(SIE_clkD1_ENOFF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD1_ENOFF_reset(SIE_clkD1_ENOFF *p);
     SIGN32 clkD1_ENOFF_cmp  (SIE_clkD1_ENOFF *p, SIE_clkD1_ENOFF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD1_ENOFF_check(p,pie,pfx,hLOG) clkD1_ENOFF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD1_ENOFF_print(p,    pfx,hLOG) clkD1_ENOFF_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD4_ENOFF
#define h_clkD4_ENOFF (){}
    #define     RA_clkD4_ENOFF_ctrl                            0x0000
    #define        clkD4_ENOFF_ctrl_ClkEn_enable                            0x1
    #define        clkD4_ENOFF_ctrl_ClkEn_disable                           0x0
    #define        clkD4_ENOFF_ctrl_ClkPllSel_CLKSRC0                       0x0
    #define        clkD4_ENOFF_ctrl_ClkPllSel_CLKSRC1                       0x1
    #define        clkD4_ENOFF_ctrl_ClkPllSel_CLKSRC2                       0x2
    #define        clkD4_ENOFF_ctrl_ClkPllSel_CLKSRC3                       0x3
    #define        clkD4_ENOFF_ctrl_ClkPllSel_CLKSRC4                       0x4
    #define        clkD4_ENOFF_ctrl_ClkPllSwitch_SYSPLL                     0x0
    #define        clkD4_ENOFF_ctrl_ClkPllSwitch_ALTPLL                     0x1
    #define        clkD4_ENOFF_ctrl_ClkSwitch_SrcClk                        0x0
    #define        clkD4_ENOFF_ctrl_ClkSwitch_DivClk                        0x1
    #define        clkD4_ENOFF_ctrl_ClkD3Switch_NonDiv3Clk                  0x0
    #define        clkD4_ENOFF_ctrl_ClkD3Switch_Div3Clk                     0x1
    #define        clkD4_ENOFF_ctrl_ClkSel_d2                               0x1
    #define        clkD4_ENOFF_ctrl_ClkSel_d4                               0x2
    #define        clkD4_ENOFF_ctrl_ClkSel_d6                               0x3
    #define        clkD4_ENOFF_ctrl_ClkSel_d8                               0x4
    #define        clkD4_ENOFF_ctrl_ClkSel_d12                              0x5
    typedef struct SIE_clkD4_ENOFF {
    #define   SET32clkD4_ENOFF_ctrl_ClkEn(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   SET16clkD4_ENOFF_ctrl_ClkEn(r16,v)               _BFSET_(r16, 0, 0,v)
    #define   SET32clkD4_ENOFF_ctrl_ClkPllSel(r32,v)           _BFSET_(r32, 3, 1,v)
    #define   SET16clkD4_ENOFF_ctrl_ClkPllSel(r16,v)           _BFSET_(r16, 3, 1,v)
    #define   SET32clkD4_ENOFF_ctrl_ClkPllSwitch(r32,v)        _BFSET_(r32, 4, 4,v)
    #define   SET16clkD4_ENOFF_ctrl_ClkPllSwitch(r16,v)        _BFSET_(r16, 4, 4,v)
    #define   SET32clkD4_ENOFF_ctrl_ClkSwitch(r32,v)           _BFSET_(r32, 5, 5,v)
    #define   SET16clkD4_ENOFF_ctrl_ClkSwitch(r16,v)           _BFSET_(r16, 5, 5,v)
    #define   SET32clkD4_ENOFF_ctrl_ClkD3Switch(r32,v)         _BFSET_(r32, 6, 6,v)
    #define   SET16clkD4_ENOFF_ctrl_ClkD3Switch(r16,v)         _BFSET_(r16, 6, 6,v)
    #define   SET32clkD4_ENOFF_ctrl_ClkSel(r32,v)              _BFSET_(r32, 9, 7,v)
    #define   SET16clkD4_ENOFF_ctrl_ClkSel(r16,v)              _BFSET_(r16, 9, 7,v)
    #define     w32clkD4_ENOFF_ctrl                            {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD4_ENOFF_ctrl;
            struct w32clkD4_ENOFF_ctrl;
          };
    } SIE_clkD4_ENOFF;
    typedef union  T32clkD4_ENOFF_ctrl
          { UNSG32 u32;
            struct w32clkD4_ENOFF_ctrl;
                 } T32clkD4_ENOFF_ctrl;
    typedef union  TclkD4_ENOFF_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD4_ENOFF_ctrl;
                   };
                 } TclkD4_ENOFF_ctrl;
     SIGN32 clkD4_ENOFF_drvrd(SIE_clkD4_ENOFF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD4_ENOFF_drvwr(SIE_clkD4_ENOFF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD4_ENOFF_reset(SIE_clkD4_ENOFF *p);
     SIGN32 clkD4_ENOFF_cmp  (SIE_clkD4_ENOFF *p, SIE_clkD4_ENOFF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD4_ENOFF_check(p,pie,pfx,hLOG) clkD4_ENOFF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD4_ENOFF_print(p,    pfx,hLOG) clkD4_ENOFF_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD8_ENOFF
#define h_clkD8_ENOFF (){}
    #define     RA_clkD8_ENOFF_ctrl                            0x0000
    #define        clkD8_ENOFF_ctrl_ClkEn_enable                            0x1
    #define        clkD8_ENOFF_ctrl_ClkEn_disable                           0x0
    #define        clkD8_ENOFF_ctrl_ClkPllSel_CLKSRC0                       0x0
    #define        clkD8_ENOFF_ctrl_ClkPllSel_CLKSRC1                       0x1
    #define        clkD8_ENOFF_ctrl_ClkPllSel_CLKSRC2                       0x2
    #define        clkD8_ENOFF_ctrl_ClkPllSel_CLKSRC3                       0x3
    #define        clkD8_ENOFF_ctrl_ClkPllSel_CLKSRC4                       0x4
    #define        clkD8_ENOFF_ctrl_ClkPllSwitch_SYSPLL                     0x0
    #define        clkD8_ENOFF_ctrl_ClkPllSwitch_ALTPLL                     0x1
    #define        clkD8_ENOFF_ctrl_ClkSwitch_SrcClk                        0x0
    #define        clkD8_ENOFF_ctrl_ClkSwitch_DivClk                        0x1
    #define        clkD8_ENOFF_ctrl_ClkD3Switch_NonDiv3Clk                  0x0
    #define        clkD8_ENOFF_ctrl_ClkD3Switch_Div3Clk                     0x1
    #define        clkD8_ENOFF_ctrl_ClkSel_d2                               0x1
    #define        clkD8_ENOFF_ctrl_ClkSel_d4                               0x2
    #define        clkD8_ENOFF_ctrl_ClkSel_d6                               0x3
    #define        clkD8_ENOFF_ctrl_ClkSel_d8                               0x4
    #define        clkD8_ENOFF_ctrl_ClkSel_d12                              0x5
    typedef struct SIE_clkD8_ENOFF {
    #define   SET32clkD8_ENOFF_ctrl_ClkEn(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   SET16clkD8_ENOFF_ctrl_ClkEn(r16,v)               _BFSET_(r16, 0, 0,v)
    #define   SET32clkD8_ENOFF_ctrl_ClkPllSel(r32,v)           _BFSET_(r32, 3, 1,v)
    #define   SET16clkD8_ENOFF_ctrl_ClkPllSel(r16,v)           _BFSET_(r16, 3, 1,v)
    #define   SET32clkD8_ENOFF_ctrl_ClkPllSwitch(r32,v)        _BFSET_(r32, 4, 4,v)
    #define   SET16clkD8_ENOFF_ctrl_ClkPllSwitch(r16,v)        _BFSET_(r16, 4, 4,v)
    #define   SET32clkD8_ENOFF_ctrl_ClkSwitch(r32,v)           _BFSET_(r32, 5, 5,v)
    #define   SET16clkD8_ENOFF_ctrl_ClkSwitch(r16,v)           _BFSET_(r16, 5, 5,v)
    #define   SET32clkD8_ENOFF_ctrl_ClkD3Switch(r32,v)         _BFSET_(r32, 6, 6,v)
    #define   SET16clkD8_ENOFF_ctrl_ClkD3Switch(r16,v)         _BFSET_(r16, 6, 6,v)
    #define   SET32clkD8_ENOFF_ctrl_ClkSel(r32,v)              _BFSET_(r32, 9, 7,v)
    #define   SET16clkD8_ENOFF_ctrl_ClkSel(r16,v)              _BFSET_(r16, 9, 7,v)
    #define     w32clkD8_ENOFF_ctrl                            {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD8_ENOFF_ctrl;
            struct w32clkD8_ENOFF_ctrl;
          };
    } SIE_clkD8_ENOFF;
    typedef union  T32clkD8_ENOFF_ctrl
          { UNSG32 u32;
            struct w32clkD8_ENOFF_ctrl;
                 } T32clkD8_ENOFF_ctrl;
    typedef union  TclkD8_ENOFF_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD8_ENOFF_ctrl;
                   };
                 } TclkD8_ENOFF_ctrl;
     SIGN32 clkD8_ENOFF_drvrd(SIE_clkD8_ENOFF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD8_ENOFF_drvwr(SIE_clkD8_ENOFF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD8_ENOFF_reset(SIE_clkD8_ENOFF *p);
     SIGN32 clkD8_ENOFF_cmp  (SIE_clkD8_ENOFF *p, SIE_clkD8_ENOFF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD8_ENOFF_check(p,pie,pfx,hLOG) clkD8_ENOFF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD8_ENOFF_print(p,    pfx,hLOG) clkD8_ENOFF_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_clkD12_ENOFF
#define h_clkD12_ENOFF (){}
    #define     RA_clkD12_ENOFF_ctrl                           0x0000
    #define        clkD12_ENOFF_ctrl_ClkEn_enable                           0x1
    #define        clkD12_ENOFF_ctrl_ClkEn_disable                          0x0
    #define        clkD12_ENOFF_ctrl_ClkPllSel_CLKSRC0                      0x0
    #define        clkD12_ENOFF_ctrl_ClkPllSel_CLKSRC1                      0x1
    #define        clkD12_ENOFF_ctrl_ClkPllSel_CLKSRC2                      0x2
    #define        clkD12_ENOFF_ctrl_ClkPllSel_CLKSRC3                      0x3
    #define        clkD12_ENOFF_ctrl_ClkPllSel_CLKSRC4                      0x4
    #define        clkD12_ENOFF_ctrl_ClkPllSwitch_SYSPLL                    0x0
    #define        clkD12_ENOFF_ctrl_ClkPllSwitch_ALTPLL                    0x1
    #define        clkD12_ENOFF_ctrl_ClkSwitch_SrcClk                       0x0
    #define        clkD12_ENOFF_ctrl_ClkSwitch_DivClk                       0x1
    #define        clkD12_ENOFF_ctrl_ClkD3Switch_NonDiv3Clk                 0x0
    #define        clkD12_ENOFF_ctrl_ClkD3Switch_Div3Clk                    0x1
    #define        clkD12_ENOFF_ctrl_ClkSel_d2                              0x1
    #define        clkD12_ENOFF_ctrl_ClkSel_d4                              0x2
    #define        clkD12_ENOFF_ctrl_ClkSel_d6                              0x3
    #define        clkD12_ENOFF_ctrl_ClkSel_d8                              0x4
    #define        clkD12_ENOFF_ctrl_ClkSel_d12                             0x5
    typedef struct SIE_clkD12_ENOFF {
    #define   SET32clkD12_ENOFF_ctrl_ClkEn(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   SET16clkD12_ENOFF_ctrl_ClkEn(r16,v)              _BFSET_(r16, 0, 0,v)
    #define   SET32clkD12_ENOFF_ctrl_ClkPllSel(r32,v)          _BFSET_(r32, 3, 1,v)
    #define   SET16clkD12_ENOFF_ctrl_ClkPllSel(r16,v)          _BFSET_(r16, 3, 1,v)
    #define   SET32clkD12_ENOFF_ctrl_ClkPllSwitch(r32,v)       _BFSET_(r32, 4, 4,v)
    #define   SET16clkD12_ENOFF_ctrl_ClkPllSwitch(r16,v)       _BFSET_(r16, 4, 4,v)
    #define   SET32clkD12_ENOFF_ctrl_ClkSwitch(r32,v)          _BFSET_(r32, 5, 5,v)
    #define   SET16clkD12_ENOFF_ctrl_ClkSwitch(r16,v)          _BFSET_(r16, 5, 5,v)
    #define   SET32clkD12_ENOFF_ctrl_ClkD3Switch(r32,v)        _BFSET_(r32, 6, 6,v)
    #define   SET16clkD12_ENOFF_ctrl_ClkD3Switch(r16,v)        _BFSET_(r16, 6, 6,v)
    #define   SET32clkD12_ENOFF_ctrl_ClkSel(r32,v)             _BFSET_(r32, 9, 7,v)
    #define   SET16clkD12_ENOFF_ctrl_ClkSel(r16,v)             _BFSET_(r16, 9, 7,v)
    #define     w32clkD12_ENOFF_ctrl                           {\
            UNSG32 uctrl_ClkEn                                 :  1;\
            UNSG32 uctrl_ClkPllSel                             :  3;\
            UNSG32 uctrl_ClkPllSwitch                          :  1;\
            UNSG32 uctrl_ClkSwitch                             :  1;\
            UNSG32 uctrl_ClkD3Switch                           :  1;\
            UNSG32 uctrl_ClkSel                                :  3;\
            UNSG32 RSVDx0_b10                                  : 22;\
          }
    union { UNSG32 u32clkD12_ENOFF_ctrl;
            struct w32clkD12_ENOFF_ctrl;
          };
    } SIE_clkD12_ENOFF;
    typedef union  T32clkD12_ENOFF_ctrl
          { UNSG32 u32;
            struct w32clkD12_ENOFF_ctrl;
                 } T32clkD12_ENOFF_ctrl;
    typedef union  TclkD12_ENOFF_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32clkD12_ENOFF_ctrl;
                   };
                 } TclkD12_ENOFF_ctrl;
     SIGN32 clkD12_ENOFF_drvrd(SIE_clkD12_ENOFF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 clkD12_ENOFF_drvwr(SIE_clkD12_ENOFF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void clkD12_ENOFF_reset(SIE_clkD12_ENOFF *p);
     SIGN32 clkD12_ENOFF_cmp  (SIE_clkD12_ENOFF *p, SIE_clkD12_ENOFF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define clkD12_ENOFF_check(p,pie,pfx,hLOG) clkD12_ENOFF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define clkD12_ENOFF_print(p,    pfx,hLOG) clkD12_ENOFF_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_PERIF
#define h_PERIF (){}
    #define     RA_PERIF_PHY_DBG_CTRL                          0x0000
    #define     RA_PERIF_TW_PORT_CTRL                          0x0004
    #define     RA_PERIF_SDIO_SIG_CTRL                         0x0008
    #define     RA_PERIF_SDIO1_SIG_CTRL                        0x000C
    #define     RA_PERIF_SDIO_TST_CTL                          0x0010
    #define     RA_PERIF_EMMC_TST_CTL                          0x0014
    #define     RA_PERIF_RGMII_CTL                             0x0018
    #define     RA_PERIF_RGMII_STATUS                          0x001C
    #define     RA_PERIF_RGMII_STATUS1                         0x0020
    #define     RA_PERIF_RGMII_STATUS2                         0x0024
    #define     RA_PERIF_RGMII_STATUS3                         0x0028
    #define     RA_PERIF_RGMII1_STATUS                         0x002C
    #define     RA_PERIF_RGMII1_STATUS1                        0x0030
    #define     RA_PERIF_RGMII1_STATUS2                        0x0034
    #define     RA_PERIF_RGMII1_STATUS3                        0x0038
    #define     RA_PERIF_RGMII1_CTL                            0x003C
    #define     RA_PERIF_RGMII_INDICATOR                       0x0040
    #define     RA_PERIF_RGMII1_INDICATOR                      0x0044
    #define     RA_PERIF_RGMII_TIME_SIGNAL                     0x0048
    #define     RA_PERIF_RGMII1_TIME_SIGNAL                    0x004C
    #define     RA_PERIF_MCGR_REGISTER                         0x0050
    #define     RA_PERIF_MCGR_REGISTER1                        0x0054
    #define     RA_PERIF_RGMII_PTP_TRIG                        0x0058
    #define     RA_PERIF_RGMI1_PTP_TRIG                        0x005C
    #define     RA_PERIF_DMA_CTRL_NS                           0x0060
    #define     RA_PERIF_DMA_CTRL_S                            0x0064
    #define     RA_PERIF_DMA_STAT_0_NS                         0x0068
    #define     RA_PERIF_DMA_STAT_0_S                          0x006C
    #define     RA_PERIF_DMA_STAT_1_NS                         0x0070
    #define     RA_PERIF_DMA_STAT_1_NS1                        0x0074
    #define     RA_PERIF_DMA_STAT_1_NS2                        0x0078
    #define     RA_PERIF_DMA_STAT_1_NS3                        0x007C
    #define     RA_PERIF_DMA_STAT_1_S                          0x0080
    #define     RA_PERIF_DMA_STAT_1_S1                         0x0084
    #define     RA_PERIF_DMA_STAT_1_S2                         0x0088
    #define     RA_PERIF_DMA_STAT_1_S3                         0x008C
    #define     RA_PERIF_DMA_STAT_2_NS                         0x0090
    #define     RA_PERIF_DMA_STAT_2_S                          0x0094
    #define     RA_PERIF_DMA_STAT_2_S1                         0x0098
    #define     RA_PERIF_DMA_BOOT_0_S                          0x009C
    #define     RA_PERIF_DMA_BOOT_1_S                          0x00A0
    #define     RA_PERIF_DMA_CLK_CTRL                          0x00A4
    #define     RA_PERIF_DMA_CTX_CTRL0                         0x00A8
    #define     RA_PERIF_DMA_CTX_CTRL1                         0x00AC
    #define     RA_PERIF_KPINT                                 0x00B0
    #define     RA_PERIF_RESERVED_CTRL0                        0x00BC
    typedef struct SIE_PERIF {
    #define   SET32PERIF_PHY_DBG_CTRL_perif_dbg_sel(r32,v)     _BFSET_(r32, 3, 0,v)
    #define   SET16PERIF_PHY_DBG_CTRL_perif_dbg_sel(r16,v)     _BFSET_(r16, 3, 0,v)
    #define     w32PERIF_PHY_DBG_CTRL                          {\
            UNSG32 uPHY_DBG_CTRL_perif_dbg_sel                 :  4;\
            UNSG32 RSVDx0_b4                                   : 28;\
          }
    union { UNSG32 u32PERIF_PHY_DBG_CTRL;
            struct w32PERIF_PHY_DBG_CTRL;
          };
    #define   SET32PERIF_TW_PORT_CTRL_TW0_SCL_SEL(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_TW_PORT_CTRL_TW0_SCL_SEL(r16,v)       _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_TW_PORT_CTRL_TW0_SDA_SEL(r32,v)       _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_TW_PORT_CTRL_TW0_SDA_SEL(r16,v)       _BFSET_(r16, 1, 1,v)
    #define   SET32PERIF_TW_PORT_CTRL_TW1_SCL_SEL(r32,v)       _BFSET_(r32, 2, 2,v)
    #define   SET16PERIF_TW_PORT_CTRL_TW1_SCL_SEL(r16,v)       _BFSET_(r16, 2, 2,v)
    #define   SET32PERIF_TW_PORT_CTRL_TW1_SDA_SEL(r32,v)       _BFSET_(r32, 3, 3,v)
    #define   SET16PERIF_TW_PORT_CTRL_TW1_SDA_SEL(r16,v)       _BFSET_(r16, 3, 3,v)
    #define   SET32PERIF_TW_PORT_CTRL_RGMII_MDIO_MDC_SEL(r32,v) _BFSET_(r32, 4, 4,v)
    #define   SET16PERIF_TW_PORT_CTRL_RGMII_MDIO_MDC_SEL(r16,v) _BFSET_(r16, 4, 4,v)
    #define   SET32PERIF_TW_PORT_CTRL_PTP_PPS_SEL(r32,v)       _BFSET_(r32, 5, 5,v)
    #define   SET16PERIF_TW_PORT_CTRL_PTP_PPS_SEL(r16,v)       _BFSET_(r16, 5, 5,v)
    #define     w32PERIF_TW_PORT_CTRL                          {\
            UNSG32 uTW_PORT_CTRL_TW0_SCL_SEL                   :  1;\
            UNSG32 uTW_PORT_CTRL_TW0_SDA_SEL                   :  1;\
            UNSG32 uTW_PORT_CTRL_TW1_SCL_SEL                   :  1;\
            UNSG32 uTW_PORT_CTRL_TW1_SDA_SEL                   :  1;\
            UNSG32 uTW_PORT_CTRL_RGMII_MDIO_MDC_SEL            :  1;\
            UNSG32 uTW_PORT_CTRL_PTP_PPS_SEL                   :  1;\
            UNSG32 RSVDx4_b6                                   : 26;\
          }
    union { UNSG32 u32PERIF_TW_PORT_CTRL;
            struct w32PERIF_TW_PORT_CTRL;
          };
    #define   SET32PERIF_SDIO_SIG_CTRL_SDIO_CDn_SW_CTRL(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_SDIO_SIG_CTRL_SDIO_CDn_SW_CTRL(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_SDIO_SIG_CTRL_SDIO_WP_SW_CTRL(r32,v)  _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_SDIO_SIG_CTRL_SDIO_WP_SW_CTRL(r16,v)  _BFSET_(r16, 1, 1,v)
    #define     w32PERIF_SDIO_SIG_CTRL                         {\
            UNSG32 uSDIO_SIG_CTRL_SDIO_CDn_SW_CTRL             :  1;\
            UNSG32 uSDIO_SIG_CTRL_SDIO_WP_SW_CTRL              :  1;\
            UNSG32 RSVDx8_b2                                   : 30;\
          }
    union { UNSG32 u32PERIF_SDIO_SIG_CTRL;
            struct w32PERIF_SDIO_SIG_CTRL;
          };
    #define   SET32PERIF_SDIO1_SIG_CTRL_SDIO_CDn_SW_CTRL(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_SDIO1_SIG_CTRL_SDIO_CDn_SW_CTRL(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_SDIO1_SIG_CTRL_SDIO_WP_SW_CTRL(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_SDIO1_SIG_CTRL_SDIO_WP_SW_CTRL(r16,v) _BFSET_(r16, 1, 1,v)
    #define     w32PERIF_SDIO1_SIG_CTRL                        {\
            UNSG32 uSDIO1_SIG_CTRL_SDIO_CDn_SW_CTRL            :  1;\
            UNSG32 uSDIO1_SIG_CTRL_SDIO_WP_SW_CTRL             :  1;\
            UNSG32 RSVDxC_b2                                   : 30;\
          }
    union { UNSG32 u32PERIF_SDIO1_SIG_CTRL;
            struct w32PERIF_SDIO1_SIG_CTRL;
          };
    #define   SET32PERIF_SDIO_TST_CTL_SDIO_PHY_TST_INTF_SEL(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_SDIO_TST_CTL_SDIO_PHY_TST_INTF_SEL(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_SDIO_TST_CTL_SDIO_PHY_TST_PAD_MODE(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_SDIO_TST_CTL_SDIO_PHY_TST_PAD_MODE(r16,v) _BFSET_(r16, 1, 1,v)
    #define   SET32PERIF_SDIO_TST_CTL_SDIO_PHY_TST_DLOUT_EN(r32,v) _BFSET_(r32, 2, 2,v)
    #define   SET16PERIF_SDIO_TST_CTL_SDIO_PHY_TST_DLOUT_EN(r16,v) _BFSET_(r16, 2, 2,v)
    #define     w32PERIF_SDIO_TST_CTL                          {\
            UNSG32 uSDIO_TST_CTL_SDIO_PHY_TST_INTF_SEL         :  1;\
            UNSG32 uSDIO_TST_CTL_SDIO_PHY_TST_PAD_MODE         :  1;\
            UNSG32 uSDIO_TST_CTL_SDIO_PHY_TST_DLOUT_EN         :  1;\
            UNSG32 RSVDx10_b3                                  : 29;\
          }
    union { UNSG32 u32PERIF_SDIO_TST_CTL;
            struct w32PERIF_SDIO_TST_CTL;
          };
    #define   SET32PERIF_EMMC_TST_CTL_EMMC_PHY_TST_INTF_SEL(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_EMMC_TST_CTL_EMMC_PHY_TST_INTF_SEL(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_EMMC_TST_CTL_EMMC_PHY_TST_PAD_MODE(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_EMMC_TST_CTL_EMMC_PHY_TST_PAD_MODE(r16,v) _BFSET_(r16, 1, 1,v)
    #define   SET32PERIF_EMMC_TST_CTL_EMMC_PHY_TST_DLOUT_EN(r32,v) _BFSET_(r32, 2, 2,v)
    #define   SET16PERIF_EMMC_TST_CTL_EMMC_PHY_TST_DLOUT_EN(r16,v) _BFSET_(r16, 2, 2,v)
    #define     w32PERIF_EMMC_TST_CTL                          {\
            UNSG32 uEMMC_TST_CTL_EMMC_PHY_TST_INTF_SEL         :  1;\
            UNSG32 uEMMC_TST_CTL_EMMC_PHY_TST_PAD_MODE         :  1;\
            UNSG32 uEMMC_TST_CTL_EMMC_PHY_TST_DLOUT_EN         :  1;\
            UNSG32 RSVDx14_b3                                  : 29;\
          }
    union { UNSG32 u32PERIF_EMMC_TST_CTL;
            struct w32PERIF_EMMC_TST_CTL;
          };
    #define   SET32PERIF_RGMII_CTL_TXC_90DEG_SEL(r32,v)        _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_RGMII_CTL_TXC_90DEG_SEL(r16,v)        _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_RGMII_CTL_PHY_INTF_SEL(r32,v)         _BFSET_(r32, 4, 1,v)
    #define   SET16PERIF_RGMII_CTL_PHY_INTF_SEL(r16,v)         _BFSET_(r16, 4, 1,v)
    #define     w32PERIF_RGMII_CTL                             {\
            UNSG32 uRGMII_CTL_TXC_90DEG_SEL                    :  1;\
            UNSG32 uRGMII_CTL_PHY_INTF_SEL                     :  4;\
            UNSG32 RSVDx18_b5                                  : 27;\
          }
    union { UNSG32 u32PERIF_RGMII_CTL;
            struct w32PERIF_RGMII_CTL;
          };
    #define   SET32PERIF_RGMII_STATUS_SUB_NANOSECOND_OUT(r32,v) _BFSET_(r32, 7, 0,v)
    #define   SET16PERIF_RGMII_STATUS_SUB_NANOSECOND_OUT(r16,v) _BFSET_(r16, 7, 0,v)
    #define     w32PERIF_RGMII_STATUS                          {\
            UNSG32 uRGMII_STATUS_SUB_NANOSECOND_OUT            :  8;\
            UNSG32 RSVDx1C_b8                                  : 24;\
          }
    union { UNSG32 u32PERIF_RGMII_STATUS;
            struct w32PERIF_RGMII_STATUS;
          };
    #define   SET32PERIF_RGMII_STATUS1_PTP_TIMESTAMP_LO(r32,v) _BFSET_(r32,31, 0,v)
    #define     w32PERIF_RGMII_STATUS1                         {\
            UNSG32 uRGMII_STATUS1_PTP_TIMESTAMP_LO             : 32;\
          }
    union { UNSG32 u32PERIF_RGMII_STATUS1;
            struct w32PERIF_RGMII_STATUS1;
          };
    #define   SET32PERIF_RGMII_STATUS2_PTP_TIMESTAMP_HI(r32,v) _BFSET_(r32,31, 0,v)
    #define     w32PERIF_RGMII_STATUS2                         {\
            UNSG32 uRGMII_STATUS2_PTP_TIMESTAMP_HI             : 32;\
          }
    union { UNSG32 u32PERIF_RGMII_STATUS2;
            struct w32PERIF_RGMII_STATUS2;
          };
    #define   SET32PERIF_RGMII_STATUS3_HIGHER_SECONDS_OUT(r32,v) _BFSET_(r32,15, 0,v)
    #define   SET16PERIF_RGMII_STATUS3_HIGHER_SECONDS_OUT(r16,v) _BFSET_(r16,15, 0,v)
    #define     w32PERIF_RGMII_STATUS3                         {\
            UNSG32 uRGMII_STATUS3_HIGHER_SECONDS_OUT           : 16;\
            UNSG32 RSVDx28_b16                                 : 16;\
          }
    union { UNSG32 u32PERIF_RGMII_STATUS3;
            struct w32PERIF_RGMII_STATUS3;
          };
    #define   SET32PERIF_RGMII1_STATUS_SUB_NANOSECOND_OUT(r32,v) _BFSET_(r32, 7, 0,v)
    #define   SET16PERIF_RGMII1_STATUS_SUB_NANOSECOND_OUT(r16,v) _BFSET_(r16, 7, 0,v)
    #define     w32PERIF_RGMII1_STATUS                         {\
            UNSG32 uRGMII1_STATUS_SUB_NANOSECOND_OUT           :  8;\
            UNSG32 RSVDx2C_b8                                  : 24;\
          }
    union { UNSG32 u32PERIF_RGMII1_STATUS;
            struct w32PERIF_RGMII1_STATUS;
          };
    #define   SET32PERIF_RGMII1_STATUS1_PTP_TIMESTAMP_LO(r32,v) _BFSET_(r32,31, 0,v)
    #define     w32PERIF_RGMII1_STATUS1                        {\
            UNSG32 uRGMII1_STATUS1_PTP_TIMESTAMP_LO            : 32;\
          }
    union { UNSG32 u32PERIF_RGMII1_STATUS1;
            struct w32PERIF_RGMII1_STATUS1;
          };
    #define   SET32PERIF_RGMII1_STATUS2_PTP_TIMESTAMP_HI(r32,v) _BFSET_(r32,31, 0,v)
    #define     w32PERIF_RGMII1_STATUS2                        {\
            UNSG32 uRGMII1_STATUS2_PTP_TIMESTAMP_HI            : 32;\
          }
    union { UNSG32 u32PERIF_RGMII1_STATUS2;
            struct w32PERIF_RGMII1_STATUS2;
          };
    #define   SET32PERIF_RGMII1_STATUS3_HIGHER_SECONDS_OUT(r32,v) _BFSET_(r32,15, 0,v)
    #define   SET16PERIF_RGMII1_STATUS3_HIGHER_SECONDS_OUT(r16,v) _BFSET_(r16,15, 0,v)
    #define     w32PERIF_RGMII1_STATUS3                        {\
            UNSG32 uRGMII1_STATUS3_HIGHER_SECONDS_OUT          : 16;\
            UNSG32 RSVDx38_b16                                 : 16;\
          }
    union { UNSG32 u32PERIF_RGMII1_STATUS3;
            struct w32PERIF_RGMII1_STATUS3;
          };
    #define   SET32PERIF_RGMII1_CTL_TXC_90DEG_SEL(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_RGMII1_CTL_TXC_90DEG_SEL(r16,v)       _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_RGMII1_CTL_PHY_INTF_SEL(r32,v)        _BFSET_(r32, 4, 1,v)
    #define   SET16PERIF_RGMII1_CTL_PHY_INTF_SEL(r16,v)        _BFSET_(r16, 4, 1,v)
    #define     w32PERIF_RGMII1_CTL                            {\
            UNSG32 uRGMII1_CTL_TXC_90DEG_SEL                   :  1;\
            UNSG32 uRGMII1_CTL_PHY_INTF_SEL                    :  4;\
            UNSG32 RSVDx3C_b5                                  : 27;\
          }
    union { UNSG32 u32PERIF_RGMII1_CTL;
            struct w32PERIF_RGMII1_CTL;
          };
    #define   SET32PERIF_RGMII_INDICATOR_ATI_DATA_TXON(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_RGMII_INDICATOR_ATI_DATA_TXON(r16,v)  _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_RGMII_INDICATOR_ATI_PMAC_TXON(r32,v)  _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_RGMII_INDICATOR_ATI_PMAC_TXON(r16,v)  _BFSET_(r16, 1, 1,v)
    #define   SET32PERIF_RGMII_INDICATOR_ATI_PMAC_ACTV(r32,v)  _BFSET_(r32, 2, 2,v)
    #define   SET16PERIF_RGMII_INDICATOR_ATI_PMAC_ACTV(r16,v)  _BFSET_(r16, 2, 2,v)
    #define   SET32PERIF_RGMII_INDICATOR_MCGR_DMA_REQ(r32,v)   _BFSET_(r32, 3, 3,v)
    #define   SET16PERIF_RGMII_INDICATOR_MCGR_DMA_REQ(r16,v)   _BFSET_(r16, 3, 3,v)
    #define     w32PERIF_RGMII_INDICATOR                       {\
            UNSG32 uRGMII_INDICATOR_ATI_DATA_TXON              :  1;\
            UNSG32 uRGMII_INDICATOR_ATI_PMAC_TXON              :  1;\
            UNSG32 uRGMII_INDICATOR_ATI_PMAC_ACTV              :  1;\
            UNSG32 uRGMII_INDICATOR_MCGR_DMA_REQ               :  1;\
            UNSG32 RSVDx40_b4                                  : 28;\
          }
    union { UNSG32 u32PERIF_RGMII_INDICATOR;
            struct w32PERIF_RGMII_INDICATOR;
          };
    #define   SET32PERIF_RGMII1_INDICATOR_ATI_DATA_TXON(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_RGMII1_INDICATOR_ATI_DATA_TXON(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_RGMII1_INDICATOR_ATI_PMAC_TXON(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_RGMII1_INDICATOR_ATI_PMAC_TXON(r16,v) _BFSET_(r16, 1, 1,v)
    #define   SET32PERIF_RGMII1_INDICATOR_ATI_PMAC_ACTV(r32,v) _BFSET_(r32, 2, 2,v)
    #define   SET16PERIF_RGMII1_INDICATOR_ATI_PMAC_ACTV(r16,v) _BFSET_(r16, 2, 2,v)
    #define   SET32PERIF_RGMII1_INDICATOR_MCGR_DMA_REQ(r32,v)  _BFSET_(r32, 3, 3,v)
    #define   SET16PERIF_RGMII1_INDICATOR_MCGR_DMA_REQ(r16,v)  _BFSET_(r16, 3, 3,v)
    #define     w32PERIF_RGMII1_INDICATOR                      {\
            UNSG32 uRGMII1_INDICATOR_ATI_DATA_TXON             :  1;\
            UNSG32 uRGMII1_INDICATOR_ATI_PMAC_TXON             :  1;\
            UNSG32 uRGMII1_INDICATOR_ATI_PMAC_ACTV             :  1;\
            UNSG32 uRGMII1_INDICATOR_MCGR_DMA_REQ              :  1;\
            UNSG32 RSVDx44_b4                                  : 28;\
          }
    union { UNSG32 u32PERIF_RGMII1_INDICATOR;
            struct w32PERIF_RGMII1_INDICATOR;
          };
    #define   SET32PERIF_RGMII_TIME_SIGNAL_SBD_PC_TIM_WDW(r32,v) _BFSET_(r32, 3, 0,v)
    #define   SET16PERIF_RGMII_TIME_SIGNAL_SBD_PC_TIM_WDW(r16,v) _BFSET_(r16, 3, 0,v)
    #define   SET32PERIF_RGMII_TIME_SIGNAL_SBD_PMAC_HOLD_I(r32,v) _BFSET_(r32, 4, 4,v)
    #define   SET16PERIF_RGMII_TIME_SIGNAL_SBD_PMAC_HOLD_I(r16,v) _BFSET_(r16, 4, 4,v)
    #define     w32PERIF_RGMII_TIME_SIGNAL                     {\
            UNSG32 uRGMII_TIME_SIGNAL_SBD_PC_TIM_WDW           :  4;\
            UNSG32 uRGMII_TIME_SIGNAL_SBD_PMAC_HOLD_I          :  1;\
            UNSG32 RSVDx48_b5                                  : 27;\
          }
    union { UNSG32 u32PERIF_RGMII_TIME_SIGNAL;
            struct w32PERIF_RGMII_TIME_SIGNAL;
          };
    #define   SET32PERIF_RGMII1_TIME_SIGNAL_SBD_PC_TIM_WDW(r32,v) _BFSET_(r32, 3, 0,v)
    #define   SET16PERIF_RGMII1_TIME_SIGNAL_SBD_PC_TIM_WDW(r16,v) _BFSET_(r16, 3, 0,v)
    #define   SET32PERIF_RGMII1_TIME_SIGNAL_SBD_PMAC_HOLD_I(r32,v) _BFSET_(r32, 4, 4,v)
    #define   SET16PERIF_RGMII1_TIME_SIGNAL_SBD_PMAC_HOLD_I(r16,v) _BFSET_(r16, 4, 4,v)
    #define     w32PERIF_RGMII1_TIME_SIGNAL                    {\
            UNSG32 uRGMII1_TIME_SIGNAL_SBD_PC_TIM_WDW          :  4;\
            UNSG32 uRGMII1_TIME_SIGNAL_SBD_PMAC_HOLD_I         :  1;\
            UNSG32 RSVDx4C_b5                                  : 27;\
          }
    union { UNSG32 u32PERIF_RGMII1_TIME_SIGNAL;
            struct w32PERIF_RGMII1_TIME_SIGNAL;
          };
    #define   SET32PERIF_MCGR_REGISTER_MCG_PST_TRIG(r32,v)     _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_MCGR_REGISTER_MCG_PST_TRIG(r16,v)     _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_MCGR_REGISTER_MCGR_DMA_ACK(r32,v)     _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_MCGR_REGISTER_MCGR_DMA_ACK(r16,v)     _BFSET_(r16, 1, 1,v)
    #define     w32PERIF_MCGR_REGISTER                         {\
            UNSG32 uMCGR_REGISTER_MCG_PST_TRIG                 :  1;\
            UNSG32 uMCGR_REGISTER_MCGR_DMA_ACK                 :  1;\
            UNSG32 RSVDx50_b2                                  : 30;\
          }
    union { UNSG32 u32PERIF_MCGR_REGISTER;
            struct w32PERIF_MCGR_REGISTER;
          };
    #define   SET32PERIF_MCGR_REGISTER1_MCG_PST_TRIG(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_MCGR_REGISTER1_MCG_PST_TRIG(r16,v)    _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_MCGR_REGISTER1_MCGR_DMA_ACK(r32,v)    _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_MCGR_REGISTER1_MCGR_DMA_ACK(r16,v)    _BFSET_(r16, 1, 1,v)
    #define     w32PERIF_MCGR_REGISTER1                        {\
            UNSG32 uMCGR_REGISTER1_MCG_PST_TRIG                :  1;\
            UNSG32 uMCGR_REGISTER1_MCGR_DMA_ACK                :  1;\
            UNSG32 RSVDx54_b2                                  : 30;\
          }
    union { UNSG32 u32PERIF_MCGR_REGISTER1;
            struct w32PERIF_MCGR_REGISTER1;
          };
    #define   SET32PERIF_RGMII_PTP_TRIG_ptp_ts_trigger_select(r32,v) _BFSET_(r32, 5, 0,v)
    #define   SET16PERIF_RGMII_PTP_TRIG_ptp_ts_trigger_select(r16,v) _BFSET_(r16, 5, 0,v)
    #define     w32PERIF_RGMII_PTP_TRIG                        {\
            UNSG32 uRGMII_PTP_TRIG_ptp_ts_trigger_select       :  6;\
            UNSG32 RSVDx58_b6                                  : 26;\
          }
    union { UNSG32 u32PERIF_RGMII_PTP_TRIG;
            struct w32PERIF_RGMII_PTP_TRIG;
          };
    #define   SET32PERIF_RGMI1_PTP_TRIG_ptp_ts_trigger_select(r32,v) _BFSET_(r32, 5, 0,v)
    #define   SET16PERIF_RGMI1_PTP_TRIG_ptp_ts_trigger_select(r16,v) _BFSET_(r16, 5, 0,v)
    #define     w32PERIF_RGMI1_PTP_TRIG                        {\
            UNSG32 uRGMI1_PTP_TRIG_ptp_ts_trigger_select       :  6;\
            UNSG32 RSVDx5C_b6                                  : 26;\
          }
    union { UNSG32 u32PERIF_RGMI1_PTP_TRIG;
            struct w32PERIF_RGMI1_PTP_TRIG;
          };
    #define   SET32PERIF_DMA_CTRL_NS_allch_stop_req_nonsec(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_DMA_CTRL_NS_allch_stop_req_nonsec(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_DMA_CTRL_NS_allch_pause_req_nonsec(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_DMA_CTRL_NS_allch_pause_req_nonsec(r16,v) _BFSET_(r16, 1, 1,v)
    #define     w32PERIF_DMA_CTRL_NS                           {\
            UNSG32 uDMA_CTRL_NS_allch_stop_req_nonsec          :  1;\
            UNSG32 uDMA_CTRL_NS_allch_pause_req_nonsec         :  1;\
            UNSG32 RSVDx60_b2                                  : 30;\
          }
    union { UNSG32 u32PERIF_DMA_CTRL_NS;
            struct w32PERIF_DMA_CTRL_NS;
          };
    #define   SET32PERIF_DMA_CTRL_S_allch_stop_req_sec(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_DMA_CTRL_S_allch_stop_req_sec(r16,v)  _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_DMA_CTRL_S_allch_pause_req_sec(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_DMA_CTRL_S_allch_pause_req_sec(r16,v) _BFSET_(r16, 1, 1,v)
    #define     w32PERIF_DMA_CTRL_S                            {\
            UNSG32 uDMA_CTRL_S_allch_stop_req_sec              :  1;\
            UNSG32 uDMA_CTRL_S_allch_pause_req_sec             :  1;\
            UNSG32 RSVDx64_b2                                  : 30;\
          }
    union { UNSG32 u32PERIF_DMA_CTRL_S;
            struct w32PERIF_DMA_CTRL_S;
          };
    #define   SET32PERIF_DMA_STAT_0_NS_allch_stop_ack_nonsec(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_DMA_STAT_0_NS_allch_stop_ack_nonsec(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_DMA_STAT_0_NS_allch_pause_ack_nonsec(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_DMA_STAT_0_NS_allch_pause_ack_nonsec(r16,v) _BFSET_(r16, 1, 1,v)
    #define     w32PERIF_DMA_STAT_0_NS                         {\
            UNSG32 uDMA_STAT_0_NS_allch_stop_ack_nonsec        :  1;\
            UNSG32 uDMA_STAT_0_NS_allch_pause_ack_nonsec       :  1;\
            UNSG32 RSVDx68_b2                                  : 30;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_0_NS;
            struct w32PERIF_DMA_STAT_0_NS;
          };
    #define   SET32PERIF_DMA_STAT_0_S_allch_stop_ack_sec(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_DMA_STAT_0_S_allch_stop_ack_sec(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_DMA_STAT_0_S_allch_pause_ack_sec(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16PERIF_DMA_STAT_0_S_allch_pause_ack_sec(r16,v) _BFSET_(r16, 1, 1,v)
    #define     w32PERIF_DMA_STAT_0_S                          {\
            UNSG32 uDMA_STAT_0_S_allch_stop_ack_sec            :  1;\
            UNSG32 uDMA_STAT_0_S_allch_pause_ack_sec           :  1;\
            UNSG32 RSVDx6C_b2                                  : 30;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_0_S;
            struct w32PERIF_DMA_STAT_0_S;
          };
    #define   SET32PERIF_DMA_STAT_1_NS_ch_enabled_nonsec(r32,v) _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_1_NS                         {\
            UNSG32 uDMA_STAT_1_NS_ch_enabled_nonsec            : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_1_NS;
            struct w32PERIF_DMA_STAT_1_NS;
          };
    #define   SET32PERIF_DMA_STAT_1_NS1_ch_err_nonsec(r32,v)   _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_1_NS1                        {\
            UNSG32 uDMA_STAT_1_NS1_ch_err_nonsec               : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_1_NS1;
            struct w32PERIF_DMA_STAT_1_NS1;
          };
    #define   SET32PERIF_DMA_STAT_1_NS2_ch_stopped_nonsec(r32,v) _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_1_NS2                        {\
            UNSG32 uDMA_STAT_1_NS2_ch_stopped_nonsec           : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_1_NS2;
            struct w32PERIF_DMA_STAT_1_NS2;
          };
    #define   SET32PERIF_DMA_STAT_1_NS3_ch_paused_nonsec(r32,v) _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_1_NS3                        {\
            UNSG32 uDMA_STAT_1_NS3_ch_paused_nonsec            : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_1_NS3;
            struct w32PERIF_DMA_STAT_1_NS3;
          };
    #define   SET32PERIF_DMA_STAT_1_S_ch_enabled_sec(r32,v)    _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_1_S                          {\
            UNSG32 uDMA_STAT_1_S_ch_enabled_sec                : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_1_S;
            struct w32PERIF_DMA_STAT_1_S;
          };
    #define   SET32PERIF_DMA_STAT_1_S1_ch_err_sec(r32,v)       _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_1_S1                         {\
            UNSG32 uDMA_STAT_1_S1_ch_err_sec                   : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_1_S1;
            struct w32PERIF_DMA_STAT_1_S1;
          };
    #define   SET32PERIF_DMA_STAT_1_S2_ch_stopped_sec(r32,v)   _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_1_S2                         {\
            UNSG32 uDMA_STAT_1_S2_ch_stopped_sec               : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_1_S2;
            struct w32PERIF_DMA_STAT_1_S2;
          };
    #define   SET32PERIF_DMA_STAT_1_S3_ch_paused_sec(r32,v)    _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_1_S3                         {\
            UNSG32 uDMA_STAT_1_S3_ch_paused_sec                : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_1_S3;
            struct w32PERIF_DMA_STAT_1_S3;
          };
    #define   SET32PERIF_DMA_STAT_2_NS_ch_priv_nonsec(r32,v)   _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_2_NS                         {\
            UNSG32 uDMA_STAT_2_NS_ch_priv_nonsec               : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_2_NS;
            struct w32PERIF_DMA_STAT_2_NS;
          };
    #define   SET32PERIF_DMA_STAT_2_S_ch_priv_sec(r32,v)       _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_2_S                          {\
            UNSG32 uDMA_STAT_2_S_ch_priv_sec                   : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_2_S;
            struct w32PERIF_DMA_STAT_2_S;
          };
    #define   SET32PERIF_DMA_STAT_2_S1_ch_nonsec(r32,v)        _BFSET_(r32,31, 0,v)
    #define     w32PERIF_DMA_STAT_2_S1                         {\
            UNSG32 uDMA_STAT_2_S1_ch_nonsec                    : 32;\
          }
    union { UNSG32 u32PERIF_DMA_STAT_2_S1;
            struct w32PERIF_DMA_STAT_2_S1;
          };
    #define   SET32PERIF_DMA_BOOT_0_S_boot_en(r32,v)           _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_DMA_BOOT_0_S_boot_en(r16,v)           _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_DMA_BOOT_0_S_boot_memattr(r32,v)      _BFSET_(r32, 8, 1,v)
    #define   SET16PERIF_DMA_BOOT_0_S_boot_memattr(r16,v)      _BFSET_(r16, 8, 1,v)
    #define   SET32PERIF_DMA_BOOT_0_S_boot_shareattr(r32,v)    _BFSET_(r32,10, 9,v)
    #define   SET16PERIF_DMA_BOOT_0_S_boot_shareattr(r16,v)    _BFSET_(r16,10, 9,v)
    #define   SET32PERIF_DMA_BOOT_0_S_boot_priv(r32,v)         _BFSET_(r32,11,11,v)
    #define   SET16PERIF_DMA_BOOT_0_S_boot_priv(r16,v)         _BFSET_(r16,11,11,v)
    #define     w32PERIF_DMA_BOOT_0_S                          {\
            UNSG32 uDMA_BOOT_0_S_boot_en                       :  1;\
            UNSG32 uDMA_BOOT_0_S_boot_memattr                  :  8;\
            UNSG32 uDMA_BOOT_0_S_boot_shareattr                :  2;\
            UNSG32 uDMA_BOOT_0_S_boot_priv                     :  1;\
            UNSG32 RSVDx9C_b12                                 : 20;\
          }
    union { UNSG32 u32PERIF_DMA_BOOT_0_S;
            struct w32PERIF_DMA_BOOT_0_S;
          };
    #define   SET32PERIF_DMA_BOOT_1_S_boot_addr(r32,v)         _BFSET_(r32,29, 0,v)
    #define     w32PERIF_DMA_BOOT_1_S                          {\
            UNSG32 uDMA_BOOT_1_S_boot_addr                     : 30;\
            UNSG32 RSVDxA0_b30                                 :  2;\
          }
    union { UNSG32 u32PERIF_DMA_BOOT_1_S;
            struct w32PERIF_DMA_BOOT_1_S;
          };
    #define   SET32PERIF_DMA_CLK_CTRL_clk_force(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_DMA_CLK_CTRL_clk_force(r16,v)         _BFSET_(r16, 0, 0,v)
    #define   SET32PERIF_DMA_CLK_CTRL_entry_delay(r32,v)       _BFSET_(r32, 8, 1,v)
    #define   SET16PERIF_DMA_CLK_CTRL_entry_delay(r16,v)       _BFSET_(r16, 8, 1,v)
    #define     w32PERIF_DMA_CLK_CTRL                          {\
            UNSG32 uDMA_CLK_CTRL_clk_force                     :  1;\
            UNSG32 uDMA_CLK_CTRL_entry_delay                   :  8;\
            UNSG32 RSVDxA4_b9                                  : 23;\
          }
    union { UNSG32 u32PERIF_DMA_CLK_CTRL;
            struct w32PERIF_DMA_CLK_CTRL;
          };
    #define   SET32PERIF_DMA_CTX_CTRL0_initial_cntxbase(r32,v) _BFSET_(r32,19, 0,v)
    #define     w32PERIF_DMA_CTX_CTRL0                         {\
            UNSG32 uDMA_CTX_CTRL0_initial_cntxbase             : 20;\
            UNSG32 RSVDxA8_b20                                 : 12;\
          }
    union { UNSG32 u32PERIF_DMA_CTX_CTRL0;
            struct w32PERIF_DMA_CTX_CTRL0;
          };
    #define   SET32PERIF_DMA_CTX_CTRL1_initial_cntxmem_clr(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16PERIF_DMA_CTX_CTRL1_initial_cntxmem_clr(r16,v) _BFSET_(r16, 0, 0,v)
    #define     w32PERIF_DMA_CTX_CTRL1                         {\
            UNSG32 uDMA_CTX_CTRL1_initial_cntxmem_clr          :  1;\
            UNSG32 RSVDxAC_b1                                  : 31;\
          }
    union { UNSG32 u32PERIF_DMA_CTX_CTRL1;
            struct w32PERIF_DMA_CTX_CTRL1;
          };
              SIE_KPINT                                        ie_KPINT;
    #define   SET32PERIF_RESERVED_CTRL0_ctrl(r32,v)            _BFSET_(r32,31, 0,v)
    #define     w32PERIF_RESERVED_CTRL0                        {\
            UNSG32 uRESERVED_CTRL0_ctrl                        : 32;\
          }
    union { UNSG32 u32PERIF_RESERVED_CTRL0;
            struct w32PERIF_RESERVED_CTRL0;
          };
    } SIE_PERIF;
    typedef union  T32PERIF_PHY_DBG_CTRL
          { UNSG32 u32;
            struct w32PERIF_PHY_DBG_CTRL;
                 } T32PERIF_PHY_DBG_CTRL;
    typedef union  T32PERIF_TW_PORT_CTRL
          { UNSG32 u32;
            struct w32PERIF_TW_PORT_CTRL;
                 } T32PERIF_TW_PORT_CTRL;
    typedef union  T32PERIF_SDIO_SIG_CTRL
          { UNSG32 u32;
            struct w32PERIF_SDIO_SIG_CTRL;
                 } T32PERIF_SDIO_SIG_CTRL;
    typedef union  T32PERIF_SDIO1_SIG_CTRL
          { UNSG32 u32;
            struct w32PERIF_SDIO1_SIG_CTRL;
                 } T32PERIF_SDIO1_SIG_CTRL;
    typedef union  T32PERIF_SDIO_TST_CTL
          { UNSG32 u32;
            struct w32PERIF_SDIO_TST_CTL;
                 } T32PERIF_SDIO_TST_CTL;
    typedef union  T32PERIF_EMMC_TST_CTL
          { UNSG32 u32;
            struct w32PERIF_EMMC_TST_CTL;
                 } T32PERIF_EMMC_TST_CTL;
    typedef union  T32PERIF_RGMII_CTL
          { UNSG32 u32;
            struct w32PERIF_RGMII_CTL;
                 } T32PERIF_RGMII_CTL;
    typedef union  T32PERIF_RGMII_STATUS
          { UNSG32 u32;
            struct w32PERIF_RGMII_STATUS;
                 } T32PERIF_RGMII_STATUS;
    typedef union  T32PERIF_RGMII_STATUS1
          { UNSG32 u32;
            struct w32PERIF_RGMII_STATUS1;
                 } T32PERIF_RGMII_STATUS1;
    typedef union  T32PERIF_RGMII_STATUS2
          { UNSG32 u32;
            struct w32PERIF_RGMII_STATUS2;
                 } T32PERIF_RGMII_STATUS2;
    typedef union  T32PERIF_RGMII_STATUS3
          { UNSG32 u32;
            struct w32PERIF_RGMII_STATUS3;
                 } T32PERIF_RGMII_STATUS3;
    typedef union  T32PERIF_RGMII1_STATUS
          { UNSG32 u32;
            struct w32PERIF_RGMII1_STATUS;
                 } T32PERIF_RGMII1_STATUS;
    typedef union  T32PERIF_RGMII1_STATUS1
          { UNSG32 u32;
            struct w32PERIF_RGMII1_STATUS1;
                 } T32PERIF_RGMII1_STATUS1;
    typedef union  T32PERIF_RGMII1_STATUS2
          { UNSG32 u32;
            struct w32PERIF_RGMII1_STATUS2;
                 } T32PERIF_RGMII1_STATUS2;
    typedef union  T32PERIF_RGMII1_STATUS3
          { UNSG32 u32;
            struct w32PERIF_RGMII1_STATUS3;
                 } T32PERIF_RGMII1_STATUS3;
    typedef union  T32PERIF_RGMII1_CTL
          { UNSG32 u32;
            struct w32PERIF_RGMII1_CTL;
                 } T32PERIF_RGMII1_CTL;
    typedef union  T32PERIF_RGMII_INDICATOR
          { UNSG32 u32;
            struct w32PERIF_RGMII_INDICATOR;
                 } T32PERIF_RGMII_INDICATOR;
    typedef union  T32PERIF_RGMII1_INDICATOR
          { UNSG32 u32;
            struct w32PERIF_RGMII1_INDICATOR;
                 } T32PERIF_RGMII1_INDICATOR;
    typedef union  T32PERIF_RGMII_TIME_SIGNAL
          { UNSG32 u32;
            struct w32PERIF_RGMII_TIME_SIGNAL;
                 } T32PERIF_RGMII_TIME_SIGNAL;
    typedef union  T32PERIF_RGMII1_TIME_SIGNAL
          { UNSG32 u32;
            struct w32PERIF_RGMII1_TIME_SIGNAL;
                 } T32PERIF_RGMII1_TIME_SIGNAL;
    typedef union  T32PERIF_MCGR_REGISTER
          { UNSG32 u32;
            struct w32PERIF_MCGR_REGISTER;
                 } T32PERIF_MCGR_REGISTER;
    typedef union  T32PERIF_MCGR_REGISTER1
          { UNSG32 u32;
            struct w32PERIF_MCGR_REGISTER1;
                 } T32PERIF_MCGR_REGISTER1;
    typedef union  T32PERIF_RGMII_PTP_TRIG
          { UNSG32 u32;
            struct w32PERIF_RGMII_PTP_TRIG;
                 } T32PERIF_RGMII_PTP_TRIG;
    typedef union  T32PERIF_RGMI1_PTP_TRIG
          { UNSG32 u32;
            struct w32PERIF_RGMI1_PTP_TRIG;
                 } T32PERIF_RGMI1_PTP_TRIG;
    typedef union  T32PERIF_DMA_CTRL_NS
          { UNSG32 u32;
            struct w32PERIF_DMA_CTRL_NS;
                 } T32PERIF_DMA_CTRL_NS;
    typedef union  T32PERIF_DMA_CTRL_S
          { UNSG32 u32;
            struct w32PERIF_DMA_CTRL_S;
                 } T32PERIF_DMA_CTRL_S;
    typedef union  T32PERIF_DMA_STAT_0_NS
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_0_NS;
                 } T32PERIF_DMA_STAT_0_NS;
    typedef union  T32PERIF_DMA_STAT_0_S
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_0_S;
                 } T32PERIF_DMA_STAT_0_S;
    typedef union  T32PERIF_DMA_STAT_1_NS
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_1_NS;
                 } T32PERIF_DMA_STAT_1_NS;
    typedef union  T32PERIF_DMA_STAT_1_NS1
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_1_NS1;
                 } T32PERIF_DMA_STAT_1_NS1;
    typedef union  T32PERIF_DMA_STAT_1_NS2
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_1_NS2;
                 } T32PERIF_DMA_STAT_1_NS2;
    typedef union  T32PERIF_DMA_STAT_1_NS3
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_1_NS3;
                 } T32PERIF_DMA_STAT_1_NS3;
    typedef union  T32PERIF_DMA_STAT_1_S
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_1_S;
                 } T32PERIF_DMA_STAT_1_S;
    typedef union  T32PERIF_DMA_STAT_1_S1
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_1_S1;
                 } T32PERIF_DMA_STAT_1_S1;
    typedef union  T32PERIF_DMA_STAT_1_S2
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_1_S2;
                 } T32PERIF_DMA_STAT_1_S2;
    typedef union  T32PERIF_DMA_STAT_1_S3
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_1_S3;
                 } T32PERIF_DMA_STAT_1_S3;
    typedef union  T32PERIF_DMA_STAT_2_NS
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_2_NS;
                 } T32PERIF_DMA_STAT_2_NS;
    typedef union  T32PERIF_DMA_STAT_2_S
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_2_S;
                 } T32PERIF_DMA_STAT_2_S;
    typedef union  T32PERIF_DMA_STAT_2_S1
          { UNSG32 u32;
            struct w32PERIF_DMA_STAT_2_S1;
                 } T32PERIF_DMA_STAT_2_S1;
    typedef union  T32PERIF_DMA_BOOT_0_S
          { UNSG32 u32;
            struct w32PERIF_DMA_BOOT_0_S;
                 } T32PERIF_DMA_BOOT_0_S;
    typedef union  T32PERIF_DMA_BOOT_1_S
          { UNSG32 u32;
            struct w32PERIF_DMA_BOOT_1_S;
                 } T32PERIF_DMA_BOOT_1_S;
    typedef union  T32PERIF_DMA_CLK_CTRL
          { UNSG32 u32;
            struct w32PERIF_DMA_CLK_CTRL;
                 } T32PERIF_DMA_CLK_CTRL;
    typedef union  T32PERIF_DMA_CTX_CTRL0
          { UNSG32 u32;
            struct w32PERIF_DMA_CTX_CTRL0;
                 } T32PERIF_DMA_CTX_CTRL0;
    typedef union  T32PERIF_DMA_CTX_CTRL1
          { UNSG32 u32;
            struct w32PERIF_DMA_CTX_CTRL1;
                 } T32PERIF_DMA_CTX_CTRL1;
    typedef union  T32PERIF_RESERVED_CTRL0
          { UNSG32 u32;
            struct w32PERIF_RESERVED_CTRL0;
                 } T32PERIF_RESERVED_CTRL0;
    typedef union  TPERIF_PHY_DBG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_PHY_DBG_CTRL;
                   };
                 } TPERIF_PHY_DBG_CTRL;
    typedef union  TPERIF_TW_PORT_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_TW_PORT_CTRL;
                   };
                 } TPERIF_TW_PORT_CTRL;
    typedef union  TPERIF_SDIO_SIG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_SDIO_SIG_CTRL;
                   };
                 } TPERIF_SDIO_SIG_CTRL;
    typedef union  TPERIF_SDIO1_SIG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_SDIO1_SIG_CTRL;
                   };
                 } TPERIF_SDIO1_SIG_CTRL;
    typedef union  TPERIF_SDIO_TST_CTL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_SDIO_TST_CTL;
                   };
                 } TPERIF_SDIO_TST_CTL;
    typedef union  TPERIF_EMMC_TST_CTL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_EMMC_TST_CTL;
                   };
                 } TPERIF_EMMC_TST_CTL;
    typedef union  TPERIF_RGMII_CTL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII_CTL;
                   };
                 } TPERIF_RGMII_CTL;
    typedef union  TPERIF_RGMII_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII_STATUS;
                   };
                 } TPERIF_RGMII_STATUS;
    typedef union  TPERIF_RGMII_STATUS1
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII_STATUS1;
                   };
                 } TPERIF_RGMII_STATUS1;
    typedef union  TPERIF_RGMII_STATUS2
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII_STATUS2;
                   };
                 } TPERIF_RGMII_STATUS2;
    typedef union  TPERIF_RGMII_STATUS3
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII_STATUS3;
                   };
                 } TPERIF_RGMII_STATUS3;
    typedef union  TPERIF_RGMII1_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII1_STATUS;
                   };
                 } TPERIF_RGMII1_STATUS;
    typedef union  TPERIF_RGMII1_STATUS1
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII1_STATUS1;
                   };
                 } TPERIF_RGMII1_STATUS1;
    typedef union  TPERIF_RGMII1_STATUS2
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII1_STATUS2;
                   };
                 } TPERIF_RGMII1_STATUS2;
    typedef union  TPERIF_RGMII1_STATUS3
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII1_STATUS3;
                   };
                 } TPERIF_RGMII1_STATUS3;
    typedef union  TPERIF_RGMII1_CTL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII1_CTL;
                   };
                 } TPERIF_RGMII1_CTL;
    typedef union  TPERIF_RGMII_INDICATOR
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII_INDICATOR;
                   };
                 } TPERIF_RGMII_INDICATOR;
    typedef union  TPERIF_RGMII1_INDICATOR
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII1_INDICATOR;
                   };
                 } TPERIF_RGMII1_INDICATOR;
    typedef union  TPERIF_RGMII_TIME_SIGNAL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII_TIME_SIGNAL;
                   };
                 } TPERIF_RGMII_TIME_SIGNAL;
    typedef union  TPERIF_RGMII1_TIME_SIGNAL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII1_TIME_SIGNAL;
                   };
                 } TPERIF_RGMII1_TIME_SIGNAL;
    typedef union  TPERIF_MCGR_REGISTER
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_MCGR_REGISTER;
                   };
                 } TPERIF_MCGR_REGISTER;
    typedef union  TPERIF_MCGR_REGISTER1
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_MCGR_REGISTER1;
                   };
                 } TPERIF_MCGR_REGISTER1;
    typedef union  TPERIF_RGMII_PTP_TRIG
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMII_PTP_TRIG;
                   };
                 } TPERIF_RGMII_PTP_TRIG;
    typedef union  TPERIF_RGMI1_PTP_TRIG
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RGMI1_PTP_TRIG;
                   };
                 } TPERIF_RGMI1_PTP_TRIG;
    typedef union  TPERIF_DMA_CTRL_NS
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_CTRL_NS;
                   };
                 } TPERIF_DMA_CTRL_NS;
    typedef union  TPERIF_DMA_CTRL_S
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_CTRL_S;
                   };
                 } TPERIF_DMA_CTRL_S;
    typedef union  TPERIF_DMA_STAT_0_NS
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_0_NS;
                   };
                 } TPERIF_DMA_STAT_0_NS;
    typedef union  TPERIF_DMA_STAT_0_S
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_0_S;
                   };
                 } TPERIF_DMA_STAT_0_S;
    typedef union  TPERIF_DMA_STAT_1_NS
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_1_NS;
                   };
                 } TPERIF_DMA_STAT_1_NS;
    typedef union  TPERIF_DMA_STAT_1_NS1
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_1_NS1;
                   };
                 } TPERIF_DMA_STAT_1_NS1;
    typedef union  TPERIF_DMA_STAT_1_NS2
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_1_NS2;
                   };
                 } TPERIF_DMA_STAT_1_NS2;
    typedef union  TPERIF_DMA_STAT_1_NS3
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_1_NS3;
                   };
                 } TPERIF_DMA_STAT_1_NS3;
    typedef union  TPERIF_DMA_STAT_1_S
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_1_S;
                   };
                 } TPERIF_DMA_STAT_1_S;
    typedef union  TPERIF_DMA_STAT_1_S1
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_1_S1;
                   };
                 } TPERIF_DMA_STAT_1_S1;
    typedef union  TPERIF_DMA_STAT_1_S2
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_1_S2;
                   };
                 } TPERIF_DMA_STAT_1_S2;
    typedef union  TPERIF_DMA_STAT_1_S3
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_1_S3;
                   };
                 } TPERIF_DMA_STAT_1_S3;
    typedef union  TPERIF_DMA_STAT_2_NS
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_2_NS;
                   };
                 } TPERIF_DMA_STAT_2_NS;
    typedef union  TPERIF_DMA_STAT_2_S
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_2_S;
                   };
                 } TPERIF_DMA_STAT_2_S;
    typedef union  TPERIF_DMA_STAT_2_S1
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_STAT_2_S1;
                   };
                 } TPERIF_DMA_STAT_2_S1;
    typedef union  TPERIF_DMA_BOOT_0_S
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_BOOT_0_S;
                   };
                 } TPERIF_DMA_BOOT_0_S;
    typedef union  TPERIF_DMA_BOOT_1_S
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_BOOT_1_S;
                   };
                 } TPERIF_DMA_BOOT_1_S;
    typedef union  TPERIF_DMA_CLK_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_CLK_CTRL;
                   };
                 } TPERIF_DMA_CLK_CTRL;
    typedef union  TPERIF_DMA_CTX_CTRL0
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_CTX_CTRL0;
                   };
                 } TPERIF_DMA_CTX_CTRL0;
    typedef union  TPERIF_DMA_CTX_CTRL1
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_DMA_CTX_CTRL1;
                   };
                 } TPERIF_DMA_CTX_CTRL1;
    typedef union  TPERIF_RESERVED_CTRL0
          { UNSG32 u32[1];
            struct {
            struct w32PERIF_RESERVED_CTRL0;
                   };
                 } TPERIF_RESERVED_CTRL0;
     SIGN32 PERIF_drvrd(SIE_PERIF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 PERIF_drvwr(SIE_PERIF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void PERIF_reset(SIE_PERIF *p);
     SIGN32 PERIF_cmp  (SIE_PERIF *p, SIE_PERIF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define PERIF_check(p,pie,pfx,hLOG) PERIF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define PERIF_print(p,    pfx,hLOG) PERIF_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_PLL_COUNTER
#define h_PLL_COUNTER (){}
    #define     RA_PLL_COUNTER_CTRL                            0x0000
    #define     RA_PLL_COUNTER_STATUS                          0x0004
    #define     RA_PLL_COUNTER_RESULT                          0x0008
    typedef struct SIE_PLL_COUNTER {
    #define   SET32PLL_COUNTER_CTRL_REF_COUNT_MAX(r32,v)       _BFSET_(r32,15, 0,v)
    #define   SET16PLL_COUNTER_CTRL_REF_COUNT_MAX(r16,v)       _BFSET_(r16,15, 0,v)
    #define   SET32PLL_COUNTER_CTRL_START(r32,v)               _BFSET_(r32,16,16,v)
    #define   SET16PLL_COUNTER_CTRL_START(r16,v)               _BFSET_(r16, 0, 0,v)
    #define   SET32PLL_COUNTER_CTRL_CLK_SEL(r32,v)             _BFSET_(r32,19,17,v)
    #define   SET16PLL_COUNTER_CTRL_CLK_SEL(r16,v)             _BFSET_(r16, 3, 1,v)
    #define     w32PLL_COUNTER_CTRL                            {\
            UNSG32 uCTRL_REF_COUNT_MAX                         : 16;\
            UNSG32 uCTRL_START                                 :  1;\
            UNSG32 uCTRL_CLK_SEL                               :  3;\
            UNSG32 RSVDx0_b20                                  : 12;\
          }
    union { UNSG32 u32PLL_COUNTER_CTRL;
            struct w32PLL_COUNTER_CTRL;
          };
    #define   SET32PLL_COUNTER_STATUS_DONE(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   SET16PLL_COUNTER_STATUS_DONE(r16,v)              _BFSET_(r16, 0, 0,v)
    #define     w32PLL_COUNTER_STATUS                          {\
            UNSG32 uSTATUS_DONE                                :  1;\
            UNSG32 RSVDx4_b1                                   : 31;\
          }
    union { UNSG32 u32PLL_COUNTER_STATUS;
            struct w32PLL_COUNTER_STATUS;
          };
    #define   SET32PLL_COUNTER_RESULT_PLL_COUNT(r32,v)         _BFSET_(r32,23, 0,v)
    #define     w32PLL_COUNTER_RESULT                          {\
            UNSG32 uRESULT_PLL_COUNT                           : 24;\
            UNSG32 RSVDx8_b24                                  :  8;\
          }
    union { UNSG32 u32PLL_COUNTER_RESULT;
            struct w32PLL_COUNTER_RESULT;
          };
    } SIE_PLL_COUNTER;
    typedef union  T32PLL_COUNTER_CTRL
          { UNSG32 u32;
            struct w32PLL_COUNTER_CTRL;
                 } T32PLL_COUNTER_CTRL;
    typedef union  T32PLL_COUNTER_STATUS
          { UNSG32 u32;
            struct w32PLL_COUNTER_STATUS;
                 } T32PLL_COUNTER_STATUS;
    typedef union  T32PLL_COUNTER_RESULT
          { UNSG32 u32;
            struct w32PLL_COUNTER_RESULT;
                 } T32PLL_COUNTER_RESULT;
    typedef union  TPLL_COUNTER_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32PLL_COUNTER_CTRL;
                   };
                 } TPLL_COUNTER_CTRL;
    typedef union  TPLL_COUNTER_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32PLL_COUNTER_STATUS;
                   };
                 } TPLL_COUNTER_STATUS;
    typedef union  TPLL_COUNTER_RESULT
          { UNSG32 u32[1];
            struct {
            struct w32PLL_COUNTER_RESULT;
                   };
                 } TPLL_COUNTER_RESULT;
     SIGN32 PLL_COUNTER_drvrd(SIE_PLL_COUNTER *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 PLL_COUNTER_drvwr(SIE_PLL_COUNTER *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void PLL_COUNTER_reset(SIE_PLL_COUNTER *p);
     SIGN32 PLL_COUNTER_cmp  (SIE_PLL_COUNTER *p, SIE_PLL_COUNTER *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define PLL_COUNTER_check(p,pie,pfx,hLOG) PLL_COUNTER_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define PLL_COUNTER_print(p,    pfx,hLOG) PLL_COUNTER_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_Gbl
#define h_Gbl (){}
    #define     RA_Gbl_ProductId                               0x0000
    #define     RA_Gbl_ProductId_ext                           0x0004
    #define     RA_Gbl_INT_ID                                  0x0008
    #define     RA_Gbl_bootStrap                               0x000C
    #define        Gbl_bootStrap_bootSrc_ROM_BOOT_FROM_USB2                 0x0
    #define        Gbl_bootStrap_bootSrc_ROM_BOOT_FROM_XSPI                 0x1
    #define        Gbl_bootStrap_bootSrc_ROM_BOOT_FROM_EMMC                 0x2
    #define        Gbl_bootStrap_bootSrc_RESERVED                           0x3
    #define        Gbl_bootStrap_cpuRstByps_CPU_INT_RST_BYPS                0x1
    #define        Gbl_bootStrap_cpuRstByps_CPU_INT_RST_EN                  0x0
    #define        Gbl_bootStrap_sysPll_0Byps_PLL_OUT                       0x0
    #define        Gbl_bootStrap_sysPll_0Byps_BYPS                          0x1
    #define        Gbl_bootStrap_sysPll_1Byps_PLL_OUT                       0x0
    #define        Gbl_bootStrap_sysPll_1Byps_BYPS                          0x1
    #define        Gbl_bootStrap_memPllByps_PLL_OUT                         0x0
    #define        Gbl_bootStrap_memPllByps_BYPS                            0x1
    #define        Gbl_bootStrap_cpuPllByps_PLL_OUT                         0x0
    #define        Gbl_bootStrap_cpuPllByps_BYPS                            0x1
    #define        Gbl_bootStrap_aPll_0Byps_PLL_OUT                         0x0
    #define        Gbl_bootStrap_aPll_0Byps_BYPS                            0x1
    #define        Gbl_bootStrap_aPll_1Byps_PLL_OUT                         0x0
    #define        Gbl_bootStrap_aPll_1Byps_BYPS                            0x1
    #define        Gbl_bootStrap_vPll_1Byps_PLL_OUT                         0x0
    #define        Gbl_bootStrap_vPll_1Byps_BYPS                            0x1
    #define        Gbl_bootStrap_ENG_EN_PRODUCTION_MODE                     0x0
    #define        Gbl_bootStrap_ENG_EN_DEVELOPE_MODE                       0x1
    #define     RA_Gbl_bootStrapEn                             0x0010
    #define        Gbl_bootStrapEn_cpuRstBypsEn_ENABLE                      0x1
    #define        Gbl_bootStrapEn_cpuRstBypsEn_DISABLE                     0x0
    #define        Gbl_bootStrapEn_sysPLL_0BypsEn_ENABLE                    0x1
    #define        Gbl_bootStrapEn_sysPLL_0BypsEn_DISABLE                   0x0
    #define        Gbl_bootStrapEn_sysPLL_1BypsEn_ENABLE                    0x1
    #define        Gbl_bootStrapEn_sysPLL_1BypsEn_DISABLE                   0x0
    #define        Gbl_bootStrapEn_memPLLBypsEn_ENABLE                      0x1
    #define        Gbl_bootStrapEn_memPLLBypsEn_DISABLE                     0x0
    #define        Gbl_bootStrapEn_cpuPLLBypsEn_ENABLE                      0x1
    #define        Gbl_bootStrapEn_cpuPLLBypsEn_DISABLE                     0x0
    #define        Gbl_bootStrapEn_aPLL_0BypsEn_ENABLE                      0x1
    #define        Gbl_bootStrapEn_aPLL_0BypsEn_DISABLE                     0x0
    #define        Gbl_bootStrapEn_aPLL_1BypsEn_ENABLE                      0x1
    #define        Gbl_bootStrapEn_aPLL_1BypsEn_DISABLE                     0x0
    #define        Gbl_bootStrapEn_vPLL_1BypsEn_ENABLE                      0x1
    #define        Gbl_bootStrapEn_vPLL_1BypsEn_DISABLE                     0x0
    #define     RA_Gbl_wounding_mcu2soc                        0x0014
    #define     RA_Gbl_chipCntl                                0x0040
    #define        Gbl_chipCntl_DIG_CAM_ENABLE_ENABLE                       0x1
    #define        Gbl_chipCntl_DIG_CAM_ENABLE_DISABLE                      0x0
    #define     RA_Gbl_AVIO_TRIG                               0x0044
    #define     RA_Gbl_chip_debug                              0x0048
    #define     RA_Gbl_sw_generic0                             0x0080
    #define     RA_Gbl_sw_generic1                             0x0084
    #define     RA_Gbl_sw_generic2                             0x0088
    #define     RA_Gbl_sw_generic3                             0x008C
    #define     RA_Gbl_FPGAR                                   0x0090
    #define     RA_Gbl_FPGARW                                  0x0094
    #define     RA_Gbl_RWTC_SOC                                0x0100
    #define     RA_Gbl_SRAM_PWR_EMMC                           0x010C
    #define     RA_Gbl_SRAM_PWR_SDIO                           0x0110
    #define     RA_Gbl_SRAM_PWR_SDIO1                          0x0114
    #define     RA_Gbl_SRAM_PWR_USB2                           0x0118
    #define     RA_Gbl_SRAM_PWR_USB2_1                         0x011C
    #define     RA_Gbl_SRAM_PWR_GFX3D                          0x0120
    #define     RA_Gbl_SRAM_PWR_MC                             0x0124
    #define     RA_Gbl_SRAM_PWR_ALM                            0x0128
    #define     RA_Gbl_SRAM_PWR_GE                             0x012C
    #define     RA_Gbl_SRAM_PWR_GE1                            0x0130
    #define     RA_Gbl_SRAM_PWR_NPU                            0x0134
    #define     RA_Gbl_SRAM_PWR_NPU_CSS                        0x0138
    #define     RA_Gbl_gfx3D_pwr_ctrl                          0x0150
    #define     RA_Gbl_gfx3D_pwr_sts                           0x0154
    #define     RA_Gbl_npu_pwr_ctrl                            0x0158
    #define     RA_Gbl_npu_pwr_sts                             0x015C
    #define     RA_Gbl_sysPll_0                                0x0200
    #define     RA_Gbl_sysPll_1                                0x0220
    #define     RA_Gbl_POR_EN_status                           0x0300
    #define     RA_Gbl_POR_EN_OVRD                             0x0304
    #define     RA_Gbl_POR_status                              0x0308
    #define     RA_Gbl_POR_CTL                                 0x030C
    #define     RA_Gbl_ResetTrigger                            0x0320
    #define        Gbl_ResetTrigger_chipReset_assert                        0x1
    #define        Gbl_ResetTrigger_chipReset_deassert                      0x0
    #define        Gbl_ResetTrigger_socDdrSyncReset_assert                  0x1
    #define        Gbl_ResetTrigger_socDdrSyncReset_deassert                0x0
    #define     RA_Gbl_ResetStatus                             0x0324
    #define        Gbl_ResetStatus_ChipResetStatus_asserted                 0x1
    #define        Gbl_ResetStatus_ChipResetStatus_deasserted               0x0
    #define        Gbl_ResetStatus_socDdrSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_socDdrSyncResetStatus_deasserted              0x0
    #define     RA_Gbl_WDTResetStatus                          0x032C
    #define        Gbl_WDTResetStatus_wd0Status_asserted                    0x1
    #define        Gbl_WDTResetStatus_wd0Status_deasserted                  0x0
    #define        Gbl_WDTResetStatus_wd1Status_asserted                    0x1
    #define        Gbl_WDTResetStatus_wd1Status_deasserted                  0x0
    #define        Gbl_WDTResetStatus_wd2Status_asserted                    0x1
    #define        Gbl_WDTResetStatus_wd2Status_deasserted                  0x0
    #define     RA_Gbl_WDTSysRstMask                           0x0330
    #define     RA_Gbl_CHIP_RESET_TRACKER                      0x0334
    #define     RA_Gbl_avioReset                               0x0340
    #define        Gbl_avioReset_SyncReset_assert                           0x1
    #define        Gbl_avioReset_SyncReset_deassert                         0x0
    #define     RA_Gbl_avioResetStatus                         0x0344
    #define        Gbl_avioResetStatus_SyncReset_assert                     0x1
    #define        Gbl_avioResetStatus_SyncReset_deassert                   0x0
    #define     RA_Gbl_perifReset                              0x0350
    #define        Gbl_perifReset_SyncReset_assert                          0x1
    #define        Gbl_perifReset_SyncReset_deassert                        0x0
    #define        Gbl_perifReset_ahbApbSyncReset_assert                    0x1
    #define        Gbl_perifReset_ahbApbSyncReset_deassert                  0x0
    #define        Gbl_perifReset_sdioSyncReset_assert                      0x1
    #define        Gbl_perifReset_sdioSyncReset_deassert                    0x0
    #define        Gbl_perifReset_ReservedSyncReset_assert                  0x1
    #define        Gbl_perifReset_ReservedSyncReset_deassert                0x0
    #define        Gbl_perifReset_usb0SyncReset_assert                      0x1
    #define        Gbl_perifReset_usb0SyncReset_deassert                    0x0
    #define        Gbl_perifReset_emmcSyncReset_assert                      0x1
    #define        Gbl_perifReset_emmcSyncReset_deassert                    0x0
    #define        Gbl_perifReset_gethRgmiiSyncReset_assert                 0x1
    #define        Gbl_perifReset_gethRgmiiSyncReset_deassert               0x0
    #define        Gbl_perifReset_sdio1SyncReset_assert                     0x1
    #define        Gbl_perifReset_sdio1SyncReset_deassert                   0x0
    #define        Gbl_perifReset_usb1SyncReset_assert                      0x1
    #define        Gbl_perifReset_usb1SyncReset_deassert                    0x0
    #define        Gbl_perifReset_gethRgmii1SyncReset_assert                0x1
    #define        Gbl_perifReset_gethRgmii1SyncReset_deassert              0x0
    #define     RA_Gbl_perifResetStatus                        0x035C
    #define        Gbl_perifResetStatus_SyncReset_assert                    0x1
    #define        Gbl_perifResetStatus_SyncReset_deassert                  0x0
    #define        Gbl_perifResetStatus_ahbApbSyncReset_assert              0x1
    #define        Gbl_perifResetStatus_ahbApbSyncReset_deassert              0x0
    #define        Gbl_perifResetStatus_sdioSyncReset_assert                0x1
    #define        Gbl_perifResetStatus_sdioSyncReset_deassert              0x0
    #define        Gbl_perifResetStatus_usb0SyncReset_assert                0x1
    #define        Gbl_perifResetStatus_usb0SyncReset_deassert              0x0
    #define        Gbl_perifResetStatus_emmcSyncReset_assert                0x1
    #define        Gbl_perifResetStatus_emmcSyncReset_deassert              0x0
    #define        Gbl_perifResetStatus_gethRgmiiSyncReset_assert              0x1
    #define        Gbl_perifResetStatus_gethRgmiiSyncReset_deassert              0x0
    #define        Gbl_perifResetStatus_sdio1SyncReset_assert               0x1
    #define        Gbl_perifResetStatus_sdio1SyncReset_deassert              0x0
    #define        Gbl_perifResetStatus_usb1SyncReset_assert                0x1
    #define        Gbl_perifResetStatus_usb1SyncReset_deassert              0x0
    #define        Gbl_perifResetStatus_gethRgmii1SyncReset_assert              0x1
    #define        Gbl_perifResetStatus_gethRgmii1SyncReset_deassert              0x0
    #define     RA_Gbl_perifStickyResetN                       0x0360
    #define        Gbl_perifStickyResetN_usb0PhyRstn_asserted               0x0
    #define        Gbl_perifStickyResetN_usb0PhyRstn_deasserted              0x1
    #define        Gbl_perifStickyResetN_usb0CoreRstn_asserted              0x0
    #define        Gbl_perifStickyResetN_usb0CoreRstn_deasserted              0x1
    #define        Gbl_perifStickyResetN_usb0MahbRstn_asserted              0x0
    #define        Gbl_perifStickyResetN_usb0MahbRstn_deasserted              0x1
    #define        Gbl_perifStickyResetN_usb1PhyRstn_asserted               0x0
    #define        Gbl_perifStickyResetN_usb1PhyRstn_deasserted              0x1
    #define        Gbl_perifStickyResetN_usb1CoreRstn_asserted              0x0
    #define        Gbl_perifStickyResetN_usb1CoreRstn_deasserted              0x1
    #define        Gbl_perifStickyResetN_usb1MahbRstn_asserted              0x0
    #define        Gbl_perifStickyResetN_usb1MahbRstn_deasserted              0x1
    #define     RA_Gbl_apbPerifResetTrigger                    0x0370
    #define        Gbl_apbPerifResetTrigger_uart0SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_uart0SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_uart1SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_uart1SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_uart2SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_uart2SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_uart3SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_uart3SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_i2c0SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_i2c0SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_i2c1SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_i2c1SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_spi0SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_spi0SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_spi1SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_spi1SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_spi2SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_spi2SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_spi3SyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_spi3SyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_apbTimersSyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_apbTimersSyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_apbSysCntSyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_apbSysCntSyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_apbWDTSyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_apbWDTSyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_apbGPIOSyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_apbGPIOSyncReset_deassert              0x0
    #define        Gbl_apbPerifResetTrigger_apbDmaSyncReset_assert              0x1
    #define        Gbl_apbPerifResetTrigger_apbDmaSyncReset_deassert              0x0
    #define     RA_Gbl_apbPerifResetStatus                     0x0374
    #define        Gbl_apbPerifResetStatus_uart0SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_uart0SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_uart1SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_uart1SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_uart2SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_uart2SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_uart3SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_uart3SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_i2c0SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_i2c0SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_i2c1SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_i2c1SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_spi0SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_spi0SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_spi1SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_spi1SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_spi2SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_spi2SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_spi3SyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_spi3SyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_apbTimersSyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_apbTimersSyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_apbSysCntSyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_apbSysCntSyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_apbWDTSyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_apbWDTSyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_apbGPIOSyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_apbGPIOSyncResetStatus_deassert              0x0
    #define        Gbl_apbPerifResetStatus_apbDmaSyncResetStatus_assert              0x1
    #define        Gbl_apbPerifResetStatus_apbDmaSyncResetStatus_deassert              0x0
    #define     RA_Gbl_topStickyResetN                         0x0378
    #define        Gbl_topStickyResetN_gpuCoreRstn_asserted                 0x0
    #define        Gbl_topStickyResetN_gpuCoreRstn_deasserted               0x1
    #define        Gbl_topStickyResetN_npuCoreRstn_asserted                 0x0
    #define        Gbl_topStickyResetN_npuCoreRstn_deasserted               0x1
    #define     RA_Gbl_avioStickyResetN                        0x037C
    #define        Gbl_avioStickyResetN_avioAIOGRstn_asserted               0x0
    #define        Gbl_avioStickyResetN_avioAIOGRstn_deasserted              0x1
    #define        Gbl_avioStickyResetN_avioVPPGRstn_asserted               0x0
    #define        Gbl_avioStickyResetN_avioVPPGRstn_deasserted              0x1
    #define        Gbl_avioStickyResetN_avioVIPGRstn_asserted               0x0
    #define        Gbl_avioStickyResetN_avioVIPGRstn_deasserted              0x1
    #define     RA_Gbl_ClkSwitch                               0x0400
    #define        Gbl_ClkSwitch_sysPLL_0SWBypass_refClk                    0x1
    #define        Gbl_ClkSwitch_sysPLL_0SWBypass_pllClk                    0x0
    #define        Gbl_ClkSwitch_sysPLL_1SWBypass_refClk                    0x1
    #define        Gbl_ClkSwitch_sysPLL_1SWBypass_pllClk                    0x0
    #define        Gbl_ClkSwitch_memPLLSWBypass_refClk                      0x1
    #define        Gbl_ClkSwitch_memPLLSWBypass_pllClk                      0x0
    #define        Gbl_ClkSwitch_cpuPLLSWBypass_refClk                      0x1
    #define        Gbl_ClkSwitch_cpuPLLSWBypass_pllClk                      0x0
    #define        Gbl_ClkSwitch_aPLL_0SWBypass_refClk                      0x1
    #define        Gbl_ClkSwitch_aPLL_0SWBypass_pllClk                      0x0
    #define        Gbl_ClkSwitch_aPLL_1SWBypass_refClk                      0x1
    #define        Gbl_ClkSwitch_aPLL_1SWBypass_pllClk                      0x0
    #define        Gbl_ClkSwitch_vPLL_0SWBypass_refClk                      0x1
    #define        Gbl_ClkSwitch_vPLL_0SWBypass_pllClk                      0x0
    #define        Gbl_ClkSwitch_vPLL_1SWBypass_refClk                      0x1
    #define        Gbl_ClkSwitch_vPLL_1SWBypass_pllClk                      0x0
    #define     RA_Gbl_clkEnable                               0x0420
    #define        Gbl_clkEnable_usb0CoreClkEn_enable                       0x1
    #define        Gbl_clkEnable_usb0CoreClkEn_disable                      0x0
    #define        Gbl_clkEnable_sdioSysClkEn_enable                        0x1
    #define        Gbl_clkEnable_sdioSysClkEn_disable                       0x0
    #define        Gbl_clkEnable_emmcSysClkEn_enable                        0x1
    #define        Gbl_clkEnable_emmcSysClkEn_disable                       0x0
    #define        Gbl_clkEnable_gpuAxiClkEn_enable                         0x1
    #define        Gbl_clkEnable_gpuAxiClkEn_disable                        0x0
    #define        Gbl_clkEnable_gethRgmiiSysClkEn_enable                   0x1
    #define        Gbl_clkEnable_gethRgmiiSysClkEn_disable                  0x0
    #define        Gbl_clkEnable_sdio1SysClkEn_enable                       0x1
    #define        Gbl_clkEnable_sdio1SysClkEn_disable                      0x0
    #define        Gbl_clkEnable_usb1CoreClkEn_enable                       0x1
    #define        Gbl_clkEnable_usb1CoreClkEn_disable                      0x0
    #define        Gbl_clkEnable_gethRgmii1SysClkEn_enable                  0x1
    #define        Gbl_clkEnable_gethRgmii1SysClkEn_disable                 0x0
    #define        Gbl_clkEnable_usb0PhyRefClkEn_enable                     0x1
    #define        Gbl_clkEnable_usb0PhyRefClkEn_disable                    0x0
    #define        Gbl_clkEnable_usb1PhyRefClkEn_enable                     0x1
    #define        Gbl_clkEnable_usb1PhyRefClkEn_disable                    0x0
    #define        Gbl_clkEnable_apbUART0ClkEn_enable                       0x1
    #define        Gbl_clkEnable_apbUART0ClkEn_disable                      0x0
    #define        Gbl_clkEnable_apbUART1ClkEn_enable                       0x1
    #define        Gbl_clkEnable_apbUART1ClkEn_disable                      0x0
    #define        Gbl_clkEnable_apbUART2ClkEn_enable                       0x1
    #define        Gbl_clkEnable_apbUART2ClkEn_disable                      0x0
    #define        Gbl_clkEnable_apbUART3ClkEn_enable                       0x1
    #define        Gbl_clkEnable_apbUART3ClkEn_disable                      0x0
    #define        Gbl_clkEnable_apbI2C0ClkEn_enable                        0x1
    #define        Gbl_clkEnable_apbI2C0ClkEn_disable                       0x0
    #define        Gbl_clkEnable_apbI2C1ClkEn_enable                        0x1
    #define        Gbl_clkEnable_apbI2C1ClkEn_disable                       0x0
    #define        Gbl_clkEnable_apbSPI0ClkEn_enable                        0x1
    #define        Gbl_clkEnable_apbSPI0ClkEn_disable                       0x0
    #define        Gbl_clkEnable_apbSPI1ClkEn_enable                        0x1
    #define        Gbl_clkEnable_apbSPI1ClkEn_disable                       0x0
    #define        Gbl_clkEnable_apbSPI2ClkEn_enable                        0x1
    #define        Gbl_clkEnable_apbSPI2ClkEn_disable                       0x0
    #define        Gbl_clkEnable_apbSPI3ClkEn_enable                        0x1
    #define        Gbl_clkEnable_apbSPI3ClkEn_disable                       0x0
    #define        Gbl_clkEnable_apbGPIOClkEn_enable                        0x1
    #define        Gbl_clkEnable_apbGPIOClkEn_disable                       0x0
    #define        Gbl_clkEnable_apbTimersClkEn_enable                      0x1
    #define        Gbl_clkEnable_apbTimersClkEn_disable                     0x0
    #define        Gbl_clkEnable_apbSysCntClkEn_enable                      0x1
    #define        Gbl_clkEnable_apbSysCntClkEn_disable                     0x0
    #define        Gbl_clkEnable_apbWDTClkEn_enable                         0x1
    #define        Gbl_clkEnable_apbWDTClkEn_disable                        0x0
    #define        Gbl_clkEnable_apbDmaClkEn_enable                         0x1
    #define        Gbl_clkEnable_apbDmaClkEn_disable                        0x0
    #define     RA_Gbl_cpufastRefClk                           0x0440
    #define     RA_Gbl_memfastRefClk                           0x0444
    #define     RA_Gbl_cfgClk                                  0x0448
    #define     RA_Gbl_sysClk                                  0x044C
    #define     RA_Gbl_perifSysClk                             0x0450
    #define     RA_Gbl_apbCoreClk                              0x0454
    #define     RA_Gbl_apbSerClk                               0x0458
    #define     RA_Gbl_atbClk                                  0x045C
    #define     RA_Gbl_hpcClk                                  0x0460
    #define     RA_Gbl_emmcClk                                 0x0464
    #define     RA_Gbl_sd0Clk                                  0x0468
    #define     RA_Gbl_sd1Clk                                  0x046C
    #define     RA_Gbl_gethRgmiiClk                            0x0470
    #define     RA_Gbl_gethRgmii1Clk                           0x0474
    #define     RA_Gbl_ge0_ptp_refClk                          0x0478
    #define     RA_Gbl_ge1_ptp_refClk                          0x047C
    #define     RA_Gbl_usb2TestClk                             0x0480
    #define     RA_Gbl_usb2TestClk480mGroup0                   0x0484
    #define     RA_Gbl_usb2TestClk480mGroup1                   0x0488
    #define     RA_Gbl_usb2TestClk480mGroup2                   0x048C
    #define     RA_Gbl_usb2TestClk100mGroup0                   0x0490
    #define     RA_Gbl_usb2TestClk100mGroup1                   0x0494
    #define     RA_Gbl_usb2TestClk100mGroup2                   0x0498
    #define     RA_Gbl_usb2TestClk100mGroup3                   0x049C
    #define     RA_Gbl_perifTestClk125mGroup0                  0x04A0
    #define     RA_Gbl_perifTestClk200mGroup0                  0x04A4
    #define     RA_Gbl_perifTestClk200mGroup1                  0x04A8
    #define     RA_Gbl_gpuClk                                  0x04AC
    #define     RA_Gbl_npuClk                                  0x04B0
    #define     RA_Gbl_avioSysClk                              0x04B4
    #define     RA_Gbl_aioSysClk                               0x04B8
    #define     RA_Gbl_avio_lcdc2ScanClk                       0x04BC
    #define     RA_Gbl_avio_ipiClk                             0x04C0
    #define     RA_Gbl_avio_pClk                               0x04C4
    #define     RA_Gbl_avio_dphyrxtxescClk                     0x04C8
    #define     RA_Gbl_avioFpllClk                             0x04CC
    #define     RA_Gbl_avio_rx_scanbyteClk                     0x04D0
    #define     RA_Gbl_avio_rx_scantestClk                     0x04D4
    #define     RA_Gbl_USBOTG_REFCLK_CTRL0                     0x04E0
    #define     RA_Gbl_USBOTG_REFCLK_CTRL1                     0x04E4
    #define     RA_Gbl_USBOTG1_REFCLK_CTRL0                    0x04E8
    #define     RA_Gbl_USBOTG1_REFCLK_CTRL1                    0x04EC
    #define     RA_Gbl_SECURE_SCAN_EN                          0x0500
    #define        Gbl_SECURE_SCAN_EN_drcg_drcgActive                       0x1
    #define        Gbl_SECURE_SCAN_EN_drcg_drcgInactive                     0x0
    #define     RA_Gbl_gic400_ctrl                             0x0560
    #define     RA_Gbl_LCDD_IO_CTRL                            0x05D0
    #define     RA_Gbl_SOC_PLL_MUX                             0x05D4
    #define     RA_Gbl_ge0_ptp_mux                             0x05D8
    #define     RA_Gbl_ge1_ptp_mux                             0x05DC
    #define     RA_Gbl_gfx_3d                                  0x05E0
    #define     RA_Gbl_pdma_dst_req_mask                       0x05E4
    #define     RA_Gbl_pdma_src_req_mask                       0x05E8
    #define     RA_Gbl_PERIF                                   0x0A00
    #define     RA_Gbl_PLL_COUNTER                             0x0B00
    #define     RA_Gbl_pinmux_cntl_bus                         0x8000
    #define        Gbl_pinmux_cntl_bus_GPIO23_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO23_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO23_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO23_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO23_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO23_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO23_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO24_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO24_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO24_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO24_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO24_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO24_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO24_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO25_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO25_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO25_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO25_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO25_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO25_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO25_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO26_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO26_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO26_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO26_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO26_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO26_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO27_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO27_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO27_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO27_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO28_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO28_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO28_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO28_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO29_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO29_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO29_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO29_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO29_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO29_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO30_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO30_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO30_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO30_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO30_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO30_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO31_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO31_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO31_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO31_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO31_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO32_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO32_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO32_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO32_MODE_7                        0x7
    #define     RA_Gbl_pinmux_cntl_bus1                        0x8004
    #define        Gbl_pinmux_cntl_bus_GPIO33_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO33_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO33_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO33_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO33_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO33_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO34_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO34_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO34_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO34_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO35_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO35_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO35_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO35_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO35_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO35_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO36_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO36_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO36_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO36_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO36_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO37_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO37_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO37_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO37_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO37_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO38_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO38_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO38_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO38_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO38_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO39_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO39_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO39_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO39_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO39_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO39_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO39_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO40_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO40_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO40_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO40_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO40_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO40_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO40_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO41_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO41_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO41_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO41_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO42_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO42_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO42_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO42_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO42_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO42_MODE_6                        0x6
    #define     RA_Gbl_pinmux_cntl_bus2                        0x8008
    #define        Gbl_pinmux_cntl_bus_GPIO43_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO43_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO43_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO43_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO44_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO44_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO44_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO44_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO44_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO44_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO45_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO45_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO45_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO46_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO46_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO46_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO46_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO46_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO46_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO46_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO47_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO47_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO47_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO47_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO47_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO47_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO47_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO48_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO48_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO48_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO48_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO48_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO49_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO49_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO49_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO49_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO49_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO50_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO50_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO50_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO50_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO51_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO51_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO51_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO52_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO52_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO52_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO52_MODE_7                        0x7
    #define     RA_Gbl_pinmux_cntl_bus3                        0x800C
    #define        Gbl_pinmux_cntl_bus_GPIO53_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO53_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO53_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO53_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO54_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO54_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO54_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO55_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO55_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO55_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO55_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO56_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO56_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO56_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO57_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO57_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO57_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO58_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO58_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO58_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO59_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO59_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO59_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO0_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO0_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO0_MODE_7                         0x7
    #define        Gbl_pinmux_cntl_bus_GPIO1_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO1_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO1_MODE_7                         0x7
    #define        Gbl_pinmux_cntl_bus_GPIO2_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO2_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO2_MODE_4                         0x4
    #define        Gbl_pinmux_cntl_bus_GPIO2_MODE_7                         0x7
    #define     RA_Gbl_pinmux_cntl_bus4                        0x8010
    #define        Gbl_pinmux_cntl_bus_GPIO3_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO3_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO3_MODE_7                         0x7
    #define        Gbl_pinmux_cntl_bus_GPIO4_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO4_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO4_MODE_4                         0x4
    #define        Gbl_pinmux_cntl_bus_GPIO4_MODE_7                         0x7
    #define        Gbl_pinmux_cntl_bus_GPIO5_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO5_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO5_MODE_2                         0x2
    #define        Gbl_pinmux_cntl_bus_GPIO5_MODE_3                         0x3
    #define        Gbl_pinmux_cntl_bus_GPIO5_MODE_4                         0x4
    #define        Gbl_pinmux_cntl_bus_GPIO5_MODE_6                         0x6
    #define        Gbl_pinmux_cntl_bus_GPIO6_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO6_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO6_MODE_2                         0x2
    #define        Gbl_pinmux_cntl_bus_GPIO6_MODE_3                         0x3
    #define        Gbl_pinmux_cntl_bus_GPIO6_MODE_4                         0x4
    #define        Gbl_pinmux_cntl_bus_GPIO6_MODE_6                         0x6
    #define        Gbl_pinmux_cntl_bus_GPIO7_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO7_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO7_MODE_3                         0x3
    #define        Gbl_pinmux_cntl_bus_GPIO7_MODE_4                         0x4
    #define        Gbl_pinmux_cntl_bus_GPIO7_MODE_5                         0x5
    #define        Gbl_pinmux_cntl_bus_GPIO7_MODE_6                         0x6
    #define        Gbl_pinmux_cntl_bus_GPIO8_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO8_MODE_1                         0x1
    #define        Gbl_pinmux_cntl_bus_GPIO8_MODE_2                         0x2
    #define        Gbl_pinmux_cntl_bus_GPIO8_MODE_3                         0x3
    #define        Gbl_pinmux_cntl_bus_GPIO8_MODE_4                         0x4
    #define        Gbl_pinmux_cntl_bus_GPIO8_MODE_6                         0x6
    #define        Gbl_pinmux_cntl_bus_GPIO9_MODE_0                         0x0
    #define        Gbl_pinmux_cntl_bus_GPIO9_MODE_2                         0x2
    #define        Gbl_pinmux_cntl_bus_GPIO9_MODE_4                         0x4
    #define        Gbl_pinmux_cntl_bus_GPIO10_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO10_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO10_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO10_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO11_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO11_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO11_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO11_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO11_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO12_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO12_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO12_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO12_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO12_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO12_MODE_7                        0x7
    #define     RA_Gbl_pinmux_cntl_bus5                        0x8014
    #define        Gbl_pinmux_cntl_bus_GPIO13_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO13_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO13_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO13_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO13_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO13_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO14_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO14_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO14_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO14_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO14_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO15_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO15_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO15_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO15_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO15_MODE_6                        0x6
    #define        Gbl_pinmux_cntl_bus_GPIO15_MODE_7                        0x7
    #define        Gbl_pinmux_cntl_bus_GPIO16_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO16_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO16_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO17_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO17_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO17_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO17_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO17_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO18_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO18_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO18_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO18_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO18_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO18_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO19_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO19_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO19_MODE_2                        0x2
    #define        Gbl_pinmux_cntl_bus_GPIO19_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO19_MODE_4                        0x4
    #define        Gbl_pinmux_cntl_bus_GPIO19_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO20_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO20_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO20_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO21_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO21_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO21_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO21_MODE_5                        0x5
    #define        Gbl_pinmux_cntl_bus_GPIO22_MODE_0                        0x0
    #define        Gbl_pinmux_cntl_bus_GPIO22_MODE_1                        0x1
    #define        Gbl_pinmux_cntl_bus_GPIO22_MODE_3                        0x3
    #define        Gbl_pinmux_cntl_bus_GPIO22_MODE_4                        0x4
    #define     RA_Gbl_GPIO23Cntl                              0x8800
    #define     RA_Gbl_GPIO24Cntl                              0x8804
    #define     RA_Gbl_GPIO25Cntl                              0x8808
    #define     RA_Gbl_GPIO26Cntl                              0x880C
    #define     RA_Gbl_GPIO27Cntl                              0x8810
    #define     RA_Gbl_GPIO28Cntl                              0x8814
    #define     RA_Gbl_GPIO29Cntl                              0x8818
    #define     RA_Gbl_GPIO30Cntl                              0x881C
    #define     RA_Gbl_GPIO31Cntl                              0x8820
    #define     RA_Gbl_GPIO32Cntl                              0x8824
    #define     RA_Gbl_GPIO33Cntl                              0x8828
    #define     RA_Gbl_GPIO34Cntl                              0x882C
    #define     RA_Gbl_GPIO35Cntl                              0x8830
    #define     RA_Gbl_GPIO36Cntl                              0x8834
    #define     RA_Gbl_GPIO37Cntl                              0x8838
    #define     RA_Gbl_GPIO38Cntl                              0x883C
    #define     RA_Gbl_GPIO39Cntl                              0x8840
    #define     RA_Gbl_GPIO40Cntl                              0x8844
    #define     RA_Gbl_GPIO41Cntl                              0x8848
    #define     RA_Gbl_GPIO42Cntl                              0x884C
    #define     RA_Gbl_GPIO43Cntl                              0x8850
    #define     RA_Gbl_GPIO44Cntl                              0x8854
    #define     RA_Gbl_GPIO45Cntl                              0x8858
    #define     RA_Gbl_GPIO46Cntl                              0x885C
    #define     RA_Gbl_GPIO47Cntl                              0x8860
    #define     RA_Gbl_GPIO48Cntl                              0x8864
    #define     RA_Gbl_GPIO49Cntl                              0x8868
    #define     RA_Gbl_GPIO50Cntl                              0x886C
    #define     RA_Gbl_GPIO51Cntl                              0x8870
    #define     RA_Gbl_GPIO52Cntl                              0x8874
    #define     RA_Gbl_GPIO53Cntl                              0x8878
    #define     RA_Gbl_GPIO54Cntl                              0x887C
    #define     RA_Gbl_GPIO55Cntl                              0x8880
    #define     RA_Gbl_GPIO56Cntl                              0x8884
    #define     RA_Gbl_GPIO57Cntl                              0x8888
    #define     RA_Gbl_GPIO58Cntl                              0x888C
    #define     RA_Gbl_GPIO59Cntl                              0x8890
    #define     RA_Gbl_GPIO0Cntl                               0x8894
    #define     RA_Gbl_GPIO1Cntl                               0x8898
    #define     RA_Gbl_GPIO2Cntl                               0x889C
    #define     RA_Gbl_GPIO3Cntl                               0x88A0
    #define     RA_Gbl_GPIO4Cntl                               0x88A4
    #define     RA_Gbl_GPIO5Cntl                               0x88A8
    #define     RA_Gbl_GPIO6Cntl                               0x88AC
    #define     RA_Gbl_GPIO7Cntl                               0x88B0
    #define     RA_Gbl_GPIO8Cntl                               0x88B4
    #define     RA_Gbl_GPIO9Cntl                               0x88B8
    #define     RA_Gbl_GPIO10Cntl                              0x88BC
    #define     RA_Gbl_GPIO11Cntl                              0x88C0
    #define     RA_Gbl_GPIO12Cntl                              0x88C4
    #define     RA_Gbl_GPIO13Cntl                              0x88C8
    #define     RA_Gbl_GPIO14Cntl                              0x88CC
    #define     RA_Gbl_GPIO15Cntl                              0x88D0
    #define     RA_Gbl_GPIO16Cntl                              0x88D4
    #define     RA_Gbl_GPIO17Cntl                              0x88D8
    #define     RA_Gbl_GPIO18Cntl                              0x88DC
    #define     RA_Gbl_GPIO19Cntl                              0x88E0
    #define     RA_Gbl_GPIO20Cntl                              0x88E4
    #define     RA_Gbl_GPIO21Cntl                              0x88E8
    #define     RA_Gbl_GPIO22Cntl                              0x88EC
    typedef struct SIE_Gbl {
    #define   SET32Gbl_ProductId_Id(r32,v)                     _BFSET_(r32,31, 0,v)
    #define     w32Gbl_ProductId                               {\
            UNSG32 uProductId_Id                               : 32;\
          }
    union { UNSG32 u32Gbl_ProductId;
            struct w32Gbl_ProductId;
          };
    #define   SET32Gbl_ProductId_ext_ID_EXT(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   SET16Gbl_ProductId_ext_ID_EXT(r16,v)             _BFSET_(r16, 7, 0,v)
    #define     w32Gbl_ProductId_ext                           {\
            UNSG32 uProductId_ext_ID_EXT                       :  8;\
            UNSG32 RSVDx4_b8                                   : 24;\
          }
    union { UNSG32 u32Gbl_ProductId_ext;
            struct w32Gbl_ProductId_ext;
          };
    #define   SET32Gbl_INT_ID_VALUE(r32,v)                     _BFSET_(r32, 7, 0,v)
    #define   SET16Gbl_INT_ID_VALUE(r16,v)                     _BFSET_(r16, 7, 0,v)
    #define     w32Gbl_INT_ID                                  {\
            UNSG32 uINT_ID_VALUE                               :  8;\
            UNSG32 RSVDx8_b8                                   : 24;\
          }
    union { UNSG32 u32Gbl_INT_ID;
            struct w32Gbl_INT_ID;
          };
    #define   SET32Gbl_bootStrap_softwareStrap(r32,v)          _BFSET_(r32,15, 0,v)
    #define   SET16Gbl_bootStrap_softwareStrap(r16,v)          _BFSET_(r16,15, 0,v)
    #define   SET32Gbl_bootStrap_bootSrc(r32,v)                _BFSET_(r32,17,16,v)
    #define   SET16Gbl_bootStrap_bootSrc(r16,v)                _BFSET_(r16, 1, 0,v)
    #define   SET32Gbl_bootStrap_cpuRstByps(r32,v)             _BFSET_(r32,18,18,v)
    #define   SET16Gbl_bootStrap_cpuRstByps(r16,v)             _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_bootStrap_sysPll_0Byps(r32,v)           _BFSET_(r32,19,19,v)
    #define   SET16Gbl_bootStrap_sysPll_0Byps(r16,v)           _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_bootStrap_sysPll_1Byps(r32,v)           _BFSET_(r32,20,20,v)
    #define   SET16Gbl_bootStrap_sysPll_1Byps(r16,v)           _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_bootStrap_memPllByps(r32,v)             _BFSET_(r32,21,21,v)
    #define   SET16Gbl_bootStrap_memPllByps(r16,v)             _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_bootStrap_cpuPllByps(r32,v)             _BFSET_(r32,22,22,v)
    #define   SET16Gbl_bootStrap_cpuPllByps(r16,v)             _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_bootStrap_aPll_0Byps(r32,v)             _BFSET_(r32,23,23,v)
    #define   SET16Gbl_bootStrap_aPll_0Byps(r16,v)             _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_bootStrap_aPll_1Byps(r32,v)             _BFSET_(r32,24,24,v)
    #define   SET16Gbl_bootStrap_aPll_1Byps(r16,v)             _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_bootStrap_vPll_1Byps(r32,v)             _BFSET_(r32,25,25,v)
    #define   SET16Gbl_bootStrap_vPll_1Byps(r16,v)             _BFSET_(r16, 9, 9,v)
    #define   SET32Gbl_bootStrap_ENG_EN(r32,v)                 _BFSET_(r32,26,26,v)
    #define   SET16Gbl_bootStrap_ENG_EN(r16,v)                 _BFSET_(r16,10,10,v)
    #define     w32Gbl_bootStrap                               {\
            UNSG32 ubootStrap_softwareStrap                    : 16;\
            UNSG32 ubootStrap_bootSrc                          :  2;\
            UNSG32 ubootStrap_cpuRstByps                       :  1;\
            UNSG32 ubootStrap_sysPll_0Byps                     :  1;\
            UNSG32 ubootStrap_sysPll_1Byps                     :  1;\
            UNSG32 ubootStrap_memPllByps                       :  1;\
            UNSG32 ubootStrap_cpuPllByps                       :  1;\
            UNSG32 ubootStrap_aPll_0Byps                       :  1;\
            UNSG32 ubootStrap_aPll_1Byps                       :  1;\
            UNSG32 ubootStrap_vPll_1Byps                       :  1;\
            UNSG32 ubootStrap_ENG_EN                           :  1;\
            UNSG32 RSVDxC_b27                                  :  5;\
          }
    union { UNSG32 u32Gbl_bootStrap;
            struct w32Gbl_bootStrap;
          };
    #define   SET32Gbl_bootStrapEn_cpuRstBypsEn(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_bootStrapEn_cpuRstBypsEn(r16,v)         _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_bootStrapEn_sysPLL_0BypsEn(r32,v)       _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_bootStrapEn_sysPLL_0BypsEn(r16,v)       _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_bootStrapEn_sysPLL_1BypsEn(r32,v)       _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_bootStrapEn_sysPLL_1BypsEn(r16,v)       _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_bootStrapEn_memPLLBypsEn(r32,v)         _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_bootStrapEn_memPLLBypsEn(r16,v)         _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_bootStrapEn_cpuPLLBypsEn(r32,v)         _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_bootStrapEn_cpuPLLBypsEn(r16,v)         _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_bootStrapEn_aPLL_0BypsEn(r32,v)         _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_bootStrapEn_aPLL_0BypsEn(r16,v)         _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_bootStrapEn_aPLL_1BypsEn(r32,v)         _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_bootStrapEn_aPLL_1BypsEn(r16,v)         _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_bootStrapEn_vPLL_1BypsEn(r32,v)         _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_bootStrapEn_vPLL_1BypsEn(r16,v)         _BFSET_(r16, 7, 7,v)
    #define     w32Gbl_bootStrapEn                             {\
            UNSG32 ubootStrapEn_cpuRstBypsEn                   :  1;\
            UNSG32 ubootStrapEn_sysPLL_0BypsEn                 :  1;\
            UNSG32 ubootStrapEn_sysPLL_1BypsEn                 :  1;\
            UNSG32 ubootStrapEn_memPLLBypsEn                   :  1;\
            UNSG32 ubootStrapEn_cpuPLLBypsEn                   :  1;\
            UNSG32 ubootStrapEn_aPLL_0BypsEn                   :  1;\
            UNSG32 ubootStrapEn_aPLL_1BypsEn                   :  1;\
            UNSG32 ubootStrapEn_vPLL_1BypsEn                   :  1;\
            UNSG32 RSVDx10_b8                                  : 24;\
          }
    union { UNSG32 u32Gbl_bootStrapEn;
            struct w32Gbl_bootStrapEn;
          };
    #define   SET32Gbl_wounding_mcu2soc_synpu_disable(r32,v)   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_wounding_mcu2soc_synpu_disable(r16,v)   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_wounding_mcu2soc_gpu_disable(r32,v)     _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_wounding_mcu2soc_gpu_disable(r16,v)     _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_wounding_mcu2soc_mipi_csi_disable(r32,v) _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_wounding_mcu2soc_mipi_csi_disable(r16,v) _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_wounding_mcu2soc_ca55_core1_disable(r32,v) _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_wounding_mcu2soc_ca55_core1_disable(r16,v) _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_wounding_mcu2soc_ge1_disable(r32,v)     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_wounding_mcu2soc_ge1_disable(r16,v)     _BFSET_(r16, 4, 4,v)
    #define     w32Gbl_wounding_mcu2soc                        {\
            UNSG32 uwounding_mcu2soc_synpu_disable             :  1;\
            UNSG32 uwounding_mcu2soc_gpu_disable               :  1;\
            UNSG32 uwounding_mcu2soc_mipi_csi_disable          :  1;\
            UNSG32 uwounding_mcu2soc_ca55_core1_disable        :  1;\
            UNSG32 uwounding_mcu2soc_ge1_disable               :  1;\
            UNSG32 RSVDx14_b5                                  : 27;\
          }
    union { UNSG32 u32Gbl_wounding_mcu2soc;
            struct w32Gbl_wounding_mcu2soc;
          };
             UNSG8 RSVDx18                                     [40];
    #define   SET32Gbl_chipCntl_MCU_PDM_SEL(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_chipCntl_MCU_PDM_SEL(r16,v)             _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_chipCntl_RMII1_MASTER_MODE_SEL(r32,v)   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_chipCntl_RMII1_MASTER_MODE_SEL(r16,v)   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_chipCntl_RMII2_MASTER_MODE_SEL(r32,v)   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_chipCntl_RMII2_MASTER_MODE_SEL(r16,v)   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_chipCntl_DIG_CAM_ENABLE(r32,v)          _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_chipCntl_DIG_CAM_ENABLE(r16,v)          _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_chipCntl_SDIO2_LPBK_CLK_EN(r32,v)       _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_chipCntl_SDIO2_LPBK_CLK_EN(r16,v)       _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_chipCntl_SDIO2_LPBK_CLK_SEL(r32,v)      _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_chipCntl_SDIO2_LPBK_CLK_SEL(r16,v)      _BFSET_(r16, 5, 5,v)
    #define     w32Gbl_chipCntl                                {\
            UNSG32 uchipCntl_MCU_PDM_SEL                       :  1;\
            UNSG32 uchipCntl_RMII1_MASTER_MODE_SEL             :  1;\
            UNSG32 uchipCntl_RMII2_MASTER_MODE_SEL             :  1;\
            UNSG32 uchipCntl_DIG_CAM_ENABLE                    :  1;\
            UNSG32 uchipCntl_SDIO2_LPBK_CLK_EN                 :  1;\
            UNSG32 uchipCntl_SDIO2_LPBK_CLK_SEL                :  1;\
            UNSG32 RSVDx40_b6                                  : 26;\
          }
    union { UNSG32 u32Gbl_chipCntl;
            struct w32Gbl_chipCntl;
          };
    #define   SET32Gbl_AVIO_TRIG_trigger_select_0(r32,v)       _BFSET_(r32, 5, 0,v)
    #define   SET16Gbl_AVIO_TRIG_trigger_select_0(r16,v)       _BFSET_(r16, 5, 0,v)
    #define   SET32Gbl_AVIO_TRIG_trigger_select_1(r32,v)       _BFSET_(r32,11, 6,v)
    #define   SET16Gbl_AVIO_TRIG_trigger_select_1(r16,v)       _BFSET_(r16,11, 6,v)
    #define   SET32Gbl_AVIO_TRIG_trigger_select_2(r32,v)       _BFSET_(r32,17,12,v)
    #define     w32Gbl_AVIO_TRIG                               {\
            UNSG32 uAVIO_TRIG_trigger_select_0                 :  6;\
            UNSG32 uAVIO_TRIG_trigger_select_1                 :  6;\
            UNSG32 uAVIO_TRIG_trigger_select_2                 :  6;\
            UNSG32 RSVDx44_b18                                 : 14;\
          }
    union { UNSG32 u32Gbl_AVIO_TRIG;
            struct w32Gbl_AVIO_TRIG;
          };
    #define   SET32Gbl_chip_debug_DBG_SEL(r32,v)               _BFSET_(r32, 7, 0,v)
    #define   SET16Gbl_chip_debug_DBG_SEL(r16,v)               _BFSET_(r16, 7, 0,v)
    #define   SET32Gbl_chip_debug_DBG_CLK_SEL(r32,v)           _BFSET_(r32,15, 8,v)
    #define   SET16Gbl_chip_debug_DBG_CLK_SEL(r16,v)           _BFSET_(r16,15, 8,v)
    #define     w32Gbl_chip_debug                              {\
            UNSG32 uchip_debug_DBG_SEL                         :  8;\
            UNSG32 uchip_debug_DBG_CLK_SEL                     :  8;\
            UNSG32 RSVDx48_b16                                 : 16;\
          }
    union { UNSG32 u32Gbl_chip_debug;
            struct w32Gbl_chip_debug;
          };
             UNSG8 RSVDx4C                                     [52];
    #define   SET32Gbl_sw_generic0_swReg0(r32,v)               _BFSET_(r32,31, 0,v)
    #define     w32Gbl_sw_generic0                             {\
            UNSG32 usw_generic0_swReg0                         : 32;\
          }
    union { UNSG32 u32Gbl_sw_generic0;
            struct w32Gbl_sw_generic0;
          };
    #define   SET32Gbl_sw_generic1_swReg1(r32,v)               _BFSET_(r32,31, 0,v)
    #define     w32Gbl_sw_generic1                             {\
            UNSG32 usw_generic1_swReg1                         : 32;\
          }
    union { UNSG32 u32Gbl_sw_generic1;
            struct w32Gbl_sw_generic1;
          };
    #define   SET32Gbl_sw_generic2_swReg2(r32,v)               _BFSET_(r32,31, 0,v)
    #define     w32Gbl_sw_generic2                             {\
            UNSG32 usw_generic2_swReg2                         : 32;\
          }
    union { UNSG32 u32Gbl_sw_generic2;
            struct w32Gbl_sw_generic2;
          };
    #define   SET32Gbl_sw_generic3_swReg3(r32,v)               _BFSET_(r32,31, 0,v)
    #define     w32Gbl_sw_generic3                             {\
            UNSG32 usw_generic3_swReg3                         : 32;\
          }
    union { UNSG32 u32Gbl_sw_generic3;
            struct w32Gbl_sw_generic3;
          };
    #define   SET32Gbl_FPGAR_FPGAR(r32,v)                      _BFSET_(r32,31, 0,v)
    #define     w32Gbl_FPGAR                                   {\
            UNSG32 uFPGAR_FPGAR                                : 32;\
          }
    union { UNSG32 u32Gbl_FPGAR;
            struct w32Gbl_FPGAR;
          };
    #define   SET32Gbl_FPGARW_FPGARW(r32,v)                    _BFSET_(r32,31, 0,v)
    #define     w32Gbl_FPGARW                                  {\
            UNSG32 uFPGARW_FPGARW                              : 32;\
          }
    union { UNSG32 u32Gbl_FPGARW;
            struct w32Gbl_FPGARW;
          };
             UNSG8 RSVDx98                                     [104];
              SIE_SRAMRWTC                                     ie_RWTC_SOC;
              SIE_SRAMPWR                                      ie_SRAM_PWR_EMMC;
              SIE_SRAMPWR                                      ie_SRAM_PWR_SDIO;
              SIE_SRAMPWR                                      ie_SRAM_PWR_SDIO1;
              SIE_SRAMPWR                                      ie_SRAM_PWR_USB2;
              SIE_SRAMPWR                                      ie_SRAM_PWR_USB2_1;
              SIE_SRAMPWR                                      ie_SRAM_PWR_GFX3D;
              SIE_SRAMPWR                                      ie_SRAM_PWR_MC;
              SIE_SRAMPWR                                      ie_SRAM_PWR_ALM;
              SIE_SRAMPWR                                      ie_SRAM_PWR_GE;
              SIE_SRAMPWR                                      ie_SRAM_PWR_GE1;
              SIE_SRAMPWR                                      ie_SRAM_PWR_NPU;
              SIE_SRAMPWR                                      ie_SRAM_PWR_NPU_CSS;
             UNSG8 RSVDx13C                                    [20];
    #define   SET32Gbl_gfx3D_pwr_ctrl_gfx3D_n_psw_sleep_en(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_gfx3D_pwr_ctrl_gfx3D_n_psw_sleep_en(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_gfx3D_pwr_ctrl_gfx3D_iso_en(r32,v)      _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_gfx3D_pwr_ctrl_gfx3D_iso_en(r16,v)      _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_gfx3D_pwr_ctrl                          {\
            UNSG32 ugfx3D_pwr_ctrl_gfx3D_n_psw_sleep_en        :  1;\
            UNSG32 ugfx3D_pwr_ctrl_gfx3D_iso_en                :  1;\
            UNSG32 RSVDx150_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_gfx3D_pwr_ctrl;
            struct w32Gbl_gfx3D_pwr_ctrl;
          };
    #define   SET32Gbl_gfx3D_pwr_sts_gfx3D_n_psw_sleep_en_out(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_gfx3D_pwr_sts_gfx3D_n_psw_sleep_en_out(r16,v) _BFSET_(r16, 0, 0,v)
    #define     w32Gbl_gfx3D_pwr_sts                           {\
            UNSG32 ugfx3D_pwr_sts_gfx3D_n_psw_sleep_en_out     :  1;\
            UNSG32 RSVDx154_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_gfx3D_pwr_sts;
            struct w32Gbl_gfx3D_pwr_sts;
          };
    #define   SET32Gbl_npu_pwr_ctrl_npu_n_psw_sleep_en(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_npu_pwr_ctrl_npu_n_psw_sleep_en(r16,v)  _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_npu_pwr_ctrl_npu_iso_en(r32,v)          _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_npu_pwr_ctrl_npu_iso_en(r16,v)          _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_npu_pwr_ctrl                            {\
            UNSG32 unpu_pwr_ctrl_npu_n_psw_sleep_en            :  1;\
            UNSG32 unpu_pwr_ctrl_npu_iso_en                    :  1;\
            UNSG32 RSVDx158_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_npu_pwr_ctrl;
            struct w32Gbl_npu_pwr_ctrl;
          };
    #define   SET32Gbl_npu_pwr_sts_npu_n_psw_sleep_en_out(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_npu_pwr_sts_npu_n_psw_sleep_en_out(r16,v) _BFSET_(r16, 0, 0,v)
    #define     w32Gbl_npu_pwr_sts                             {\
            UNSG32 unpu_pwr_sts_npu_n_psw_sleep_en_out         :  1;\
            UNSG32 RSVDx15C_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_npu_pwr_sts;
            struct w32Gbl_npu_pwr_sts;
          };
             UNSG8 RSVDx160                                    [160];
              SIE_abipll                                       ie_sysPll_0;
              SIE_abipll                                       ie_sysPll_1;
             UNSG8 RSVDx240                                    [192];
    #define   SET32Gbl_POR_EN_status_POR_EN(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_POR_EN_status_POR_EN(r16,v)             _BFSET_(r16, 0, 0,v)
    #define     w32Gbl_POR_EN_status                           {\
            UNSG32 uPOR_EN_status_POR_EN                       :  1;\
            UNSG32 RSVDx300_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_POR_EN_status;
            struct w32Gbl_POR_EN_status;
          };
    #define   SET32Gbl_POR_EN_OVRD_POR_EN_OVRD_EN(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_POR_EN_OVRD_POR_EN_OVRD_EN(r16,v)       _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_POR_EN_OVRD_POR_EN_OVRD_VAL(r32,v)      _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_POR_EN_OVRD_POR_EN_OVRD_VAL(r16,v)      _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_POR_EN_OVRD                             {\
            UNSG32 uPOR_EN_OVRD_POR_EN_OVRD_EN                 :  1;\
            UNSG32 uPOR_EN_OVRD_POR_EN_OVRD_VAL                :  1;\
            UNSG32 RSVDx304_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_POR_EN_OVRD;
            struct w32Gbl_POR_EN_OVRD;
          };
    #define   SET32Gbl_POR_status_POR_CORE_SOC(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_POR_status_POR_CORE_SOC(r16,v)          _BFSET_(r16, 0, 0,v)
    #define     w32Gbl_POR_status                              {\
            UNSG32 uPOR_status_POR_CORE_SOC                    :  1;\
            UNSG32 RSVDx308_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_POR_status;
            struct w32Gbl_POR_status;
          };
    #define   SET32Gbl_POR_CTL_SOC_CORE_BYPASS(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_POR_CTL_SOC_CORE_BYPASS(r16,v)          _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_POR_CTL_POR_SOC_CORE_PD(r32,v)          _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_POR_CTL_POR_SOC_CORE_PD(r16,v)          _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_POR_CTL                                 {\
            UNSG32 uPOR_CTL_SOC_CORE_BYPASS                    :  1;\
            UNSG32 uPOR_CTL_POR_SOC_CORE_PD                    :  1;\
            UNSG32 RSVDx30C_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_POR_CTL;
            struct w32Gbl_POR_CTL;
          };
             UNSG8 RSVDx310                                    [16];
    #define   SET32Gbl_ResetTrigger_chipReset(r32,v)           _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_ResetTrigger_chipReset(r16,v)           _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_ResetTrigger_socDdrSyncReset(r32,v)     _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_ResetTrigger_socDdrSyncReset(r16,v)     _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_ResetTrigger                            {\
            UNSG32 uResetTrigger_chipReset                     :  1;\
            UNSG32 uResetTrigger_socDdrSyncReset               :  1;\
            UNSG32 RSVDx320_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_ResetTrigger;
            struct w32Gbl_ResetTrigger;
          };
    #define   SET32Gbl_ResetStatus_ChipResetStatus(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_ResetStatus_ChipResetStatus(r16,v)      _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_ResetStatus_socDdrSyncResetStatus(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_ResetStatus_socDdrSyncResetStatus(r16,v) _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_ResetStatus                             {\
            UNSG32 uResetStatus_ChipResetStatus                :  1;\
            UNSG32 uResetStatus_socDdrSyncResetStatus          :  1;\
            UNSG32 RSVDx324_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_ResetStatus;
            struct w32Gbl_ResetStatus;
          };
             UNSG8 RSVDx328                                    [4];
    #define   SET32Gbl_WDTResetStatus_wd0Status(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_WDTResetStatus_wd0Status(r16,v)         _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_WDTResetStatus_wd1Status(r32,v)         _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_WDTResetStatus_wd1Status(r16,v)         _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_WDTResetStatus_wd2Status(r32,v)         _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_WDTResetStatus_wd2Status(r16,v)         _BFSET_(r16, 2, 2,v)
    #define     w32Gbl_WDTResetStatus                          {\
            UNSG32 uWDTResetStatus_wd0Status                   :  1;\
            UNSG32 uWDTResetStatus_wd1Status                   :  1;\
            UNSG32 uWDTResetStatus_wd2Status                   :  1;\
            UNSG32 RSVDx32C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_WDTResetStatus;
            struct w32Gbl_WDTResetStatus;
          };
    #define   SET32Gbl_WDTSysRstMask_wdt0Mask(r32,v)           _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_WDTSysRstMask_wdt0Mask(r16,v)           _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_WDTSysRstMask_wdt1Mask(r32,v)           _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_WDTSysRstMask_wdt1Mask(r16,v)           _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_WDTSysRstMask_wdt2Mask(r32,v)           _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_WDTSysRstMask_wdt2Mask(r16,v)           _BFSET_(r16, 2, 2,v)
    #define     w32Gbl_WDTSysRstMask                           {\
            UNSG32 uWDTSysRstMask_wdt0Mask                     :  1;\
            UNSG32 uWDTSysRstMask_wdt1Mask                     :  1;\
            UNSG32 uWDTSysRstMask_wdt2Mask                     :  1;\
            UNSG32 RSVDx330_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_WDTSysRstMask;
            struct w32Gbl_WDTSysRstMask;
          };
    #define   SET32Gbl_CHIP_RESET_TRACKER_VALUE(r32,v)         _BFSET_(r32,31, 0,v)
    #define     w32Gbl_CHIP_RESET_TRACKER                      {\
            UNSG32 uCHIP_RESET_TRACKER_VALUE                   : 32;\
          }
    union { UNSG32 u32Gbl_CHIP_RESET_TRACKER;
            struct w32Gbl_CHIP_RESET_TRACKER;
          };
             UNSG8 RSVDx338                                    [8];
    #define   SET32Gbl_avioReset_SyncReset(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_avioReset_SyncReset(r16,v)              _BFSET_(r16, 0, 0,v)
    #define     w32Gbl_avioReset                               {\
            UNSG32 uavioReset_SyncReset                        :  1;\
            UNSG32 RSVDx340_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_avioReset;
            struct w32Gbl_avioReset;
          };
    #define   SET32Gbl_avioResetStatus_SyncReset(r32,v)        _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_avioResetStatus_SyncReset(r16,v)        _BFSET_(r16, 0, 0,v)
    #define     w32Gbl_avioResetStatus                         {\
            UNSG32 uavioResetStatus_SyncReset                  :  1;\
            UNSG32 RSVDx344_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_avioResetStatus;
            struct w32Gbl_avioResetStatus;
          };
             UNSG8 RSVDx348                                    [8];
    #define   SET32Gbl_perifReset_SyncReset(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_perifReset_SyncReset(r16,v)             _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_perifReset_ahbApbSyncReset(r32,v)       _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_perifReset_ahbApbSyncReset(r16,v)       _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_perifReset_sdioSyncReset(r32,v)         _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_perifReset_sdioSyncReset(r16,v)         _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_perifReset_ReservedSyncReset(r32,v)     _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_perifReset_ReservedSyncReset(r16,v)     _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_perifReset_usb0SyncReset(r32,v)         _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_perifReset_usb0SyncReset(r16,v)         _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_perifReset_emmcSyncReset(r32,v)         _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_perifReset_emmcSyncReset(r16,v)         _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_perifReset_gethRgmiiSyncReset(r32,v)    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_perifReset_gethRgmiiSyncReset(r16,v)    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_perifReset_sdio1SyncReset(r32,v)        _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_perifReset_sdio1SyncReset(r16,v)        _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_perifReset_usb1SyncReset(r32,v)         _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_perifReset_usb1SyncReset(r16,v)         _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_perifReset_gethRgmii1SyncReset(r32,v)   _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_perifReset_gethRgmii1SyncReset(r16,v)   _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_perifReset                              {\
            UNSG32 uperifReset_SyncReset                       :  1;\
            UNSG32 uperifReset_ahbApbSyncReset                 :  1;\
            UNSG32 uperifReset_sdioSyncReset                   :  1;\
            UNSG32 uperifReset_ReservedSyncReset               :  1;\
            UNSG32 uperifReset_usb0SyncReset                   :  1;\
            UNSG32 uperifReset_emmcSyncReset                   :  1;\
            UNSG32 uperifReset_gethRgmiiSyncReset              :  1;\
            UNSG32 uperifReset_sdio1SyncReset                  :  1;\
            UNSG32 uperifReset_usb1SyncReset                   :  1;\
            UNSG32 uperifReset_gethRgmii1SyncReset             :  1;\
            UNSG32 RSVDx350_b10                                : 22;\
          }
    union { UNSG32 u32Gbl_perifReset;
            struct w32Gbl_perifReset;
          };
             UNSG8 RSVDx354                                    [8];
    #define   SET32Gbl_perifResetStatus_SyncReset(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_perifResetStatus_SyncReset(r16,v)       _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_perifResetStatus_ahbApbSyncReset(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_perifResetStatus_ahbApbSyncReset(r16,v) _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_perifResetStatus_sdioSyncReset(r32,v)   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_perifResetStatus_sdioSyncReset(r16,v)   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_perifResetStatus_usb0SyncReset(r32,v)   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_perifResetStatus_usb0SyncReset(r16,v)   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_perifResetStatus_emmcSyncReset(r32,v)   _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_perifResetStatus_emmcSyncReset(r16,v)   _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_perifResetStatus_gethRgmiiSyncReset(r32,v) _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_perifResetStatus_gethRgmiiSyncReset(r16,v) _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_perifResetStatus_sdio1SyncReset(r32,v)  _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_perifResetStatus_sdio1SyncReset(r16,v)  _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_perifResetStatus_usb1SyncReset(r32,v)   _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_perifResetStatus_usb1SyncReset(r16,v)   _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_perifResetStatus_gethRgmii1SyncReset(r32,v) _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_perifResetStatus_gethRgmii1SyncReset(r16,v) _BFSET_(r16, 8, 8,v)
    #define     w32Gbl_perifResetStatus                        {\
            UNSG32 uperifResetStatus_SyncReset                 :  1;\
            UNSG32 uperifResetStatus_ahbApbSyncReset           :  1;\
            UNSG32 uperifResetStatus_sdioSyncReset             :  1;\
            UNSG32 uperifResetStatus_usb0SyncReset             :  1;\
            UNSG32 uperifResetStatus_emmcSyncReset             :  1;\
            UNSG32 uperifResetStatus_gethRgmiiSyncReset        :  1;\
            UNSG32 uperifResetStatus_sdio1SyncReset            :  1;\
            UNSG32 uperifResetStatus_usb1SyncReset             :  1;\
            UNSG32 uperifResetStatus_gethRgmii1SyncReset       :  1;\
            UNSG32 RSVDx35C_b9                                 : 23;\
          }
    union { UNSG32 u32Gbl_perifResetStatus;
            struct w32Gbl_perifResetStatus;
          };
    #define   SET32Gbl_perifStickyResetN_usb0PhyRstn(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_perifStickyResetN_usb0PhyRstn(r16,v)    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_perifStickyResetN_usb0CoreRstn(r32,v)   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_perifStickyResetN_usb0CoreRstn(r16,v)   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_perifStickyResetN_usb0MahbRstn(r32,v)   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_perifStickyResetN_usb0MahbRstn(r16,v)   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_perifStickyResetN_usb1PhyRstn(r32,v)    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_perifStickyResetN_usb1PhyRstn(r16,v)    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_perifStickyResetN_usb1CoreRstn(r32,v)   _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_perifStickyResetN_usb1CoreRstn(r16,v)   _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_perifStickyResetN_usb1MahbRstn(r32,v)   _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_perifStickyResetN_usb1MahbRstn(r16,v)   _BFSET_(r16, 5, 5,v)
    #define     w32Gbl_perifStickyResetN                       {\
            UNSG32 uperifStickyResetN_usb0PhyRstn              :  1;\
            UNSG32 uperifStickyResetN_usb0CoreRstn             :  1;\
            UNSG32 uperifStickyResetN_usb0MahbRstn             :  1;\
            UNSG32 uperifStickyResetN_usb1PhyRstn              :  1;\
            UNSG32 uperifStickyResetN_usb1CoreRstn             :  1;\
            UNSG32 uperifStickyResetN_usb1MahbRstn             :  1;\
            UNSG32 RSVDx360_b6                                 : 26;\
          }
    union { UNSG32 u32Gbl_perifStickyResetN;
            struct w32Gbl_perifStickyResetN;
          };
             UNSG8 RSVDx364                                    [12];
    #define   SET32Gbl_apbPerifResetTrigger_uart0SyncReset(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_apbPerifResetTrigger_uart0SyncReset(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_apbPerifResetTrigger_uart1SyncReset(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_apbPerifResetTrigger_uart1SyncReset(r16,v) _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_apbPerifResetTrigger_uart2SyncReset(r32,v) _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_apbPerifResetTrigger_uart2SyncReset(r16,v) _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_apbPerifResetTrigger_uart3SyncReset(r32,v) _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_apbPerifResetTrigger_uart3SyncReset(r16,v) _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_apbPerifResetTrigger_i2c0SyncReset(r32,v) _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_apbPerifResetTrigger_i2c0SyncReset(r16,v) _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_apbPerifResetTrigger_i2c1SyncReset(r32,v) _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_apbPerifResetTrigger_i2c1SyncReset(r16,v) _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_apbPerifResetTrigger_spi0SyncReset(r32,v) _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_apbPerifResetTrigger_spi0SyncReset(r16,v) _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_apbPerifResetTrigger_spi1SyncReset(r32,v) _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_apbPerifResetTrigger_spi1SyncReset(r16,v) _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_apbPerifResetTrigger_spi2SyncReset(r32,v) _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_apbPerifResetTrigger_spi2SyncReset(r16,v) _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_apbPerifResetTrigger_spi3SyncReset(r32,v) _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_apbPerifResetTrigger_spi3SyncReset(r16,v) _BFSET_(r16, 9, 9,v)
    #define   SET32Gbl_apbPerifResetTrigger_apbTimersSyncReset(r32,v) _BFSET_(r32,10,10,v)
    #define   SET16Gbl_apbPerifResetTrigger_apbTimersSyncReset(r16,v) _BFSET_(r16,10,10,v)
    #define   SET32Gbl_apbPerifResetTrigger_apbSysCntSyncReset(r32,v) _BFSET_(r32,11,11,v)
    #define   SET16Gbl_apbPerifResetTrigger_apbSysCntSyncReset(r16,v) _BFSET_(r16,11,11,v)
    #define   SET32Gbl_apbPerifResetTrigger_apbWDTSyncReset(r32,v) _BFSET_(r32,12,12,v)
    #define   SET16Gbl_apbPerifResetTrigger_apbWDTSyncReset(r16,v) _BFSET_(r16,12,12,v)
    #define   SET32Gbl_apbPerifResetTrigger_apbGPIOSyncReset(r32,v) _BFSET_(r32,13,13,v)
    #define   SET16Gbl_apbPerifResetTrigger_apbGPIOSyncReset(r16,v) _BFSET_(r16,13,13,v)
    #define   SET32Gbl_apbPerifResetTrigger_apbDmaSyncReset(r32,v) _BFSET_(r32,14,14,v)
    #define   SET16Gbl_apbPerifResetTrigger_apbDmaSyncReset(r16,v) _BFSET_(r16,14,14,v)
    #define     w32Gbl_apbPerifResetTrigger                    {\
            UNSG32 uapbPerifResetTrigger_uart0SyncReset        :  1;\
            UNSG32 uapbPerifResetTrigger_uart1SyncReset        :  1;\
            UNSG32 uapbPerifResetTrigger_uart2SyncReset        :  1;\
            UNSG32 uapbPerifResetTrigger_uart3SyncReset        :  1;\
            UNSG32 uapbPerifResetTrigger_i2c0SyncReset         :  1;\
            UNSG32 uapbPerifResetTrigger_i2c1SyncReset         :  1;\
            UNSG32 uapbPerifResetTrigger_spi0SyncReset         :  1;\
            UNSG32 uapbPerifResetTrigger_spi1SyncReset         :  1;\
            UNSG32 uapbPerifResetTrigger_spi2SyncReset         :  1;\
            UNSG32 uapbPerifResetTrigger_spi3SyncReset         :  1;\
            UNSG32 uapbPerifResetTrigger_apbTimersSyncReset    :  1;\
            UNSG32 uapbPerifResetTrigger_apbSysCntSyncReset    :  1;\
            UNSG32 uapbPerifResetTrigger_apbWDTSyncReset       :  1;\
            UNSG32 uapbPerifResetTrigger_apbGPIOSyncReset      :  1;\
            UNSG32 uapbPerifResetTrigger_apbDmaSyncReset       :  1;\
            UNSG32 RSVDx370_b15                                : 17;\
          }
    union { UNSG32 u32Gbl_apbPerifResetTrigger;
            struct w32Gbl_apbPerifResetTrigger;
          };
    #define   SET32Gbl_apbPerifResetStatus_uart0SyncResetStatus(r32,v) _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_apbPerifResetStatus_uart0SyncResetStatus(r16,v) _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_apbPerifResetStatus_uart1SyncResetStatus(r32,v) _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_apbPerifResetStatus_uart1SyncResetStatus(r16,v) _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_apbPerifResetStatus_uart2SyncResetStatus(r32,v) _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_apbPerifResetStatus_uart2SyncResetStatus(r16,v) _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_apbPerifResetStatus_uart3SyncResetStatus(r32,v) _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_apbPerifResetStatus_uart3SyncResetStatus(r16,v) _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_apbPerifResetStatus_i2c0SyncResetStatus(r32,v) _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_apbPerifResetStatus_i2c0SyncResetStatus(r16,v) _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_apbPerifResetStatus_i2c1SyncResetStatus(r32,v) _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_apbPerifResetStatus_i2c1SyncResetStatus(r16,v) _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_apbPerifResetStatus_spi0SyncResetStatus(r32,v) _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_apbPerifResetStatus_spi0SyncResetStatus(r16,v) _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_apbPerifResetStatus_spi1SyncResetStatus(r32,v) _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_apbPerifResetStatus_spi1SyncResetStatus(r16,v) _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_apbPerifResetStatus_spi2SyncResetStatus(r32,v) _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_apbPerifResetStatus_spi2SyncResetStatus(r16,v) _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_apbPerifResetStatus_spi3SyncResetStatus(r32,v) _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_apbPerifResetStatus_spi3SyncResetStatus(r16,v) _BFSET_(r16, 9, 9,v)
    #define   SET32Gbl_apbPerifResetStatus_apbTimersSyncResetStatus(r32,v) _BFSET_(r32,10,10,v)
    #define   SET16Gbl_apbPerifResetStatus_apbTimersSyncResetStatus(r16,v) _BFSET_(r16,10,10,v)
    #define   SET32Gbl_apbPerifResetStatus_apbSysCntSyncResetStatus(r32,v) _BFSET_(r32,11,11,v)
    #define   SET16Gbl_apbPerifResetStatus_apbSysCntSyncResetStatus(r16,v) _BFSET_(r16,11,11,v)
    #define   SET32Gbl_apbPerifResetStatus_apbWDTSyncResetStatus(r32,v) _BFSET_(r32,12,12,v)
    #define   SET16Gbl_apbPerifResetStatus_apbWDTSyncResetStatus(r16,v) _BFSET_(r16,12,12,v)
    #define   SET32Gbl_apbPerifResetStatus_apbGPIOSyncResetStatus(r32,v) _BFSET_(r32,13,13,v)
    #define   SET16Gbl_apbPerifResetStatus_apbGPIOSyncResetStatus(r16,v) _BFSET_(r16,13,13,v)
    #define   SET32Gbl_apbPerifResetStatus_apbDmaSyncResetStatus(r32,v) _BFSET_(r32,14,14,v)
    #define   SET16Gbl_apbPerifResetStatus_apbDmaSyncResetStatus(r16,v) _BFSET_(r16,14,14,v)
    #define     w32Gbl_apbPerifResetStatus                     {\
            UNSG32 uapbPerifResetStatus_uart0SyncResetStatus   :  1;\
            UNSG32 uapbPerifResetStatus_uart1SyncResetStatus   :  1;\
            UNSG32 uapbPerifResetStatus_uart2SyncResetStatus   :  1;\
            UNSG32 uapbPerifResetStatus_uart3SyncResetStatus   :  1;\
            UNSG32 uapbPerifResetStatus_i2c0SyncResetStatus    :  1;\
            UNSG32 uapbPerifResetStatus_i2c1SyncResetStatus    :  1;\
            UNSG32 uapbPerifResetStatus_spi0SyncResetStatus    :  1;\
            UNSG32 uapbPerifResetStatus_spi1SyncResetStatus    :  1;\
            UNSG32 uapbPerifResetStatus_spi2SyncResetStatus    :  1;\
            UNSG32 uapbPerifResetStatus_spi3SyncResetStatus    :  1;\
            UNSG32 uapbPerifResetStatus_apbTimersSyncResetStatus :  1;\
            UNSG32 uapbPerifResetStatus_apbSysCntSyncResetStatus :  1;\
            UNSG32 uapbPerifResetStatus_apbWDTSyncResetStatus  :  1;\
            UNSG32 uapbPerifResetStatus_apbGPIOSyncResetStatus :  1;\
            UNSG32 uapbPerifResetStatus_apbDmaSyncResetStatus  :  1;\
            UNSG32 RSVDx374_b15                                : 17;\
          }
    union { UNSG32 u32Gbl_apbPerifResetStatus;
            struct w32Gbl_apbPerifResetStatus;
          };
    #define   SET32Gbl_topStickyResetN_gpuCoreRstn(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_topStickyResetN_gpuCoreRstn(r16,v)      _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_topStickyResetN_npuCoreRstn(r32,v)      _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_topStickyResetN_npuCoreRstn(r16,v)      _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_topStickyResetN                         {\
            UNSG32 utopStickyResetN_gpuCoreRstn                :  1;\
            UNSG32 utopStickyResetN_npuCoreRstn                :  1;\
            UNSG32 RSVDx378_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_topStickyResetN;
            struct w32Gbl_topStickyResetN;
          };
    #define   SET32Gbl_avioStickyResetN_avioAIOGRstn(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_avioStickyResetN_avioAIOGRstn(r16,v)    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_avioStickyResetN_avioVPPGRstn(r32,v)    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_avioStickyResetN_avioVPPGRstn(r16,v)    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_avioStickyResetN_avioVIPGRstn(r32,v)    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_avioStickyResetN_avioVIPGRstn(r16,v)    _BFSET_(r16, 2, 2,v)
    #define     w32Gbl_avioStickyResetN                        {\
            UNSG32 uavioStickyResetN_avioAIOGRstn              :  1;\
            UNSG32 uavioStickyResetN_avioVPPGRstn              :  1;\
            UNSG32 uavioStickyResetN_avioVIPGRstn              :  1;\
            UNSG32 RSVDx37C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_avioStickyResetN;
            struct w32Gbl_avioStickyResetN;
          };
             UNSG8 RSVDx380                                    [128];
    #define   SET32Gbl_ClkSwitch_sysPLL_0SWBypass(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_ClkSwitch_sysPLL_0SWBypass(r16,v)       _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_ClkSwitch_sysPLL_1SWBypass(r32,v)       _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_ClkSwitch_sysPLL_1SWBypass(r16,v)       _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_ClkSwitch_memPLLSWBypass(r32,v)         _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_ClkSwitch_memPLLSWBypass(r16,v)         _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_ClkSwitch_cpuPLLSWBypass(r32,v)         _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_ClkSwitch_cpuPLLSWBypass(r16,v)         _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_ClkSwitch_aPLL_0SWBypass(r32,v)         _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_ClkSwitch_aPLL_0SWBypass(r16,v)         _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_ClkSwitch_aPLL_1SWBypass(r32,v)         _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_ClkSwitch_aPLL_1SWBypass(r16,v)         _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_ClkSwitch_vPLL_0SWBypass(r32,v)         _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_ClkSwitch_vPLL_0SWBypass(r16,v)         _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_ClkSwitch_vPLL_1SWBypass(r32,v)         _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_ClkSwitch_vPLL_1SWBypass(r16,v)         _BFSET_(r16, 7, 7,v)
    #define     w32Gbl_ClkSwitch                               {\
            UNSG32 uClkSwitch_sysPLL_0SWBypass                 :  1;\
            UNSG32 uClkSwitch_sysPLL_1SWBypass                 :  1;\
            UNSG32 uClkSwitch_memPLLSWBypass                   :  1;\
            UNSG32 uClkSwitch_cpuPLLSWBypass                   :  1;\
            UNSG32 uClkSwitch_aPLL_0SWBypass                   :  1;\
            UNSG32 uClkSwitch_aPLL_1SWBypass                   :  1;\
            UNSG32 uClkSwitch_vPLL_0SWBypass                   :  1;\
            UNSG32 uClkSwitch_vPLL_1SWBypass                   :  1;\
            UNSG32 RSVDx400_b8                                 : 24;\
          }
    union { UNSG32 u32Gbl_ClkSwitch;
            struct w32Gbl_ClkSwitch;
          };
             UNSG8 RSVDx404                                    [28];
    #define   SET32Gbl_clkEnable_usb0CoreClkEn(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_clkEnable_usb0CoreClkEn(r16,v)          _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_clkEnable_sdioSysClkEn(r32,v)           _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_clkEnable_sdioSysClkEn(r16,v)           _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_clkEnable_emmcSysClkEn(r32,v)           _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_clkEnable_emmcSysClkEn(r16,v)           _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_clkEnable_gpuAxiClkEn(r32,v)            _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_clkEnable_gpuAxiClkEn(r16,v)            _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_clkEnable_gethRgmiiSysClkEn(r32,v)      _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_clkEnable_gethRgmiiSysClkEn(r16,v)      _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_clkEnable_sdio1SysClkEn(r32,v)          _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_clkEnable_sdio1SysClkEn(r16,v)          _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_clkEnable_usb1CoreClkEn(r32,v)          _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_clkEnable_usb1CoreClkEn(r16,v)          _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_clkEnable_gethRgmii1SysClkEn(r32,v)     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_clkEnable_gethRgmii1SysClkEn(r16,v)     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_clkEnable_usb0PhyRefClkEn(r32,v)        _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_clkEnable_usb0PhyRefClkEn(r16,v)        _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_clkEnable_usb1PhyRefClkEn(r32,v)        _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_clkEnable_usb1PhyRefClkEn(r16,v)        _BFSET_(r16, 9, 9,v)
    #define   SET32Gbl_clkEnable_apbUART0ClkEn(r32,v)          _BFSET_(r32,10,10,v)
    #define   SET16Gbl_clkEnable_apbUART0ClkEn(r16,v)          _BFSET_(r16,10,10,v)
    #define   SET32Gbl_clkEnable_apbUART1ClkEn(r32,v)          _BFSET_(r32,11,11,v)
    #define   SET16Gbl_clkEnable_apbUART1ClkEn(r16,v)          _BFSET_(r16,11,11,v)
    #define   SET32Gbl_clkEnable_apbUART2ClkEn(r32,v)          _BFSET_(r32,12,12,v)
    #define   SET16Gbl_clkEnable_apbUART2ClkEn(r16,v)          _BFSET_(r16,12,12,v)
    #define   SET32Gbl_clkEnable_apbUART3ClkEn(r32,v)          _BFSET_(r32,13,13,v)
    #define   SET16Gbl_clkEnable_apbUART3ClkEn(r16,v)          _BFSET_(r16,13,13,v)
    #define   SET32Gbl_clkEnable_apbI2C0ClkEn(r32,v)           _BFSET_(r32,14,14,v)
    #define   SET16Gbl_clkEnable_apbI2C0ClkEn(r16,v)           _BFSET_(r16,14,14,v)
    #define   SET32Gbl_clkEnable_apbI2C1ClkEn(r32,v)           _BFSET_(r32,15,15,v)
    #define   SET16Gbl_clkEnable_apbI2C1ClkEn(r16,v)           _BFSET_(r16,15,15,v)
    #define   SET32Gbl_clkEnable_apbSPI0ClkEn(r32,v)           _BFSET_(r32,16,16,v)
    #define   SET16Gbl_clkEnable_apbSPI0ClkEn(r16,v)           _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_clkEnable_apbSPI1ClkEn(r32,v)           _BFSET_(r32,17,17,v)
    #define   SET16Gbl_clkEnable_apbSPI1ClkEn(r16,v)           _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_clkEnable_apbSPI2ClkEn(r32,v)           _BFSET_(r32,18,18,v)
    #define   SET16Gbl_clkEnable_apbSPI2ClkEn(r16,v)           _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_clkEnable_apbSPI3ClkEn(r32,v)           _BFSET_(r32,19,19,v)
    #define   SET16Gbl_clkEnable_apbSPI3ClkEn(r16,v)           _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_clkEnable_apbGPIOClkEn(r32,v)           _BFSET_(r32,20,20,v)
    #define   SET16Gbl_clkEnable_apbGPIOClkEn(r16,v)           _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_clkEnable_apbTimersClkEn(r32,v)         _BFSET_(r32,21,21,v)
    #define   SET16Gbl_clkEnable_apbTimersClkEn(r16,v)         _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_clkEnable_apbSysCntClkEn(r32,v)         _BFSET_(r32,22,22,v)
    #define   SET16Gbl_clkEnable_apbSysCntClkEn(r16,v)         _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_clkEnable_apbWDTClkEn(r32,v)            _BFSET_(r32,23,23,v)
    #define   SET16Gbl_clkEnable_apbWDTClkEn(r16,v)            _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_clkEnable_apbDmaClkEn(r32,v)            _BFSET_(r32,24,24,v)
    #define   SET16Gbl_clkEnable_apbDmaClkEn(r16,v)            _BFSET_(r16, 8, 8,v)
    #define     w32Gbl_clkEnable                               {\
            UNSG32 uclkEnable_usb0CoreClkEn                    :  1;\
            UNSG32 uclkEnable_sdioSysClkEn                     :  1;\
            UNSG32 uclkEnable_emmcSysClkEn                     :  1;\
            UNSG32 uclkEnable_gpuAxiClkEn                      :  1;\
            UNSG32 uclkEnable_gethRgmiiSysClkEn                :  1;\
            UNSG32 uclkEnable_sdio1SysClkEn                    :  1;\
            UNSG32 uclkEnable_usb1CoreClkEn                    :  1;\
            UNSG32 uclkEnable_gethRgmii1SysClkEn               :  1;\
            UNSG32 uclkEnable_usb0PhyRefClkEn                  :  1;\
            UNSG32 uclkEnable_usb1PhyRefClkEn                  :  1;\
            UNSG32 uclkEnable_apbUART0ClkEn                    :  1;\
            UNSG32 uclkEnable_apbUART1ClkEn                    :  1;\
            UNSG32 uclkEnable_apbUART2ClkEn                    :  1;\
            UNSG32 uclkEnable_apbUART3ClkEn                    :  1;\
            UNSG32 uclkEnable_apbI2C0ClkEn                     :  1;\
            UNSG32 uclkEnable_apbI2C1ClkEn                     :  1;\
            UNSG32 uclkEnable_apbSPI0ClkEn                     :  1;\
            UNSG32 uclkEnable_apbSPI1ClkEn                     :  1;\
            UNSG32 uclkEnable_apbSPI2ClkEn                     :  1;\
            UNSG32 uclkEnable_apbSPI3ClkEn                     :  1;\
            UNSG32 uclkEnable_apbGPIOClkEn                     :  1;\
            UNSG32 uclkEnable_apbTimersClkEn                   :  1;\
            UNSG32 uclkEnable_apbSysCntClkEn                   :  1;\
            UNSG32 uclkEnable_apbWDTClkEn                      :  1;\
            UNSG32 uclkEnable_apbDmaClkEn                      :  1;\
            UNSG32 RSVDx420_b25                                :  7;\
          }
    union { UNSG32 u32Gbl_clkEnable;
            struct w32Gbl_clkEnable;
          };
             UNSG8 RSVDx424                                    [28];
              SIE_clkD2                                        ie_cpufastRefClk;
              SIE_clkD2                                        ie_memfastRefClk;
              SIE_clkD8                                        ie_cfgClk;
              SIE_clkD4                                        ie_sysClk;
              SIE_clkD4                                        ie_perifSysClk;
              SIE_clkD8                                        ie_apbCoreClk;
              SIE_clkD8                                        ie_apbSerClk;
              SIE_clkD12                                       ie_atbClk;
              SIE_clkD2                                        ie_hpcClk;
              SIE_clkD4                                        ie_emmcClk;
              SIE_clkD8_ENOFF                                  ie_sd0Clk;
              SIE_clkD8_ENOFF                                  ie_sd1Clk;
              SIE_clkD4_ENOFF                                  ie_gethRgmiiClk;
              SIE_clkD4_ENOFF                                  ie_gethRgmii1Clk;
              SIE_clkD1_ENOFF                                  ie_ge0_ptp_refClk;
              SIE_clkD1_ENOFF                                  ie_ge1_ptp_refClk;
              SIE_clkD12_ENOFF                                 ie_usb2TestClk;
              SIE_clkD12_ENOFF                                 ie_usb2TestClk480mGroup0;
              SIE_clkD12_ENOFF                                 ie_usb2TestClk480mGroup1;
              SIE_clkD12_ENOFF                                 ie_usb2TestClk480mGroup2;
              SIE_clkD12_ENOFF                                 ie_usb2TestClk100mGroup0;
              SIE_clkD12_ENOFF                                 ie_usb2TestClk100mGroup1;
              SIE_clkD12_ENOFF                                 ie_usb2TestClk100mGroup2;
              SIE_clkD12_ENOFF                                 ie_usb2TestClk100mGroup3;
              SIE_clkD12_ENOFF                                 ie_perifTestClk125mGroup0;
              SIE_clkD12_ENOFF                                 ie_perifTestClk200mGroup0;
              SIE_clkD12_ENOFF                                 ie_perifTestClk200mGroup1;
              SIE_clkD4_ENOFF                                  ie_gpuClk;
              SIE_clkD4                                        ie_npuClk;
              SIE_clkD4_ENOFF                                  ie_avioSysClk;
              SIE_clkD4_ENOFF                                  ie_aioSysClk;
              SIE_clkD4_ENOFF                                  ie_avio_lcdc2ScanClk;
              SIE_clkD4_ENOFF                                  ie_avio_ipiClk;
              SIE_clkD8_ENOFF                                  ie_avio_pClk;
              SIE_clkD4_ENOFF                                  ie_avio_dphyrxtxescClk;
              SIE_clkD4_ENOFF                                  ie_avioFpllClk;
              SIE_clkD4_ENOFF                                  ie_avio_rx_scanbyteClk;
              SIE_clkD8_ENOFF                                  ie_avio_rx_scantestClk;
             UNSG8 RSVDx4D8                                    [8];
    #define   SET32Gbl_USBOTG_REFCLK_CTRL0_divval(r32,v)       _BFSET_(r32,31, 0,v)
    #define     w32Gbl_USBOTG_REFCLK_CTRL0                     {\
            UNSG32 uUSBOTG_REFCLK_CTRL0_divval                 : 32;\
          }
    union { UNSG32 u32Gbl_USBOTG_REFCLK_CTRL0;
            struct w32Gbl_USBOTG_REFCLK_CTRL0;
          };
    #define   SET32Gbl_USBOTG_REFCLK_CTRL1_lowcnt(r32,v)       _BFSET_(r32,31, 0,v)
    #define     w32Gbl_USBOTG_REFCLK_CTRL1                     {\
            UNSG32 uUSBOTG_REFCLK_CTRL1_lowcnt                 : 32;\
          }
    union { UNSG32 u32Gbl_USBOTG_REFCLK_CTRL1;
            struct w32Gbl_USBOTG_REFCLK_CTRL1;
          };
    #define   SET32Gbl_USBOTG1_REFCLK_CTRL0_divval(r32,v)      _BFSET_(r32,31, 0,v)
    #define     w32Gbl_USBOTG1_REFCLK_CTRL0                    {\
            UNSG32 uUSBOTG1_REFCLK_CTRL0_divval                : 32;\
          }
    union { UNSG32 u32Gbl_USBOTG1_REFCLK_CTRL0;
            struct w32Gbl_USBOTG1_REFCLK_CTRL0;
          };
    #define   SET32Gbl_USBOTG1_REFCLK_CTRL1_lowcnt(r32,v)      _BFSET_(r32,31, 0,v)
    #define     w32Gbl_USBOTG1_REFCLK_CTRL1                    {\
            UNSG32 uUSBOTG1_REFCLK_CTRL1_lowcnt                : 32;\
          }
    union { UNSG32 u32Gbl_USBOTG1_REFCLK_CTRL1;
            struct w32Gbl_USBOTG1_REFCLK_CTRL1;
          };
             UNSG8 RSVDx4F0                                    [16];
    #define   SET32Gbl_SECURE_SCAN_EN_SET(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_SECURE_SCAN_EN_SET(r16,v)               _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_SECURE_SCAN_EN_drcg(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_SECURE_SCAN_EN_drcg(r16,v)              _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_SECURE_SCAN_EN                          {\
            UNSG32 uSECURE_SCAN_EN_SET                         :  1;\
            UNSG32 uSECURE_SCAN_EN_drcg                        :  1;\
            UNSG32 RSVDx500_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_SECURE_SCAN_EN;
            struct w32Gbl_SECURE_SCAN_EN;
          };
             UNSG8 RSVDx504                                    [92];
    #define   SET32Gbl_gic400_ctrl_cfgsdisable(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_gic400_ctrl_cfgsdisable(r16,v)          _BFSET_(r16, 0, 0,v)
    #define     w32Gbl_gic400_ctrl                             {\
            UNSG32 ugic400_ctrl_cfgsdisable                    :  1;\
            UNSG32 RSVDx560_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_gic400_ctrl;
            struct w32Gbl_gic400_ctrl;
          };
             UNSG8 RSVDx564                                    [108];
    #define   SET32Gbl_LCDD_IO_CTRL_MS(r32,v)                  _BFSET_(r32, 1, 0,v)
    #define   SET16Gbl_LCDD_IO_CTRL_MS(r16,v)                  _BFSET_(r16, 1, 0,v)
    #define     w32Gbl_LCDD_IO_CTRL                            {\
            UNSG32 uLCDD_IO_CTRL_MS                            :  2;\
            UNSG32 RSVDx5D0_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_LCDD_IO_CTRL;
            struct w32Gbl_LCDD_IO_CTRL;
          };
    #define   SET32Gbl_SOC_PLL_MUX_src_clk_sel(r32,v)          _BFSET_(r32, 2, 0,v)
    #define   SET16Gbl_SOC_PLL_MUX_src_clk_sel(r16,v)          _BFSET_(r16, 2, 0,v)
    #define     w32Gbl_SOC_PLL_MUX                             {\
            UNSG32 uSOC_PLL_MUX_src_clk_sel                    :  3;\
            UNSG32 RSVDx5D4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SOC_PLL_MUX;
            struct w32Gbl_SOC_PLL_MUX;
          };
    #define   SET32Gbl_ge0_ptp_mux_syspll0_clk_mux_sel(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_ge0_ptp_mux_syspll0_clk_mux_sel(r16,v)  _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_ge0_ptp_mux_syspll1_clk_mux_sel(r32,v)  _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_ge0_ptp_mux_syspll1_clk_mux_sel(r16,v)  _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_ge0_ptp_mux                             {\
            UNSG32 uge0_ptp_mux_syspll0_clk_mux_sel            :  1;\
            UNSG32 uge0_ptp_mux_syspll1_clk_mux_sel            :  1;\
            UNSG32 RSVDx5D8_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_ge0_ptp_mux;
            struct w32Gbl_ge0_ptp_mux;
          };
    #define   SET32Gbl_ge1_ptp_mux_syspll0_clk_mux_sel(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_ge1_ptp_mux_syspll0_clk_mux_sel(r16,v)  _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_ge1_ptp_mux_syspll1_clk_mux_sel(r32,v)  _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_ge1_ptp_mux_syspll1_clk_mux_sel(r16,v)  _BFSET_(r16, 1, 1,v)
    #define     w32Gbl_ge1_ptp_mux                             {\
            UNSG32 uge1_ptp_mux_syspll0_clk_mux_sel            :  1;\
            UNSG32 uge1_ptp_mux_syspll1_clk_mux_sel            :  1;\
            UNSG32 RSVDx5DC_b2                                 : 30;\
          }
    union { UNSG32 u32Gbl_ge1_ptp_mux;
            struct w32Gbl_ge1_ptp_mux;
          };
    #define   SET32Gbl_gfx_3d_PROTMODEM0(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_gfx_3d_PROTMODEM0(r16,v)                _BFSET_(r16, 0, 0,v)
    #define     w32Gbl_gfx_3d                                  {\
            UNSG32 ugfx_3d_PROTMODEM0                          :  1;\
            UNSG32 RSVDx5E0_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_gfx_3d;
            struct w32Gbl_gfx_3d;
          };
    #define   SET32Gbl_pdma_dst_req_mask_value(r32,v)          _BFSET_(r32,31, 0,v)
    #define     w32Gbl_pdma_dst_req_mask                       {\
            UNSG32 updma_dst_req_mask_value                    : 32;\
          }
    union { UNSG32 u32Gbl_pdma_dst_req_mask;
            struct w32Gbl_pdma_dst_req_mask;
          };
    #define   SET32Gbl_pdma_src_req_mask_value(r32,v)          _BFSET_(r32,31, 0,v)
    #define     w32Gbl_pdma_src_req_mask                       {\
            UNSG32 updma_src_req_mask_value                    : 32;\
          }
    union { UNSG32 u32Gbl_pdma_src_req_mask;
            struct w32Gbl_pdma_src_req_mask;
          };
             UNSG8 RSVDx5EC                                    [1044];
              SIE_PERIF                                        ie_PERIF;
             UNSG8 RSVDxAC0                                    [64];
              SIE_PLL_COUNTER                                  ie_PLL_COUNTER;
             UNSG8 RSVDxB0C                                    [29940];
    #define   SET32Gbl_pinmux_cntl_bus_GPIO23(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO23(r16,v)           _BFSET_(r16, 2, 0,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO24(r32,v)           _BFSET_(r32, 5, 3,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO24(r16,v)           _BFSET_(r16, 5, 3,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO25(r32,v)           _BFSET_(r32, 8, 6,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO25(r16,v)           _BFSET_(r16, 8, 6,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO26(r32,v)           _BFSET_(r32,11, 9,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO26(r16,v)           _BFSET_(r16,11, 9,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO27(r32,v)           _BFSET_(r32,14,12,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO27(r16,v)           _BFSET_(r16,14,12,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO28(r32,v)           _BFSET_(r32,17,15,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO29(r32,v)           _BFSET_(r32,20,18,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO29(r16,v)           _BFSET_(r16, 4, 2,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO30(r32,v)           _BFSET_(r32,23,21,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO30(r16,v)           _BFSET_(r16, 7, 5,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO31(r32,v)           _BFSET_(r32,26,24,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO31(r16,v)           _BFSET_(r16,10, 8,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO32(r32,v)           _BFSET_(r32,29,27,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO32(r16,v)           _BFSET_(r16,13,11,v)
    #define     w32Gbl_pinmux_cntl_bus                         {\
            UNSG32 upinmux_cntl_bus_GPIO23                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO24                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO25                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO26                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO27                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO28                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO29                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO30                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO31                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO32                     :  3;\
            UNSG32 RSVDx8000_b30                               :  2;\
          }
    union { UNSG32 u32Gbl_pinmux_cntl_bus;
            struct w32Gbl_pinmux_cntl_bus;
          };
    #define   SET32Gbl_pinmux_cntl_bus_GPIO33(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO33(r16,v)           _BFSET_(r16, 2, 0,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO34(r32,v)           _BFSET_(r32, 5, 3,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO34(r16,v)           _BFSET_(r16, 5, 3,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO35(r32,v)           _BFSET_(r32, 8, 6,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO35(r16,v)           _BFSET_(r16, 8, 6,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO36(r32,v)           _BFSET_(r32,11, 9,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO36(r16,v)           _BFSET_(r16,11, 9,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO37(r32,v)           _BFSET_(r32,14,12,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO37(r16,v)           _BFSET_(r16,14,12,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO38(r32,v)           _BFSET_(r32,17,15,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO39(r32,v)           _BFSET_(r32,20,18,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO39(r16,v)           _BFSET_(r16, 4, 2,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO40(r32,v)           _BFSET_(r32,23,21,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO40(r16,v)           _BFSET_(r16, 7, 5,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO41(r32,v)           _BFSET_(r32,26,24,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO41(r16,v)           _BFSET_(r16,10, 8,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO42(r32,v)           _BFSET_(r32,29,27,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO42(r16,v)           _BFSET_(r16,13,11,v)
    #define     w32Gbl_pinmux_cntl_bus1                        {\
            UNSG32 upinmux_cntl_bus_GPIO33                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO34                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO35                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO36                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO37                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO38                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO39                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO40                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO41                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO42                     :  3;\
            UNSG32 RSVDx8004_b30                               :  2;\
          }
    union { UNSG32 u32Gbl_pinmux_cntl_bus1;
            struct w32Gbl_pinmux_cntl_bus1;
          };
    #define   SET32Gbl_pinmux_cntl_bus_GPIO43(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO43(r16,v)           _BFSET_(r16, 2, 0,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO44(r32,v)           _BFSET_(r32, 5, 3,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO44(r16,v)           _BFSET_(r16, 5, 3,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO45(r32,v)           _BFSET_(r32, 8, 6,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO45(r16,v)           _BFSET_(r16, 8, 6,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO46(r32,v)           _BFSET_(r32,11, 9,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO46(r16,v)           _BFSET_(r16,11, 9,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO47(r32,v)           _BFSET_(r32,14,12,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO47(r16,v)           _BFSET_(r16,14,12,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO48(r32,v)           _BFSET_(r32,17,15,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO49(r32,v)           _BFSET_(r32,20,18,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO49(r16,v)           _BFSET_(r16, 4, 2,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO50(r32,v)           _BFSET_(r32,23,21,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO50(r16,v)           _BFSET_(r16, 7, 5,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO51(r32,v)           _BFSET_(r32,26,24,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO51(r16,v)           _BFSET_(r16,10, 8,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO52(r32,v)           _BFSET_(r32,29,27,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO52(r16,v)           _BFSET_(r16,13,11,v)
    #define     w32Gbl_pinmux_cntl_bus2                        {\
            UNSG32 upinmux_cntl_bus_GPIO43                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO44                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO45                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO46                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO47                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO48                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO49                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO50                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO51                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO52                     :  3;\
            UNSG32 RSVDx8008_b30                               :  2;\
          }
    union { UNSG32 u32Gbl_pinmux_cntl_bus2;
            struct w32Gbl_pinmux_cntl_bus2;
          };
    #define   SET32Gbl_pinmux_cntl_bus_GPIO53(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO53(r16,v)           _BFSET_(r16, 2, 0,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO54(r32,v)           _BFSET_(r32, 5, 3,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO54(r16,v)           _BFSET_(r16, 5, 3,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO55(r32,v)           _BFSET_(r32, 8, 6,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO55(r16,v)           _BFSET_(r16, 8, 6,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO56(r32,v)           _BFSET_(r32,11, 9,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO56(r16,v)           _BFSET_(r16,11, 9,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO57(r32,v)           _BFSET_(r32,14,12,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO57(r16,v)           _BFSET_(r16,14,12,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO58(r32,v)           _BFSET_(r32,17,15,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO59(r32,v)           _BFSET_(r32,20,18,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO59(r16,v)           _BFSET_(r16, 4, 2,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO0(r32,v)            _BFSET_(r32,23,21,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO0(r16,v)            _BFSET_(r16, 7, 5,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO1(r32,v)            _BFSET_(r32,26,24,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO1(r16,v)            _BFSET_(r16,10, 8,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO2(r32,v)            _BFSET_(r32,29,27,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO2(r16,v)            _BFSET_(r16,13,11,v)
    #define     w32Gbl_pinmux_cntl_bus3                        {\
            UNSG32 upinmux_cntl_bus_GPIO53                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO54                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO55                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO56                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO57                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO58                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO59                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO0                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO1                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO2                      :  3;\
            UNSG32 RSVDx800C_b30                               :  2;\
          }
    union { UNSG32 u32Gbl_pinmux_cntl_bus3;
            struct w32Gbl_pinmux_cntl_bus3;
          };
    #define   SET32Gbl_pinmux_cntl_bus_GPIO3(r32,v)            _BFSET_(r32, 2, 0,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO3(r16,v)            _BFSET_(r16, 2, 0,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO4(r32,v)            _BFSET_(r32, 5, 3,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO4(r16,v)            _BFSET_(r16, 5, 3,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO5(r32,v)            _BFSET_(r32, 8, 6,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO5(r16,v)            _BFSET_(r16, 8, 6,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO6(r32,v)            _BFSET_(r32,11, 9,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO6(r16,v)            _BFSET_(r16,11, 9,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO7(r32,v)            _BFSET_(r32,14,12,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO7(r16,v)            _BFSET_(r16,14,12,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO8(r32,v)            _BFSET_(r32,17,15,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO9(r32,v)            _BFSET_(r32,20,18,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO9(r16,v)            _BFSET_(r16, 4, 2,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO10(r32,v)           _BFSET_(r32,23,21,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO10(r16,v)           _BFSET_(r16, 7, 5,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO11(r32,v)           _BFSET_(r32,26,24,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO11(r16,v)           _BFSET_(r16,10, 8,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO12(r32,v)           _BFSET_(r32,29,27,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO12(r16,v)           _BFSET_(r16,13,11,v)
    #define     w32Gbl_pinmux_cntl_bus4                        {\
            UNSG32 upinmux_cntl_bus_GPIO3                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO4                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO5                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO6                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO7                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO8                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO9                      :  3;\
            UNSG32 upinmux_cntl_bus_GPIO10                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO11                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO12                     :  3;\
            UNSG32 RSVDx8010_b30                               :  2;\
          }
    union { UNSG32 u32Gbl_pinmux_cntl_bus4;
            struct w32Gbl_pinmux_cntl_bus4;
          };
    #define   SET32Gbl_pinmux_cntl_bus_GPIO13(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO13(r16,v)           _BFSET_(r16, 2, 0,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO14(r32,v)           _BFSET_(r32, 5, 3,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO14(r16,v)           _BFSET_(r16, 5, 3,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO15(r32,v)           _BFSET_(r32, 8, 6,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO15(r16,v)           _BFSET_(r16, 8, 6,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO16(r32,v)           _BFSET_(r32,11, 9,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO16(r16,v)           _BFSET_(r16,11, 9,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO17(r32,v)           _BFSET_(r32,14,12,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO17(r16,v)           _BFSET_(r16,14,12,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO18(r32,v)           _BFSET_(r32,17,15,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO19(r32,v)           _BFSET_(r32,20,18,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO19(r16,v)           _BFSET_(r16, 4, 2,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO20(r32,v)           _BFSET_(r32,23,21,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO20(r16,v)           _BFSET_(r16, 7, 5,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO21(r32,v)           _BFSET_(r32,26,24,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO21(r16,v)           _BFSET_(r16,10, 8,v)
    #define   SET32Gbl_pinmux_cntl_bus_GPIO22(r32,v)           _BFSET_(r32,29,27,v)
    #define   SET16Gbl_pinmux_cntl_bus_GPIO22(r16,v)           _BFSET_(r16,13,11,v)
    #define     w32Gbl_pinmux_cntl_bus5                        {\
            UNSG32 upinmux_cntl_bus_GPIO13                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO14                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO15                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO16                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO17                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO18                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO19                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO20                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO21                     :  3;\
            UNSG32 upinmux_cntl_bus_GPIO22                     :  3;\
            UNSG32 RSVDx8014_b30                               :  2;\
          }
    union { UNSG32 u32Gbl_pinmux_cntl_bus5;
            struct w32Gbl_pinmux_cntl_bus5;
          };
             UNSG8 RSVDx8018                                   [2024];
    #define   SET32Gbl_GPIO23Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO23Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO23Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO23Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO23Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO23Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO23Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO23Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO23Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO23Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO23Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO23Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO23Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO23Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO23Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO23Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO23Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO23Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO23Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO23Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO23Cntl                              {\
            UNSG32 uGPIO23Cntl_DS0                             :  1;\
            UNSG32 uGPIO23Cntl_DS1                             :  1;\
            UNSG32 uGPIO23Cntl_DS2                             :  1;\
            UNSG32 uGPIO23Cntl_DS3                             :  1;\
            UNSG32 uGPIO23Cntl_IE                              :  1;\
            UNSG32 uGPIO23Cntl_PE                              :  1;\
            UNSG32 uGPIO23Cntl_PS                              :  1;\
            UNSG32 uGPIO23Cntl_SL                              :  1;\
            UNSG32 uGPIO23Cntl_SPU                             :  1;\
            UNSG32 uGPIO23Cntl_ST                              :  1;\
            UNSG32 RSVDx8800_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO23Cntl;
            struct w32Gbl_GPIO23Cntl;
          };
    #define   SET32Gbl_GPIO24Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO24Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO24Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO24Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO24Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO24Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO24Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO24Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO24Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO24Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO24Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO24Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO24Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO24Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO24Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO24Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO24Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO24Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO24Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO24Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO24Cntl                              {\
            UNSG32 uGPIO24Cntl_DS0                             :  1;\
            UNSG32 uGPIO24Cntl_DS1                             :  1;\
            UNSG32 uGPIO24Cntl_DS2                             :  1;\
            UNSG32 uGPIO24Cntl_DS3                             :  1;\
            UNSG32 uGPIO24Cntl_IE                              :  1;\
            UNSG32 uGPIO24Cntl_PE                              :  1;\
            UNSG32 uGPIO24Cntl_PS                              :  1;\
            UNSG32 uGPIO24Cntl_SL                              :  1;\
            UNSG32 uGPIO24Cntl_SPU                             :  1;\
            UNSG32 uGPIO24Cntl_ST                              :  1;\
            UNSG32 RSVDx8804_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO24Cntl;
            struct w32Gbl_GPIO24Cntl;
          };
    #define   SET32Gbl_GPIO25Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO25Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO25Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO25Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO25Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO25Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO25Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO25Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO25Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO25Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO25Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO25Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO25Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO25Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO25Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO25Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO25Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO25Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO25Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO25Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO25Cntl                              {\
            UNSG32 uGPIO25Cntl_DS0                             :  1;\
            UNSG32 uGPIO25Cntl_DS1                             :  1;\
            UNSG32 uGPIO25Cntl_DS2                             :  1;\
            UNSG32 uGPIO25Cntl_DS3                             :  1;\
            UNSG32 uGPIO25Cntl_IE                              :  1;\
            UNSG32 uGPIO25Cntl_PE                              :  1;\
            UNSG32 uGPIO25Cntl_PS                              :  1;\
            UNSG32 uGPIO25Cntl_SL                              :  1;\
            UNSG32 uGPIO25Cntl_SPU                             :  1;\
            UNSG32 uGPIO25Cntl_ST                              :  1;\
            UNSG32 RSVDx8808_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO25Cntl;
            struct w32Gbl_GPIO25Cntl;
          };
    #define   SET32Gbl_GPIO26Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO26Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO26Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO26Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO26Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO26Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO26Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO26Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO26Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO26Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO26Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO26Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO26Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO26Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO26Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO26Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO26Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO26Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO26Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO26Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO26Cntl                              {\
            UNSG32 uGPIO26Cntl_DS0                             :  1;\
            UNSG32 uGPIO26Cntl_DS1                             :  1;\
            UNSG32 uGPIO26Cntl_DS2                             :  1;\
            UNSG32 uGPIO26Cntl_DS3                             :  1;\
            UNSG32 uGPIO26Cntl_IE                              :  1;\
            UNSG32 uGPIO26Cntl_PE                              :  1;\
            UNSG32 uGPIO26Cntl_PS                              :  1;\
            UNSG32 uGPIO26Cntl_SL                              :  1;\
            UNSG32 uGPIO26Cntl_SPU                             :  1;\
            UNSG32 uGPIO26Cntl_ST                              :  1;\
            UNSG32 RSVDx880C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO26Cntl;
            struct w32Gbl_GPIO26Cntl;
          };
    #define   SET32Gbl_GPIO27Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO27Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO27Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO27Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO27Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO27Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO27Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO27Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO27Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO27Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO27Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO27Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO27Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO27Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO27Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO27Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO27Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO27Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO27Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO27Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO27Cntl                              {\
            UNSG32 uGPIO27Cntl_DS0                             :  1;\
            UNSG32 uGPIO27Cntl_DS1                             :  1;\
            UNSG32 uGPIO27Cntl_DS2                             :  1;\
            UNSG32 uGPIO27Cntl_DS3                             :  1;\
            UNSG32 uGPIO27Cntl_IE                              :  1;\
            UNSG32 uGPIO27Cntl_PE                              :  1;\
            UNSG32 uGPIO27Cntl_PS                              :  1;\
            UNSG32 uGPIO27Cntl_SL                              :  1;\
            UNSG32 uGPIO27Cntl_SPU                             :  1;\
            UNSG32 uGPIO27Cntl_ST                              :  1;\
            UNSG32 RSVDx8810_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO27Cntl;
            struct w32Gbl_GPIO27Cntl;
          };
    #define   SET32Gbl_GPIO28Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO28Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO28Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO28Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO28Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO28Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO28Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO28Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO28Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO28Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO28Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO28Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO28Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO28Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO28Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO28Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO28Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO28Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO28Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO28Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO28Cntl                              {\
            UNSG32 uGPIO28Cntl_DS0                             :  1;\
            UNSG32 uGPIO28Cntl_DS1                             :  1;\
            UNSG32 uGPIO28Cntl_DS2                             :  1;\
            UNSG32 uGPIO28Cntl_DS3                             :  1;\
            UNSG32 uGPIO28Cntl_IE                              :  1;\
            UNSG32 uGPIO28Cntl_PE                              :  1;\
            UNSG32 uGPIO28Cntl_PS                              :  1;\
            UNSG32 uGPIO28Cntl_SL                              :  1;\
            UNSG32 uGPIO28Cntl_SPU                             :  1;\
            UNSG32 uGPIO28Cntl_ST                              :  1;\
            UNSG32 RSVDx8814_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO28Cntl;
            struct w32Gbl_GPIO28Cntl;
          };
    #define   SET32Gbl_GPIO29Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO29Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO29Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO29Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO29Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO29Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO29Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO29Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO29Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO29Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO29Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO29Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO29Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO29Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO29Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO29Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO29Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO29Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO29Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO29Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO29Cntl                              {\
            UNSG32 uGPIO29Cntl_DS0                             :  1;\
            UNSG32 uGPIO29Cntl_DS1                             :  1;\
            UNSG32 uGPIO29Cntl_DS2                             :  1;\
            UNSG32 uGPIO29Cntl_DS3                             :  1;\
            UNSG32 uGPIO29Cntl_IE                              :  1;\
            UNSG32 uGPIO29Cntl_PE                              :  1;\
            UNSG32 uGPIO29Cntl_PS                              :  1;\
            UNSG32 uGPIO29Cntl_SL                              :  1;\
            UNSG32 uGPIO29Cntl_SPU                             :  1;\
            UNSG32 uGPIO29Cntl_ST                              :  1;\
            UNSG32 RSVDx8818_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO29Cntl;
            struct w32Gbl_GPIO29Cntl;
          };
    #define   SET32Gbl_GPIO30Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO30Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO30Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO30Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO30Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO30Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO30Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO30Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO30Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO30Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO30Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO30Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO30Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO30Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO30Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO30Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO30Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO30Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO30Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO30Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO30Cntl                              {\
            UNSG32 uGPIO30Cntl_DS0                             :  1;\
            UNSG32 uGPIO30Cntl_DS1                             :  1;\
            UNSG32 uGPIO30Cntl_DS2                             :  1;\
            UNSG32 uGPIO30Cntl_DS3                             :  1;\
            UNSG32 uGPIO30Cntl_IE                              :  1;\
            UNSG32 uGPIO30Cntl_PE                              :  1;\
            UNSG32 uGPIO30Cntl_PS                              :  1;\
            UNSG32 uGPIO30Cntl_SL                              :  1;\
            UNSG32 uGPIO30Cntl_SPU                             :  1;\
            UNSG32 uGPIO30Cntl_ST                              :  1;\
            UNSG32 RSVDx881C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO30Cntl;
            struct w32Gbl_GPIO30Cntl;
          };
    #define   SET32Gbl_GPIO31Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO31Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO31Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO31Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO31Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO31Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO31Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO31Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO31Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO31Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO31Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO31Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO31Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO31Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO31Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO31Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO31Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO31Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO31Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO31Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO31Cntl                              {\
            UNSG32 uGPIO31Cntl_DS0                             :  1;\
            UNSG32 uGPIO31Cntl_DS1                             :  1;\
            UNSG32 uGPIO31Cntl_DS2                             :  1;\
            UNSG32 uGPIO31Cntl_DS3                             :  1;\
            UNSG32 uGPIO31Cntl_IE                              :  1;\
            UNSG32 uGPIO31Cntl_PE                              :  1;\
            UNSG32 uGPIO31Cntl_PS                              :  1;\
            UNSG32 uGPIO31Cntl_SL                              :  1;\
            UNSG32 uGPIO31Cntl_SPU                             :  1;\
            UNSG32 uGPIO31Cntl_ST                              :  1;\
            UNSG32 RSVDx8820_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO31Cntl;
            struct w32Gbl_GPIO31Cntl;
          };
    #define   SET32Gbl_GPIO32Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO32Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO32Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO32Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO32Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO32Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO32Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO32Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO32Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO32Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO32Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO32Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO32Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO32Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO32Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO32Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO32Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO32Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO32Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO32Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO32Cntl                              {\
            UNSG32 uGPIO32Cntl_DS0                             :  1;\
            UNSG32 uGPIO32Cntl_DS1                             :  1;\
            UNSG32 uGPIO32Cntl_DS2                             :  1;\
            UNSG32 uGPIO32Cntl_DS3                             :  1;\
            UNSG32 uGPIO32Cntl_IE                              :  1;\
            UNSG32 uGPIO32Cntl_PE                              :  1;\
            UNSG32 uGPIO32Cntl_PS                              :  1;\
            UNSG32 uGPIO32Cntl_SL                              :  1;\
            UNSG32 uGPIO32Cntl_SPU                             :  1;\
            UNSG32 uGPIO32Cntl_ST                              :  1;\
            UNSG32 RSVDx8824_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO32Cntl;
            struct w32Gbl_GPIO32Cntl;
          };
    #define   SET32Gbl_GPIO33Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO33Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO33Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO33Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO33Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO33Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO33Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO33Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO33Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO33Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO33Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO33Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO33Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO33Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO33Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO33Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO33Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO33Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO33Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO33Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO33Cntl                              {\
            UNSG32 uGPIO33Cntl_DS0                             :  1;\
            UNSG32 uGPIO33Cntl_DS1                             :  1;\
            UNSG32 uGPIO33Cntl_DS2                             :  1;\
            UNSG32 uGPIO33Cntl_DS3                             :  1;\
            UNSG32 uGPIO33Cntl_IE                              :  1;\
            UNSG32 uGPIO33Cntl_PE                              :  1;\
            UNSG32 uGPIO33Cntl_PS                              :  1;\
            UNSG32 uGPIO33Cntl_SL                              :  1;\
            UNSG32 uGPIO33Cntl_SPU                             :  1;\
            UNSG32 uGPIO33Cntl_ST                              :  1;\
            UNSG32 RSVDx8828_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO33Cntl;
            struct w32Gbl_GPIO33Cntl;
          };
    #define   SET32Gbl_GPIO34Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO34Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO34Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO34Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO34Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO34Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO34Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO34Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO34Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO34Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO34Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO34Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO34Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO34Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO34Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO34Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO34Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO34Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO34Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO34Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO34Cntl                              {\
            UNSG32 uGPIO34Cntl_DS0                             :  1;\
            UNSG32 uGPIO34Cntl_DS1                             :  1;\
            UNSG32 uGPIO34Cntl_DS2                             :  1;\
            UNSG32 uGPIO34Cntl_DS3                             :  1;\
            UNSG32 uGPIO34Cntl_IE                              :  1;\
            UNSG32 uGPIO34Cntl_PE                              :  1;\
            UNSG32 uGPIO34Cntl_PS                              :  1;\
            UNSG32 uGPIO34Cntl_SL                              :  1;\
            UNSG32 uGPIO34Cntl_SPU                             :  1;\
            UNSG32 uGPIO34Cntl_ST                              :  1;\
            UNSG32 RSVDx882C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO34Cntl;
            struct w32Gbl_GPIO34Cntl;
          };
    #define   SET32Gbl_GPIO35Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO35Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO35Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO35Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO35Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO35Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO35Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO35Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO35Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO35Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO35Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO35Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO35Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO35Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO35Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO35Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO35Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO35Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO35Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO35Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO35Cntl                              {\
            UNSG32 uGPIO35Cntl_DS0                             :  1;\
            UNSG32 uGPIO35Cntl_DS1                             :  1;\
            UNSG32 uGPIO35Cntl_DS2                             :  1;\
            UNSG32 uGPIO35Cntl_DS3                             :  1;\
            UNSG32 uGPIO35Cntl_IE                              :  1;\
            UNSG32 uGPIO35Cntl_PE                              :  1;\
            UNSG32 uGPIO35Cntl_PS                              :  1;\
            UNSG32 uGPIO35Cntl_SL                              :  1;\
            UNSG32 uGPIO35Cntl_SPU                             :  1;\
            UNSG32 uGPIO35Cntl_ST                              :  1;\
            UNSG32 RSVDx8830_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO35Cntl;
            struct w32Gbl_GPIO35Cntl;
          };
    #define   SET32Gbl_GPIO36Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO36Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO36Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO36Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO36Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO36Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO36Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO36Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO36Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO36Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO36Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO36Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO36Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO36Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO36Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO36Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO36Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO36Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO36Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO36Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO36Cntl                              {\
            UNSG32 uGPIO36Cntl_DS0                             :  1;\
            UNSG32 uGPIO36Cntl_DS1                             :  1;\
            UNSG32 uGPIO36Cntl_DS2                             :  1;\
            UNSG32 uGPIO36Cntl_DS3                             :  1;\
            UNSG32 uGPIO36Cntl_IE                              :  1;\
            UNSG32 uGPIO36Cntl_PE                              :  1;\
            UNSG32 uGPIO36Cntl_PS                              :  1;\
            UNSG32 uGPIO36Cntl_SL                              :  1;\
            UNSG32 uGPIO36Cntl_SPU                             :  1;\
            UNSG32 uGPIO36Cntl_ST                              :  1;\
            UNSG32 RSVDx8834_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO36Cntl;
            struct w32Gbl_GPIO36Cntl;
          };
    #define   SET32Gbl_GPIO37Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO37Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO37Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO37Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO37Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO37Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO37Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO37Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO37Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO37Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO37Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO37Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO37Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO37Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO37Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO37Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO37Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO37Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO37Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO37Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO37Cntl                              {\
            UNSG32 uGPIO37Cntl_DS0                             :  1;\
            UNSG32 uGPIO37Cntl_DS1                             :  1;\
            UNSG32 uGPIO37Cntl_DS2                             :  1;\
            UNSG32 uGPIO37Cntl_DS3                             :  1;\
            UNSG32 uGPIO37Cntl_IE                              :  1;\
            UNSG32 uGPIO37Cntl_PE                              :  1;\
            UNSG32 uGPIO37Cntl_PS                              :  1;\
            UNSG32 uGPIO37Cntl_SL                              :  1;\
            UNSG32 uGPIO37Cntl_SPU                             :  1;\
            UNSG32 uGPIO37Cntl_ST                              :  1;\
            UNSG32 RSVDx8838_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO37Cntl;
            struct w32Gbl_GPIO37Cntl;
          };
    #define   SET32Gbl_GPIO38Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO38Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO38Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO38Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO38Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO38Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO38Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO38Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO38Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO38Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO38Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO38Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO38Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO38Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO38Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO38Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO38Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO38Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO38Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO38Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO38Cntl                              {\
            UNSG32 uGPIO38Cntl_DS0                             :  1;\
            UNSG32 uGPIO38Cntl_DS1                             :  1;\
            UNSG32 uGPIO38Cntl_DS2                             :  1;\
            UNSG32 uGPIO38Cntl_DS3                             :  1;\
            UNSG32 uGPIO38Cntl_IE                              :  1;\
            UNSG32 uGPIO38Cntl_PE                              :  1;\
            UNSG32 uGPIO38Cntl_PS                              :  1;\
            UNSG32 uGPIO38Cntl_SL                              :  1;\
            UNSG32 uGPIO38Cntl_SPU                             :  1;\
            UNSG32 uGPIO38Cntl_ST                              :  1;\
            UNSG32 RSVDx883C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO38Cntl;
            struct w32Gbl_GPIO38Cntl;
          };
    #define   SET32Gbl_GPIO39Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO39Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO39Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO39Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO39Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO39Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO39Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO39Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO39Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO39Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO39Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO39Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO39Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO39Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO39Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO39Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO39Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO39Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO39Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO39Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO39Cntl                              {\
            UNSG32 uGPIO39Cntl_DS0                             :  1;\
            UNSG32 uGPIO39Cntl_DS1                             :  1;\
            UNSG32 uGPIO39Cntl_DS2                             :  1;\
            UNSG32 uGPIO39Cntl_DS3                             :  1;\
            UNSG32 uGPIO39Cntl_IE                              :  1;\
            UNSG32 uGPIO39Cntl_PE                              :  1;\
            UNSG32 uGPIO39Cntl_PS                              :  1;\
            UNSG32 uGPIO39Cntl_SL                              :  1;\
            UNSG32 uGPIO39Cntl_SPU                             :  1;\
            UNSG32 uGPIO39Cntl_ST                              :  1;\
            UNSG32 RSVDx8840_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO39Cntl;
            struct w32Gbl_GPIO39Cntl;
          };
    #define   SET32Gbl_GPIO40Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO40Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO40Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO40Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO40Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO40Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO40Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO40Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO40Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO40Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO40Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO40Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO40Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO40Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO40Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO40Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO40Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO40Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO40Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO40Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO40Cntl                              {\
            UNSG32 uGPIO40Cntl_DS0                             :  1;\
            UNSG32 uGPIO40Cntl_DS1                             :  1;\
            UNSG32 uGPIO40Cntl_DS2                             :  1;\
            UNSG32 uGPIO40Cntl_DS3                             :  1;\
            UNSG32 uGPIO40Cntl_IE                              :  1;\
            UNSG32 uGPIO40Cntl_PE                              :  1;\
            UNSG32 uGPIO40Cntl_PS                              :  1;\
            UNSG32 uGPIO40Cntl_SL                              :  1;\
            UNSG32 uGPIO40Cntl_SPU                             :  1;\
            UNSG32 uGPIO40Cntl_ST                              :  1;\
            UNSG32 RSVDx8844_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO40Cntl;
            struct w32Gbl_GPIO40Cntl;
          };
    #define   SET32Gbl_GPIO41Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO41Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO41Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO41Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO41Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO41Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO41Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO41Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO41Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO41Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO41Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO41Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO41Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO41Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO41Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO41Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO41Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO41Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO41Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO41Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO41Cntl                              {\
            UNSG32 uGPIO41Cntl_DS0                             :  1;\
            UNSG32 uGPIO41Cntl_DS1                             :  1;\
            UNSG32 uGPIO41Cntl_DS2                             :  1;\
            UNSG32 uGPIO41Cntl_DS3                             :  1;\
            UNSG32 uGPIO41Cntl_IE                              :  1;\
            UNSG32 uGPIO41Cntl_PE                              :  1;\
            UNSG32 uGPIO41Cntl_PS                              :  1;\
            UNSG32 uGPIO41Cntl_SL                              :  1;\
            UNSG32 uGPIO41Cntl_SPU                             :  1;\
            UNSG32 uGPIO41Cntl_ST                              :  1;\
            UNSG32 RSVDx8848_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO41Cntl;
            struct w32Gbl_GPIO41Cntl;
          };
    #define   SET32Gbl_GPIO42Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO42Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO42Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO42Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO42Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO42Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO42Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO42Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO42Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO42Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO42Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO42Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO42Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO42Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO42Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO42Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO42Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO42Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO42Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO42Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO42Cntl                              {\
            UNSG32 uGPIO42Cntl_DS0                             :  1;\
            UNSG32 uGPIO42Cntl_DS1                             :  1;\
            UNSG32 uGPIO42Cntl_DS2                             :  1;\
            UNSG32 uGPIO42Cntl_DS3                             :  1;\
            UNSG32 uGPIO42Cntl_IE                              :  1;\
            UNSG32 uGPIO42Cntl_PE                              :  1;\
            UNSG32 uGPIO42Cntl_PS                              :  1;\
            UNSG32 uGPIO42Cntl_SL                              :  1;\
            UNSG32 uGPIO42Cntl_SPU                             :  1;\
            UNSG32 uGPIO42Cntl_ST                              :  1;\
            UNSG32 RSVDx884C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO42Cntl;
            struct w32Gbl_GPIO42Cntl;
          };
    #define   SET32Gbl_GPIO43Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO43Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO43Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO43Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO43Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO43Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO43Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO43Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO43Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO43Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO43Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO43Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO43Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO43Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO43Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO43Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO43Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO43Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO43Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO43Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO43Cntl                              {\
            UNSG32 uGPIO43Cntl_DS0                             :  1;\
            UNSG32 uGPIO43Cntl_DS1                             :  1;\
            UNSG32 uGPIO43Cntl_DS2                             :  1;\
            UNSG32 uGPIO43Cntl_DS3                             :  1;\
            UNSG32 uGPIO43Cntl_IE                              :  1;\
            UNSG32 uGPIO43Cntl_PE                              :  1;\
            UNSG32 uGPIO43Cntl_PS                              :  1;\
            UNSG32 uGPIO43Cntl_SL                              :  1;\
            UNSG32 uGPIO43Cntl_SPU                             :  1;\
            UNSG32 uGPIO43Cntl_ST                              :  1;\
            UNSG32 RSVDx8850_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO43Cntl;
            struct w32Gbl_GPIO43Cntl;
          };
    #define   SET32Gbl_GPIO44Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO44Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO44Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO44Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO44Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO44Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO44Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO44Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO44Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO44Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO44Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO44Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO44Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO44Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO44Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO44Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO44Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO44Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO44Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO44Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO44Cntl                              {\
            UNSG32 uGPIO44Cntl_DS0                             :  1;\
            UNSG32 uGPIO44Cntl_DS1                             :  1;\
            UNSG32 uGPIO44Cntl_DS2                             :  1;\
            UNSG32 uGPIO44Cntl_DS3                             :  1;\
            UNSG32 uGPIO44Cntl_IE                              :  1;\
            UNSG32 uGPIO44Cntl_PE                              :  1;\
            UNSG32 uGPIO44Cntl_PS                              :  1;\
            UNSG32 uGPIO44Cntl_SL                              :  1;\
            UNSG32 uGPIO44Cntl_SPU                             :  1;\
            UNSG32 uGPIO44Cntl_ST                              :  1;\
            UNSG32 RSVDx8854_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO44Cntl;
            struct w32Gbl_GPIO44Cntl;
          };
    #define   SET32Gbl_GPIO45Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO45Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO45Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO45Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO45Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO45Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO45Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO45Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO45Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO45Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO45Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO45Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO45Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO45Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO45Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO45Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO45Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO45Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO45Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO45Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO45Cntl                              {\
            UNSG32 uGPIO45Cntl_DS0                             :  1;\
            UNSG32 uGPIO45Cntl_DS1                             :  1;\
            UNSG32 uGPIO45Cntl_DS2                             :  1;\
            UNSG32 uGPIO45Cntl_DS3                             :  1;\
            UNSG32 uGPIO45Cntl_IE                              :  1;\
            UNSG32 uGPIO45Cntl_PE                              :  1;\
            UNSG32 uGPIO45Cntl_PS                              :  1;\
            UNSG32 uGPIO45Cntl_SL                              :  1;\
            UNSG32 uGPIO45Cntl_SPU                             :  1;\
            UNSG32 uGPIO45Cntl_ST                              :  1;\
            UNSG32 RSVDx8858_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO45Cntl;
            struct w32Gbl_GPIO45Cntl;
          };
    #define   SET32Gbl_GPIO46Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO46Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO46Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO46Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO46Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO46Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO46Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO46Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO46Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO46Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO46Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO46Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO46Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO46Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO46Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO46Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO46Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO46Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO46Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO46Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO46Cntl                              {\
            UNSG32 uGPIO46Cntl_DS0                             :  1;\
            UNSG32 uGPIO46Cntl_DS1                             :  1;\
            UNSG32 uGPIO46Cntl_DS2                             :  1;\
            UNSG32 uGPIO46Cntl_DS3                             :  1;\
            UNSG32 uGPIO46Cntl_IE                              :  1;\
            UNSG32 uGPIO46Cntl_PE                              :  1;\
            UNSG32 uGPIO46Cntl_PS                              :  1;\
            UNSG32 uGPIO46Cntl_SL                              :  1;\
            UNSG32 uGPIO46Cntl_SPU                             :  1;\
            UNSG32 uGPIO46Cntl_ST                              :  1;\
            UNSG32 RSVDx885C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO46Cntl;
            struct w32Gbl_GPIO46Cntl;
          };
    #define   SET32Gbl_GPIO47Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO47Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO47Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO47Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO47Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO47Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO47Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO47Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO47Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO47Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO47Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO47Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO47Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO47Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO47Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO47Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO47Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO47Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO47Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO47Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO47Cntl                              {\
            UNSG32 uGPIO47Cntl_DS0                             :  1;\
            UNSG32 uGPIO47Cntl_DS1                             :  1;\
            UNSG32 uGPIO47Cntl_DS2                             :  1;\
            UNSG32 uGPIO47Cntl_DS3                             :  1;\
            UNSG32 uGPIO47Cntl_IE                              :  1;\
            UNSG32 uGPIO47Cntl_PE                              :  1;\
            UNSG32 uGPIO47Cntl_PS                              :  1;\
            UNSG32 uGPIO47Cntl_SL                              :  1;\
            UNSG32 uGPIO47Cntl_SPU                             :  1;\
            UNSG32 uGPIO47Cntl_ST                              :  1;\
            UNSG32 RSVDx8860_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO47Cntl;
            struct w32Gbl_GPIO47Cntl;
          };
    #define   SET32Gbl_GPIO48Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO48Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO48Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO48Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO48Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO48Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO48Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO48Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO48Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO48Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO48Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO48Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO48Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO48Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO48Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO48Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO48Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO48Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO48Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO48Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO48Cntl                              {\
            UNSG32 uGPIO48Cntl_DS0                             :  1;\
            UNSG32 uGPIO48Cntl_DS1                             :  1;\
            UNSG32 uGPIO48Cntl_DS2                             :  1;\
            UNSG32 uGPIO48Cntl_DS3                             :  1;\
            UNSG32 uGPIO48Cntl_IE                              :  1;\
            UNSG32 uGPIO48Cntl_PE                              :  1;\
            UNSG32 uGPIO48Cntl_PS                              :  1;\
            UNSG32 uGPIO48Cntl_SL                              :  1;\
            UNSG32 uGPIO48Cntl_SPU                             :  1;\
            UNSG32 uGPIO48Cntl_ST                              :  1;\
            UNSG32 RSVDx8864_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO48Cntl;
            struct w32Gbl_GPIO48Cntl;
          };
    #define   SET32Gbl_GPIO49Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO49Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO49Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO49Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO49Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO49Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO49Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO49Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO49Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO49Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO49Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO49Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO49Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO49Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO49Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO49Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO49Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO49Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO49Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO49Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO49Cntl                              {\
            UNSG32 uGPIO49Cntl_DS0                             :  1;\
            UNSG32 uGPIO49Cntl_DS1                             :  1;\
            UNSG32 uGPIO49Cntl_DS2                             :  1;\
            UNSG32 uGPIO49Cntl_DS3                             :  1;\
            UNSG32 uGPIO49Cntl_IE                              :  1;\
            UNSG32 uGPIO49Cntl_PE                              :  1;\
            UNSG32 uGPIO49Cntl_PS                              :  1;\
            UNSG32 uGPIO49Cntl_SL                              :  1;\
            UNSG32 uGPIO49Cntl_SPU                             :  1;\
            UNSG32 uGPIO49Cntl_ST                              :  1;\
            UNSG32 RSVDx8868_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO49Cntl;
            struct w32Gbl_GPIO49Cntl;
          };
    #define   SET32Gbl_GPIO50Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO50Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO50Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO50Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO50Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO50Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO50Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO50Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO50Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO50Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO50Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO50Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO50Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO50Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO50Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO50Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO50Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO50Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO50Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO50Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO50Cntl                              {\
            UNSG32 uGPIO50Cntl_DS0                             :  1;\
            UNSG32 uGPIO50Cntl_DS1                             :  1;\
            UNSG32 uGPIO50Cntl_DS2                             :  1;\
            UNSG32 uGPIO50Cntl_DS3                             :  1;\
            UNSG32 uGPIO50Cntl_IE                              :  1;\
            UNSG32 uGPIO50Cntl_PE                              :  1;\
            UNSG32 uGPIO50Cntl_PS                              :  1;\
            UNSG32 uGPIO50Cntl_SL                              :  1;\
            UNSG32 uGPIO50Cntl_SPU                             :  1;\
            UNSG32 uGPIO50Cntl_ST                              :  1;\
            UNSG32 RSVDx886C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO50Cntl;
            struct w32Gbl_GPIO50Cntl;
          };
    #define   SET32Gbl_GPIO51Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO51Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO51Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO51Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO51Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO51Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO51Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO51Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO51Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO51Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO51Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO51Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO51Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO51Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO51Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO51Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO51Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO51Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO51Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO51Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO51Cntl                              {\
            UNSG32 uGPIO51Cntl_DS0                             :  1;\
            UNSG32 uGPIO51Cntl_DS1                             :  1;\
            UNSG32 uGPIO51Cntl_DS2                             :  1;\
            UNSG32 uGPIO51Cntl_DS3                             :  1;\
            UNSG32 uGPIO51Cntl_IE                              :  1;\
            UNSG32 uGPIO51Cntl_PE                              :  1;\
            UNSG32 uGPIO51Cntl_PS                              :  1;\
            UNSG32 uGPIO51Cntl_SL                              :  1;\
            UNSG32 uGPIO51Cntl_SPU                             :  1;\
            UNSG32 uGPIO51Cntl_ST                              :  1;\
            UNSG32 RSVDx8870_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO51Cntl;
            struct w32Gbl_GPIO51Cntl;
          };
    #define   SET32Gbl_GPIO52Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO52Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO52Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO52Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO52Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO52Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO52Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO52Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO52Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO52Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO52Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO52Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO52Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO52Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO52Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO52Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO52Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO52Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO52Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO52Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO52Cntl                              {\
            UNSG32 uGPIO52Cntl_DS0                             :  1;\
            UNSG32 uGPIO52Cntl_DS1                             :  1;\
            UNSG32 uGPIO52Cntl_DS2                             :  1;\
            UNSG32 uGPIO52Cntl_DS3                             :  1;\
            UNSG32 uGPIO52Cntl_IE                              :  1;\
            UNSG32 uGPIO52Cntl_PE                              :  1;\
            UNSG32 uGPIO52Cntl_PS                              :  1;\
            UNSG32 uGPIO52Cntl_SL                              :  1;\
            UNSG32 uGPIO52Cntl_SPU                             :  1;\
            UNSG32 uGPIO52Cntl_ST                              :  1;\
            UNSG32 RSVDx8874_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO52Cntl;
            struct w32Gbl_GPIO52Cntl;
          };
    #define   SET32Gbl_GPIO53Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO53Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO53Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO53Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO53Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO53Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO53Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO53Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO53Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO53Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO53Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO53Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO53Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO53Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO53Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO53Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO53Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO53Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO53Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO53Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO53Cntl                              {\
            UNSG32 uGPIO53Cntl_DS0                             :  1;\
            UNSG32 uGPIO53Cntl_DS1                             :  1;\
            UNSG32 uGPIO53Cntl_DS2                             :  1;\
            UNSG32 uGPIO53Cntl_DS3                             :  1;\
            UNSG32 uGPIO53Cntl_IE                              :  1;\
            UNSG32 uGPIO53Cntl_PE                              :  1;\
            UNSG32 uGPIO53Cntl_PS                              :  1;\
            UNSG32 uGPIO53Cntl_SL                              :  1;\
            UNSG32 uGPIO53Cntl_SPU                             :  1;\
            UNSG32 uGPIO53Cntl_ST                              :  1;\
            UNSG32 RSVDx8878_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO53Cntl;
            struct w32Gbl_GPIO53Cntl;
          };
    #define   SET32Gbl_GPIO54Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO54Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO54Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO54Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO54Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO54Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO54Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO54Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO54Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO54Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO54Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO54Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO54Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO54Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO54Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO54Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO54Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO54Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO54Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO54Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO54Cntl                              {\
            UNSG32 uGPIO54Cntl_DS0                             :  1;\
            UNSG32 uGPIO54Cntl_DS1                             :  1;\
            UNSG32 uGPIO54Cntl_DS2                             :  1;\
            UNSG32 uGPIO54Cntl_DS3                             :  1;\
            UNSG32 uGPIO54Cntl_IE                              :  1;\
            UNSG32 uGPIO54Cntl_PE                              :  1;\
            UNSG32 uGPIO54Cntl_PS                              :  1;\
            UNSG32 uGPIO54Cntl_SL                              :  1;\
            UNSG32 uGPIO54Cntl_SPU                             :  1;\
            UNSG32 uGPIO54Cntl_ST                              :  1;\
            UNSG32 RSVDx887C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO54Cntl;
            struct w32Gbl_GPIO54Cntl;
          };
    #define   SET32Gbl_GPIO55Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO55Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO55Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO55Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO55Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO55Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO55Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO55Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO55Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO55Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO55Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO55Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO55Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO55Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO55Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO55Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO55Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO55Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO55Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO55Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO55Cntl                              {\
            UNSG32 uGPIO55Cntl_DS0                             :  1;\
            UNSG32 uGPIO55Cntl_DS1                             :  1;\
            UNSG32 uGPIO55Cntl_DS2                             :  1;\
            UNSG32 uGPIO55Cntl_DS3                             :  1;\
            UNSG32 uGPIO55Cntl_IE                              :  1;\
            UNSG32 uGPIO55Cntl_PE                              :  1;\
            UNSG32 uGPIO55Cntl_PS                              :  1;\
            UNSG32 uGPIO55Cntl_SL                              :  1;\
            UNSG32 uGPIO55Cntl_SPU                             :  1;\
            UNSG32 uGPIO55Cntl_ST                              :  1;\
            UNSG32 RSVDx8880_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO55Cntl;
            struct w32Gbl_GPIO55Cntl;
          };
    #define   SET32Gbl_GPIO56Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO56Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO56Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO56Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO56Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO56Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO56Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO56Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO56Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO56Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO56Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO56Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO56Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO56Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO56Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO56Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO56Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO56Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO56Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO56Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO56Cntl                              {\
            UNSG32 uGPIO56Cntl_DS0                             :  1;\
            UNSG32 uGPIO56Cntl_DS1                             :  1;\
            UNSG32 uGPIO56Cntl_DS2                             :  1;\
            UNSG32 uGPIO56Cntl_DS3                             :  1;\
            UNSG32 uGPIO56Cntl_IE                              :  1;\
            UNSG32 uGPIO56Cntl_PE                              :  1;\
            UNSG32 uGPIO56Cntl_PS                              :  1;\
            UNSG32 uGPIO56Cntl_SL                              :  1;\
            UNSG32 uGPIO56Cntl_SPU                             :  1;\
            UNSG32 uGPIO56Cntl_ST                              :  1;\
            UNSG32 RSVDx8884_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO56Cntl;
            struct w32Gbl_GPIO56Cntl;
          };
    #define   SET32Gbl_GPIO57Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO57Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO57Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO57Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO57Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO57Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO57Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO57Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO57Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO57Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO57Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO57Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO57Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO57Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO57Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO57Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO57Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO57Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO57Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO57Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO57Cntl                              {\
            UNSG32 uGPIO57Cntl_DS0                             :  1;\
            UNSG32 uGPIO57Cntl_DS1                             :  1;\
            UNSG32 uGPIO57Cntl_DS2                             :  1;\
            UNSG32 uGPIO57Cntl_DS3                             :  1;\
            UNSG32 uGPIO57Cntl_IE                              :  1;\
            UNSG32 uGPIO57Cntl_PE                              :  1;\
            UNSG32 uGPIO57Cntl_PS                              :  1;\
            UNSG32 uGPIO57Cntl_SL                              :  1;\
            UNSG32 uGPIO57Cntl_SPU                             :  1;\
            UNSG32 uGPIO57Cntl_ST                              :  1;\
            UNSG32 RSVDx8888_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO57Cntl;
            struct w32Gbl_GPIO57Cntl;
          };
    #define   SET32Gbl_GPIO58Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO58Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO58Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO58Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO58Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO58Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO58Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO58Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO58Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO58Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO58Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO58Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO58Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO58Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO58Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO58Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO58Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO58Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO58Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO58Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO58Cntl                              {\
            UNSG32 uGPIO58Cntl_DS0                             :  1;\
            UNSG32 uGPIO58Cntl_DS1                             :  1;\
            UNSG32 uGPIO58Cntl_DS2                             :  1;\
            UNSG32 uGPIO58Cntl_DS3                             :  1;\
            UNSG32 uGPIO58Cntl_IE                              :  1;\
            UNSG32 uGPIO58Cntl_PE                              :  1;\
            UNSG32 uGPIO58Cntl_PS                              :  1;\
            UNSG32 uGPIO58Cntl_SL                              :  1;\
            UNSG32 uGPIO58Cntl_SPU                             :  1;\
            UNSG32 uGPIO58Cntl_ST                              :  1;\
            UNSG32 RSVDx888C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO58Cntl;
            struct w32Gbl_GPIO58Cntl;
          };
    #define   SET32Gbl_GPIO59Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO59Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO59Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO59Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO59Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO59Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO59Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO59Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO59Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO59Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO59Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO59Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO59Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO59Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO59Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO59Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO59Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO59Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO59Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO59Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO59Cntl                              {\
            UNSG32 uGPIO59Cntl_DS0                             :  1;\
            UNSG32 uGPIO59Cntl_DS1                             :  1;\
            UNSG32 uGPIO59Cntl_DS2                             :  1;\
            UNSG32 uGPIO59Cntl_DS3                             :  1;\
            UNSG32 uGPIO59Cntl_IE                              :  1;\
            UNSG32 uGPIO59Cntl_PE                              :  1;\
            UNSG32 uGPIO59Cntl_PS                              :  1;\
            UNSG32 uGPIO59Cntl_SL                              :  1;\
            UNSG32 uGPIO59Cntl_SPU                             :  1;\
            UNSG32 uGPIO59Cntl_ST                              :  1;\
            UNSG32 RSVDx8890_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO59Cntl;
            struct w32Gbl_GPIO59Cntl;
          };
    #define   SET32Gbl_GPIO0Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO0Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO0Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO0Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO0Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO0Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO0Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO0Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO0Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO0Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO0Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO0Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO0Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO0Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO0Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO0Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO0Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO0Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO0Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO0Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO0Cntl                               {\
            UNSG32 uGPIO0Cntl_DS0                              :  1;\
            UNSG32 uGPIO0Cntl_DS1                              :  1;\
            UNSG32 uGPIO0Cntl_DS2                              :  1;\
            UNSG32 uGPIO0Cntl_DS3                              :  1;\
            UNSG32 uGPIO0Cntl_IE                               :  1;\
            UNSG32 uGPIO0Cntl_PE                               :  1;\
            UNSG32 uGPIO0Cntl_PS                               :  1;\
            UNSG32 uGPIO0Cntl_SL                               :  1;\
            UNSG32 uGPIO0Cntl_SPU                              :  1;\
            UNSG32 uGPIO0Cntl_ST                               :  1;\
            UNSG32 RSVDx8894_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO0Cntl;
            struct w32Gbl_GPIO0Cntl;
          };
    #define   SET32Gbl_GPIO1Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO1Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO1Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO1Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO1Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO1Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO1Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO1Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO1Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO1Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO1Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO1Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO1Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO1Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO1Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO1Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO1Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO1Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO1Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO1Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO1Cntl                               {\
            UNSG32 uGPIO1Cntl_DS0                              :  1;\
            UNSG32 uGPIO1Cntl_DS1                              :  1;\
            UNSG32 uGPIO1Cntl_DS2                              :  1;\
            UNSG32 uGPIO1Cntl_DS3                              :  1;\
            UNSG32 uGPIO1Cntl_IE                               :  1;\
            UNSG32 uGPIO1Cntl_PE                               :  1;\
            UNSG32 uGPIO1Cntl_PS                               :  1;\
            UNSG32 uGPIO1Cntl_SL                               :  1;\
            UNSG32 uGPIO1Cntl_SPU                              :  1;\
            UNSG32 uGPIO1Cntl_ST                               :  1;\
            UNSG32 RSVDx8898_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO1Cntl;
            struct w32Gbl_GPIO1Cntl;
          };
    #define   SET32Gbl_GPIO2Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO2Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO2Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO2Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO2Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO2Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO2Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO2Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO2Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO2Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO2Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO2Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO2Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO2Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO2Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO2Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO2Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO2Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO2Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO2Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO2Cntl                               {\
            UNSG32 uGPIO2Cntl_DS0                              :  1;\
            UNSG32 uGPIO2Cntl_DS1                              :  1;\
            UNSG32 uGPIO2Cntl_DS2                              :  1;\
            UNSG32 uGPIO2Cntl_DS3                              :  1;\
            UNSG32 uGPIO2Cntl_IE                               :  1;\
            UNSG32 uGPIO2Cntl_PE                               :  1;\
            UNSG32 uGPIO2Cntl_PS                               :  1;\
            UNSG32 uGPIO2Cntl_SL                               :  1;\
            UNSG32 uGPIO2Cntl_SPU                              :  1;\
            UNSG32 uGPIO2Cntl_ST                               :  1;\
            UNSG32 RSVDx889C_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO2Cntl;
            struct w32Gbl_GPIO2Cntl;
          };
    #define   SET32Gbl_GPIO3Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO3Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO3Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO3Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO3Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO3Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO3Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO3Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO3Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO3Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO3Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO3Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO3Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO3Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO3Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO3Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO3Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO3Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO3Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO3Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO3Cntl                               {\
            UNSG32 uGPIO3Cntl_DS0                              :  1;\
            UNSG32 uGPIO3Cntl_DS1                              :  1;\
            UNSG32 uGPIO3Cntl_DS2                              :  1;\
            UNSG32 uGPIO3Cntl_DS3                              :  1;\
            UNSG32 uGPIO3Cntl_IE                               :  1;\
            UNSG32 uGPIO3Cntl_PE                               :  1;\
            UNSG32 uGPIO3Cntl_PS                               :  1;\
            UNSG32 uGPIO3Cntl_SL                               :  1;\
            UNSG32 uGPIO3Cntl_SPU                              :  1;\
            UNSG32 uGPIO3Cntl_ST                               :  1;\
            UNSG32 RSVDx88A0_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO3Cntl;
            struct w32Gbl_GPIO3Cntl;
          };
    #define   SET32Gbl_GPIO4Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO4Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO4Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO4Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO4Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO4Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO4Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO4Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO4Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO4Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO4Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO4Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO4Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO4Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO4Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO4Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO4Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO4Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO4Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO4Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO4Cntl                               {\
            UNSG32 uGPIO4Cntl_DS0                              :  1;\
            UNSG32 uGPIO4Cntl_DS1                              :  1;\
            UNSG32 uGPIO4Cntl_DS2                              :  1;\
            UNSG32 uGPIO4Cntl_DS3                              :  1;\
            UNSG32 uGPIO4Cntl_IE                               :  1;\
            UNSG32 uGPIO4Cntl_PE                               :  1;\
            UNSG32 uGPIO4Cntl_PS                               :  1;\
            UNSG32 uGPIO4Cntl_SL                               :  1;\
            UNSG32 uGPIO4Cntl_SPU                              :  1;\
            UNSG32 uGPIO4Cntl_ST                               :  1;\
            UNSG32 RSVDx88A4_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO4Cntl;
            struct w32Gbl_GPIO4Cntl;
          };
    #define   SET32Gbl_GPIO5Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO5Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO5Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO5Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO5Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO5Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO5Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO5Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO5Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO5Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO5Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO5Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO5Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO5Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO5Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO5Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO5Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO5Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO5Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO5Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO5Cntl                               {\
            UNSG32 uGPIO5Cntl_DS0                              :  1;\
            UNSG32 uGPIO5Cntl_DS1                              :  1;\
            UNSG32 uGPIO5Cntl_DS2                              :  1;\
            UNSG32 uGPIO5Cntl_DS3                              :  1;\
            UNSG32 uGPIO5Cntl_IE                               :  1;\
            UNSG32 uGPIO5Cntl_PE                               :  1;\
            UNSG32 uGPIO5Cntl_PS                               :  1;\
            UNSG32 uGPIO5Cntl_SL                               :  1;\
            UNSG32 uGPIO5Cntl_SPU                              :  1;\
            UNSG32 uGPIO5Cntl_ST                               :  1;\
            UNSG32 RSVDx88A8_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO5Cntl;
            struct w32Gbl_GPIO5Cntl;
          };
    #define   SET32Gbl_GPIO6Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO6Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO6Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO6Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO6Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO6Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO6Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO6Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO6Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO6Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO6Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO6Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO6Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO6Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO6Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO6Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO6Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO6Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO6Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO6Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO6Cntl                               {\
            UNSG32 uGPIO6Cntl_DS0                              :  1;\
            UNSG32 uGPIO6Cntl_DS1                              :  1;\
            UNSG32 uGPIO6Cntl_DS2                              :  1;\
            UNSG32 uGPIO6Cntl_DS3                              :  1;\
            UNSG32 uGPIO6Cntl_IE                               :  1;\
            UNSG32 uGPIO6Cntl_PE                               :  1;\
            UNSG32 uGPIO6Cntl_PS                               :  1;\
            UNSG32 uGPIO6Cntl_SL                               :  1;\
            UNSG32 uGPIO6Cntl_SPU                              :  1;\
            UNSG32 uGPIO6Cntl_ST                               :  1;\
            UNSG32 RSVDx88AC_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO6Cntl;
            struct w32Gbl_GPIO6Cntl;
          };
    #define   SET32Gbl_GPIO7Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO7Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO7Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO7Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO7Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO7Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO7Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO7Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO7Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO7Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO7Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO7Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO7Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO7Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO7Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO7Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO7Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO7Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO7Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO7Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO7Cntl                               {\
            UNSG32 uGPIO7Cntl_DS0                              :  1;\
            UNSG32 uGPIO7Cntl_DS1                              :  1;\
            UNSG32 uGPIO7Cntl_DS2                              :  1;\
            UNSG32 uGPIO7Cntl_DS3                              :  1;\
            UNSG32 uGPIO7Cntl_IE                               :  1;\
            UNSG32 uGPIO7Cntl_PE                               :  1;\
            UNSG32 uGPIO7Cntl_PS                               :  1;\
            UNSG32 uGPIO7Cntl_SL                               :  1;\
            UNSG32 uGPIO7Cntl_SPU                              :  1;\
            UNSG32 uGPIO7Cntl_ST                               :  1;\
            UNSG32 RSVDx88B0_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO7Cntl;
            struct w32Gbl_GPIO7Cntl;
          };
    #define   SET32Gbl_GPIO8Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO8Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO8Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO8Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO8Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO8Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO8Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO8Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO8Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO8Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO8Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO8Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO8Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO8Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO8Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO8Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO8Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO8Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO8Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO8Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO8Cntl                               {\
            UNSG32 uGPIO8Cntl_DS0                              :  1;\
            UNSG32 uGPIO8Cntl_DS1                              :  1;\
            UNSG32 uGPIO8Cntl_DS2                              :  1;\
            UNSG32 uGPIO8Cntl_DS3                              :  1;\
            UNSG32 uGPIO8Cntl_IE                               :  1;\
            UNSG32 uGPIO8Cntl_PE                               :  1;\
            UNSG32 uGPIO8Cntl_PS                               :  1;\
            UNSG32 uGPIO8Cntl_SL                               :  1;\
            UNSG32 uGPIO8Cntl_SPU                              :  1;\
            UNSG32 uGPIO8Cntl_ST                               :  1;\
            UNSG32 RSVDx88B4_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO8Cntl;
            struct w32Gbl_GPIO8Cntl;
          };
    #define   SET32Gbl_GPIO9Cntl_DS0(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO9Cntl_DS0(r16,v)                    _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO9Cntl_DS1(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO9Cntl_DS1(r16,v)                    _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO9Cntl_DS2(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO9Cntl_DS2(r16,v)                    _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO9Cntl_DS3(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO9Cntl_DS3(r16,v)                    _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO9Cntl_IE(r32,v)                     _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO9Cntl_IE(r16,v)                     _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO9Cntl_PE(r32,v)                     _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO9Cntl_PE(r16,v)                     _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO9Cntl_PS(r32,v)                     _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO9Cntl_PS(r16,v)                     _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO9Cntl_SL(r32,v)                     _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO9Cntl_SL(r16,v)                     _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO9Cntl_SPU(r32,v)                    _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO9Cntl_SPU(r16,v)                    _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO9Cntl_ST(r32,v)                     _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO9Cntl_ST(r16,v)                     _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO9Cntl                               {\
            UNSG32 uGPIO9Cntl_DS0                              :  1;\
            UNSG32 uGPIO9Cntl_DS1                              :  1;\
            UNSG32 uGPIO9Cntl_DS2                              :  1;\
            UNSG32 uGPIO9Cntl_DS3                              :  1;\
            UNSG32 uGPIO9Cntl_IE                               :  1;\
            UNSG32 uGPIO9Cntl_PE                               :  1;\
            UNSG32 uGPIO9Cntl_PS                               :  1;\
            UNSG32 uGPIO9Cntl_SL                               :  1;\
            UNSG32 uGPIO9Cntl_SPU                              :  1;\
            UNSG32 uGPIO9Cntl_ST                               :  1;\
            UNSG32 RSVDx88B8_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO9Cntl;
            struct w32Gbl_GPIO9Cntl;
          };
    #define   SET32Gbl_GPIO10Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO10Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO10Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO10Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO10Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO10Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO10Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO10Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO10Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO10Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO10Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO10Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO10Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO10Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO10Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO10Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO10Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO10Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO10Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO10Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO10Cntl                              {\
            UNSG32 uGPIO10Cntl_DS0                             :  1;\
            UNSG32 uGPIO10Cntl_DS1                             :  1;\
            UNSG32 uGPIO10Cntl_DS2                             :  1;\
            UNSG32 uGPIO10Cntl_DS3                             :  1;\
            UNSG32 uGPIO10Cntl_IE                              :  1;\
            UNSG32 uGPIO10Cntl_PE                              :  1;\
            UNSG32 uGPIO10Cntl_PS                              :  1;\
            UNSG32 uGPIO10Cntl_SL                              :  1;\
            UNSG32 uGPIO10Cntl_SPU                             :  1;\
            UNSG32 uGPIO10Cntl_ST                              :  1;\
            UNSG32 RSVDx88BC_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO10Cntl;
            struct w32Gbl_GPIO10Cntl;
          };
    #define   SET32Gbl_GPIO11Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO11Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO11Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO11Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO11Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO11Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO11Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO11Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO11Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO11Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO11Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO11Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO11Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO11Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO11Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO11Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO11Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO11Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO11Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO11Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO11Cntl                              {\
            UNSG32 uGPIO11Cntl_DS0                             :  1;\
            UNSG32 uGPIO11Cntl_DS1                             :  1;\
            UNSG32 uGPIO11Cntl_DS2                             :  1;\
            UNSG32 uGPIO11Cntl_DS3                             :  1;\
            UNSG32 uGPIO11Cntl_IE                              :  1;\
            UNSG32 uGPIO11Cntl_PE                              :  1;\
            UNSG32 uGPIO11Cntl_PS                              :  1;\
            UNSG32 uGPIO11Cntl_SL                              :  1;\
            UNSG32 uGPIO11Cntl_SPU                             :  1;\
            UNSG32 uGPIO11Cntl_ST                              :  1;\
            UNSG32 RSVDx88C0_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO11Cntl;
            struct w32Gbl_GPIO11Cntl;
          };
    #define   SET32Gbl_GPIO12Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO12Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO12Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO12Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO12Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO12Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO12Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO12Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO12Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO12Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO12Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO12Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO12Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO12Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO12Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO12Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO12Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO12Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO12Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO12Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO12Cntl                              {\
            UNSG32 uGPIO12Cntl_DS0                             :  1;\
            UNSG32 uGPIO12Cntl_DS1                             :  1;\
            UNSG32 uGPIO12Cntl_DS2                             :  1;\
            UNSG32 uGPIO12Cntl_DS3                             :  1;\
            UNSG32 uGPIO12Cntl_IE                              :  1;\
            UNSG32 uGPIO12Cntl_PE                              :  1;\
            UNSG32 uGPIO12Cntl_PS                              :  1;\
            UNSG32 uGPIO12Cntl_SL                              :  1;\
            UNSG32 uGPIO12Cntl_SPU                             :  1;\
            UNSG32 uGPIO12Cntl_ST                              :  1;\
            UNSG32 RSVDx88C4_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO12Cntl;
            struct w32Gbl_GPIO12Cntl;
          };
    #define   SET32Gbl_GPIO13Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO13Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO13Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO13Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO13Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO13Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO13Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO13Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO13Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO13Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO13Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO13Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO13Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO13Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO13Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO13Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO13Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO13Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO13Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO13Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO13Cntl                              {\
            UNSG32 uGPIO13Cntl_DS0                             :  1;\
            UNSG32 uGPIO13Cntl_DS1                             :  1;\
            UNSG32 uGPIO13Cntl_DS2                             :  1;\
            UNSG32 uGPIO13Cntl_DS3                             :  1;\
            UNSG32 uGPIO13Cntl_IE                              :  1;\
            UNSG32 uGPIO13Cntl_PE                              :  1;\
            UNSG32 uGPIO13Cntl_PS                              :  1;\
            UNSG32 uGPIO13Cntl_SL                              :  1;\
            UNSG32 uGPIO13Cntl_SPU                             :  1;\
            UNSG32 uGPIO13Cntl_ST                              :  1;\
            UNSG32 RSVDx88C8_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO13Cntl;
            struct w32Gbl_GPIO13Cntl;
          };
    #define   SET32Gbl_GPIO14Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO14Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO14Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO14Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO14Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO14Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO14Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO14Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO14Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO14Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO14Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO14Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO14Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO14Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO14Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO14Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO14Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO14Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO14Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO14Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO14Cntl                              {\
            UNSG32 uGPIO14Cntl_DS0                             :  1;\
            UNSG32 uGPIO14Cntl_DS1                             :  1;\
            UNSG32 uGPIO14Cntl_DS2                             :  1;\
            UNSG32 uGPIO14Cntl_DS3                             :  1;\
            UNSG32 uGPIO14Cntl_IE                              :  1;\
            UNSG32 uGPIO14Cntl_PE                              :  1;\
            UNSG32 uGPIO14Cntl_PS                              :  1;\
            UNSG32 uGPIO14Cntl_SL                              :  1;\
            UNSG32 uGPIO14Cntl_SPU                             :  1;\
            UNSG32 uGPIO14Cntl_ST                              :  1;\
            UNSG32 RSVDx88CC_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO14Cntl;
            struct w32Gbl_GPIO14Cntl;
          };
    #define   SET32Gbl_GPIO15Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO15Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO15Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO15Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO15Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO15Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO15Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO15Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO15Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO15Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO15Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO15Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO15Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO15Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO15Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO15Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO15Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO15Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO15Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO15Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO15Cntl                              {\
            UNSG32 uGPIO15Cntl_DS0                             :  1;\
            UNSG32 uGPIO15Cntl_DS1                             :  1;\
            UNSG32 uGPIO15Cntl_DS2                             :  1;\
            UNSG32 uGPIO15Cntl_DS3                             :  1;\
            UNSG32 uGPIO15Cntl_IE                              :  1;\
            UNSG32 uGPIO15Cntl_PE                              :  1;\
            UNSG32 uGPIO15Cntl_PS                              :  1;\
            UNSG32 uGPIO15Cntl_SL                              :  1;\
            UNSG32 uGPIO15Cntl_SPU                             :  1;\
            UNSG32 uGPIO15Cntl_ST                              :  1;\
            UNSG32 RSVDx88D0_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO15Cntl;
            struct w32Gbl_GPIO15Cntl;
          };
    #define   SET32Gbl_GPIO16Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO16Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO16Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO16Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO16Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO16Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO16Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO16Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO16Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO16Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO16Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO16Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO16Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO16Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO16Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO16Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO16Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO16Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO16Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO16Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO16Cntl                              {\
            UNSG32 uGPIO16Cntl_DS0                             :  1;\
            UNSG32 uGPIO16Cntl_DS1                             :  1;\
            UNSG32 uGPIO16Cntl_DS2                             :  1;\
            UNSG32 uGPIO16Cntl_DS3                             :  1;\
            UNSG32 uGPIO16Cntl_IE                              :  1;\
            UNSG32 uGPIO16Cntl_PE                              :  1;\
            UNSG32 uGPIO16Cntl_PS                              :  1;\
            UNSG32 uGPIO16Cntl_SL                              :  1;\
            UNSG32 uGPIO16Cntl_SPU                             :  1;\
            UNSG32 uGPIO16Cntl_ST                              :  1;\
            UNSG32 RSVDx88D4_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO16Cntl;
            struct w32Gbl_GPIO16Cntl;
          };
    #define   SET32Gbl_GPIO17Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO17Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO17Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO17Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO17Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO17Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO17Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO17Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO17Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO17Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO17Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO17Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO17Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO17Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO17Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO17Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO17Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO17Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO17Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO17Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO17Cntl                              {\
            UNSG32 uGPIO17Cntl_DS0                             :  1;\
            UNSG32 uGPIO17Cntl_DS1                             :  1;\
            UNSG32 uGPIO17Cntl_DS2                             :  1;\
            UNSG32 uGPIO17Cntl_DS3                             :  1;\
            UNSG32 uGPIO17Cntl_IE                              :  1;\
            UNSG32 uGPIO17Cntl_PE                              :  1;\
            UNSG32 uGPIO17Cntl_PS                              :  1;\
            UNSG32 uGPIO17Cntl_SL                              :  1;\
            UNSG32 uGPIO17Cntl_SPU                             :  1;\
            UNSG32 uGPIO17Cntl_ST                              :  1;\
            UNSG32 RSVDx88D8_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO17Cntl;
            struct w32Gbl_GPIO17Cntl;
          };
    #define   SET32Gbl_GPIO18Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO18Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO18Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO18Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO18Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO18Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO18Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO18Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO18Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO18Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO18Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO18Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO18Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO18Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO18Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO18Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO18Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO18Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO18Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO18Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO18Cntl                              {\
            UNSG32 uGPIO18Cntl_DS0                             :  1;\
            UNSG32 uGPIO18Cntl_DS1                             :  1;\
            UNSG32 uGPIO18Cntl_DS2                             :  1;\
            UNSG32 uGPIO18Cntl_DS3                             :  1;\
            UNSG32 uGPIO18Cntl_IE                              :  1;\
            UNSG32 uGPIO18Cntl_PE                              :  1;\
            UNSG32 uGPIO18Cntl_PS                              :  1;\
            UNSG32 uGPIO18Cntl_SL                              :  1;\
            UNSG32 uGPIO18Cntl_SPU                             :  1;\
            UNSG32 uGPIO18Cntl_ST                              :  1;\
            UNSG32 RSVDx88DC_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO18Cntl;
            struct w32Gbl_GPIO18Cntl;
          };
    #define   SET32Gbl_GPIO19Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO19Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO19Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO19Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO19Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO19Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO19Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO19Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO19Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO19Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO19Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO19Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO19Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO19Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO19Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO19Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO19Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO19Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO19Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO19Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO19Cntl                              {\
            UNSG32 uGPIO19Cntl_DS0                             :  1;\
            UNSG32 uGPIO19Cntl_DS1                             :  1;\
            UNSG32 uGPIO19Cntl_DS2                             :  1;\
            UNSG32 uGPIO19Cntl_DS3                             :  1;\
            UNSG32 uGPIO19Cntl_IE                              :  1;\
            UNSG32 uGPIO19Cntl_PE                              :  1;\
            UNSG32 uGPIO19Cntl_PS                              :  1;\
            UNSG32 uGPIO19Cntl_SL                              :  1;\
            UNSG32 uGPIO19Cntl_SPU                             :  1;\
            UNSG32 uGPIO19Cntl_ST                              :  1;\
            UNSG32 RSVDx88E0_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO19Cntl;
            struct w32Gbl_GPIO19Cntl;
          };
    #define   SET32Gbl_GPIO20Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO20Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO20Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO20Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO20Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO20Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO20Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO20Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO20Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO20Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO20Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO20Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO20Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO20Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO20Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO20Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO20Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO20Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO20Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO20Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO20Cntl                              {\
            UNSG32 uGPIO20Cntl_DS0                             :  1;\
            UNSG32 uGPIO20Cntl_DS1                             :  1;\
            UNSG32 uGPIO20Cntl_DS2                             :  1;\
            UNSG32 uGPIO20Cntl_DS3                             :  1;\
            UNSG32 uGPIO20Cntl_IE                              :  1;\
            UNSG32 uGPIO20Cntl_PE                              :  1;\
            UNSG32 uGPIO20Cntl_PS                              :  1;\
            UNSG32 uGPIO20Cntl_SL                              :  1;\
            UNSG32 uGPIO20Cntl_SPU                             :  1;\
            UNSG32 uGPIO20Cntl_ST                              :  1;\
            UNSG32 RSVDx88E4_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO20Cntl;
            struct w32Gbl_GPIO20Cntl;
          };
    #define   SET32Gbl_GPIO21Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO21Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO21Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO21Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO21Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO21Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO21Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO21Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO21Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO21Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO21Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO21Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO21Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO21Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO21Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO21Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO21Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO21Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO21Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO21Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO21Cntl                              {\
            UNSG32 uGPIO21Cntl_DS0                             :  1;\
            UNSG32 uGPIO21Cntl_DS1                             :  1;\
            UNSG32 uGPIO21Cntl_DS2                             :  1;\
            UNSG32 uGPIO21Cntl_DS3                             :  1;\
            UNSG32 uGPIO21Cntl_IE                              :  1;\
            UNSG32 uGPIO21Cntl_PE                              :  1;\
            UNSG32 uGPIO21Cntl_PS                              :  1;\
            UNSG32 uGPIO21Cntl_SL                              :  1;\
            UNSG32 uGPIO21Cntl_SPU                             :  1;\
            UNSG32 uGPIO21Cntl_ST                              :  1;\
            UNSG32 RSVDx88E8_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO21Cntl;
            struct w32Gbl_GPIO21Cntl;
          };
    #define   SET32Gbl_GPIO22Cntl_DS0(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   SET16Gbl_GPIO22Cntl_DS0(r16,v)                   _BFSET_(r16, 0, 0,v)
    #define   SET32Gbl_GPIO22Cntl_DS1(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   SET16Gbl_GPIO22Cntl_DS1(r16,v)                   _BFSET_(r16, 1, 1,v)
    #define   SET32Gbl_GPIO22Cntl_DS2(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   SET16Gbl_GPIO22Cntl_DS2(r16,v)                   _BFSET_(r16, 2, 2,v)
    #define   SET32Gbl_GPIO22Cntl_DS3(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   SET16Gbl_GPIO22Cntl_DS3(r16,v)                   _BFSET_(r16, 3, 3,v)
    #define   SET32Gbl_GPIO22Cntl_IE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   SET16Gbl_GPIO22Cntl_IE(r16,v)                    _BFSET_(r16, 4, 4,v)
    #define   SET32Gbl_GPIO22Cntl_PE(r32,v)                    _BFSET_(r32, 5, 5,v)
    #define   SET16Gbl_GPIO22Cntl_PE(r16,v)                    _BFSET_(r16, 5, 5,v)
    #define   SET32Gbl_GPIO22Cntl_PS(r32,v)                    _BFSET_(r32, 6, 6,v)
    #define   SET16Gbl_GPIO22Cntl_PS(r16,v)                    _BFSET_(r16, 6, 6,v)
    #define   SET32Gbl_GPIO22Cntl_SL(r32,v)                    _BFSET_(r32, 7, 7,v)
    #define   SET16Gbl_GPIO22Cntl_SL(r16,v)                    _BFSET_(r16, 7, 7,v)
    #define   SET32Gbl_GPIO22Cntl_SPU(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   SET16Gbl_GPIO22Cntl_SPU(r16,v)                   _BFSET_(r16, 8, 8,v)
    #define   SET32Gbl_GPIO22Cntl_ST(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   SET16Gbl_GPIO22Cntl_ST(r16,v)                    _BFSET_(r16, 9, 9,v)
    #define     w32Gbl_GPIO22Cntl                              {\
            UNSG32 uGPIO22Cntl_DS0                             :  1;\
            UNSG32 uGPIO22Cntl_DS1                             :  1;\
            UNSG32 uGPIO22Cntl_DS2                             :  1;\
            UNSG32 uGPIO22Cntl_DS3                             :  1;\
            UNSG32 uGPIO22Cntl_IE                              :  1;\
            UNSG32 uGPIO22Cntl_PE                              :  1;\
            UNSG32 uGPIO22Cntl_PS                              :  1;\
            UNSG32 uGPIO22Cntl_SL                              :  1;\
            UNSG32 uGPIO22Cntl_SPU                             :  1;\
            UNSG32 uGPIO22Cntl_ST                              :  1;\
            UNSG32 RSVDx88EC_b10                               : 22;\
          }
    union { UNSG32 u32Gbl_GPIO22Cntl;
            struct w32Gbl_GPIO22Cntl;
          };
    } SIE_Gbl;
    typedef union  T32Gbl_ProductId
          { UNSG32 u32;
            struct w32Gbl_ProductId;
                 } T32Gbl_ProductId;
    typedef union  T32Gbl_ProductId_ext
          { UNSG32 u32;
            struct w32Gbl_ProductId_ext;
                 } T32Gbl_ProductId_ext;
    typedef union  T32Gbl_INT_ID
          { UNSG32 u32;
            struct w32Gbl_INT_ID;
                 } T32Gbl_INT_ID;
    typedef union  T32Gbl_bootStrap
          { UNSG32 u32;
            struct w32Gbl_bootStrap;
                 } T32Gbl_bootStrap;
    typedef union  T32Gbl_bootStrapEn
          { UNSG32 u32;
            struct w32Gbl_bootStrapEn;
                 } T32Gbl_bootStrapEn;
    typedef union  T32Gbl_wounding_mcu2soc
          { UNSG32 u32;
            struct w32Gbl_wounding_mcu2soc;
                 } T32Gbl_wounding_mcu2soc;
    typedef union  T32Gbl_chipCntl
          { UNSG32 u32;
            struct w32Gbl_chipCntl;
                 } T32Gbl_chipCntl;
    typedef union  T32Gbl_AVIO_TRIG
          { UNSG32 u32;
            struct w32Gbl_AVIO_TRIG;
                 } T32Gbl_AVIO_TRIG;
    typedef union  T32Gbl_chip_debug
          { UNSG32 u32;
            struct w32Gbl_chip_debug;
                 } T32Gbl_chip_debug;
    typedef union  T32Gbl_sw_generic0
          { UNSG32 u32;
            struct w32Gbl_sw_generic0;
                 } T32Gbl_sw_generic0;
    typedef union  T32Gbl_sw_generic1
          { UNSG32 u32;
            struct w32Gbl_sw_generic1;
                 } T32Gbl_sw_generic1;
    typedef union  T32Gbl_sw_generic2
          { UNSG32 u32;
            struct w32Gbl_sw_generic2;
                 } T32Gbl_sw_generic2;
    typedef union  T32Gbl_sw_generic3
          { UNSG32 u32;
            struct w32Gbl_sw_generic3;
                 } T32Gbl_sw_generic3;
    typedef union  T32Gbl_FPGAR
          { UNSG32 u32;
            struct w32Gbl_FPGAR;
                 } T32Gbl_FPGAR;
    typedef union  T32Gbl_FPGARW
          { UNSG32 u32;
            struct w32Gbl_FPGARW;
                 } T32Gbl_FPGARW;
    typedef union  T32Gbl_gfx3D_pwr_ctrl
          { UNSG32 u32;
            struct w32Gbl_gfx3D_pwr_ctrl;
                 } T32Gbl_gfx3D_pwr_ctrl;
    typedef union  T32Gbl_gfx3D_pwr_sts
          { UNSG32 u32;
            struct w32Gbl_gfx3D_pwr_sts;
                 } T32Gbl_gfx3D_pwr_sts;
    typedef union  T32Gbl_npu_pwr_ctrl
          { UNSG32 u32;
            struct w32Gbl_npu_pwr_ctrl;
                 } T32Gbl_npu_pwr_ctrl;
    typedef union  T32Gbl_npu_pwr_sts
          { UNSG32 u32;
            struct w32Gbl_npu_pwr_sts;
                 } T32Gbl_npu_pwr_sts;
    typedef union  T32Gbl_POR_EN_status
          { UNSG32 u32;
            struct w32Gbl_POR_EN_status;
                 } T32Gbl_POR_EN_status;
    typedef union  T32Gbl_POR_EN_OVRD
          { UNSG32 u32;
            struct w32Gbl_POR_EN_OVRD;
                 } T32Gbl_POR_EN_OVRD;
    typedef union  T32Gbl_POR_status
          { UNSG32 u32;
            struct w32Gbl_POR_status;
                 } T32Gbl_POR_status;
    typedef union  T32Gbl_POR_CTL
          { UNSG32 u32;
            struct w32Gbl_POR_CTL;
                 } T32Gbl_POR_CTL;
    typedef union  T32Gbl_ResetTrigger
          { UNSG32 u32;
            struct w32Gbl_ResetTrigger;
                 } T32Gbl_ResetTrigger;
    typedef union  T32Gbl_ResetStatus
          { UNSG32 u32;
            struct w32Gbl_ResetStatus;
                 } T32Gbl_ResetStatus;
    typedef union  T32Gbl_WDTResetStatus
          { UNSG32 u32;
            struct w32Gbl_WDTResetStatus;
                 } T32Gbl_WDTResetStatus;
    typedef union  T32Gbl_WDTSysRstMask
          { UNSG32 u32;
            struct w32Gbl_WDTSysRstMask;
                 } T32Gbl_WDTSysRstMask;
    typedef union  T32Gbl_CHIP_RESET_TRACKER
          { UNSG32 u32;
            struct w32Gbl_CHIP_RESET_TRACKER;
                 } T32Gbl_CHIP_RESET_TRACKER;
    typedef union  T32Gbl_avioReset
          { UNSG32 u32;
            struct w32Gbl_avioReset;
                 } T32Gbl_avioReset;
    typedef union  T32Gbl_avioResetStatus
          { UNSG32 u32;
            struct w32Gbl_avioResetStatus;
                 } T32Gbl_avioResetStatus;
    typedef union  T32Gbl_perifReset
          { UNSG32 u32;
            struct w32Gbl_perifReset;
                 } T32Gbl_perifReset;
    typedef union  T32Gbl_perifResetStatus
          { UNSG32 u32;
            struct w32Gbl_perifResetStatus;
                 } T32Gbl_perifResetStatus;
    typedef union  T32Gbl_perifStickyResetN
          { UNSG32 u32;
            struct w32Gbl_perifStickyResetN;
                 } T32Gbl_perifStickyResetN;
    typedef union  T32Gbl_apbPerifResetTrigger
          { UNSG32 u32;
            struct w32Gbl_apbPerifResetTrigger;
                 } T32Gbl_apbPerifResetTrigger;
    typedef union  T32Gbl_apbPerifResetStatus
          { UNSG32 u32;
            struct w32Gbl_apbPerifResetStatus;
                 } T32Gbl_apbPerifResetStatus;
    typedef union  T32Gbl_topStickyResetN
          { UNSG32 u32;
            struct w32Gbl_topStickyResetN;
                 } T32Gbl_topStickyResetN;
    typedef union  T32Gbl_avioStickyResetN
          { UNSG32 u32;
            struct w32Gbl_avioStickyResetN;
                 } T32Gbl_avioStickyResetN;
    typedef union  T32Gbl_ClkSwitch
          { UNSG32 u32;
            struct w32Gbl_ClkSwitch;
                 } T32Gbl_ClkSwitch;
    typedef union  T32Gbl_clkEnable
          { UNSG32 u32;
            struct w32Gbl_clkEnable;
                 } T32Gbl_clkEnable;
    typedef union  T32Gbl_USBOTG_REFCLK_CTRL0
          { UNSG32 u32;
            struct w32Gbl_USBOTG_REFCLK_CTRL0;
                 } T32Gbl_USBOTG_REFCLK_CTRL0;
    typedef union  T32Gbl_USBOTG_REFCLK_CTRL1
          { UNSG32 u32;
            struct w32Gbl_USBOTG_REFCLK_CTRL1;
                 } T32Gbl_USBOTG_REFCLK_CTRL1;
    typedef union  T32Gbl_USBOTG1_REFCLK_CTRL0
          { UNSG32 u32;
            struct w32Gbl_USBOTG1_REFCLK_CTRL0;
                 } T32Gbl_USBOTG1_REFCLK_CTRL0;
    typedef union  T32Gbl_USBOTG1_REFCLK_CTRL1
          { UNSG32 u32;
            struct w32Gbl_USBOTG1_REFCLK_CTRL1;
                 } T32Gbl_USBOTG1_REFCLK_CTRL1;
    typedef union  T32Gbl_SECURE_SCAN_EN
          { UNSG32 u32;
            struct w32Gbl_SECURE_SCAN_EN;
                 } T32Gbl_SECURE_SCAN_EN;
    typedef union  T32Gbl_gic400_ctrl
          { UNSG32 u32;
            struct w32Gbl_gic400_ctrl;
                 } T32Gbl_gic400_ctrl;
    typedef union  T32Gbl_LCDD_IO_CTRL
          { UNSG32 u32;
            struct w32Gbl_LCDD_IO_CTRL;
                 } T32Gbl_LCDD_IO_CTRL;
    typedef union  T32Gbl_SOC_PLL_MUX
          { UNSG32 u32;
            struct w32Gbl_SOC_PLL_MUX;
                 } T32Gbl_SOC_PLL_MUX;
    typedef union  T32Gbl_ge0_ptp_mux
          { UNSG32 u32;
            struct w32Gbl_ge0_ptp_mux;
                 } T32Gbl_ge0_ptp_mux;
    typedef union  T32Gbl_ge1_ptp_mux
          { UNSG32 u32;
            struct w32Gbl_ge1_ptp_mux;
                 } T32Gbl_ge1_ptp_mux;
    typedef union  T32Gbl_gfx_3d
          { UNSG32 u32;
            struct w32Gbl_gfx_3d;
                 } T32Gbl_gfx_3d;
    typedef union  T32Gbl_pdma_dst_req_mask
          { UNSG32 u32;
            struct w32Gbl_pdma_dst_req_mask;
                 } T32Gbl_pdma_dst_req_mask;
    typedef union  T32Gbl_pdma_src_req_mask
          { UNSG32 u32;
            struct w32Gbl_pdma_src_req_mask;
                 } T32Gbl_pdma_src_req_mask;
    typedef union  T32Gbl_pinmux_cntl_bus
          { UNSG32 u32;
            struct w32Gbl_pinmux_cntl_bus;
                 } T32Gbl_pinmux_cntl_bus;
    typedef union  T32Gbl_pinmux_cntl_bus1
          { UNSG32 u32;
            struct w32Gbl_pinmux_cntl_bus1;
                 } T32Gbl_pinmux_cntl_bus1;
    typedef union  T32Gbl_pinmux_cntl_bus2
          { UNSG32 u32;
            struct w32Gbl_pinmux_cntl_bus2;
                 } T32Gbl_pinmux_cntl_bus2;
    typedef union  T32Gbl_pinmux_cntl_bus3
          { UNSG32 u32;
            struct w32Gbl_pinmux_cntl_bus3;
                 } T32Gbl_pinmux_cntl_bus3;
    typedef union  T32Gbl_pinmux_cntl_bus4
          { UNSG32 u32;
            struct w32Gbl_pinmux_cntl_bus4;
                 } T32Gbl_pinmux_cntl_bus4;
    typedef union  T32Gbl_pinmux_cntl_bus5
          { UNSG32 u32;
            struct w32Gbl_pinmux_cntl_bus5;
                 } T32Gbl_pinmux_cntl_bus5;
    typedef union  T32Gbl_GPIO23Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO23Cntl;
                 } T32Gbl_GPIO23Cntl;
    typedef union  T32Gbl_GPIO24Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO24Cntl;
                 } T32Gbl_GPIO24Cntl;
    typedef union  T32Gbl_GPIO25Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO25Cntl;
                 } T32Gbl_GPIO25Cntl;
    typedef union  T32Gbl_GPIO26Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO26Cntl;
                 } T32Gbl_GPIO26Cntl;
    typedef union  T32Gbl_GPIO27Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO27Cntl;
                 } T32Gbl_GPIO27Cntl;
    typedef union  T32Gbl_GPIO28Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO28Cntl;
                 } T32Gbl_GPIO28Cntl;
    typedef union  T32Gbl_GPIO29Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO29Cntl;
                 } T32Gbl_GPIO29Cntl;
    typedef union  T32Gbl_GPIO30Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO30Cntl;
                 } T32Gbl_GPIO30Cntl;
    typedef union  T32Gbl_GPIO31Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO31Cntl;
                 } T32Gbl_GPIO31Cntl;
    typedef union  T32Gbl_GPIO32Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO32Cntl;
                 } T32Gbl_GPIO32Cntl;
    typedef union  T32Gbl_GPIO33Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO33Cntl;
                 } T32Gbl_GPIO33Cntl;
    typedef union  T32Gbl_GPIO34Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO34Cntl;
                 } T32Gbl_GPIO34Cntl;
    typedef union  T32Gbl_GPIO35Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO35Cntl;
                 } T32Gbl_GPIO35Cntl;
    typedef union  T32Gbl_GPIO36Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO36Cntl;
                 } T32Gbl_GPIO36Cntl;
    typedef union  T32Gbl_GPIO37Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO37Cntl;
                 } T32Gbl_GPIO37Cntl;
    typedef union  T32Gbl_GPIO38Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO38Cntl;
                 } T32Gbl_GPIO38Cntl;
    typedef union  T32Gbl_GPIO39Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO39Cntl;
                 } T32Gbl_GPIO39Cntl;
    typedef union  T32Gbl_GPIO40Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO40Cntl;
                 } T32Gbl_GPIO40Cntl;
    typedef union  T32Gbl_GPIO41Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO41Cntl;
                 } T32Gbl_GPIO41Cntl;
    typedef union  T32Gbl_GPIO42Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO42Cntl;
                 } T32Gbl_GPIO42Cntl;
    typedef union  T32Gbl_GPIO43Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO43Cntl;
                 } T32Gbl_GPIO43Cntl;
    typedef union  T32Gbl_GPIO44Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO44Cntl;
                 } T32Gbl_GPIO44Cntl;
    typedef union  T32Gbl_GPIO45Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO45Cntl;
                 } T32Gbl_GPIO45Cntl;
    typedef union  T32Gbl_GPIO46Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO46Cntl;
                 } T32Gbl_GPIO46Cntl;
    typedef union  T32Gbl_GPIO47Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO47Cntl;
                 } T32Gbl_GPIO47Cntl;
    typedef union  T32Gbl_GPIO48Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO48Cntl;
                 } T32Gbl_GPIO48Cntl;
    typedef union  T32Gbl_GPIO49Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO49Cntl;
                 } T32Gbl_GPIO49Cntl;
    typedef union  T32Gbl_GPIO50Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO50Cntl;
                 } T32Gbl_GPIO50Cntl;
    typedef union  T32Gbl_GPIO51Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO51Cntl;
                 } T32Gbl_GPIO51Cntl;
    typedef union  T32Gbl_GPIO52Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO52Cntl;
                 } T32Gbl_GPIO52Cntl;
    typedef union  T32Gbl_GPIO53Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO53Cntl;
                 } T32Gbl_GPIO53Cntl;
    typedef union  T32Gbl_GPIO54Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO54Cntl;
                 } T32Gbl_GPIO54Cntl;
    typedef union  T32Gbl_GPIO55Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO55Cntl;
                 } T32Gbl_GPIO55Cntl;
    typedef union  T32Gbl_GPIO56Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO56Cntl;
                 } T32Gbl_GPIO56Cntl;
    typedef union  T32Gbl_GPIO57Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO57Cntl;
                 } T32Gbl_GPIO57Cntl;
    typedef union  T32Gbl_GPIO58Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO58Cntl;
                 } T32Gbl_GPIO58Cntl;
    typedef union  T32Gbl_GPIO59Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO59Cntl;
                 } T32Gbl_GPIO59Cntl;
    typedef union  T32Gbl_GPIO0Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO0Cntl;
                 } T32Gbl_GPIO0Cntl;
    typedef union  T32Gbl_GPIO1Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO1Cntl;
                 } T32Gbl_GPIO1Cntl;
    typedef union  T32Gbl_GPIO2Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO2Cntl;
                 } T32Gbl_GPIO2Cntl;
    typedef union  T32Gbl_GPIO3Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO3Cntl;
                 } T32Gbl_GPIO3Cntl;
    typedef union  T32Gbl_GPIO4Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO4Cntl;
                 } T32Gbl_GPIO4Cntl;
    typedef union  T32Gbl_GPIO5Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO5Cntl;
                 } T32Gbl_GPIO5Cntl;
    typedef union  T32Gbl_GPIO6Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO6Cntl;
                 } T32Gbl_GPIO6Cntl;
    typedef union  T32Gbl_GPIO7Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO7Cntl;
                 } T32Gbl_GPIO7Cntl;
    typedef union  T32Gbl_GPIO8Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO8Cntl;
                 } T32Gbl_GPIO8Cntl;
    typedef union  T32Gbl_GPIO9Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO9Cntl;
                 } T32Gbl_GPIO9Cntl;
    typedef union  T32Gbl_GPIO10Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO10Cntl;
                 } T32Gbl_GPIO10Cntl;
    typedef union  T32Gbl_GPIO11Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO11Cntl;
                 } T32Gbl_GPIO11Cntl;
    typedef union  T32Gbl_GPIO12Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO12Cntl;
                 } T32Gbl_GPIO12Cntl;
    typedef union  T32Gbl_GPIO13Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO13Cntl;
                 } T32Gbl_GPIO13Cntl;
    typedef union  T32Gbl_GPIO14Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO14Cntl;
                 } T32Gbl_GPIO14Cntl;
    typedef union  T32Gbl_GPIO15Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO15Cntl;
                 } T32Gbl_GPIO15Cntl;
    typedef union  T32Gbl_GPIO16Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO16Cntl;
                 } T32Gbl_GPIO16Cntl;
    typedef union  T32Gbl_GPIO17Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO17Cntl;
                 } T32Gbl_GPIO17Cntl;
    typedef union  T32Gbl_GPIO18Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO18Cntl;
                 } T32Gbl_GPIO18Cntl;
    typedef union  T32Gbl_GPIO19Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO19Cntl;
                 } T32Gbl_GPIO19Cntl;
    typedef union  T32Gbl_GPIO20Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO20Cntl;
                 } T32Gbl_GPIO20Cntl;
    typedef union  T32Gbl_GPIO21Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO21Cntl;
                 } T32Gbl_GPIO21Cntl;
    typedef union  T32Gbl_GPIO22Cntl
          { UNSG32 u32;
            struct w32Gbl_GPIO22Cntl;
                 } T32Gbl_GPIO22Cntl;
    typedef union  TGbl_ProductId
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ProductId;
                   };
                 } TGbl_ProductId;
    typedef union  TGbl_ProductId_ext
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ProductId_ext;
                   };
                 } TGbl_ProductId_ext;
    typedef union  TGbl_INT_ID
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_INT_ID;
                   };
                 } TGbl_INT_ID;
    typedef union  TGbl_bootStrap
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_bootStrap;
                   };
                 } TGbl_bootStrap;
    typedef union  TGbl_bootStrapEn
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_bootStrapEn;
                   };
                 } TGbl_bootStrapEn;
    typedef union  TGbl_wounding_mcu2soc
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_wounding_mcu2soc;
                   };
                 } TGbl_wounding_mcu2soc;
    typedef union  TGbl_chipCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_chipCntl;
                   };
                 } TGbl_chipCntl;
    typedef union  TGbl_AVIO_TRIG
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_AVIO_TRIG;
                   };
                 } TGbl_AVIO_TRIG;
    typedef union  TGbl_chip_debug
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_chip_debug;
                   };
                 } TGbl_chip_debug;
    typedef union  TGbl_sw_generic0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_sw_generic0;
                   };
                 } TGbl_sw_generic0;
    typedef union  TGbl_sw_generic1
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_sw_generic1;
                   };
                 } TGbl_sw_generic1;
    typedef union  TGbl_sw_generic2
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_sw_generic2;
                   };
                 } TGbl_sw_generic2;
    typedef union  TGbl_sw_generic3
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_sw_generic3;
                   };
                 } TGbl_sw_generic3;
    typedef union  TGbl_FPGAR
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_FPGAR;
                   };
                 } TGbl_FPGAR;
    typedef union  TGbl_FPGARW
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_FPGARW;
                   };
                 } TGbl_FPGARW;
    typedef union  TGbl_gfx3D_pwr_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_gfx3D_pwr_ctrl;
                   };
                 } TGbl_gfx3D_pwr_ctrl;
    typedef union  TGbl_gfx3D_pwr_sts
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_gfx3D_pwr_sts;
                   };
                 } TGbl_gfx3D_pwr_sts;
    typedef union  TGbl_npu_pwr_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_npu_pwr_ctrl;
                   };
                 } TGbl_npu_pwr_ctrl;
    typedef union  TGbl_npu_pwr_sts
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_npu_pwr_sts;
                   };
                 } TGbl_npu_pwr_sts;
    typedef union  TGbl_POR_EN_status
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_POR_EN_status;
                   };
                 } TGbl_POR_EN_status;
    typedef union  TGbl_POR_EN_OVRD
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_POR_EN_OVRD;
                   };
                 } TGbl_POR_EN_OVRD;
    typedef union  TGbl_POR_status
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_POR_status;
                   };
                 } TGbl_POR_status;
    typedef union  TGbl_POR_CTL
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_POR_CTL;
                   };
                 } TGbl_POR_CTL;
    typedef union  TGbl_ResetTrigger
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ResetTrigger;
                   };
                 } TGbl_ResetTrigger;
    typedef union  TGbl_ResetStatus
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ResetStatus;
                   };
                 } TGbl_ResetStatus;
    typedef union  TGbl_WDTResetStatus
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_WDTResetStatus;
                   };
                 } TGbl_WDTResetStatus;
    typedef union  TGbl_WDTSysRstMask
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_WDTSysRstMask;
                   };
                 } TGbl_WDTSysRstMask;
    typedef union  TGbl_CHIP_RESET_TRACKER
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_CHIP_RESET_TRACKER;
                   };
                 } TGbl_CHIP_RESET_TRACKER;
    typedef union  TGbl_avioReset
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_avioReset;
                   };
                 } TGbl_avioReset;
    typedef union  TGbl_avioResetStatus
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_avioResetStatus;
                   };
                 } TGbl_avioResetStatus;
    typedef union  TGbl_perifReset
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_perifReset;
                   };
                 } TGbl_perifReset;
    typedef union  TGbl_perifResetStatus
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_perifResetStatus;
                   };
                 } TGbl_perifResetStatus;
    typedef union  TGbl_perifStickyResetN
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_perifStickyResetN;
                   };
                 } TGbl_perifStickyResetN;
    typedef union  TGbl_apbPerifResetTrigger
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_apbPerifResetTrigger;
                   };
                 } TGbl_apbPerifResetTrigger;
    typedef union  TGbl_apbPerifResetStatus
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_apbPerifResetStatus;
                   };
                 } TGbl_apbPerifResetStatus;
    typedef union  TGbl_topStickyResetN
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_topStickyResetN;
                   };
                 } TGbl_topStickyResetN;
    typedef union  TGbl_avioStickyResetN
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_avioStickyResetN;
                   };
                 } TGbl_avioStickyResetN;
    typedef union  TGbl_ClkSwitch
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ClkSwitch;
                   };
                 } TGbl_ClkSwitch;
    typedef union  TGbl_clkEnable
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_clkEnable;
                   };
                 } TGbl_clkEnable;
    typedef union  TGbl_USBOTG_REFCLK_CTRL0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_USBOTG_REFCLK_CTRL0;
                   };
                 } TGbl_USBOTG_REFCLK_CTRL0;
    typedef union  TGbl_USBOTG_REFCLK_CTRL1
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_USBOTG_REFCLK_CTRL1;
                   };
                 } TGbl_USBOTG_REFCLK_CTRL1;
    typedef union  TGbl_USBOTG1_REFCLK_CTRL0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_USBOTG1_REFCLK_CTRL0;
                   };
                 } TGbl_USBOTG1_REFCLK_CTRL0;
    typedef union  TGbl_USBOTG1_REFCLK_CTRL1
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_USBOTG1_REFCLK_CTRL1;
                   };
                 } TGbl_USBOTG1_REFCLK_CTRL1;
    typedef union  TGbl_SECURE_SCAN_EN
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SECURE_SCAN_EN;
                   };
                 } TGbl_SECURE_SCAN_EN;
    typedef union  TGbl_gic400_ctrl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_gic400_ctrl;
                   };
                 } TGbl_gic400_ctrl;
    typedef union  TGbl_LCDD_IO_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_LCDD_IO_CTRL;
                   };
                 } TGbl_LCDD_IO_CTRL;
    typedef union  TGbl_SOC_PLL_MUX
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SOC_PLL_MUX;
                   };
                 } TGbl_SOC_PLL_MUX;
    typedef union  TGbl_ge0_ptp_mux
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ge0_ptp_mux;
                   };
                 } TGbl_ge0_ptp_mux;
    typedef union  TGbl_ge1_ptp_mux
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ge1_ptp_mux;
                   };
                 } TGbl_ge1_ptp_mux;
    typedef union  TGbl_gfx_3d
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_gfx_3d;
                   };
                 } TGbl_gfx_3d;
    typedef union  TGbl_pdma_dst_req_mask
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_pdma_dst_req_mask;
                   };
                 } TGbl_pdma_dst_req_mask;
    typedef union  TGbl_pdma_src_req_mask
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_pdma_src_req_mask;
                   };
                 } TGbl_pdma_src_req_mask;
    typedef union  TGbl_pinmux_cntl_bus
          { UNSG32 u32[6];
            struct {
            struct w32Gbl_pinmux_cntl_bus;
            struct w32Gbl_pinmux_cntl_bus1;
            struct w32Gbl_pinmux_cntl_bus2;
            struct w32Gbl_pinmux_cntl_bus3;
            struct w32Gbl_pinmux_cntl_bus4;
            struct w32Gbl_pinmux_cntl_bus5;
                   };
                 } TGbl_pinmux_cntl_bus;
    typedef union  TGbl_GPIO23Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO23Cntl;
                   };
                 } TGbl_GPIO23Cntl;
    typedef union  TGbl_GPIO24Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO24Cntl;
                   };
                 } TGbl_GPIO24Cntl;
    typedef union  TGbl_GPIO25Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO25Cntl;
                   };
                 } TGbl_GPIO25Cntl;
    typedef union  TGbl_GPIO26Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO26Cntl;
                   };
                 } TGbl_GPIO26Cntl;
    typedef union  TGbl_GPIO27Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO27Cntl;
                   };
                 } TGbl_GPIO27Cntl;
    typedef union  TGbl_GPIO28Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO28Cntl;
                   };
                 } TGbl_GPIO28Cntl;
    typedef union  TGbl_GPIO29Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO29Cntl;
                   };
                 } TGbl_GPIO29Cntl;
    typedef union  TGbl_GPIO30Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO30Cntl;
                   };
                 } TGbl_GPIO30Cntl;
    typedef union  TGbl_GPIO31Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO31Cntl;
                   };
                 } TGbl_GPIO31Cntl;
    typedef union  TGbl_GPIO32Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO32Cntl;
                   };
                 } TGbl_GPIO32Cntl;
    typedef union  TGbl_GPIO33Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO33Cntl;
                   };
                 } TGbl_GPIO33Cntl;
    typedef union  TGbl_GPIO34Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO34Cntl;
                   };
                 } TGbl_GPIO34Cntl;
    typedef union  TGbl_GPIO35Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO35Cntl;
                   };
                 } TGbl_GPIO35Cntl;
    typedef union  TGbl_GPIO36Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO36Cntl;
                   };
                 } TGbl_GPIO36Cntl;
    typedef union  TGbl_GPIO37Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO37Cntl;
                   };
                 } TGbl_GPIO37Cntl;
    typedef union  TGbl_GPIO38Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO38Cntl;
                   };
                 } TGbl_GPIO38Cntl;
    typedef union  TGbl_GPIO39Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO39Cntl;
                   };
                 } TGbl_GPIO39Cntl;
    typedef union  TGbl_GPIO40Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO40Cntl;
                   };
                 } TGbl_GPIO40Cntl;
    typedef union  TGbl_GPIO41Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO41Cntl;
                   };
                 } TGbl_GPIO41Cntl;
    typedef union  TGbl_GPIO42Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO42Cntl;
                   };
                 } TGbl_GPIO42Cntl;
    typedef union  TGbl_GPIO43Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO43Cntl;
                   };
                 } TGbl_GPIO43Cntl;
    typedef union  TGbl_GPIO44Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO44Cntl;
                   };
                 } TGbl_GPIO44Cntl;
    typedef union  TGbl_GPIO45Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO45Cntl;
                   };
                 } TGbl_GPIO45Cntl;
    typedef union  TGbl_GPIO46Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO46Cntl;
                   };
                 } TGbl_GPIO46Cntl;
    typedef union  TGbl_GPIO47Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO47Cntl;
                   };
                 } TGbl_GPIO47Cntl;
    typedef union  TGbl_GPIO48Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO48Cntl;
                   };
                 } TGbl_GPIO48Cntl;
    typedef union  TGbl_GPIO49Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO49Cntl;
                   };
                 } TGbl_GPIO49Cntl;
    typedef union  TGbl_GPIO50Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO50Cntl;
                   };
                 } TGbl_GPIO50Cntl;
    typedef union  TGbl_GPIO51Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO51Cntl;
                   };
                 } TGbl_GPIO51Cntl;
    typedef union  TGbl_GPIO52Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO52Cntl;
                   };
                 } TGbl_GPIO52Cntl;
    typedef union  TGbl_GPIO53Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO53Cntl;
                   };
                 } TGbl_GPIO53Cntl;
    typedef union  TGbl_GPIO54Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO54Cntl;
                   };
                 } TGbl_GPIO54Cntl;
    typedef union  TGbl_GPIO55Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO55Cntl;
                   };
                 } TGbl_GPIO55Cntl;
    typedef union  TGbl_GPIO56Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO56Cntl;
                   };
                 } TGbl_GPIO56Cntl;
    typedef union  TGbl_GPIO57Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO57Cntl;
                   };
                 } TGbl_GPIO57Cntl;
    typedef union  TGbl_GPIO58Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO58Cntl;
                   };
                 } TGbl_GPIO58Cntl;
    typedef union  TGbl_GPIO59Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO59Cntl;
                   };
                 } TGbl_GPIO59Cntl;
    typedef union  TGbl_GPIO0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO0Cntl;
                   };
                 } TGbl_GPIO0Cntl;
    typedef union  TGbl_GPIO1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO1Cntl;
                   };
                 } TGbl_GPIO1Cntl;
    typedef union  TGbl_GPIO2Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO2Cntl;
                   };
                 } TGbl_GPIO2Cntl;
    typedef union  TGbl_GPIO3Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO3Cntl;
                   };
                 } TGbl_GPIO3Cntl;
    typedef union  TGbl_GPIO4Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO4Cntl;
                   };
                 } TGbl_GPIO4Cntl;
    typedef union  TGbl_GPIO5Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO5Cntl;
                   };
                 } TGbl_GPIO5Cntl;
    typedef union  TGbl_GPIO6Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO6Cntl;
                   };
                 } TGbl_GPIO6Cntl;
    typedef union  TGbl_GPIO7Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO7Cntl;
                   };
                 } TGbl_GPIO7Cntl;
    typedef union  TGbl_GPIO8Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO8Cntl;
                   };
                 } TGbl_GPIO8Cntl;
    typedef union  TGbl_GPIO9Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO9Cntl;
                   };
                 } TGbl_GPIO9Cntl;
    typedef union  TGbl_GPIO10Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO10Cntl;
                   };
                 } TGbl_GPIO10Cntl;
    typedef union  TGbl_GPIO11Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO11Cntl;
                   };
                 } TGbl_GPIO11Cntl;
    typedef union  TGbl_GPIO12Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO12Cntl;
                   };
                 } TGbl_GPIO12Cntl;
    typedef union  TGbl_GPIO13Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO13Cntl;
                   };
                 } TGbl_GPIO13Cntl;
    typedef union  TGbl_GPIO14Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO14Cntl;
                   };
                 } TGbl_GPIO14Cntl;
    typedef union  TGbl_GPIO15Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO15Cntl;
                   };
                 } TGbl_GPIO15Cntl;
    typedef union  TGbl_GPIO16Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO16Cntl;
                   };
                 } TGbl_GPIO16Cntl;
    typedef union  TGbl_GPIO17Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO17Cntl;
                   };
                 } TGbl_GPIO17Cntl;
    typedef union  TGbl_GPIO18Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO18Cntl;
                   };
                 } TGbl_GPIO18Cntl;
    typedef union  TGbl_GPIO19Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO19Cntl;
                   };
                 } TGbl_GPIO19Cntl;
    typedef union  TGbl_GPIO20Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO20Cntl;
                   };
                 } TGbl_GPIO20Cntl;
    typedef union  TGbl_GPIO21Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO21Cntl;
                   };
                 } TGbl_GPIO21Cntl;
    typedef union  TGbl_GPIO22Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_GPIO22Cntl;
                   };
                 } TGbl_GPIO22Cntl;
     SIGN32 Gbl_drvrd(SIE_Gbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 Gbl_drvwr(SIE_Gbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void Gbl_reset(SIE_Gbl *p);
     SIGN32 Gbl_cmp  (SIE_Gbl *p, SIE_Gbl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define Gbl_check(p,pie,pfx,hLOG) Gbl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define Gbl_print(p,    pfx,hLOG) Gbl_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifdef __cplusplus
  }
#endif
#pragma  pack()
#endif
