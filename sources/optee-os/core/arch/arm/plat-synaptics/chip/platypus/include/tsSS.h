// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef tsSS_h
#define tsSS_h (){}


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
/// $INTERFACE BCM_Security                             (4,4)
///     ###
///     * Only BCM can access this register
///     ###
///     # # ----------------------------------------------------------
///     @ 0x00000 ChipID               (P)
///     # 0x00000 ChipID               
///               $OneReg              ChipID            REG       [2]
///                                    ###
///                                    * Chip ID written out by SCPU. Open for ACPU to read.
///                                    ###
///     @ 0x00008 V_Version            (R-)
///               %unsigned 32 V_Version                 0x0
///                                    ###
///                                    * Vendor version
///                                    * ACPU can access this register.
///                                    ###
///     @ 0x0000C Berlin_DDR_Base      (P)
///               %unsigned 32 Berlin_DDR_Base           0x0
///                                    ###
///                                    * This register defines the base address of Berlin’s DDR for BCM M3 to access. M3 can address a 1G space from this base. Only BCM can access this register.
///                                    * For M3 memory map, the following 1GB space is mapped to Berlin DDR:
///                                    * 32'h6000_0000~ 32'h9FFF_FFFF
///                                    ###
///     @ 0x00010 Berlin_SOC_Base      (P)
///               %unsigned 32 Berlin_SOC_Base           0xF0000000
///                                    ###
///                                    * This register defines the base address of Berlin’s SOC for BCM M3 to access. Only BCM can access this register.
///                                    * For M3 memory map, the following 512MB space is mapped to Berlin SOC:
///                                    * 32'hD000_0000~ 32'hDFFF_FFFF
///                                    ###
///     @ 0x00014 BCM_DIR_En           (P)
///               %unsigned 1  BCM_2_DDR                 0x1
///                                    ###
///                                    * 1: Enable BCM direct access to DDR
///                                    ###
///               %unsigned 1  BCM_2_SOC                 0x1
///                                    ###
///                                    * 1: Enable BCM direct access to DDR
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x00018 ACPU_Access          (P)
///               %unsigned 1  TSOTP                     0x0
///                                    ###
///                                    * 1: Grant the R/W access to TSOTP from ACPU
///                                    ###
///               %unsigned 1  TspKeyTbl                 0x0
///                                    ###
///                                    * 1: Grant the R/W access to TspKeyTble from ACPU
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x0001C Efuse_Read_Disable   (RW-)
///               ###
///               * When 1 BCM won’t be able to read the corresponding efuse entry. One bit per entry. Once a bit is set to 1, it can’t be set back to 0 by any hardware/software means other than reset. Only BCM can access this register
///               ###
///               %unsigned 1  Row_0                     0x0
///               %unsigned 1  Row_1                     0x0
///               %unsigned 1  Row_2                     0x0
///               %unsigned 1  Row_3                     0x0
///               %unsigned 1  Row_4                     0x0
///               %unsigned 1  Row_5                     0x0
///               %unsigned 1  Row_6                     0x0
///               %unsigned 1  Row_7                     0x0
///               %unsigned 1  Row_8                     0x0
///               %unsigned 1  Row_9                     0x0
///               %unsigned 1  Row_10                    0x0
///               %unsigned 1  Row_11                    0x0
///               %unsigned 1  Row_12                    0x0
///               %unsigned 1  Row_13                    0x0
///               %unsigned 1  Row_14                    0x0
///               %unsigned 1  Row_15                    0x0
///               %unsigned 1  Row_16                    0x0
///               %unsigned 1  Row_17                    0x0
///               %unsigned 1  Row_18                    0x0
///               %unsigned 1  Row_19                    0x0
///               %unsigned 1  Row_20                    0x0
///               %unsigned 1  Row_21                    0x0
///               %unsigned 1  Row_22                    0x0
///               %unsigned 1  Row_23                    0x0
///               %unsigned 1  Row_24                    0x0
///               %unsigned 1  Row_25                    0x0
///               %unsigned 1  Row_26                    0x0
///               %unsigned 1  Row_27                    0x0
///               %unsigned 1  Row_28                    0x0
///               %unsigned 1  Row_29                    0x0
///               %unsigned 1  Row_30                    0x0
///               %unsigned 1  Row_31                    0x0
///     # 0x00020 Efuse_Read_Disable1  
///               %unsigned 1  Row_32                    0x0
///               %unsigned 1  Row_33                    0x0
///               %unsigned 1  Row_34                    0x0
///               %unsigned 1  Row_35                    0x0
///               %unsigned 1  Row_36                    0x0
///               %unsigned 1  Row_37                    0x0
///               %unsigned 1  Row_38                    0x0
///               %unsigned 1  Row_39                    0x0
///               %unsigned 1  Row_40                    0x0
///               %unsigned 1  Row_41                    0x0
///               %unsigned 1  Row_42                    0x0
///               %unsigned 1  Row_43                    0x0
///               %unsigned 1  Row_44                    0x0
///               %unsigned 1  Row_45                    0x0
///               %unsigned 1  Row_46                    0x0
///               %unsigned 1  Row_47                    0x0
///               %unsigned 1  Row_48                    0x0
///               %unsigned 1  Row_49                    0x0
///               %unsigned 1  Row_50                    0x0
///               %unsigned 1  Row_51                    0x0
///               %unsigned 1  Row_52                    0x0
///               %unsigned 1  Row_53                    0x0
///               %unsigned 1  Row_54                    0x0
///               %unsigned 1  Row_55                    0x0
///               %unsigned 1  Row_56                    0x0
///               %unsigned 1  Row_57                    0x0
///               %unsigned 1  Row_58                    0x0
///               %unsigned 1  Row_59                    0x0
///               %unsigned 1  Row_60                    0x0
///               %unsigned 1  Row_61                    0x0
///               %unsigned 1  Row_62                    0x0
///               %unsigned 1  Row_63                    0x0
///     @ 0x00024                      (W-)
///     #         # Stuffing bytes...
///               %% 64
///     @ 0x0002C RKEKReg              (P)
///     # 0x0002C RKEKReg              
///               $OneReg              RKEKReg           REG       [8]
///                                    ###
///                                    * This register is directly connected to the AES engine of BCM. This register can be accessed by BCM before it is locked.
///                                    ###
///     @ 0x0004C RKEKRegReadDisable   (RW)
///               %unsigned 1  RKEKRegReadDisable        0x0
///                                    ###
///                                    * If RKEKRegReadDisable is set to one, BCM cannot read RKEKReg. Default value of RKEKRegReadDisable is zero. Once it is set to one, it can’t be set back to zero by any hardware/software means other than reset. Only BCM can access this register
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00050 RKEKRegWriteDisable  (RW)
///               %unsigned 1  RKEKRegWriteDisable       0x0
///                                    ###
///                                    * If RKEKRegWriteDisable is set to one, BCM cannot Write RKEKReg. Default value of RKEKRegWriteDisable is zero. Once it is set to one, it can’t be set back to zero by any hardware/software means other than reset. Only BCM can access this register
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00054 FeatureCtrlReg       (P)
///               ###
///               * SCPU access only
///               ###
///               %unsigned 32 V2G_Fuse                  0x0
///                                    ###
///                                    * V2G FeatureCtrl register, only SCPU can R/W
///                                    ###
///     # 0x00058 FeatureCtrlReg1      
///               %unsigned 32 Hantro_Fuse_Dec           0x0
///                                    ###
///                                    * Hantro FuseDec FeatureCtrl register, only SCPU can R/W
///                                    ###
///     # 0x0005C FeatureCtrlReg2      
///               %unsigned 32 Hantro_Fuse_PP            0x0
///                                    ###
///                                    * Hantro Fuse PP FeatureCtrl register, only SCPU can R/W
///                                    ###
///     # 0x00060 FeatureCtrlReg3      
///               %unsigned 32 Hantro_Fuse_Enc           0xFFFFFFFF
///                                    ###
///                                    * Hantro Fuse Encode FeatureCtrl register, only SCPU can R/W
///                                    ###
///     # 0x00064 FeatureCtrlReg4      
///               %unsigned 32 GPU_Feature               0x0
///                                    ###
///                                    * GPU_Feature
///                                    ###
///     @ 0x00068                      (P)
///     # 0x00068 AVIO                 
///               $OneReg              AVIO              REG       [4]
///     @ 0x00078                      (P)
///               %unsigned 32 Reseved_6                 0x0
///                                    ###
///                                    * Reserved for future definition
///                                    ###
///               %unsigned 32 Reseved_7                 0x0
///                                    ###
///                                    * Reserved for future definition
///                                    ###
///     @ 0x00080 EROM_Golden_CRC      (RW)
///               %unsigned 32 Block_0                   0x0
///     # 0x00084 EROM_Golden_CRC1     
///               %unsigned 32 Block_1                   0x0
///     # 0x00088 EROM_Golden_CRC2     
///               %unsigned 32 Block_2                   0x0
///     # 0x0008C EROM_Golden_CRC3     
///               %unsigned 32 Block_3                   0x0
///     # 0x00090 EROM_Golden_CRC4     
///               %unsigned 32 Block_4                   0x0
///     # 0x00094 EROM_Golden_CRC5     
///               %unsigned 32 Block_5                   0x0
///     # 0x00098 EROM_Golden_CRC6     
///               %unsigned 32 Block_6                   0x0
///     # 0x0009C EROM_Golden_CRC7     
///               %unsigned 32 Block_7                   0x0
///     # 0x000A0 EROM_Golden_CRC8     
///               %unsigned 32 Block_8                   0x0
///     # 0x000A4 EROM_Golden_CRC9     
///               %unsigned 32 Block_9                   0x0
///     # 0x000A8 EROM_Golden_CRC10    
///               %unsigned 32 Block_10                  0x0
///     # 0x000AC EROM_Golden_CRC11    
///               %unsigned 32 Block_11                  0x0
///     # 0x000B0 EROM_Golden_CRC12    
///               %unsigned 32 Block_12                  0x0
///     # 0x000B4 EROM_Golden_CRC13    
///               %unsigned 32 Block_13                  0x0
///     # 0x000B8 EROM_Golden_CRC14    
///               %unsigned 32 Block_14                  0x0
///     # 0x000BC EROM_Golden_CRC15    
///               %unsigned 32 Block_15                  0x0
///     @ 0x000C0 EROM_BackgroundCheck_Start_Addresss (P)
///               %unsigned 32 Start_Address             0x28000
///     @ 0x000C4 EROM_BackgroundCheck_End_Addresss (P)
///               %unsigned 32 End_Address               0x2C000
///     @ 0x000C8 EROM_BackgroundCheck_Start (RW)
///               %unsigned 4  Start                     0x5
///                                    ###
///                                    * 0101: disable
///                                    * Any other value is enable.
///                                    * To start back ground check, IROM needs to program this register to ‘1010’. Do not program it to any other value.
///                                    * Once it is not 0101, it will lock itselt until HW sees BCM fetch IROM from reset vector.
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x000CC EROM_BackgroundCheck_Password (P)
///               %unsigned 32 Password                  0x0
///                                    ###
///                                    * BCM IROM needs to provide the correct password in order to program EROM_BackgroundCheck_Start
///                                    ###
///     @ 0x000D0 EROM_BackgroundCheck_Password_match (R-)
///               %unsigned 1  Password_match            0x0
///                                    ###
///                                    * BCM IROM provided the correct password matches with RTL
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x000D4 IROM_SCS_Start_Address (RW)
///               %unsigned 32 Start_Address             0x0
///                                    ###
///                                    * SCS code Start address, self lock along with mask. BCM access only.
///                                    * Locked by IROM_SCS_Mask
///                                    ###
///     @ 0x000D8 IROM_SCS_End_Address (RW)
///               %unsigned 32 End_Address               0x0
///                                    ###
///                                    * SCS code Start address, self lock along with mask. BCM access only.
///                                    * Locked by IROM_SCS_Mask
///                                    ###
///     @ 0x000DC IROM_SCS_Mask        (RW)
///               %unsigned 4  Mask                      0x5
///                                    ###
///                                    * 0101: disable
///                                    * Any other value is enable.
///                                    * To mask the IROM, IROM needs to program this register to ‘1010’. Do not program it to any other value.
///                                    * Once it is not 0101, it will lock itselt and related registers until HW sees BCM fetch IROM from reset vector.
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x000E0 BCM_reset_2_soc      (RW)
///               %unsigned 1  BCM_reset_2_soc           0x0
///                                    ###
///                                    * Reset full-chip. Level signal.
///                                    * Can be triggered by:
///                                    * an access to masked IROM, or
///                                    * EROM background check fail or
///                                    * BCM
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x000E4 EROM_InstructionFilter_Start (RW)
///               %unsigned 4  Start                     0x5
///                                    ###
///                                    * 0101: disable
///                                    * Any other value is enable.
///                                    * To start instruction bus filter, IROM needs to program this register to ‘1010’. Do not program it to any other value.
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x000E8 EROM_InstructionFilter_Lock (RW)
///               %unsigned 4  Lock                      0x5
///                                    ###
///                                    * 0101: unlock
///                                    * Any other value is lock.
///                                    * To lock instruction bus filter, IROM needs to program this register to ‘1010’. Do not program it to any other value. This register locks:
///                                    * EROM_InstructionFilter_Lock
///                                    * EROM_InstructionFilter_Start
///                                    * EROM_Instruction_Range
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x000EC                      (W-)
///     #         # Stuffing bytes...
///               %% 32
///     @ 0x000F0 EROM_Instruction_Range (RW)
///               %unsigned 32 Range0_Low                0x0
///     # 0x000F4 EROM_Instruction_Range1 
///               %unsigned 32 Range0_High               0x0
///     # 0x000F8 EROM_Instruction_Range2 
///               %unsigned 32 Range1_Low                0x0
///     # 0x000FC EROM_Instruction_Range3 
///               %unsigned 32 Range1_High               0x0
///     # 0x00100 EROM_Instruction_Range4 
///               %unsigned 32 Range2_Low                0x0
///     # 0x00104 EROM_Instruction_Range5 
///               %unsigned 32 Range2_High               0x0
///     # 0x00108 EROM_Instruction_Range6 
///               %unsigned 32 Range3_Low                0x0
///     # 0x0010C EROM_Instruction_Range7 
///               %unsigned 32 Range3_High               0x0
///     # 0x00110 EROM_Instruction_Range8 
///               %unsigned 32 Range4_Low                0x0
///     # 0x00114 EROM_Instruction_Range9 
///               %unsigned 32 Range4_High               0x0
///     # 0x00118 EROM_Instruction_Range10 
///               %unsigned 32 Range5_Low                0x0
///     # 0x0011C EROM_Instruction_Range11 
///               %unsigned 32 Range5_High               0x0
///     # 0x00120 EROM_Instruction_Range12 
///               %unsigned 32 Range6_Low                0x0
///     # 0x00124 EROM_Instruction_Range13 
///               %unsigned 32 Range6_High               0x0
///     # 0x00128 EROM_Instruction_Range14 
///               %unsigned 32 Range7_Low                0x0
///     # 0x0012C EROM_Instruction_Range15 
///               %unsigned 32 Range7_High               0x0
///     @ 0x00130                      (W-)
///     #         # Stuffing bytes...
///               %% 640
///     @ 0x00180 EROM_DataFilter_Start (RW)
///               %unsigned 4  Start                     0x5
///                                    ###
///                                    * 0101: disable
///                                    * Any other value is enable.
///                                    * To start data bus filter, IROM needs to program this register to ‘1010’. Do not program it to any other value.
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x00184 EROM_DataFilter_Lock (RW)
///               %unsigned 4  Lock                      0x5
///                                    ###
///                                    * 0101: unlock
///                                    * Any other value is lock.
///                                    * To lock data bus filter, IROM needs to program this register to ‘1010’. Do not program it to any other value. This register locks:
///                                    * EROM_DataFilter_Lock
///                                    * EROM_DataFilter_Start
///                                    * EROM_Data_Range
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x00188                      (W-)
///     #         # Stuffing bytes...
///               %% 64
///     @ 0x00190 EROM_Data_Range      (RW)
///               %unsigned 32 Range0_Low                0x0
///     # 0x00194 EROM_Data_Range1     
///               %unsigned 32 Range0_High               0x0
///     # 0x00198 EROM_Data_Range2     
///               %unsigned 32 Range1_Low                0x0
///     # 0x0019C EROM_Data_Range3     
///               %unsigned 32 Range1_High               0x0
///     # 0x001A0 EROM_Data_Range4     
///               %unsigned 32 Range2_Low                0x0
///     # 0x001A4 EROM_Data_Range5     
///               %unsigned 32 Range2_High               0x0
///     # 0x001A8 EROM_Data_Range6     
///               %unsigned 32 Range3_Low                0x0
///     # 0x001AC EROM_Data_Range7     
///               %unsigned 32 Range3_High               0x0
///     # 0x001B0 EROM_Data_Range8     
///               %unsigned 32 Range4_Low                0x0
///     # 0x001B4 EROM_Data_Range9     
///               %unsigned 32 Range4_High               0x0
///     # 0x001B8 EROM_Data_Range10    
///               %unsigned 32 Range5_Low                0x0
///     # 0x001BC EROM_Data_Range11    
///               %unsigned 32 Range5_High               0x0
///     # 0x001C0 EROM_Data_Range12    
///               %unsigned 32 Range6_Low                0x0
///     # 0x001C4 EROM_Data_Range13    
///               %unsigned 32 Range6_High               0x0
///     # 0x001C8 EROM_Data_Range14    
///               %unsigned 32 Range7_Low                0x0
///     # 0x001CC EROM_Data_Range15    
///               %unsigned 32 Range7_High               0x0
///     @ 0x001D0                      (W-)
///     #         # Stuffing bytes...
///               %% 1568
///     @ 0x00294 Allow_ACPU_Init_AKL  (P)
///               %unsigned 1  Allow_ACPU_Init_AKL       0x0
///                                    ###
///                                    * 1: allow ACPU to init AKL
///                                    * 0: not allow ACPU to init AKL
///                                    * BCM access only
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00298 IROM_reset_address   (RW)
///               %unsigned 32 IROM_reset_address        0x0
///     @ 0x0029C IROM_reset_address_lock (RW)
///               %unsigned 4  IROM_reset_address_lock   0x5
///                                    ###
///                                    * 0101 is unlock, any other value is lock
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x002A0 Security_Reg         (P)
///     # 0x002A0 Security_Reg         
///               $OneReg              Security_Reg      REG       [8]
///                                    ###
///                                    * Security registers. Only SCPU can read/write it.
///                                    ###
///     @ 0x002C0                      (W-)
///     #         # Stuffing bytes...
///               %% 128
///     @ 0x002D0 KP_ACPU_Access0      (RW)
///               %unsigned 32 ACPU_Region_en0           0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of ACPU to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002D4 KP_ACPU_Access1      (RW)
///               %unsigned 32 ACPU_Region_en1           0xFFFF00
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of ACPU to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002D8 KP_ACPU_Access2      (RW)
///               %unsigned 32 ACPU_Region_en2           0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of ACPU to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002DC KP_ACPU_Access3      (RW)
///               %unsigned 32 ACPU_Region_en3           0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of ACPU to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002E0 KP_BCM_Access0       (RW)
///               %unsigned 32 BCM_Region_en0            0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of BCM to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002E4 KP_BCM_Access1       (RW)
///               %unsigned 32 BCM_Region_en1            0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of BCM to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002E8 KP_BCM_Access2       (RW)
///               %unsigned 32 BCM_Region_en2            0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of BCM to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002EC KP_BCM_Access3       (RW)
///               %unsigned 32 BCM_Region_en3            0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of BCM to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002F0 KP_IFCP_Access0      (RW)
///               %unsigned 32 IFCP_Region_en0           0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of IFCP to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002F4 KP_IFCP_Access1      (RW)
///               %unsigned 32 IFCP_Region_en1           0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of IFCP to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002F8 KP_IFCP_Access2      (RW)
///               %unsigned 32 IFCP_Region_en2           0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of IFCP to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x002FC KP_IFCP_Access3      (RW)
///               %unsigned 32 IFCP_Region_en3           0x0
///                                    ###
///                                    * KP OTP is divided into 128 regions, each is 256-bit. Each register represents the access control of IFCP to this region.
///                                    * Bit 0: OTP bit0-bit 255
///                                    * Bit 1: OTP bit256-bit 511 and so on
///                                    * Value 0 is disable
///                                    * Value 1 is enable
///                                    ###
///     @ 0x00300 KP_ACPU_Access_lock  (RW)
///               %unsigned 4  lock                      0x0
///                                    ###
///                                    * 0000 is unblock, any other value is lock
///                                    * This register will lock entire KP_ACPU_Access and itself
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x00304 KP_BCM_Access_lock   (RW)
///               %unsigned 4  lock                      0x0
///                                    ###
///                                    * 0000 is unblock, any other value is lock
///                                    * This register will lock entire KP_BCM_Access and itself
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x00308 KP_IFCP_Access_lock  (RW)
///               %unsigned 4  lock                      0x0
///                                    ###
///                                    * 0000 is unblock, any other value is lock
///                                    * This register will lock entire KP_IFCP_Access and itself
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x0030C Spare_reg            (RW)
///               %unsigned 32 Reg0                      0x1
///     # 0x00310 Spare_reg1           
///               %unsigned 32 Reg1                      0x0
///     # 0x00314 Spare_reg2           
///               %unsigned 32 Reg2                      0x0
///     # 0x00318 Spare_reg3           
///               %unsigned 32 Reg3                      0x0
///     # 0x0031C Spare_reg4           
///               %unsigned 32 Reg4                      0x0
///     # 0x00320 Spare_reg5           
///               %unsigned 32 Reg5                      0x0
///     # 0x00324 Spare_reg6           
///               %unsigned 32 Reg6                      0x0
///     # 0x00328 Spare_reg7           
///               %unsigned 32 Reg7                      0x0
///     @ 0x0032C FI_Status            (R-)
///               %unsigned 1  FI_PL_SOC                 
///               %unsigned 1  FI_BIU                    
///               %%        30         # Stuffing bits...
///     @ 0x00330                      (W-)
///     #         # Stuffing bytes...
///               %% 32
///     @ 0x00334 IFCP_RAM_Addr_Key    (P)
///               %unsigned 32 RAM_Addr_Key              0x0
///     @ 0x00338 IFCP_RAM_Data_Key    (P)
///               %unsigned 32 RAM_Data_Key              0x0
///     @ 0x0033C                      (W-)
///     #         # Stuffing bytes...
///               %% 32
///     @ 0x00340 IROM_Mask1_Start_Address (RW)
///               %unsigned 32 Start_Address             0x10000
///                                    ###
///                                    * SCS code Start address, self lock along with mask. BCM access only.
///                                    * Locked by IROM_SCS_Mask1
///                                    ###
///     @ 0x00344 IROM_Mask1_End_Address (RW)
///               %unsigned 32 End_Address               0x20000
///                                    ###
///                                    * SCS code Start address, self lock along with mask. BCM access only.
///                                    * Locked by IROM_SCS_Mask1
///                                    ###
///     @ 0x00348 IROM_Mask1_Mask      (RW)
///               %unsigned 4  Mask                      0x5
///                                    ###
///                                    * 0101: Enabled but not locked
///                                    * 0000: disabled and not locked
///                                    * any other value: enabled and locked
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x0034C                      (W-)
///     #         # Stuffing bytes...
///               %% 32
///     @ 0x00350 IROM_Mask2_Start_Address (RW)
///               %unsigned 32 Start_Address             0x10000
///                                    ###
///                                    * SCS code Start address, self lock along with mask. BCM access only.
///                                    * Locked by IROM_SCS_Mask1
///                                    ###
///     @ 0x00354 IROM_Mask2_End_Address (RW)
///               %unsigned 32 End_Address               0x20000
///                                    ###
///                                    * SCS code Start address, self lock along with mask. BCM access only.
///                                    * Locked by IROM_SCS_Mask1
///                                    ###
///     @ 0x00358 IROM_Mask2_Mask      (RW)
///               %unsigned 4  Mask                      0x5
///                                    ###
///                                    * 0101: Enabled but not locked
///                                    * 0000: disabled and not locked
///                                    * any other value: enabled and locked
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x0035C                      (W-)
///     #         # Stuffing bytes...
///               %% 32
///     @ 0x00360 IROM_Mask3_Start_Address (RW)
///               %unsigned 32 Start_Address             0x10000
///                                    ###
///                                    * SCS code Start address, self lock along with mask. BCM access only.
///                                    * Locked by IROM_SCS_Mask1
///                                    ###
///     @ 0x00364 IROM_Mask3_End_Address (RW)
///               %unsigned 32 End_Address               0x20000
///                                    ###
///                                    * SCS code Start address, self lock along with mask. BCM access only.
///                                    * Locked by IROM_SCS_Mask1
///                                    ###
///     @ 0x00368 IROM_Mask3_Mask      (RW)
///               %unsigned 4  Mask                      0x5
///                                    ###
///                                    * 0101: Enabled but not locked
///                                    * 0000: disabled and not locked
///                                    * any other value: enabled and locked
///                                    ###
///               %%        28         # Stuffing bits...
///     @ 0x0036C CERT_Priv_Mode_CFG   (RW)
///               %unsigned 1  Secure_Boot_Activation    0x1
///               %unsigned 1  Flash_Protection_Activation 0x1
///               %unsigned 30 RFU                       0x0
///     @ 0x00370 EnvMon_Status        (R-)
///               %unsigned 1  Clock                     0x0
///               %unsigned 1  Reset                     0x0
///               %unsigned 1  Voltage                   0x0
///               %%        29         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     884B, bits:    3810b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_BCM_Security
#define h_BCM_Security (){}

    #define     RA_BCM_Security_ChipID                         0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_V_Version                      0x0008

    #define     BA_BCM_Security_V_Version_V_Version            0x0008
    #define     B16BCM_Security_V_Version_V_Version            0x0008
    #define   LSb32BCM_Security_V_Version_V_Version               0
    #define   LSb16BCM_Security_V_Version_V_Version               0
    #define       bBCM_Security_V_Version_V_Version            32
    #define   MSK32BCM_Security_V_Version_V_Version               0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_Berlin_DDR_Base                0x000C

    #define     BA_BCM_Security_Berlin_DDR_Base_Berlin_DDR_Base 0x000C
    #define     B16BCM_Security_Berlin_DDR_Base_Berlin_DDR_Base 0x000C
    #define   LSb32BCM_Security_Berlin_DDR_Base_Berlin_DDR_Base    0
    #define   LSb16BCM_Security_Berlin_DDR_Base_Berlin_DDR_Base    0
    #define       bBCM_Security_Berlin_DDR_Base_Berlin_DDR_Base 32
    #define   MSK32BCM_Security_Berlin_DDR_Base_Berlin_DDR_Base    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_Berlin_SOC_Base                0x0010

    #define     BA_BCM_Security_Berlin_SOC_Base_Berlin_SOC_Base 0x0010
    #define     B16BCM_Security_Berlin_SOC_Base_Berlin_SOC_Base 0x0010
    #define   LSb32BCM_Security_Berlin_SOC_Base_Berlin_SOC_Base    0
    #define   LSb16BCM_Security_Berlin_SOC_Base_Berlin_SOC_Base    0
    #define       bBCM_Security_Berlin_SOC_Base_Berlin_SOC_Base 32
    #define   MSK32BCM_Security_Berlin_SOC_Base_Berlin_SOC_Base    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_BCM_DIR_En                     0x0014

    #define     BA_BCM_Security_BCM_DIR_En_BCM_2_DDR           0x0014
    #define     B16BCM_Security_BCM_DIR_En_BCM_2_DDR           0x0014
    #define   LSb32BCM_Security_BCM_DIR_En_BCM_2_DDR              0
    #define   LSb16BCM_Security_BCM_DIR_En_BCM_2_DDR              0
    #define       bBCM_Security_BCM_DIR_En_BCM_2_DDR           1
    #define   MSK32BCM_Security_BCM_DIR_En_BCM_2_DDR              0x00000001

    #define     BA_BCM_Security_BCM_DIR_En_BCM_2_SOC           0x0014
    #define     B16BCM_Security_BCM_DIR_En_BCM_2_SOC           0x0014
    #define   LSb32BCM_Security_BCM_DIR_En_BCM_2_SOC              1
    #define   LSb16BCM_Security_BCM_DIR_En_BCM_2_SOC              1
    #define       bBCM_Security_BCM_DIR_En_BCM_2_SOC           1
    #define   MSK32BCM_Security_BCM_DIR_En_BCM_2_SOC              0x00000002
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_ACPU_Access                    0x0018

    #define     BA_BCM_Security_ACPU_Access_TSOTP              0x0018
    #define     B16BCM_Security_ACPU_Access_TSOTP              0x0018
    #define   LSb32BCM_Security_ACPU_Access_TSOTP                 0
    #define   LSb16BCM_Security_ACPU_Access_TSOTP                 0
    #define       bBCM_Security_ACPU_Access_TSOTP              1
    #define   MSK32BCM_Security_ACPU_Access_TSOTP                 0x00000001

    #define     BA_BCM_Security_ACPU_Access_TspKeyTbl          0x0018
    #define     B16BCM_Security_ACPU_Access_TspKeyTbl          0x0018
    #define   LSb32BCM_Security_ACPU_Access_TspKeyTbl             1
    #define   LSb16BCM_Security_ACPU_Access_TspKeyTbl             1
    #define       bBCM_Security_ACPU_Access_TspKeyTbl          1
    #define   MSK32BCM_Security_ACPU_Access_TspKeyTbl             0x00000002
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_Efuse_Read_Disable             0x001C

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_0       0x001C
    #define     B16BCM_Security_Efuse_Read_Disable_Row_0       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_0          0
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_0          0
    #define       bBCM_Security_Efuse_Read_Disable_Row_0       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_0          0x00000001

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_1       0x001C
    #define     B16BCM_Security_Efuse_Read_Disable_Row_1       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_1          1
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_1          1
    #define       bBCM_Security_Efuse_Read_Disable_Row_1       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_1          0x00000002

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_2       0x001C
    #define     B16BCM_Security_Efuse_Read_Disable_Row_2       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_2          2
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_2          2
    #define       bBCM_Security_Efuse_Read_Disable_Row_2       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_2          0x00000004

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_3       0x001C
    #define     B16BCM_Security_Efuse_Read_Disable_Row_3       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_3          3
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_3          3
    #define       bBCM_Security_Efuse_Read_Disable_Row_3       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_3          0x00000008

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_4       0x001C
    #define     B16BCM_Security_Efuse_Read_Disable_Row_4       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_4          4
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_4          4
    #define       bBCM_Security_Efuse_Read_Disable_Row_4       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_4          0x00000010

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_5       0x001C
    #define     B16BCM_Security_Efuse_Read_Disable_Row_5       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_5          5
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_5          5
    #define       bBCM_Security_Efuse_Read_Disable_Row_5       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_5          0x00000020

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_6       0x001C
    #define     B16BCM_Security_Efuse_Read_Disable_Row_6       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_6          6
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_6          6
    #define       bBCM_Security_Efuse_Read_Disable_Row_6       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_6          0x00000040

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_7       0x001C
    #define     B16BCM_Security_Efuse_Read_Disable_Row_7       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_7          7
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_7          7
    #define       bBCM_Security_Efuse_Read_Disable_Row_7       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_7          0x00000080

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_8       0x001D
    #define     B16BCM_Security_Efuse_Read_Disable_Row_8       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_8          8
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_8          8
    #define       bBCM_Security_Efuse_Read_Disable_Row_8       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_8          0x00000100

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_9       0x001D
    #define     B16BCM_Security_Efuse_Read_Disable_Row_9       0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_9          9
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_9          9
    #define       bBCM_Security_Efuse_Read_Disable_Row_9       1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_9          0x00000200

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_10      0x001D
    #define     B16BCM_Security_Efuse_Read_Disable_Row_10      0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_10         10
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_10         10
    #define       bBCM_Security_Efuse_Read_Disable_Row_10      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_10         0x00000400

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_11      0x001D
    #define     B16BCM_Security_Efuse_Read_Disable_Row_11      0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_11         11
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_11         11
    #define       bBCM_Security_Efuse_Read_Disable_Row_11      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_11         0x00000800

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_12      0x001D
    #define     B16BCM_Security_Efuse_Read_Disable_Row_12      0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_12         12
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_12         12
    #define       bBCM_Security_Efuse_Read_Disable_Row_12      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_12         0x00001000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_13      0x001D
    #define     B16BCM_Security_Efuse_Read_Disable_Row_13      0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_13         13
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_13         13
    #define       bBCM_Security_Efuse_Read_Disable_Row_13      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_13         0x00002000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_14      0x001D
    #define     B16BCM_Security_Efuse_Read_Disable_Row_14      0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_14         14
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_14         14
    #define       bBCM_Security_Efuse_Read_Disable_Row_14      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_14         0x00004000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_15      0x001D
    #define     B16BCM_Security_Efuse_Read_Disable_Row_15      0x001C
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_15         15
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_15         15
    #define       bBCM_Security_Efuse_Read_Disable_Row_15      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_15         0x00008000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_16      0x001E
    #define     B16BCM_Security_Efuse_Read_Disable_Row_16      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_16         16
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_16         0
    #define       bBCM_Security_Efuse_Read_Disable_Row_16      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_16         0x00010000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_17      0x001E
    #define     B16BCM_Security_Efuse_Read_Disable_Row_17      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_17         17
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_17         1
    #define       bBCM_Security_Efuse_Read_Disable_Row_17      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_17         0x00020000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_18      0x001E
    #define     B16BCM_Security_Efuse_Read_Disable_Row_18      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_18         18
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_18         2
    #define       bBCM_Security_Efuse_Read_Disable_Row_18      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_18         0x00040000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_19      0x001E
    #define     B16BCM_Security_Efuse_Read_Disable_Row_19      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_19         19
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_19         3
    #define       bBCM_Security_Efuse_Read_Disable_Row_19      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_19         0x00080000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_20      0x001E
    #define     B16BCM_Security_Efuse_Read_Disable_Row_20      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_20         20
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_20         4
    #define       bBCM_Security_Efuse_Read_Disable_Row_20      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_20         0x00100000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_21      0x001E
    #define     B16BCM_Security_Efuse_Read_Disable_Row_21      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_21         21
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_21         5
    #define       bBCM_Security_Efuse_Read_Disable_Row_21      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_21         0x00200000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_22      0x001E
    #define     B16BCM_Security_Efuse_Read_Disable_Row_22      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_22         22
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_22         6
    #define       bBCM_Security_Efuse_Read_Disable_Row_22      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_22         0x00400000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_23      0x001E
    #define     B16BCM_Security_Efuse_Read_Disable_Row_23      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_23         23
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_23         7
    #define       bBCM_Security_Efuse_Read_Disable_Row_23      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_23         0x00800000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_24      0x001F
    #define     B16BCM_Security_Efuse_Read_Disable_Row_24      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_24         24
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_24         8
    #define       bBCM_Security_Efuse_Read_Disable_Row_24      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_24         0x01000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_25      0x001F
    #define     B16BCM_Security_Efuse_Read_Disable_Row_25      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_25         25
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_25         9
    #define       bBCM_Security_Efuse_Read_Disable_Row_25      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_25         0x02000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_26      0x001F
    #define     B16BCM_Security_Efuse_Read_Disable_Row_26      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_26         26
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_26         10
    #define       bBCM_Security_Efuse_Read_Disable_Row_26      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_26         0x04000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_27      0x001F
    #define     B16BCM_Security_Efuse_Read_Disable_Row_27      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_27         27
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_27         11
    #define       bBCM_Security_Efuse_Read_Disable_Row_27      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_27         0x08000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_28      0x001F
    #define     B16BCM_Security_Efuse_Read_Disable_Row_28      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_28         28
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_28         12
    #define       bBCM_Security_Efuse_Read_Disable_Row_28      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_28         0x10000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_29      0x001F
    #define     B16BCM_Security_Efuse_Read_Disable_Row_29      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_29         29
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_29         13
    #define       bBCM_Security_Efuse_Read_Disable_Row_29      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_29         0x20000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_30      0x001F
    #define     B16BCM_Security_Efuse_Read_Disable_Row_30      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_30         30
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_30         14
    #define       bBCM_Security_Efuse_Read_Disable_Row_30      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_30         0x40000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_31      0x001F
    #define     B16BCM_Security_Efuse_Read_Disable_Row_31      0x001E
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_31         31
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_31         15
    #define       bBCM_Security_Efuse_Read_Disable_Row_31      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_31         0x80000000

    #define     RA_BCM_Security_Efuse_Read_Disable1            0x0020

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_32      0x0020
    #define     B16BCM_Security_Efuse_Read_Disable_Row_32      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_32         0
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_32         0
    #define       bBCM_Security_Efuse_Read_Disable_Row_32      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_32         0x00000001

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_33      0x0020
    #define     B16BCM_Security_Efuse_Read_Disable_Row_33      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_33         1
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_33         1
    #define       bBCM_Security_Efuse_Read_Disable_Row_33      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_33         0x00000002

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_34      0x0020
    #define     B16BCM_Security_Efuse_Read_Disable_Row_34      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_34         2
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_34         2
    #define       bBCM_Security_Efuse_Read_Disable_Row_34      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_34         0x00000004

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_35      0x0020
    #define     B16BCM_Security_Efuse_Read_Disable_Row_35      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_35         3
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_35         3
    #define       bBCM_Security_Efuse_Read_Disable_Row_35      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_35         0x00000008

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_36      0x0020
    #define     B16BCM_Security_Efuse_Read_Disable_Row_36      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_36         4
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_36         4
    #define       bBCM_Security_Efuse_Read_Disable_Row_36      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_36         0x00000010

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_37      0x0020
    #define     B16BCM_Security_Efuse_Read_Disable_Row_37      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_37         5
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_37         5
    #define       bBCM_Security_Efuse_Read_Disable_Row_37      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_37         0x00000020

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_38      0x0020
    #define     B16BCM_Security_Efuse_Read_Disable_Row_38      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_38         6
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_38         6
    #define       bBCM_Security_Efuse_Read_Disable_Row_38      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_38         0x00000040

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_39      0x0020
    #define     B16BCM_Security_Efuse_Read_Disable_Row_39      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_39         7
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_39         7
    #define       bBCM_Security_Efuse_Read_Disable_Row_39      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_39         0x00000080

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_40      0x0021
    #define     B16BCM_Security_Efuse_Read_Disable_Row_40      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_40         8
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_40         8
    #define       bBCM_Security_Efuse_Read_Disable_Row_40      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_40         0x00000100

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_41      0x0021
    #define     B16BCM_Security_Efuse_Read_Disable_Row_41      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_41         9
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_41         9
    #define       bBCM_Security_Efuse_Read_Disable_Row_41      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_41         0x00000200

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_42      0x0021
    #define     B16BCM_Security_Efuse_Read_Disable_Row_42      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_42         10
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_42         10
    #define       bBCM_Security_Efuse_Read_Disable_Row_42      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_42         0x00000400

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_43      0x0021
    #define     B16BCM_Security_Efuse_Read_Disable_Row_43      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_43         11
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_43         11
    #define       bBCM_Security_Efuse_Read_Disable_Row_43      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_43         0x00000800

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_44      0x0021
    #define     B16BCM_Security_Efuse_Read_Disable_Row_44      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_44         12
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_44         12
    #define       bBCM_Security_Efuse_Read_Disable_Row_44      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_44         0x00001000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_45      0x0021
    #define     B16BCM_Security_Efuse_Read_Disable_Row_45      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_45         13
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_45         13
    #define       bBCM_Security_Efuse_Read_Disable_Row_45      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_45         0x00002000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_46      0x0021
    #define     B16BCM_Security_Efuse_Read_Disable_Row_46      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_46         14
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_46         14
    #define       bBCM_Security_Efuse_Read_Disable_Row_46      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_46         0x00004000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_47      0x0021
    #define     B16BCM_Security_Efuse_Read_Disable_Row_47      0x0020
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_47         15
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_47         15
    #define       bBCM_Security_Efuse_Read_Disable_Row_47      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_47         0x00008000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_48      0x0022
    #define     B16BCM_Security_Efuse_Read_Disable_Row_48      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_48         16
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_48         0
    #define       bBCM_Security_Efuse_Read_Disable_Row_48      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_48         0x00010000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_49      0x0022
    #define     B16BCM_Security_Efuse_Read_Disable_Row_49      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_49         17
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_49         1
    #define       bBCM_Security_Efuse_Read_Disable_Row_49      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_49         0x00020000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_50      0x0022
    #define     B16BCM_Security_Efuse_Read_Disable_Row_50      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_50         18
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_50         2
    #define       bBCM_Security_Efuse_Read_Disable_Row_50      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_50         0x00040000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_51      0x0022
    #define     B16BCM_Security_Efuse_Read_Disable_Row_51      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_51         19
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_51         3
    #define       bBCM_Security_Efuse_Read_Disable_Row_51      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_51         0x00080000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_52      0x0022
    #define     B16BCM_Security_Efuse_Read_Disable_Row_52      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_52         20
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_52         4
    #define       bBCM_Security_Efuse_Read_Disable_Row_52      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_52         0x00100000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_53      0x0022
    #define     B16BCM_Security_Efuse_Read_Disable_Row_53      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_53         21
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_53         5
    #define       bBCM_Security_Efuse_Read_Disable_Row_53      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_53         0x00200000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_54      0x0022
    #define     B16BCM_Security_Efuse_Read_Disable_Row_54      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_54         22
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_54         6
    #define       bBCM_Security_Efuse_Read_Disable_Row_54      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_54         0x00400000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_55      0x0022
    #define     B16BCM_Security_Efuse_Read_Disable_Row_55      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_55         23
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_55         7
    #define       bBCM_Security_Efuse_Read_Disable_Row_55      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_55         0x00800000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_56      0x0023
    #define     B16BCM_Security_Efuse_Read_Disable_Row_56      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_56         24
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_56         8
    #define       bBCM_Security_Efuse_Read_Disable_Row_56      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_56         0x01000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_57      0x0023
    #define     B16BCM_Security_Efuse_Read_Disable_Row_57      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_57         25
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_57         9
    #define       bBCM_Security_Efuse_Read_Disable_Row_57      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_57         0x02000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_58      0x0023
    #define     B16BCM_Security_Efuse_Read_Disable_Row_58      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_58         26
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_58         10
    #define       bBCM_Security_Efuse_Read_Disable_Row_58      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_58         0x04000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_59      0x0023
    #define     B16BCM_Security_Efuse_Read_Disable_Row_59      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_59         27
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_59         11
    #define       bBCM_Security_Efuse_Read_Disable_Row_59      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_59         0x08000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_60      0x0023
    #define     B16BCM_Security_Efuse_Read_Disable_Row_60      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_60         28
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_60         12
    #define       bBCM_Security_Efuse_Read_Disable_Row_60      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_60         0x10000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_61      0x0023
    #define     B16BCM_Security_Efuse_Read_Disable_Row_61      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_61         29
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_61         13
    #define       bBCM_Security_Efuse_Read_Disable_Row_61      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_61         0x20000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_62      0x0023
    #define     B16BCM_Security_Efuse_Read_Disable_Row_62      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_62         30
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_62         14
    #define       bBCM_Security_Efuse_Read_Disable_Row_62      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_62         0x40000000

    #define     BA_BCM_Security_Efuse_Read_Disable_Row_63      0x0023
    #define     B16BCM_Security_Efuse_Read_Disable_Row_63      0x0022
    #define   LSb32BCM_Security_Efuse_Read_Disable_Row_63         31
    #define   LSb16BCM_Security_Efuse_Read_Disable_Row_63         15
    #define       bBCM_Security_Efuse_Read_Disable_Row_63      1
    #define   MSK32BCM_Security_Efuse_Read_Disable_Row_63         0x80000000
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_RKEKReg                        0x002C
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_RKEKRegReadDisable             0x004C

    #define     BA_BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable 0x004C
    #define     B16BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable 0x004C
    #define   LSb32BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable    0
    #define   LSb16BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable    0
    #define       bBCM_Security_RKEKRegReadDisable_RKEKRegReadDisable 1
    #define   MSK32BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_RKEKRegWriteDisable            0x0050

    #define     BA_BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable 0x0050
    #define     B16BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable 0x0050
    #define   LSb32BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable    0
    #define   LSb16BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable    0
    #define       bBCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable 1
    #define   MSK32BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_FeatureCtrlReg                 0x0054

    #define     BA_BCM_Security_FeatureCtrlReg_V2G_Fuse        0x0054
    #define     B16BCM_Security_FeatureCtrlReg_V2G_Fuse        0x0054
    #define   LSb32BCM_Security_FeatureCtrlReg_V2G_Fuse           0
    #define   LSb16BCM_Security_FeatureCtrlReg_V2G_Fuse           0
    #define       bBCM_Security_FeatureCtrlReg_V2G_Fuse        32
    #define   MSK32BCM_Security_FeatureCtrlReg_V2G_Fuse           0xFFFFFFFF

    #define     RA_BCM_Security_FeatureCtrlReg1                0x0058

    #define     BA_BCM_Security_FeatureCtrlReg_Hantro_Fuse_Dec 0x0058
    #define     B16BCM_Security_FeatureCtrlReg_Hantro_Fuse_Dec 0x0058
    #define   LSb32BCM_Security_FeatureCtrlReg_Hantro_Fuse_Dec    0
    #define   LSb16BCM_Security_FeatureCtrlReg_Hantro_Fuse_Dec    0
    #define       bBCM_Security_FeatureCtrlReg_Hantro_Fuse_Dec 32
    #define   MSK32BCM_Security_FeatureCtrlReg_Hantro_Fuse_Dec    0xFFFFFFFF

    #define     RA_BCM_Security_FeatureCtrlReg2                0x005C

    #define     BA_BCM_Security_FeatureCtrlReg_Hantro_Fuse_PP  0x005C
    #define     B16BCM_Security_FeatureCtrlReg_Hantro_Fuse_PP  0x005C
    #define   LSb32BCM_Security_FeatureCtrlReg_Hantro_Fuse_PP     0
    #define   LSb16BCM_Security_FeatureCtrlReg_Hantro_Fuse_PP     0
    #define       bBCM_Security_FeatureCtrlReg_Hantro_Fuse_PP  32
    #define   MSK32BCM_Security_FeatureCtrlReg_Hantro_Fuse_PP     0xFFFFFFFF

    #define     RA_BCM_Security_FeatureCtrlReg3                0x0060

    #define     BA_BCM_Security_FeatureCtrlReg_Hantro_Fuse_Enc 0x0060
    #define     B16BCM_Security_FeatureCtrlReg_Hantro_Fuse_Enc 0x0060
    #define   LSb32BCM_Security_FeatureCtrlReg_Hantro_Fuse_Enc    0
    #define   LSb16BCM_Security_FeatureCtrlReg_Hantro_Fuse_Enc    0
    #define       bBCM_Security_FeatureCtrlReg_Hantro_Fuse_Enc 32
    #define   MSK32BCM_Security_FeatureCtrlReg_Hantro_Fuse_Enc    0xFFFFFFFF

    #define     RA_BCM_Security_FeatureCtrlReg4                0x0064

    #define     BA_BCM_Security_FeatureCtrlReg_GPU_Feature     0x0064
    #define     B16BCM_Security_FeatureCtrlReg_GPU_Feature     0x0064
    #define   LSb32BCM_Security_FeatureCtrlReg_GPU_Feature        0
    #define   LSb16BCM_Security_FeatureCtrlReg_GPU_Feature        0
    #define       bBCM_Security_FeatureCtrlReg_GPU_Feature     32
    #define   MSK32BCM_Security_FeatureCtrlReg_GPU_Feature        0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_AVIO                           0x0068
    ///////////////////////////////////////////////////////////

    #define     BA_BCM_Security_Reseved_6                      0x0078
    #define     B16BCM_Security_Reseved_6                      0x0078
    #define   LSb32BCM_Security_Reseved_6                         0
    #define   LSb16BCM_Security_Reseved_6                         0
    #define       bBCM_Security_Reseved_6                      32
    #define   MSK32BCM_Security_Reseved_6                         0xFFFFFFFF

    #define     BA_BCM_Security_Reseved_7                      0x007C
    #define     B16BCM_Security_Reseved_7                      0x007C
    #define   LSb32BCM_Security_Reseved_7                         0
    #define   LSb16BCM_Security_Reseved_7                         0
    #define       bBCM_Security_Reseved_7                      32
    #define   MSK32BCM_Security_Reseved_7                         0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_Golden_CRC                0x0080

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_0        0x0080
    #define     B16BCM_Security_EROM_Golden_CRC_Block_0        0x0080
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_0           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_0           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_0        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_0           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC1               0x0084

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_1        0x0084
    #define     B16BCM_Security_EROM_Golden_CRC_Block_1        0x0084
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_1           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_1           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_1        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_1           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC2               0x0088

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_2        0x0088
    #define     B16BCM_Security_EROM_Golden_CRC_Block_2        0x0088
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_2           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_2           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_2        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_2           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC3               0x008C

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_3        0x008C
    #define     B16BCM_Security_EROM_Golden_CRC_Block_3        0x008C
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_3           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_3           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_3        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_3           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC4               0x0090

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_4        0x0090
    #define     B16BCM_Security_EROM_Golden_CRC_Block_4        0x0090
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_4           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_4           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_4        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_4           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC5               0x0094

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_5        0x0094
    #define     B16BCM_Security_EROM_Golden_CRC_Block_5        0x0094
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_5           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_5           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_5        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_5           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC6               0x0098

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_6        0x0098
    #define     B16BCM_Security_EROM_Golden_CRC_Block_6        0x0098
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_6           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_6           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_6        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_6           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC7               0x009C

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_7        0x009C
    #define     B16BCM_Security_EROM_Golden_CRC_Block_7        0x009C
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_7           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_7           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_7        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_7           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC8               0x00A0

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_8        0x00A0
    #define     B16BCM_Security_EROM_Golden_CRC_Block_8        0x00A0
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_8           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_8           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_8        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_8           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC9               0x00A4

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_9        0x00A4
    #define     B16BCM_Security_EROM_Golden_CRC_Block_9        0x00A4
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_9           0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_9           0
    #define       bBCM_Security_EROM_Golden_CRC_Block_9        32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_9           0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC10              0x00A8

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_10       0x00A8
    #define     B16BCM_Security_EROM_Golden_CRC_Block_10       0x00A8
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_10          0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_10          0
    #define       bBCM_Security_EROM_Golden_CRC_Block_10       32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_10          0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC11              0x00AC

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_11       0x00AC
    #define     B16BCM_Security_EROM_Golden_CRC_Block_11       0x00AC
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_11          0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_11          0
    #define       bBCM_Security_EROM_Golden_CRC_Block_11       32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_11          0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC12              0x00B0

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_12       0x00B0
    #define     B16BCM_Security_EROM_Golden_CRC_Block_12       0x00B0
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_12          0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_12          0
    #define       bBCM_Security_EROM_Golden_CRC_Block_12       32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_12          0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC13              0x00B4

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_13       0x00B4
    #define     B16BCM_Security_EROM_Golden_CRC_Block_13       0x00B4
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_13          0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_13          0
    #define       bBCM_Security_EROM_Golden_CRC_Block_13       32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_13          0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC14              0x00B8

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_14       0x00B8
    #define     B16BCM_Security_EROM_Golden_CRC_Block_14       0x00B8
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_14          0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_14          0
    #define       bBCM_Security_EROM_Golden_CRC_Block_14       32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_14          0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Golden_CRC15              0x00BC

    #define     BA_BCM_Security_EROM_Golden_CRC_Block_15       0x00BC
    #define     B16BCM_Security_EROM_Golden_CRC_Block_15       0x00BC
    #define   LSb32BCM_Security_EROM_Golden_CRC_Block_15          0
    #define   LSb16BCM_Security_EROM_Golden_CRC_Block_15          0
    #define       bBCM_Security_EROM_Golden_CRC_Block_15       32
    #define   MSK32BCM_Security_EROM_Golden_CRC_Block_15          0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_BackgroundCheck_Start_Addresss 0x00C0

    #define     BA_BCM_Security_EROM_BackgroundCheck_Start_Addresss_Start_Address 0x00C0
    #define     B16BCM_Security_EROM_BackgroundCheck_Start_Addresss_Start_Address 0x00C0
    #define   LSb32BCM_Security_EROM_BackgroundCheck_Start_Addresss_Start_Address    0
    #define   LSb16BCM_Security_EROM_BackgroundCheck_Start_Addresss_Start_Address    0
    #define       bBCM_Security_EROM_BackgroundCheck_Start_Addresss_Start_Address 32
    #define   MSK32BCM_Security_EROM_BackgroundCheck_Start_Addresss_Start_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_BackgroundCheck_End_Addresss 0x00C4

    #define     BA_BCM_Security_EROM_BackgroundCheck_End_Addresss_End_Address 0x00C4
    #define     B16BCM_Security_EROM_BackgroundCheck_End_Addresss_End_Address 0x00C4
    #define   LSb32BCM_Security_EROM_BackgroundCheck_End_Addresss_End_Address    0
    #define   LSb16BCM_Security_EROM_BackgroundCheck_End_Addresss_End_Address    0
    #define       bBCM_Security_EROM_BackgroundCheck_End_Addresss_End_Address 32
    #define   MSK32BCM_Security_EROM_BackgroundCheck_End_Addresss_End_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_BackgroundCheck_Start     0x00C8

    #define     BA_BCM_Security_EROM_BackgroundCheck_Start_Start 0x00C8
    #define     B16BCM_Security_EROM_BackgroundCheck_Start_Start 0x00C8
    #define   LSb32BCM_Security_EROM_BackgroundCheck_Start_Start    0
    #define   LSb16BCM_Security_EROM_BackgroundCheck_Start_Start    0
    #define       bBCM_Security_EROM_BackgroundCheck_Start_Start 4
    #define   MSK32BCM_Security_EROM_BackgroundCheck_Start_Start    0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_BackgroundCheck_Password  0x00CC

    #define     BA_BCM_Security_EROM_BackgroundCheck_Password_Password 0x00CC
    #define     B16BCM_Security_EROM_BackgroundCheck_Password_Password 0x00CC
    #define   LSb32BCM_Security_EROM_BackgroundCheck_Password_Password    0
    #define   LSb16BCM_Security_EROM_BackgroundCheck_Password_Password    0
    #define       bBCM_Security_EROM_BackgroundCheck_Password_Password 32
    #define   MSK32BCM_Security_EROM_BackgroundCheck_Password_Password    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_BackgroundCheck_Password_match 0x00D0

    #define     BA_BCM_Security_EROM_BackgroundCheck_Password_match_Password_match 0x00D0
    #define     B16BCM_Security_EROM_BackgroundCheck_Password_match_Password_match 0x00D0
    #define   LSb32BCM_Security_EROM_BackgroundCheck_Password_match_Password_match    0
    #define   LSb16BCM_Security_EROM_BackgroundCheck_Password_match_Password_match    0
    #define       bBCM_Security_EROM_BackgroundCheck_Password_match_Password_match 1
    #define   MSK32BCM_Security_EROM_BackgroundCheck_Password_match_Password_match    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_SCS_Start_Address         0x00D4

    #define     BA_BCM_Security_IROM_SCS_Start_Address_Start_Address 0x00D4
    #define     B16BCM_Security_IROM_SCS_Start_Address_Start_Address 0x00D4
    #define   LSb32BCM_Security_IROM_SCS_Start_Address_Start_Address    0
    #define   LSb16BCM_Security_IROM_SCS_Start_Address_Start_Address    0
    #define       bBCM_Security_IROM_SCS_Start_Address_Start_Address 32
    #define   MSK32BCM_Security_IROM_SCS_Start_Address_Start_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_SCS_End_Address           0x00D8

    #define     BA_BCM_Security_IROM_SCS_End_Address_End_Address 0x00D8
    #define     B16BCM_Security_IROM_SCS_End_Address_End_Address 0x00D8
    #define   LSb32BCM_Security_IROM_SCS_End_Address_End_Address    0
    #define   LSb16BCM_Security_IROM_SCS_End_Address_End_Address    0
    #define       bBCM_Security_IROM_SCS_End_Address_End_Address 32
    #define   MSK32BCM_Security_IROM_SCS_End_Address_End_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_SCS_Mask                  0x00DC

    #define     BA_BCM_Security_IROM_SCS_Mask_Mask             0x00DC
    #define     B16BCM_Security_IROM_SCS_Mask_Mask             0x00DC
    #define   LSb32BCM_Security_IROM_SCS_Mask_Mask                0
    #define   LSb16BCM_Security_IROM_SCS_Mask_Mask                0
    #define       bBCM_Security_IROM_SCS_Mask_Mask             4
    #define   MSK32BCM_Security_IROM_SCS_Mask_Mask                0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_BCM_reset_2_soc                0x00E0

    #define     BA_BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc 0x00E0
    #define     B16BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc 0x00E0
    #define   LSb32BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc    0
    #define   LSb16BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc    0
    #define       bBCM_Security_BCM_reset_2_soc_BCM_reset_2_soc 1
    #define   MSK32BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_InstructionFilter_Start   0x00E4

    #define     BA_BCM_Security_EROM_InstructionFilter_Start_Start 0x00E4
    #define     B16BCM_Security_EROM_InstructionFilter_Start_Start 0x00E4
    #define   LSb32BCM_Security_EROM_InstructionFilter_Start_Start    0
    #define   LSb16BCM_Security_EROM_InstructionFilter_Start_Start    0
    #define       bBCM_Security_EROM_InstructionFilter_Start_Start 4
    #define   MSK32BCM_Security_EROM_InstructionFilter_Start_Start    0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_InstructionFilter_Lock    0x00E8

    #define     BA_BCM_Security_EROM_InstructionFilter_Lock_Lock 0x00E8
    #define     B16BCM_Security_EROM_InstructionFilter_Lock_Lock 0x00E8
    #define   LSb32BCM_Security_EROM_InstructionFilter_Lock_Lock    0
    #define   LSb16BCM_Security_EROM_InstructionFilter_Lock_Lock    0
    #define       bBCM_Security_EROM_InstructionFilter_Lock_Lock 4
    #define   MSK32BCM_Security_EROM_InstructionFilter_Lock_Lock    0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_Instruction_Range         0x00F0

    #define     BA_BCM_Security_EROM_Instruction_Range_Range0_Low 0x00F0
    #define     B16BCM_Security_EROM_Instruction_Range_Range0_Low 0x00F0
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range0_Low    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range0_Low    0
    #define       bBCM_Security_EROM_Instruction_Range_Range0_Low 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range0_Low    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range1        0x00F4

    #define     BA_BCM_Security_EROM_Instruction_Range_Range0_High 0x00F4
    #define     B16BCM_Security_EROM_Instruction_Range_Range0_High 0x00F4
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range0_High    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range0_High    0
    #define       bBCM_Security_EROM_Instruction_Range_Range0_High 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range0_High    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range2        0x00F8

    #define     BA_BCM_Security_EROM_Instruction_Range_Range1_Low 0x00F8
    #define     B16BCM_Security_EROM_Instruction_Range_Range1_Low 0x00F8
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range1_Low    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range1_Low    0
    #define       bBCM_Security_EROM_Instruction_Range_Range1_Low 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range1_Low    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range3        0x00FC

    #define     BA_BCM_Security_EROM_Instruction_Range_Range1_High 0x00FC
    #define     B16BCM_Security_EROM_Instruction_Range_Range1_High 0x00FC
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range1_High    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range1_High    0
    #define       bBCM_Security_EROM_Instruction_Range_Range1_High 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range1_High    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range4        0x0100

    #define     BA_BCM_Security_EROM_Instruction_Range_Range2_Low 0x0100
    #define     B16BCM_Security_EROM_Instruction_Range_Range2_Low 0x0100
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range2_Low    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range2_Low    0
    #define       bBCM_Security_EROM_Instruction_Range_Range2_Low 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range2_Low    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range5        0x0104

    #define     BA_BCM_Security_EROM_Instruction_Range_Range2_High 0x0104
    #define     B16BCM_Security_EROM_Instruction_Range_Range2_High 0x0104
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range2_High    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range2_High    0
    #define       bBCM_Security_EROM_Instruction_Range_Range2_High 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range2_High    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range6        0x0108

    #define     BA_BCM_Security_EROM_Instruction_Range_Range3_Low 0x0108
    #define     B16BCM_Security_EROM_Instruction_Range_Range3_Low 0x0108
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range3_Low    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range3_Low    0
    #define       bBCM_Security_EROM_Instruction_Range_Range3_Low 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range3_Low    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range7        0x010C

    #define     BA_BCM_Security_EROM_Instruction_Range_Range3_High 0x010C
    #define     B16BCM_Security_EROM_Instruction_Range_Range3_High 0x010C
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range3_High    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range3_High    0
    #define       bBCM_Security_EROM_Instruction_Range_Range3_High 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range3_High    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range8        0x0110

    #define     BA_BCM_Security_EROM_Instruction_Range_Range4_Low 0x0110
    #define     B16BCM_Security_EROM_Instruction_Range_Range4_Low 0x0110
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range4_Low    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range4_Low    0
    #define       bBCM_Security_EROM_Instruction_Range_Range4_Low 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range4_Low    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range9        0x0114

    #define     BA_BCM_Security_EROM_Instruction_Range_Range4_High 0x0114
    #define     B16BCM_Security_EROM_Instruction_Range_Range4_High 0x0114
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range4_High    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range4_High    0
    #define       bBCM_Security_EROM_Instruction_Range_Range4_High 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range4_High    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range10       0x0118

    #define     BA_BCM_Security_EROM_Instruction_Range_Range5_Low 0x0118
    #define     B16BCM_Security_EROM_Instruction_Range_Range5_Low 0x0118
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range5_Low    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range5_Low    0
    #define       bBCM_Security_EROM_Instruction_Range_Range5_Low 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range5_Low    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range11       0x011C

    #define     BA_BCM_Security_EROM_Instruction_Range_Range5_High 0x011C
    #define     B16BCM_Security_EROM_Instruction_Range_Range5_High 0x011C
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range5_High    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range5_High    0
    #define       bBCM_Security_EROM_Instruction_Range_Range5_High 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range5_High    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range12       0x0120

    #define     BA_BCM_Security_EROM_Instruction_Range_Range6_Low 0x0120
    #define     B16BCM_Security_EROM_Instruction_Range_Range6_Low 0x0120
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range6_Low    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range6_Low    0
    #define       bBCM_Security_EROM_Instruction_Range_Range6_Low 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range6_Low    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range13       0x0124

    #define     BA_BCM_Security_EROM_Instruction_Range_Range6_High 0x0124
    #define     B16BCM_Security_EROM_Instruction_Range_Range6_High 0x0124
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range6_High    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range6_High    0
    #define       bBCM_Security_EROM_Instruction_Range_Range6_High 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range6_High    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range14       0x0128

    #define     BA_BCM_Security_EROM_Instruction_Range_Range7_Low 0x0128
    #define     B16BCM_Security_EROM_Instruction_Range_Range7_Low 0x0128
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range7_Low    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range7_Low    0
    #define       bBCM_Security_EROM_Instruction_Range_Range7_Low 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range7_Low    0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Instruction_Range15       0x012C

    #define     BA_BCM_Security_EROM_Instruction_Range_Range7_High 0x012C
    #define     B16BCM_Security_EROM_Instruction_Range_Range7_High 0x012C
    #define   LSb32BCM_Security_EROM_Instruction_Range_Range7_High    0
    #define   LSb16BCM_Security_EROM_Instruction_Range_Range7_High    0
    #define       bBCM_Security_EROM_Instruction_Range_Range7_High 32
    #define   MSK32BCM_Security_EROM_Instruction_Range_Range7_High    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_DataFilter_Start          0x0180

    #define     BA_BCM_Security_EROM_DataFilter_Start_Start    0x0180
    #define     B16BCM_Security_EROM_DataFilter_Start_Start    0x0180
    #define   LSb32BCM_Security_EROM_DataFilter_Start_Start       0
    #define   LSb16BCM_Security_EROM_DataFilter_Start_Start       0
    #define       bBCM_Security_EROM_DataFilter_Start_Start    4
    #define   MSK32BCM_Security_EROM_DataFilter_Start_Start       0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_DataFilter_Lock           0x0184

    #define     BA_BCM_Security_EROM_DataFilter_Lock_Lock      0x0184
    #define     B16BCM_Security_EROM_DataFilter_Lock_Lock      0x0184
    #define   LSb32BCM_Security_EROM_DataFilter_Lock_Lock         0
    #define   LSb16BCM_Security_EROM_DataFilter_Lock_Lock         0
    #define       bBCM_Security_EROM_DataFilter_Lock_Lock      4
    #define   MSK32BCM_Security_EROM_DataFilter_Lock_Lock         0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EROM_Data_Range                0x0190

    #define     BA_BCM_Security_EROM_Data_Range_Range0_Low     0x0190
    #define     B16BCM_Security_EROM_Data_Range_Range0_Low     0x0190
    #define   LSb32BCM_Security_EROM_Data_Range_Range0_Low        0
    #define   LSb16BCM_Security_EROM_Data_Range_Range0_Low        0
    #define       bBCM_Security_EROM_Data_Range_Range0_Low     32
    #define   MSK32BCM_Security_EROM_Data_Range_Range0_Low        0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range1               0x0194

    #define     BA_BCM_Security_EROM_Data_Range_Range0_High    0x0194
    #define     B16BCM_Security_EROM_Data_Range_Range0_High    0x0194
    #define   LSb32BCM_Security_EROM_Data_Range_Range0_High       0
    #define   LSb16BCM_Security_EROM_Data_Range_Range0_High       0
    #define       bBCM_Security_EROM_Data_Range_Range0_High    32
    #define   MSK32BCM_Security_EROM_Data_Range_Range0_High       0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range2               0x0198

    #define     BA_BCM_Security_EROM_Data_Range_Range1_Low     0x0198
    #define     B16BCM_Security_EROM_Data_Range_Range1_Low     0x0198
    #define   LSb32BCM_Security_EROM_Data_Range_Range1_Low        0
    #define   LSb16BCM_Security_EROM_Data_Range_Range1_Low        0
    #define       bBCM_Security_EROM_Data_Range_Range1_Low     32
    #define   MSK32BCM_Security_EROM_Data_Range_Range1_Low        0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range3               0x019C

    #define     BA_BCM_Security_EROM_Data_Range_Range1_High    0x019C
    #define     B16BCM_Security_EROM_Data_Range_Range1_High    0x019C
    #define   LSb32BCM_Security_EROM_Data_Range_Range1_High       0
    #define   LSb16BCM_Security_EROM_Data_Range_Range1_High       0
    #define       bBCM_Security_EROM_Data_Range_Range1_High    32
    #define   MSK32BCM_Security_EROM_Data_Range_Range1_High       0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range4               0x01A0

    #define     BA_BCM_Security_EROM_Data_Range_Range2_Low     0x01A0
    #define     B16BCM_Security_EROM_Data_Range_Range2_Low     0x01A0
    #define   LSb32BCM_Security_EROM_Data_Range_Range2_Low        0
    #define   LSb16BCM_Security_EROM_Data_Range_Range2_Low        0
    #define       bBCM_Security_EROM_Data_Range_Range2_Low     32
    #define   MSK32BCM_Security_EROM_Data_Range_Range2_Low        0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range5               0x01A4

    #define     BA_BCM_Security_EROM_Data_Range_Range2_High    0x01A4
    #define     B16BCM_Security_EROM_Data_Range_Range2_High    0x01A4
    #define   LSb32BCM_Security_EROM_Data_Range_Range2_High       0
    #define   LSb16BCM_Security_EROM_Data_Range_Range2_High       0
    #define       bBCM_Security_EROM_Data_Range_Range2_High    32
    #define   MSK32BCM_Security_EROM_Data_Range_Range2_High       0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range6               0x01A8

    #define     BA_BCM_Security_EROM_Data_Range_Range3_Low     0x01A8
    #define     B16BCM_Security_EROM_Data_Range_Range3_Low     0x01A8
    #define   LSb32BCM_Security_EROM_Data_Range_Range3_Low        0
    #define   LSb16BCM_Security_EROM_Data_Range_Range3_Low        0
    #define       bBCM_Security_EROM_Data_Range_Range3_Low     32
    #define   MSK32BCM_Security_EROM_Data_Range_Range3_Low        0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range7               0x01AC

    #define     BA_BCM_Security_EROM_Data_Range_Range3_High    0x01AC
    #define     B16BCM_Security_EROM_Data_Range_Range3_High    0x01AC
    #define   LSb32BCM_Security_EROM_Data_Range_Range3_High       0
    #define   LSb16BCM_Security_EROM_Data_Range_Range3_High       0
    #define       bBCM_Security_EROM_Data_Range_Range3_High    32
    #define   MSK32BCM_Security_EROM_Data_Range_Range3_High       0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range8               0x01B0

    #define     BA_BCM_Security_EROM_Data_Range_Range4_Low     0x01B0
    #define     B16BCM_Security_EROM_Data_Range_Range4_Low     0x01B0
    #define   LSb32BCM_Security_EROM_Data_Range_Range4_Low        0
    #define   LSb16BCM_Security_EROM_Data_Range_Range4_Low        0
    #define       bBCM_Security_EROM_Data_Range_Range4_Low     32
    #define   MSK32BCM_Security_EROM_Data_Range_Range4_Low        0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range9               0x01B4

    #define     BA_BCM_Security_EROM_Data_Range_Range4_High    0x01B4
    #define     B16BCM_Security_EROM_Data_Range_Range4_High    0x01B4
    #define   LSb32BCM_Security_EROM_Data_Range_Range4_High       0
    #define   LSb16BCM_Security_EROM_Data_Range_Range4_High       0
    #define       bBCM_Security_EROM_Data_Range_Range4_High    32
    #define   MSK32BCM_Security_EROM_Data_Range_Range4_High       0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range10              0x01B8

    #define     BA_BCM_Security_EROM_Data_Range_Range5_Low     0x01B8
    #define     B16BCM_Security_EROM_Data_Range_Range5_Low     0x01B8
    #define   LSb32BCM_Security_EROM_Data_Range_Range5_Low        0
    #define   LSb16BCM_Security_EROM_Data_Range_Range5_Low        0
    #define       bBCM_Security_EROM_Data_Range_Range5_Low     32
    #define   MSK32BCM_Security_EROM_Data_Range_Range5_Low        0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range11              0x01BC

    #define     BA_BCM_Security_EROM_Data_Range_Range5_High    0x01BC
    #define     B16BCM_Security_EROM_Data_Range_Range5_High    0x01BC
    #define   LSb32BCM_Security_EROM_Data_Range_Range5_High       0
    #define   LSb16BCM_Security_EROM_Data_Range_Range5_High       0
    #define       bBCM_Security_EROM_Data_Range_Range5_High    32
    #define   MSK32BCM_Security_EROM_Data_Range_Range5_High       0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range12              0x01C0

    #define     BA_BCM_Security_EROM_Data_Range_Range6_Low     0x01C0
    #define     B16BCM_Security_EROM_Data_Range_Range6_Low     0x01C0
    #define   LSb32BCM_Security_EROM_Data_Range_Range6_Low        0
    #define   LSb16BCM_Security_EROM_Data_Range_Range6_Low        0
    #define       bBCM_Security_EROM_Data_Range_Range6_Low     32
    #define   MSK32BCM_Security_EROM_Data_Range_Range6_Low        0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range13              0x01C4

    #define     BA_BCM_Security_EROM_Data_Range_Range6_High    0x01C4
    #define     B16BCM_Security_EROM_Data_Range_Range6_High    0x01C4
    #define   LSb32BCM_Security_EROM_Data_Range_Range6_High       0
    #define   LSb16BCM_Security_EROM_Data_Range_Range6_High       0
    #define       bBCM_Security_EROM_Data_Range_Range6_High    32
    #define   MSK32BCM_Security_EROM_Data_Range_Range6_High       0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range14              0x01C8

    #define     BA_BCM_Security_EROM_Data_Range_Range7_Low     0x01C8
    #define     B16BCM_Security_EROM_Data_Range_Range7_Low     0x01C8
    #define   LSb32BCM_Security_EROM_Data_Range_Range7_Low        0
    #define   LSb16BCM_Security_EROM_Data_Range_Range7_Low        0
    #define       bBCM_Security_EROM_Data_Range_Range7_Low     32
    #define   MSK32BCM_Security_EROM_Data_Range_Range7_Low        0xFFFFFFFF

    #define     RA_BCM_Security_EROM_Data_Range15              0x01CC

    #define     BA_BCM_Security_EROM_Data_Range_Range7_High    0x01CC
    #define     B16BCM_Security_EROM_Data_Range_Range7_High    0x01CC
    #define   LSb32BCM_Security_EROM_Data_Range_Range7_High       0
    #define   LSb16BCM_Security_EROM_Data_Range_Range7_High       0
    #define       bBCM_Security_EROM_Data_Range_Range7_High    32
    #define   MSK32BCM_Security_EROM_Data_Range_Range7_High       0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_Allow_ACPU_Init_AKL            0x0294

    #define     BA_BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL 0x0294
    #define     B16BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL 0x0294
    #define   LSb32BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL    0
    #define   LSb16BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL    0
    #define       bBCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL 1
    #define   MSK32BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_reset_address             0x0298

    #define     BA_BCM_Security_IROM_reset_address_IROM_reset_address 0x0298
    #define     B16BCM_Security_IROM_reset_address_IROM_reset_address 0x0298
    #define   LSb32BCM_Security_IROM_reset_address_IROM_reset_address    0
    #define   LSb16BCM_Security_IROM_reset_address_IROM_reset_address    0
    #define       bBCM_Security_IROM_reset_address_IROM_reset_address 32
    #define   MSK32BCM_Security_IROM_reset_address_IROM_reset_address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_reset_address_lock        0x029C

    #define     BA_BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock 0x029C
    #define     B16BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock 0x029C
    #define   LSb32BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock    0
    #define   LSb16BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock    0
    #define       bBCM_Security_IROM_reset_address_lock_IROM_reset_address_lock 4
    #define   MSK32BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock    0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_Security_Reg                   0x02A0
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_ACPU_Access0                0x02D0

    #define     BA_BCM_Security_KP_ACPU_Access0_ACPU_Region_en0 0x02D0
    #define     B16BCM_Security_KP_ACPU_Access0_ACPU_Region_en0 0x02D0
    #define   LSb32BCM_Security_KP_ACPU_Access0_ACPU_Region_en0    0
    #define   LSb16BCM_Security_KP_ACPU_Access0_ACPU_Region_en0    0
    #define       bBCM_Security_KP_ACPU_Access0_ACPU_Region_en0 32
    #define   MSK32BCM_Security_KP_ACPU_Access0_ACPU_Region_en0    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_ACPU_Access1                0x02D4

    #define     BA_BCM_Security_KP_ACPU_Access1_ACPU_Region_en1 0x02D4
    #define     B16BCM_Security_KP_ACPU_Access1_ACPU_Region_en1 0x02D4
    #define   LSb32BCM_Security_KP_ACPU_Access1_ACPU_Region_en1    0
    #define   LSb16BCM_Security_KP_ACPU_Access1_ACPU_Region_en1    0
    #define       bBCM_Security_KP_ACPU_Access1_ACPU_Region_en1 32
    #define   MSK32BCM_Security_KP_ACPU_Access1_ACPU_Region_en1    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_ACPU_Access2                0x02D8

    #define     BA_BCM_Security_KP_ACPU_Access2_ACPU_Region_en2 0x02D8
    #define     B16BCM_Security_KP_ACPU_Access2_ACPU_Region_en2 0x02D8
    #define   LSb32BCM_Security_KP_ACPU_Access2_ACPU_Region_en2    0
    #define   LSb16BCM_Security_KP_ACPU_Access2_ACPU_Region_en2    0
    #define       bBCM_Security_KP_ACPU_Access2_ACPU_Region_en2 32
    #define   MSK32BCM_Security_KP_ACPU_Access2_ACPU_Region_en2    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_ACPU_Access3                0x02DC

    #define     BA_BCM_Security_KP_ACPU_Access3_ACPU_Region_en3 0x02DC
    #define     B16BCM_Security_KP_ACPU_Access3_ACPU_Region_en3 0x02DC
    #define   LSb32BCM_Security_KP_ACPU_Access3_ACPU_Region_en3    0
    #define   LSb16BCM_Security_KP_ACPU_Access3_ACPU_Region_en3    0
    #define       bBCM_Security_KP_ACPU_Access3_ACPU_Region_en3 32
    #define   MSK32BCM_Security_KP_ACPU_Access3_ACPU_Region_en3    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_BCM_Access0                 0x02E0

    #define     BA_BCM_Security_KP_BCM_Access0_BCM_Region_en0  0x02E0
    #define     B16BCM_Security_KP_BCM_Access0_BCM_Region_en0  0x02E0
    #define   LSb32BCM_Security_KP_BCM_Access0_BCM_Region_en0     0
    #define   LSb16BCM_Security_KP_BCM_Access0_BCM_Region_en0     0
    #define       bBCM_Security_KP_BCM_Access0_BCM_Region_en0  32
    #define   MSK32BCM_Security_KP_BCM_Access0_BCM_Region_en0     0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_BCM_Access1                 0x02E4

    #define     BA_BCM_Security_KP_BCM_Access1_BCM_Region_en1  0x02E4
    #define     B16BCM_Security_KP_BCM_Access1_BCM_Region_en1  0x02E4
    #define   LSb32BCM_Security_KP_BCM_Access1_BCM_Region_en1     0
    #define   LSb16BCM_Security_KP_BCM_Access1_BCM_Region_en1     0
    #define       bBCM_Security_KP_BCM_Access1_BCM_Region_en1  32
    #define   MSK32BCM_Security_KP_BCM_Access1_BCM_Region_en1     0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_BCM_Access2                 0x02E8

    #define     BA_BCM_Security_KP_BCM_Access2_BCM_Region_en2  0x02E8
    #define     B16BCM_Security_KP_BCM_Access2_BCM_Region_en2  0x02E8
    #define   LSb32BCM_Security_KP_BCM_Access2_BCM_Region_en2     0
    #define   LSb16BCM_Security_KP_BCM_Access2_BCM_Region_en2     0
    #define       bBCM_Security_KP_BCM_Access2_BCM_Region_en2  32
    #define   MSK32BCM_Security_KP_BCM_Access2_BCM_Region_en2     0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_BCM_Access3                 0x02EC

    #define     BA_BCM_Security_KP_BCM_Access3_BCM_Region_en3  0x02EC
    #define     B16BCM_Security_KP_BCM_Access3_BCM_Region_en3  0x02EC
    #define   LSb32BCM_Security_KP_BCM_Access3_BCM_Region_en3     0
    #define   LSb16BCM_Security_KP_BCM_Access3_BCM_Region_en3     0
    #define       bBCM_Security_KP_BCM_Access3_BCM_Region_en3  32
    #define   MSK32BCM_Security_KP_BCM_Access3_BCM_Region_en3     0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_IFCP_Access0                0x02F0

    #define     BA_BCM_Security_KP_IFCP_Access0_IFCP_Region_en0 0x02F0
    #define     B16BCM_Security_KP_IFCP_Access0_IFCP_Region_en0 0x02F0
    #define   LSb32BCM_Security_KP_IFCP_Access0_IFCP_Region_en0    0
    #define   LSb16BCM_Security_KP_IFCP_Access0_IFCP_Region_en0    0
    #define       bBCM_Security_KP_IFCP_Access0_IFCP_Region_en0 32
    #define   MSK32BCM_Security_KP_IFCP_Access0_IFCP_Region_en0    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_IFCP_Access1                0x02F4

    #define     BA_BCM_Security_KP_IFCP_Access1_IFCP_Region_en1 0x02F4
    #define     B16BCM_Security_KP_IFCP_Access1_IFCP_Region_en1 0x02F4
    #define   LSb32BCM_Security_KP_IFCP_Access1_IFCP_Region_en1    0
    #define   LSb16BCM_Security_KP_IFCP_Access1_IFCP_Region_en1    0
    #define       bBCM_Security_KP_IFCP_Access1_IFCP_Region_en1 32
    #define   MSK32BCM_Security_KP_IFCP_Access1_IFCP_Region_en1    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_IFCP_Access2                0x02F8

    #define     BA_BCM_Security_KP_IFCP_Access2_IFCP_Region_en2 0x02F8
    #define     B16BCM_Security_KP_IFCP_Access2_IFCP_Region_en2 0x02F8
    #define   LSb32BCM_Security_KP_IFCP_Access2_IFCP_Region_en2    0
    #define   LSb16BCM_Security_KP_IFCP_Access2_IFCP_Region_en2    0
    #define       bBCM_Security_KP_IFCP_Access2_IFCP_Region_en2 32
    #define   MSK32BCM_Security_KP_IFCP_Access2_IFCP_Region_en2    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_IFCP_Access3                0x02FC

    #define     BA_BCM_Security_KP_IFCP_Access3_IFCP_Region_en3 0x02FC
    #define     B16BCM_Security_KP_IFCP_Access3_IFCP_Region_en3 0x02FC
    #define   LSb32BCM_Security_KP_IFCP_Access3_IFCP_Region_en3    0
    #define   LSb16BCM_Security_KP_IFCP_Access3_IFCP_Region_en3    0
    #define       bBCM_Security_KP_IFCP_Access3_IFCP_Region_en3 32
    #define   MSK32BCM_Security_KP_IFCP_Access3_IFCP_Region_en3    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_ACPU_Access_lock            0x0300

    #define     BA_BCM_Security_KP_ACPU_Access_lock_lock       0x0300
    #define     B16BCM_Security_KP_ACPU_Access_lock_lock       0x0300
    #define   LSb32BCM_Security_KP_ACPU_Access_lock_lock          0
    #define   LSb16BCM_Security_KP_ACPU_Access_lock_lock          0
    #define       bBCM_Security_KP_ACPU_Access_lock_lock       4
    #define   MSK32BCM_Security_KP_ACPU_Access_lock_lock          0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_BCM_Access_lock             0x0304

    #define     BA_BCM_Security_KP_BCM_Access_lock_lock        0x0304
    #define     B16BCM_Security_KP_BCM_Access_lock_lock        0x0304
    #define   LSb32BCM_Security_KP_BCM_Access_lock_lock           0
    #define   LSb16BCM_Security_KP_BCM_Access_lock_lock           0
    #define       bBCM_Security_KP_BCM_Access_lock_lock        4
    #define   MSK32BCM_Security_KP_BCM_Access_lock_lock           0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_KP_IFCP_Access_lock            0x0308

    #define     BA_BCM_Security_KP_IFCP_Access_lock_lock       0x0308
    #define     B16BCM_Security_KP_IFCP_Access_lock_lock       0x0308
    #define   LSb32BCM_Security_KP_IFCP_Access_lock_lock          0
    #define   LSb16BCM_Security_KP_IFCP_Access_lock_lock          0
    #define       bBCM_Security_KP_IFCP_Access_lock_lock       4
    #define   MSK32BCM_Security_KP_IFCP_Access_lock_lock          0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_Spare_reg                      0x030C

    #define     BA_BCM_Security_Spare_reg_Reg0                 0x030C
    #define     B16BCM_Security_Spare_reg_Reg0                 0x030C
    #define   LSb32BCM_Security_Spare_reg_Reg0                    0
    #define   LSb16BCM_Security_Spare_reg_Reg0                    0
    #define       bBCM_Security_Spare_reg_Reg0                 32
    #define   MSK32BCM_Security_Spare_reg_Reg0                    0xFFFFFFFF

    #define     RA_BCM_Security_Spare_reg1                     0x0310

    #define     BA_BCM_Security_Spare_reg_Reg1                 0x0310
    #define     B16BCM_Security_Spare_reg_Reg1                 0x0310
    #define   LSb32BCM_Security_Spare_reg_Reg1                    0
    #define   LSb16BCM_Security_Spare_reg_Reg1                    0
    #define       bBCM_Security_Spare_reg_Reg1                 32
    #define   MSK32BCM_Security_Spare_reg_Reg1                    0xFFFFFFFF

    #define     RA_BCM_Security_Spare_reg2                     0x0314

    #define     BA_BCM_Security_Spare_reg_Reg2                 0x0314
    #define     B16BCM_Security_Spare_reg_Reg2                 0x0314
    #define   LSb32BCM_Security_Spare_reg_Reg2                    0
    #define   LSb16BCM_Security_Spare_reg_Reg2                    0
    #define       bBCM_Security_Spare_reg_Reg2                 32
    #define   MSK32BCM_Security_Spare_reg_Reg2                    0xFFFFFFFF

    #define     RA_BCM_Security_Spare_reg3                     0x0318

    #define     BA_BCM_Security_Spare_reg_Reg3                 0x0318
    #define     B16BCM_Security_Spare_reg_Reg3                 0x0318
    #define   LSb32BCM_Security_Spare_reg_Reg3                    0
    #define   LSb16BCM_Security_Spare_reg_Reg3                    0
    #define       bBCM_Security_Spare_reg_Reg3                 32
    #define   MSK32BCM_Security_Spare_reg_Reg3                    0xFFFFFFFF

    #define     RA_BCM_Security_Spare_reg4                     0x031C

    #define     BA_BCM_Security_Spare_reg_Reg4                 0x031C
    #define     B16BCM_Security_Spare_reg_Reg4                 0x031C
    #define   LSb32BCM_Security_Spare_reg_Reg4                    0
    #define   LSb16BCM_Security_Spare_reg_Reg4                    0
    #define       bBCM_Security_Spare_reg_Reg4                 32
    #define   MSK32BCM_Security_Spare_reg_Reg4                    0xFFFFFFFF

    #define     RA_BCM_Security_Spare_reg5                     0x0320

    #define     BA_BCM_Security_Spare_reg_Reg5                 0x0320
    #define     B16BCM_Security_Spare_reg_Reg5                 0x0320
    #define   LSb32BCM_Security_Spare_reg_Reg5                    0
    #define   LSb16BCM_Security_Spare_reg_Reg5                    0
    #define       bBCM_Security_Spare_reg_Reg5                 32
    #define   MSK32BCM_Security_Spare_reg_Reg5                    0xFFFFFFFF

    #define     RA_BCM_Security_Spare_reg6                     0x0324

    #define     BA_BCM_Security_Spare_reg_Reg6                 0x0324
    #define     B16BCM_Security_Spare_reg_Reg6                 0x0324
    #define   LSb32BCM_Security_Spare_reg_Reg6                    0
    #define   LSb16BCM_Security_Spare_reg_Reg6                    0
    #define       bBCM_Security_Spare_reg_Reg6                 32
    #define   MSK32BCM_Security_Spare_reg_Reg6                    0xFFFFFFFF

    #define     RA_BCM_Security_Spare_reg7                     0x0328

    #define     BA_BCM_Security_Spare_reg_Reg7                 0x0328
    #define     B16BCM_Security_Spare_reg_Reg7                 0x0328
    #define   LSb32BCM_Security_Spare_reg_Reg7                    0
    #define   LSb16BCM_Security_Spare_reg_Reg7                    0
    #define       bBCM_Security_Spare_reg_Reg7                 32
    #define   MSK32BCM_Security_Spare_reg_Reg7                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_FI_Status                      0x032C

    #define     BA_BCM_Security_FI_Status_FI_PL_SOC            0x032C
    #define     B16BCM_Security_FI_Status_FI_PL_SOC            0x032C
    #define   LSb32BCM_Security_FI_Status_FI_PL_SOC               0
    #define   LSb16BCM_Security_FI_Status_FI_PL_SOC               0
    #define       bBCM_Security_FI_Status_FI_PL_SOC            1
    #define   MSK32BCM_Security_FI_Status_FI_PL_SOC               0x00000001

    #define     BA_BCM_Security_FI_Status_FI_BIU               0x032C
    #define     B16BCM_Security_FI_Status_FI_BIU               0x032C
    #define   LSb32BCM_Security_FI_Status_FI_BIU                  1
    #define   LSb16BCM_Security_FI_Status_FI_BIU                  1
    #define       bBCM_Security_FI_Status_FI_BIU               1
    #define   MSK32BCM_Security_FI_Status_FI_BIU                  0x00000002
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IFCP_RAM_Addr_Key              0x0334

    #define     BA_BCM_Security_IFCP_RAM_Addr_Key_RAM_Addr_Key 0x0334
    #define     B16BCM_Security_IFCP_RAM_Addr_Key_RAM_Addr_Key 0x0334
    #define   LSb32BCM_Security_IFCP_RAM_Addr_Key_RAM_Addr_Key    0
    #define   LSb16BCM_Security_IFCP_RAM_Addr_Key_RAM_Addr_Key    0
    #define       bBCM_Security_IFCP_RAM_Addr_Key_RAM_Addr_Key 32
    #define   MSK32BCM_Security_IFCP_RAM_Addr_Key_RAM_Addr_Key    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IFCP_RAM_Data_Key              0x0338

    #define     BA_BCM_Security_IFCP_RAM_Data_Key_RAM_Data_Key 0x0338
    #define     B16BCM_Security_IFCP_RAM_Data_Key_RAM_Data_Key 0x0338
    #define   LSb32BCM_Security_IFCP_RAM_Data_Key_RAM_Data_Key    0
    #define   LSb16BCM_Security_IFCP_RAM_Data_Key_RAM_Data_Key    0
    #define       bBCM_Security_IFCP_RAM_Data_Key_RAM_Data_Key 32
    #define   MSK32BCM_Security_IFCP_RAM_Data_Key_RAM_Data_Key    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask1_Start_Address       0x0340

    #define     BA_BCM_Security_IROM_Mask1_Start_Address_Start_Address 0x0340
    #define     B16BCM_Security_IROM_Mask1_Start_Address_Start_Address 0x0340
    #define   LSb32BCM_Security_IROM_Mask1_Start_Address_Start_Address    0
    #define   LSb16BCM_Security_IROM_Mask1_Start_Address_Start_Address    0
    #define       bBCM_Security_IROM_Mask1_Start_Address_Start_Address 32
    #define   MSK32BCM_Security_IROM_Mask1_Start_Address_Start_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask1_End_Address         0x0344

    #define     BA_BCM_Security_IROM_Mask1_End_Address_End_Address 0x0344
    #define     B16BCM_Security_IROM_Mask1_End_Address_End_Address 0x0344
    #define   LSb32BCM_Security_IROM_Mask1_End_Address_End_Address    0
    #define   LSb16BCM_Security_IROM_Mask1_End_Address_End_Address    0
    #define       bBCM_Security_IROM_Mask1_End_Address_End_Address 32
    #define   MSK32BCM_Security_IROM_Mask1_End_Address_End_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask1_Mask                0x0348

    #define     BA_BCM_Security_IROM_Mask1_Mask_Mask           0x0348
    #define     B16BCM_Security_IROM_Mask1_Mask_Mask           0x0348
    #define   LSb32BCM_Security_IROM_Mask1_Mask_Mask              0
    #define   LSb16BCM_Security_IROM_Mask1_Mask_Mask              0
    #define       bBCM_Security_IROM_Mask1_Mask_Mask           4
    #define   MSK32BCM_Security_IROM_Mask1_Mask_Mask              0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask2_Start_Address       0x0350

    #define     BA_BCM_Security_IROM_Mask2_Start_Address_Start_Address 0x0350
    #define     B16BCM_Security_IROM_Mask2_Start_Address_Start_Address 0x0350
    #define   LSb32BCM_Security_IROM_Mask2_Start_Address_Start_Address    0
    #define   LSb16BCM_Security_IROM_Mask2_Start_Address_Start_Address    0
    #define       bBCM_Security_IROM_Mask2_Start_Address_Start_Address 32
    #define   MSK32BCM_Security_IROM_Mask2_Start_Address_Start_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask2_End_Address         0x0354

    #define     BA_BCM_Security_IROM_Mask2_End_Address_End_Address 0x0354
    #define     B16BCM_Security_IROM_Mask2_End_Address_End_Address 0x0354
    #define   LSb32BCM_Security_IROM_Mask2_End_Address_End_Address    0
    #define   LSb16BCM_Security_IROM_Mask2_End_Address_End_Address    0
    #define       bBCM_Security_IROM_Mask2_End_Address_End_Address 32
    #define   MSK32BCM_Security_IROM_Mask2_End_Address_End_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask2_Mask                0x0358

    #define     BA_BCM_Security_IROM_Mask2_Mask_Mask           0x0358
    #define     B16BCM_Security_IROM_Mask2_Mask_Mask           0x0358
    #define   LSb32BCM_Security_IROM_Mask2_Mask_Mask              0
    #define   LSb16BCM_Security_IROM_Mask2_Mask_Mask              0
    #define       bBCM_Security_IROM_Mask2_Mask_Mask           4
    #define   MSK32BCM_Security_IROM_Mask2_Mask_Mask              0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask3_Start_Address       0x0360

    #define     BA_BCM_Security_IROM_Mask3_Start_Address_Start_Address 0x0360
    #define     B16BCM_Security_IROM_Mask3_Start_Address_Start_Address 0x0360
    #define   LSb32BCM_Security_IROM_Mask3_Start_Address_Start_Address    0
    #define   LSb16BCM_Security_IROM_Mask3_Start_Address_Start_Address    0
    #define       bBCM_Security_IROM_Mask3_Start_Address_Start_Address 32
    #define   MSK32BCM_Security_IROM_Mask3_Start_Address_Start_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask3_End_Address         0x0364

    #define     BA_BCM_Security_IROM_Mask3_End_Address_End_Address 0x0364
    #define     B16BCM_Security_IROM_Mask3_End_Address_End_Address 0x0364
    #define   LSb32BCM_Security_IROM_Mask3_End_Address_End_Address    0
    #define   LSb16BCM_Security_IROM_Mask3_End_Address_End_Address    0
    #define       bBCM_Security_IROM_Mask3_End_Address_End_Address 32
    #define   MSK32BCM_Security_IROM_Mask3_End_Address_End_Address    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_IROM_Mask3_Mask                0x0368

    #define     BA_BCM_Security_IROM_Mask3_Mask_Mask           0x0368
    #define     B16BCM_Security_IROM_Mask3_Mask_Mask           0x0368
    #define   LSb32BCM_Security_IROM_Mask3_Mask_Mask              0
    #define   LSb16BCM_Security_IROM_Mask3_Mask_Mask              0
    #define       bBCM_Security_IROM_Mask3_Mask_Mask           4
    #define   MSK32BCM_Security_IROM_Mask3_Mask_Mask              0x0000000F
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_CERT_Priv_Mode_CFG             0x036C

    #define     BA_BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation 0x036C
    #define     B16BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation 0x036C
    #define   LSb32BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation    0
    #define   LSb16BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation    0
    #define       bBCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation 1
    #define   MSK32BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation    0x00000001

    #define     BA_BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation 0x036C
    #define     B16BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation 0x036C
    #define   LSb32BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation    1
    #define   LSb16BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation    1
    #define       bBCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation 1
    #define   MSK32BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation    0x00000002

    #define     BA_BCM_Security_CERT_Priv_Mode_CFG_RFU         0x036C
    #define     B16BCM_Security_CERT_Priv_Mode_CFG_RFU         0x036C
    #define   LSb32BCM_Security_CERT_Priv_Mode_CFG_RFU            2
    #define   LSb16BCM_Security_CERT_Priv_Mode_CFG_RFU            2
    #define       bBCM_Security_CERT_Priv_Mode_CFG_RFU         30
    #define   MSK32BCM_Security_CERT_Priv_Mode_CFG_RFU            0xFFFFFFFC
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_Security_EnvMon_Status                  0x0370

    #define     BA_BCM_Security_EnvMon_Status_Clock            0x0370
    #define     B16BCM_Security_EnvMon_Status_Clock            0x0370
    #define   LSb32BCM_Security_EnvMon_Status_Clock               0
    #define   LSb16BCM_Security_EnvMon_Status_Clock               0
    #define       bBCM_Security_EnvMon_Status_Clock            1
    #define   MSK32BCM_Security_EnvMon_Status_Clock               0x00000001

    #define     BA_BCM_Security_EnvMon_Status_Reset            0x0370
    #define     B16BCM_Security_EnvMon_Status_Reset            0x0370
    #define   LSb32BCM_Security_EnvMon_Status_Reset               1
    #define   LSb16BCM_Security_EnvMon_Status_Reset               1
    #define       bBCM_Security_EnvMon_Status_Reset            1
    #define   MSK32BCM_Security_EnvMon_Status_Reset               0x00000002

    #define     BA_BCM_Security_EnvMon_Status_Voltage          0x0370
    #define     B16BCM_Security_EnvMon_Status_Voltage          0x0370
    #define   LSb32BCM_Security_EnvMon_Status_Voltage             2
    #define   LSb16BCM_Security_EnvMon_Status_Voltage             2
    #define       bBCM_Security_EnvMon_Status_Voltage          1
    #define   MSK32BCM_Security_EnvMon_Status_Voltage             0x00000004
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCM_Security {
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_ChipID[2];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_V_Version_V_Version(r32)       _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_V_Version_V_Version(r32,v)     _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_V_Version                      {\
            UNSG32 uV_Version_V_Version                        : 32;\
          }
    union { UNSG32 u32BCM_Security_V_Version;
            struct w32BCM_Security_V_Version;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_Berlin_DDR_Base_Berlin_DDR_Base(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Berlin_DDR_Base_Berlin_DDR_Base(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Berlin_DDR_Base                {\
            UNSG32 uBerlin_DDR_Base_Berlin_DDR_Base            : 32;\
          }
    union { UNSG32 u32BCM_Security_Berlin_DDR_Base;
            struct w32BCM_Security_Berlin_DDR_Base;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_Berlin_SOC_Base_Berlin_SOC_Base(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Berlin_SOC_Base_Berlin_SOC_Base(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Berlin_SOC_Base                {\
            UNSG32 uBerlin_SOC_Base_Berlin_SOC_Base            : 32;\
          }
    union { UNSG32 u32BCM_Security_Berlin_SOC_Base;
            struct w32BCM_Security_Berlin_SOC_Base;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_BCM_DIR_En_BCM_2_DDR(r32)      _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_BCM_DIR_En_BCM_2_DDR(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_BCM_DIR_En_BCM_2_DDR(r16)      _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_BCM_DIR_En_BCM_2_DDR(r16,v)    _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_BCM_DIR_En_BCM_2_SOC(r32)      _BFGET_(r32, 1, 1)
    #define   SET32BCM_Security_BCM_DIR_En_BCM_2_SOC(r32,v)    _BFSET_(r32, 1, 1,v)
    #define   GET16BCM_Security_BCM_DIR_En_BCM_2_SOC(r16)      _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_BCM_DIR_En_BCM_2_SOC(r16,v)    _BFSET_(r16, 1, 1,v)

    #define     w32BCM_Security_BCM_DIR_En                     {\
            UNSG32 uBCM_DIR_En_BCM_2_DDR                       :  1;\
            UNSG32 uBCM_DIR_En_BCM_2_SOC                       :  1;\
            UNSG32 RSVDx14_b2                                  : 30;\
          }
    union { UNSG32 u32BCM_Security_BCM_DIR_En;
            struct w32BCM_Security_BCM_DIR_En;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_ACPU_Access_TSOTP(r32)         _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_ACPU_Access_TSOTP(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_ACPU_Access_TSOTP(r16)         _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_ACPU_Access_TSOTP(r16,v)       _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_ACPU_Access_TspKeyTbl(r32)     _BFGET_(r32, 1, 1)
    #define   SET32BCM_Security_ACPU_Access_TspKeyTbl(r32,v)   _BFSET_(r32, 1, 1,v)
    #define   GET16BCM_Security_ACPU_Access_TspKeyTbl(r16)     _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_ACPU_Access_TspKeyTbl(r16,v)   _BFSET_(r16, 1, 1,v)

    #define     w32BCM_Security_ACPU_Access                    {\
            UNSG32 uACPU_Access_TSOTP                          :  1;\
            UNSG32 uACPU_Access_TspKeyTbl                      :  1;\
            UNSG32 RSVDx18_b2                                  : 30;\
          }
    union { UNSG32 u32BCM_Security_ACPU_Access;
            struct w32BCM_Security_ACPU_Access;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_Efuse_Read_Disable_Row_0(r32)  _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_0(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_0(r16)  _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_0(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_1(r32)  _BFGET_(r32, 1, 1)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_1(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_1(r16)  _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_1(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_2(r32)  _BFGET_(r32, 2, 2)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_2(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_2(r16)  _BFGET_(r16, 2, 2)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_2(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_3(r32)  _BFGET_(r32, 3, 3)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_3(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_3(r16)  _BFGET_(r16, 3, 3)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_3(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_4(r32)  _BFGET_(r32, 4, 4)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_4(r32,v) _BFSET_(r32, 4, 4,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_4(r16)  _BFGET_(r16, 4, 4)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_4(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_5(r32)  _BFGET_(r32, 5, 5)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_5(r32,v) _BFSET_(r32, 5, 5,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_5(r16)  _BFGET_(r16, 5, 5)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_5(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_6(r32)  _BFGET_(r32, 6, 6)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_6(r32,v) _BFSET_(r32, 6, 6,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_6(r16)  _BFGET_(r16, 6, 6)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_6(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_7(r32)  _BFGET_(r32, 7, 7)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_7(r32,v) _BFSET_(r32, 7, 7,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_7(r16)  _BFGET_(r16, 7, 7)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_7(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_8(r32)  _BFGET_(r32, 8, 8)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_8(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_8(r16)  _BFGET_(r16, 8, 8)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_8(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_9(r32)  _BFGET_(r32, 9, 9)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_9(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_9(r16)  _BFGET_(r16, 9, 9)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_9(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_10(r32) _BFGET_(r32,10,10)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_10(r32,v) _BFSET_(r32,10,10,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_10(r16) _BFGET_(r16,10,10)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_10(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_11(r32) _BFGET_(r32,11,11)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_11(r32,v) _BFSET_(r32,11,11,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_11(r16) _BFGET_(r16,11,11)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_11(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_12(r32) _BFGET_(r32,12,12)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_12(r32,v) _BFSET_(r32,12,12,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_12(r16) _BFGET_(r16,12,12)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_12(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_13(r32) _BFGET_(r32,13,13)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_13(r32,v) _BFSET_(r32,13,13,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_13(r16) _BFGET_(r16,13,13)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_13(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_14(r32) _BFGET_(r32,14,14)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_14(r32,v) _BFSET_(r32,14,14,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_14(r16) _BFGET_(r16,14,14)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_14(r16,v) _BFSET_(r16,14,14,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_15(r32) _BFGET_(r32,15,15)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_15(r32,v) _BFSET_(r32,15,15,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_15(r16) _BFGET_(r16,15,15)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_15(r16,v) _BFSET_(r16,15,15,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_16(r32) _BFGET_(r32,16,16)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_16(r32,v) _BFSET_(r32,16,16,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_16(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_16(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_17(r32) _BFGET_(r32,17,17)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_17(r32,v) _BFSET_(r32,17,17,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_17(r16) _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_17(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_18(r32) _BFGET_(r32,18,18)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_18(r32,v) _BFSET_(r32,18,18,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_18(r16) _BFGET_(r16, 2, 2)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_18(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_19(r32) _BFGET_(r32,19,19)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_19(r32,v) _BFSET_(r32,19,19,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_19(r16) _BFGET_(r16, 3, 3)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_19(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_20(r32) _BFGET_(r32,20,20)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_20(r32,v) _BFSET_(r32,20,20,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_20(r16) _BFGET_(r16, 4, 4)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_20(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_21(r32) _BFGET_(r32,21,21)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_21(r32,v) _BFSET_(r32,21,21,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_21(r16) _BFGET_(r16, 5, 5)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_21(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_22(r32) _BFGET_(r32,22,22)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_22(r32,v) _BFSET_(r32,22,22,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_22(r16) _BFGET_(r16, 6, 6)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_22(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_23(r32) _BFGET_(r32,23,23)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_23(r32,v) _BFSET_(r32,23,23,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_23(r16) _BFGET_(r16, 7, 7)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_23(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_24(r32) _BFGET_(r32,24,24)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_24(r32,v) _BFSET_(r32,24,24,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_24(r16) _BFGET_(r16, 8, 8)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_24(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_25(r32) _BFGET_(r32,25,25)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_25(r32,v) _BFSET_(r32,25,25,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_25(r16) _BFGET_(r16, 9, 9)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_25(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_26(r32) _BFGET_(r32,26,26)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_26(r32,v) _BFSET_(r32,26,26,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_26(r16) _BFGET_(r16,10,10)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_26(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_27(r32) _BFGET_(r32,27,27)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_27(r32,v) _BFSET_(r32,27,27,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_27(r16) _BFGET_(r16,11,11)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_27(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_28(r32) _BFGET_(r32,28,28)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_28(r32,v) _BFSET_(r32,28,28,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_28(r16) _BFGET_(r16,12,12)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_28(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_29(r32) _BFGET_(r32,29,29)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_29(r32,v) _BFSET_(r32,29,29,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_29(r16) _BFGET_(r16,13,13)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_29(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_30(r32) _BFGET_(r32,30,30)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_30(r32,v) _BFSET_(r32,30,30,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_30(r16) _BFGET_(r16,14,14)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_30(r16,v) _BFSET_(r16,14,14,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_31(r32) _BFGET_(r32,31,31)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_31(r32,v) _BFSET_(r32,31,31,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_31(r16) _BFGET_(r16,15,15)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_31(r16,v) _BFSET_(r16,15,15,v)

    #define     w32BCM_Security_Efuse_Read_Disable             {\
            UNSG32 uEfuse_Read_Disable_Row_0                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_1                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_2                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_3                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_4                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_5                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_6                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_7                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_8                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_9                   :  1;\
            UNSG32 uEfuse_Read_Disable_Row_10                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_11                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_12                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_13                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_14                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_15                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_16                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_17                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_18                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_19                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_20                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_21                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_22                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_23                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_24                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_25                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_26                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_27                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_28                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_29                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_30                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_31                  :  1;\
          }
    union { UNSG32 u32BCM_Security_Efuse_Read_Disable;
            struct w32BCM_Security_Efuse_Read_Disable;
          };
    #define   GET32BCM_Security_Efuse_Read_Disable_Row_32(r32) _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_32(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_32(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_32(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_33(r32) _BFGET_(r32, 1, 1)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_33(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_33(r16) _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_33(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_34(r32) _BFGET_(r32, 2, 2)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_34(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_34(r16) _BFGET_(r16, 2, 2)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_34(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_35(r32) _BFGET_(r32, 3, 3)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_35(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_35(r16) _BFGET_(r16, 3, 3)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_35(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_36(r32) _BFGET_(r32, 4, 4)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_36(r32,v) _BFSET_(r32, 4, 4,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_36(r16) _BFGET_(r16, 4, 4)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_36(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_37(r32) _BFGET_(r32, 5, 5)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_37(r32,v) _BFSET_(r32, 5, 5,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_37(r16) _BFGET_(r16, 5, 5)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_37(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_38(r32) _BFGET_(r32, 6, 6)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_38(r32,v) _BFSET_(r32, 6, 6,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_38(r16) _BFGET_(r16, 6, 6)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_38(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_39(r32) _BFGET_(r32, 7, 7)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_39(r32,v) _BFSET_(r32, 7, 7,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_39(r16) _BFGET_(r16, 7, 7)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_39(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_40(r32) _BFGET_(r32, 8, 8)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_40(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_40(r16) _BFGET_(r16, 8, 8)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_40(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_41(r32) _BFGET_(r32, 9, 9)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_41(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_41(r16) _BFGET_(r16, 9, 9)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_41(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_42(r32) _BFGET_(r32,10,10)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_42(r32,v) _BFSET_(r32,10,10,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_42(r16) _BFGET_(r16,10,10)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_42(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_43(r32) _BFGET_(r32,11,11)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_43(r32,v) _BFSET_(r32,11,11,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_43(r16) _BFGET_(r16,11,11)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_43(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_44(r32) _BFGET_(r32,12,12)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_44(r32,v) _BFSET_(r32,12,12,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_44(r16) _BFGET_(r16,12,12)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_44(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_45(r32) _BFGET_(r32,13,13)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_45(r32,v) _BFSET_(r32,13,13,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_45(r16) _BFGET_(r16,13,13)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_45(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_46(r32) _BFGET_(r32,14,14)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_46(r32,v) _BFSET_(r32,14,14,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_46(r16) _BFGET_(r16,14,14)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_46(r16,v) _BFSET_(r16,14,14,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_47(r32) _BFGET_(r32,15,15)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_47(r32,v) _BFSET_(r32,15,15,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_47(r16) _BFGET_(r16,15,15)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_47(r16,v) _BFSET_(r16,15,15,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_48(r32) _BFGET_(r32,16,16)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_48(r32,v) _BFSET_(r32,16,16,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_48(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_48(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_49(r32) _BFGET_(r32,17,17)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_49(r32,v) _BFSET_(r32,17,17,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_49(r16) _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_49(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_50(r32) _BFGET_(r32,18,18)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_50(r32,v) _BFSET_(r32,18,18,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_50(r16) _BFGET_(r16, 2, 2)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_50(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_51(r32) _BFGET_(r32,19,19)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_51(r32,v) _BFSET_(r32,19,19,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_51(r16) _BFGET_(r16, 3, 3)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_51(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_52(r32) _BFGET_(r32,20,20)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_52(r32,v) _BFSET_(r32,20,20,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_52(r16) _BFGET_(r16, 4, 4)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_52(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_53(r32) _BFGET_(r32,21,21)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_53(r32,v) _BFSET_(r32,21,21,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_53(r16) _BFGET_(r16, 5, 5)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_53(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_54(r32) _BFGET_(r32,22,22)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_54(r32,v) _BFSET_(r32,22,22,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_54(r16) _BFGET_(r16, 6, 6)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_54(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_55(r32) _BFGET_(r32,23,23)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_55(r32,v) _BFSET_(r32,23,23,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_55(r16) _BFGET_(r16, 7, 7)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_55(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_56(r32) _BFGET_(r32,24,24)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_56(r32,v) _BFSET_(r32,24,24,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_56(r16) _BFGET_(r16, 8, 8)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_56(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_57(r32) _BFGET_(r32,25,25)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_57(r32,v) _BFSET_(r32,25,25,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_57(r16) _BFGET_(r16, 9, 9)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_57(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_58(r32) _BFGET_(r32,26,26)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_58(r32,v) _BFSET_(r32,26,26,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_58(r16) _BFGET_(r16,10,10)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_58(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_59(r32) _BFGET_(r32,27,27)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_59(r32,v) _BFSET_(r32,27,27,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_59(r16) _BFGET_(r16,11,11)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_59(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_60(r32) _BFGET_(r32,28,28)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_60(r32,v) _BFSET_(r32,28,28,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_60(r16) _BFGET_(r16,12,12)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_60(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_61(r32) _BFGET_(r32,29,29)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_61(r32,v) _BFSET_(r32,29,29,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_61(r16) _BFGET_(r16,13,13)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_61(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_62(r32) _BFGET_(r32,30,30)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_62(r32,v) _BFSET_(r32,30,30,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_62(r16) _BFGET_(r16,14,14)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_62(r16,v) _BFSET_(r16,14,14,v)

    #define   GET32BCM_Security_Efuse_Read_Disable_Row_63(r32) _BFGET_(r32,31,31)
    #define   SET32BCM_Security_Efuse_Read_Disable_Row_63(r32,v) _BFSET_(r32,31,31,v)
    #define   GET16BCM_Security_Efuse_Read_Disable_Row_63(r16) _BFGET_(r16,15,15)
    #define   SET16BCM_Security_Efuse_Read_Disable_Row_63(r16,v) _BFSET_(r16,15,15,v)

    #define     w32BCM_Security_Efuse_Read_Disable1            {\
            UNSG32 uEfuse_Read_Disable_Row_32                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_33                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_34                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_35                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_36                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_37                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_38                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_39                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_40                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_41                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_42                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_43                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_44                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_45                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_46                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_47                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_48                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_49                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_50                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_51                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_52                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_53                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_54                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_55                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_56                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_57                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_58                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_59                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_60                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_61                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_62                  :  1;\
            UNSG32 uEfuse_Read_Disable_Row_63                  :  1;\
          }
    union { UNSG32 u32BCM_Security_Efuse_Read_Disable1;
            struct w32BCM_Security_Efuse_Read_Disable1;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx24                                     [8];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_RKEKReg[8];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable(r32) _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_RKEKRegReadDisable_RKEKRegReadDisable(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32BCM_Security_RKEKRegReadDisable             {\
            UNSG32 uRKEKRegReadDisable_RKEKRegReadDisable      :  1;\
            UNSG32 RSVDx4C_b1                                  : 31;\
          }
    union { UNSG32 u32BCM_Security_RKEKRegReadDisable;
            struct w32BCM_Security_RKEKRegReadDisable;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable(r32) _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_RKEKRegWriteDisable_RKEKRegWriteDisable(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32BCM_Security_RKEKRegWriteDisable            {\
            UNSG32 uRKEKRegWriteDisable_RKEKRegWriteDisable    :  1;\
            UNSG32 RSVDx50_b1                                  : 31;\
          }
    union { UNSG32 u32BCM_Security_RKEKRegWriteDisable;
            struct w32BCM_Security_RKEKRegWriteDisable;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_FeatureCtrlReg_V2G_Fuse(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_FeatureCtrlReg_V2G_Fuse(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_FeatureCtrlReg                 {\
            UNSG32 uFeatureCtrlReg_V2G_Fuse                    : 32;\
          }
    union { UNSG32 u32BCM_Security_FeatureCtrlReg;
            struct w32BCM_Security_FeatureCtrlReg;
          };
    #define   GET32BCM_Security_FeatureCtrlReg_Hantro_Fuse_Dec(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_FeatureCtrlReg_Hantro_Fuse_Dec(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_FeatureCtrlReg1                {\
            UNSG32 uFeatureCtrlReg_Hantro_Fuse_Dec             : 32;\
          }
    union { UNSG32 u32BCM_Security_FeatureCtrlReg1;
            struct w32BCM_Security_FeatureCtrlReg1;
          };
    #define   GET32BCM_Security_FeatureCtrlReg_Hantro_Fuse_PP(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_FeatureCtrlReg_Hantro_Fuse_PP(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_FeatureCtrlReg2                {\
            UNSG32 uFeatureCtrlReg_Hantro_Fuse_PP              : 32;\
          }
    union { UNSG32 u32BCM_Security_FeatureCtrlReg2;
            struct w32BCM_Security_FeatureCtrlReg2;
          };
    #define   GET32BCM_Security_FeatureCtrlReg_Hantro_Fuse_Enc(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_FeatureCtrlReg_Hantro_Fuse_Enc(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_FeatureCtrlReg3                {\
            UNSG32 uFeatureCtrlReg_Hantro_Fuse_Enc             : 32;\
          }
    union { UNSG32 u32BCM_Security_FeatureCtrlReg3;
            struct w32BCM_Security_FeatureCtrlReg3;
          };
    #define   GET32BCM_Security_FeatureCtrlReg_GPU_Feature(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_FeatureCtrlReg_GPU_Feature(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_FeatureCtrlReg4                {\
            UNSG32 uFeatureCtrlReg_GPU_Feature                 : 32;\
          }
    union { UNSG32 u32BCM_Security_FeatureCtrlReg4;
            struct w32BCM_Security_FeatureCtrlReg4;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_AVIO[4];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_Reseved_6(r32)                 _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Reseved_6(r32,v)               _BFSET_(r32,31, 0,v)

            UNSG32 u_Reseved_6                                 : 32;
    ///////////////////////////////////////////////////////////

    #define   GET32BCM_Security_Reseved_7(r32)                 _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Reseved_7(r32,v)               _BFSET_(r32,31, 0,v)

            UNSG32 u_Reseved_7                                 : 32;
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_0(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_0(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC                {\
            UNSG32 uEROM_Golden_CRC_Block_0                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC;
            struct w32BCM_Security_EROM_Golden_CRC;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_1(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_1(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC1               {\
            UNSG32 uEROM_Golden_CRC_Block_1                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC1;
            struct w32BCM_Security_EROM_Golden_CRC1;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_2(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_2(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC2               {\
            UNSG32 uEROM_Golden_CRC_Block_2                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC2;
            struct w32BCM_Security_EROM_Golden_CRC2;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_3(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_3(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC3               {\
            UNSG32 uEROM_Golden_CRC_Block_3                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC3;
            struct w32BCM_Security_EROM_Golden_CRC3;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_4(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_4(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC4               {\
            UNSG32 uEROM_Golden_CRC_Block_4                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC4;
            struct w32BCM_Security_EROM_Golden_CRC4;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_5(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_5(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC5               {\
            UNSG32 uEROM_Golden_CRC_Block_5                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC5;
            struct w32BCM_Security_EROM_Golden_CRC5;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_6(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_6(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC6               {\
            UNSG32 uEROM_Golden_CRC_Block_6                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC6;
            struct w32BCM_Security_EROM_Golden_CRC6;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_7(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_7(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC7               {\
            UNSG32 uEROM_Golden_CRC_Block_7                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC7;
            struct w32BCM_Security_EROM_Golden_CRC7;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_8(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_8(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC8               {\
            UNSG32 uEROM_Golden_CRC_Block_8                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC8;
            struct w32BCM_Security_EROM_Golden_CRC8;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_9(r32)   _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_9(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC9               {\
            UNSG32 uEROM_Golden_CRC_Block_9                    : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC9;
            struct w32BCM_Security_EROM_Golden_CRC9;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_10(r32)  _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_10(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC10              {\
            UNSG32 uEROM_Golden_CRC_Block_10                   : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC10;
            struct w32BCM_Security_EROM_Golden_CRC10;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_11(r32)  _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_11(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC11              {\
            UNSG32 uEROM_Golden_CRC_Block_11                   : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC11;
            struct w32BCM_Security_EROM_Golden_CRC11;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_12(r32)  _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_12(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC12              {\
            UNSG32 uEROM_Golden_CRC_Block_12                   : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC12;
            struct w32BCM_Security_EROM_Golden_CRC12;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_13(r32)  _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_13(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC13              {\
            UNSG32 uEROM_Golden_CRC_Block_13                   : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC13;
            struct w32BCM_Security_EROM_Golden_CRC13;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_14(r32)  _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_14(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC14              {\
            UNSG32 uEROM_Golden_CRC_Block_14                   : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC14;
            struct w32BCM_Security_EROM_Golden_CRC14;
          };
    #define   GET32BCM_Security_EROM_Golden_CRC_Block_15(r32)  _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Golden_CRC_Block_15(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Golden_CRC15              {\
            UNSG32 uEROM_Golden_CRC_Block_15                   : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Golden_CRC15;
            struct w32BCM_Security_EROM_Golden_CRC15;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_BackgroundCheck_Start_Addresss_Start_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_BackgroundCheck_Start_Addresss_Start_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_BackgroundCheck_Start_Addresss {\
            UNSG32 uEROM_BackgroundCheck_Start_Addresss_Start_Address : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_BackgroundCheck_Start_Addresss;
            struct w32BCM_Security_EROM_BackgroundCheck_Start_Addresss;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_BackgroundCheck_End_Addresss_End_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_BackgroundCheck_End_Addresss_End_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_BackgroundCheck_End_Addresss {\
            UNSG32 uEROM_BackgroundCheck_End_Addresss_End_Address : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_BackgroundCheck_End_Addresss;
            struct w32BCM_Security_EROM_BackgroundCheck_End_Addresss;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_BackgroundCheck_Start_Start(r32) _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_EROM_BackgroundCheck_Start_Start(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_EROM_BackgroundCheck_Start_Start(r16) _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_EROM_BackgroundCheck_Start_Start(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_EROM_BackgroundCheck_Start     {\
            UNSG32 uEROM_BackgroundCheck_Start_Start           :  4;\
            UNSG32 RSVDxC8_b4                                  : 28;\
          }
    union { UNSG32 u32BCM_Security_EROM_BackgroundCheck_Start;
            struct w32BCM_Security_EROM_BackgroundCheck_Start;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_BackgroundCheck_Password_Password(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_BackgroundCheck_Password_Password(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_BackgroundCheck_Password  {\
            UNSG32 uEROM_BackgroundCheck_Password_Password     : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_BackgroundCheck_Password;
            struct w32BCM_Security_EROM_BackgroundCheck_Password;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_BackgroundCheck_Password_match_Password_match(r32) _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_EROM_BackgroundCheck_Password_match_Password_match(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_EROM_BackgroundCheck_Password_match_Password_match(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_EROM_BackgroundCheck_Password_match_Password_match(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32BCM_Security_EROM_BackgroundCheck_Password_match {\
            UNSG32 uEROM_BackgroundCheck_Password_match_Password_match :  1;\
            UNSG32 RSVDxD0_b1                                  : 31;\
          }
    union { UNSG32 u32BCM_Security_EROM_BackgroundCheck_Password_match;
            struct w32BCM_Security_EROM_BackgroundCheck_Password_match;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_SCS_Start_Address_Start_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_SCS_Start_Address_Start_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_SCS_Start_Address         {\
            UNSG32 uIROM_SCS_Start_Address_Start_Address       : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_SCS_Start_Address;
            struct w32BCM_Security_IROM_SCS_Start_Address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_SCS_End_Address_End_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_SCS_End_Address_End_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_SCS_End_Address           {\
            UNSG32 uIROM_SCS_End_Address_End_Address           : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_SCS_End_Address;
            struct w32BCM_Security_IROM_SCS_End_Address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_SCS_Mask_Mask(r32)        _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_IROM_SCS_Mask_Mask(r32,v)      _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_IROM_SCS_Mask_Mask(r16)        _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_IROM_SCS_Mask_Mask(r16,v)      _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_IROM_SCS_Mask                  {\
            UNSG32 uIROM_SCS_Mask_Mask                         :  4;\
            UNSG32 RSVDxDC_b4                                  : 28;\
          }
    union { UNSG32 u32BCM_Security_IROM_SCS_Mask;
            struct w32BCM_Security_IROM_SCS_Mask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc(r32) _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_BCM_reset_2_soc_BCM_reset_2_soc(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32BCM_Security_BCM_reset_2_soc                {\
            UNSG32 uBCM_reset_2_soc_BCM_reset_2_soc            :  1;\
            UNSG32 RSVDxE0_b1                                  : 31;\
          }
    union { UNSG32 u32BCM_Security_BCM_reset_2_soc;
            struct w32BCM_Security_BCM_reset_2_soc;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_InstructionFilter_Start_Start(r32) _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_EROM_InstructionFilter_Start_Start(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_EROM_InstructionFilter_Start_Start(r16) _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_EROM_InstructionFilter_Start_Start(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_EROM_InstructionFilter_Start   {\
            UNSG32 uEROM_InstructionFilter_Start_Start         :  4;\
            UNSG32 RSVDxE4_b4                                  : 28;\
          }
    union { UNSG32 u32BCM_Security_EROM_InstructionFilter_Start;
            struct w32BCM_Security_EROM_InstructionFilter_Start;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_InstructionFilter_Lock_Lock(r32) _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_EROM_InstructionFilter_Lock_Lock(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_EROM_InstructionFilter_Lock_Lock(r16) _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_EROM_InstructionFilter_Lock_Lock(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_EROM_InstructionFilter_Lock    {\
            UNSG32 uEROM_InstructionFilter_Lock_Lock           :  4;\
            UNSG32 RSVDxE8_b4                                  : 28;\
          }
    union { UNSG32 u32BCM_Security_EROM_InstructionFilter_Lock;
            struct w32BCM_Security_EROM_InstructionFilter_Lock;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDxEC                                     [4];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_Instruction_Range_Range0_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range0_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range         {\
            UNSG32 uEROM_Instruction_Range_Range0_Low          : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range;
            struct w32BCM_Security_EROM_Instruction_Range;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range0_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range0_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range1        {\
            UNSG32 uEROM_Instruction_Range_Range0_High         : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range1;
            struct w32BCM_Security_EROM_Instruction_Range1;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range1_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range1_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range2        {\
            UNSG32 uEROM_Instruction_Range_Range1_Low          : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range2;
            struct w32BCM_Security_EROM_Instruction_Range2;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range1_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range1_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range3        {\
            UNSG32 uEROM_Instruction_Range_Range1_High         : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range3;
            struct w32BCM_Security_EROM_Instruction_Range3;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range2_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range2_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range4        {\
            UNSG32 uEROM_Instruction_Range_Range2_Low          : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range4;
            struct w32BCM_Security_EROM_Instruction_Range4;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range2_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range2_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range5        {\
            UNSG32 uEROM_Instruction_Range_Range2_High         : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range5;
            struct w32BCM_Security_EROM_Instruction_Range5;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range3_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range3_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range6        {\
            UNSG32 uEROM_Instruction_Range_Range3_Low          : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range6;
            struct w32BCM_Security_EROM_Instruction_Range6;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range3_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range3_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range7        {\
            UNSG32 uEROM_Instruction_Range_Range3_High         : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range7;
            struct w32BCM_Security_EROM_Instruction_Range7;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range4_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range4_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range8        {\
            UNSG32 uEROM_Instruction_Range_Range4_Low          : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range8;
            struct w32BCM_Security_EROM_Instruction_Range8;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range4_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range4_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range9        {\
            UNSG32 uEROM_Instruction_Range_Range4_High         : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range9;
            struct w32BCM_Security_EROM_Instruction_Range9;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range5_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range5_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range10       {\
            UNSG32 uEROM_Instruction_Range_Range5_Low          : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range10;
            struct w32BCM_Security_EROM_Instruction_Range10;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range5_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range5_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range11       {\
            UNSG32 uEROM_Instruction_Range_Range5_High         : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range11;
            struct w32BCM_Security_EROM_Instruction_Range11;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range6_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range6_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range12       {\
            UNSG32 uEROM_Instruction_Range_Range6_Low          : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range12;
            struct w32BCM_Security_EROM_Instruction_Range12;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range6_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range6_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range13       {\
            UNSG32 uEROM_Instruction_Range_Range6_High         : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range13;
            struct w32BCM_Security_EROM_Instruction_Range13;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range7_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range7_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range14       {\
            UNSG32 uEROM_Instruction_Range_Range7_Low          : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range14;
            struct w32BCM_Security_EROM_Instruction_Range14;
          };
    #define   GET32BCM_Security_EROM_Instruction_Range_Range7_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Instruction_Range_Range7_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Instruction_Range15       {\
            UNSG32 uEROM_Instruction_Range_Range7_High         : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Instruction_Range15;
            struct w32BCM_Security_EROM_Instruction_Range15;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx130                                    [80];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_DataFilter_Start_Start(r32) _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_EROM_DataFilter_Start_Start(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_EROM_DataFilter_Start_Start(r16) _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_EROM_DataFilter_Start_Start(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_EROM_DataFilter_Start          {\
            UNSG32 uEROM_DataFilter_Start_Start                :  4;\
            UNSG32 RSVDx180_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_EROM_DataFilter_Start;
            struct w32BCM_Security_EROM_DataFilter_Start;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_DataFilter_Lock_Lock(r32) _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_EROM_DataFilter_Lock_Lock(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_EROM_DataFilter_Lock_Lock(r16) _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_EROM_DataFilter_Lock_Lock(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_EROM_DataFilter_Lock           {\
            UNSG32 uEROM_DataFilter_Lock_Lock                  :  4;\
            UNSG32 RSVDx184_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_EROM_DataFilter_Lock;
            struct w32BCM_Security_EROM_DataFilter_Lock;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx188                                    [8];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EROM_Data_Range_Range0_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range0_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range                {\
            UNSG32 uEROM_Data_Range_Range0_Low                 : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range;
            struct w32BCM_Security_EROM_Data_Range;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range0_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range0_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range1               {\
            UNSG32 uEROM_Data_Range_Range0_High                : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range1;
            struct w32BCM_Security_EROM_Data_Range1;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range1_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range1_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range2               {\
            UNSG32 uEROM_Data_Range_Range1_Low                 : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range2;
            struct w32BCM_Security_EROM_Data_Range2;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range1_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range1_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range3               {\
            UNSG32 uEROM_Data_Range_Range1_High                : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range3;
            struct w32BCM_Security_EROM_Data_Range3;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range2_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range2_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range4               {\
            UNSG32 uEROM_Data_Range_Range2_Low                 : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range4;
            struct w32BCM_Security_EROM_Data_Range4;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range2_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range2_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range5               {\
            UNSG32 uEROM_Data_Range_Range2_High                : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range5;
            struct w32BCM_Security_EROM_Data_Range5;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range3_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range3_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range6               {\
            UNSG32 uEROM_Data_Range_Range3_Low                 : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range6;
            struct w32BCM_Security_EROM_Data_Range6;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range3_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range3_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range7               {\
            UNSG32 uEROM_Data_Range_Range3_High                : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range7;
            struct w32BCM_Security_EROM_Data_Range7;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range4_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range4_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range8               {\
            UNSG32 uEROM_Data_Range_Range4_Low                 : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range8;
            struct w32BCM_Security_EROM_Data_Range8;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range4_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range4_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range9               {\
            UNSG32 uEROM_Data_Range_Range4_High                : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range9;
            struct w32BCM_Security_EROM_Data_Range9;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range5_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range5_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range10              {\
            UNSG32 uEROM_Data_Range_Range5_Low                 : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range10;
            struct w32BCM_Security_EROM_Data_Range10;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range5_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range5_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range11              {\
            UNSG32 uEROM_Data_Range_Range5_High                : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range11;
            struct w32BCM_Security_EROM_Data_Range11;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range6_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range6_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range12              {\
            UNSG32 uEROM_Data_Range_Range6_Low                 : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range12;
            struct w32BCM_Security_EROM_Data_Range12;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range6_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range6_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range13              {\
            UNSG32 uEROM_Data_Range_Range6_High                : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range13;
            struct w32BCM_Security_EROM_Data_Range13;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range7_Low(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range7_Low(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range14              {\
            UNSG32 uEROM_Data_Range_Range7_Low                 : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range14;
            struct w32BCM_Security_EROM_Data_Range14;
          };
    #define   GET32BCM_Security_EROM_Data_Range_Range7_High(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_EROM_Data_Range_Range7_High(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_EROM_Data_Range15              {\
            UNSG32 uEROM_Data_Range_Range7_High                : 32;\
          }
    union { UNSG32 u32BCM_Security_EROM_Data_Range15;
            struct w32BCM_Security_EROM_Data_Range15;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx1D0                                    [196];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL(r32) _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_Allow_ACPU_Init_AKL_Allow_ACPU_Init_AKL(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32BCM_Security_Allow_ACPU_Init_AKL            {\
            UNSG32 uAllow_ACPU_Init_AKL_Allow_ACPU_Init_AKL    :  1;\
            UNSG32 RSVDx294_b1                                 : 31;\
          }
    union { UNSG32 u32BCM_Security_Allow_ACPU_Init_AKL;
            struct w32BCM_Security_Allow_ACPU_Init_AKL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_reset_address_IROM_reset_address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_reset_address_IROM_reset_address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_reset_address             {\
            UNSG32 uIROM_reset_address_IROM_reset_address      : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_reset_address;
            struct w32BCM_Security_IROM_reset_address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock(r32) _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock(r16) _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_IROM_reset_address_lock_IROM_reset_address_lock(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_IROM_reset_address_lock        {\
            UNSG32 uIROM_reset_address_lock_IROM_reset_address_lock :  4;\
            UNSG32 RSVDx29C_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_IROM_reset_address_lock;
            struct w32BCM_Security_IROM_reset_address_lock;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_Security_Reg[8];
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx2C0                                    [16];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_ACPU_Access0_ACPU_Region_en0(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_ACPU_Access0_ACPU_Region_en0(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_ACPU_Access0                {\
            UNSG32 uKP_ACPU_Access0_ACPU_Region_en0            : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_ACPU_Access0;
            struct w32BCM_Security_KP_ACPU_Access0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_ACPU_Access1_ACPU_Region_en1(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_ACPU_Access1_ACPU_Region_en1(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_ACPU_Access1                {\
            UNSG32 uKP_ACPU_Access1_ACPU_Region_en1            : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_ACPU_Access1;
            struct w32BCM_Security_KP_ACPU_Access1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_ACPU_Access2_ACPU_Region_en2(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_ACPU_Access2_ACPU_Region_en2(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_ACPU_Access2                {\
            UNSG32 uKP_ACPU_Access2_ACPU_Region_en2            : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_ACPU_Access2;
            struct w32BCM_Security_KP_ACPU_Access2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_ACPU_Access3_ACPU_Region_en3(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_ACPU_Access3_ACPU_Region_en3(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_ACPU_Access3                {\
            UNSG32 uKP_ACPU_Access3_ACPU_Region_en3            : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_ACPU_Access3;
            struct w32BCM_Security_KP_ACPU_Access3;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_BCM_Access0_BCM_Region_en0(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_BCM_Access0_BCM_Region_en0(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_BCM_Access0                 {\
            UNSG32 uKP_BCM_Access0_BCM_Region_en0              : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_BCM_Access0;
            struct w32BCM_Security_KP_BCM_Access0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_BCM_Access1_BCM_Region_en1(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_BCM_Access1_BCM_Region_en1(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_BCM_Access1                 {\
            UNSG32 uKP_BCM_Access1_BCM_Region_en1              : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_BCM_Access1;
            struct w32BCM_Security_KP_BCM_Access1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_BCM_Access2_BCM_Region_en2(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_BCM_Access2_BCM_Region_en2(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_BCM_Access2                 {\
            UNSG32 uKP_BCM_Access2_BCM_Region_en2              : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_BCM_Access2;
            struct w32BCM_Security_KP_BCM_Access2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_BCM_Access3_BCM_Region_en3(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_BCM_Access3_BCM_Region_en3(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_BCM_Access3                 {\
            UNSG32 uKP_BCM_Access3_BCM_Region_en3              : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_BCM_Access3;
            struct w32BCM_Security_KP_BCM_Access3;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_IFCP_Access0_IFCP_Region_en0(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_IFCP_Access0_IFCP_Region_en0(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_IFCP_Access0                {\
            UNSG32 uKP_IFCP_Access0_IFCP_Region_en0            : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_IFCP_Access0;
            struct w32BCM_Security_KP_IFCP_Access0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_IFCP_Access1_IFCP_Region_en1(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_IFCP_Access1_IFCP_Region_en1(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_IFCP_Access1                {\
            UNSG32 uKP_IFCP_Access1_IFCP_Region_en1            : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_IFCP_Access1;
            struct w32BCM_Security_KP_IFCP_Access1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_IFCP_Access2_IFCP_Region_en2(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_IFCP_Access2_IFCP_Region_en2(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_IFCP_Access2                {\
            UNSG32 uKP_IFCP_Access2_IFCP_Region_en2            : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_IFCP_Access2;
            struct w32BCM_Security_KP_IFCP_Access2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_IFCP_Access3_IFCP_Region_en3(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_KP_IFCP_Access3_IFCP_Region_en3(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_KP_IFCP_Access3                {\
            UNSG32 uKP_IFCP_Access3_IFCP_Region_en3            : 32;\
          }
    union { UNSG32 u32BCM_Security_KP_IFCP_Access3;
            struct w32BCM_Security_KP_IFCP_Access3;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_ACPU_Access_lock_lock(r32)  _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_KP_ACPU_Access_lock_lock(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_KP_ACPU_Access_lock_lock(r16)  _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_KP_ACPU_Access_lock_lock(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_KP_ACPU_Access_lock            {\
            UNSG32 uKP_ACPU_Access_lock_lock                   :  4;\
            UNSG32 RSVDx300_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_KP_ACPU_Access_lock;
            struct w32BCM_Security_KP_ACPU_Access_lock;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_BCM_Access_lock_lock(r32)   _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_KP_BCM_Access_lock_lock(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_KP_BCM_Access_lock_lock(r16)   _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_KP_BCM_Access_lock_lock(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_KP_BCM_Access_lock             {\
            UNSG32 uKP_BCM_Access_lock_lock                    :  4;\
            UNSG32 RSVDx304_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_KP_BCM_Access_lock;
            struct w32BCM_Security_KP_BCM_Access_lock;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_KP_IFCP_Access_lock_lock(r32)  _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_KP_IFCP_Access_lock_lock(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_KP_IFCP_Access_lock_lock(r16)  _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_KP_IFCP_Access_lock_lock(r16,v) _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_KP_IFCP_Access_lock            {\
            UNSG32 uKP_IFCP_Access_lock_lock                   :  4;\
            UNSG32 RSVDx308_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_KP_IFCP_Access_lock;
            struct w32BCM_Security_KP_IFCP_Access_lock;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_Spare_reg_Reg0(r32)            _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Spare_reg_Reg0(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Spare_reg                      {\
            UNSG32 uSpare_reg_Reg0                             : 32;\
          }
    union { UNSG32 u32BCM_Security_Spare_reg;
            struct w32BCM_Security_Spare_reg;
          };
    #define   GET32BCM_Security_Spare_reg_Reg1(r32)            _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Spare_reg_Reg1(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Spare_reg1                     {\
            UNSG32 uSpare_reg_Reg1                             : 32;\
          }
    union { UNSG32 u32BCM_Security_Spare_reg1;
            struct w32BCM_Security_Spare_reg1;
          };
    #define   GET32BCM_Security_Spare_reg_Reg2(r32)            _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Spare_reg_Reg2(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Spare_reg2                     {\
            UNSG32 uSpare_reg_Reg2                             : 32;\
          }
    union { UNSG32 u32BCM_Security_Spare_reg2;
            struct w32BCM_Security_Spare_reg2;
          };
    #define   GET32BCM_Security_Spare_reg_Reg3(r32)            _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Spare_reg_Reg3(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Spare_reg3                     {\
            UNSG32 uSpare_reg_Reg3                             : 32;\
          }
    union { UNSG32 u32BCM_Security_Spare_reg3;
            struct w32BCM_Security_Spare_reg3;
          };
    #define   GET32BCM_Security_Spare_reg_Reg4(r32)            _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Spare_reg_Reg4(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Spare_reg4                     {\
            UNSG32 uSpare_reg_Reg4                             : 32;\
          }
    union { UNSG32 u32BCM_Security_Spare_reg4;
            struct w32BCM_Security_Spare_reg4;
          };
    #define   GET32BCM_Security_Spare_reg_Reg5(r32)            _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Spare_reg_Reg5(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Spare_reg5                     {\
            UNSG32 uSpare_reg_Reg5                             : 32;\
          }
    union { UNSG32 u32BCM_Security_Spare_reg5;
            struct w32BCM_Security_Spare_reg5;
          };
    #define   GET32BCM_Security_Spare_reg_Reg6(r32)            _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Spare_reg_Reg6(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Spare_reg6                     {\
            UNSG32 uSpare_reg_Reg6                             : 32;\
          }
    union { UNSG32 u32BCM_Security_Spare_reg6;
            struct w32BCM_Security_Spare_reg6;
          };
    #define   GET32BCM_Security_Spare_reg_Reg7(r32)            _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_Spare_reg_Reg7(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_Spare_reg7                     {\
            UNSG32 uSpare_reg_Reg7                             : 32;\
          }
    union { UNSG32 u32BCM_Security_Spare_reg7;
            struct w32BCM_Security_Spare_reg7;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_FI_Status_FI_PL_SOC(r32)       _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_FI_Status_FI_PL_SOC(r32,v)     _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_FI_Status_FI_PL_SOC(r16)       _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_FI_Status_FI_PL_SOC(r16,v)     _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_FI_Status_FI_BIU(r32)          _BFGET_(r32, 1, 1)
    #define   SET32BCM_Security_FI_Status_FI_BIU(r32,v)        _BFSET_(r32, 1, 1,v)
    #define   GET16BCM_Security_FI_Status_FI_BIU(r16)          _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_FI_Status_FI_BIU(r16,v)        _BFSET_(r16, 1, 1,v)

    #define     w32BCM_Security_FI_Status                      {\
            UNSG32 uFI_Status_FI_PL_SOC                        :  1;\
            UNSG32 uFI_Status_FI_BIU                           :  1;\
            UNSG32 RSVDx32C_b2                                 : 30;\
          }
    union { UNSG32 u32BCM_Security_FI_Status;
            struct w32BCM_Security_FI_Status;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx330                                    [4];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IFCP_RAM_Addr_Key_RAM_Addr_Key(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IFCP_RAM_Addr_Key_RAM_Addr_Key(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IFCP_RAM_Addr_Key              {\
            UNSG32 uIFCP_RAM_Addr_Key_RAM_Addr_Key             : 32;\
          }
    union { UNSG32 u32BCM_Security_IFCP_RAM_Addr_Key;
            struct w32BCM_Security_IFCP_RAM_Addr_Key;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IFCP_RAM_Data_Key_RAM_Data_Key(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IFCP_RAM_Data_Key_RAM_Data_Key(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IFCP_RAM_Data_Key              {\
            UNSG32 uIFCP_RAM_Data_Key_RAM_Data_Key             : 32;\
          }
    union { UNSG32 u32BCM_Security_IFCP_RAM_Data_Key;
            struct w32BCM_Security_IFCP_RAM_Data_Key;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx33C                                    [4];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask1_Start_Address_Start_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_Mask1_Start_Address_Start_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_Mask1_Start_Address       {\
            UNSG32 uIROM_Mask1_Start_Address_Start_Address     : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask1_Start_Address;
            struct w32BCM_Security_IROM_Mask1_Start_Address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask1_End_Address_End_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_Mask1_End_Address_End_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_Mask1_End_Address         {\
            UNSG32 uIROM_Mask1_End_Address_End_Address         : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask1_End_Address;
            struct w32BCM_Security_IROM_Mask1_End_Address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask1_Mask_Mask(r32)      _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_IROM_Mask1_Mask_Mask(r32,v)    _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_IROM_Mask1_Mask_Mask(r16)      _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_IROM_Mask1_Mask_Mask(r16,v)    _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_IROM_Mask1_Mask                {\
            UNSG32 uIROM_Mask1_Mask_Mask                       :  4;\
            UNSG32 RSVDx348_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask1_Mask;
            struct w32BCM_Security_IROM_Mask1_Mask;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx34C                                    [4];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask2_Start_Address_Start_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_Mask2_Start_Address_Start_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_Mask2_Start_Address       {\
            UNSG32 uIROM_Mask2_Start_Address_Start_Address     : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask2_Start_Address;
            struct w32BCM_Security_IROM_Mask2_Start_Address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask2_End_Address_End_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_Mask2_End_Address_End_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_Mask2_End_Address         {\
            UNSG32 uIROM_Mask2_End_Address_End_Address         : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask2_End_Address;
            struct w32BCM_Security_IROM_Mask2_End_Address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask2_Mask_Mask(r32)      _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_IROM_Mask2_Mask_Mask(r32,v)    _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_IROM_Mask2_Mask_Mask(r16)      _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_IROM_Mask2_Mask_Mask(r16,v)    _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_IROM_Mask2_Mask                {\
            UNSG32 uIROM_Mask2_Mask_Mask                       :  4;\
            UNSG32 RSVDx358_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask2_Mask;
            struct w32BCM_Security_IROM_Mask2_Mask;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx35C                                    [4];
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask3_Start_Address_Start_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_Mask3_Start_Address_Start_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_Mask3_Start_Address       {\
            UNSG32 uIROM_Mask3_Start_Address_Start_Address     : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask3_Start_Address;
            struct w32BCM_Security_IROM_Mask3_Start_Address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask3_End_Address_End_Address(r32) _BFGET_(r32,31, 0)
    #define   SET32BCM_Security_IROM_Mask3_End_Address_End_Address(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32BCM_Security_IROM_Mask3_End_Address         {\
            UNSG32 uIROM_Mask3_End_Address_End_Address         : 32;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask3_End_Address;
            struct w32BCM_Security_IROM_Mask3_End_Address;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_IROM_Mask3_Mask_Mask(r32)      _BFGET_(r32, 3, 0)
    #define   SET32BCM_Security_IROM_Mask3_Mask_Mask(r32,v)    _BFSET_(r32, 3, 0,v)
    #define   GET16BCM_Security_IROM_Mask3_Mask_Mask(r16)      _BFGET_(r16, 3, 0)
    #define   SET16BCM_Security_IROM_Mask3_Mask_Mask(r16,v)    _BFSET_(r16, 3, 0,v)

    #define     w32BCM_Security_IROM_Mask3_Mask                {\
            UNSG32 uIROM_Mask3_Mask_Mask                       :  4;\
            UNSG32 RSVDx368_b4                                 : 28;\
          }
    union { UNSG32 u32BCM_Security_IROM_Mask3_Mask;
            struct w32BCM_Security_IROM_Mask3_Mask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation(r32) _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation(r16) _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_CERT_Priv_Mode_CFG_Secure_Boot_Activation(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation(r32) _BFGET_(r32, 1, 1)
    #define   SET32BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation(r16) _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_CERT_Priv_Mode_CFG_Flash_Protection_Activation(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32BCM_Security_CERT_Priv_Mode_CFG_RFU(r32)    _BFGET_(r32,31, 2)
    #define   SET32BCM_Security_CERT_Priv_Mode_CFG_RFU(r32,v)  _BFSET_(r32,31, 2,v)

    #define     w32BCM_Security_CERT_Priv_Mode_CFG             {\
            UNSG32 uCERT_Priv_Mode_CFG_Secure_Boot_Activation  :  1;\
            UNSG32 uCERT_Priv_Mode_CFG_Flash_Protection_Activation :  1;\
            UNSG32 uCERT_Priv_Mode_CFG_RFU                     : 30;\
          }
    union { UNSG32 u32BCM_Security_CERT_Priv_Mode_CFG;
            struct w32BCM_Security_CERT_Priv_Mode_CFG;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_Security_EnvMon_Status_Clock(r32)       _BFGET_(r32, 0, 0)
    #define   SET32BCM_Security_EnvMon_Status_Clock(r32,v)     _BFSET_(r32, 0, 0,v)
    #define   GET16BCM_Security_EnvMon_Status_Clock(r16)       _BFGET_(r16, 0, 0)
    #define   SET16BCM_Security_EnvMon_Status_Clock(r16,v)     _BFSET_(r16, 0, 0,v)

    #define   GET32BCM_Security_EnvMon_Status_Reset(r32)       _BFGET_(r32, 1, 1)
    #define   SET32BCM_Security_EnvMon_Status_Reset(r32,v)     _BFSET_(r32, 1, 1,v)
    #define   GET16BCM_Security_EnvMon_Status_Reset(r16)       _BFGET_(r16, 1, 1)
    #define   SET16BCM_Security_EnvMon_Status_Reset(r16,v)     _BFSET_(r16, 1, 1,v)

    #define   GET32BCM_Security_EnvMon_Status_Voltage(r32)     _BFGET_(r32, 2, 2)
    #define   SET32BCM_Security_EnvMon_Status_Voltage(r32,v)   _BFSET_(r32, 2, 2,v)
    #define   GET16BCM_Security_EnvMon_Status_Voltage(r16)     _BFGET_(r16, 2, 2)
    #define   SET16BCM_Security_EnvMon_Status_Voltage(r16,v)   _BFSET_(r16, 2, 2,v)

    #define     w32BCM_Security_EnvMon_Status                  {\
            UNSG32 uEnvMon_Status_Clock                        :  1;\
            UNSG32 uEnvMon_Status_Reset                        :  1;\
            UNSG32 uEnvMon_Status_Voltage                      :  1;\
            UNSG32 RSVDx370_b3                                 : 29;\
          }
    union { UNSG32 u32BCM_Security_EnvMon_Status;
            struct w32BCM_Security_EnvMon_Status;
          };
    ///////////////////////////////////////////////////////////
    } SIE_BCM_Security;

    typedef union  T32BCM_Security_V_Version
          { UNSG32 u32;
            struct w32BCM_Security_V_Version;
                 } T32BCM_Security_V_Version;
    typedef union  T32BCM_Security_Berlin_DDR_Base
          { UNSG32 u32;
            struct w32BCM_Security_Berlin_DDR_Base;
                 } T32BCM_Security_Berlin_DDR_Base;
    typedef union  T32BCM_Security_Berlin_SOC_Base
          { UNSG32 u32;
            struct w32BCM_Security_Berlin_SOC_Base;
                 } T32BCM_Security_Berlin_SOC_Base;
    typedef union  T32BCM_Security_BCM_DIR_En
          { UNSG32 u32;
            struct w32BCM_Security_BCM_DIR_En;
                 } T32BCM_Security_BCM_DIR_En;
    typedef union  T32BCM_Security_ACPU_Access
          { UNSG32 u32;
            struct w32BCM_Security_ACPU_Access;
                 } T32BCM_Security_ACPU_Access;
    typedef union  T32BCM_Security_Efuse_Read_Disable
          { UNSG32 u32;
            struct w32BCM_Security_Efuse_Read_Disable;
                 } T32BCM_Security_Efuse_Read_Disable;
    typedef union  T32BCM_Security_Efuse_Read_Disable1
          { UNSG32 u32;
            struct w32BCM_Security_Efuse_Read_Disable1;
                 } T32BCM_Security_Efuse_Read_Disable1;
    typedef union  T32BCM_Security_RKEKRegReadDisable
          { UNSG32 u32;
            struct w32BCM_Security_RKEKRegReadDisable;
                 } T32BCM_Security_RKEKRegReadDisable;
    typedef union  T32BCM_Security_RKEKRegWriteDisable
          { UNSG32 u32;
            struct w32BCM_Security_RKEKRegWriteDisable;
                 } T32BCM_Security_RKEKRegWriteDisable;
    typedef union  T32BCM_Security_FeatureCtrlReg
          { UNSG32 u32;
            struct w32BCM_Security_FeatureCtrlReg;
                 } T32BCM_Security_FeatureCtrlReg;
    typedef union  T32BCM_Security_FeatureCtrlReg1
          { UNSG32 u32;
            struct w32BCM_Security_FeatureCtrlReg1;
                 } T32BCM_Security_FeatureCtrlReg1;
    typedef union  T32BCM_Security_FeatureCtrlReg2
          { UNSG32 u32;
            struct w32BCM_Security_FeatureCtrlReg2;
                 } T32BCM_Security_FeatureCtrlReg2;
    typedef union  T32BCM_Security_FeatureCtrlReg3
          { UNSG32 u32;
            struct w32BCM_Security_FeatureCtrlReg3;
                 } T32BCM_Security_FeatureCtrlReg3;
    typedef union  T32BCM_Security_FeatureCtrlReg4
          { UNSG32 u32;
            struct w32BCM_Security_FeatureCtrlReg4;
                 } T32BCM_Security_FeatureCtrlReg4;
    typedef union  T32BCM_Security_EROM_Golden_CRC
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC;
                 } T32BCM_Security_EROM_Golden_CRC;
    typedef union  T32BCM_Security_EROM_Golden_CRC1
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC1;
                 } T32BCM_Security_EROM_Golden_CRC1;
    typedef union  T32BCM_Security_EROM_Golden_CRC2
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC2;
                 } T32BCM_Security_EROM_Golden_CRC2;
    typedef union  T32BCM_Security_EROM_Golden_CRC3
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC3;
                 } T32BCM_Security_EROM_Golden_CRC3;
    typedef union  T32BCM_Security_EROM_Golden_CRC4
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC4;
                 } T32BCM_Security_EROM_Golden_CRC4;
    typedef union  T32BCM_Security_EROM_Golden_CRC5
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC5;
                 } T32BCM_Security_EROM_Golden_CRC5;
    typedef union  T32BCM_Security_EROM_Golden_CRC6
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC6;
                 } T32BCM_Security_EROM_Golden_CRC6;
    typedef union  T32BCM_Security_EROM_Golden_CRC7
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC7;
                 } T32BCM_Security_EROM_Golden_CRC7;
    typedef union  T32BCM_Security_EROM_Golden_CRC8
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC8;
                 } T32BCM_Security_EROM_Golden_CRC8;
    typedef union  T32BCM_Security_EROM_Golden_CRC9
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC9;
                 } T32BCM_Security_EROM_Golden_CRC9;
    typedef union  T32BCM_Security_EROM_Golden_CRC10
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC10;
                 } T32BCM_Security_EROM_Golden_CRC10;
    typedef union  T32BCM_Security_EROM_Golden_CRC11
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC11;
                 } T32BCM_Security_EROM_Golden_CRC11;
    typedef union  T32BCM_Security_EROM_Golden_CRC12
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC12;
                 } T32BCM_Security_EROM_Golden_CRC12;
    typedef union  T32BCM_Security_EROM_Golden_CRC13
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC13;
                 } T32BCM_Security_EROM_Golden_CRC13;
    typedef union  T32BCM_Security_EROM_Golden_CRC14
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC14;
                 } T32BCM_Security_EROM_Golden_CRC14;
    typedef union  T32BCM_Security_EROM_Golden_CRC15
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Golden_CRC15;
                 } T32BCM_Security_EROM_Golden_CRC15;
    typedef union  T32BCM_Security_EROM_BackgroundCheck_Start_Addresss
          { UNSG32 u32;
            struct w32BCM_Security_EROM_BackgroundCheck_Start_Addresss;
                 } T32BCM_Security_EROM_BackgroundCheck_Start_Addresss;
    typedef union  T32BCM_Security_EROM_BackgroundCheck_End_Addresss
          { UNSG32 u32;
            struct w32BCM_Security_EROM_BackgroundCheck_End_Addresss;
                 } T32BCM_Security_EROM_BackgroundCheck_End_Addresss;
    typedef union  T32BCM_Security_EROM_BackgroundCheck_Start
          { UNSG32 u32;
            struct w32BCM_Security_EROM_BackgroundCheck_Start;
                 } T32BCM_Security_EROM_BackgroundCheck_Start;
    typedef union  T32BCM_Security_EROM_BackgroundCheck_Password
          { UNSG32 u32;
            struct w32BCM_Security_EROM_BackgroundCheck_Password;
                 } T32BCM_Security_EROM_BackgroundCheck_Password;
    typedef union  T32BCM_Security_EROM_BackgroundCheck_Password_match
          { UNSG32 u32;
            struct w32BCM_Security_EROM_BackgroundCheck_Password_match;
                 } T32BCM_Security_EROM_BackgroundCheck_Password_match;
    typedef union  T32BCM_Security_IROM_SCS_Start_Address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_SCS_Start_Address;
                 } T32BCM_Security_IROM_SCS_Start_Address;
    typedef union  T32BCM_Security_IROM_SCS_End_Address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_SCS_End_Address;
                 } T32BCM_Security_IROM_SCS_End_Address;
    typedef union  T32BCM_Security_IROM_SCS_Mask
          { UNSG32 u32;
            struct w32BCM_Security_IROM_SCS_Mask;
                 } T32BCM_Security_IROM_SCS_Mask;
    typedef union  T32BCM_Security_BCM_reset_2_soc
          { UNSG32 u32;
            struct w32BCM_Security_BCM_reset_2_soc;
                 } T32BCM_Security_BCM_reset_2_soc;
    typedef union  T32BCM_Security_EROM_InstructionFilter_Start
          { UNSG32 u32;
            struct w32BCM_Security_EROM_InstructionFilter_Start;
                 } T32BCM_Security_EROM_InstructionFilter_Start;
    typedef union  T32BCM_Security_EROM_InstructionFilter_Lock
          { UNSG32 u32;
            struct w32BCM_Security_EROM_InstructionFilter_Lock;
                 } T32BCM_Security_EROM_InstructionFilter_Lock;
    typedef union  T32BCM_Security_EROM_Instruction_Range
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range;
                 } T32BCM_Security_EROM_Instruction_Range;
    typedef union  T32BCM_Security_EROM_Instruction_Range1
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range1;
                 } T32BCM_Security_EROM_Instruction_Range1;
    typedef union  T32BCM_Security_EROM_Instruction_Range2
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range2;
                 } T32BCM_Security_EROM_Instruction_Range2;
    typedef union  T32BCM_Security_EROM_Instruction_Range3
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range3;
                 } T32BCM_Security_EROM_Instruction_Range3;
    typedef union  T32BCM_Security_EROM_Instruction_Range4
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range4;
                 } T32BCM_Security_EROM_Instruction_Range4;
    typedef union  T32BCM_Security_EROM_Instruction_Range5
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range5;
                 } T32BCM_Security_EROM_Instruction_Range5;
    typedef union  T32BCM_Security_EROM_Instruction_Range6
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range6;
                 } T32BCM_Security_EROM_Instruction_Range6;
    typedef union  T32BCM_Security_EROM_Instruction_Range7
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range7;
                 } T32BCM_Security_EROM_Instruction_Range7;
    typedef union  T32BCM_Security_EROM_Instruction_Range8
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range8;
                 } T32BCM_Security_EROM_Instruction_Range8;
    typedef union  T32BCM_Security_EROM_Instruction_Range9
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range9;
                 } T32BCM_Security_EROM_Instruction_Range9;
    typedef union  T32BCM_Security_EROM_Instruction_Range10
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range10;
                 } T32BCM_Security_EROM_Instruction_Range10;
    typedef union  T32BCM_Security_EROM_Instruction_Range11
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range11;
                 } T32BCM_Security_EROM_Instruction_Range11;
    typedef union  T32BCM_Security_EROM_Instruction_Range12
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range12;
                 } T32BCM_Security_EROM_Instruction_Range12;
    typedef union  T32BCM_Security_EROM_Instruction_Range13
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range13;
                 } T32BCM_Security_EROM_Instruction_Range13;
    typedef union  T32BCM_Security_EROM_Instruction_Range14
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range14;
                 } T32BCM_Security_EROM_Instruction_Range14;
    typedef union  T32BCM_Security_EROM_Instruction_Range15
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Instruction_Range15;
                 } T32BCM_Security_EROM_Instruction_Range15;
    typedef union  T32BCM_Security_EROM_DataFilter_Start
          { UNSG32 u32;
            struct w32BCM_Security_EROM_DataFilter_Start;
                 } T32BCM_Security_EROM_DataFilter_Start;
    typedef union  T32BCM_Security_EROM_DataFilter_Lock
          { UNSG32 u32;
            struct w32BCM_Security_EROM_DataFilter_Lock;
                 } T32BCM_Security_EROM_DataFilter_Lock;
    typedef union  T32BCM_Security_EROM_Data_Range
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range;
                 } T32BCM_Security_EROM_Data_Range;
    typedef union  T32BCM_Security_EROM_Data_Range1
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range1;
                 } T32BCM_Security_EROM_Data_Range1;
    typedef union  T32BCM_Security_EROM_Data_Range2
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range2;
                 } T32BCM_Security_EROM_Data_Range2;
    typedef union  T32BCM_Security_EROM_Data_Range3
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range3;
                 } T32BCM_Security_EROM_Data_Range3;
    typedef union  T32BCM_Security_EROM_Data_Range4
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range4;
                 } T32BCM_Security_EROM_Data_Range4;
    typedef union  T32BCM_Security_EROM_Data_Range5
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range5;
                 } T32BCM_Security_EROM_Data_Range5;
    typedef union  T32BCM_Security_EROM_Data_Range6
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range6;
                 } T32BCM_Security_EROM_Data_Range6;
    typedef union  T32BCM_Security_EROM_Data_Range7
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range7;
                 } T32BCM_Security_EROM_Data_Range7;
    typedef union  T32BCM_Security_EROM_Data_Range8
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range8;
                 } T32BCM_Security_EROM_Data_Range8;
    typedef union  T32BCM_Security_EROM_Data_Range9
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range9;
                 } T32BCM_Security_EROM_Data_Range9;
    typedef union  T32BCM_Security_EROM_Data_Range10
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range10;
                 } T32BCM_Security_EROM_Data_Range10;
    typedef union  T32BCM_Security_EROM_Data_Range11
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range11;
                 } T32BCM_Security_EROM_Data_Range11;
    typedef union  T32BCM_Security_EROM_Data_Range12
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range12;
                 } T32BCM_Security_EROM_Data_Range12;
    typedef union  T32BCM_Security_EROM_Data_Range13
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range13;
                 } T32BCM_Security_EROM_Data_Range13;
    typedef union  T32BCM_Security_EROM_Data_Range14
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range14;
                 } T32BCM_Security_EROM_Data_Range14;
    typedef union  T32BCM_Security_EROM_Data_Range15
          { UNSG32 u32;
            struct w32BCM_Security_EROM_Data_Range15;
                 } T32BCM_Security_EROM_Data_Range15;
    typedef union  T32BCM_Security_Allow_ACPU_Init_AKL
          { UNSG32 u32;
            struct w32BCM_Security_Allow_ACPU_Init_AKL;
                 } T32BCM_Security_Allow_ACPU_Init_AKL;
    typedef union  T32BCM_Security_IROM_reset_address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_reset_address;
                 } T32BCM_Security_IROM_reset_address;
    typedef union  T32BCM_Security_IROM_reset_address_lock
          { UNSG32 u32;
            struct w32BCM_Security_IROM_reset_address_lock;
                 } T32BCM_Security_IROM_reset_address_lock;
    typedef union  T32BCM_Security_KP_ACPU_Access0
          { UNSG32 u32;
            struct w32BCM_Security_KP_ACPU_Access0;
                 } T32BCM_Security_KP_ACPU_Access0;
    typedef union  T32BCM_Security_KP_ACPU_Access1
          { UNSG32 u32;
            struct w32BCM_Security_KP_ACPU_Access1;
                 } T32BCM_Security_KP_ACPU_Access1;
    typedef union  T32BCM_Security_KP_ACPU_Access2
          { UNSG32 u32;
            struct w32BCM_Security_KP_ACPU_Access2;
                 } T32BCM_Security_KP_ACPU_Access2;
    typedef union  T32BCM_Security_KP_ACPU_Access3
          { UNSG32 u32;
            struct w32BCM_Security_KP_ACPU_Access3;
                 } T32BCM_Security_KP_ACPU_Access3;
    typedef union  T32BCM_Security_KP_BCM_Access0
          { UNSG32 u32;
            struct w32BCM_Security_KP_BCM_Access0;
                 } T32BCM_Security_KP_BCM_Access0;
    typedef union  T32BCM_Security_KP_BCM_Access1
          { UNSG32 u32;
            struct w32BCM_Security_KP_BCM_Access1;
                 } T32BCM_Security_KP_BCM_Access1;
    typedef union  T32BCM_Security_KP_BCM_Access2
          { UNSG32 u32;
            struct w32BCM_Security_KP_BCM_Access2;
                 } T32BCM_Security_KP_BCM_Access2;
    typedef union  T32BCM_Security_KP_BCM_Access3
          { UNSG32 u32;
            struct w32BCM_Security_KP_BCM_Access3;
                 } T32BCM_Security_KP_BCM_Access3;
    typedef union  T32BCM_Security_KP_IFCP_Access0
          { UNSG32 u32;
            struct w32BCM_Security_KP_IFCP_Access0;
                 } T32BCM_Security_KP_IFCP_Access0;
    typedef union  T32BCM_Security_KP_IFCP_Access1
          { UNSG32 u32;
            struct w32BCM_Security_KP_IFCP_Access1;
                 } T32BCM_Security_KP_IFCP_Access1;
    typedef union  T32BCM_Security_KP_IFCP_Access2
          { UNSG32 u32;
            struct w32BCM_Security_KP_IFCP_Access2;
                 } T32BCM_Security_KP_IFCP_Access2;
    typedef union  T32BCM_Security_KP_IFCP_Access3
          { UNSG32 u32;
            struct w32BCM_Security_KP_IFCP_Access3;
                 } T32BCM_Security_KP_IFCP_Access3;
    typedef union  T32BCM_Security_KP_ACPU_Access_lock
          { UNSG32 u32;
            struct w32BCM_Security_KP_ACPU_Access_lock;
                 } T32BCM_Security_KP_ACPU_Access_lock;
    typedef union  T32BCM_Security_KP_BCM_Access_lock
          { UNSG32 u32;
            struct w32BCM_Security_KP_BCM_Access_lock;
                 } T32BCM_Security_KP_BCM_Access_lock;
    typedef union  T32BCM_Security_KP_IFCP_Access_lock
          { UNSG32 u32;
            struct w32BCM_Security_KP_IFCP_Access_lock;
                 } T32BCM_Security_KP_IFCP_Access_lock;
    typedef union  T32BCM_Security_Spare_reg
          { UNSG32 u32;
            struct w32BCM_Security_Spare_reg;
                 } T32BCM_Security_Spare_reg;
    typedef union  T32BCM_Security_Spare_reg1
          { UNSG32 u32;
            struct w32BCM_Security_Spare_reg1;
                 } T32BCM_Security_Spare_reg1;
    typedef union  T32BCM_Security_Spare_reg2
          { UNSG32 u32;
            struct w32BCM_Security_Spare_reg2;
                 } T32BCM_Security_Spare_reg2;
    typedef union  T32BCM_Security_Spare_reg3
          { UNSG32 u32;
            struct w32BCM_Security_Spare_reg3;
                 } T32BCM_Security_Spare_reg3;
    typedef union  T32BCM_Security_Spare_reg4
          { UNSG32 u32;
            struct w32BCM_Security_Spare_reg4;
                 } T32BCM_Security_Spare_reg4;
    typedef union  T32BCM_Security_Spare_reg5
          { UNSG32 u32;
            struct w32BCM_Security_Spare_reg5;
                 } T32BCM_Security_Spare_reg5;
    typedef union  T32BCM_Security_Spare_reg6
          { UNSG32 u32;
            struct w32BCM_Security_Spare_reg6;
                 } T32BCM_Security_Spare_reg6;
    typedef union  T32BCM_Security_Spare_reg7
          { UNSG32 u32;
            struct w32BCM_Security_Spare_reg7;
                 } T32BCM_Security_Spare_reg7;
    typedef union  T32BCM_Security_FI_Status
          { UNSG32 u32;
            struct w32BCM_Security_FI_Status;
                 } T32BCM_Security_FI_Status;
    typedef union  T32BCM_Security_IFCP_RAM_Addr_Key
          { UNSG32 u32;
            struct w32BCM_Security_IFCP_RAM_Addr_Key;
                 } T32BCM_Security_IFCP_RAM_Addr_Key;
    typedef union  T32BCM_Security_IFCP_RAM_Data_Key
          { UNSG32 u32;
            struct w32BCM_Security_IFCP_RAM_Data_Key;
                 } T32BCM_Security_IFCP_RAM_Data_Key;
    typedef union  T32BCM_Security_IROM_Mask1_Start_Address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask1_Start_Address;
                 } T32BCM_Security_IROM_Mask1_Start_Address;
    typedef union  T32BCM_Security_IROM_Mask1_End_Address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask1_End_Address;
                 } T32BCM_Security_IROM_Mask1_End_Address;
    typedef union  T32BCM_Security_IROM_Mask1_Mask
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask1_Mask;
                 } T32BCM_Security_IROM_Mask1_Mask;
    typedef union  T32BCM_Security_IROM_Mask2_Start_Address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask2_Start_Address;
                 } T32BCM_Security_IROM_Mask2_Start_Address;
    typedef union  T32BCM_Security_IROM_Mask2_End_Address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask2_End_Address;
                 } T32BCM_Security_IROM_Mask2_End_Address;
    typedef union  T32BCM_Security_IROM_Mask2_Mask
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask2_Mask;
                 } T32BCM_Security_IROM_Mask2_Mask;
    typedef union  T32BCM_Security_IROM_Mask3_Start_Address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask3_Start_Address;
                 } T32BCM_Security_IROM_Mask3_Start_Address;
    typedef union  T32BCM_Security_IROM_Mask3_End_Address
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask3_End_Address;
                 } T32BCM_Security_IROM_Mask3_End_Address;
    typedef union  T32BCM_Security_IROM_Mask3_Mask
          { UNSG32 u32;
            struct w32BCM_Security_IROM_Mask3_Mask;
                 } T32BCM_Security_IROM_Mask3_Mask;
    typedef union  T32BCM_Security_CERT_Priv_Mode_CFG
          { UNSG32 u32;
            struct w32BCM_Security_CERT_Priv_Mode_CFG;
                 } T32BCM_Security_CERT_Priv_Mode_CFG;
    typedef union  T32BCM_Security_EnvMon_Status
          { UNSG32 u32;
            struct w32BCM_Security_EnvMon_Status;
                 } T32BCM_Security_EnvMon_Status;
    ///////////////////////////////////////////////////////////

    typedef union  TBCM_Security_V_Version
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_V_Version;
                   };
                 } TBCM_Security_V_Version;
    typedef union  TBCM_Security_Berlin_DDR_Base
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_Berlin_DDR_Base;
                   };
                 } TBCM_Security_Berlin_DDR_Base;
    typedef union  TBCM_Security_Berlin_SOC_Base
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_Berlin_SOC_Base;
                   };
                 } TBCM_Security_Berlin_SOC_Base;
    typedef union  TBCM_Security_BCM_DIR_En
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_BCM_DIR_En;
                   };
                 } TBCM_Security_BCM_DIR_En;
    typedef union  TBCM_Security_ACPU_Access
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_ACPU_Access;
                   };
                 } TBCM_Security_ACPU_Access;
    typedef union  TBCM_Security_Efuse_Read_Disable
          { UNSG32 u32[2];
            struct {
            struct w32BCM_Security_Efuse_Read_Disable;
            struct w32BCM_Security_Efuse_Read_Disable1;
                   };
                 } TBCM_Security_Efuse_Read_Disable;
    typedef union  TBCM_Security_RKEKRegReadDisable
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_RKEKRegReadDisable;
                   };
                 } TBCM_Security_RKEKRegReadDisable;
    typedef union  TBCM_Security_RKEKRegWriteDisable
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_RKEKRegWriteDisable;
                   };
                 } TBCM_Security_RKEKRegWriteDisable;
    typedef union  TBCM_Security_FeatureCtrlReg
          { UNSG32 u32[5];
            struct {
            struct w32BCM_Security_FeatureCtrlReg;
            struct w32BCM_Security_FeatureCtrlReg1;
            struct w32BCM_Security_FeatureCtrlReg2;
            struct w32BCM_Security_FeatureCtrlReg3;
            struct w32BCM_Security_FeatureCtrlReg4;
                   };
                 } TBCM_Security_FeatureCtrlReg;
    typedef union  TBCM_Security_EROM_Golden_CRC
          { UNSG32 u32[16];
            struct {
            struct w32BCM_Security_EROM_Golden_CRC;
            struct w32BCM_Security_EROM_Golden_CRC1;
            struct w32BCM_Security_EROM_Golden_CRC2;
            struct w32BCM_Security_EROM_Golden_CRC3;
            struct w32BCM_Security_EROM_Golden_CRC4;
            struct w32BCM_Security_EROM_Golden_CRC5;
            struct w32BCM_Security_EROM_Golden_CRC6;
            struct w32BCM_Security_EROM_Golden_CRC7;
            struct w32BCM_Security_EROM_Golden_CRC8;
            struct w32BCM_Security_EROM_Golden_CRC9;
            struct w32BCM_Security_EROM_Golden_CRC10;
            struct w32BCM_Security_EROM_Golden_CRC11;
            struct w32BCM_Security_EROM_Golden_CRC12;
            struct w32BCM_Security_EROM_Golden_CRC13;
            struct w32BCM_Security_EROM_Golden_CRC14;
            struct w32BCM_Security_EROM_Golden_CRC15;
                   };
                 } TBCM_Security_EROM_Golden_CRC;
    typedef union  TBCM_Security_EROM_BackgroundCheck_Start_Addresss
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_BackgroundCheck_Start_Addresss;
                   };
                 } TBCM_Security_EROM_BackgroundCheck_Start_Addresss;
    typedef union  TBCM_Security_EROM_BackgroundCheck_End_Addresss
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_BackgroundCheck_End_Addresss;
                   };
                 } TBCM_Security_EROM_BackgroundCheck_End_Addresss;
    typedef union  TBCM_Security_EROM_BackgroundCheck_Start
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_BackgroundCheck_Start;
                   };
                 } TBCM_Security_EROM_BackgroundCheck_Start;
    typedef union  TBCM_Security_EROM_BackgroundCheck_Password
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_BackgroundCheck_Password;
                   };
                 } TBCM_Security_EROM_BackgroundCheck_Password;
    typedef union  TBCM_Security_EROM_BackgroundCheck_Password_match
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_BackgroundCheck_Password_match;
                   };
                 } TBCM_Security_EROM_BackgroundCheck_Password_match;
    typedef union  TBCM_Security_IROM_SCS_Start_Address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_SCS_Start_Address;
                   };
                 } TBCM_Security_IROM_SCS_Start_Address;
    typedef union  TBCM_Security_IROM_SCS_End_Address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_SCS_End_Address;
                   };
                 } TBCM_Security_IROM_SCS_End_Address;
    typedef union  TBCM_Security_IROM_SCS_Mask
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_SCS_Mask;
                   };
                 } TBCM_Security_IROM_SCS_Mask;
    typedef union  TBCM_Security_BCM_reset_2_soc
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_BCM_reset_2_soc;
                   };
                 } TBCM_Security_BCM_reset_2_soc;
    typedef union  TBCM_Security_EROM_InstructionFilter_Start
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_InstructionFilter_Start;
                   };
                 } TBCM_Security_EROM_InstructionFilter_Start;
    typedef union  TBCM_Security_EROM_InstructionFilter_Lock
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_InstructionFilter_Lock;
                   };
                 } TBCM_Security_EROM_InstructionFilter_Lock;
    typedef union  TBCM_Security_EROM_Instruction_Range
          { UNSG32 u32[16];
            struct {
            struct w32BCM_Security_EROM_Instruction_Range;
            struct w32BCM_Security_EROM_Instruction_Range1;
            struct w32BCM_Security_EROM_Instruction_Range2;
            struct w32BCM_Security_EROM_Instruction_Range3;
            struct w32BCM_Security_EROM_Instruction_Range4;
            struct w32BCM_Security_EROM_Instruction_Range5;
            struct w32BCM_Security_EROM_Instruction_Range6;
            struct w32BCM_Security_EROM_Instruction_Range7;
            struct w32BCM_Security_EROM_Instruction_Range8;
            struct w32BCM_Security_EROM_Instruction_Range9;
            struct w32BCM_Security_EROM_Instruction_Range10;
            struct w32BCM_Security_EROM_Instruction_Range11;
            struct w32BCM_Security_EROM_Instruction_Range12;
            struct w32BCM_Security_EROM_Instruction_Range13;
            struct w32BCM_Security_EROM_Instruction_Range14;
            struct w32BCM_Security_EROM_Instruction_Range15;
                   };
                 } TBCM_Security_EROM_Instruction_Range;
    typedef union  TBCM_Security_EROM_DataFilter_Start
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_DataFilter_Start;
                   };
                 } TBCM_Security_EROM_DataFilter_Start;
    typedef union  TBCM_Security_EROM_DataFilter_Lock
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EROM_DataFilter_Lock;
                   };
                 } TBCM_Security_EROM_DataFilter_Lock;
    typedef union  TBCM_Security_EROM_Data_Range
          { UNSG32 u32[16];
            struct {
            struct w32BCM_Security_EROM_Data_Range;
            struct w32BCM_Security_EROM_Data_Range1;
            struct w32BCM_Security_EROM_Data_Range2;
            struct w32BCM_Security_EROM_Data_Range3;
            struct w32BCM_Security_EROM_Data_Range4;
            struct w32BCM_Security_EROM_Data_Range5;
            struct w32BCM_Security_EROM_Data_Range6;
            struct w32BCM_Security_EROM_Data_Range7;
            struct w32BCM_Security_EROM_Data_Range8;
            struct w32BCM_Security_EROM_Data_Range9;
            struct w32BCM_Security_EROM_Data_Range10;
            struct w32BCM_Security_EROM_Data_Range11;
            struct w32BCM_Security_EROM_Data_Range12;
            struct w32BCM_Security_EROM_Data_Range13;
            struct w32BCM_Security_EROM_Data_Range14;
            struct w32BCM_Security_EROM_Data_Range15;
                   };
                 } TBCM_Security_EROM_Data_Range;
    typedef union  TBCM_Security_Allow_ACPU_Init_AKL
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_Allow_ACPU_Init_AKL;
                   };
                 } TBCM_Security_Allow_ACPU_Init_AKL;
    typedef union  TBCM_Security_IROM_reset_address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_reset_address;
                   };
                 } TBCM_Security_IROM_reset_address;
    typedef union  TBCM_Security_IROM_reset_address_lock
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_reset_address_lock;
                   };
                 } TBCM_Security_IROM_reset_address_lock;
    typedef union  TBCM_Security_KP_ACPU_Access0
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_ACPU_Access0;
                   };
                 } TBCM_Security_KP_ACPU_Access0;
    typedef union  TBCM_Security_KP_ACPU_Access1
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_ACPU_Access1;
                   };
                 } TBCM_Security_KP_ACPU_Access1;
    typedef union  TBCM_Security_KP_ACPU_Access2
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_ACPU_Access2;
                   };
                 } TBCM_Security_KP_ACPU_Access2;
    typedef union  TBCM_Security_KP_ACPU_Access3
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_ACPU_Access3;
                   };
                 } TBCM_Security_KP_ACPU_Access3;
    typedef union  TBCM_Security_KP_BCM_Access0
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_BCM_Access0;
                   };
                 } TBCM_Security_KP_BCM_Access0;
    typedef union  TBCM_Security_KP_BCM_Access1
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_BCM_Access1;
                   };
                 } TBCM_Security_KP_BCM_Access1;
    typedef union  TBCM_Security_KP_BCM_Access2
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_BCM_Access2;
                   };
                 } TBCM_Security_KP_BCM_Access2;
    typedef union  TBCM_Security_KP_BCM_Access3
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_BCM_Access3;
                   };
                 } TBCM_Security_KP_BCM_Access3;
    typedef union  TBCM_Security_KP_IFCP_Access0
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_IFCP_Access0;
                   };
                 } TBCM_Security_KP_IFCP_Access0;
    typedef union  TBCM_Security_KP_IFCP_Access1
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_IFCP_Access1;
                   };
                 } TBCM_Security_KP_IFCP_Access1;
    typedef union  TBCM_Security_KP_IFCP_Access2
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_IFCP_Access2;
                   };
                 } TBCM_Security_KP_IFCP_Access2;
    typedef union  TBCM_Security_KP_IFCP_Access3
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_IFCP_Access3;
                   };
                 } TBCM_Security_KP_IFCP_Access3;
    typedef union  TBCM_Security_KP_ACPU_Access_lock
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_ACPU_Access_lock;
                   };
                 } TBCM_Security_KP_ACPU_Access_lock;
    typedef union  TBCM_Security_KP_BCM_Access_lock
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_BCM_Access_lock;
                   };
                 } TBCM_Security_KP_BCM_Access_lock;
    typedef union  TBCM_Security_KP_IFCP_Access_lock
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_KP_IFCP_Access_lock;
                   };
                 } TBCM_Security_KP_IFCP_Access_lock;
    typedef union  TBCM_Security_Spare_reg
          { UNSG32 u32[8];
            struct {
            struct w32BCM_Security_Spare_reg;
            struct w32BCM_Security_Spare_reg1;
            struct w32BCM_Security_Spare_reg2;
            struct w32BCM_Security_Spare_reg3;
            struct w32BCM_Security_Spare_reg4;
            struct w32BCM_Security_Spare_reg5;
            struct w32BCM_Security_Spare_reg6;
            struct w32BCM_Security_Spare_reg7;
                   };
                 } TBCM_Security_Spare_reg;
    typedef union  TBCM_Security_FI_Status
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_FI_Status;
                   };
                 } TBCM_Security_FI_Status;
    typedef union  TBCM_Security_IFCP_RAM_Addr_Key
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IFCP_RAM_Addr_Key;
                   };
                 } TBCM_Security_IFCP_RAM_Addr_Key;
    typedef union  TBCM_Security_IFCP_RAM_Data_Key
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IFCP_RAM_Data_Key;
                   };
                 } TBCM_Security_IFCP_RAM_Data_Key;
    typedef union  TBCM_Security_IROM_Mask1_Start_Address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask1_Start_Address;
                   };
                 } TBCM_Security_IROM_Mask1_Start_Address;
    typedef union  TBCM_Security_IROM_Mask1_End_Address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask1_End_Address;
                   };
                 } TBCM_Security_IROM_Mask1_End_Address;
    typedef union  TBCM_Security_IROM_Mask1_Mask
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask1_Mask;
                   };
                 } TBCM_Security_IROM_Mask1_Mask;
    typedef union  TBCM_Security_IROM_Mask2_Start_Address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask2_Start_Address;
                   };
                 } TBCM_Security_IROM_Mask2_Start_Address;
    typedef union  TBCM_Security_IROM_Mask2_End_Address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask2_End_Address;
                   };
                 } TBCM_Security_IROM_Mask2_End_Address;
    typedef union  TBCM_Security_IROM_Mask2_Mask
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask2_Mask;
                   };
                 } TBCM_Security_IROM_Mask2_Mask;
    typedef union  TBCM_Security_IROM_Mask3_Start_Address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask3_Start_Address;
                   };
                 } TBCM_Security_IROM_Mask3_Start_Address;
    typedef union  TBCM_Security_IROM_Mask3_End_Address
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask3_End_Address;
                   };
                 } TBCM_Security_IROM_Mask3_End_Address;
    typedef union  TBCM_Security_IROM_Mask3_Mask
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_IROM_Mask3_Mask;
                   };
                 } TBCM_Security_IROM_Mask3_Mask;
    typedef union  TBCM_Security_CERT_Priv_Mode_CFG
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_CERT_Priv_Mode_CFG;
                   };
                 } TBCM_Security_CERT_Priv_Mode_CFG;
    typedef union  TBCM_Security_EnvMon_Status
          { UNSG32 u32[1];
            struct {
            struct w32BCM_Security_EnvMon_Status;
                   };
                 } TBCM_Security_EnvMon_Status;

    ///////////////////////////////////////////////////////////
     SIGN32 BCM_Security_drvrd(SIE_BCM_Security *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCM_Security_drvwr(SIE_BCM_Security *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCM_Security_reset(SIE_BCM_Security *p);
     SIGN32 BCM_Security_cmp  (SIE_BCM_Security *p, SIE_BCM_Security *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCM_Security_check(p,pie,pfx,hLOG) BCM_Security_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCM_Security_print(p,    pfx,hLOG) BCM_Security_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: BCM_Security
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE DBG_IF                                   (4,4)
///     ###
///     * Only BCM can access this register
///     ###
///     # # ----------------------------------------------------------
///     @ 0x00000 MRVL_JTAG_password_0 (P)
///     # 0x00000 JTAG_password_0      
///               $OneReg              JTAG_password_0   REG       [4]
///                                    ###
///                                    * Golden password 0. Only SCPU can read/write it
///                                    ###
///     @ 0x00010 MRVL_JTAG_password_1 (P)
///     # 0x00010 JTAG_password_1      
///               $OneReg              JTAG_password_1   REG       [4]
///                                    ###
///                                    * Golden password 1. Only SCPU can read/write it
///                                    ###
///     @ 0x00020 MRVL_JTAG_password_valid (P)
///               %unsigned 2  JTAG_password_valid       0x0
///                                    ###
///                                    * bit0: password_0 valid
///                                    * bit1: password_1 valid
///                                    * Only SCPU can read/write it
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x00024 MRVL_JTAG_protection (P)
///               %unsigned 2  JTAG_protection           0x1
///                                    ###
///                                    * 00 :open,
///                                    * 01:password
///                                    * 10/11: close
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x00028                      (W-)
///     #         # Stuffing bytes...
///               %% 64
///     @ 0x00030 MRVL_BSCAN_password_0 (P)
///     # 0x00030 BSCAN_password_0     
///               $OneReg              BSCAN_password_0  REG       [4]
///                                    ###
///                                    * Golden password 0. Only SCPU can read/write it
///                                    ###
///     @ 0x00040 MRVL_BSCAN_password_1 (P)
///     # 0x00040 BSCAN_password_1     
///               $OneReg              BSCAN_password_1  REG       [4]
///                                    ###
///                                    * Golden password 1. Only SCPU can read/write it
///                                    ###
///     @ 0x00050 MRVL_BSCAN_password_valid (P)
///               %unsigned 2  BSCAN_password_valid      0x0
///                                    ###
///                                    * bit0: password_0 valid
///                                    * bit1: password_1 valid
///                                    * Only SCPU can read/write it
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x00054 MRVL_BSCAN_protection (P)
///               %unsigned 2  BSCAN_protection          0x1
///                                    ###
///                                    * 00 :open,
///                                    * 01:password
///                                    * 10/11: close
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x00058                      (W-)
///     #         # Stuffing bytes...
///               %% 64
///     @ 0x00060 MRVL_SCANTEST_password_0 (P)
///     # 0x00060 SCANTEST_password_0  
///               $OneReg              SCANTEST_password_0 REG       [4]
///                                    ###
///                                    * Golden password 0. Only SCPU can read/write it
///                                    ###
///     @ 0x00070 MRVL_SCANTEST_password_1 (P)
///     # 0x00070 SCANTEST_password_1  
///               $OneReg              SCANTEST_password_1 REG       [4]
///                                    ###
///                                    * Golden password 1. Only SCPU can read/write it
///                                    ###
///     @ 0x00080 MRVL_SCANTEST_password_valid (P)
///               %unsigned 2  SCANTEST_password_valid   0x0
///                                    ###
///                                    * bit0: password_0 valid
///                                    * bit1: password_1 valid
///                                    * Only SCPU can read/write it
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x00084 MRVL_STEST_protection (P)
///               %unsigned 2  STEST_protection          0x1
///                                    ###
///                                    * 00 :open,
///                                    * 01:password
///                                    * 10/11: close
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x00088                      (W-)
///     #         # Stuffing bytes...
///               %% 64
///     @ 0x00090 IceEn                (P)
///               %unsigned 1  ZspIceEn                  0x0
///                                    ###
///                                    * ZSP JTAG enable
///                                    * Open for ACPU to read
///                                    ###
///               %unsigned 1  Cpu0IceEn                 0x1
///                                    ###
///                                    * CPU0 JTAG enable
///                                    * Open for ACPU to read
///                                    ###
///               %unsigned 1  Cpu1IceEn                 0x0
///                                    ###
///                                    * CPU1 JTAG enable
///                                    * Open for ACPU to read
///                                    ###
///               %%        29         # Stuffing bits...
///     @ 0x00094 Debug_On             (R-)
///               %unsigned 8  Debug_On                  0x0
///                                    ###
///                                    * 0: debug is not active
///                                    * 1: debug is active
///                                    * During testing, user can set this register through JTAG I/F. IROM needs to read out this register every time when it wants to perform a SW reset.
///                                    ###
///               %%        24         # Stuffing bits...
///     @ 0x00098 Hold_Dbg             (RW)
///               ###
///               * 1: DBG I/F is closed
///               * 0: DBG I/F status is determined by OTP field.
///               * BCM must set this register to 0 after SCS
///               ###
///               %unsigned 1  JTAG                      0x1
///               %unsigned 1  BSCAN                     0x1
///               %unsigned 1  SCANTEST                  0x1
///               %unsigned 1  TEE                       0x1
///               %%        28         # Stuffing bits...
///     @ 0x0009C                      (W-)
///     #         # Stuffing bytes...
///               %% 32
///     @ 0x000A0 TEEDBG_Mrvl_password_0 (P)
///     # 0x000A0 TEEDBG_Mrvl_password_0 
///               $OneReg              TEEDBG_Mrvl_password_0 REG       [4]
///                                    ###
///                                    * user password
///                                    ###
///     @ 0x000B0 TEEDBG_Mrvl_password_1 (P)
///     # 0x000B0 TEEDBG_Mrvl_password_1 
///               $OneReg              TEEDBG_Mrvl_password_1 REG       [4]
///                                    ###
///                                    * user password
///                                    ###
///     @ 0x000C0 TEEDBG_Mrvl_password_valid (P)
///               %unsigned 2  TEEDBG_Mrvl_password_valid 0x0
///                                    ###
///                                    * bit0: password_0 valid
///                                    * bit1: password_1 valid
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x000C4 TEEDBG_Mrvl_protected (P)
///               %unsigned 2  TEEDBG_Mrvl_protected     0x1
///                                    ###
///                                    * 00 open
///                                    * 01 password
///                                    * 10/11 close
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x000C8 DIP_EnStatus         (R-)
///               ###
///               * DIP status for IROM to read
///               ###
///               %unsigned 1  REEJTAG                   0x0
///                                    ###
///                                    * 0: not enabled; 1: enabled
///                                    ###
///               %unsigned 1  TEE                       0x0
///                                    ###
///                                    * 0: not enabled; 1: enabled
///                                    ###
///               %unsigned 1  BSCAN                     0x0
///                                    ###
///                                    * 0: not enabled; 1: enabled
///                                    ###
///               %unsigned 1  STEST                     0x0
///                                    ###
///                                    * 0: not enabled; 1: enabled
///                                    ###
///               %%        28         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:     204B, bits:    1059b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_DBG_IF
#define h_DBG_IF (){}

    #define     RA_DBG_IF_MRVL_JTAG_password_0                 0x0000
    #define     RA_DBG_IF_JTAG_password_0                      0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_JTAG_password_1                 0x0010
    #define     RA_DBG_IF_JTAG_password_1                      0x0010
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_JTAG_password_valid             0x0020

    #define     BA_DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid 0x0020
    #define     B16DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid 0x0020
    #define   LSb32DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid    0
    #define   LSb16DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid    0
    #define       bDBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid 2
    #define   MSK32DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid    0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_JTAG_protection                 0x0024

    #define     BA_DBG_IF_MRVL_JTAG_protection_JTAG_protection 0x0024
    #define     B16DBG_IF_MRVL_JTAG_protection_JTAG_protection 0x0024
    #define   LSb32DBG_IF_MRVL_JTAG_protection_JTAG_protection    0
    #define   LSb16DBG_IF_MRVL_JTAG_protection_JTAG_protection    0
    #define       bDBG_IF_MRVL_JTAG_protection_JTAG_protection 2
    #define   MSK32DBG_IF_MRVL_JTAG_protection_JTAG_protection    0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_BSCAN_password_0                0x0030
    #define     RA_DBG_IF_BSCAN_password_0                     0x0030
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_BSCAN_password_1                0x0040
    #define     RA_DBG_IF_BSCAN_password_1                     0x0040
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_BSCAN_password_valid            0x0050

    #define     BA_DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid 0x0050
    #define     B16DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid 0x0050
    #define   LSb32DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid    0
    #define   LSb16DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid    0
    #define       bDBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid 2
    #define   MSK32DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid    0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_BSCAN_protection                0x0054

    #define     BA_DBG_IF_MRVL_BSCAN_protection_BSCAN_protection 0x0054
    #define     B16DBG_IF_MRVL_BSCAN_protection_BSCAN_protection 0x0054
    #define   LSb32DBG_IF_MRVL_BSCAN_protection_BSCAN_protection    0
    #define   LSb16DBG_IF_MRVL_BSCAN_protection_BSCAN_protection    0
    #define       bDBG_IF_MRVL_BSCAN_protection_BSCAN_protection 2
    #define   MSK32DBG_IF_MRVL_BSCAN_protection_BSCAN_protection    0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_SCANTEST_password_0             0x0060
    #define     RA_DBG_IF_SCANTEST_password_0                  0x0060
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_SCANTEST_password_1             0x0070
    #define     RA_DBG_IF_SCANTEST_password_1                  0x0070
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_SCANTEST_password_valid         0x0080

    #define     BA_DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid 0x0080
    #define     B16DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid 0x0080
    #define   LSb32DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid    0
    #define   LSb16DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid    0
    #define       bDBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid 2
    #define   MSK32DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid    0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_MRVL_STEST_protection                0x0084

    #define     BA_DBG_IF_MRVL_STEST_protection_STEST_protection 0x0084
    #define     B16DBG_IF_MRVL_STEST_protection_STEST_protection 0x0084
    #define   LSb32DBG_IF_MRVL_STEST_protection_STEST_protection    0
    #define   LSb16DBG_IF_MRVL_STEST_protection_STEST_protection    0
    #define       bDBG_IF_MRVL_STEST_protection_STEST_protection 2
    #define   MSK32DBG_IF_MRVL_STEST_protection_STEST_protection    0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_IceEn                                0x0090

    #define     BA_DBG_IF_IceEn_ZspIceEn                       0x0090
    #define     B16DBG_IF_IceEn_ZspIceEn                       0x0090
    #define   LSb32DBG_IF_IceEn_ZspIceEn                          0
    #define   LSb16DBG_IF_IceEn_ZspIceEn                          0
    #define       bDBG_IF_IceEn_ZspIceEn                       1
    #define   MSK32DBG_IF_IceEn_ZspIceEn                          0x00000001

    #define     BA_DBG_IF_IceEn_Cpu0IceEn                      0x0090
    #define     B16DBG_IF_IceEn_Cpu0IceEn                      0x0090
    #define   LSb32DBG_IF_IceEn_Cpu0IceEn                         1
    #define   LSb16DBG_IF_IceEn_Cpu0IceEn                         1
    #define       bDBG_IF_IceEn_Cpu0IceEn                      1
    #define   MSK32DBG_IF_IceEn_Cpu0IceEn                         0x00000002

    #define     BA_DBG_IF_IceEn_Cpu1IceEn                      0x0090
    #define     B16DBG_IF_IceEn_Cpu1IceEn                      0x0090
    #define   LSb32DBG_IF_IceEn_Cpu1IceEn                         2
    #define   LSb16DBG_IF_IceEn_Cpu1IceEn                         2
    #define       bDBG_IF_IceEn_Cpu1IceEn                      1
    #define   MSK32DBG_IF_IceEn_Cpu1IceEn                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_Debug_On                             0x0094

    #define     BA_DBG_IF_Debug_On_Debug_On                    0x0094
    #define     B16DBG_IF_Debug_On_Debug_On                    0x0094
    #define   LSb32DBG_IF_Debug_On_Debug_On                       0
    #define   LSb16DBG_IF_Debug_On_Debug_On                       0
    #define       bDBG_IF_Debug_On_Debug_On                    8
    #define   MSK32DBG_IF_Debug_On_Debug_On                       0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_Hold_Dbg                             0x0098

    #define     BA_DBG_IF_Hold_Dbg_JTAG                        0x0098
    #define     B16DBG_IF_Hold_Dbg_JTAG                        0x0098
    #define   LSb32DBG_IF_Hold_Dbg_JTAG                           0
    #define   LSb16DBG_IF_Hold_Dbg_JTAG                           0
    #define       bDBG_IF_Hold_Dbg_JTAG                        1
    #define   MSK32DBG_IF_Hold_Dbg_JTAG                           0x00000001

    #define     BA_DBG_IF_Hold_Dbg_BSCAN                       0x0098
    #define     B16DBG_IF_Hold_Dbg_BSCAN                       0x0098
    #define   LSb32DBG_IF_Hold_Dbg_BSCAN                          1
    #define   LSb16DBG_IF_Hold_Dbg_BSCAN                          1
    #define       bDBG_IF_Hold_Dbg_BSCAN                       1
    #define   MSK32DBG_IF_Hold_Dbg_BSCAN                          0x00000002

    #define     BA_DBG_IF_Hold_Dbg_SCANTEST                    0x0098
    #define     B16DBG_IF_Hold_Dbg_SCANTEST                    0x0098
    #define   LSb32DBG_IF_Hold_Dbg_SCANTEST                       2
    #define   LSb16DBG_IF_Hold_Dbg_SCANTEST                       2
    #define       bDBG_IF_Hold_Dbg_SCANTEST                    1
    #define   MSK32DBG_IF_Hold_Dbg_SCANTEST                       0x00000004

    #define     BA_DBG_IF_Hold_Dbg_TEE                         0x0098
    #define     B16DBG_IF_Hold_Dbg_TEE                         0x0098
    #define   LSb32DBG_IF_Hold_Dbg_TEE                            3
    #define   LSb16DBG_IF_Hold_Dbg_TEE                            3
    #define       bDBG_IF_Hold_Dbg_TEE                         1
    #define   MSK32DBG_IF_Hold_Dbg_TEE                            0x00000008
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_TEEDBG_Mrvl_password_0               0x00A0
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_TEEDBG_Mrvl_password_1               0x00B0
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_TEEDBG_Mrvl_password_valid           0x00C0

    #define     BA_DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid 0x00C0
    #define     B16DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid 0x00C0
    #define   LSb32DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid    0
    #define   LSb16DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid    0
    #define       bDBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid 2
    #define   MSK32DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid    0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_TEEDBG_Mrvl_protected                0x00C4

    #define     BA_DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected 0x00C4
    #define     B16DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected 0x00C4
    #define   LSb32DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected    0
    #define   LSb16DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected    0
    #define       bDBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected 2
    #define   MSK32DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected    0x00000003
    ///////////////////////////////////////////////////////////
    #define     RA_DBG_IF_DIP_EnStatus                         0x00C8

    #define     BA_DBG_IF_DIP_EnStatus_REEJTAG                 0x00C8
    #define     B16DBG_IF_DIP_EnStatus_REEJTAG                 0x00C8
    #define   LSb32DBG_IF_DIP_EnStatus_REEJTAG                    0
    #define   LSb16DBG_IF_DIP_EnStatus_REEJTAG                    0
    #define       bDBG_IF_DIP_EnStatus_REEJTAG                 1
    #define   MSK32DBG_IF_DIP_EnStatus_REEJTAG                    0x00000001

    #define     BA_DBG_IF_DIP_EnStatus_TEE                     0x00C8
    #define     B16DBG_IF_DIP_EnStatus_TEE                     0x00C8
    #define   LSb32DBG_IF_DIP_EnStatus_TEE                        1
    #define   LSb16DBG_IF_DIP_EnStatus_TEE                        1
    #define       bDBG_IF_DIP_EnStatus_TEE                     1
    #define   MSK32DBG_IF_DIP_EnStatus_TEE                        0x00000002

    #define     BA_DBG_IF_DIP_EnStatus_BSCAN                   0x00C8
    #define     B16DBG_IF_DIP_EnStatus_BSCAN                   0x00C8
    #define   LSb32DBG_IF_DIP_EnStatus_BSCAN                      2
    #define   LSb16DBG_IF_DIP_EnStatus_BSCAN                      2
    #define       bDBG_IF_DIP_EnStatus_BSCAN                   1
    #define   MSK32DBG_IF_DIP_EnStatus_BSCAN                      0x00000004

    #define     BA_DBG_IF_DIP_EnStatus_STEST                   0x00C8
    #define     B16DBG_IF_DIP_EnStatus_STEST                   0x00C8
    #define   LSb32DBG_IF_DIP_EnStatus_STEST                      3
    #define   LSb16DBG_IF_DIP_EnStatus_STEST                      3
    #define       bDBG_IF_DIP_EnStatus_STEST                   1
    #define   MSK32DBG_IF_DIP_EnStatus_STEST                      0x00000008
    ///////////////////////////////////////////////////////////

    typedef struct SIE_DBG_IF {
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_JTAG_password_0[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_JTAG_password_1[4];
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid(r32) _BFGET_(r32, 1, 0)
    #define   SET32DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid(r16) _BFGET_(r16, 1, 0)
    #define   SET16DBG_IF_MRVL_JTAG_password_valid_JTAG_password_valid(r16,v) _BFSET_(r16, 1, 0,v)

    #define     w32DBG_IF_MRVL_JTAG_password_valid             {\
            UNSG32 uMRVL_JTAG_password_valid_JTAG_password_valid :  2;\
            UNSG32 RSVDx20_b2                                  : 30;\
          }
    union { UNSG32 u32DBG_IF_MRVL_JTAG_password_valid;
            struct w32DBG_IF_MRVL_JTAG_password_valid;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_MRVL_JTAG_protection_JTAG_protection(r32) _BFGET_(r32, 1, 0)
    #define   SET32DBG_IF_MRVL_JTAG_protection_JTAG_protection(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16DBG_IF_MRVL_JTAG_protection_JTAG_protection(r16) _BFGET_(r16, 1, 0)
    #define   SET16DBG_IF_MRVL_JTAG_protection_JTAG_protection(r16,v) _BFSET_(r16, 1, 0,v)

    #define     w32DBG_IF_MRVL_JTAG_protection                 {\
            UNSG32 uMRVL_JTAG_protection_JTAG_protection       :  2;\
            UNSG32 RSVDx24_b2                                  : 30;\
          }
    union { UNSG32 u32DBG_IF_MRVL_JTAG_protection;
            struct w32DBG_IF_MRVL_JTAG_protection;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx28                                     [8];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_BSCAN_password_0[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_BSCAN_password_1[4];
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid(r32) _BFGET_(r32, 1, 0)
    #define   SET32DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid(r16) _BFGET_(r16, 1, 0)
    #define   SET16DBG_IF_MRVL_BSCAN_password_valid_BSCAN_password_valid(r16,v) _BFSET_(r16, 1, 0,v)

    #define     w32DBG_IF_MRVL_BSCAN_password_valid            {\
            UNSG32 uMRVL_BSCAN_password_valid_BSCAN_password_valid :  2;\
            UNSG32 RSVDx50_b2                                  : 30;\
          }
    union { UNSG32 u32DBG_IF_MRVL_BSCAN_password_valid;
            struct w32DBG_IF_MRVL_BSCAN_password_valid;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_MRVL_BSCAN_protection_BSCAN_protection(r32) _BFGET_(r32, 1, 0)
    #define   SET32DBG_IF_MRVL_BSCAN_protection_BSCAN_protection(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16DBG_IF_MRVL_BSCAN_protection_BSCAN_protection(r16) _BFGET_(r16, 1, 0)
    #define   SET16DBG_IF_MRVL_BSCAN_protection_BSCAN_protection(r16,v) _BFSET_(r16, 1, 0,v)

    #define     w32DBG_IF_MRVL_BSCAN_protection                {\
            UNSG32 uMRVL_BSCAN_protection_BSCAN_protection     :  2;\
            UNSG32 RSVDx54_b2                                  : 30;\
          }
    union { UNSG32 u32DBG_IF_MRVL_BSCAN_protection;
            struct w32DBG_IF_MRVL_BSCAN_protection;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx58                                     [8];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SCANTEST_password_0[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_SCANTEST_password_1[4];
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid(r32) _BFGET_(r32, 1, 0)
    #define   SET32DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid(r16) _BFGET_(r16, 1, 0)
    #define   SET16DBG_IF_MRVL_SCANTEST_password_valid_SCANTEST_password_valid(r16,v) _BFSET_(r16, 1, 0,v)

    #define     w32DBG_IF_MRVL_SCANTEST_password_valid         {\
            UNSG32 uMRVL_SCANTEST_password_valid_SCANTEST_password_valid :  2;\
            UNSG32 RSVDx80_b2                                  : 30;\
          }
    union { UNSG32 u32DBG_IF_MRVL_SCANTEST_password_valid;
            struct w32DBG_IF_MRVL_SCANTEST_password_valid;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_MRVL_STEST_protection_STEST_protection(r32) _BFGET_(r32, 1, 0)
    #define   SET32DBG_IF_MRVL_STEST_protection_STEST_protection(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16DBG_IF_MRVL_STEST_protection_STEST_protection(r16) _BFGET_(r16, 1, 0)
    #define   SET16DBG_IF_MRVL_STEST_protection_STEST_protection(r16,v) _BFSET_(r16, 1, 0,v)

    #define     w32DBG_IF_MRVL_STEST_protection                {\
            UNSG32 uMRVL_STEST_protection_STEST_protection     :  2;\
            UNSG32 RSVDx84_b2                                  : 30;\
          }
    union { UNSG32 u32DBG_IF_MRVL_STEST_protection;
            struct w32DBG_IF_MRVL_STEST_protection;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx88                                     [8];
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_IceEn_ZspIceEn(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32DBG_IF_IceEn_ZspIceEn(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16DBG_IF_IceEn_ZspIceEn(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16DBG_IF_IceEn_ZspIceEn(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32DBG_IF_IceEn_Cpu0IceEn(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32DBG_IF_IceEn_Cpu0IceEn(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16DBG_IF_IceEn_Cpu0IceEn(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16DBG_IF_IceEn_Cpu0IceEn(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32DBG_IF_IceEn_Cpu1IceEn(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32DBG_IF_IceEn_Cpu1IceEn(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16DBG_IF_IceEn_Cpu1IceEn(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16DBG_IF_IceEn_Cpu1IceEn(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32DBG_IF_IceEn                                {\
            UNSG32 uIceEn_ZspIceEn                             :  1;\
            UNSG32 uIceEn_Cpu0IceEn                            :  1;\
            UNSG32 uIceEn_Cpu1IceEn                            :  1;\
            UNSG32 RSVDx90_b3                                  : 29;\
          }
    union { UNSG32 u32DBG_IF_IceEn;
            struct w32DBG_IF_IceEn;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_Debug_On_Debug_On(r32)               _BFGET_(r32, 7, 0)
    #define   SET32DBG_IF_Debug_On_Debug_On(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   GET16DBG_IF_Debug_On_Debug_On(r16)               _BFGET_(r16, 7, 0)
    #define   SET16DBG_IF_Debug_On_Debug_On(r16,v)             _BFSET_(r16, 7, 0,v)

    #define     w32DBG_IF_Debug_On                             {\
            UNSG32 uDebug_On_Debug_On                          :  8;\
            UNSG32 RSVDx94_b8                                  : 24;\
          }
    union { UNSG32 u32DBG_IF_Debug_On;
            struct w32DBG_IF_Debug_On;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_Hold_Dbg_JTAG(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32DBG_IF_Hold_Dbg_JTAG(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16DBG_IF_Hold_Dbg_JTAG(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16DBG_IF_Hold_Dbg_JTAG(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32DBG_IF_Hold_Dbg_BSCAN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32DBG_IF_Hold_Dbg_BSCAN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16DBG_IF_Hold_Dbg_BSCAN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16DBG_IF_Hold_Dbg_BSCAN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32DBG_IF_Hold_Dbg_SCANTEST(r32)               _BFGET_(r32, 2, 2)
    #define   SET32DBG_IF_Hold_Dbg_SCANTEST(r32,v)             _BFSET_(r32, 2, 2,v)
    #define   GET16DBG_IF_Hold_Dbg_SCANTEST(r16)               _BFGET_(r16, 2, 2)
    #define   SET16DBG_IF_Hold_Dbg_SCANTEST(r16,v)             _BFSET_(r16, 2, 2,v)

    #define   GET32DBG_IF_Hold_Dbg_TEE(r32)                    _BFGET_(r32, 3, 3)
    #define   SET32DBG_IF_Hold_Dbg_TEE(r32,v)                  _BFSET_(r32, 3, 3,v)
    #define   GET16DBG_IF_Hold_Dbg_TEE(r16)                    _BFGET_(r16, 3, 3)
    #define   SET16DBG_IF_Hold_Dbg_TEE(r16,v)                  _BFSET_(r16, 3, 3,v)

    #define     w32DBG_IF_Hold_Dbg                             {\
            UNSG32 uHold_Dbg_JTAG                              :  1;\
            UNSG32 uHold_Dbg_BSCAN                             :  1;\
            UNSG32 uHold_Dbg_SCANTEST                          :  1;\
            UNSG32 uHold_Dbg_TEE                               :  1;\
            UNSG32 RSVDx98_b4                                  : 28;\
          }
    union { UNSG32 u32DBG_IF_Hold_Dbg;
            struct w32DBG_IF_Hold_Dbg;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx9C                                     [4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_TEEDBG_Mrvl_password_0[4];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_TEEDBG_Mrvl_password_1[4];
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid(r32) _BFGET_(r32, 1, 0)
    #define   SET32DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid(r16) _BFGET_(r16, 1, 0)
    #define   SET16DBG_IF_TEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid(r16,v) _BFSET_(r16, 1, 0,v)

    #define     w32DBG_IF_TEEDBG_Mrvl_password_valid           {\
            UNSG32 uTEEDBG_Mrvl_password_valid_TEEDBG_Mrvl_password_valid :  2;\
            UNSG32 RSVDxC0_b2                                  : 30;\
          }
    union { UNSG32 u32DBG_IF_TEEDBG_Mrvl_password_valid;
            struct w32DBG_IF_TEEDBG_Mrvl_password_valid;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected(r32) _BFGET_(r32, 1, 0)
    #define   SET32DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected(r16) _BFGET_(r16, 1, 0)
    #define   SET16DBG_IF_TEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected(r16,v) _BFSET_(r16, 1, 0,v)

    #define     w32DBG_IF_TEEDBG_Mrvl_protected                {\
            UNSG32 uTEEDBG_Mrvl_protected_TEEDBG_Mrvl_protected :  2;\
            UNSG32 RSVDxC4_b2                                  : 30;\
          }
    union { UNSG32 u32DBG_IF_TEEDBG_Mrvl_protected;
            struct w32DBG_IF_TEEDBG_Mrvl_protected;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32DBG_IF_DIP_EnStatus_REEJTAG(r32)            _BFGET_(r32, 0, 0)
    #define   SET32DBG_IF_DIP_EnStatus_REEJTAG(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   GET16DBG_IF_DIP_EnStatus_REEJTAG(r16)            _BFGET_(r16, 0, 0)
    #define   SET16DBG_IF_DIP_EnStatus_REEJTAG(r16,v)          _BFSET_(r16, 0, 0,v)

    #define   GET32DBG_IF_DIP_EnStatus_TEE(r32)                _BFGET_(r32, 1, 1)
    #define   SET32DBG_IF_DIP_EnStatus_TEE(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16DBG_IF_DIP_EnStatus_TEE(r16)                _BFGET_(r16, 1, 1)
    #define   SET16DBG_IF_DIP_EnStatus_TEE(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32DBG_IF_DIP_EnStatus_BSCAN(r32)              _BFGET_(r32, 2, 2)
    #define   SET32DBG_IF_DIP_EnStatus_BSCAN(r32,v)            _BFSET_(r32, 2, 2,v)
    #define   GET16DBG_IF_DIP_EnStatus_BSCAN(r16)              _BFGET_(r16, 2, 2)
    #define   SET16DBG_IF_DIP_EnStatus_BSCAN(r16,v)            _BFSET_(r16, 2, 2,v)

    #define   GET32DBG_IF_DIP_EnStatus_STEST(r32)              _BFGET_(r32, 3, 3)
    #define   SET32DBG_IF_DIP_EnStatus_STEST(r32,v)            _BFSET_(r32, 3, 3,v)
    #define   GET16DBG_IF_DIP_EnStatus_STEST(r16)              _BFGET_(r16, 3, 3)
    #define   SET16DBG_IF_DIP_EnStatus_STEST(r16,v)            _BFSET_(r16, 3, 3,v)

    #define     w32DBG_IF_DIP_EnStatus                         {\
            UNSG32 uDIP_EnStatus_REEJTAG                       :  1;\
            UNSG32 uDIP_EnStatus_TEE                           :  1;\
            UNSG32 uDIP_EnStatus_BSCAN                         :  1;\
            UNSG32 uDIP_EnStatus_STEST                         :  1;\
            UNSG32 RSVDxC8_b4                                  : 28;\
          }
    union { UNSG32 u32DBG_IF_DIP_EnStatus;
            struct w32DBG_IF_DIP_EnStatus;
          };
    ///////////////////////////////////////////////////////////
    } SIE_DBG_IF;

    typedef union  T32DBG_IF_MRVL_JTAG_password_valid
          { UNSG32 u32;
            struct w32DBG_IF_MRVL_JTAG_password_valid;
                 } T32DBG_IF_MRVL_JTAG_password_valid;
    typedef union  T32DBG_IF_MRVL_JTAG_protection
          { UNSG32 u32;
            struct w32DBG_IF_MRVL_JTAG_protection;
                 } T32DBG_IF_MRVL_JTAG_protection;
    typedef union  T32DBG_IF_MRVL_BSCAN_password_valid
          { UNSG32 u32;
            struct w32DBG_IF_MRVL_BSCAN_password_valid;
                 } T32DBG_IF_MRVL_BSCAN_password_valid;
    typedef union  T32DBG_IF_MRVL_BSCAN_protection
          { UNSG32 u32;
            struct w32DBG_IF_MRVL_BSCAN_protection;
                 } T32DBG_IF_MRVL_BSCAN_protection;
    typedef union  T32DBG_IF_MRVL_SCANTEST_password_valid
          { UNSG32 u32;
            struct w32DBG_IF_MRVL_SCANTEST_password_valid;
                 } T32DBG_IF_MRVL_SCANTEST_password_valid;
    typedef union  T32DBG_IF_MRVL_STEST_protection
          { UNSG32 u32;
            struct w32DBG_IF_MRVL_STEST_protection;
                 } T32DBG_IF_MRVL_STEST_protection;
    typedef union  T32DBG_IF_IceEn
          { UNSG32 u32;
            struct w32DBG_IF_IceEn;
                 } T32DBG_IF_IceEn;
    typedef union  T32DBG_IF_Debug_On
          { UNSG32 u32;
            struct w32DBG_IF_Debug_On;
                 } T32DBG_IF_Debug_On;
    typedef union  T32DBG_IF_Hold_Dbg
          { UNSG32 u32;
            struct w32DBG_IF_Hold_Dbg;
                 } T32DBG_IF_Hold_Dbg;
    typedef union  T32DBG_IF_TEEDBG_Mrvl_password_valid
          { UNSG32 u32;
            struct w32DBG_IF_TEEDBG_Mrvl_password_valid;
                 } T32DBG_IF_TEEDBG_Mrvl_password_valid;
    typedef union  T32DBG_IF_TEEDBG_Mrvl_protected
          { UNSG32 u32;
            struct w32DBG_IF_TEEDBG_Mrvl_protected;
                 } T32DBG_IF_TEEDBG_Mrvl_protected;
    typedef union  T32DBG_IF_DIP_EnStatus
          { UNSG32 u32;
            struct w32DBG_IF_DIP_EnStatus;
                 } T32DBG_IF_DIP_EnStatus;
    ///////////////////////////////////////////////////////////

    typedef union  TDBG_IF_MRVL_JTAG_password_valid
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_MRVL_JTAG_password_valid;
                   };
                 } TDBG_IF_MRVL_JTAG_password_valid;
    typedef union  TDBG_IF_MRVL_JTAG_protection
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_MRVL_JTAG_protection;
                   };
                 } TDBG_IF_MRVL_JTAG_protection;
    typedef union  TDBG_IF_MRVL_BSCAN_password_valid
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_MRVL_BSCAN_password_valid;
                   };
                 } TDBG_IF_MRVL_BSCAN_password_valid;
    typedef union  TDBG_IF_MRVL_BSCAN_protection
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_MRVL_BSCAN_protection;
                   };
                 } TDBG_IF_MRVL_BSCAN_protection;
    typedef union  TDBG_IF_MRVL_SCANTEST_password_valid
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_MRVL_SCANTEST_password_valid;
                   };
                 } TDBG_IF_MRVL_SCANTEST_password_valid;
    typedef union  TDBG_IF_MRVL_STEST_protection
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_MRVL_STEST_protection;
                   };
                 } TDBG_IF_MRVL_STEST_protection;
    typedef union  TDBG_IF_IceEn
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_IceEn;
                   };
                 } TDBG_IF_IceEn;
    typedef union  TDBG_IF_Debug_On
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_Debug_On;
                   };
                 } TDBG_IF_Debug_On;
    typedef union  TDBG_IF_Hold_Dbg
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_Hold_Dbg;
                   };
                 } TDBG_IF_Hold_Dbg;
    typedef union  TDBG_IF_TEEDBG_Mrvl_password_valid
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_TEEDBG_Mrvl_password_valid;
                   };
                 } TDBG_IF_TEEDBG_Mrvl_password_valid;
    typedef union  TDBG_IF_TEEDBG_Mrvl_protected
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_TEEDBG_Mrvl_protected;
                   };
                 } TDBG_IF_TEEDBG_Mrvl_protected;
    typedef union  TDBG_IF_DIP_EnStatus
          { UNSG32 u32[1];
            struct {
            struct w32DBG_IF_DIP_EnStatus;
                   };
                 } TDBG_IF_DIP_EnStatus;

    ///////////////////////////////////////////////////////////
     SIGN32 DBG_IF_drvrd(SIE_DBG_IF *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DBG_IF_drvwr(SIE_DBG_IF *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DBG_IF_reset(SIE_DBG_IF *p);
     SIGN32 DBG_IF_cmp  (SIE_DBG_IF *p, SIE_DBG_IF *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DBG_IF_check(p,pie,pfx,hLOG) DBG_IF_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DBG_IF_print(p,    pfx,hLOG) DBG_IF_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: DBG_IF
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE TSOTP                                    (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (RW)
///               %unsigned 4  test                      0x5
///               %unsigned 1  rstb                      0x1
///               %unsigned 1  csb                       0x1
///               %unsigned 1  pgmb                      0x1
///               %unsigned 1  sclk                      0x0
///               %unsigned 1  load                      0x1
///               %unsigned 1  pdwn                      0x0
///               %unsigned 13 addr                      0x0
///                                    ###
///                                    * select the OTP cells to be programmed; only 0~2699 are valid numbers
///                                    ###
///               %unsigned 8  crcchk                    0x0
///               %%        1          # Stuffing bits...
///     @ 0x00004                      (RW)
///               %unsigned 1  progseqcode               0x0
///               %unsigned 1  progseqcodeclk            0x0
///               %%        30         # Stuffing bits...
///     @ 0x00008                      (R-)
///               %unsigned 32 data0                     0x1
///               %unsigned 32 data1                     0x1
///               %unsigned 2  data2                     0x1
///               %unsigned 1  security                  0x1
///               %unsigned 1  rddone                    0x0
///               %unsigned 1  dout                      0x1
///               %unsigned 1  match                     0x1
///                                    ###
///                                    * please refer to OTP Macro Document regarding to definition and programming timing about these bits.
///                                    ###
///               %%        26         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:      20B, bits:     103b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_TSOTP
#define h_TSOTP (){}

    #define     BA_TSOTP_test                                  0x0000
    #define     B16TSOTP_test                                  0x0000
    #define   LSb32TSOTP_test                                     0
    #define   LSb16TSOTP_test                                     0
    #define       bTSOTP_test                                  4
    #define   MSK32TSOTP_test                                     0x0000000F

    #define     BA_TSOTP_rstb                                  0x0000
    #define     B16TSOTP_rstb                                  0x0000
    #define   LSb32TSOTP_rstb                                     4
    #define   LSb16TSOTP_rstb                                     4
    #define       bTSOTP_rstb                                  1
    #define   MSK32TSOTP_rstb                                     0x00000010

    #define     BA_TSOTP_csb                                   0x0000
    #define     B16TSOTP_csb                                   0x0000
    #define   LSb32TSOTP_csb                                      5
    #define   LSb16TSOTP_csb                                      5
    #define       bTSOTP_csb                                   1
    #define   MSK32TSOTP_csb                                      0x00000020

    #define     BA_TSOTP_pgmb                                  0x0000
    #define     B16TSOTP_pgmb                                  0x0000
    #define   LSb32TSOTP_pgmb                                     6
    #define   LSb16TSOTP_pgmb                                     6
    #define       bTSOTP_pgmb                                  1
    #define   MSK32TSOTP_pgmb                                     0x00000040

    #define     BA_TSOTP_sclk                                  0x0000
    #define     B16TSOTP_sclk                                  0x0000
    #define   LSb32TSOTP_sclk                                     7
    #define   LSb16TSOTP_sclk                                     7
    #define       bTSOTP_sclk                                  1
    #define   MSK32TSOTP_sclk                                     0x00000080

    #define     BA_TSOTP_load                                  0x0001
    #define     B16TSOTP_load                                  0x0000
    #define   LSb32TSOTP_load                                     8
    #define   LSb16TSOTP_load                                     8
    #define       bTSOTP_load                                  1
    #define   MSK32TSOTP_load                                     0x00000100

    #define     BA_TSOTP_pdwn                                  0x0001
    #define     B16TSOTP_pdwn                                  0x0000
    #define   LSb32TSOTP_pdwn                                     9
    #define   LSb16TSOTP_pdwn                                     9
    #define       bTSOTP_pdwn                                  1
    #define   MSK32TSOTP_pdwn                                     0x00000200

    #define     BA_TSOTP_addr                                  0x0001
    #define     B16TSOTP_addr                                  0x0000
    #define   LSb32TSOTP_addr                                     10
    #define   LSb16TSOTP_addr                                     10
    #define       bTSOTP_addr                                  13
    #define   MSK32TSOTP_addr                                     0x007FFC00

    #define     BA_TSOTP_crcchk                                0x0002
    #define     B16TSOTP_crcchk                                0x0002
    #define   LSb32TSOTP_crcchk                                   23
    #define   LSb16TSOTP_crcchk                                   7
    #define       bTSOTP_crcchk                                8
    #define   MSK32TSOTP_crcchk                                   0x7F800000
    ///////////////////////////////////////////////////////////

    #define     BA_TSOTP_progseqcode                           0x0004
    #define     B16TSOTP_progseqcode                           0x0004
    #define   LSb32TSOTP_progseqcode                              0
    #define   LSb16TSOTP_progseqcode                              0
    #define       bTSOTP_progseqcode                           1
    #define   MSK32TSOTP_progseqcode                              0x00000001

    #define     BA_TSOTP_progseqcodeclk                        0x0004
    #define     B16TSOTP_progseqcodeclk                        0x0004
    #define   LSb32TSOTP_progseqcodeclk                           1
    #define   LSb16TSOTP_progseqcodeclk                           1
    #define       bTSOTP_progseqcodeclk                        1
    #define   MSK32TSOTP_progseqcodeclk                           0x00000002
    ///////////////////////////////////////////////////////////

    #define     BA_TSOTP_data0                                 0x0008
    #define     B16TSOTP_data0                                 0x0008
    #define   LSb32TSOTP_data0                                    0
    #define   LSb16TSOTP_data0                                    0
    #define       bTSOTP_data0                                 32
    #define   MSK32TSOTP_data0                                    0xFFFFFFFF

    #define     BA_TSOTP_data1                                 0x000C
    #define     B16TSOTP_data1                                 0x000C
    #define   LSb32TSOTP_data1                                    0
    #define   LSb16TSOTP_data1                                    0
    #define       bTSOTP_data1                                 32
    #define   MSK32TSOTP_data1                                    0xFFFFFFFF

    #define     BA_TSOTP_data2                                 0x0010
    #define     B16TSOTP_data2                                 0x0010
    #define   LSb32TSOTP_data2                                    0
    #define   LSb16TSOTP_data2                                    0
    #define       bTSOTP_data2                                 2
    #define   MSK32TSOTP_data2                                    0x00000003

    #define     BA_TSOTP_security                              0x0010
    #define     B16TSOTP_security                              0x0010
    #define   LSb32TSOTP_security                                 2
    #define   LSb16TSOTP_security                                 2
    #define       bTSOTP_security                              1
    #define   MSK32TSOTP_security                                 0x00000004

    #define     BA_TSOTP_rddone                                0x0010
    #define     B16TSOTP_rddone                                0x0010
    #define   LSb32TSOTP_rddone                                   3
    #define   LSb16TSOTP_rddone                                   3
    #define       bTSOTP_rddone                                1
    #define   MSK32TSOTP_rddone                                   0x00000008

    #define     BA_TSOTP_dout                                  0x0010
    #define     B16TSOTP_dout                                  0x0010
    #define   LSb32TSOTP_dout                                     4
    #define   LSb16TSOTP_dout                                     4
    #define       bTSOTP_dout                                  1
    #define   MSK32TSOTP_dout                                     0x00000010

    #define     BA_TSOTP_match                                 0x0010
    #define     B16TSOTP_match                                 0x0010
    #define   LSb32TSOTP_match                                    5
    #define   LSb16TSOTP_match                                    5
    #define       bTSOTP_match                                 1
    #define   MSK32TSOTP_match                                    0x00000020
    ///////////////////////////////////////////////////////////

    typedef struct SIE_TSOTP {
    ///////////////////////////////////////////////////////////
    #define   GET32TSOTP_test(r32)                             _BFGET_(r32, 3, 0)
    #define   SET32TSOTP_test(r32,v)                           _BFSET_(r32, 3, 0,v)
    #define   GET16TSOTP_test(r16)                             _BFGET_(r16, 3, 0)
    #define   SET16TSOTP_test(r16,v)                           _BFSET_(r16, 3, 0,v)

    #define   GET32TSOTP_rstb(r32)                             _BFGET_(r32, 4, 4)
    #define   SET32TSOTP_rstb(r32,v)                           _BFSET_(r32, 4, 4,v)
    #define   GET16TSOTP_rstb(r16)                             _BFGET_(r16, 4, 4)
    #define   SET16TSOTP_rstb(r16,v)                           _BFSET_(r16, 4, 4,v)

    #define   GET32TSOTP_csb(r32)                              _BFGET_(r32, 5, 5)
    #define   SET32TSOTP_csb(r32,v)                            _BFSET_(r32, 5, 5,v)
    #define   GET16TSOTP_csb(r16)                              _BFGET_(r16, 5, 5)
    #define   SET16TSOTP_csb(r16,v)                            _BFSET_(r16, 5, 5,v)

    #define   GET32TSOTP_pgmb(r32)                             _BFGET_(r32, 6, 6)
    #define   SET32TSOTP_pgmb(r32,v)                           _BFSET_(r32, 6, 6,v)
    #define   GET16TSOTP_pgmb(r16)                             _BFGET_(r16, 6, 6)
    #define   SET16TSOTP_pgmb(r16,v)                           _BFSET_(r16, 6, 6,v)

    #define   GET32TSOTP_sclk(r32)                             _BFGET_(r32, 7, 7)
    #define   SET32TSOTP_sclk(r32,v)                           _BFSET_(r32, 7, 7,v)
    #define   GET16TSOTP_sclk(r16)                             _BFGET_(r16, 7, 7)
    #define   SET16TSOTP_sclk(r16,v)                           _BFSET_(r16, 7, 7,v)

    #define   GET32TSOTP_load(r32)                             _BFGET_(r32, 8, 8)
    #define   SET32TSOTP_load(r32,v)                           _BFSET_(r32, 8, 8,v)
    #define   GET16TSOTP_load(r16)                             _BFGET_(r16, 8, 8)
    #define   SET16TSOTP_load(r16,v)                           _BFSET_(r16, 8, 8,v)

    #define   GET32TSOTP_pdwn(r32)                             _BFGET_(r32, 9, 9)
    #define   SET32TSOTP_pdwn(r32,v)                           _BFSET_(r32, 9, 9,v)
    #define   GET16TSOTP_pdwn(r16)                             _BFGET_(r16, 9, 9)
    #define   SET16TSOTP_pdwn(r16,v)                           _BFSET_(r16, 9, 9,v)

    #define   GET32TSOTP_addr(r32)                             _BFGET_(r32,22,10)
    #define   SET32TSOTP_addr(r32,v)                           _BFSET_(r32,22,10,v)

    #define   GET32TSOTP_crcchk(r32)                           _BFGET_(r32,30,23)
    #define   SET32TSOTP_crcchk(r32,v)                         _BFSET_(r32,30,23,v)
    #define   GET16TSOTP_crcchk(r16)                           _BFGET_(r16,14, 7)
    #define   SET16TSOTP_crcchk(r16,v)                         _BFSET_(r16,14, 7,v)

            UNSG32 u_test                                      :  4;
            UNSG32 u_rstb                                      :  1;
            UNSG32 u_csb                                       :  1;
            UNSG32 u_pgmb                                      :  1;
            UNSG32 u_sclk                                      :  1;
            UNSG32 u_load                                      :  1;
            UNSG32 u_pdwn                                      :  1;
            UNSG32 u_addr                                      : 13;
            UNSG32 u_crcchk                                    :  8;
            UNSG32 RSVDx0_b31                                  :  1;
    ///////////////////////////////////////////////////////////
    #define   GET32TSOTP_progseqcode(r32)                      _BFGET_(r32, 0, 0)
    #define   SET32TSOTP_progseqcode(r32,v)                    _BFSET_(r32, 0, 0,v)
    #define   GET16TSOTP_progseqcode(r16)                      _BFGET_(r16, 0, 0)
    #define   SET16TSOTP_progseqcode(r16,v)                    _BFSET_(r16, 0, 0,v)

    #define   GET32TSOTP_progseqcodeclk(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32TSOTP_progseqcodeclk(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16TSOTP_progseqcodeclk(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16TSOTP_progseqcodeclk(r16,v)                 _BFSET_(r16, 1, 1,v)

            UNSG32 u_progseqcode                               :  1;
            UNSG32 u_progseqcodeclk                            :  1;
            UNSG32 RSVDx4_b2                                   : 30;
    ///////////////////////////////////////////////////////////
    #define   GET32TSOTP_data0(r32)                            _BFGET_(r32,31, 0)
    #define   SET32TSOTP_data0(r32,v)                          _BFSET_(r32,31, 0,v)

            UNSG32 u_data0                                     : 32;
    ///////////////////////////////////////////////////////////

    #define   GET32TSOTP_data1(r32)                            _BFGET_(r32,31, 0)
    #define   SET32TSOTP_data1(r32,v)                          _BFSET_(r32,31, 0,v)

            UNSG32 u_data1                                     : 32;
    ///////////////////////////////////////////////////////////

    #define   GET32TSOTP_data2(r32)                            _BFGET_(r32, 1, 0)
    #define   SET32TSOTP_data2(r32,v)                          _BFSET_(r32, 1, 0,v)
    #define   GET16TSOTP_data2(r16)                            _BFGET_(r16, 1, 0)
    #define   SET16TSOTP_data2(r16,v)                          _BFSET_(r16, 1, 0,v)

    #define   GET32TSOTP_security(r32)                         _BFGET_(r32, 2, 2)
    #define   SET32TSOTP_security(r32,v)                       _BFSET_(r32, 2, 2,v)
    #define   GET16TSOTP_security(r16)                         _BFGET_(r16, 2, 2)
    #define   SET16TSOTP_security(r16,v)                       _BFSET_(r16, 2, 2,v)

    #define   GET32TSOTP_rddone(r32)                           _BFGET_(r32, 3, 3)
    #define   SET32TSOTP_rddone(r32,v)                         _BFSET_(r32, 3, 3,v)
    #define   GET16TSOTP_rddone(r16)                           _BFGET_(r16, 3, 3)
    #define   SET16TSOTP_rddone(r16,v)                         _BFSET_(r16, 3, 3,v)

    #define   GET32TSOTP_dout(r32)                             _BFGET_(r32, 4, 4)
    #define   SET32TSOTP_dout(r32,v)                           _BFSET_(r32, 4, 4,v)
    #define   GET16TSOTP_dout(r16)                             _BFGET_(r16, 4, 4)
    #define   SET16TSOTP_dout(r16,v)                           _BFSET_(r16, 4, 4,v)

    #define   GET32TSOTP_match(r32)                            _BFGET_(r32, 5, 5)
    #define   SET32TSOTP_match(r32,v)                          _BFSET_(r32, 5, 5,v)
    #define   GET16TSOTP_match(r16)                            _BFGET_(r16, 5, 5)
    #define   SET16TSOTP_match(r16,v)                          _BFSET_(r16, 5, 5,v)

            UNSG32 u_data2                                     :  2;
            UNSG32 u_security                                  :  1;
            UNSG32 u_rddone                                    :  1;
            UNSG32 u_dout                                      :  1;
            UNSG32 u_match                                     :  1;
            UNSG32 RSVDx10_b6                                  : 26;
    ///////////////////////////////////////////////////////////
    } SIE_TSOTP;

    ///////////////////////////////////////////////////////////
     SIGN32 TSOTP_drvrd(SIE_TSOTP *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TSOTP_drvwr(SIE_TSOTP *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TSOTP_reset(SIE_TSOTP *p);
     SIGN32 TSOTP_cmp  (SIE_TSOTP *p, SIE_TSOTP *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TSOTP_check(p,pie,pfx,hLOG) TSOTP_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TSOTP_print(p,    pfx,hLOG) TSOTP_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: TSOTP
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE TSSSWrapper                              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (W-)
///     #         # Stuffing bytes...
///               %% 6144
///     @ 0x00300 IntStatus            (WOC-)
///               ###
///               * Writing ones to any of the bits will clear those bits. ACPU can access it
///               ###
///               %unsigned 32 Reseved0                  0x0
///                                    ###
///                                    * Reserved for future
///                                    ###
///     @ 0x00304 IntMask              (P)
///               %unsigned 32 IntMask                   0x0
///                                    ###
///                                    * 0: do not mask out interrupt bit
///                                    * 1: mask out interrupt bit, register can be updated by hardware, but interrupt will not be generated.
///                                    * ACPU can access it
///                                    ###
///     @ 0x00308 IntEnable            (P)
///               %unsigned 32 IntEnable                 0xFFFF
///                                    ###
///                                    * 0: disable interrupt bit, register will not be updated by hardware
///                                    * 1: enable interrupt bit
///                                    ###
///     @ 0x0030C                      (W-)
///     #         # Stuffing bytes...
///               %% 5024
///     @ 0x00580 TransError           (R-)
///               ###
///               * Invalid transfer information. When there is a invalid transfer from ACPU/SCPU, it will be logged.
///               ###
///               %unsigned 32 TransError                0x0
///     @ 0x00584                      (W-)
///     #         # Stuffing bytes...
///               %% 224
///     @ 0x005A0 Spare_reg            (RW)
///               %unsigned 32 Reg0                      0x0
///     # 0x005A4 Spare_reg1           
///               %unsigned 32 Reg1                      0x0
///     # 0x005A8 Spare_reg2           
///               %unsigned 32 Reg2                      0x0
///     # 0x005AC Spare_reg3           
///               %unsigned 32 Reg3                      0x0
///     # 0x005B0 Spare_reg4           
///               %unsigned 32 Reg4                      0x0
///     # 0x005B4 Spare_reg5           
///               %unsigned 32 Reg5                      0x0
///     # 0x005B8 Spare_reg6           
///               %unsigned 32 Reg6                      0x0
///     # 0x005BC Spare_reg7           
///               %unsigned 1  Gen_RNG_Reset             0x1
///                                    ###
///                                    * 0: reset the generic RNG
///                                    * Sticky reset, must write 1 to release the reset.
///                                    ###
///               %unsigned 31 Spare                     0x0
///                                    ###
///                                    * Reserved for future
///                                    ###
///     @ 0x005C0 TEE_user_password    (W-)
///     # 0x005C0 TEE_password         
///               $OneReg              TEE_password      REG       [4]
///                                    ###
///                                    * Write only for user
///                                    ###
///     @ 0x005D0 TEE_user_password_valid (P)
///               %unsigned 1  TEE_password_valid        0x0
///               %%        31         # Stuffing bits...
///     @ 0x005D4 TEE_user_password_sel (P)
///               %unsigned 1  TEE_user_password_sel     0x0
///                                    ###
///                                    * 0:select 0; 1:select 1
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x005D8 FPGA_Control         (P)
///               %unsigned 1  KP_reset                  0x1
///                                    ###
///                                    * Reset Kilopass OTP
///                                    ###
///               %unsigned 1  IFCPM3_reset              0x0
///                                    ###
///                                    * Reset IFCP M3
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x005DC IFCP_BIV             (RW)
///     # 0x005DC BIV                  
///               $OneReg              BIV               REG       [4]
///                                    ###
///                                    * 128-bit box image version (BIV) register.
///                                    ###
///     @ 0x005EC IFCP_BIV_Lock        (RW)
///               %unsigned 1  BIV_Lock                  0x0
///                                    ###
///                                    * Lock the 128-bit box image version (BIV) register. Once been programmed to 1 it will stay at 1 until next reset.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x005F0 SCPU_Key_Sel         (P)
///               %unsigned 3  SCPU_Key_Sel              0x0
///                                    ###
///                                    * Sel the different keys for BCM RKEK:
///                                    * 0: select BCM security RKEK
///                                    * 1: select CCCK
///                                    * 2: select SSUK
///                                    * 3: select CSGK1
///                                    * 4: select NSKSCPUKey
///                                    ###
///               %%        29         # Stuffing bits...
///     @ 0x005F4 Warm_Reset_Indicator (RW)
///               %unsigned 32 Warm_Reset_Indicator      0x0
///                                    ###
///                                    * Preserve value during warm reset
///                                    ###
///     @ 0x005F8 Warm_Reset_Indicator_Latch (RW)
///               %unsigned 1  Latch                     0x0
///                                    ###
///                                    * Write 1 to let HW latch warm-reset-indicator
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x005FC Warm_Reset_Indicator_Load (RW)
///               %unsigned 1  Load                      0x0
///                                    ###
///                                    * Write 1 to let HW load unresetable warm-reset-indicator to register
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00600 DBG_IF               (P)
///     # 0x00600 DBG_IF               
///               $DBG_IF              DBG_IF            REG          
///                                    ###
///                                    * DBG_IF register
///                                    ###
///     @ 0x006CC                      (W-)
///     #         # Stuffing bytes...
///               %% 416
///     @ 0x00700 BCM_Security         (P)
///     # 0x00700 BCM_Security         
///               $BCM_Security        BCM_Security      REG          
///                                    ###
///                                    * DBG_IF register
///                                    ###
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:    2676B, bits:    5551b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_TSSSWrapper
#define h_TSSSWrapper (){}

    #define     RA_TSSSWrapper_IntStatus                       0x0300

    #define     BA_TSSSWrapper_IntStatus_Reseved0              0x0300
    #define     B16TSSSWrapper_IntStatus_Reseved0              0x0300
    #define   LSb32TSSSWrapper_IntStatus_Reseved0                 0
    #define   LSb16TSSSWrapper_IntStatus_Reseved0                 0
    #define       bTSSSWrapper_IntStatus_Reseved0              32
    #define   MSK32TSSSWrapper_IntStatus_Reseved0                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_IntMask                         0x0304

    #define     BA_TSSSWrapper_IntMask_IntMask                 0x0304
    #define     B16TSSSWrapper_IntMask_IntMask                 0x0304
    #define   LSb32TSSSWrapper_IntMask_IntMask                    0
    #define   LSb16TSSSWrapper_IntMask_IntMask                    0
    #define       bTSSSWrapper_IntMask_IntMask                 32
    #define   MSK32TSSSWrapper_IntMask_IntMask                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_IntEnable                       0x0308

    #define     BA_TSSSWrapper_IntEnable_IntEnable             0x0308
    #define     B16TSSSWrapper_IntEnable_IntEnable             0x0308
    #define   LSb32TSSSWrapper_IntEnable_IntEnable                0
    #define   LSb16TSSSWrapper_IntEnable_IntEnable                0
    #define       bTSSSWrapper_IntEnable_IntEnable             32
    #define   MSK32TSSSWrapper_IntEnable_IntEnable                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_TransError                      0x0580

    #define     BA_TSSSWrapper_TransError_TransError           0x0580
    #define     B16TSSSWrapper_TransError_TransError           0x0580
    #define   LSb32TSSSWrapper_TransError_TransError              0
    #define   LSb16TSSSWrapper_TransError_TransError              0
    #define       bTSSSWrapper_TransError_TransError           32
    #define   MSK32TSSSWrapper_TransError_TransError              0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_Spare_reg                       0x05A0

    #define     BA_TSSSWrapper_Spare_reg_Reg0                  0x05A0
    #define     B16TSSSWrapper_Spare_reg_Reg0                  0x05A0
    #define   LSb32TSSSWrapper_Spare_reg_Reg0                     0
    #define   LSb16TSSSWrapper_Spare_reg_Reg0                     0
    #define       bTSSSWrapper_Spare_reg_Reg0                  32
    #define   MSK32TSSSWrapper_Spare_reg_Reg0                     0xFFFFFFFF

    #define     RA_TSSSWrapper_Spare_reg1                      0x05A4

    #define     BA_TSSSWrapper_Spare_reg_Reg1                  0x05A4
    #define     B16TSSSWrapper_Spare_reg_Reg1                  0x05A4
    #define   LSb32TSSSWrapper_Spare_reg_Reg1                     0
    #define   LSb16TSSSWrapper_Spare_reg_Reg1                     0
    #define       bTSSSWrapper_Spare_reg_Reg1                  32
    #define   MSK32TSSSWrapper_Spare_reg_Reg1                     0xFFFFFFFF

    #define     RA_TSSSWrapper_Spare_reg2                      0x05A8

    #define     BA_TSSSWrapper_Spare_reg_Reg2                  0x05A8
    #define     B16TSSSWrapper_Spare_reg_Reg2                  0x05A8
    #define   LSb32TSSSWrapper_Spare_reg_Reg2                     0
    #define   LSb16TSSSWrapper_Spare_reg_Reg2                     0
    #define       bTSSSWrapper_Spare_reg_Reg2                  32
    #define   MSK32TSSSWrapper_Spare_reg_Reg2                     0xFFFFFFFF

    #define     RA_TSSSWrapper_Spare_reg3                      0x05AC

    #define     BA_TSSSWrapper_Spare_reg_Reg3                  0x05AC
    #define     B16TSSSWrapper_Spare_reg_Reg3                  0x05AC
    #define   LSb32TSSSWrapper_Spare_reg_Reg3                     0
    #define   LSb16TSSSWrapper_Spare_reg_Reg3                     0
    #define       bTSSSWrapper_Spare_reg_Reg3                  32
    #define   MSK32TSSSWrapper_Spare_reg_Reg3                     0xFFFFFFFF

    #define     RA_TSSSWrapper_Spare_reg4                      0x05B0

    #define     BA_TSSSWrapper_Spare_reg_Reg4                  0x05B0
    #define     B16TSSSWrapper_Spare_reg_Reg4                  0x05B0
    #define   LSb32TSSSWrapper_Spare_reg_Reg4                     0
    #define   LSb16TSSSWrapper_Spare_reg_Reg4                     0
    #define       bTSSSWrapper_Spare_reg_Reg4                  32
    #define   MSK32TSSSWrapper_Spare_reg_Reg4                     0xFFFFFFFF

    #define     RA_TSSSWrapper_Spare_reg5                      0x05B4

    #define     BA_TSSSWrapper_Spare_reg_Reg5                  0x05B4
    #define     B16TSSSWrapper_Spare_reg_Reg5                  0x05B4
    #define   LSb32TSSSWrapper_Spare_reg_Reg5                     0
    #define   LSb16TSSSWrapper_Spare_reg_Reg5                     0
    #define       bTSSSWrapper_Spare_reg_Reg5                  32
    #define   MSK32TSSSWrapper_Spare_reg_Reg5                     0xFFFFFFFF

    #define     RA_TSSSWrapper_Spare_reg6                      0x05B8

    #define     BA_TSSSWrapper_Spare_reg_Reg6                  0x05B8
    #define     B16TSSSWrapper_Spare_reg_Reg6                  0x05B8
    #define   LSb32TSSSWrapper_Spare_reg_Reg6                     0
    #define   LSb16TSSSWrapper_Spare_reg_Reg6                     0
    #define       bTSSSWrapper_Spare_reg_Reg6                  32
    #define   MSK32TSSSWrapper_Spare_reg_Reg6                     0xFFFFFFFF

    #define     RA_TSSSWrapper_Spare_reg7                      0x05BC

    #define     BA_TSSSWrapper_Spare_reg_Gen_RNG_Reset         0x05BC
    #define     B16TSSSWrapper_Spare_reg_Gen_RNG_Reset         0x05BC
    #define   LSb32TSSSWrapper_Spare_reg_Gen_RNG_Reset            0
    #define   LSb16TSSSWrapper_Spare_reg_Gen_RNG_Reset            0
    #define       bTSSSWrapper_Spare_reg_Gen_RNG_Reset         1
    #define   MSK32TSSSWrapper_Spare_reg_Gen_RNG_Reset            0x00000001

    #define     BA_TSSSWrapper_Spare_reg_Spare                 0x05BC
    #define     B16TSSSWrapper_Spare_reg_Spare                 0x05BC
    #define   LSb32TSSSWrapper_Spare_reg_Spare                    1
    #define   LSb16TSSSWrapper_Spare_reg_Spare                    1
    #define       bTSSSWrapper_Spare_reg_Spare                 31
    #define   MSK32TSSSWrapper_Spare_reg_Spare                    0xFFFFFFFE
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_TEE_user_password               0x05C0
    #define     RA_TSSSWrapper_TEE_password                    0x05C0
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_TEE_user_password_valid         0x05D0

    #define     BA_TSSSWrapper_TEE_user_password_valid_TEE_password_valid 0x05D0
    #define     B16TSSSWrapper_TEE_user_password_valid_TEE_password_valid 0x05D0
    #define   LSb32TSSSWrapper_TEE_user_password_valid_TEE_password_valid    0
    #define   LSb16TSSSWrapper_TEE_user_password_valid_TEE_password_valid    0
    #define       bTSSSWrapper_TEE_user_password_valid_TEE_password_valid 1
    #define   MSK32TSSSWrapper_TEE_user_password_valid_TEE_password_valid    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_TEE_user_password_sel           0x05D4

    #define     BA_TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel 0x05D4
    #define     B16TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel 0x05D4
    #define   LSb32TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel    0
    #define   LSb16TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel    0
    #define       bTSSSWrapper_TEE_user_password_sel_TEE_user_password_sel 1
    #define   MSK32TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_FPGA_Control                    0x05D8

    #define     BA_TSSSWrapper_FPGA_Control_KP_reset           0x05D8
    #define     B16TSSSWrapper_FPGA_Control_KP_reset           0x05D8
    #define   LSb32TSSSWrapper_FPGA_Control_KP_reset              0
    #define   LSb16TSSSWrapper_FPGA_Control_KP_reset              0
    #define       bTSSSWrapper_FPGA_Control_KP_reset           1
    #define   MSK32TSSSWrapper_FPGA_Control_KP_reset              0x00000001

    #define     BA_TSSSWrapper_FPGA_Control_IFCPM3_reset       0x05D8
    #define     B16TSSSWrapper_FPGA_Control_IFCPM3_reset       0x05D8
    #define   LSb32TSSSWrapper_FPGA_Control_IFCPM3_reset          1
    #define   LSb16TSSSWrapper_FPGA_Control_IFCPM3_reset          1
    #define       bTSSSWrapper_FPGA_Control_IFCPM3_reset       1
    #define   MSK32TSSSWrapper_FPGA_Control_IFCPM3_reset          0x00000002
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_IFCP_BIV                        0x05DC
    #define     RA_TSSSWrapper_BIV                             0x05DC
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_IFCP_BIV_Lock                   0x05EC

    #define     BA_TSSSWrapper_IFCP_BIV_Lock_BIV_Lock          0x05EC
    #define     B16TSSSWrapper_IFCP_BIV_Lock_BIV_Lock          0x05EC
    #define   LSb32TSSSWrapper_IFCP_BIV_Lock_BIV_Lock             0
    #define   LSb16TSSSWrapper_IFCP_BIV_Lock_BIV_Lock             0
    #define       bTSSSWrapper_IFCP_BIV_Lock_BIV_Lock          1
    #define   MSK32TSSSWrapper_IFCP_BIV_Lock_BIV_Lock             0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_SCPU_Key_Sel                    0x05F0

    #define     BA_TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel       0x05F0
    #define     B16TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel       0x05F0
    #define   LSb32TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel          0
    #define   LSb16TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel          0
    #define       bTSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel       3
    #define   MSK32TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel          0x00000007
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_Warm_Reset_Indicator            0x05F4

    #define     BA_TSSSWrapper_Warm_Reset_Indicator_Warm_Reset_Indicator 0x05F4
    #define     B16TSSSWrapper_Warm_Reset_Indicator_Warm_Reset_Indicator 0x05F4
    #define   LSb32TSSSWrapper_Warm_Reset_Indicator_Warm_Reset_Indicator    0
    #define   LSb16TSSSWrapper_Warm_Reset_Indicator_Warm_Reset_Indicator    0
    #define       bTSSSWrapper_Warm_Reset_Indicator_Warm_Reset_Indicator 32
    #define   MSK32TSSSWrapper_Warm_Reset_Indicator_Warm_Reset_Indicator    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_Warm_Reset_Indicator_Latch      0x05F8

    #define     BA_TSSSWrapper_Warm_Reset_Indicator_Latch_Latch 0x05F8
    #define     B16TSSSWrapper_Warm_Reset_Indicator_Latch_Latch 0x05F8
    #define   LSb32TSSSWrapper_Warm_Reset_Indicator_Latch_Latch    0
    #define   LSb16TSSSWrapper_Warm_Reset_Indicator_Latch_Latch    0
    #define       bTSSSWrapper_Warm_Reset_Indicator_Latch_Latch 1
    #define   MSK32TSSSWrapper_Warm_Reset_Indicator_Latch_Latch    0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_Warm_Reset_Indicator_Load       0x05FC

    #define     BA_TSSSWrapper_Warm_Reset_Indicator_Load_Load  0x05FC
    #define     B16TSSSWrapper_Warm_Reset_Indicator_Load_Load  0x05FC
    #define   LSb32TSSSWrapper_Warm_Reset_Indicator_Load_Load     0
    #define   LSb16TSSSWrapper_Warm_Reset_Indicator_Load_Load     0
    #define       bTSSSWrapper_Warm_Reset_Indicator_Load_Load  1
    #define   MSK32TSSSWrapper_Warm_Reset_Indicator_Load_Load     0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_DBG_IF                          0x0600
    ///////////////////////////////////////////////////////////
    #define     RA_TSSSWrapper_BCM_Security                    0x0700
    ///////////////////////////////////////////////////////////

    typedef struct SIE_TSSSWrapper {
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx0                                      [768];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_IntStatus_Reseved0(r32)         _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_IntStatus_Reseved0(r32,v)       _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_IntStatus                       {\
            UNSG32 uIntStatus_Reseved0                         : 32;\
          }
    union { UNSG32 u32TSSSWrapper_IntStatus;
            struct w32TSSSWrapper_IntStatus;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_IntMask_IntMask(r32)            _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_IntMask_IntMask(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_IntMask                         {\
            UNSG32 uIntMask_IntMask                            : 32;\
          }
    union { UNSG32 u32TSSSWrapper_IntMask;
            struct w32TSSSWrapper_IntMask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_IntEnable_IntEnable(r32)        _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_IntEnable_IntEnable(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_IntEnable                       {\
            UNSG32 uIntEnable_IntEnable                        : 32;\
          }
    union { UNSG32 u32TSSSWrapper_IntEnable;
            struct w32TSSSWrapper_IntEnable;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx30C                                    [628];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_TransError_TransError(r32)      _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_TransError_TransError(r32,v)    _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_TransError                      {\
            UNSG32 uTransError_TransError                      : 32;\
          }
    union { UNSG32 u32TSSSWrapper_TransError;
            struct w32TSSSWrapper_TransError;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx584                                    [28];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_Spare_reg_Reg0(r32)             _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_Spare_reg_Reg0(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_Spare_reg                       {\
            UNSG32 uSpare_reg_Reg0                             : 32;\
          }
    union { UNSG32 u32TSSSWrapper_Spare_reg;
            struct w32TSSSWrapper_Spare_reg;
          };
    #define   GET32TSSSWrapper_Spare_reg_Reg1(r32)             _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_Spare_reg_Reg1(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_Spare_reg1                      {\
            UNSG32 uSpare_reg_Reg1                             : 32;\
          }
    union { UNSG32 u32TSSSWrapper_Spare_reg1;
            struct w32TSSSWrapper_Spare_reg1;
          };
    #define   GET32TSSSWrapper_Spare_reg_Reg2(r32)             _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_Spare_reg_Reg2(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_Spare_reg2                      {\
            UNSG32 uSpare_reg_Reg2                             : 32;\
          }
    union { UNSG32 u32TSSSWrapper_Spare_reg2;
            struct w32TSSSWrapper_Spare_reg2;
          };
    #define   GET32TSSSWrapper_Spare_reg_Reg3(r32)             _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_Spare_reg_Reg3(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_Spare_reg3                      {\
            UNSG32 uSpare_reg_Reg3                             : 32;\
          }
    union { UNSG32 u32TSSSWrapper_Spare_reg3;
            struct w32TSSSWrapper_Spare_reg3;
          };
    #define   GET32TSSSWrapper_Spare_reg_Reg4(r32)             _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_Spare_reg_Reg4(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_Spare_reg4                      {\
            UNSG32 uSpare_reg_Reg4                             : 32;\
          }
    union { UNSG32 u32TSSSWrapper_Spare_reg4;
            struct w32TSSSWrapper_Spare_reg4;
          };
    #define   GET32TSSSWrapper_Spare_reg_Reg5(r32)             _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_Spare_reg_Reg5(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_Spare_reg5                      {\
            UNSG32 uSpare_reg_Reg5                             : 32;\
          }
    union { UNSG32 u32TSSSWrapper_Spare_reg5;
            struct w32TSSSWrapper_Spare_reg5;
          };
    #define   GET32TSSSWrapper_Spare_reg_Reg6(r32)             _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_Spare_reg_Reg6(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_Spare_reg6                      {\
            UNSG32 uSpare_reg_Reg6                             : 32;\
          }
    union { UNSG32 u32TSSSWrapper_Spare_reg6;
            struct w32TSSSWrapper_Spare_reg6;
          };
    #define   GET32TSSSWrapper_Spare_reg_Gen_RNG_Reset(r32)    _BFGET_(r32, 0, 0)
    #define   SET32TSSSWrapper_Spare_reg_Gen_RNG_Reset(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   GET16TSSSWrapper_Spare_reg_Gen_RNG_Reset(r16)    _BFGET_(r16, 0, 0)
    #define   SET16TSSSWrapper_Spare_reg_Gen_RNG_Reset(r16,v)  _BFSET_(r16, 0, 0,v)

    #define   GET32TSSSWrapper_Spare_reg_Spare(r32)            _BFGET_(r32,31, 1)
    #define   SET32TSSSWrapper_Spare_reg_Spare(r32,v)          _BFSET_(r32,31, 1,v)

    #define     w32TSSSWrapper_Spare_reg7                      {\
            UNSG32 uSpare_reg_Gen_RNG_Reset                    :  1;\
            UNSG32 uSpare_reg_Spare                            : 31;\
          }
    union { UNSG32 u32TSSSWrapper_Spare_reg7;
            struct w32TSSSWrapper_Spare_reg7;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_TEE_password[4];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_TEE_user_password_valid_TEE_password_valid(r32) _BFGET_(r32, 0, 0)
    #define   SET32TSSSWrapper_TEE_user_password_valid_TEE_password_valid(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16TSSSWrapper_TEE_user_password_valid_TEE_password_valid(r16) _BFGET_(r16, 0, 0)
    #define   SET16TSSSWrapper_TEE_user_password_valid_TEE_password_valid(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32TSSSWrapper_TEE_user_password_valid         {\
            UNSG32 uTEE_user_password_valid_TEE_password_valid :  1;\
            UNSG32 RSVDx5D0_b1                                 : 31;\
          }
    union { UNSG32 u32TSSSWrapper_TEE_user_password_valid;
            struct w32TSSSWrapper_TEE_user_password_valid;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel(r32) _BFGET_(r32, 0, 0)
    #define   SET32TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel(r16) _BFGET_(r16, 0, 0)
    #define   SET16TSSSWrapper_TEE_user_password_sel_TEE_user_password_sel(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32TSSSWrapper_TEE_user_password_sel           {\
            UNSG32 uTEE_user_password_sel_TEE_user_password_sel :  1;\
            UNSG32 RSVDx5D4_b1                                 : 31;\
          }
    union { UNSG32 u32TSSSWrapper_TEE_user_password_sel;
            struct w32TSSSWrapper_TEE_user_password_sel;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_FPGA_Control_KP_reset(r32)      _BFGET_(r32, 0, 0)
    #define   SET32TSSSWrapper_FPGA_Control_KP_reset(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   GET16TSSSWrapper_FPGA_Control_KP_reset(r16)      _BFGET_(r16, 0, 0)
    #define   SET16TSSSWrapper_FPGA_Control_KP_reset(r16,v)    _BFSET_(r16, 0, 0,v)

    #define   GET32TSSSWrapper_FPGA_Control_IFCPM3_reset(r32)  _BFGET_(r32, 1, 1)
    #define   SET32TSSSWrapper_FPGA_Control_IFCPM3_reset(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16TSSSWrapper_FPGA_Control_IFCPM3_reset(r16)  _BFGET_(r16, 1, 1)
    #define   SET16TSSSWrapper_FPGA_Control_IFCPM3_reset(r16,v) _BFSET_(r16, 1, 1,v)

    #define     w32TSSSWrapper_FPGA_Control                    {\
            UNSG32 uFPGA_Control_KP_reset                      :  1;\
            UNSG32 uFPGA_Control_IFCPM3_reset                  :  1;\
            UNSG32 RSVDx5D8_b2                                 : 30;\
          }
    union { UNSG32 u32TSSSWrapper_FPGA_Control;
            struct w32TSSSWrapper_FPGA_Control;
          };
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_BIV[4];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_IFCP_BIV_Lock_BIV_Lock(r32)     _BFGET_(r32, 0, 0)
    #define   SET32TSSSWrapper_IFCP_BIV_Lock_BIV_Lock(r32,v)   _BFSET_(r32, 0, 0,v)
    #define   GET16TSSSWrapper_IFCP_BIV_Lock_BIV_Lock(r16)     _BFGET_(r16, 0, 0)
    #define   SET16TSSSWrapper_IFCP_BIV_Lock_BIV_Lock(r16,v)   _BFSET_(r16, 0, 0,v)

    #define     w32TSSSWrapper_IFCP_BIV_Lock                   {\
            UNSG32 uIFCP_BIV_Lock_BIV_Lock                     :  1;\
            UNSG32 RSVDx5EC_b1                                 : 31;\
          }
    union { UNSG32 u32TSSSWrapper_IFCP_BIV_Lock;
            struct w32TSSSWrapper_IFCP_BIV_Lock;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel(r32)  _BFGET_(r32, 2, 0)
    #define   SET32TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel(r32,v) _BFSET_(r32, 2, 0,v)
    #define   GET16TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel(r16)  _BFGET_(r16, 2, 0)
    #define   SET16TSSSWrapper_SCPU_Key_Sel_SCPU_Key_Sel(r16,v) _BFSET_(r16, 2, 0,v)

    #define     w32TSSSWrapper_SCPU_Key_Sel                    {\
            UNSG32 uSCPU_Key_Sel_SCPU_Key_Sel                  :  3;\
            UNSG32 RSVDx5F0_b3                                 : 29;\
          }
    union { UNSG32 u32TSSSWrapper_SCPU_Key_Sel;
            struct w32TSSSWrapper_SCPU_Key_Sel;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_Warm_Reset_Indicator_Warm_Reset_Indicator(r32) _BFGET_(r32,31, 0)
    #define   SET32TSSSWrapper_Warm_Reset_Indicator_Warm_Reset_Indicator(r32,v) _BFSET_(r32,31, 0,v)

    #define     w32TSSSWrapper_Warm_Reset_Indicator            {\
            UNSG32 uWarm_Reset_Indicator_Warm_Reset_Indicator  : 32;\
          }
    union { UNSG32 u32TSSSWrapper_Warm_Reset_Indicator;
            struct w32TSSSWrapper_Warm_Reset_Indicator;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_Warm_Reset_Indicator_Latch_Latch(r32) _BFGET_(r32, 0, 0)
    #define   SET32TSSSWrapper_Warm_Reset_Indicator_Latch_Latch(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16TSSSWrapper_Warm_Reset_Indicator_Latch_Latch(r16) _BFGET_(r16, 0, 0)
    #define   SET16TSSSWrapper_Warm_Reset_Indicator_Latch_Latch(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32TSSSWrapper_Warm_Reset_Indicator_Latch      {\
            UNSG32 uWarm_Reset_Indicator_Latch_Latch           :  1;\
            UNSG32 RSVDx5F8_b1                                 : 31;\
          }
    union { UNSG32 u32TSSSWrapper_Warm_Reset_Indicator_Latch;
            struct w32TSSSWrapper_Warm_Reset_Indicator_Latch;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSSSWrapper_Warm_Reset_Indicator_Load_Load(r32) _BFGET_(r32, 0, 0)
    #define   SET32TSSSWrapper_Warm_Reset_Indicator_Load_Load(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16TSSSWrapper_Warm_Reset_Indicator_Load_Load(r16) _BFGET_(r16, 0, 0)
    #define   SET16TSSSWrapper_Warm_Reset_Indicator_Load_Load(r16,v) _BFSET_(r16, 0, 0,v)

    #define     w32TSSSWrapper_Warm_Reset_Indicator_Load       {\
            UNSG32 uWarm_Reset_Indicator_Load_Load             :  1;\
            UNSG32 RSVDx5FC_b1                                 : 31;\
          }
    union { UNSG32 u32TSSSWrapper_Warm_Reset_Indicator_Load;
            struct w32TSSSWrapper_Warm_Reset_Indicator_Load;
          };
    ///////////////////////////////////////////////////////////
              SIE_DBG_IF                                       ie_DBG_IF;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx6CC                                    [52];
    ///////////////////////////////////////////////////////////
              SIE_BCM_Security                                 ie_BCM_Security;
    ///////////////////////////////////////////////////////////
    } SIE_TSSSWrapper;

    typedef union  T32TSSSWrapper_IntStatus
          { UNSG32 u32;
            struct w32TSSSWrapper_IntStatus;
                 } T32TSSSWrapper_IntStatus;
    typedef union  T32TSSSWrapper_IntMask
          { UNSG32 u32;
            struct w32TSSSWrapper_IntMask;
                 } T32TSSSWrapper_IntMask;
    typedef union  T32TSSSWrapper_IntEnable
          { UNSG32 u32;
            struct w32TSSSWrapper_IntEnable;
                 } T32TSSSWrapper_IntEnable;
    typedef union  T32TSSSWrapper_TransError
          { UNSG32 u32;
            struct w32TSSSWrapper_TransError;
                 } T32TSSSWrapper_TransError;
    typedef union  T32TSSSWrapper_Spare_reg
          { UNSG32 u32;
            struct w32TSSSWrapper_Spare_reg;
                 } T32TSSSWrapper_Spare_reg;
    typedef union  T32TSSSWrapper_Spare_reg1
          { UNSG32 u32;
            struct w32TSSSWrapper_Spare_reg1;
                 } T32TSSSWrapper_Spare_reg1;
    typedef union  T32TSSSWrapper_Spare_reg2
          { UNSG32 u32;
            struct w32TSSSWrapper_Spare_reg2;
                 } T32TSSSWrapper_Spare_reg2;
    typedef union  T32TSSSWrapper_Spare_reg3
          { UNSG32 u32;
            struct w32TSSSWrapper_Spare_reg3;
                 } T32TSSSWrapper_Spare_reg3;
    typedef union  T32TSSSWrapper_Spare_reg4
          { UNSG32 u32;
            struct w32TSSSWrapper_Spare_reg4;
                 } T32TSSSWrapper_Spare_reg4;
    typedef union  T32TSSSWrapper_Spare_reg5
          { UNSG32 u32;
            struct w32TSSSWrapper_Spare_reg5;
                 } T32TSSSWrapper_Spare_reg5;
    typedef union  T32TSSSWrapper_Spare_reg6
          { UNSG32 u32;
            struct w32TSSSWrapper_Spare_reg6;
                 } T32TSSSWrapper_Spare_reg6;
    typedef union  T32TSSSWrapper_Spare_reg7
          { UNSG32 u32;
            struct w32TSSSWrapper_Spare_reg7;
                 } T32TSSSWrapper_Spare_reg7;
    typedef union  T32TSSSWrapper_TEE_user_password_valid
          { UNSG32 u32;
            struct w32TSSSWrapper_TEE_user_password_valid;
                 } T32TSSSWrapper_TEE_user_password_valid;
    typedef union  T32TSSSWrapper_TEE_user_password_sel
          { UNSG32 u32;
            struct w32TSSSWrapper_TEE_user_password_sel;
                 } T32TSSSWrapper_TEE_user_password_sel;
    typedef union  T32TSSSWrapper_FPGA_Control
          { UNSG32 u32;
            struct w32TSSSWrapper_FPGA_Control;
                 } T32TSSSWrapper_FPGA_Control;
    typedef union  T32TSSSWrapper_IFCP_BIV_Lock
          { UNSG32 u32;
            struct w32TSSSWrapper_IFCP_BIV_Lock;
                 } T32TSSSWrapper_IFCP_BIV_Lock;
    typedef union  T32TSSSWrapper_SCPU_Key_Sel
          { UNSG32 u32;
            struct w32TSSSWrapper_SCPU_Key_Sel;
                 } T32TSSSWrapper_SCPU_Key_Sel;
    typedef union  T32TSSSWrapper_Warm_Reset_Indicator
          { UNSG32 u32;
            struct w32TSSSWrapper_Warm_Reset_Indicator;
                 } T32TSSSWrapper_Warm_Reset_Indicator;
    typedef union  T32TSSSWrapper_Warm_Reset_Indicator_Latch
          { UNSG32 u32;
            struct w32TSSSWrapper_Warm_Reset_Indicator_Latch;
                 } T32TSSSWrapper_Warm_Reset_Indicator_Latch;
    typedef union  T32TSSSWrapper_Warm_Reset_Indicator_Load
          { UNSG32 u32;
            struct w32TSSSWrapper_Warm_Reset_Indicator_Load;
                 } T32TSSSWrapper_Warm_Reset_Indicator_Load;
    ///////////////////////////////////////////////////////////

    typedef union  TTSSSWrapper_IntStatus
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_IntStatus;
                   };
                 } TTSSSWrapper_IntStatus;
    typedef union  TTSSSWrapper_IntMask
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_IntMask;
                   };
                 } TTSSSWrapper_IntMask;
    typedef union  TTSSSWrapper_IntEnable
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_IntEnable;
                   };
                 } TTSSSWrapper_IntEnable;
    typedef union  TTSSSWrapper_TransError
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_TransError;
                   };
                 } TTSSSWrapper_TransError;
    typedef union  TTSSSWrapper_Spare_reg
          { UNSG32 u32[8];
            struct {
            struct w32TSSSWrapper_Spare_reg;
            struct w32TSSSWrapper_Spare_reg1;
            struct w32TSSSWrapper_Spare_reg2;
            struct w32TSSSWrapper_Spare_reg3;
            struct w32TSSSWrapper_Spare_reg4;
            struct w32TSSSWrapper_Spare_reg5;
            struct w32TSSSWrapper_Spare_reg6;
            struct w32TSSSWrapper_Spare_reg7;
                   };
                 } TTSSSWrapper_Spare_reg;
    typedef union  TTSSSWrapper_TEE_user_password_valid
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_TEE_user_password_valid;
                   };
                 } TTSSSWrapper_TEE_user_password_valid;
    typedef union  TTSSSWrapper_TEE_user_password_sel
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_TEE_user_password_sel;
                   };
                 } TTSSSWrapper_TEE_user_password_sel;
    typedef union  TTSSSWrapper_FPGA_Control
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_FPGA_Control;
                   };
                 } TTSSSWrapper_FPGA_Control;
    typedef union  TTSSSWrapper_IFCP_BIV_Lock
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_IFCP_BIV_Lock;
                   };
                 } TTSSSWrapper_IFCP_BIV_Lock;
    typedef union  TTSSSWrapper_SCPU_Key_Sel
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_SCPU_Key_Sel;
                   };
                 } TTSSSWrapper_SCPU_Key_Sel;
    typedef union  TTSSSWrapper_Warm_Reset_Indicator
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_Warm_Reset_Indicator;
                   };
                 } TTSSSWrapper_Warm_Reset_Indicator;
    typedef union  TTSSSWrapper_Warm_Reset_Indicator_Latch
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_Warm_Reset_Indicator_Latch;
                   };
                 } TTSSSWrapper_Warm_Reset_Indicator_Latch;
    typedef union  TTSSSWrapper_Warm_Reset_Indicator_Load
          { UNSG32 u32[1];
            struct {
            struct w32TSSSWrapper_Warm_Reset_Indicator_Load;
                   };
                 } TTSSSWrapper_Warm_Reset_Indicator_Load;

    ///////////////////////////////////////////////////////////
     SIGN32 TSSSWrapper_drvrd(SIE_TSSSWrapper *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TSSSWrapper_drvwr(SIE_TSSSWrapper *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TSSSWrapper_reset(SIE_TSSSWrapper *p);
     SIGN32 TSSSWrapper_cmp  (SIE_TSSSWrapper *p, SIE_TSSSWrapper *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TSSSWrapper_check(p,pie,pfx,hLOG) TSSSWrapper_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TSSSWrapper_print(p,    pfx,hLOG) TSSSWrapper_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: TSSSWrapper
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE TSSS                    biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 IFCP_ROM             (RW)
///               %unsigned 1  IFCP_ROM                  0x0
///               %%        31         # Stuffing bits...
///     @ 0x00004                      (W-)
///     #         # Stuffing bytes...
///               %% 524256
///     @ 0x10000 NSK2_WRAPPER         (RW-)
///               %unsigned 1  NSK2_WRAPPER              0x0
///               %%        31         # Stuffing bits...
///     @ 0x10004                      (W-)
///     #         # Stuffing bytes...
///               %% 557024
///     @ 0x21000 IFCP                 (RW)
///               %unsigned 1  IFCP                      0x0
///               %%        31         # Stuffing bits...
///     @ 0x21004                      (W-)
///     #         # Stuffing bytes...
///               %% 32736
///     @ 0x22000 NOCS3_WRAPPER        (RW-)
///               %unsigned 1  NOCS3_WRAPPER             0x0
///               %%        31         # Stuffing bits...
///     @ 0x22004                      (W-)
///     #         # Stuffing bytes...
///               %% 65504
///     @ 0x24000 KILOPASS_OTP         (RW-)
///               %unsigned 1  KILOPASS_OTP              0x0
///               %%        31         # Stuffing bits...
///     @ 0x24004                      (W-)
///     #         # Stuffing bytes...
///               %% 196576
///     @ 0x2A000 TspKeyTbl            (P)
///     # 0x2A000 TspKeyTbl            
///               $OneReg              TspKeyTbl         MEM    [1024]
///     @ 0x2B000                      (W-)
///     #         # Stuffing bytes...
///               %% 32768
///     @ 0x2C000 WRAPPER              (P)
///     # 0x2C000 WRAPPER              
///               $TSSSWrapper         WRAPPER           REG          
///                                    ###
///                                    * wrapper registers
///                                    * Accessibility:
///                                    * Only SPU has the R/W access
///                                    ###
///     @ 0x2CA74                      (W-)
///     #         # Stuffing bytes...
///               %% 11360
///     @ 0x2D000 Tsp1KeyTbl           (P)
///     # 0x2D000 Tsp1KeyTbl           
///               $OneReg              Tsp1KeyTbl        MEM    [1024]
///     @ 0x2E000 TSIO                 (P)
///               ###
///               * TSIO register space
///               * Accessibility:
///               * Any master
///               ###
///     @ 0x2E000                      (W-)
///     #         # Stuffing bytes...
///               %% 32768
///     @ 0x2F000 BC                   (RW)
///               %unsigned 1  BC                        0x0
///               %%        31         # Stuffing bits...
///     @ 0x2F004                      (W-)
///     #         # Stuffing bytes...
///               %% 8160
///     @ 0x2F400 GKL                  (RW)
///               %unsigned 1  GKL                       0x0
///               %%        31         # Stuffing bits...
///     @ 0x2F404                      (W-)
///     #         # Stuffing bytes...
///               %% 24544
///     @ 0x30000 EMM                  (RW)
///               %unsigned 1  EMM                       0x0
///                                    ###
///                                    * EMM registers 64KB
///                                    * Both ACPU and SCPU can access this region
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x30004                      (W-)
///     #         # Stuffing bytes...
///               %% 32736
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:  200704B, bits:    5623b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_TSSS
#define h_TSSS (){}

    #define     RA_TSSS_IFCP_ROM                               0x0000

    #define     BA_TSSS_IFCP_ROM_IFCP_ROM                      0x0000
    #define     B16TSSS_IFCP_ROM_IFCP_ROM                      0x0000
    #define   LSb32TSSS_IFCP_ROM_IFCP_ROM                         0
    #define   LSb16TSSS_IFCP_ROM_IFCP_ROM                         0
    #define       bTSSS_IFCP_ROM_IFCP_ROM                      1
    #define   MSK32TSSS_IFCP_ROM_IFCP_ROM                         0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_NSK2_WRAPPER                           0x10000

    #define     BA_TSSS_NSK2_WRAPPER_NSK2_WRAPPER              0x10000
    #define     B16TSSS_NSK2_WRAPPER_NSK2_WRAPPER              0x10000
    #define   LSb32TSSS_NSK2_WRAPPER_NSK2_WRAPPER                 0
    #define   LSb16TSSS_NSK2_WRAPPER_NSK2_WRAPPER                 0
    #define       bTSSS_NSK2_WRAPPER_NSK2_WRAPPER              1
    #define   MSK32TSSS_NSK2_WRAPPER_NSK2_WRAPPER                 0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_IFCP                                   0x21000

    #define     BA_TSSS_IFCP_IFCP                              0x21000
    #define     B16TSSS_IFCP_IFCP                              0x21000
    #define   LSb32TSSS_IFCP_IFCP                                 0
    #define   LSb16TSSS_IFCP_IFCP                                 0
    #define       bTSSS_IFCP_IFCP                              1
    #define   MSK32TSSS_IFCP_IFCP                                 0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_NOCS3_WRAPPER                          0x22000

    #define     BA_TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER            0x22000
    #define     B16TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER            0x22000
    #define   LSb32TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER               0
    #define   LSb16TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER               0
    #define       bTSSS_NOCS3_WRAPPER_NOCS3_WRAPPER            1
    #define   MSK32TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER               0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_KILOPASS_OTP                           0x24000

    #define     BA_TSSS_KILOPASS_OTP_KILOPASS_OTP              0x24000
    #define     B16TSSS_KILOPASS_OTP_KILOPASS_OTP              0x24000
    #define   LSb32TSSS_KILOPASS_OTP_KILOPASS_OTP                 0
    #define   LSb16TSSS_KILOPASS_OTP_KILOPASS_OTP                 0
    #define       bTSSS_KILOPASS_OTP_KILOPASS_OTP              1
    #define   MSK32TSSS_KILOPASS_OTP_KILOPASS_OTP                 0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_TspKeyTbl                              0x2A000
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_WRAPPER                                0x2C000
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_Tsp1KeyTbl                             0x2D000
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_TSIO                                   0x2E000
    #define     RA_TSSS_BC                                     0x2F000

    #define     BA_TSSS_BC_BC                                  0x2F000
    #define     B16TSSS_BC_BC                                  0x2F000
    #define   LSb32TSSS_BC_BC                                     0
    #define   LSb16TSSS_BC_BC                                     0
    #define       bTSSS_BC_BC                                  1
    #define   MSK32TSSS_BC_BC                                     0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_GKL                                    0x2F400

    #define     BA_TSSS_GKL_GKL                                0x2F400
    #define     B16TSSS_GKL_GKL                                0x2F400
    #define   LSb32TSSS_GKL_GKL                                   0
    #define   LSb16TSSS_GKL_GKL                                   0
    #define       bTSSS_GKL_GKL                                1
    #define   MSK32TSSS_GKL_GKL                                   0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_TSSS_EMM                                    0x30000

    #define     BA_TSSS_EMM_EMM                                0x30000
    #define     B16TSSS_EMM_EMM                                0x30000
    #define   LSb32TSSS_EMM_EMM                                   0
    #define   LSb16TSSS_EMM_EMM                                   0
    #define       bTSSS_EMM_EMM                                1
    #define   MSK32TSSS_EMM_EMM                                   0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_TSSS {
    ///////////////////////////////////////////////////////////
    #define   GET32TSSS_IFCP_ROM_IFCP_ROM(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32TSSS_IFCP_ROM_IFCP_ROM(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16TSSS_IFCP_ROM_IFCP_ROM(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16TSSS_IFCP_ROM_IFCP_ROM(r16,v)               _BFSET_(r16, 0, 0,v)

    #define     w32TSSS_IFCP_ROM                               {\
            UNSG32 uIFCP_ROM_IFCP_ROM                          :  1;\
            UNSG32 RSVDx0_b1                                   : 31;\
          }
    union { UNSG32 u32TSSS_IFCP_ROM;
            struct w32TSSS_IFCP_ROM;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx4                                      [65532];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSS_NSK2_WRAPPER_NSK2_WRAPPER(r32)         _BFGET_(r32, 0, 0)
    #define   SET32TSSS_NSK2_WRAPPER_NSK2_WRAPPER(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   GET16TSSS_NSK2_WRAPPER_NSK2_WRAPPER(r16)         _BFGET_(r16, 0, 0)
    #define   SET16TSSS_NSK2_WRAPPER_NSK2_WRAPPER(r16,v)       _BFSET_(r16, 0, 0,v)

    #define     w32TSSS_NSK2_WRAPPER                           {\
            UNSG32 uNSK2_WRAPPER_NSK2_WRAPPER                  :  1;\
            UNSG32 RSVDx10000_b1                               : 31;\
          }
    union { UNSG32 u32TSSS_NSK2_WRAPPER;
            struct w32TSSS_NSK2_WRAPPER;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx10004                                  [69628];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSS_IFCP_IFCP(r32)                         _BFGET_(r32, 0, 0)
    #define   SET32TSSS_IFCP_IFCP(r32,v)                       _BFSET_(r32, 0, 0,v)
    #define   GET16TSSS_IFCP_IFCP(r16)                         _BFGET_(r16, 0, 0)
    #define   SET16TSSS_IFCP_IFCP(r16,v)                       _BFSET_(r16, 0, 0,v)

    #define     w32TSSS_IFCP                                   {\
            UNSG32 uIFCP_IFCP                                  :  1;\
            UNSG32 RSVDx21000_b1                               : 31;\
          }
    union { UNSG32 u32TSSS_IFCP;
            struct w32TSSS_IFCP;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx21004                                  [4092];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER(r32)       _BFGET_(r32, 0, 0)
    #define   SET32TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER(r32,v)     _BFSET_(r32, 0, 0,v)
    #define   GET16TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER(r16)       _BFGET_(r16, 0, 0)
    #define   SET16TSSS_NOCS3_WRAPPER_NOCS3_WRAPPER(r16,v)     _BFSET_(r16, 0, 0,v)

    #define     w32TSSS_NOCS3_WRAPPER                          {\
            UNSG32 uNOCS3_WRAPPER_NOCS3_WRAPPER                :  1;\
            UNSG32 RSVDx22000_b1                               : 31;\
          }
    union { UNSG32 u32TSSS_NOCS3_WRAPPER;
            struct w32TSSS_NOCS3_WRAPPER;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx22004                                  [8188];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSS_KILOPASS_OTP_KILOPASS_OTP(r32)         _BFGET_(r32, 0, 0)
    #define   SET32TSSS_KILOPASS_OTP_KILOPASS_OTP(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   GET16TSSS_KILOPASS_OTP_KILOPASS_OTP(r16)         _BFGET_(r16, 0, 0)
    #define   SET16TSSS_KILOPASS_OTP_KILOPASS_OTP(r16,v)       _BFSET_(r16, 0, 0,v)

    #define     w32TSSS_KILOPASS_OTP                           {\
            UNSG32 uKILOPASS_OTP_KILOPASS_OTP                  :  1;\
            UNSG32 RSVDx24000_b1                               : 31;\
          }
    union { UNSG32 u32TSSS_KILOPASS_OTP;
            struct w32TSSS_KILOPASS_OTP;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx24004                                  [24572];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_TspKeyTbl[1024];
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx2B000                                  [4096];
    ///////////////////////////////////////////////////////////
              SIE_TSSSWrapper                                  ie_WRAPPER;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx2CA74                                  [1420];
    ///////////////////////////////////////////////////////////
              SIE_OneReg                                       ie_Tsp1KeyTbl[1024];
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx2E000                                  [4096];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSS_BC_BC(r32)                             _BFGET_(r32, 0, 0)
    #define   SET32TSSS_BC_BC(r32,v)                           _BFSET_(r32, 0, 0,v)
    #define   GET16TSSS_BC_BC(r16)                             _BFGET_(r16, 0, 0)
    #define   SET16TSSS_BC_BC(r16,v)                           _BFSET_(r16, 0, 0,v)

    #define     w32TSSS_BC                                     {\
            UNSG32 uBC_BC                                      :  1;\
            UNSG32 RSVDx2F000_b1                               : 31;\
          }
    union { UNSG32 u32TSSS_BC;
            struct w32TSSS_BC;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx2F004                                  [1020];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSS_GKL_GKL(r32)                           _BFGET_(r32, 0, 0)
    #define   SET32TSSS_GKL_GKL(r32,v)                         _BFSET_(r32, 0, 0,v)
    #define   GET16TSSS_GKL_GKL(r16)                           _BFGET_(r16, 0, 0)
    #define   SET16TSSS_GKL_GKL(r16,v)                         _BFSET_(r16, 0, 0,v)

    #define     w32TSSS_GKL                                    {\
            UNSG32 uGKL_GKL                                    :  1;\
            UNSG32 RSVDx2F400_b1                               : 31;\
          }
    union { UNSG32 u32TSSS_GKL;
            struct w32TSSS_GKL;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx2F404                                  [3068];
    ///////////////////////////////////////////////////////////
    #define   GET32TSSS_EMM_EMM(r32)                           _BFGET_(r32, 0, 0)
    #define   SET32TSSS_EMM_EMM(r32,v)                         _BFSET_(r32, 0, 0,v)
    #define   GET16TSSS_EMM_EMM(r16)                           _BFGET_(r16, 0, 0)
    #define   SET16TSSS_EMM_EMM(r16,v)                         _BFSET_(r16, 0, 0,v)

    #define     w32TSSS_EMM                                    {\
            UNSG32 uEMM_EMM                                    :  1;\
            UNSG32 RSVDx30000_b1                               : 31;\
          }
    union { UNSG32 u32TSSS_EMM;
            struct w32TSSS_EMM;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx30004                                  [4092];
    ///////////////////////////////////////////////////////////
    } SIE_TSSS;

    typedef union  T32TSSS_IFCP_ROM
          { UNSG32 u32;
            struct w32TSSS_IFCP_ROM;
                 } T32TSSS_IFCP_ROM;
    typedef union  T32TSSS_NSK2_WRAPPER
          { UNSG32 u32;
            struct w32TSSS_NSK2_WRAPPER;
                 } T32TSSS_NSK2_WRAPPER;
    typedef union  T32TSSS_IFCP
          { UNSG32 u32;
            struct w32TSSS_IFCP;
                 } T32TSSS_IFCP;
    typedef union  T32TSSS_NOCS3_WRAPPER
          { UNSG32 u32;
            struct w32TSSS_NOCS3_WRAPPER;
                 } T32TSSS_NOCS3_WRAPPER;
    typedef union  T32TSSS_KILOPASS_OTP
          { UNSG32 u32;
            struct w32TSSS_KILOPASS_OTP;
                 } T32TSSS_KILOPASS_OTP;
    typedef union  T32TSSS_BC
          { UNSG32 u32;
            struct w32TSSS_BC;
                 } T32TSSS_BC;
    typedef union  T32TSSS_GKL
          { UNSG32 u32;
            struct w32TSSS_GKL;
                 } T32TSSS_GKL;
    typedef union  T32TSSS_EMM
          { UNSG32 u32;
            struct w32TSSS_EMM;
                 } T32TSSS_EMM;
    ///////////////////////////////////////////////////////////

    typedef union  TTSSS_IFCP_ROM
          { UNSG32 u32[1];
            struct {
            struct w32TSSS_IFCP_ROM;
                   };
                 } TTSSS_IFCP_ROM;
    typedef union  TTSSS_NSK2_WRAPPER
          { UNSG32 u32[1];
            struct {
            struct w32TSSS_NSK2_WRAPPER;
                   };
                 } TTSSS_NSK2_WRAPPER;
    typedef union  TTSSS_IFCP
          { UNSG32 u32[1];
            struct {
            struct w32TSSS_IFCP;
                   };
                 } TTSSS_IFCP;
    typedef union  TTSSS_NOCS3_WRAPPER
          { UNSG32 u32[1];
            struct {
            struct w32TSSS_NOCS3_WRAPPER;
                   };
                 } TTSSS_NOCS3_WRAPPER;
    typedef union  TTSSS_KILOPASS_OTP
          { UNSG32 u32[1];
            struct {
            struct w32TSSS_KILOPASS_OTP;
                   };
                 } TTSSS_KILOPASS_OTP;
    typedef union  TTSSS_BC
          { UNSG32 u32[1];
            struct {
            struct w32TSSS_BC;
                   };
                 } TTSSS_BC;
    typedef union  TTSSS_GKL
          { UNSG32 u32[1];
            struct {
            struct w32TSSS_GKL;
                   };
                 } TTSSS_GKL;
    typedef union  TTSSS_EMM
          { UNSG32 u32[1];
            struct {
            struct w32TSSS_EMM;
                   };
                 } TTSSS_EMM;

    ///////////////////////////////////////////////////////////
     SIGN32 TSSS_drvrd(SIE_TSSS *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TSSS_drvwr(SIE_TSSS *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TSSS_reset(SIE_TSSS *p);
     SIGN32 TSSS_cmp  (SIE_TSSS *p, SIE_TSSS *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TSSS_check(p,pie,pfx,hLOG) TSSS_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TSSS_print(p,    pfx,hLOG) TSSS_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: TSSS
////////////////////////////////////////////////////////////



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif
//////
/// ENDOFFILE: tsSS.h
////////////////////////////////////////////////////////////

