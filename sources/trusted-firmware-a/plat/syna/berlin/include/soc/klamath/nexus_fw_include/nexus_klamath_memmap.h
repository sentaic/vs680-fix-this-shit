//////////////////////////////////////////////// 
// 
// Nexus C Defines from Memmap Excel File 
// Source Excel: nexus_memmap.xlsx 
//         Time: Fri Feb  7 09:25:20 2025
//         User: ftoong
// 
//////////////////////////////////////////////// 

#define       START_MCU_ITCM_NSC                   0x00000000 
#define         END_MCU_ITCM_NSC                   0x0003FFFF 
#define        SIZE_MCU_ITCM_NSC                   0x40000 
#define     DEC_BIT_MCU_ITCM_NSC                   0X12 

#define       START_MCU_ROM_NSC                    0x01000000 
#define         END_MCU_ROM_NSC                    0x0100FFFF 
#define        SIZE_MCU_ROM_NSC                    0x10000 
#define     DEC_BIT_MCU_ROM_NSC                    0X10 

#define       START_MCU_XSPIMEM_NSC                0x08000000 
#define         END_MCU_XSPIMEM_NSC                0x0FFFFFFF 
#define        SIZE_MCU_XSPIMEM_NSC                0x8000000 
#define     DEC_BIT_MCU_XSPIMEM_NSC                0X1B 

#define       START_MCU_ITCM_SC                    0x10000000 
#define         END_MCU_ITCM_SC                    0x1003FFFF 
#define        SIZE_MCU_ITCM_SC                    0x40000 
#define     DEC_BIT_MCU_ITCM_SC                    0X12 

#define       START_MCU_ROM_SC                     0x11000000 
#define         END_MCU_ROM_SC                     0x1100FFFF 
#define        SIZE_MCU_ROM_SC                     0x10000 
#define     DEC_BIT_MCU_ROM_SC                     0X10 

#define       START_MCU_XSPIMEM_SC                 0x18000000 
#define         END_MCU_XSPIMEM_SC                 0x1FFFFFFF 
#define        SIZE_MCU_XSPIMEM_SC                 0x8000000 
#define     DEC_BIT_MCU_XSPIMEM_SC                 0X1B 

#define       START_MCU_DTCM_NS                    0x20000000 
#define         END_MCU_DTCM_NS                    0x2003FFFF 
#define        SIZE_MCU_DTCM_NS                    0x40000 
#define     DEC_BIT_MCU_DTCM_NS                    0X12 

#define       START_MCU_XSPIMEM_NS                 0x28000000 
#define         END_MCU_XSPIMEM_NS                 0x2FFFFFFF 
#define        SIZE_MCU_XSPIMEM_NS                 0x8000000 
#define     DEC_BIT_MCU_XSPIMEM_NS                 0X1B 

#define       START_MCU_DTCM_S                     0x30000000 
#define         END_MCU_DTCM_S                     0x3003FFFF 
#define        SIZE_MCU_DTCM_S                     0x40000 
#define     DEC_BIT_MCU_DTCM_S                     0X12 

#define       START_MCU_XSPIMEM_S                  0x38000000 
#define         END_MCU_XSPIMEM_S                  0x3FFFFFFF 
#define        SIZE_MCU_XSPIMEM_S                  0x8000000 
#define     DEC_BIT_MCU_XSPIMEM_S                  0X1B 

#define       START_MCU_SDMA_NS                    0x40000000 
#define         END_MCU_SDMA_NS                    0x40001FFF 
#define        SIZE_MCU_SDMA_NS                    0x2000 
#define     DEC_BIT_MCU_SDMA_NS                    0XD 

#define       START_MCU_PDMA_NS                    0x40004000 
#define         END_MCU_PDMA_NS                    0x40007FFF 
#define        SIZE_MCU_PDMA_NS                    0x4000 
#define     DEC_BIT_MCU_PDMA_NS                    0XE 

#define       START_MCU_SPROT_OTP_NS               0x40010000 
#define         END_MCU_SPROT_OTP_NS               0x40013FFF 
#define        SIZE_MCU_SPROT_OTP_NS               0x4000 
#define     DEC_BIT_MCU_SPROT_OTP_NS               0XE 

