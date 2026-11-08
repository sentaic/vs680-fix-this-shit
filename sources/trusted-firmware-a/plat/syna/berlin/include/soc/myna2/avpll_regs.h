/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AVPLL_REGS_H__
#define __AVPLL_REGS_H__

#define AVIO_MEMMAP_AVIO_GBL_BASE   0xA0000
#define RA_avioGbl_AVPLLA           0x0000
#define RA_avPll_ctrlPLL            0x0000
#define RA_avPll_C1                 0x001C
#define RA_avPll_C2                 0x002C
#define RA_avPll_C3                 0x003C
#define RA_avPll_C4                 0x004C
#define RA_avPll_C5                 0x005C
#define RA_avPll_C6                 0x006C
#define RA_avPll_C7                 0x007C
#define RA_avPll_C8                 0x008C
#define RA_avpllCh_ctrl1            0x0004
#define RA_avpllCh8_ctrl1           0x0004

#define MSK32avPll_ctrlPLL_PU       0x00000002
#define MSK32avpllCh_ctrl_PU        0x00100000

#define RA_Gbl_cpufastRefClk        0x720
#define RA_Gbl_memfastRefClk        0x724
#define RA_Gbl_cfgClk               0x740
#define RA_Gbl_perifSysClk          0x744
#define RA_Gbl_atbClk               0x748
#define RA_Gbl_decoderClk           0x74C
#define RA_Gbl_decoderM3Clk         0x750
#define RA_Gbl_encoderClk           0x754
#define RA_Gbl_ovpCoreClk           0x758
#define RA_Gbl_gfx3DCoreClk         0x760
#define RA_Gbl_gfx3DSysClk          0x764
#define RA_Gbl_avioSysClk           0x770
#define RA_Gbl_vppSysClk            0x774
#define RA_Gbl_eddcClk              0x778
#define RA_Gbl_avioBiuClk           0x77C
#define RA_Gbl_zspClk               0x780
#define RA_Gbl_tspClk               0x790
#define RA_Gbl_tspRefClk            0x794
#define RA_Gbl_ndsClk               0x798
#define RA_Gbl_nocsClk              0x79C
#define RA_Gbl_apbCoreClk           0x7A0
#define RA_Gbl_emmcClk              0x7B0
#define RA_Gbl_sd0Clk               0x7B4
#define RA_Gbl_sd1Clk               0x7B8
#define RA_Gbl_dllMstRefClk         0x7BC
#define RA_Gbl_gethRgmiiClk         0x7C0
#define RA_Gbl_gethRgmiiSysClk      0x7C4
#define RA_Gbl_usim0Clk             0x7D0
#define RA_Gbl_pcieTestClk          0x7E0
#define RA_Gbl_usb2TestClk          0x7F0
#define RA_Gbl_usb3TestClk          0x7F4
#define RA_Gbl_usb3CoreClk          0x7F8
#define RA_Gbl_nfcEccClk            0x800
#define RA_Gbl_bcmClk               0x810

#define AVPLL_CONTROL_NUM           9

#define CPU_FAST_REF_CLK            0
#define MEM_FAST_REF_CLK            1
#define CFG_CLK                     2
#define PERIF_SYS_CLK               3
#define ATB_CLK                     4
#define DECODER_CLK                 5
#define DECODER_M3_CLK              6
#define ENCODER_CLK                 7
#define OVP_CORE_CLK                8
#define GFX3D_CORE_CLK              9
#define GFX3D_SYS_CLK               10
#define AVIO_SYS_CLK                11
#define VPP_SYS_CLK                 12
#define EDDC_CLK                    13
#define AVIO_BIU_CLK                14
#define ZSP_CLK                     15
#define TSP_CLK                     16
#define TSP_REF_CLK                 17
#define NDS_CLK                     18
#define NOCS_CLK                    19
#define APB_CORE_CLK                20
#define EMMC_CLK                    21
#define SD0_CLK                     22
#define SD1_CLK                     23
#define DLL_MASTER_REF_CLK          24
#define GMAC_SMARTCARD_CLK          25
#define GMAC_SMARTCARD_SYS_CLK      26
#define USIM0_CLK                   27
#define PCIE_TEST_CLK               28
#define USB2_TEST_CLK               29
#define USB3_TEST_CLK               30
#define USB3_CORE_CLK               31
#define NFC_ECC_CLK                 32
#define BCM_CLK                     33
#define AVPLL_CLK_MODULE_NUM        34

#endif
