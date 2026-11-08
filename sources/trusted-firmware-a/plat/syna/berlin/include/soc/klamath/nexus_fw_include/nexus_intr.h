//////////////////////////////////////////////// 
// 
// Nexus C Interrupt Defines from Memmap Excel File 
// Source Excel: nexus_memmap.xlsx 
//         Time: Thu Jan  9 16:06:00 2025
//         User: ftoong
// 
//////////////////////////////////////////////// 

#define   MCU_IRQ_gpio_wake_req_0        0 
#define   MCU_IRQ_gpio_wake_req_1        1 
#define   MCU_IRQ_gpio_wake_req_2        2 
#define   MCU_IRQ_gpio_wake_req_3        3 
#define   MCU_IRQ_wdog_slowclk           4 
#define   MCU_IRQ_wdog_rst_slowclk       5 
#define   MCU_IRQ_timer_slowclk          6 
#define   MCU_IRQ_csyspwrupreq           7 
// #define   RESERVED                    8 
// #define   RESERVED                    9 
// #define   RESERVED                    10 
// #define   RESERVED                    11 
// #define   RESERVED                    12 
// #define   RESERVED                    13 
// #define   RESERVED                    14 
// #define   RESERVED                    15 
#define   MCU_IRQ_channel_sdma_0         16 
#define   MCU_IRQ_channel_sdma_1         17 
#define   MCU_IRQ_comb_nonsec_sdma       18 
#define   MCU_IRQ_comb_sec_sdma          19 
#define   MCU_IRQ_sec_viol_err_sdma      20 
#define   MCU_IRQ_app_crypto             21 
#define   MCU_IRQ_timer0                 22 
#define   MCU_IRQ_timer1                 23 
#define   MCU_IRQ_timer2                 24 
#define   MCU_IRQ_timer3                 25 
#define   MCU_IRQ_timer4                 26 
#define   MCU_IRQ_wdog0                  27 
#define   MCU_IRQ_wdog0_rst              28 
#define   MCU_IRQ_wdog1                  29 
#define   MCU_IRQ_wdog1_rst              30 
#define   MCU_IRQ_wdog2                  31 
#define   MCU_IRQ_wdog2_rst              32 
#define   MCU_IRQ_channel_pdma_0         33 
#define   MCU_IRQ_channel_pdma_1         34 
#define   MCU_IRQ_channel_pdma_2         35 
#define   MCU_IRQ_channel_pdma_3         36 
#define   MCU_IRQ_channel_pdma_4         37 
#define   MCU_IRQ_channel_pdma_5         38 
#define   MCU_IRQ_channel_pdma_6         39 
#define   MCU_IRQ_channel_pdma_7         40 
#define   MCU_IRQ_channel_pdma_8         41 
#define   MCU_IRQ_channel_pdma_9         42 
#define   MCU_IRQ_channel_pdma_10        43 
#define   MCU_IRQ_channel_pdma_11        44 
#define   MCU_IRQ_channel_pdma_12        45 
#define   MCU_IRQ_channel_pdma_13        46 
#define   MCU_IRQ_channel_pdma_14        47 
#define   MCU_IRQ_channel_pdma_15        48 
#define   MCU_IRQ_channel_pdma_16        49 
#define   MCU_IRQ_channel_pdma_17        50 
#define   MCU_IRQ_channel_pdma_18        51 
#define   MCU_IRQ_channel_pdma_19        52 
#define   MCU_IRQ_channel_pdma_20        53 
#define   MCU_IRQ_channel_pdma_21        54 
#define   MCU_IRQ_channel_pdma_22        55 
#define   MCU_IRQ_channel_pdma_23        56 
#define   MCU_IRQ_channel_pdma_24        57 
#define   MCU_IRQ_channel_pdma_25        58 
#define   MCU_IRQ_channel_pdma_26        59 
#define   MCU_IRQ_channel_pdma_27        60 
#define   MCU_IRQ_channel_pdma_28        61 
#define   MCU_IRQ_channel_pdma_29        62 
#define   MCU_IRQ_channel_pdma_30        63 
#define   MCU_IRQ_channel_pdma_31        64 
#define   MCU_IRQ_comb_nonsec_pdma       65 
#define   MCU_IRQ_comb_sec_pdma          66 
#define   MCU_IRQ_sec_viol_err_pdma      67 
#define   MCU_IRQ_fault_0                68 
#define   MCU_IRQ_fault_1                69 
#define   MCU_IRQ_fault_2                70 
#define   MCU_IRQ_fault_3                71 
#define   MCU_IRQ_fault_4                72 
#define   MCU_IRQ_fault_5                73 
#define   MCU_IRQ_fault_6                74 
#define   MCU_IRQ_fault_7                75 
#define   MCU_IRQ_fault_8                76 
#define   MCU_IRQ_fault_9                77 
#define   MCU_IRQ_fault_10               78 
#define   MCU_IRQ_fault_11               79 
#define   MCU_IRQ_fault_12               80 
#define   MCU_IRQ_fault_13               81 
#define   MCU_IRQ_fault_14               82 
#define   MCU_IRQ_fault_15               83 
#define   MCU_IRQ_i3c                    84 
#define   MCU_IRQ_uart0                  85 
#define   MCU_IRQ_uart1                  86 
#define   MCU_IRQ_uart2                  87 
#define   MCU_IRQ_uart3                  88 
#define   MCU_IRQ_spi                    89 
#define   MCU_IRQ_i2c0                   90 
#define   MCU_IRQ_i2c1                   91 
#define   MCU_IRQ_gpio0                  92 
#define   MCU_IRQ_gpio1                  93 
#define   MCU_IRQ_gpio2                  94 
#define   MCU_IRQ_gpio3                  95 
#define   MCU_IRQ_pdm                    96 
#define   MCU_IRQ_spi_s                  97 
#define   MCU_IRQ_adc                    98 
#define   MCU_IRQ_xspi                   99 
#define   MCU_IRQ_can0                   100 
#define   MCU_IRQ_can1                   101 
#define   MCU_IRQ_tcpwm_0                102 
#define   MCU_IRQ_tcpwm_1                103 
#define   MCU_IRQ_tcpwm_2                104 
#define   MCU_IRQ_tcpwm_3                105 
#define   MCU_IRQ_tcpwm_4                106 
#define   MCU_IRQ_tcpwm_5                107 
#define   MCU_IRQ_tcpwm_6                108 
#define   MCU_IRQ_tcpwm_7                109 
#define   MCU_IRQ_tcpwm_8                110 
#define   MCU_IRQ_tcpwm_9                111 
#define   MCU_IRQ_tcpwm_10               112 
#define   MCU_IRQ_tcpwm_11               113 
#define   MCU_IRQ_tcpwm_12               114 
#define   MCU_IRQ_tcpwm_13               115 
#define   MCU_IRQ_tcpwm_14               116 
#define   MCU_IRQ_clk_mon                117 
#define   MCU_IRQ_mcu_gbl                118 
#define   MCU_IRQ_otf_crypto             119 
#define   MCU_IRQ_trng                   120 
#define   MCU_IRQ_ppc                    121 
#define   MCU_IRQ_mpc_sysrom             122 
#define   MCU_IRQ_mpc_xspi               123 
#define   MCU_IRQ_msc                    124 
#define   MCU_IRQ_dsense                 125 
#define   MCU_IRQ_fpixc                  126 
#define   MCU_IRQ_fpidc                  127 
#define   MCU_IRQ_fpofc                  128 
#define   MCU_IRQ_fpufc                  129 
#define   MCU_IRQ_fpdzc                  130 
#define   MCU_IRQ_fpioc                  131 
#define   MCU_IRQ_pvt                    132 
#define   MCU_IRQ_pvt_temp_oor           133 
#define   MCU_IRQ_sprot_mb_ree_s2h       134 
#define   MCU_IRQ_sprot_mb_tee_s2h       135 
#define   MCU_IRQ_sprot_mb_ree_h2s       136 
#define   MCU_IRQ_sprot_mb_tee_h2s       137 
#define   MCU_IRQ_lock_sprot_mbox        138 
#define   MCU_IRQ_lock_app_crypto        139 
#define   MCU_IRQ_bmc                    140 
// #define   RESERVED                    141 
// #define   RESERVED                    142 
// #define   RESERVED                    143 
// #define   RESERVED                    163:144 
#define   MCU_IRQ_soc2mcu_0              164 
#define   MCU_IRQ_soc2mcu_1              165 
#define   MCU_IRQ_soc2mcu_2              166 
#define   MCU_IRQ_soc2mcu_3              167 
#define   MCU_IRQ_soc2mcu_4              168 
#define   MCU_IRQ_soc2mcu_5              169 
#define   MCU_IRQ_soc2mcu_6              170 
#define   MCU_IRQ_soc2mcu_7              171 
#define   MCU_IRQ_soc2mcu_8              172 
#define   MCU_IRQ_soc2mcu_9              173 
#define   MCU_IRQ_soc2mcu_10             174 
#define   MCU_IRQ_soc2mcu_11             175 
#define   MCU_IRQ_soc2mcu_12             176 
#define   MCU_IRQ_soc2mcu_13             177 
#define   MCU_IRQ_soc2mcu_14             178 
#define   MCU_IRQ_soc2mcu_15             179 
#define   MCU_IRQ_soc2mcu_16             180 
#define   MCU_IRQ_soc2mcu_17             181 
#define   MCU_IRQ_soc2mcu_18             182 
#define   MCU_IRQ_soc2mcu_19             183 
#define   MCU_IRQ_soc2mcu_20             184 
#define   MCU_IRQ_soc2mcu_21             185 
#define   MCU_IRQ_soc2mcu_22             186 
#define   MCU_IRQ_soc2mcu_23             187 
#define   MCU_IRQ_soc2mcu_24             188 
#define   MCU_IRQ_soc2mcu_25             189 
#define   MCU_IRQ_soc2mcu_26             190 
#define   MCU_IRQ_soc2mcu_27             191 
#define   MCU_IRQ_soc2mcu_28             192 
#define   MCU_IRQ_soc2mcu_29             193 
#define   MCU_IRQ_soc2mcu_30             194 
#define   MCU_IRQ_soc2mcu_31             195 