#define       START_MCU_SPROT_MBOX_NS              0x40014000 
#define         END_MCU_SPROT_MBOX_NS              0x40014FFF 
#define        SIZE_MCU_SPROT_MBOX_NS              0x1000 
#define     DEC_BIT_MCU_SPROT_MBOX_NS              0XC 

#define       START_MCU_OTF_CRYPTO_NS              0x40015000 
#define         END_MCU_OTF_CRYPTO_NS              0x40015FFF 
#define        SIZE_MCU_OTF_CRYPTO_NS              0x1000 
#define     DEC_BIT_MCU_OTF_CRYPTO_NS              0XC 

#define       START_MCU_OTF_CACHE_NS               0x40016000 
#define         END_MCU_OTF_CACHE_NS               0x40016FFF 
#define        SIZE_MCU_OTF_CACHE_NS               0x1000 
#define     DEC_BIT_MCU_OTF_CACHE_NS               0XC 

#define       START_MCU_APP_CRYPTO_NS              0x40018000 
#define         END_MCU_APP_CRYPTO_NS              0x4001BFFF 
#define        SIZE_MCU_APP_CRYPTO_NS              0x4000 
#define     DEC_BIT_MCU_APP_CRYPTO_NS              0XE 

#define       START_MCU_APP_CRYPTO_RKEK_NS         0x4001C000 
#define         END_MCU_APP_CRYPTO_RKEK_NS         0x4001CFFF 
#define        SIZE_MCU_APP_CRYPTO_RKEK_NS         0x1000 
#define     DEC_BIT_MCU_APP_CRYPTO_RKEK_NS         0XC 

#define       START_MCU_TRNG_NS                    0x4001D000 
#define         END_MCU_TRNG_NS                    0x4001DFFF 
#define        SIZE_MCU_TRNG_NS                    0x1000 
#define     DEC_BIT_MCU_TRNG_NS                    0XC 

#define       START_MCU_MAIN_GPV_NS                0x40020000 
#define         END_MCU_MAIN_GPV_NS                0x4002FFFF 
#define        SIZE_MCU_MAIN_GPV_NS                0x10000 
#define     DEC_BIT_MCU_MAIN_GPV_NS                0X10 

#define       START_MCU_TIMER0_NS                  0x48000000 
#define         END_MCU_TIMER0_NS                  0x48000FFF 
#define        SIZE_MCU_TIMER0_NS                  0x1000 
#define     DEC_BIT_MCU_TIMER0_NS                  0XC 

#define       START_MCU_TIMER1_NS                  0x48001000 
#define         END_MCU_TIMER1_NS                  0x48001FFF 
#define        SIZE_MCU_TIMER1_NS                  0x1000 
#define     DEC_BIT_MCU_TIMER1_NS                  0XC 

#define       START_MCU_TIMER2_NS                  0x48002000 
#define         END_MCU_TIMER2_NS                  0x48002FFF 
#define        SIZE_MCU_TIMER2_NS                  0x1000 
#define     DEC_BIT_MCU_TIMER2_NS                  0XC 

#define       START_MCU_TIMER3_NS                  0x48003000 
#define         END_MCU_TIMER3_NS                  0x48003FFF 
#define        SIZE_MCU_TIMER3_NS                  0x1000 
#define     DEC_BIT_MCU_TIMER3_NS                  0XC 

#define       START_MCU_TIMER4_NS                  0x48004000 
#define         END_MCU_TIMER4_NS                  0x48004FFF 
#define        SIZE_MCU_TIMER4_NS                  0x1000 
#define     DEC_BIT_MCU_TIMER4_NS                  0XC 

#define       START_MCU_WD0_NS                     0x48006000 
#define         END_MCU_WD0_NS                     0x48007FFF 
#define        SIZE_MCU_WD0_NS                     0x2000 
#define     DEC_BIT_MCU_WD0_NS                     0XD 

#define       START_MCU_WD1_NS                     0x48008000 
#define         END_MCU_WD1_NS                     0x48009FFF 
#define        SIZE_MCU_WD1_NS                     0x2000 
#define     DEC_BIT_MCU_WD1_NS                     0XD 

