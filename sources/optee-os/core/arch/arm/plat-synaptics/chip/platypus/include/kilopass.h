// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef kilopass_h
#define kilopass_h (){}


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



//////
/// 
/// $INTERFACE OneReg                                   (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 OneReg               (RW)
///               %unsigned 8  Byte0                     0x0
///               %unsigned 8  Byte1                     0x0
///               %unsigned 8  Byte2                     0x0
///               %unsigned 8  Byte3                     0x0
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:       4B, bits:      32b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_OneReg
#define h_OneReg (){}

    #define     RA_OneReg_OneReg                               0x0000

    #define     BA_OneReg_OneReg_Byte0                         0x0000
    #define     B16OneReg_OneReg_Byte0                         0x0000
    #define   LSb32OneReg_OneReg_Byte0                            0
    #define   LSb16OneReg_OneReg_Byte0                            0
    #define       bOneReg_OneReg_Byte0                         8
    #define   MSK32OneReg_OneReg_Byte0                            0x000000FF

    #define     BA_OneReg_OneReg_Byte1                         0x0001
    #define     B16OneReg_OneReg_Byte1                         0x0000
    #define   LSb32OneReg_OneReg_Byte1                            8
    #define   LSb16OneReg_OneReg_Byte1                            8
    #define       bOneReg_OneReg_Byte1                         8
    #define   MSK32OneReg_OneReg_Byte1                            0x0000FF00

    #define     BA_OneReg_OneReg_Byte2                         0x0002
    #define     B16OneReg_OneReg_Byte2                         0x0002
    #define   LSb32OneReg_OneReg_Byte2                            16
    #define   LSb16OneReg_OneReg_Byte2                            0
    #define       bOneReg_OneReg_Byte2                         8
    #define   MSK32OneReg_OneReg_Byte2                            0x00FF0000

    #define     BA_OneReg_OneReg_Byte3                         0x0003
    #define     B16OneReg_OneReg_Byte3                         0x0002
    #define   LSb32OneReg_OneReg_Byte3                            24
    #define   LSb16OneReg_OneReg_Byte3                            8
    #define       bOneReg_OneReg_Byte3                         8
    #define   MSK32OneReg_OneReg_Byte3                            0xFF000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_OneReg {
    ///////////////////////////////////////////////////////////
    #define   GET32OneReg_OneReg_Byte0(r32)                    _BFGET_(r32, 7, 0)
    #define   SET32OneReg_OneReg_Byte0(r32,v)                  _BFSET_(r32, 7, 0,v)
    #define   GET16OneReg_OneReg_Byte0(r16)                    _BFGET_(r16, 7, 0)
    #define   SET16OneReg_OneReg_Byte0(r16,v)                  _BFSET_(r16, 7, 0,v)

    #define   GET32OneReg_OneReg_Byte1(r32)                    _BFGET_(r32,15, 8)
    #define   SET32OneReg_OneReg_Byte1(r32,v)                  _BFSET_(r32,15, 8,v)
    #define   GET16OneReg_OneReg_Byte1(r16)                    _BFGET_(r16,15, 8)
    #define   SET16OneReg_OneReg_Byte1(r16,v)                  _BFSET_(r16,15, 8,v)

    #define   GET32OneReg_OneReg_Byte2(r32)                    _BFGET_(r32,23,16)
    #define   SET32OneReg_OneReg_Byte2(r32,v)                  _BFSET_(r32,23,16,v)
    #define   GET16OneReg_OneReg_Byte2(r16)                    _BFGET_(r16, 7, 0)
    #define   SET16OneReg_OneReg_Byte2(r16,v)                  _BFSET_(r16, 7, 0,v)

    #define   GET32OneReg_OneReg_Byte3(r32)                    _BFGET_(r32,31,24)
    #define   SET32OneReg_OneReg_Byte3(r32,v)                  _BFSET_(r32,31,24,v)
    #define   GET16OneReg_OneReg_Byte3(r16)                    _BFGET_(r16,15, 8)
    #define   SET16OneReg_OneReg_Byte3(r16,v)                  _BFSET_(r16,15, 8,v)

    #define     w32OneReg_OneReg                               {\
            UNSG32 uOneReg_Byte0                               :  8;\
            UNSG32 uOneReg_Byte1                               :  8;\
            UNSG32 uOneReg_Byte2                               :  8;\
            UNSG32 uOneReg_Byte3                               :  8;\
          }
    union { UNSG32 u32OneReg_OneReg;
            struct w32OneReg_OneReg;
          };
    ///////////////////////////////////////////////////////////
    } SIE_OneReg;

    typedef union  T32OneReg_OneReg
          { UNSG32 u32;
            struct w32OneReg_OneReg;
                 } T32OneReg_OneReg;
    ///////////////////////////////////////////////////////////

    typedef union  TOneReg_OneReg
          { UNSG32 u32[1];
            struct {
            struct w32OneReg_OneReg;
                   };
                 } TOneReg_OneReg;

    ///////////////////////////////////////////////////////////
     SIGN32 OneReg_drvrd(SIE_OneReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 OneReg_drvwr(SIE_OneReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void OneReg_reset(SIE_OneReg *p);
     SIGN32 OneReg_cmp  (SIE_OneReg *p, SIE_OneReg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define OneReg_check(p,pie,pfx,hLOG) OneReg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define OneReg_print(p,    pfx,hLOG) OneReg_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: OneReg
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE OneReg_RO                                (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 OneReg_RO            (R-)
///               %unsigned 8  Byte0                     0x0
///               %unsigned 8  Byte1                     0x0
///               %unsigned 8  Byte2                     0x0
///               %unsigned 8  Byte3                     0x0
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:       4B, bits:      32b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_OneReg_RO
#define h_OneReg_RO (){}

    #define     RA_OneReg_RO_OneReg_RO                         0x0000

    #define     BA_OneReg_RO_OneReg_RO_Byte0                   0x0000
    #define     B16OneReg_RO_OneReg_RO_Byte0                   0x0000
    #define   LSb32OneReg_RO_OneReg_RO_Byte0                      0
    #define   LSb16OneReg_RO_OneReg_RO_Byte0                      0
    #define       bOneReg_RO_OneReg_RO_Byte0                   8
    #define   MSK32OneReg_RO_OneReg_RO_Byte0                      0x000000FF

    #define     BA_OneReg_RO_OneReg_RO_Byte1                   0x0001
    #define     B16OneReg_RO_OneReg_RO_Byte1                   0x0000
    #define   LSb32OneReg_RO_OneReg_RO_Byte1                      8
    #define   LSb16OneReg_RO_OneReg_RO_Byte1                      8
    #define       bOneReg_RO_OneReg_RO_Byte1                   8
    #define   MSK32OneReg_RO_OneReg_RO_Byte1                      0x0000FF00

    #define     BA_OneReg_RO_OneReg_RO_Byte2                   0x0002
    #define     B16OneReg_RO_OneReg_RO_Byte2                   0x0002
    #define   LSb32OneReg_RO_OneReg_RO_Byte2                      16
    #define   LSb16OneReg_RO_OneReg_RO_Byte2                      0
    #define       bOneReg_RO_OneReg_RO_Byte2                   8
    #define   MSK32OneReg_RO_OneReg_RO_Byte2                      0x00FF0000

    #define     BA_OneReg_RO_OneReg_RO_Byte3                   0x0003
    #define     B16OneReg_RO_OneReg_RO_Byte3                   0x0002
    #define   LSb32OneReg_RO_OneReg_RO_Byte3                      24
    #define   LSb16OneReg_RO_OneReg_RO_Byte3                      8
    #define       bOneReg_RO_OneReg_RO_Byte3                   8
    #define   MSK32OneReg_RO_OneReg_RO_Byte3                      0xFF000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_OneReg_RO {
    ///////////////////////////////////////////////////////////
    #define   GET32OneReg_RO_OneReg_RO_Byte0(r32)              _BFGET_(r32, 7, 0)
    #define   SET32OneReg_RO_OneReg_RO_Byte0(r32,v)            _BFSET_(r32, 7, 0,v)
    #define   GET16OneReg_RO_OneReg_RO_Byte0(r16)              _BFGET_(r16, 7, 0)
    #define   SET16OneReg_RO_OneReg_RO_Byte0(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32OneReg_RO_OneReg_RO_Byte1(r32)              _BFGET_(r32,15, 8)
    #define   SET32OneReg_RO_OneReg_RO_Byte1(r32,v)            _BFSET_(r32,15, 8,v)
    #define   GET16OneReg_RO_OneReg_RO_Byte1(r16)              _BFGET_(r16,15, 8)
    #define   SET16OneReg_RO_OneReg_RO_Byte1(r16,v)            _BFSET_(r16,15, 8,v)

    #define   GET32OneReg_RO_OneReg_RO_Byte2(r32)              _BFGET_(r32,23,16)
    #define   SET32OneReg_RO_OneReg_RO_Byte2(r32,v)            _BFSET_(r32,23,16,v)
    #define   GET16OneReg_RO_OneReg_RO_Byte2(r16)              _BFGET_(r16, 7, 0)
    #define   SET16OneReg_RO_OneReg_RO_Byte2(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32OneReg_RO_OneReg_RO_Byte3(r32)              _BFGET_(r32,31,24)
    #define   SET32OneReg_RO_OneReg_RO_Byte3(r32,v)            _BFSET_(r32,31,24,v)
    #define   GET16OneReg_RO_OneReg_RO_Byte3(r16)              _BFGET_(r16,15, 8)
    #define   SET16OneReg_RO_OneReg_RO_Byte3(r16,v)            _BFSET_(r16,15, 8,v)

    #define     w32OneReg_RO_OneReg_RO                         {\
            UNSG32 uOneReg_RO_Byte0                            :  8;\
            UNSG32 uOneReg_RO_Byte1                            :  8;\
            UNSG32 uOneReg_RO_Byte2                            :  8;\
            UNSG32 uOneReg_RO_Byte3                            :  8;\
          }
    union { UNSG32 u32OneReg_RO_OneReg_RO;
            struct w32OneReg_RO_OneReg_RO;
          };
    ///////////////////////////////////////////////////////////
    } SIE_OneReg_RO;

    typedef union  T32OneReg_RO_OneReg_RO
          { UNSG32 u32;
            struct w32OneReg_RO_OneReg_RO;
                 } T32OneReg_RO_OneReg_RO;
    ///////////////////////////////////////////////////////////

    typedef union  TOneReg_RO_OneReg_RO
          { UNSG32 u32[1];
            struct {
            struct w32OneReg_RO_OneReg_RO;
                   };
                 } TOneReg_RO_OneReg_RO;

    ///////////////////////////////////////////////////////////
     SIGN32 OneReg_RO_drvrd(SIE_OneReg_RO *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 OneReg_RO_drvwr(SIE_OneReg_RO *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void OneReg_RO_reset(SIE_OneReg_RO *p);
     SIGN32 OneReg_RO_cmp  (SIE_OneReg_RO *p, SIE_OneReg_RO *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define OneReg_RO_check(p,pie,pfx,hLOG) OneReg_RO_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define OneReg_RO_print(p,    pfx,hLOG) OneReg_RO_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: OneReg_RO
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE KilopassOTP_Pin                          (4,4)
///     ###
///     * ACPU can access this register
///     ###
///     # # ----------------------------------------------------------
///     @ 0x00000 OTP_A                (RW)
///               %unsigned 16 OTP_A                     0x0
///               %%        16         # Stuffing bits...
///     @ 0x00004 OTP_CEB              (RW)
///               %unsigned 1  OTP_CEB                   0x1
///               %%        31         # Stuffing bits...
///     @ 0x00008 OTP_CLE              (RW)
///               %unsigned 1  OTP_CLE                   0x0
///               %%        31         # Stuffing bits...
///     @ 0x0000C OTP_DLE              (RW)
///               %unsigned 1  OTP_DLE                   0x0
///               %%        31         # Stuffing bits...
///     @ 0x00010 OTP_PGMEN            (RW)
///               %unsigned 1  OTP_PGMEN                 0x0
///               %%        31         # Stuffing bits...
///     @ 0x00014 OTP_READEN           (RW)
///               %unsigned 1  OTP_READEN                0x0
///               %%        31         # Stuffing bits...
///     @ 0x00018 OTP_RSTB             (RW)
///               %unsigned 1  OTP_RSTB                  0x1
///               %%        31         # Stuffing bits...
///     @ 0x0001C OTP_WEB              (RW)
///               %unsigned 1  OTP_WEB                   0x1
///               %%        31         # Stuffing bits...
///     @ 0x00020 OTP_CPUMPEN          (RW)
///               %unsigned 1  OTP_CPUMPEN               0x0
///               %%        31         # Stuffing bits...
///     @ 0x00024 OTP_SELTM            (RW)
///               %unsigned 2  OTP_SELTM                 0x0
///               %%        30         # Stuffing bits...
///     @ 0x00028 OTP_LOCK             (RW)
///               %unsigned 1  OTP_LOCK                  0x0
///               %%        31         # Stuffing bits...
///     @ 0x0002C OTP_PWROK            (R-)
///               %unsigned 1  OTP_PWROK                 0x0
///               %%        31         # Stuffing bits...
///     @ 0x00030 OTP_D                (R-)
///               %unsigned 16 OTP_D                     0x0
///               %%        16         # Stuffing bits...
///     @ 0x00034 OTP_DIN              (RW)
///               %unsigned 1  OTP_DIN                   0x0
///               %%        31         # Stuffing bits...
///     @ 0x00038 OTP_READOK           (R-)
///               %unsigned 1  OTP_READOK                0x0
///               %%        31         # Stuffing bits...
///     @ 0x0003C OTP_READBLK          (RW)
///               %unsigned 1  OTP_READBLK               0x0
///               %%        31         # Stuffing bits...
///     @ 0x00040 OTP_V0F              (R-)
///               %unsigned 16 OTP_V0F                   0x0
///               %%        16         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:      68B, bits:      63b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_KilopassOTP_Pin
#define h_KilopassOTP_Pin (){}

    #define     RA_KilopassOTP_Pin_OTP_A                       0x0000

    #define     BA_KilopassOTP_Pin_OTP_A_OTP_A                 0x0000
    #define     B16KilopassOTP_Pin_OTP_A_OTP_A                 0x0000
    #define   LSb32KilopassOTP_Pin_OTP_A_OTP_A                    0
    #define   LSb16KilopassOTP_Pin_OTP_A_OTP_A                    0
    #define       bKilopassOTP_Pin_OTP_A_OTP_A                 16
    #define   MSK32KilopassOTP_Pin_OTP_A_OTP_A                    0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_CEB                     0x0004

    #define     BA_KilopassOTP_Pin_OTP_CEB_OTP_CEB             0x0004
    #define     B16KilopassOTP_Pin_OTP_CEB_OTP_CEB             0x0004
    #define   LSb32KilopassOTP_Pin_OTP_CEB_OTP_CEB                0
    #define   LSb16KilopassOTP_Pin_OTP_CEB_OTP_CEB                0
    #define       bKilopassOTP_Pin_OTP_CEB_OTP_CEB             1
    #define   MSK32KilopassOTP_Pin_OTP_CEB_OTP_CEB                0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_CLE                     0x0008

    #define     BA_KilopassOTP_Pin_OTP_CLE_OTP_CLE             0x0008
    #define     B16KilopassOTP_Pin_OTP_CLE_OTP_CLE             0x0008
    #define   LSb32KilopassOTP_Pin_OTP_CLE_OTP_CLE                0
    #define   LSb16KilopassOTP_Pin_OTP_CLE_OTP_CLE                0
    #define       bKilopassOTP_Pin_OTP_CLE_OTP_CLE             1
    #define   MSK32KilopassOTP_Pin_OTP_CLE_OTP_CLE                0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_DLE                     0x000C

    #define     BA_KilopassOTP_Pin_OTP_DLE_OTP_DLE             0x000C
    #define     B16KilopassOTP_Pin_OTP_DLE_OTP_DLE             0x000C
    #define   LSb32KilopassOTP_Pin_OTP_DLE_OTP_DLE                0
    #define   LSb16KilopassOTP_Pin_OTP_DLE_OTP_DLE                0
    #define       bKilopassOTP_Pin_OTP_DLE_OTP_DLE             1
    #define   MSK32KilopassOTP_Pin_OTP_DLE_OTP_DLE                0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_PGMEN                   0x0010

    #define     BA_KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN         0x0010
    #define     B16KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN         0x0010
    #define   LSb32KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN            0
    #define   LSb16KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN            0
    #define       bKilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN         1
    #define   MSK32KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN            0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_READEN                  0x0014

    #define     BA_KilopassOTP_Pin_OTP_READEN_OTP_READEN       0x0014
    #define     B16KilopassOTP_Pin_OTP_READEN_OTP_READEN       0x0014
    #define   LSb32KilopassOTP_Pin_OTP_READEN_OTP_READEN          0
    #define   LSb16KilopassOTP_Pin_OTP_READEN_OTP_READEN          0
    #define       bKilopassOTP_Pin_OTP_READEN_OTP_READEN       1
    #define   MSK32KilopassOTP_Pin_OTP_READEN_OTP_READEN          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_RSTB                    0x0018

    #define     BA_KilopassOTP_Pin_OTP_RSTB_OTP_RSTB           0x0018
    #define     B16KilopassOTP_Pin_OTP_RSTB_OTP_RSTB           0x0018
    #define   LSb32KilopassOTP_Pin_OTP_RSTB_OTP_RSTB              0
    #define   LSb16KilopassOTP_Pin_OTP_RSTB_OTP_RSTB              0
    #define       bKilopassOTP_Pin_OTP_RSTB_OTP_RSTB           1
    #define   MSK32KilopassOTP_Pin_OTP_RSTB_OTP_RSTB              0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_WEB                     0x001C

    #define     BA_KilopassOTP_Pin_OTP_WEB_OTP_WEB             0x001C
    #define     B16KilopassOTP_Pin_OTP_WEB_OTP_WEB             0x001C
    #define   LSb32KilopassOTP_Pin_OTP_WEB_OTP_WEB                0
    #define   LSb16KilopassOTP_Pin_OTP_WEB_OTP_WEB                0
    #define       bKilopassOTP_Pin_OTP_WEB_OTP_WEB             1
    #define   MSK32KilopassOTP_Pin_OTP_WEB_OTP_WEB                0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_CPUMPEN                 0x0020

    #define     BA_KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN     0x0020
    #define     B16KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN     0x0020
    #define   LSb32KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN        0
    #define   LSb16KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN        0
    #define       bKilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN     1
    #define   MSK32KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN        0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_SELTM                   0x0024

    #define     BA_KilopassOTP_Pin_OTP_SELTM_OTP_SELTM         0x0024
    #define     B16KilopassOTP_Pin_OTP_SELTM_OTP_SELTM         0x0024
    #define   LSb32KilopassOTP_Pin_OTP_SELTM_OTP_SELTM            0
    #define   LSb16KilopassOTP_Pin_OTP_SELTM_OTP_SELTM            0
    #define       bKilopassOTP_Pin_OTP_SELTM_OTP_SELTM         2
    #define   MSK32KilopassOTP_Pin_OTP_SELTM_OTP_SELTM            0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_LOCK                    0x0028

    #define     BA_KilopassOTP_Pin_OTP_LOCK_OTP_LOCK           0x0028
    #define     B16KilopassOTP_Pin_OTP_LOCK_OTP_LOCK           0x0028
    #define   LSb32KilopassOTP_Pin_OTP_LOCK_OTP_LOCK              0
    #define   LSb16KilopassOTP_Pin_OTP_LOCK_OTP_LOCK              0
    #define       bKilopassOTP_Pin_OTP_LOCK_OTP_LOCK           1
    #define   MSK32KilopassOTP_Pin_OTP_LOCK_OTP_LOCK              0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_PWROK                   0x002C

    #define     BA_KilopassOTP_Pin_OTP_PWROK_OTP_PWROK         0x002C
    #define     B16KilopassOTP_Pin_OTP_PWROK_OTP_PWROK         0x002C
    #define   LSb32KilopassOTP_Pin_OTP_PWROK_OTP_PWROK            0
    #define   LSb16KilopassOTP_Pin_OTP_PWROK_OTP_PWROK            0
    #define       bKilopassOTP_Pin_OTP_PWROK_OTP_PWROK         1
    #define   MSK32KilopassOTP_Pin_OTP_PWROK_OTP_PWROK            0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_D                       0x0030

    #define     BA_KilopassOTP_Pin_OTP_D_OTP_D                 0x0030
    #define     B16KilopassOTP_Pin_OTP_D_OTP_D                 0x0030
    #define   LSb32KilopassOTP_Pin_OTP_D_OTP_D                    0
    #define   LSb16KilopassOTP_Pin_OTP_D_OTP_D                    0
    #define       bKilopassOTP_Pin_OTP_D_OTP_D                 16
    #define   MSK32KilopassOTP_Pin_OTP_D_OTP_D                    0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_DIN                     0x0034

    #define     BA_KilopassOTP_Pin_OTP_DIN_OTP_DIN             0x0034
    #define     B16KilopassOTP_Pin_OTP_DIN_OTP_DIN             0x0034
    #define   LSb32KilopassOTP_Pin_OTP_DIN_OTP_DIN                0
    #define   LSb16KilopassOTP_Pin_OTP_DIN_OTP_DIN                0
    #define       bKilopassOTP_Pin_OTP_DIN_OTP_DIN             1
    #define   MSK32KilopassOTP_Pin_OTP_DIN_OTP_DIN                0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_READOK                  0x0038

    #define     BA_KilopassOTP_Pin_OTP_READOK_OTP_READOK       0x0038
    #define     B16KilopassOTP_Pin_OTP_READOK_OTP_READOK       0x0038
    #define   LSb32KilopassOTP_Pin_OTP_READOK_OTP_READOK          0
    #define   LSb16KilopassOTP_Pin_OTP_READOK_OTP_READOK          0
    #define       bKilopassOTP_Pin_OTP_READOK_OTP_READOK       1
    #define   MSK32KilopassOTP_Pin_OTP_READOK_OTP_READOK          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_READBLK                 0x003C

    #define     BA_KilopassOTP_Pin_OTP_READBLK_OTP_READBLK     0x003C
    #define     B16KilopassOTP_Pin_OTP_READBLK_OTP_READBLK     0x003C
    #define   LSb32KilopassOTP_Pin_OTP_READBLK_OTP_READBLK        0
    #define   LSb16KilopassOTP_Pin_OTP_READBLK_OTP_READBLK        0
    #define       bKilopassOTP_Pin_OTP_READBLK_OTP_READBLK     1
    #define   MSK32KilopassOTP_Pin_OTP_READBLK_OTP_READBLK        0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Pin_OTP_V0F                     0x0040

    #define     BA_KilopassOTP_Pin_OTP_V0F_OTP_V0F             0x0040
    #define     B16KilopassOTP_Pin_OTP_V0F_OTP_V0F             0x0040
    #define   LSb32KilopassOTP_Pin_OTP_V0F_OTP_V0F                0
    #define   LSb16KilopassOTP_Pin_OTP_V0F_OTP_V0F                0
    #define       bKilopassOTP_Pin_OTP_V0F_OTP_V0F             16
    #define   MSK32KilopassOTP_Pin_OTP_V0F_OTP_V0F                0x0000FFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_KilopassOTP_Pin {
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_A_OTP_A(r32)            _BFGET_(r32,15, 0)
    #define   SET32KilopassOTP_Pin_OTP_A_OTP_A(r32,v)          _BFSET_(r32,15, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_A_OTP_A(r16)            _BFGET_(r16,15, 0)
    #define   SET16KilopassOTP_Pin_OTP_A_OTP_A(r16,v)          _BFSET_(r16,15, 0,v)

    #define     w32KilopassOTP_Pin_OTP_A                       {\
            UNSG32 uOTP_A_OTP_A                                : 16;\
            UNSG32 RSVDx0_b16                                  : 16;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_A;
            struct w32KilopassOTP_Pin_OTP_A;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_CEB_OTP_CEB(r32)        _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_CEB_OTP_CEB(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_CEB_OTP_CEB(r16)        _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_CEB_OTP_CEB(r16,v)      _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_CEB                     {\
            UNSG32 uOTP_CEB_OTP_CEB                            :  1;\
            UNSG32 RSVDx4_b1                                   : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_CEB;
            struct w32KilopassOTP_Pin_OTP_CEB;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_CLE_OTP_CLE(r32)        _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_CLE_OTP_CLE(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_CLE_OTP_CLE(r16)        _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_CLE_OTP_CLE(r16,v)      _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_CLE                     {\
            UNSG32 uOTP_CLE_OTP_CLE                            :  1;\
            UNSG32 RSVDx8_b1                                   : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_CLE;
            struct w32KilopassOTP_Pin_OTP_CLE;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_DLE_OTP_DLE(r32)        _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_DLE_OTP_DLE(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_DLE_OTP_DLE(r16)        _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_DLE_OTP_DLE(r16,v)      _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_DLE                     {\
            UNSG32 uOTP_DLE_OTP_DLE                            :  1;\
            UNSG32 RSVDxC_b1                                   : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_DLE;
            struct w32KilopassOTP_Pin_OTP_DLE;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN(r32)    _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN(r16)    _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_PGMEN_OTP_PGMEN(r16,v)  _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_PGMEN                   {\
            UNSG32 uOTP_PGMEN_OTP_PGMEN                        :  1;\
            UNSG32 RSVDx10_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_PGMEN;
            struct w32KilopassOTP_Pin_OTP_PGMEN;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_READEN_OTP_READEN(r32)  _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_READEN_OTP_READEN(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_READEN_OTP_READEN(r16)  _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_READEN_OTP_READEN(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_READEN                  {\
            UNSG32 uOTP_READEN_OTP_READEN                      :  1;\
            UNSG32 RSVDx14_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_READEN;
            struct w32KilopassOTP_Pin_OTP_READEN;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_RSTB_OTP_RSTB(r32)      _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_RSTB_OTP_RSTB(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_RSTB_OTP_RSTB(r16)      _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_RSTB_OTP_RSTB(r16,v)    _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_RSTB                    {\
            UNSG32 uOTP_RSTB_OTP_RSTB                          :  1;\
            UNSG32 RSVDx18_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_RSTB;
            struct w32KilopassOTP_Pin_OTP_RSTB;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_WEB_OTP_WEB(r32)        _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_WEB_OTP_WEB(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_WEB_OTP_WEB(r16)        _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_WEB_OTP_WEB(r16,v)      _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_WEB                     {\
            UNSG32 uOTP_WEB_OTP_WEB                            :  1;\
            UNSG32 RSVDx1C_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_WEB;
            struct w32KilopassOTP_Pin_OTP_WEB;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_CPUMPEN_OTP_CPUMPEN(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_CPUMPEN                 {\
            UNSG32 uOTP_CPUMPEN_OTP_CPUMPEN                    :  1;\
            UNSG32 RSVDx20_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_CPUMPEN;
            struct w32KilopassOTP_Pin_OTP_CPUMPEN;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_SELTM_OTP_SELTM(r32)    _BFGET_(r32, 1, 0)
    #define   SET32KilopassOTP_Pin_OTP_SELTM_OTP_SELTM(r32,v)  _BFSET_(r32, 1, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_SELTM_OTP_SELTM(r16)    _BFGET_(r16, 1, 0)
    #define   SET16KilopassOTP_Pin_OTP_SELTM_OTP_SELTM(r16,v)  _BFSET_(r16, 1, 0,v)

    #define     w32KilopassOTP_Pin_OTP_SELTM                   {\
            UNSG32 uOTP_SELTM_OTP_SELTM                        :  2;\
            UNSG32 RSVDx24_b2                                  : 30;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_SELTM;
            struct w32KilopassOTP_Pin_OTP_SELTM;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_LOCK_OTP_LOCK(r32)      _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_LOCK_OTP_LOCK(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_LOCK_OTP_LOCK(r16)      _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_LOCK_OTP_LOCK(r16,v)    _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_LOCK                    {\
            UNSG32 uOTP_LOCK_OTP_LOCK                          :  1;\
            UNSG32 RSVDx28_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_LOCK;
            struct w32KilopassOTP_Pin_OTP_LOCK;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_PWROK_OTP_PWROK(r32)    _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_PWROK_OTP_PWROK(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_PWROK_OTP_PWROK(r16)    _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_PWROK_OTP_PWROK(r16,v)  _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_PWROK                   {\
            UNSG32 uOTP_PWROK_OTP_PWROK                        :  1;\
            UNSG32 RSVDx2C_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_PWROK;
            struct w32KilopassOTP_Pin_OTP_PWROK;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_D_OTP_D(r32)            _BFGET_(r32,15, 0)
    #define   SET32KilopassOTP_Pin_OTP_D_OTP_D(r32,v)          _BFSET_(r32,15, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_D_OTP_D(r16)            _BFGET_(r16,15, 0)
    #define   SET16KilopassOTP_Pin_OTP_D_OTP_D(r16,v)          _BFSET_(r16,15, 0,v)

    #define     w32KilopassOTP_Pin_OTP_D                       {\
            UNSG32 uOTP_D_OTP_D                                : 16;\
            UNSG32 RSVDx30_b16                                 : 16;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_D;
            struct w32KilopassOTP_Pin_OTP_D;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_DIN_OTP_DIN(r32)        _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_DIN_OTP_DIN(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_DIN_OTP_DIN(r16)        _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_DIN_OTP_DIN(r16,v)      _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_DIN                     {\
            UNSG32 uOTP_DIN_OTP_DIN                            :  1;\
            UNSG32 RSVDx34_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_DIN;
            struct w32KilopassOTP_Pin_OTP_DIN;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_READOK_OTP_READOK(r32)  _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_READOK_OTP_READOK(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_READOK_OTP_READOK(r16)  _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_READOK_OTP_READOK(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_READOK                  {\
            UNSG32 uOTP_READOK_OTP_READOK                      :  1;\
            UNSG32 RSVDx38_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_READOK;
            struct w32KilopassOTP_Pin_OTP_READOK;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_READBLK_OTP_READBLK(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassOTP_Pin_OTP_READBLK_OTP_READBLK(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_READBLK_OTP_READBLK(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassOTP_Pin_OTP_READBLK_OTP_READBLK(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassOTP_Pin_OTP_READBLK                 {\
            UNSG32 uOTP_READBLK_OTP_READBLK                    :  1;\
            UNSG32 RSVDx3C_b1                                  : 31;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_READBLK;
            struct w32KilopassOTP_Pin_OTP_READBLK;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Pin_OTP_V0F_OTP_V0F(r32)        _BFGET_(r32,15, 0)
    #define   SET32KilopassOTP_Pin_OTP_V0F_OTP_V0F(r32,v)      _BFSET_(r32,15, 0,v)
    #define   GET16KilopassOTP_Pin_OTP_V0F_OTP_V0F(r16)        _BFGET_(r16,15, 0)
    #define   SET16KilopassOTP_Pin_OTP_V0F_OTP_V0F(r16,v)      _BFSET_(r16,15, 0,v)

    #define     w32KilopassOTP_Pin_OTP_V0F                     {\
            UNSG32 uOTP_V0F_OTP_V0F                            : 16;\
            UNSG32 RSVDx40_b16                                 : 16;\
          }
    union { UNSG32 u32KilopassOTP_Pin_OTP_V0F;
            struct w32KilopassOTP_Pin_OTP_V0F;
          };
    ///////////////////////////////////////////////////////////
    } SIE_KilopassOTP_Pin;

    typedef union  T32KilopassOTP_Pin_OTP_A
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_A;
                 } T32KilopassOTP_Pin_OTP_A;
    typedef union  T32KilopassOTP_Pin_OTP_CEB
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_CEB;
                 } T32KilopassOTP_Pin_OTP_CEB;
    typedef union  T32KilopassOTP_Pin_OTP_CLE
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_CLE;
                 } T32KilopassOTP_Pin_OTP_CLE;
    typedef union  T32KilopassOTP_Pin_OTP_DLE
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_DLE;
                 } T32KilopassOTP_Pin_OTP_DLE;
    typedef union  T32KilopassOTP_Pin_OTP_PGMEN
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_PGMEN;
                 } T32KilopassOTP_Pin_OTP_PGMEN;
    typedef union  T32KilopassOTP_Pin_OTP_READEN
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_READEN;
                 } T32KilopassOTP_Pin_OTP_READEN;
    typedef union  T32KilopassOTP_Pin_OTP_RSTB
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_RSTB;
                 } T32KilopassOTP_Pin_OTP_RSTB;
    typedef union  T32KilopassOTP_Pin_OTP_WEB
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_WEB;
                 } T32KilopassOTP_Pin_OTP_WEB;
    typedef union  T32KilopassOTP_Pin_OTP_CPUMPEN
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_CPUMPEN;
                 } T32KilopassOTP_Pin_OTP_CPUMPEN;
    typedef union  T32KilopassOTP_Pin_OTP_SELTM
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_SELTM;
                 } T32KilopassOTP_Pin_OTP_SELTM;
    typedef union  T32KilopassOTP_Pin_OTP_LOCK
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_LOCK;
                 } T32KilopassOTP_Pin_OTP_LOCK;
    typedef union  T32KilopassOTP_Pin_OTP_PWROK
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_PWROK;
                 } T32KilopassOTP_Pin_OTP_PWROK;
    typedef union  T32KilopassOTP_Pin_OTP_D
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_D;
                 } T32KilopassOTP_Pin_OTP_D;
    typedef union  T32KilopassOTP_Pin_OTP_DIN
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_DIN;
                 } T32KilopassOTP_Pin_OTP_DIN;
    typedef union  T32KilopassOTP_Pin_OTP_READOK
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_READOK;
                 } T32KilopassOTP_Pin_OTP_READOK;
    typedef union  T32KilopassOTP_Pin_OTP_READBLK
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_READBLK;
                 } T32KilopassOTP_Pin_OTP_READBLK;
    typedef union  T32KilopassOTP_Pin_OTP_V0F
          { UNSG32 u32;
            struct w32KilopassOTP_Pin_OTP_V0F;
                 } T32KilopassOTP_Pin_OTP_V0F;
    ///////////////////////////////////////////////////////////

    typedef union  TKilopassOTP_Pin_OTP_A
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_A;
                   };
                 } TKilopassOTP_Pin_OTP_A;
    typedef union  TKilopassOTP_Pin_OTP_CEB
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_CEB;
                   };
                 } TKilopassOTP_Pin_OTP_CEB;
    typedef union  TKilopassOTP_Pin_OTP_CLE
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_CLE;
                   };
                 } TKilopassOTP_Pin_OTP_CLE;
    typedef union  TKilopassOTP_Pin_OTP_DLE
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_DLE;
                   };
                 } TKilopassOTP_Pin_OTP_DLE;
    typedef union  TKilopassOTP_Pin_OTP_PGMEN
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_PGMEN;
                   };
                 } TKilopassOTP_Pin_OTP_PGMEN;
    typedef union  TKilopassOTP_Pin_OTP_READEN
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_READEN;
                   };
                 } TKilopassOTP_Pin_OTP_READEN;
    typedef union  TKilopassOTP_Pin_OTP_RSTB
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_RSTB;
                   };
                 } TKilopassOTP_Pin_OTP_RSTB;
    typedef union  TKilopassOTP_Pin_OTP_WEB
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_WEB;
                   };
                 } TKilopassOTP_Pin_OTP_WEB;
    typedef union  TKilopassOTP_Pin_OTP_CPUMPEN
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_CPUMPEN;
                   };
                 } TKilopassOTP_Pin_OTP_CPUMPEN;
    typedef union  TKilopassOTP_Pin_OTP_SELTM
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_SELTM;
                   };
                 } TKilopassOTP_Pin_OTP_SELTM;
    typedef union  TKilopassOTP_Pin_OTP_LOCK
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_LOCK;
                   };
                 } TKilopassOTP_Pin_OTP_LOCK;
    typedef union  TKilopassOTP_Pin_OTP_PWROK
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_PWROK;
                   };
                 } TKilopassOTP_Pin_OTP_PWROK;
    typedef union  TKilopassOTP_Pin_OTP_D
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_D;
                   };
                 } TKilopassOTP_Pin_OTP_D;
    typedef union  TKilopassOTP_Pin_OTP_DIN
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_DIN;
                   };
                 } TKilopassOTP_Pin_OTP_DIN;
    typedef union  TKilopassOTP_Pin_OTP_READOK
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_READOK;
                   };
                 } TKilopassOTP_Pin_OTP_READOK;
    typedef union  TKilopassOTP_Pin_OTP_READBLK
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_READBLK;
                   };
                 } TKilopassOTP_Pin_OTP_READBLK;
    typedef union  TKilopassOTP_Pin_OTP_V0F
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Pin_OTP_V0F;
                   };
                 } TKilopassOTP_Pin_OTP_V0F;

    ///////////////////////////////////////////////////////////
     SIGN32 KilopassOTP_Pin_drvrd(SIE_KilopassOTP_Pin *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 KilopassOTP_Pin_drvwr(SIE_KilopassOTP_Pin *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void KilopassOTP_Pin_reset(SIE_KilopassOTP_Pin *p);
     SIGN32 KilopassOTP_Pin_cmp  (SIE_KilopassOTP_Pin *p, SIE_KilopassOTP_Pin *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define KilopassOTP_Pin_check(p,pie,pfx,hLOG) KilopassOTP_Pin_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define KilopassOTP_Pin_print(p,    pfx,hLOG) KilopassOTP_Pin_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: KilopassOTP_Pin
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE KilopassOTP_Timing                       (4,4)
///     ###
///     * ACPU can access this register
///     ###
///     # # ----------------------------------------------------------
///     @ 0x00000 CSRST                (P)
///               %unsigned 8  CSRST                     0x1E
///                                    ###
///                                    * CEB setup time before RSTB
///                                    * Min 1000ns, default is 1200ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00004 RW                   (P)
///               %unsigned 8  RW                        0x1
///                                    ###
///                                    * RSTB pulse width
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00008 RS                   (P)
///               %unsigned 8  RS                        0x1E
///                                    ###
///                                    * RSTB setup time before address
///                                    * Min 1000ns, default is 1200ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0000C AS                   (P)
///               %unsigned 8  AS                        0x1
///                                    ###
///                                    * Address setup time before WEB
///                                    * Min 25ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00010 DS                   (P)
///               %unsigned 8  DS                        0x1
///                                    ###
///                                    * Data setup time before WEB
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00014 DLES                 (P)
///               %unsigned 8  DLES                      0x1
///                                    ###
///                                    * DLE setup time before WEB
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00018 WWL                  (P)
///               %unsigned 8  WWL                       0x1
///                                    ###
///                                    * WEB pulse width low
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0001C DLEH                 (P)
///               %unsigned 8  DLEH                      0x1
///                                    ###
///                                    * DLE hold time after WEB
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00020 DH                   (P)
///               %unsigned 8  DH                        0x1
///                                    ###
///                                    * Data hold time after WEB
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00024 DPD                  (P)
///               %unsigned 8  DPD                       0x1
///                                    ###
///                                    * DLE de-assertion to PGMEN assertion delay
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00028 PES                  (P)
///               %unsigned 8  PES                       0x1E
///                                    ###
///                                    * PGMEN setup time before CPUMPEN
///                                    * Min 1000ns, default is 1200ns, value=30
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0002C CPS                  (P)
///               %unsigned 8  CPS                       0x1E
///                                    ###
///                                    * CPUMPEN setup time before WEN
///                                    * Min 1000ns, default is 1200ns
///                                    * Value=30
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00030 PW                   (P)
///               %unsigned 8  PW                        0x7D
///                                    ###
///                                    * Program pulse width for WEB
///                                    * Min 4500ns, Typical is 5000ns, Max is 5500ns, default is 5000ns
///                                    * Value =125
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00034 CPH                  (P)
///               %unsigned 8  CPH                       0x1E
///                                    ###
///                                    * CPUMPEN hold time after WEB
///                                    * Min 1000ns, default is 1200ns
///                                    * Value=30
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00038 PEH                  (P)
///               %unsigned 8  PEH                       0x4
///                                    ###
///                                    * CPUMPEN de-assert to PGMEN de-assert delay
///                                    * Min 120ns, Max 200ns, default is 160ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0003C PGMAS                (P)
///               %unsigned 8  PGMAS                     0x4
///                                    ###
///                                    * PGMEN de-assert to address change delay
///                                    * Min 100ns, default is 160ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00040 PGMRD                (P)
///               %unsigned 8  PGMRD                     0x3
///                                    ###
///                                    * PGMEN de-assert to READEN assert delay,
///                                    * Min 2000ns, default is 2400ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00044 RSRD                 (P)
///               %unsigned 8  RSRD                      0x1E
///                                    ###
///                                    * RSTB setup time before READEN
///                                    * Min 1000ns, default is 1200ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00048 READEN               (P)
///               %unsigned 8  READEN                    0x4
///                                    ###
///                                    * READEN to data output delay
///                                    * max 120ns, default is 160ns. This determines the read sample rate. 160ns = 4 cycles for one ready operation
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0004C CEW                  (P)
///               %unsigned 8  CEW                       0x1
///                                    ###
///                                    * Minimum CEB high pulse width
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00050 CERD                 (P)
///               %unsigned 8  CERD                      0x3C
///                                    ###
///                                    * CEB setup time before READEN
///                                    * Min 2020ns, default is 2400ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00054 NP2                  (P)
///               %unsigned 8  NP2                       0x15
///                                    ###
///                                    * Program attempt time. If hardware attempts this much time to program a bit but still fail, hardware will stop programming and return interrupt.
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00058 RSCLE                (P)
///               %unsigned 8  RSCLE                     0x1E
///                                    ###
///                                    * RSTB setup time before CLE
///                                    * Min 1000ns, default is 1200ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0005C TWS                  (P)
///               %unsigned 8  TWS                       0x1
///                                    ###
///                                    * Test word setup time before WEB
///                                    * Min 25ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00060 CLES                 (P)
///               %unsigned 8  CLES                      0x1
///                                    ###
///                                    * CLE setup time before WEB
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00064 CLEH                 (P)
///               %unsigned 8  CLEH                      0x1
///                                    ###
///                                    * CLE hold time after WEB
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00068 TWH                  (P)
///               %unsigned 8  TWH                       0x1
///                                    ###
///                                    * Test word hold time after WEB
///                                    * Min 25ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0006C WDRD                 (P)
///               %unsigned 8  WDRD                      0x1E
///                                    ###
///                                    * WEB de-assert to READEN assert delay
///                                    * Min 1000ns, default is 1200ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00070 BCREADEN             (P)
///               %unsigned 8  BCREADEN                  0x26
///                                    ###
///                                    * Read Enable to test output delay
///                                    * Max 1500ns, default is 1540ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00074 BCACC                (P)
///               %unsigned 8  BCACC                     0x26
///                                    ###
///                                    * Address to test output delay
///                                    * Max 1500ns, default is 1540ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00078 AAC                  (P)
///               %unsigned 8  AAC                       0x26
///                                    ###
///                                    * Blank Check Address to Address change
///                                    * Min 1500ns, Max 1600ns default is 1540ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0007C RRC                  (P)
///               %unsigned 8  RRC                       0x2D
///                                    ###
///                                    * Blank Check READEN assertion to READEN de-assertion time
///                                    * Max 1500ns, default is 1800ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00080 RDES                 (P)
///               %unsigned 8  RDES                      0x3
///                                    ###
///                                    * WEB to READEN SELTM Delay
///                                    * Max 100ns, default is 120ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00084 RWH                  (P)
///               %unsigned 8  RWH                       0x4
///                                    ###
///                                    * Minimum READEN
///                                    * Max 120ns, default is 160ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00088 READENT              (P)
///               %unsigned 8  READENT                   0x4
///                                    ###
///                                    * Read Enable to Test output Delay
///                                    * Max 120ns, default is 160ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0008C AACT                 (P)
///               %unsigned 8  AACT                      0x4
///                                    ###
///                                    * Address to Test Output Delay
///                                    * Max 120ns, default is 160ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00090 CDD                  (P)
///               %unsigned 8  CDD                       0x1
///                                    ###
///                                    * CLE de-assert to DLE assert delay
///                                    * Min 20ns, default is 40ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00094 PGMSD                (P)
///               %unsigned 8  PGMSD                     0x0
///                                    ###
///                                    * Address Change to Data Output Delay
///                                    * Max 0n, default is 0ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00098 PGMDD                (P)
///               %unsigned 8  PGMDD                     0x0
///                                    ###
///                                    * Address Change to Data Output Delay
///                                    * Max 0n, default is 0ns
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x0009C ACC                  (P)
///               %unsigned 8  ACC                       0x4
///                                    ###
///                                    * Address to Address change
///                                    * Max 120ns default is 160ns
///                                    ###
///               %%        24         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     160B, bits:     320b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_KilopassOTP_Timing
#define h_KilopassOTP_Timing (){}

    #define     RA_KilopassOTP_Timing_CSRST                    0x0000

    #define     BA_KilopassOTP_Timing_CSRST_CSRST              0x0000
    #define     B16KilopassOTP_Timing_CSRST_CSRST              0x0000
    #define   LSb32KilopassOTP_Timing_CSRST_CSRST                 0
    #define   LSb16KilopassOTP_Timing_CSRST_CSRST                 0
    #define       bKilopassOTP_Timing_CSRST_CSRST              8
    #define   MSK32KilopassOTP_Timing_CSRST_CSRST                 0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_RW                       0x0004

    #define     BA_KilopassOTP_Timing_RW_RW                    0x0004
    #define     B16KilopassOTP_Timing_RW_RW                    0x0004
    #define   LSb32KilopassOTP_Timing_RW_RW                       0
    #define   LSb16KilopassOTP_Timing_RW_RW                       0
    #define       bKilopassOTP_Timing_RW_RW                    8
    #define   MSK32KilopassOTP_Timing_RW_RW                       0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_RS                       0x0008

    #define     BA_KilopassOTP_Timing_RS_RS                    0x0008
    #define     B16KilopassOTP_Timing_RS_RS                    0x0008
    #define   LSb32KilopassOTP_Timing_RS_RS                       0
    #define   LSb16KilopassOTP_Timing_RS_RS                       0
    #define       bKilopassOTP_Timing_RS_RS                    8
    #define   MSK32KilopassOTP_Timing_RS_RS                       0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_AS                       0x000C

    #define     BA_KilopassOTP_Timing_AS_AS                    0x000C
    #define     B16KilopassOTP_Timing_AS_AS                    0x000C
    #define   LSb32KilopassOTP_Timing_AS_AS                       0
    #define   LSb16KilopassOTP_Timing_AS_AS                       0
    #define       bKilopassOTP_Timing_AS_AS                    8
    #define   MSK32KilopassOTP_Timing_AS_AS                       0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_DS                       0x0010

    #define     BA_KilopassOTP_Timing_DS_DS                    0x0010
    #define     B16KilopassOTP_Timing_DS_DS                    0x0010
    #define   LSb32KilopassOTP_Timing_DS_DS                       0
    #define   LSb16KilopassOTP_Timing_DS_DS                       0
    #define       bKilopassOTP_Timing_DS_DS                    8
    #define   MSK32KilopassOTP_Timing_DS_DS                       0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_DLES                     0x0014

    #define     BA_KilopassOTP_Timing_DLES_DLES                0x0014
    #define     B16KilopassOTP_Timing_DLES_DLES                0x0014
    #define   LSb32KilopassOTP_Timing_DLES_DLES                   0
    #define   LSb16KilopassOTP_Timing_DLES_DLES                   0
    #define       bKilopassOTP_Timing_DLES_DLES                8
    #define   MSK32KilopassOTP_Timing_DLES_DLES                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_WWL                      0x0018

    #define     BA_KilopassOTP_Timing_WWL_WWL                  0x0018
    #define     B16KilopassOTP_Timing_WWL_WWL                  0x0018
    #define   LSb32KilopassOTP_Timing_WWL_WWL                     0
    #define   LSb16KilopassOTP_Timing_WWL_WWL                     0
    #define       bKilopassOTP_Timing_WWL_WWL                  8
    #define   MSK32KilopassOTP_Timing_WWL_WWL                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_DLEH                     0x001C

    #define     BA_KilopassOTP_Timing_DLEH_DLEH                0x001C
    #define     B16KilopassOTP_Timing_DLEH_DLEH                0x001C
    #define   LSb32KilopassOTP_Timing_DLEH_DLEH                   0
    #define   LSb16KilopassOTP_Timing_DLEH_DLEH                   0
    #define       bKilopassOTP_Timing_DLEH_DLEH                8
    #define   MSK32KilopassOTP_Timing_DLEH_DLEH                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_DH                       0x0020

    #define     BA_KilopassOTP_Timing_DH_DH                    0x0020
    #define     B16KilopassOTP_Timing_DH_DH                    0x0020
    #define   LSb32KilopassOTP_Timing_DH_DH                       0
    #define   LSb16KilopassOTP_Timing_DH_DH                       0
    #define       bKilopassOTP_Timing_DH_DH                    8
    #define   MSK32KilopassOTP_Timing_DH_DH                       0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_DPD                      0x0024

    #define     BA_KilopassOTP_Timing_DPD_DPD                  0x0024
    #define     B16KilopassOTP_Timing_DPD_DPD                  0x0024
    #define   LSb32KilopassOTP_Timing_DPD_DPD                     0
    #define   LSb16KilopassOTP_Timing_DPD_DPD                     0
    #define       bKilopassOTP_Timing_DPD_DPD                  8
    #define   MSK32KilopassOTP_Timing_DPD_DPD                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_PES                      0x0028

    #define     BA_KilopassOTP_Timing_PES_PES                  0x0028
    #define     B16KilopassOTP_Timing_PES_PES                  0x0028
    #define   LSb32KilopassOTP_Timing_PES_PES                     0
    #define   LSb16KilopassOTP_Timing_PES_PES                     0
    #define       bKilopassOTP_Timing_PES_PES                  8
    #define   MSK32KilopassOTP_Timing_PES_PES                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_CPS                      0x002C

    #define     BA_KilopassOTP_Timing_CPS_CPS                  0x002C
    #define     B16KilopassOTP_Timing_CPS_CPS                  0x002C
    #define   LSb32KilopassOTP_Timing_CPS_CPS                     0
    #define   LSb16KilopassOTP_Timing_CPS_CPS                     0
    #define       bKilopassOTP_Timing_CPS_CPS                  8
    #define   MSK32KilopassOTP_Timing_CPS_CPS                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_PW                       0x0030

    #define     BA_KilopassOTP_Timing_PW_PW                    0x0030
    #define     B16KilopassOTP_Timing_PW_PW                    0x0030
    #define   LSb32KilopassOTP_Timing_PW_PW                       0
    #define   LSb16KilopassOTP_Timing_PW_PW                       0
    #define       bKilopassOTP_Timing_PW_PW                    8
    #define   MSK32KilopassOTP_Timing_PW_PW                       0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_CPH                      0x0034

    #define     BA_KilopassOTP_Timing_CPH_CPH                  0x0034
    #define     B16KilopassOTP_Timing_CPH_CPH                  0x0034
    #define   LSb32KilopassOTP_Timing_CPH_CPH                     0
    #define   LSb16KilopassOTP_Timing_CPH_CPH                     0
    #define       bKilopassOTP_Timing_CPH_CPH                  8
    #define   MSK32KilopassOTP_Timing_CPH_CPH                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_PEH                      0x0038

    #define     BA_KilopassOTP_Timing_PEH_PEH                  0x0038
    #define     B16KilopassOTP_Timing_PEH_PEH                  0x0038
    #define   LSb32KilopassOTP_Timing_PEH_PEH                     0
    #define   LSb16KilopassOTP_Timing_PEH_PEH                     0
    #define       bKilopassOTP_Timing_PEH_PEH                  8
    #define   MSK32KilopassOTP_Timing_PEH_PEH                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_PGMAS                    0x003C

    #define     BA_KilopassOTP_Timing_PGMAS_PGMAS              0x003C
    #define     B16KilopassOTP_Timing_PGMAS_PGMAS              0x003C
    #define   LSb32KilopassOTP_Timing_PGMAS_PGMAS                 0
    #define   LSb16KilopassOTP_Timing_PGMAS_PGMAS                 0
    #define       bKilopassOTP_Timing_PGMAS_PGMAS              8
    #define   MSK32KilopassOTP_Timing_PGMAS_PGMAS                 0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_PGMRD                    0x0040

    #define     BA_KilopassOTP_Timing_PGMRD_PGMRD              0x0040
    #define     B16KilopassOTP_Timing_PGMRD_PGMRD              0x0040
    #define   LSb32KilopassOTP_Timing_PGMRD_PGMRD                 0
    #define   LSb16KilopassOTP_Timing_PGMRD_PGMRD                 0
    #define       bKilopassOTP_Timing_PGMRD_PGMRD              8
    #define   MSK32KilopassOTP_Timing_PGMRD_PGMRD                 0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_RSRD                     0x0044

    #define     BA_KilopassOTP_Timing_RSRD_RSRD                0x0044
    #define     B16KilopassOTP_Timing_RSRD_RSRD                0x0044
    #define   LSb32KilopassOTP_Timing_RSRD_RSRD                   0
    #define   LSb16KilopassOTP_Timing_RSRD_RSRD                   0
    #define       bKilopassOTP_Timing_RSRD_RSRD                8
    #define   MSK32KilopassOTP_Timing_RSRD_RSRD                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_READEN                   0x0048

    #define     BA_KilopassOTP_Timing_READEN_READEN            0x0048
    #define     B16KilopassOTP_Timing_READEN_READEN            0x0048
    #define   LSb32KilopassOTP_Timing_READEN_READEN               0
    #define   LSb16KilopassOTP_Timing_READEN_READEN               0
    #define       bKilopassOTP_Timing_READEN_READEN            8
    #define   MSK32KilopassOTP_Timing_READEN_READEN               0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_CEW                      0x004C

    #define     BA_KilopassOTP_Timing_CEW_CEW                  0x004C
    #define     B16KilopassOTP_Timing_CEW_CEW                  0x004C
    #define   LSb32KilopassOTP_Timing_CEW_CEW                     0
    #define   LSb16KilopassOTP_Timing_CEW_CEW                     0
    #define       bKilopassOTP_Timing_CEW_CEW                  8
    #define   MSK32KilopassOTP_Timing_CEW_CEW                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_CERD                     0x0050

    #define     BA_KilopassOTP_Timing_CERD_CERD                0x0050
    #define     B16KilopassOTP_Timing_CERD_CERD                0x0050
    #define   LSb32KilopassOTP_Timing_CERD_CERD                   0
    #define   LSb16KilopassOTP_Timing_CERD_CERD                   0
    #define       bKilopassOTP_Timing_CERD_CERD                8
    #define   MSK32KilopassOTP_Timing_CERD_CERD                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_NP2                      0x0054

    #define     BA_KilopassOTP_Timing_NP2_NP2                  0x0054
    #define     B16KilopassOTP_Timing_NP2_NP2                  0x0054
    #define   LSb32KilopassOTP_Timing_NP2_NP2                     0
    #define   LSb16KilopassOTP_Timing_NP2_NP2                     0
    #define       bKilopassOTP_Timing_NP2_NP2                  8
    #define   MSK32KilopassOTP_Timing_NP2_NP2                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_RSCLE                    0x0058

    #define     BA_KilopassOTP_Timing_RSCLE_RSCLE              0x0058
    #define     B16KilopassOTP_Timing_RSCLE_RSCLE              0x0058
    #define   LSb32KilopassOTP_Timing_RSCLE_RSCLE                 0
    #define   LSb16KilopassOTP_Timing_RSCLE_RSCLE                 0
    #define       bKilopassOTP_Timing_RSCLE_RSCLE              8
    #define   MSK32KilopassOTP_Timing_RSCLE_RSCLE                 0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_TWS                      0x005C

    #define     BA_KilopassOTP_Timing_TWS_TWS                  0x005C
    #define     B16KilopassOTP_Timing_TWS_TWS                  0x005C
    #define   LSb32KilopassOTP_Timing_TWS_TWS                     0
    #define   LSb16KilopassOTP_Timing_TWS_TWS                     0
    #define       bKilopassOTP_Timing_TWS_TWS                  8
    #define   MSK32KilopassOTP_Timing_TWS_TWS                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_CLES                     0x0060

    #define     BA_KilopassOTP_Timing_CLES_CLES                0x0060
    #define     B16KilopassOTP_Timing_CLES_CLES                0x0060
    #define   LSb32KilopassOTP_Timing_CLES_CLES                   0
    #define   LSb16KilopassOTP_Timing_CLES_CLES                   0
    #define       bKilopassOTP_Timing_CLES_CLES                8
    #define   MSK32KilopassOTP_Timing_CLES_CLES                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_CLEH                     0x0064

    #define     BA_KilopassOTP_Timing_CLEH_CLEH                0x0064
    #define     B16KilopassOTP_Timing_CLEH_CLEH                0x0064
    #define   LSb32KilopassOTP_Timing_CLEH_CLEH                   0
    #define   LSb16KilopassOTP_Timing_CLEH_CLEH                   0
    #define       bKilopassOTP_Timing_CLEH_CLEH                8
    #define   MSK32KilopassOTP_Timing_CLEH_CLEH                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_TWH                      0x0068

    #define     BA_KilopassOTP_Timing_TWH_TWH                  0x0068
    #define     B16KilopassOTP_Timing_TWH_TWH                  0x0068
    #define   LSb32KilopassOTP_Timing_TWH_TWH                     0
    #define   LSb16KilopassOTP_Timing_TWH_TWH                     0
    #define       bKilopassOTP_Timing_TWH_TWH                  8
    #define   MSK32KilopassOTP_Timing_TWH_TWH                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_WDRD                     0x006C

    #define     BA_KilopassOTP_Timing_WDRD_WDRD                0x006C
    #define     B16KilopassOTP_Timing_WDRD_WDRD                0x006C
    #define   LSb32KilopassOTP_Timing_WDRD_WDRD                   0
    #define   LSb16KilopassOTP_Timing_WDRD_WDRD                   0
    #define       bKilopassOTP_Timing_WDRD_WDRD                8
    #define   MSK32KilopassOTP_Timing_WDRD_WDRD                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_BCREADEN                 0x0070

    #define     BA_KilopassOTP_Timing_BCREADEN_BCREADEN        0x0070
    #define     B16KilopassOTP_Timing_BCREADEN_BCREADEN        0x0070
    #define   LSb32KilopassOTP_Timing_BCREADEN_BCREADEN           0
    #define   LSb16KilopassOTP_Timing_BCREADEN_BCREADEN           0
    #define       bKilopassOTP_Timing_BCREADEN_BCREADEN        8
    #define   MSK32KilopassOTP_Timing_BCREADEN_BCREADEN           0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_BCACC                    0x0074

    #define     BA_KilopassOTP_Timing_BCACC_BCACC              0x0074
    #define     B16KilopassOTP_Timing_BCACC_BCACC              0x0074
    #define   LSb32KilopassOTP_Timing_BCACC_BCACC                 0
    #define   LSb16KilopassOTP_Timing_BCACC_BCACC                 0
    #define       bKilopassOTP_Timing_BCACC_BCACC              8
    #define   MSK32KilopassOTP_Timing_BCACC_BCACC                 0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_AAC                      0x0078

    #define     BA_KilopassOTP_Timing_AAC_AAC                  0x0078
    #define     B16KilopassOTP_Timing_AAC_AAC                  0x0078
    #define   LSb32KilopassOTP_Timing_AAC_AAC                     0
    #define   LSb16KilopassOTP_Timing_AAC_AAC                     0
    #define       bKilopassOTP_Timing_AAC_AAC                  8
    #define   MSK32KilopassOTP_Timing_AAC_AAC                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_RRC                      0x007C

    #define     BA_KilopassOTP_Timing_RRC_RRC                  0x007C
    #define     B16KilopassOTP_Timing_RRC_RRC                  0x007C
    #define   LSb32KilopassOTP_Timing_RRC_RRC                     0
    #define   LSb16KilopassOTP_Timing_RRC_RRC                     0
    #define       bKilopassOTP_Timing_RRC_RRC                  8
    #define   MSK32KilopassOTP_Timing_RRC_RRC                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_RDES                     0x0080

    #define     BA_KilopassOTP_Timing_RDES_RDES                0x0080
    #define     B16KilopassOTP_Timing_RDES_RDES                0x0080
    #define   LSb32KilopassOTP_Timing_RDES_RDES                   0
    #define   LSb16KilopassOTP_Timing_RDES_RDES                   0
    #define       bKilopassOTP_Timing_RDES_RDES                8
    #define   MSK32KilopassOTP_Timing_RDES_RDES                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_RWH                      0x0084

    #define     BA_KilopassOTP_Timing_RWH_RWH                  0x0084
    #define     B16KilopassOTP_Timing_RWH_RWH                  0x0084
    #define   LSb32KilopassOTP_Timing_RWH_RWH                     0
    #define   LSb16KilopassOTP_Timing_RWH_RWH                     0
    #define       bKilopassOTP_Timing_RWH_RWH                  8
    #define   MSK32KilopassOTP_Timing_RWH_RWH                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_READENT                  0x0088

    #define     BA_KilopassOTP_Timing_READENT_READENT          0x0088
    #define     B16KilopassOTP_Timing_READENT_READENT          0x0088
    #define   LSb32KilopassOTP_Timing_READENT_READENT             0
    #define   LSb16KilopassOTP_Timing_READENT_READENT             0
    #define       bKilopassOTP_Timing_READENT_READENT          8
    #define   MSK32KilopassOTP_Timing_READENT_READENT             0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_AACT                     0x008C

    #define     BA_KilopassOTP_Timing_AACT_AACT                0x008C
    #define     B16KilopassOTP_Timing_AACT_AACT                0x008C
    #define   LSb32KilopassOTP_Timing_AACT_AACT                   0
    #define   LSb16KilopassOTP_Timing_AACT_AACT                   0
    #define       bKilopassOTP_Timing_AACT_AACT                8
    #define   MSK32KilopassOTP_Timing_AACT_AACT                   0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_CDD                      0x0090

    #define     BA_KilopassOTP_Timing_CDD_CDD                  0x0090
    #define     B16KilopassOTP_Timing_CDD_CDD                  0x0090
    #define   LSb32KilopassOTP_Timing_CDD_CDD                     0
    #define   LSb16KilopassOTP_Timing_CDD_CDD                     0
    #define       bKilopassOTP_Timing_CDD_CDD                  8
    #define   MSK32KilopassOTP_Timing_CDD_CDD                     0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_PGMSD                    0x0094

    #define     BA_KilopassOTP_Timing_PGMSD_PGMSD              0x0094
    #define     B16KilopassOTP_Timing_PGMSD_PGMSD              0x0094
    #define   LSb32KilopassOTP_Timing_PGMSD_PGMSD                 0
    #define   LSb16KilopassOTP_Timing_PGMSD_PGMSD                 0
    #define       bKilopassOTP_Timing_PGMSD_PGMSD              8
    #define   MSK32KilopassOTP_Timing_PGMSD_PGMSD                 0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_PGMDD                    0x0098

    #define     BA_KilopassOTP_Timing_PGMDD_PGMDD              0x0098
    #define     B16KilopassOTP_Timing_PGMDD_PGMDD              0x0098
    #define   LSb32KilopassOTP_Timing_PGMDD_PGMDD                 0
    #define   LSb16KilopassOTP_Timing_PGMDD_PGMDD                 0
    #define       bKilopassOTP_Timing_PGMDD_PGMDD              8
    #define   MSK32KilopassOTP_Timing_PGMDD_PGMDD                 0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassOTP_Timing_ACC                      0x009C

    #define     BA_KilopassOTP_Timing_ACC_ACC                  0x009C
    #define     B16KilopassOTP_Timing_ACC_ACC                  0x009C
    #define   LSb32KilopassOTP_Timing_ACC_ACC                     0
    #define   LSb16KilopassOTP_Timing_ACC_ACC                     0
    #define       bKilopassOTP_Timing_ACC_ACC                  8
    #define   MSK32KilopassOTP_Timing_ACC_ACC                     0x000000FF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_KilopassOTP_Timing {
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_CSRST_CSRST(r32)         _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_CSRST_CSRST(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_CSRST_CSRST(r16)         _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_CSRST_CSRST(r16,v)       _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_CSRST                    {\
            UNSG32 uCSRST_CSRST                                :  8;\
            UNSG32 RSVDx0_b8                                   : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_CSRST;
            struct w32KilopassOTP_Timing_CSRST;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_RW_RW(r32)               _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_RW_RW(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_RW_RW(r16)               _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_RW_RW(r16,v)             _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_RW                       {\
            UNSG32 uRW_RW                                      :  8;\
            UNSG32 RSVDx4_b8                                   : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_RW;
            struct w32KilopassOTP_Timing_RW;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_RS_RS(r32)               _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_RS_RS(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_RS_RS(r16)               _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_RS_RS(r16,v)             _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_RS                       {\
            UNSG32 uRS_RS                                      :  8;\
            UNSG32 RSVDx8_b8                                   : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_RS;
            struct w32KilopassOTP_Timing_RS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_AS_AS(r32)               _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_AS_AS(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_AS_AS(r16)               _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_AS_AS(r16,v)             _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_AS                       {\
            UNSG32 uAS_AS                                      :  8;\
            UNSG32 RSVDxC_b8                                   : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_AS;
            struct w32KilopassOTP_Timing_AS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_DS_DS(r32)               _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_DS_DS(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_DS_DS(r16)               _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_DS_DS(r16,v)             _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_DS                       {\
            UNSG32 uDS_DS                                      :  8;\
            UNSG32 RSVDx10_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_DS;
            struct w32KilopassOTP_Timing_DS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_DLES_DLES(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_DLES_DLES(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_DLES_DLES(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_DLES_DLES(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_DLES                     {\
            UNSG32 uDLES_DLES                                  :  8;\
            UNSG32 RSVDx14_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_DLES;
            struct w32KilopassOTP_Timing_DLES;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_WWL_WWL(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_WWL_WWL(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_WWL_WWL(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_WWL_WWL(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_WWL                      {\
            UNSG32 uWWL_WWL                                    :  8;\
            UNSG32 RSVDx18_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_WWL;
            struct w32KilopassOTP_Timing_WWL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_DLEH_DLEH(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_DLEH_DLEH(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_DLEH_DLEH(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_DLEH_DLEH(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_DLEH                     {\
            UNSG32 uDLEH_DLEH                                  :  8;\
            UNSG32 RSVDx1C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_DLEH;
            struct w32KilopassOTP_Timing_DLEH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_DH_DH(r32)               _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_DH_DH(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_DH_DH(r16)               _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_DH_DH(r16,v)             _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_DH                       {\
            UNSG32 uDH_DH                                      :  8;\
            UNSG32 RSVDx20_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_DH;
            struct w32KilopassOTP_Timing_DH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_DPD_DPD(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_DPD_DPD(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_DPD_DPD(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_DPD_DPD(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_DPD                      {\
            UNSG32 uDPD_DPD                                    :  8;\
            UNSG32 RSVDx24_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_DPD;
            struct w32KilopassOTP_Timing_DPD;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_PES_PES(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_PES_PES(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_PES_PES(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_PES_PES(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_PES                      {\
            UNSG32 uPES_PES                                    :  8;\
            UNSG32 RSVDx28_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_PES;
            struct w32KilopassOTP_Timing_PES;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_CPS_CPS(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_CPS_CPS(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_CPS_CPS(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_CPS_CPS(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_CPS                      {\
            UNSG32 uCPS_CPS                                    :  8;\
            UNSG32 RSVDx2C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_CPS;
            struct w32KilopassOTP_Timing_CPS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_PW_PW(r32)               _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_PW_PW(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_PW_PW(r16)               _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_PW_PW(r16,v)             _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_PW                       {\
            UNSG32 uPW_PW                                      :  8;\
            UNSG32 RSVDx30_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_PW;
            struct w32KilopassOTP_Timing_PW;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_CPH_CPH(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_CPH_CPH(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_CPH_CPH(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_CPH_CPH(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_CPH                      {\
            UNSG32 uCPH_CPH                                    :  8;\
            UNSG32 RSVDx34_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_CPH;
            struct w32KilopassOTP_Timing_CPH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_PEH_PEH(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_PEH_PEH(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_PEH_PEH(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_PEH_PEH(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_PEH                      {\
            UNSG32 uPEH_PEH                                    :  8;\
            UNSG32 RSVDx38_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_PEH;
            struct w32KilopassOTP_Timing_PEH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_PGMAS_PGMAS(r32)         _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_PGMAS_PGMAS(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_PGMAS_PGMAS(r16)         _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_PGMAS_PGMAS(r16,v)       _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_PGMAS                    {\
            UNSG32 uPGMAS_PGMAS                                :  8;\
            UNSG32 RSVDx3C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_PGMAS;
            struct w32KilopassOTP_Timing_PGMAS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_PGMRD_PGMRD(r32)         _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_PGMRD_PGMRD(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_PGMRD_PGMRD(r16)         _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_PGMRD_PGMRD(r16,v)       _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_PGMRD                    {\
            UNSG32 uPGMRD_PGMRD                                :  8;\
            UNSG32 RSVDx40_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_PGMRD;
            struct w32KilopassOTP_Timing_PGMRD;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_RSRD_RSRD(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_RSRD_RSRD(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_RSRD_RSRD(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_RSRD_RSRD(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_RSRD                     {\
            UNSG32 uRSRD_RSRD                                  :  8;\
            UNSG32 RSVDx44_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_RSRD;
            struct w32KilopassOTP_Timing_RSRD;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_READEN_READEN(r32)       _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_READEN_READEN(r32,v)     _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_READEN_READEN(r16)       _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_READEN_READEN(r16,v)     _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_READEN                   {\
            UNSG32 uREADEN_READEN                              :  8;\
            UNSG32 RSVDx48_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_READEN;
            struct w32KilopassOTP_Timing_READEN;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_CEW_CEW(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_CEW_CEW(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_CEW_CEW(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_CEW_CEW(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_CEW                      {\
            UNSG32 uCEW_CEW                                    :  8;\
            UNSG32 RSVDx4C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_CEW;
            struct w32KilopassOTP_Timing_CEW;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_CERD_CERD(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_CERD_CERD(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_CERD_CERD(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_CERD_CERD(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_CERD                     {\
            UNSG32 uCERD_CERD                                  :  8;\
            UNSG32 RSVDx50_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_CERD;
            struct w32KilopassOTP_Timing_CERD;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_NP2_NP2(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_NP2_NP2(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_NP2_NP2(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_NP2_NP2(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_NP2                      {\
            UNSG32 uNP2_NP2                                    :  8;\
            UNSG32 RSVDx54_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_NP2;
            struct w32KilopassOTP_Timing_NP2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_RSCLE_RSCLE(r32)         _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_RSCLE_RSCLE(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_RSCLE_RSCLE(r16)         _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_RSCLE_RSCLE(r16,v)       _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_RSCLE                    {\
            UNSG32 uRSCLE_RSCLE                                :  8;\
            UNSG32 RSVDx58_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_RSCLE;
            struct w32KilopassOTP_Timing_RSCLE;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_TWS_TWS(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_TWS_TWS(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_TWS_TWS(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_TWS_TWS(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_TWS                      {\
            UNSG32 uTWS_TWS                                    :  8;\
            UNSG32 RSVDx5C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_TWS;
            struct w32KilopassOTP_Timing_TWS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_CLES_CLES(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_CLES_CLES(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_CLES_CLES(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_CLES_CLES(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_CLES                     {\
            UNSG32 uCLES_CLES                                  :  8;\
            UNSG32 RSVDx60_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_CLES;
            struct w32KilopassOTP_Timing_CLES;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_CLEH_CLEH(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_CLEH_CLEH(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_CLEH_CLEH(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_CLEH_CLEH(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_CLEH                     {\
            UNSG32 uCLEH_CLEH                                  :  8;\
            UNSG32 RSVDx64_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_CLEH;
            struct w32KilopassOTP_Timing_CLEH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_TWH_TWH(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_TWH_TWH(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_TWH_TWH(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_TWH_TWH(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_TWH                      {\
            UNSG32 uTWH_TWH                                    :  8;\
            UNSG32 RSVDx68_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_TWH;
            struct w32KilopassOTP_Timing_TWH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_WDRD_WDRD(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_WDRD_WDRD(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_WDRD_WDRD(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_WDRD_WDRD(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_WDRD                     {\
            UNSG32 uWDRD_WDRD                                  :  8;\
            UNSG32 RSVDx6C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_WDRD;
            struct w32KilopassOTP_Timing_WDRD;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_BCREADEN_BCREADEN(r32)   _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_BCREADEN_BCREADEN(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_BCREADEN_BCREADEN(r16)   _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_BCREADEN_BCREADEN(r16,v) _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_BCREADEN                 {\
            UNSG32 uBCREADEN_BCREADEN                          :  8;\
            UNSG32 RSVDx70_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_BCREADEN;
            struct w32KilopassOTP_Timing_BCREADEN;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_BCACC_BCACC(r32)         _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_BCACC_BCACC(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_BCACC_BCACC(r16)         _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_BCACC_BCACC(r16,v)       _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_BCACC                    {\
            UNSG32 uBCACC_BCACC                                :  8;\
            UNSG32 RSVDx74_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_BCACC;
            struct w32KilopassOTP_Timing_BCACC;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_AAC_AAC(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_AAC_AAC(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_AAC_AAC(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_AAC_AAC(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_AAC                      {\
            UNSG32 uAAC_AAC                                    :  8;\
            UNSG32 RSVDx78_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_AAC;
            struct w32KilopassOTP_Timing_AAC;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_RRC_RRC(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_RRC_RRC(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_RRC_RRC(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_RRC_RRC(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_RRC                      {\
            UNSG32 uRRC_RRC                                    :  8;\
            UNSG32 RSVDx7C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_RRC;
            struct w32KilopassOTP_Timing_RRC;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_RDES_RDES(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_RDES_RDES(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_RDES_RDES(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_RDES_RDES(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_RDES                     {\
            UNSG32 uRDES_RDES                                  :  8;\
            UNSG32 RSVDx80_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_RDES;
            struct w32KilopassOTP_Timing_RDES;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_RWH_RWH(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_RWH_RWH(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_RWH_RWH(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_RWH_RWH(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_RWH                      {\
            UNSG32 uRWH_RWH                                    :  8;\
            UNSG32 RSVDx84_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_RWH;
            struct w32KilopassOTP_Timing_RWH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_READENT_READENT(r32)     _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_READENT_READENT(r32,v)   _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_READENT_READENT(r16)     _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_READENT_READENT(r16,v)   _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_READENT                  {\
            UNSG32 uREADENT_READENT                            :  8;\
            UNSG32 RSVDx88_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_READENT;
            struct w32KilopassOTP_Timing_READENT;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_AACT_AACT(r32)           _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_AACT_AACT(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_AACT_AACT(r16)           _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_AACT_AACT(r16,v)         _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_AACT                     {\
            UNSG32 uAACT_AACT                                  :  8;\
            UNSG32 RSVDx8C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_AACT;
            struct w32KilopassOTP_Timing_AACT;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_CDD_CDD(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_CDD_CDD(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_CDD_CDD(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_CDD_CDD(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_CDD                      {\
            UNSG32 uCDD_CDD                                    :  8;\
            UNSG32 RSVDx90_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_CDD;
            struct w32KilopassOTP_Timing_CDD;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_PGMSD_PGMSD(r32)         _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_PGMSD_PGMSD(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_PGMSD_PGMSD(r16)         _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_PGMSD_PGMSD(r16,v)       _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_PGMSD                    {\
            UNSG32 uPGMSD_PGMSD                                :  8;\
            UNSG32 RSVDx94_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_PGMSD;
            struct w32KilopassOTP_Timing_PGMSD;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_PGMDD_PGMDD(r32)         _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_PGMDD_PGMDD(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_PGMDD_PGMDD(r16)         _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_PGMDD_PGMDD(r16,v)       _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_PGMDD                    {\
            UNSG32 uPGMDD_PGMDD                                :  8;\
            UNSG32 RSVDx98_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_PGMDD;
            struct w32KilopassOTP_Timing_PGMDD;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassOTP_Timing_ACC_ACC(r32)             _BFGET_(r32, 7, 0)
    #define   SET32KilopassOTP_Timing_ACC_ACC(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16KilopassOTP_Timing_ACC_ACC(r16)             _BFGET_(r16, 7, 0)
    #define   SET16KilopassOTP_Timing_ACC_ACC(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32KilopassOTP_Timing_ACC                      {\
            UNSG32 uACC_ACC                                    :  8;\
            UNSG32 RSVDx9C_b8                                  : 24;\
          }
    union { UNSG32 u32KilopassOTP_Timing_ACC;
            struct w32KilopassOTP_Timing_ACC;
          };
    ///////////////////////////////////////////////////////////
    } SIE_KilopassOTP_Timing;

    typedef union  T32KilopassOTP_Timing_CSRST
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_CSRST;
                 } T32KilopassOTP_Timing_CSRST;
    typedef union  T32KilopassOTP_Timing_RW
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_RW;
                 } T32KilopassOTP_Timing_RW;
    typedef union  T32KilopassOTP_Timing_RS
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_RS;
                 } T32KilopassOTP_Timing_RS;
    typedef union  T32KilopassOTP_Timing_AS
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_AS;
                 } T32KilopassOTP_Timing_AS;
    typedef union  T32KilopassOTP_Timing_DS
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_DS;
                 } T32KilopassOTP_Timing_DS;
    typedef union  T32KilopassOTP_Timing_DLES
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_DLES;
                 } T32KilopassOTP_Timing_DLES;
    typedef union  T32KilopassOTP_Timing_WWL
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_WWL;
                 } T32KilopassOTP_Timing_WWL;
    typedef union  T32KilopassOTP_Timing_DLEH
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_DLEH;
                 } T32KilopassOTP_Timing_DLEH;
    typedef union  T32KilopassOTP_Timing_DH
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_DH;
                 } T32KilopassOTP_Timing_DH;
    typedef union  T32KilopassOTP_Timing_DPD
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_DPD;
                 } T32KilopassOTP_Timing_DPD;
    typedef union  T32KilopassOTP_Timing_PES
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_PES;
                 } T32KilopassOTP_Timing_PES;
    typedef union  T32KilopassOTP_Timing_CPS
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_CPS;
                 } T32KilopassOTP_Timing_CPS;
    typedef union  T32KilopassOTP_Timing_PW
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_PW;
                 } T32KilopassOTP_Timing_PW;
    typedef union  T32KilopassOTP_Timing_CPH
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_CPH;
                 } T32KilopassOTP_Timing_CPH;
    typedef union  T32KilopassOTP_Timing_PEH
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_PEH;
                 } T32KilopassOTP_Timing_PEH;
    typedef union  T32KilopassOTP_Timing_PGMAS
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_PGMAS;
                 } T32KilopassOTP_Timing_PGMAS;
    typedef union  T32KilopassOTP_Timing_PGMRD
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_PGMRD;
                 } T32KilopassOTP_Timing_PGMRD;
    typedef union  T32KilopassOTP_Timing_RSRD
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_RSRD;
                 } T32KilopassOTP_Timing_RSRD;
    typedef union  T32KilopassOTP_Timing_READEN
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_READEN;
                 } T32KilopassOTP_Timing_READEN;
    typedef union  T32KilopassOTP_Timing_CEW
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_CEW;
                 } T32KilopassOTP_Timing_CEW;
    typedef union  T32KilopassOTP_Timing_CERD
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_CERD;
                 } T32KilopassOTP_Timing_CERD;
    typedef union  T32KilopassOTP_Timing_NP2
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_NP2;
                 } T32KilopassOTP_Timing_NP2;
    typedef union  T32KilopassOTP_Timing_RSCLE
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_RSCLE;
                 } T32KilopassOTP_Timing_RSCLE;
    typedef union  T32KilopassOTP_Timing_TWS
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_TWS;
                 } T32KilopassOTP_Timing_TWS;
    typedef union  T32KilopassOTP_Timing_CLES
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_CLES;
                 } T32KilopassOTP_Timing_CLES;
    typedef union  T32KilopassOTP_Timing_CLEH
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_CLEH;
                 } T32KilopassOTP_Timing_CLEH;
    typedef union  T32KilopassOTP_Timing_TWH
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_TWH;
                 } T32KilopassOTP_Timing_TWH;
    typedef union  T32KilopassOTP_Timing_WDRD
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_WDRD;
                 } T32KilopassOTP_Timing_WDRD;
    typedef union  T32KilopassOTP_Timing_BCREADEN
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_BCREADEN;
                 } T32KilopassOTP_Timing_BCREADEN;
    typedef union  T32KilopassOTP_Timing_BCACC
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_BCACC;
                 } T32KilopassOTP_Timing_BCACC;
    typedef union  T32KilopassOTP_Timing_AAC
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_AAC;
                 } T32KilopassOTP_Timing_AAC;
    typedef union  T32KilopassOTP_Timing_RRC
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_RRC;
                 } T32KilopassOTP_Timing_RRC;
    typedef union  T32KilopassOTP_Timing_RDES
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_RDES;
                 } T32KilopassOTP_Timing_RDES;
    typedef union  T32KilopassOTP_Timing_RWH
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_RWH;
                 } T32KilopassOTP_Timing_RWH;
    typedef union  T32KilopassOTP_Timing_READENT
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_READENT;
                 } T32KilopassOTP_Timing_READENT;
    typedef union  T32KilopassOTP_Timing_AACT
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_AACT;
                 } T32KilopassOTP_Timing_AACT;
    typedef union  T32KilopassOTP_Timing_CDD
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_CDD;
                 } T32KilopassOTP_Timing_CDD;
    typedef union  T32KilopassOTP_Timing_PGMSD
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_PGMSD;
                 } T32KilopassOTP_Timing_PGMSD;
    typedef union  T32KilopassOTP_Timing_PGMDD
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_PGMDD;
                 } T32KilopassOTP_Timing_PGMDD;
    typedef union  T32KilopassOTP_Timing_ACC
          { UNSG32 u32;
            struct w32KilopassOTP_Timing_ACC;
                 } T32KilopassOTP_Timing_ACC;
    ///////////////////////////////////////////////////////////

    typedef union  TKilopassOTP_Timing_CSRST
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_CSRST;
                   };
                 } TKilopassOTP_Timing_CSRST;
    typedef union  TKilopassOTP_Timing_RW
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_RW;
                   };
                 } TKilopassOTP_Timing_RW;
    typedef union  TKilopassOTP_Timing_RS
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_RS;
                   };
                 } TKilopassOTP_Timing_RS;
    typedef union  TKilopassOTP_Timing_AS
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_AS;
                   };
                 } TKilopassOTP_Timing_AS;
    typedef union  TKilopassOTP_Timing_DS
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_DS;
                   };
                 } TKilopassOTP_Timing_DS;
    typedef union  TKilopassOTP_Timing_DLES
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_DLES;
                   };
                 } TKilopassOTP_Timing_DLES;
    typedef union  TKilopassOTP_Timing_WWL
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_WWL;
                   };
                 } TKilopassOTP_Timing_WWL;
    typedef union  TKilopassOTP_Timing_DLEH
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_DLEH;
                   };
                 } TKilopassOTP_Timing_DLEH;
    typedef union  TKilopassOTP_Timing_DH
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_DH;
                   };
                 } TKilopassOTP_Timing_DH;
    typedef union  TKilopassOTP_Timing_DPD
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_DPD;
                   };
                 } TKilopassOTP_Timing_DPD;
    typedef union  TKilopassOTP_Timing_PES
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_PES;
                   };
                 } TKilopassOTP_Timing_PES;
    typedef union  TKilopassOTP_Timing_CPS
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_CPS;
                   };
                 } TKilopassOTP_Timing_CPS;
    typedef union  TKilopassOTP_Timing_PW
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_PW;
                   };
                 } TKilopassOTP_Timing_PW;
    typedef union  TKilopassOTP_Timing_CPH
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_CPH;
                   };
                 } TKilopassOTP_Timing_CPH;
    typedef union  TKilopassOTP_Timing_PEH
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_PEH;
                   };
                 } TKilopassOTP_Timing_PEH;
    typedef union  TKilopassOTP_Timing_PGMAS
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_PGMAS;
                   };
                 } TKilopassOTP_Timing_PGMAS;
    typedef union  TKilopassOTP_Timing_PGMRD
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_PGMRD;
                   };
                 } TKilopassOTP_Timing_PGMRD;
    typedef union  TKilopassOTP_Timing_RSRD
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_RSRD;
                   };
                 } TKilopassOTP_Timing_RSRD;
    typedef union  TKilopassOTP_Timing_READEN
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_READEN;
                   };
                 } TKilopassOTP_Timing_READEN;
    typedef union  TKilopassOTP_Timing_CEW
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_CEW;
                   };
                 } TKilopassOTP_Timing_CEW;
    typedef union  TKilopassOTP_Timing_CERD
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_CERD;
                   };
                 } TKilopassOTP_Timing_CERD;
    typedef union  TKilopassOTP_Timing_NP2
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_NP2;
                   };
                 } TKilopassOTP_Timing_NP2;
    typedef union  TKilopassOTP_Timing_RSCLE
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_RSCLE;
                   };
                 } TKilopassOTP_Timing_RSCLE;
    typedef union  TKilopassOTP_Timing_TWS
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_TWS;
                   };
                 } TKilopassOTP_Timing_TWS;
    typedef union  TKilopassOTP_Timing_CLES
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_CLES;
                   };
                 } TKilopassOTP_Timing_CLES;
    typedef union  TKilopassOTP_Timing_CLEH
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_CLEH;
                   };
                 } TKilopassOTP_Timing_CLEH;
    typedef union  TKilopassOTP_Timing_TWH
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_TWH;
                   };
                 } TKilopassOTP_Timing_TWH;
    typedef union  TKilopassOTP_Timing_WDRD
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_WDRD;
                   };
                 } TKilopassOTP_Timing_WDRD;
    typedef union  TKilopassOTP_Timing_BCREADEN
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_BCREADEN;
                   };
                 } TKilopassOTP_Timing_BCREADEN;
    typedef union  TKilopassOTP_Timing_BCACC
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_BCACC;
                   };
                 } TKilopassOTP_Timing_BCACC;
    typedef union  TKilopassOTP_Timing_AAC
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_AAC;
                   };
                 } TKilopassOTP_Timing_AAC;
    typedef union  TKilopassOTP_Timing_RRC
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_RRC;
                   };
                 } TKilopassOTP_Timing_RRC;
    typedef union  TKilopassOTP_Timing_RDES
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_RDES;
                   };
                 } TKilopassOTP_Timing_RDES;
    typedef union  TKilopassOTP_Timing_RWH
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_RWH;
                   };
                 } TKilopassOTP_Timing_RWH;
    typedef union  TKilopassOTP_Timing_READENT
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_READENT;
                   };
                 } TKilopassOTP_Timing_READENT;
    typedef union  TKilopassOTP_Timing_AACT
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_AACT;
                   };
                 } TKilopassOTP_Timing_AACT;
    typedef union  TKilopassOTP_Timing_CDD
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_CDD;
                   };
                 } TKilopassOTP_Timing_CDD;
    typedef union  TKilopassOTP_Timing_PGMSD
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_PGMSD;
                   };
                 } TKilopassOTP_Timing_PGMSD;
    typedef union  TKilopassOTP_Timing_PGMDD
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_PGMDD;
                   };
                 } TKilopassOTP_Timing_PGMDD;
    typedef union  TKilopassOTP_Timing_ACC
          { UNSG32 u32[1];
            struct {
            struct w32KilopassOTP_Timing_ACC;
                   };
                 } TKilopassOTP_Timing_ACC;

    ///////////////////////////////////////////////////////////
     SIGN32 KilopassOTP_Timing_drvrd(SIE_KilopassOTP_Timing *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 KilopassOTP_Timing_drvwr(SIE_KilopassOTP_Timing *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void KilopassOTP_Timing_reset(SIE_KilopassOTP_Timing *p);
     SIGN32 KilopassOTP_Timing_cmp  (SIE_KilopassOTP_Timing *p, SIE_KilopassOTP_Timing *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define KilopassOTP_Timing_check(p,pie,pfx,hLOG) KilopassOTP_Timing_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define KilopassOTP_Timing_print(p,    pfx,hLOG) KilopassOTP_Timing_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: KilopassOTP_Timing
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE NAGRA_CERT_OLD                           (4,4)
///     ###
///     * This region is only accessible to AKL except slot_0_lock. It’s writeable through AHB bus.
///     * Entire register is open for R/W at eng_en chip
///     ###
///     # # ----------------------------------------------------------
///     @ 0x00000 Slot_0               (RW-)
///               %unsigned 1  Slot_0                    0x0
///                                    ###
///                                    * Slot 0 is 384-bit
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00004                      (W-)
///     #         # Stuffing bytes...
///               %% 352
///     @ 0x00030 Slot_1               (RW-)
///               %unsigned 1  Slot_1                    0x0
///                                    ###
///                                    * Slot 1 is 384-bit
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00034                      (W-)
///     #         # Stuffing bytes...
///               %% 352
///     @ 0x00060 Slot_2               (RW-)
///               %unsigned 1  Slot_2                    0x0
///                                    ###
///                                    * Slot 2 is 128-bit
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00064                      (W-)
///     #         # Stuffing bytes...
///               %% 96
///     @ 0x00070 Slot_3               (RW-)
///               %unsigned 1  Slot_3                    0x0
///                                    ###
///                                    * Slot 3 is 128-bit
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00074                      (W-)
///     #         # Stuffing bytes...
///               %% 96
///     @ 0x00080 Slot_lock            (RW-)
///               %unsigned 2  Slot_0_lock               0x0
///                                    ###
///                                    * Slot_0_lock
///                                    ###
///               %unsigned 2  Slot_1_lock               0x0
///                                    ###
///                                    * Slot_1_lock
///                                    ###
///               %unsigned 2  Slot_2_lock               0x0
///                                    ###
///                                    * Slot_2_lock
///                                    ###
///               %unsigned 2  Slot_3_lock               0x0
///                                    ###
///                                    * Slot_3_lock
///                                    ###
///               %%        24         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     132B, bits:      12b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_NAGRA_CERT_OLD
#define h_NAGRA_CERT_OLD (){}

    #define     RA_NAGRA_CERT_OLD_Slot_0                       0x0000

    #define     BA_NAGRA_CERT_OLD_Slot_0_Slot_0                0x0000
    #define     B16NAGRA_CERT_OLD_Slot_0_Slot_0                0x0000
    #define   LSb32NAGRA_CERT_OLD_Slot_0_Slot_0                   0
    #define   LSb16NAGRA_CERT_OLD_Slot_0_Slot_0                   0
    #define       bNAGRA_CERT_OLD_Slot_0_Slot_0                1
    #define   MSK32NAGRA_CERT_OLD_Slot_0_Slot_0                   0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_NAGRA_CERT_OLD_Slot_1                       0x0030

    #define     BA_NAGRA_CERT_OLD_Slot_1_Slot_1                0x0030
    #define     B16NAGRA_CERT_OLD_Slot_1_Slot_1                0x0030
    #define   LSb32NAGRA_CERT_OLD_Slot_1_Slot_1                   0
    #define   LSb16NAGRA_CERT_OLD_Slot_1_Slot_1                   0
    #define       bNAGRA_CERT_OLD_Slot_1_Slot_1                1
    #define   MSK32NAGRA_CERT_OLD_Slot_1_Slot_1                   0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_NAGRA_CERT_OLD_Slot_2                       0x0060

    #define     BA_NAGRA_CERT_OLD_Slot_2_Slot_2                0x0060
    #define     B16NAGRA_CERT_OLD_Slot_2_Slot_2                0x0060
    #define   LSb32NAGRA_CERT_OLD_Slot_2_Slot_2                   0
    #define   LSb16NAGRA_CERT_OLD_Slot_2_Slot_2                   0
    #define       bNAGRA_CERT_OLD_Slot_2_Slot_2                1
    #define   MSK32NAGRA_CERT_OLD_Slot_2_Slot_2                   0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_NAGRA_CERT_OLD_Slot_3                       0x0070

    #define     BA_NAGRA_CERT_OLD_Slot_3_Slot_3                0x0070
    #define     B16NAGRA_CERT_OLD_Slot_3_Slot_3                0x0070
    #define   LSb32NAGRA_CERT_OLD_Slot_3_Slot_3                   0
    #define   LSb16NAGRA_CERT_OLD_Slot_3_Slot_3                   0
    #define       bNAGRA_CERT_OLD_Slot_3_Slot_3                1
    #define   MSK32NAGRA_CERT_OLD_Slot_3_Slot_3                   0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_NAGRA_CERT_OLD_Slot_lock                    0x0080

    #define     BA_NAGRA_CERT_OLD_Slot_lock_Slot_0_lock        0x0080
    #define     B16NAGRA_CERT_OLD_Slot_lock_Slot_0_lock        0x0080
    #define   LSb32NAGRA_CERT_OLD_Slot_lock_Slot_0_lock           0
    #define   LSb16NAGRA_CERT_OLD_Slot_lock_Slot_0_lock           0
    #define       bNAGRA_CERT_OLD_Slot_lock_Slot_0_lock        2
    #define   MSK32NAGRA_CERT_OLD_Slot_lock_Slot_0_lock           0x00000003

    #define     BA_NAGRA_CERT_OLD_Slot_lock_Slot_1_lock        0x0080
    #define     B16NAGRA_CERT_OLD_Slot_lock_Slot_1_lock        0x0080
    #define   LSb32NAGRA_CERT_OLD_Slot_lock_Slot_1_lock           2
    #define   LSb16NAGRA_CERT_OLD_Slot_lock_Slot_1_lock           2
    #define       bNAGRA_CERT_OLD_Slot_lock_Slot_1_lock        2
    #define   MSK32NAGRA_CERT_OLD_Slot_lock_Slot_1_lock           0x0000000C

    #define     BA_NAGRA_CERT_OLD_Slot_lock_Slot_2_lock        0x0080
    #define     B16NAGRA_CERT_OLD_Slot_lock_Slot_2_lock        0x0080
    #define   LSb32NAGRA_CERT_OLD_Slot_lock_Slot_2_lock           4
    #define   LSb16NAGRA_CERT_OLD_Slot_lock_Slot_2_lock           4
    #define       bNAGRA_CERT_OLD_Slot_lock_Slot_2_lock        2
    #define   MSK32NAGRA_CERT_OLD_Slot_lock_Slot_2_lock           0x00000030

    #define     BA_NAGRA_CERT_OLD_Slot_lock_Slot_3_lock        0x0080
    #define     B16NAGRA_CERT_OLD_Slot_lock_Slot_3_lock        0x0080
    #define   LSb32NAGRA_CERT_OLD_Slot_lock_Slot_3_lock           6
    #define   LSb16NAGRA_CERT_OLD_Slot_lock_Slot_3_lock           6
    #define       bNAGRA_CERT_OLD_Slot_lock_Slot_3_lock        2
    #define   MSK32NAGRA_CERT_OLD_Slot_lock_Slot_3_lock           0x000000C0
    ///////////////////////////////////////////////////////////

    typedef struct SIE_NAGRA_CERT_OLD {
    ///////////////////////////////////////////////////////////
    #define   GET32NAGRA_CERT_OLD_Slot_0_Slot_0(r32)           _BFGET_(r32, 0, 0)
    #define   SET32NAGRA_CERT_OLD_Slot_0_Slot_0(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   GET16NAGRA_CERT_OLD_Slot_0_Slot_0(r16)           _BFGET_(r16, 0, 0)
    #define   SET16NAGRA_CERT_OLD_Slot_0_Slot_0(r16,v)         _BFSET_(r16, 0, 0,v)

    #define     w32NAGRA_CERT_OLD_Slot_0                       {\
            UNSG32 uSlot_0_Slot_0                              :  1;\
            UNSG32 RSVDx0_b1                                   : 31;\
          }
    union { UNSG32 u32NAGRA_CERT_OLD_Slot_0;
            struct w32NAGRA_CERT_OLD_Slot_0;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx4                                      [44];
    ///////////////////////////////////////////////////////////
    #define   GET32NAGRA_CERT_OLD_Slot_1_Slot_1(r32)           _BFGET_(r32, 0, 0)
    #define   SET32NAGRA_CERT_OLD_Slot_1_Slot_1(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   GET16NAGRA_CERT_OLD_Slot_1_Slot_1(r16)           _BFGET_(r16, 0, 0)
    #define   SET16NAGRA_CERT_OLD_Slot_1_Slot_1(r16,v)         _BFSET_(r16, 0, 0,v)

    #define     w32NAGRA_CERT_OLD_Slot_1                       {\
            UNSG32 uSlot_1_Slot_1                              :  1;\
            UNSG32 RSVDx30_b1                                  : 31;\
          }
    union { UNSG32 u32NAGRA_CERT_OLD_Slot_1;
            struct w32NAGRA_CERT_OLD_Slot_1;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx34                                     [44];
    ///////////////////////////////////////////////////////////
    #define   GET32NAGRA_CERT_OLD_Slot_2_Slot_2(r32)           _BFGET_(r32, 0, 0)
    #define   SET32NAGRA_CERT_OLD_Slot_2_Slot_2(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   GET16NAGRA_CERT_OLD_Slot_2_Slot_2(r16)           _BFGET_(r16, 0, 0)
    #define   SET16NAGRA_CERT_OLD_Slot_2_Slot_2(r16,v)         _BFSET_(r16, 0, 0,v)

    #define     w32NAGRA_CERT_OLD_Slot_2                       {\
            UNSG32 uSlot_2_Slot_2                              :  1;\
            UNSG32 RSVDx60_b1                                  : 31;\
          }
    union { UNSG32 u32NAGRA_CERT_OLD_Slot_2;
            struct w32NAGRA_CERT_OLD_Slot_2;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx64                                     [12];
    ///////////////////////////////////////////////////////////
    #define   GET32NAGRA_CERT_OLD_Slot_3_Slot_3(r32)           _BFGET_(r32, 0, 0)
    #define   SET32NAGRA_CERT_OLD_Slot_3_Slot_3(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   GET16NAGRA_CERT_OLD_Slot_3_Slot_3(r16)           _BFGET_(r16, 0, 0)
    #define   SET16NAGRA_CERT_OLD_Slot_3_Slot_3(r16,v)         _BFSET_(r16, 0, 0,v)

    #define     w32NAGRA_CERT_OLD_Slot_3                       {\
            UNSG32 uSlot_3_Slot_3                              :  1;\
            UNSG32 RSVDx70_b1                                  : 31;\
          }
    union { UNSG32 u32NAGRA_CERT_OLD_Slot_3;
            struct w32NAGRA_CERT_OLD_Slot_3;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx74                                     [12];
    ///////////////////////////////////////////////////////////
    #define   GET32NAGRA_CERT_OLD_Slot_lock_Slot_0_lock(r32)   _BFGET_(r32, 1, 0)
    #define   SET32NAGRA_CERT_OLD_Slot_lock_Slot_0_lock(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16NAGRA_CERT_OLD_Slot_lock_Slot_0_lock(r16)   _BFGET_(r16, 1, 0)
    #define   SET16NAGRA_CERT_OLD_Slot_lock_Slot_0_lock(r16,v) _BFSET_(r16, 1, 0,v)

    #define   GET32NAGRA_CERT_OLD_Slot_lock_Slot_1_lock(r32)   _BFGET_(r32, 3, 2)
    #define   SET32NAGRA_CERT_OLD_Slot_lock_Slot_1_lock(r32,v) _BFSET_(r32, 3, 2,v)
    #define   GET16NAGRA_CERT_OLD_Slot_lock_Slot_1_lock(r16)   _BFGET_(r16, 3, 2)
    #define   SET16NAGRA_CERT_OLD_Slot_lock_Slot_1_lock(r16,v) _BFSET_(r16, 3, 2,v)

    #define   GET32NAGRA_CERT_OLD_Slot_lock_Slot_2_lock(r32)   _BFGET_(r32, 5, 4)
    #define   SET32NAGRA_CERT_OLD_Slot_lock_Slot_2_lock(r32,v) _BFSET_(r32, 5, 4,v)
    #define   GET16NAGRA_CERT_OLD_Slot_lock_Slot_2_lock(r16)   _BFGET_(r16, 5, 4)
    #define   SET16NAGRA_CERT_OLD_Slot_lock_Slot_2_lock(r16,v) _BFSET_(r16, 5, 4,v)

    #define   GET32NAGRA_CERT_OLD_Slot_lock_Slot_3_lock(r32)   _BFGET_(r32, 7, 6)
    #define   SET32NAGRA_CERT_OLD_Slot_lock_Slot_3_lock(r32,v) _BFSET_(r32, 7, 6,v)
    #define   GET16NAGRA_CERT_OLD_Slot_lock_Slot_3_lock(r16)   _BFGET_(r16, 7, 6)
    #define   SET16NAGRA_CERT_OLD_Slot_lock_Slot_3_lock(r16,v) _BFSET_(r16, 7, 6,v)

    #define     w32NAGRA_CERT_OLD_Slot_lock                    {\
            UNSG32 uSlot_lock_Slot_0_lock                      :  2;\
            UNSG32 uSlot_lock_Slot_1_lock                      :  2;\
            UNSG32 uSlot_lock_Slot_2_lock                      :  2;\
            UNSG32 uSlot_lock_Slot_3_lock                      :  2;\
            UNSG32 RSVDx80_b8                                  : 24;\
          }
    union { UNSG32 u32NAGRA_CERT_OLD_Slot_lock;
            struct w32NAGRA_CERT_OLD_Slot_lock;
          };
    ///////////////////////////////////////////////////////////
    } SIE_NAGRA_CERT_OLD;

    typedef union  T32NAGRA_CERT_OLD_Slot_0
          { UNSG32 u32;
            struct w32NAGRA_CERT_OLD_Slot_0;
                 } T32NAGRA_CERT_OLD_Slot_0;
    typedef union  T32NAGRA_CERT_OLD_Slot_1
          { UNSG32 u32;
            struct w32NAGRA_CERT_OLD_Slot_1;
                 } T32NAGRA_CERT_OLD_Slot_1;
    typedef union  T32NAGRA_CERT_OLD_Slot_2
          { UNSG32 u32;
            struct w32NAGRA_CERT_OLD_Slot_2;
                 } T32NAGRA_CERT_OLD_Slot_2;
    typedef union  T32NAGRA_CERT_OLD_Slot_3
          { UNSG32 u32;
            struct w32NAGRA_CERT_OLD_Slot_3;
                 } T32NAGRA_CERT_OLD_Slot_3;
    typedef union  T32NAGRA_CERT_OLD_Slot_lock
          { UNSG32 u32;
            struct w32NAGRA_CERT_OLD_Slot_lock;
                 } T32NAGRA_CERT_OLD_Slot_lock;
    ///////////////////////////////////////////////////////////

    typedef union  TNAGRA_CERT_OLD_Slot_0
          { UNSG32 u32[1];
            struct {
            struct w32NAGRA_CERT_OLD_Slot_0;
                   };
                 } TNAGRA_CERT_OLD_Slot_0;
    typedef union  TNAGRA_CERT_OLD_Slot_1
          { UNSG32 u32[1];
            struct {
            struct w32NAGRA_CERT_OLD_Slot_1;
                   };
                 } TNAGRA_CERT_OLD_Slot_1;
    typedef union  TNAGRA_CERT_OLD_Slot_2
          { UNSG32 u32[1];
            struct {
            struct w32NAGRA_CERT_OLD_Slot_2;
                   };
                 } TNAGRA_CERT_OLD_Slot_2;
    typedef union  TNAGRA_CERT_OLD_Slot_3
          { UNSG32 u32[1];
            struct {
            struct w32NAGRA_CERT_OLD_Slot_3;
                   };
                 } TNAGRA_CERT_OLD_Slot_3;
    typedef union  TNAGRA_CERT_OLD_Slot_lock
          { UNSG32 u32[1];
            struct {
            struct w32NAGRA_CERT_OLD_Slot_lock;
                   };
                 } TNAGRA_CERT_OLD_Slot_lock;

    ///////////////////////////////////////////////////////////
     SIGN32 NAGRA_CERT_OLD_drvrd(SIE_NAGRA_CERT_OLD *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 NAGRA_CERT_OLD_drvwr(SIE_NAGRA_CERT_OLD *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void NAGRA_CERT_OLD_reset(SIE_NAGRA_CERT_OLD *p);
     SIGN32 NAGRA_CERT_OLD_cmp  (SIE_NAGRA_CERT_OLD *p, SIE_NAGRA_CERT_OLD *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define NAGRA_CERT_OLD_check(p,pie,pfx,hLOG) NAGRA_CERT_OLD_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define NAGRA_CERT_OLD_print(p,    pfx,hLOG) NAGRA_CERT_OLD_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: NAGRA_CERT_OLD
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE GENERIC_NORMAL                           (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (W-)
///     #         # Stuffing bytes...
///               %% 448
///     @ 0x00038 MDK                  (P)
///               %unsigned 1  MDK                       
///                                    ###
///                                    * 16 BYTEs MDK Key code.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x0003C                      (W-)
///     #         # Stuffing bytes...
///               %% 96
///     @ 0x00048 MDK2                 (P)
///               %unsigned 1  MDK2                      
///                                    ###
///                                    * 16 BYTEs MDK2 Key code.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x0004C                      (W-)
///     #         # Stuffing bytes...
///               %% 1888
///     @ 0x00138 RKEK                 (P)
///               %unsigned 1  RKEK                      
///                                    ###
///                                    * 16 BYTEs RKEK Key code.
///                                    ###
///               %%        31         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     316B, bits:       3b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_GENERIC_NORMAL
#define h_GENERIC_NORMAL (){}

    #define     RA_GENERIC_NORMAL_MDK                          0x0038

    #define     BA_GENERIC_NORMAL_MDK_MDK                      0x0038
    #define     B16GENERIC_NORMAL_MDK_MDK                      0x0038
    #define   LSb32GENERIC_NORMAL_MDK_MDK                         0
    #define   LSb16GENERIC_NORMAL_MDK_MDK                         0
    #define       bGENERIC_NORMAL_MDK_MDK                      1
    #define   MSK32GENERIC_NORMAL_MDK_MDK                         0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_GENERIC_NORMAL_MDK2                         0x0048

    #define     BA_GENERIC_NORMAL_MDK2_MDK2                    0x0048
    #define     B16GENERIC_NORMAL_MDK2_MDK2                    0x0048
    #define   LSb32GENERIC_NORMAL_MDK2_MDK2                       0
    #define   LSb16GENERIC_NORMAL_MDK2_MDK2                       0
    #define       bGENERIC_NORMAL_MDK2_MDK2                    1
    #define   MSK32GENERIC_NORMAL_MDK2_MDK2                       0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_GENERIC_NORMAL_RKEK                         0x0138

    #define     BA_GENERIC_NORMAL_RKEK_RKEK                    0x0138
    #define     B16GENERIC_NORMAL_RKEK_RKEK                    0x0138
    #define   LSb32GENERIC_NORMAL_RKEK_RKEK                       0
    #define   LSb16GENERIC_NORMAL_RKEK_RKEK                       0
    #define       bGENERIC_NORMAL_RKEK_RKEK                    1
    #define   MSK32GENERIC_NORMAL_RKEK_RKEK                       0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_GENERIC_NORMAL {
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx0                                      [56];
    ///////////////////////////////////////////////////////////
    #define   GET32GENERIC_NORMAL_MDK_MDK(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32GENERIC_NORMAL_MDK_MDK(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16GENERIC_NORMAL_MDK_MDK(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16GENERIC_NORMAL_MDK_MDK(r16,v)               _BFSET_(r16, 0, 0,v)

    #define     w32GENERIC_NORMAL_MDK                          {\
            UNSG32 uMDK_MDK                                    :  1;\
            UNSG32 RSVDx38_b1                                  : 31;\
          }
    union { UNSG32 u32GENERIC_NORMAL_MDK;
            struct w32GENERIC_NORMAL_MDK;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx3C                                     [12];
    ///////////////////////////////////////////////////////////
    #define   GET32GENERIC_NORMAL_MDK2_MDK2(r32)               _BFGET_(r32, 0, 0)
    #define   SET32GENERIC_NORMAL_MDK2_MDK2(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16GENERIC_NORMAL_MDK2_MDK2(r16)               _BFGET_(r16, 0, 0)
    #define   SET16GENERIC_NORMAL_MDK2_MDK2(r16,v)             _BFSET_(r16, 0, 0,v)

    #define     w32GENERIC_NORMAL_MDK2                         {\
            UNSG32 uMDK2_MDK2                                  :  1;\
            UNSG32 RSVDx48_b1                                  : 31;\
          }
    union { UNSG32 u32GENERIC_NORMAL_MDK2;
            struct w32GENERIC_NORMAL_MDK2;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx4C                                     [236];
    ///////////////////////////////////////////////////////////
    #define   GET32GENERIC_NORMAL_RKEK_RKEK(r32)               _BFGET_(r32, 0, 0)
    #define   SET32GENERIC_NORMAL_RKEK_RKEK(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16GENERIC_NORMAL_RKEK_RKEK(r16)               _BFGET_(r16, 0, 0)
    #define   SET16GENERIC_NORMAL_RKEK_RKEK(r16,v)             _BFSET_(r16, 0, 0,v)

    #define     w32GENERIC_NORMAL_RKEK                         {\
            UNSG32 uRKEK_RKEK                                  :  1;\
            UNSG32 RSVDx138_b1                                 : 31;\
          }
    union { UNSG32 u32GENERIC_NORMAL_RKEK;
            struct w32GENERIC_NORMAL_RKEK;
          };
    ///////////////////////////////////////////////////////////
    } SIE_GENERIC_NORMAL;

    typedef union  T32GENERIC_NORMAL_MDK
          { UNSG32 u32;
            struct w32GENERIC_NORMAL_MDK;
                 } T32GENERIC_NORMAL_MDK;
    typedef union  T32GENERIC_NORMAL_MDK2
          { UNSG32 u32;
            struct w32GENERIC_NORMAL_MDK2;
                 } T32GENERIC_NORMAL_MDK2;
    typedef union  T32GENERIC_NORMAL_RKEK
          { UNSG32 u32;
            struct w32GENERIC_NORMAL_RKEK;
                 } T32GENERIC_NORMAL_RKEK;
    ///////////////////////////////////////////////////////////

    typedef union  TGENERIC_NORMAL_MDK
          { UNSG32 u32[1];
            struct {
            struct w32GENERIC_NORMAL_MDK;
                   };
                 } TGENERIC_NORMAL_MDK;
    typedef union  TGENERIC_NORMAL_MDK2
          { UNSG32 u32[1];
            struct {
            struct w32GENERIC_NORMAL_MDK2;
                   };
                 } TGENERIC_NORMAL_MDK2;
    typedef union  TGENERIC_NORMAL_RKEK
          { UNSG32 u32[1];
            struct {
            struct w32GENERIC_NORMAL_RKEK;
                   };
                 } TGENERIC_NORMAL_RKEK;

    ///////////////////////////////////////////////////////////
     SIGN32 GENERIC_NORMAL_drvrd(SIE_GENERIC_NORMAL *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 GENERIC_NORMAL_drvwr(SIE_GENERIC_NORMAL *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void GENERIC_NORMAL_reset(SIE_GENERIC_NORMAL *p);
     SIGN32 GENERIC_NORMAL_cmp  (SIE_GENERIC_NORMAL *p, SIE_GENERIC_NORMAL *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define GENERIC_NORMAL_check(p,pie,pfx,hLOG) GENERIC_NORMAL_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define GENERIC_NORMAL_print(p,    pfx,hLOG) GENERIC_NORMAL_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: GENERIC_NORMAL
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE Gen3_SecureBoot                          (4,4)
///     ###
///     * ACPU& BCM can read this register except key.
///     * Entire register is open for R/W at eng_en chip
///     ###
///     # # ----------------------------------------------------------
///     @ 0x00000 CAKeyIDA             (RW-)
///     # 0x00000 CAKeyIDA             
///               $OneReg              CAKeyIDA          REG       [2]
///                                    ###
///                                    * KeyID0. Factory Program
///                                    ###
///     @ 0x00008 CARootKeyA           (RW-)
///     # 0x00008 CARootKeyA           
///               $OneReg              CARootKeyA        REG       [4]
///                                    ###
///                                    * Key0. Factory Program
///                                    ###
///     @ 0x00018 CAKeyIDB             (RW-)
///     # 0x00018 CAKeyIDB             
///               $OneReg              CAKeyIDB          REG       [2]
///                                    ###
///                                    * KeyID1. Factory Program
///                                    ###
///     @ 0x00020 CARootKeyB           (RW-)
///     # 0x00020 CARootKeyB           
///               $OneReg              CARootKeyB        REG       [4]
///                                    ###
///                                    * Key1. Factory Program
///                                    ###
///     @ 0x00030 soc_uid              (RW-)
///     # 0x00030 soc_uid              
///               $OneReg              soc_uid           REG       [2]
///                                    ###
///                                    * Marvell unique chip ID. No Factory Program
///                                    ###
///     @ 0x00038 STB_CA_SN_REF        (RW-)
///     # 0x00038 STB_CA_SN_REF        
///               $OneReg              STB_CA_SN_REF     REG          
///                                    ###
///                                    * STB_CA_SN_REF. No Factory Program
///                                    ###
///     @ 0x0003C Market_ID            (RW-)
///     # 0x0003C Market_ID            
///               $OneReg              Market_ID         REG          
///                                    ###
///                                    * Market_ID. No Factory Program
///                                    ###
///     @ 0x00040 Code_version         (RW-)
///     # 0x00040 Code_version         
///               $OneReg              Code_version      REG          
///                                    ###
///                                    * Code_version. No Factory Program. One-way
///                                    ###
///     @ 0x00044 Field_0              (RW-)
///               ###
///               * lock
///               ###
///               %unsigned 8  MIDLock_CASNLock          0x0
///                                    ###
///                                    * 3:0 STB_CA_SN lock
///                                    * 7:4 Market_ID lock
///                                    ###
///               %unsigned 8  Production_Chip_Flag      0x0
///               %unsigned 8  UART_Disable              0x0
///               %unsigned 8  CR_Disable_BlankRead_Done 0x0
///                                    ###
///                                    * Bit0: blankread_done
///                                    * Bit4: CR_Disable
///                                    ###
///     @ 0x00048 Field_1              (RW-)
///               ###
///               * factory program
///               ###
///               %unsigned 8  RFU0                      0x0
///               %unsigned 8  SecretDebugIFDisable      0x0
///                                    ###
///                                    * 0: SecretDebugIFDisable
///                                    ###
///               %unsigned 8  Perso_done_Nagra_Enable   0x0
///                                    ###
///                                    * 4: Perso_done
///                                    * 0: Gen3_Enable
///                                    ###
///               %unsigned 8  CERT_Enable               0x0
///                                    ###
///                                    * 0: CERT_Enable
///                                    * 4: slow clock enable
///                                    ###
///     @ 0x0004C Field_2              (RW-)
///               ###
///               * No factory program
///               * One-way
///               ###
///               %unsigned 8  JTAGPasswordProtected     0x0
///                                    ###
///                                    * 1:0 JTAGPasswordProtected
///                                    * 5:4 TEEPasswordProtected
///                                    ###
///               %unsigned 8  KeyladderDisable          0x0
///                                    ###
///                                    * 0: CERT_Protected
///                                    * 2: TEE_Protected
///                                    * 4: KeyladderDisable
///                                    * 6: USB_ROM_Code_Disable
///                                    ###
///               %unsigned 8  DRAMScrambleEnable        0x0
///                                    ###
///                                    * 0: DRAMScrambleEnable One-way
///                                    * 6: ROM1_Code_Disable
///                                    ###
///               %unsigned 8  SecureBootEnable_Priv_mode_act 0x0
///                                    ###
///                                    * 0: SecureBootEnable One-way
///                                    * 4: private_mode_act
///                                    ###
///     @ 0x00050 Active_Indicator     (RW-)
///     # 0x00050 Active_Indicator     
///               $OneReg              Active_Indicator  REG          
///                                    ###
///                                    * Active_Indicator. No Factory Program. One-way
///                                    ###
///     @ 0x00054 SOC_Version          (RW-)
///     # 0x00054 SOC_Version          
///               $OneReg              SOC_Version       REG          
///                                    ###
///                                    * One-way
///                                    ###
///     @ 0x00058 TEE_Version          (RW-)
///     # 0x00058 TEE_Version          
///               $OneReg              TEE_Version       REG          
///                                    ###
///                                    * One-way
///                                    ###
///     @ 0x0005C RSVD_OneWay0         (RW-)
///     # 0x0005C RSVD_OneWay0         
///               $OneReg              RSVD_OneWay0      REG          
///                                    ###
///                                    * One-way
///                                    ###
///     @ 0x00060 RSVD_OneWay1         (RW-)
///     # 0x00060 RSVD_OneWay1         
///               $OneReg              RSVD_OneWay1      REG          
///                                    ###
///                                    * One-way
///                                    ###
///     @ 0x00064 CARootKeyAChkSum     (RW-)
///     # 0x00064 CARootKeyAChkSum     
///               $OneReg              CARootKeyAChkSum  REG          
///     @ 0x00068 CARootKeyBChkSum     (RW-)
///     # 0x00068 CARootKeyBChkSum     
///               $OneReg              CARootKeyBChkSum  REG          
///     @ 0x0006C TEE_SegID            (RW-)
///     # 0x0006C TEE_SegID            
///               $OneReg              TEE_SegID         REG          
///     @ 0x00070 VendorID             (RW-)
///               %unsigned 8  SW_VendorID0              0x0
///                                    ###
///                                    * SW_VendorID0
///                                    ###
///               %unsigned 8  SW_VendorID1              0x0
///                                    ###
///                                    * SW_VendorID1
///                                    ###
///               %unsigned 8  SW_VendorID2              0x0
///                                    ###
///                                    * SW_VendorID2
///                                    ###
///               %unsigned 8  SW_VendorID3              0x0
///                                    ###
///                                    * SW_VendorID3
///                                    ###
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     116B, bits:     928b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_Gen3_SecureBoot
#define h_Gen3_SecureBoot (){}

    #define     RA_Gen3_SecureBoot_CAKeyIDA                    0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_CARootKeyA                  0x0008
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_CAKeyIDB                    0x0018
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_CARootKeyB                  0x0020
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_soc_uid                     0x0030
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_STB_CA_SN_REF               0x0038
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_Market_ID                   0x003C
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_Code_version                0x0040
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_Field_0                     0x0044

    #define     BA_Gen3_SecureBoot_Field_0_MIDLock_CASNLock    0x0044
    #define     B16Gen3_SecureBoot_Field_0_MIDLock_CASNLock    0x0044
    #define   LSb32Gen3_SecureBoot_Field_0_MIDLock_CASNLock       0
    #define   LSb16Gen3_SecureBoot_Field_0_MIDLock_CASNLock       0
    #define       bGen3_SecureBoot_Field_0_MIDLock_CASNLock    8
    #define   MSK32Gen3_SecureBoot_Field_0_MIDLock_CASNLock       0x000000FF

    #define     BA_Gen3_SecureBoot_Field_0_Production_Chip_Flag 0x0045
    #define     B16Gen3_SecureBoot_Field_0_Production_Chip_Flag 0x0044
    #define   LSb32Gen3_SecureBoot_Field_0_Production_Chip_Flag    8
    #define   LSb16Gen3_SecureBoot_Field_0_Production_Chip_Flag    8
    #define       bGen3_SecureBoot_Field_0_Production_Chip_Flag 8
    #define   MSK32Gen3_SecureBoot_Field_0_Production_Chip_Flag    0x0000FF00

    #define     BA_Gen3_SecureBoot_Field_0_UART_Disable        0x0046
    #define     B16Gen3_SecureBoot_Field_0_UART_Disable        0x0046
    #define   LSb32Gen3_SecureBoot_Field_0_UART_Disable           16
    #define   LSb16Gen3_SecureBoot_Field_0_UART_Disable           0
    #define       bGen3_SecureBoot_Field_0_UART_Disable        8
    #define   MSK32Gen3_SecureBoot_Field_0_UART_Disable           0x00FF0000

    #define     BA_Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done 0x0047
    #define     B16Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done 0x0046
    #define   LSb32Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done    24
    #define   LSb16Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done    8
    #define       bGen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done 8
    #define   MSK32Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done    0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_Field_1                     0x0048

    #define     BA_Gen3_SecureBoot_Field_1_RFU0                0x0048
    #define     B16Gen3_SecureBoot_Field_1_RFU0                0x0048
    #define   LSb32Gen3_SecureBoot_Field_1_RFU0                   0
    #define   LSb16Gen3_SecureBoot_Field_1_RFU0                   0
    #define       bGen3_SecureBoot_Field_1_RFU0                8
    #define   MSK32Gen3_SecureBoot_Field_1_RFU0                   0x000000FF

    #define     BA_Gen3_SecureBoot_Field_1_SecretDebugIFDisable 0x0049
    #define     B16Gen3_SecureBoot_Field_1_SecretDebugIFDisable 0x0048
    #define   LSb32Gen3_SecureBoot_Field_1_SecretDebugIFDisable    8
    #define   LSb16Gen3_SecureBoot_Field_1_SecretDebugIFDisable    8
    #define       bGen3_SecureBoot_Field_1_SecretDebugIFDisable 8
    #define   MSK32Gen3_SecureBoot_Field_1_SecretDebugIFDisable    0x0000FF00

    #define     BA_Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable 0x004A
    #define     B16Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable 0x004A
    #define   LSb32Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable    16
    #define   LSb16Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable    0
    #define       bGen3_SecureBoot_Field_1_Perso_done_Nagra_Enable 8
    #define   MSK32Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable    0x00FF0000

    #define     BA_Gen3_SecureBoot_Field_1_CERT_Enable         0x004B
    #define     B16Gen3_SecureBoot_Field_1_CERT_Enable         0x004A
    #define   LSb32Gen3_SecureBoot_Field_1_CERT_Enable            24
    #define   LSb16Gen3_SecureBoot_Field_1_CERT_Enable            8
    #define       bGen3_SecureBoot_Field_1_CERT_Enable         8
    #define   MSK32Gen3_SecureBoot_Field_1_CERT_Enable            0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_Field_2                     0x004C

    #define     BA_Gen3_SecureBoot_Field_2_JTAGPasswordProtected 0x004C
    #define     B16Gen3_SecureBoot_Field_2_JTAGPasswordProtected 0x004C
    #define   LSb32Gen3_SecureBoot_Field_2_JTAGPasswordProtected    0
    #define   LSb16Gen3_SecureBoot_Field_2_JTAGPasswordProtected    0
    #define       bGen3_SecureBoot_Field_2_JTAGPasswordProtected 8
    #define   MSK32Gen3_SecureBoot_Field_2_JTAGPasswordProtected    0x000000FF

    #define     BA_Gen3_SecureBoot_Field_2_KeyladderDisable    0x004D
    #define     B16Gen3_SecureBoot_Field_2_KeyladderDisable    0x004C
    #define   LSb32Gen3_SecureBoot_Field_2_KeyladderDisable       8
    #define   LSb16Gen3_SecureBoot_Field_2_KeyladderDisable       8
    #define       bGen3_SecureBoot_Field_2_KeyladderDisable    8
    #define   MSK32Gen3_SecureBoot_Field_2_KeyladderDisable       0x0000FF00

    #define     BA_Gen3_SecureBoot_Field_2_DRAMScrambleEnable  0x004E
    #define     B16Gen3_SecureBoot_Field_2_DRAMScrambleEnable  0x004E
    #define   LSb32Gen3_SecureBoot_Field_2_DRAMScrambleEnable     16
    #define   LSb16Gen3_SecureBoot_Field_2_DRAMScrambleEnable     0
    #define       bGen3_SecureBoot_Field_2_DRAMScrambleEnable  8
    #define   MSK32Gen3_SecureBoot_Field_2_DRAMScrambleEnable     0x00FF0000

    #define     BA_Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act 0x004F
    #define     B16Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act 0x004E
    #define   LSb32Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act    24
    #define   LSb16Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act    8
    #define       bGen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act 8
    #define   MSK32Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act    0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_Active_Indicator            0x0050
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_SOC_Version                 0x0054
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_TEE_Version                 0x0058
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_RSVD_OneWay0                0x005C
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_RSVD_OneWay1                0x0060
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_CARootKeyAChkSum            0x0064
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_CARootKeyBChkSum            0x0068
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_TEE_SegID                   0x006C
    ///////////////////////////////////////////////////////////
    #define     RA_Gen3_SecureBoot_VendorID                    0x0070

    #define     BA_Gen3_SecureBoot_VendorID_SW_VendorID0       0x0070
    #define     B16Gen3_SecureBoot_VendorID_SW_VendorID0       0x0070
    #define   LSb32Gen3_SecureBoot_VendorID_SW_VendorID0          0
    #define   LSb16Gen3_SecureBoot_VendorID_SW_VendorID0          0
    #define       bGen3_SecureBoot_VendorID_SW_VendorID0       8
    #define   MSK32Gen3_SecureBoot_VendorID_SW_VendorID0          0x000000FF

    #define     BA_Gen3_SecureBoot_VendorID_SW_VendorID1       0x0071
    #define     B16Gen3_SecureBoot_VendorID_SW_VendorID1       0x0070
    #define   LSb32Gen3_SecureBoot_VendorID_SW_VendorID1          8
    #define   LSb16Gen3_SecureBoot_VendorID_SW_VendorID1          8
    #define       bGen3_SecureBoot_VendorID_SW_VendorID1       8
    #define   MSK32Gen3_SecureBoot_VendorID_SW_VendorID1          0x0000FF00

    #define     BA_Gen3_SecureBoot_VendorID_SW_VendorID2       0x0072
    #define     B16Gen3_SecureBoot_VendorID_SW_VendorID2       0x0072
    #define   LSb32Gen3_SecureBoot_VendorID_SW_VendorID2          16
    #define   LSb16Gen3_SecureBoot_VendorID_SW_VendorID2          0
    #define       bGen3_SecureBoot_VendorID_SW_VendorID2       8
    #define   MSK32Gen3_SecureBoot_VendorID_SW_VendorID2          0x00FF0000

    #define     BA_Gen3_SecureBoot_VendorID_SW_VendorID3       0x0073
    #define     B16Gen3_SecureBoot_VendorID_SW_VendorID3       0x0072
    #define   LSb32Gen3_SecureBoot_VendorID_SW_VendorID3          24
    #define   LSb16Gen3_SecureBoot_VendorID_SW_VendorID3          8
    #define       bGen3_SecureBoot_VendorID_SW_VendorID3       8
    #define   MSK32Gen3_SecureBoot_VendorID_SW_VendorID3          0xFF000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_Gen3_SecureBoot {
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CAKeyIDA[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CARootKeyA[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CAKeyIDB[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CARootKeyB[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_soc_uid[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_STB_CA_SN_REF;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_Market_ID;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_Code_version;
    ///////////////////////////////////////////////////////////
    #define   GET32Gen3_SecureBoot_Field_0_MIDLock_CASNLock(r32) _BFGET_(r32, 7, 0)
    #define   SET32Gen3_SecureBoot_Field_0_MIDLock_CASNLock(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16Gen3_SecureBoot_Field_0_MIDLock_CASNLock(r16) _BFGET_(r16, 7, 0)
    #define   SET16Gen3_SecureBoot_Field_0_MIDLock_CASNLock(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32Gen3_SecureBoot_Field_0_Production_Chip_Flag(r32) _BFGET_(r32,15, 8)
    #define   SET32Gen3_SecureBoot_Field_0_Production_Chip_Flag(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16Gen3_SecureBoot_Field_0_Production_Chip_Flag(r16) _BFGET_(r16,15, 8)
    #define   SET16Gen3_SecureBoot_Field_0_Production_Chip_Flag(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32Gen3_SecureBoot_Field_0_UART_Disable(r32)   _BFGET_(r32,23,16)
    #define   SET32Gen3_SecureBoot_Field_0_UART_Disable(r32,v) _BFSET_(r32,23,16,v)
    #define   GET16Gen3_SecureBoot_Field_0_UART_Disable(r16)   _BFGET_(r16, 7, 0)
    #define   SET16Gen3_SecureBoot_Field_0_UART_Disable(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done(r32) _BFGET_(r32,31,24)
    #define   SET32Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done(r32,v) _BFSET_(r32,31,24,v)
    #define   GET16Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done(r16) _BFGET_(r16,15, 8)
    #define   SET16Gen3_SecureBoot_Field_0_CR_Disable_BlankRead_Done(r16,v) _BFSET_(r16,15, 8,v)

    #define     w32Gen3_SecureBoot_Field_0                     {\
            UNSG32 uField_0_MIDLock_CASNLock                   :  8;\
            UNSG32 uField_0_Production_Chip_Flag               :  8;\
            UNSG32 uField_0_UART_Disable                       :  8;\
            UNSG32 uField_0_CR_Disable_BlankRead_Done          :  8;\
          }
    union { UNSG32 u32Gen3_SecureBoot_Field_0;
            struct w32Gen3_SecureBoot_Field_0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gen3_SecureBoot_Field_1_RFU0(r32)           _BFGET_(r32, 7, 0)
    #define   SET32Gen3_SecureBoot_Field_1_RFU0(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16Gen3_SecureBoot_Field_1_RFU0(r16)           _BFGET_(r16, 7, 0)
    #define   SET16Gen3_SecureBoot_Field_1_RFU0(r16,v)         _BFSET_(r16, 7, 0,v)

    #define   GET32Gen3_SecureBoot_Field_1_SecretDebugIFDisable(r32) _BFGET_(r32,15, 8)
    #define   SET32Gen3_SecureBoot_Field_1_SecretDebugIFDisable(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16Gen3_SecureBoot_Field_1_SecretDebugIFDisable(r16) _BFGET_(r16,15, 8)
    #define   SET16Gen3_SecureBoot_Field_1_SecretDebugIFDisable(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable(r32) _BFGET_(r32,23,16)
    #define   SET32Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable(r32,v) _BFSET_(r32,23,16,v)
    #define   GET16Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable(r16) _BFGET_(r16, 7, 0)
    #define   SET16Gen3_SecureBoot_Field_1_Perso_done_Nagra_Enable(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32Gen3_SecureBoot_Field_1_CERT_Enable(r32)    _BFGET_(r32,31,24)
    #define   SET32Gen3_SecureBoot_Field_1_CERT_Enable(r32,v)  _BFSET_(r32,31,24,v)
    #define   GET16Gen3_SecureBoot_Field_1_CERT_Enable(r16)    _BFGET_(r16,15, 8)
    #define   SET16Gen3_SecureBoot_Field_1_CERT_Enable(r16,v)  _BFSET_(r16,15, 8,v)

    #define     w32Gen3_SecureBoot_Field_1                     {\
            UNSG32 uField_1_RFU0                               :  8;\
            UNSG32 uField_1_SecretDebugIFDisable               :  8;\
            UNSG32 uField_1_Perso_done_Nagra_Enable            :  8;\
            UNSG32 uField_1_CERT_Enable                        :  8;\
          }
    union { UNSG32 u32Gen3_SecureBoot_Field_1;
            struct w32Gen3_SecureBoot_Field_1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gen3_SecureBoot_Field_2_JTAGPasswordProtected(r32) _BFGET_(r32, 7, 0)
    #define   SET32Gen3_SecureBoot_Field_2_JTAGPasswordProtected(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16Gen3_SecureBoot_Field_2_JTAGPasswordProtected(r16) _BFGET_(r16, 7, 0)
    #define   SET16Gen3_SecureBoot_Field_2_JTAGPasswordProtected(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32Gen3_SecureBoot_Field_2_KeyladderDisable(r32) _BFGET_(r32,15, 8)
    #define   SET32Gen3_SecureBoot_Field_2_KeyladderDisable(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16Gen3_SecureBoot_Field_2_KeyladderDisable(r16) _BFGET_(r16,15, 8)
    #define   SET16Gen3_SecureBoot_Field_2_KeyladderDisable(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32Gen3_SecureBoot_Field_2_DRAMScrambleEnable(r32) _BFGET_(r32,23,16)
    #define   SET32Gen3_SecureBoot_Field_2_DRAMScrambleEnable(r32,v) _BFSET_(r32,23,16,v)
    #define   GET16Gen3_SecureBoot_Field_2_DRAMScrambleEnable(r16) _BFGET_(r16, 7, 0)
    #define   SET16Gen3_SecureBoot_Field_2_DRAMScrambleEnable(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act(r32) _BFGET_(r32,31,24)
    #define   SET32Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act(r32,v) _BFSET_(r32,31,24,v)
    #define   GET16Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act(r16) _BFGET_(r16,15, 8)
    #define   SET16Gen3_SecureBoot_Field_2_SecureBootEnable_Priv_mode_act(r16,v) _BFSET_(r16,15, 8,v)

    #define     w32Gen3_SecureBoot_Field_2                     {\
            UNSG32 uField_2_JTAGPasswordProtected              :  8;\
            UNSG32 uField_2_KeyladderDisable                   :  8;\
            UNSG32 uField_2_DRAMScrambleEnable                 :  8;\
            UNSG32 uField_2_SecureBootEnable_Priv_mode_act     :  8;\
          }
    union { UNSG32 u32Gen3_SecureBoot_Field_2;
            struct w32Gen3_SecureBoot_Field_2;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_Active_Indicator;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SOC_Version;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_TEE_Version;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_RSVD_OneWay0;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_RSVD_OneWay1;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CARootKeyAChkSum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CARootKeyBChkSum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_TEE_SegID;
    ///////////////////////////////////////////////////////////
    #define   GET32Gen3_SecureBoot_VendorID_SW_VendorID0(r32)  _BFGET_(r32, 7, 0)
    #define   SET32Gen3_SecureBoot_VendorID_SW_VendorID0(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16Gen3_SecureBoot_VendorID_SW_VendorID0(r16)  _BFGET_(r16, 7, 0)
    #define   SET16Gen3_SecureBoot_VendorID_SW_VendorID0(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32Gen3_SecureBoot_VendorID_SW_VendorID1(r32)  _BFGET_(r32,15, 8)
    #define   SET32Gen3_SecureBoot_VendorID_SW_VendorID1(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16Gen3_SecureBoot_VendorID_SW_VendorID1(r16)  _BFGET_(r16,15, 8)
    #define   SET16Gen3_SecureBoot_VendorID_SW_VendorID1(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32Gen3_SecureBoot_VendorID_SW_VendorID2(r32)  _BFGET_(r32,23,16)
    #define   SET32Gen3_SecureBoot_VendorID_SW_VendorID2(r32,v) _BFSET_(r32,23,16,v)
    #define   GET16Gen3_SecureBoot_VendorID_SW_VendorID2(r16)  _BFGET_(r16, 7, 0)
    #define   SET16Gen3_SecureBoot_VendorID_SW_VendorID2(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32Gen3_SecureBoot_VendorID_SW_VendorID3(r32)  _BFGET_(r32,31,24)
    #define   SET32Gen3_SecureBoot_VendorID_SW_VendorID3(r32,v) _BFSET_(r32,31,24,v)
    #define   GET16Gen3_SecureBoot_VendorID_SW_VendorID3(r16)  _BFGET_(r16,15, 8)
    #define   SET16Gen3_SecureBoot_VendorID_SW_VendorID3(r16,v) _BFSET_(r16,15, 8,v)

    #define     w32Gen3_SecureBoot_VendorID                    {\
            UNSG32 uVendorID_SW_VendorID0                      :  8;\
            UNSG32 uVendorID_SW_VendorID1                      :  8;\
            UNSG32 uVendorID_SW_VendorID2                      :  8;\
            UNSG32 uVendorID_SW_VendorID3                      :  8;\
          }
    union { UNSG32 u32Gen3_SecureBoot_VendorID;
            struct w32Gen3_SecureBoot_VendorID;
          };
    ///////////////////////////////////////////////////////////
    } SIE_Gen3_SecureBoot;

    typedef union  T32Gen3_SecureBoot_Field_0
          { UNSG32 u32;
            struct w32Gen3_SecureBoot_Field_0;
                 } T32Gen3_SecureBoot_Field_0;
    typedef union  T32Gen3_SecureBoot_Field_1
          { UNSG32 u32;
            struct w32Gen3_SecureBoot_Field_1;
                 } T32Gen3_SecureBoot_Field_1;
    typedef union  T32Gen3_SecureBoot_Field_2
          { UNSG32 u32;
            struct w32Gen3_SecureBoot_Field_2;
                 } T32Gen3_SecureBoot_Field_2;
    typedef union  T32Gen3_SecureBoot_VendorID
          { UNSG32 u32;
            struct w32Gen3_SecureBoot_VendorID;
                 } T32Gen3_SecureBoot_VendorID;
    ///////////////////////////////////////////////////////////

    typedef union  TGen3_SecureBoot_Field_0
          { UNSG32 u32[1];
            struct {
            struct w32Gen3_SecureBoot_Field_0;
                   };
                 } TGen3_SecureBoot_Field_0;
    typedef union  TGen3_SecureBoot_Field_1
          { UNSG32 u32[1];
            struct {
            struct w32Gen3_SecureBoot_Field_1;
                   };
                 } TGen3_SecureBoot_Field_1;
    typedef union  TGen3_SecureBoot_Field_2
          { UNSG32 u32[1];
            struct {
            struct w32Gen3_SecureBoot_Field_2;
                   };
                 } TGen3_SecureBoot_Field_2;
    typedef union  TGen3_SecureBoot_VendorID
          { UNSG32 u32[1];
            struct {
            struct w32Gen3_SecureBoot_VendorID;
                   };
                 } TGen3_SecureBoot_VendorID;

    ///////////////////////////////////////////////////////////
     SIGN32 Gen3_SecureBoot_drvrd(SIE_Gen3_SecureBoot *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 Gen3_SecureBoot_drvwr(SIE_Gen3_SecureBoot *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void Gen3_SecureBoot_reset(SIE_Gen3_SecureBoot *p);
     SIGN32 Gen3_SecureBoot_cmp  (SIE_Gen3_SecureBoot *p, SIE_Gen3_SecureBoot *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define Gen3_SecureBoot_check(p,pie,pfx,hLOG) Gen3_SecureBoot_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define Gen3_SecureBoot_print(p,    pfx,hLOG) Gen3_SecureBoot_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: Gen3_SecureBoot
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE MBIST_Field                              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 MBIST                (P)
///     # 0x00000 MBIST                
///               $OneReg              MBIST             REG     [128]
///                                    ###
///                                    * 4096 bits MBIST data for memory repair feature.
///                                    ###
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     512B, bits:    4096b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_MBIST_Field
#define h_MBIST_Field (){}

    #define     RA_MBIST_Field_MBIST                           0x0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_MBIST_Field {
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_MBIST[128];
    ///////////////////////////////////////////////////////////
    } SIE_MBIST_Field;

    ///////////////////////////////////////////////////////////
     SIGN32 MBIST_Field_drvrd(SIE_MBIST_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 MBIST_Field_drvwr(SIE_MBIST_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void MBIST_Field_reset(SIE_MBIST_Field *p);
     SIGN32 MBIST_Field_cmp  (SIE_MBIST_Field *p, SIE_MBIST_Field *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define MBIST_Field_check(p,pie,pfx,hLOG) MBIST_Field_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define MBIST_Field_print(p,    pfx,hLOG) MBIST_Field_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: MBIST_Field
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE Irdeto_Field                             (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 CSUK1                (RW-)
///     # 0x00000 CSUK1                
///               $OneReg              CSUK1             REG       [4]
///     @ 0x00010 CSUK2                (RW-)
///     # 0x00010 CSUK2                
///               $OneReg              CSUK2             REG       [4]
///     @ 0x00020 CSGK1                (RW-)
///     # 0x00020 CSGK1                
///               $OneReg              CSGK1             REG       [4]
///     @ 0x00030 CSGK2                (RW-)
///     # 0x00030 CSGK2                
///               $OneReg              CSGK2             REG       [4]
///     @ 0x00040 DOK1                 (RW-)
///     # 0x00040 DOK1                 
///               $OneReg              DOK1              REG       [4]
///     @ 0x00050 CSSN                 (RW-)
///     # 0x00050 CSSN                 
///               $OneReg              CSSN              REG       [2]
///     @ 0x00058 CSUK3                (RW-)
///     # 0x00058 CSUK3                
///               $OneReg              CSUK3             REG       [4]
///     @ 0x00068 FIELD1               (RW-)
///     # 0x00068 FIELD1               
///               $OneReg              FIELD1            REG          
///                                    ###
///                                    * [1:0] ACPU_Disable_Clear_CW
///                                    * [3] Irdeto_Enable
///                                    * [5:4] IFCP_DevKey_Disable
///                                    * [7] Irdeto_Disable
///                                    ###
///     @ 0x0006C FIELD2               (RW-)
///     # 0x0006C Field2               
///               $OneReg              Field2            REG          
///                                    ###
///                                    * [1:0] Irdeto_JtagProtectionLevel
///                                    * [5:4] Irdeto_DFT_JtagProtectionLevel
///                                    * [7:6] Irdeto_TEE_JtagProtectionLevel
///                                    * Register is HW one-way only
///                                    ###
///     @ 0x00070 RFU                  (RW-)
///     # 0x00070 RFU                  
///               $OneReg              RFU               REG       [4]
///     @ 0x00080 CSUK1_CHKNUM         (RW-)
///     # 0x00080 CSUK1_CHKNUM         
///               $OneReg              CSUK1_CHKNUM      REG       [2]
///     @ 0x00088 CSUK2_CHKNUM         (RW-)
///     # 0x00088 CSUK2_CHKNUM         
///               $OneReg              CSUK2_CHKNUM      REG       [2]
///     @ 0x00090 CSGK1_CHKNUM         (RW-)
///     # 0x00090 CSGK1_CHKNUM         
///               $OneReg              CSGK1_CHKNUM      REG       [2]
///     @ 0x00098 CSGK2_CHKNUM         (RW-)
///     # 0x00098 CSGK2_CHKNUM         
///               $OneReg              CSGK2_CHKNUM      REG       [2]
///     @ 0x000A0 DOK_CHKNUM           (RW-)
///     # 0x000A0 DOK_CHKNUM           
///               $OneReg              DOK_CHKNUM        REG       [2]
///     @ 0x000A8 CSUK3_CHKNUM         (RW-)
///     # 0x000A8 CSUK3_CHKNUM         
///               $OneReg              CSUK3_CHKNUM      REG       [2]
///     @ 0x000B0 RFU_END              (RW-)
///     # 0x000B0 RFU_END              
///               $OneReg              RFU_END           REG       [4]
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     192B, bits:    1536b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_Irdeto_Field
#define h_Irdeto_Field (){}

    #define     RA_Irdeto_Field_CSUK1                          0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSUK2                          0x0010
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSGK1                          0x0020
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSGK2                          0x0030
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_DOK1                           0x0040
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSSN                           0x0050
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSUK3                          0x0058
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_FIELD1                         0x0068
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_FIELD2                         0x006C
    #define     RA_Irdeto_Field_Field2                         0x006C
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_RFU                            0x0070
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSUK1_CHKNUM                   0x0080
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSUK2_CHKNUM                   0x0088
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSGK1_CHKNUM                   0x0090
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSGK2_CHKNUM                   0x0098
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_DOK_CHKNUM                     0x00A0
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_CSUK3_CHKNUM                   0x00A8
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto_Field_RFU_END                        0x00B0
    ///////////////////////////////////////////////////////////

    typedef struct SIE_Irdeto_Field {
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSUK1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSUK2[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSGK1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSGK2[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_DOK1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSSN[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSUK3[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_FIELD1;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_Field2;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_RFU[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSUK1_CHKNUM[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSUK2_CHKNUM[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSGK1_CHKNUM[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSGK2_CHKNUM[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_DOK_CHKNUM[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_CSUK3_CHKNUM[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_RFU_END[4];
    ///////////////////////////////////////////////////////////
    } SIE_Irdeto_Field;

    ///////////////////////////////////////////////////////////
     SIGN32 Irdeto_Field_drvrd(SIE_Irdeto_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 Irdeto_Field_drvwr(SIE_Irdeto_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void Irdeto_Field_reset(SIE_Irdeto_Field *p);
     SIGN32 Irdeto_Field_cmp  (SIE_Irdeto_Field *p, SIE_Irdeto_Field *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define Irdeto_Field_check(p,pie,pfx,hLOG) Irdeto_Field_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define Irdeto_Field_print(p,    pfx,hLOG) Irdeto_Field_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: Irdeto_Field
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE Irdeto2_Field                            (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 CA_DATA              (P)
///               %unsigned 1  CA_DATA                   
///                                    ###
///                                    * Macro Only
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00004                      (W-)
///     #         # Stuffing bytes...
///               %% 480
///     @ 0x00040 VERSION              (P)
///               %unsigned 1  Irdeto_Version            
///                                    ###
///                                    * Macro Only
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00044                      (W-)
///     #         # Stuffing bytes...
///               %% 480
///     @ 0x00080 PID                  (P)
///               %unsigned 1  PID                       
///                                    ###
///                                    * Macro Only
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00084 TEST_SWITCH          (P)
///               %unsigned 1  TEST_SWITCH               
///                                    ###
///                                    * Macro Only
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00088 RFU                  (P)
///               %unsigned 1  RFU                       
///                                    ###
///                                    * Macro Only
///                                    ###
///               %%        31         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     140B, bits:       5b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_Irdeto2_Field
#define h_Irdeto2_Field (){}

    #define     RA_Irdeto2_Field_CA_DATA                       0x0000

    #define     BA_Irdeto2_Field_CA_DATA_CA_DATA               0x0000
    #define     B16Irdeto2_Field_CA_DATA_CA_DATA               0x0000
    #define   LSb32Irdeto2_Field_CA_DATA_CA_DATA                  0
    #define   LSb16Irdeto2_Field_CA_DATA_CA_DATA                  0
    #define       bIrdeto2_Field_CA_DATA_CA_DATA               1
    #define   MSK32Irdeto2_Field_CA_DATA_CA_DATA                  0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto2_Field_VERSION                       0x0040

    #define     BA_Irdeto2_Field_VERSION_Irdeto_Version        0x0040
    #define     B16Irdeto2_Field_VERSION_Irdeto_Version        0x0040
    #define   LSb32Irdeto2_Field_VERSION_Irdeto_Version           0
    #define   LSb16Irdeto2_Field_VERSION_Irdeto_Version           0
    #define       bIrdeto2_Field_VERSION_Irdeto_Version        1
    #define   MSK32Irdeto2_Field_VERSION_Irdeto_Version           0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto2_Field_PID                           0x0080

    #define     BA_Irdeto2_Field_PID_PID                       0x0080
    #define     B16Irdeto2_Field_PID_PID                       0x0080
    #define   LSb32Irdeto2_Field_PID_PID                          0
    #define   LSb16Irdeto2_Field_PID_PID                          0
    #define       bIrdeto2_Field_PID_PID                       1
    #define   MSK32Irdeto2_Field_PID_PID                          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto2_Field_TEST_SWITCH                   0x0084

    #define     BA_Irdeto2_Field_TEST_SWITCH_TEST_SWITCH       0x0084
    #define     B16Irdeto2_Field_TEST_SWITCH_TEST_SWITCH       0x0084
    #define   LSb32Irdeto2_Field_TEST_SWITCH_TEST_SWITCH          0
    #define   LSb16Irdeto2_Field_TEST_SWITCH_TEST_SWITCH          0
    #define       bIrdeto2_Field_TEST_SWITCH_TEST_SWITCH       1
    #define   MSK32Irdeto2_Field_TEST_SWITCH_TEST_SWITCH          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Irdeto2_Field_RFU                           0x0088

    #define     BA_Irdeto2_Field_RFU_RFU                       0x0088
    #define     B16Irdeto2_Field_RFU_RFU                       0x0088
    #define   LSb32Irdeto2_Field_RFU_RFU                          0
    #define   LSb16Irdeto2_Field_RFU_RFU                          0
    #define       bIrdeto2_Field_RFU_RFU                       1
    #define   MSK32Irdeto2_Field_RFU_RFU                          0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_Irdeto2_Field {
    ///////////////////////////////////////////////////////////
    #define   GET32Irdeto2_Field_CA_DATA_CA_DATA(r32)          _BFGET_(r32, 0, 0)
    #define   SET32Irdeto2_Field_CA_DATA_CA_DATA(r32,v)        _BFSET_(r32, 0, 0,v)
    #define   GET16Irdeto2_Field_CA_DATA_CA_DATA(r16)          _BFGET_(r16, 0, 0)
    #define   SET16Irdeto2_Field_CA_DATA_CA_DATA(r16,v)        _BFSET_(r16, 0, 0,v)

    #define     w32Irdeto2_Field_CA_DATA                       {\
            UNSG32 uCA_DATA_CA_DATA                            :  1;\
            UNSG32 RSVDx0_b1                                   : 31;\
          }
    union { UNSG32 u32Irdeto2_Field_CA_DATA;
            struct w32Irdeto2_Field_CA_DATA;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx4                                      [60];
    ///////////////////////////////////////////////////////////
    #define   GET32Irdeto2_Field_VERSION_Irdeto_Version(r32)   _BFGET_(r32, 0, 0)
    #define   SET32Irdeto2_Field_VERSION_Irdeto_Version(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16Irdeto2_Field_VERSION_Irdeto_Version(r16)   _BFGET_(r16, 0, 0)
    #define   SET16Irdeto2_Field_VERSION_Irdeto_Version(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32Irdeto2_Field_VERSION                       {\
            UNSG32 uVERSION_Irdeto_Version                     :  1;\
            UNSG32 RSVDx40_b1                                  : 31;\
          }
    union { UNSG32 u32Irdeto2_Field_VERSION;
            struct w32Irdeto2_Field_VERSION;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx44                                     [60];
    ///////////////////////////////////////////////////////////
    #define   GET32Irdeto2_Field_PID_PID(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Irdeto2_Field_PID_PID(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Irdeto2_Field_PID_PID(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Irdeto2_Field_PID_PID(r16,v)                _BFSET_(r16, 0, 0,v)

    #define     w32Irdeto2_Field_PID                           {\
            UNSG32 uPID_PID                                    :  1;\
            UNSG32 RSVDx80_b1                                  : 31;\
          }
    union { UNSG32 u32Irdeto2_Field_PID;
            struct w32Irdeto2_Field_PID;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Irdeto2_Field_TEST_SWITCH_TEST_SWITCH(r32)  _BFGET_(r32, 0, 0)
    #define   SET32Irdeto2_Field_TEST_SWITCH_TEST_SWITCH(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16Irdeto2_Field_TEST_SWITCH_TEST_SWITCH(r16)  _BFGET_(r16, 0, 0)
    #define   SET16Irdeto2_Field_TEST_SWITCH_TEST_SWITCH(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32Irdeto2_Field_TEST_SWITCH                   {\
            UNSG32 uTEST_SWITCH_TEST_SWITCH                    :  1;\
            UNSG32 RSVDx84_b1                                  : 31;\
          }
    union { UNSG32 u32Irdeto2_Field_TEST_SWITCH;
            struct w32Irdeto2_Field_TEST_SWITCH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Irdeto2_Field_RFU_RFU(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Irdeto2_Field_RFU_RFU(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Irdeto2_Field_RFU_RFU(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Irdeto2_Field_RFU_RFU(r16,v)                _BFSET_(r16, 0, 0,v)

    #define     w32Irdeto2_Field_RFU                           {\
            UNSG32 uRFU_RFU                                    :  1;\
            UNSG32 RSVDx88_b1                                  : 31;\
          }
    union { UNSG32 u32Irdeto2_Field_RFU;
            struct w32Irdeto2_Field_RFU;
          };
    ///////////////////////////////////////////////////////////
    } SIE_Irdeto2_Field;

    typedef union  T32Irdeto2_Field_CA_DATA
          { UNSG32 u32;
            struct w32Irdeto2_Field_CA_DATA;
                 } T32Irdeto2_Field_CA_DATA;
    typedef union  T32Irdeto2_Field_VERSION
          { UNSG32 u32;
            struct w32Irdeto2_Field_VERSION;
                 } T32Irdeto2_Field_VERSION;
    typedef union  T32Irdeto2_Field_PID
          { UNSG32 u32;
            struct w32Irdeto2_Field_PID;
                 } T32Irdeto2_Field_PID;
    typedef union  T32Irdeto2_Field_TEST_SWITCH
          { UNSG32 u32;
            struct w32Irdeto2_Field_TEST_SWITCH;
                 } T32Irdeto2_Field_TEST_SWITCH;
    typedef union  T32Irdeto2_Field_RFU
          { UNSG32 u32;
            struct w32Irdeto2_Field_RFU;
                 } T32Irdeto2_Field_RFU;
    ///////////////////////////////////////////////////////////

    typedef union  TIrdeto2_Field_CA_DATA
          { UNSG32 u32[1];
            struct {
            struct w32Irdeto2_Field_CA_DATA;
                   };
                 } TIrdeto2_Field_CA_DATA;
    typedef union  TIrdeto2_Field_VERSION
          { UNSG32 u32[1];
            struct {
            struct w32Irdeto2_Field_VERSION;
                   };
                 } TIrdeto2_Field_VERSION;
    typedef union  TIrdeto2_Field_PID
          { UNSG32 u32[1];
            struct {
            struct w32Irdeto2_Field_PID;
                   };
                 } TIrdeto2_Field_PID;
    typedef union  TIrdeto2_Field_TEST_SWITCH
          { UNSG32 u32[1];
            struct {
            struct w32Irdeto2_Field_TEST_SWITCH;
                   };
                 } TIrdeto2_Field_TEST_SWITCH;
    typedef union  TIrdeto2_Field_RFU
          { UNSG32 u32[1];
            struct {
            struct w32Irdeto2_Field_RFU;
                   };
                 } TIrdeto2_Field_RFU;

    ///////////////////////////////////////////////////////////
     SIGN32 Irdeto2_Field_drvrd(SIE_Irdeto2_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 Irdeto2_Field_drvwr(SIE_Irdeto2_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void Irdeto2_Field_reset(SIE_Irdeto2_Field *p);
     SIGN32 Irdeto2_Field_cmp  (SIE_Irdeto2_Field *p, SIE_Irdeto2_Field *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define Irdeto2_Field_check(p,pie,pfx,hLOG) Irdeto2_Field_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define Irdeto2_Field_print(p,    pfx,hLOG) Irdeto2_Field_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: Irdeto2_Field
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE GKL_Field                                (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 V1_Key_0             (RW-)
///     # 0x00000 V1_Key_0             
///               $OneReg              V1_Key_0          REG       [4]
///     @ 0x00010 V1_Key_1             (RW-)
///     # 0x00010 V1_Key_1             
///               $OneReg              V1_Key_1          REG       [4]
///     @ 0x00020 V1_Key_2             (RW-)
///     # 0x00020 V1_Key_2             
///               $OneReg              V1_Key_2          REG       [4]
///     @ 0x00030 V1_Key_3             (RW-)
///     # 0x00030 V1_Key_3             
///               $OneReg              V1_Key_3          REG       [4]
///     @ 0x00040 V1_DOBK              (RW-)
///     # 0x00040 V1_DOBK              
///               $OneReg              V1_DOBK           REG       [4]
///     @ 0x00050 V1_PADDING_0         (RW-)
///     # 0x00050 V1_PADDING_0         
///               $OneReg              V1_PADDING_0      REG       [3]
///     @ 0x0005C V1_PADDING_VID       (RW-)
///               %unsigned 8  V1_PADDING_0              0x0
///               %unsigned 8  V1_PADDING_1              0x0
///               %unsigned 8  V1_PADDING_2              0x0
///               %unsigned 8  V1_VID_OTP                0x0
///     @ 0x00060 V1_KEY_DISABLE       (RW-)
///     # 0x00060 V1_KEY_DISABLE       
///               $OneReg              V1_KEY_DISABLE    REG       [2]
///     @ 0x00068 V1_RESERVED0         (RW-)
///     # 0x00068 V1_RESERVED0         
///               $OneReg              V1_RESERVED0      REG          
///     @ 0x0006C V1_VID_OTP           (RW-)
///     # 0x0006C V1_VID_OTP           
///               $OneReg              V1_VID_OTP        REG          
///     @ 0x00070 V2_Key_0             (RW-)
///     # 0x00070 V2_Key_0             
///               $OneReg              V2_Key_0          REG       [4]
///     @ 0x00080 V2_Key_1             (RW-)
///     # 0x00080 V2_Key_1             
///               $OneReg              V2_Key_1          REG       [4]
///     @ 0x00090 V2_Key_2             (RW-)
///     # 0x00090 V2_Key_2             
///               $OneReg              V2_Key_2          REG       [4]
///     @ 0x000A0 V2_Key_3             (RW-)
///     # 0x000A0 V2_Key_3             
///               $OneReg              V2_Key_3          REG       [4]
///     @ 0x000B0 V2_DOBK              (RW-)
///     # 0x000B0 V2_DOBK              
///               $OneReg              V2_DOBK           REG       [4]
///     @ 0x000C0 V2_PADDING_0         (RW-)
///     # 0x000C0 V2_PADDING_0         
///               $OneReg              V2_PADDING_0      REG       [3]
///     @ 0x000CC V2_PADDING_VID       (RW-)
///               %unsigned 8  V2_PADDING_0              0x0
///               %unsigned 8  V2_PADDING_1              0x0
///               %unsigned 8  V2_PADDING_2              0x0
///               %unsigned 8  V2_VID_OTP                0x0
///     @ 0x000D0 V2_KEY_DISABLE       (RW-)
///     # 0x000D0 V2_KEY_DISABLE       
///               $OneReg              V2_KEY_DISABLE    REG       [2]
///     @ 0x000D8 V2_RESERVED          (RW-)
///     # 0x000D8 V2_RESERVED          
///               $OneReg              V2_RESERVED       REG       [2]
///     @ 0x000E0 V3_Key_0             (RW-)
///     # 0x000E0 V3_Key_0             
///               $OneReg              V3_Key_0          REG       [4]
///     @ 0x000F0 V3_Key_1             (RW-)
///     # 0x000F0 V3_Key_1             
///               $OneReg              V3_Key_1          REG       [4]
///     @ 0x00100 V3_Key_2             (RW-)
///     # 0x00100 V3_Key_2             
///               $OneReg              V3_Key_2          REG       [4]
///     @ 0x00110 V3_Key_3             (RW-)
///     # 0x00110 V3_Key_3             
///               $OneReg              V3_Key_3          REG       [4]
///     @ 0x00120 V3_DOBK              (RW-)
///     # 0x00120 V3_DOBK              
///               $OneReg              V3_DOBK           REG       [4]
///     @ 0x00130 V3_PADDING_0         (RW-)
///     # 0x00130 V3_PADDING_0         
///               $OneReg              V3_PADDING_0      REG       [3]
///     @ 0x0013C V3_PADDING_VID       (RW-)
///               %unsigned 8  V3_PADDING_0              0x0
///               %unsigned 8  V3_PADDING_1              0x0
///               %unsigned 8  V3_PADDING_2              0x0
///               %unsigned 8  V3_VID_OTP                0x0
///     @ 0x00140 V3_KEY_DISABLE       (RW-)
///     # 0x00140 V3_KEY_DISABLE       
///               $OneReg              V3_KEY_DISABLE    REG       [2]
///     @ 0x00148 V3_RESERVED          (RW-)
///     # 0x00148 V3_RESERVED          
///               $OneReg              V3_RESERVED       REG       [2]
///     @ 0x00150 V4_Key_0             (RW-)
///     # 0x00150 V4_Key_0             
///               $OneReg              V4_Key_0          REG       [4]
///     @ 0x00160 V4_Key_1             (RW-)
///     # 0x00160 V4_Key_1             
///               $OneReg              V4_Key_1          REG       [4]
///     @ 0x00170 V4_Key_2             (RW-)
///     # 0x00170 V4_Key_2             
///               $OneReg              V4_Key_2          REG       [4]
///     @ 0x00180 V4_Key_3             (RW-)
///     # 0x00180 V4_Key_3             
///               $OneReg              V4_Key_3          REG       [4]
///     @ 0x00190 V4_DOBK              (RW-)
///     # 0x00190 V4_DOBK              
///               $OneReg              V4_DOBK           REG       [4]
///     @ 0x001A0 V4_PADDING_0         (RW-)
///     # 0x001A0 V4_PADDING_0         
///               $OneReg              V4_PADDING_0      REG       [3]
///     @ 0x001AC V4_PADDING_VID       (RW-)
///               %unsigned 8  V4_PADDING_0              0x0
///               %unsigned 8  V4_PADDING_1              0x0
///               %unsigned 8  V4_PADDING_2              0x0
///               %unsigned 8  V4_VID_OTP                0x0
///     @ 0x001B0 V4_KEY_DISABLE       (RW-)
///     # 0x001B0 V4_KEY_DISABLE       
///               $OneReg              V4_KEY_DISABLE    REG       [2]
///     @ 0x001B8 V4_RESERVED          (RW-)
///     # 0x001B8 V4_RESERVED          
///               $OneReg              V4_RESERVED       REG       [2]
///     @ 0x001C0 V1_KEY0_CHECKSUM     (RW-)
///     # 0x001C0 V1_KEY0_CHECKSUM     
///               $OneReg              V1_KEY0_CHECKSUM  REG          
///     @ 0x001C4 V1_KEY1_CHECKSUM     (RW-)
///     # 0x001C4 V1_KEY1_CHECKSUM     
///               $OneReg              V1_KEY1_CHECKSUM  REG          
///     @ 0x001C8 V1_KEY2_CHECKSUM     (RW-)
///     # 0x001C8 V1_KEY2_CHECKSUM     
///               $OneReg              V1_KEY2_CHECKSUM  REG          
///     @ 0x001CC V1_KEY3_CHECKSUM     (RW-)
///     # 0x001CC V1_KEY3_CHECKSUM     
///               $OneReg              V1_KEY3_CHECKSUM  REG          
///     @ 0x001D0 V2_KEY0_CHECKSUM     (RW-)
///     # 0x001D0 V2_KEY0_CHECKSUM     
///               $OneReg              V2_KEY0_CHECKSUM  REG          
///     @ 0x001D4 V2_KEY1_CHECKSUM     (RW-)
///     # 0x001D4 V2_KEY1_CHECKSUM     
///               $OneReg              V2_KEY1_CHECKSUM  REG          
///     @ 0x001D8 V2_KEY2_CHECKSUM     (RW-)
///     # 0x001D8 V2_KEY2_CHECKSUM     
///               $OneReg              V2_KEY2_CHECKSUM  REG          
///     @ 0x001DC V2_KEY3_CHECKSUM     (RW-)
///     # 0x001DC V2_KEY3_CHECKSUM     
///               $OneReg              V2_KEY3_CHECKSUM  REG          
///     @ 0x001E0 V3_KEY0_CHECKSUM     (RW-)
///     # 0x001E0 V3_KEY0_CHECKSUM     
///               $OneReg              V3_KEY0_CHECKSUM  REG          
///     @ 0x001E4 V3_KEY1_CHECKSUM     (RW-)
///     # 0x001E4 V3_KEY1_CHECKSUM     
///               $OneReg              V3_KEY1_CHECKSUM  REG          
///     @ 0x001E8 V3_KEY2_CHECKSUM     (RW-)
///     # 0x001E8 V3_KEY2_CHECKSUM     
///               $OneReg              V3_KEY2_CHECKSUM  REG          
///     @ 0x001EC V3_KEY3_CHECKSUM     (RW-)
///     # 0x001EC V3_KEY3_CHECKSUM     
///               $OneReg              V3_KEY3_CHECKSUM  REG          
///     @ 0x001F0 V4_KEY0_CHECKSUM     (RW-)
///     # 0x001F0 V4_KEY0_CHECKSUM     
///               $OneReg              V4_KEY0_CHECKSUM  REG          
///     @ 0x001F4 V4_KEY1_CHECKSUM     (RW-)
///     # 0x001F4 V4_KEY1_CHECKSUM     
///               $OneReg              V4_KEY1_CHECKSUM  REG          
///     @ 0x001F8 V4_KEY2_CHECKSUM     (RW-)
///     # 0x001F8 V4_KEY2_CHECKSUM     
///               $OneReg              V4_KEY2_CHECKSUM  REG          
///     @ 0x001FC V4_KEY3_CHECKSUM     (RW-)
///     # 0x001FC V4_KEY3_CHECKSUM     
///               $OneReg              V4_KEY3_CHECKSUM  REG          
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     512B, bits:    4096b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_GKL_Field
#define h_GKL_Field (){}

    #define     RA_GKL_Field_V1_Key_0                          0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_Key_1                          0x0010
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_Key_2                          0x0020
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_Key_3                          0x0030
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_DOBK                           0x0040
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_PADDING_0                      0x0050
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_PADDING_VID                    0x005C

    #define     BA_GKL_Field_V1_PADDING_VID_V1_PADDING_0       0x005C
    #define     B16GKL_Field_V1_PADDING_VID_V1_PADDING_0       0x005C
    #define   LSb32GKL_Field_V1_PADDING_VID_V1_PADDING_0          0
    #define   LSb16GKL_Field_V1_PADDING_VID_V1_PADDING_0          0
    #define       bGKL_Field_V1_PADDING_VID_V1_PADDING_0       8
    #define   MSK32GKL_Field_V1_PADDING_VID_V1_PADDING_0          0x000000FF

    #define     BA_GKL_Field_V1_PADDING_VID_V1_PADDING_1       0x005D
    #define     B16GKL_Field_V1_PADDING_VID_V1_PADDING_1       0x005C
    #define   LSb32GKL_Field_V1_PADDING_VID_V1_PADDING_1          8
    #define   LSb16GKL_Field_V1_PADDING_VID_V1_PADDING_1          8
    #define       bGKL_Field_V1_PADDING_VID_V1_PADDING_1       8
    #define   MSK32GKL_Field_V1_PADDING_VID_V1_PADDING_1          0x0000FF00

    #define     BA_GKL_Field_V1_PADDING_VID_V1_PADDING_2       0x005E
    #define     B16GKL_Field_V1_PADDING_VID_V1_PADDING_2       0x005E
    #define   LSb32GKL_Field_V1_PADDING_VID_V1_PADDING_2          16
    #define   LSb16GKL_Field_V1_PADDING_VID_V1_PADDING_2          0
    #define       bGKL_Field_V1_PADDING_VID_V1_PADDING_2       8
    #define   MSK32GKL_Field_V1_PADDING_VID_V1_PADDING_2          0x00FF0000

    #define     BA_GKL_Field_V1_PADDING_VID_V1_VID_OTP         0x005F
    #define     B16GKL_Field_V1_PADDING_VID_V1_VID_OTP         0x005E
    #define   LSb32GKL_Field_V1_PADDING_VID_V1_VID_OTP            24
    #define   LSb16GKL_Field_V1_PADDING_VID_V1_VID_OTP            8
    #define       bGKL_Field_V1_PADDING_VID_V1_VID_OTP         8
    #define   MSK32GKL_Field_V1_PADDING_VID_V1_VID_OTP            0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_KEY_DISABLE                    0x0060
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_RESERVED0                      0x0068
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_VID_OTP                        0x006C
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_Key_0                          0x0070
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_Key_1                          0x0080
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_Key_2                          0x0090
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_Key_3                          0x00A0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_DOBK                           0x00B0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_PADDING_0                      0x00C0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_PADDING_VID                    0x00CC

    #define     BA_GKL_Field_V2_PADDING_VID_V2_PADDING_0       0x00CC
    #define     B16GKL_Field_V2_PADDING_VID_V2_PADDING_0       0x00CC
    #define   LSb32GKL_Field_V2_PADDING_VID_V2_PADDING_0          0
    #define   LSb16GKL_Field_V2_PADDING_VID_V2_PADDING_0          0
    #define       bGKL_Field_V2_PADDING_VID_V2_PADDING_0       8
    #define   MSK32GKL_Field_V2_PADDING_VID_V2_PADDING_0          0x000000FF

    #define     BA_GKL_Field_V2_PADDING_VID_V2_PADDING_1       0x00CD
    #define     B16GKL_Field_V2_PADDING_VID_V2_PADDING_1       0x00CC
    #define   LSb32GKL_Field_V2_PADDING_VID_V2_PADDING_1          8
    #define   LSb16GKL_Field_V2_PADDING_VID_V2_PADDING_1          8
    #define       bGKL_Field_V2_PADDING_VID_V2_PADDING_1       8
    #define   MSK32GKL_Field_V2_PADDING_VID_V2_PADDING_1          0x0000FF00

    #define     BA_GKL_Field_V2_PADDING_VID_V2_PADDING_2       0x00CE
    #define     B16GKL_Field_V2_PADDING_VID_V2_PADDING_2       0x00CE
    #define   LSb32GKL_Field_V2_PADDING_VID_V2_PADDING_2          16
    #define   LSb16GKL_Field_V2_PADDING_VID_V2_PADDING_2          0
    #define       bGKL_Field_V2_PADDING_VID_V2_PADDING_2       8
    #define   MSK32GKL_Field_V2_PADDING_VID_V2_PADDING_2          0x00FF0000

    #define     BA_GKL_Field_V2_PADDING_VID_V2_VID_OTP         0x00CF
    #define     B16GKL_Field_V2_PADDING_VID_V2_VID_OTP         0x00CE
    #define   LSb32GKL_Field_V2_PADDING_VID_V2_VID_OTP            24
    #define   LSb16GKL_Field_V2_PADDING_VID_V2_VID_OTP            8
    #define       bGKL_Field_V2_PADDING_VID_V2_VID_OTP         8
    #define   MSK32GKL_Field_V2_PADDING_VID_V2_VID_OTP            0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_KEY_DISABLE                    0x00D0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_RESERVED                       0x00D8
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_Key_0                          0x00E0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_Key_1                          0x00F0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_Key_2                          0x0100
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_Key_3                          0x0110
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_DOBK                           0x0120
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_PADDING_0                      0x0130
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_PADDING_VID                    0x013C

    #define     BA_GKL_Field_V3_PADDING_VID_V3_PADDING_0       0x013C
    #define     B16GKL_Field_V3_PADDING_VID_V3_PADDING_0       0x013C
    #define   LSb32GKL_Field_V3_PADDING_VID_V3_PADDING_0          0
    #define   LSb16GKL_Field_V3_PADDING_VID_V3_PADDING_0          0
    #define       bGKL_Field_V3_PADDING_VID_V3_PADDING_0       8
    #define   MSK32GKL_Field_V3_PADDING_VID_V3_PADDING_0          0x000000FF

    #define     BA_GKL_Field_V3_PADDING_VID_V3_PADDING_1       0x013D
    #define     B16GKL_Field_V3_PADDING_VID_V3_PADDING_1       0x013C
    #define   LSb32GKL_Field_V3_PADDING_VID_V3_PADDING_1          8
    #define   LSb16GKL_Field_V3_PADDING_VID_V3_PADDING_1          8
    #define       bGKL_Field_V3_PADDING_VID_V3_PADDING_1       8
    #define   MSK32GKL_Field_V3_PADDING_VID_V3_PADDING_1          0x0000FF00

    #define     BA_GKL_Field_V3_PADDING_VID_V3_PADDING_2       0x013E
    #define     B16GKL_Field_V3_PADDING_VID_V3_PADDING_2       0x013E
    #define   LSb32GKL_Field_V3_PADDING_VID_V3_PADDING_2          16
    #define   LSb16GKL_Field_V3_PADDING_VID_V3_PADDING_2          0
    #define       bGKL_Field_V3_PADDING_VID_V3_PADDING_2       8
    #define   MSK32GKL_Field_V3_PADDING_VID_V3_PADDING_2          0x00FF0000

    #define     BA_GKL_Field_V3_PADDING_VID_V3_VID_OTP         0x013F
    #define     B16GKL_Field_V3_PADDING_VID_V3_VID_OTP         0x013E
    #define   LSb32GKL_Field_V3_PADDING_VID_V3_VID_OTP            24
    #define   LSb16GKL_Field_V3_PADDING_VID_V3_VID_OTP            8
    #define       bGKL_Field_V3_PADDING_VID_V3_VID_OTP         8
    #define   MSK32GKL_Field_V3_PADDING_VID_V3_VID_OTP            0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_KEY_DISABLE                    0x0140
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_RESERVED                       0x0148
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_Key_0                          0x0150
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_Key_1                          0x0160
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_Key_2                          0x0170
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_Key_3                          0x0180
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_DOBK                           0x0190
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_PADDING_0                      0x01A0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_PADDING_VID                    0x01AC

    #define     BA_GKL_Field_V4_PADDING_VID_V4_PADDING_0       0x01AC
    #define     B16GKL_Field_V4_PADDING_VID_V4_PADDING_0       0x01AC
    #define   LSb32GKL_Field_V4_PADDING_VID_V4_PADDING_0          0
    #define   LSb16GKL_Field_V4_PADDING_VID_V4_PADDING_0          0
    #define       bGKL_Field_V4_PADDING_VID_V4_PADDING_0       8
    #define   MSK32GKL_Field_V4_PADDING_VID_V4_PADDING_0          0x000000FF

    #define     BA_GKL_Field_V4_PADDING_VID_V4_PADDING_1       0x01AD
    #define     B16GKL_Field_V4_PADDING_VID_V4_PADDING_1       0x01AC
    #define   LSb32GKL_Field_V4_PADDING_VID_V4_PADDING_1          8
    #define   LSb16GKL_Field_V4_PADDING_VID_V4_PADDING_1          8
    #define       bGKL_Field_V4_PADDING_VID_V4_PADDING_1       8
    #define   MSK32GKL_Field_V4_PADDING_VID_V4_PADDING_1          0x0000FF00

    #define     BA_GKL_Field_V4_PADDING_VID_V4_PADDING_2       0x01AE
    #define     B16GKL_Field_V4_PADDING_VID_V4_PADDING_2       0x01AE
    #define   LSb32GKL_Field_V4_PADDING_VID_V4_PADDING_2          16
    #define   LSb16GKL_Field_V4_PADDING_VID_V4_PADDING_2          0
    #define       bGKL_Field_V4_PADDING_VID_V4_PADDING_2       8
    #define   MSK32GKL_Field_V4_PADDING_VID_V4_PADDING_2          0x00FF0000

    #define     BA_GKL_Field_V4_PADDING_VID_V4_VID_OTP         0x01AF
    #define     B16GKL_Field_V4_PADDING_VID_V4_VID_OTP         0x01AE
    #define   LSb32GKL_Field_V4_PADDING_VID_V4_VID_OTP            24
    #define   LSb16GKL_Field_V4_PADDING_VID_V4_VID_OTP            8
    #define       bGKL_Field_V4_PADDING_VID_V4_VID_OTP         8
    #define   MSK32GKL_Field_V4_PADDING_VID_V4_VID_OTP            0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_KEY_DISABLE                    0x01B0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_RESERVED                       0x01B8
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_KEY0_CHECKSUM                  0x01C0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_KEY1_CHECKSUM                  0x01C4
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_KEY2_CHECKSUM                  0x01C8
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V1_KEY3_CHECKSUM                  0x01CC
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_KEY0_CHECKSUM                  0x01D0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_KEY1_CHECKSUM                  0x01D4
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_KEY2_CHECKSUM                  0x01D8
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V2_KEY3_CHECKSUM                  0x01DC
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_KEY0_CHECKSUM                  0x01E0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_KEY1_CHECKSUM                  0x01E4
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_KEY2_CHECKSUM                  0x01E8
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V3_KEY3_CHECKSUM                  0x01EC
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_KEY0_CHECKSUM                  0x01F0
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_KEY1_CHECKSUM                  0x01F4
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_KEY2_CHECKSUM                  0x01F8
    ///////////////////////////////////////////////////////////
    #define     RA_GKL_Field_V4_KEY3_CHECKSUM                  0x01FC
    ///////////////////////////////////////////////////////////

    typedef struct SIE_GKL_Field {
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_Key_0[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_Key_1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_Key_2[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_Key_3[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_DOBK[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_PADDING_0[3];
    ///////////////////////////////////////////////////////////
    #define   GET32GKL_Field_V1_PADDING_VID_V1_PADDING_0(r32)  _BFGET_(r32, 7, 0)
    #define   SET32GKL_Field_V1_PADDING_VID_V1_PADDING_0(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16GKL_Field_V1_PADDING_VID_V1_PADDING_0(r16)  _BFGET_(r16, 7, 0)
    #define   SET16GKL_Field_V1_PADDING_VID_V1_PADDING_0(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32GKL_Field_V1_PADDING_VID_V1_PADDING_1(r32)  _BFGET_(r32,15, 8)
    #define   SET32GKL_Field_V1_PADDING_VID_V1_PADDING_1(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16GKL_Field_V1_PADDING_VID_V1_PADDING_1(r16)  _BFGET_(r16,15, 8)
    #define   SET16GKL_Field_V1_PADDING_VID_V1_PADDING_1(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32GKL_Field_V1_PADDING_VID_V1_PADDING_2(r32)  _BFGET_(r32,23,16)
    #define   SET32GKL_Field_V1_PADDING_VID_V1_PADDING_2(r32,v) _BFSET_(r32,23,16,v)
    #define   GET16GKL_Field_V1_PADDING_VID_V1_PADDING_2(r16)  _BFGET_(r16, 7, 0)
    #define   SET16GKL_Field_V1_PADDING_VID_V1_PADDING_2(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32GKL_Field_V1_PADDING_VID_V1_VID_OTP(r32)    _BFGET_(r32,31,24)
    #define   SET32GKL_Field_V1_PADDING_VID_V1_VID_OTP(r32,v)  _BFSET_(r32,31,24,v)
    #define   GET16GKL_Field_V1_PADDING_VID_V1_VID_OTP(r16)    _BFGET_(r16,15, 8)
    #define   SET16GKL_Field_V1_PADDING_VID_V1_VID_OTP(r16,v)  _BFSET_(r16,15, 8,v)

    #define     w32GKL_Field_V1_PADDING_VID                    {\
            UNSG32 uV1_PADDING_VID_V1_PADDING_0                :  8;\
            UNSG32 uV1_PADDING_VID_V1_PADDING_1                :  8;\
            UNSG32 uV1_PADDING_VID_V1_PADDING_2                :  8;\
            UNSG32 uV1_PADDING_VID_V1_VID_OTP                  :  8;\
          }
    union { UNSG32 u32GKL_Field_V1_PADDING_VID;
            struct w32GKL_Field_V1_PADDING_VID;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_KEY_DISABLE[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_RESERVED0;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_VID_OTP;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_Key_0[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_Key_1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_Key_2[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_Key_3[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_DOBK[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_PADDING_0[3];
    ///////////////////////////////////////////////////////////
    #define   GET32GKL_Field_V2_PADDING_VID_V2_PADDING_0(r32)  _BFGET_(r32, 7, 0)
    #define   SET32GKL_Field_V2_PADDING_VID_V2_PADDING_0(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16GKL_Field_V2_PADDING_VID_V2_PADDING_0(r16)  _BFGET_(r16, 7, 0)
    #define   SET16GKL_Field_V2_PADDING_VID_V2_PADDING_0(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32GKL_Field_V2_PADDING_VID_V2_PADDING_1(r32)  _BFGET_(r32,15, 8)
    #define   SET32GKL_Field_V2_PADDING_VID_V2_PADDING_1(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16GKL_Field_V2_PADDING_VID_V2_PADDING_1(r16)  _BFGET_(r16,15, 8)
    #define   SET16GKL_Field_V2_PADDING_VID_V2_PADDING_1(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32GKL_Field_V2_PADDING_VID_V2_PADDING_2(r32)  _BFGET_(r32,23,16)
    #define   SET32GKL_Field_V2_PADDING_VID_V2_PADDING_2(r32,v) _BFSET_(r32,23,16,v)
    #define   GET16GKL_Field_V2_PADDING_VID_V2_PADDING_2(r16)  _BFGET_(r16, 7, 0)
    #define   SET16GKL_Field_V2_PADDING_VID_V2_PADDING_2(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32GKL_Field_V2_PADDING_VID_V2_VID_OTP(r32)    _BFGET_(r32,31,24)
    #define   SET32GKL_Field_V2_PADDING_VID_V2_VID_OTP(r32,v)  _BFSET_(r32,31,24,v)
    #define   GET16GKL_Field_V2_PADDING_VID_V2_VID_OTP(r16)    _BFGET_(r16,15, 8)
    #define   SET16GKL_Field_V2_PADDING_VID_V2_VID_OTP(r16,v)  _BFSET_(r16,15, 8,v)

    #define     w32GKL_Field_V2_PADDING_VID                    {\
            UNSG32 uV2_PADDING_VID_V2_PADDING_0                :  8;\
            UNSG32 uV2_PADDING_VID_V2_PADDING_1                :  8;\
            UNSG32 uV2_PADDING_VID_V2_PADDING_2                :  8;\
            UNSG32 uV2_PADDING_VID_V2_VID_OTP                  :  8;\
          }
    union { UNSG32 u32GKL_Field_V2_PADDING_VID;
            struct w32GKL_Field_V2_PADDING_VID;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_KEY_DISABLE[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_RESERVED[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_Key_0[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_Key_1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_Key_2[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_Key_3[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_DOBK[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_PADDING_0[3];
    ///////////////////////////////////////////////////////////
    #define   GET32GKL_Field_V3_PADDING_VID_V3_PADDING_0(r32)  _BFGET_(r32, 7, 0)
    #define   SET32GKL_Field_V3_PADDING_VID_V3_PADDING_0(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16GKL_Field_V3_PADDING_VID_V3_PADDING_0(r16)  _BFGET_(r16, 7, 0)
    #define   SET16GKL_Field_V3_PADDING_VID_V3_PADDING_0(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32GKL_Field_V3_PADDING_VID_V3_PADDING_1(r32)  _BFGET_(r32,15, 8)
    #define   SET32GKL_Field_V3_PADDING_VID_V3_PADDING_1(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16GKL_Field_V3_PADDING_VID_V3_PADDING_1(r16)  _BFGET_(r16,15, 8)
    #define   SET16GKL_Field_V3_PADDING_VID_V3_PADDING_1(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32GKL_Field_V3_PADDING_VID_V3_PADDING_2(r32)  _BFGET_(r32,23,16)
    #define   SET32GKL_Field_V3_PADDING_VID_V3_PADDING_2(r32,v) _BFSET_(r32,23,16,v)
    #define   GET16GKL_Field_V3_PADDING_VID_V3_PADDING_2(r16)  _BFGET_(r16, 7, 0)
    #define   SET16GKL_Field_V3_PADDING_VID_V3_PADDING_2(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32GKL_Field_V3_PADDING_VID_V3_VID_OTP(r32)    _BFGET_(r32,31,24)
    #define   SET32GKL_Field_V3_PADDING_VID_V3_VID_OTP(r32,v)  _BFSET_(r32,31,24,v)
    #define   GET16GKL_Field_V3_PADDING_VID_V3_VID_OTP(r16)    _BFGET_(r16,15, 8)
    #define   SET16GKL_Field_V3_PADDING_VID_V3_VID_OTP(r16,v)  _BFSET_(r16,15, 8,v)

    #define     w32GKL_Field_V3_PADDING_VID                    {\
            UNSG32 uV3_PADDING_VID_V3_PADDING_0                :  8;\
            UNSG32 uV3_PADDING_VID_V3_PADDING_1                :  8;\
            UNSG32 uV3_PADDING_VID_V3_PADDING_2                :  8;\
            UNSG32 uV3_PADDING_VID_V3_VID_OTP                  :  8;\
          }
    union { UNSG32 u32GKL_Field_V3_PADDING_VID;
            struct w32GKL_Field_V3_PADDING_VID;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_KEY_DISABLE[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_RESERVED[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_Key_0[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_Key_1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_Key_2[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_Key_3[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_DOBK[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_PADDING_0[3];
    ///////////////////////////////////////////////////////////
    #define   GET32GKL_Field_V4_PADDING_VID_V4_PADDING_0(r32)  _BFGET_(r32, 7, 0)
    #define   SET32GKL_Field_V4_PADDING_VID_V4_PADDING_0(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16GKL_Field_V4_PADDING_VID_V4_PADDING_0(r16)  _BFGET_(r16, 7, 0)
    #define   SET16GKL_Field_V4_PADDING_VID_V4_PADDING_0(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32GKL_Field_V4_PADDING_VID_V4_PADDING_1(r32)  _BFGET_(r32,15, 8)
    #define   SET32GKL_Field_V4_PADDING_VID_V4_PADDING_1(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16GKL_Field_V4_PADDING_VID_V4_PADDING_1(r16)  _BFGET_(r16,15, 8)
    #define   SET16GKL_Field_V4_PADDING_VID_V4_PADDING_1(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32GKL_Field_V4_PADDING_VID_V4_PADDING_2(r32)  _BFGET_(r32,23,16)
    #define   SET32GKL_Field_V4_PADDING_VID_V4_PADDING_2(r32,v) _BFSET_(r32,23,16,v)
    #define   GET16GKL_Field_V4_PADDING_VID_V4_PADDING_2(r16)  _BFGET_(r16, 7, 0)
    #define   SET16GKL_Field_V4_PADDING_VID_V4_PADDING_2(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32GKL_Field_V4_PADDING_VID_V4_VID_OTP(r32)    _BFGET_(r32,31,24)
    #define   SET32GKL_Field_V4_PADDING_VID_V4_VID_OTP(r32,v)  _BFSET_(r32,31,24,v)
    #define   GET16GKL_Field_V4_PADDING_VID_V4_VID_OTP(r16)    _BFGET_(r16,15, 8)
    #define   SET16GKL_Field_V4_PADDING_VID_V4_VID_OTP(r16,v)  _BFSET_(r16,15, 8,v)

    #define     w32GKL_Field_V4_PADDING_VID                    {\
            UNSG32 uV4_PADDING_VID_V4_PADDING_0                :  8;\
            UNSG32 uV4_PADDING_VID_V4_PADDING_1                :  8;\
            UNSG32 uV4_PADDING_VID_V4_PADDING_2                :  8;\
            UNSG32 uV4_PADDING_VID_V4_VID_OTP                  :  8;\
          }
    union { UNSG32 u32GKL_Field_V4_PADDING_VID;
            struct w32GKL_Field_V4_PADDING_VID;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_KEY_DISABLE[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_RESERVED[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_KEY0_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_KEY1_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_KEY2_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V1_KEY3_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_KEY0_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_KEY1_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_KEY2_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V2_KEY3_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_KEY0_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_KEY1_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_KEY2_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V3_KEY3_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_KEY0_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_KEY1_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_KEY2_CHECKSUM;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_V4_KEY3_CHECKSUM;
    ///////////////////////////////////////////////////////////
    } SIE_GKL_Field;

    typedef union  T32GKL_Field_V1_PADDING_VID
          { UNSG32 u32;
            struct w32GKL_Field_V1_PADDING_VID;
                 } T32GKL_Field_V1_PADDING_VID;
    typedef union  T32GKL_Field_V2_PADDING_VID
          { UNSG32 u32;
            struct w32GKL_Field_V2_PADDING_VID;
                 } T32GKL_Field_V2_PADDING_VID;
    typedef union  T32GKL_Field_V3_PADDING_VID
          { UNSG32 u32;
            struct w32GKL_Field_V3_PADDING_VID;
                 } T32GKL_Field_V3_PADDING_VID;
    typedef union  T32GKL_Field_V4_PADDING_VID
          { UNSG32 u32;
            struct w32GKL_Field_V4_PADDING_VID;
                 } T32GKL_Field_V4_PADDING_VID;
    ///////////////////////////////////////////////////////////

    typedef union  TGKL_Field_V1_PADDING_VID
          { UNSG32 u32[1];
            struct {
            struct w32GKL_Field_V1_PADDING_VID;
                   };
                 } TGKL_Field_V1_PADDING_VID;
    typedef union  TGKL_Field_V2_PADDING_VID
          { UNSG32 u32[1];
            struct {
            struct w32GKL_Field_V2_PADDING_VID;
                   };
                 } TGKL_Field_V2_PADDING_VID;
    typedef union  TGKL_Field_V3_PADDING_VID
          { UNSG32 u32[1];
            struct {
            struct w32GKL_Field_V3_PADDING_VID;
                   };
                 } TGKL_Field_V3_PADDING_VID;
    typedef union  TGKL_Field_V4_PADDING_VID
          { UNSG32 u32[1];
            struct {
            struct w32GKL_Field_V4_PADDING_VID;
                   };
                 } TGKL_Field_V4_PADDING_VID;

    ///////////////////////////////////////////////////////////
     SIGN32 GKL_Field_drvrd(SIE_GKL_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 GKL_Field_drvwr(SIE_GKL_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void GKL_Field_reset(SIE_GKL_Field *p);
     SIGN32 GKL_Field_cmp  (SIE_GKL_Field *p, SIE_GKL_Field *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define GKL_Field_check(p,pie,pfx,hLOG) GKL_Field_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define GKL_Field_print(p,    pfx,hLOG) GKL_Field_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: GKL_Field
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE NSK_Field                                (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 SecKey1              (RW-)
///     # 0x00000 SecretKey1           
///               $OneReg              SecretKey1        REG       [4]
///     @ 0x00010 SecKey2              (RW-)
///     # 0x00010 SecretKey2           
///               $OneReg              SecretKey2        REG       [4]
///     @ 0x00020 SecKey3              (RW-)
///     # 0x00020 SecretKey3           
///               $OneReg              SecretKey3        REG       [4]
///     @ 0x00030 SecKey4              (RW-)
///     # 0x00030 SecretKey4           
///               $OneReg              SecretKey4        REG       [4]
///     @ 0x00040 SecKey5              (RW-)
///     # 0x00040 SecretKey5           
///               $OneReg              SecretKey5        REG       [4]
///     @ 0x00050 SecKey6              (RW-)
///     # 0x00050 SecretKey6           
///               $OneReg              SecretKey6        REG       [4]
///     @ 0x00060 SecKey7              (RW-)
///     # 0x00060 SecretKey7           
///               $OneReg              SecretKey7        REG       [4]
///     @ 0x00070 SecKey8              (RW-)
///     # 0x00070 SecretKey8           
///               $OneReg              SecretKey8        REG       [4]
///     @ 0x00080 KeyTagA              (RW-)
///               %unsigned 8  KeyTag1                   
///               %unsigned 8  KeyTag2                   
///               %unsigned 8  KeyTag3                   
///               %unsigned 8  KeyTag4                   
///     @ 0x00084 KeyTagB              (RW-)
///               %unsigned 8  KeyTag5                   
///               %unsigned 8  KeyTag6                   
///               %unsigned 8  KeyTag7                   
///               %unsigned 8  KeyTag8                   
///     @ 0x00088 KeyChecksumA         (RW-)
///               %unsigned 8  KeyCheckSum1              
///               %unsigned 8  KeyCheckSum2              
///               %unsigned 8  KeyCheckSum3              
///               %unsigned 8  KeyCheckSum4              
///     @ 0x0008C KeyChecksumB         (RW-)
///               %unsigned 8  KeyCheckSum5              
///               %unsigned 8  KeyCheckSum6              
///               %unsigned 8  KeyCheckSum7              
///               %unsigned 8  KeyCheckSum8              
///     @ 0x00090 KeyLock              (RW-)
///               %unsigned 8  KeyLock12                 
///               %unsigned 8  KeyLock34                 
///               %unsigned 8  KeyLock56                 
///               %unsigned 8  KeyLock78                 
///     @ 0x00094 ACPUValueLock        (RW-)
///               %unsigned 8  ACPUValueLock12           
///               %unsigned 8  ACPUValueLock34           
///               %unsigned 8  SOCLock                   
///               %unsigned 8  RFU3                      
///     @ 0x00098 ConCtrl              (RW-)
///     # 0x00098 ConCtrl              
///               $OneReg              ConCtrl           REG          
///     @ 0x0009C NDS_Misc             (RW-)
///               %unsigned 8  Enables                   
///               %unsigned 8  Csa2_Prohibited           
///               %unsigned 8  Csa3_Perm_Ovrd            
///               %unsigned 8  KeyValid                  
///     @ 0x000A0 PublicOTP            (RW-)
///     # 0x000A0 PublicOTP            
///               $OneReg              PublicOTP         REG       [4]
///     @ 0x000B0 NV_Counter           (RW-)
///     # 0x000B0 NV_Counter           
///               $OneReg              NV_Counter        REG       [8]
///                                    ###
///                                    * HW One-Way
///                                    ###
///     @ 0x000D0 Misc0                (RW-)
///               %unsigned 8  TEE_Protection            
///                                    ###
///                                    * Misc0:
///                                    * [7:0] : TEE_Protection
///                                    * [9:8] : JTAG_Protection
///                                    * [17:16] : BSCAN_Protection
///                                    * [25:24] : STEST_Protection
///                                    * Misc1:
///                                    * [2:0] : FLAGAA
///                                    * [9:8] : SOCVersionSel
///                                    * [7:5] : SOCSecondarySel
///                                    * [16] : NSK_Enable
///                                    * [31:24] : FLAG55
///                                    * All other non-defined bits are RFU
///                                    ###
///               %unsigned 8  JTAG_Protection           0x0
///                                    ###
///                                    * HW One-Way
///                                    ###
///               %unsigned 8  BSCAN_Protection          0x0
///                                    ###
///                                    * HW One-Way
///                                    ###
///               %unsigned 8  STEST_Protection          0x0
///                                    ###
///                                    * HW One-Way
///                                    ###
///     @ 0x000D4 Misc1                (RW-)
///               %unsigned 8  FLAGAA                    0x0
///               %unsigned 8  RFU1                      0x0
///               %unsigned 8  RFU2                      0x0
///               %unsigned 8  FLAG55                    0x0
///     @ 0x000D8 SOCVerSel            (RW-)
///     # 0x000D8 SOCVerSel            
///               $OneReg              SOCVerSel         REG          
///     @ 0x000DC SOCSecSel            (RW-)
///     # 0x000DC SOCSecSel            
///               $OneReg              SOCSecSel         REG          
///     @ 0x000E0 ChipID               (RW-)
///     # 0x000E0 ChipID               
///               $OneReg              ChipID            REG       [2]
///     @ 0x000E8 SecKey1ChkNum        (RW-)
///     # 0x000E8 SecKey1ChkNum        
///               $OneReg              SecKey1ChkNum     REG          
///     @ 0x000EC SecKey2ChkNum        (RW-)
///     # 0x000EC SecKey2ChkNum        
///               $OneReg              SecKey2ChkNum     REG          
///     @ 0x000F0 SecKey3ChkNum        (RW-)
///     # 0x000F0 SecKey3ChkNum        
///               $OneReg              SecKey3ChkNum     REG          
///     @ 0x000F4 SecKey4ChkNum        (RW-)
///     # 0x000F4 SecKey4ChkNum        
///               $OneReg              SecKey4ChkNum     REG          
///     @ 0x000F8 SecKey5ChkNum        (RW-)
///     # 0x000F8 SecKey5ChkNum        
///               $OneReg              SecKey5ChkNum     REG          
///     @ 0x000FC SecKey6ChkNum        (RW-)
///     # 0x000FC SecKey6ChkNum        
///               $OneReg              SecKey6ChkNum     REG          
///     @ 0x00100 SecKey7ChkNum        (RW-)
///     # 0x00100 SecKey7ChkNum        
///               $OneReg              SecKey7ChkNum     REG          
///     @ 0x00104 SecKey8ChkNum        (RW-)
///     # 0x00104 SecKey8ChkNum        
///               $OneReg              SecKey8ChkNum     REG          
///     @ 0x00108 ACPU_RW_VALUE1       (RW-)
///     # 0x00108 ACPU_RW_VALUE1       
///               $OneReg              ACPU_RW_VALUE1    REG       [4]
///     @ 0x00118 ACPU_RW_VALUE2       (RW-)
///     # 0x00118 ACPU_RW_VALUE2       
///               $OneReg              ACPU_RW_VALUE2    REG       [4]
///     @ 0x00128 ACPU_RW_VALUE3       (RW-)
///     # 0x00128 ACPU_RW_VALUE3       
///               $OneReg              ACPU_RW_VALUE3    REG       [4]
///     @ 0x00138 ACPU_RW_VALUE4       (RW-)
///     # 0x00138 ACPU_RW_VALUE4       
///               $OneReg              ACPU_RW_VALUE4    REG       [4]
///     @ 0x00148 MonCtrl0             (RW-)
///               %unsigned 8  Enable                    0x0
///               %unsigned 8  RFU1                      0x0
///               %unsigned 8  VtgMonTune0               0x0
///               %unsigned 8  VtgMonTune1               0x0
///     @ 0x0014C MonCtrl1             (RW-)
///               %unsigned 8  PadMonTune0               0x0
///               %unsigned 8  PadMonTune1               0x0
///               %unsigned 8  FClkMonTune0              0x0
///               %unsigned 8  FClkMonTune1              0x0
///     @ 0x00150 MonCtrl2             (RW-)
///               %unsigned 8  SClkMonTune0              0x0
///               %unsigned 8  SClkMonTune1              0x0
///               %unsigned 8  FClkMonAdjust0            0x0
///               %unsigned 8  FClkMonAdjust1            0x0
///     @ 0x00154 MonCtrl3             (RW-)
///               %unsigned 8  SClkMonAdjust0            0x0
///               %unsigned 8  SClkMonAdjust1            0x0
///               %unsigned 8  ClkMonPeriod0             0x0
///               %unsigned 8  ClkMonPeriod1             0x0
///     @ 0x00158 RFU                  (P)
///               %unsigned 1  RFU                       0x0
///                                    ###
///                                    * Macro only
///                                    ###
///               %%        31         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     348B, bits:    2753b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_NSK_Field
#define h_NSK_Field (){}

    #define     RA_NSK_Field_SecKey1                           0x0000
    #define     RA_NSK_Field_SecretKey1                        0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey2                           0x0010
    #define     RA_NSK_Field_SecretKey2                        0x0010
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey3                           0x0020
    #define     RA_NSK_Field_SecretKey3                        0x0020
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey4                           0x0030
    #define     RA_NSK_Field_SecretKey4                        0x0030
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey5                           0x0040
    #define     RA_NSK_Field_SecretKey5                        0x0040
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey6                           0x0050
    #define     RA_NSK_Field_SecretKey6                        0x0050
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey7                           0x0060
    #define     RA_NSK_Field_SecretKey7                        0x0060
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey8                           0x0070
    #define     RA_NSK_Field_SecretKey8                        0x0070
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_KeyTagA                           0x0080

    #define     BA_NSK_Field_KeyTagA_KeyTag1                   0x0080
    #define     B16NSK_Field_KeyTagA_KeyTag1                   0x0080
    #define   LSb32NSK_Field_KeyTagA_KeyTag1                      0
    #define   LSb16NSK_Field_KeyTagA_KeyTag1                      0
    #define       bNSK_Field_KeyTagA_KeyTag1                   8
    #define   MSK32NSK_Field_KeyTagA_KeyTag1                      0x000000FF

    #define     BA_NSK_Field_KeyTagA_KeyTag2                   0x0081
    #define     B16NSK_Field_KeyTagA_KeyTag2                   0x0080
    #define   LSb32NSK_Field_KeyTagA_KeyTag2                      8
    #define   LSb16NSK_Field_KeyTagA_KeyTag2                      8
    #define       bNSK_Field_KeyTagA_KeyTag2                   8
    #define   MSK32NSK_Field_KeyTagA_KeyTag2                      0x0000FF00

    #define     BA_NSK_Field_KeyTagA_KeyTag3                   0x0082
    #define     B16NSK_Field_KeyTagA_KeyTag3                   0x0082
    #define   LSb32NSK_Field_KeyTagA_KeyTag3                      16
    #define   LSb16NSK_Field_KeyTagA_KeyTag3                      0
    #define       bNSK_Field_KeyTagA_KeyTag3                   8
    #define   MSK32NSK_Field_KeyTagA_KeyTag3                      0x00FF0000

    #define     BA_NSK_Field_KeyTagA_KeyTag4                   0x0083
    #define     B16NSK_Field_KeyTagA_KeyTag4                   0x0082
    #define   LSb32NSK_Field_KeyTagA_KeyTag4                      24
    #define   LSb16NSK_Field_KeyTagA_KeyTag4                      8
    #define       bNSK_Field_KeyTagA_KeyTag4                   8
    #define   MSK32NSK_Field_KeyTagA_KeyTag4                      0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_KeyTagB                           0x0084

    #define     BA_NSK_Field_KeyTagB_KeyTag5                   0x0084
    #define     B16NSK_Field_KeyTagB_KeyTag5                   0x0084
    #define   LSb32NSK_Field_KeyTagB_KeyTag5                      0
    #define   LSb16NSK_Field_KeyTagB_KeyTag5                      0
    #define       bNSK_Field_KeyTagB_KeyTag5                   8
    #define   MSK32NSK_Field_KeyTagB_KeyTag5                      0x000000FF

    #define     BA_NSK_Field_KeyTagB_KeyTag6                   0x0085
    #define     B16NSK_Field_KeyTagB_KeyTag6                   0x0084
    #define   LSb32NSK_Field_KeyTagB_KeyTag6                      8
    #define   LSb16NSK_Field_KeyTagB_KeyTag6                      8
    #define       bNSK_Field_KeyTagB_KeyTag6                   8
    #define   MSK32NSK_Field_KeyTagB_KeyTag6                      0x0000FF00

    #define     BA_NSK_Field_KeyTagB_KeyTag7                   0x0086
    #define     B16NSK_Field_KeyTagB_KeyTag7                   0x0086
    #define   LSb32NSK_Field_KeyTagB_KeyTag7                      16
    #define   LSb16NSK_Field_KeyTagB_KeyTag7                      0
    #define       bNSK_Field_KeyTagB_KeyTag7                   8
    #define   MSK32NSK_Field_KeyTagB_KeyTag7                      0x00FF0000

    #define     BA_NSK_Field_KeyTagB_KeyTag8                   0x0087
    #define     B16NSK_Field_KeyTagB_KeyTag8                   0x0086
    #define   LSb32NSK_Field_KeyTagB_KeyTag8                      24
    #define   LSb16NSK_Field_KeyTagB_KeyTag8                      8
    #define       bNSK_Field_KeyTagB_KeyTag8                   8
    #define   MSK32NSK_Field_KeyTagB_KeyTag8                      0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_KeyChecksumA                      0x0088

    #define     BA_NSK_Field_KeyChecksumA_KeyCheckSum1         0x0088
    #define     B16NSK_Field_KeyChecksumA_KeyCheckSum1         0x0088
    #define   LSb32NSK_Field_KeyChecksumA_KeyCheckSum1            0
    #define   LSb16NSK_Field_KeyChecksumA_KeyCheckSum1            0
    #define       bNSK_Field_KeyChecksumA_KeyCheckSum1         8
    #define   MSK32NSK_Field_KeyChecksumA_KeyCheckSum1            0x000000FF

    #define     BA_NSK_Field_KeyChecksumA_KeyCheckSum2         0x0089
    #define     B16NSK_Field_KeyChecksumA_KeyCheckSum2         0x0088
    #define   LSb32NSK_Field_KeyChecksumA_KeyCheckSum2            8
    #define   LSb16NSK_Field_KeyChecksumA_KeyCheckSum2            8
    #define       bNSK_Field_KeyChecksumA_KeyCheckSum2         8
    #define   MSK32NSK_Field_KeyChecksumA_KeyCheckSum2            0x0000FF00

    #define     BA_NSK_Field_KeyChecksumA_KeyCheckSum3         0x008A
    #define     B16NSK_Field_KeyChecksumA_KeyCheckSum3         0x008A
    #define   LSb32NSK_Field_KeyChecksumA_KeyCheckSum3            16
    #define   LSb16NSK_Field_KeyChecksumA_KeyCheckSum3            0
    #define       bNSK_Field_KeyChecksumA_KeyCheckSum3         8
    #define   MSK32NSK_Field_KeyChecksumA_KeyCheckSum3            0x00FF0000

    #define     BA_NSK_Field_KeyChecksumA_KeyCheckSum4         0x008B
    #define     B16NSK_Field_KeyChecksumA_KeyCheckSum4         0x008A
    #define   LSb32NSK_Field_KeyChecksumA_KeyCheckSum4            24
    #define   LSb16NSK_Field_KeyChecksumA_KeyCheckSum4            8
    #define       bNSK_Field_KeyChecksumA_KeyCheckSum4         8
    #define   MSK32NSK_Field_KeyChecksumA_KeyCheckSum4            0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_KeyChecksumB                      0x008C

    #define     BA_NSK_Field_KeyChecksumB_KeyCheckSum5         0x008C
    #define     B16NSK_Field_KeyChecksumB_KeyCheckSum5         0x008C
    #define   LSb32NSK_Field_KeyChecksumB_KeyCheckSum5            0
    #define   LSb16NSK_Field_KeyChecksumB_KeyCheckSum5            0
    #define       bNSK_Field_KeyChecksumB_KeyCheckSum5         8
    #define   MSK32NSK_Field_KeyChecksumB_KeyCheckSum5            0x000000FF

    #define     BA_NSK_Field_KeyChecksumB_KeyCheckSum6         0x008D
    #define     B16NSK_Field_KeyChecksumB_KeyCheckSum6         0x008C
    #define   LSb32NSK_Field_KeyChecksumB_KeyCheckSum6            8
    #define   LSb16NSK_Field_KeyChecksumB_KeyCheckSum6            8
    #define       bNSK_Field_KeyChecksumB_KeyCheckSum6         8
    #define   MSK32NSK_Field_KeyChecksumB_KeyCheckSum6            0x0000FF00

    #define     BA_NSK_Field_KeyChecksumB_KeyCheckSum7         0x008E
    #define     B16NSK_Field_KeyChecksumB_KeyCheckSum7         0x008E
    #define   LSb32NSK_Field_KeyChecksumB_KeyCheckSum7            16
    #define   LSb16NSK_Field_KeyChecksumB_KeyCheckSum7            0
    #define       bNSK_Field_KeyChecksumB_KeyCheckSum7         8
    #define   MSK32NSK_Field_KeyChecksumB_KeyCheckSum7            0x00FF0000

    #define     BA_NSK_Field_KeyChecksumB_KeyCheckSum8         0x008F
    #define     B16NSK_Field_KeyChecksumB_KeyCheckSum8         0x008E
    #define   LSb32NSK_Field_KeyChecksumB_KeyCheckSum8            24
    #define   LSb16NSK_Field_KeyChecksumB_KeyCheckSum8            8
    #define       bNSK_Field_KeyChecksumB_KeyCheckSum8         8
    #define   MSK32NSK_Field_KeyChecksumB_KeyCheckSum8            0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_KeyLock                           0x0090

    #define     BA_NSK_Field_KeyLock_KeyLock12                 0x0090
    #define     B16NSK_Field_KeyLock_KeyLock12                 0x0090
    #define   LSb32NSK_Field_KeyLock_KeyLock12                    0
    #define   LSb16NSK_Field_KeyLock_KeyLock12                    0
    #define       bNSK_Field_KeyLock_KeyLock12                 8
    #define   MSK32NSK_Field_KeyLock_KeyLock12                    0x000000FF

    #define     BA_NSK_Field_KeyLock_KeyLock34                 0x0091
    #define     B16NSK_Field_KeyLock_KeyLock34                 0x0090
    #define   LSb32NSK_Field_KeyLock_KeyLock34                    8
    #define   LSb16NSK_Field_KeyLock_KeyLock34                    8
    #define       bNSK_Field_KeyLock_KeyLock34                 8
    #define   MSK32NSK_Field_KeyLock_KeyLock34                    0x0000FF00

    #define     BA_NSK_Field_KeyLock_KeyLock56                 0x0092
    #define     B16NSK_Field_KeyLock_KeyLock56                 0x0092
    #define   LSb32NSK_Field_KeyLock_KeyLock56                    16
    #define   LSb16NSK_Field_KeyLock_KeyLock56                    0
    #define       bNSK_Field_KeyLock_KeyLock56                 8
    #define   MSK32NSK_Field_KeyLock_KeyLock56                    0x00FF0000

    #define     BA_NSK_Field_KeyLock_KeyLock78                 0x0093
    #define     B16NSK_Field_KeyLock_KeyLock78                 0x0092
    #define   LSb32NSK_Field_KeyLock_KeyLock78                    24
    #define   LSb16NSK_Field_KeyLock_KeyLock78                    8
    #define       bNSK_Field_KeyLock_KeyLock78                 8
    #define   MSK32NSK_Field_KeyLock_KeyLock78                    0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_ACPUValueLock                     0x0094

    #define     BA_NSK_Field_ACPUValueLock_ACPUValueLock12     0x0094
    #define     B16NSK_Field_ACPUValueLock_ACPUValueLock12     0x0094
    #define   LSb32NSK_Field_ACPUValueLock_ACPUValueLock12        0
    #define   LSb16NSK_Field_ACPUValueLock_ACPUValueLock12        0
    #define       bNSK_Field_ACPUValueLock_ACPUValueLock12     8
    #define   MSK32NSK_Field_ACPUValueLock_ACPUValueLock12        0x000000FF

    #define     BA_NSK_Field_ACPUValueLock_ACPUValueLock34     0x0095
    #define     B16NSK_Field_ACPUValueLock_ACPUValueLock34     0x0094
    #define   LSb32NSK_Field_ACPUValueLock_ACPUValueLock34        8
    #define   LSb16NSK_Field_ACPUValueLock_ACPUValueLock34        8
    #define       bNSK_Field_ACPUValueLock_ACPUValueLock34     8
    #define   MSK32NSK_Field_ACPUValueLock_ACPUValueLock34        0x0000FF00

    #define     BA_NSK_Field_ACPUValueLock_SOCLock             0x0096
    #define     B16NSK_Field_ACPUValueLock_SOCLock             0x0096
    #define   LSb32NSK_Field_ACPUValueLock_SOCLock                16
    #define   LSb16NSK_Field_ACPUValueLock_SOCLock                0
    #define       bNSK_Field_ACPUValueLock_SOCLock             8
    #define   MSK32NSK_Field_ACPUValueLock_SOCLock                0x00FF0000

    #define     BA_NSK_Field_ACPUValueLock_RFU3                0x0097
    #define     B16NSK_Field_ACPUValueLock_RFU3                0x0096
    #define   LSb32NSK_Field_ACPUValueLock_RFU3                   24
    #define   LSb16NSK_Field_ACPUValueLock_RFU3                   8
    #define       bNSK_Field_ACPUValueLock_RFU3                8
    #define   MSK32NSK_Field_ACPUValueLock_RFU3                   0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_ConCtrl                           0x0098
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_NDS_Misc                          0x009C

    #define     BA_NSK_Field_NDS_Misc_Enables                  0x009C
    #define     B16NSK_Field_NDS_Misc_Enables                  0x009C
    #define   LSb32NSK_Field_NDS_Misc_Enables                     0
    #define   LSb16NSK_Field_NDS_Misc_Enables                     0
    #define       bNSK_Field_NDS_Misc_Enables                  8
    #define   MSK32NSK_Field_NDS_Misc_Enables                     0x000000FF

    #define     BA_NSK_Field_NDS_Misc_Csa2_Prohibited          0x009D
    #define     B16NSK_Field_NDS_Misc_Csa2_Prohibited          0x009C
    #define   LSb32NSK_Field_NDS_Misc_Csa2_Prohibited             8
    #define   LSb16NSK_Field_NDS_Misc_Csa2_Prohibited             8
    #define       bNSK_Field_NDS_Misc_Csa2_Prohibited          8
    #define   MSK32NSK_Field_NDS_Misc_Csa2_Prohibited             0x0000FF00

    #define     BA_NSK_Field_NDS_Misc_Csa3_Perm_Ovrd           0x009E
    #define     B16NSK_Field_NDS_Misc_Csa3_Perm_Ovrd           0x009E
    #define   LSb32NSK_Field_NDS_Misc_Csa3_Perm_Ovrd              16
    #define   LSb16NSK_Field_NDS_Misc_Csa3_Perm_Ovrd              0
    #define       bNSK_Field_NDS_Misc_Csa3_Perm_Ovrd           8
    #define   MSK32NSK_Field_NDS_Misc_Csa3_Perm_Ovrd              0x00FF0000

    #define     BA_NSK_Field_NDS_Misc_KeyValid                 0x009F
    #define     B16NSK_Field_NDS_Misc_KeyValid                 0x009E
    #define   LSb32NSK_Field_NDS_Misc_KeyValid                    24
    #define   LSb16NSK_Field_NDS_Misc_KeyValid                    8
    #define       bNSK_Field_NDS_Misc_KeyValid                 8
    #define   MSK32NSK_Field_NDS_Misc_KeyValid                    0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_PublicOTP                         0x00A0
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_NV_Counter                        0x00B0
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_Misc0                             0x00D0

    #define     BA_NSK_Field_Misc0_TEE_Protection              0x00D0
    #define     B16NSK_Field_Misc0_TEE_Protection              0x00D0
    #define   LSb32NSK_Field_Misc0_TEE_Protection                 0
    #define   LSb16NSK_Field_Misc0_TEE_Protection                 0
    #define       bNSK_Field_Misc0_TEE_Protection              8
    #define   MSK32NSK_Field_Misc0_TEE_Protection                 0x000000FF

    #define     BA_NSK_Field_Misc0_JTAG_Protection             0x00D1
    #define     B16NSK_Field_Misc0_JTAG_Protection             0x00D0
    #define   LSb32NSK_Field_Misc0_JTAG_Protection                8
    #define   LSb16NSK_Field_Misc0_JTAG_Protection                8
    #define       bNSK_Field_Misc0_JTAG_Protection             8
    #define   MSK32NSK_Field_Misc0_JTAG_Protection                0x0000FF00

    #define     BA_NSK_Field_Misc0_BSCAN_Protection            0x00D2
    #define     B16NSK_Field_Misc0_BSCAN_Protection            0x00D2
    #define   LSb32NSK_Field_Misc0_BSCAN_Protection               16
    #define   LSb16NSK_Field_Misc0_BSCAN_Protection               0
    #define       bNSK_Field_Misc0_BSCAN_Protection            8
    #define   MSK32NSK_Field_Misc0_BSCAN_Protection               0x00FF0000

    #define     BA_NSK_Field_Misc0_STEST_Protection            0x00D3
    #define     B16NSK_Field_Misc0_STEST_Protection            0x00D2
    #define   LSb32NSK_Field_Misc0_STEST_Protection               24
    #define   LSb16NSK_Field_Misc0_STEST_Protection               8
    #define       bNSK_Field_Misc0_STEST_Protection            8
    #define   MSK32NSK_Field_Misc0_STEST_Protection               0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_Misc1                             0x00D4

    #define     BA_NSK_Field_Misc1_FLAGAA                      0x00D4
    #define     B16NSK_Field_Misc1_FLAGAA                      0x00D4
    #define   LSb32NSK_Field_Misc1_FLAGAA                         0
    #define   LSb16NSK_Field_Misc1_FLAGAA                         0
    #define       bNSK_Field_Misc1_FLAGAA                      8
    #define   MSK32NSK_Field_Misc1_FLAGAA                         0x000000FF

    #define     BA_NSK_Field_Misc1_RFU1                        0x00D5
    #define     B16NSK_Field_Misc1_RFU1                        0x00D4
    #define   LSb32NSK_Field_Misc1_RFU1                           8
    #define   LSb16NSK_Field_Misc1_RFU1                           8
    #define       bNSK_Field_Misc1_RFU1                        8
    #define   MSK32NSK_Field_Misc1_RFU1                           0x0000FF00

    #define     BA_NSK_Field_Misc1_RFU2                        0x00D6
    #define     B16NSK_Field_Misc1_RFU2                        0x00D6
    #define   LSb32NSK_Field_Misc1_RFU2                           16
    #define   LSb16NSK_Field_Misc1_RFU2                           0
    #define       bNSK_Field_Misc1_RFU2                        8
    #define   MSK32NSK_Field_Misc1_RFU2                           0x00FF0000

    #define     BA_NSK_Field_Misc1_FLAG55                      0x00D7
    #define     B16NSK_Field_Misc1_FLAG55                      0x00D6
    #define   LSb32NSK_Field_Misc1_FLAG55                         24
    #define   LSb16NSK_Field_Misc1_FLAG55                         8
    #define       bNSK_Field_Misc1_FLAG55                      8
    #define   MSK32NSK_Field_Misc1_FLAG55                         0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SOCVerSel                         0x00D8
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SOCSecSel                         0x00DC
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_ChipID                            0x00E0
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey1ChkNum                     0x00E8
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey2ChkNum                     0x00EC
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey3ChkNum                     0x00F0
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey4ChkNum                     0x00F4
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey5ChkNum                     0x00F8
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey6ChkNum                     0x00FC
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey7ChkNum                     0x0100
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_SecKey8ChkNum                     0x0104
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_ACPU_RW_VALUE1                    0x0108
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_ACPU_RW_VALUE2                    0x0118
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_ACPU_RW_VALUE3                    0x0128
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_ACPU_RW_VALUE4                    0x0138
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_MonCtrl0                          0x0148

    #define     BA_NSK_Field_MonCtrl0_Enable                   0x0148
    #define     B16NSK_Field_MonCtrl0_Enable                   0x0148
    #define   LSb32NSK_Field_MonCtrl0_Enable                      0
    #define   LSb16NSK_Field_MonCtrl0_Enable                      0
    #define       bNSK_Field_MonCtrl0_Enable                   8
    #define   MSK32NSK_Field_MonCtrl0_Enable                      0x000000FF

    #define     BA_NSK_Field_MonCtrl0_RFU1                     0x0149
    #define     B16NSK_Field_MonCtrl0_RFU1                     0x0148
    #define   LSb32NSK_Field_MonCtrl0_RFU1                        8
    #define   LSb16NSK_Field_MonCtrl0_RFU1                        8
    #define       bNSK_Field_MonCtrl0_RFU1                     8
    #define   MSK32NSK_Field_MonCtrl0_RFU1                        0x0000FF00

    #define     BA_NSK_Field_MonCtrl0_VtgMonTune0              0x014A
    #define     B16NSK_Field_MonCtrl0_VtgMonTune0              0x014A
    #define   LSb32NSK_Field_MonCtrl0_VtgMonTune0                 16
    #define   LSb16NSK_Field_MonCtrl0_VtgMonTune0                 0
    #define       bNSK_Field_MonCtrl0_VtgMonTune0              8
    #define   MSK32NSK_Field_MonCtrl0_VtgMonTune0                 0x00FF0000

    #define     BA_NSK_Field_MonCtrl0_VtgMonTune1              0x014B
    #define     B16NSK_Field_MonCtrl0_VtgMonTune1              0x014A
    #define   LSb32NSK_Field_MonCtrl0_VtgMonTune1                 24
    #define   LSb16NSK_Field_MonCtrl0_VtgMonTune1                 8
    #define       bNSK_Field_MonCtrl0_VtgMonTune1              8
    #define   MSK32NSK_Field_MonCtrl0_VtgMonTune1                 0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_MonCtrl1                          0x014C

    #define     BA_NSK_Field_MonCtrl1_PadMonTune0              0x014C
    #define     B16NSK_Field_MonCtrl1_PadMonTune0              0x014C
    #define   LSb32NSK_Field_MonCtrl1_PadMonTune0                 0
    #define   LSb16NSK_Field_MonCtrl1_PadMonTune0                 0
    #define       bNSK_Field_MonCtrl1_PadMonTune0              8
    #define   MSK32NSK_Field_MonCtrl1_PadMonTune0                 0x000000FF

    #define     BA_NSK_Field_MonCtrl1_PadMonTune1              0x014D
    #define     B16NSK_Field_MonCtrl1_PadMonTune1              0x014C
    #define   LSb32NSK_Field_MonCtrl1_PadMonTune1                 8
    #define   LSb16NSK_Field_MonCtrl1_PadMonTune1                 8
    #define       bNSK_Field_MonCtrl1_PadMonTune1              8
    #define   MSK32NSK_Field_MonCtrl1_PadMonTune1                 0x0000FF00

    #define     BA_NSK_Field_MonCtrl1_FClkMonTune0             0x014E
    #define     B16NSK_Field_MonCtrl1_FClkMonTune0             0x014E
    #define   LSb32NSK_Field_MonCtrl1_FClkMonTune0                16
    #define   LSb16NSK_Field_MonCtrl1_FClkMonTune0                0
    #define       bNSK_Field_MonCtrl1_FClkMonTune0             8
    #define   MSK32NSK_Field_MonCtrl1_FClkMonTune0                0x00FF0000

    #define     BA_NSK_Field_MonCtrl1_FClkMonTune1             0x014F
    #define     B16NSK_Field_MonCtrl1_FClkMonTune1             0x014E
    #define   LSb32NSK_Field_MonCtrl1_FClkMonTune1                24
    #define   LSb16NSK_Field_MonCtrl1_FClkMonTune1                8
    #define       bNSK_Field_MonCtrl1_FClkMonTune1             8
    #define   MSK32NSK_Field_MonCtrl1_FClkMonTune1                0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_MonCtrl2                          0x0150

    #define     BA_NSK_Field_MonCtrl2_SClkMonTune0             0x0150
    #define     B16NSK_Field_MonCtrl2_SClkMonTune0             0x0150
    #define   LSb32NSK_Field_MonCtrl2_SClkMonTune0                0
    #define   LSb16NSK_Field_MonCtrl2_SClkMonTune0                0
    #define       bNSK_Field_MonCtrl2_SClkMonTune0             8
    #define   MSK32NSK_Field_MonCtrl2_SClkMonTune0                0x000000FF

    #define     BA_NSK_Field_MonCtrl2_SClkMonTune1             0x0151
    #define     B16NSK_Field_MonCtrl2_SClkMonTune1             0x0150
    #define   LSb32NSK_Field_MonCtrl2_SClkMonTune1                8
    #define   LSb16NSK_Field_MonCtrl2_SClkMonTune1                8
    #define       bNSK_Field_MonCtrl2_SClkMonTune1             8
    #define   MSK32NSK_Field_MonCtrl2_SClkMonTune1                0x0000FF00

    #define     BA_NSK_Field_MonCtrl2_FClkMonAdjust0           0x0152
    #define     B16NSK_Field_MonCtrl2_FClkMonAdjust0           0x0152
    #define   LSb32NSK_Field_MonCtrl2_FClkMonAdjust0              16
    #define   LSb16NSK_Field_MonCtrl2_FClkMonAdjust0              0
    #define       bNSK_Field_MonCtrl2_FClkMonAdjust0           8
    #define   MSK32NSK_Field_MonCtrl2_FClkMonAdjust0              0x00FF0000

    #define     BA_NSK_Field_MonCtrl2_FClkMonAdjust1           0x0153
    #define     B16NSK_Field_MonCtrl2_FClkMonAdjust1           0x0152
    #define   LSb32NSK_Field_MonCtrl2_FClkMonAdjust1              24
    #define   LSb16NSK_Field_MonCtrl2_FClkMonAdjust1              8
    #define       bNSK_Field_MonCtrl2_FClkMonAdjust1           8
    #define   MSK32NSK_Field_MonCtrl2_FClkMonAdjust1              0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_MonCtrl3                          0x0154

    #define     BA_NSK_Field_MonCtrl3_SClkMonAdjust0           0x0154
    #define     B16NSK_Field_MonCtrl3_SClkMonAdjust0           0x0154
    #define   LSb32NSK_Field_MonCtrl3_SClkMonAdjust0              0
    #define   LSb16NSK_Field_MonCtrl3_SClkMonAdjust0              0
    #define       bNSK_Field_MonCtrl3_SClkMonAdjust0           8
    #define   MSK32NSK_Field_MonCtrl3_SClkMonAdjust0              0x000000FF

    #define     BA_NSK_Field_MonCtrl3_SClkMonAdjust1           0x0155
    #define     B16NSK_Field_MonCtrl3_SClkMonAdjust1           0x0154
    #define   LSb32NSK_Field_MonCtrl3_SClkMonAdjust1              8
    #define   LSb16NSK_Field_MonCtrl3_SClkMonAdjust1              8
    #define       bNSK_Field_MonCtrl3_SClkMonAdjust1           8
    #define   MSK32NSK_Field_MonCtrl3_SClkMonAdjust1              0x0000FF00

    #define     BA_NSK_Field_MonCtrl3_ClkMonPeriod0            0x0156
    #define     B16NSK_Field_MonCtrl3_ClkMonPeriod0            0x0156
    #define   LSb32NSK_Field_MonCtrl3_ClkMonPeriod0               16
    #define   LSb16NSK_Field_MonCtrl3_ClkMonPeriod0               0
    #define       bNSK_Field_MonCtrl3_ClkMonPeriod0            8
    #define   MSK32NSK_Field_MonCtrl3_ClkMonPeriod0               0x00FF0000

    #define     BA_NSK_Field_MonCtrl3_ClkMonPeriod1            0x0157
    #define     B16NSK_Field_MonCtrl3_ClkMonPeriod1            0x0156
    #define   LSb32NSK_Field_MonCtrl3_ClkMonPeriod1               24
    #define   LSb16NSK_Field_MonCtrl3_ClkMonPeriod1               8
    #define       bNSK_Field_MonCtrl3_ClkMonPeriod1            8
    #define   MSK32NSK_Field_MonCtrl3_ClkMonPeriod1               0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_NSK_Field_RFU                               0x0158

    #define     BA_NSK_Field_RFU_RFU                           0x0158
    #define     B16NSK_Field_RFU_RFU                           0x0158
    #define   LSb32NSK_Field_RFU_RFU                              0
    #define   LSb16NSK_Field_RFU_RFU                              0
    #define       bNSK_Field_RFU_RFU                           1
    #define   MSK32NSK_Field_RFU_RFU                              0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_NSK_Field {
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecretKey1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecretKey2[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecretKey3[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecretKey4[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecretKey5[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecretKey6[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecretKey7[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecretKey8[4];
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_KeyTagA_KeyTag1(r32)              _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_KeyTagA_KeyTag1(r32,v)            _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_KeyTagA_KeyTag1(r16)              _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyTagA_KeyTag1(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyTagA_KeyTag2(r32)              _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_KeyTagA_KeyTag2(r32,v)            _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_KeyTagA_KeyTag2(r16)              _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyTagA_KeyTag2(r16,v)            _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_KeyTagA_KeyTag3(r32)              _BFGET_(r32,23,16)
    #define   SET32NSK_Field_KeyTagA_KeyTag3(r32,v)            _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_KeyTagA_KeyTag3(r16)              _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyTagA_KeyTag3(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyTagA_KeyTag4(r32)              _BFGET_(r32,31,24)
    #define   SET32NSK_Field_KeyTagA_KeyTag4(r32,v)            _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_KeyTagA_KeyTag4(r16)              _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyTagA_KeyTag4(r16,v)            _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_KeyTagA                           {\
            UNSG32 uKeyTagA_KeyTag1                            :  8;\
            UNSG32 uKeyTagA_KeyTag2                            :  8;\
            UNSG32 uKeyTagA_KeyTag3                            :  8;\
            UNSG32 uKeyTagA_KeyTag4                            :  8;\
          }
    union { UNSG32 u32NSK_Field_KeyTagA;
            struct w32NSK_Field_KeyTagA;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_KeyTagB_KeyTag5(r32)              _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_KeyTagB_KeyTag5(r32,v)            _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_KeyTagB_KeyTag5(r16)              _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyTagB_KeyTag5(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyTagB_KeyTag6(r32)              _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_KeyTagB_KeyTag6(r32,v)            _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_KeyTagB_KeyTag6(r16)              _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyTagB_KeyTag6(r16,v)            _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_KeyTagB_KeyTag7(r32)              _BFGET_(r32,23,16)
    #define   SET32NSK_Field_KeyTagB_KeyTag7(r32,v)            _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_KeyTagB_KeyTag7(r16)              _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyTagB_KeyTag7(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyTagB_KeyTag8(r32)              _BFGET_(r32,31,24)
    #define   SET32NSK_Field_KeyTagB_KeyTag8(r32,v)            _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_KeyTagB_KeyTag8(r16)              _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyTagB_KeyTag8(r16,v)            _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_KeyTagB                           {\
            UNSG32 uKeyTagB_KeyTag5                            :  8;\
            UNSG32 uKeyTagB_KeyTag6                            :  8;\
            UNSG32 uKeyTagB_KeyTag7                            :  8;\
            UNSG32 uKeyTagB_KeyTag8                            :  8;\
          }
    union { UNSG32 u32NSK_Field_KeyTagB;
            struct w32NSK_Field_KeyTagB;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_KeyChecksumA_KeyCheckSum1(r32)    _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_KeyChecksumA_KeyCheckSum1(r32,v)  _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_KeyChecksumA_KeyCheckSum1(r16)    _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyChecksumA_KeyCheckSum1(r16,v)  _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyChecksumA_KeyCheckSum2(r32)    _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_KeyChecksumA_KeyCheckSum2(r32,v)  _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_KeyChecksumA_KeyCheckSum2(r16)    _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyChecksumA_KeyCheckSum2(r16,v)  _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_KeyChecksumA_KeyCheckSum3(r32)    _BFGET_(r32,23,16)
    #define   SET32NSK_Field_KeyChecksumA_KeyCheckSum3(r32,v)  _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_KeyChecksumA_KeyCheckSum3(r16)    _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyChecksumA_KeyCheckSum3(r16,v)  _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyChecksumA_KeyCheckSum4(r32)    _BFGET_(r32,31,24)
    #define   SET32NSK_Field_KeyChecksumA_KeyCheckSum4(r32,v)  _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_KeyChecksumA_KeyCheckSum4(r16)    _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyChecksumA_KeyCheckSum4(r16,v)  _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_KeyChecksumA                      {\
            UNSG32 uKeyChecksumA_KeyCheckSum1                  :  8;\
            UNSG32 uKeyChecksumA_KeyCheckSum2                  :  8;\
            UNSG32 uKeyChecksumA_KeyCheckSum3                  :  8;\
            UNSG32 uKeyChecksumA_KeyCheckSum4                  :  8;\
          }
    union { UNSG32 u32NSK_Field_KeyChecksumA;
            struct w32NSK_Field_KeyChecksumA;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_KeyChecksumB_KeyCheckSum5(r32)    _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_KeyChecksumB_KeyCheckSum5(r32,v)  _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_KeyChecksumB_KeyCheckSum5(r16)    _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyChecksumB_KeyCheckSum5(r16,v)  _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyChecksumB_KeyCheckSum6(r32)    _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_KeyChecksumB_KeyCheckSum6(r32,v)  _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_KeyChecksumB_KeyCheckSum6(r16)    _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyChecksumB_KeyCheckSum6(r16,v)  _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_KeyChecksumB_KeyCheckSum7(r32)    _BFGET_(r32,23,16)
    #define   SET32NSK_Field_KeyChecksumB_KeyCheckSum7(r32,v)  _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_KeyChecksumB_KeyCheckSum7(r16)    _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyChecksumB_KeyCheckSum7(r16,v)  _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyChecksumB_KeyCheckSum8(r32)    _BFGET_(r32,31,24)
    #define   SET32NSK_Field_KeyChecksumB_KeyCheckSum8(r32,v)  _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_KeyChecksumB_KeyCheckSum8(r16)    _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyChecksumB_KeyCheckSum8(r16,v)  _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_KeyChecksumB                      {\
            UNSG32 uKeyChecksumB_KeyCheckSum5                  :  8;\
            UNSG32 uKeyChecksumB_KeyCheckSum6                  :  8;\
            UNSG32 uKeyChecksumB_KeyCheckSum7                  :  8;\
            UNSG32 uKeyChecksumB_KeyCheckSum8                  :  8;\
          }
    union { UNSG32 u32NSK_Field_KeyChecksumB;
            struct w32NSK_Field_KeyChecksumB;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_KeyLock_KeyLock12(r32)            _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_KeyLock_KeyLock12(r32,v)          _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_KeyLock_KeyLock12(r16)            _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyLock_KeyLock12(r16,v)          _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyLock_KeyLock34(r32)            _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_KeyLock_KeyLock34(r32,v)          _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_KeyLock_KeyLock34(r16)            _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyLock_KeyLock34(r16,v)          _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_KeyLock_KeyLock56(r32)            _BFGET_(r32,23,16)
    #define   SET32NSK_Field_KeyLock_KeyLock56(r32,v)          _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_KeyLock_KeyLock56(r16)            _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_KeyLock_KeyLock56(r16,v)          _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_KeyLock_KeyLock78(r32)            _BFGET_(r32,31,24)
    #define   SET32NSK_Field_KeyLock_KeyLock78(r32,v)          _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_KeyLock_KeyLock78(r16)            _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_KeyLock_KeyLock78(r16,v)          _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_KeyLock                           {\
            UNSG32 uKeyLock_KeyLock12                          :  8;\
            UNSG32 uKeyLock_KeyLock34                          :  8;\
            UNSG32 uKeyLock_KeyLock56                          :  8;\
            UNSG32 uKeyLock_KeyLock78                          :  8;\
          }
    union { UNSG32 u32NSK_Field_KeyLock;
            struct w32NSK_Field_KeyLock;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_ACPUValueLock_ACPUValueLock12(r32) _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_ACPUValueLock_ACPUValueLock12(r32,v) _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_ACPUValueLock_ACPUValueLock12(r16) _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_ACPUValueLock_ACPUValueLock12(r16,v) _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_ACPUValueLock_ACPUValueLock34(r32) _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_ACPUValueLock_ACPUValueLock34(r32,v) _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_ACPUValueLock_ACPUValueLock34(r16) _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_ACPUValueLock_ACPUValueLock34(r16,v) _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_ACPUValueLock_SOCLock(r32)        _BFGET_(r32,23,16)
    #define   SET32NSK_Field_ACPUValueLock_SOCLock(r32,v)      _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_ACPUValueLock_SOCLock(r16)        _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_ACPUValueLock_SOCLock(r16,v)      _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_ACPUValueLock_RFU3(r32)           _BFGET_(r32,31,24)
    #define   SET32NSK_Field_ACPUValueLock_RFU3(r32,v)         _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_ACPUValueLock_RFU3(r16)           _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_ACPUValueLock_RFU3(r16,v)         _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_ACPUValueLock                     {\
            UNSG32 uACPUValueLock_ACPUValueLock12              :  8;\
            UNSG32 uACPUValueLock_ACPUValueLock34              :  8;\
            UNSG32 uACPUValueLock_SOCLock                      :  8;\
            UNSG32 uACPUValueLock_RFU3                         :  8;\
          }
    union { UNSG32 u32NSK_Field_ACPUValueLock;
            struct w32NSK_Field_ACPUValueLock;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_ConCtrl;
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_NDS_Misc_Enables(r32)             _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_NDS_Misc_Enables(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_NDS_Misc_Enables(r16)             _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_NDS_Misc_Enables(r16,v)           _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_NDS_Misc_Csa2_Prohibited(r32)     _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_NDS_Misc_Csa2_Prohibited(r32,v)   _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_NDS_Misc_Csa2_Prohibited(r16)     _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_NDS_Misc_Csa2_Prohibited(r16,v)   _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_NDS_Misc_Csa3_Perm_Ovrd(r32)      _BFGET_(r32,23,16)
    #define   SET32NSK_Field_NDS_Misc_Csa3_Perm_Ovrd(r32,v)    _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_NDS_Misc_Csa3_Perm_Ovrd(r16)      _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_NDS_Misc_Csa3_Perm_Ovrd(r16,v)    _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_NDS_Misc_KeyValid(r32)            _BFGET_(r32,31,24)
    #define   SET32NSK_Field_NDS_Misc_KeyValid(r32,v)          _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_NDS_Misc_KeyValid(r16)            _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_NDS_Misc_KeyValid(r16,v)          _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_NDS_Misc                          {\
            UNSG32 uNDS_Misc_Enables                           :  8;\
            UNSG32 uNDS_Misc_Csa2_Prohibited                   :  8;\
            UNSG32 uNDS_Misc_Csa3_Perm_Ovrd                    :  8;\
            UNSG32 uNDS_Misc_KeyValid                          :  8;\
          }
    union { UNSG32 u32NSK_Field_NDS_Misc;
            struct w32NSK_Field_NDS_Misc;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_PublicOTP[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_NV_Counter[8];
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_Misc0_TEE_Protection(r32)         _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_Misc0_TEE_Protection(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_Misc0_TEE_Protection(r16)         _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_Misc0_TEE_Protection(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_Misc0_JTAG_Protection(r32)        _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_Misc0_JTAG_Protection(r32,v)      _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_Misc0_JTAG_Protection(r16)        _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_Misc0_JTAG_Protection(r16,v)      _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_Misc0_BSCAN_Protection(r32)       _BFGET_(r32,23,16)
    #define   SET32NSK_Field_Misc0_BSCAN_Protection(r32,v)     _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_Misc0_BSCAN_Protection(r16)       _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_Misc0_BSCAN_Protection(r16,v)     _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_Misc0_STEST_Protection(r32)       _BFGET_(r32,31,24)
    #define   SET32NSK_Field_Misc0_STEST_Protection(r32,v)     _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_Misc0_STEST_Protection(r16)       _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_Misc0_STEST_Protection(r16,v)     _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_Misc0                             {\
            UNSG32 uMisc0_TEE_Protection                       :  8;\
            UNSG32 uMisc0_JTAG_Protection                      :  8;\
            UNSG32 uMisc0_BSCAN_Protection                     :  8;\
            UNSG32 uMisc0_STEST_Protection                     :  8;\
          }
    union { UNSG32 u32NSK_Field_Misc0;
            struct w32NSK_Field_Misc0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_Misc1_FLAGAA(r32)                 _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_Misc1_FLAGAA(r32,v)               _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_Misc1_FLAGAA(r16)                 _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_Misc1_FLAGAA(r16,v)               _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_Misc1_RFU1(r32)                   _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_Misc1_RFU1(r32,v)                 _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_Misc1_RFU1(r16)                   _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_Misc1_RFU1(r16,v)                 _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_Misc1_RFU2(r32)                   _BFGET_(r32,23,16)
    #define   SET32NSK_Field_Misc1_RFU2(r32,v)                 _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_Misc1_RFU2(r16)                   _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_Misc1_RFU2(r16,v)                 _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_Misc1_FLAG55(r32)                 _BFGET_(r32,31,24)
    #define   SET32NSK_Field_Misc1_FLAG55(r32,v)               _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_Misc1_FLAG55(r16)                 _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_Misc1_FLAG55(r16,v)               _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_Misc1                             {\
            UNSG32 uMisc1_FLAGAA                               :  8;\
            UNSG32 uMisc1_RFU1                                 :  8;\
            UNSG32 uMisc1_RFU2                                 :  8;\
            UNSG32 uMisc1_FLAG55                               :  8;\
          }
    union { UNSG32 u32NSK_Field_Misc1;
            struct w32NSK_Field_Misc1;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SOCVerSel;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SOCSecSel;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_ChipID[2];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecKey1ChkNum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecKey2ChkNum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecKey3ChkNum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecKey4ChkNum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecKey5ChkNum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecKey6ChkNum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecKey7ChkNum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SecKey8ChkNum;
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_ACPU_RW_VALUE1[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_ACPU_RW_VALUE2[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_ACPU_RW_VALUE3[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_ACPU_RW_VALUE4[4];
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_MonCtrl0_Enable(r32)              _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_MonCtrl0_Enable(r32,v)            _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_MonCtrl0_Enable(r16)              _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_MonCtrl0_Enable(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_MonCtrl0_RFU1(r32)                _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_MonCtrl0_RFU1(r32,v)              _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_MonCtrl0_RFU1(r16)                _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_MonCtrl0_RFU1(r16,v)              _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_MonCtrl0_VtgMonTune0(r32)         _BFGET_(r32,23,16)
    #define   SET32NSK_Field_MonCtrl0_VtgMonTune0(r32,v)       _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_MonCtrl0_VtgMonTune0(r16)         _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_MonCtrl0_VtgMonTune0(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_MonCtrl0_VtgMonTune1(r32)         _BFGET_(r32,31,24)
    #define   SET32NSK_Field_MonCtrl0_VtgMonTune1(r32,v)       _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_MonCtrl0_VtgMonTune1(r16)         _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_MonCtrl0_VtgMonTune1(r16,v)       _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_MonCtrl0                          {\
            UNSG32 uMonCtrl0_Enable                            :  8;\
            UNSG32 uMonCtrl0_RFU1                              :  8;\
            UNSG32 uMonCtrl0_VtgMonTune0                       :  8;\
            UNSG32 uMonCtrl0_VtgMonTune1                       :  8;\
          }
    union { UNSG32 u32NSK_Field_MonCtrl0;
            struct w32NSK_Field_MonCtrl0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_MonCtrl1_PadMonTune0(r32)         _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_MonCtrl1_PadMonTune0(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_MonCtrl1_PadMonTune0(r16)         _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_MonCtrl1_PadMonTune0(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_MonCtrl1_PadMonTune1(r32)         _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_MonCtrl1_PadMonTune1(r32,v)       _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_MonCtrl1_PadMonTune1(r16)         _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_MonCtrl1_PadMonTune1(r16,v)       _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_MonCtrl1_FClkMonTune0(r32)        _BFGET_(r32,23,16)
    #define   SET32NSK_Field_MonCtrl1_FClkMonTune0(r32,v)      _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_MonCtrl1_FClkMonTune0(r16)        _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_MonCtrl1_FClkMonTune0(r16,v)      _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_MonCtrl1_FClkMonTune1(r32)        _BFGET_(r32,31,24)
    #define   SET32NSK_Field_MonCtrl1_FClkMonTune1(r32,v)      _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_MonCtrl1_FClkMonTune1(r16)        _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_MonCtrl1_FClkMonTune1(r16,v)      _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_MonCtrl1                          {\
            UNSG32 uMonCtrl1_PadMonTune0                       :  8;\
            UNSG32 uMonCtrl1_PadMonTune1                       :  8;\
            UNSG32 uMonCtrl1_FClkMonTune0                      :  8;\
            UNSG32 uMonCtrl1_FClkMonTune1                      :  8;\
          }
    union { UNSG32 u32NSK_Field_MonCtrl1;
            struct w32NSK_Field_MonCtrl1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_MonCtrl2_SClkMonTune0(r32)        _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_MonCtrl2_SClkMonTune0(r32,v)      _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_MonCtrl2_SClkMonTune0(r16)        _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_MonCtrl2_SClkMonTune0(r16,v)      _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_MonCtrl2_SClkMonTune1(r32)        _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_MonCtrl2_SClkMonTune1(r32,v)      _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_MonCtrl2_SClkMonTune1(r16)        _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_MonCtrl2_SClkMonTune1(r16,v)      _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_MonCtrl2_FClkMonAdjust0(r32)      _BFGET_(r32,23,16)
    #define   SET32NSK_Field_MonCtrl2_FClkMonAdjust0(r32,v)    _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_MonCtrl2_FClkMonAdjust0(r16)      _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_MonCtrl2_FClkMonAdjust0(r16,v)    _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_MonCtrl2_FClkMonAdjust1(r32)      _BFGET_(r32,31,24)
    #define   SET32NSK_Field_MonCtrl2_FClkMonAdjust1(r32,v)    _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_MonCtrl2_FClkMonAdjust1(r16)      _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_MonCtrl2_FClkMonAdjust1(r16,v)    _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_MonCtrl2                          {\
            UNSG32 uMonCtrl2_SClkMonTune0                      :  8;\
            UNSG32 uMonCtrl2_SClkMonTune1                      :  8;\
            UNSG32 uMonCtrl2_FClkMonAdjust0                    :  8;\
            UNSG32 uMonCtrl2_FClkMonAdjust1                    :  8;\
          }
    union { UNSG32 u32NSK_Field_MonCtrl2;
            struct w32NSK_Field_MonCtrl2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_MonCtrl3_SClkMonAdjust0(r32)      _BFGET_(r32, 7, 0)
    #define   SET32NSK_Field_MonCtrl3_SClkMonAdjust0(r32,v)    _BFSET_(r32, 7, 0,v)
    #define   GET16NSK_Field_MonCtrl3_SClkMonAdjust0(r16)      _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_MonCtrl3_SClkMonAdjust0(r16,v)    _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_MonCtrl3_SClkMonAdjust1(r32)      _BFGET_(r32,15, 8)
    #define   SET32NSK_Field_MonCtrl3_SClkMonAdjust1(r32,v)    _BFSET_(r32,15, 8,v)
    #define   GET16NSK_Field_MonCtrl3_SClkMonAdjust1(r16)      _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_MonCtrl3_SClkMonAdjust1(r16,v)    _BFSET_(r16,15, 8,v)

    #define   GET32NSK_Field_MonCtrl3_ClkMonPeriod0(r32)       _BFGET_(r32,23,16)
    #define   SET32NSK_Field_MonCtrl3_ClkMonPeriod0(r32,v)     _BFSET_(r32,23,16,v)
    #define   GET16NSK_Field_MonCtrl3_ClkMonPeriod0(r16)       _BFGET_(r16, 7, 0)
    #define   SET16NSK_Field_MonCtrl3_ClkMonPeriod0(r16,v)     _BFSET_(r16, 7, 0,v)

    #define   GET32NSK_Field_MonCtrl3_ClkMonPeriod1(r32)       _BFGET_(r32,31,24)
    #define   SET32NSK_Field_MonCtrl3_ClkMonPeriod1(r32,v)     _BFSET_(r32,31,24,v)
    #define   GET16NSK_Field_MonCtrl3_ClkMonPeriod1(r16)       _BFGET_(r16,15, 8)
    #define   SET16NSK_Field_MonCtrl3_ClkMonPeriod1(r16,v)     _BFSET_(r16,15, 8,v)

    #define     w32NSK_Field_MonCtrl3                          {\
            UNSG32 uMonCtrl3_SClkMonAdjust0                    :  8;\
            UNSG32 uMonCtrl3_SClkMonAdjust1                    :  8;\
            UNSG32 uMonCtrl3_ClkMonPeriod0                     :  8;\
            UNSG32 uMonCtrl3_ClkMonPeriod1                     :  8;\
          }
    union { UNSG32 u32NSK_Field_MonCtrl3;
            struct w32NSK_Field_MonCtrl3;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32NSK_Field_RFU_RFU(r32)                      _BFGET_(r32, 0, 0)
    #define   SET32NSK_Field_RFU_RFU(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   GET16NSK_Field_RFU_RFU(r16)                      _BFGET_(r16, 0, 0)
    #define   SET16NSK_Field_RFU_RFU(r16,v)                    _BFSET_(r16, 0, 0,v)

    #define     w32NSK_Field_RFU                               {\
            UNSG32 uRFU_RFU                                    :  1;\
            UNSG32 RSVDx158_b1                                 : 31;\
          }
    union { UNSG32 u32NSK_Field_RFU;
            struct w32NSK_Field_RFU;
          };
    ///////////////////////////////////////////////////////////
    } SIE_NSK_Field;

    typedef union  T32NSK_Field_KeyTagA
          { UNSG32 u32;
            struct w32NSK_Field_KeyTagA;
                 } T32NSK_Field_KeyTagA;
    typedef union  T32NSK_Field_KeyTagB
          { UNSG32 u32;
            struct w32NSK_Field_KeyTagB;
                 } T32NSK_Field_KeyTagB;
    typedef union  T32NSK_Field_KeyChecksumA
          { UNSG32 u32;
            struct w32NSK_Field_KeyChecksumA;
                 } T32NSK_Field_KeyChecksumA;
    typedef union  T32NSK_Field_KeyChecksumB
          { UNSG32 u32;
            struct w32NSK_Field_KeyChecksumB;
                 } T32NSK_Field_KeyChecksumB;
    typedef union  T32NSK_Field_KeyLock
          { UNSG32 u32;
            struct w32NSK_Field_KeyLock;
                 } T32NSK_Field_KeyLock;
    typedef union  T32NSK_Field_ACPUValueLock
          { UNSG32 u32;
            struct w32NSK_Field_ACPUValueLock;
                 } T32NSK_Field_ACPUValueLock;
    typedef union  T32NSK_Field_NDS_Misc
          { UNSG32 u32;
            struct w32NSK_Field_NDS_Misc;
                 } T32NSK_Field_NDS_Misc;
    typedef union  T32NSK_Field_Misc0
          { UNSG32 u32;
            struct w32NSK_Field_Misc0;
                 } T32NSK_Field_Misc0;
    typedef union  T32NSK_Field_Misc1
          { UNSG32 u32;
            struct w32NSK_Field_Misc1;
                 } T32NSK_Field_Misc1;
    typedef union  T32NSK_Field_MonCtrl0
          { UNSG32 u32;
            struct w32NSK_Field_MonCtrl0;
                 } T32NSK_Field_MonCtrl0;
    typedef union  T32NSK_Field_MonCtrl1
          { UNSG32 u32;
            struct w32NSK_Field_MonCtrl1;
                 } T32NSK_Field_MonCtrl1;
    typedef union  T32NSK_Field_MonCtrl2
          { UNSG32 u32;
            struct w32NSK_Field_MonCtrl2;
                 } T32NSK_Field_MonCtrl2;
    typedef union  T32NSK_Field_MonCtrl3
          { UNSG32 u32;
            struct w32NSK_Field_MonCtrl3;
                 } T32NSK_Field_MonCtrl3;
    typedef union  T32NSK_Field_RFU
          { UNSG32 u32;
            struct w32NSK_Field_RFU;
                 } T32NSK_Field_RFU;
    ///////////////////////////////////////////////////////////

    typedef union  TNSK_Field_KeyTagA
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_KeyTagA;
                   };
                 } TNSK_Field_KeyTagA;
    typedef union  TNSK_Field_KeyTagB
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_KeyTagB;
                   };
                 } TNSK_Field_KeyTagB;
    typedef union  TNSK_Field_KeyChecksumA
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_KeyChecksumA;
                   };
                 } TNSK_Field_KeyChecksumA;
    typedef union  TNSK_Field_KeyChecksumB
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_KeyChecksumB;
                   };
                 } TNSK_Field_KeyChecksumB;
    typedef union  TNSK_Field_KeyLock
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_KeyLock;
                   };
                 } TNSK_Field_KeyLock;
    typedef union  TNSK_Field_ACPUValueLock
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_ACPUValueLock;
                   };
                 } TNSK_Field_ACPUValueLock;
    typedef union  TNSK_Field_NDS_Misc
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_NDS_Misc;
                   };
                 } TNSK_Field_NDS_Misc;
    typedef union  TNSK_Field_Misc0
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_Misc0;
                   };
                 } TNSK_Field_Misc0;
    typedef union  TNSK_Field_Misc1
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_Misc1;
                   };
                 } TNSK_Field_Misc1;
    typedef union  TNSK_Field_MonCtrl0
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_MonCtrl0;
                   };
                 } TNSK_Field_MonCtrl0;
    typedef union  TNSK_Field_MonCtrl1
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_MonCtrl1;
                   };
                 } TNSK_Field_MonCtrl1;
    typedef union  TNSK_Field_MonCtrl2
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_MonCtrl2;
                   };
                 } TNSK_Field_MonCtrl2;
    typedef union  TNSK_Field_MonCtrl3
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_MonCtrl3;
                   };
                 } TNSK_Field_MonCtrl3;
    typedef union  TNSK_Field_RFU
          { UNSG32 u32[1];
            struct {
            struct w32NSK_Field_RFU;
                   };
                 } TNSK_Field_RFU;

    ///////////////////////////////////////////////////////////
     SIGN32 NSK_Field_drvrd(SIE_NSK_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 NSK_Field_drvwr(SIE_NSK_Field *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void NSK_Field_reset(SIE_NSK_Field *p);
     SIGN32 NSK_Field_cmp  (SIE_NSK_Field *p, SIE_NSK_Field *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define NSK_Field_check(p,pie,pfx,hLOG) NSK_Field_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define NSK_Field_print(p,    pfx,hLOG) NSK_Field_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: NSK_Field
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE KilopassReg                              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 KilopassOTP_Pin      (P)
///     # 0x00000 KilopassOTP_Pin      
///               $KilopassOTP_Pin     KilopassOTP_Pin   REG          
///                                    ###
///                                    * KilopassOTP_pin
///                                    ###
///     @ 0x00044                      (W-)
///     #         # Stuffing bytes...
///               %% 480
///     @ 0x00080 KilopassOTP_Timing   (P)
///     # 0x00080 KilopassOTP_Timing   
///               $KilopassOTP_Timing  KilopassOTP_Timing REG          
///                                    ###
///                                    * KilopassOTP_timing
///                                    ###
///     @ 0x00120                      (W-)
///     #         # Stuffing bytes...
///               %% 768
///     @ 0x00180 CopyOTP              (RW-)
///               %unsigned 1  CopyOTP                   0x0
///                                    ###
///                                    * By writing to address, host CPU sends command to OTP wrapper to start a copy of OTP to shadow register. This is used when a OTP_RdFail interrupt happens. ACPU should expect a CopyDone interrupt after issuing this command.
///                                    * Self_recover
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00184 OTPMask              (P)
///               %unsigned 32 OTPMask                   0x0
///                                    ###
///                                    * OTP mask when issuing program command to OTP.
///                                    ###
///     @ 0x00188 OTP_ErrorAddress     (R-)
///               %unsigned 32 OTP_ErrorAddress          0x0
///                                    ###
///                                    * When CPU sends command to read or program Kilopass OTP, an error might happen. This register Indicates at which OTP address the operation fail happened. Note that is this address matches with Galois memmap.
///                                    * ACPU can access this register on Engineer_enable chip.
///                                    * This register will be disabled in production chip.
///                                    ###
///     @ 0x0018C OTP_ErrorBit         (R-)
///               %unsigned 32 OTP_ErrorBit              0x0
///                                    ###
///                                    * When CPU sends command to read or program Kilopass OTP, an error might happen. This register Indicates at which bit the error happened.
///                                    * 1: error bit
///                                    * 0: normal bit
///                                    * ACPU can access this register on Engineer_enable chip..
///                                    * This register will be disabled in production chip.
///                                    ###
///     @ 0x00190 OtpReady             (P)
///               %unsigned 1  OtpReady                  0x0
///                                    ###
///                                    * Once hardware set ‘ShadowRegCopyDone’, CPU can
///                                    * 1) Directly set this bit to 1 to inform NSK2 that OTP is ready for use;
///                                    * or
///                                    * 2) Start to program OTP field, once all the programming are done, CPU needs to set this bit to 1 to inform NSK2.
///                                    * Firmware needs to make sure that once this bit is set, it must stay in this value until next global reset.
///                                    * ACPU can access this register.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00194 Otp_forcedly_const   (P)
///               %unsigned 32 Otp_forcedly_const        0x4C4B40
///                                    ###
///                                    * Delay is 200ms, clock is 40ns, the count number is 5000000
///                                    * ACPU can access this register.
///                                    ###
///     @ 0x00198 Otp_password         (P)
///     # 0x00198 Otp_password         
///               $OneReg              Otp_password      REG       [4]
///                                    ###
///                                    * User provide password in order to have the access to write to Kilopass OTP.
///                                    * ACPU can access this register.
///                                    ###
///     @ 0x001A8 Otp_password_match   (R-)
///               %unsigned 1  Otp_password_match        0x0
///                                    ###
///                                    * Whether the password user provided is matching
///                                    * ACPU can access this register.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001AC Otp_password_valid   (P)
///               %unsigned 1  Otp_password_valid        0x0
///                                    ###
///                                    * When write to this register, software marks user password as valid so that hardware will start check if the user password for OTP programming is matching. When command finishes, software can read ‘Otp_password_match’ to check the result.
///                                    * ACPU can access this register.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001B0 KilopassOTP_DebugMode (P)
///               %unsigned 1  KilopassOTP_DebugMode     0x0
///                                    ###
///                                    * 1: Debug Mode for KilopassOTP. CPU can directly write and read to KilopassOTP pins. All read and write request from AHB bus will be ignored.
///                                    * 0: Function Mode for KilopassOTP. CPU can issue read and write transaction through AHB bus. All read and write request to KilopassOTP pins will be ignored.
///                                    * ACPU can access this register.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001B4 IntStatus            (WOC-)
///               ###
///               * Writing ones to any of the bits will clear those bits. ACPU can access it
///               ###
///               %unsigned 1  ShadowRegCopyDone         0x0
///                                    ###
///                                    * Hardware finishes the copy from OTP to shadow register
///                                    ###
///               %unsigned 1  ShadowRegCopyFail         0x0
///                                    ###
///                                    * Hardware fails the copy from OTP to shadow register. Error happened during the reading of one of the OTP field
///                                    ###
///               %unsigned 1  OTP_Cp_Fail               0x0
///                                    ###
///                                    * After reset, OTP wrapper will read out all OTP content and copy it to shadow register, an error happened during this copy. ‘OTP_ErrorAddress’ has the information that at which address the error occurs.
///                                    ###
///               %unsigned 1  OTP_Rd_Fail               0x0
///                                    ###
///                                    * CPU sends command to read OTP but an error happened during reading. ‘OTP_ErrorAddress’ has the information that at which address the error occurs. Please ref to OTP spec for more details.
///                                    ###
///               %unsigned 1  OTP_Done                  0x0
///                                    ###
///                                    * OTP wrapper has finished last command. CPU may issue another command now. If CPU issues another command before this interrupt, this command will be ignored.
///                                    ###
///               %unsigned 1  OTP_Wr_RdCheck            0x0
///                                    ###
///                                    * CPU sends command to update OTP. OTP wrapper first read out but this read transaction fails.
///                                    ###
///               %unsigned 1  OTP_Wr_Check              0x0
///                                    ###
///                                    * CPU sends command to update OTP. OTP wrapper first read out the current value and check if the write request is legal.
///                                    ###
///               %unsigned 1  OTP_Wr_Fail               0x0
///                                    ###
///                                    * CPU sends command to update OTP but the program command is failed.
///                                    ###
///               %unsigned 1  OTP_Wr_Rdback             0x0
///                                    ###
///                                    * CPU sends command to update OTP. Program is finished, then hardware will update shadowreg’s corresponding field. Error occurs when hardware is reading the corresponding OTP field.
///                                    ###
///               %unsigned 1  AKL_Access_OTP            0x0
///                                    ###
///                                    * CPU sends command to access OTP while AKL is accessing OTP. AHB command will be ignored. CPU needs to send the same command again
///                                    ###
///               %unsigned 1  OTP_Wr_OneWayCheck        0x0
///                                    ###
///                                    * CPU sends command to write 0 to one-way field.
///                                    ###
///               %unsigned 1  NSK_SDCP_Err              0x0
///                                    ###
///                                    * Sandwich Read in NSK Region has error.
///                                    * Only enabled with ENG_EN
///                                    ###
///               %unsigned 20 Reseved                   0x0
///                                    ###
///                                    * Reserved for future
///                                    ###
///     @ 0x001B8 IntMask              (P)
///               %unsigned 32 IntMask                   0x0
///                                    ###
///                                    * 0: do not mask out interrupt bit
///                                    * 1: mask out interrupt bit, register can be updated by hardware, but interrupt will not be generated.
///                                    * ACPU can access it
///                                    ###
///     @ 0x001BC IntEnable            (P)
///               %unsigned 32 IntEnable                 0xFFFFFFFF
///                                    ###
///                                    * 0: disable interrupt bit, register will not be updated by hardware
///                                    * 1: enable interrupt bit
///                                    ###
///     @ 0x001C0 OTPunProgram         (R-)
///               %unsigned 1  OTPunProgram              0x0
///                                    ###
///                                    * OTPunProgram set by hardware
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001C4 ACPU_Access          (P)
///               %unsigned 1  OTP                       0x1
///                                    ###
///                                    * 1 allow ACPU access OTP
///                                    ###
///               %unsigned 1  Shadow                    0x1
///                                    ###
///                                    * 1 allow ACPU access Shadow
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x001C8 TransError           (R-)
///               ###
///               * Invalid transfer information. When there is a invalid transfer from ACPU/SCPU, it will be logged.
///               ###
///               %unsigned 32 TransError                0x0
///     @ 0x001CC OTP_Read_V0F         (R-)
///               %unsigned 32 V0F                       0x0
///                                    ###
///                                    * Virgin 0 state flag
///                                    * 0: this bit is at virgin state
///                                    * 1: this bit is programmed
///                                    ###
///     @ 0x001D0 Spare_reg            (RW)
///               %unsigned 32 Reg0                      0x0
///     @ 0x001D4 BIST_BlankCheckTest  (RW)
///               %unsigned 1  BIST_BlankCheckTest       0x0
///                                    ###
///                                    * Write 1 to start blank check test.
///                                    * Once command is finished, HW will update it to 0.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001D8 BIST_RBRead          (RW)
///               %unsigned 1  BIST_RBRead               0x0
///                                    ###
///                                    * Write 1 to read the RB0 and RB1 values.
///                                    * Once command is finished, HW will update this bit to 0.
///                                    * The RB0 and RB1 values will be placed in BIST_BLCK_Result register.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001DC BIST_WLBLTestM       (RW)
///               %unsigned 1  BIST_WLBLTestM            0x0
///                                    ###
///                                    * Write 1 to start WLBL test for main bank.
///                                    * This is the TESTDEC0 (tm_testdec0) of the TMDG.
///                                    * Once command is finished, HW will update it to 0.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001E0 BIST_WLBLTestE       (RW)
///               %unsigned 1  BIST_WLBLTestE            0x0
///                                    ###
///                                    * Write 1 to start WLBL test for extra bank.
///                                    * This is the TESTDEC1 (tm_testdec1) of the TMDG.
///                                    * Once command is finished, HW will update it to 0.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001E4 BIST_ProgTest        (RW)
///               %unsigned 1  BIST_ProgTest             0x0
///                                    ###
///                                    * Write 1 to start programming test
///                                    * This is the WRTEST of the TMDG
///                                    * Once command is finished, HW will update it to 0.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001E8 BIST_ProgTestRead    (RW)
///               %unsigned 1  BIST_ProgTestRead         0x0
///                                    ###
///                                    * Write 1 to start read to the spare row used by WRTEST
///                                    * Once command is finished, HW will update it to 0.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x001EC BIST_BlankCheckTest_StAddr (RW)
///               %unsigned 16 BIST_BlankCheckTest_StAddr 0x0
///                                    ###
///                                    * Blank check test start address
///                                    ###
///               %%        16         # Stuffing bits...
///     @ 0x001F0 BIST_BlankCheckTest_EndAddr (RW)
///               %unsigned 16 BIST_BlankCheckTest_EndAddr 0xFFE
///                                    ###
///                                    * Blank check test end address
///                                    ###
///               %%        16         # Stuffing bits...
///     @ 0x001F4 BIST_WLBLTest_StAddr (RW)
///               %unsigned 16 BIST_WLBLTest_StAddr      0x0
///                                    ###
///                                    * WLBL start address
///                                    ###
///               %%        16         # Stuffing bits...
///     @ 0x001F8 BIST_WLBLTest_EndAddr (RW)
///               %unsigned 16 BIST_WLBLTest_EndAddr     0xFFE
///                                    ###
///                                    * WLBL end address
///                                    ###
///               %%        16         # Stuffing bits...
///     @ 0x001FC BIST_ProgTest_Addr   (RW)
///               %unsigned 16 BIST_ProgTest_Addr        0x0
///                                    ###
///                                    * WRTEST Prog address
///                                    * This address is used by both BIST_ProgTest command and BIST_ProgTestRead command for address.
///                                    * In BIST_ProgTestRead command, bits [3:0] are unused are don't cares as each read will read out 16-bits
///                                    ###
///               %unsigned 1  BIST_ProgTest_Bit         0x0
///                                    ###
///                                    * WRTEST Prog bit
///                                    * This bit is only used as the data bit for BIST_ProgTest. It is ignored by BIST_ProgTestRead
///                                    ###
///               %%        15         # Stuffing bits...
///     @ 0x00200 BIST_BLCK_Fail       (R-)
///               %unsigned 1  BIST_BLCK_Fail            0x0
///                                    ###
///                                    * Blank check fail.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00204 BIST_BLCK_Result     (R-)
///               %unsigned 5  BIST_BLCK_Result          0x0
///                                    ###
///                                    * Latched blank check results from previous BLCK command.
///                                    * [0]: Data for RB0
///                                    * [1]: Data for RB1
///                                    * [5:2] REJECT Fail statuses:
///                                    * 000 : Black Check RB0 & RB1 F
///                                    * 001: RB0 Programming (Blank Check 0)
///                                    * 010: RB1 Programming (Blank Check 0)
///                                    * 011: RB0 Programming (Blank Check 1)
///                                    * 100: RB1 Programming (Blank Check 1)
///                                    * 101: RB0 Programming (Blank Check 2)
///                                    * 110: RB1 Programming (Blank Check 2)
///                                    * 111: Blank Check 2 Fail
///                                    * Note:
///                                    * Bits [1:0] are valid only immediately after executing BIST_BlankCheck command and BLCK_Fail = 0.
///                                    * These two bits are also valid after executing BIST_RBRead command.
///                                    * Bits [5:2] is valid only after executing BIST_BlankCheck command and BLCK_Fail = 1
///                                    ###
///               %%        27         # Stuffing bits...
///     @ 0x00208 BIST_WLBLTestM_Fail  (R-)
///               %unsigned 1  BIST_WLBLTestM_Fail       
///                                    ###
///                                    * WLBLTestM Fail
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x0020C BIST_WLBLTestE_Fail  (R-)
///               %unsigned 1  BIST_WLBLTestE_Fail       
///                                    ###
///                                    * WLBLTestE Fail
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00210 BIST_ProgTest_Fail   (R-)
///               %unsigned 1  BIST_ProgTest_Fail        
///                                    ###
///                                    * ProgTest Fail
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00214 BIST_ProgTest_Bit    (R-)
///               %unsigned 16 BIST_ProgTest_Bit         
///                                    ###
///                                    * The value returned by the BIST_ProgTestRead command.
///                                    ###
///               %%        16         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     536B, bits:     951b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_KilopassReg
#define h_KilopassReg (){}

    #define     RA_KilopassReg_KilopassOTP_Pin                 0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_KilopassOTP_Timing              0x0080
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_CopyOTP                         0x0180

    #define     BA_KilopassReg_CopyOTP_CopyOTP                 0x0180
    #define     B16KilopassReg_CopyOTP_CopyOTP                 0x0180
    #define   LSb32KilopassReg_CopyOTP_CopyOTP                    0
    #define   LSb16KilopassReg_CopyOTP_CopyOTP                    0
    #define       bKilopassReg_CopyOTP_CopyOTP                 1
    #define   MSK32KilopassReg_CopyOTP_CopyOTP                    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_OTPMask                         0x0184

    #define     BA_KilopassReg_OTPMask_OTPMask                 0x0184
    #define     B16KilopassReg_OTPMask_OTPMask                 0x0184
    #define   LSb32KilopassReg_OTPMask_OTPMask                    0
    #define   LSb16KilopassReg_OTPMask_OTPMask                    0
    #define       bKilopassReg_OTPMask_OTPMask                 32
    #define   MSK32KilopassReg_OTPMask_OTPMask                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_OTP_ErrorAddress                0x0188

    #define     BA_KilopassReg_OTP_ErrorAddress_OTP_ErrorAddress 0x0188
    #define     B16KilopassReg_OTP_ErrorAddress_OTP_ErrorAddress 0x0188
    #define   LSb32KilopassReg_OTP_ErrorAddress_OTP_ErrorAddress    0
    #define   LSb16KilopassReg_OTP_ErrorAddress_OTP_ErrorAddress    0
    #define       bKilopassReg_OTP_ErrorAddress_OTP_ErrorAddress 32
    #define   MSK32KilopassReg_OTP_ErrorAddress_OTP_ErrorAddress    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_OTP_ErrorBit                    0x018C

    #define     BA_KilopassReg_OTP_ErrorBit_OTP_ErrorBit       0x018C
    #define     B16KilopassReg_OTP_ErrorBit_OTP_ErrorBit       0x018C
    #define   LSb32KilopassReg_OTP_ErrorBit_OTP_ErrorBit          0
    #define   LSb16KilopassReg_OTP_ErrorBit_OTP_ErrorBit          0
    #define       bKilopassReg_OTP_ErrorBit_OTP_ErrorBit       32
    #define   MSK32KilopassReg_OTP_ErrorBit_OTP_ErrorBit          0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_OtpReady                        0x0190

    #define     BA_KilopassReg_OtpReady_OtpReady               0x0190
    #define     B16KilopassReg_OtpReady_OtpReady               0x0190
    #define   LSb32KilopassReg_OtpReady_OtpReady                  0
    #define   LSb16KilopassReg_OtpReady_OtpReady                  0
    #define       bKilopassReg_OtpReady_OtpReady               1
    #define   MSK32KilopassReg_OtpReady_OtpReady                  0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_Otp_forcedly_const              0x0194

    #define     BA_KilopassReg_Otp_forcedly_const_Otp_forcedly_const 0x0194
    #define     B16KilopassReg_Otp_forcedly_const_Otp_forcedly_const 0x0194
    #define   LSb32KilopassReg_Otp_forcedly_const_Otp_forcedly_const    0
    #define   LSb16KilopassReg_Otp_forcedly_const_Otp_forcedly_const    0
    #define       bKilopassReg_Otp_forcedly_const_Otp_forcedly_const 32
    #define   MSK32KilopassReg_Otp_forcedly_const_Otp_forcedly_const    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_Otp_password                    0x0198
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_Otp_password_match              0x01A8

    #define     BA_KilopassReg_Otp_password_match_Otp_password_match 0x01A8
    #define     B16KilopassReg_Otp_password_match_Otp_password_match 0x01A8
    #define   LSb32KilopassReg_Otp_password_match_Otp_password_match    0
    #define   LSb16KilopassReg_Otp_password_match_Otp_password_match    0
    #define       bKilopassReg_Otp_password_match_Otp_password_match 1
    #define   MSK32KilopassReg_Otp_password_match_Otp_password_match    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_Otp_password_valid              0x01AC

    #define     BA_KilopassReg_Otp_password_valid_Otp_password_valid 0x01AC
    #define     B16KilopassReg_Otp_password_valid_Otp_password_valid 0x01AC
    #define   LSb32KilopassReg_Otp_password_valid_Otp_password_valid    0
    #define   LSb16KilopassReg_Otp_password_valid_Otp_password_valid    0
    #define       bKilopassReg_Otp_password_valid_Otp_password_valid 1
    #define   MSK32KilopassReg_Otp_password_valid_Otp_password_valid    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_KilopassOTP_DebugMode           0x01B0

    #define     BA_KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode 0x01B0
    #define     B16KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode 0x01B0
    #define   LSb32KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode    0
    #define   LSb16KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode    0
    #define       bKilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode 1
    #define   MSK32KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_IntStatus                       0x01B4

    #define     BA_KilopassReg_IntStatus_ShadowRegCopyDone     0x01B4
    #define     B16KilopassReg_IntStatus_ShadowRegCopyDone     0x01B4
    #define   LSb32KilopassReg_IntStatus_ShadowRegCopyDone        0
    #define   LSb16KilopassReg_IntStatus_ShadowRegCopyDone        0
    #define       bKilopassReg_IntStatus_ShadowRegCopyDone     1
    #define   MSK32KilopassReg_IntStatus_ShadowRegCopyDone        0x00000001

    #define     BA_KilopassReg_IntStatus_ShadowRegCopyFail     0x01B4
    #define     B16KilopassReg_IntStatus_ShadowRegCopyFail     0x01B4
    #define   LSb32KilopassReg_IntStatus_ShadowRegCopyFail        1
    #define   LSb16KilopassReg_IntStatus_ShadowRegCopyFail        1
    #define       bKilopassReg_IntStatus_ShadowRegCopyFail     1
    #define   MSK32KilopassReg_IntStatus_ShadowRegCopyFail        0x00000002

    #define     BA_KilopassReg_IntStatus_OTP_Cp_Fail           0x01B4
    #define     B16KilopassReg_IntStatus_OTP_Cp_Fail           0x01B4
    #define   LSb32KilopassReg_IntStatus_OTP_Cp_Fail              2
    #define   LSb16KilopassReg_IntStatus_OTP_Cp_Fail              2
    #define       bKilopassReg_IntStatus_OTP_Cp_Fail           1
    #define   MSK32KilopassReg_IntStatus_OTP_Cp_Fail              0x00000004

    #define     BA_KilopassReg_IntStatus_OTP_Rd_Fail           0x01B4
    #define     B16KilopassReg_IntStatus_OTP_Rd_Fail           0x01B4
    #define   LSb32KilopassReg_IntStatus_OTP_Rd_Fail              3
    #define   LSb16KilopassReg_IntStatus_OTP_Rd_Fail              3
    #define       bKilopassReg_IntStatus_OTP_Rd_Fail           1
    #define   MSK32KilopassReg_IntStatus_OTP_Rd_Fail              0x00000008

    #define     BA_KilopassReg_IntStatus_OTP_Done              0x01B4
    #define     B16KilopassReg_IntStatus_OTP_Done              0x01B4
    #define   LSb32KilopassReg_IntStatus_OTP_Done                 4
    #define   LSb16KilopassReg_IntStatus_OTP_Done                 4
    #define       bKilopassReg_IntStatus_OTP_Done              1
    #define   MSK32KilopassReg_IntStatus_OTP_Done                 0x00000010

    #define     BA_KilopassReg_IntStatus_OTP_Wr_RdCheck        0x01B4
    #define     B16KilopassReg_IntStatus_OTP_Wr_RdCheck        0x01B4
    #define   LSb32KilopassReg_IntStatus_OTP_Wr_RdCheck           5
    #define   LSb16KilopassReg_IntStatus_OTP_Wr_RdCheck           5
    #define       bKilopassReg_IntStatus_OTP_Wr_RdCheck        1
    #define   MSK32KilopassReg_IntStatus_OTP_Wr_RdCheck           0x00000020

    #define     BA_KilopassReg_IntStatus_OTP_Wr_Check          0x01B4
    #define     B16KilopassReg_IntStatus_OTP_Wr_Check          0x01B4
    #define   LSb32KilopassReg_IntStatus_OTP_Wr_Check             6
    #define   LSb16KilopassReg_IntStatus_OTP_Wr_Check             6
    #define       bKilopassReg_IntStatus_OTP_Wr_Check          1
    #define   MSK32KilopassReg_IntStatus_OTP_Wr_Check             0x00000040

    #define     BA_KilopassReg_IntStatus_OTP_Wr_Fail           0x01B4
    #define     B16KilopassReg_IntStatus_OTP_Wr_Fail           0x01B4
    #define   LSb32KilopassReg_IntStatus_OTP_Wr_Fail              7
    #define   LSb16KilopassReg_IntStatus_OTP_Wr_Fail              7
    #define       bKilopassReg_IntStatus_OTP_Wr_Fail           1
    #define   MSK32KilopassReg_IntStatus_OTP_Wr_Fail              0x00000080

    #define     BA_KilopassReg_IntStatus_OTP_Wr_Rdback         0x01B5
    #define     B16KilopassReg_IntStatus_OTP_Wr_Rdback         0x01B4
    #define   LSb32KilopassReg_IntStatus_OTP_Wr_Rdback            8
    #define   LSb16KilopassReg_IntStatus_OTP_Wr_Rdback            8
    #define       bKilopassReg_IntStatus_OTP_Wr_Rdback         1
    #define   MSK32KilopassReg_IntStatus_OTP_Wr_Rdback            0x00000100

    #define     BA_KilopassReg_IntStatus_AKL_Access_OTP        0x01B5
    #define     B16KilopassReg_IntStatus_AKL_Access_OTP        0x01B4
    #define   LSb32KilopassReg_IntStatus_AKL_Access_OTP           9
    #define   LSb16KilopassReg_IntStatus_AKL_Access_OTP           9
    #define       bKilopassReg_IntStatus_AKL_Access_OTP        1
    #define   MSK32KilopassReg_IntStatus_AKL_Access_OTP           0x00000200

    #define     BA_KilopassReg_IntStatus_OTP_Wr_OneWayCheck    0x01B5
    #define     B16KilopassReg_IntStatus_OTP_Wr_OneWayCheck    0x01B4
    #define   LSb32KilopassReg_IntStatus_OTP_Wr_OneWayCheck       10
    #define   LSb16KilopassReg_IntStatus_OTP_Wr_OneWayCheck       10
    #define       bKilopassReg_IntStatus_OTP_Wr_OneWayCheck    1
    #define   MSK32KilopassReg_IntStatus_OTP_Wr_OneWayCheck       0x00000400

    #define     BA_KilopassReg_IntStatus_NSK_SDCP_Err          0x01B5
    #define     B16KilopassReg_IntStatus_NSK_SDCP_Err          0x01B4
    #define   LSb32KilopassReg_IntStatus_NSK_SDCP_Err             11
    #define   LSb16KilopassReg_IntStatus_NSK_SDCP_Err             11
    #define       bKilopassReg_IntStatus_NSK_SDCP_Err          1
    #define   MSK32KilopassReg_IntStatus_NSK_SDCP_Err             0x00000800

    #define     BA_KilopassReg_IntStatus_Reseved               0x01B5
    #define     B16KilopassReg_IntStatus_Reseved               0x01B4
    #define   LSb32KilopassReg_IntStatus_Reseved                  12
    #define   LSb16KilopassReg_IntStatus_Reseved                  12
    #define       bKilopassReg_IntStatus_Reseved               20
    #define   MSK32KilopassReg_IntStatus_Reseved                  0xFFFFF000
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_IntMask                         0x01B8

    #define     BA_KilopassReg_IntMask_IntMask                 0x01B8
    #define     B16KilopassReg_IntMask_IntMask                 0x01B8
    #define   LSb32KilopassReg_IntMask_IntMask                    0
    #define   LSb16KilopassReg_IntMask_IntMask                    0
    #define       bKilopassReg_IntMask_IntMask                 32
    #define   MSK32KilopassReg_IntMask_IntMask                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_IntEnable                       0x01BC

    #define     BA_KilopassReg_IntEnable_IntEnable             0x01BC
    #define     B16KilopassReg_IntEnable_IntEnable             0x01BC
    #define   LSb32KilopassReg_IntEnable_IntEnable                0
    #define   LSb16KilopassReg_IntEnable_IntEnable                0
    #define       bKilopassReg_IntEnable_IntEnable             32
    #define   MSK32KilopassReg_IntEnable_IntEnable                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_OTPunProgram                    0x01C0

    #define     BA_KilopassReg_OTPunProgram_OTPunProgram       0x01C0
    #define     B16KilopassReg_OTPunProgram_OTPunProgram       0x01C0
    #define   LSb32KilopassReg_OTPunProgram_OTPunProgram          0
    #define   LSb16KilopassReg_OTPunProgram_OTPunProgram          0
    #define       bKilopassReg_OTPunProgram_OTPunProgram       1
    #define   MSK32KilopassReg_OTPunProgram_OTPunProgram          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_ACPU_Access                     0x01C4

    #define     BA_KilopassReg_ACPU_Access_OTP                 0x01C4
    #define     B16KilopassReg_ACPU_Access_OTP                 0x01C4
    #define   LSb32KilopassReg_ACPU_Access_OTP                    0
    #define   LSb16KilopassReg_ACPU_Access_OTP                    0
    #define       bKilopassReg_ACPU_Access_OTP                 1
    #define   MSK32KilopassReg_ACPU_Access_OTP                    0x00000001

    #define     BA_KilopassReg_ACPU_Access_Shadow              0x01C4
    #define     B16KilopassReg_ACPU_Access_Shadow              0x01C4
    #define   LSb32KilopassReg_ACPU_Access_Shadow                 1
    #define   LSb16KilopassReg_ACPU_Access_Shadow                 1
    #define       bKilopassReg_ACPU_Access_Shadow              1
    #define   MSK32KilopassReg_ACPU_Access_Shadow                 0x00000002
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_TransError                      0x01C8

    #define     BA_KilopassReg_TransError_TransError           0x01C8
    #define     B16KilopassReg_TransError_TransError           0x01C8
    #define   LSb32KilopassReg_TransError_TransError              0
    #define   LSb16KilopassReg_TransError_TransError              0
    #define       bKilopassReg_TransError_TransError           32
    #define   MSK32KilopassReg_TransError_TransError              0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_OTP_Read_V0F                    0x01CC

    #define     BA_KilopassReg_OTP_Read_V0F_V0F                0x01CC
    #define     B16KilopassReg_OTP_Read_V0F_V0F                0x01CC
    #define   LSb32KilopassReg_OTP_Read_V0F_V0F                   0
    #define   LSb16KilopassReg_OTP_Read_V0F_V0F                   0
    #define       bKilopassReg_OTP_Read_V0F_V0F                32
    #define   MSK32KilopassReg_OTP_Read_V0F_V0F                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_Spare_reg                       0x01D0

    #define     BA_KilopassReg_Spare_reg_Reg0                  0x01D0
    #define     B16KilopassReg_Spare_reg_Reg0                  0x01D0
    #define   LSb32KilopassReg_Spare_reg_Reg0                     0
    #define   LSb16KilopassReg_Spare_reg_Reg0                     0
    #define       bKilopassReg_Spare_reg_Reg0                  32
    #define   MSK32KilopassReg_Spare_reg_Reg0                     0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_BlankCheckTest             0x01D4

    #define     BA_KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest 0x01D4
    #define     B16KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest 0x01D4
    #define   LSb32KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest    0
    #define   LSb16KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest    0
    #define       bKilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest 1
    #define   MSK32KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_RBRead                     0x01D8

    #define     BA_KilopassReg_BIST_RBRead_BIST_RBRead         0x01D8
    #define     B16KilopassReg_BIST_RBRead_BIST_RBRead         0x01D8
    #define   LSb32KilopassReg_BIST_RBRead_BIST_RBRead            0
    #define   LSb16KilopassReg_BIST_RBRead_BIST_RBRead            0
    #define       bKilopassReg_BIST_RBRead_BIST_RBRead         1
    #define   MSK32KilopassReg_BIST_RBRead_BIST_RBRead            0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_WLBLTestM                  0x01DC

    #define     BA_KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM   0x01DC
    #define     B16KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM   0x01DC
    #define   LSb32KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM      0
    #define   LSb16KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM      0
    #define       bKilopassReg_BIST_WLBLTestM_BIST_WLBLTestM   1
    #define   MSK32KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM      0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_WLBLTestE                  0x01E0

    #define     BA_KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE   0x01E0
    #define     B16KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE   0x01E0
    #define   LSb32KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE      0
    #define   LSb16KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE      0
    #define       bKilopassReg_BIST_WLBLTestE_BIST_WLBLTestE   1
    #define   MSK32KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE      0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_ProgTest                   0x01E4

    #define     BA_KilopassReg_BIST_ProgTest_BIST_ProgTest     0x01E4
    #define     B16KilopassReg_BIST_ProgTest_BIST_ProgTest     0x01E4
    #define   LSb32KilopassReg_BIST_ProgTest_BIST_ProgTest        0
    #define   LSb16KilopassReg_BIST_ProgTest_BIST_ProgTest        0
    #define       bKilopassReg_BIST_ProgTest_BIST_ProgTest     1
    #define   MSK32KilopassReg_BIST_ProgTest_BIST_ProgTest        0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_ProgTestRead               0x01E8

    #define     BA_KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead 0x01E8
    #define     B16KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead 0x01E8
    #define   LSb32KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead    0
    #define   LSb16KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead    0
    #define       bKilopassReg_BIST_ProgTestRead_BIST_ProgTestRead 1
    #define   MSK32KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_BlankCheckTest_StAddr      0x01EC

    #define     BA_KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr 0x01EC
    #define     B16KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr 0x01EC
    #define   LSb32KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr    0
    #define   LSb16KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr    0
    #define       bKilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr 16
    #define   MSK32KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr    0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_BlankCheckTest_EndAddr     0x01F0

    #define     BA_KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr 0x01F0
    #define     B16KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr 0x01F0
    #define   LSb32KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr    0
    #define   LSb16KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr    0
    #define       bKilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr 16
    #define   MSK32KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr    0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_WLBLTest_StAddr            0x01F4

    #define     BA_KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr 0x01F4
    #define     B16KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr 0x01F4
    #define   LSb32KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr    0
    #define   LSb16KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr    0
    #define       bKilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr 16
    #define   MSK32KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr    0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_WLBLTest_EndAddr           0x01F8

    #define     BA_KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr 0x01F8
    #define     B16KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr 0x01F8
    #define   LSb32KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr    0
    #define   LSb16KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr    0
    #define       bKilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr 16
    #define   MSK32KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr    0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_ProgTest_Addr              0x01FC

    #define     BA_KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr 0x01FC
    #define     B16KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr 0x01FC
    #define   LSb32KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr    0
    #define   LSb16KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr    0
    #define       bKilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr 16
    #define   MSK32KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr    0x0000FFFF

    #define     BA_KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit 0x01FE
    #define     B16KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit 0x01FE
    #define   LSb32KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit    16
    #define   LSb16KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit    0
    #define       bKilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit 1
    #define   MSK32KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit    0x00010000
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_BLCK_Fail                  0x0200

    #define     BA_KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail   0x0200
    #define     B16KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail   0x0200
    #define   LSb32KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail      0
    #define   LSb16KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail      0
    #define       bKilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail   1
    #define   MSK32KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail      0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_BLCK_Result                0x0204

    #define     BA_KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result 0x0204
    #define     B16KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result 0x0204
    #define   LSb32KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result    0
    #define   LSb16KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result    0
    #define       bKilopassReg_BIST_BLCK_Result_BIST_BLCK_Result 5
    #define   MSK32KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result    0x0000001F
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_WLBLTestM_Fail             0x0208

    #define     BA_KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail 0x0208
    #define     B16KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail 0x0208
    #define   LSb32KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail    0
    #define   LSb16KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail    0
    #define       bKilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail 1
    #define   MSK32KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_WLBLTestE_Fail             0x020C

    #define     BA_KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail 0x020C
    #define     B16KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail 0x020C
    #define   LSb32KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail    0
    #define   LSb16KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail    0
    #define       bKilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail 1
    #define   MSK32KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_ProgTest_Fail              0x0210

    #define     BA_KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail 0x0210
    #define     B16KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail 0x0210
    #define   LSb32KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail    0
    #define   LSb16KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail    0
    #define       bKilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail 1
    #define   MSK32KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KilopassReg_BIST_ProgTest_Bit               0x0214

    #define     BA_KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit 0x0214
    #define     B16KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit 0x0214
    #define   LSb32KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit    0
    #define   LSb16KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit    0
    #define       bKilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit 16
    #define   MSK32KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit    0x0000FFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_KilopassReg {
    ///////////////////////////////////////////////////////////
              SIE_KilopassOTP_Pin                              ie_KilopassOTP_Pin;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx44                                     [60];
    ///////////////////////////////////////////////////////////
              SIE_KilopassOTP_Timing                           ie_KilopassOTP_Timing;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx120                                    [96];
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_CopyOTP_CopyOTP(r32)            _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_CopyOTP_CopyOTP(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_CopyOTP_CopyOTP(r16)            _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_CopyOTP_CopyOTP(r16,v)          _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_CopyOTP                         {\
            UNSG32 uCopyOTP_CopyOTP                            :  1;\
            UNSG32 RSVDx180_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_CopyOTP;
            struct w32KilopassReg_CopyOTP;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_OTPMask_OTPMask(r32)            _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_OTPMask_OTPMask(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_OTPMask                         {\
            UNSG32 uOTPMask_OTPMask                            : 32;\
          }
    union { UNSG32 u32KilopassReg_OTPMask;
            struct w32KilopassReg_OTPMask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_OTP_ErrorAddress_OTP_ErrorAddress(r32) _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_OTP_ErrorAddress_OTP_ErrorAddress(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_OTP_ErrorAddress                {\
            UNSG32 uOTP_ErrorAddress_OTP_ErrorAddress          : 32;\
          }
    union { UNSG32 u32KilopassReg_OTP_ErrorAddress;
            struct w32KilopassReg_OTP_ErrorAddress;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_OTP_ErrorBit_OTP_ErrorBit(r32)  _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_OTP_ErrorBit_OTP_ErrorBit(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_OTP_ErrorBit                    {\
            UNSG32 uOTP_ErrorBit_OTP_ErrorBit                  : 32;\
          }
    union { UNSG32 u32KilopassReg_OTP_ErrorBit;
            struct w32KilopassReg_OTP_ErrorBit;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_OtpReady_OtpReady(r32)          _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_OtpReady_OtpReady(r32,v)        _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_OtpReady_OtpReady(r16)          _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_OtpReady_OtpReady(r16,v)        _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_OtpReady                        {\
            UNSG32 uOtpReady_OtpReady                          :  1;\
            UNSG32 RSVDx190_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_OtpReady;
            struct w32KilopassReg_OtpReady;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_Otp_forcedly_const_Otp_forcedly_const(r32) _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_Otp_forcedly_const_Otp_forcedly_const(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_Otp_forcedly_const              {\
            UNSG32 uOtp_forcedly_const_Otp_forcedly_const      : 32;\
          }
    union { UNSG32 u32KilopassReg_Otp_forcedly_const;
            struct w32KilopassReg_Otp_forcedly_const;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_Otp_password[4];
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_Otp_password_match_Otp_password_match(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_Otp_password_match_Otp_password_match(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_Otp_password_match_Otp_password_match(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_Otp_password_match_Otp_password_match(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_Otp_password_match              {\
            UNSG32 uOtp_password_match_Otp_password_match      :  1;\
            UNSG32 RSVDx1A8_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_Otp_password_match;
            struct w32KilopassReg_Otp_password_match;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_Otp_password_valid_Otp_password_valid(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_Otp_password_valid_Otp_password_valid(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_Otp_password_valid_Otp_password_valid(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_Otp_password_valid_Otp_password_valid(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_Otp_password_valid              {\
            UNSG32 uOtp_password_valid_Otp_password_valid      :  1;\
            UNSG32 RSVDx1AC_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_Otp_password_valid;
            struct w32KilopassReg_Otp_password_valid;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_KilopassOTP_DebugMode_KilopassOTP_DebugMode(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_KilopassOTP_DebugMode           {\
            UNSG32 uKilopassOTP_DebugMode_KilopassOTP_DebugMode :  1;\
            UNSG32 RSVDx1B0_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_KilopassOTP_DebugMode;
            struct w32KilopassReg_KilopassOTP_DebugMode;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_IntStatus_ShadowRegCopyDone(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_IntStatus_ShadowRegCopyDone(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_IntStatus_ShadowRegCopyDone(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_IntStatus_ShadowRegCopyDone(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32KilopassReg_IntStatus_ShadowRegCopyFail(r32) _BFGET_(r32, 1, 1)
    #define   SET32KilopassReg_IntStatus_ShadowRegCopyFail(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16KilopassReg_IntStatus_ShadowRegCopyFail(r16) _BFGET_(r16, 1, 1)
    #define   SET16KilopassReg_IntStatus_ShadowRegCopyFail(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32KilopassReg_IntStatus_OTP_Cp_Fail(r32)      _BFGET_(r32, 2, 2)
    #define   SET32KilopassReg_IntStatus_OTP_Cp_Fail(r32,v)    _BFSET_(r32, 2, 2,v)
    #define   GET16KilopassReg_IntStatus_OTP_Cp_Fail(r16)      _BFGET_(r16, 2, 2)
    #define   SET16KilopassReg_IntStatus_OTP_Cp_Fail(r16,v)    _BFSET_(r16, 2, 2,v)

    #define   GET32KilopassReg_IntStatus_OTP_Rd_Fail(r32)      _BFGET_(r32, 3, 3)
    #define   SET32KilopassReg_IntStatus_OTP_Rd_Fail(r32,v)    _BFSET_(r32, 3, 3,v)
    #define   GET16KilopassReg_IntStatus_OTP_Rd_Fail(r16)      _BFGET_(r16, 3, 3)
    #define   SET16KilopassReg_IntStatus_OTP_Rd_Fail(r16,v)    _BFSET_(r16, 3, 3,v)

    #define   GET32KilopassReg_IntStatus_OTP_Done(r32)         _BFGET_(r32, 4, 4)
    #define   SET32KilopassReg_IntStatus_OTP_Done(r32,v)       _BFSET_(r32, 4, 4,v)
    #define   GET16KilopassReg_IntStatus_OTP_Done(r16)         _BFGET_(r16, 4, 4)
    #define   SET16KilopassReg_IntStatus_OTP_Done(r16,v)       _BFSET_(r16, 4, 4,v)

    #define   GET32KilopassReg_IntStatus_OTP_Wr_RdCheck(r32)   _BFGET_(r32, 5, 5)
    #define   SET32KilopassReg_IntStatus_OTP_Wr_RdCheck(r32,v) _BFSET_(r32, 5, 5,v)
    #define   GET16KilopassReg_IntStatus_OTP_Wr_RdCheck(r16)   _BFGET_(r16, 5, 5)
    #define   SET16KilopassReg_IntStatus_OTP_Wr_RdCheck(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32KilopassReg_IntStatus_OTP_Wr_Check(r32)     _BFGET_(r32, 6, 6)
    #define   SET32KilopassReg_IntStatus_OTP_Wr_Check(r32,v)   _BFSET_(r32, 6, 6,v)
    #define   GET16KilopassReg_IntStatus_OTP_Wr_Check(r16)     _BFGET_(r16, 6, 6)
    #define   SET16KilopassReg_IntStatus_OTP_Wr_Check(r16,v)   _BFSET_(r16, 6, 6,v)

    #define   GET32KilopassReg_IntStatus_OTP_Wr_Fail(r32)      _BFGET_(r32, 7, 7)
    #define   SET32KilopassReg_IntStatus_OTP_Wr_Fail(r32,v)    _BFSET_(r32, 7, 7,v)
    #define   GET16KilopassReg_IntStatus_OTP_Wr_Fail(r16)      _BFGET_(r16, 7, 7)
    #define   SET16KilopassReg_IntStatus_OTP_Wr_Fail(r16,v)    _BFSET_(r16, 7, 7,v)

    #define   GET32KilopassReg_IntStatus_OTP_Wr_Rdback(r32)    _BFGET_(r32, 8, 8)
    #define   SET32KilopassReg_IntStatus_OTP_Wr_Rdback(r32,v)  _BFSET_(r32, 8, 8,v)
    #define   GET16KilopassReg_IntStatus_OTP_Wr_Rdback(r16)    _BFGET_(r16, 8, 8)
    #define   SET16KilopassReg_IntStatus_OTP_Wr_Rdback(r16,v)  _BFSET_(r16, 8, 8,v)

    #define   GET32KilopassReg_IntStatus_AKL_Access_OTP(r32)   _BFGET_(r32, 9, 9)
    #define   SET32KilopassReg_IntStatus_AKL_Access_OTP(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16KilopassReg_IntStatus_AKL_Access_OTP(r16)   _BFGET_(r16, 9, 9)
    #define   SET16KilopassReg_IntStatus_AKL_Access_OTP(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32KilopassReg_IntStatus_OTP_Wr_OneWayCheck(r32) _BFGET_(r32,10,10)
    #define   SET32KilopassReg_IntStatus_OTP_Wr_OneWayCheck(r32,v) _BFSET_(r32,10,10,v)
    #define   GET16KilopassReg_IntStatus_OTP_Wr_OneWayCheck(r16) _BFGET_(r16,10,10)
    #define   SET16KilopassReg_IntStatus_OTP_Wr_OneWayCheck(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32KilopassReg_IntStatus_NSK_SDCP_Err(r32)     _BFGET_(r32,11,11)
    #define   SET32KilopassReg_IntStatus_NSK_SDCP_Err(r32,v)   _BFSET_(r32,11,11,v)
    #define   GET16KilopassReg_IntStatus_NSK_SDCP_Err(r16)     _BFGET_(r16,11,11)
    #define   SET16KilopassReg_IntStatus_NSK_SDCP_Err(r16,v)   _BFSET_(r16,11,11,v)

    #define   GET32KilopassReg_IntStatus_Reseved(r32)          _BFGET_(r32,31,12)
    #define   SET32KilopassReg_IntStatus_Reseved(r32,v)        _BFSET_(r32,31,12,v)

    #define     w32KilopassReg_IntStatus                       {\
            UNSG32 uIntStatus_ShadowRegCopyDone                :  1;\
            UNSG32 uIntStatus_ShadowRegCopyFail                :  1;\
            UNSG32 uIntStatus_OTP_Cp_Fail                      :  1;\
            UNSG32 uIntStatus_OTP_Rd_Fail                      :  1;\
            UNSG32 uIntStatus_OTP_Done                         :  1;\
            UNSG32 uIntStatus_OTP_Wr_RdCheck                   :  1;\
            UNSG32 uIntStatus_OTP_Wr_Check                     :  1;\
            UNSG32 uIntStatus_OTP_Wr_Fail                      :  1;\
            UNSG32 uIntStatus_OTP_Wr_Rdback                    :  1;\
            UNSG32 uIntStatus_AKL_Access_OTP                   :  1;\
            UNSG32 uIntStatus_OTP_Wr_OneWayCheck               :  1;\
            UNSG32 uIntStatus_NSK_SDCP_Err                     :  1;\
            UNSG32 uIntStatus_Reseved                          : 20;\
          }
    union { UNSG32 u32KilopassReg_IntStatus;
            struct w32KilopassReg_IntStatus;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_IntMask_IntMask(r32)            _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_IntMask_IntMask(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_IntMask                         {\
            UNSG32 uIntMask_IntMask                            : 32;\
          }
    union { UNSG32 u32KilopassReg_IntMask;
            struct w32KilopassReg_IntMask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_IntEnable_IntEnable(r32)        _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_IntEnable_IntEnable(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_IntEnable                       {\
            UNSG32 uIntEnable_IntEnable                        : 32;\
          }
    union { UNSG32 u32KilopassReg_IntEnable;
            struct w32KilopassReg_IntEnable;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_OTPunProgram_OTPunProgram(r32)  _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_OTPunProgram_OTPunProgram(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_OTPunProgram_OTPunProgram(r16)  _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_OTPunProgram_OTPunProgram(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_OTPunProgram                    {\
            UNSG32 uOTPunProgram_OTPunProgram                  :  1;\
            UNSG32 RSVDx1C0_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_OTPunProgram;
            struct w32KilopassReg_OTPunProgram;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_ACPU_Access_OTP(r32)            _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_ACPU_Access_OTP(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_ACPU_Access_OTP(r16)            _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_ACPU_Access_OTP(r16,v)          _BFSET_(r16, 0, 0,v)

    #define   GET32KilopassReg_ACPU_Access_Shadow(r32)         _BFGET_(r32, 1, 1)
    #define   SET32KilopassReg_ACPU_Access_Shadow(r32,v)       _BFSET_(r32, 1, 1,v)
    #define   GET16KilopassReg_ACPU_Access_Shadow(r16)         _BFGET_(r16, 1, 1)
    #define   SET16KilopassReg_ACPU_Access_Shadow(r16,v)       _BFSET_(r16, 1, 1,v)

    #define     w32KilopassReg_ACPU_Access                     {\
            UNSG32 uACPU_Access_OTP                            :  1;\
            UNSG32 uACPU_Access_Shadow                         :  1;\
            UNSG32 RSVDx1C4_b2                                 : 30;\
          }
    union { UNSG32 u32KilopassReg_ACPU_Access;
            struct w32KilopassReg_ACPU_Access;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_TransError_TransError(r32)      _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_TransError_TransError(r32,v)    _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_TransError                      {\
            UNSG32 uTransError_TransError                      : 32;\
          }
    union { UNSG32 u32KilopassReg_TransError;
            struct w32KilopassReg_TransError;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_OTP_Read_V0F_V0F(r32)           _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_OTP_Read_V0F_V0F(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_OTP_Read_V0F                    {\
            UNSG32 uOTP_Read_V0F_V0F                           : 32;\
          }
    union { UNSG32 u32KilopassReg_OTP_Read_V0F;
            struct w32KilopassReg_OTP_Read_V0F;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_Spare_reg_Reg0(r32)             _BFGET_(r32,31, 0)
    #define   SET32KilopassReg_Spare_reg_Reg0(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32KilopassReg_Spare_reg                       {\
            UNSG32 uSpare_reg_Reg0                             : 32;\
          }
    union { UNSG32 u32KilopassReg_Spare_reg;
            struct w32KilopassReg_Spare_reg;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_BlankCheckTest_BIST_BlankCheckTest(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_BlankCheckTest             {\
            UNSG32 uBIST_BlankCheckTest_BIST_BlankCheckTest    :  1;\
            UNSG32 RSVDx1D4_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_BlankCheckTest;
            struct w32KilopassReg_BIST_BlankCheckTest;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_RBRead_BIST_RBRead(r32)    _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_RBRead_BIST_RBRead(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_RBRead_BIST_RBRead(r16)    _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_RBRead_BIST_RBRead(r16,v)  _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_RBRead                     {\
            UNSG32 uBIST_RBRead_BIST_RBRead                    :  1;\
            UNSG32 RSVDx1D8_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_RBRead;
            struct w32KilopassReg_BIST_RBRead;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_WLBLTestM_BIST_WLBLTestM(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_WLBLTestM                  {\
            UNSG32 uBIST_WLBLTestM_BIST_WLBLTestM              :  1;\
            UNSG32 RSVDx1DC_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_WLBLTestM;
            struct w32KilopassReg_BIST_WLBLTestM;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_WLBLTestE_BIST_WLBLTestE(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_WLBLTestE                  {\
            UNSG32 uBIST_WLBLTestE_BIST_WLBLTestE              :  1;\
            UNSG32 RSVDx1E0_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_WLBLTestE;
            struct w32KilopassReg_BIST_WLBLTestE;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_ProgTest_BIST_ProgTest(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_ProgTest_BIST_ProgTest(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_ProgTest_BIST_ProgTest(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_ProgTest_BIST_ProgTest(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_ProgTest                   {\
            UNSG32 uBIST_ProgTest_BIST_ProgTest                :  1;\
            UNSG32 RSVDx1E4_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_ProgTest;
            struct w32KilopassReg_BIST_ProgTest;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_ProgTestRead_BIST_ProgTestRead(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_ProgTestRead               {\
            UNSG32 uBIST_ProgTestRead_BIST_ProgTestRead        :  1;\
            UNSG32 RSVDx1E8_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_ProgTestRead;
            struct w32KilopassReg_BIST_ProgTestRead;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr(r32) _BFGET_(r32,15, 0)
    #define   SET32KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr(r32,v) _BFSET_(r32,15, 0,v)
    #define   GET16KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr(r16) _BFGET_(r16,15, 0)
    #define   SET16KilopassReg_BIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr(r16,v) _BFSET_(r16,15, 0,v)

    #define     w32KilopassReg_BIST_BlankCheckTest_StAddr      {\
            UNSG32 uBIST_BlankCheckTest_StAddr_BIST_BlankCheckTest_StAddr : 16;\
            UNSG32 RSVDx1EC_b16                                : 16;\
          }
    union { UNSG32 u32KilopassReg_BIST_BlankCheckTest_StAddr;
            struct w32KilopassReg_BIST_BlankCheckTest_StAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr(r32) _BFGET_(r32,15, 0)
    #define   SET32KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr(r32,v) _BFSET_(r32,15, 0,v)
    #define   GET16KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr(r16) _BFGET_(r16,15, 0)
    #define   SET16KilopassReg_BIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr(r16,v) _BFSET_(r16,15, 0,v)

    #define     w32KilopassReg_BIST_BlankCheckTest_EndAddr     {\
            UNSG32 uBIST_BlankCheckTest_EndAddr_BIST_BlankCheckTest_EndAddr : 16;\
            UNSG32 RSVDx1F0_b16                                : 16;\
          }
    union { UNSG32 u32KilopassReg_BIST_BlankCheckTest_EndAddr;
            struct w32KilopassReg_BIST_BlankCheckTest_EndAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr(r32) _BFGET_(r32,15, 0)
    #define   SET32KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr(r32,v) _BFSET_(r32,15, 0,v)
    #define   GET16KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr(r16) _BFGET_(r16,15, 0)
    #define   SET16KilopassReg_BIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr(r16,v) _BFSET_(r16,15, 0,v)

    #define     w32KilopassReg_BIST_WLBLTest_StAddr            {\
            UNSG32 uBIST_WLBLTest_StAddr_BIST_WLBLTest_StAddr  : 16;\
            UNSG32 RSVDx1F4_b16                                : 16;\
          }
    union { UNSG32 u32KilopassReg_BIST_WLBLTest_StAddr;
            struct w32KilopassReg_BIST_WLBLTest_StAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr(r32) _BFGET_(r32,15, 0)
    #define   SET32KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr(r32,v) _BFSET_(r32,15, 0,v)
    #define   GET16KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr(r16) _BFGET_(r16,15, 0)
    #define   SET16KilopassReg_BIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr(r16,v) _BFSET_(r16,15, 0,v)

    #define     w32KilopassReg_BIST_WLBLTest_EndAddr           {\
            UNSG32 uBIST_WLBLTest_EndAddr_BIST_WLBLTest_EndAddr : 16;\
            UNSG32 RSVDx1F8_b16                                : 16;\
          }
    union { UNSG32 u32KilopassReg_BIST_WLBLTest_EndAddr;
            struct w32KilopassReg_BIST_WLBLTest_EndAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr(r32) _BFGET_(r32,15, 0)
    #define   SET32KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr(r32,v) _BFSET_(r32,15, 0,v)
    #define   GET16KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr(r16) _BFGET_(r16,15, 0)
    #define   SET16KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Addr(r16,v) _BFSET_(r16,15, 0,v)

    #define   GET32KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit(r32) _BFGET_(r32,16,16)
    #define   SET32KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit(r32,v) _BFSET_(r32,16,16,v)
    #define   GET16KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_ProgTest_Addr_BIST_ProgTest_Bit(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_ProgTest_Addr              {\
            UNSG32 uBIST_ProgTest_Addr_BIST_ProgTest_Addr      : 16;\
            UNSG32 uBIST_ProgTest_Addr_BIST_ProgTest_Bit       :  1;\
            UNSG32 RSVDx1FC_b17                                : 15;\
          }
    union { UNSG32 u32KilopassReg_BIST_ProgTest_Addr;
            struct w32KilopassReg_BIST_ProgTest_Addr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_BLCK_Fail_BIST_BLCK_Fail(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_BLCK_Fail                  {\
            UNSG32 uBIST_BLCK_Fail_BIST_BLCK_Fail              :  1;\
            UNSG32 RSVDx200_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_BLCK_Fail;
            struct w32KilopassReg_BIST_BLCK_Fail;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result(r32) _BFGET_(r32, 4, 0)
    #define   SET32KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result(r32,v) _BFSET_(r32, 4, 0,v)
    #define   GET16KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result(r16) _BFGET_(r16, 4, 0)
    #define   SET16KilopassReg_BIST_BLCK_Result_BIST_BLCK_Result(r16,v) _BFSET_(r16, 4, 0,v)

    #define     w32KilopassReg_BIST_BLCK_Result                {\
            UNSG32 uBIST_BLCK_Result_BIST_BLCK_Result          :  5;\
            UNSG32 RSVDx204_b5                                 : 27;\
          }
    union { UNSG32 u32KilopassReg_BIST_BLCK_Result;
            struct w32KilopassReg_BIST_BLCK_Result;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_WLBLTestM_Fail             {\
            UNSG32 uBIST_WLBLTestM_Fail_BIST_WLBLTestM_Fail    :  1;\
            UNSG32 RSVDx208_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_WLBLTestM_Fail;
            struct w32KilopassReg_BIST_WLBLTestM_Fail;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_WLBLTestE_Fail             {\
            UNSG32 uBIST_WLBLTestE_Fail_BIST_WLBLTestE_Fail    :  1;\
            UNSG32 RSVDx20C_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_WLBLTestE_Fail;
            struct w32KilopassReg_BIST_WLBLTestE_Fail;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail(r32) _BFGET_(r32, 0, 0)
    #define   SET32KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail(r16) _BFGET_(r16, 0, 0)
    #define   SET16KilopassReg_BIST_ProgTest_Fail_BIST_ProgTest_Fail(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KilopassReg_BIST_ProgTest_Fail              {\
            UNSG32 uBIST_ProgTest_Fail_BIST_ProgTest_Fail      :  1;\
            UNSG32 RSVDx210_b1                                 : 31;\
          }
    union { UNSG32 u32KilopassReg_BIST_ProgTest_Fail;
            struct w32KilopassReg_BIST_ProgTest_Fail;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit(r32) _BFGET_(r32,15, 0)
    #define   SET32KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit(r32,v) _BFSET_(r32,15, 0,v)
    #define   GET16KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit(r16) _BFGET_(r16,15, 0)
    #define   SET16KilopassReg_BIST_ProgTest_Bit_BIST_ProgTest_Bit(r16,v) _BFSET_(r16,15, 0,v)

    #define     w32KilopassReg_BIST_ProgTest_Bit               {\
            UNSG32 uBIST_ProgTest_Bit_BIST_ProgTest_Bit        : 16;\
            UNSG32 RSVDx214_b16                                : 16;\
          }
    union { UNSG32 u32KilopassReg_BIST_ProgTest_Bit;
            struct w32KilopassReg_BIST_ProgTest_Bit;
          };
    ///////////////////////////////////////////////////////////
    } SIE_KilopassReg;

    typedef union  T32KilopassReg_CopyOTP
          { UNSG32 u32;
            struct w32KilopassReg_CopyOTP;
                 } T32KilopassReg_CopyOTP;
    typedef union  T32KilopassReg_OTPMask
          { UNSG32 u32;
            struct w32KilopassReg_OTPMask;
                 } T32KilopassReg_OTPMask;
    typedef union  T32KilopassReg_OTP_ErrorAddress
          { UNSG32 u32;
            struct w32KilopassReg_OTP_ErrorAddress;
                 } T32KilopassReg_OTP_ErrorAddress;
    typedef union  T32KilopassReg_OTP_ErrorBit
          { UNSG32 u32;
            struct w32KilopassReg_OTP_ErrorBit;
                 } T32KilopassReg_OTP_ErrorBit;
    typedef union  T32KilopassReg_OtpReady
          { UNSG32 u32;
            struct w32KilopassReg_OtpReady;
                 } T32KilopassReg_OtpReady;
    typedef union  T32KilopassReg_Otp_forcedly_const
          { UNSG32 u32;
            struct w32KilopassReg_Otp_forcedly_const;
                 } T32KilopassReg_Otp_forcedly_const;
    typedef union  T32KilopassReg_Otp_password_match
          { UNSG32 u32;
            struct w32KilopassReg_Otp_password_match;
                 } T32KilopassReg_Otp_password_match;
    typedef union  T32KilopassReg_Otp_password_valid
          { UNSG32 u32;
            struct w32KilopassReg_Otp_password_valid;
                 } T32KilopassReg_Otp_password_valid;
    typedef union  T32KilopassReg_KilopassOTP_DebugMode
          { UNSG32 u32;
            struct w32KilopassReg_KilopassOTP_DebugMode;
                 } T32KilopassReg_KilopassOTP_DebugMode;
    typedef union  T32KilopassReg_IntStatus
          { UNSG32 u32;
            struct w32KilopassReg_IntStatus;
                 } T32KilopassReg_IntStatus;
    typedef union  T32KilopassReg_IntMask
          { UNSG32 u32;
            struct w32KilopassReg_IntMask;
                 } T32KilopassReg_IntMask;
    typedef union  T32KilopassReg_IntEnable
          { UNSG32 u32;
            struct w32KilopassReg_IntEnable;
                 } T32KilopassReg_IntEnable;
    typedef union  T32KilopassReg_OTPunProgram
          { UNSG32 u32;
            struct w32KilopassReg_OTPunProgram;
                 } T32KilopassReg_OTPunProgram;
    typedef union  T32KilopassReg_ACPU_Access
          { UNSG32 u32;
            struct w32KilopassReg_ACPU_Access;
                 } T32KilopassReg_ACPU_Access;
    typedef union  T32KilopassReg_TransError
          { UNSG32 u32;
            struct w32KilopassReg_TransError;
                 } T32KilopassReg_TransError;
    typedef union  T32KilopassReg_OTP_Read_V0F
          { UNSG32 u32;
            struct w32KilopassReg_OTP_Read_V0F;
                 } T32KilopassReg_OTP_Read_V0F;
    typedef union  T32KilopassReg_Spare_reg
          { UNSG32 u32;
            struct w32KilopassReg_Spare_reg;
                 } T32KilopassReg_Spare_reg;
    typedef union  T32KilopassReg_BIST_BlankCheckTest
          { UNSG32 u32;
            struct w32KilopassReg_BIST_BlankCheckTest;
                 } T32KilopassReg_BIST_BlankCheckTest;
    typedef union  T32KilopassReg_BIST_RBRead
          { UNSG32 u32;
            struct w32KilopassReg_BIST_RBRead;
                 } T32KilopassReg_BIST_RBRead;
    typedef union  T32KilopassReg_BIST_WLBLTestM
          { UNSG32 u32;
            struct w32KilopassReg_BIST_WLBLTestM;
                 } T32KilopassReg_BIST_WLBLTestM;
    typedef union  T32KilopassReg_BIST_WLBLTestE
          { UNSG32 u32;
            struct w32KilopassReg_BIST_WLBLTestE;
                 } T32KilopassReg_BIST_WLBLTestE;
    typedef union  T32KilopassReg_BIST_ProgTest
          { UNSG32 u32;
            struct w32KilopassReg_BIST_ProgTest;
                 } T32KilopassReg_BIST_ProgTest;
    typedef union  T32KilopassReg_BIST_ProgTestRead
          { UNSG32 u32;
            struct w32KilopassReg_BIST_ProgTestRead;
                 } T32KilopassReg_BIST_ProgTestRead;
    typedef union  T32KilopassReg_BIST_BlankCheckTest_StAddr
          { UNSG32 u32;
            struct w32KilopassReg_BIST_BlankCheckTest_StAddr;
                 } T32KilopassReg_BIST_BlankCheckTest_StAddr;
    typedef union  T32KilopassReg_BIST_BlankCheckTest_EndAddr
          { UNSG32 u32;
            struct w32KilopassReg_BIST_BlankCheckTest_EndAddr;
                 } T32KilopassReg_BIST_BlankCheckTest_EndAddr;
    typedef union  T32KilopassReg_BIST_WLBLTest_StAddr
          { UNSG32 u32;
            struct w32KilopassReg_BIST_WLBLTest_StAddr;
                 } T32KilopassReg_BIST_WLBLTest_StAddr;
    typedef union  T32KilopassReg_BIST_WLBLTest_EndAddr
          { UNSG32 u32;
            struct w32KilopassReg_BIST_WLBLTest_EndAddr;
                 } T32KilopassReg_BIST_WLBLTest_EndAddr;
    typedef union  T32KilopassReg_BIST_ProgTest_Addr
          { UNSG32 u32;
            struct w32KilopassReg_BIST_ProgTest_Addr;
                 } T32KilopassReg_BIST_ProgTest_Addr;
    typedef union  T32KilopassReg_BIST_BLCK_Fail
          { UNSG32 u32;
            struct w32KilopassReg_BIST_BLCK_Fail;
                 } T32KilopassReg_BIST_BLCK_Fail;
    typedef union  T32KilopassReg_BIST_BLCK_Result
          { UNSG32 u32;
            struct w32KilopassReg_BIST_BLCK_Result;
                 } T32KilopassReg_BIST_BLCK_Result;
    typedef union  T32KilopassReg_BIST_WLBLTestM_Fail
          { UNSG32 u32;
            struct w32KilopassReg_BIST_WLBLTestM_Fail;
                 } T32KilopassReg_BIST_WLBLTestM_Fail;
    typedef union  T32KilopassReg_BIST_WLBLTestE_Fail
          { UNSG32 u32;
            struct w32KilopassReg_BIST_WLBLTestE_Fail;
                 } T32KilopassReg_BIST_WLBLTestE_Fail;
    typedef union  T32KilopassReg_BIST_ProgTest_Fail
          { UNSG32 u32;
            struct w32KilopassReg_BIST_ProgTest_Fail;
                 } T32KilopassReg_BIST_ProgTest_Fail;
    typedef union  T32KilopassReg_BIST_ProgTest_Bit
          { UNSG32 u32;
            struct w32KilopassReg_BIST_ProgTest_Bit;
                 } T32KilopassReg_BIST_ProgTest_Bit;
    ///////////////////////////////////////////////////////////

    typedef union  TKilopassReg_CopyOTP
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_CopyOTP;
                   };
                 } TKilopassReg_CopyOTP;
    typedef union  TKilopassReg_OTPMask
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_OTPMask;
                   };
                 } TKilopassReg_OTPMask;
    typedef union  TKilopassReg_OTP_ErrorAddress
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_OTP_ErrorAddress;
                   };
                 } TKilopassReg_OTP_ErrorAddress;
    typedef union  TKilopassReg_OTP_ErrorBit
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_OTP_ErrorBit;
                   };
                 } TKilopassReg_OTP_ErrorBit;
    typedef union  TKilopassReg_OtpReady
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_OtpReady;
                   };
                 } TKilopassReg_OtpReady;
    typedef union  TKilopassReg_Otp_forcedly_const
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_Otp_forcedly_const;
                   };
                 } TKilopassReg_Otp_forcedly_const;
    typedef union  TKilopassReg_Otp_password_match
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_Otp_password_match;
                   };
                 } TKilopassReg_Otp_password_match;
    typedef union  TKilopassReg_Otp_password_valid
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_Otp_password_valid;
                   };
                 } TKilopassReg_Otp_password_valid;
    typedef union  TKilopassReg_KilopassOTP_DebugMode
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_KilopassOTP_DebugMode;
                   };
                 } TKilopassReg_KilopassOTP_DebugMode;
    typedef union  TKilopassReg_IntStatus
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_IntStatus;
                   };
                 } TKilopassReg_IntStatus;
    typedef union  TKilopassReg_IntMask
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_IntMask;
                   };
                 } TKilopassReg_IntMask;
    typedef union  TKilopassReg_IntEnable
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_IntEnable;
                   };
                 } TKilopassReg_IntEnable;
    typedef union  TKilopassReg_OTPunProgram
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_OTPunProgram;
                   };
                 } TKilopassReg_OTPunProgram;
    typedef union  TKilopassReg_ACPU_Access
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_ACPU_Access;
                   };
                 } TKilopassReg_ACPU_Access;
    typedef union  TKilopassReg_TransError
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_TransError;
                   };
                 } TKilopassReg_TransError;
    typedef union  TKilopassReg_OTP_Read_V0F
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_OTP_Read_V0F;
                   };
                 } TKilopassReg_OTP_Read_V0F;
    typedef union  TKilopassReg_Spare_reg
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_Spare_reg;
                   };
                 } TKilopassReg_Spare_reg;
    typedef union  TKilopassReg_BIST_BlankCheckTest
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_BlankCheckTest;
                   };
                 } TKilopassReg_BIST_BlankCheckTest;
    typedef union  TKilopassReg_BIST_RBRead
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_RBRead;
                   };
                 } TKilopassReg_BIST_RBRead;
    typedef union  TKilopassReg_BIST_WLBLTestM
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_WLBLTestM;
                   };
                 } TKilopassReg_BIST_WLBLTestM;
    typedef union  TKilopassReg_BIST_WLBLTestE
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_WLBLTestE;
                   };
                 } TKilopassReg_BIST_WLBLTestE;
    typedef union  TKilopassReg_BIST_ProgTest
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_ProgTest;
                   };
                 } TKilopassReg_BIST_ProgTest;
    typedef union  TKilopassReg_BIST_ProgTestRead
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_ProgTestRead;
                   };
                 } TKilopassReg_BIST_ProgTestRead;
    typedef union  TKilopassReg_BIST_BlankCheckTest_StAddr
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_BlankCheckTest_StAddr;
                   };
                 } TKilopassReg_BIST_BlankCheckTest_StAddr;
    typedef union  TKilopassReg_BIST_BlankCheckTest_EndAddr
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_BlankCheckTest_EndAddr;
                   };
                 } TKilopassReg_BIST_BlankCheckTest_EndAddr;
    typedef union  TKilopassReg_BIST_WLBLTest_StAddr
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_WLBLTest_StAddr;
                   };
                 } TKilopassReg_BIST_WLBLTest_StAddr;
    typedef union  TKilopassReg_BIST_WLBLTest_EndAddr
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_WLBLTest_EndAddr;
                   };
                 } TKilopassReg_BIST_WLBLTest_EndAddr;
    typedef union  TKilopassReg_BIST_ProgTest_Addr
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_ProgTest_Addr;
                   };
                 } TKilopassReg_BIST_ProgTest_Addr;
    typedef union  TKilopassReg_BIST_BLCK_Fail
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_BLCK_Fail;
                   };
                 } TKilopassReg_BIST_BLCK_Fail;
    typedef union  TKilopassReg_BIST_BLCK_Result
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_BLCK_Result;
                   };
                 } TKilopassReg_BIST_BLCK_Result;
    typedef union  TKilopassReg_BIST_WLBLTestM_Fail
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_WLBLTestM_Fail;
                   };
                 } TKilopassReg_BIST_WLBLTestM_Fail;
    typedef union  TKilopassReg_BIST_WLBLTestE_Fail
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_WLBLTestE_Fail;
                   };
                 } TKilopassReg_BIST_WLBLTestE_Fail;
    typedef union  TKilopassReg_BIST_ProgTest_Fail
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_ProgTest_Fail;
                   };
                 } TKilopassReg_BIST_ProgTest_Fail;
    typedef union  TKilopassReg_BIST_ProgTest_Bit
          { UNSG32 u32[1];
            struct {
            struct w32KilopassReg_BIST_ProgTest_Bit;
                   };
                 } TKilopassReg_BIST_ProgTest_Bit;

    ///////////////////////////////////////////////////////////
     SIGN32 KilopassReg_drvrd(SIE_KilopassReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 KilopassReg_drvwr(SIE_KilopassReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void KilopassReg_reset(SIE_KilopassReg *p);
     SIGN32 KilopassReg_cmp  (SIE_KilopassReg *p, SIE_KilopassReg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define KilopassReg_check(p,pie,pfx,hLOG) KilopassReg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define KilopassReg_print(p,    pfx,hLOG) KilopassReg_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: KilopassReg
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE ShadowReg                                (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 GENERIC_NORMAL       (P)
///     # 0x00000 GENERIC_NORMAL       
///               $GENERIC_NORMAL      GENERIC_NORMAL    REG          
///                                    ###
///                                    * GENERIC_NORMAL
///                                    ###
///     @ 0x0013C                      (W-)
///     #         # Stuffing bytes...
///               %% 3616
///     @ 0x00300 GENERIC_ONEWAY       (P)
///               %unsigned 1  GENERIC_ONEWAY            0x0
///                                    ###
///                                    * GENERIC_ONEWAY start macro only
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00304                      (W-)
///     #         # Stuffing bytes...
///               %% 2016
///     @ 0x00400 NAGRA_CERT           (P)
///               %unsigned 1  NAGRA_CERT                
///                                    ###
///                                    * NAGRA_CERT start macro only
///                                    * AKA CAS1
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00404                      (W-)
///     #         # Stuffing bytes...
///               %% 2016
///     @ 0x00500 MBIST_Field          (P)
///     # 0x00500 MBIST_Field          
///               $MBIST_Field         MBIST_Field       REG          
///                                    ###
///                                    * MBIST_Field
///                                    ###
///     @ 0x00700 Irdeto_Field         (P)
///     # 0x00700 Irdeto_Field         
///               $Irdeto_Field        Irdeto_Field      REG          
///                                    ###
///                                    * Irdeto_Field
///                                    * AKA CAS2
///                                    ###
///     @ 0x007C0 GKL_Field            (P)
///     # 0x007C0 GKL_Field            
///               $GKL_Field           GKL_Field         REG          
///                                    ###
///                                    * GKL_Field
///                                    ###
///     @ 0x009C0 Gen3_SecureBoot      (P)
///     # 0x009C0 Gen3_SecureBoot      
///               $Gen3_SecureBoot     Gen3_SecureBoot   REG          
///                                    ###
///                                    * Gen3_SecureBoot
///                                    ###
///     @ 0x00A34                      (W-)
///     #         # Stuffing bytes...
///               %% 1120
///     @ 0x00AC0 Irdeto2_Field        (P)
///     # 0x00AC0 Irdeto2_Field        
///               $Irdeto2_Field       Irdeto2_Field     REG          
///                                    ###
///                                    * Irdeto2_Field
///                                    ###
///     @ 0x00B4C                      (W-)
///     #         # Stuffing bytes...
///               %% 416
///     @ 0x00B80 NSK_Field            (P)
///     # 0x00B80 NSK_Field            
///               $NSK_Field           NSK_Field         REG          
///                                    ###
///                                    * NSK_Field
///                                    * AKA CAS3
///                                    ###
///     @ 0x00CDC                      (W-)
///     #         # Stuffing bytes...
///               %% 1312
///     @ 0x00D80 RFU2                 (P)
///               %unsigned 1  RFU2_Start                0x0
///                                    ###
///                                    * RFU2 start macro only
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00D84                      (W-)
///     #         # Stuffing bytes...
///               %% 5088
///     @ 0x01000 OTP_End              (P)
///               %unsigned 1  OTP_End                   0x0
///                                    ###
///                                    * OTP End macro only
///                                    ###
///               %%        31         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:    4100B, bits:   13421b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_ShadowReg
#define h_ShadowReg (){}

    #define     RA_ShadowReg_GENERIC_NORMAL                    0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_GENERIC_ONEWAY                    0x0300

    #define     BA_ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY     0x0300
    #define     B16ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY     0x0300
    #define   LSb32ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY        0
    #define   LSb16ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY        0
    #define       bShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY     1
    #define   MSK32ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY        0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_NAGRA_CERT                        0x0400

    #define     BA_ShadowReg_NAGRA_CERT_NAGRA_CERT             0x0400
    #define     B16ShadowReg_NAGRA_CERT_NAGRA_CERT             0x0400
    #define   LSb32ShadowReg_NAGRA_CERT_NAGRA_CERT                0
    #define   LSb16ShadowReg_NAGRA_CERT_NAGRA_CERT                0
    #define       bShadowReg_NAGRA_CERT_NAGRA_CERT             1
    #define   MSK32ShadowReg_NAGRA_CERT_NAGRA_CERT                0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_MBIST_Field                       0x0500
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_Irdeto_Field                      0x0700
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_GKL_Field                         0x07C0
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_Gen3_SecureBoot                   0x09C0
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_Irdeto2_Field                     0x0AC0
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_NSK_Field                         0x0B80
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_RFU2                              0x0D80

    #define     BA_ShadowReg_RFU2_RFU2_Start                   0x0D80
    #define     B16ShadowReg_RFU2_RFU2_Start                   0x0D80
    #define   LSb32ShadowReg_RFU2_RFU2_Start                      0
    #define   LSb16ShadowReg_RFU2_RFU2_Start                      0
    #define       bShadowReg_RFU2_RFU2_Start                   1
    #define   MSK32ShadowReg_RFU2_RFU2_Start                      0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_ShadowReg_OTP_End                           0x1000

    #define     BA_ShadowReg_OTP_End_OTP_End                   0x1000
    #define     B16ShadowReg_OTP_End_OTP_End                   0x1000
    #define   LSb32ShadowReg_OTP_End_OTP_End                      0
    #define   LSb16ShadowReg_OTP_End_OTP_End                      0
    #define       bShadowReg_OTP_End_OTP_End                   1
    #define   MSK32ShadowReg_OTP_End_OTP_End                      0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_ShadowReg {
    ///////////////////////////////////////////////////////////
              SIE_GENERIC_NORMAL                               ie_GENERIC_NORMAL;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx13C                                    [452];
    ///////////////////////////////////////////////////////////
    #define   GET32ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY(r32) _BFGET_(r32, 0, 0)
    #define   SET32ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY(r16) _BFGET_(r16, 0, 0)
    #define   SET16ShadowReg_GENERIC_ONEWAY_GENERIC_ONEWAY(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32ShadowReg_GENERIC_ONEWAY                    {\
            UNSG32 uGENERIC_ONEWAY_GENERIC_ONEWAY              :  1;\
            UNSG32 RSVDx300_b1                                 : 31;\
          }
    union { UNSG32 u32ShadowReg_GENERIC_ONEWAY;
            struct w32ShadowReg_GENERIC_ONEWAY;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx304                                    [252];
    ///////////////////////////////////////////////////////////
    #define   GET32ShadowReg_NAGRA_CERT_NAGRA_CERT(r32)        _BFGET_(r32, 0, 0)
    #define   SET32ShadowReg_NAGRA_CERT_NAGRA_CERT(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16ShadowReg_NAGRA_CERT_NAGRA_CERT(r16)        _BFGET_(r16, 0, 0)
    #define   SET16ShadowReg_NAGRA_CERT_NAGRA_CERT(r16,v)      _BFSET_(r16, 0, 0,v)

    #define     w32ShadowReg_NAGRA_CERT                        {\
            UNSG32 uNAGRA_CERT_NAGRA_CERT                      :  1;\
            UNSG32 RSVDx400_b1                                 : 31;\
          }
    union { UNSG32 u32ShadowReg_NAGRA_CERT;
            struct w32ShadowReg_NAGRA_CERT;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx404                                    [252];
    ///////////////////////////////////////////////////////////
              SIE_MBIST_Field                                  ie_MBIST_Field;
    ///////////////////////////////////////////////////////////
              SIE_Irdeto_Field                                 ie_Irdeto_Field;
    ///////////////////////////////////////////////////////////
              SIE_GKL_Field                                    ie_GKL_Field;
    ///////////////////////////////////////////////////////////
              SIE_Gen3_SecureBoot                              ie_Gen3_SecureBoot;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDxA34                                    [140];
    ///////////////////////////////////////////////////////////
              SIE_Irdeto2_Field                                ie_Irdeto2_Field;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDxB4C                                    [52];
    ///////////////////////////////////////////////////////////
              SIE_NSK_Field                                    ie_NSK_Field;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDxCDC                                    [164];
    ///////////////////////////////////////////////////////////
    #define   GET32ShadowReg_RFU2_RFU2_Start(r32)              _BFGET_(r32, 0, 0)
    #define   SET32ShadowReg_RFU2_RFU2_Start(r32,v)            _BFSET_(r32, 0, 0,v)
    #define   GET16ShadowReg_RFU2_RFU2_Start(r16)              _BFGET_(r16, 0, 0)
    #define   SET16ShadowReg_RFU2_RFU2_Start(r16,v)            _BFSET_(r16, 0, 0,v)

    #define     w32ShadowReg_RFU2                              {\
            UNSG32 uRFU2_RFU2_Start                            :  1;\
            UNSG32 RSVDxD80_b1                                 : 31;\
          }
    union { UNSG32 u32ShadowReg_RFU2;
            struct w32ShadowReg_RFU2;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDxD84                                    [636];
    ///////////////////////////////////////////////////////////
    #define   GET32ShadowReg_OTP_End_OTP_End(r32)              _BFGET_(r32, 0, 0)
    #define   SET32ShadowReg_OTP_End_OTP_End(r32,v)            _BFSET_(r32, 0, 0,v)
    #define   GET16ShadowReg_OTP_End_OTP_End(r16)              _BFGET_(r16, 0, 0)
    #define   SET16ShadowReg_OTP_End_OTP_End(r16,v)            _BFSET_(r16, 0, 0,v)

    #define     w32ShadowReg_OTP_End                           {\
            UNSG32 uOTP_End_OTP_End                            :  1;\
            UNSG32 RSVDx1000_b1                                : 31;\
          }
    union { UNSG32 u32ShadowReg_OTP_End;
            struct w32ShadowReg_OTP_End;
          };
    ///////////////////////////////////////////////////////////
    } SIE_ShadowReg;

    typedef union  T32ShadowReg_GENERIC_ONEWAY
          { UNSG32 u32;
            struct w32ShadowReg_GENERIC_ONEWAY;
                 } T32ShadowReg_GENERIC_ONEWAY;
    typedef union  T32ShadowReg_NAGRA_CERT
          { UNSG32 u32;
            struct w32ShadowReg_NAGRA_CERT;
                 } T32ShadowReg_NAGRA_CERT;
    typedef union  T32ShadowReg_RFU2
          { UNSG32 u32;
            struct w32ShadowReg_RFU2;
                 } T32ShadowReg_RFU2;
    typedef union  T32ShadowReg_OTP_End
          { UNSG32 u32;
            struct w32ShadowReg_OTP_End;
                 } T32ShadowReg_OTP_End;
    ///////////////////////////////////////////////////////////

    typedef union  TShadowReg_GENERIC_ONEWAY
          { UNSG32 u32[1];
            struct {
            struct w32ShadowReg_GENERIC_ONEWAY;
                   };
                 } TShadowReg_GENERIC_ONEWAY;
    typedef union  TShadowReg_NAGRA_CERT
          { UNSG32 u32[1];
            struct {
            struct w32ShadowReg_NAGRA_CERT;
                   };
                 } TShadowReg_NAGRA_CERT;
    typedef union  TShadowReg_RFU2
          { UNSG32 u32[1];
            struct {
            struct w32ShadowReg_RFU2;
                   };
                 } TShadowReg_RFU2;
    typedef union  TShadowReg_OTP_End
          { UNSG32 u32[1];
            struct {
            struct w32ShadowReg_OTP_End;
                   };
                 } TShadowReg_OTP_End;

    ///////////////////////////////////////////////////////////
     SIGN32 ShadowReg_drvrd(SIE_ShadowReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 ShadowReg_drvwr(SIE_ShadowReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void ShadowReg_reset(SIE_ShadowReg *p);
     SIGN32 ShadowReg_cmp  (SIE_ShadowReg *p, SIE_ShadowReg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define ShadowReg_check(p,pie,pfx,hLOG) ShadowReg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define ShadowReg_print(p,    pfx,hLOG) ShadowReg_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: ShadowReg
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE KILOPASS_OTP            biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 KilopassOTP          (P)
///               %unsigned 1  KilopassOTP               0x0
///                                    ###
///                                    * Kilopass OTP_Field: 32 K bit
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00004                      (W-)
///     #         # Stuffing bytes...
///               %% 38880
///     @ 0x01300 KilopassReg          (P)
///     # 0x01300 KilopassReg          
///               $KilopassReg         KilopassReg       REG          
///                                    ###
///                                    * Kilopass OTP register
///                                    ###
///     @ 0x01518                      (W-)
///     #         # Stuffing bytes...
///               %% 5952
///     @ 0x01800 ShadowReg            (P)
///     # 0x01800 ShadowReg            
///               $ShadowReg           ShadowReg         REG          
///                                    ###
///                                    * Shadow register
///                                    ###
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:   10244B, bits:   14373b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_KILOPASS_OTP
#define h_KILOPASS_OTP (){}

    #define     RA_KILOPASS_OTP_KilopassOTP                    0x0000

    #define     BA_KILOPASS_OTP_KilopassOTP_KilopassOTP        0x0000
    #define     B16KILOPASS_OTP_KilopassOTP_KilopassOTP        0x0000
    #define   LSb32KILOPASS_OTP_KilopassOTP_KilopassOTP           0
    #define   LSb16KILOPASS_OTP_KilopassOTP_KilopassOTP           0
    #define       bKILOPASS_OTP_KilopassOTP_KilopassOTP        1
    #define   MSK32KILOPASS_OTP_KilopassOTP_KilopassOTP           0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_KILOPASS_OTP_KilopassReg                    0x1300
    ///////////////////////////////////////////////////////////
    #define     RA_KILOPASS_OTP_ShadowReg                      0x1800
    ///////////////////////////////////////////////////////////

    typedef struct SIE_KILOPASS_OTP {
    ///////////////////////////////////////////////////////////
    #define   GET32KILOPASS_OTP_KilopassOTP_KilopassOTP(r32)   _BFGET_(r32, 0, 0)
    #define   SET32KILOPASS_OTP_KilopassOTP_KilopassOTP(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16KILOPASS_OTP_KilopassOTP_KilopassOTP(r16)   _BFGET_(r16, 0, 0)
    #define   SET16KILOPASS_OTP_KilopassOTP_KilopassOTP(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32KILOPASS_OTP_KilopassOTP                    {\
            UNSG32 uKilopassOTP_KilopassOTP                    :  1;\
            UNSG32 RSVDx0_b1                                   : 31;\
          }
    union { UNSG32 u32KILOPASS_OTP_KilopassOTP;
            struct w32KILOPASS_OTP_KilopassOTP;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx4                                      [4860];
    ///////////////////////////////////////////////////////////
              SIE_KilopassReg                                  ie_KilopassReg;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx1518                                   [744];
    ///////////////////////////////////////////////////////////
              SIE_ShadowReg                                    ie_ShadowReg;
    ///////////////////////////////////////////////////////////
    } SIE_KILOPASS_OTP;

    typedef union  T32KILOPASS_OTP_KilopassOTP
          { UNSG32 u32;
            struct w32KILOPASS_OTP_KilopassOTP;
                 } T32KILOPASS_OTP_KilopassOTP;
    ///////////////////////////////////////////////////////////

    typedef union  TKILOPASS_OTP_KilopassOTP
          { UNSG32 u32[1];
            struct {
            struct w32KILOPASS_OTP_KilopassOTP;
                   };
                 } TKILOPASS_OTP_KilopassOTP;

    ///////////////////////////////////////////////////////////
     SIGN32 KILOPASS_OTP_drvrd(SIE_KILOPASS_OTP *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 KILOPASS_OTP_drvwr(SIE_KILOPASS_OTP *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void KILOPASS_OTP_reset(SIE_KILOPASS_OTP *p);
     SIGN32 KILOPASS_OTP_cmp  (SIE_KILOPASS_OTP *p, SIE_KILOPASS_OTP *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define KILOPASS_OTP_check(p,pie,pfx,hLOG) KILOPASS_OTP_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define KILOPASS_OTP_print(p,    pfx,hLOG) KILOPASS_OTP_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: KILOPASS_OTP
////////////////////////////////////////////////////////////



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif
//////
/// ENDOFFILE: kilopass.h
////////////////////////////////////////////////////////////

