// SPDX-License-Identifier: GPL-2.0
/*
* Copyright (C) 2025 Synaptics Incorporated
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*/
#ifndef csiPipe_h
#define csiPipe_h (){}
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
#ifndef h_TG_PL
#define h_TG_PL (){}
    #define     RA_TG_PL_X                                     0x0000
    #define   LSb32TG_PL_X_start                                  0
    #define       bTG_PL_X_start                               13
    #define   MSK32TG_PL_X_start                                  0x00001FFF
    #define   LSb32TG_PL_X_end                                    13
    #define       bTG_PL_X_end                                 13
    #define   MSK32TG_PL_X_end                                    0x03FFE000
    #define     RA_TG_PL_Y                                     0x0004
    #define   LSb32TG_PL_Y_start                                  0
    #define       bTG_PL_Y_start                               12
    #define   MSK32TG_PL_Y_start                                  0x00000FFF
    #define   LSb32TG_PL_Y_end                                    12
    #define       bTG_PL_Y_end                                 12
    #define   MSK32TG_PL_Y_end                                    0x00FFF000
    typedef struct SIE_TG_PL {
    #define     w32TG_PL_X                                     {\
            UNSG32 uX_start                                    : 13;\
            UNSG32 uX_end                                      : 13;\
            UNSG32 RSVDx0_b26                                  :  6;\
          }
    union { UNSG32 u32TG_PL_X;
            struct w32TG_PL_X;
          };
    #define     w32TG_PL_Y                                     {\
            UNSG32 uY_start                                    : 12;\
            UNSG32 uY_end                                      : 12;\
            UNSG32 RSVDx4_b24                                  :  8;\
          }
    union { UNSG32 u32TG_PL_Y;
            struct w32TG_PL_Y;
          };
    } SIE_TG_PL;
    typedef union  T32TG_PL_X
          { UNSG32 u32;
            struct w32TG_PL_X;
                 } T32TG_PL_X;
    typedef union  T32TG_PL_Y
          { UNSG32 u32;
            struct w32TG_PL_Y;
                 } T32TG_PL_Y;
    typedef union  TTG_PL_X
          { UNSG32 u32[1];
            struct {
            struct w32TG_PL_X;
                   };
                 } TTG_PL_X;
    typedef union  TTG_PL_Y
          { UNSG32 u32[1];
            struct {
            struct w32TG_PL_Y;
                   };
                 } TTG_PL_Y;
     SIGN32 TG_PL_drvrd(SIE_TG_PL *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TG_PL_drvwr(SIE_TG_PL *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TG_PL_reset(SIE_TG_PL *p);
     SIGN32 TG_PL_cmp  (SIE_TG_PL *p, SIE_TG_PL *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TG_PL_check(p,pie,pfx,hLOG) TG_PL_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TG_PL_print(p,    pfx,hLOG) TG_PL_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_TG_PRG
#define h_TG_PRG (){}
    #define     RA_TG_PRG_CTRL                                 0x0000
    #define   LSb32TG_PRG_CTRL_mode                               0
    #define       bTG_PRG_CTRL_mode                            2
    #define   MSK32TG_PRG_CTRL_mode                               0x00000003
    #define   LSb32TG_PRG_CTRL_lwin                               2
    #define       bTG_PRG_CTRL_lwin                            8
    #define   MSK32TG_PRG_CTRL_lwin                               0x000003FC
    #define   LSb32TG_PRG_CTRL_frst                               10
    #define       bTG_PRG_CTRL_frst                            12
    #define   MSK32TG_PRG_CTRL_frst                               0x003FFC00
    #define   LSb32TG_PRG_CTRL_freeze                             22
    #define       bTG_PRG_CTRL_freeze                          10
    #define   MSK32TG_PRG_CTRL_freeze                             0xFFC00000
    #define     RA_TG_PRG_CTRL1                                0x0004
    #define   LSb32TG_PRG_CTRL1_sync_ctrl                         0
    #define       bTG_PRG_CTRL1_sync_ctrl                      2
    #define   MSK32TG_PRG_CTRL1_sync_ctrl                         0x00000003
    #define   LSb32TG_PRG_CTRL1_res_change_en                     2
    #define       bTG_PRG_CTRL1_res_change_en                  9
    #define   MSK32TG_PRG_CTRL1_res_change_en                     0x000007FC
    #define     RA_TG_PRG_Total                                0x0008
    #define   LSb32TG_PRG_Total_vertical                          0
    #define       bTG_PRG_Total_vertical                       12
    #define   MSK32TG_PRG_Total_vertical                          0x00000FFF
    #define   LSb32TG_PRG_Total_horizontal                        12
    #define       bTG_PRG_Total_horizontal                     13
    #define   MSK32TG_PRG_Total_horizontal                        0x01FFF000
    #define     RA_TG_PRG_Initial                              0x000C
    #define   LSb32TG_PRG_Initial_xi                              0
    #define       bTG_PRG_Initial_xi                           13
    #define   MSK32TG_PRG_Initial_xi                              0x00001FFF
    #define   LSb32TG_PRG_Initial_yi                              13
    #define       bTG_PRG_Initial_yi                           12
    #define   MSK32TG_PRG_Initial_yi                              0x01FFE000
    #define     RA_TG_PRG_HSYNC                                0x0010
    #define   LSb32TG_PRG_HSYNC_h_start                           0
    #define       bTG_PRG_HSYNC_h_start                        13
    #define   MSK32TG_PRG_HSYNC_h_start                           0x00001FFF
    #define   LSb32TG_PRG_HSYNC_h_end                             13
    #define       bTG_PRG_HSYNC_h_end                          13
    #define   MSK32TG_PRG_HSYNC_h_end                             0x03FFE000
    #define     RA_TG_PRG_VSYNC                                0x0014
    #define   LSb32TG_PRG_VSYNC_v_start                           0
    #define       bTG_PRG_VSYNC_v_start                        12
    #define   MSK32TG_PRG_VSYNC_v_start                           0x00000FFF
    #define   LSb32TG_PRG_VSYNC_v_end                             12
    #define       bTG_PRG_VSYNC_v_end                          12
    #define   MSK32TG_PRG_VSYNC_v_end                             0x00FFF000
    #define     RA_TG_PRG_VS                                   0x0018
    #define   LSb32TG_PRG_VS_h_start                              0
    #define       bTG_PRG_VS_h_start                           13
    #define   MSK32TG_PRG_VS_h_start                              0x00001FFF
    #define   LSb32TG_PRG_VS_h_end                                13
    #define       bTG_PRG_VS_h_end                             13
    #define   MSK32TG_PRG_VS_h_end                                0x03FFE000
    #define     RA_TG_PRG_FT                                   0x001C
    #define   LSb32TG_PRG_FT_frame                                0
    #define       bTG_PRG_FT_frame                             8
    #define   MSK32TG_PRG_FT_frame                                0x000000FF
    #define     RA_TG_PRG_VX                                   0x0020
    #define   LSb32TG_PRG_VX_vx                                   0
    #define       bTG_PRG_VX_vx                                13
    #define   MSK32TG_PRG_VX_vx                                   0x00001FFF
    typedef struct SIE_TG_PRG {
    #define     w32TG_PRG_CTRL                                 {\
            UNSG32 uCTRL_mode                                  :  2;\
            UNSG32 uCTRL_lwin                                  :  8;\
            UNSG32 uCTRL_frst                                  : 12;\
            UNSG32 uCTRL_freeze                                : 10;\
          }
    union { UNSG32 u32TG_PRG_CTRL;
            struct w32TG_PRG_CTRL;
          };
    #define     w32TG_PRG_CTRL1                                {\
            UNSG32 uCTRL1_sync_ctrl                            :  2;\
            UNSG32 uCTRL1_res_change_en                        :  9;\
            UNSG32 RSVDx4_b11                                  : 21;\
          }
    union { UNSG32 u32TG_PRG_CTRL1;
            struct w32TG_PRG_CTRL1;
          };
    #define     w32TG_PRG_Total                                {\
            UNSG32 uTotal_vertical                             : 12;\
            UNSG32 uTotal_horizontal                           : 13;\
            UNSG32 RSVDx8_b25                                  :  7;\
          }
    union { UNSG32 u32TG_PRG_Total;
            struct w32TG_PRG_Total;
          };
    #define     w32TG_PRG_Initial                              {\
            UNSG32 uInitial_xi                                 : 13;\
            UNSG32 uInitial_yi                                 : 12;\
            UNSG32 RSVDxC_b25                                  :  7;\
          }
    union { UNSG32 u32TG_PRG_Initial;
            struct w32TG_PRG_Initial;
          };
    #define     w32TG_PRG_HSYNC                                {\
            UNSG32 uHSYNC_h_start                              : 13;\
            UNSG32 uHSYNC_h_end                                : 13;\
            UNSG32 RSVDx10_b26                                 :  6;\
          }
    union { UNSG32 u32TG_PRG_HSYNC;
            struct w32TG_PRG_HSYNC;
          };
    #define     w32TG_PRG_VSYNC                                {\
            UNSG32 uVSYNC_v_start                              : 12;\
            UNSG32 uVSYNC_v_end                                : 12;\
            UNSG32 RSVDx14_b24                                 :  8;\
          }
    union { UNSG32 u32TG_PRG_VSYNC;
            struct w32TG_PRG_VSYNC;
          };
    #define     w32TG_PRG_VS                                   {\
            UNSG32 uVS_h_start                                 : 13;\
            UNSG32 uVS_h_end                                   : 13;\
            UNSG32 RSVDx18_b26                                 :  6;\
          }
    union { UNSG32 u32TG_PRG_VS;
            struct w32TG_PRG_VS;
          };
    #define     w32TG_PRG_FT                                   {\
            UNSG32 uFT_frame                                   :  8;\
            UNSG32 RSVDx1C_b8                                  : 24;\
          }
    union { UNSG32 u32TG_PRG_FT;
            struct w32TG_PRG_FT;
          };
    #define     w32TG_PRG_VX                                   {\
            UNSG32 uVX_vx                                      : 13;\
            UNSG32 RSVDx20_b13                                 : 19;\
          }
    union { UNSG32 u32TG_PRG_VX;
            struct w32TG_PRG_VX;
          };
    } SIE_TG_PRG;
    typedef union  T32TG_PRG_CTRL
          { UNSG32 u32;
            struct w32TG_PRG_CTRL;
                 } T32TG_PRG_CTRL;
    typedef union  T32TG_PRG_CTRL1
          { UNSG32 u32;
            struct w32TG_PRG_CTRL1;
                 } T32TG_PRG_CTRL1;
    typedef union  T32TG_PRG_Total
          { UNSG32 u32;
            struct w32TG_PRG_Total;
                 } T32TG_PRG_Total;
    typedef union  T32TG_PRG_Initial
          { UNSG32 u32;
            struct w32TG_PRG_Initial;
                 } T32TG_PRG_Initial;
    typedef union  T32TG_PRG_HSYNC
          { UNSG32 u32;
            struct w32TG_PRG_HSYNC;
                 } T32TG_PRG_HSYNC;
    typedef union  T32TG_PRG_VSYNC
          { UNSG32 u32;
            struct w32TG_PRG_VSYNC;
                 } T32TG_PRG_VSYNC;
    typedef union  T32TG_PRG_VS
          { UNSG32 u32;
            struct w32TG_PRG_VS;
                 } T32TG_PRG_VS;
    typedef union  T32TG_PRG_FT
          { UNSG32 u32;
            struct w32TG_PRG_FT;
                 } T32TG_PRG_FT;
    typedef union  T32TG_PRG_VX
          { UNSG32 u32;
            struct w32TG_PRG_VX;
                 } T32TG_PRG_VX;
    typedef union  TTG_PRG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_CTRL;
                   };
                 } TTG_PRG_CTRL;
    typedef union  TTG_PRG_CTRL1
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_CTRL1;
                   };
                 } TTG_PRG_CTRL1;
    typedef union  TTG_PRG_Total
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_Total;
                   };
                 } TTG_PRG_Total;
    typedef union  TTG_PRG_Initial
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_Initial;
                   };
                 } TTG_PRG_Initial;
    typedef union  TTG_PRG_HSYNC
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_HSYNC;
                   };
                 } TTG_PRG_HSYNC;
    typedef union  TTG_PRG_VSYNC
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_VSYNC;
                   };
                 } TTG_PRG_VSYNC;
    typedef union  TTG_PRG_VS
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_VS;
                   };
                 } TTG_PRG_VS;
    typedef union  TTG_PRG_FT
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_FT;
                   };
                 } TTG_PRG_FT;
    typedef union  TTG_PRG_VX
          { UNSG32 u32[1];
            struct {
            struct w32TG_PRG_VX;
                   };
                 } TTG_PRG_VX;
     SIGN32 TG_PRG_drvrd(SIE_TG_PRG *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TG_PRG_drvwr(SIE_TG_PRG *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TG_PRG_reset(SIE_TG_PRG *p);
     SIGN32 TG_PRG_cmp  (SIE_TG_PRG *p, SIE_TG_PRG *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TG_PRG_check(p,pie,pfx,hLOG) TG_PRG_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TG_PRG_print(p,    pfx,hLOG) TG_PRG_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_TG
#define h_TG (){}
    #define     RA_TG_INIT                                     0x0000
    #define   LSb32TG_INIT_Y                                      0
    #define       bTG_INIT_Y                                   16
    #define   MSK32TG_INIT_Y                                      0x0000FFFF
    #define   LSb32TG_INIT_X                                      16
    #define       bTG_INIT_X                                   16
    #define   MSK32TG_INIT_X                                      0xFFFF0000
    #define     RA_TG_SIZE                                     0x0004
    #define   LSb32TG_SIZE_Y                                      0
    #define       bTG_SIZE_Y                                   16
    #define   MSK32TG_SIZE_Y                                      0x0000FFFF
    #define   LSb32TG_SIZE_X                                      16
    #define       bTG_SIZE_X                                   16
    #define   MSK32TG_SIZE_X                                      0xFFFF0000
    #define     RA_TG_HS                                       0x0008
    #define   LSb32TG_HS_FE                                       0
    #define       bTG_HS_FE                                    16
    #define   MSK32TG_HS_FE                                       0x0000FFFF
    #define   LSb32TG_HS_BE                                       16
    #define       bTG_HS_BE                                    16
    #define   MSK32TG_HS_BE                                       0xFFFF0000
    #define     RA_TG_HB                                       0x000C
    #define   LSb32TG_HB_FE                                       0
    #define       bTG_HB_FE                                    16
    #define   MSK32TG_HB_FE                                       0x0000FFFF
    #define   LSb32TG_HB_BE                                       16
    #define       bTG_HB_BE                                    16
    #define   MSK32TG_HB_BE                                       0xFFFF0000
    #define     RA_TG_HB_CR                                    0x0010
    #define   LSb32TG_HB_CR_FE                                    0
    #define       bTG_HB_CR_FE                                 16
    #define   MSK32TG_HB_CR_FE                                    0x0000FFFF
    #define   LSb32TG_HB_CR_BE                                    16
    #define       bTG_HB_CR_BE                                 16
    #define   MSK32TG_HB_CR_BE                                    0xFFFF0000
    #define     RA_TG_HB_CR2                                   0x0014
    #define   LSb32TG_HB_CR2_FE                                   0
    #define       bTG_HB_CR2_FE                                16
    #define   MSK32TG_HB_CR2_FE                                   0x0000FFFF
    #define   LSb32TG_HB_CR2_BE                                   16
    #define       bTG_HB_CR2_BE                                16
    #define   MSK32TG_HB_CR2_BE                                   0xFFFF0000
    #define     RA_TG_VS0                                      0x0018
    #define   LSb32TG_VS0_FE                                      0
    #define       bTG_VS0_FE                                   16
    #define   MSK32TG_VS0_FE                                      0x0000FFFF
    #define   LSb32TG_VS0_BE                                      16
    #define       bTG_VS0_BE                                   16
    #define   MSK32TG_VS0_BE                                      0xFFFF0000
    #define     RA_TG_VS1                                      0x001C
    #define   LSb32TG_VS1_FE                                      0
    #define       bTG_VS1_FE                                   16
    #define   MSK32TG_VS1_FE                                      0x0000FFFF
    #define   LSb32TG_VS1_BE                                      16
    #define       bTG_VS1_BE                                   16
    #define   MSK32TG_VS1_BE                                      0xFFFF0000
    #define     RA_TG_VB0                                      0x0020
    #define   LSb32TG_VB0_FE                                      0
    #define       bTG_VB0_FE                                   16
    #define   MSK32TG_VB0_FE                                      0x0000FFFF
    #define   LSb32TG_VB0_BE                                      16
    #define       bTG_VB0_BE                                   16
    #define   MSK32TG_VB0_BE                                      0xFFFF0000
    #define     RA_TG_VB0_CR                                   0x0024
    #define   LSb32TG_VB0_CR_FE                                   0
    #define       bTG_VB0_CR_FE                                16
    #define   MSK32TG_VB0_CR_FE                                   0x0000FFFF
    #define   LSb32TG_VB0_CR_BE                                   16
    #define       bTG_VB0_CR_BE                                16
    #define   MSK32TG_VB0_CR_BE                                   0xFFFF0000
    #define     RA_TG_VB0_CR2                                  0x0028
    #define   LSb32TG_VB0_CR2_FE                                  0
    #define       bTG_VB0_CR2_FE                               16
    #define   MSK32TG_VB0_CR2_FE                                  0x0000FFFF
    #define   LSb32TG_VB0_CR2_BE                                  16
    #define       bTG_VB0_CR2_BE                               16
    #define   MSK32TG_VB0_CR2_BE                                  0xFFFF0000
    #define     RA_TG_VB1                                      0x002C
    #define   LSb32TG_VB1_FE                                      0
    #define       bTG_VB1_FE                                   16
    #define   MSK32TG_VB1_FE                                      0x0000FFFF
    #define   LSb32TG_VB1_BE                                      16
    #define       bTG_VB1_BE                                   16
    #define   MSK32TG_VB1_BE                                      0xFFFF0000
    #define     RA_TG_SCAN                                     0x0030
    #define   LSb32TG_SCAN_MODE                                   0
    #define       bTG_SCAN_MODE                                1
    #define   MSK32TG_SCAN_MODE                                   0x00000001
    #define        TG_SCAN_MODE_PROG                                        0x0
    #define        TG_SCAN_MODE_INTER                                       0x1
    #define     RA_TG_INTPOS                                   0x0034
    #define   LSb32TG_INTPOS_FRAME                                0
    #define       bTG_INTPOS_FRAME                             16
    #define   MSK32TG_INTPOS_FRAME                                0x0000FFFF
    #define   LSb32TG_INTPOS_FIELD                                16
    #define       bTG_INTPOS_FIELD                             16
    #define   MSK32TG_INTPOS_FIELD                                0xFFFF0000
    #define     RA_TG_MODE                                     0x0038
    #define   LSb32TG_MODE_EN                                     0
    #define       bTG_MODE_EN                                  1
    #define   MSK32TG_MODE_EN                                     0x00000001
    #define        TG_MODE_EN_MASTER                                        0x0
    #define        TG_MODE_EN_SLAVE                                         0x1
    #define     RA_TG_HVREF                                    0x003C
    #define   LSb32TG_HVREF_SEL                                   0
    #define       bTG_HVREF_SEL                                1
    #define   MSK32TG_HVREF_SEL                                   0x00000001
    #define        TG_HVREF_SEL_SYNC                                        0x0
    #define        TG_HVREF_SEL_BLANK                                       0x1
    #define   LSb32TG_HVREF_POL                                   1
    #define       bTG_HVREF_POL                                1
    #define   MSK32TG_HVREF_POL                                   0x00000002
    #define        TG_HVREF_POL_NEG_PULSE                                   0x0
    #define        TG_HVREF_POL_POS_PULSE                                   0x1
    typedef struct SIE_TG {
    #define     w32TG_INIT                                     {\
            UNSG32 uINIT_Y                                     : 16;\
            UNSG32 uINIT_X                                     : 16;\
          }
    union { UNSG32 u32TG_INIT;
            struct w32TG_INIT;
          };
    #define     w32TG_SIZE                                     {\
            UNSG32 uSIZE_Y                                     : 16;\
            UNSG32 uSIZE_X                                     : 16;\
          }
    union { UNSG32 u32TG_SIZE;
            struct w32TG_SIZE;
          };
    #define     w32TG_HS                                       {\
            UNSG32 uHS_FE                                      : 16;\
            UNSG32 uHS_BE                                      : 16;\
          }
    union { UNSG32 u32TG_HS;
            struct w32TG_HS;
          };
    #define     w32TG_HB                                       {\
            UNSG32 uHB_FE                                      : 16;\
            UNSG32 uHB_BE                                      : 16;\
          }
    union { UNSG32 u32TG_HB;
            struct w32TG_HB;
          };
    #define     w32TG_HB_CR                                    {\
            UNSG32 uHB_CR_FE                                   : 16;\
            UNSG32 uHB_CR_BE                                   : 16;\
          }
    union { UNSG32 u32TG_HB_CR;
            struct w32TG_HB_CR;
          };
    #define     w32TG_HB_CR2                                   {\
            UNSG32 uHB_CR2_FE                                  : 16;\
            UNSG32 uHB_CR2_BE                                  : 16;\
          }
    union { UNSG32 u32TG_HB_CR2;
            struct w32TG_HB_CR2;
          };
    #define     w32TG_VS0                                      {\
            UNSG32 uVS0_FE                                     : 16;\
            UNSG32 uVS0_BE                                     : 16;\
          }
    union { UNSG32 u32TG_VS0;
            struct w32TG_VS0;
          };
    #define     w32TG_VS1                                      {\
            UNSG32 uVS1_FE                                     : 16;\
            UNSG32 uVS1_BE                                     : 16;\
          }
    union { UNSG32 u32TG_VS1;
            struct w32TG_VS1;
          };
    #define     w32TG_VB0                                      {\
            UNSG32 uVB0_FE                                     : 16;\
            UNSG32 uVB0_BE                                     : 16;\
          }
    union { UNSG32 u32TG_VB0;
            struct w32TG_VB0;
          };
    #define     w32TG_VB0_CR                                   {\
            UNSG32 uVB0_CR_FE                                  : 16;\
            UNSG32 uVB0_CR_BE                                  : 16;\
          }
    union { UNSG32 u32TG_VB0_CR;
            struct w32TG_VB0_CR;
          };
    #define     w32TG_VB0_CR2                                  {\
            UNSG32 uVB0_CR2_FE                                 : 16;\
            UNSG32 uVB0_CR2_BE                                 : 16;\
          }
    union { UNSG32 u32TG_VB0_CR2;
            struct w32TG_VB0_CR2;
          };
    #define     w32TG_VB1                                      {\
            UNSG32 uVB1_FE                                     : 16;\
            UNSG32 uVB1_BE                                     : 16;\
          }
    union { UNSG32 u32TG_VB1;
            struct w32TG_VB1;
          };
    #define     w32TG_SCAN                                     {\
            UNSG32 uSCAN_MODE                                  :  1;\
            UNSG32 RSVDx30_b1                                  : 31;\
          }
    union { UNSG32 u32TG_SCAN;
            struct w32TG_SCAN;
          };
    #define     w32TG_INTPOS                                   {\
            UNSG32 uINTPOS_FRAME                               : 16;\
            UNSG32 uINTPOS_FIELD                               : 16;\
          }
    union { UNSG32 u32TG_INTPOS;
            struct w32TG_INTPOS;
          };
    #define     w32TG_MODE                                     {\
            UNSG32 uMODE_EN                                    :  1;\
            UNSG32 RSVDx38_b1                                  : 31;\
          }
    union { UNSG32 u32TG_MODE;
            struct w32TG_MODE;
          };
    #define     w32TG_HVREF                                    {\
            UNSG32 uHVREF_SEL                                  :  1;\
            UNSG32 uHVREF_POL                                  :  1;\
            UNSG32 RSVDx3C_b2                                  : 30;\
          }
    union { UNSG32 u32TG_HVREF;
            struct w32TG_HVREF;
          };
    } SIE_TG;
    typedef union  T32TG_INIT
          { UNSG32 u32;
            struct w32TG_INIT;
                 } T32TG_INIT;
    typedef union  T32TG_SIZE
          { UNSG32 u32;
            struct w32TG_SIZE;
                 } T32TG_SIZE;
    typedef union  T32TG_HS
          { UNSG32 u32;
            struct w32TG_HS;
                 } T32TG_HS;
    typedef union  T32TG_HB
          { UNSG32 u32;
            struct w32TG_HB;
                 } T32TG_HB;
    typedef union  T32TG_HB_CR
          { UNSG32 u32;
            struct w32TG_HB_CR;
                 } T32TG_HB_CR;
    typedef union  T32TG_HB_CR2
          { UNSG32 u32;
            struct w32TG_HB_CR2;
                 } T32TG_HB_CR2;
    typedef union  T32TG_VS0
          { UNSG32 u32;
            struct w32TG_VS0;
                 } T32TG_VS0;
    typedef union  T32TG_VS1
          { UNSG32 u32;
            struct w32TG_VS1;
                 } T32TG_VS1;
    typedef union  T32TG_VB0
          { UNSG32 u32;
            struct w32TG_VB0;
                 } T32TG_VB0;
    typedef union  T32TG_VB0_CR
          { UNSG32 u32;
            struct w32TG_VB0_CR;
                 } T32TG_VB0_CR;
    typedef union  T32TG_VB0_CR2
          { UNSG32 u32;
            struct w32TG_VB0_CR2;
                 } T32TG_VB0_CR2;
    typedef union  T32TG_VB1
          { UNSG32 u32;
            struct w32TG_VB1;
                 } T32TG_VB1;
    typedef union  T32TG_SCAN
          { UNSG32 u32;
            struct w32TG_SCAN;
                 } T32TG_SCAN;
    typedef union  T32TG_INTPOS
          { UNSG32 u32;
            struct w32TG_INTPOS;
                 } T32TG_INTPOS;
    typedef union  T32TG_MODE
          { UNSG32 u32;
            struct w32TG_MODE;
                 } T32TG_MODE;
    typedef union  T32TG_HVREF
          { UNSG32 u32;
            struct w32TG_HVREF;
                 } T32TG_HVREF;
    typedef union  TTG_INIT
          { UNSG32 u32[1];
            struct {
            struct w32TG_INIT;
                   };
                 } TTG_INIT;
    typedef union  TTG_SIZE
          { UNSG32 u32[1];
            struct {
            struct w32TG_SIZE;
                   };
                 } TTG_SIZE;
    typedef union  TTG_HS
          { UNSG32 u32[1];
            struct {
            struct w32TG_HS;
                   };
                 } TTG_HS;
    typedef union  TTG_HB
          { UNSG32 u32[1];
            struct {
            struct w32TG_HB;
                   };
                 } TTG_HB;
    typedef union  TTG_HB_CR
          { UNSG32 u32[1];
            struct {
            struct w32TG_HB_CR;
                   };
                 } TTG_HB_CR;
    typedef union  TTG_HB_CR2
          { UNSG32 u32[1];
            struct {
            struct w32TG_HB_CR2;
                   };
                 } TTG_HB_CR2;
    typedef union  TTG_VS0
          { UNSG32 u32[1];
            struct {
            struct w32TG_VS0;
                   };
                 } TTG_VS0;
    typedef union  TTG_VS1
          { UNSG32 u32[1];
            struct {
            struct w32TG_VS1;
                   };
                 } TTG_VS1;
    typedef union  TTG_VB0
          { UNSG32 u32[1];
            struct {
            struct w32TG_VB0;
                   };
                 } TTG_VB0;
    typedef union  TTG_VB0_CR
          { UNSG32 u32[1];
            struct {
            struct w32TG_VB0_CR;
                   };
                 } TTG_VB0_CR;
    typedef union  TTG_VB0_CR2
          { UNSG32 u32[1];
            struct {
            struct w32TG_VB0_CR2;
                   };
                 } TTG_VB0_CR2;
    typedef union  TTG_VB1
          { UNSG32 u32[1];
            struct {
            struct w32TG_VB1;
                   };
                 } TTG_VB1;
    typedef union  TTG_SCAN
          { UNSG32 u32[1];
            struct {
            struct w32TG_SCAN;
                   };
                 } TTG_SCAN;
    typedef union  TTG_INTPOS
          { UNSG32 u32[1];
            struct {
            struct w32TG_INTPOS;
                   };
                 } TTG_INTPOS;
    typedef union  TTG_MODE
          { UNSG32 u32[1];
            struct {
            struct w32TG_MODE;
                   };
                 } TTG_MODE;
    typedef union  TTG_HVREF
          { UNSG32 u32[1];
            struct {
            struct w32TG_HVREF;
                   };
                 } TTG_HVREF;
     SIGN32 TG_drvrd(SIE_TG *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TG_drvwr(SIE_TG *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TG_reset(SIE_TG *p);
     SIGN32 TG_cmp  (SIE_TG *p, SIE_TG *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TG_check(p,pie,pfx,hLOG) TG_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TG_print(p,    pfx,hLOG) TG_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_TG_MAIN
#define h_TG_MAIN (){}
    #define     RA_TG_MAIN_INIT                                0x0000
    #define   LSb32TG_MAIN_INIT_Y                                 0
    #define       bTG_MAIN_INIT_Y                              16
    #define   MSK32TG_MAIN_INIT_Y                                 0x0000FFFF
    #define   LSb32TG_MAIN_INIT_X                                 16
    #define       bTG_MAIN_INIT_X                              16
    #define   MSK32TG_MAIN_INIT_X                                 0xFFFF0000
    #define     RA_TG_MAIN_SIZE                                0x0004
    #define   LSb32TG_MAIN_SIZE_Y                                 0
    #define       bTG_MAIN_SIZE_Y                              16
    #define   MSK32TG_MAIN_SIZE_Y                                 0x0000FFFF
    #define   LSb32TG_MAIN_SIZE_X                                 16
    #define       bTG_MAIN_SIZE_X                              16
    #define   MSK32TG_MAIN_SIZE_X                                 0xFFFF0000
    #define     RA_TG_MAIN_HS                                  0x0008
    #define   LSb32TG_MAIN_HS_FE                                  0
    #define       bTG_MAIN_HS_FE                               16
    #define   MSK32TG_MAIN_HS_FE                                  0x0000FFFF
    #define   LSb32TG_MAIN_HS_BE                                  16
    #define       bTG_MAIN_HS_BE                               16
    #define   MSK32TG_MAIN_HS_BE                                  0xFFFF0000
    #define     RA_TG_MAIN_HB                                  0x000C
    #define   LSb32TG_MAIN_HB_FE                                  0
    #define       bTG_MAIN_HB_FE                               16
    #define   MSK32TG_MAIN_HB_FE                                  0x0000FFFF
    #define   LSb32TG_MAIN_HB_BE                                  16
    #define       bTG_MAIN_HB_BE                               16
    #define   MSK32TG_MAIN_HB_BE                                  0xFFFF0000
    #define     RA_TG_MAIN_HB_Y                                0x0010
    #define   LSb32TG_MAIN_HB_Y_FE                                0
    #define       bTG_MAIN_HB_Y_FE                             16
    #define   MSK32TG_MAIN_HB_Y_FE                                0x0000FFFF
    #define   LSb32TG_MAIN_HB_Y_BE                                16
    #define       bTG_MAIN_HB_Y_BE                             16
    #define   MSK32TG_MAIN_HB_Y_BE                                0xFFFF0000
    #define     RA_TG_MAIN_HB_C                                0x0014
    #define   LSb32TG_MAIN_HB_C_FE                                0
    #define       bTG_MAIN_HB_C_FE                             16
    #define   MSK32TG_MAIN_HB_C_FE                                0x0000FFFF
    #define   LSb32TG_MAIN_HB_C_BE                                16
    #define       bTG_MAIN_HB_C_BE                             16
    #define   MSK32TG_MAIN_HB_C_BE                                0xFFFF0000
    #define     RA_TG_MAIN_HB_OUT                              0x0018
    #define   LSb32TG_MAIN_HB_OUT_FE                              0
    #define       bTG_MAIN_HB_OUT_FE                           16
    #define   MSK32TG_MAIN_HB_OUT_FE                              0x0000FFFF
    #define   LSb32TG_MAIN_HB_OUT_BE                              16
    #define       bTG_MAIN_HB_OUT_BE                           16
    #define   MSK32TG_MAIN_HB_OUT_BE                              0xFFFF0000
    #define     RA_TG_MAIN_VS0                                 0x001C
    #define   LSb32TG_MAIN_VS0_FE                                 0
    #define       bTG_MAIN_VS0_FE                              16
    #define   MSK32TG_MAIN_VS0_FE                                 0x0000FFFF
    #define   LSb32TG_MAIN_VS0_BE                                 16
    #define       bTG_MAIN_VS0_BE                              16
    #define   MSK32TG_MAIN_VS0_BE                                 0xFFFF0000
    #define     RA_TG_MAIN_VS1                                 0x0020
    #define   LSb32TG_MAIN_VS1_FE                                 0
    #define       bTG_MAIN_VS1_FE                              16
    #define   MSK32TG_MAIN_VS1_FE                                 0x0000FFFF
    #define   LSb32TG_MAIN_VS1_BE                                 16
    #define       bTG_MAIN_VS1_BE                              16
    #define   MSK32TG_MAIN_VS1_BE                                 0xFFFF0000
    #define     RA_TG_MAIN_VB0                                 0x0024
    #define   LSb32TG_MAIN_VB0_FE                                 0
    #define       bTG_MAIN_VB0_FE                              16
    #define   MSK32TG_MAIN_VB0_FE                                 0x0000FFFF
    #define   LSb32TG_MAIN_VB0_BE                                 16
    #define       bTG_MAIN_VB0_BE                              16
    #define   MSK32TG_MAIN_VB0_BE                                 0xFFFF0000
    #define     RA_TG_MAIN_VB0_Y                               0x0028
    #define   LSb32TG_MAIN_VB0_Y_FE                               0
    #define       bTG_MAIN_VB0_Y_FE                            16
    #define   MSK32TG_MAIN_VB0_Y_FE                               0x0000FFFF
    #define   LSb32TG_MAIN_VB0_Y_BE                               16
    #define       bTG_MAIN_VB0_Y_BE                            16
    #define   MSK32TG_MAIN_VB0_Y_BE                               0xFFFF0000
    #define     RA_TG_MAIN_VB0_C                               0x002C
    #define   LSb32TG_MAIN_VB0_C_FE                               0
    #define       bTG_MAIN_VB0_C_FE                            16
    #define   MSK32TG_MAIN_VB0_C_FE                               0x0000FFFF
    #define   LSb32TG_MAIN_VB0_C_BE                               16
    #define       bTG_MAIN_VB0_C_BE                            16
    #define   MSK32TG_MAIN_VB0_C_BE                               0xFFFF0000
    #define     RA_TG_MAIN_VB0_OUT                             0x0030
    #define   LSb32TG_MAIN_VB0_OUT_FE                             0
    #define       bTG_MAIN_VB0_OUT_FE                          16
    #define   MSK32TG_MAIN_VB0_OUT_FE                             0x0000FFFF
    #define   LSb32TG_MAIN_VB0_OUT_BE                             16
    #define       bTG_MAIN_VB0_OUT_BE                          16
    #define   MSK32TG_MAIN_VB0_OUT_BE                             0xFFFF0000
    #define     RA_TG_MAIN_VB1                                 0x0034
    #define   LSb32TG_MAIN_VB1_FE                                 0
    #define       bTG_MAIN_VB1_FE                              16
    #define   MSK32TG_MAIN_VB1_FE                                 0x0000FFFF
    #define   LSb32TG_MAIN_VB1_BE                                 16
    #define       bTG_MAIN_VB1_BE                              16
    #define   MSK32TG_MAIN_VB1_BE                                 0xFFFF0000
    #define     RA_TG_MAIN_SCAN                                0x0038
    #define   LSb32TG_MAIN_SCAN_MODE                              0
    #define       bTG_MAIN_SCAN_MODE                           1
    #define   MSK32TG_MAIN_SCAN_MODE                              0x00000001
    #define        TG_MAIN_SCAN_MODE_PROG                                   0x0
    #define        TG_MAIN_SCAN_MODE_INTER                                  0x1
    #define     RA_TG_MAIN_INTPOS                              0x003C
    #define   LSb32TG_MAIN_INTPOS_FRAME                           0
    #define       bTG_MAIN_INTPOS_FRAME                        16
    #define   MSK32TG_MAIN_INTPOS_FRAME                           0x0000FFFF
    #define   LSb32TG_MAIN_INTPOS_FIELD                           16
    #define       bTG_MAIN_INTPOS_FIELD                        16
    #define   MSK32TG_MAIN_INTPOS_FIELD                           0xFFFF0000
    #define     RA_TG_MAIN_MODE                                0x0040
    #define   LSb32TG_MAIN_MODE_EN                                0
    #define       bTG_MAIN_MODE_EN                             1
    #define   MSK32TG_MAIN_MODE_EN                                0x00000001
    #define        TG_MAIN_MODE_EN_MASTER                                   0x0
    #define        TG_MAIN_MODE_EN_SLAVE                                    0x1
    #define     RA_TG_MAIN_HVREF                               0x0044
    #define   LSb32TG_MAIN_HVREF_SEL                              0
    #define       bTG_MAIN_HVREF_SEL                           1
    #define   MSK32TG_MAIN_HVREF_SEL                              0x00000001
    #define        TG_MAIN_HVREF_SEL_SYNC                                   0x0
    #define        TG_MAIN_HVREF_SEL_BLANK                                  0x1
    #define   LSb32TG_MAIN_HVREF_POL                              1
    #define       bTG_MAIN_HVREF_POL                           1
    #define   MSK32TG_MAIN_HVREF_POL                              0x00000002
    #define        TG_MAIN_HVREF_POL_NEG_PULSE                              0x0
    #define        TG_MAIN_HVREF_POL_POS_PULSE                              0x1
    typedef struct SIE_TG_MAIN {
    #define     w32TG_MAIN_INIT                                {\
            UNSG32 uINIT_Y                                     : 16;\
            UNSG32 uINIT_X                                     : 16;\
          }
    union { UNSG32 u32TG_MAIN_INIT;
            struct w32TG_MAIN_INIT;
          };
    #define     w32TG_MAIN_SIZE                                {\
            UNSG32 uSIZE_Y                                     : 16;\
            UNSG32 uSIZE_X                                     : 16;\
          }
    union { UNSG32 u32TG_MAIN_SIZE;
            struct w32TG_MAIN_SIZE;
          };
    #define     w32TG_MAIN_HS                                  {\
            UNSG32 uHS_FE                                      : 16;\
            UNSG32 uHS_BE                                      : 16;\
          }
    union { UNSG32 u32TG_MAIN_HS;
            struct w32TG_MAIN_HS;
          };
    #define     w32TG_MAIN_HB                                  {\
            UNSG32 uHB_FE                                      : 16;\
            UNSG32 uHB_BE                                      : 16;\
          }
    union { UNSG32 u32TG_MAIN_HB;
            struct w32TG_MAIN_HB;
          };
    #define     w32TG_MAIN_HB_Y                                {\
            UNSG32 uHB_Y_FE                                    : 16;\
            UNSG32 uHB_Y_BE                                    : 16;\
          }
    union { UNSG32 u32TG_MAIN_HB_Y;
            struct w32TG_MAIN_HB_Y;
          };
    #define     w32TG_MAIN_HB_C                                {\
            UNSG32 uHB_C_FE                                    : 16;\
            UNSG32 uHB_C_BE                                    : 16;\
          }
    union { UNSG32 u32TG_MAIN_HB_C;
            struct w32TG_MAIN_HB_C;
          };
    #define     w32TG_MAIN_HB_OUT                              {\
            UNSG32 uHB_OUT_FE                                  : 16;\
            UNSG32 uHB_OUT_BE                                  : 16;\
          }
    union { UNSG32 u32TG_MAIN_HB_OUT;
            struct w32TG_MAIN_HB_OUT;
          };
    #define     w32TG_MAIN_VS0                                 {\
            UNSG32 uVS0_FE                                     : 16;\
            UNSG32 uVS0_BE                                     : 16;\
          }
    union { UNSG32 u32TG_MAIN_VS0;
            struct w32TG_MAIN_VS0;
          };
    #define     w32TG_MAIN_VS1                                 {\
            UNSG32 uVS1_FE                                     : 16;\
            UNSG32 uVS1_BE                                     : 16;\
          }
    union { UNSG32 u32TG_MAIN_VS1;
            struct w32TG_MAIN_VS1;
          };
    #define     w32TG_MAIN_VB0                                 {\
            UNSG32 uVB0_FE                                     : 16;\
            UNSG32 uVB0_BE                                     : 16;\
          }
    union { UNSG32 u32TG_MAIN_VB0;
            struct w32TG_MAIN_VB0;
          };
    #define     w32TG_MAIN_VB0_Y                               {\
            UNSG32 uVB0_Y_FE                                   : 16;\
            UNSG32 uVB0_Y_BE                                   : 16;\
          }
    union { UNSG32 u32TG_MAIN_VB0_Y;
            struct w32TG_MAIN_VB0_Y;
          };
    #define     w32TG_MAIN_VB0_C                               {\
            UNSG32 uVB0_C_FE                                   : 16;\
            UNSG32 uVB0_C_BE                                   : 16;\
          }
    union { UNSG32 u32TG_MAIN_VB0_C;
            struct w32TG_MAIN_VB0_C;
          };
    #define     w32TG_MAIN_VB0_OUT                             {\
            UNSG32 uVB0_OUT_FE                                 : 16;\
            UNSG32 uVB0_OUT_BE                                 : 16;\
          }
    union { UNSG32 u32TG_MAIN_VB0_OUT;
            struct w32TG_MAIN_VB0_OUT;
          };
    #define     w32TG_MAIN_VB1                                 {\
            UNSG32 uVB1_FE                                     : 16;\
            UNSG32 uVB1_BE                                     : 16;\
          }
    union { UNSG32 u32TG_MAIN_VB1;
            struct w32TG_MAIN_VB1;
          };
    #define     w32TG_MAIN_SCAN                                {\
            UNSG32 uSCAN_MODE                                  :  1;\
            UNSG32 RSVDx38_b1                                  : 31;\
          }
    union { UNSG32 u32TG_MAIN_SCAN;
            struct w32TG_MAIN_SCAN;
          };
    #define     w32TG_MAIN_INTPOS                              {\
            UNSG32 uINTPOS_FRAME                               : 16;\
            UNSG32 uINTPOS_FIELD                               : 16;\
          }
    union { UNSG32 u32TG_MAIN_INTPOS;
            struct w32TG_MAIN_INTPOS;
          };
    #define     w32TG_MAIN_MODE                                {\
            UNSG32 uMODE_EN                                    :  1;\
            UNSG32 RSVDx40_b1                                  : 31;\
          }
    union { UNSG32 u32TG_MAIN_MODE;
            struct w32TG_MAIN_MODE;
          };
    #define     w32TG_MAIN_HVREF                               {\
            UNSG32 uHVREF_SEL                                  :  1;\
            UNSG32 uHVREF_POL                                  :  1;\
            UNSG32 RSVDx44_b2                                  : 30;\
          }
    union { UNSG32 u32TG_MAIN_HVREF;
            struct w32TG_MAIN_HVREF;
          };
    } SIE_TG_MAIN;
    typedef union  T32TG_MAIN_INIT
          { UNSG32 u32;
            struct w32TG_MAIN_INIT;
                 } T32TG_MAIN_INIT;
    typedef union  T32TG_MAIN_SIZE
          { UNSG32 u32;
            struct w32TG_MAIN_SIZE;
                 } T32TG_MAIN_SIZE;
    typedef union  T32TG_MAIN_HS
          { UNSG32 u32;
            struct w32TG_MAIN_HS;
                 } T32TG_MAIN_HS;
    typedef union  T32TG_MAIN_HB
          { UNSG32 u32;
            struct w32TG_MAIN_HB;
                 } T32TG_MAIN_HB;
    typedef union  T32TG_MAIN_HB_Y
          { UNSG32 u32;
            struct w32TG_MAIN_HB_Y;
                 } T32TG_MAIN_HB_Y;
    typedef union  T32TG_MAIN_HB_C
          { UNSG32 u32;
            struct w32TG_MAIN_HB_C;
                 } T32TG_MAIN_HB_C;
    typedef union  T32TG_MAIN_HB_OUT
          { UNSG32 u32;
            struct w32TG_MAIN_HB_OUT;
                 } T32TG_MAIN_HB_OUT;
    typedef union  T32TG_MAIN_VS0
          { UNSG32 u32;
            struct w32TG_MAIN_VS0;
                 } T32TG_MAIN_VS0;
    typedef union  T32TG_MAIN_VS1
          { UNSG32 u32;
            struct w32TG_MAIN_VS1;
                 } T32TG_MAIN_VS1;
    typedef union  T32TG_MAIN_VB0
          { UNSG32 u32;
            struct w32TG_MAIN_VB0;
                 } T32TG_MAIN_VB0;
    typedef union  T32TG_MAIN_VB0_Y
          { UNSG32 u32;
            struct w32TG_MAIN_VB0_Y;
                 } T32TG_MAIN_VB0_Y;
    typedef union  T32TG_MAIN_VB0_C
          { UNSG32 u32;
            struct w32TG_MAIN_VB0_C;
                 } T32TG_MAIN_VB0_C;
    typedef union  T32TG_MAIN_VB0_OUT
          { UNSG32 u32;
            struct w32TG_MAIN_VB0_OUT;
                 } T32TG_MAIN_VB0_OUT;
    typedef union  T32TG_MAIN_VB1
          { UNSG32 u32;
            struct w32TG_MAIN_VB1;
                 } T32TG_MAIN_VB1;
    typedef union  T32TG_MAIN_SCAN
          { UNSG32 u32;
            struct w32TG_MAIN_SCAN;
                 } T32TG_MAIN_SCAN;
    typedef union  T32TG_MAIN_INTPOS
          { UNSG32 u32;
            struct w32TG_MAIN_INTPOS;
                 } T32TG_MAIN_INTPOS;
    typedef union  T32TG_MAIN_MODE
          { UNSG32 u32;
            struct w32TG_MAIN_MODE;
                 } T32TG_MAIN_MODE;
    typedef union  T32TG_MAIN_HVREF
          { UNSG32 u32;
            struct w32TG_MAIN_HVREF;
                 } T32TG_MAIN_HVREF;
    typedef union  TTG_MAIN_INIT
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_INIT;
                   };
                 } TTG_MAIN_INIT;
    typedef union  TTG_MAIN_SIZE
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_SIZE;
                   };
                 } TTG_MAIN_SIZE;
    typedef union  TTG_MAIN_HS
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_HS;
                   };
                 } TTG_MAIN_HS;
    typedef union  TTG_MAIN_HB
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_HB;
                   };
                 } TTG_MAIN_HB;
    typedef union  TTG_MAIN_HB_Y
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_HB_Y;
                   };
                 } TTG_MAIN_HB_Y;
    typedef union  TTG_MAIN_HB_C
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_HB_C;
                   };
                 } TTG_MAIN_HB_C;
    typedef union  TTG_MAIN_HB_OUT
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_HB_OUT;
                   };
                 } TTG_MAIN_HB_OUT;
    typedef union  TTG_MAIN_VS0
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_VS0;
                   };
                 } TTG_MAIN_VS0;
    typedef union  TTG_MAIN_VS1
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_VS1;
                   };
                 } TTG_MAIN_VS1;
    typedef union  TTG_MAIN_VB0
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_VB0;
                   };
                 } TTG_MAIN_VB0;
    typedef union  TTG_MAIN_VB0_Y
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_VB0_Y;
                   };
                 } TTG_MAIN_VB0_Y;
    typedef union  TTG_MAIN_VB0_C
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_VB0_C;
                   };
                 } TTG_MAIN_VB0_C;
    typedef union  TTG_MAIN_VB0_OUT
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_VB0_OUT;
                   };
                 } TTG_MAIN_VB0_OUT;
    typedef union  TTG_MAIN_VB1
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_VB1;
                   };
                 } TTG_MAIN_VB1;
    typedef union  TTG_MAIN_SCAN
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_SCAN;
                   };
                 } TTG_MAIN_SCAN;
    typedef union  TTG_MAIN_INTPOS
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_INTPOS;
                   };
                 } TTG_MAIN_INTPOS;
    typedef union  TTG_MAIN_MODE
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_MODE;
                   };
                 } TTG_MAIN_MODE;
    typedef union  TTG_MAIN_HVREF
          { UNSG32 u32[1];
            struct {
            struct w32TG_MAIN_HVREF;
                   };
                 } TTG_MAIN_HVREF;
     SIGN32 TG_MAIN_drvrd(SIE_TG_MAIN *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TG_MAIN_drvwr(SIE_TG_MAIN *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TG_MAIN_reset(SIE_TG_MAIN *p);
     SIGN32 TG_MAIN_cmp  (SIE_TG_MAIN *p, SIE_TG_MAIN *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TG_MAIN_check(p,pie,pfx,hLOG) TG_MAIN_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TG_MAIN_print(p,    pfx,hLOG) TG_MAIN_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_BITMAP40
#define h_BITMAP40 (){}
    #define     RA_BITMAP40_SEL                                0x0000
    #define   LSb32BITMAP40_SEL_BIT_POS0                          0
    #define       bBITMAP40_SEL_BIT_POS0                       6
    #define   MSK32BITMAP40_SEL_BIT_POS0                          0x0000003F
    #define   LSb32BITMAP40_SEL_BIT_POS1                          6
    #define       bBITMAP40_SEL_BIT_POS1                       6
    #define   MSK32BITMAP40_SEL_BIT_POS1                          0x00000FC0
    #define   LSb32BITMAP40_SEL_BIT_POS2                          12
    #define       bBITMAP40_SEL_BIT_POS2                       6
    #define   MSK32BITMAP40_SEL_BIT_POS2                          0x0003F000
    #define   LSb32BITMAP40_SEL_BIT_POS3                          18
    #define       bBITMAP40_SEL_BIT_POS3                       6
    #define   MSK32BITMAP40_SEL_BIT_POS3                          0x00FC0000
    #define   LSb32BITMAP40_SEL_BIT_POS4                          24
    #define       bBITMAP40_SEL_BIT_POS4                       6
    #define   MSK32BITMAP40_SEL_BIT_POS4                          0x3F000000
    #define     RA_BITMAP40_SEL1                               0x0004
    #define   LSb32BITMAP40_SEL_BIT_POS5                          0
    #define       bBITMAP40_SEL_BIT_POS5                       6
    #define   MSK32BITMAP40_SEL_BIT_POS5                          0x0000003F
    #define   LSb32BITMAP40_SEL_BIT_POS6                          6
    #define       bBITMAP40_SEL_BIT_POS6                       6
    #define   MSK32BITMAP40_SEL_BIT_POS6                          0x00000FC0
    #define   LSb32BITMAP40_SEL_BIT_POS7                          12
    #define       bBITMAP40_SEL_BIT_POS7                       6
    #define   MSK32BITMAP40_SEL_BIT_POS7                          0x0003F000
    #define   LSb32BITMAP40_SEL_BIT_POS8                          18
    #define       bBITMAP40_SEL_BIT_POS8                       6
    #define   MSK32BITMAP40_SEL_BIT_POS8                          0x00FC0000
    #define   LSb32BITMAP40_SEL_BIT_POS9                          24
    #define       bBITMAP40_SEL_BIT_POS9                       6
    #define   MSK32BITMAP40_SEL_BIT_POS9                          0x3F000000
    #define     RA_BITMAP40_SEL2                               0x0008
    #define   LSb32BITMAP40_SEL_BIT_POS10                         0
    #define       bBITMAP40_SEL_BIT_POS10                      6
    #define   MSK32BITMAP40_SEL_BIT_POS10                         0x0000003F
    #define   LSb32BITMAP40_SEL_BIT_POS11                         6
    #define       bBITMAP40_SEL_BIT_POS11                      6
    #define   MSK32BITMAP40_SEL_BIT_POS11                         0x00000FC0
    #define   LSb32BITMAP40_SEL_BIT_POS12                         12
    #define       bBITMAP40_SEL_BIT_POS12                      6
    #define   MSK32BITMAP40_SEL_BIT_POS12                         0x0003F000
    #define   LSb32BITMAP40_SEL_BIT_POS13                         18
    #define       bBITMAP40_SEL_BIT_POS13                      6
    #define   MSK32BITMAP40_SEL_BIT_POS13                         0x00FC0000
    #define   LSb32BITMAP40_SEL_BIT_POS14                         24
    #define       bBITMAP40_SEL_BIT_POS14                      6
    #define   MSK32BITMAP40_SEL_BIT_POS14                         0x3F000000
    #define     RA_BITMAP40_SEL3                               0x000C
    #define   LSb32BITMAP40_SEL_BIT_POS15                         0
    #define       bBITMAP40_SEL_BIT_POS15                      6
    #define   MSK32BITMAP40_SEL_BIT_POS15                         0x0000003F
    #define   LSb32BITMAP40_SEL_BIT_POS16                         6
    #define       bBITMAP40_SEL_BIT_POS16                      6
    #define   MSK32BITMAP40_SEL_BIT_POS16                         0x00000FC0
    #define   LSb32BITMAP40_SEL_BIT_POS17                         12
    #define       bBITMAP40_SEL_BIT_POS17                      6
    #define   MSK32BITMAP40_SEL_BIT_POS17                         0x0003F000
    #define   LSb32BITMAP40_SEL_BIT_POS18                         18
    #define       bBITMAP40_SEL_BIT_POS18                      6
    #define   MSK32BITMAP40_SEL_BIT_POS18                         0x00FC0000
    #define   LSb32BITMAP40_SEL_BIT_POS19                         24
    #define       bBITMAP40_SEL_BIT_POS19                      6
    #define   MSK32BITMAP40_SEL_BIT_POS19                         0x3F000000
    #define     RA_BITMAP40_SEL4                               0x0010
    #define   LSb32BITMAP40_SEL_BIT_POS20                         0
    #define       bBITMAP40_SEL_BIT_POS20                      6
    #define   MSK32BITMAP40_SEL_BIT_POS20                         0x0000003F
    #define   LSb32BITMAP40_SEL_BIT_POS21                         6
    #define       bBITMAP40_SEL_BIT_POS21                      6
    #define   MSK32BITMAP40_SEL_BIT_POS21                         0x00000FC0
    #define   LSb32BITMAP40_SEL_BIT_POS22                         12
    #define       bBITMAP40_SEL_BIT_POS22                      6
    #define   MSK32BITMAP40_SEL_BIT_POS22                         0x0003F000
    #define   LSb32BITMAP40_SEL_BIT_POS23                         18
    #define       bBITMAP40_SEL_BIT_POS23                      6
    #define   MSK32BITMAP40_SEL_BIT_POS23                         0x00FC0000
    #define   LSb32BITMAP40_SEL_BIT_POS24                         24
    #define       bBITMAP40_SEL_BIT_POS24                      6
    #define   MSK32BITMAP40_SEL_BIT_POS24                         0x3F000000
    #define     RA_BITMAP40_SEL5                               0x0014
    #define   LSb32BITMAP40_SEL_BIT_POS25                         0
    #define       bBITMAP40_SEL_BIT_POS25                      6
    #define   MSK32BITMAP40_SEL_BIT_POS25                         0x0000003F
    #define   LSb32BITMAP40_SEL_BIT_POS26                         6
    #define       bBITMAP40_SEL_BIT_POS26                      6
    #define   MSK32BITMAP40_SEL_BIT_POS26                         0x00000FC0
    #define   LSb32BITMAP40_SEL_BIT_POS27                         12
    #define       bBITMAP40_SEL_BIT_POS27                      6
    #define   MSK32BITMAP40_SEL_BIT_POS27                         0x0003F000
    #define   LSb32BITMAP40_SEL_BIT_POS28                         18
    #define       bBITMAP40_SEL_BIT_POS28                      6
    #define   MSK32BITMAP40_SEL_BIT_POS28                         0x00FC0000
    #define   LSb32BITMAP40_SEL_BIT_POS29                         24
    #define       bBITMAP40_SEL_BIT_POS29                      6
    #define   MSK32BITMAP40_SEL_BIT_POS29                         0x3F000000
    #define     RA_BITMAP40_SEL6                               0x0018
    #define   LSb32BITMAP40_SEL_BIT_POS30                         0
    #define       bBITMAP40_SEL_BIT_POS30                      6
    #define   MSK32BITMAP40_SEL_BIT_POS30                         0x0000003F
    #define   LSb32BITMAP40_SEL_BIT_POS31                         6
    #define       bBITMAP40_SEL_BIT_POS31                      6
    #define   MSK32BITMAP40_SEL_BIT_POS31                         0x00000FC0
    #define   LSb32BITMAP40_SEL_BIT_POS32                         12
    #define       bBITMAP40_SEL_BIT_POS32                      6
    #define   MSK32BITMAP40_SEL_BIT_POS32                         0x0003F000
    #define   LSb32BITMAP40_SEL_BIT_POS33                         18
    #define       bBITMAP40_SEL_BIT_POS33                      6
    #define   MSK32BITMAP40_SEL_BIT_POS33                         0x00FC0000
    #define   LSb32BITMAP40_SEL_BIT_POS34                         24
    #define       bBITMAP40_SEL_BIT_POS34                      6
    #define   MSK32BITMAP40_SEL_BIT_POS34                         0x3F000000
    #define     RA_BITMAP40_SEL7                               0x001C
    #define   LSb32BITMAP40_SEL_BIT_POS35                         0
    #define       bBITMAP40_SEL_BIT_POS35                      6
    #define   MSK32BITMAP40_SEL_BIT_POS35                         0x0000003F
    #define   LSb32BITMAP40_SEL_BIT_POS36                         6
    #define       bBITMAP40_SEL_BIT_POS36                      6
    #define   MSK32BITMAP40_SEL_BIT_POS36                         0x00000FC0
    #define   LSb32BITMAP40_SEL_BIT_POS37                         12
    #define       bBITMAP40_SEL_BIT_POS37                      6
    #define   MSK32BITMAP40_SEL_BIT_POS37                         0x0003F000
    #define   LSb32BITMAP40_SEL_BIT_POS38                         18
    #define       bBITMAP40_SEL_BIT_POS38                      6
    #define   MSK32BITMAP40_SEL_BIT_POS38                         0x00FC0000
    #define   LSb32BITMAP40_SEL_BIT_POS39                         24
    #define       bBITMAP40_SEL_BIT_POS39                      6
    #define   MSK32BITMAP40_SEL_BIT_POS39                         0x3F000000
    typedef struct SIE_BITMAP40 {
    #define     w32BITMAP40_SEL                                {\
            UNSG32 uSEL_BIT_POS0                               :  6;\
            UNSG32 uSEL_BIT_POS1                               :  6;\
            UNSG32 uSEL_BIT_POS2                               :  6;\
            UNSG32 uSEL_BIT_POS3                               :  6;\
            UNSG32 uSEL_BIT_POS4                               :  6;\
            UNSG32 RSVDx0_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP40_SEL;
            struct w32BITMAP40_SEL;
          };
    #define     w32BITMAP40_SEL1                               {\
            UNSG32 uSEL_BIT_POS5                               :  6;\
            UNSG32 uSEL_BIT_POS6                               :  6;\
            UNSG32 uSEL_BIT_POS7                               :  6;\
            UNSG32 uSEL_BIT_POS8                               :  6;\
            UNSG32 uSEL_BIT_POS9                               :  6;\
            UNSG32 RSVDx4_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP40_SEL1;
            struct w32BITMAP40_SEL1;
          };
    #define     w32BITMAP40_SEL2                               {\
            UNSG32 uSEL_BIT_POS10                              :  6;\
            UNSG32 uSEL_BIT_POS11                              :  6;\
            UNSG32 uSEL_BIT_POS12                              :  6;\
            UNSG32 uSEL_BIT_POS13                              :  6;\
            UNSG32 uSEL_BIT_POS14                              :  6;\
            UNSG32 RSVDx8_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP40_SEL2;
            struct w32BITMAP40_SEL2;
          };
    #define     w32BITMAP40_SEL3                               {\
            UNSG32 uSEL_BIT_POS15                              :  6;\
            UNSG32 uSEL_BIT_POS16                              :  6;\
            UNSG32 uSEL_BIT_POS17                              :  6;\
            UNSG32 uSEL_BIT_POS18                              :  6;\
            UNSG32 uSEL_BIT_POS19                              :  6;\
            UNSG32 RSVDxC_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP40_SEL3;
            struct w32BITMAP40_SEL3;
          };
    #define     w32BITMAP40_SEL4                               {\
            UNSG32 uSEL_BIT_POS20                              :  6;\
            UNSG32 uSEL_BIT_POS21                              :  6;\
            UNSG32 uSEL_BIT_POS22                              :  6;\
            UNSG32 uSEL_BIT_POS23                              :  6;\
            UNSG32 uSEL_BIT_POS24                              :  6;\
            UNSG32 RSVDx10_b30                                 :  2;\
          }
    union { UNSG32 u32BITMAP40_SEL4;
            struct w32BITMAP40_SEL4;
          };
    #define     w32BITMAP40_SEL5                               {\
            UNSG32 uSEL_BIT_POS25                              :  6;\
            UNSG32 uSEL_BIT_POS26                              :  6;\
            UNSG32 uSEL_BIT_POS27                              :  6;\
            UNSG32 uSEL_BIT_POS28                              :  6;\
            UNSG32 uSEL_BIT_POS29                              :  6;\
            UNSG32 RSVDx14_b30                                 :  2;\
          }
    union { UNSG32 u32BITMAP40_SEL5;
            struct w32BITMAP40_SEL5;
          };
    #define     w32BITMAP40_SEL6                               {\
            UNSG32 uSEL_BIT_POS30                              :  6;\
            UNSG32 uSEL_BIT_POS31                              :  6;\
            UNSG32 uSEL_BIT_POS32                              :  6;\
            UNSG32 uSEL_BIT_POS33                              :  6;\
            UNSG32 uSEL_BIT_POS34                              :  6;\
            UNSG32 RSVDx18_b30                                 :  2;\
          }
    union { UNSG32 u32BITMAP40_SEL6;
            struct w32BITMAP40_SEL6;
          };
    #define     w32BITMAP40_SEL7                               {\
            UNSG32 uSEL_BIT_POS35                              :  6;\
            UNSG32 uSEL_BIT_POS36                              :  6;\
            UNSG32 uSEL_BIT_POS37                              :  6;\
            UNSG32 uSEL_BIT_POS38                              :  6;\
            UNSG32 uSEL_BIT_POS39                              :  6;\
            UNSG32 RSVDx1C_b30                                 :  2;\
          }
    union { UNSG32 u32BITMAP40_SEL7;
            struct w32BITMAP40_SEL7;
          };
    } SIE_BITMAP40;
    typedef union  T32BITMAP40_SEL
          { UNSG32 u32;
            struct w32BITMAP40_SEL;
                 } T32BITMAP40_SEL;
    typedef union  T32BITMAP40_SEL1
          { UNSG32 u32;
            struct w32BITMAP40_SEL1;
                 } T32BITMAP40_SEL1;
    typedef union  T32BITMAP40_SEL2
          { UNSG32 u32;
            struct w32BITMAP40_SEL2;
                 } T32BITMAP40_SEL2;
    typedef union  T32BITMAP40_SEL3
          { UNSG32 u32;
            struct w32BITMAP40_SEL3;
                 } T32BITMAP40_SEL3;
    typedef union  T32BITMAP40_SEL4
          { UNSG32 u32;
            struct w32BITMAP40_SEL4;
                 } T32BITMAP40_SEL4;
    typedef union  T32BITMAP40_SEL5
          { UNSG32 u32;
            struct w32BITMAP40_SEL5;
                 } T32BITMAP40_SEL5;
    typedef union  T32BITMAP40_SEL6
          { UNSG32 u32;
            struct w32BITMAP40_SEL6;
                 } T32BITMAP40_SEL6;
    typedef union  T32BITMAP40_SEL7
          { UNSG32 u32;
            struct w32BITMAP40_SEL7;
                 } T32BITMAP40_SEL7;
    typedef union  TBITMAP40_SEL
          { UNSG32 u32[8];
            struct {
            struct w32BITMAP40_SEL;
            struct w32BITMAP40_SEL1;
            struct w32BITMAP40_SEL2;
            struct w32BITMAP40_SEL3;
            struct w32BITMAP40_SEL4;
            struct w32BITMAP40_SEL5;
            struct w32BITMAP40_SEL6;
            struct w32BITMAP40_SEL7;
                   };
                 } TBITMAP40_SEL;
     SIGN32 BITMAP40_drvrd(SIE_BITMAP40 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BITMAP40_drvwr(SIE_BITMAP40 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BITMAP40_reset(SIE_BITMAP40 *p);
     SIGN32 BITMAP40_cmp  (SIE_BITMAP40 *p, SIE_BITMAP40 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BITMAP40_check(p,pie,pfx,hLOG) BITMAP40_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BITMAP40_print(p,    pfx,hLOG) BITMAP40_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_BITMAP20
#define h_BITMAP20 (){}
    #define     RA_BITMAP20_SEL                                0x0000
    #define   LSb32BITMAP20_SEL_BIT_POS0                          0
    #define       bBITMAP20_SEL_BIT_POS0                       5
    #define   MSK32BITMAP20_SEL_BIT_POS0                          0x0000001F
    #define   LSb32BITMAP20_SEL_BIT_POS1                          5
    #define       bBITMAP20_SEL_BIT_POS1                       5
    #define   MSK32BITMAP20_SEL_BIT_POS1                          0x000003E0
    #define   LSb32BITMAP20_SEL_BIT_POS2                          10
    #define       bBITMAP20_SEL_BIT_POS2                       5
    #define   MSK32BITMAP20_SEL_BIT_POS2                          0x00007C00
    #define   LSb32BITMAP20_SEL_BIT_POS3                          15
    #define       bBITMAP20_SEL_BIT_POS3                       5
    #define   MSK32BITMAP20_SEL_BIT_POS3                          0x000F8000
    #define   LSb32BITMAP20_SEL_BIT_POS4                          20
    #define       bBITMAP20_SEL_BIT_POS4                       5
    #define   MSK32BITMAP20_SEL_BIT_POS4                          0x01F00000
    #define   LSb32BITMAP20_SEL_BIT_POS5                          25
    #define       bBITMAP20_SEL_BIT_POS5                       5
    #define   MSK32BITMAP20_SEL_BIT_POS5                          0x3E000000
    #define     RA_BITMAP20_SEL1                               0x0004
    #define   LSb32BITMAP20_SEL_BIT_POS6                          0
    #define       bBITMAP20_SEL_BIT_POS6                       5
    #define   MSK32BITMAP20_SEL_BIT_POS6                          0x0000001F
    #define   LSb32BITMAP20_SEL_BIT_POS7                          5
    #define       bBITMAP20_SEL_BIT_POS7                       5
    #define   MSK32BITMAP20_SEL_BIT_POS7                          0x000003E0
    #define   LSb32BITMAP20_SEL_BIT_POS8                          10
    #define       bBITMAP20_SEL_BIT_POS8                       5
    #define   MSK32BITMAP20_SEL_BIT_POS8                          0x00007C00
    #define   LSb32BITMAP20_SEL_BIT_POS9                          15
    #define       bBITMAP20_SEL_BIT_POS9                       5
    #define   MSK32BITMAP20_SEL_BIT_POS9                          0x000F8000
    #define   LSb32BITMAP20_SEL_BIT_POS10                         20
    #define       bBITMAP20_SEL_BIT_POS10                      5
    #define   MSK32BITMAP20_SEL_BIT_POS10                         0x01F00000
    #define   LSb32BITMAP20_SEL_BIT_POS11                         25
    #define       bBITMAP20_SEL_BIT_POS11                      5
    #define   MSK32BITMAP20_SEL_BIT_POS11                         0x3E000000
    #define     RA_BITMAP20_SEL2                               0x0008
    #define   LSb32BITMAP20_SEL_BIT_POS12                         0
    #define       bBITMAP20_SEL_BIT_POS12                      5
    #define   MSK32BITMAP20_SEL_BIT_POS12                         0x0000001F
    #define   LSb32BITMAP20_SEL_BIT_POS13                         5
    #define       bBITMAP20_SEL_BIT_POS13                      5
    #define   MSK32BITMAP20_SEL_BIT_POS13                         0x000003E0
    #define   LSb32BITMAP20_SEL_BIT_POS14                         10
    #define       bBITMAP20_SEL_BIT_POS14                      5
    #define   MSK32BITMAP20_SEL_BIT_POS14                         0x00007C00
    #define   LSb32BITMAP20_SEL_BIT_POS15                         15
    #define       bBITMAP20_SEL_BIT_POS15                      5
    #define   MSK32BITMAP20_SEL_BIT_POS15                         0x000F8000
    #define   LSb32BITMAP20_SEL_BIT_POS16                         20
    #define       bBITMAP20_SEL_BIT_POS16                      5
    #define   MSK32BITMAP20_SEL_BIT_POS16                         0x01F00000
    #define   LSb32BITMAP20_SEL_BIT_POS17                         25
    #define       bBITMAP20_SEL_BIT_POS17                      5
    #define   MSK32BITMAP20_SEL_BIT_POS17                         0x3E000000
    #define     RA_BITMAP20_SEL3                               0x000C
    #define   LSb32BITMAP20_SEL_BIT_POS18                         0
    #define       bBITMAP20_SEL_BIT_POS18                      5
    #define   MSK32BITMAP20_SEL_BIT_POS18                         0x0000001F
    #define   LSb32BITMAP20_SEL_BIT_POS19                         5
    #define       bBITMAP20_SEL_BIT_POS19                      5
    #define   MSK32BITMAP20_SEL_BIT_POS19                         0x000003E0
    typedef struct SIE_BITMAP20 {
    #define     w32BITMAP20_SEL                                {\
            UNSG32 uSEL_BIT_POS0                               :  5;\
            UNSG32 uSEL_BIT_POS1                               :  5;\
            UNSG32 uSEL_BIT_POS2                               :  5;\
            UNSG32 uSEL_BIT_POS3                               :  5;\
            UNSG32 uSEL_BIT_POS4                               :  5;\
            UNSG32 uSEL_BIT_POS5                               :  5;\
            UNSG32 RSVDx0_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP20_SEL;
            struct w32BITMAP20_SEL;
          };
    #define     w32BITMAP20_SEL1                               {\
            UNSG32 uSEL_BIT_POS6                               :  5;\
            UNSG32 uSEL_BIT_POS7                               :  5;\
            UNSG32 uSEL_BIT_POS8                               :  5;\
            UNSG32 uSEL_BIT_POS9                               :  5;\
            UNSG32 uSEL_BIT_POS10                              :  5;\
            UNSG32 uSEL_BIT_POS11                              :  5;\
            UNSG32 RSVDx4_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP20_SEL1;
            struct w32BITMAP20_SEL1;
          };
    #define     w32BITMAP20_SEL2                               {\
            UNSG32 uSEL_BIT_POS12                              :  5;\
            UNSG32 uSEL_BIT_POS13                              :  5;\
            UNSG32 uSEL_BIT_POS14                              :  5;\
            UNSG32 uSEL_BIT_POS15                              :  5;\
            UNSG32 uSEL_BIT_POS16                              :  5;\
            UNSG32 uSEL_BIT_POS17                              :  5;\
            UNSG32 RSVDx8_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP20_SEL2;
            struct w32BITMAP20_SEL2;
          };
    #define     w32BITMAP20_SEL3                               {\
            UNSG32 uSEL_BIT_POS18                              :  5;\
            UNSG32 uSEL_BIT_POS19                              :  5;\
            UNSG32 RSVDxC_b10                                  : 22;\
          }
    union { UNSG32 u32BITMAP20_SEL3;
            struct w32BITMAP20_SEL3;
          };
    } SIE_BITMAP20;
    typedef union  T32BITMAP20_SEL
          { UNSG32 u32;
            struct w32BITMAP20_SEL;
                 } T32BITMAP20_SEL;
    typedef union  T32BITMAP20_SEL1
          { UNSG32 u32;
            struct w32BITMAP20_SEL1;
                 } T32BITMAP20_SEL1;
    typedef union  T32BITMAP20_SEL2
          { UNSG32 u32;
            struct w32BITMAP20_SEL2;
                 } T32BITMAP20_SEL2;
    typedef union  T32BITMAP20_SEL3
          { UNSG32 u32;
            struct w32BITMAP20_SEL3;
                 } T32BITMAP20_SEL3;
    typedef union  TBITMAP20_SEL
          { UNSG32 u32[4];
            struct {
            struct w32BITMAP20_SEL;
            struct w32BITMAP20_SEL1;
            struct w32BITMAP20_SEL2;
            struct w32BITMAP20_SEL3;
                   };
                 } TBITMAP20_SEL;
     SIGN32 BITMAP20_drvrd(SIE_BITMAP20 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BITMAP20_drvwr(SIE_BITMAP20 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BITMAP20_reset(SIE_BITMAP20 *p);
     SIGN32 BITMAP20_cmp  (SIE_BITMAP20 *p, SIE_BITMAP20 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BITMAP20_check(p,pie,pfx,hLOG) BITMAP20_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BITMAP20_print(p,    pfx,hLOG) BITMAP20_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_BITMAP32
#define h_BITMAP32 (){}
    #define     RA_BITMAP32_SEL                                0x0000
    #define   LSb32BITMAP32_SEL_BIT_POS0                          0
    #define       bBITMAP32_SEL_BIT_POS0                       5
    #define   MSK32BITMAP32_SEL_BIT_POS0                          0x0000001F
    #define   LSb32BITMAP32_SEL_BIT_POS1                          5
    #define       bBITMAP32_SEL_BIT_POS1                       5
    #define   MSK32BITMAP32_SEL_BIT_POS1                          0x000003E0
    #define   LSb32BITMAP32_SEL_BIT_POS2                          10
    #define       bBITMAP32_SEL_BIT_POS2                       5
    #define   MSK32BITMAP32_SEL_BIT_POS2                          0x00007C00
    #define   LSb32BITMAP32_SEL_BIT_POS3                          15
    #define       bBITMAP32_SEL_BIT_POS3                       5
    #define   MSK32BITMAP32_SEL_BIT_POS3                          0x000F8000
    #define   LSb32BITMAP32_SEL_BIT_POS4                          20
    #define       bBITMAP32_SEL_BIT_POS4                       5
    #define   MSK32BITMAP32_SEL_BIT_POS4                          0x01F00000
    #define   LSb32BITMAP32_SEL_BIT_POS5                          25
    #define       bBITMAP32_SEL_BIT_POS5                       5
    #define   MSK32BITMAP32_SEL_BIT_POS5                          0x3E000000
    #define     RA_BITMAP32_SEL1                               0x0004
    #define   LSb32BITMAP32_SEL_BIT_POS6                          0
    #define       bBITMAP32_SEL_BIT_POS6                       5
    #define   MSK32BITMAP32_SEL_BIT_POS6                          0x0000001F
    #define   LSb32BITMAP32_SEL_BIT_POS7                          5
    #define       bBITMAP32_SEL_BIT_POS7                       5
    #define   MSK32BITMAP32_SEL_BIT_POS7                          0x000003E0
    #define   LSb32BITMAP32_SEL_BIT_POS8                          10
    #define       bBITMAP32_SEL_BIT_POS8                       5
    #define   MSK32BITMAP32_SEL_BIT_POS8                          0x00007C00
    #define   LSb32BITMAP32_SEL_BIT_POS9                          15
    #define       bBITMAP32_SEL_BIT_POS9                       5
    #define   MSK32BITMAP32_SEL_BIT_POS9                          0x000F8000
    #define   LSb32BITMAP32_SEL_BIT_POS10                         20
    #define       bBITMAP32_SEL_BIT_POS10                      5
    #define   MSK32BITMAP32_SEL_BIT_POS10                         0x01F00000
    #define   LSb32BITMAP32_SEL_BIT_POS11                         25
    #define       bBITMAP32_SEL_BIT_POS11                      5
    #define   MSK32BITMAP32_SEL_BIT_POS11                         0x3E000000
    #define     RA_BITMAP32_SEL2                               0x0008
    #define   LSb32BITMAP32_SEL_BIT_POS12                         0
    #define       bBITMAP32_SEL_BIT_POS12                      5
    #define   MSK32BITMAP32_SEL_BIT_POS12                         0x0000001F
    #define   LSb32BITMAP32_SEL_BIT_POS13                         5
    #define       bBITMAP32_SEL_BIT_POS13                      5
    #define   MSK32BITMAP32_SEL_BIT_POS13                         0x000003E0
    #define   LSb32BITMAP32_SEL_BIT_POS14                         10
    #define       bBITMAP32_SEL_BIT_POS14                      5
    #define   MSK32BITMAP32_SEL_BIT_POS14                         0x00007C00
    #define   LSb32BITMAP32_SEL_BIT_POS15                         15
    #define       bBITMAP32_SEL_BIT_POS15                      5
    #define   MSK32BITMAP32_SEL_BIT_POS15                         0x000F8000
    #define   LSb32BITMAP32_SEL_BIT_POS16                         20
    #define       bBITMAP32_SEL_BIT_POS16                      5
    #define   MSK32BITMAP32_SEL_BIT_POS16                         0x01F00000
    #define   LSb32BITMAP32_SEL_BIT_POS17                         25
    #define       bBITMAP32_SEL_BIT_POS17                      5
    #define   MSK32BITMAP32_SEL_BIT_POS17                         0x3E000000
    #define     RA_BITMAP32_SEL3                               0x000C
    #define   LSb32BITMAP32_SEL_BIT_POS18                         0
    #define       bBITMAP32_SEL_BIT_POS18                      5
    #define   MSK32BITMAP32_SEL_BIT_POS18                         0x0000001F
    #define   LSb32BITMAP32_SEL_BIT_POS19                         5
    #define       bBITMAP32_SEL_BIT_POS19                      5
    #define   MSK32BITMAP32_SEL_BIT_POS19                         0x000003E0
    #define   LSb32BITMAP32_SEL_BIT_POS20                         10
    #define       bBITMAP32_SEL_BIT_POS20                      5
    #define   MSK32BITMAP32_SEL_BIT_POS20                         0x00007C00
    #define   LSb32BITMAP32_SEL_BIT_POS21                         15
    #define       bBITMAP32_SEL_BIT_POS21                      5
    #define   MSK32BITMAP32_SEL_BIT_POS21                         0x000F8000
    #define   LSb32BITMAP32_SEL_BIT_POS22                         20
    #define       bBITMAP32_SEL_BIT_POS22                      5
    #define   MSK32BITMAP32_SEL_BIT_POS22                         0x01F00000
    #define   LSb32BITMAP32_SEL_BIT_POS23                         25
    #define       bBITMAP32_SEL_BIT_POS23                      5
    #define   MSK32BITMAP32_SEL_BIT_POS23                         0x3E000000
    #define     RA_BITMAP32_SEL4                               0x0010
    #define   LSb32BITMAP32_SEL_BIT_POS24                         0
    #define       bBITMAP32_SEL_BIT_POS24                      5
    #define   MSK32BITMAP32_SEL_BIT_POS24                         0x0000001F
    #define   LSb32BITMAP32_SEL_BIT_POS25                         5
    #define       bBITMAP32_SEL_BIT_POS25                      5
    #define   MSK32BITMAP32_SEL_BIT_POS25                         0x000003E0
    #define   LSb32BITMAP32_SEL_BIT_POS26                         10
    #define       bBITMAP32_SEL_BIT_POS26                      5
    #define   MSK32BITMAP32_SEL_BIT_POS26                         0x00007C00
    #define   LSb32BITMAP32_SEL_BIT_POS27                         15
    #define       bBITMAP32_SEL_BIT_POS27                      5
    #define   MSK32BITMAP32_SEL_BIT_POS27                         0x000F8000
    #define   LSb32BITMAP32_SEL_BIT_POS28                         20
    #define       bBITMAP32_SEL_BIT_POS28                      5
    #define   MSK32BITMAP32_SEL_BIT_POS28                         0x01F00000
    #define   LSb32BITMAP32_SEL_BIT_POS29                         25
    #define       bBITMAP32_SEL_BIT_POS29                      5
    #define   MSK32BITMAP32_SEL_BIT_POS29                         0x3E000000
    #define     RA_BITMAP32_SEL5                               0x0014
    #define   LSb32BITMAP32_SEL_BIT_POS30                         0
    #define       bBITMAP32_SEL_BIT_POS30                      5
    #define   MSK32BITMAP32_SEL_BIT_POS30                         0x0000001F
    #define   LSb32BITMAP32_SEL_BIT_POS31                         5
    #define       bBITMAP32_SEL_BIT_POS31                      5
    #define   MSK32BITMAP32_SEL_BIT_POS31                         0x000003E0
    typedef struct SIE_BITMAP32 {
    #define     w32BITMAP32_SEL                                {\
            UNSG32 uSEL_BIT_POS0                               :  5;\
            UNSG32 uSEL_BIT_POS1                               :  5;\
            UNSG32 uSEL_BIT_POS2                               :  5;\
            UNSG32 uSEL_BIT_POS3                               :  5;\
            UNSG32 uSEL_BIT_POS4                               :  5;\
            UNSG32 uSEL_BIT_POS5                               :  5;\
            UNSG32 RSVDx0_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP32_SEL;
            struct w32BITMAP32_SEL;
          };
    #define     w32BITMAP32_SEL1                               {\
            UNSG32 uSEL_BIT_POS6                               :  5;\
            UNSG32 uSEL_BIT_POS7                               :  5;\
            UNSG32 uSEL_BIT_POS8                               :  5;\
            UNSG32 uSEL_BIT_POS9                               :  5;\
            UNSG32 uSEL_BIT_POS10                              :  5;\
            UNSG32 uSEL_BIT_POS11                              :  5;\
            UNSG32 RSVDx4_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP32_SEL1;
            struct w32BITMAP32_SEL1;
          };
    #define     w32BITMAP32_SEL2                               {\
            UNSG32 uSEL_BIT_POS12                              :  5;\
            UNSG32 uSEL_BIT_POS13                              :  5;\
            UNSG32 uSEL_BIT_POS14                              :  5;\
            UNSG32 uSEL_BIT_POS15                              :  5;\
            UNSG32 uSEL_BIT_POS16                              :  5;\
            UNSG32 uSEL_BIT_POS17                              :  5;\
            UNSG32 RSVDx8_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP32_SEL2;
            struct w32BITMAP32_SEL2;
          };
    #define     w32BITMAP32_SEL3                               {\
            UNSG32 uSEL_BIT_POS18                              :  5;\
            UNSG32 uSEL_BIT_POS19                              :  5;\
            UNSG32 uSEL_BIT_POS20                              :  5;\
            UNSG32 uSEL_BIT_POS21                              :  5;\
            UNSG32 uSEL_BIT_POS22                              :  5;\
            UNSG32 uSEL_BIT_POS23                              :  5;\
            UNSG32 RSVDxC_b30                                  :  2;\
          }
    union { UNSG32 u32BITMAP32_SEL3;
            struct w32BITMAP32_SEL3;
          };
    #define     w32BITMAP32_SEL4                               {\
            UNSG32 uSEL_BIT_POS24                              :  5;\
            UNSG32 uSEL_BIT_POS25                              :  5;\
            UNSG32 uSEL_BIT_POS26                              :  5;\
            UNSG32 uSEL_BIT_POS27                              :  5;\
            UNSG32 uSEL_BIT_POS28                              :  5;\
            UNSG32 uSEL_BIT_POS29                              :  5;\
            UNSG32 RSVDx10_b30                                 :  2;\
          }
    union { UNSG32 u32BITMAP32_SEL4;
            struct w32BITMAP32_SEL4;
          };
    #define     w32BITMAP32_SEL5                               {\
            UNSG32 uSEL_BIT_POS30                              :  5;\
            UNSG32 uSEL_BIT_POS31                              :  5;\
            UNSG32 RSVDx14_b10                                 : 22;\
          }
    union { UNSG32 u32BITMAP32_SEL5;
            struct w32BITMAP32_SEL5;
          };
    } SIE_BITMAP32;
    typedef union  T32BITMAP32_SEL
          { UNSG32 u32;
            struct w32BITMAP32_SEL;
                 } T32BITMAP32_SEL;
    typedef union  T32BITMAP32_SEL1
          { UNSG32 u32;
            struct w32BITMAP32_SEL1;
                 } T32BITMAP32_SEL1;
    typedef union  T32BITMAP32_SEL2
          { UNSG32 u32;
            struct w32BITMAP32_SEL2;
                 } T32BITMAP32_SEL2;
    typedef union  T32BITMAP32_SEL3
          { UNSG32 u32;
            struct w32BITMAP32_SEL3;
                 } T32BITMAP32_SEL3;
    typedef union  T32BITMAP32_SEL4
          { UNSG32 u32;
            struct w32BITMAP32_SEL4;
                 } T32BITMAP32_SEL4;
    typedef union  T32BITMAP32_SEL5
          { UNSG32 u32;
            struct w32BITMAP32_SEL5;
                 } T32BITMAP32_SEL5;
    typedef union  TBITMAP32_SEL
          { UNSG32 u32[6];
            struct {
            struct w32BITMAP32_SEL;
            struct w32BITMAP32_SEL1;
            struct w32BITMAP32_SEL2;
            struct w32BITMAP32_SEL3;
            struct w32BITMAP32_SEL4;
            struct w32BITMAP32_SEL5;
                   };
                 } TBITMAP32_SEL;
     SIGN32 BITMAP32_drvrd(SIE_BITMAP32 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BITMAP32_drvwr(SIE_BITMAP32 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BITMAP32_reset(SIE_BITMAP32 *p);
     SIGN32 BITMAP32_cmp  (SIE_BITMAP32 *p, SIE_BITMAP32 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BITMAP32_check(p,pie,pfx,hLOG) BITMAP32_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BITMAP32_print(p,    pfx,hLOG) BITMAP32_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_BITMAP16
#define h_BITMAP16 (){}
    #define     RA_BITMAP16_SEL                                0x0000
    #define   LSb32BITMAP16_SEL_BIT_POS0                          0
    #define       bBITMAP16_SEL_BIT_POS0                       4
    #define   MSK32BITMAP16_SEL_BIT_POS0                          0x0000000F
    #define   LSb32BITMAP16_SEL_BIT_POS1                          4
    #define       bBITMAP16_SEL_BIT_POS1                       4
    #define   MSK32BITMAP16_SEL_BIT_POS1                          0x000000F0
    #define   LSb32BITMAP16_SEL_BIT_POS2                          8
    #define       bBITMAP16_SEL_BIT_POS2                       4
    #define   MSK32BITMAP16_SEL_BIT_POS2                          0x00000F00
    #define   LSb32BITMAP16_SEL_BIT_POS3                          12
    #define       bBITMAP16_SEL_BIT_POS3                       4
    #define   MSK32BITMAP16_SEL_BIT_POS3                          0x0000F000
    #define   LSb32BITMAP16_SEL_BIT_POS4                          16
    #define       bBITMAP16_SEL_BIT_POS4                       4
    #define   MSK32BITMAP16_SEL_BIT_POS4                          0x000F0000
    #define   LSb32BITMAP16_SEL_BIT_POS5                          20
    #define       bBITMAP16_SEL_BIT_POS5                       4
    #define   MSK32BITMAP16_SEL_BIT_POS5                          0x00F00000
    #define   LSb32BITMAP16_SEL_BIT_POS6                          24
    #define       bBITMAP16_SEL_BIT_POS6                       4
    #define   MSK32BITMAP16_SEL_BIT_POS6                          0x0F000000
    #define   LSb32BITMAP16_SEL_BIT_POS7                          28
    #define       bBITMAP16_SEL_BIT_POS7                       4
    #define   MSK32BITMAP16_SEL_BIT_POS7                          0xF0000000
    #define     RA_BITMAP16_SEL1                               0x0004
    #define   LSb32BITMAP16_SEL_BIT_POS8                          0
    #define       bBITMAP16_SEL_BIT_POS8                       4
    #define   MSK32BITMAP16_SEL_BIT_POS8                          0x0000000F
    #define   LSb32BITMAP16_SEL_BIT_POS9                          4
    #define       bBITMAP16_SEL_BIT_POS9                       4
    #define   MSK32BITMAP16_SEL_BIT_POS9                          0x000000F0
    #define   LSb32BITMAP16_SEL_BIT_POS10                         8
    #define       bBITMAP16_SEL_BIT_POS10                      4
    #define   MSK32BITMAP16_SEL_BIT_POS10                         0x00000F00
    #define   LSb32BITMAP16_SEL_BIT_POS11                         12
    #define       bBITMAP16_SEL_BIT_POS11                      4
    #define   MSK32BITMAP16_SEL_BIT_POS11                         0x0000F000
    #define   LSb32BITMAP16_SEL_BIT_POS12                         16
    #define       bBITMAP16_SEL_BIT_POS12                      4
    #define   MSK32BITMAP16_SEL_BIT_POS12                         0x000F0000
    #define   LSb32BITMAP16_SEL_BIT_POS13                         20
    #define       bBITMAP16_SEL_BIT_POS13                      4
    #define   MSK32BITMAP16_SEL_BIT_POS13                         0x00F00000
    #define   LSb32BITMAP16_SEL_BIT_POS14                         24
    #define       bBITMAP16_SEL_BIT_POS14                      4
    #define   MSK32BITMAP16_SEL_BIT_POS14                         0x0F000000
    #define   LSb32BITMAP16_SEL_BIT_POS15                         28
    #define       bBITMAP16_SEL_BIT_POS15                      4
    #define   MSK32BITMAP16_SEL_BIT_POS15                         0xF0000000
    typedef struct SIE_BITMAP16 {
    #define     w32BITMAP16_SEL                                {\
            UNSG32 uSEL_BIT_POS0                               :  4;\
            UNSG32 uSEL_BIT_POS1                               :  4;\
            UNSG32 uSEL_BIT_POS2                               :  4;\
            UNSG32 uSEL_BIT_POS3                               :  4;\
            UNSG32 uSEL_BIT_POS4                               :  4;\
            UNSG32 uSEL_BIT_POS5                               :  4;\
            UNSG32 uSEL_BIT_POS6                               :  4;\
            UNSG32 uSEL_BIT_POS7                               :  4;\
          }
    union { UNSG32 u32BITMAP16_SEL;
            struct w32BITMAP16_SEL;
          };
    #define     w32BITMAP16_SEL1                               {\
            UNSG32 uSEL_BIT_POS8                               :  4;\
            UNSG32 uSEL_BIT_POS9                               :  4;\
            UNSG32 uSEL_BIT_POS10                              :  4;\
            UNSG32 uSEL_BIT_POS11                              :  4;\
            UNSG32 uSEL_BIT_POS12                              :  4;\
            UNSG32 uSEL_BIT_POS13                              :  4;\
            UNSG32 uSEL_BIT_POS14                              :  4;\
            UNSG32 uSEL_BIT_POS15                              :  4;\
          }
    union { UNSG32 u32BITMAP16_SEL1;
            struct w32BITMAP16_SEL1;
          };
    } SIE_BITMAP16;
    typedef union  T32BITMAP16_SEL
          { UNSG32 u32;
            struct w32BITMAP16_SEL;
                 } T32BITMAP16_SEL;
    typedef union  T32BITMAP16_SEL1
          { UNSG32 u32;
            struct w32BITMAP16_SEL1;
                 } T32BITMAP16_SEL1;
    typedef union  TBITMAP16_SEL
          { UNSG32 u32[2];
            struct {
            struct w32BITMAP16_SEL;
            struct w32BITMAP16_SEL1;
                   };
                 } TBITMAP16_SEL;
     SIGN32 BITMAP16_drvrd(SIE_BITMAP16 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BITMAP16_drvwr(SIE_BITMAP16 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BITMAP16_reset(SIE_BITMAP16 *p);
     SIGN32 BITMAP16_cmp  (SIE_BITMAP16 *p, SIE_BITMAP16 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BITMAP16_check(p,pie,pfx,hLOG) BITMAP16_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BITMAP16_print(p,    pfx,hLOG) BITMAP16_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_BITMAP12
#define h_BITMAP12 (){}
    #define     RA_BITMAP12_SEL                                0x0000
    #define   LSb32BITMAP12_SEL_BIT_POS0                          0
    #define       bBITMAP12_SEL_BIT_POS0                       4
    #define   MSK32BITMAP12_SEL_BIT_POS0                          0x0000000F
    #define   LSb32BITMAP12_SEL_BIT_POS1                          4
    #define       bBITMAP12_SEL_BIT_POS1                       4
    #define   MSK32BITMAP12_SEL_BIT_POS1                          0x000000F0
    #define   LSb32BITMAP12_SEL_BIT_POS2                          8
    #define       bBITMAP12_SEL_BIT_POS2                       4
    #define   MSK32BITMAP12_SEL_BIT_POS2                          0x00000F00
    #define   LSb32BITMAP12_SEL_BIT_POS3                          12
    #define       bBITMAP12_SEL_BIT_POS3                       4
    #define   MSK32BITMAP12_SEL_BIT_POS3                          0x0000F000
    #define   LSb32BITMAP12_SEL_BIT_POS4                          16
    #define       bBITMAP12_SEL_BIT_POS4                       4
    #define   MSK32BITMAP12_SEL_BIT_POS4                          0x000F0000
    #define   LSb32BITMAP12_SEL_BIT_POS5                          20
    #define       bBITMAP12_SEL_BIT_POS5                       4
    #define   MSK32BITMAP12_SEL_BIT_POS5                          0x00F00000
    #define   LSb32BITMAP12_SEL_BIT_POS6                          24
    #define       bBITMAP12_SEL_BIT_POS6                       4
    #define   MSK32BITMAP12_SEL_BIT_POS6                          0x0F000000
    #define   LSb32BITMAP12_SEL_BIT_POS7                          28
    #define       bBITMAP12_SEL_BIT_POS7                       4
    #define   MSK32BITMAP12_SEL_BIT_POS7                          0xF0000000
    #define     RA_BITMAP12_SEL1                               0x0004
    #define   LSb32BITMAP12_SEL_BIT_POS8                          0
    #define       bBITMAP12_SEL_BIT_POS8                       4
    #define   MSK32BITMAP12_SEL_BIT_POS8                          0x0000000F
    #define   LSb32BITMAP12_SEL_BIT_POS9                          4
    #define       bBITMAP12_SEL_BIT_POS9                       4
    #define   MSK32BITMAP12_SEL_BIT_POS9                          0x000000F0
    #define   LSb32BITMAP12_SEL_BIT_POS10                         8
    #define       bBITMAP12_SEL_BIT_POS10                      4
    #define   MSK32BITMAP12_SEL_BIT_POS10                         0x00000F00
    #define   LSb32BITMAP12_SEL_BIT_POS11                         12
    #define       bBITMAP12_SEL_BIT_POS11                      4
    #define   MSK32BITMAP12_SEL_BIT_POS11                         0x0000F000
    typedef struct SIE_BITMAP12 {
    #define     w32BITMAP12_SEL                                {\
            UNSG32 uSEL_BIT_POS0                               :  4;\
            UNSG32 uSEL_BIT_POS1                               :  4;\
            UNSG32 uSEL_BIT_POS2                               :  4;\
            UNSG32 uSEL_BIT_POS3                               :  4;\
            UNSG32 uSEL_BIT_POS4                               :  4;\
            UNSG32 uSEL_BIT_POS5                               :  4;\
            UNSG32 uSEL_BIT_POS6                               :  4;\
            UNSG32 uSEL_BIT_POS7                               :  4;\
          }
    union { UNSG32 u32BITMAP12_SEL;
            struct w32BITMAP12_SEL;
          };
    #define     w32BITMAP12_SEL1                               {\
            UNSG32 uSEL_BIT_POS8                               :  4;\
            UNSG32 uSEL_BIT_POS9                               :  4;\
            UNSG32 uSEL_BIT_POS10                              :  4;\
            UNSG32 uSEL_BIT_POS11                              :  4;\
            UNSG32 RSVDx4_b16                                  : 16;\
          }
    union { UNSG32 u32BITMAP12_SEL1;
            struct w32BITMAP12_SEL1;
          };
    } SIE_BITMAP12;
    typedef union  T32BITMAP12_SEL
          { UNSG32 u32;
            struct w32BITMAP12_SEL;
                 } T32BITMAP12_SEL;
    typedef union  T32BITMAP12_SEL1
          { UNSG32 u32;
            struct w32BITMAP12_SEL1;
                 } T32BITMAP12_SEL1;
    typedef union  TBITMAP12_SEL
          { UNSG32 u32[2];
            struct {
            struct w32BITMAP12_SEL;
            struct w32BITMAP12_SEL1;
                   };
                 } TBITMAP12_SEL;
     SIGN32 BITMAP12_drvrd(SIE_BITMAP12 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BITMAP12_drvwr(SIE_BITMAP12 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BITMAP12_reset(SIE_BITMAP12 *p);
     SIGN32 BITMAP12_cmp  (SIE_BITMAP12 *p, SIE_BITMAP12 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BITMAP12_check(p,pie,pfx,hLOG) BITMAP12_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BITMAP12_print(p,    pfx,hLOG) BITMAP12_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_ReadClient
#define h_ReadClient (){}
    #define     RA_ReadClient_Rd                               0x0000
    #define   LSb32ReadClient_Rd_start                            0
    #define       bReadClient_Rd_start                         1
    #define   MSK32ReadClient_Rd_start                            0x00000001
    #define   LSb32ReadClient_Rd_clear                            1
    #define       bReadClient_Rd_clear                         1
    #define   MSK32ReadClient_Rd_clear                            0x00000002
    #define     RA_ReadClient_Word                             0x0004
    #define   LSb32ReadClient_Word_tot                            0
    #define       bReadClient_Word_tot                         32
    #define   MSK32ReadClient_Word_tot                            0xFFFFFFFF
    #define     RA_ReadClient_NonStdRes                        0x0008
    #define   LSb32ReadClient_NonStdRes_enable                    0
    #define       bReadClient_NonStdRes_enable                 1
    #define   MSK32ReadClient_NonStdRes_enable                    0x00000001
    #define   LSb32ReadClient_NonStdRes_pixlineTot                1
    #define       bReadClient_NonStdRes_pixlineTot             13
    #define   MSK32ReadClient_NonStdRes_pixlineTot                0x00003FFE
    #define   LSb32ReadClient_NonStdRes_flushCnt                  14
    #define       bReadClient_NonStdRes_flushCnt               4
    #define   MSK32ReadClient_NonStdRes_flushCnt                  0x0003C000
    #define     RA_ReadClient_pack                             0x000C
    #define   LSb32ReadClient_pack_Sel                            0
    #define       bReadClient_pack_Sel                         4
    #define   MSK32ReadClient_pack_Sel                            0x0000000F
    typedef struct SIE_ReadClient {
    #define     w32ReadClient_Rd                               {\
            UNSG32 uRd_start                                   :  1;\
            UNSG32 uRd_clear                                   :  1;\
            UNSG32 RSVDx0_b2                                   : 30;\
          }
    union { UNSG32 u32ReadClient_Rd;
            struct w32ReadClient_Rd;
          };
    #define     w32ReadClient_Word                             {\
            UNSG32 uWord_tot                                   : 32;\
          }
    union { UNSG32 u32ReadClient_Word;
            struct w32ReadClient_Word;
          };
    #define     w32ReadClient_NonStdRes                        {\
            UNSG32 uNonStdRes_enable                           :  1;\
            UNSG32 uNonStdRes_pixlineTot                       : 13;\
            UNSG32 uNonStdRes_flushCnt                         :  4;\
            UNSG32 RSVDx8_b18                                  : 14;\
          }
    union { UNSG32 u32ReadClient_NonStdRes;
            struct w32ReadClient_NonStdRes;
          };
    #define     w32ReadClient_pack                             {\
            UNSG32 upack_Sel                                   :  4;\
            UNSG32 RSVDxC_b4                                   : 28;\
          }
    union { UNSG32 u32ReadClient_pack;
            struct w32ReadClient_pack;
          };
    } SIE_ReadClient;
    typedef union  T32ReadClient_Rd
          { UNSG32 u32;
            struct w32ReadClient_Rd;
                 } T32ReadClient_Rd;
    typedef union  T32ReadClient_Word
          { UNSG32 u32;
            struct w32ReadClient_Word;
                 } T32ReadClient_Word;
    typedef union  T32ReadClient_NonStdRes
          { UNSG32 u32;
            struct w32ReadClient_NonStdRes;
                 } T32ReadClient_NonStdRes;
    typedef union  T32ReadClient_pack
          { UNSG32 u32;
            struct w32ReadClient_pack;
                 } T32ReadClient_pack;
    typedef union  TReadClient_Rd
          { UNSG32 u32[1];
            struct {
            struct w32ReadClient_Rd;
                   };
                 } TReadClient_Rd;
    typedef union  TReadClient_Word
          { UNSG32 u32[1];
            struct {
            struct w32ReadClient_Word;
                   };
                 } TReadClient_Word;
    typedef union  TReadClient_NonStdRes
          { UNSG32 u32[1];
            struct {
            struct w32ReadClient_NonStdRes;
                   };
                 } TReadClient_NonStdRes;
    typedef union  TReadClient_pack
          { UNSG32 u32[1];
            struct {
            struct w32ReadClient_pack;
                   };
                 } TReadClient_pack;
     SIGN32 ReadClient_drvrd(SIE_ReadClient *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 ReadClient_drvwr(SIE_ReadClient *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void ReadClient_reset(SIE_ReadClient *p);
     SIGN32 ReadClient_cmp  (SIE_ReadClient *p, SIE_ReadClient *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define ReadClient_check(p,pie,pfx,hLOG) ReadClient_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define ReadClient_print(p,    pfx,hLOG) ReadClient_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_WriteClient
#define h_WriteClient (){}
    #define     RA_WriteClient_Wr                              0x0000
    #define   LSb32WriteClient_Wr_start                           0
    #define       bWriteClient_Wr_start                        1
    #define   MSK32WriteClient_Wr_start                           0x00000001
    #define   LSb32WriteClient_Wr_clear                           1
    #define       bWriteClient_Wr_clear                        1
    #define   MSK32WriteClient_Wr_clear                           0x00000002
    #define     RA_WriteClient_pix                             0x0004
    #define   LSb32WriteClient_pix_tot                            0
    #define       bWriteClient_pix_tot                         32
    #define   MSK32WriteClient_pix_tot                            0xFFFFFFFF
    #define     RA_WriteClient_NonStdRes                       0x0008
    #define   LSb32WriteClient_NonStdRes_enable                   0
    #define       bWriteClient_NonStdRes_enable                1
    #define   MSK32WriteClient_NonStdRes_enable                   0x00000001
    #define   LSb32WriteClient_NonStdRes_pixlineTot               1
    #define       bWriteClient_NonStdRes_pixlineTot            13
    #define   MSK32WriteClient_NonStdRes_pixlineTot               0x00003FFE
    #define     RA_WriteClient_pack                            0x000C
    #define   LSb32WriteClient_pack_Sel                           0
    #define       bWriteClient_pack_Sel                        4
    #define   MSK32WriteClient_pack_Sel                           0x0000000F
    typedef struct SIE_WriteClient {
    #define     w32WriteClient_Wr                              {\
            UNSG32 uWr_start                                   :  1;\
            UNSG32 uWr_clear                                   :  1;\
            UNSG32 RSVDx0_b2                                   : 30;\
          }
    union { UNSG32 u32WriteClient_Wr;
            struct w32WriteClient_Wr;
          };
    #define     w32WriteClient_pix                             {\
            UNSG32 upix_tot                                    : 32;\
          }
    union { UNSG32 u32WriteClient_pix;
            struct w32WriteClient_pix;
          };
    #define     w32WriteClient_NonStdRes                       {\
            UNSG32 uNonStdRes_enable                           :  1;\
            UNSG32 uNonStdRes_pixlineTot                       : 13;\
            UNSG32 RSVDx8_b14                                  : 18;\
          }
    union { UNSG32 u32WriteClient_NonStdRes;
            struct w32WriteClient_NonStdRes;
          };
    #define     w32WriteClient_pack                            {\
            UNSG32 upack_Sel                                   :  4;\
            UNSG32 RSVDxC_b4                                   : 28;\
          }
    union { UNSG32 u32WriteClient_pack;
            struct w32WriteClient_pack;
          };
    } SIE_WriteClient;
    typedef union  T32WriteClient_Wr
          { UNSG32 u32;
            struct w32WriteClient_Wr;
                 } T32WriteClient_Wr;
    typedef union  T32WriteClient_pix
          { UNSG32 u32;
            struct w32WriteClient_pix;
                 } T32WriteClient_pix;
    typedef union  T32WriteClient_NonStdRes
          { UNSG32 u32;
            struct w32WriteClient_NonStdRes;
                 } T32WriteClient_NonStdRes;
    typedef union  T32WriteClient_pack
          { UNSG32 u32;
            struct w32WriteClient_pack;
                 } T32WriteClient_pack;
    typedef union  TWriteClient_Wr
          { UNSG32 u32[1];
            struct {
            struct w32WriteClient_Wr;
                   };
                 } TWriteClient_Wr;
    typedef union  TWriteClient_pix
          { UNSG32 u32[1];
            struct {
            struct w32WriteClient_pix;
                   };
                 } TWriteClient_pix;
    typedef union  TWriteClient_NonStdRes
          { UNSG32 u32[1];
            struct {
            struct w32WriteClient_NonStdRes;
                   };
                 } TWriteClient_NonStdRes;
    typedef union  TWriteClient_pack
          { UNSG32 u32[1];
            struct {
            struct w32WriteClient_pack;
                   };
                 } TWriteClient_pack;
     SIGN32 WriteClient_drvrd(SIE_WriteClient *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 WriteClient_drvwr(SIE_WriteClient *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void WriteClient_reset(SIE_WriteClient *p);
     SIGN32 WriteClient_cmp  (SIE_WriteClient *p, SIE_WriteClient *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define WriteClient_check(p,pie,pfx,hLOG) WriteClient_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define WriteClient_print(p,    pfx,hLOG) WriteClient_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_ClientIF
#define h_ClientIF (){}
    #define     RA_ClientIF_MR0                                0x0000
    #define   LSb32ClientIF_MR0_start                             0
    #define       bClientIF_MR0_start                          1
    #define   MSK32ClientIF_MR0_start                             0x00000001
    #define   LSb32ClientIF_MR0_clear                             1
    #define       bClientIF_MR0_clear                          1
    #define   MSK32ClientIF_MR0_clear                             0x00000002
    #define     RA_ClientIF_MR0_word                           0x0004
    #define   LSb32ClientIF_MR0_word_tot                          0
    #define       bClientIF_MR0_word_tot                       32
    #define   MSK32ClientIF_MR0_word_tot                          0xFFFFFFFF
    #define     RA_ClientIF_MR1                                0x0008
    #define   LSb32ClientIF_MR1_start                             0
    #define       bClientIF_MR1_start                          1
    #define   MSK32ClientIF_MR1_start                             0x00000001
    #define   LSb32ClientIF_MR1_clear                             1
    #define       bClientIF_MR1_clear                          1
    #define   MSK32ClientIF_MR1_clear                             0x00000002
    #define     RA_ClientIF_MR1_word                           0x000C
    #define   LSb32ClientIF_MR1_word_tot                          0
    #define       bClientIF_MR1_word_tot                       32
    #define   MSK32ClientIF_MR1_word_tot                          0xFFFFFFFF
    #define     RA_ClientIF_CTRL0                              0x0010
    #define   LSb32ClientIF_CTRL0_CLKEN_Main0                     0
    #define       bClientIF_CTRL0_CLKEN_Main0                  1
    #define   MSK32ClientIF_CTRL0_CLKEN_Main0                     0x00000001
    #define   LSb32ClientIF_CTRL0_CLKEN_Main1                     1
    #define       bClientIF_CTRL0_CLKEN_Main1                  1
    #define   MSK32ClientIF_CTRL0_CLKEN_Main1                     0x00000002
    #define   LSb32ClientIF_CTRL0_rdmain_initval0                 2
    #define       bClientIF_CTRL0_rdmain_initval0              1
    #define   MSK32ClientIF_CTRL0_rdmain_initval0                 0x00000004
    #define   LSb32ClientIF_CTRL0_rdmain_initval1                 3
    #define       bClientIF_CTRL0_rdmain_initval1              1
    #define   MSK32ClientIF_CTRL0_rdmain_initval1                 0x00000008
    #define   LSb32ClientIF_CTRL0_rdm_mask_sftrst                 4
    #define       bClientIF_CTRL0_rdm_mask_sftrst              1
    #define   MSK32ClientIF_CTRL0_rdm_mask_sftrst                 0x00000010
    #define   LSb32ClientIF_CTRL0_packSel_MR0                     5
    #define       bClientIF_CTRL0_packSel_MR0                  4
    #define   MSK32ClientIF_CTRL0_packSel_MR0                     0x000001E0
    #define   LSb32ClientIF_CTRL0_packSel_MR1                     9
    #define       bClientIF_CTRL0_packSel_MR1                  2
    #define   MSK32ClientIF_CTRL0_packSel_MR1                     0x00000600
    #define   LSb32ClientIF_CTRL0_ups420_idataSelM                11
    #define       bClientIF_CTRL0_ups420_idataSelM             1
    #define   MSK32ClientIF_CTRL0_ups420_idataSelM                0x00000800
    #define   LSb32ClientIF_CTRL0_read_sel_420SP                  12
    #define       bClientIF_CTRL0_read_sel_420SP               1
    #define   MSK32ClientIF_CTRL0_read_sel_420SP                  0x00001000
    #define   LSb32ClientIF_CTRL0_ups420_idat_ctrl                13
    #define       bClientIF_CTRL0_ups420_idat_ctrl             3
    #define   MSK32ClientIF_CTRL0_ups420_idat_ctrl                0x0000E000
    #define     RA_ClientIF_DUMMY                              0x0014
    #define   LSb32ClientIF_DUMMY_dummy                           0
    #define       bClientIF_DUMMY_dummy                        32
    #define   MSK32ClientIF_DUMMY_dummy                           0xFFFFFFFF
    #define     RA_ClientIF_CTRL2                              0x0018
    #define   LSb32ClientIF_CTRL2_nonStdResEn_MR0                 0
    #define       bClientIF_CTRL2_nonStdResEn_MR0              1
    #define   MSK32ClientIF_CTRL2_nonStdResEn_MR0                 0x00000001
    #define   LSb32ClientIF_CTRL2_pixlineTot_MR0                  1
    #define       bClientIF_CTRL2_pixlineTot_MR0               13
    #define   MSK32ClientIF_CTRL2_pixlineTot_MR0                  0x00003FFE
    #define   LSb32ClientIF_CTRL2_flushCnt_MR0                    14
    #define       bClientIF_CTRL2_flushCnt_MR0                 4
    #define   MSK32ClientIF_CTRL2_flushCnt_MR0                    0x0003C000
    #define     RA_ClientIF_CTRL3                              0x001C
    #define   LSb32ClientIF_CTRL3_nonStdResEn_MR1                 0
    #define       bClientIF_CTRL3_nonStdResEn_MR1              1
    #define   MSK32ClientIF_CTRL3_nonStdResEn_MR1                 0x00000001
    #define   LSb32ClientIF_CTRL3_pixlineTot_MR1                  1
    #define       bClientIF_CTRL3_pixlineTot_MR1               13
    #define   MSK32ClientIF_CTRL3_pixlineTot_MR1                  0x00003FFE
    #define   LSb32ClientIF_CTRL3_flushCnt_MR1                    14
    #define       bClientIF_CTRL3_flushCnt_MR1                 4
    #define   MSK32ClientIF_CTRL3_flushCnt_MR1                    0x0003C000
    #define     RA_ClientIF_RdClientVmxVm                      0x0020
    typedef struct SIE_ClientIF {
    #define     w32ClientIF_MR0                                {\
            UNSG32 uMR0_start                                  :  1;\
            UNSG32 uMR0_clear                                  :  1;\
            UNSG32 RSVDx0_b2                                   : 30;\
          }
    union { UNSG32 u32ClientIF_MR0;
            struct w32ClientIF_MR0;
          };
    #define     w32ClientIF_MR0_word                           {\
            UNSG32 uMR0_word_tot                               : 32;\
          }
    union { UNSG32 u32ClientIF_MR0_word;
            struct w32ClientIF_MR0_word;
          };
    #define     w32ClientIF_MR1                                {\
            UNSG32 uMR1_start                                  :  1;\
            UNSG32 uMR1_clear                                  :  1;\
            UNSG32 RSVDx8_b2                                   : 30;\
          }
    union { UNSG32 u32ClientIF_MR1;
            struct w32ClientIF_MR1;
          };
    #define     w32ClientIF_MR1_word                           {\
            UNSG32 uMR1_word_tot                               : 32;\
          }
    union { UNSG32 u32ClientIF_MR1_word;
            struct w32ClientIF_MR1_word;
          };
    #define     w32ClientIF_CTRL0                              {\
            UNSG32 uCTRL0_CLKEN_Main0                          :  1;\
            UNSG32 uCTRL0_CLKEN_Main1                          :  1;\
            UNSG32 uCTRL0_rdmain_initval0                      :  1;\
            UNSG32 uCTRL0_rdmain_initval1                      :  1;\
            UNSG32 uCTRL0_rdm_mask_sftrst                      :  1;\
            UNSG32 uCTRL0_packSel_MR0                          :  4;\
            UNSG32 uCTRL0_packSel_MR1                          :  2;\
            UNSG32 uCTRL0_ups420_idataSelM                     :  1;\
            UNSG32 uCTRL0_read_sel_420SP                       :  1;\
            UNSG32 uCTRL0_ups420_idat_ctrl                     :  3;\
            UNSG32 RSVDx10_b16                                 : 16;\
          }
    union { UNSG32 u32ClientIF_CTRL0;
            struct w32ClientIF_CTRL0;
          };
    #define     w32ClientIF_DUMMY                              {\
            UNSG32 uDUMMY_dummy                                : 32;\
          }
    union { UNSG32 u32ClientIF_DUMMY;
            struct w32ClientIF_DUMMY;
          };
    #define     w32ClientIF_CTRL2                              {\
            UNSG32 uCTRL2_nonStdResEn_MR0                      :  1;\
            UNSG32 uCTRL2_pixlineTot_MR0                       : 13;\
            UNSG32 uCTRL2_flushCnt_MR0                         :  4;\
            UNSG32 RSVDx18_b18                                 : 14;\
          }
    union { UNSG32 u32ClientIF_CTRL2;
            struct w32ClientIF_CTRL2;
          };
    #define     w32ClientIF_CTRL3                              {\
            UNSG32 uCTRL3_nonStdResEn_MR1                      :  1;\
            UNSG32 uCTRL3_pixlineTot_MR1                       : 13;\
            UNSG32 uCTRL3_flushCnt_MR1                         :  4;\
            UNSG32 RSVDx1C_b18                                 : 14;\
          }
    union { UNSG32 u32ClientIF_CTRL3;
            struct w32ClientIF_CTRL3;
          };
              SIE_ReadClient                                   ie_RdClientVmxVm;
    } SIE_ClientIF;
    typedef union  T32ClientIF_MR0
          { UNSG32 u32;
            struct w32ClientIF_MR0;
                 } T32ClientIF_MR0;
    typedef union  T32ClientIF_MR0_word
          { UNSG32 u32;
            struct w32ClientIF_MR0_word;
                 } T32ClientIF_MR0_word;
    typedef union  T32ClientIF_MR1
          { UNSG32 u32;
            struct w32ClientIF_MR1;
                 } T32ClientIF_MR1;
    typedef union  T32ClientIF_MR1_word
          { UNSG32 u32;
            struct w32ClientIF_MR1_word;
                 } T32ClientIF_MR1_word;
    typedef union  T32ClientIF_CTRL0
          { UNSG32 u32;
            struct w32ClientIF_CTRL0;
                 } T32ClientIF_CTRL0;
    typedef union  T32ClientIF_DUMMY
          { UNSG32 u32;
            struct w32ClientIF_DUMMY;
                 } T32ClientIF_DUMMY;
    typedef union  T32ClientIF_CTRL2
          { UNSG32 u32;
            struct w32ClientIF_CTRL2;
                 } T32ClientIF_CTRL2;
    typedef union  T32ClientIF_CTRL3
          { UNSG32 u32;
            struct w32ClientIF_CTRL3;
                 } T32ClientIF_CTRL3;
    typedef union  TClientIF_MR0
          { UNSG32 u32[1];
            struct {
            struct w32ClientIF_MR0;
                   };
                 } TClientIF_MR0;
    typedef union  TClientIF_MR0_word
          { UNSG32 u32[1];
            struct {
            struct w32ClientIF_MR0_word;
                   };
                 } TClientIF_MR0_word;
    typedef union  TClientIF_MR1
          { UNSG32 u32[1];
            struct {
            struct w32ClientIF_MR1;
                   };
                 } TClientIF_MR1;
    typedef union  TClientIF_MR1_word
          { UNSG32 u32[1];
            struct {
            struct w32ClientIF_MR1_word;
                   };
                 } TClientIF_MR1_word;
    typedef union  TClientIF_CTRL0
          { UNSG32 u32[1];
            struct {
            struct w32ClientIF_CTRL0;
                   };
                 } TClientIF_CTRL0;
    typedef union  TClientIF_DUMMY
          { UNSG32 u32[1];
            struct {
            struct w32ClientIF_DUMMY;
                   };
                 } TClientIF_DUMMY;
    typedef union  TClientIF_CTRL2
          { UNSG32 u32[1];
            struct {
            struct w32ClientIF_CTRL2;
                   };
                 } TClientIF_CTRL2;
    typedef union  TClientIF_CTRL3
          { UNSG32 u32[1];
            struct {
            struct w32ClientIF_CTRL3;
                   };
                 } TClientIF_CTRL3;
     SIGN32 ClientIF_drvrd(SIE_ClientIF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 ClientIF_drvwr(SIE_ClientIF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void ClientIF_reset(SIE_ClientIF *p);
     SIGN32 ClientIF_cmp  (SIE_ClientIF *p, SIE_ClientIF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define ClientIF_check(p,pie,pfx,hLOG) ClientIF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define ClientIF_print(p,    pfx,hLOG) ClientIF_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_BETG
#define h_BETG (){}
    #define     RA_BETG_PL0                                    0x0000
    #define     RA_BETG_PL1                                    0x0008
    #define     RA_BETG_PL2                                    0x0010
    #define     RA_BETG_PL3                                    0x0018
    #define     RA_BETG_PL4                                    0x0020
    #define     RA_BETG_PL5                                    0x0028
    #define     RA_BETG_PL6                                    0x0030
    #define     RA_BETG_PL7                                    0x0038
    #define     RA_BETG_PL8                                    0x0040
    #define     RA_BETG_PL1_CR                                 0x0048
    #define     RA_BETG_PL2_CR                                 0x0050
    #define     RA_BETG_PL3_CR                                 0x0058
    #define     RA_BETG_PL4_CR                                 0x0060
    #define     RA_BETG_PL5_CR                                 0x0068
    #define     RA_BETG_PL6_CR                                 0x0070
    #define     RA_BETG_PL7_CR                                 0x0078
    #define     RA_BETG_PL8_CR                                 0x0080
    #define     RA_BETG_PL_FLD                                 0x0088
    #define     RA_BETG_TG_PRG                                 0x0090
    typedef struct SIE_BETG {
              SIE_TG_PL                                        ie_PL0;
              SIE_TG_PL                                        ie_PL1;
              SIE_TG_PL                                        ie_PL2;
              SIE_TG_PL                                        ie_PL3;
              SIE_TG_PL                                        ie_PL4;
              SIE_TG_PL                                        ie_PL5;
              SIE_TG_PL                                        ie_PL6;
              SIE_TG_PL                                        ie_PL7;
              SIE_TG_PL                                        ie_PL8;
              SIE_TG_PL                                        ie_PL1_CR;
              SIE_TG_PL                                        ie_PL2_CR;
              SIE_TG_PL                                        ie_PL3_CR;
              SIE_TG_PL                                        ie_PL4_CR;
              SIE_TG_PL                                        ie_PL5_CR;
              SIE_TG_PL                                        ie_PL6_CR;
              SIE_TG_PL                                        ie_PL7_CR;
              SIE_TG_PL                                        ie_PL8_CR;
              SIE_TG_PL                                        ie_PL_FLD;
              SIE_TG_PRG                                       ie_TG_PRG;
    } SIE_BETG;
     SIGN32 BETG_drvrd(SIE_BETG *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BETG_drvwr(SIE_BETG *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BETG_reset(SIE_BETG *p);
     SIGN32 BETG_cmp  (SIE_BETG *p, SIE_BETG *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BETG_check(p,pie,pfx,hLOG) BETG_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BETG_print(p,    pfx,hLOG) BETG_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_DNS444_422
#define h_DNS444_422 (){}
    #define     RA_DNS444_422_CFG0                             0x0000
    #define   LSb32DNS444_422_CFG0_c4_14_11tap_regs               0
    #define       bDNS444_422_CFG0_c4_14_11tap_regs            13
    #define   MSK32DNS444_422_CFG0_c4_14_11tap_regs               0x00001FFF
    #define   LSb32DNS444_422_CFG0_c5_13_11tap_regs               13
    #define       bDNS444_422_CFG0_c5_13_11tap_regs            13
    #define   MSK32DNS444_422_CFG0_c5_13_11tap_regs               0x03FFE000
    #define     RA_DNS444_422_CFG1                             0x0004
    #define   LSb32DNS444_422_CFG1_c6_12_11tap_regs               0
    #define       bDNS444_422_CFG1_c6_12_11tap_regs            13
    #define   MSK32DNS444_422_CFG1_c6_12_11tap_regs               0x00001FFF
    #define   LSb32DNS444_422_CFG1_c7_11_11tap_regs               13
    #define       bDNS444_422_CFG1_c7_11_11tap_regs            13
    #define   MSK32DNS444_422_CFG1_c7_11_11tap_regs               0x03FFE000
    #define     RA_DNS444_422_CFG2                             0x0008
    #define   LSb32DNS444_422_CFG2_c8_10_11tap_regs               0
    #define       bDNS444_422_CFG2_c8_10_11tap_regs            13
    #define   MSK32DNS444_422_CFG2_c8_10_11tap_regs               0x00001FFF
    #define   LSb32DNS444_422_CFG2_c9_11tap_regs                  13
    #define       bDNS444_422_CFG2_c9_11tap_regs               13
    #define   MSK32DNS444_422_CFG2_c9_11tap_regs                  0x03FFE000
    #define     RA_DNS444_422_CFG3                             0x000C
    #define   LSb32DNS444_422_CFG3_c6_12_7tap_regs                0
    #define       bDNS444_422_CFG3_c6_12_7tap_regs             13
    #define   MSK32DNS444_422_CFG3_c6_12_7tap_regs                0x00001FFF
    #define   LSb32DNS444_422_CFG3_c7_11_7tap_regs                13
    #define       bDNS444_422_CFG3_c7_11_7tap_regs             13
    #define   MSK32DNS444_422_CFG3_c7_11_7tap_regs                0x03FFE000
    #define     RA_DNS444_422_CFG4                             0x0010
    #define   LSb32DNS444_422_CFG4_c8_10_7tap_regs                0
    #define       bDNS444_422_CFG4_c8_10_7tap_regs             13
    #define   MSK32DNS444_422_CFG4_c8_10_7tap_regs                0x00001FFF
    #define   LSb32DNS444_422_CFG4_c9_7tap_regs                   13
    #define       bDNS444_422_CFG4_c9_7tap_regs                13
    #define   MSK32DNS444_422_CFG4_c9_7tap_regs                   0x03FFE000
    #define     RA_DNS444_422_CFG5                             0x0014
    #define   LSb32DNS444_422_CFG5_edge_thresh_y                  0
    #define       bDNS444_422_CFG5_edge_thresh_y               8
    #define   MSK32DNS444_422_CFG5_edge_thresh_y                  0x000000FF
    #define   LSb32DNS444_422_CFG5_edge_thresh_c                  8
    #define       bDNS444_422_CFG5_edge_thresh_c               8
    #define   MSK32DNS444_422_CFG5_edge_thresh_c                  0x0000FF00
    #define   LSb32DNS444_422_CFG5_mode_regs                      16
    #define       bDNS444_422_CFG5_mode_regs                   2
    #define   MSK32DNS444_422_CFG5_mode_regs                      0x00030000
    #define     RA_DNS444_422_CFG6                             0x0018
    #define   LSb32DNS444_422_CFG6_yblank                         0
    #define       bDNS444_422_CFG6_yblank                      12
    #define   MSK32DNS444_422_CFG6_yblank                         0x00000FFF
    #define   LSb32DNS444_422_CFG6_cblank                         12
    #define       bDNS444_422_CFG6_cblank                      12
    #define   MSK32DNS444_422_CFG6_cblank                         0x00FFF000
    #define   LSb32DNS444_422_CFG6_use_blank_regs                 24
    #define       bDNS444_422_CFG6_use_blank_regs              1
    #define   MSK32DNS444_422_CFG6_use_blank_regs                 0x01000000
    typedef struct SIE_DNS444_422 {
    #define     w32DNS444_422_CFG0                             {\
            UNSG32 mCFG0_c4_14_11tap_regs                      : 13;\
            UNSG32 mCFG0_c5_13_11tap_regs                      : 13;\
            UNSG32 RSVDx0_b26                                  :  6;\
          }
    union { UNSG32 u32DNS444_422_CFG0;
            struct w32DNS444_422_CFG0;
          };
    #define     w32DNS444_422_CFG1                             {\
            UNSG32 mCFG1_c6_12_11tap_regs                      : 13;\
            UNSG32 mCFG1_c7_11_11tap_regs                      : 13;\
            UNSG32 RSVDx4_b26                                  :  6;\
          }
    union { UNSG32 u32DNS444_422_CFG1;
            struct w32DNS444_422_CFG1;
          };
    #define     w32DNS444_422_CFG2                             {\
            UNSG32 mCFG2_c8_10_11tap_regs                      : 13;\
            UNSG32 mCFG2_c9_11tap_regs                         : 13;\
            UNSG32 RSVDx8_b26                                  :  6;\
          }
    union { UNSG32 u32DNS444_422_CFG2;
            struct w32DNS444_422_CFG2;
          };
    #define     w32DNS444_422_CFG3                             {\
            UNSG32 mCFG3_c6_12_7tap_regs                       : 13;\
            UNSG32 mCFG3_c7_11_7tap_regs                       : 13;\
            UNSG32 RSVDxC_b26                                  :  6;\
          }
    union { UNSG32 u32DNS444_422_CFG3;
            struct w32DNS444_422_CFG3;
          };
    #define     w32DNS444_422_CFG4                             {\
            UNSG32 mCFG4_c8_10_7tap_regs                       : 13;\
            UNSG32 mCFG4_c9_7tap_regs                          : 13;\
            UNSG32 RSVDx10_b26                                 :  6;\
          }
    union { UNSG32 u32DNS444_422_CFG4;
            struct w32DNS444_422_CFG4;
          };
    #define     w32DNS444_422_CFG5                             {\
            UNSG32 uCFG5_edge_thresh_y                         :  8;\
            UNSG32 uCFG5_edge_thresh_c                         :  8;\
            UNSG32 uCFG5_mode_regs                             :  2;\
            UNSG32 RSVDx14_b18                                 : 14;\
          }
    union { UNSG32 u32DNS444_422_CFG5;
            struct w32DNS444_422_CFG5;
          };
    #define     w32DNS444_422_CFG6                             {\
            UNSG32 uCFG6_yblank                                : 12;\
            UNSG32 uCFG6_cblank                                : 12;\
            UNSG32 uCFG6_use_blank_regs                        :  1;\
            UNSG32 RSVDx18_b25                                 :  7;\
          }
    union { UNSG32 u32DNS444_422_CFG6;
            struct w32DNS444_422_CFG6;
          };
    } SIE_DNS444_422;
    typedef union  T32DNS444_422_CFG0
          { UNSG32 u32;
            struct w32DNS444_422_CFG0;
                 } T32DNS444_422_CFG0;
    typedef union  T32DNS444_422_CFG1
          { UNSG32 u32;
            struct w32DNS444_422_CFG1;
                 } T32DNS444_422_CFG1;
    typedef union  T32DNS444_422_CFG2
          { UNSG32 u32;
            struct w32DNS444_422_CFG2;
                 } T32DNS444_422_CFG2;
    typedef union  T32DNS444_422_CFG3
          { UNSG32 u32;
            struct w32DNS444_422_CFG3;
                 } T32DNS444_422_CFG3;
    typedef union  T32DNS444_422_CFG4
          { UNSG32 u32;
            struct w32DNS444_422_CFG4;
                 } T32DNS444_422_CFG4;
    typedef union  T32DNS444_422_CFG5
          { UNSG32 u32;
            struct w32DNS444_422_CFG5;
                 } T32DNS444_422_CFG5;
    typedef union  T32DNS444_422_CFG6
          { UNSG32 u32;
            struct w32DNS444_422_CFG6;
                 } T32DNS444_422_CFG6;
    typedef union  TDNS444_422_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32DNS444_422_CFG0;
                   };
                 } TDNS444_422_CFG0;
    typedef union  TDNS444_422_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32DNS444_422_CFG1;
                   };
                 } TDNS444_422_CFG1;
    typedef union  TDNS444_422_CFG2
          { UNSG32 u32[1];
            struct {
            struct w32DNS444_422_CFG2;
                   };
                 } TDNS444_422_CFG2;
    typedef union  TDNS444_422_CFG3
          { UNSG32 u32[1];
            struct {
            struct w32DNS444_422_CFG3;
                   };
                 } TDNS444_422_CFG3;
    typedef union  TDNS444_422_CFG4
          { UNSG32 u32[1];
            struct {
            struct w32DNS444_422_CFG4;
                   };
                 } TDNS444_422_CFG4;
    typedef union  TDNS444_422_CFG5
          { UNSG32 u32[1];
            struct {
            struct w32DNS444_422_CFG5;
                   };
                 } TDNS444_422_CFG5;
    typedef union  TDNS444_422_CFG6
          { UNSG32 u32[1];
            struct {
            struct w32DNS444_422_CFG6;
                   };
                 } TDNS444_422_CFG6;
     SIGN32 DNS444_422_drvrd(SIE_DNS444_422 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DNS444_422_drvwr(SIE_DNS444_422 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DNS444_422_reset(SIE_DNS444_422 *p);
     SIGN32 DNS444_422_cmp  (SIE_DNS444_422 *p, SIE_DNS444_422 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DNS444_422_check(p,pie,pfx,hLOG) DNS444_422_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DNS444_422_print(p,    pfx,hLOG) DNS444_422_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_CSC_C17O24
#define h_CSC_C17O24 (){}
    #define     RA_CSC_C17O24_CFG0                             0x0000
    #define   LSb32CSC_C17O24_CFG0_C0                             0
    #define       bCSC_C17O24_CFG0_C0                          17
    #define   MSK32CSC_C17O24_CFG0_C0                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG1                             0x0004
    #define   LSb32CSC_C17O24_CFG1_C1                             0
    #define       bCSC_C17O24_CFG1_C1                          17
    #define   MSK32CSC_C17O24_CFG1_C1                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG2                             0x0008
    #define   LSb32CSC_C17O24_CFG2_C2                             0
    #define       bCSC_C17O24_CFG2_C2                          17
    #define   MSK32CSC_C17O24_CFG2_C2                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG3                             0x000C
    #define   LSb32CSC_C17O24_CFG3_C3                             0
    #define       bCSC_C17O24_CFG3_C3                          17
    #define   MSK32CSC_C17O24_CFG3_C3                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG4                             0x0010
    #define   LSb32CSC_C17O24_CFG4_C4                             0
    #define       bCSC_C17O24_CFG4_C4                          17
    #define   MSK32CSC_C17O24_CFG4_C4                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG5                             0x0014
    #define   LSb32CSC_C17O24_CFG5_C5                             0
    #define       bCSC_C17O24_CFG5_C5                          17
    #define   MSK32CSC_C17O24_CFG5_C5                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG6                             0x0018
    #define   LSb32CSC_C17O24_CFG6_C6                             0
    #define       bCSC_C17O24_CFG6_C6                          17
    #define   MSK32CSC_C17O24_CFG6_C6                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG7                             0x001C
    #define   LSb32CSC_C17O24_CFG7_C7                             0
    #define       bCSC_C17O24_CFG7_C7                          17
    #define   MSK32CSC_C17O24_CFG7_C7                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG8                             0x0020
    #define   LSb32CSC_C17O24_CFG8_C8                             0
    #define       bCSC_C17O24_CFG8_C8                          17
    #define   MSK32CSC_C17O24_CFG8_C8                             0x0001FFFF
    #define     RA_CSC_C17O24_CFG9                             0x0024
    #define   LSb32CSC_C17O24_CFG9_OFF1                           0
    #define       bCSC_C17O24_CFG9_OFF1                        24
    #define   MSK32CSC_C17O24_CFG9_OFF1                           0x00FFFFFF
    #define     RA_CSC_C17O24_CFG10                            0x0028
    #define   LSb32CSC_C17O24_CFG10_OFF2                          0
    #define       bCSC_C17O24_CFG10_OFF2                       24
    #define   MSK32CSC_C17O24_CFG10_OFF2                          0x00FFFFFF
    #define     RA_CSC_C17O24_CFG11                            0x002C
    #define   LSb32CSC_C17O24_CFG11_OFF3                          0
    #define       bCSC_C17O24_CFG11_OFF3                       24
    #define   MSK32CSC_C17O24_CFG11_OFF3                          0x00FFFFFF
    #define     RA_CSC_C17O24_CFG12                            0x0030
    #define   LSb32CSC_C17O24_CFG12_CL1MIN                        0
    #define       bCSC_C17O24_CFG12_CL1MIN                     12
    #define   MSK32CSC_C17O24_CFG12_CL1MIN                        0x00000FFF
    #define   LSb32CSC_C17O24_CFG12_CL1MAX                        12
    #define       bCSC_C17O24_CFG12_CL1MAX                     12
    #define   MSK32CSC_C17O24_CFG12_CL1MAX                        0x00FFF000
    #define     RA_CSC_C17O24_CFG13                            0x0034
    #define   LSb32CSC_C17O24_CFG13_CL2MIN                        0
    #define       bCSC_C17O24_CFG13_CL2MIN                     12
    #define   MSK32CSC_C17O24_CFG13_CL2MIN                        0x00000FFF
    #define   LSb32CSC_C17O24_CFG13_CL2MAX                        12
    #define       bCSC_C17O24_CFG13_CL2MAX                     12
    #define   MSK32CSC_C17O24_CFG13_CL2MAX                        0x00FFF000
    #define     RA_CSC_C17O24_CFG14                            0x0038
    #define   LSb32CSC_C17O24_CFG14_CL3MIN                        0
    #define       bCSC_C17O24_CFG14_CL3MIN                     12
    #define   MSK32CSC_C17O24_CFG14_CL3MIN                        0x00000FFF
    #define   LSb32CSC_C17O24_CFG14_CL3MAX                        12
    #define       bCSC_C17O24_CFG14_CL3MAX                     12
    #define   MSK32CSC_C17O24_CFG14_CL3MAX                        0x00FFF000
    typedef struct SIE_CSC_C17O24 {
    #define     w32CSC_C17O24_CFG0                             {\
            UNSG32 mCFG0_C0                                    : 17;\
            UNSG32 RSVDx0_b17                                  : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG0;
            struct w32CSC_C17O24_CFG0;
          };
    #define     w32CSC_C17O24_CFG1                             {\
            UNSG32 mCFG1_C1                                    : 17;\
            UNSG32 RSVDx4_b17                                  : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG1;
            struct w32CSC_C17O24_CFG1;
          };
    #define     w32CSC_C17O24_CFG2                             {\
            UNSG32 mCFG2_C2                                    : 17;\
            UNSG32 RSVDx8_b17                                  : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG2;
            struct w32CSC_C17O24_CFG2;
          };
    #define     w32CSC_C17O24_CFG3                             {\
            UNSG32 mCFG3_C3                                    : 17;\
            UNSG32 RSVDxC_b17                                  : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG3;
            struct w32CSC_C17O24_CFG3;
          };
    #define     w32CSC_C17O24_CFG4                             {\
            UNSG32 mCFG4_C4                                    : 17;\
            UNSG32 RSVDx10_b17                                 : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG4;
            struct w32CSC_C17O24_CFG4;
          };
    #define     w32CSC_C17O24_CFG5                             {\
            UNSG32 mCFG5_C5                                    : 17;\
            UNSG32 RSVDx14_b17                                 : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG5;
            struct w32CSC_C17O24_CFG5;
          };
    #define     w32CSC_C17O24_CFG6                             {\
            UNSG32 mCFG6_C6                                    : 17;\
            UNSG32 RSVDx18_b17                                 : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG6;
            struct w32CSC_C17O24_CFG6;
          };
    #define     w32CSC_C17O24_CFG7                             {\
            UNSG32 mCFG7_C7                                    : 17;\
            UNSG32 RSVDx1C_b17                                 : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG7;
            struct w32CSC_C17O24_CFG7;
          };
    #define     w32CSC_C17O24_CFG8                             {\
            UNSG32 mCFG8_C8                                    : 17;\
            UNSG32 RSVDx20_b17                                 : 15;\
          }
    union { UNSG32 u32CSC_C17O24_CFG8;
            struct w32CSC_C17O24_CFG8;
          };
    #define     w32CSC_C17O24_CFG9                             {\
            UNSG32 mCFG9_OFF1                                  : 24;\
            UNSG32 RSVDx24_b24                                 :  8;\
          }
    union { UNSG32 u32CSC_C17O24_CFG9;
            struct w32CSC_C17O24_CFG9;
          };
    #define     w32CSC_C17O24_CFG10                            {\
            UNSG32 mCFG10_OFF2                                 : 24;\
            UNSG32 RSVDx28_b24                                 :  8;\
          }
    union { UNSG32 u32CSC_C17O24_CFG10;
            struct w32CSC_C17O24_CFG10;
          };
    #define     w32CSC_C17O24_CFG11                            {\
            UNSG32 mCFG11_OFF3                                 : 24;\
            UNSG32 RSVDx2C_b24                                 :  8;\
          }
    union { UNSG32 u32CSC_C17O24_CFG11;
            struct w32CSC_C17O24_CFG11;
          };
    #define     w32CSC_C17O24_CFG12                            {\
            UNSG32 uCFG12_CL1MIN                               : 12;\
            UNSG32 uCFG12_CL1MAX                               : 12;\
            UNSG32 RSVDx30_b24                                 :  8;\
          }
    union { UNSG32 u32CSC_C17O24_CFG12;
            struct w32CSC_C17O24_CFG12;
          };
    #define     w32CSC_C17O24_CFG13                            {\
            UNSG32 uCFG13_CL2MIN                               : 12;\
            UNSG32 uCFG13_CL2MAX                               : 12;\
            UNSG32 RSVDx34_b24                                 :  8;\
          }
    union { UNSG32 u32CSC_C17O24_CFG13;
            struct w32CSC_C17O24_CFG13;
          };
    #define     w32CSC_C17O24_CFG14                            {\
            UNSG32 uCFG14_CL3MIN                               : 12;\
            UNSG32 uCFG14_CL3MAX                               : 12;\
            UNSG32 RSVDx38_b24                                 :  8;\
          }
    union { UNSG32 u32CSC_C17O24_CFG14;
            struct w32CSC_C17O24_CFG14;
          };
    } SIE_CSC_C17O24;
    typedef union  T32CSC_C17O24_CFG0
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG0;
                 } T32CSC_C17O24_CFG0;
    typedef union  T32CSC_C17O24_CFG1
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG1;
                 } T32CSC_C17O24_CFG1;
    typedef union  T32CSC_C17O24_CFG2
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG2;
                 } T32CSC_C17O24_CFG2;
    typedef union  T32CSC_C17O24_CFG3
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG3;
                 } T32CSC_C17O24_CFG3;
    typedef union  T32CSC_C17O24_CFG4
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG4;
                 } T32CSC_C17O24_CFG4;
    typedef union  T32CSC_C17O24_CFG5
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG5;
                 } T32CSC_C17O24_CFG5;
    typedef union  T32CSC_C17O24_CFG6
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG6;
                 } T32CSC_C17O24_CFG6;
    typedef union  T32CSC_C17O24_CFG7
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG7;
                 } T32CSC_C17O24_CFG7;
    typedef union  T32CSC_C17O24_CFG8
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG8;
                 } T32CSC_C17O24_CFG8;
    typedef union  T32CSC_C17O24_CFG9
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG9;
                 } T32CSC_C17O24_CFG9;
    typedef union  T32CSC_C17O24_CFG10
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG10;
                 } T32CSC_C17O24_CFG10;
    typedef union  T32CSC_C17O24_CFG11
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG11;
                 } T32CSC_C17O24_CFG11;
    typedef union  T32CSC_C17O24_CFG12
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG12;
                 } T32CSC_C17O24_CFG12;
    typedef union  T32CSC_C17O24_CFG13
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG13;
                 } T32CSC_C17O24_CFG13;
    typedef union  T32CSC_C17O24_CFG14
          { UNSG32 u32;
            struct w32CSC_C17O24_CFG14;
                 } T32CSC_C17O24_CFG14;
    typedef union  TCSC_C17O24_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG0;
                   };
                 } TCSC_C17O24_CFG0;
    typedef union  TCSC_C17O24_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG1;
                   };
                 } TCSC_C17O24_CFG1;
    typedef union  TCSC_C17O24_CFG2
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG2;
                   };
                 } TCSC_C17O24_CFG2;
    typedef union  TCSC_C17O24_CFG3
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG3;
                   };
                 } TCSC_C17O24_CFG3;
    typedef union  TCSC_C17O24_CFG4
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG4;
                   };
                 } TCSC_C17O24_CFG4;
    typedef union  TCSC_C17O24_CFG5
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG5;
                   };
                 } TCSC_C17O24_CFG5;
    typedef union  TCSC_C17O24_CFG6
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG6;
                   };
                 } TCSC_C17O24_CFG6;
    typedef union  TCSC_C17O24_CFG7
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG7;
                   };
                 } TCSC_C17O24_CFG7;
    typedef union  TCSC_C17O24_CFG8
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG8;
                   };
                 } TCSC_C17O24_CFG8;
    typedef union  TCSC_C17O24_CFG9
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG9;
                   };
                 } TCSC_C17O24_CFG9;
    typedef union  TCSC_C17O24_CFG10
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG10;
                   };
                 } TCSC_C17O24_CFG10;
    typedef union  TCSC_C17O24_CFG11
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG11;
                   };
                 } TCSC_C17O24_CFG11;
    typedef union  TCSC_C17O24_CFG12
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG12;
                   };
                 } TCSC_C17O24_CFG12;
    typedef union  TCSC_C17O24_CFG13
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG13;
                   };
                 } TCSC_C17O24_CFG13;
    typedef union  TCSC_C17O24_CFG14
          { UNSG32 u32[1];
            struct {
            struct w32CSC_C17O24_CFG14;
                   };
                 } TCSC_C17O24_CFG14;
     SIGN32 CSC_C17O24_drvrd(SIE_CSC_C17O24 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 CSC_C17O24_drvwr(SIE_CSC_C17O24 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void CSC_C17O24_reset(SIE_CSC_C17O24 *p);
     SIGN32 CSC_C17O24_cmp  (SIE_CSC_C17O24 *p, SIE_CSC_C17O24 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define CSC_C17O24_check(p,pie,pfx,hLOG) CSC_C17O24_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define CSC_C17O24_print(p,    pfx,hLOG) CSC_C17O24_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_DNS422_420
#define h_DNS422_420 (){}
    #define     RA_DNS422_420_CFG0                             0x0000
    #define   LSb32DNS422_420_CFG0_dns422_420_en                  0
    #define       bDNS422_420_CFG0_dns422_420_en               1
    #define   MSK32DNS422_420_CFG0_dns422_420_en                  0x00000001
    #define   LSb32DNS422_420_CFG0_dns422_420_msb_swap            1
    #define       bDNS422_420_CFG0_dns422_420_msb_swap         1
    #define   MSK32DNS422_420_CFG0_dns422_420_msb_swap            0x00000002
    #define   LSb32DNS422_420_CFG0_sp_en                          2
    #define       bDNS422_420_CFG0_sp_en                       1
    #define   MSK32DNS422_420_CFG0_sp_en                          0x00000004
    #define   LSb32DNS422_420_CFG0_dns422_420_auto_pixcnt         3
    #define       bDNS422_420_CFG0_dns422_420_auto_pixcnt      1
    #define   MSK32DNS422_420_CFG0_dns422_420_auto_pixcnt         0x00000008
    #define   LSb32DNS422_420_CFG0_dns422_420_hres                4
    #define       bDNS422_420_CFG0_dns422_420_hres             13
    #define   MSK32DNS422_420_CFG0_dns422_420_hres                0x0001FFF0
    #define   LSb32DNS422_420_CFG0_dns422_420_htot                17
    #define       bDNS422_420_CFG0_dns422_420_htot             13
    #define   MSK32DNS422_420_CFG0_dns422_420_htot                0x3FFE0000
    #define   LSb32DNS422_420_CFG0_outdata_mode                   30
    #define       bDNS422_420_CFG0_outdata_mode                2
    #define   MSK32DNS422_420_CFG0_outdata_mode                   0xC0000000
    #define     RA_DNS422_420_CFG1                             0x0004
    #define   LSb32DNS422_420_CFG1_PDWN                           0
    #define       bDNS422_420_CFG1_PDWN                        1
    #define   MSK32DNS422_420_CFG1_PDWN                           0x00000001
    #define   LSb32DNS422_420_CFG1_PDLVMC                         1
    #define       bDNS422_420_CFG1_PDLVMC                      1
    #define   MSK32DNS422_420_CFG1_PDLVMC                         0x00000002
    #define   LSb32DNS422_420_CFG1_PDFVSSM                        2
    #define       bDNS422_420_CFG1_PDFVSSM                     1
    #define   MSK32DNS422_420_CFG1_PDFVSSM                        0x00000004
    typedef struct SIE_DNS422_420 {
    #define     w32DNS422_420_CFG0                             {\
            UNSG32 uCFG0_dns422_420_en                         :  1;\
            UNSG32 uCFG0_dns422_420_msb_swap                   :  1;\
            UNSG32 uCFG0_sp_en                                 :  1;\
            UNSG32 uCFG0_dns422_420_auto_pixcnt                :  1;\
            UNSG32 uCFG0_dns422_420_hres                       : 13;\
            UNSG32 uCFG0_dns422_420_htot                       : 13;\
            UNSG32 uCFG0_outdata_mode                          :  2;\
          }
    union { UNSG32 u32DNS422_420_CFG0;
            struct w32DNS422_420_CFG0;
          };
    #define     w32DNS422_420_CFG1                             {\
            UNSG32 uCFG1_PDWN                                  :  1;\
            UNSG32 uCFG1_PDLVMC                                :  1;\
            UNSG32 uCFG1_PDFVSSM                               :  1;\
            UNSG32 RSVDx4_b3                                   : 29;\
          }
    union { UNSG32 u32DNS422_420_CFG1;
            struct w32DNS422_420_CFG1;
          };
    } SIE_DNS422_420;
    typedef union  T32DNS422_420_CFG0
          { UNSG32 u32;
            struct w32DNS422_420_CFG0;
                 } T32DNS422_420_CFG0;
    typedef union  T32DNS422_420_CFG1
          { UNSG32 u32;
            struct w32DNS422_420_CFG1;
                 } T32DNS422_420_CFG1;
    typedef union  TDNS422_420_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32DNS422_420_CFG0;
                   };
                 } TDNS422_420_CFG0;
    typedef union  TDNS422_420_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32DNS422_420_CFG1;
                   };
                 } TDNS422_420_CFG1;
     SIGN32 DNS422_420_drvrd(SIE_DNS422_420 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DNS422_420_drvwr(SIE_DNS422_420 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DNS422_420_reset(SIE_DNS422_420 *p);
     SIGN32 DNS422_420_cmp  (SIE_DNS422_420 *p, SIE_DNS422_420 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DNS422_420_check(p,pie,pfx,hLOG) DNS422_420_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DNS422_420_print(p,    pfx,hLOG) DNS422_420_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_CSCDNS
#define h_CSCDNS (){}
    #define     RA_CSCDNS_TG                                   0x0000
    #define     RA_CSCDNS_CSC                                  0x0040
    #define     RA_CSCDNS_DNS444_422                           0x007C
    #define     RA_CSCDNS_DNS422_420                           0x0098
    typedef struct SIE_CSCDNS {
              SIE_TG                                           ie_TG;
              SIE_CSC_C17O24                                   ie_CSC;
              SIE_DNS444_422                                   ie_DNS444_422;
              SIE_DNS422_420                                   ie_DNS422_420;
    } SIE_CSCDNS;
     SIGN32 CSCDNS_drvrd(SIE_CSCDNS *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 CSCDNS_drvwr(SIE_CSCDNS *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void CSCDNS_reset(SIE_CSCDNS *p);
     SIGN32 CSCDNS_cmp  (SIE_CSCDNS *p, SIE_CSCDNS *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define CSCDNS_check(p,pie,pfx,hLOG) CSCDNS_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define CSCDNS_print(p,    pfx,hLOG) CSCDNS_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_WB
#define h_WB (){}
    #define     RA_WB_CTRL                                     0x0000
    #define   LSb32WB_CTRL_mode                                   0
    #define       bWB_CTRL_mode                                3
    #define   MSK32WB_CTRL_mode                                   0x00000007
    #define   LSb32WB_CTRL_enable                                 3
    #define       bWB_CTRL_enable                              1
    #define   MSK32WB_CTRL_enable                                 0x00000008
    #define   LSb32WB_CTRL_p00_man                                4
    #define       bWB_CTRL_p00_man                             6
    #define   MSK32WB_CTRL_p00_man                                0x000003F0
    #define   LSb32WB_CTRL_p00_exp                                10
    #define       bWB_CTRL_p00_exp                             2
    #define   MSK32WB_CTRL_p00_exp                                0x00000C00
    #define   LSb32WB_CTRL_p01_man                                12
    #define       bWB_CTRL_p01_man                             6
    #define   MSK32WB_CTRL_p01_man                                0x0003F000
    #define   LSb32WB_CTRL_p01_exp                                18
    #define       bWB_CTRL_p01_exp                             2
    #define   MSK32WB_CTRL_p01_exp                                0x000C0000
    #define   LSb32WB_CTRL_enable_dyn                             20
    #define       bWB_CTRL_enable_dyn                          1
    #define   MSK32WB_CTRL_enable_dyn                             0x00100000
    #define     RA_WB_CTRL1                                    0x0004
    #define   LSb32WB_CTRL1_p10_man                               0
    #define       bWB_CTRL1_p10_man                            6
    #define   MSK32WB_CTRL1_p10_man                               0x0000003F
    #define   LSb32WB_CTRL1_p10_exp                               6
    #define       bWB_CTRL1_p10_exp                            2
    #define   MSK32WB_CTRL1_p10_exp                               0x000000C0
    #define   LSb32WB_CTRL1_p11_man                               8
    #define       bWB_CTRL1_p11_man                            6
    #define   MSK32WB_CTRL1_p11_man                               0x00003F00
    #define   LSb32WB_CTRL1_p11_exp                               14
    #define       bWB_CTRL1_p11_exp                            2
    #define   MSK32WB_CTRL1_p11_exp                               0x0000C000
    #define   LSb32WB_CTRL1_input_sel                             16
    #define       bWB_CTRL1_input_sel                          1
    #define   MSK32WB_CTRL1_input_sel                             0x00010000
    typedef struct SIE_WB {
    #define     w32WB_CTRL                                     {\
            UNSG32 uCTRL_mode                                  :  3;\
            UNSG32 uCTRL_enable                                :  1;\
            UNSG32 uCTRL_p00_man                               :  6;\
            UNSG32 uCTRL_p00_exp                               :  2;\
            UNSG32 uCTRL_p01_man                               :  6;\
            UNSG32 uCTRL_p01_exp                               :  2;\
            UNSG32 uCTRL_enable_dyn                            :  1;\
            UNSG32 RSVDx0_b21                                  : 11;\
          }
    union { UNSG32 u32WB_CTRL;
            struct w32WB_CTRL;
          };
    #define     w32WB_CTRL1                                    {\
            UNSG32 uCTRL1_p10_man                              :  6;\
            UNSG32 uCTRL1_p10_exp                              :  2;\
            UNSG32 uCTRL1_p11_man                              :  6;\
            UNSG32 uCTRL1_p11_exp                              :  2;\
            UNSG32 uCTRL1_input_sel                            :  1;\
            UNSG32 RSVDx4_b17                                  : 15;\
          }
    union { UNSG32 u32WB_CTRL1;
            struct w32WB_CTRL1;
          };
    } SIE_WB;
    typedef union  T32WB_CTRL
          { UNSG32 u32;
            struct w32WB_CTRL;
                 } T32WB_CTRL;
    typedef union  T32WB_CTRL1
          { UNSG32 u32;
            struct w32WB_CTRL1;
                 } T32WB_CTRL1;
    typedef union  TWB_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32WB_CTRL;
                   };
                 } TWB_CTRL;
    typedef union  TWB_CTRL1
          { UNSG32 u32[1];
            struct {
            struct w32WB_CTRL1;
                   };
                 } TWB_CTRL1;
     SIGN32 WB_drvrd(SIE_WB *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 WB_drvwr(SIE_WB *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void WB_reset(SIE_WB *p);
     SIGN32 WB_cmp  (SIE_WB *p, SIE_WB *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define WB_check(p,pie,pfx,hLOG) WB_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define WB_print(p,    pfx,hLOG) WB_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_FVF
#define h_FVF (){}
    #define     RA_FVF_FVF_CFG0                                0x0000
    #define   LSb32FVF_FVF_CFG0_FVF_ENABLE                        0
    #define       bFVF_FVF_CFG0_FVF_ENABLE                     1
    #define   MSK32FVF_FVF_CFG0_FVF_ENABLE                        0x00000001
    #define   LSb32FVF_FVF_CFG0_FVF_FRAME_WIDTH                   1
    #define       bFVF_FVF_CFG0_FVF_FRAME_WIDTH                11
    #define   MSK32FVF_FVF_CFG0_FVF_FRAME_WIDTH                   0x00000FFE
    #define   LSb32FVF_FVF_CFG0_FVF_FRAME_HEIGHT                  12
    #define       bFVF_FVF_CFG0_FVF_FRAME_HEIGHT               11
    #define   MSK32FVF_FVF_CFG0_FVF_FRAME_HEIGHT                  0x007FF000
    #define   LSb32FVF_FVF_CFG0_IGNORE_LINE_CTL                   23
    #define       bFVF_FVF_CFG0_IGNORE_LINE_CTL                1
    #define   MSK32FVF_FVF_CFG0_IGNORE_LINE_CTL                   0x00800000
    #define   LSb32FVF_FVF_CFG0_FRAME_COUNTERS_EN                 24
    #define       bFVF_FVF_CFG0_FRAME_COUNTERS_EN              1
    #define   MSK32FVF_FVF_CFG0_FRAME_COUNTERS_EN                 0x01000000
    #define     RA_FVF_FVF_CFG1                                0x0004
    #define   LSb32FVF_FVF_CFG1_MIN_FRAME_GAP                     0
    #define       bFVF_FVF_CFG1_MIN_FRAME_GAP                  16
    #define   MSK32FVF_FVF_CFG1_MIN_FRAME_GAP                     0x0000FFFF
    #define   LSb32FVF_FVF_CFG1_MIN_LINE_GAP                      16
    #define       bFVF_FVF_CFG1_MIN_LINE_GAP                   11
    #define   MSK32FVF_FVF_CFG1_MIN_LINE_GAP                      0x07FF0000
    #define   LSb32FVF_FVF_CFG1_HALT_EN                           27
    #define       bFVF_FVF_CFG1_HALT_EN                        1
    #define   MSK32FVF_FVF_CFG1_HALT_EN                           0x08000000
    #define   LSb32FVF_FVF_CFG1_HALT_PERIOD                       28
    #define       bFVF_FVF_CFG1_HALT_PERIOD                    4
    #define   MSK32FVF_FVF_CFG1_HALT_PERIOD                       0xF0000000
    #define     RA_FVF_FVF_MAX_FRM_CFG                         0x0008
    #define   LSb32FVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_EN      0
    #define       bFVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_EN   1
    #define   MSK32FVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_EN      0x00000001
    #define   LSb32FVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_VALUE    1
    #define       bFVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_VALUE 26
    #define   MSK32FVF_FVF_MAX_FRM_CFG_MAX_FRAME_DURATION_VALUE    0x07FFFFFE
    #define     RA_FVF_FVF_CNT_CLEAR                           0x000C
    #define   LSb32FVF_FVF_CNT_CLEAR_CLEAR_FRAME_COUNTERS         0
    #define       bFVF_FVF_CNT_CLEAR_CLEAR_FRAME_COUNTERS      1
    #define   MSK32FVF_FVF_CNT_CLEAR_CLEAR_FRAME_COUNTERS         0x00000001
    #define     RA_FVF_FVF_STATUS                              0x0010
    #define   LSb32FVF_FVF_STATUS_FVF_STATE                       0
    #define       bFVF_FVF_STATUS_FVF_STATE                    2
    #define   MSK32FVF_FVF_STATUS_FVF_STATE                       0x00000003
    #define     RA_FVF_FVF_COUNT                               0x0014
    #define   LSb32FVF_FVF_COUNT_FRAME_COUNT                      0
    #define       bFVF_FVF_COUNT_FRAME_COUNT                   16
    #define   MSK32FVF_FVF_COUNT_FRAME_COUNT                      0x0000FFFF
    #define   LSb32FVF_FVF_COUNT_PROCESSED_FRAME_COUNT            16
    #define       bFVF_FVF_COUNT_PROCESSED_FRAME_COUNT         16
    #define   MSK32FVF_FVF_COUNT_PROCESSED_FRAME_COUNT            0xFFFF0000
    #define     RA_FVF_FVF_STATUS1                             0x0018
    #define   LSb32FVF_FVF_STATUS1_WRONG_FRAME_WIDTH              0
    #define       bFVF_FVF_STATUS1_WRONG_FRAME_WIDTH           1
    #define   MSK32FVF_FVF_STATUS1_WRONG_FRAME_WIDTH              0x00000001
    #define   LSb32FVF_FVF_STATUS1_WRONG_FRAME_HEIGHT             1
    #define       bFVF_FVF_STATUS1_WRONG_FRAME_HEIGHT          1
    #define   MSK32FVF_FVF_STATUS1_WRONG_FRAME_HEIGHT             0x00000002
    #define   LSb32FVF_FVF_STATUS1_MIN_LINE_GAP_SUCCEEDED         2
    #define       bFVF_FVF_STATUS1_MIN_LINE_GAP_SUCCEEDED      1
    #define   MSK32FVF_FVF_STATUS1_MIN_LINE_GAP_SUCCEEDED         0x00000004
    #define   LSb32FVF_FVF_STATUS1_MIN_FRAME_GAP_SUCCEEDED        3
    #define       bFVF_FVF_STATUS1_MIN_FRAME_GAP_SUCCEEDED     1
    #define   MSK32FVF_FVF_STATUS1_MIN_FRAME_GAP_SUCCEEDED        0x00000008
    #define   LSb32FVF_FVF_STATUS1_MAX_DURATION_EXCEEDED          4
    #define       bFVF_FVF_STATUS1_MAX_DURATION_EXCEEDED       1
    #define   MSK32FVF_FVF_STATUS1_MAX_DURATION_EXCEEDED          0x00000010
    #define   LSb32FVF_FVF_STATUS1_FRAME_START_AFTER_START        5
    #define       bFVF_FVF_STATUS1_FRAME_START_AFTER_START     1
    #define   MSK32FVF_FVF_STATUS1_FRAME_START_AFTER_START        0x00000020
    #define   LSb32FVF_FVF_STATUS1_LINE_START_AFTER_START         6
    #define       bFVF_FVF_STATUS1_LINE_START_AFTER_START      1
    #define   MSK32FVF_FVF_STATUS1_LINE_START_AFTER_START         0x00000040
    #define   LSb32FVF_FVF_STATUS1_FRAME_END_AFTER_END            7
    #define       bFVF_FVF_STATUS1_FRAME_END_AFTER_END         1
    #define   MSK32FVF_FVF_STATUS1_FRAME_END_AFTER_END            0x00000080
    #define   LSb32FVF_FVF_STATUS1_LINE_END_AFTER_END             8
    #define       bFVF_FVF_STATUS1_LINE_END_AFTER_END          1
    #define   MSK32FVF_FVF_STATUS1_LINE_END_AFTER_END             0x00000100
    #define     RA_FVF_FVF_ERROR_EN_CFG                        0x001C
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_WRONG_FRAME_WIDTH_IND_EN    0
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_WRONG_FRAME_WIDTH_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_WRONG_FRAME_WIDTH_IND_EN    0x00000001
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_WRONG_FRAME_HEIGHT_IND_EN    1
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_WRONG_FRAME_HEIGHT_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_WRONG_FRAME_HEIGHT_IND_EN    0x00000002
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MIN_LINE_GAP_SUCCEEDED_IND_EN    2
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MIN_LINE_GAP_SUCCEEDED_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MIN_LINE_GAP_SUCCEEDED_IND_EN    0x00000004
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MIN_FRAME_GAP_SUCCEEDED_IND_EN    3
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MIN_FRAME_GAP_SUCCEEDED_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MIN_FRAME_GAP_SUCCEEDED_IND_EN    0x00000008
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MAX_DURATION_EXCEEDED_IND_EN    4
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MAX_DURATION_EXCEEDED_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_MAX_DURATION_EXCEEDED_IND_EN    0x00000010
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_FRAME_START_AFTER_START_IND_EN    5
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_FRAME_START_AFTER_START_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_FRAME_START_AFTER_START_IND_EN    0x00000020
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_LINE_START_AFTER_START_IND_EN    6
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_LINE_START_AFTER_START_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_LINE_START_AFTER_START_IND_EN    0x00000040
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_FRAME_END_AFTER_END_IND_EN    7
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_FRAME_END_AFTER_END_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_FRAME_END_AFTER_END_IND_EN    0x00000080
    #define   LSb32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_LINE_END_AFTER_END_IND_EN    8
    #define       bFVF_FVF_ERROR_EN_CFG_INVALID_FRAME_LINE_END_AFTER_END_IND_EN 1
    #define   MSK32FVF_FVF_ERROR_EN_CFG_INVALID_FRAME_LINE_END_AFTER_END_IND_EN    0x00000100
    typedef struct SIE_FVF {
    #define     w32FVF_FVF_CFG0                                {\
            UNSG32 uFVF_CFG0_FVF_ENABLE                        :  1;\
            UNSG32 uFVF_CFG0_FVF_FRAME_WIDTH                   : 11;\
            UNSG32 uFVF_CFG0_FVF_FRAME_HEIGHT                  : 11;\
            UNSG32 uFVF_CFG0_IGNORE_LINE_CTL                   :  1;\
            UNSG32 uFVF_CFG0_FRAME_COUNTERS_EN                 :  1;\
            UNSG32 RSVDx0_b25                                  :  7;\
          }
    union { UNSG32 u32FVF_FVF_CFG0;
            struct w32FVF_FVF_CFG0;
          };
    #define     w32FVF_FVF_CFG1                                {\
            UNSG32 uFVF_CFG1_MIN_FRAME_GAP                     : 16;\
            UNSG32 uFVF_CFG1_MIN_LINE_GAP                      : 11;\
            UNSG32 uFVF_CFG1_HALT_EN                           :  1;\
            UNSG32 uFVF_CFG1_HALT_PERIOD                       :  4;\
          }
    union { UNSG32 u32FVF_FVF_CFG1;
            struct w32FVF_FVF_CFG1;
          };
    #define     w32FVF_FVF_MAX_FRM_CFG                         {\
            UNSG32 uFVF_MAX_FRM_CFG_MAX_FRAME_DURATION_EN      :  1;\
            UNSG32 uFVF_MAX_FRM_CFG_MAX_FRAME_DURATION_VALUE   : 26;\
            UNSG32 RSVDx8_b27                                  :  5;\
          }
    union { UNSG32 u32FVF_FVF_MAX_FRM_CFG;
            struct w32FVF_FVF_MAX_FRM_CFG;
          };
    #define     w32FVF_FVF_CNT_CLEAR                           {\
            UNSG32 uFVF_CNT_CLEAR_CLEAR_FRAME_COUNTERS         :  1;\
            UNSG32 RSVDxC_b1                                   : 31;\
          }
    union { UNSG32 u32FVF_FVF_CNT_CLEAR;
            struct w32FVF_FVF_CNT_CLEAR;
          };
    #define     w32FVF_FVF_STATUS                              {\
            UNSG32 uFVF_STATUS_FVF_STATE                       :  2;\
            UNSG32 RSVDx10_b2                                  : 30;\
          }
    union { UNSG32 u32FVF_FVF_STATUS;
            struct w32FVF_FVF_STATUS;
          };
    #define     w32FVF_FVF_COUNT                               {\
            UNSG32 uFVF_COUNT_FRAME_COUNT                      : 16;\
            UNSG32 uFVF_COUNT_PROCESSED_FRAME_COUNT            : 16;\
          }
    union { UNSG32 u32FVF_FVF_COUNT;
            struct w32FVF_FVF_COUNT;
          };
    #define     w32FVF_FVF_STATUS1                             {\
            UNSG32 uFVF_STATUS1_WRONG_FRAME_WIDTH              :  1;\
            UNSG32 uFVF_STATUS1_WRONG_FRAME_HEIGHT             :  1;\
            UNSG32 uFVF_STATUS1_MIN_LINE_GAP_SUCCEEDED         :  1;\
            UNSG32 uFVF_STATUS1_MIN_FRAME_GAP_SUCCEEDED        :  1;\
            UNSG32 uFVF_STATUS1_MAX_DURATION_EXCEEDED          :  1;\
            UNSG32 uFVF_STATUS1_FRAME_START_AFTER_START        :  1;\
            UNSG32 uFVF_STATUS1_LINE_START_AFTER_START         :  1;\
            UNSG32 uFVF_STATUS1_FRAME_END_AFTER_END            :  1;\
            UNSG32 uFVF_STATUS1_LINE_END_AFTER_END             :  1;\
            UNSG32 RSVDx18_b9                                  : 23;\
          }
    union { UNSG32 u32FVF_FVF_STATUS1;
            struct w32FVF_FVF_STATUS1;
          };
    #define     w32FVF_FVF_ERROR_EN_CFG                        {\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_WRONG_FRAME_WIDTH_IND_EN :  1;\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_WRONG_FRAME_HEIGHT_IND_EN :  1;\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_MIN_LINE_GAP_SUCCEEDED_IND_EN :  1;\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_MIN_FRAME_GAP_SUCCEEDED_IND_EN :  1;\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_MAX_DURATION_EXCEEDED_IND_EN :  1;\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_FRAME_START_AFTER_START_IND_EN :  1;\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_LINE_START_AFTER_START_IND_EN :  1;\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_FRAME_END_AFTER_END_IND_EN :  1;\
            UNSG32 uFVF_ERROR_EN_CFG_INVALID_FRAME_LINE_END_AFTER_END_IND_EN :  1;\
            UNSG32 RSVDx1C_b9                                  : 23;\
          }
    union { UNSG32 u32FVF_FVF_ERROR_EN_CFG;
            struct w32FVF_FVF_ERROR_EN_CFG;
          };
    } SIE_FVF;
    typedef union  T32FVF_FVF_CFG0
          { UNSG32 u32;
            struct w32FVF_FVF_CFG0;
                 } T32FVF_FVF_CFG0;
    typedef union  T32FVF_FVF_CFG1
          { UNSG32 u32;
            struct w32FVF_FVF_CFG1;
                 } T32FVF_FVF_CFG1;
    typedef union  T32FVF_FVF_MAX_FRM_CFG
          { UNSG32 u32;
            struct w32FVF_FVF_MAX_FRM_CFG;
                 } T32FVF_FVF_MAX_FRM_CFG;
    typedef union  T32FVF_FVF_CNT_CLEAR
          { UNSG32 u32;
            struct w32FVF_FVF_CNT_CLEAR;
                 } T32FVF_FVF_CNT_CLEAR;
    typedef union  T32FVF_FVF_STATUS
          { UNSG32 u32;
            struct w32FVF_FVF_STATUS;
                 } T32FVF_FVF_STATUS;
    typedef union  T32FVF_FVF_COUNT
          { UNSG32 u32;
            struct w32FVF_FVF_COUNT;
                 } T32FVF_FVF_COUNT;
    typedef union  T32FVF_FVF_STATUS1
          { UNSG32 u32;
            struct w32FVF_FVF_STATUS1;
                 } T32FVF_FVF_STATUS1;
    typedef union  T32FVF_FVF_ERROR_EN_CFG
          { UNSG32 u32;
            struct w32FVF_FVF_ERROR_EN_CFG;
                 } T32FVF_FVF_ERROR_EN_CFG;
    typedef union  TFVF_FVF_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32FVF_FVF_CFG0;
                   };
                 } TFVF_FVF_CFG0;
    typedef union  TFVF_FVF_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32FVF_FVF_CFG1;
                   };
                 } TFVF_FVF_CFG1;
    typedef union  TFVF_FVF_MAX_FRM_CFG
          { UNSG32 u32[1];
            struct {
            struct w32FVF_FVF_MAX_FRM_CFG;
                   };
                 } TFVF_FVF_MAX_FRM_CFG;
    typedef union  TFVF_FVF_CNT_CLEAR
          { UNSG32 u32[1];
            struct {
            struct w32FVF_FVF_CNT_CLEAR;
                   };
                 } TFVF_FVF_CNT_CLEAR;
    typedef union  TFVF_FVF_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32FVF_FVF_STATUS;
                   };
                 } TFVF_FVF_STATUS;
    typedef union  TFVF_FVF_COUNT
          { UNSG32 u32[1];
            struct {
            struct w32FVF_FVF_COUNT;
                   };
                 } TFVF_FVF_COUNT;
    typedef union  TFVF_FVF_STATUS1
          { UNSG32 u32[1];
            struct {
            struct w32FVF_FVF_STATUS1;
                   };
                 } TFVF_FVF_STATUS1;
    typedef union  TFVF_FVF_ERROR_EN_CFG
          { UNSG32 u32[1];
            struct {
            struct w32FVF_FVF_ERROR_EN_CFG;
                   };
                 } TFVF_FVF_ERROR_EN_CFG;
     SIGN32 FVF_drvrd(SIE_FVF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 FVF_drvwr(SIE_FVF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void FVF_reset(SIE_FVF *p);
     SIGN32 FVF_cmp  (SIE_FVF *p, SIE_FVF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define FVF_check(p,pie,pfx,hLOG) FVF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define FVF_print(p,    pfx,hLOG) FVF_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_DEMOSAIC
#define h_DEMOSAIC (){}
    #define     RA_DEMOSAIC_CTRL                               0x0000
    #define   LSb32DEMOSAIC_CTRL_mode                             0
    #define       bDEMOSAIC_CTRL_mode                          3
    #define   MSK32DEMOSAIC_CTRL_mode                             0x00000007
    #define   LSb32DEMOSAIC_CTRL_image_width                      3
    #define       bDEMOSAIC_CTRL_image_width                   11
    #define   MSK32DEMOSAIC_CTRL_image_width                      0x00003FF8
    #define   LSb32DEMOSAIC_CTRL_image_height                     14
    #define       bDEMOSAIC_CTRL_image_height                  11
    #define   MSK32DEMOSAIC_CTRL_image_height                     0x01FFC000
    #define   LSb32DEMOSAIC_CTRL_enable                           25
    #define       bDEMOSAIC_CTRL_enable                        1
    #define   MSK32DEMOSAIC_CTRL_enable                           0x02000000
    #define   LSb32DEMOSAIC_CTRL_input_sel                        26
    #define       bDEMOSAIC_CTRL_input_sel                     1
    #define   MSK32DEMOSAIC_CTRL_input_sel                        0x04000000
    #define   LSb32DEMOSAIC_CTRL_enable_dyn                       27
    #define       bDEMOSAIC_CTRL_enable_dyn                    1
    #define   MSK32DEMOSAIC_CTRL_enable_dyn                       0x08000000
    typedef struct SIE_DEMOSAIC {
    #define     w32DEMOSAIC_CTRL                               {\
            UNSG32 uCTRL_mode                                  :  3;\
            UNSG32 uCTRL_image_width                           : 11;\
            UNSG32 uCTRL_image_height                          : 11;\
            UNSG32 uCTRL_enable                                :  1;\
            UNSG32 uCTRL_input_sel                             :  1;\
            UNSG32 uCTRL_enable_dyn                            :  1;\
            UNSG32 RSVDx0_b28                                  :  4;\
          }
    union { UNSG32 u32DEMOSAIC_CTRL;
            struct w32DEMOSAIC_CTRL;
          };
    } SIE_DEMOSAIC;
    typedef union  T32DEMOSAIC_CTRL
          { UNSG32 u32;
            struct w32DEMOSAIC_CTRL;
                 } T32DEMOSAIC_CTRL;
    typedef union  TDEMOSAIC_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32DEMOSAIC_CTRL;
                   };
                 } TDEMOSAIC_CTRL;
     SIGN32 DEMOSAIC_drvrd(SIE_DEMOSAIC *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DEMOSAIC_drvwr(SIE_DEMOSAIC *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DEMOSAIC_reset(SIE_DEMOSAIC *p);
     SIGN32 DEMOSAIC_cmp  (SIE_DEMOSAIC *p, SIE_DEMOSAIC *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DEMOSAIC_check(p,pie,pfx,hLOG) DEMOSAIC_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DEMOSAIC_print(p,    pfx,hLOG) DEMOSAIC_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_IMAGERES
#define h_IMAGERES (){}
    #define     RA_IMAGERES_IMGINSIZE                          0x0000
    #define   LSb32IMAGERES_IMGINSIZE_imgInHSize                  0
    #define       bIMAGERES_IMGINSIZE_imgInHSize               12
    #define   MSK32IMAGERES_IMGINSIZE_imgInHSize                  0x00000FFF
    #define   LSb32IMAGERES_IMGINSIZE_imgInVSize                  12
    #define       bIMAGERES_IMGINSIZE_imgInVSize               12
    #define   MSK32IMAGERES_IMGINSIZE_imgInVSize                  0x00FFF000
    #define     RA_IMAGERES_IMGOUTSIZE                         0x0004
    #define   LSb32IMAGERES_IMGOUTSIZE_imgOutHSize                0
    #define       bIMAGERES_IMGOUTSIZE_imgOutHSize             12
    #define   MSK32IMAGERES_IMGOUTSIZE_imgOutHSize                0x00000FFF
    #define   LSb32IMAGERES_IMGOUTSIZE_imgOutVSize                12
    #define       bIMAGERES_IMGOUTSIZE_imgOutVSize             12
    #define   MSK32IMAGERES_IMGOUTSIZE_imgOutVSize                0x00FFF000
    #define     RA_IMAGERES_CROPSTART                          0x0008
    #define   LSb32IMAGERES_CROPSTART_cropHStart                  0
    #define       bIMAGERES_CROPSTART_cropHStart               12
    #define   MSK32IMAGERES_CROPSTART_cropHStart                  0x00000FFF
    #define   LSb32IMAGERES_CROPSTART_cropVStart                  12
    #define       bIMAGERES_CROPSTART_cropVStart               12
    #define   MSK32IMAGERES_CROPSTART_cropVStart                  0x00FFF000
    #define     RA_IMAGERES_CROPEND                            0x000C
    #define   LSb32IMAGERES_CROPEND_cropHEnd                      0
    #define       bIMAGERES_CROPEND_cropHEnd                   12
    #define   MSK32IMAGERES_CROPEND_cropHEnd                      0x00000FFF
    #define   LSb32IMAGERES_CROPEND_cropVEnd                      12
    #define       bIMAGERES_CROPEND_cropVEnd                   12
    #define   MSK32IMAGERES_CROPEND_cropVEnd                      0x00FFF000
    #define     RA_IMAGERES_CTRL                               0x0010
    #define   LSb32IMAGERES_CTRL_imgType                          0
    #define       bIMAGERES_CTRL_imgType                       1
    #define   MSK32IMAGERES_CTRL_imgType                          0x00000001
    #define   LSb32IMAGERES_CTRL_imgResOpr                        1
    #define       bIMAGERES_CTRL_imgResOpr                     1
    #define   MSK32IMAGERES_CTRL_imgResOpr                        0x00000002
    #define   LSb32IMAGERES_CTRL_ratio                            2
    #define       bIMAGERES_CTRL_ratio                         3
    #define   MSK32IMAGERES_CTRL_ratio                            0x0000001C
    #define   LSb32IMAGERES_CTRL_fifoFlush                        5
    #define       bIMAGERES_CTRL_fifoFlush                     2
    #define   MSK32IMAGERES_CTRL_fifoFlush                        0x00000060
    typedef struct SIE_IMAGERES {
    #define     w32IMAGERES_IMGINSIZE                          {\
            UNSG32 uIMGINSIZE_imgInHSize                       : 12;\
            UNSG32 uIMGINSIZE_imgInVSize                       : 12;\
            UNSG32 RSVDx0_b24                                  :  8;\
          }
    union { UNSG32 u32IMAGERES_IMGINSIZE;
            struct w32IMAGERES_IMGINSIZE;
          };
    #define     w32IMAGERES_IMGOUTSIZE                         {\
            UNSG32 uIMGOUTSIZE_imgOutHSize                     : 12;\
            UNSG32 uIMGOUTSIZE_imgOutVSize                     : 12;\
            UNSG32 RSVDx4_b24                                  :  8;\
          }
    union { UNSG32 u32IMAGERES_IMGOUTSIZE;
            struct w32IMAGERES_IMGOUTSIZE;
          };
    #define     w32IMAGERES_CROPSTART                          {\
            UNSG32 uCROPSTART_cropHStart                       : 12;\
            UNSG32 uCROPSTART_cropVStart                       : 12;\
            UNSG32 RSVDx8_b24                                  :  8;\
          }
    union { UNSG32 u32IMAGERES_CROPSTART;
            struct w32IMAGERES_CROPSTART;
          };
    #define     w32IMAGERES_CROPEND                            {\
            UNSG32 uCROPEND_cropHEnd                           : 12;\
            UNSG32 uCROPEND_cropVEnd                           : 12;\
            UNSG32 RSVDxC_b24                                  :  8;\
          }
    union { UNSG32 u32IMAGERES_CROPEND;
            struct w32IMAGERES_CROPEND;
          };
    #define     w32IMAGERES_CTRL                               {\
            UNSG32 uCTRL_imgType                               :  1;\
            UNSG32 uCTRL_imgResOpr                             :  1;\
            UNSG32 uCTRL_ratio                                 :  3;\
            UNSG32 uCTRL_fifoFlush                             :  2;\
            UNSG32 RSVDx10_b7                                  : 25;\
          }
    union { UNSG32 u32IMAGERES_CTRL;
            struct w32IMAGERES_CTRL;
          };
    } SIE_IMAGERES;
    typedef union  T32IMAGERES_IMGINSIZE
          { UNSG32 u32;
            struct w32IMAGERES_IMGINSIZE;
                 } T32IMAGERES_IMGINSIZE;
    typedef union  T32IMAGERES_IMGOUTSIZE
          { UNSG32 u32;
            struct w32IMAGERES_IMGOUTSIZE;
                 } T32IMAGERES_IMGOUTSIZE;
    typedef union  T32IMAGERES_CROPSTART
          { UNSG32 u32;
            struct w32IMAGERES_CROPSTART;
                 } T32IMAGERES_CROPSTART;
    typedef union  T32IMAGERES_CROPEND
          { UNSG32 u32;
            struct w32IMAGERES_CROPEND;
                 } T32IMAGERES_CROPEND;
    typedef union  T32IMAGERES_CTRL
          { UNSG32 u32;
            struct w32IMAGERES_CTRL;
                 } T32IMAGERES_CTRL;
    typedef union  TIMAGERES_IMGINSIZE
          { UNSG32 u32[1];
            struct {
            struct w32IMAGERES_IMGINSIZE;
                   };
                 } TIMAGERES_IMGINSIZE;
    typedef union  TIMAGERES_IMGOUTSIZE
          { UNSG32 u32[1];
            struct {
            struct w32IMAGERES_IMGOUTSIZE;
                   };
                 } TIMAGERES_IMGOUTSIZE;
    typedef union  TIMAGERES_CROPSTART
          { UNSG32 u32[1];
            struct {
            struct w32IMAGERES_CROPSTART;
                   };
                 } TIMAGERES_CROPSTART;
    typedef union  TIMAGERES_CROPEND
          { UNSG32 u32[1];
            struct {
            struct w32IMAGERES_CROPEND;
                   };
                 } TIMAGERES_CROPEND;
    typedef union  TIMAGERES_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32IMAGERES_CTRL;
                   };
                 } TIMAGERES_CTRL;
     SIGN32 IMAGERES_drvrd(SIE_IMAGERES *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 IMAGERES_drvwr(SIE_IMAGERES *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void IMAGERES_reset(SIE_IMAGERES *p);
     SIGN32 IMAGERES_cmp  (SIE_IMAGERES *p, SIE_IMAGERES *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define IMAGERES_check(p,pie,pfx,hLOG) IMAGERES_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define IMAGERES_print(p,    pfx,hLOG) IMAGERES_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_IMAGERESWRAP
#define h_IMAGERESWRAP (){}
    #define     RA_IMAGERESWRAP_IMAGERES                       0x0000
    #define     RA_IMAGERESWRAP_CTRL                           0x0014
    #define   LSb32IMAGERESWRAP_CTRL_host0_halt_en                0
    #define       bIMAGERESWRAP_CTRL_host0_halt_en             1
    #define   MSK32IMAGERESWRAP_CTRL_host0_halt_en                0x00000001
    #define   LSb32IMAGERESWRAP_CTRL_host1_halt_en                1
    #define       bIMAGERESWRAP_CTRL_host1_halt_en             1
    #define   MSK32IMAGERESWRAP_CTRL_host1_halt_en                0x00000002
    #define   LSb32IMAGERESWRAP_CTRL_enable                       2
    #define       bIMAGERESWRAP_CTRL_enable                    1
    #define   MSK32IMAGERESWRAP_CTRL_enable                       0x00000004
    #define   LSb32IMAGERESWRAP_CTRL_nframes_skip                 3
    #define       bIMAGERESWRAP_CTRL_nframes_skip              8
    #define   MSK32IMAGERESWRAP_CTRL_nframes_skip                 0x000007F8
    #define   LSb32IMAGERESWRAP_CTRL_capture_sw                   11
    #define       bIMAGERESWRAP_CTRL_capture_sw                1
    #define   MSK32IMAGERESWRAP_CTRL_capture_sw                   0x00000800
    #define   LSb32IMAGERESWRAP_CTRL_one_shot_capture_on          12
    #define       bIMAGERESWRAP_CTRL_one_shot_capture_on       1
    #define   MSK32IMAGERESWRAP_CTRL_one_shot_capture_on          0x00001000
    #define   LSb32IMAGERESWRAP_CTRL_one_shot_capture             13
    #define       bIMAGERESWRAP_CTRL_one_shot_capture          1
    #define   MSK32IMAGERESWRAP_CTRL_one_shot_capture             0x00002000
    #define   LSb32IMAGERESWRAP_CTRL_n_eof                        14
    #define       bIMAGERESWRAP_CTRL_n_eof                     8
    #define   MSK32IMAGERESWRAP_CTRL_n_eof                        0x003FC000
    #define   LSb32IMAGERESWRAP_CTRL_input_sel                    22
    #define       bIMAGERESWRAP_CTRL_input_sel                 2
    #define   MSK32IMAGERESWRAP_CTRL_input_sel                    0x00C00000
    #define   LSb32IMAGERESWRAP_CTRL_WrClient_rdy_en              24
    #define       bIMAGERESWRAP_CTRL_WrClient_rdy_en           1
    #define   MSK32IMAGERESWRAP_CTRL_WrClient_rdy_en              0x01000000
    #define   LSb32IMAGERESWRAP_CTRL_lps_pixReq_en                25
    #define       bIMAGERESWRAP_CTRL_lps_pixReq_en             1
    #define   MSK32IMAGERESWRAP_CTRL_lps_pixReq_en                0x02000000
    #define   LSb32IMAGERESWRAP_CTRL_ovflowClr0                   26
    #define       bIMAGERESWRAP_CTRL_ovflowClr0                1
    #define   MSK32IMAGERESWRAP_CTRL_ovflowClr0                   0x04000000
    #define   LSb32IMAGERESWRAP_CTRL_ovflowClr1                   27
    #define       bIMAGERESWRAP_CTRL_ovflowClr1                1
    #define   MSK32IMAGERESWRAP_CTRL_ovflowClr1                   0x08000000
    #define   LSb32IMAGERESWRAP_CTRL_ipi_swap_en                  28
    #define       bIMAGERESWRAP_CTRL_ipi_swap_en               1
    #define   MSK32IMAGERESWRAP_CTRL_ipi_swap_en                  0x10000000
    #define   LSb32IMAGERESWRAP_CTRL_ipi_swap_en_lvl2             29
    #define       bIMAGERESWRAP_CTRL_ipi_swap_en_lvl2          2
    #define   MSK32IMAGERESWRAP_CTRL_ipi_swap_en_lvl2             0x60000000
    #define     RA_IMAGERESWRAP_CTRL1                          0x0018
    #define   LSb32IMAGERESWRAP_CTRL1_format_based_mask_lsb       0
    #define       bIMAGERESWRAP_CTRL1_format_based_mask_lsb    32
    #define   MSK32IMAGERESWRAP_CTRL1_format_based_mask_lsb       0xFFFFFFFF
    #define     RA_IMAGERESWRAP_CTRL2                          0x001C
    #define   LSb32IMAGERESWRAP_CTRL2_format_based_mask_msb       0
    #define       bIMAGERESWRAP_CTRL2_format_based_mask_msb    16
    #define   MSK32IMAGERESWRAP_CTRL2_format_based_mask_msb       0x0000FFFF
    typedef struct SIE_IMAGERESWRAP {
              SIE_IMAGERES                                     ie_IMAGERES;
    #define     w32IMAGERESWRAP_CTRL                           {\
            UNSG32 uCTRL_host0_halt_en                         :  1;\
            UNSG32 uCTRL_host1_halt_en                         :  1;\
            UNSG32 uCTRL_enable                                :  1;\
            UNSG32 uCTRL_nframes_skip                          :  8;\
            UNSG32 uCTRL_capture_sw                            :  1;\
            UNSG32 uCTRL_one_shot_capture_on                   :  1;\
            UNSG32 uCTRL_one_shot_capture                      :  1;\
            UNSG32 uCTRL_n_eof                                 :  8;\
            UNSG32 uCTRL_input_sel                             :  2;\
            UNSG32 uCTRL_WrClient_rdy_en                       :  1;\
            UNSG32 uCTRL_lps_pixReq_en                         :  1;\
            UNSG32 uCTRL_ovflowClr0                            :  1;\
            UNSG32 uCTRL_ovflowClr1                            :  1;\
            UNSG32 uCTRL_ipi_swap_en                           :  1;\
            UNSG32 uCTRL_ipi_swap_en_lvl2                      :  2;\
            UNSG32 RSVDx14_b31                                 :  1;\
          }
    union { UNSG32 u32IMAGERESWRAP_CTRL;
            struct w32IMAGERESWRAP_CTRL;
          };
    #define     w32IMAGERESWRAP_CTRL1                          {\
            UNSG32 uCTRL1_format_based_mask_lsb                : 32;\
          }
    union { UNSG32 u32IMAGERESWRAP_CTRL1;
            struct w32IMAGERESWRAP_CTRL1;
          };
    #define     w32IMAGERESWRAP_CTRL2                          {\
            UNSG32 uCTRL2_format_based_mask_msb                : 16;\
            UNSG32 RSVDx1C_b16                                 : 16;\
          }
    union { UNSG32 u32IMAGERESWRAP_CTRL2;
            struct w32IMAGERESWRAP_CTRL2;
          };
    } SIE_IMAGERESWRAP;
    typedef union  T32IMAGERESWRAP_CTRL
          { UNSG32 u32;
            struct w32IMAGERESWRAP_CTRL;
                 } T32IMAGERESWRAP_CTRL;
    typedef union  T32IMAGERESWRAP_CTRL1
          { UNSG32 u32;
            struct w32IMAGERESWRAP_CTRL1;
                 } T32IMAGERESWRAP_CTRL1;
    typedef union  T32IMAGERESWRAP_CTRL2
          { UNSG32 u32;
            struct w32IMAGERESWRAP_CTRL2;
                 } T32IMAGERESWRAP_CTRL2;
    typedef union  TIMAGERESWRAP_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32IMAGERESWRAP_CTRL;
                   };
                 } TIMAGERESWRAP_CTRL;
    typedef union  TIMAGERESWRAP_CTRL1
          { UNSG32 u32[1];
            struct {
            struct w32IMAGERESWRAP_CTRL1;
                   };
                 } TIMAGERESWRAP_CTRL1;
    typedef union  TIMAGERESWRAP_CTRL2
          { UNSG32 u32[1];
            struct {
            struct w32IMAGERESWRAP_CTRL2;
                   };
                 } TIMAGERESWRAP_CTRL2;
     SIGN32 IMAGERESWRAP_drvrd(SIE_IMAGERESWRAP *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 IMAGERESWRAP_drvwr(SIE_IMAGERESWRAP *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void IMAGERESWRAP_reset(SIE_IMAGERESWRAP *p);
     SIGN32 IMAGERESWRAP_cmp  (SIE_IMAGERESWRAP *p, SIE_IMAGERESWRAP *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define IMAGERESWRAP_check(p,pie,pfx,hLOG) IMAGERESWRAP_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define IMAGERESWRAP_print(p,    pfx,hLOG) IMAGERESWRAP_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_IIF
#define h_IIF (){}
    #define     RA_IIF_IIF_CFG0                                0x0000
    #define   LSb32IIF_IIF_CFG0_INTERFACE_TYPE                    0
    #define       bIIF_IIF_CFG0_INTERFACE_TYPE                 1
    #define   MSK32IIF_IIF_CFG0_INTERFACE_TYPE                    0x00000001
    #define   LSb32IIF_IIF_CFG0_INPUT_WIDTH                       1
    #define       bIIF_IIF_CFG0_INPUT_WIDTH                    2
    #define   MSK32IIF_IIF_CFG0_INPUT_WIDTH                       0x00000006
    #define   LSb32IIF_IIF_CFG0_PACK_MODE                         3
    #define       bIIF_IIF_CFG0_PACK_MODE                      1
    #define   MSK32IIF_IIF_CFG0_PACK_MODE                         0x00000008
    #define   LSb32IIF_IIF_CFG0_INPUT_FLIP                        4
    #define       bIIF_IIF_CFG0_INPUT_FLIP                     1
    #define   MSK32IIF_IIF_CFG0_INPUT_FLIP                        0x00000010
    #define   LSb32IIF_IIF_CFG0_PCLK_POLARITY                     5
    #define       bIIF_IIF_CFG0_PCLK_POLARITY                  1
    #define   MSK32IIF_IIF_CFG0_PCLK_POLARITY                     0x00000020
    #define   LSb32IIF_IIF_CFG0_VSYNC_POLARITY                    6
    #define       bIIF_IIF_CFG0_VSYNC_POLARITY                 1
    #define   MSK32IIF_IIF_CFG0_VSYNC_POLARITY                    0x00000040
    #define   LSb32IIF_IIF_CFG0_HSYNC_POLARITY                    7
    #define       bIIF_IIF_CFG0_HSYNC_POLARITY                 1
    #define   MSK32IIF_IIF_CFG0_HSYNC_POLARITY                    0x00000080
    #define   LSb32IIF_IIF_CFG0_CALC_CRC                          8
    #define       bIIF_IIF_CFG0_CALC_CRC                       1
    #define   MSK32IIF_IIF_CFG0_CALC_CRC                          0x00000100
    #define   LSb32IIF_IIF_CFG0_RAW10_FRAME                       9
    #define       bIIF_IIF_CFG0_RAW10_FRAME                    1
    #define   MSK32IIF_IIF_CFG0_RAW10_FRAME                       0x00000200
    #define   LSb32IIF_IIF_CFG0_START_OF_FRAME_HEADER_EXIST       10
    #define       bIIF_IIF_CFG0_START_OF_FRAME_HEADER_EXIST    1
    #define   MSK32IIF_IIF_CFG0_START_OF_FRAME_HEADER_EXIST       0x00000400
    #define   LSb32IIF_IIF_CFG0_END_OF_FRAME_HEADER_EXIST         11
    #define       bIIF_IIF_CFG0_END_OF_FRAME_HEADER_EXIST      1
    #define   MSK32IIF_IIF_CFG0_END_OF_FRAME_HEADER_EXIST         0x00000800
    #define   LSb32IIF_IIF_CFG0_DATA_HEADER_EXIST                 12
    #define       bIIF_IIF_CFG0_DATA_HEADER_EXIST              1
    #define   MSK32IIF_IIF_CFG0_DATA_HEADER_EXIST                 0x00001000
    #define   LSb32IIF_IIF_CFG0_DATA_HEADER_LAST_INDEX            13
    #define       bIIF_IIF_CFG0_DATA_HEADER_LAST_INDEX         4
    #define   MSK32IIF_IIF_CFG0_DATA_HEADER_LAST_INDEX            0x0001E000
    #define   LSb32IIF_IIF_CFG0_ERROR_CHECK_ENABLE                17
    #define       bIIF_IIF_CFG0_ERROR_CHECK_ENABLE             5
    #define   MSK32IIF_IIF_CFG0_ERROR_CHECK_ENABLE                0x003E0000
    #define   LSb32IIF_IIF_CFG0_FIRST_VSYNC_EN                    22
    #define       bIIF_IIF_CFG0_FIRST_VSYNC_EN                 1
    #define   MSK32IIF_IIF_CFG0_FIRST_VSYNC_EN                    0x00400000
    #define   LSb32IIF_IIF_CFG0_RESET_PCLK_SYNC_EN                23
    #define       bIIF_IIF_CFG0_RESET_PCLK_SYNC_EN             1
    #define   MSK32IIF_IIF_CFG0_RESET_PCLK_SYNC_EN                0x00800000
    #define   LSb32IIF_IIF_CFG0_FRAME_IND_EN                      24
    #define       bIIF_IIF_CFG0_FRAME_IND_EN                   1
    #define   MSK32IIF_IIF_CFG0_FRAME_IND_EN                      0x01000000
    #define     RA_IIF_IIF_CFG1                                0x0004
    #define   LSb32IIF_IIF_CFG1_LAST_COL_INDEX                    0
    #define       bIIF_IIF_CFG1_LAST_COL_INDEX                 10
    #define   MSK32IIF_IIF_CFG1_LAST_COL_INDEX                    0x000003FF
    #define   LSb32IIF_IIF_CFG1_LAST_ROW_INDEX                    10
    #define       bIIF_IIF_CFG1_LAST_ROW_INDEX                 10
    #define   MSK32IIF_IIF_CFG1_LAST_ROW_INDEX                    0x000FFC00
    #define   LSb32IIF_IIF_CFG1_START_OF_FRAME_HEADER_LAST_INDEX    20
    #define       bIIF_IIF_CFG1_START_OF_FRAME_HEADER_LAST_INDEX 4
    #define   MSK32IIF_IIF_CFG1_START_OF_FRAME_HEADER_LAST_INDEX    0x00F00000
    #define   LSb32IIF_IIF_CFG1_END_OF_FRAME_HEADER_LAST_INDEX    24
    #define       bIIF_IIF_CFG1_END_OF_FRAME_HEADER_LAST_INDEX 4
    #define   MSK32IIF_IIF_CFG1_END_OF_FRAME_HEADER_LAST_INDEX    0x0F000000
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG 0x0008
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG 0x000C
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG 0x0010
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG 0x0014
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG 0x0018
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG 0x001C
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG 0x0020
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG 0x0024
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_VALUE_000_031_CFG       0x0028
    #define   LSb32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG_VAL      0
    #define       bIIF_IIF_DATA_HEADER_VALUE_000_031_CFG_VAL   32
    #define   MSK32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG_VAL      0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_VALUE_032_063_CFG       0x002C
    #define   LSb32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG_VAL      0
    #define       bIIF_IIF_DATA_HEADER_VALUE_032_063_CFG_VAL   32
    #define   MSK32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG_VAL      0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_VALUE_064_095_CFG       0x0030
    #define   LSb32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG_VAL      0
    #define       bIIF_IIF_DATA_HEADER_VALUE_064_095_CFG_VAL   32
    #define   MSK32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG_VAL      0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_VALUE_096_127_CFG       0x0034
    #define   LSb32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG_VAL      0
    #define       bIIF_IIF_DATA_HEADER_VALUE_096_127_CFG_VAL   32
    #define   MSK32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG_VAL      0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG 0x0038
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG 0x003C
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG 0x0040
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG 0x0044
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG 0x0048
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG 0x004C
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG 0x0050
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG 0x0054
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_MASK_000_031_CFG        0x0058
    #define   LSb32IIF_IIF_DATA_HEADER_MASK_000_031_CFG_VAL       0
    #define       bIIF_IIF_DATA_HEADER_MASK_000_031_CFG_VAL    32
    #define   MSK32IIF_IIF_DATA_HEADER_MASK_000_031_CFG_VAL       0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_MASK_032_063_CFG        0x005C
    #define   LSb32IIF_IIF_DATA_HEADER_MASK_032_063_CFG_VAL       0
    #define       bIIF_IIF_DATA_HEADER_MASK_032_063_CFG_VAL    32
    #define   MSK32IIF_IIF_DATA_HEADER_MASK_032_063_CFG_VAL       0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_MASK_064_095_CFG        0x0060
    #define   LSb32IIF_IIF_DATA_HEADER_MASK_064_095_CFG_VAL       0
    #define       bIIF_IIF_DATA_HEADER_MASK_064_095_CFG_VAL    32
    #define   MSK32IIF_IIF_DATA_HEADER_MASK_064_095_CFG_VAL       0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_MASK_096_127_CFG        0x0064
    #define   LSb32IIF_IIF_DATA_HEADER_MASK_096_127_CFG_VAL       0
    #define       bIIF_IIF_DATA_HEADER_MASK_096_127_CFG_VAL    32
    #define   MSK32IIF_IIF_DATA_HEADER_MASK_096_127_CFG_VAL       0xFFFFFFFF
    #define     RA_IIF_IIF_STATUS0                             0x0068
    #define   LSb32IIF_IIF_STATUS0_IMAGE_INTERFACE_STATE          0
    #define       bIIF_IIF_STATUS0_IMAGE_INTERFACE_STATE       2
    #define   MSK32IIF_IIF_STATUS0_IMAGE_INTERFACE_STATE          0x00000003
    #define   LSb32IIF_IIF_STATUS0_CUR_STAT                       2
    #define       bIIF_IIF_STATUS0_CUR_STAT                    4
    #define   MSK32IIF_IIF_STATUS0_CUR_STAT                       0x0000003C
    #define   LSb32IIF_IIF_STATUS0_COL_CNT                        6
    #define       bIIF_IIF_STATUS0_COL_CNT                     10
    #define   MSK32IIF_IIF_STATUS0_COL_CNT                        0x0000FFC0
    #define   LSb32IIF_IIF_STATUS0_ROW_CNT                        16
    #define       bIIF_IIF_STATUS0_ROW_CNT                     10
    #define   MSK32IIF_IIF_STATUS0_ROW_CNT                        0x03FF0000
    #define     RA_IIF_IIF_STATUS1                             0x006C
    #define   LSb32IIF_IIF_STATUS1_CRC_VAL                        0
    #define       bIIF_IIF_STATUS1_CRC_VAL                     16
    #define   MSK32IIF_IIF_STATUS1_CRC_VAL                        0x0000FFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031   0x0070
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_VAL_000_031_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063   0x0074
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_VAL_032_063_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095   0x0078
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_VAL_064_095_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127   0x007C
    #define   LSb32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127_VAL    0
    #define       bIIF_IIF_START_OF_FRAME_HEADER_VAL_096_127_VAL 32
    #define   MSK32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031     0x0080
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_VAL_000_031_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063     0x0084
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_VAL_032_063_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095     0x0088
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_VAL_064_095_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127     0x008C
    #define   LSb32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127_VAL    0
    #define       bIIF_IIF_END_OF_FRAME_HEADER_VAL_096_127_VAL 32
    #define   MSK32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127_VAL    0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_VAL_000_031             0x0090
    #define   LSb32IIF_IIF_DATA_HEADER_VAL_000_031_VAL            0
    #define       bIIF_IIF_DATA_HEADER_VAL_000_031_VAL         32
    #define   MSK32IIF_IIF_DATA_HEADER_VAL_000_031_VAL            0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_VAL_032_063             0x0094
    #define   LSb32IIF_IIF_DATA_HEADER_VAL_032_063_VAL            0
    #define       bIIF_IIF_DATA_HEADER_VAL_032_063_VAL         32
    #define   MSK32IIF_IIF_DATA_HEADER_VAL_032_063_VAL            0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_VAL_064_095             0x0098
    #define   LSb32IIF_IIF_DATA_HEADER_VAL_064_095_VAL            0
    #define       bIIF_IIF_DATA_HEADER_VAL_064_095_VAL         32
    #define   MSK32IIF_IIF_DATA_HEADER_VAL_064_095_VAL            0xFFFFFFFF
    #define     RA_IIF_IIF_DATA_HEADER_VAL_096_127             0x009C
    #define   LSb32IIF_IIF_DATA_HEADER_VAL_096_127_VAL            0
    #define       bIIF_IIF_DATA_HEADER_VAL_096_127_VAL         32
    #define   MSK32IIF_IIF_DATA_HEADER_VAL_096_127_VAL            0xFFFFFFFF
    #define     RA_IIF_IIF_ERROR_EN_CFG                        0x00A0
    #define   LSb32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_START_FRAME_HEADER_ERROR_IND_EN    0
    #define       bIIF_IIF_ERROR_EN_CFG_IMAGE_IF_START_FRAME_HEADER_ERROR_IND_EN 1
    #define   MSK32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_START_FRAME_HEADER_ERROR_IND_EN    0x00000001
    #define   LSb32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_DATA_HEADER_ERROR_IND_EN    1
    #define       bIIF_IIF_ERROR_EN_CFG_IMAGE_IF_DATA_HEADER_ERROR_IND_EN 1
    #define   MSK32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_DATA_HEADER_ERROR_IND_EN    0x00000002
    #define   LSb32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_END_FRAME_HEADER_ERROR_IND_EN    2
    #define       bIIF_IIF_ERROR_EN_CFG_IMAGE_IF_END_FRAME_HEADER_ERROR_IND_EN 1
    #define   MSK32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_END_FRAME_HEADER_ERROR_IND_EN    0x00000004
    #define   LSb32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_FIFO_ERROR_IND_EN    3
    #define       bIIF_IIF_ERROR_EN_CFG_IMAGE_IF_FIFO_ERROR_IND_EN 1
    #define   MSK32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_FIFO_ERROR_IND_EN    0x00000008
    #define   LSb32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_VSYNC_WRAP_HSYNC_ERROR_IND_EN    4
    #define       bIIF_IIF_ERROR_EN_CFG_IMAGE_IF_VSYNC_WRAP_HSYNC_ERROR_IND_EN 1
    #define   MSK32IIF_IIF_ERROR_EN_CFG_IMAGE_IF_VSYNC_WRAP_HSYNC_ERROR_IND_EN    0x00000010
    #define     RA_IIF_IIF_STATUS                              0x00A4
    #define   LSb32IIF_IIF_STATUS_IMAGE_IF_START_FRAME_HEADER_ERROR_STAT    0
    #define       bIIF_IIF_STATUS_IMAGE_IF_START_FRAME_HEADER_ERROR_STAT 1
    #define   MSK32IIF_IIF_STATUS_IMAGE_IF_START_FRAME_HEADER_ERROR_STAT    0x00000001
    #define   LSb32IIF_IIF_STATUS_IMAGE_IF_DATA_HEADER_ERROR_STAT    1
    #define       bIIF_IIF_STATUS_IMAGE_IF_DATA_HEADER_ERROR_STAT 1
    #define   MSK32IIF_IIF_STATUS_IMAGE_IF_DATA_HEADER_ERROR_STAT    0x00000002
    #define   LSb32IIF_IIF_STATUS_IMAGE_IF_END_FRAME_HEADER_ERROR_STAT    2
    #define       bIIF_IIF_STATUS_IMAGE_IF_END_FRAME_HEADER_ERROR_STAT 1
    #define   MSK32IIF_IIF_STATUS_IMAGE_IF_END_FRAME_HEADER_ERROR_STAT    0x00000004
    #define   LSb32IIF_IIF_STATUS_IMAGE_IF_FIFO_ERROR_STAT        3
    #define       bIIF_IIF_STATUS_IMAGE_IF_FIFO_ERROR_STAT     1
    #define   MSK32IIF_IIF_STATUS_IMAGE_IF_FIFO_ERROR_STAT        0x00000008
    #define   LSb32IIF_IIF_STATUS_IMAGE_IF_VSYNC_WRAP_HSYNC_ERROR_STAT    4
    #define       bIIF_IIF_STATUS_IMAGE_IF_VSYNC_WRAP_HSYNC_ERROR_STAT 1
    #define   MSK32IIF_IIF_STATUS_IMAGE_IF_VSYNC_WRAP_HSYNC_ERROR_STAT    0x00000010
    typedef struct SIE_IIF {
    #define     w32IIF_IIF_CFG0                                {\
            UNSG32 uIIF_CFG0_INTERFACE_TYPE                    :  1;\
            UNSG32 uIIF_CFG0_INPUT_WIDTH                       :  2;\
            UNSG32 uIIF_CFG0_PACK_MODE                         :  1;\
            UNSG32 uIIF_CFG0_INPUT_FLIP                        :  1;\
            UNSG32 uIIF_CFG0_PCLK_POLARITY                     :  1;\
            UNSG32 uIIF_CFG0_VSYNC_POLARITY                    :  1;\
            UNSG32 uIIF_CFG0_HSYNC_POLARITY                    :  1;\
            UNSG32 uIIF_CFG0_CALC_CRC                          :  1;\
            UNSG32 uIIF_CFG0_RAW10_FRAME                       :  1;\
            UNSG32 uIIF_CFG0_START_OF_FRAME_HEADER_EXIST       :  1;\
            UNSG32 uIIF_CFG0_END_OF_FRAME_HEADER_EXIST         :  1;\
            UNSG32 uIIF_CFG0_DATA_HEADER_EXIST                 :  1;\
            UNSG32 uIIF_CFG0_DATA_HEADER_LAST_INDEX            :  4;\
            UNSG32 uIIF_CFG0_ERROR_CHECK_ENABLE                :  5;\
            UNSG32 uIIF_CFG0_FIRST_VSYNC_EN                    :  1;\
            UNSG32 uIIF_CFG0_RESET_PCLK_SYNC_EN                :  1;\
            UNSG32 uIIF_CFG0_FRAME_IND_EN                      :  1;\
            UNSG32 RSVDx0_b25                                  :  7;\
          }
    union { UNSG32 u32IIF_IIF_CFG0;
            struct w32IIF_IIF_CFG0;
          };
    #define     w32IIF_IIF_CFG1                                {\
            UNSG32 uIIF_CFG1_LAST_COL_INDEX                    : 10;\
            UNSG32 uIIF_CFG1_LAST_ROW_INDEX                    : 10;\
            UNSG32 uIIF_CFG1_START_OF_FRAME_HEADER_LAST_INDEX  :  4;\
            UNSG32 uIIF_CFG1_END_OF_FRAME_HEADER_LAST_INDEX    :  4;\
            UNSG32 RSVDx4_b28                                  :  4;\
          }
    union { UNSG32 u32IIF_IIF_CFG1;
            struct w32IIF_IIF_CFG1;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG;
          };
    #define     w32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG       {\
            UNSG32 uIIF_DATA_HEADER_VALUE_000_031_CFG_VAL      : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG;
            struct w32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG;
          };
    #define     w32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG       {\
            UNSG32 uIIF_DATA_HEADER_VALUE_032_063_CFG_VAL      : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG;
            struct w32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG;
          };
    #define     w32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG       {\
            UNSG32 uIIF_DATA_HEADER_VALUE_064_095_CFG_VAL      : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG;
            struct w32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG;
          };
    #define     w32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG       {\
            UNSG32 uIIF_DATA_HEADER_VALUE_096_127_CFG_VAL      : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG;
            struct w32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_MASK_000_031_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_MASK_032_063_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_MASK_064_095_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_MASK_096_127_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_MASK_000_031_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_MASK_032_063_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_MASK_064_095_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_MASK_096_127_CFG_VAL : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG;
          };
    #define     w32IIF_IIF_DATA_HEADER_MASK_000_031_CFG        {\
            UNSG32 uIIF_DATA_HEADER_MASK_000_031_CFG_VAL       : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_MASK_000_031_CFG;
            struct w32IIF_IIF_DATA_HEADER_MASK_000_031_CFG;
          };
    #define     w32IIF_IIF_DATA_HEADER_MASK_032_063_CFG        {\
            UNSG32 uIIF_DATA_HEADER_MASK_032_063_CFG_VAL       : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_MASK_032_063_CFG;
            struct w32IIF_IIF_DATA_HEADER_MASK_032_063_CFG;
          };
    #define     w32IIF_IIF_DATA_HEADER_MASK_064_095_CFG        {\
            UNSG32 uIIF_DATA_HEADER_MASK_064_095_CFG_VAL       : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_MASK_064_095_CFG;
            struct w32IIF_IIF_DATA_HEADER_MASK_064_095_CFG;
          };
    #define     w32IIF_IIF_DATA_HEADER_MASK_096_127_CFG        {\
            UNSG32 uIIF_DATA_HEADER_MASK_096_127_CFG_VAL       : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_MASK_096_127_CFG;
            struct w32IIF_IIF_DATA_HEADER_MASK_096_127_CFG;
          };
    #define     w32IIF_IIF_STATUS0                             {\
            UNSG32 uIIF_STATUS0_IMAGE_INTERFACE_STATE          :  2;\
            UNSG32 uIIF_STATUS0_CUR_STAT                       :  4;\
            UNSG32 uIIF_STATUS0_COL_CNT                        : 10;\
            UNSG32 uIIF_STATUS0_ROW_CNT                        : 10;\
            UNSG32 RSVDx68_b26                                 :  6;\
          }
    union { UNSG32 u32IIF_IIF_STATUS0;
            struct w32IIF_IIF_STATUS0;
          };
    #define     w32IIF_IIF_STATUS1                             {\
            UNSG32 uIIF_STATUS1_CRC_VAL                        : 16;\
            UNSG32 RSVDx6C_b16                                 : 16;\
          }
    union { UNSG32 u32IIF_IIF_STATUS1;
            struct w32IIF_IIF_STATUS1;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031   {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_VAL_000_031_VAL  : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063   {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_VAL_032_063_VAL  : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095   {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_VAL_064_095_VAL  : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095;
          };
    #define     w32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127   {\
            UNSG32 uIIF_START_OF_FRAME_HEADER_VAL_096_127_VAL  : 32;\
          }
    union { UNSG32 u32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031     {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_VAL_000_031_VAL    : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063     {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_VAL_032_063_VAL    : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095     {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_VAL_064_095_VAL    : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095;
          };
    #define     w32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127     {\
            UNSG32 uIIF_END_OF_FRAME_HEADER_VAL_096_127_VAL    : 32;\
          }
    union { UNSG32 u32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127;
          };
    #define     w32IIF_IIF_DATA_HEADER_VAL_000_031             {\
            UNSG32 uIIF_DATA_HEADER_VAL_000_031_VAL            : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_VAL_000_031;
            struct w32IIF_IIF_DATA_HEADER_VAL_000_031;
          };
    #define     w32IIF_IIF_DATA_HEADER_VAL_032_063             {\
            UNSG32 uIIF_DATA_HEADER_VAL_032_063_VAL            : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_VAL_032_063;
            struct w32IIF_IIF_DATA_HEADER_VAL_032_063;
          };
    #define     w32IIF_IIF_DATA_HEADER_VAL_064_095             {\
            UNSG32 uIIF_DATA_HEADER_VAL_064_095_VAL            : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_VAL_064_095;
            struct w32IIF_IIF_DATA_HEADER_VAL_064_095;
          };
    #define     w32IIF_IIF_DATA_HEADER_VAL_096_127             {\
            UNSG32 uIIF_DATA_HEADER_VAL_096_127_VAL            : 32;\
          }
    union { UNSG32 u32IIF_IIF_DATA_HEADER_VAL_096_127;
            struct w32IIF_IIF_DATA_HEADER_VAL_096_127;
          };
    #define     w32IIF_IIF_ERROR_EN_CFG                        {\
            UNSG32 uIIF_ERROR_EN_CFG_IMAGE_IF_START_FRAME_HEADER_ERROR_IND_EN :  1;\
            UNSG32 uIIF_ERROR_EN_CFG_IMAGE_IF_DATA_HEADER_ERROR_IND_EN :  1;\
            UNSG32 uIIF_ERROR_EN_CFG_IMAGE_IF_END_FRAME_HEADER_ERROR_IND_EN :  1;\
            UNSG32 uIIF_ERROR_EN_CFG_IMAGE_IF_FIFO_ERROR_IND_EN :  1;\
            UNSG32 uIIF_ERROR_EN_CFG_IMAGE_IF_VSYNC_WRAP_HSYNC_ERROR_IND_EN :  1;\
            UNSG32 RSVDxA0_b5                                  : 27;\
          }
    union { UNSG32 u32IIF_IIF_ERROR_EN_CFG;
            struct w32IIF_IIF_ERROR_EN_CFG;
          };
    #define     w32IIF_IIF_STATUS                              {\
            UNSG32 uIIF_STATUS_IMAGE_IF_START_FRAME_HEADER_ERROR_STAT :  1;\
            UNSG32 uIIF_STATUS_IMAGE_IF_DATA_HEADER_ERROR_STAT :  1;\
            UNSG32 uIIF_STATUS_IMAGE_IF_END_FRAME_HEADER_ERROR_STAT :  1;\
            UNSG32 uIIF_STATUS_IMAGE_IF_FIFO_ERROR_STAT        :  1;\
            UNSG32 uIIF_STATUS_IMAGE_IF_VSYNC_WRAP_HSYNC_ERROR_STAT :  1;\
            UNSG32 RSVDxA4_b5                                  : 27;\
          }
    union { UNSG32 u32IIF_IIF_STATUS;
            struct w32IIF_IIF_STATUS;
          };
    } SIE_IIF;
    typedef union  T32IIF_IIF_CFG0
          { UNSG32 u32;
            struct w32IIF_IIF_CFG0;
                 } T32IIF_IIF_CFG0;
    typedef union  T32IIF_IIF_CFG1
          { UNSG32 u32;
            struct w32IIF_IIF_CFG1;
                 } T32IIF_IIF_CFG1;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG;
    typedef union  T32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG;
                 } T32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG;
    typedef union  T32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG;
                 } T32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG;
    typedef union  T32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG;
                 } T32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG;
    typedef union  T32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG;
                 } T32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG;
    typedef union  T32IIF_IIF_DATA_HEADER_MASK_000_031_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_MASK_000_031_CFG;
                 } T32IIF_IIF_DATA_HEADER_MASK_000_031_CFG;
    typedef union  T32IIF_IIF_DATA_HEADER_MASK_032_063_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_MASK_032_063_CFG;
                 } T32IIF_IIF_DATA_HEADER_MASK_032_063_CFG;
    typedef union  T32IIF_IIF_DATA_HEADER_MASK_064_095_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_MASK_064_095_CFG;
                 } T32IIF_IIF_DATA_HEADER_MASK_064_095_CFG;
    typedef union  T32IIF_IIF_DATA_HEADER_MASK_096_127_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_MASK_096_127_CFG;
                 } T32IIF_IIF_DATA_HEADER_MASK_096_127_CFG;
    typedef union  T32IIF_IIF_STATUS0
          { UNSG32 u32;
            struct w32IIF_IIF_STATUS0;
                 } T32IIF_IIF_STATUS0;
    typedef union  T32IIF_IIF_STATUS1
          { UNSG32 u32;
            struct w32IIF_IIF_STATUS1;
                 } T32IIF_IIF_STATUS1;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095;
    typedef union  T32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127
          { UNSG32 u32;
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127;
                 } T32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095;
    typedef union  T32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127
          { UNSG32 u32;
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127;
                 } T32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127;
    typedef union  T32IIF_IIF_DATA_HEADER_VAL_000_031
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_VAL_000_031;
                 } T32IIF_IIF_DATA_HEADER_VAL_000_031;
    typedef union  T32IIF_IIF_DATA_HEADER_VAL_032_063
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_VAL_032_063;
                 } T32IIF_IIF_DATA_HEADER_VAL_032_063;
    typedef union  T32IIF_IIF_DATA_HEADER_VAL_064_095
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_VAL_064_095;
                 } T32IIF_IIF_DATA_HEADER_VAL_064_095;
    typedef union  T32IIF_IIF_DATA_HEADER_VAL_096_127
          { UNSG32 u32;
            struct w32IIF_IIF_DATA_HEADER_VAL_096_127;
                 } T32IIF_IIF_DATA_HEADER_VAL_096_127;
    typedef union  T32IIF_IIF_ERROR_EN_CFG
          { UNSG32 u32;
            struct w32IIF_IIF_ERROR_EN_CFG;
                 } T32IIF_IIF_ERROR_EN_CFG;
    typedef union  T32IIF_IIF_STATUS
          { UNSG32 u32;
            struct w32IIF_IIF_STATUS;
                 } T32IIF_IIF_STATUS;
    typedef union  TIIF_IIF_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_CFG0;
                   };
                 } TIIF_IIF_CFG0;
    typedef union  TIIF_IIF_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_CFG1;
                   };
                 } TIIF_IIF_CFG1;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_VALUE_000_031_CFG;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_VALUE_032_063_CFG;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_VALUE_064_095_CFG;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_VALUE_096_127_CFG;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_VALUE_000_031_CFG;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_VALUE_032_063_CFG;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_VALUE_064_095_CFG;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_VALUE_096_127_CFG;
    typedef union  TIIF_IIF_DATA_HEADER_VALUE_000_031_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_VALUE_000_031_CFG;
                   };
                 } TIIF_IIF_DATA_HEADER_VALUE_000_031_CFG;
    typedef union  TIIF_IIF_DATA_HEADER_VALUE_032_063_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_VALUE_032_063_CFG;
                   };
                 } TIIF_IIF_DATA_HEADER_VALUE_032_063_CFG;
    typedef union  TIIF_IIF_DATA_HEADER_VALUE_064_095_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_VALUE_064_095_CFG;
                   };
                 } TIIF_IIF_DATA_HEADER_VALUE_064_095_CFG;
    typedef union  TIIF_IIF_DATA_HEADER_VALUE_096_127_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_VALUE_096_127_CFG;
                   };
                 } TIIF_IIF_DATA_HEADER_VALUE_096_127_CFG;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_MASK_000_031_CFG;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_MASK_032_063_CFG;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_MASK_064_095_CFG;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_MASK_096_127_CFG;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_MASK_000_031_CFG;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_MASK_032_063_CFG;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_MASK_064_095_CFG;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_MASK_096_127_CFG;
    typedef union  TIIF_IIF_DATA_HEADER_MASK_000_031_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_MASK_000_031_CFG;
                   };
                 } TIIF_IIF_DATA_HEADER_MASK_000_031_CFG;
    typedef union  TIIF_IIF_DATA_HEADER_MASK_032_063_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_MASK_032_063_CFG;
                   };
                 } TIIF_IIF_DATA_HEADER_MASK_032_063_CFG;
    typedef union  TIIF_IIF_DATA_HEADER_MASK_064_095_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_MASK_064_095_CFG;
                   };
                 } TIIF_IIF_DATA_HEADER_MASK_064_095_CFG;
    typedef union  TIIF_IIF_DATA_HEADER_MASK_096_127_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_MASK_096_127_CFG;
                   };
                 } TIIF_IIF_DATA_HEADER_MASK_096_127_CFG;
    typedef union  TIIF_IIF_STATUS0
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_STATUS0;
                   };
                 } TIIF_IIF_STATUS0;
    typedef union  TIIF_IIF_STATUS1
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_STATUS1;
                   };
                 } TIIF_IIF_STATUS1;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_VAL_000_031
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_000_031;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_VAL_000_031;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_VAL_032_063
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_032_063;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_VAL_032_063;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_VAL_064_095
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_064_095;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_VAL_064_095;
    typedef union  TIIF_IIF_START_OF_FRAME_HEADER_VAL_096_127
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_START_OF_FRAME_HEADER_VAL_096_127;
                   };
                 } TIIF_IIF_START_OF_FRAME_HEADER_VAL_096_127;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_VAL_000_031
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_000_031;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_VAL_000_031;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_VAL_032_063
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_032_063;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_VAL_032_063;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_VAL_064_095
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_064_095;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_VAL_064_095;
    typedef union  TIIF_IIF_END_OF_FRAME_HEADER_VAL_096_127
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_END_OF_FRAME_HEADER_VAL_096_127;
                   };
                 } TIIF_IIF_END_OF_FRAME_HEADER_VAL_096_127;
    typedef union  TIIF_IIF_DATA_HEADER_VAL_000_031
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_VAL_000_031;
                   };
                 } TIIF_IIF_DATA_HEADER_VAL_000_031;
    typedef union  TIIF_IIF_DATA_HEADER_VAL_032_063
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_VAL_032_063;
                   };
                 } TIIF_IIF_DATA_HEADER_VAL_032_063;
    typedef union  TIIF_IIF_DATA_HEADER_VAL_064_095
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_VAL_064_095;
                   };
                 } TIIF_IIF_DATA_HEADER_VAL_064_095;
    typedef union  TIIF_IIF_DATA_HEADER_VAL_096_127
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_DATA_HEADER_VAL_096_127;
                   };
                 } TIIF_IIF_DATA_HEADER_VAL_096_127;
    typedef union  TIIF_IIF_ERROR_EN_CFG
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_ERROR_EN_CFG;
                   };
                 } TIIF_IIF_ERROR_EN_CFG;
    typedef union  TIIF_IIF_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32IIF_IIF_STATUS;
                   };
                 } TIIF_IIF_STATUS;
     SIGN32 IIF_drvrd(SIE_IIF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 IIF_drvwr(SIE_IIF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void IIF_reset(SIE_IIF *p);
     SIGN32 IIF_cmp  (SIE_IIF *p, SIE_IIF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define IIF_check(p,pie,pfx,hLOG) IIF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define IIF_print(p,    pfx,hLOG) IIF_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_HOST2DHUB
#define h_HOST2DHUB (){}
    #define     RA_HOST2DHUB_IMAGERESWRAP                      0x0000
    #define     RA_HOST2DHUB_FVF                               0x0020
    #define     RA_HOST2DHUB_WB                                0x0040
    #define     RA_HOST2DHUB_DEMOSAIC                          0x0048
    #define     RA_HOST2DHUB_CSCDNS                            0x004C
    #define     RA_HOST2DHUB_WrClient_Y                        0x00EC
    #define     RA_HOST2DHUB_WrClient_C                        0x00FC
    #define     RA_HOST2DHUB_TG_CTRL                           0x010C
    #define   LSb32HOST2DHUB_TG_CTRL_tg_start                     0
    #define       bHOST2DHUB_TG_CTRL_tg_start                  1
    #define   MSK32HOST2DHUB_TG_CTRL_tg_start                     0x00000001
    #define   LSb32HOST2DHUB_TG_CTRL_tg_clear                     1
    #define       bHOST2DHUB_TG_CTRL_tg_clear                  1
    #define   MSK32HOST2DHUB_TG_CTRL_tg_clear                     0x00000002
    #define   LSb32HOST2DHUB_TG_CTRL_clken_ctrl0                  2
    #define       bHOST2DHUB_TG_CTRL_clken_ctrl0               1
    #define   MSK32HOST2DHUB_TG_CTRL_clken_ctrl0                  0x00000004
    #define   LSb32HOST2DHUB_TG_CTRL_clken_ctrl1                  3
    #define       bHOST2DHUB_TG_CTRL_clken_ctrl1               1
    #define   MSK32HOST2DHUB_TG_CTRL_clken_ctrl1                  0x00000008
    #define   LSb32HOST2DHUB_TG_CTRL_clken_ctrl2                  4
    #define       bHOST2DHUB_TG_CTRL_clken_ctrl2               1
    #define   MSK32HOST2DHUB_TG_CTRL_clken_ctrl2                  0x00000010
    #define   LSb32HOST2DHUB_TG_CTRL_clken_ctrl3                  5
    #define       bHOST2DHUB_TG_CTRL_clken_ctrl3               1
    #define   MSK32HOST2DHUB_TG_CTRL_clken_ctrl3                  0x00000020
    #define   LSb32HOST2DHUB_TG_CTRL_clken_ctrl4                  6
    #define       bHOST2DHUB_TG_CTRL_clken_ctrl4               1
    #define   MSK32HOST2DHUB_TG_CTRL_clken_ctrl4                  0x00000040
    #define   LSb32HOST2DHUB_TG_CTRL_clken_ctrl5                  7
    #define       bHOST2DHUB_TG_CTRL_clken_ctrl5               1
    #define   MSK32HOST2DHUB_TG_CTRL_clken_ctrl5                  0x00000080
    #define   LSb32HOST2DHUB_TG_CTRL_fifo_ctrlEn                  8
    #define       bHOST2DHUB_TG_CTRL_fifo_ctrlEn               1
    #define   MSK32HOST2DHUB_TG_CTRL_fifo_ctrlEn                  0x00000100
    #define   LSb32HOST2DHUB_TG_CTRL_zero_line_delay_en           9
    #define       bHOST2DHUB_TG_CTRL_zero_line_delay_en        1
    #define   MSK32HOST2DHUB_TG_CTRL_zero_line_delay_en           0x00000200
    #define     RA_HOST2DHUB_CLKEN_CTRL                        0x0110
    #define   LSb32HOST2DHUB_CLKEN_CTRL_imageRes_clken            0
    #define       bHOST2DHUB_CLKEN_CTRL_imageRes_clken         1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_imageRes_clken            0x00000001
    #define   LSb32HOST2DHUB_CLKEN_CTRL_imageRes_bypass_clken     1
    #define       bHOST2DHUB_CLKEN_CTRL_imageRes_bypass_clken  1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_imageRes_bypass_clken     0x00000002
    #define   LSb32HOST2DHUB_CLKEN_CTRL_image_crop_clken          2
    #define       bHOST2DHUB_CLKEN_CTRL_image_crop_clken       1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_image_crop_clken          0x00000004
    #define   LSb32HOST2DHUB_CLKEN_CTRL_wb_clken                  3
    #define       bHOST2DHUB_CLKEN_CTRL_wb_clken               1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_wb_clken                  0x00000008
    #define   LSb32HOST2DHUB_CLKEN_CTRL_demosaic_clken            4
    #define       bHOST2DHUB_CLKEN_CTRL_demosaic_clken         1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_demosaic_clken            0x00000010
    #define   LSb32HOST2DHUB_CLKEN_CTRL_csc_clken                 5
    #define       bHOST2DHUB_CLKEN_CTRL_csc_clken              1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_csc_clken                 0x00000020
    #define   LSb32HOST2DHUB_CLKEN_CTRL_dns444to422_clken         6
    #define       bHOST2DHUB_CLKEN_CTRL_dns444to422_clken      1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_dns444to422_clken         0x00000040
    #define   LSb32HOST2DHUB_CLKEN_CTRL_dns444to420_clken         7
    #define       bHOST2DHUB_CLKEN_CTRL_dns444to420_clken      1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_dns444to420_clken         0x00000080
    #define   LSb32HOST2DHUB_CLKEN_CTRL_WrClient_C_clken          8
    #define       bHOST2DHUB_CLKEN_CTRL_WrClient_C_clken       1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_WrClient_C_clken          0x00000100
    #define   LSb32HOST2DHUB_CLKEN_CTRL_FVF_clken                 9
    #define       bHOST2DHUB_CLKEN_CTRL_FVF_clken              1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_FVF_clken                 0x00000200
    #define   LSb32HOST2DHUB_CLKEN_CTRL_wb_clkg_dyn_en            10
    #define       bHOST2DHUB_CLKEN_CTRL_wb_clkg_dyn_en         1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_wb_clkg_dyn_en            0x00000400
    #define   LSb32HOST2DHUB_CLKEN_CTRL_demosaic_clkg_dyn_en      11
    #define       bHOST2DHUB_CLKEN_CTRL_demosaic_clkg_dyn_en   1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_demosaic_clkg_dyn_en      0x00000800
    #define   LSb32HOST2DHUB_CLKEN_CTRL_fifo_read_on_TG_ACTIVE_en    12
    #define       bHOST2DHUB_CLKEN_CTRL_fifo_read_on_TG_ACTIVE_en 1
    #define   MSK32HOST2DHUB_CLKEN_CTRL_fifo_read_on_TG_ACTIVE_en    0x00001000
    #define     RA_HOST2DHUB_CTRL1                             0x0114
    #define   LSb32HOST2DHUB_CTRL1_DEMOSAIC_SWIZZLE               0
    #define       bHOST2DHUB_CTRL1_DEMOSAIC_SWIZZLE            3
    #define   MSK32HOST2DHUB_CTRL1_DEMOSAIC_SWIZZLE               0x00000007
    #define   LSb32HOST2DHUB_CTRL1_CSC_IPI_swap_ctrl              3
    #define       bHOST2DHUB_CTRL1_CSC_IPI_swap_ctrl           1
    #define   MSK32HOST2DHUB_CTRL1_CSC_IPI_swap_ctrl              0x00000008
    #define   LSb32HOST2DHUB_CTRL1_CSC_IPI_swap_ctrl_lvl2         4
    #define       bHOST2DHUB_CTRL1_CSC_IPI_swap_ctrl_lvl2      2
    #define   MSK32HOST2DHUB_CTRL1_CSC_IPI_swap_ctrl_lvl2         0x00000030
    #define   LSb32HOST2DHUB_CTRL1_CSC_FIFO_wr_ctrl               6
    #define       bHOST2DHUB_CTRL1_CSC_FIFO_wr_ctrl            3
    #define   MSK32HOST2DHUB_CTRL1_CSC_FIFO_wr_ctrl               0x000001C0
    #define   LSb32HOST2DHUB_CTRL1_enable_halt_front              9
    #define       bHOST2DHUB_CTRL1_enable_halt_front           1
    #define   MSK32HOST2DHUB_CTRL1_enable_halt_front              0x00000200
    #define   LSb32HOST2DHUB_CTRL1_enable_halt_back               10
    #define       bHOST2DHUB_CTRL1_enable_halt_back            1
    #define   MSK32HOST2DHUB_CTRL1_enable_halt_back               0x00000400
    #define   LSb32HOST2DHUB_CTRL1_enable_csc                     11
    #define       bHOST2DHUB_CTRL1_enable_csc                  1
    #define   MSK32HOST2DHUB_CTRL1_enable_csc                     0x00000800
    #define   LSb32HOST2DHUB_CTRL1_init_val_444to422              12
    #define       bHOST2DHUB_CTRL1_init_val_444to422           1
    #define   MSK32HOST2DHUB_CTRL1_init_val_444to422              0x00001000
    #define   LSb32HOST2DHUB_CTRL1_pix_toggle_en_444to422         13
    #define       bHOST2DHUB_CTRL1_pix_toggle_en_444to422      1
    #define   MSK32HOST2DHUB_CTRL1_pix_toggle_en_444to422         0x00002000
    #define   LSb32HOST2DHUB_CTRL1_UV_swap_en_422to420            14
    #define       bHOST2DHUB_CTRL1_UV_swap_en_422to420         1
    #define   MSK32HOST2DHUB_CTRL1_UV_swap_en_422to420            0x00004000
    #define   LSb32HOST2DHUB_CTRL1_init_val_422to420              15
    #define       bHOST2DHUB_CTRL1_init_val_422to420           1
    #define   MSK32HOST2DHUB_CTRL1_init_val_422to420              0x00008000
    #define   LSb32HOST2DHUB_CTRL1_line_toggle_en_422to420        16
    #define       bHOST2DHUB_CTRL1_line_toggle_en_422to420     1
    #define   MSK32HOST2DHUB_CTRL1_line_toggle_en_422to420        0x00010000
    #define   LSb32HOST2DHUB_CTRL1_fifo_flush                     17
    #define       bHOST2DHUB_CTRL1_fifo_flush                  1
    #define   MSK32HOST2DHUB_CTRL1_fifo_flush                     0x00020000
    #define   LSb32HOST2DHUB_CTRL1_IMGRES_sram_pwr_ctl1           18
    #define       bHOST2DHUB_CTRL1_IMGRES_sram_pwr_ctl1        3
    #define   MSK32HOST2DHUB_CTRL1_IMGRES_sram_pwr_ctl1           0x001C0000
    #define   LSb32HOST2DHUB_CTRL1_IMGRES_sram_pwr_ctl2           21
    #define       bHOST2DHUB_CTRL1_IMGRES_sram_pwr_ctl2        3
    #define   MSK32HOST2DHUB_CTRL1_IMGRES_sram_pwr_ctl2           0x00E00000
    #define   LSb32HOST2DHUB_CTRL1_HS_sel_422to420                24
    #define       bHOST2DHUB_CTRL1_HS_sel_422to420             1
    #define   MSK32HOST2DHUB_CTRL1_HS_sel_422to420                0x01000000
    #define   LSb32HOST2DHUB_CTRL1_VS_sel_422to420                25
    #define       bHOST2DHUB_CTRL1_VS_sel_422to420             1
    #define   MSK32HOST2DHUB_CTRL1_VS_sel_422to420                0x02000000
    #define   LSb32HOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl1     26
    #define       bHOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl1  1
    #define   MSK32HOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl1     0x04000000
    #define   LSb32HOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl2     27
    #define       bHOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl2  1
    #define   MSK32HOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl2     0x08000000
    #define   LSb32HOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl3     28
    #define       bHOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl3  1
    #define   MSK32HOST2DHUB_CTRL1_ipi_420_dirw_mux_ctrl_lvl3     0x10000000
    #define   LSb32HOST2DHUB_CTRL1_enable_565_write               29
    #define       bHOST2DHUB_CTRL1_enable_565_write            1
    #define   MSK32HOST2DHUB_CTRL1_enable_565_write               0x20000000
    #define   LSb32HOST2DHUB_CTRL1_FVF_bypass                     30
    #define       bHOST2DHUB_CTRL1_FVF_bypass                  1
    #define   MSK32HOST2DHUB_CTRL1_FVF_bypass                     0x40000000
    #define     RA_HOST2DHUB_CTRL2                             0x0118
    #define   LSb32HOST2DHUB_CTRL2_wrClient_C_input_ctrl          0
    #define       bHOST2DHUB_CTRL2_wrClient_C_input_ctrl       3
    #define   MSK32HOST2DHUB_CTRL2_wrClient_C_input_ctrl          0x00000007
    #define   LSb32HOST2DHUB_CTRL2_wrClient_Y_input_ctrl          3
    #define       bHOST2DHUB_CTRL2_wrClient_Y_input_ctrl       4
    #define   MSK32HOST2DHUB_CTRL2_wrClient_Y_input_ctrl          0x00000078
    #define   LSb32HOST2DHUB_CTRL2_WrClient_clear_C               7
    #define       bHOST2DHUB_CTRL2_WrClient_clear_C            1
    #define   MSK32HOST2DHUB_CTRL2_WrClient_clear_C               0x00000080
    #define   LSb32HOST2DHUB_CTRL2_WrClient_start_C               8
    #define       bHOST2DHUB_CTRL2_WrClient_start_C            1
    #define   MSK32HOST2DHUB_CTRL2_WrClient_start_C               0x00000100
    #define   LSb32HOST2DHUB_CTRL2_WrClient_clear_Y               9
    #define       bHOST2DHUB_CTRL2_WrClient_clear_Y            1
    #define   MSK32HOST2DHUB_CTRL2_WrClient_clear_Y               0x00000200
    #define   LSb32HOST2DHUB_CTRL2_WrClient_start_Y               10
    #define       bHOST2DHUB_CTRL2_WrClient_start_Y            1
    #define   MSK32HOST2DHUB_CTRL2_WrClient_start_Y               0x00000400
    #define   LSb32HOST2DHUB_CTRL2_input_fifo_flush               11
    #define       bHOST2DHUB_CTRL2_input_fifo_flush            1
    #define   MSK32HOST2DHUB_CTRL2_input_fifo_flush               0x00000800
    #define   LSb32HOST2DHUB_CTRL2_endian_ctrl                    12
    #define       bHOST2DHUB_CTRL2_endian_ctrl                 1
    #define   MSK32HOST2DHUB_CTRL2_endian_ctrl                    0x00001000
    #define     RA_HOST2DHUB_CTRL3                             0x011C
    #define   LSb32HOST2DHUB_CTRL3_cscpix_tot                     0
    #define       bHOST2DHUB_CTRL3_cscpix_tot                  32
    #define   MSK32HOST2DHUB_CTRL3_cscpix_tot                     0xFFFFFFFF
    #define     RA_HOST2DHUB_CTRL4                             0x0120
    #define   LSb32HOST2DHUB_CTRL4_image_width                    0
    #define       bHOST2DHUB_CTRL4_image_width                 14
    #define   MSK32HOST2DHUB_CTRL4_image_width                    0x00003FFF
    #define   LSb32HOST2DHUB_CTRL4_image_height                   14
    #define       bHOST2DHUB_CTRL4_image_height                14
    #define   MSK32HOST2DHUB_CTRL4_image_height                   0x0FFFC000
    #define     RA_HOST2DHUB_CTRL5                             0x0124
    #define   LSb32HOST2DHUB_CTRL5_hblank                         0
    #define       bHOST2DHUB_CTRL5_hblank                      8
    #define   MSK32HOST2DHUB_CTRL5_hblank                         0x000000FF
    #define   LSb32HOST2DHUB_CTRL5_iif_mode                       8
    #define       bHOST2DHUB_CTRL5_iif_mode                    1
    #define   MSK32HOST2DHUB_CTRL5_iif_mode                       0x00000100
    #define     RA_HOST2DHUB_CTRL6                             0x0128
    #define   LSb32HOST2DHUB_CTRL6_image_crop_en                  0
    #define       bHOST2DHUB_CTRL6_image_crop_en               1
    #define   MSK32HOST2DHUB_CTRL6_image_crop_en                  0x00000001
    #define   LSb32HOST2DHUB_CTRL6_image_crop_in_sel              1
    #define       bHOST2DHUB_CTRL6_image_crop_in_sel           1
    #define   MSK32HOST2DHUB_CTRL6_image_crop_in_sel              0x00000002
    #define   LSb32HOST2DHUB_CTRL6_crop_left                      2
    #define       bHOST2DHUB_CTRL6_crop_left                   14
    #define   MSK32HOST2DHUB_CTRL6_crop_left                      0x0000FFFC
    #define   LSb32HOST2DHUB_CTRL6_crop_right                     16
    #define       bHOST2DHUB_CTRL6_crop_right                  14
    #define   MSK32HOST2DHUB_CTRL6_crop_right                     0x3FFF0000
    #define   LSb32HOST2DHUB_CTRL6_validate                       30
    #define       bHOST2DHUB_CTRL6_validate                    1
    #define   MSK32HOST2DHUB_CTRL6_validate                       0x40000000
    #define     RA_HOST2DHUB_CTRL7                             0x012C
    #define   LSb32HOST2DHUB_CTRL7_crop_top                       0
    #define       bHOST2DHUB_CTRL7_crop_top                    14
    #define   MSK32HOST2DHUB_CTRL7_crop_top                       0x00003FFF
    #define   LSb32HOST2DHUB_CTRL7_crop_bot                       14
    #define       bHOST2DHUB_CTRL7_crop_bot                    14
    #define   MSK32HOST2DHUB_CTRL7_crop_bot                       0x0FFFC000
    typedef struct SIE_HOST2DHUB {
              SIE_IMAGERESWRAP                                 ie_IMAGERESWRAP;
              SIE_FVF                                          ie_FVF;
              SIE_WB                                           ie_WB;
              SIE_DEMOSAIC                                     ie_DEMOSAIC;
              SIE_CSCDNS                                       ie_CSCDNS;
              SIE_WriteClient                                  ie_WrClient_Y;
              SIE_WriteClient                                  ie_WrClient_C;
    #define     w32HOST2DHUB_TG_CTRL                           {\
            UNSG32 uTG_CTRL_tg_start                           :  1;\
            UNSG32 uTG_CTRL_tg_clear                           :  1;\
            UNSG32 uTG_CTRL_clken_ctrl0                        :  1;\
            UNSG32 uTG_CTRL_clken_ctrl1                        :  1;\
            UNSG32 uTG_CTRL_clken_ctrl2                        :  1;\
            UNSG32 uTG_CTRL_clken_ctrl3                        :  1;\
            UNSG32 uTG_CTRL_clken_ctrl4                        :  1;\
            UNSG32 uTG_CTRL_clken_ctrl5                        :  1;\
            UNSG32 uTG_CTRL_fifo_ctrlEn                        :  1;\
            UNSG32 uTG_CTRL_zero_line_delay_en                 :  1;\
            UNSG32 RSVDx10C_b10                                : 22;\
          }
    union { UNSG32 u32HOST2DHUB_TG_CTRL;
            struct w32HOST2DHUB_TG_CTRL;
          };
    #define     w32HOST2DHUB_CLKEN_CTRL                        {\
            UNSG32 uCLKEN_CTRL_imageRes_clken                  :  1;\
            UNSG32 uCLKEN_CTRL_imageRes_bypass_clken           :  1;\
            UNSG32 uCLKEN_CTRL_image_crop_clken                :  1;\
            UNSG32 uCLKEN_CTRL_wb_clken                        :  1;\
            UNSG32 uCLKEN_CTRL_demosaic_clken                  :  1;\
            UNSG32 uCLKEN_CTRL_csc_clken                       :  1;\
            UNSG32 uCLKEN_CTRL_dns444to422_clken               :  1;\
            UNSG32 uCLKEN_CTRL_dns444to420_clken               :  1;\
            UNSG32 uCLKEN_CTRL_WrClient_C_clken                :  1;\
            UNSG32 uCLKEN_CTRL_FVF_clken                       :  1;\
            UNSG32 uCLKEN_CTRL_wb_clkg_dyn_en                  :  1;\
            UNSG32 uCLKEN_CTRL_demosaic_clkg_dyn_en            :  1;\
            UNSG32 uCLKEN_CTRL_fifo_read_on_TG_ACTIVE_en       :  1;\
            UNSG32 RSVDx110_b13                                : 19;\
          }
    union { UNSG32 u32HOST2DHUB_CLKEN_CTRL;
            struct w32HOST2DHUB_CLKEN_CTRL;
          };
    #define     w32HOST2DHUB_CTRL1                             {\
            UNSG32 uCTRL1_DEMOSAIC_SWIZZLE                     :  3;\
            UNSG32 uCTRL1_CSC_IPI_swap_ctrl                    :  1;\
            UNSG32 uCTRL1_CSC_IPI_swap_ctrl_lvl2               :  2;\
            UNSG32 uCTRL1_CSC_FIFO_wr_ctrl                     :  3;\
            UNSG32 uCTRL1_enable_halt_front                    :  1;\
            UNSG32 uCTRL1_enable_halt_back                     :  1;\
            UNSG32 uCTRL1_enable_csc                           :  1;\
            UNSG32 uCTRL1_init_val_444to422                    :  1;\
            UNSG32 uCTRL1_pix_toggle_en_444to422               :  1;\
            UNSG32 uCTRL1_UV_swap_en_422to420                  :  1;\
            UNSG32 uCTRL1_init_val_422to420                    :  1;\
            UNSG32 uCTRL1_line_toggle_en_422to420              :  1;\
            UNSG32 uCTRL1_fifo_flush                           :  1;\
            UNSG32 uCTRL1_IMGRES_sram_pwr_ctl1                 :  3;\
            UNSG32 uCTRL1_IMGRES_sram_pwr_ctl2                 :  3;\
            UNSG32 uCTRL1_HS_sel_422to420                      :  1;\
            UNSG32 uCTRL1_VS_sel_422to420                      :  1;\
            UNSG32 uCTRL1_ipi_420_dirw_mux_ctrl_lvl1           :  1;\
            UNSG32 uCTRL1_ipi_420_dirw_mux_ctrl_lvl2           :  1;\
            UNSG32 uCTRL1_ipi_420_dirw_mux_ctrl_lvl3           :  1;\
            UNSG32 uCTRL1_enable_565_write                     :  1;\
            UNSG32 uCTRL1_FVF_bypass                           :  1;\
            UNSG32 RSVDx114_b31                                :  1;\
          }
    union { UNSG32 u32HOST2DHUB_CTRL1;
            struct w32HOST2DHUB_CTRL1;
          };
    #define     w32HOST2DHUB_CTRL2                             {\
            UNSG32 uCTRL2_wrClient_C_input_ctrl                :  3;\
            UNSG32 uCTRL2_wrClient_Y_input_ctrl                :  4;\
            UNSG32 uCTRL2_WrClient_clear_C                     :  1;\
            UNSG32 uCTRL2_WrClient_start_C                     :  1;\
            UNSG32 uCTRL2_WrClient_clear_Y                     :  1;\
            UNSG32 uCTRL2_WrClient_start_Y                     :  1;\
            UNSG32 uCTRL2_input_fifo_flush                     :  1;\
            UNSG32 uCTRL2_endian_ctrl                          :  1;\
            UNSG32 RSVDx118_b13                                : 19;\
          }
    union { UNSG32 u32HOST2DHUB_CTRL2;
            struct w32HOST2DHUB_CTRL2;
          };
    #define     w32HOST2DHUB_CTRL3                             {\
            UNSG32 uCTRL3_cscpix_tot                           : 32;\
          }
    union { UNSG32 u32HOST2DHUB_CTRL3;
            struct w32HOST2DHUB_CTRL3;
          };
    #define     w32HOST2DHUB_CTRL4                             {\
            UNSG32 uCTRL4_image_width                          : 14;\
            UNSG32 uCTRL4_image_height                         : 14;\
            UNSG32 RSVDx120_b28                                :  4;\
          }
    union { UNSG32 u32HOST2DHUB_CTRL4;
            struct w32HOST2DHUB_CTRL4;
          };
    #define     w32HOST2DHUB_CTRL5                             {\
            UNSG32 uCTRL5_hblank                               :  8;\
            UNSG32 uCTRL5_iif_mode                             :  1;\
            UNSG32 RSVDx124_b9                                 : 23;\
          }
    union { UNSG32 u32HOST2DHUB_CTRL5;
            struct w32HOST2DHUB_CTRL5;
          };
    #define     w32HOST2DHUB_CTRL6                             {\
            UNSG32 uCTRL6_image_crop_en                        :  1;\
            UNSG32 uCTRL6_image_crop_in_sel                    :  1;\
            UNSG32 uCTRL6_crop_left                            : 14;\
            UNSG32 uCTRL6_crop_right                           : 14;\
            UNSG32 uCTRL6_validate                             :  1;\
            UNSG32 RSVDx128_b31                                :  1;\
          }
    union { UNSG32 u32HOST2DHUB_CTRL6;
            struct w32HOST2DHUB_CTRL6;
          };
    #define     w32HOST2DHUB_CTRL7                             {\
            UNSG32 uCTRL7_crop_top                             : 14;\
            UNSG32 uCTRL7_crop_bot                             : 14;\
            UNSG32 RSVDx12C_b28                                :  4;\
          }
    union { UNSG32 u32HOST2DHUB_CTRL7;
            struct w32HOST2DHUB_CTRL7;
          };
    } SIE_HOST2DHUB;
    typedef union  T32HOST2DHUB_TG_CTRL
          { UNSG32 u32;
            struct w32HOST2DHUB_TG_CTRL;
                 } T32HOST2DHUB_TG_CTRL;
    typedef union  T32HOST2DHUB_CLKEN_CTRL
          { UNSG32 u32;
            struct w32HOST2DHUB_CLKEN_CTRL;
                 } T32HOST2DHUB_CLKEN_CTRL;
    typedef union  T32HOST2DHUB_CTRL1
          { UNSG32 u32;
            struct w32HOST2DHUB_CTRL1;
                 } T32HOST2DHUB_CTRL1;
    typedef union  T32HOST2DHUB_CTRL2
          { UNSG32 u32;
            struct w32HOST2DHUB_CTRL2;
                 } T32HOST2DHUB_CTRL2;
    typedef union  T32HOST2DHUB_CTRL3
          { UNSG32 u32;
            struct w32HOST2DHUB_CTRL3;
                 } T32HOST2DHUB_CTRL3;
    typedef union  T32HOST2DHUB_CTRL4
          { UNSG32 u32;
            struct w32HOST2DHUB_CTRL4;
                 } T32HOST2DHUB_CTRL4;
    typedef union  T32HOST2DHUB_CTRL5
          { UNSG32 u32;
            struct w32HOST2DHUB_CTRL5;
                 } T32HOST2DHUB_CTRL5;
    typedef union  T32HOST2DHUB_CTRL6
          { UNSG32 u32;
            struct w32HOST2DHUB_CTRL6;
                 } T32HOST2DHUB_CTRL6;
    typedef union  T32HOST2DHUB_CTRL7
          { UNSG32 u32;
            struct w32HOST2DHUB_CTRL7;
                 } T32HOST2DHUB_CTRL7;
    typedef union  THOST2DHUB_TG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_TG_CTRL;
                   };
                 } THOST2DHUB_TG_CTRL;
    typedef union  THOST2DHUB_CLKEN_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_CLKEN_CTRL;
                   };
                 } THOST2DHUB_CLKEN_CTRL;
    typedef union  THOST2DHUB_CTRL1
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_CTRL1;
                   };
                 } THOST2DHUB_CTRL1;
    typedef union  THOST2DHUB_CTRL2
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_CTRL2;
                   };
                 } THOST2DHUB_CTRL2;
    typedef union  THOST2DHUB_CTRL3
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_CTRL3;
                   };
                 } THOST2DHUB_CTRL3;
    typedef union  THOST2DHUB_CTRL4
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_CTRL4;
                   };
                 } THOST2DHUB_CTRL4;
    typedef union  THOST2DHUB_CTRL5
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_CTRL5;
                   };
                 } THOST2DHUB_CTRL5;
    typedef union  THOST2DHUB_CTRL6
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_CTRL6;
                   };
                 } THOST2DHUB_CTRL6;
    typedef union  THOST2DHUB_CTRL7
          { UNSG32 u32[1];
            struct {
            struct w32HOST2DHUB_CTRL7;
                   };
                 } THOST2DHUB_CTRL7;
     SIGN32 HOST2DHUB_drvrd(SIE_HOST2DHUB *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 HOST2DHUB_drvwr(SIE_HOST2DHUB *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void HOST2DHUB_reset(SIE_HOST2DHUB *p);
     SIGN32 HOST2DHUB_cmp  (SIE_HOST2DHUB *p, SIE_HOST2DHUB *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define HOST2DHUB_check(p,pie,pfx,hLOG) HOST2DHUB_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define HOST2DHUB_print(p,    pfx,hLOG) HOST2DHUB_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_CSIPIPE
#define h_CSIPIPE (){}
    #define     RA_CSIPIPE_IIF                                 0x0000
    #define     RA_CSIPIPE_HOST2DHUB1                          0x00A8
    #define     RA_CSIPIPE_HOST2DHUB2                          0x01D8
    #define     RA_CSIPIPE_CTRL                                0x0308
    #define   LSb32CSIPIPE_CTRL_HC0_ipi_halt_source               0
    #define       bCSIPIPE_CTRL_HC0_ipi_halt_source            2
    #define   MSK32CSIPIPE_CTRL_HC0_ipi_halt_source               0x00000003
    #define   LSb32CSIPIPE_CTRL_HC1_ipi_halt_source               2
    #define       bCSIPIPE_CTRL_HC1_ipi_halt_source            2
    #define   MSK32CSIPIPE_CTRL_HC1_ipi_halt_source               0x0000000C
    #define   LSb32CSIPIPE_CTRL_IIF_clken                         4
    #define       bCSIPIPE_CTRL_IIF_clken                      1
    #define   MSK32CSIPIPE_CTRL_IIF_clken                         0x00000010
    #define   LSb32CSIPIPE_CTRL_IIF_FVF_error_clear               5
    #define       bCSIPIPE_CTRL_IIF_FVF_error_clear            1
    #define   MSK32CSIPIPE_CTRL_IIF_FVF_error_clear               0x00000020
    #define   LSb32CSIPIPE_CTRL_IIF_FVF_error_auto_clear          6
    #define       bCSIPIPE_CTRL_IIF_FVF_error_auto_clear       1
    #define   MSK32CSIPIPE_CTRL_IIF_FVF_error_auto_clear          0x00000040
    #define     RA_CSIPIPE_VIDEO_MUTE                          0x030C
    #define   LSb32CSIPIPE_VIDEO_MUTE_DEB_DELAY                   0
    #define       bCSIPIPE_VIDEO_MUTE_DEB_DELAY                8
    #define   MSK32CSIPIPE_VIDEO_MUTE_DEB_DELAY                   0x000000FF
    #define   LSb32CSIPIPE_VIDEO_MUTE_EFF_DELAY                   8
    #define       bCSIPIPE_VIDEO_MUTE_EFF_DELAY                8
    #define   MSK32CSIPIPE_VIDEO_MUTE_EFF_DELAY                   0x0000FF00
    #define   LSb32CSIPIPE_VIDEO_MUTE_SW_MUTE_H0                  16
    #define       bCSIPIPE_VIDEO_MUTE_SW_MUTE_H0               1
    #define   MSK32CSIPIPE_VIDEO_MUTE_SW_MUTE_H0                  0x00010000
    #define   LSb32CSIPIPE_VIDEO_MUTE_SW_MUTE_H1                  17
    #define       bCSIPIPE_VIDEO_MUTE_SW_MUTE_H1               1
    #define   MSK32CSIPIPE_VIDEO_MUTE_SW_MUTE_H1                  0x00020000
    #define   LSb32CSIPIPE_VIDEO_MUTE_polarity_sel                18
    #define       bCSIPIPE_VIDEO_MUTE_polarity_sel             1
    #define   MSK32CSIPIPE_VIDEO_MUTE_polarity_sel                0x00040000
    #define   LSb32CSIPIPE_VIDEO_MUTE_SW_MUTE_H2                  19
    #define       bCSIPIPE_VIDEO_MUTE_SW_MUTE_H2               1
    #define   MSK32CSIPIPE_VIDEO_MUTE_SW_MUTE_H2                  0x00080000
    #define     RA_CSIPIPE_TPOINT0_CFG                         0x0310
    #define   LSb32CSIPIPE_TPOINT0_CFG_TPOINT0_CTL                0
    #define       bCSIPIPE_TPOINT0_CFG_TPOINT0_CTL             8
    #define   MSK32CSIPIPE_TPOINT0_CFG_TPOINT0_CTL                0x000000FF
    #define     RA_CSIPIPE_TPOINT1_CFG                         0x0314
    #define   LSb32CSIPIPE_TPOINT1_CFG_TPOINT1_CTL                0
    #define       bCSIPIPE_TPOINT1_CFG_TPOINT1_CTL             8
    #define   MSK32CSIPIPE_TPOINT1_CFG_TPOINT1_CTL                0x000000FF
    #define     RA_CSIPIPE_TPOINT2_CFG                         0x0318
    #define   LSb32CSIPIPE_TPOINT2_CFG_TPOINT2_CTL                0
    #define       bCSIPIPE_TPOINT2_CFG_TPOINT2_CTL             8
    #define   MSK32CSIPIPE_TPOINT2_CFG_TPOINT2_CTL                0x000000FF
    #define     RA_CSIPIPE_TPOINT3_CFG                         0x031C
    #define   LSb32CSIPIPE_TPOINT3_CFG_TPOINT3_CTL                0
    #define       bCSIPIPE_TPOINT3_CFG_TPOINT3_CTL             8
    #define   MSK32CSIPIPE_TPOINT3_CFG_TPOINT3_CTL                0x000000FF
    #define     RA_CSIPIPE_TPOINT4_CFG                         0x0320
    #define   LSb32CSIPIPE_TPOINT4_CFG_TPOINT4_CTL                0
    #define       bCSIPIPE_TPOINT4_CFG_TPOINT4_CTL             8
    #define   MSK32CSIPIPE_TPOINT4_CFG_TPOINT4_CTL                0x000000FF
    #define     RA_CSIPIPE_TPOINT5_CFG                         0x0324
    #define   LSb32CSIPIPE_TPOINT5_CFG_TPOINT5_CTL                0
    #define       bCSIPIPE_TPOINT5_CFG_TPOINT5_CTL             8
    #define   MSK32CSIPIPE_TPOINT5_CFG_TPOINT5_CTL                0x000000FF
    #define     RA_CSIPIPE_TPOINT6_CFG                         0x0328
    #define   LSb32CSIPIPE_TPOINT6_CFG_TPOINT6_CTL                0
    #define       bCSIPIPE_TPOINT6_CFG_TPOINT6_CTL             8
    #define   MSK32CSIPIPE_TPOINT6_CFG_TPOINT6_CTL                0x000000FF
    #define     RA_CSIPIPE_TPOINT7_CFG                         0x032C
    #define   LSb32CSIPIPE_TPOINT7_CFG_TPOINT7_CTL                0
    #define       bCSIPIPE_TPOINT7_CFG_TPOINT7_CTL             8
    #define   MSK32CSIPIPE_TPOINT7_CFG_TPOINT7_CTL                0x000000FF
    #define     RA_CSIPIPE_STATUS0                             0x0330
    #define   LSb32CSIPIPE_STATUS0_CSC_in_fifo_overflow_h2dh1     0
    #define       bCSIPIPE_STATUS0_CSC_in_fifo_overflow_h2dh1  1
    #define   MSK32CSIPIPE_STATUS0_CSC_in_fifo_overflow_h2dh1     0x00000001
    #define   LSb32CSIPIPE_STATUS0_CSC_in_fifo_overflow_h2dh2     1
    #define       bCSIPIPE_STATUS0_CSC_in_fifo_overflow_h2dh2  1
    #define   MSK32CSIPIPE_STATUS0_CSC_in_fifo_overflow_h2dh2     0x00000002
    #define   LSb32CSIPIPE_STATUS0_CSC_in_fifo_underflow_h2dh1    2
    #define       bCSIPIPE_STATUS0_CSC_in_fifo_underflow_h2dh1 1
    #define   MSK32CSIPIPE_STATUS0_CSC_in_fifo_underflow_h2dh1    0x00000004
    #define   LSb32CSIPIPE_STATUS0_CSC_in_fifo_underflow_h2dh2    3
    #define       bCSIPIPE_STATUS0_CSC_in_fifo_underflow_h2dh2 1
    #define   MSK32CSIPIPE_STATUS0_CSC_in_fifo_underflow_h2dh2    0x00000008
    typedef struct SIE_CSIPIPE {
              SIE_IIF                                          ie_IIF;
              SIE_HOST2DHUB                                    ie_HOST2DHUB1;
              SIE_HOST2DHUB                                    ie_HOST2DHUB2;
    #define     w32CSIPIPE_CTRL                                {\
            UNSG32 uCTRL_HC0_ipi_halt_source                   :  2;\
            UNSG32 uCTRL_HC1_ipi_halt_source                   :  2;\
            UNSG32 uCTRL_IIF_clken                             :  1;\
            UNSG32 uCTRL_IIF_FVF_error_clear                   :  1;\
            UNSG32 uCTRL_IIF_FVF_error_auto_clear              :  1;\
            UNSG32 RSVDx308_b7                                 : 25;\
          }
    union { UNSG32 u32CSIPIPE_CTRL;
            struct w32CSIPIPE_CTRL;
          };
    #define     w32CSIPIPE_VIDEO_MUTE                          {\
            UNSG32 uVIDEO_MUTE_DEB_DELAY                       :  8;\
            UNSG32 uVIDEO_MUTE_EFF_DELAY                       :  8;\
            UNSG32 uVIDEO_MUTE_SW_MUTE_H0                      :  1;\
            UNSG32 uVIDEO_MUTE_SW_MUTE_H1                      :  1;\
            UNSG32 uVIDEO_MUTE_polarity_sel                    :  1;\
            UNSG32 uVIDEO_MUTE_SW_MUTE_H2                      :  1;\
            UNSG32 RSVDx30C_b20                                : 12;\
          }
    union { UNSG32 u32CSIPIPE_VIDEO_MUTE;
            struct w32CSIPIPE_VIDEO_MUTE;
          };
    #define     w32CSIPIPE_TPOINT0_CFG                         {\
            UNSG32 uTPOINT0_CFG_TPOINT0_CTL                    :  8;\
            UNSG32 RSVDx310_b8                                 : 24;\
          }
    union { UNSG32 u32CSIPIPE_TPOINT0_CFG;
            struct w32CSIPIPE_TPOINT0_CFG;
          };
    #define     w32CSIPIPE_TPOINT1_CFG                         {\
            UNSG32 uTPOINT1_CFG_TPOINT1_CTL                    :  8;\
            UNSG32 RSVDx314_b8                                 : 24;\
          }
    union { UNSG32 u32CSIPIPE_TPOINT1_CFG;
            struct w32CSIPIPE_TPOINT1_CFG;
          };
    #define     w32CSIPIPE_TPOINT2_CFG                         {\
            UNSG32 uTPOINT2_CFG_TPOINT2_CTL                    :  8;\
            UNSG32 RSVDx318_b8                                 : 24;\
          }
    union { UNSG32 u32CSIPIPE_TPOINT2_CFG;
            struct w32CSIPIPE_TPOINT2_CFG;
          };
    #define     w32CSIPIPE_TPOINT3_CFG                         {\
            UNSG32 uTPOINT3_CFG_TPOINT3_CTL                    :  8;\
            UNSG32 RSVDx31C_b8                                 : 24;\
          }
    union { UNSG32 u32CSIPIPE_TPOINT3_CFG;
            struct w32CSIPIPE_TPOINT3_CFG;
          };
    #define     w32CSIPIPE_TPOINT4_CFG                         {\
            UNSG32 uTPOINT4_CFG_TPOINT4_CTL                    :  8;\
            UNSG32 RSVDx320_b8                                 : 24;\
          }
    union { UNSG32 u32CSIPIPE_TPOINT4_CFG;
            struct w32CSIPIPE_TPOINT4_CFG;
          };
    #define     w32CSIPIPE_TPOINT5_CFG                         {\
            UNSG32 uTPOINT5_CFG_TPOINT5_CTL                    :  8;\
            UNSG32 RSVDx324_b8                                 : 24;\
          }
    union { UNSG32 u32CSIPIPE_TPOINT5_CFG;
            struct w32CSIPIPE_TPOINT5_CFG;
          };
    #define     w32CSIPIPE_TPOINT6_CFG                         {\
            UNSG32 uTPOINT6_CFG_TPOINT6_CTL                    :  8;\
            UNSG32 RSVDx328_b8                                 : 24;\
          }
    union { UNSG32 u32CSIPIPE_TPOINT6_CFG;
            struct w32CSIPIPE_TPOINT6_CFG;
          };
    #define     w32CSIPIPE_TPOINT7_CFG                         {\
            UNSG32 uTPOINT7_CFG_TPOINT7_CTL                    :  8;\
            UNSG32 RSVDx32C_b8                                 : 24;\
          }
    union { UNSG32 u32CSIPIPE_TPOINT7_CFG;
            struct w32CSIPIPE_TPOINT7_CFG;
          };
    #define     w32CSIPIPE_STATUS0                             {\
            UNSG32 uSTATUS0_CSC_in_fifo_overflow_h2dh1         :  1;\
            UNSG32 uSTATUS0_CSC_in_fifo_overflow_h2dh2         :  1;\
            UNSG32 uSTATUS0_CSC_in_fifo_underflow_h2dh1        :  1;\
            UNSG32 uSTATUS0_CSC_in_fifo_underflow_h2dh2        :  1;\
            UNSG32 RSVDx330_b4                                 : 28;\
          }
    union { UNSG32 u32CSIPIPE_STATUS0;
            struct w32CSIPIPE_STATUS0;
          };
    } SIE_CSIPIPE;
    typedef union  T32CSIPIPE_CTRL
          { UNSG32 u32;
            struct w32CSIPIPE_CTRL;
                 } T32CSIPIPE_CTRL;
    typedef union  T32CSIPIPE_VIDEO_MUTE
          { UNSG32 u32;
            struct w32CSIPIPE_VIDEO_MUTE;
                 } T32CSIPIPE_VIDEO_MUTE;
    typedef union  T32CSIPIPE_TPOINT0_CFG
          { UNSG32 u32;
            struct w32CSIPIPE_TPOINT0_CFG;
                 } T32CSIPIPE_TPOINT0_CFG;
    typedef union  T32CSIPIPE_TPOINT1_CFG
          { UNSG32 u32;
            struct w32CSIPIPE_TPOINT1_CFG;
                 } T32CSIPIPE_TPOINT1_CFG;
    typedef union  T32CSIPIPE_TPOINT2_CFG
          { UNSG32 u32;
            struct w32CSIPIPE_TPOINT2_CFG;
                 } T32CSIPIPE_TPOINT2_CFG;
    typedef union  T32CSIPIPE_TPOINT3_CFG
          { UNSG32 u32;
            struct w32CSIPIPE_TPOINT3_CFG;
                 } T32CSIPIPE_TPOINT3_CFG;
    typedef union  T32CSIPIPE_TPOINT4_CFG
          { UNSG32 u32;
            struct w32CSIPIPE_TPOINT4_CFG;
                 } T32CSIPIPE_TPOINT4_CFG;
    typedef union  T32CSIPIPE_TPOINT5_CFG
          { UNSG32 u32;
            struct w32CSIPIPE_TPOINT5_CFG;
                 } T32CSIPIPE_TPOINT5_CFG;
    typedef union  T32CSIPIPE_TPOINT6_CFG
          { UNSG32 u32;
            struct w32CSIPIPE_TPOINT6_CFG;
                 } T32CSIPIPE_TPOINT6_CFG;
    typedef union  T32CSIPIPE_TPOINT7_CFG
          { UNSG32 u32;
            struct w32CSIPIPE_TPOINT7_CFG;
                 } T32CSIPIPE_TPOINT7_CFG;
    typedef union  T32CSIPIPE_STATUS0
          { UNSG32 u32;
            struct w32CSIPIPE_STATUS0;
                 } T32CSIPIPE_STATUS0;
    typedef union  TCSIPIPE_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_CTRL;
                   };
                 } TCSIPIPE_CTRL;
    typedef union  TCSIPIPE_VIDEO_MUTE
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_VIDEO_MUTE;
                   };
                 } TCSIPIPE_VIDEO_MUTE;
    typedef union  TCSIPIPE_TPOINT0_CFG
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_TPOINT0_CFG;
                   };
                 } TCSIPIPE_TPOINT0_CFG;
    typedef union  TCSIPIPE_TPOINT1_CFG
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_TPOINT1_CFG;
                   };
                 } TCSIPIPE_TPOINT1_CFG;
    typedef union  TCSIPIPE_TPOINT2_CFG
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_TPOINT2_CFG;
                   };
                 } TCSIPIPE_TPOINT2_CFG;
    typedef union  TCSIPIPE_TPOINT3_CFG
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_TPOINT3_CFG;
                   };
                 } TCSIPIPE_TPOINT3_CFG;
    typedef union  TCSIPIPE_TPOINT4_CFG
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_TPOINT4_CFG;
                   };
                 } TCSIPIPE_TPOINT4_CFG;
    typedef union  TCSIPIPE_TPOINT5_CFG
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_TPOINT5_CFG;
                   };
                 } TCSIPIPE_TPOINT5_CFG;
    typedef union  TCSIPIPE_TPOINT6_CFG
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_TPOINT6_CFG;
                   };
                 } TCSIPIPE_TPOINT6_CFG;
    typedef union  TCSIPIPE_TPOINT7_CFG
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_TPOINT7_CFG;
                   };
                 } TCSIPIPE_TPOINT7_CFG;
    typedef union  TCSIPIPE_STATUS0
          { UNSG32 u32[1];
            struct {
            struct w32CSIPIPE_STATUS0;
                   };
                 } TCSIPIPE_STATUS0;
     SIGN32 CSIPIPE_drvrd(SIE_CSIPIPE *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 CSIPIPE_drvwr(SIE_CSIPIPE *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void CSIPIPE_reset(SIE_CSIPIPE *p);
     SIGN32 CSIPIPE_cmp  (SIE_CSIPIPE *p, SIE_CSIPIPE *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define CSIPIPE_check(p,pie,pfx,hLOG) CSIPIPE_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define CSIPIPE_print(p,    pfx,hLOG) CSIPIPE_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifdef __cplusplus
  }
#endif
#pragma  pack()
#endif