#define       START_MCU_WD2_NS                     0x4800A000 
#define         END_MCU_WD2_NS                     0x4800BFFF 
#define        SIZE_MCU_WD2_NS                     0x2000 
#define     DEC_BIT_MCU_WD2_NS                     0XD 

#define       START_MCU_SYSCOUNT_CTRL_NS           0x4800C000 
#define         END_MCU_SYSCOUNT_CTRL_NS           0x4800CFFF 
#define        SIZE_MCU_SYSCOUNT_CTRL_NS           0x1000 
#define     DEC_BIT_MCU_SYSCOUNT_CTRL_NS           0XC 

#define       START_MCU_SYSCOUNT_FRM_NS            0x4800D000 
#define         END_MCU_SYSCOUNT_FRM_NS            0x4800DFFF 
#define        SIZE_MCU_SYSCOUNT_FRM_NS            0x1000 
#define     DEC_BIT_MCU_SYSCOUNT_FRM_NS            0XC 

#define       START_MCU_TIMER_SLOWCLK_NS           0x4800E000 
#define         END_MCU_TIMER_SLOWCLK_NS           0x4800EFFF 
#define        SIZE_MCU_TIMER_SLOWCLK_NS           0x1000 
#define     DEC_BIT_MCU_TIMER_SLOWCLK_NS           0XC 

#define       START_MCU_WD_SLOWCLK_NS              0x4800F000 
#define         END_MCU_WD_SLOWCLK_NS              0x4800FFFF 
#define        SIZE_MCU_WD_SLOWCLK_NS              0x1000 
#define     DEC_BIT_MCU_WD_SLOWCLK_NS              0XC 

#define       START_MCU_LCS_NS                     0x48010000 
#define         END_MCU_LCS_NS                     0x48010FFF 
#define        SIZE_MCU_LCS_NS                     0x1000 
#define     DEC_BIT_MCU_LCS_NS                     0XC 

#define       START_MCU_FC_NS                      0x48011000 
#define         END_MCU_FC_NS                      0x48011FFF 
#define        SIZE_MCU_FC_NS                      0x1000 
#define     DEC_BIT_MCU_FC_NS                      0XC 

#define       START_MCU_PPU_NS                     0x48012000 
#define         END_MCU_PPU_NS                     0x48012FFF 
#define        SIZE_MCU_PPU_NS                     0x1000 
#define     DEC_BIT_MCU_PPU_NS                     0XC 

#define       START_MCU_SECCTRL_NS                 0x48020000 
#define         END_MCU_SECCTRL_NS                 0x48020FFF 
#define        SIZE_MCU_SECCTRL_NS                 0x1000 
#define     DEC_BIT_MCU_SECCTRL_NS                 0XC 

#define       START_MCU_SYSCTRL_NS                 0x48021000 
#define         END_MCU_SYSCTRL_NS                 0x48021FFF 
#define        SIZE_MCU_SYSCTRL_NS                 0x1000 
#define     DEC_BIT_MCU_SYSCTRL_NS                 0XC 

#define       START_MCU_ADC_NS                     0x48022000 
#define         END_MCU_ADC_NS                     0x48022FFF 
#define        SIZE_MCU_ADC_NS                     0x1000 
#define     DEC_BIT_MCU_ADC_NS                     0XC 

#define       START_MCU_PVT_NS                     0x48023000 
#define         END_MCU_PVT_NS                     0x48023FFF 
#define        SIZE_MCU_PVT_NS                     0x1000 
#define     DEC_BIT_MCU_PVT_NS                     0XC 

#define       START_MCU_GBL_SEC_NS                 0x48024000 
#define         END_MCU_GBL_SEC_NS                 0x48024FFF 
#define        SIZE_MCU_GBL_SEC_NS                 0x1000 
#define     DEC_BIT_MCU_GBL_SEC_NS                 0XC 

#define       START_MCU_GBL_NS                     0x48025000 
#define         END_MCU_GBL_NS                     0x48025FFF 
#define        SIZE_MCU_GBL_NS                     0x1000 
#define     DEC_BIT_MCU_GBL_NS                     0XC 

#define       START_MCU_DSENSE_NS                  0x48026000 
#define         END_MCU_DSENSE_NS                  0x48026FFF 
#define        SIZE_MCU_DSENSE_NS                  0x1000 
#define     DEC_BIT_MCU_DSENSE_NS                  0XC 

