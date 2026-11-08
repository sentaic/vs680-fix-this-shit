#ifndef vppGbl_h
#define vppGbl_h (){}
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
#ifndef h_DPHYTX
#define h_DPHYTX (){}
    #define     RA_DPHYTX_DPHY_CTL0                            0x0000
    #define     RA_DPHYTX_DPHY_CTL1                            0x0004
    #define     RA_DPHYTX_DPHY_CTL2                            0x0008
    #define     RA_DPHYTX_DPHY_CTL3                            0x000C
    #define     RA_DPHYTX_DPHY_CTL4                            0x0010
    #define     RA_DPHYTX_DPHY_CTL5                            0x0014
    #define     RA_DPHYTX_DPHY_CTL6                            0x0018
    #define     RA_DPHYTX_DPHY_CTL7                            0x001C
    #define     RA_DPHYTX_DPHY_CTL8                            0x0020
    #define     RA_DPHYTX_DPHY_RB0                             0x0024
    #define     RA_DPHYTX_DPHY_RB1                             0x0028
    #define     RA_DPHYTX_DPHY_RB2                             0x002C
    #define     RA_DPHYTX_DPHY_RB3                             0x0030
    #define     RA_DPHYTX_DPHY_PLL0                            0x0034
    #define     RA_DPHYTX_DPHY_PLL1                            0x0038
    #define     RA_DPHYTX_DPHY_PLL2                            0x003C
    #define     RA_DPHYTX_DPHY_PLLRB0                          0x0040
    #define     RA_DPHYTX_DPHY_PLLRB1                          0x0044
    typedef struct SIE_DPHYTX {
    #define     w32DPHYTX_DPHY_CTL0                            {\
            UNSG32 uDPHY_CTL0_BiuCtrlPhyEn                     :  1;\
            UNSG32 RSVDx0_b1                                   : 31;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL0;
            struct w32DPHYTX_DPHY_CTL0;
          };
    #define     w32DPHYTX_DPHY_CTL1                            {\
            UNSG32 uDPHY_CTL1_shutdownz                        :  1;\
            UNSG32 uDPHY_CTL1_rstz                             :  1;\
            UNSG32 uDPHY_CTL1_forcepll                         :  1;\
            UNSG32 uDPHY_CTL1_txrequesthsclk                   :  1;\
            UNSG32 uDPHY_CTL1_enableclkBIU                     :  1;\
            UNSG32 uDPHY_CTL1_turndisable_0                    :  1;\
            UNSG32 uDPHY_CTL1_forcerxmode_0                    :  1;\
            UNSG32 uDPHY_CTL1_basedir_0                        :  1;\
            UNSG32 uDPHY_CTL1_forcetxstopmode_0                :  1;\
            UNSG32 uDPHY_CTL1_forcetxstopmode_1                :  1;\
            UNSG32 uDPHY_CTL1_forcetxstopmode_2                :  1;\
            UNSG32 uDPHY_CTL1_forcetxstopmode_3                :  1;\
            UNSG32 uDPHY_CTL1_enable_0                         :  1;\
            UNSG32 uDPHY_CTL1_enable_1                         :  1;\
            UNSG32 uDPHY_CTL1_enable_2                         :  1;\
            UNSG32 uDPHY_CTL1_enable_3                         :  1;\
            UNSG32 uDPHY_CTL1_hsfreqrange                      :  7;\
            UNSG32 uDPHY_CTL1_cont_en                          :  1;\
            UNSG32 uDPHY_CTL1_cfgclkfreqrange                  :  6;\
            UNSG32 uDPHY_CTL1_biston                           :  1;\
            UNSG32 RSVDx4_b31                                  :  1;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL1;
            struct w32DPHYTX_DPHY_CTL1;
          };
    #define     w32DPHYTX_DPHY_CTL2                            {\
            UNSG32 uDPHY_CTL2_txulpsclkBIU                     :  1;\
            UNSG32 uDPHY_CTL2_txulpsexitclk                    :  1;\
            UNSG32 uDPHY_CTL2_turnrequest_0                    :  1;\
            UNSG32 RSVDx8_b3                                   : 29;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL2;
            struct w32DPHYTX_DPHY_CTL2;
          };
    #define     w32DPHYTX_DPHY_CTL3                            {\
            UNSG32 uDPHY_CTL3_txdataesc_0                      :  8;\
            UNSG32 uDPHY_CTL3_txtriggeresc_0                   :  4;\
            UNSG32 uDPHY_CTL3_txrequestesc_0                   :  1;\
            UNSG32 uDPHY_CTL3_txlpdtesc_0                      :  1;\
            UNSG32 uDPHY_CTL3_txulpsesc_0                      :  1;\
            UNSG32 uDPHY_CTL3_txulpsexit_0                     :  1;\
            UNSG32 uDPHY_CTL3_txvalidesc_0                     :  1;\
            UNSG32 RSVDxC_b17                                  : 15;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL3;
            struct w32DPHYTX_DPHY_CTL3;
          };
    #define     w32DPHYTX_DPHY_CTL4                            {\
            UNSG32 uDPHY_CTL4_txdataesc_1                      :  8;\
            UNSG32 uDPHY_CTL4_txtriggeresc_1                   :  4;\
            UNSG32 uDPHY_CTL4_txrequestesc_1                   :  1;\
            UNSG32 uDPHY_CTL4_txlpdtesc_1                      :  1;\
            UNSG32 uDPHY_CTL4_txulpsesc_1                      :  1;\
            UNSG32 uDPHY_CTL4_txulpsexit_1                     :  1;\
            UNSG32 uDPHY_CTL4_txvalidesc_1                     :  1;\
            UNSG32 RSVDx10_b17                                 : 15;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL4;
            struct w32DPHYTX_DPHY_CTL4;
          };
    #define     w32DPHYTX_DPHY_CTL5                            {\
            UNSG32 uDPHY_CTL5_txdataesc_2                      :  8;\
            UNSG32 uDPHY_CTL5_txtriggeresc_2                   :  4;\
            UNSG32 uDPHY_CTL5_txrequestesc_2                   :  1;\
            UNSG32 uDPHY_CTL5_txlpdtesc_2                      :  1;\
            UNSG32 uDPHY_CTL5_txulpsesc_2                      :  1;\
            UNSG32 uDPHY_CTL5_txulpsexit_2                     :  1;\
            UNSG32 uDPHY_CTL5_txvalidesc_2                     :  1;\
            UNSG32 RSVDx14_b17                                 : 15;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL5;
            struct w32DPHYTX_DPHY_CTL5;
          };
    #define     w32DPHYTX_DPHY_CTL6                            {\
            UNSG32 uDPHY_CTL6_txdataesc_3                      :  8;\
            UNSG32 uDPHY_CTL6_txtriggeresc_3                   :  4;\
            UNSG32 uDPHY_CTL6_txrequestesc_3                   :  1;\
            UNSG32 uDPHY_CTL6_txlpdtesc_3                      :  1;\
            UNSG32 uDPHY_CTL6_txulpsesc_3                      :  1;\
            UNSG32 uDPHY_CTL6_txulpsexit_3                     :  1;\
            UNSG32 uDPHY_CTL6_txvalidesc_3                     :  1;\
            UNSG32 RSVDx18_b17                                 : 15;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL6;
            struct w32DPHYTX_DPHY_CTL6;
          };
    #define     w32DPHYTX_DPHY_CTL7                            {\
            UNSG32 uDPHY_CTL7_txblaneclkSel                    :  1;\
            UNSG32 uDPHY_CTL7_txskewcalhsBIU                   :  1;\
            UNSG32 uDPHY_CTL7_txrequestdatahs_0                :  1;\
            UNSG32 uDPHY_CTL7_txrequestdatahs_1                :  1;\
            UNSG32 uDPHY_CTL7_txrequestdatahs_2                :  1;\
            UNSG32 uDPHY_CTL7_txrequestdatahs_3                :  1;\
            UNSG32 RSVDx1C_b6                                  : 26;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL7;
            struct w32DPHYTX_DPHY_CTL7;
          };
    #define     w32DPHYTX_DPHY_CTL8                            {\
            UNSG32 uDPHY_CTL8_txdatahs_0                       :  8;\
            UNSG32 uDPHY_CTL8_txdatahs_1                       :  8;\
            UNSG32 uDPHY_CTL8_txdatahs_2                       :  8;\
            UNSG32 uDPHY_CTL8_txdatahs_3                       :  8;\
          }
    union { UNSG32 u32DPHYTX_DPHY_CTL8;
            struct w32DPHYTX_DPHY_CTL8;
          };
    #define     w32DPHYTX_DPHY_RB0                             {\
            UNSG32 uDPHY_RB0_cont_data                         : 11;\
            UNSG32 uDPHY_RB0_lock                              :  1;\
            UNSG32 uDPHY_RB0_stopstateclk                      :  1;\
            UNSG32 uDPHY_RB0_ulpsactivenotclk                  :  1;\
            UNSG32 uDPHY_RB0_stopstatedata_0                   :  1;\
            UNSG32 uDPHY_RB0_stopstatedata_1                   :  1;\
            UNSG32 uDPHY_RB0_stopstatedata_2                   :  1;\
            UNSG32 uDPHY_RB0_stopstatedata_3                   :  1;\
            UNSG32 uDPHY_RB0_ulpsactivenot_0                   :  1;\
            UNSG32 uDPHY_RB0_ulpsactivenot_1                   :  1;\
            UNSG32 uDPHY_RB0_ulpsactivenot_2                   :  1;\
            UNSG32 uDPHY_RB0_ulpsactivenot_3                   :  1;\
            UNSG32 RSVDx24_b22                                 : 10;\
          }
    union { UNSG32 u32DPHYTX_DPHY_RB0;
            struct w32DPHYTX_DPHY_RB0;
          };
    #define     w32DPHYTX_DPHY_RB1                             {\
            UNSG32 uDPHY_RB1_txreadyhs_0                       :  1;\
            UNSG32 uDPHY_RB1_txreadyhs_1                       :  1;\
            UNSG32 uDPHY_RB1_txreadyhs_2                       :  1;\
            UNSG32 uDPHY_RB1_txreadyhs_3                       :  1;\
            UNSG32 RSVDx28_b4                                  : 28;\
          }
    union { UNSG32 u32DPHYTX_DPHY_RB1;
            struct w32DPHYTX_DPHY_RB1;
          };
    #define     w32DPHYTX_DPHY_RB2                             {\
            UNSG32 uDPHY_RB2_txreadyesc_0                      :  1;\
            UNSG32 uDPHY_RB2_txreadyesc_1                      :  1;\
            UNSG32 uDPHY_RB2_txreadyesc_2                      :  1;\
            UNSG32 uDPHY_RB2_txreadyesc_3                      :  1;\
            UNSG32 uDPHY_RB2_errcontrol_0                      :  1;\
            UNSG32 uDPHY_RB2_errcontentionlp0_0                :  1;\
            UNSG32 uDPHY_RB2_errcontentionlp1_0                :  1;\
            UNSG32 RSVDx2C_b7                                  : 25;\
          }
    union { UNSG32 u32DPHYTX_DPHY_RB2;
            struct w32DPHYTX_DPHY_RB2;
          };
    #define     w32DPHYTX_DPHY_RB3                             {\
            UNSG32 uDPHY_RB3_rxdataesc_0                       :  8;\
            UNSG32 uDPHY_RB3_rxtriggeresc_0                    :  4;\
            UNSG32 uDPHY_RB3_rxlpdtesc_0                       :  1;\
            UNSG32 uDPHY_RB3_rxulpsesc_0                       :  1;\
            UNSG32 uDPHY_RB3_rxvalidesc_0                      :  1;\
            UNSG32 uDPHY_RB3_direction_0                       :  1;\
            UNSG32 uDPHY_RB3_erresc_0                          :  1;\
            UNSG32 uDPHY_RB3_errsyncesc_0                      :  1;\
            UNSG32 RSVDx30_b18                                 : 14;\
          }
    union { UNSG32 u32DPHYTX_DPHY_RB3;
            struct w32DPHYTX_DPHY_RB3;
          };
    #define     w32DPHYTX_DPHY_PLL0                            {\
            UNSG32 uDPHY_PLL0_updatepll                        :  1;\
            UNSG32 RSVDx34_b1                                  : 31;\
          }
    union { UNSG32 u32DPHYTX_DPHY_PLL0;
            struct w32DPHYTX_DPHY_PLL0;
          };
    #define     w32DPHYTX_DPHY_PLL1                            {\
            UNSG32 uDPHY_PLL1_n                                :  4;\
            UNSG32 uDPHY_PLL1_m                                : 10;\
            UNSG32 uDPHY_PLL1_vco_cntrl                        :  6;\
            UNSG32 uDPHY_PLL1_prop_cntrl                       :  6;\
            UNSG32 uDPHY_PLL1_int_cntrl                        :  6;\
          }
    union { UNSG32 u32DPHYTX_DPHY_PLL1;
            struct w32DPHYTX_DPHY_PLL1;
          };
    #define     w32DPHYTX_DPHY_PLL2                            {\
            UNSG32 uDPHY_PLL2_gmp_cntrl                        :  2;\
            UNSG32 uDPHY_PLL2_cpbias_cntrl                     :  7;\
            UNSG32 uDPHY_PLL2_clksel                           :  2;\
            UNSG32 uDPHY_PLL2_force_lock                       :  1;\
            UNSG32 uDPHY_PLL2_pll_shadow_control               :  1;\
            UNSG32 uDPHY_PLL2_shadow_clear                     :  1;\
            UNSG32 uDPHY_PLL2_gp_clk_en                        :  1;\
            UNSG32 RSVDx3C_b15                                 : 17;\
          }
    union { UNSG32 u32DPHYTX_DPHY_PLL2;
            struct w32DPHYTX_DPHY_PLL2;
          };
    #define     w32DPHYTX_DPHY_PLLRB0                          {\
            UNSG32 uDPHY_PLLRB0_n_obs                          :  4;\
            UNSG32 uDPHY_PLLRB0_m_obs                          : 10;\
            UNSG32 uDPHY_PLLRB0_vco_cntrl_obs                  :  6;\
            UNSG32 uDPHY_PLLRB0_prop_cntrl_obs                 :  6;\
            UNSG32 uDPHY_PLLRB0_int_cntrl_obs                  :  6;\
          }
    union { UNSG32 u32DPHYTX_DPHY_PLLRB0;
            struct w32DPHYTX_DPHY_PLLRB0;
          };
    #define     w32DPHYTX_DPHY_PLLRB1                          {\
            UNSG32 uDPHY_PLLRB1_gmp_cntrl_obs                  :  2;\
            UNSG32 uDPHY_PLLRB1_cpbias_cntrl_obs               :  7;\
            UNSG32 uDPHY_PLLRB1_pll_shadow_control_obs         :  1;\
            UNSG32 uDPHY_PLLRB1_lock_pll                       :  1;\
            UNSG32 RSVDx44_b11                                 : 21;\
          }
    union { UNSG32 u32DPHYTX_DPHY_PLLRB1;
            struct w32DPHYTX_DPHY_PLLRB1;
          };
    } SIE_DPHYTX;
    typedef union  T32DPHYTX_DPHY_CTL0
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL0;
                 } T32DPHYTX_DPHY_CTL0;
    typedef union  T32DPHYTX_DPHY_CTL1
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL1;
                 } T32DPHYTX_DPHY_CTL1;
    typedef union  T32DPHYTX_DPHY_CTL2
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL2;
                 } T32DPHYTX_DPHY_CTL2;
    typedef union  T32DPHYTX_DPHY_CTL3
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL3;
                 } T32DPHYTX_DPHY_CTL3;
    typedef union  T32DPHYTX_DPHY_CTL4
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL4;
                 } T32DPHYTX_DPHY_CTL4;
    typedef union  T32DPHYTX_DPHY_CTL5
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL5;
                 } T32DPHYTX_DPHY_CTL5;
    typedef union  T32DPHYTX_DPHY_CTL6
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL6;
                 } T32DPHYTX_DPHY_CTL6;
    typedef union  T32DPHYTX_DPHY_CTL7
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL7;
                 } T32DPHYTX_DPHY_CTL7;
    typedef union  T32DPHYTX_DPHY_CTL8
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_CTL8;
                 } T32DPHYTX_DPHY_CTL8;
    typedef union  T32DPHYTX_DPHY_RB0
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_RB0;
                 } T32DPHYTX_DPHY_RB0;
    typedef union  T32DPHYTX_DPHY_RB1
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_RB1;
                 } T32DPHYTX_DPHY_RB1;
    typedef union  T32DPHYTX_DPHY_RB2
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_RB2;
                 } T32DPHYTX_DPHY_RB2;
    typedef union  T32DPHYTX_DPHY_RB3
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_RB3;
                 } T32DPHYTX_DPHY_RB3;
    typedef union  T32DPHYTX_DPHY_PLL0
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_PLL0;
                 } T32DPHYTX_DPHY_PLL0;
    typedef union  T32DPHYTX_DPHY_PLL1
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_PLL1;
                 } T32DPHYTX_DPHY_PLL1;
    typedef union  T32DPHYTX_DPHY_PLL2
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_PLL2;
                 } T32DPHYTX_DPHY_PLL2;
    typedef union  T32DPHYTX_DPHY_PLLRB0
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_PLLRB0;
                 } T32DPHYTX_DPHY_PLLRB0;
    typedef union  T32DPHYTX_DPHY_PLLRB1
          { UNSG32 u32;
            struct w32DPHYTX_DPHY_PLLRB1;
                 } T32DPHYTX_DPHY_PLLRB1;
    typedef union  TDPHYTX_DPHY_CTL0
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL0;
                   };
                 } TDPHYTX_DPHY_CTL0;
    typedef union  TDPHYTX_DPHY_CTL1
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL1;
                   };
                 } TDPHYTX_DPHY_CTL1;
    typedef union  TDPHYTX_DPHY_CTL2
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL2;
                   };
                 } TDPHYTX_DPHY_CTL2;
    typedef union  TDPHYTX_DPHY_CTL3
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL3;
                   };
                 } TDPHYTX_DPHY_CTL3;
    typedef union  TDPHYTX_DPHY_CTL4
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL4;
                   };
                 } TDPHYTX_DPHY_CTL4;
    typedef union  TDPHYTX_DPHY_CTL5
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL5;
                   };
                 } TDPHYTX_DPHY_CTL5;
    typedef union  TDPHYTX_DPHY_CTL6
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL6;
                   };
                 } TDPHYTX_DPHY_CTL6;
    typedef union  TDPHYTX_DPHY_CTL7
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL7;
                   };
                 } TDPHYTX_DPHY_CTL7;
    typedef union  TDPHYTX_DPHY_CTL8
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_CTL8;
                   };
                 } TDPHYTX_DPHY_CTL8;
    typedef union  TDPHYTX_DPHY_RB0
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_RB0;
                   };
                 } TDPHYTX_DPHY_RB0;
    typedef union  TDPHYTX_DPHY_RB1
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_RB1;
                   };
                 } TDPHYTX_DPHY_RB1;
    typedef union  TDPHYTX_DPHY_RB2
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_RB2;
                   };
                 } TDPHYTX_DPHY_RB2;
    typedef union  TDPHYTX_DPHY_RB3
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_RB3;
                   };
                 } TDPHYTX_DPHY_RB3;
    typedef union  TDPHYTX_DPHY_PLL0
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_PLL0;
                   };
                 } TDPHYTX_DPHY_PLL0;
    typedef union  TDPHYTX_DPHY_PLL1
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_PLL1;
                   };
                 } TDPHYTX_DPHY_PLL1;
    typedef union  TDPHYTX_DPHY_PLL2
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_PLL2;
                   };
                 } TDPHYTX_DPHY_PLL2;
    typedef union  TDPHYTX_DPHY_PLLRB0
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_PLLRB0;
                   };
                 } TDPHYTX_DPHY_PLLRB0;
    typedef union  TDPHYTX_DPHY_PLLRB1
          { UNSG32 u32[1];
            struct {
            struct w32DPHYTX_DPHY_PLLRB1;
                   };
                 } TDPHYTX_DPHY_PLLRB1;
     SIGN32 DPHYTX_drvrd(SIE_DPHYTX *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DPHYTX_drvwr(SIE_DPHYTX *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DPHYTX_reset(SIE_DPHYTX *p);
     SIGN32 DPHYTX_cmp  (SIE_DPHYTX *p, SIE_DPHYTX *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DPHYTX_check(p,pie,pfx,hLOG) DPHYTX_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DPHYTX_print(p,    pfx,hLOG) DPHYTX_cmp(p,0,  pfx,(void*)(hLOG),0,0)
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
#ifndef h_Dummy3Reg
#define h_Dummy3Reg (){}
    typedef struct SIE_Dummy3Reg {
            UNSG32 u_0x0                                       : 32;
    } SIE_Dummy3Reg;
     SIGN32 Dummy3Reg_drvrd(SIE_Dummy3Reg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 Dummy3Reg_drvwr(SIE_Dummy3Reg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void Dummy3Reg_reset(SIE_Dummy3Reg *p);
     SIGN32 Dummy3Reg_cmp  (SIE_Dummy3Reg *p, SIE_Dummy3Reg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define Dummy3Reg_check(p,pie,pfx,hLOG) Dummy3Reg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define Dummy3Reg_print(p,    pfx,hLOG) Dummy3Reg_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_AVIOGBLREG3
#define h_AVIOGBLREG3 (){}
    #define     RA_AVIOGBLREG3_Dummy3                          0x0000
    typedef struct SIE_AVIOGBLREG3 {
              SIE_Dummy3Reg                                    ie_Dummy3[2048];
    } SIE_AVIOGBLREG3;
     SIGN32 AVIOGBLREG3_drvrd(SIE_AVIOGBLREG3 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 AVIOGBLREG3_drvwr(SIE_AVIOGBLREG3 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void AVIOGBLREG3_reset(SIE_AVIOGBLREG3 *p);
     SIGN32 AVIOGBLREG3_cmp  (SIE_AVIOGBLREG3 *p, SIE_AVIOGBLREG3 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define AVIOGBLREG3_check(p,pie,pfx,hLOG) AVIOGBLREG3_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define AVIOGBLREG3_print(p,    pfx,hLOG) AVIOGBLREG3_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_VPLL_WRAP
#define h_VPLL_WRAP (){}
    #define     RA_VPLL_WRAP_VPLL_CTRL                         0x0000
    #define        VPLL_WRAP_VPLL_CTRL_clkOSel_d2                           0x1
    #define        VPLL_WRAP_VPLL_CTRL_clkOSel_d4                           0x2
    #define        VPLL_WRAP_VPLL_CTRL_clkOSel_d6                           0x3
    #define        VPLL_WRAP_VPLL_CTRL_clkOSel_d8                           0x4
    #define        VPLL_WRAP_VPLL_CTRL_clkOSel_d12                          0x5
    #define        VPLL_WRAP_VPLL_CTRL_clkO1Sel_d2                          0x1
    #define        VPLL_WRAP_VPLL_CTRL_clkO1Sel_d4                          0x2
    #define        VPLL_WRAP_VPLL_CTRL_clkO1Sel_d6                          0x3
    #define        VPLL_WRAP_VPLL_CTRL_clkO1Sel_d8                          0x4
    #define        VPLL_WRAP_VPLL_CTRL_clkO1Sel_d12                         0x5
    #define     RA_VPLL_WRAP_VPLL                              0x0004
    typedef struct SIE_VPLL_WRAP {
    #define     w32VPLL_WRAP_VPLL_CTRL                         {\
            UNSG32 uVPLL_CTRL_clkOSwitch                       :  1;\
            UNSG32 uVPLL_CTRL_clkOD3Switch                     :  1;\
            UNSG32 uVPLL_CTRL_clkOSel                          :  3;\
            UNSG32 uVPLL_CTRL_clkOEn                           :  1;\
            UNSG32 uVPLL_CTRL_clkO1Switch                      :  1;\
            UNSG32 uVPLL_CTRL_clkO1D3Switch                    :  1;\
            UNSG32 uVPLL_CTRL_clkO1Sel                         :  3;\
            UNSG32 uVPLL_CTRL_clkO1En                          :  1;\
            UNSG32 RSVDx0_b12                                  : 20;\
          }
    union { UNSG32 u32VPLL_WRAP_VPLL_CTRL;
            struct w32VPLL_WRAP_VPLL_CTRL;
          };
              SIE_abipll                                       ie_VPLL;
    } SIE_VPLL_WRAP;
    typedef union  T32VPLL_WRAP_VPLL_CTRL
          { UNSG32 u32;
            struct w32VPLL_WRAP_VPLL_CTRL;
                 } T32VPLL_WRAP_VPLL_CTRL;
    typedef union  TVPLL_WRAP_VPLL_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32VPLL_WRAP_VPLL_CTRL;
                   };
                 } TVPLL_WRAP_VPLL_CTRL;
     SIGN32 VPLL_WRAP_drvrd(SIE_VPLL_WRAP *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 VPLL_WRAP_drvwr(SIE_VPLL_WRAP *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void VPLL_WRAP_reset(SIE_VPLL_WRAP *p);
     SIGN32 VPLL_WRAP_cmp  (SIE_VPLL_WRAP *p, SIE_VPLL_WRAP *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define VPLL_WRAP_check(p,pie,pfx,hLOG) VPLL_WRAP_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define VPLL_WRAP_print(p,    pfx,hLOG) VPLL_WRAP_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_vppGbl
#define h_vppGbl (){}
    #define     RA_vppGbl_MEMMAP_MIPI                          0x0000
    #define     RA_vppGbl_VPLL0_WRAP                           0x2000
    #define     RA_vppGbl_VPLL1_WRAP                           0x2024
    #define     RA_vppGbl_DPHYTX                               0x2048
    #define     RA_vppGbl_AVPLLA_CLK_EN                        0x2090
    #define     RA_vppGbl_SWPDWN_CTRL                          0x2094
    #define     RA_vppGbl_CTRL                                 0x2098
    #define     RA_vppGbl_VPP128bDHUB_SRAMPWR                  0x209C
    #define     RA_vppGbl_MIPI_SRAMPWR                         0x20A0
    #define     RA_vppGbl_LCDC_SRAMPWR                         0x20A4
    #define     RA_vppGbl_INTR_CTRL                            0x20A8
    #define     RA_vppGbl_SWRST_CTRL                           0x20AC
    #define     RA_vppGbl_LCDC_CTRL                            0x20B0
    #define     RA_vppGbl_LCDC2_CTRL                           0x20B4
    #define     RA_vppGbl_MIPI_CTRL                            0x20B8
    #define     RA_vppGbl_MIPI_CTRL1                           0x20BC
    #define     RA_vppGbl_CG_CTRL                              0x20C0
    #define     RA_vppGbl_MIPI_CTRL_STS                        0x20C4
    typedef struct SIE_vppGbl {
              SIE_AVIOGBLREG3                                  ie_MEMMAP_MIPI;
              SIE_VPLL_WRAP                                    ie_VPLL0_WRAP;
              SIE_VPLL_WRAP                                    ie_VPLL1_WRAP;
              SIE_DPHYTX                                       ie_DPHYTX;
    #define     w32vppGbl_AVPLLA_CLK_EN                        {\
            UNSG32 uAVPLLA_CLK_EN_ctrl                         :  2;\
            UNSG32 uAVPLLA_CLK_EN_dbg_mux_sel                  :  1;\
            UNSG32 RSVDx2090_b3                                : 29;\
          }
    union { UNSG32 u32vppGbl_AVPLLA_CLK_EN;
            struct w32vppGbl_AVPLLA_CLK_EN;
          };
    #define     w32vppGbl_SWPDWN_CTRL                          {\
            UNSG32 uSWPDWN_CTRL_VPLL0_PD                       :  1;\
            UNSG32 uSWPDWN_CTRL_VPLL1_PD                       :  1;\
            UNSG32 RSVDx2094_b2                                : 30;\
          }
    union { UNSG32 u32vppGbl_SWPDWN_CTRL;
            struct w32vppGbl_SWPDWN_CTRL;
          };
    #define     w32vppGbl_CTRL                                 {\
            UNSG32 uCTRL_VPPDHUB_dyCG_en                       :  1;\
            UNSG32 uCTRL_VPPDHUB_CG_en                         :  1;\
            UNSG32 uCTRL_BCM_FIFO_FLUSH                        :  1;\
            UNSG32 uCTRL_BCMQ_FIFO_FLUSH                       :  1;\
            UNSG32 RSVDx2098_b4                                : 28;\
          }
    union { UNSG32 u32vppGbl_CTRL;
            struct w32vppGbl_CTRL;
          };
              SIE_SRAMPWR                                      ie_VPP128bDHUB_SRAMPWR;
              SIE_SRAMPWR                                      ie_MIPI_SRAMPWR;
              SIE_SRAMPWR                                      ie_LCDC_SRAMPWR;
    #define     w32vppGbl_INTR_CTRL                            {\
            UNSG32 uINTR_CTRL_mipi_int_en                      :  1;\
            UNSG32 uINTR_CTRL_lcdc2_int_en                     :  1;\
            UNSG32 uINTR_CTRL_bcm_invalid_int_en               :  1;\
            UNSG32 RSVDx20A8_b3                                : 29;\
          }
    union { UNSG32 u32vppGbl_INTR_CTRL;
            struct w32vppGbl_INTR_CTRL;
          };
    #define     w32vppGbl_SWRST_CTRL                           {\
            UNSG32 uSWRST_CTRL_dpiSyncRstn                     :  1;\
            UNSG32 uSWRST_CTRL_lcdc2SyncRstn                   :  1;\
            UNSG32 uSWRST_CTRL_lcdc2SysSyncRstn                :  1;\
            UNSG32 RSVDx20AC_b3                                : 29;\
          }
    union { UNSG32 u32vppGbl_SWRST_CTRL;
            struct w32vppGbl_SWRST_CTRL;
          };
    #define     w32vppGbl_LCDC_CTRL                            {\
            UNSG32 uLCDC_CTRL_lcdc1_clk_sel                    :  1;\
            UNSG32 uLCDC_CTRL_lcdc2_clk_sel                    :  1;\
            UNSG32 uLCDC_CTRL_lcdc1_ClkSel                     :  3;\
            UNSG32 uLCDC_CTRL_lcdc1_ClkSwitch                  :  1;\
            UNSG32 uLCDC_CTRL_lcdc1_ClkD3Switch                :  1;\
            UNSG32 uLCDC_CTRL_lcdc1_ClkEn                      :  1;\
            UNSG32 uLCDC_CTRL_lcdc1_sysClk_en                  :  1;\
            UNSG32 uLCDC_CTRL_lcdc2_ClkSel                     :  3;\
            UNSG32 uLCDC_CTRL_lcdc2_ClkSwitch                  :  1;\
            UNSG32 uLCDC_CTRL_lcdc2_ClkD3Switch                :  1;\
            UNSG32 uLCDC_CTRL_lcdc2_ClkEn                      :  1;\
            UNSG32 uLCDC_CTRL_lcdc2_sysClk_en                  :  1;\
            UNSG32 RSVDx20B0_b16                               : 16;\
          }
    union { UNSG32 u32vppGbl_LCDC_CTRL;
            struct w32vppGbl_LCDC_CTRL;
          };
    #define     w32vppGbl_LCDC2_CTRL                           {\
            UNSG32 uLCDC2_CTRL_eof_frst_sel                    :  1;\
            UNSG32 uLCDC2_CTRL_tear_sftrst                     :  1;\
            UNSG32 uLCDC2_CTRL_clken_ctrl0                     :  1;\
            UNSG32 uLCDC2_CTRL_clken_ctrl1                     :  1;\
            UNSG32 uLCDC2_CTRL_clken_ctrl2                     :  1;\
            UNSG32 uLCDC2_CTRL_clken_ctrl3                     :  1;\
            UNSG32 RSVDx20B4_b6                                : 26;\
          }
    union { UNSG32 u32vppGbl_LCDC2_CTRL;
            struct w32vppGbl_LCDC2_CTRL;
          };
    #define     w32vppGbl_MIPI_CTRL                            {\
            UNSG32 uMIPI_CTRL_mipiClkg_en                      :  1;\
            UNSG32 uMIPI_CTRL_mipiSysClkg_en                   :  1;\
            UNSG32 uMIPI_CTRL_dpidataen_pol                    :  1;\
            UNSG32 uMIPI_CTRL_dpivsync_pol                     :  1;\
            UNSG32 uMIPI_CTRL_dpihsync_pol                     :  1;\
            UNSG32 uMIPI_CTRL_dpishutd_pol                     :  1;\
            UNSG32 uMIPI_CTRL_dpicolorm_pol                    :  1;\
            UNSG32 uMIPI_CTRL_colormode                        :  1;\
            UNSG32 uMIPI_CTRL_shutdn                           :  1;\
            UNSG32 uMIPI_CTRL_force_pll_on                     :  1;\
            UNSG32 uMIPI_CTRL_tear_request_pulse               :  1;\
            UNSG32 uMIPI_CTRL_updatecfg_pulse                  :  1;\
            UNSG32 uMIPI_CTRL_tear_sftrst                      :  1;\
            UNSG32 uMIPI_CTRL_dsi_te_in_sel                    :  1;\
            UNSG32 uMIPI_CTRL_dsi_te_in_sense                  :  1;\
            UNSG32 uMIPI_CTRL_dsi_te_enable                    :  1;\
            UNSG32 RSVDx20B8_b16                               : 16;\
          }
    union { UNSG32 u32vppGbl_MIPI_CTRL;
            struct w32vppGbl_MIPI_CTRL;
          };
    #define     w32vppGbl_MIPI_CTRL1                           {\
            UNSG32 uMIPI_CTRL_dsitedelay                       : 22;\
            UNSG32 uMIPI_CTRL_edpi_mode                        :  1;\
            UNSG32 RSVDx20BC_b23                               :  9;\
          }
    union { UNSG32 u32vppGbl_MIPI_CTRL1;
            struct w32vppGbl_MIPI_CTRL1;
          };
    #define     w32vppGbl_CG_CTRL                              {\
            UNSG32 uCG_CTRL_Reserved_cg_en                     :  1;\
            UNSG32 RSVDx20C0_b1                                : 31;\
          }
    union { UNSG32 u32vppGbl_CG_CTRL;
            struct w32vppGbl_CG_CTRL;
          };
    #define     w32vppGbl_MIPI_CTRL_STS                        {\
            UNSG32 uMIPI_CTRL_STS_dsitewait                    :  1;\
            UNSG32 RSVDx20C4_b1                                : 31;\
          }
    union { UNSG32 u32vppGbl_MIPI_CTRL_STS;
            struct w32vppGbl_MIPI_CTRL_STS;
          };
             UNSG8 RSVDx20C8                                   [7992];
    } SIE_vppGbl;
    typedef union  T32vppGbl_AVPLLA_CLK_EN
          { UNSG32 u32;
            struct w32vppGbl_AVPLLA_CLK_EN;
                 } T32vppGbl_AVPLLA_CLK_EN;
    typedef union  T32vppGbl_SWPDWN_CTRL
          { UNSG32 u32;
            struct w32vppGbl_SWPDWN_CTRL;
                 } T32vppGbl_SWPDWN_CTRL;
    typedef union  T32vppGbl_CTRL
          { UNSG32 u32;
            struct w32vppGbl_CTRL;
                 } T32vppGbl_CTRL;
    typedef union  T32vppGbl_INTR_CTRL
          { UNSG32 u32;
            struct w32vppGbl_INTR_CTRL;
                 } T32vppGbl_INTR_CTRL;
    typedef union  T32vppGbl_SWRST_CTRL
          { UNSG32 u32;
            struct w32vppGbl_SWRST_CTRL;
                 } T32vppGbl_SWRST_CTRL;
    typedef union  T32vppGbl_LCDC_CTRL
          { UNSG32 u32;
            struct w32vppGbl_LCDC_CTRL;
                 } T32vppGbl_LCDC_CTRL;
    typedef union  T32vppGbl_LCDC2_CTRL
          { UNSG32 u32;
            struct w32vppGbl_LCDC2_CTRL;
                 } T32vppGbl_LCDC2_CTRL;
    typedef union  T32vppGbl_MIPI_CTRL
          { UNSG32 u32;
            struct w32vppGbl_MIPI_CTRL;
                 } T32vppGbl_MIPI_CTRL;
    typedef union  T32vppGbl_MIPI_CTRL1
          { UNSG32 u32;
            struct w32vppGbl_MIPI_CTRL1;
                 } T32vppGbl_MIPI_CTRL1;
    typedef union  T32vppGbl_CG_CTRL
          { UNSG32 u32;
            struct w32vppGbl_CG_CTRL;
                 } T32vppGbl_CG_CTRL;
    typedef union  T32vppGbl_MIPI_CTRL_STS
          { UNSG32 u32;
            struct w32vppGbl_MIPI_CTRL_STS;
                 } T32vppGbl_MIPI_CTRL_STS;
    typedef union  TvppGbl_AVPLLA_CLK_EN
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_AVPLLA_CLK_EN;
                   };
                 } TvppGbl_AVPLLA_CLK_EN;
    typedef union  TvppGbl_SWPDWN_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_SWPDWN_CTRL;
                   };
                 } TvppGbl_SWPDWN_CTRL;
    typedef union  TvppGbl_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_CTRL;
                   };
                 } TvppGbl_CTRL;
    typedef union  TvppGbl_INTR_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_INTR_CTRL;
                   };
                 } TvppGbl_INTR_CTRL;
    typedef union  TvppGbl_SWRST_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_SWRST_CTRL;
                   };
                 } TvppGbl_SWRST_CTRL;
    typedef union  TvppGbl_LCDC_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_LCDC_CTRL;
                   };
                 } TvppGbl_LCDC_CTRL;
    typedef union  TvppGbl_LCDC2_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_LCDC2_CTRL;
                   };
                 } TvppGbl_LCDC2_CTRL;
    typedef union  TvppGbl_MIPI_CTRL
          { UNSG32 u32[2];
            struct {
            struct w32vppGbl_MIPI_CTRL;
            struct w32vppGbl_MIPI_CTRL1;
                   };
                 } TvppGbl_MIPI_CTRL;
    typedef union  TvppGbl_CG_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_CG_CTRL;
                   };
                 } TvppGbl_CG_CTRL;
    typedef union  TvppGbl_MIPI_CTRL_STS
          { UNSG32 u32[1];
            struct {
            struct w32vppGbl_MIPI_CTRL_STS;
                   };
                 } TvppGbl_MIPI_CTRL_STS;
     SIGN32 vppGbl_drvrd(SIE_vppGbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 vppGbl_drvwr(SIE_vppGbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void vppGbl_reset(SIE_vppGbl *p);
     SIGN32 vppGbl_cmp  (SIE_vppGbl *p, SIE_vppGbl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define vppGbl_check(p,pie,pfx,hLOG) vppGbl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define vppGbl_print(p,    pfx,hLOG) vppGbl_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifdef __cplusplus
  }
#endif
#pragma  pack()
#endif