#define   MCU_IRQ_TOTAL                  196 

#define   IRQ_MCU2SOC_app_crypto         0 
#define   IRQ_MCU2SOC_i3c                1 
#define   IRQ_MCU2SOC_uart0              2 
#define   IRQ_MCU2SOC_uart1              3 
#define   IRQ_MCU2SOC_uart2              4 
#define   IRQ_MCU2SOC_uart3              5 
#define   IRQ_MCU2SOC_spi                6 
#define   IRQ_MCU2SOC_i2c0               7 
#define   IRQ_MCU2SOC_i2c1               8 
#define   IRQ_MCU2SOC_gpio0              9 
#define   IRQ_MCU2SOC_gpio1              10 
#define   IRQ_MCU2SOC_gpio2              11 
#define   IRQ_MCU2SOC_gpio3              12 
#define   IRQ_MCU2SOC_pdm                13 
#define   IRQ_MCU2SOC_xspi               14 
#define   IRQ_MCU2SOC_tcpwm_0            15 
#define   IRQ_MCU2SOC_tcpwm_1            16 
#define   IRQ_MCU2SOC_tcpwm_2            17 
#define   IRQ_MCU2SOC_tcpwm_3            18 
#define   IRQ_MCU2SOC_tcpwm_4            19 
#define   IRQ_MCU2SOC_tcpwm_5            20 
#define   IRQ_MCU2SOC_tcpwm_6            21 
#define   IRQ_MCU2SOC_tcpwm_7            22 
#define   IRQ_MCU2SOC_tcpwm_8            23 
#define   IRQ_MCU2SOC_tcpwm_9            24 
#define   IRQ_MCU2SOC_tcpwm_10           25 
#define   IRQ_MCU2SOC_tcpwm_11           26 
#define   IRQ_MCU2SOC_tcpwm_12           27 
#define   IRQ_MCU2SOC_tcpwm_13           28 
#define   IRQ_MCU2SOC_tcpwm_14           29 
#define   IRQ_MCU2SOC_adc                30 
#define   IRQ_MCU2SOC_trng               31 
#define   IRQ_MCU2SOC_comb_nonsec_pdma   32 
#define   IRQ_MCU2SOC_comb_sec_pdma      33 
#define   IRQ_MCU2SOC_sec_viol_err_pdma  34 
#define   IRQ_MCU2SOC_can0               35 
#define   IRQ_MCU2SOC_can1               36 
#define   IRQ_MCU2SOC_channel_pdma_0     37 
#define   IRQ_MCU2SOC_channel_pdma_1     38 
#define   IRQ_MCU2SOC_channel_pdma_2     39 
#define   IRQ_MCU2SOC_channel_pdma_3     40 
#define   IRQ_MCU2SOC_channel_pdma_4     41 
#define   IRQ_MCU2SOC_channel_pdma_5     42 
#define   IRQ_MCU2SOC_channel_pdma_6     43 
#define   IRQ_MCU2SOC_channel_pdma_7     44 
#define   IRQ_MCU2SOC_channel_pdma_8     45 
#define   IRQ_MCU2SOC_fault_0            46 
#define   IRQ_MCU2SOC_fault_1            47 
#define   IRQ_MCU2SOC_fault_2            48 
#define   IRQ_MCU2SOC_fault_3            49 
#define   IRQ_MCU2SOC_pvt                50 
#define   IRQ_MCU2SOC_pvt_temp_oor       51 
#define   IRQ_MCU2SOC_sprot_mb_ree_s2h   52 
#define   IRQ_MCU2SOC_sprot_mb_tee_s2h   53 
#define   IRQ_MCU2SOC_sprot_mb_ree_h2s   54 
#define   IRQ_MCU2SOC_sprot_mb_tee_h2s   55 
#define   IRQ_MCU2SOC_lock_sprot_mbox    56 
#define   IRQ_MCU2SOC_lock_app_crypto    57 
// #define   RESERVED                    58 
// #define   RESERVED                    59 
// #define   RESERVED                    60 
// #define   RESERVED                    61 
// #define   RESERVED                    62 
// #define   RESERVED                    63 

#define   IRQ_MCU2SOC_TOTAL              64 