#define       START_MCU_UART0_NS                   0x48030000 
#define         END_MCU_UART0_NS                   0x48030FFF 
#define        SIZE_MCU_UART0_NS                   0x1000 
#define     DEC_BIT_MCU_UART0_NS                   0XC 

#define       START_MCU_UART1_NS                   0x48031000 
#define         END_MCU_UART1_NS                   0x48031FFF 
#define        SIZE_MCU_UART1_NS                   0x1000 
#define     DEC_BIT_MCU_UART1_NS                   0XC 

#define       START_MCU_UART2_NS                   0x48032000 
#define         END_MCU_UART2_NS                   0x48032FFF 
#define        SIZE_MCU_UART2_NS                   0x1000 
#define     DEC_BIT_MCU_UART2_NS                   0XC 

#define       START_MCU_UART3_NS                   0x48033000 
#define         END_MCU_UART3_NS                   0x48033FFF 
#define        SIZE_MCU_UART3_NS                   0x1000 
#define     DEC_BIT_MCU_UART3_NS                   0XC 

#define       START_MCU_SPI_NS                     0x48034000 
#define         END_MCU_SPI_NS                     0x48034FFF 
#define        SIZE_MCU_SPI_NS                     0x1000 
#define     DEC_BIT_MCU_SPI_NS                     0XC 

#define       START_MCU_I2C0_NS                    0x48035000 
#define         END_MCU_I2C0_NS                    0x48035FFF 
#define        SIZE_MCU_I2C0_NS                    0x1000 
#define     DEC_BIT_MCU_I2C0_NS                    0XC 

#define       START_MCU_I2C1_NS                    0x48036000 
#define         END_MCU_I2C1_NS                    0x48036FFF 
#define        SIZE_MCU_I2C1_NS                    0x1000 
#define     DEC_BIT_MCU_I2C1_NS                    0XC 

#define       START_MCU_I3C_NS                     0x48037000 
#define         END_MCU_I3C_NS                     0x48037FFF 
#define        SIZE_MCU_I3C_NS                     0x1000 
#define     DEC_BIT_MCU_I3C_NS                     0XC 

#define       START_MCU_GPIO0_NS                   0x48038000 
#define         END_MCU_GPIO0_NS                   0x48038FFF 
#define        SIZE_MCU_GPIO0_NS                   0x1000 
#define     DEC_BIT_MCU_GPIO0_NS                   0XC 

#define       START_MCU_GPIO1_NS                   0x48039000 
#define         END_MCU_GPIO1_NS                   0x48039FFF 
#define        SIZE_MCU_GPIO1_NS                   0x1000 
#define     DEC_BIT_MCU_GPIO1_NS                   0XC 

#define       START_MCU_GPIO2_NS                   0x4803A000 
#define         END_MCU_GPIO2_NS                   0x4803AFFF 
#define        SIZE_MCU_GPIO2_NS                   0x1000 
#define     DEC_BIT_MCU_GPIO2_NS                   0XC 

#define       START_MCU_GPIO3_NS                   0x4803B000 
#define         END_MCU_GPIO3_NS                   0x4803BFFF 
#define        SIZE_MCU_GPIO3_NS                   0x1000 
#define     DEC_BIT_MCU_GPIO3_NS                   0XC 

#define       START_MCU_PDM_NS                     0x4803C000 
#define         END_MCU_PDM_NS                     0x4803CFFF 
#define        SIZE_MCU_PDM_NS                     0x1000 
#define     DEC_BIT_MCU_PDM_NS                     0XC 

#define       START_MCU_SPI_S_NS                   0x4803D000 
#define         END_MCU_SPI_S_NS                   0x4803DFFF 
#define        SIZE_MCU_SPI_S_NS                   0x1000 
#define     DEC_BIT_MCU_SPI_S_NS                   0XC 

#define       START_MCU_TCPWM_NS                   0x48040000 
#define         END_MCU_TCPWM_NS                   0x48040FFF 
#define        SIZE_MCU_TCPWM_NS                   0x1000 
#define     DEC_BIT_MCU_TCPWM_NS                   0XC 

