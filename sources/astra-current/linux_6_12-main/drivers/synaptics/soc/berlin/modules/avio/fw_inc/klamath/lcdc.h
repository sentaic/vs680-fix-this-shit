#ifndef lcdc_h
#define lcdc_h (){}
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
#ifndef h_CSC_C17O24
#define h_CSC_C17O24 (){}
    #define     RA_CSC_C17O24_CFG0                             0x0000
    #define     RA_CSC_C17O24_CFG1                             0x0004
    #define     RA_CSC_C17O24_CFG2                             0x0008
    #define     RA_CSC_C17O24_CFG3                             0x000C
    #define     RA_CSC_C17O24_CFG4                             0x0010
    #define     RA_CSC_C17O24_CFG5                             0x0014
    #define     RA_CSC_C17O24_CFG6                             0x0018
    #define     RA_CSC_C17O24_CFG7                             0x001C
    #define     RA_CSC_C17O24_CFG8                             0x0020
    #define     RA_CSC_C17O24_CFG9                             0x0024
    #define     RA_CSC_C17O24_CFG10                            0x0028
    #define     RA_CSC_C17O24_CFG11                            0x002C
    #define     RA_CSC_C17O24_CFG12                            0x0030
    #define     RA_CSC_C17O24_CFG13                            0x0034
    #define     RA_CSC_C17O24_CFG14                            0x0038
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
#ifndef h_UPS8
#define h_UPS8 (){}
    #define     RA_UPS8_CFG0                                   0x0000
    #define     RA_UPS8_CFG1                                   0x0004
    #define     RA_UPS8_CFG2                                   0x0008
    #define     RA_UPS8_CFG3                                   0x000C
    #define     RA_UPS8_CFG4                                   0x0010
    #define     RA_UPS8_CFG5                                   0x0014
    #define     RA_UPS8_CFG6                                   0x0018
    #define     RA_UPS8_CFG7                                   0x001C
    #define     RA_UPS8_CFG8                                   0x0020
    #define     RA_UPS8_CFG9                                   0x0024
    typedef struct SIE_UPS8 {
    #define     w32UPS8_CFG0                                   {\
            UNSG32 uCFG0_ups_en                                :  2;\
            UNSG32 uCFG0_rsv                                   :  2;\
            UNSG32 uCFG0_dpwr                                  :  1;\
            UNSG32 RSVDx0_b5                                   : 27;\
          }
    union { UNSG32 u32UPS8_CFG0;
            struct w32UPS8_CFG0;
          };
    #define     w32UPS8_CFG1                                   {\
            UNSG32 mCFG1_ups_c0                                : 13;\
            UNSG32 mCFG1_ups_c1                                : 13;\
            UNSG32 RSVDx4_b26                                  :  6;\
          }
    union { UNSG32 u32UPS8_CFG1;
            struct w32UPS8_CFG1;
          };
    #define     w32UPS8_CFG2                                   {\
            UNSG32 mCFG2_ups_c2                                : 13;\
            UNSG32 mCFG2_ups_c3                                : 13;\
            UNSG32 RSVDx8_b26                                  :  6;\
          }
    union { UNSG32 u32UPS8_CFG2;
            struct w32UPS8_CFG2;
          };
    #define     w32UPS8_CFG3                                   {\
            UNSG32 mCFG3_ups_c4                                : 13;\
            UNSG32 mCFG3_ups_c5                                : 13;\
            UNSG32 RSVDxC_b26                                  :  6;\
          }
    union { UNSG32 u32UPS8_CFG3;
            struct w32UPS8_CFG3;
          };
    #define     w32UPS8_CFG4                                   {\
            UNSG32 mCFG4_ups_c6                                : 13;\
            UNSG32 RSVDx10_b13                                 : 19;\
          }
    union { UNSG32 u32UPS8_CFG4;
            struct w32UPS8_CFG4;
          };
    #define     w32UPS8_CFG5                                   {\
            UNSG32 uCFG5_ups_yshift                            :  1;\
            UNSG32 uCFG5_ups_cshift                            :  1;\
            UNSG32 uCFG5_ups_cswap                             :  1;\
            UNSG32 uCFG5_ups_yblank                            :  8;\
            UNSG32 uCFG5_ups_cblank                            :  8;\
            UNSG32 uCFG5_ups_use_blank                         :  1;\
            UNSG32 RSVDx14_b20                                 : 12;\
          }
    union { UNSG32 u32UPS8_CFG5;
            struct w32UPS8_CFG5;
          };
    #define     w32UPS8_CFG6                                   {\
            UNSG32 uCFG6_ups_y_th                              :  8;\
            UNSG32 uCFG6_ups_c_th                              :  8;\
            UNSG32 RSVDx18_b16                                 : 16;\
          }
    union { UNSG32 u32UPS8_CFG6;
            struct w32UPS8_CFG6;
          };
    #define     w32UPS8_CFG7                                   {\
            UNSG32 mCFG7_ups_7c0                               : 13;\
            UNSG32 mCFG7_ups_7c1                               : 13;\
            UNSG32 RSVDx1C_b26                                 :  6;\
          }
    union { UNSG32 u32UPS8_CFG7;
            struct w32UPS8_CFG7;
          };
    #define     w32UPS8_CFG8                                   {\
            UNSG32 mCFG8_ups_7c2                               : 13;\
            UNSG32 mCFG8_ups_7c3                               : 13;\
            UNSG32 RSVDx20_b26                                 :  6;\
          }
    union { UNSG32 u32UPS8_CFG8;
            struct w32UPS8_CFG8;
          };
    #define     w32UPS8_CFG9                                   {\
            UNSG32 mCFG9_ups_7c4                               : 13;\
            UNSG32 RSVDx24_b13                                 : 19;\
          }
    union { UNSG32 u32UPS8_CFG9;
            struct w32UPS8_CFG9;
          };
    } SIE_UPS8;
    typedef union  T32UPS8_CFG0
          { UNSG32 u32;
            struct w32UPS8_CFG0;
                 } T32UPS8_CFG0;
    typedef union  T32UPS8_CFG1
          { UNSG32 u32;
            struct w32UPS8_CFG1;
                 } T32UPS8_CFG1;
    typedef union  T32UPS8_CFG2
          { UNSG32 u32;
            struct w32UPS8_CFG2;
                 } T32UPS8_CFG2;
    typedef union  T32UPS8_CFG3
          { UNSG32 u32;
            struct w32UPS8_CFG3;
                 } T32UPS8_CFG3;
    typedef union  T32UPS8_CFG4
          { UNSG32 u32;
            struct w32UPS8_CFG4;
                 } T32UPS8_CFG4;
    typedef union  T32UPS8_CFG5
          { UNSG32 u32;
            struct w32UPS8_CFG5;
                 } T32UPS8_CFG5;
    typedef union  T32UPS8_CFG6
          { UNSG32 u32;
            struct w32UPS8_CFG6;
                 } T32UPS8_CFG6;
    typedef union  T32UPS8_CFG7
          { UNSG32 u32;
            struct w32UPS8_CFG7;
                 } T32UPS8_CFG7;
    typedef union  T32UPS8_CFG8
          { UNSG32 u32;
            struct w32UPS8_CFG8;
                 } T32UPS8_CFG8;
    typedef union  T32UPS8_CFG9
          { UNSG32 u32;
            struct w32UPS8_CFG9;
                 } T32UPS8_CFG9;
    typedef union  TUPS8_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG0;
                   };
                 } TUPS8_CFG0;
    typedef union  TUPS8_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG1;
                   };
                 } TUPS8_CFG1;
    typedef union  TUPS8_CFG2
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG2;
                   };
                 } TUPS8_CFG2;
    typedef union  TUPS8_CFG3
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG3;
                   };
                 } TUPS8_CFG3;
    typedef union  TUPS8_CFG4
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG4;
                   };
                 } TUPS8_CFG4;
    typedef union  TUPS8_CFG5
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG5;
                   };
                 } TUPS8_CFG5;
    typedef union  TUPS8_CFG6
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG6;
                   };
                 } TUPS8_CFG6;
    typedef union  TUPS8_CFG7
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG7;
                   };
                 } TUPS8_CFG7;
    typedef union  TUPS8_CFG8
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG8;
                   };
                 } TUPS8_CFG8;
    typedef union  TUPS8_CFG9
          { UNSG32 u32[1];
            struct {
            struct w32UPS8_CFG9;
                   };
                 } TUPS8_CFG9;
     SIGN32 UPS8_drvrd(SIE_UPS8 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 UPS8_drvwr(SIE_UPS8 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void UPS8_reset(SIE_UPS8 *p);
     SIGN32 UPS8_cmp  (SIE_UPS8 *p, SIE_UPS8 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define UPS8_check(p,pie,pfx,hLOG) UPS8_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define UPS8_print(p,    pfx,hLOG) UPS8_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_UPS10
#define h_UPS10 (){}
    #define     RA_UPS10_CFG0                                  0x0000
    #define     RA_UPS10_CFG1                                  0x0004
    #define     RA_UPS10_CFG2                                  0x0008
    #define     RA_UPS10_CFG3                                  0x000C
    #define     RA_UPS10_CFG4                                  0x0010
    #define     RA_UPS10_CFG5                                  0x0014
    #define     RA_UPS10_CFG6                                  0x0018
    #define     RA_UPS10_CFG7                                  0x001C
    #define     RA_UPS10_CFG8                                  0x0020
    #define     RA_UPS10_CFG9                                  0x0024
    typedef struct SIE_UPS10 {
    #define     w32UPS10_CFG0                                  {\
            UNSG32 uCFG0_ups_en                                :  2;\
            UNSG32 uCFG0_rsv                                   :  2;\
            UNSG32 uCFG0_dpwr                                  :  1;\
            UNSG32 RSVDx0_b5                                   : 27;\
          }
    union { UNSG32 u32UPS10_CFG0;
            struct w32UPS10_CFG0;
          };
    #define     w32UPS10_CFG1                                  {\
            UNSG32 mCFG1_ups_c0                                : 13;\
            UNSG32 mCFG1_ups_c1                                : 13;\
            UNSG32 RSVDx4_b26                                  :  6;\
          }
    union { UNSG32 u32UPS10_CFG1;
            struct w32UPS10_CFG1;
          };
    #define     w32UPS10_CFG2                                  {\
            UNSG32 mCFG2_ups_c2                                : 13;\
            UNSG32 mCFG2_ups_c3                                : 13;\
            UNSG32 RSVDx8_b26                                  :  6;\
          }
    union { UNSG32 u32UPS10_CFG2;
            struct w32UPS10_CFG2;
          };
    #define     w32UPS10_CFG3                                  {\
            UNSG32 mCFG3_ups_c4                                : 13;\
            UNSG32 mCFG3_ups_c5                                : 13;\
            UNSG32 RSVDxC_b26                                  :  6;\
          }
    union { UNSG32 u32UPS10_CFG3;
            struct w32UPS10_CFG3;
          };
    #define     w32UPS10_CFG4                                  {\
            UNSG32 mCFG4_ups_c6                                : 13;\
            UNSG32 RSVDx10_b13                                 : 19;\
          }
    union { UNSG32 u32UPS10_CFG4;
            struct w32UPS10_CFG4;
          };
    #define     w32UPS10_CFG5                                  {\
            UNSG32 uCFG5_ups_yshift                            :  1;\
            UNSG32 uCFG5_ups_cshift                            :  1;\
            UNSG32 uCFG5_ups_cswap                             :  1;\
            UNSG32 uCFG5_ups_yblank                            : 10;\
            UNSG32 uCFG5_ups_cblank                            : 10;\
            UNSG32 uCFG5_ups_use_blank                         :  1;\
            UNSG32 RSVDx14_b24                                 :  8;\
          }
    union { UNSG32 u32UPS10_CFG5;
            struct w32UPS10_CFG5;
          };
    #define     w32UPS10_CFG6                                  {\
            UNSG32 uCFG6_ups_y_th                              :  8;\
            UNSG32 uCFG6_ups_c_th                              :  8;\
            UNSG32 RSVDx18_b16                                 : 16;\
          }
    union { UNSG32 u32UPS10_CFG6;
            struct w32UPS10_CFG6;
          };
    #define     w32UPS10_CFG7                                  {\
            UNSG32 mCFG7_ups_7c0                               : 13;\
            UNSG32 mCFG7_ups_7c1                               : 13;\
            UNSG32 RSVDx1C_b26                                 :  6;\
          }
    union { UNSG32 u32UPS10_CFG7;
            struct w32UPS10_CFG7;
          };
    #define     w32UPS10_CFG8                                  {\
            UNSG32 mCFG8_ups_7c2                               : 13;\
            UNSG32 mCFG8_ups_7c3                               : 13;\
            UNSG32 RSVDx20_b26                                 :  6;\
          }
    union { UNSG32 u32UPS10_CFG8;
            struct w32UPS10_CFG8;
          };
    #define     w32UPS10_CFG9                                  {\
            UNSG32 mCFG9_ups_7c4                               : 13;\
            UNSG32 RSVDx24_b13                                 : 19;\
          }
    union { UNSG32 u32UPS10_CFG9;
            struct w32UPS10_CFG9;
          };
    } SIE_UPS10;
    typedef union  T32UPS10_CFG0
          { UNSG32 u32;
            struct w32UPS10_CFG0;
                 } T32UPS10_CFG0;
    typedef union  T32UPS10_CFG1
          { UNSG32 u32;
            struct w32UPS10_CFG1;
                 } T32UPS10_CFG1;
    typedef union  T32UPS10_CFG2
          { UNSG32 u32;
            struct w32UPS10_CFG2;
                 } T32UPS10_CFG2;
    typedef union  T32UPS10_CFG3
          { UNSG32 u32;
            struct w32UPS10_CFG3;
                 } T32UPS10_CFG3;
    typedef union  T32UPS10_CFG4
          { UNSG32 u32;
            struct w32UPS10_CFG4;
                 } T32UPS10_CFG4;
    typedef union  T32UPS10_CFG5
          { UNSG32 u32;
            struct w32UPS10_CFG5;
                 } T32UPS10_CFG5;
    typedef union  T32UPS10_CFG6
          { UNSG32 u32;
            struct w32UPS10_CFG6;
                 } T32UPS10_CFG6;
    typedef union  T32UPS10_CFG7
          { UNSG32 u32;
            struct w32UPS10_CFG7;
                 } T32UPS10_CFG7;
    typedef union  T32UPS10_CFG8
          { UNSG32 u32;
            struct w32UPS10_CFG8;
                 } T32UPS10_CFG8;
    typedef union  T32UPS10_CFG9
          { UNSG32 u32;
            struct w32UPS10_CFG9;
                 } T32UPS10_CFG9;
    typedef union  TUPS10_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG0;
                   };
                 } TUPS10_CFG0;
    typedef union  TUPS10_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG1;
                   };
                 } TUPS10_CFG1;
    typedef union  TUPS10_CFG2
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG2;
                   };
                 } TUPS10_CFG2;
    typedef union  TUPS10_CFG3
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG3;
                   };
                 } TUPS10_CFG3;
    typedef union  TUPS10_CFG4
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG4;
                   };
                 } TUPS10_CFG4;
    typedef union  TUPS10_CFG5
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG5;
                   };
                 } TUPS10_CFG5;
    typedef union  TUPS10_CFG6
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG6;
                   };
                 } TUPS10_CFG6;
    typedef union  TUPS10_CFG7
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG7;
                   };
                 } TUPS10_CFG7;
    typedef union  TUPS10_CFG8
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG8;
                   };
                 } TUPS10_CFG8;
    typedef union  TUPS10_CFG9
          { UNSG32 u32[1];
            struct {
            struct w32UPS10_CFG9;
                   };
                 } TUPS10_CFG9;
     SIGN32 UPS10_drvrd(SIE_UPS10 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 UPS10_drvwr(SIE_UPS10 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void UPS10_reset(SIE_UPS10 *p);
     SIGN32 UPS10_cmp  (SIE_UPS10 *p, SIE_UPS10 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define UPS10_check(p,pie,pfx,hLOG) UPS10_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define UPS10_print(p,    pfx,hLOG) UPS10_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_UPS12
#define h_UPS12 (){}
    #define     RA_UPS12_CFG0                                  0x0000
    #define     RA_UPS12_CFG1                                  0x0004
    #define     RA_UPS12_CFG2                                  0x0008
    #define     RA_UPS12_CFG3                                  0x000C
    #define     RA_UPS12_CFG4                                  0x0010
    #define     RA_UPS12_CFG5                                  0x0014
    #define     RA_UPS12_CFG6                                  0x0018
    #define     RA_UPS12_CFG7                                  0x001C
    #define     RA_UPS12_CFG8                                  0x0020
    #define     RA_UPS12_CFG9                                  0x0024
    typedef struct SIE_UPS12 {
    #define     w32UPS12_CFG0                                  {\
            UNSG32 uCFG0_ups_en                                :  2;\
            UNSG32 uCFG0_rsv                                   :  2;\
            UNSG32 uCFG0_dpwr                                  :  1;\
            UNSG32 RSVDx0_b5                                   : 27;\
          }
    union { UNSG32 u32UPS12_CFG0;
            struct w32UPS12_CFG0;
          };
    #define     w32UPS12_CFG1                                  {\
            UNSG32 mCFG1_ups_c0                                : 13;\
            UNSG32 mCFG1_ups_c1                                : 13;\
            UNSG32 RSVDx4_b26                                  :  6;\
          }
    union { UNSG32 u32UPS12_CFG1;
            struct w32UPS12_CFG1;
          };
    #define     w32UPS12_CFG2                                  {\
            UNSG32 mCFG2_ups_c2                                : 13;\
            UNSG32 mCFG2_ups_c3                                : 13;\
            UNSG32 RSVDx8_b26                                  :  6;\
          }
    union { UNSG32 u32UPS12_CFG2;
            struct w32UPS12_CFG2;
          };
    #define     w32UPS12_CFG3                                  {\
            UNSG32 mCFG3_ups_c4                                : 13;\
            UNSG32 mCFG3_ups_c5                                : 13;\
            UNSG32 RSVDxC_b26                                  :  6;\
          }
    union { UNSG32 u32UPS12_CFG3;
            struct w32UPS12_CFG3;
          };
    #define     w32UPS12_CFG4                                  {\
            UNSG32 mCFG4_ups_c6                                : 13;\
            UNSG32 RSVDx10_b13                                 : 19;\
          }
    union { UNSG32 u32UPS12_CFG4;
            struct w32UPS12_CFG4;
          };
    #define     w32UPS12_CFG5                                  {\
            UNSG32 uCFG5_ups_yshift                            :  1;\
            UNSG32 uCFG5_ups_cshift                            :  1;\
            UNSG32 uCFG5_ups_cswap                             :  1;\
            UNSG32 uCFG5_ups_yblank                            : 12;\
            UNSG32 uCFG5_ups_cblank                            : 12;\
            UNSG32 uCFG5_ups_use_blank                         :  1;\
            UNSG32 RSVDx14_b28                                 :  4;\
          }
    union { UNSG32 u32UPS12_CFG5;
            struct w32UPS12_CFG5;
          };
    #define     w32UPS12_CFG6                                  {\
            UNSG32 uCFG6_ups_y_th                              :  8;\
            UNSG32 uCFG6_ups_c_th                              :  8;\
            UNSG32 RSVDx18_b16                                 : 16;\
          }
    union { UNSG32 u32UPS12_CFG6;
            struct w32UPS12_CFG6;
          };
    #define     w32UPS12_CFG7                                  {\
            UNSG32 mCFG7_ups_7c0                               : 13;\
            UNSG32 mCFG7_ups_7c1                               : 13;\
            UNSG32 RSVDx1C_b26                                 :  6;\
          }
    union { UNSG32 u32UPS12_CFG7;
            struct w32UPS12_CFG7;
          };
    #define     w32UPS12_CFG8                                  {\
            UNSG32 mCFG8_ups_7c2                               : 13;\
            UNSG32 mCFG8_ups_7c3                               : 13;\
            UNSG32 RSVDx20_b26                                 :  6;\
          }
    union { UNSG32 u32UPS12_CFG8;
            struct w32UPS12_CFG8;
          };
    #define     w32UPS12_CFG9                                  {\
            UNSG32 mCFG9_ups_7c4                               : 13;\
            UNSG32 RSVDx24_b13                                 : 19;\
          }
    union { UNSG32 u32UPS12_CFG9;
            struct w32UPS12_CFG9;
          };
    } SIE_UPS12;
    typedef union  T32UPS12_CFG0
          { UNSG32 u32;
            struct w32UPS12_CFG0;
                 } T32UPS12_CFG0;
    typedef union  T32UPS12_CFG1
          { UNSG32 u32;
            struct w32UPS12_CFG1;
                 } T32UPS12_CFG1;
    typedef union  T32UPS12_CFG2
          { UNSG32 u32;
            struct w32UPS12_CFG2;
                 } T32UPS12_CFG2;
    typedef union  T32UPS12_CFG3
          { UNSG32 u32;
            struct w32UPS12_CFG3;
                 } T32UPS12_CFG3;
    typedef union  T32UPS12_CFG4
          { UNSG32 u32;
            struct w32UPS12_CFG4;
                 } T32UPS12_CFG4;
    typedef union  T32UPS12_CFG5
          { UNSG32 u32;
            struct w32UPS12_CFG5;
                 } T32UPS12_CFG5;
    typedef union  T32UPS12_CFG6
          { UNSG32 u32;
            struct w32UPS12_CFG6;
                 } T32UPS12_CFG6;
    typedef union  T32UPS12_CFG7
          { UNSG32 u32;
            struct w32UPS12_CFG7;
                 } T32UPS12_CFG7;
    typedef union  T32UPS12_CFG8
          { UNSG32 u32;
            struct w32UPS12_CFG8;
                 } T32UPS12_CFG8;
    typedef union  T32UPS12_CFG9
          { UNSG32 u32;
            struct w32UPS12_CFG9;
                 } T32UPS12_CFG9;
    typedef union  TUPS12_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG0;
                   };
                 } TUPS12_CFG0;
    typedef union  TUPS12_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG1;
                   };
                 } TUPS12_CFG1;
    typedef union  TUPS12_CFG2
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG2;
                   };
                 } TUPS12_CFG2;
    typedef union  TUPS12_CFG3
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG3;
                   };
                 } TUPS12_CFG3;
    typedef union  TUPS12_CFG4
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG4;
                   };
                 } TUPS12_CFG4;
    typedef union  TUPS12_CFG5
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG5;
                   };
                 } TUPS12_CFG5;
    typedef union  TUPS12_CFG6
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG6;
                   };
                 } TUPS12_CFG6;
    typedef union  TUPS12_CFG7
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG7;
                   };
                 } TUPS12_CFG7;
    typedef union  TUPS12_CFG8
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG8;
                   };
                 } TUPS12_CFG8;
    typedef union  TUPS12_CFG9
          { UNSG32 u32[1];
            struct {
            struct w32UPS12_CFG9;
                   };
                 } TUPS12_CFG9;
     SIGN32 UPS12_drvrd(SIE_UPS12 *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 UPS12_drvwr(SIE_UPS12 *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void UPS12_reset(SIE_UPS12 *p);
     SIGN32 UPS12_cmp  (SIE_UPS12 *p, SIE_UPS12 *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define UPS12_check(p,pie,pfx,hLOG) UPS12_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define UPS12_print(p,    pfx,hLOG) UPS12_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_UPS_420_422_SP
#define h_UPS_420_422_SP (){}
    #define     RA_UPS_420_422_SP_CFG0                         0x0000
    #define     RA_UPS_420_422_SP_CFG1                         0x0004
    #define     RA_UPS_420_422_SP_CFG2                         0x0008
    #define     RA_UPS_420_422_SP_CFG3                         0x000C
    #define     RA_UPS_420_422_SP_CFG4                         0x0010
    #define     RA_UPS_420_422_SP_CFG5                         0x0014
    #define     RA_UPS_420_422_SP_CFG6                         0x0018
    #define     RA_UPS_420_422_SP_CFG7                         0x001C
    #define     RA_UPS_420_422_SP_CFG8                         0x0020
    typedef struct SIE_UPS_420_422_SP {
    #define     w32UPS_420_422_SP_CFG0                         {\
            UNSG32 uCFG0_enable                                :  1;\
            UNSG32 uCFG0_cswap_regs                            :  1;\
            UNSG32 uCFG0_yshift_regs                           :  1;\
            UNSG32 uCFG0_cshift_regs                           :  1;\
            UNSG32 uCFG0_sft_rst                               :  1;\
            UNSG32 uCFG0_use_blank                             :  1;\
            UNSG32 uCFG0_yblank_regs                           :  8;\
            UNSG32 uCFG0_cblank_regs                           :  8;\
            UNSG32 RSVDx0_b22                                  : 10;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG0;
            struct w32UPS_420_422_SP_CFG0;
          };
    #define     w32UPS_420_422_SP_CFG1                         {\
            UNSG32 uCFG1_ups_c0                                : 13;\
            UNSG32 uCFG1_ups_c1                                : 13;\
            UNSG32 RSVDx4_b26                                  :  6;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG1;
            struct w32UPS_420_422_SP_CFG1;
          };
    #define     w32UPS_420_422_SP_CFG2                         {\
            UNSG32 uCFG2_ups_c2                                : 13;\
            UNSG32 uCFG2_ups_c3                                : 13;\
            UNSG32 RSVDx8_b26                                  :  6;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG2;
            struct w32UPS_420_422_SP_CFG2;
          };
    #define     w32UPS_420_422_SP_CFG3                         {\
            UNSG32 uCFG3_ups_c4                                : 13;\
            UNSG32 uCFG3_ups_c5                                : 13;\
            UNSG32 RSVDxC_b26                                  :  6;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG3;
            struct w32UPS_420_422_SP_CFG3;
          };
    #define     w32UPS_420_422_SP_CFG4                         {\
            UNSG32 uCFG4_ups_c6                                : 13;\
            UNSG32 RSVDx10_b13                                 : 19;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG4;
            struct w32UPS_420_422_SP_CFG4;
          };
    #define     w32UPS_420_422_SP_CFG5                         {\
            UNSG32 uCFG5_hres_regs                             : 13;\
            UNSG32 uCFG5_htot_regs                             : 13;\
            UNSG32 uCFG5_auto_pixcnt                           :  1;\
            UNSG32 RSVDx14_b27                                 :  5;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG5;
            struct w32UPS_420_422_SP_CFG5;
          };
    #define     w32UPS_420_422_SP_CFG6                         {\
            UNSG32 uCFG6_top_crop                              :  6;\
            UNSG32 uCFG6_bot_crop                              :  6;\
            UNSG32 uCFG6_left_crop                             :  6;\
            UNSG32 uCFG6_right_crop                            :  6;\
            UNSG32 RSVDx18_b24                                 :  8;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG6;
            struct w32UPS_420_422_SP_CFG6;
          };
    #define     w32UPS_420_422_SP_CFG7                         {\
            UNSG32 uCFG7_vres_regs                             : 13;\
            UNSG32 RSVDx1C_b13                                 : 19;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG7;
            struct w32UPS_420_422_SP_CFG7;
          };
    #define     w32UPS_420_422_SP_CFG8                         {\
            UNSG32 uCFG8_pdwn_regs                             :  1;\
            UNSG32 uCFG8_pdlvmc_regs                           :  1;\
            UNSG32 uCFG8_pdfvssm_regs                          :  1;\
            UNSG32 uCFG8_dpwr_regs                             :  2;\
            UNSG32 RSVDx20_b5                                  : 27;\
          }
    union { UNSG32 u32UPS_420_422_SP_CFG8;
            struct w32UPS_420_422_SP_CFG8;
          };
    } SIE_UPS_420_422_SP;
    typedef union  T32UPS_420_422_SP_CFG0
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG0;
                 } T32UPS_420_422_SP_CFG0;
    typedef union  T32UPS_420_422_SP_CFG1
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG1;
                 } T32UPS_420_422_SP_CFG1;
    typedef union  T32UPS_420_422_SP_CFG2
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG2;
                 } T32UPS_420_422_SP_CFG2;
    typedef union  T32UPS_420_422_SP_CFG3
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG3;
                 } T32UPS_420_422_SP_CFG3;
    typedef union  T32UPS_420_422_SP_CFG4
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG4;
                 } T32UPS_420_422_SP_CFG4;
    typedef union  T32UPS_420_422_SP_CFG5
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG5;
                 } T32UPS_420_422_SP_CFG5;
    typedef union  T32UPS_420_422_SP_CFG6
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG6;
                 } T32UPS_420_422_SP_CFG6;
    typedef union  T32UPS_420_422_SP_CFG7
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG7;
                 } T32UPS_420_422_SP_CFG7;
    typedef union  T32UPS_420_422_SP_CFG8
          { UNSG32 u32;
            struct w32UPS_420_422_SP_CFG8;
                 } T32UPS_420_422_SP_CFG8;
    typedef union  TUPS_420_422_SP_CFG0
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG0;
                   };
                 } TUPS_420_422_SP_CFG0;
    typedef union  TUPS_420_422_SP_CFG1
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG1;
                   };
                 } TUPS_420_422_SP_CFG1;
    typedef union  TUPS_420_422_SP_CFG2
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG2;
                   };
                 } TUPS_420_422_SP_CFG2;
    typedef union  TUPS_420_422_SP_CFG3
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG3;
                   };
                 } TUPS_420_422_SP_CFG3;
    typedef union  TUPS_420_422_SP_CFG4
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG4;
                   };
                 } TUPS_420_422_SP_CFG4;
    typedef union  TUPS_420_422_SP_CFG5
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG5;
                   };
                 } TUPS_420_422_SP_CFG5;
    typedef union  TUPS_420_422_SP_CFG6
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG6;
                   };
                 } TUPS_420_422_SP_CFG6;
    typedef union  TUPS_420_422_SP_CFG7
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG7;
                   };
                 } TUPS_420_422_SP_CFG7;
    typedef union  TUPS_420_422_SP_CFG8
          { UNSG32 u32[1];
            struct {
            struct w32UPS_420_422_SP_CFG8;
                   };
                 } TUPS_420_422_SP_CFG8;
     SIGN32 UPS_420_422_SP_drvrd(SIE_UPS_420_422_SP *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 UPS_420_422_SP_drvwr(SIE_UPS_420_422_SP *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void UPS_420_422_SP_reset(SIE_UPS_420_422_SP *p);
     SIGN32 UPS_420_422_SP_cmp  (SIE_UPS_420_422_SP *p, SIE_UPS_420_422_SP *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define UPS_420_422_SP_check(p,pie,pfx,hLOG) UPS_420_422_SP_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define UPS_420_422_SP_print(p,    pfx,hLOG) UPS_420_422_SP_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_TG_PL
#define h_TG_PL (){}
    #define     RA_TG_PL_X                                     0x0000
    #define     RA_TG_PL_Y                                     0x0004
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
    #define     RA_TG_PRG_CTRL1                                0x0004
    #define     RA_TG_PRG_Total                                0x0008
    #define     RA_TG_PRG_Initial                              0x000C
    #define     RA_TG_PRG_HSYNC                                0x0010
    #define     RA_TG_PRG_VSYNC                                0x0014
    #define     RA_TG_PRG_VS                                   0x0018
    #define     RA_TG_PRG_FT                                   0x001C
    #define     RA_TG_PRG_VX                                   0x0020
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
    #define     RA_TG_SIZE                                     0x0004
    #define     RA_TG_HS                                       0x0008
    #define     RA_TG_HB                                       0x000C
    #define     RA_TG_HB_CR                                    0x0010
    #define     RA_TG_HB_CR2                                   0x0014
    #define     RA_TG_VS0                                      0x0018
    #define     RA_TG_VS1                                      0x001C
    #define     RA_TG_VB0                                      0x0020
    #define     RA_TG_VB0_CR                                   0x0024
    #define     RA_TG_VB0_CR2                                  0x0028
    #define     RA_TG_VB1                                      0x002C
    #define     RA_TG_SCAN                                     0x0030
    #define        TG_SCAN_MODE_PROG                                        0x0
    #define        TG_SCAN_MODE_INTER                                       0x1
    #define     RA_TG_INTPOS                                   0x0034
    #define     RA_TG_MODE                                     0x0038
    #define        TG_MODE_EN_MASTER                                        0x0
    #define        TG_MODE_EN_SLAVE                                         0x1
    #define     RA_TG_HVREF                                    0x003C
    #define        TG_HVREF_SEL_SYNC                                        0x0
    #define        TG_HVREF_SEL_BLANK                                       0x1
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
    #define     RA_TG_MAIN_SIZE                                0x0004
    #define     RA_TG_MAIN_HS                                  0x0008
    #define     RA_TG_MAIN_HB                                  0x000C
    #define     RA_TG_MAIN_HB_Y                                0x0010
    #define     RA_TG_MAIN_HB_C                                0x0014
    #define     RA_TG_MAIN_HB_OUT                              0x0018
    #define     RA_TG_MAIN_VS0                                 0x001C
    #define     RA_TG_MAIN_VS1                                 0x0020
    #define     RA_TG_MAIN_VB0                                 0x0024
    #define     RA_TG_MAIN_VB0_Y                               0x0028
    #define     RA_TG_MAIN_VB0_C                               0x002C
    #define     RA_TG_MAIN_VB0_OUT                             0x0030
    #define     RA_TG_MAIN_VB1                                 0x0034
    #define     RA_TG_MAIN_SCAN                                0x0038
    #define        TG_MAIN_SCAN_MODE_PROG                                   0x0
    #define        TG_MAIN_SCAN_MODE_INTER                                  0x1
    #define     RA_TG_MAIN_INTPOS                              0x003C
    #define     RA_TG_MAIN_MODE                                0x0040
    #define        TG_MAIN_MODE_EN_MASTER                                   0x0
    #define        TG_MAIN_MODE_EN_SLAVE                                    0x1
    #define     RA_TG_MAIN_HVREF                               0x0044
    #define        TG_MAIN_HVREF_SEL_SYNC                                   0x0
    #define        TG_MAIN_HVREF_SEL_BLANK                                  0x1
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
    #define     RA_BITMAP40_SEL1                               0x0004
    #define     RA_BITMAP40_SEL2                               0x0008
    #define     RA_BITMAP40_SEL3                               0x000C
    #define     RA_BITMAP40_SEL4                               0x0010
    #define     RA_BITMAP40_SEL5                               0x0014
    #define     RA_BITMAP40_SEL6                               0x0018
    #define     RA_BITMAP40_SEL7                               0x001C
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
    #define     RA_BITMAP20_SEL1                               0x0004
    #define     RA_BITMAP20_SEL2                               0x0008
    #define     RA_BITMAP20_SEL3                               0x000C
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
    #define     RA_BITMAP32_SEL1                               0x0004
    #define     RA_BITMAP32_SEL2                               0x0008
    #define     RA_BITMAP32_SEL3                               0x000C
    #define     RA_BITMAP32_SEL4                               0x0010
    #define     RA_BITMAP32_SEL5                               0x0014
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
    #define     RA_BITMAP16_SEL1                               0x0004
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
    #define     RA_BITMAP12_SEL1                               0x0004
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
    #define     RA_ReadClient_Word                             0x0004
    #define     RA_ReadClient_NonStdRes                        0x0008
    #define     RA_ReadClient_pack                             0x000C
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
    #define     RA_WriteClient_pix                             0x0004
    #define     RA_WriteClient_NonStdRes                       0x0008
    #define     RA_WriteClient_pack                            0x000C
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
    #define     RA_ClientIF_MR0_word                           0x0004
    #define     RA_ClientIF_MR1                                0x0008
    #define     RA_ClientIF_MR1_word                           0x000C
    #define     RA_ClientIF_CTRL0                              0x0010
    #define     RA_ClientIF_DUMMY                              0x0014
    #define     RA_ClientIF_CTRL2                              0x0018
    #define     RA_ClientIF_CTRL3                              0x001C
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
#ifndef h_oneReg
#define h_oneReg (){}
    typedef struct SIE_oneReg {
            UNSG32 u_0x00000000                                : 32;
    } SIE_oneReg;
     SIGN32 oneReg_drvrd(SIE_oneReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 oneReg_drvwr(SIE_oneReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void oneReg_reset(SIE_oneReg *p);
     SIGN32 oneReg_cmp  (SIE_oneReg *p, SIE_oneReg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define oneReg_check(p,pie,pfx,hLOG) oneReg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define oneReg_print(p,    pfx,hLOG) oneReg_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_LCDC_REG
#define h_LCDC_REG (){}
    #define     RA_LCDC_REG_dummy                              0x0000
    typedef struct SIE_LCDC_REG {
              SIE_oneReg                                       ie_dummy[6143];
    } SIE_LCDC_REG;
     SIGN32 LCDC_REG_drvrd(SIE_LCDC_REG *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 LCDC_REG_drvwr(SIE_LCDC_REG *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void LCDC_REG_reset(SIE_LCDC_REG *p);
     SIGN32 LCDC_REG_cmp  (SIE_LCDC_REG *p, SIE_LCDC_REG *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define LCDC_REG_check(p,pie,pfx,hLOG) LCDC_REG_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define LCDC_REG_print(p,    pfx,hLOG) LCDC_REG_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifndef h_LCDC
#define h_LCDC (){}
    #define     RA_LCDC_cfgReg                                 0x0000
    #define     RA_LCDC_CTRL1                                  0x8000
    #define     RA_LCDC_CTRL2                                  0x8004
    #define     RA_LCDC_CTRL3                                  0x8008
    #define     RA_LCDC_CTRL4                                  0x800C
    #define     RA_LCDC_CTRL5                                  0x8010
    #define     RA_LCDC_CTRL6                                  0x8014
    #define     RA_LCDC_CTRL7                                  0x8018
    #define     RA_LCDC_CTRL1_NF0                              0x801C
    #define     RA_LCDC_CTRL2_NF0                              0x8020
    #define     RA_LCDC_DYNCTRL_NF0                            0x8024
    #define     RA_LCDC_CTRL3_NF0                              0x8028
    #define     RA_LCDC_CTRL4_NF0                              0x802C
    #define     RA_LCDC_CTRL5_NF0                              0x8030
    #define     RA_LCDC_LCDC_1BIT_RESOLUTION                   0x8034
    #define     RA_LCDC_LCDC_1BIT_COLOR_0                      0x8038
    #define     RA_LCDC_LCDC_1BIT_COLOR_1                      0x803C
    #define     RA_LCDC_DHUB_READ_In                           0x8040
    #define     RA_LCDC_DHUB_READ_Out                          0x8044
    #define     RA_LCDC_STATUS0                                0x8048
    #define     RA_LCDC_STATUS1                                0x804C
    #define     RA_LCDC_bitmap32_ctrl_R0                       0x8050
    #define     RA_LCDC_bitmap32_ctrl_R1                       0x8068
    #define     RA_LCDC_TG                                     0x8080
    #define     RA_LCDC_UPS8                                   0x80C0
    #define     RA_LCDC_UPS420_SP                              0x80E8
    #define     RA_LCDC_CSC                                    0x810C
    #define     RA_LCDC_dummy1                                 0x8148
    typedef struct SIE_LCDC {
              SIE_LCDC_REG                                     ie_cfgReg;
             UNSG8 RSVD_cfgReg                                 [8196];
    #define     w32LCDC_CTRL1                                  {\
            UNSG32 uCTRL1_wordTot_R0                           : 32;\
          }
    union { UNSG32 u32LCDC_CTRL1;
            struct w32LCDC_CTRL1;
          };
    #define     w32LCDC_CTRL2                                  {\
            UNSG32 uCTRL2_packSel_R0                           :  3;\
            UNSG32 uCTRL2_byteoffsetRGB_R0                     :  2;\
            UNSG32 uCTRL2_nonStdResEn_R0                       :  1;\
            UNSG32 uCTRL2_flushCnt_R0                          :  4;\
            UNSG32 uCTRL2_pixlineTot_R0                        : 13;\
            UNSG32 RSVDx8004_b23                               :  9;\
          }
    union { UNSG32 u32LCDC_CTRL2;
            struct w32LCDC_CTRL2;
          };
    #define     w32LCDC_CTRL3                                  {\
            UNSG32 uCTRL3_ClientR0_start                       :  1;\
            UNSG32 uCTRL3_ClientR0_clear                       :  1;\
            UNSG32 uCTRL3_ClientR1_start                       :  1;\
            UNSG32 uCTRL3_ClientR1_clear                       :  1;\
            UNSG32 RSVDx8008_b4                                : 28;\
          }
    union { UNSG32 u32LCDC_CTRL3;
            struct w32LCDC_CTRL3;
          };
    #define     w32LCDC_CTRL4                                  {\
            UNSG32 uCTRL4_bitmap32_en_R0                       :  1;\
            UNSG32 uCTRL4_bitmap128_en_R0                      :  1;\
            UNSG32 uCTRL4_en_syncinit2_wait                    :  1;\
            UNSG32 uCTRL4_bitmap32_en_R1                       :  1;\
            UNSG32 uCTRL4_bitmap128_en_R1                      :  1;\
            UNSG32 RSVDx800C_b5                                : 27;\
          }
    union { UNSG32 u32LCDC_CTRL4;
            struct w32LCDC_CTRL4;
          };
    #define     w32LCDC_CTRL5                                  {\
            UNSG32 uCTRL5_contiguous16                         :  1;\
            UNSG32 uCTRL5_b16as18                              :  1;\
            UNSG32 uCTRL5_cpu_mode18                           :  1;\
            UNSG32 uCTRL5_cpu_cmd_shift1                       :  1;\
            UNSG32 uCTRL5_cpu_mode24                           :  1;\
            UNSG32 uCTRL5_cpu_mux_exp_en                       :  1;\
            UNSG32 uCTRL5_cpu_mode16as18                       :  1;\
            UNSG32 uCTRL5_cpu_mode16as24                       :  1;\
            UNSG32 uCTRL5_oen_inv                              :  1;\
            UNSG32 uCTRL5_en_req_DE                            :  1;\
            UNSG32 uCTRL5_halt_en_front                        :  1;\
            UNSG32 uCTRL5_halt_en_back                         :  1;\
            UNSG32 uCTRL5_full_level_14_en                     :  1;\
            UNSG32 uCTRL5_bg_delay_ctrl_en                     :  1;\
            UNSG32 uCTRL5_auto_int_clear_sel                   :  6;\
            UNSG32 RSVDx8010_b20                               : 12;\
          }
    union { UNSG32 u32LCDC_CTRL5;
            struct w32LCDC_CTRL5;
          };
    #define     w32LCDC_CTRL6                                  {\
            UNSG32 uCTRL6_frst_halt_gen_delay                  : 11;\
            UNSG32 uCTRL6_debug_out_sel                        :  5;\
            UNSG32 RSVDx8014_b16                               : 16;\
          }
    union { UNSG32 u32LCDC_CTRL6;
            struct w32LCDC_CTRL6;
          };
    #define     w32LCDC_CTRL7                                  {\
            UNSG32 uCTRL7_oen_reg                              : 24;\
            UNSG32 uCTRL7_oen_reg_en                           :  1;\
            UNSG32 uCTRL7_data_inv_en                          :  1;\
            UNSG32 uCTRL7_fifo_lvl                             :  5;\
            UNSG32 RSVDx8018_b31                               :  1;\
          }
    union { UNSG32 u32LCDC_CTRL7;
            struct w32LCDC_CTRL7;
          };
    #define     w32LCDC_CTRL1_NF0                              {\
            UNSG32 uCTRL1_NF0_wordTot_R1                       : 32;\
          }
    union { UNSG32 u32LCDC_CTRL1_NF0;
            struct w32LCDC_CTRL1_NF0;
          };
    #define     w32LCDC_CTRL2_NF0                              {\
            UNSG32 uCTRL2_NF0_packSel_R1                       :  3;\
            UNSG32 uCTRL2_NF0_byteoffsetRGB_R1                 :  2;\
            UNSG32 uCTRL2_NF0_nonStdResEn_R1                   :  1;\
            UNSG32 uCTRL2_NF0_flushCnt_R1                      :  4;\
            UNSG32 uCTRL2_NF0_pixlineTot_R1                    : 13;\
            UNSG32 RSVDx8020_b23                               :  9;\
          }
    union { UNSG32 u32LCDC_CTRL2_NF0;
            struct w32LCDC_CTRL2_NF0;
          };
    #define     w32LCDC_DYNCTRL_NF0                            {\
            UNSG32 uDYNCTRL_NF0_tg_start                       :  1;\
            UNSG32 uDYNCTRL_NF0_tg_clear                       :  1;\
            UNSG32 uDYNCTRL_NF0_fifo_flush                     :  1;\
            UNSG32 uDYNCTRL_NF0_lcdc_1bit_lbuf_flush           :  1;\
            UNSG32 RSVDx8024_b4                                : 28;\
          }
    union { UNSG32 u32LCDC_DYNCTRL_NF0;
            struct w32LCDC_DYNCTRL_NF0;
          };
    #define     w32LCDC_CTRL3_NF0                              {\
            UNSG32 uCTRL3_NF0_clken_ctrl0                      :  1;\
            UNSG32 uCTRL3_NF0_clken_ctrl1                      :  1;\
            UNSG32 uCTRL3_NF0_clken_ctrl2                      :  1;\
            UNSG32 uCTRL3_NF0_clken_ctrl3                      :  1;\
            UNSG32 uCTRL3_NF0_rd_mask_sftrst                   :  1;\
            UNSG32 uCTRL3_NF0_rd_mask_initval0                 :  1;\
            UNSG32 uCTRL3_NF0_rd_mask_initval1                 :  1;\
            UNSG32 uCTRL3_NF0_crop_de_en                       :  1;\
            UNSG32 uCTRL3_NF0_chroma_re                        :  1;\
            UNSG32 uCTRL3_NF0_chroma_mask_en                   :  1;\
            UNSG32 uCTRL3_NF0_dhub_read_back_comp              :  1;\
            UNSG32 RSVDx8028_b11                               : 21;\
          }
    union { UNSG32 u32LCDC_CTRL3_NF0;
            struct w32LCDC_CTRL3_NF0;
          };
    #define     w32LCDC_CTRL4_NF0                              {\
            UNSG32 uCTRL4_NF0_idata_format_sel                 :  3;\
            UNSG32 uCTRL4_NF0_ups420_bypass                    :  1;\
            UNSG32 uCTRL4_NF0_ups422_bypass                    :  1;\
            UNSG32 uCTRL4_NF0_csc_bypass                       :  1;\
            UNSG32 uCTRL4_NF0_out_fifo_in_sel                  :  2;\
            UNSG32 uCTRL4_NF0_lcdc_1bit_scale                  :  2;\
            UNSG32 uCTRL4_NF0_CSC_IN_SWIZZLE                   :  3;\
            UNSG32 uCTRL4_NF0_CSC_SWIZZLE                      :  3;\
            UNSG32 RSVDx802C_b16                               : 16;\
          }
    union { UNSG32 u32LCDC_CTRL4_NF0;
            struct w32LCDC_CTRL4_NF0;
          };
    #define     w32LCDC_CTRL5_NF0                              {\
            UNSG32 uCTRL5_NF0_yblank_regs                      : 12;\
            UNSG32 uCTRL5_NF0_cblank_regs                      : 12;\
            UNSG32 RSVDx8030_b24                               :  8;\
          }
    union { UNSG32 u32LCDC_CTRL5_NF0;
            struct w32LCDC_CTRL5_NF0;
          };
    #define     w32LCDC_LCDC_1BIT_RESOLUTION                   {\
            UNSG32 uLCDC_1BIT_RESOLUTION_hsize                 : 16;\
            UNSG32 uLCDC_1BIT_RESOLUTION_vsize                 : 16;\
          }
    union { UNSG32 u32LCDC_LCDC_1BIT_RESOLUTION;
            struct w32LCDC_LCDC_1BIT_RESOLUTION;
          };
    #define     w32LCDC_LCDC_1BIT_COLOR_0                      {\
            UNSG32 uLCDC_1BIT_COLOR_0_rgb24                    : 24;\
            UNSG32 RSVDx8038_b24                               :  8;\
          }
    union { UNSG32 u32LCDC_LCDC_1BIT_COLOR_0;
            struct w32LCDC_LCDC_1BIT_COLOR_0;
          };
    #define     w32LCDC_LCDC_1BIT_COLOR_1                      {\
            UNSG32 uLCDC_1BIT_COLOR_1_rgb24                    : 24;\
            UNSG32 RSVDx803C_b24                               :  8;\
          }
    union { UNSG32 u32LCDC_LCDC_1BIT_COLOR_1;
            struct w32LCDC_LCDC_1BIT_COLOR_1;
          };
    #define     w32LCDC_DHUB_READ_In                           {\
            UNSG32 uDHUB_READ_In_pix_tot                       : 32;\
          }
    union { UNSG32 u32LCDC_DHUB_READ_In;
            struct w32LCDC_DHUB_READ_In;
          };
    #define     w32LCDC_DHUB_READ_Out                          {\
            UNSG32 uDHUB_READ_Out_pix_tot                      : 32;\
          }
    union { UNSG32 u32LCDC_DHUB_READ_Out;
            struct w32LCDC_DHUB_READ_Out;
          };
    #define     w32LCDC_STATUS0                                {\
            UNSG32 uSTATUS0_TGVCNT                             : 11;\
            UNSG32 uSTATUS0_SHUFFVCNT                          : 11;\
            UNSG32 RSVDx8048_b22                               : 10;\
          }
    union { UNSG32 u32LCDC_STATUS0;
            struct w32LCDC_STATUS0;
          };
    #define     w32LCDC_STATUS1                                {\
            UNSG32 uSTATUS1_GPVCNT                             : 11;\
            UNSG32 RSVDx804C_b11                               : 21;\
          }
    union { UNSG32 u32LCDC_STATUS1;
            struct w32LCDC_STATUS1;
          };
              SIE_BITMAP32                                     ie_bitmap32_ctrl_R0;
              SIE_BITMAP32                                     ie_bitmap32_ctrl_R1;
              SIE_TG                                           ie_TG;
              SIE_UPS8                                         ie_UPS8;
              SIE_UPS_420_422_SP                               ie_UPS420_SP;
              SIE_CSC_C17O24                                   ie_CSC;
    #define     w32LCDC_dummy1                                 {\
            UNSG32 udummy1_STS                                 : 32;\
          }
    union { UNSG32 u32LCDC_dummy1;
            struct w32LCDC_dummy1;
          };
             UNSG8 RSVDx814C                                   [32436];
    } SIE_LCDC;
    typedef union  T32LCDC_CTRL1
          { UNSG32 u32;
            struct w32LCDC_CTRL1;
                 } T32LCDC_CTRL1;
    typedef union  T32LCDC_CTRL2
          { UNSG32 u32;
            struct w32LCDC_CTRL2;
                 } T32LCDC_CTRL2;
    typedef union  T32LCDC_CTRL3
          { UNSG32 u32;
            struct w32LCDC_CTRL3;
                 } T32LCDC_CTRL3;
    typedef union  T32LCDC_CTRL4
          { UNSG32 u32;
            struct w32LCDC_CTRL4;
                 } T32LCDC_CTRL4;
    typedef union  T32LCDC_CTRL5
          { UNSG32 u32;
            struct w32LCDC_CTRL5;
                 } T32LCDC_CTRL5;
    typedef union  T32LCDC_CTRL6
          { UNSG32 u32;
            struct w32LCDC_CTRL6;
                 } T32LCDC_CTRL6;
    typedef union  T32LCDC_CTRL7
          { UNSG32 u32;
            struct w32LCDC_CTRL7;
                 } T32LCDC_CTRL7;
    typedef union  T32LCDC_CTRL1_NF0
          { UNSG32 u32;
            struct w32LCDC_CTRL1_NF0;
                 } T32LCDC_CTRL1_NF0;
    typedef union  T32LCDC_CTRL2_NF0
          { UNSG32 u32;
            struct w32LCDC_CTRL2_NF0;
                 } T32LCDC_CTRL2_NF0;
    typedef union  T32LCDC_DYNCTRL_NF0
          { UNSG32 u32;
            struct w32LCDC_DYNCTRL_NF0;
                 } T32LCDC_DYNCTRL_NF0;
    typedef union  T32LCDC_CTRL3_NF0
          { UNSG32 u32;
            struct w32LCDC_CTRL3_NF0;
                 } T32LCDC_CTRL3_NF0;
    typedef union  T32LCDC_CTRL4_NF0
          { UNSG32 u32;
            struct w32LCDC_CTRL4_NF0;
                 } T32LCDC_CTRL4_NF0;
    typedef union  T32LCDC_CTRL5_NF0
          { UNSG32 u32;
            struct w32LCDC_CTRL5_NF0;
                 } T32LCDC_CTRL5_NF0;
    typedef union  T32LCDC_LCDC_1BIT_RESOLUTION
          { UNSG32 u32;
            struct w32LCDC_LCDC_1BIT_RESOLUTION;
                 } T32LCDC_LCDC_1BIT_RESOLUTION;
    typedef union  T32LCDC_LCDC_1BIT_COLOR_0
          { UNSG32 u32;
            struct w32LCDC_LCDC_1BIT_COLOR_0;
                 } T32LCDC_LCDC_1BIT_COLOR_0;
    typedef union  T32LCDC_LCDC_1BIT_COLOR_1
          { UNSG32 u32;
            struct w32LCDC_LCDC_1BIT_COLOR_1;
                 } T32LCDC_LCDC_1BIT_COLOR_1;
    typedef union  T32LCDC_DHUB_READ_In
          { UNSG32 u32;
            struct w32LCDC_DHUB_READ_In;
                 } T32LCDC_DHUB_READ_In;
    typedef union  T32LCDC_DHUB_READ_Out
          { UNSG32 u32;
            struct w32LCDC_DHUB_READ_Out;
                 } T32LCDC_DHUB_READ_Out;
    typedef union  T32LCDC_STATUS0
          { UNSG32 u32;
            struct w32LCDC_STATUS0;
                 } T32LCDC_STATUS0;
    typedef union  T32LCDC_STATUS1
          { UNSG32 u32;
            struct w32LCDC_STATUS1;
                 } T32LCDC_STATUS1;
    typedef union  T32LCDC_dummy1
          { UNSG32 u32;
            struct w32LCDC_dummy1;
                 } T32LCDC_dummy1;
    typedef union  TLCDC_CTRL1
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL1;
                   };
                 } TLCDC_CTRL1;
    typedef union  TLCDC_CTRL2
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL2;
                   };
                 } TLCDC_CTRL2;
    typedef union  TLCDC_CTRL3
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL3;
                   };
                 } TLCDC_CTRL3;
    typedef union  TLCDC_CTRL4
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL4;
                   };
                 } TLCDC_CTRL4;
    typedef union  TLCDC_CTRL5
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL5;
                   };
                 } TLCDC_CTRL5;
    typedef union  TLCDC_CTRL6
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL6;
                   };
                 } TLCDC_CTRL6;
    typedef union  TLCDC_CTRL7
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL7;
                   };
                 } TLCDC_CTRL7;
    typedef union  TLCDC_CTRL1_NF0
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL1_NF0;
                   };
                 } TLCDC_CTRL1_NF0;
    typedef union  TLCDC_CTRL2_NF0
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL2_NF0;
                   };
                 } TLCDC_CTRL2_NF0;
    typedef union  TLCDC_DYNCTRL_NF0
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_DYNCTRL_NF0;
                   };
                 } TLCDC_DYNCTRL_NF0;
    typedef union  TLCDC_CTRL3_NF0
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL3_NF0;
                   };
                 } TLCDC_CTRL3_NF0;
    typedef union  TLCDC_CTRL4_NF0
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL4_NF0;
                   };
                 } TLCDC_CTRL4_NF0;
    typedef union  TLCDC_CTRL5_NF0
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_CTRL5_NF0;
                   };
                 } TLCDC_CTRL5_NF0;
    typedef union  TLCDC_LCDC_1BIT_RESOLUTION
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_LCDC_1BIT_RESOLUTION;
                   };
                 } TLCDC_LCDC_1BIT_RESOLUTION;
    typedef union  TLCDC_LCDC_1BIT_COLOR_0
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_LCDC_1BIT_COLOR_0;
                   };
                 } TLCDC_LCDC_1BIT_COLOR_0;
    typedef union  TLCDC_LCDC_1BIT_COLOR_1
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_LCDC_1BIT_COLOR_1;
                   };
                 } TLCDC_LCDC_1BIT_COLOR_1;
    typedef union  TLCDC_DHUB_READ_In
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_DHUB_READ_In;
                   };
                 } TLCDC_DHUB_READ_In;
    typedef union  TLCDC_DHUB_READ_Out
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_DHUB_READ_Out;
                   };
                 } TLCDC_DHUB_READ_Out;
    typedef union  TLCDC_STATUS0
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_STATUS0;
                   };
                 } TLCDC_STATUS0;
    typedef union  TLCDC_STATUS1
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_STATUS1;
                   };
                 } TLCDC_STATUS1;
    typedef union  TLCDC_dummy1
          { UNSG32 u32[1];
            struct {
            struct w32LCDC_dummy1;
                   };
                 } TLCDC_dummy1;
     SIGN32 LCDC_drvrd(SIE_LCDC *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 LCDC_drvwr(SIE_LCDC *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void LCDC_reset(SIE_LCDC *p);
     SIGN32 LCDC_cmp  (SIE_LCDC *p, SIE_LCDC *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define LCDC_check(p,pie,pfx,hLOG) LCDC_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define LCDC_print(p,    pfx,hLOG) LCDC_cmp(p,0,  pfx,(void*)(hLOG),0,0)
#endif
#ifdef __cplusplus
  }
#endif
#pragma  pack()
#endif
