#ifndef avioGbl_h
#define avioGbl_h (){}
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
    #define     w32abipll_ctrlC                                {\
            UNSG32 uctrlC_DIVR                                 :  6;\
            UNSG32 RSVDx8_b6                                   : 26;\
          }
    union { UNSG32 u32abipll_ctrlC;
            struct w32abipll_ctrlC;
          };
    #define     w32abipll_ctrlD                                {\
            UNSG32 uctrlD_DIVFI                                :  9;\
            UNSG32 RSVDxC_b9                                   : 23;\
          }
    union { UNSG32 u32abipll_ctrlD;
            struct w32abipll_ctrlD;
          };
    #define     w32abipll_ctrlE                                {\
            UNSG32 uctrlE_DIVFF                                : 24;\
            UNSG32 RSVDx10_b24                                 :  8;\
          }
    union { UNSG32 u32abipll_ctrlE;
            struct w32abipll_ctrlE;
          };
    #define     w32abipll_ctrlF                                {\
            UNSG32 uctrlF_DIVQ                                 :  5;\
            UNSG32 RSVDx14_b5                                  : 27;\
          }
    union { UNSG32 u32abipll_ctrlF;
            struct w32abipll_ctrlF;
          };
    #define     w32abipll_ctrlG                                {\
            UNSG32 uctrlG_DIVQF                                :  3;\
            UNSG32 RSVDx18_b3                                  : 29;\
          }
    union { UNSG32 u32abipll_ctrlG;
            struct w32abipll_ctrlG;
          };
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
#ifndef h_INT
#define h_INT (){}
    #define     RA_INT_EN                                      0x0000
    #define     RA_INT_STS                                     0x0004
    typedef struct SIE_INT {
    #define     w32INT_EN                                      {\
            UNSG32 uEN_bus                                     : 15;\
            UNSG32 RSVDx0_b15                                  : 17;\
          }
    union { UNSG32 u32INT_EN;
            struct w32INT_EN;
          };
    #define     w32INT_STS                                     {\
            UNSG32 uSTS_bus                                    : 15;\
            UNSG32 RSVDx4_b15                                  : 17;\
          }
    union { UNSG32 u32INT_STS;
            struct w32INT_STS;
          };
    } SIE_INT;
    typedef union  T32INT_EN
          { UNSG32 u32;
            struct w32INT_EN;
                 } T32INT_EN;
    typedef union  T32INT_STS
          { UNSG32 u32;
            struct w32INT_STS;
                 } T32INT_STS;
    typedef union  TINT_EN
          { UNSG32 u32[1];
            struct {
            struct w32INT_EN;
                   };
                 } TINT_EN;
    typedef union  TINT_STS
          { UNSG32 u32[1];
            struct {
            struct w32INT_STS;
                   };
                 } TINT_STS;
     SIGN32 INT_drvrd(SIE_INT *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 INT_drvwr(SIE_INT *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void INT_reset(SIE_INT *p);
     SIGN32 INT_cmp  (SIE_INT *p, SIE_INT *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define INT_check(p,pie,pfx,hLOG) INT_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define INT_print(p,    pfx,hLOG) INT_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_AVIO_debug_ctrl
#define h_AVIO_debug_ctrl (){}
    #define     RA_AVIO_debug_ctrl_Ctrl0                       0x0000
    typedef struct SIE_AVIO_debug_ctrl {
    #define     w32AVIO_debug_ctrl_Ctrl0                       {\
            UNSG32 uCtrl0_debug_ctrl0                          :  5;\
            UNSG32 RSVDx0_b5                                   : 27;\
          }
    union { UNSG32 u32AVIO_debug_ctrl_Ctrl0;
            struct w32AVIO_debug_ctrl_Ctrl0;
          };
    } SIE_AVIO_debug_ctrl;
    typedef union  T32AVIO_debug_ctrl_Ctrl0
          { UNSG32 u32;
            struct w32AVIO_debug_ctrl_Ctrl0;
                 } T32AVIO_debug_ctrl_Ctrl0;
    typedef union  TAVIO_debug_ctrl_Ctrl0
          { UNSG32 u32[1];
            struct {
            struct w32AVIO_debug_ctrl_Ctrl0;
                   };
                 } TAVIO_debug_ctrl_Ctrl0;
     SIGN32 AVIO_debug_ctrl_drvrd(SIE_AVIO_debug_ctrl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 AVIO_debug_ctrl_drvwr(SIE_AVIO_debug_ctrl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void AVIO_debug_ctrl_reset(SIE_AVIO_debug_ctrl *p);
     SIGN32 AVIO_debug_ctrl_cmp  (SIE_AVIO_debug_ctrl *p, SIE_AVIO_debug_ctrl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define AVIO_debug_ctrl_check(p,pie,pfx,hLOG) AVIO_debug_ctrl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define AVIO_debug_ctrl_print(p,    pfx,hLOG) AVIO_debug_ctrl_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_APLL_WRAP
#define h_APLL_WRAP (){}
    #define     RA_APLL_WRAP_APLL_CLK1_CTRL                    0x0000
    #define        APLL_WRAP_APLL_CLK1_CTRL_clkSel_d2                       0x1
    #define        APLL_WRAP_APLL_CLK1_CTRL_clkSel_d4                       0x2
    #define        APLL_WRAP_APLL_CLK1_CTRL_clkSel_d6                       0x3
    #define        APLL_WRAP_APLL_CLK1_CTRL_clkSel_d8                       0x4
    #define        APLL_WRAP_APLL_CLK1_CTRL_clkSel_d12                      0x5
    #define     RA_APLL_WRAP_CTRL0                             0x0004
    #define     RA_APLL_WRAP_APLL                              0x0008
    typedef struct SIE_APLL_WRAP {
    #define     w32APLL_WRAP_APLL_CLK1_CTRL                    {\
            UNSG32 uAPLL_CLK1_CTRL_clkSwitch                   :  1;\
            UNSG32 uAPLL_CLK1_CTRL_clkD3Switch                 :  1;\
            UNSG32 uAPLL_CLK1_CTRL_clkSel                      :  3;\
            UNSG32 uAPLL_CLK1_CTRL_clkEn                       :  1;\
            UNSG32 RSVDx0_b6                                   : 26;\
          }
    union { UNSG32 u32APLL_WRAP_APLL_CLK1_CTRL;
            struct w32APLL_WRAP_APLL_CLK1_CTRL;
          };
    #define     w32APLL_WRAP_CTRL0                             {\
            UNSG32 uCTRL0_clk_sel0                             :  4;\
            UNSG32 uCTRL0_clk_sel1                             :  1;\
            UNSG32 uCTRL0_clk_sel2                             :  2;\
            UNSG32 uCTRL0_clk_sel3                             :  1;\
            UNSG32 RSVDx4_b8                                   : 24;\
          }
    union { UNSG32 u32APLL_WRAP_CTRL0;
            struct w32APLL_WRAP_CTRL0;
          };
              SIE_abipll                                       ie_APLL;
    } SIE_APLL_WRAP;
    typedef union  T32APLL_WRAP_APLL_CLK1_CTRL
          { UNSG32 u32;
            struct w32APLL_WRAP_APLL_CLK1_CTRL;
                 } T32APLL_WRAP_APLL_CLK1_CTRL;
    typedef union  T32APLL_WRAP_CTRL0
          { UNSG32 u32;
            struct w32APLL_WRAP_CTRL0;
                 } T32APLL_WRAP_CTRL0;
    typedef union  TAPLL_WRAP_APLL_CLK1_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32APLL_WRAP_APLL_CLK1_CTRL;
                   };
                 } TAPLL_WRAP_APLL_CLK1_CTRL;
    typedef union  TAPLL_WRAP_CTRL0
          { UNSG32 u32[1];
            struct {
            struct w32APLL_WRAP_CTRL0;
                   };
                 } TAPLL_WRAP_CTRL0;
     SIGN32 APLL_WRAP_drvrd(SIE_APLL_WRAP *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 APLL_WRAP_drvwr(SIE_APLL_WRAP *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void APLL_WRAP_reset(SIE_APLL_WRAP *p);
     SIGN32 APLL_WRAP_cmp  (SIE_APLL_WRAP *p, SIE_APLL_WRAP *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define APLL_WRAP_check(p,pie,pfx,hLOG) APLL_WRAP_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define APLL_WRAP_print(p,    pfx,hLOG) APLL_WRAP_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_aioGbl
#define h_aioGbl (){}
    #define     RA_aioGbl_APLL0_WRAP                           0x0000
    #define     RA_aioGbl_APLL1_WRAP                           0x0028
    #define     RA_aioGbl_AVIO_debug_ctrl                      0x0050
    #define     RA_aioGbl_AVPLLA_CLK_EN                        0x0054
    #define     RA_aioGbl_SWPDWN_CTRL                          0x0058
    #define     RA_aioGbl_CTRL                                 0x005C
    #define     RA_aioGbl_CTRL0                                0x0060
    #define     RA_aioGbl_AIO64bDHUB_SRAMPWR                   0x0064
    #define     RA_aioGbl_SRAMRWTC                             0x0068
    #define     RA_aioGbl_GIC_INT0                             0x0074
    #define     RA_aioGbl_GIC_INT1                             0x007C
    #define     RA_aioGbl_GIC_INT2                             0x0084
    #define     RA_aioGbl_GIC_INT3                             0x008C
    #define     RA_aioGbl_GIC_INT4                             0x0094
    #define     RA_aioGbl_GIC_INT5                             0x009C
    #define     RA_aioGbl_GIC_INT6                             0x00A4
    #define     RA_aioGbl_GIC_INT7                             0x00AC
    #define     RA_aioGbl_GIC_INT8                             0x00B4
    #define     RA_aioGbl_GIC_INT9                             0x00BC
    #define     RA_aioGbl_GIC_INT10                            0x00C4
    #define     RA_aioGbl_GIC_INT11                            0x00CC
    #define     RA_aioGbl_GIC_INT12                            0x00D4
    #define     RA_aioGbl_GIC_INT13                            0x00DC
    #define     RA_aioGbl_GIC_INT                              0x00E4
    #define     RA_aioGbl_INTR_CTRL                            0x00E8
    #define     RA_aioGbl_SWRST_CTRL                           0x00EC
    #define     RA_aioGbl_PTRACK_CTRL                          0x00F0
    #define     RA_aioGbl_CG_CTRL                              0x00F4
    typedef struct SIE_aioGbl {
              SIE_APLL_WRAP                                    ie_APLL0_WRAP;
              SIE_APLL_WRAP                                    ie_APLL1_WRAP;
              SIE_AVIO_debug_ctrl                              ie_AVIO_debug_ctrl;
    #define     w32aioGbl_AVPLLA_CLK_EN                        {\
            UNSG32 uAVPLLA_CLK_EN_ctrl                         :  6;\
            UNSG32 uAVPLLA_CLK_EN_dbg_mux_sel                  :  1;\
            UNSG32 RSVDx54_b7                                  : 25;\
          }
    union { UNSG32 u32aioGbl_AVPLLA_CLK_EN;
            struct w32aioGbl_AVPLLA_CLK_EN;
          };
    #define     w32aioGbl_SWPDWN_CTRL                          {\
            UNSG32 uSWPDWN_CTRL_APLL0_PD                       :  1;\
            UNSG32 uSWPDWN_CTRL_APLL1_PD                       :  1;\
            UNSG32 RSVDx58_b2                                  : 30;\
          }
    union { UNSG32 u32aioGbl_SWPDWN_CTRL;
            struct w32aioGbl_SWPDWN_CTRL;
          };
    #define     w32aioGbl_CTRL                                 {\
            UNSG32 uCTRL_AIODHUB_dyCG_en                       :  1;\
            UNSG32 uCTRL_AIODHUB_CG_en                         :  1;\
            UNSG32 uCTRL_INTR_EN                               :  4;\
            UNSG32 uCTRL_BCM_FIFO_FLUSH                        :  1;\
            UNSG32 uCTRL_BCMQ_FIFO_FLUSH                       :  1;\
            UNSG32 RSVDx5C_b8                                  : 24;\
          }
    union { UNSG32 u32aioGbl_CTRL;
            struct w32aioGbl_CTRL;
          };
    #define     w32aioGbl_CTRL0                                {\
            UNSG32 uCTRL0_I2S1_MCLK_SEL                        :  3;\
            UNSG32 uCTRL0_I2S2_MCLK_SEL                        :  3;\
            UNSG32 uCTRL0_i2s1_mclk_inv                        :  1;\
            UNSG32 uCTRL0_i2s2_mclk_inv                        :  1;\
            UNSG32 uCTRL0_I2S1_MCLK_OEN                        :  1;\
            UNSG32 uCTRL0_I2S2_MCLK_OEN                        :  1;\
            UNSG32 uCTRL0_PDM_CLK_OEN                          :  1;\
            UNSG32 RSVDx60_b11                                 : 21;\
          }
    union { UNSG32 u32aioGbl_CTRL0;
            struct w32aioGbl_CTRL0;
          };
              SIE_SRAMPWR                                      ie_AIO64bDHUB_SRAMPWR;
              SIE_SRAMRWTC                                     ie_SRAMRWTC;
              SIE_INT                                          ie_GIC_INT0;
              SIE_INT                                          ie_GIC_INT1;
              SIE_INT                                          ie_GIC_INT2;
              SIE_INT                                          ie_GIC_INT3;
              SIE_INT                                          ie_GIC_INT4;
              SIE_INT                                          ie_GIC_INT5;
              SIE_INT                                          ie_GIC_INT6;
              SIE_INT                                          ie_GIC_INT7;
              SIE_INT                                          ie_GIC_INT8;
              SIE_INT                                          ie_GIC_INT9;
              SIE_INT                                          ie_GIC_INT10;
              SIE_INT                                          ie_GIC_INT11;
              SIE_INT                                          ie_GIC_INT12;
              SIE_INT                                          ie_GIC_INT13;
    #define     w32aioGbl_GIC_INT                              {\
            UNSG32 uGIC_INT_status                             : 15;\
            UNSG32 RSVDxE4_b15                                 : 17;\
          }
    union { UNSG32 u32aioGbl_GIC_INT;
            struct w32aioGbl_GIC_INT;
          };
    #define     w32aioGbl_INTR_CTRL                            {\
            UNSG32 uINTR_CTRL_ptrack1_int_en                   :  1;\
            UNSG32 uINTR_CTRL_ptrack2_int_en                   :  1;\
            UNSG32 RSVDxE8_b2                                  : 30;\
          }
    union { UNSG32 u32aioGbl_INTR_CTRL;
            struct w32aioGbl_INTR_CTRL;
          };
    #define     w32aioGbl_SWRST_CTRL                           {\
            UNSG32 uSWRST_CTRL_aioSyncRstn                     :  1;\
            UNSG32 uSWRST_CTRL_ptrack1SysSyncRstn              :  1;\
            UNSG32 uSWRST_CTRL_ptrack1SyncRstn                 :  1;\
            UNSG32 uSWRST_CTRL_ptrack2SysSyncRstn              :  1;\
            UNSG32 uSWRST_CTRL_ptrack2SyncRstn                 :  1;\
            UNSG32 RSVDxEC_b5                                  : 27;\
          }
    union { UNSG32 u32aioGbl_SWRST_CTRL;
            struct w32aioGbl_SWRST_CTRL;
          };
    #define     w32aioGbl_PTRACK_CTRL                          {\
            UNSG32 uPTRACK_CTRL_ptrack1_sysClk_en              :  1;\
            UNSG32 uPTRACK_CTRL_ptrack1_clk_en                 :  1;\
            UNSG32 uPTRACK_CTRL_ptrack2_sysClk_en              :  1;\
            UNSG32 uPTRACK_CTRL_ptrack2_clk_en                 :  1;\
            UNSG32 RSVDxF0_b4                                  : 28;\
          }
    union { UNSG32 u32aioGbl_PTRACK_CTRL;
            struct w32aioGbl_PTRACK_CTRL;
          };
    #define     w32aioGbl_CG_CTRL                              {\
            UNSG32 uCG_CTRL_spdifrx_avioFpll400_clk_en         :  1;\
            UNSG32 RSVDxF4_b1                                  : 31;\
          }
    union { UNSG32 u32aioGbl_CG_CTRL;
            struct w32aioGbl_CG_CTRL;
          };
    } SIE_aioGbl;
    typedef union  T32aioGbl_AVPLLA_CLK_EN
          { UNSG32 u32;
            struct w32aioGbl_AVPLLA_CLK_EN;
                 } T32aioGbl_AVPLLA_CLK_EN;
    typedef union  T32aioGbl_SWPDWN_CTRL
          { UNSG32 u32;
            struct w32aioGbl_SWPDWN_CTRL;
                 } T32aioGbl_SWPDWN_CTRL;
    typedef union  T32aioGbl_CTRL
          { UNSG32 u32;
            struct w32aioGbl_CTRL;
                 } T32aioGbl_CTRL;
    typedef union  T32aioGbl_CTRL0
          { UNSG32 u32;
            struct w32aioGbl_CTRL0;
                 } T32aioGbl_CTRL0;
    typedef union  T32aioGbl_GIC_INT
          { UNSG32 u32;
            struct w32aioGbl_GIC_INT;
                 } T32aioGbl_GIC_INT;
    typedef union  T32aioGbl_INTR_CTRL
          { UNSG32 u32;
            struct w32aioGbl_INTR_CTRL;
                 } T32aioGbl_INTR_CTRL;
    typedef union  T32aioGbl_SWRST_CTRL
          { UNSG32 u32;
            struct w32aioGbl_SWRST_CTRL;
                 } T32aioGbl_SWRST_CTRL;
    typedef union  T32aioGbl_PTRACK_CTRL
          { UNSG32 u32;
            struct w32aioGbl_PTRACK_CTRL;
                 } T32aioGbl_PTRACK_CTRL;
    typedef union  T32aioGbl_CG_CTRL
          { UNSG32 u32;
            struct w32aioGbl_CG_CTRL;
                 } T32aioGbl_CG_CTRL;
    typedef union  TaioGbl_AVPLLA_CLK_EN
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_AVPLLA_CLK_EN;
                   };
                 } TaioGbl_AVPLLA_CLK_EN;
    typedef union  TaioGbl_SWPDWN_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_SWPDWN_CTRL;
                   };
                 } TaioGbl_SWPDWN_CTRL;
    typedef union  TaioGbl_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_CTRL;
                   };
                 } TaioGbl_CTRL;
    typedef union  TaioGbl_CTRL0
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_CTRL0;
                   };
                 } TaioGbl_CTRL0;
    typedef union  TaioGbl_GIC_INT
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_GIC_INT;
                   };
                 } TaioGbl_GIC_INT;
    typedef union  TaioGbl_INTR_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_INTR_CTRL;
                   };
                 } TaioGbl_INTR_CTRL;
    typedef union  TaioGbl_SWRST_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_SWRST_CTRL;
                   };
                 } TaioGbl_SWRST_CTRL;
    typedef union  TaioGbl_PTRACK_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_PTRACK_CTRL;
                   };
                 } TaioGbl_PTRACK_CTRL;
    typedef union  TaioGbl_CG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32aioGbl_CG_CTRL;
                   };
                 } TaioGbl_CG_CTRL;
     SIGN32 aioGbl_drvrd(SIE_aioGbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 aioGbl_drvwr(SIE_aioGbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void aioGbl_reset(SIE_aioGbl *p);
     SIGN32 aioGbl_cmp  (SIE_aioGbl *p, SIE_aioGbl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define aioGbl_check(p,pie,pfx,hLOG) aioGbl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define aioGbl_print(p,    pfx,hLOG) aioGbl_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifdef __cplusplus
  }
#endif
#pragma  pack()
#endif