#define       START_MCU_XSPI_PHY_NS                0x48050000 
#define         END_MCU_XSPI_PHY_NS                0x48053FFF 
#define        SIZE_MCU_XSPI_PHY_NS                0x4000 
#define     DEC_BIT_MCU_XSPI_PHY_NS                0XE 

#define       START_MCU_XSPI_CTRL_NS               0x48050000 
#define         END_MCU_XSPI_CTRL_NS               0x48050FFF 
#define        SIZE_MCU_XSPI_CTRL_NS               0x1000 
#define     DEC_BIT_MCU_XSPI_CTRL_NS               0XC 

#define       START_MCU_CAN0_NS                    0x48060000 
#define         END_MCU_CAN0_NS                    0x48060FFF 
#define        SIZE_MCU_CAN0_NS                    0x1000 
#define     DEC_BIT_MCU_CAN0_NS                    0XC 

#define       START_MCU_CAN1_NS                    0x48061000 
#define         END_MCU_CAN1_NS                    0x48061FFF 
#define        SIZE_MCU_CAN1_NS                    0x1000 
#define     DEC_BIT_MCU_CAN1_NS                    0XC 

#define       START_MCU_SDMA_S                     0x50000000 
#define         END_MCU_SDMA_S                     0x50001FFF 
#define        SIZE_MCU_SDMA_S                     0x2000 
#define     DEC_BIT_MCU_SDMA_S                     0XD 

#define       START_MCU_PDMA_S                     0x50004000 
#define         END_MCU_PDMA_S                     0x50007FFF 
#define        SIZE_MCU_PDMA_S                     0x4000 
#define     DEC_BIT_MCU_PDMA_S                     0XE 

#define       START_MCU_SPROT_OTP_S                0x50010000 
#define         END_MCU_SPROT_OTP_S                0x50013FFF 
#define        SIZE_MCU_SPROT_OTP_S                0x4000 
#define     DEC_BIT_MCU_SPROT_OTP_S                0XE 

#define       START_MCU_SPROT_MBOX_S               0x50014000 
#define         END_MCU_SPROT_MBOX_S               0x50014FFF 
#define        SIZE_MCU_SPROT_MBOX_S               0x1000 
#define     DEC_BIT_MCU_SPROT_MBOX_S               0XC 

#define       START_MCU_OTF_CRYPTO_S               0x50015000 
#define         END_MCU_OTF_CRYPTO_S               0x50015FFF 
#define        SIZE_MCU_OTF_CRYPTO_S               0x1000 
#define     DEC_BIT_MCU_OTF_CRYPTO_S               0XC 

#define       START_MCU_OTF_CACHE_S                0x50016000 
#define         END_MCU_OTF_CACHE_S                0x50016FFF 
#define        SIZE_MCU_OTF_CACHE_S                0x1000 
#define     DEC_BIT_MCU_OTF_CACHE_S                0XC 

#define       START_MCU_APP_CRYPTO_S               0x50018000 
#define         END_MCU_APP_CRYPTO_S               0x5001BFFF 
#define        SIZE_MCU_APP_CRYPTO_S               0x4000 
#define     DEC_BIT_MCU_APP_CRYPTO_S               0XE 

#define       START_MCU_APP_CRYPTO_RKEK_S          0x5001C000 
#define         END_MCU_APP_CRYPTO_RKEK_S          0x5001CFFF 
#define        SIZE_MCU_APP_CRYPTO_RKEK_S          0x1000 
#define     DEC_BIT_MCU_APP_CRYPTO_RKEK_S          0XC 

#define       START_MCU_TRNG_S                     0x5001D000 
#define         END_MCU_TRNG_S                     0x5001DFFF 
#define        SIZE_MCU_TRNG_S                     0x1000 
#define     DEC_BIT_MCU_TRNG_S                     0XC 

#define       START_MCU_MAIN_GPV_S                 0x50020000 
#define         END_MCU_MAIN_GPV_S                 0x5002FFFF 
#define        SIZE_MCU_MAIN_GPV_S                 0x10000 
#define     DEC_BIT_MCU_MAIN_GPV_S                 0X10 

#define       START_MCU_TIMER0_S                   0x58000000 
#define         END_MCU_TIMER0_S                   0x58000FFF 
#define        SIZE_MCU_TIMER0_S                   0x1000 
#define     DEC_BIT_MCU_TIMER0_S                   0XC 

