/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


//*****************************************************************************
//
// bcm_drv_xbar.h - BCM XBAR Component related structure definition
//
//*****************************************************************************
#include "bcm_drv_xbar.h"

STATUS bcm_drv_xbar_set_path(uint32_t path_id)
{
     volatile struct abus * pABUS = (volatile struct abus *) BASE_ABUS;
     struct abus_ABUS_CFG temp;

     // uncomment and fix stuff as we progress
     temp.u.value = 0;
     switch(path_id)
     {  
         //case OUT_DMA_PATH:   pABUS->ABUS_CFG = 0x271;break; //group B    group A   wr_cb_ctrl    rd_cb_ctrl	/////
         //case AES_PATH:       pABUS->ABUS_CFG = 0x070;break; //AES      disconnect  straight      straight	/////
         //case SP_AES_PATH:    pABUS->ABUS_CFG = 0x025;break; //AES      Bypass      cross         cross	/////
         //case HASH_AES_PATH:  pABUS->ABUS_CFG = 0x001;break; //AES      HASH        straight      cross	/////
         //case AES_HASH_PATH:  pABUS->ABUS_CFG = 0x004;break; //AES      HASH        cross         straight	/////
         //case ECP_AES_PATH:   pABUS->ABUS_CFG = 0x050;break; //AES      ECP         straight      straight	/////
         //case ZMODP_AES_PATH: pABUS->ABUS_CFG = 0x060;break; //AES      ZMODP       straight      straight	/////
         //case BYPASS_PATH:    pABUS->ABUS_CFG = 0x222;break; //Bypass   Bypass      cross         straight	/////
         //case RC4_PATH:       pABUS->ABUS_CFG = 0x440;break; //RC4      RC4         straight      straight	/////
         //case DES_PATH:       pABUS->ABUS_CFG = 0x170;break; //DES      disconnect  straight      straight     /////

         //straight: accellerator <--> group A
         //   cross: accellerator <--> group B
         case IN_DMA_PATH:
           temp.u.b.RdCrossBar_Sel = 0;
           temp.u.b.WrCrossBar_Sel = 1;
           temp.u.b.GroupA_Sel     = 1; //Else = reserved
           temp.u.b.GroupB_Sel     = 2; //**** Bypass
           temp.u.b.GroupC_Sel     = 1; //Else = reserved
           break;      
         case OUT_DMA_PATH:
           temp.u.b.RdCrossBar_Sel = 1;
           temp.u.b.WrCrossBar_Sel = 0;
           temp.u.b.GroupA_Sel     = 1; //Else = reserved
           temp.u.b.GroupB_Sel     = 2; //**** Bypass
           temp.u.b.GroupC_Sel     = 1; //Else = reserved
           break;
         case IN_OUT_DMA_PATH:
           temp.u.b.RdCrossBar_Sel = 0;
           temp.u.b.WrCrossBar_Sel = 0;
           temp.u.b.GroupA_Sel     = 1; //Else = reserved
           temp.u.b.GroupB_Sel     = 2; //**** Bypass
           temp.u.b.GroupC_Sel     = 1; //Else = reserved
           break;
         case AES_PATH:
           temp.u.b.RdCrossBar_Sel = 0; //straight
           temp.u.b.WrCrossBar_Sel = 0; //straight
           temp.u.b.GroupA_Sel     = 1; //Else = reserved
           temp.u.b.GroupB_Sel     = 0; //**** AES
           temp.u.b.GroupC_Sel     = 1; //Else = reserved
           break;
         case SP_AES_PATH:
           temp.u.b.RdCrossBar_Sel = 1; //cross
           temp.u.b.WrCrossBar_Sel = 1; //cross
           temp.u.b.GroupA_Sel     = 1; //Else = reserved
           temp.u.b.GroupB_Sel     = 0; //**** AES
           temp.u.b.GroupC_Sel     = 1; //Else = reserved
           break;
         case HASH_AES_PATH:
           temp.u.b.RdCrossBar_Sel = 1; //cross
           temp.u.b.WrCrossBar_Sel = 0; //straight
           temp.u.b.GroupA_Sel     = 0; //**** HASH
           temp.u.b.GroupB_Sel     = 0; //**** AES
           temp.u.b.GroupC_Sel     = 1; //Else = reserved
           break;
         case AES_HASH_PATH:
           temp.u.b.RdCrossBar_Sel = 0; //straight
           temp.u.b.WrCrossBar_Sel = 1; //cross
           temp.u.b.GroupA_Sel     = 0; //**** HASH
           temp.u.b.GroupB_Sel     = 0; //**** AES
           temp.u.b.GroupC_Sel     = 1; //Else = reserved
           break;
         case ZMODP_AES_PATH:
         case ECP_AES_PATH:
           temp.u.b.RdCrossBar_Sel = 0; //straight
           temp.u.b.WrCrossBar_Sel = 0; //straight
           temp.u.b.GroupA_Sel     = 1; //Else = reserved
           temp.u.b.GroupB_Sel     = 0; //**** AES
           temp.u.b.GroupC_Sel     = 0; //**** AMC (ZMODP, ECP)
           break;
         case RC4_PATH:
           temp.u.b.RdCrossBar_Sel = 1; //cross
           temp.u.b.WrCrossBar_Sel = 1; //cross
           temp.u.b.GroupA_Sel     = 2; //3; //**** RC4
           temp.u.b.GroupB_Sel     = 2; //Bypass
           temp.u.b.GroupC_Sel     = 2; //3; //**** RC4
           break;
         case DES_PATH:
           temp.u.b.RdCrossBar_Sel = 0; //straight
           temp.u.b.WrCrossBar_Sel = 0; //straight
           temp.u.b.GroupA_Sel     = 1; //Else = reserved
           temp.u.b.GroupB_Sel     = 1; //***** DES/TDES
           temp.u.b.GroupC_Sel     = 1; //Else = reserved
           break;
         case BYPASS_PATH:
           temp.u.b.RdCrossBar_Sel = 0; //straight
           temp.u.b.WrCrossBar_Sel = 1; //cross
           temp.u.b.GroupA_Sel     = 2; //Bypass (Else, reserved)
           temp.u.b.GroupB_Sel     = 2; //Bypass
           temp.u.b.GroupC_Sel     = 2; //Bypass (Else, reserved)
           break;

         //                                                      group B    group A   wr_cb_ctrl    rd_cb_ctrl
         //case GHASH_PATH:     pABUS->ABUS_CFG = 0x125;break; //blocked  GHASH       cross         cross
         default:return(-1);
     }
     pABUS->ABUS_CFG.u.value = temp.u.value;
     return STATUS_SUCCESS;
}