#define       START_MCU_TIMER1_S                   0x58001000 
#define         END_MCU_TIMER1_S                   0x58001FFF 
#define        SIZE_MCU_TIMER1_S                   0x1000 
#define     DEC_BIT_MCU_TIMER1_S                   0XC 

#define       START_MCU_TIMER2_S                   0x58002000 
#define         END_MCU_TIMER2_S                   0x58002FFF 
#define        SIZE_MCU_TIMER2_S                   0x1000 
#define     DEC_BIT_MCU_TIMER2_S                   0XC 

#define       START_MCU_TIMER3_S                   0x58003000 
#define         END_MCU_TIMER3_S                   0x58003FFF 
#define        SIZE_MCU_TIMER3_S                   0x1000 
#define     DEC_BIT_MCU_TIMER3_S                   0XC 

#define       START_MCU_TIMER4_S                   0x58004000 
#define         END_MCU_TIMER4_S                   0x58004FFF 
#define        SIZE_MCU_TIMER4_S                   0x1000 
#define     DEC_BIT_MCU_TIMER4_S                   0XC 

#define       START_MCU_WD0_S                      0x58006000 
#define         END_MCU_WD0_S                      0x58007FFF 
#define        SIZE_MCU_WD0_S                      0x2000 
#define     DEC_BIT_MCU_WD0_S                      0XD 

#define       START_MCU_WD1_S                      0x58008000 
#define         END_MCU_WD1_S                      0x58009FFF 
#define        SIZE_MCU_WD1_S                      0x2000 
#define     DEC_BIT_MCU_WD1_S                      0XD 

#define       START_MCU_WD2_S                      0x5800A000 
#define         END_MCU_WD2_S                      0x5800BFFF 
#define        SIZE_MCU_WD2_S                      0x2000 
#define     DEC_BIT_MCU_WD2_S                      0XD 

#define       START_MCU_SYSCOUNT_CTRL_S            0x5800C000 
#define         END_MCU_SYSCOUNT_CTRL_S            0x5800CFFF 
#define        SIZE_MCU_SYSCOUNT_CTRL_S            0x1000 
#define     DEC_BIT_MCU_SYSCOUNT_CTRL_S            0XC 

#define       START_MCU_SYSCOUNT_FRM_S             0x5800D000 
#define         END_MCU_SYSCOUNT_FRM_S             0x5800DFFF 
#define        SIZE_MCU_SYSCOUNT_FRM_S             0x1000 
#define     DEC_BIT_MCU_SYSCOUNT_FRM_S             0XC 

#define       START_MCU_TIMER_SLOWCLK_S            0x5800E000 
#define         END_MCU_TIMER_SLOWCLK_S            0x5800EFFF 
#define        SIZE_MCU_TIMER_SLOWCLK_S            0x1000 
#define     DEC_BIT_MCU_TIMER_SLOWCLK_S            0XC 

#define       START_MCU_WD_SLOWCLK_S               0x5800F000 
#define         END_MCU_WD_SLOWCLK_S               0x5800FFFF 
#define        SIZE_MCU_WD_SLOWCLK_S               0x1000 
#define     DEC_BIT_MCU_WD_SLOWCLK_S               0XC 

#define       START_MCU_LCS_S                      0x58010000 
#define         END_MCU_LCS_S                      0x58010FFF 
#define        SIZE_MCU_LCS_S                      0x1000 
#define     DEC_BIT_MCU_LCS_S                      0XC 

#define       START_MCU_FC_S                       0x58011000 
#define         END_MCU_FC_S                       0x58011FFF 
#define        SIZE_MCU_FC_S                       0x1000 
#define     DEC_BIT_MCU_FC_S                       0XC 

#define       START_MCU_PPU_S                      0x58012000 
#define         END_MCU_PPU_S                      0x58012FFF 
#define        SIZE_MCU_PPU_S                      0x1000 
#define     DEC_BIT_MCU_PPU_S                      0XC 

#define       START_MCU_SECCTRL_S                  0x58020000 
#define         END_MCU_SECCTRL_S                  0x58020FFF 
#define        SIZE_MCU_SECCTRL_S                  0x1000 
#define     DEC_BIT_MCU_SECCTRL_S                  0XC 

#define       START_MCU_SYSCTRL_S                  0x58021000 
#define         END_MCU_SYSCTRL_S                  0x58021FFF 
#define        SIZE_MCU_SYSCTRL_S                  0x1000 
#define     DEC_BIT_MCU_SYSCTRL_S                  0XC 

#define       START_MCU_ADC_S                      0x58022000 
#define         END_MCU_ADC_S                      0x58022FFF 
#define        SIZE_MCU_ADC_S                      0x1000 
#define     DEC_BIT_MCU_ADC_S                      0XC 

#define       START_MCU_PVT_S                      0x58023000 
#define         END_MCU_PVT_S                      0x58023FFF 
#define        SIZE_MCU_PVT_S                      0x1000 
#define     DEC_BIT_MCU_PVT_S                      0XC 

#define       START_MCU_GBL_SEC_S                  0x58024000 
#define         END_MCU_GBL_SEC_S                  0x58024FFF 
#define        SIZE_MCU_GBL_SEC_S                  0x1000 
#define     DEC_BIT_MCU_GBL_SEC_S                  0XC 

#define       START_MCU_GBL_S                      0x58025000 
#define         END_MCU_GBL_S                      0x58025FFF 
#define        SIZE_MCU_GBL_S                      0x1000 
#define     DEC_BIT_MCU_GBL_S                      0XC 

#define       START_MCU_DSENSE_S                   0x58026000 
#define         END_MCU_DSENSE_S                   0x58026FFF 
#define        SIZE_MCU_DSENSE_S                   0x1000 
#define     DEC_BIT_MCU_DSENSE_S                   0XC 

#define       START_MCU_UART0_S                    0x58030000 
#define         END_MCU_UART0_S                    0x58030FFF 
#define        SIZE_MCU_UART0_S                    0x1000 
#define     DEC_BIT_MCU_UART0_S                    0XC 

#define       START_MCU_UART1_S                    0x58031000 
#define         END_MCU_UART1_S                    0x58031FFF 
#define        SIZE_MCU_UART1_S                    0x1000 
#define     DEC_BIT_MCU_UART1_S                    0XC 

#define       START_MCU_UART2_S                    0x58032000 
#define         END_MCU_UART2_S                    0x58032FFF 
#define        SIZE_MCU_UART2_S                    0x1000 
#define     DEC_BIT_MCU_UART2_S                    0XC 

#define       START_MCU_UART3_S                    0x58033000 
#define         END_MCU_UART3_S                    0x58033FFF 
#define        SIZE_MCU_UART3_S                    0x1000 
#define     DEC_BIT_MCU_UART3_S                    0XC 

#define       START_MCU_SPI_S                      0x58034000 
#define         END_MCU_SPI_S                      0x58034FFF 
#define        SIZE_MCU_SPI_S                      0x1000 
#define     DEC_BIT_MCU_SPI_S                      0XC 

#define       START_MCU_I2C0_S                     0x58035000 
#define         END_MCU_I2C0_S                     0x58035FFF 
#define        SIZE_MCU_I2C0_S                     0x1000 
#define     DEC_BIT_MCU_I2C0_S                     0XC 

#define       START_MCU_I2C1_S                     0x58036000 
#define         END_MCU_I2C1_S                     0x58036FFF 
#define        SIZE_MCU_I2C1_S                     0x1000 
#define     DEC_BIT_MCU_I2C1_S                     0XC 

#define       START_MCU_I3C_S                      0x58037000 
#define         END_MCU_I3C_S                      0x58037FFF 
#define        SIZE_MCU_I3C_S                      0x1000 
#define     DEC_BIT_MCU_I3C_S                      0XC 

#define       START_MCU_GPIO0_S                    0x58038000 
#define         END_MCU_GPIO0_S                    0x58038FFF 
#define        SIZE_MCU_GPIO0_S                    0x1000 
#define     DEC_BIT_MCU_GPIO0_S                    0XC 

#define       START_MCU_GPIO1_S                    0x58039000 
#define         END_MCU_GPIO1_S                    0x58039FFF 
#define        SIZE_MCU_GPIO1_S                    0x1000 
#define     DEC_BIT_MCU_GPIO1_S                    0XC 

#define       START_MCU_GPIO2_S                    0x5803A000 
#define         END_MCU_GPIO2_S                    0x5803AFFF 
#define        SIZE_MCU_GPIO2_S                    0x1000 
#define     DEC_BIT_MCU_GPIO2_S                    0XC 

#define       START_MCU_GPIO3_S                    0x5803B000 
#define         END_MCU_GPIO3_S                    0x5803BFFF 
#define        SIZE_MCU_GPIO3_S                    0x1000 
#define     DEC_BIT_MCU_GPIO3_S                    0XC 

#define       START_MCU_PDM_S                      0x5803C000 
#define         END_MCU_PDM_S                      0x5803CFFF 
#define        SIZE_MCU_PDM_S                      0x1000 
#define     DEC_BIT_MCU_PDM_S                      0XC 

#define       START_MCU_SPI_S_S                    0x5803D000 
#define         END_MCU_SPI_S_S                    0x5803DFFF 
#define        SIZE_MCU_SPI_S_S                    0x1000 
#define     DEC_BIT_MCU_SPI_S_S                    0XC 

#define       START_MCU_TCPWM_S                    0x58040000 
#define         END_MCU_TCPWM_S                    0x58040FFF 
#define        SIZE_MCU_TCPWM_S                    0x1000 
#define     DEC_BIT_MCU_TCPWM_S                    0XC 

#define       START_MCU_XSPI_PHY_S                 0x58050000 
#define         END_MCU_XSPI_PHY_S                 0x58053FFF 
#define        SIZE_MCU_XSPI_PHY_S                 0x4000 
#define     DEC_BIT_MCU_XSPI_PHY_S                 0XE 

#define       START_MCU_XSPI_CTRL_S                0x58050000 
#define         END_MCU_XSPI_CTRL_S                0x58050FFF 
#define        SIZE_MCU_XSPI_CTRL_S                0x1000 
#define     DEC_BIT_MCU_XSPI_CTRL_S                0XC 

#define       START_MCU_CAN0_S                     0x58060000 
#define         END_MCU_CAN0_S                     0x58060FFF 
#define        SIZE_MCU_CAN0_S                     0x1000 
#define     DEC_BIT_MCU_CAN0_S                     0XC 

#define       START_MCU_CAN1_S                     0x58061000 
#define         END_MCU_CAN1_S                     0x58061FFF 
#define        SIZE_MCU_CAN1_S                     0x1000 
#define     DEC_BIT_MCU_CAN1_S                     0XC 

#define       START_MCU_MPC_ROM_S                  0x58080000 
#define         END_MCU_MPC_ROM_S                  0x58080FFF 
#define        SIZE_MCU_MPC_ROM_S                  0x1000 
#define     DEC_BIT_MCU_MPC_ROM_S                  0XC 

#define       START_MCU_MPC_XSPI_S                 0x58081000 
#define         END_MCU_MPC_XSPI_S                 0x58081FFF 
#define        SIZE_MCU_MPC_XSPI_S                 0x1000 
#define     DEC_BIT_MCU_MPC_XSPI_S                 0XC 

#define       START_MCU_SOC0_NS                    0xA0000000 
#define         END_MCU_SOC0_NS                    0xAFFFFFFF 
#define        SIZE_MCU_SOC0_NS                    0x10000000 
#define     DEC_BIT_MCU_SOC0_NS                    0X1C 

#define       START_MCU_SOC0_S                     0xB0000000 
#define         END_MCU_SOC0_S                     0xBFFFFFFF 
#define        SIZE_MCU_SOC0_S                     0x10000000 
#define     DEC_BIT_MCU_SOC0_S                     0X1C 

#define       START_MCU_SOC1_NS                    0xC0000000 
#define         END_MCU_SOC1_NS                    0xCFFFFFFF 
#define        SIZE_MCU_SOC1_NS                    0x10000000 
#define     DEC_BIT_MCU_SOC1_NS                    0X1C 

#define       START_MCU_SOC1_S                     0xD0000000 
#define         END_MCU_SOC1_S                     0xDFFFFFFF 
#define        SIZE_MCU_SOC1_S                     0x10000000 
#define     DEC_BIT_MCU_SOC1_S                     0X1C 

