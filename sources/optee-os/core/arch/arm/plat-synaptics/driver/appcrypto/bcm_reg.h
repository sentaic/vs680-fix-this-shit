/*
 * Copyright (c) 2025, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef BCMREG_H
#define BCMREG_H

#include "bcm_external.h"
#include "bcm_reg_map.h"

typedef struct
{
    uint32_t command_parameter_0;                           // 0x0
    uint32_t command_parameter_1;                           // 0x4
    uint32_t command_parameter_2;                           // 0x8
    uint32_t command_parameter_3;                           // 0xc
    uint32_t command_parameter_4;                           // 0x10
    uint32_t command_parameter_5;                           // 0x14
    uint32_t command_parameter_6;                           // 0x18
    uint32_t command_parameter_7;                           // 0x1c
    uint32_t command_parameter_8;                           // 0x20
    uint32_t command_parameter_9;                           // 0x24
    uint32_t command_parameter_10;                          // 0x28
    uint32_t command_parameter_11;                          // 0x2c
    uint32_t command_parameter_12;                          // 0x30
    uint32_t command_parameter_13;                          // 0x34
    uint32_t command_parameter_14;                          // 0x38
    uint32_t command_parameter_15;                          // 0x3c
    uint32_t secure_processor_command;                      // 0x40
    uint8_t  reserved_0x44[60];
    uint32_t command_return_status;                         // 0x80
    uint32_t command_status_0;                              // 0x84
    uint32_t command_status_1;                              // 0x88
    uint32_t command_status_2;                              // 0x8c
    uint32_t command_status_3;                              // 0x90
    uint32_t command_status_4;                              // 0x94
    uint32_t command_status_5;                              // 0x98
    uint32_t command_status_6;                              // 0x9c
    uint32_t command_status_7;                              // 0xa0
    uint32_t command_status_8;                              // 0xa4
    uint32_t command_status_9;                              // 0xa8
    uint32_t command_status_10;                             // 0xac
    uint32_t command_status_11;                             // 0xb0
    uint32_t command_status_12;                             // 0xb4
    uint32_t command_status_13;                             // 0xb8
    uint32_t command_status_14;                             // 0xbc
    uint32_t command_status_15;                             // 0xc0
    uint32_t command_fifo_status;                           // 0xc4
    uint32_t host_interrupt_reset;                          // 0xc8
    uint32_t host_interrupt_mask;                           // 0xcc
    uint32_t host_exception_address;                        // 0xd0
    uint32_t sp_trust_register;                             // 0xd4
    uint32_t wtm_identification;                            // 0xd8
    uint32_t wtm_revision;                                  // 0xdc
    uint32_t context_status;                                // 0xe0
    uint8_t  reserved_0xe4[300];
    uint32_t sp_interrupt_set;                              // 0x210
    uint32_t sp_sram_control;                               // 0x214
    uint32_t sp_interrupt_reset;                            // 0x218
    uint32_t sp_interrupt_mask;                             // 0x21c
    uint32_t sp_control;                                    // 0x220
    uint32_t sp_exception_address;                          // 0x224
    uint32_t sp_exception_data;                             // 0x228
    uint32_t sp_exception_parity;                           // 0x22c
    uint8_t  reserved_0x230[4];
    uint32_t host_interrupt_set;                            // 0x234
    uint8_t  reserved_0x238[16];
    uint32_t trust_level;                                   // 0x248
    uint8_t  reserved_0x24c[436];
} BCM_BIU; // Space: 1024 bytes

#define TOP_RESET       BIT(0)
#define DMA_RESET       BIT(1)
#define XBAR_RESET      BIT(2)
#define AES_RESET       BIT(3)
#define ECC_RESET       BIT(4)
#define HASH_RESET      BIT(5)
#define SCRATCH_RESET   BIT(6)
#define ZMODP_RESET     BIT(7)
#define MCT_RESET       BIT(8)
#define OTP_RESET       BIT(9)
#define EBG_RESET       BIT(10)
#define AES2_RESET      BIT(13)

#define TOP_CLOCK       BIT(16)
#define DMA_CLOCK       BIT(17)
#define XBAR_CLOCK      BIT(18)
#define AES_CLOCK       BIT(19)
#define ECC_CLOCK       BIT(20)
#define HASH_CLOCK      BIT(21)
#define SCRATCH_CLOCK   BIT(22)
#define ZMODP_CLOCK     BIT(23)
#define MCT_CLOCK       BIT(24)
#define OTP_CLOCK       BIT(25)
#define EBG_CLOCK       BIT(26)
#define AES2_CLOCK      BIT(29)

#define ALL_ENG_RESET  (DMA_RESET|XBAR_RESET|AES_RESET|ECC_RESET|SCRATCH_RESET|ZMODP_RESET|MCT_RESET|OTP_RESET|HASH_RESET|EBG_RESET)
#define ALL_ENG_CLOCKS (DMA_CLOCK|XBAR_CLOCK|AES_CLOCK|ECC_CLOCK|SCRATCH_CLOCK|ZMODP_CLOCK|MCT_CLOCK|OTP_CLOCK|HASH_CLOCK|EBG_CLOCK)

typedef struct
{
    uint32_t engine_control;                                // 0x0
    uint32_t engine_control_2;                              // 0x4
    uint32_t axi_slave_control_register;                    // 0x8
    uint32_t address_decoder_interrupt_register;            // 0xc
    uint32_t address_decoder_interrupt_mask;                // 0x10
    uint32_t apb_access_error_address_register;             // 0x14
    uint32_t mp_fifo_error_read_pointer;                    // 0x18
    uint8_t  reserved_0x1c[996];
} BCM_APB_ADEC; // Space: 1024 bytes

typedef struct
{
    uint32_t input_dma_control_register;                    // 0x0
    uint32_t input_dma_status_register;                     // 0x4
    uint32_t dma_source_address;                            // 0x8
    uint32_t dma_input_transfer_counter;                    // 0xc
    uint32_t input_dma_next_link_list_address;              // 0x10
    uint32_t input_dma_interrupt_1;                         // 0x14
    uint32_t input_dma_interrupt_1_mask;                    // 0x18
    uint32_t output_dma_control_register;                   // 0x1c
    uint32_t output_dma_status_register;                    // 0x20
    uint32_t dma_output_fifo_destination_address;           // 0x24
    uint32_t output_dma_transfer_counter;                   // 0x28
    uint32_t output_dma_next_link_list_address;             // 0x2c
    uint32_t output_dma_interrupt_1;                        // 0x30
    uint32_t output_dma_interrupt_1_mask;                   // 0x34
    uint32_t axi_control_register;                          // 0x38
    uint32_t dma_input_fifo_remain_count;                   // 0x3c
    uint32_t dma_input_fifo_error_read_pointer;             // 0x40
    uint32_t dma_output_fifo_remain_space;                  // 0x44
    uint32_t dma_output_fifo_error_read_pointer;            // 0x48
    uint8_t  reserved_0x4c[20];
    uint32_t input_dma_control_2;                           // 0x60
    uint32_t output_dma_control_2;                          // 0x64
    uint8_t  reserved_0x68[152];
    uint32_t dma_input_fifo_access_port[64];                // 0x100
    uint8_t  reserved_0x200[256];
    uint32_t dma_output_fifo_access_port[64];               // 0x300
} BCM_DMA; // Space: 1024 bytes

typedef struct
{
    uint32_t input_dma_control_register;                    // 0x0
    uint32_t input_dma_source_address;                      // 0x4
    uint32_t input_transaction_length;                      // 0x8
    uint32_t input_dma_interrupt;                           // 0xc
    uint32_t input_dma_interrupt_mask;                      // 0x10
    uint8_t  reserved_0x14[12];
    uint32_t output_dma_control_register;                   // 0x20
    uint32_t output_dma_destination_address;                // 0x24
    uint32_t output_transaction_length;                     // 0x28
    uint32_t output_dma_interrupt;                          // 0x2c
    uint32_t output_dma_interrupt_mask;                     // 0x30
    uint8_t  reserved_0x34[204];
    uint32_t input_fifo_write_pointer;                      // 0x100
    uint32_t input_fifo_read_pointer;                       // 0x104
    uint32_t input_fifo_data_access;                        // 0x108
    uint32_t output_fifo_write_pointer;                     // 0x10c
    uint32_t output_fifo_read_pointer;                      // 0x110
    uint32_t output_fifo_data_access;                       // 0x114
    uint8_t  reserved_0x118[744];
} BCM_MAHB; // Space: 1024 bytes

typedef struct
{
    uint32_t trusted_module_internal_bus_control;           // 0x0
    uint8_t  reserved_0x4[1020];
} BCM_ACCEL_BUS_REG; // Space: 1024 bytes

typedef struct
{
    uint32_t aes_cfg_reg;                                   // 0x0
    uint32_t aes_ctrl_reg;                                  // 0x4
    uint32_t aes_cmd_reg;                                   // 0x8
    uint32_t aes_status_reg;                                // 0xc
    uint32_t aes_intrpt_reg;                                // 0x10
    uint32_t aes_intrpt_src_en_reg;                         // 0x14
    uint32_t aes_stream_size;                               // 0x18
    uint32_t des_cfg_reg;                                   // 0x1c
    uint32_t des_ctrl_reg;                                  // 0x20
    uint32_t des_cmd_reg;                                   // 0x24
    uint32_t des_status_reg;                                // 0x28
    uint32_t des_intrpt_reg;                                // 0x2c
    uint32_t des_intrpt_src_en_reg;                         // 0x30
    uint32_t des_stream_size;                               // 0x34
    uint32_t rc4_cfg_reg;                                   // 0x38
    uint32_t rc4_ctrl_reg;                                  // 0x3c
    uint32_t rc4_cmd_reg;                                   // 0x40
    uint32_t rc4_status_reg;                                // 0x44
    uint32_t rc4_intrpt_reg;                                // 0x48
    uint32_t rc4_intrpt_src_en_reg;                         // 0x4c
    uint32_t rc4_stream_size;                               // 0x50
    uint32_t rc4_index;                                     // 0x54
    uint32_t key2_word0;                                    // 0x58
    uint32_t key2_word1;                                    // 0x5c
    uint32_t key2_word2;                                    // 0x60
    uint32_t key2_word3;                                    // 0x64
    uint32_t key2_word4;                                    // 0x68
    uint32_t key2_word5;                                    // 0x6c
    uint32_t key2_word6;                                    // 0x70
    uint32_t key2_word7;                                    // 0x74
    uint32_t key1_word0;                                    // 0x78
    uint32_t key1_word1;                                    // 0x7c
    uint32_t key1_word2;                                    // 0x80
    uint32_t key1_word3;                                    // 0x84
    uint32_t key1_word4;                                    // 0x88
    uint32_t key1_word5;                                    // 0x8c
    uint32_t key1_word6;                                    // 0x90
    uint32_t key1_word7;                                    // 0x94
    uint32_t iv_word0;                                      // 0x98
    uint32_t iv_word1;                                      // 0x9c
    uint32_t iv_word2;                                      // 0xa0
    uint32_t iv_word3;                                      // 0xa4
    uint32_t engine_select_reg;                             // 0xa8
    uint8_t  reserved_0xac[852];
} BCM_CRYPT; // Space: 1024 bytes

typedef struct
{
    uint32_t aes_cfg_reg;                                   // 0x0
    uint32_t aes_ctrl_reg;                                  // 0x4
    uint32_t aes_cmd_reg;                                   // 0x8
    uint32_t aes_status_reg;                                // 0xc
    uint32_t aes_intrpt_reg;                                // 0x10
    uint32_t aes_intrpt_src_en_reg;                         // 0x14
    uint32_t aes_stream_size;                               // 0x18
    uint8_t  reserved_0x1c[60];
    uint32_t key2_word0;                                    // 0x58
    uint32_t key2_word1;                                    // 0x5c
    uint32_t key2_word2;                                    // 0x60
    uint32_t key2_word3;                                    // 0x64
    uint32_t key2_word4;                                    // 0x68
    uint32_t key2_word5;                                    // 0x6c
    uint32_t key2_word6;                                    // 0x70
    uint32_t key2_word7;                                    // 0x74
    uint32_t key1_word0;                                    // 0x78
    uint32_t key1_word1;                                    // 0x7c
    uint32_t key1_word2;                                    // 0x80
    uint32_t key1_word3;                                    // 0x84
    uint32_t key1_word4;                                    // 0x88
    uint32_t key1_word5;                                    // 0x8c
    uint32_t key1_word6;                                    // 0x90
    uint32_t key1_word7;                                    // 0x94
    uint32_t iv_word0;                                      // 0x98
    uint32_t iv_word1;                                      // 0x9c
    uint32_t iv_word2;                                      // 0xa0
    uint32_t iv_word3;                                      // 0xa4
    uint32_t engine_select_reg;                             // 0xa8
    uint32_t aes_sector_size;                               // 0xac
    uint32_t lba_word0;                                     // 0xb0
    uint32_t lba_word1;                                     // 0xb4
    uint8_t  reserved_0xb8[840];
} BCM_AES; // Space: 1024 bytes

typedef struct
{
    uint32_t configuration_register;                        // 0x0
    uint32_t ecp_control_register;                          // 0x4
    uint32_t command_register;                              // 0x8
    uint32_t ecp_interrupt_source_register;                 // 0xc
    uint32_t ecp_status_register;                           // 0x10
    uint32_t ecp_interrupt_mask_register;                   // 0x14
    uint32_t ecp_internal_sram[136];                        // 0x18
    uint8_t  reserved_0x238[456];
} BCM_ECP; // Space: 1024 bytes

typedef struct
{
    uint32_t ecp_cfg_reg;                                   // 0x0
    uint32_t ecp_ctrl_reg;                                  // 0x4
    uint32_t ecp_cmd_reg;                                   // 0x8
    uint32_t ecp_intrpt_reg;                                // 0xc
    uint32_t ecp_status_reg;                                // 0x10
    uint32_t ecp_intrpt_mask_reg;                           // 0x14
    uint32_t ecp_sram[153];                                 // 0x18
    uint8_t  reserved_0x27c[388];
} BCM_ECRSA_ECP; // Space: 1024 bytes

typedef struct
{
    uint32_t hash_configuration;                            // 0x0
    uint32_t hash_control;                                  // 0x4
    uint32_t hash_command;                                  // 0x8
    uint32_t hash_status;                                   // 0xc
    uint32_t incoming_message_segment_size_word;            // 0x10
    uint8_t  reserved_0x14[4];
    uint32_t total_message_size_low_word;                   // 0x18
    uint32_t total_message_size_high_word;                  // 0x1c
    uint32_t hash_digest_and_context_a;                     // 0x20
    uint32_t hash_digest_and_context_b;                     // 0x24
    uint32_t hash_digest_and_context_c;                     // 0x28
    uint32_t hash_digest_and_context_d;                     // 0x2c
    uint32_t hash_digest_and_context_e;                     // 0x30
    uint32_t hash_digest_and_context_f;                     // 0x34
    uint32_t hash_digest_and_context_g;                     // 0x38
    uint32_t hash_digest_and_context_h;                     // 0x3c
    uint32_t hash_digest_and_context_a_high_word;           // 0x40
    uint32_t hash_digest_and_context_b_high_word;           // 0x44
    uint32_t hash_digest_and_context_c_high_word;           // 0x48
    uint32_t hash_digest_and_context_d_high_word;           // 0x4c
    uint32_t hash_digest_and_context_e_high_word;           // 0x50
    uint32_t hash_digest_and_context_f_high_word;           // 0x54
    uint32_t hash_digest_and_context_g_high_word;           // 0x58
    uint32_t hash_digest_and_context_h_high_word;           // 0x5c
    uint32_t hmac_contexto_a;                               // 0x60
    uint32_t hmac_contexto_b;                               // 0x64
    uint32_t hmac_contexto_c;                               // 0x68
    uint32_t hmac_contexto_d;                               // 0x6c
    uint32_t hmac_contexto_e;                               // 0x70
    uint32_t hmac_contexto_f;                               // 0x74
    uint32_t hmac_contexto_g;                               // 0x78
    uint32_t hmac_contexto_h;                               // 0x7c
    uint32_t hmac_contexto_a_high_word;                     // 0x80
    uint32_t hmac_contexto_b_high_word;                     // 0x84
    uint32_t hmac_contexto_c_high_word;                     // 0x88
    uint32_t hmac_contexto_d_high_word;                     // 0x8c
    uint32_t hmac_contexto_e_high_word;                     // 0x90
    uint32_t hmac_contexto_f_high_word;                     // 0x94
    uint32_t hmac_contexto_g_high_word;                     // 0x98
    uint32_t hmac_contexto_h_high_word;                     // 0x9c
    uint32_t hmac_key_length;                               // 0xa0
    uint32_t hmac_key_0;                                    // 0xa4
    uint32_t hmac_key_1;                                    // 0xa8
    uint32_t hmac_key_2;                                    // 0xac
    uint32_t hmac_key_3;                                    // 0xb0
    uint32_t hmac_key_4;                                    // 0xb4
    uint32_t hmac_key_5;                                    // 0xb8
    uint32_t hmac_key_6;                                    // 0xbc
    uint32_t hmac_key_7;                                    // 0xc0
    uint32_t hmac_key_8;                                    // 0xc4
    uint32_t hmac_key_9;                                    // 0xc8
    uint32_t hmac_key_10;                                   // 0xcc
    uint32_t hmac_key_11;                                   // 0xd0
    uint32_t hmac_key_12;                                   // 0xd4
    uint32_t hmac_key_13;                                   // 0xd8
    uint32_t hmac_key_14;                                   // 0xdc
    uint32_t hmac_key_15;                                   // 0xe0
    uint32_t hmac_key_16;                                   // 0xe4
    uint32_t hmac_key_17;                                   // 0xe8
    uint32_t hmac_key_18;                                   // 0xec
    uint32_t hmac_key_19;                                   // 0xf0
    uint32_t hmac_key_20;                                   // 0xf4
    uint32_t hmac_key_21;                                   // 0xf8
    uint32_t hmac_key_22;                                   // 0xfc
    uint32_t hmac_key_23;                                   // 0x100
    uint32_t hmac_key_24;                                   // 0x104
    uint32_t hmac_key_25;                                   // 0x108
    uint32_t hmac_key_26;                                   // 0x10c
    uint32_t hmac_key_27;                                   // 0x110
    uint32_t hmac_key_28;                                   // 0x114
    uint32_t hmac_key_29;                                   // 0x118
    uint32_t hmac_key_30;                                   // 0x11c
    uint32_t hmac_key_31;                                   // 0x120
    uint8_t  reserved_0x124[732];
} BCM_HASH; // Space: 1024 bytes

typedef struct
{
    uint32_t access_control_for_scratch_pad;                // 0x0
    uint32_t access_status_for_scratch_pad;                 // 0x4
    uint32_t write_address_for_scratch_pad_data_access;     // 0x8
    uint32_t write_data_for_scratch_pad_for_data_access;    // 0xc
    uint32_t read_address_for_scratch_pad_data_access;      // 0x10
    uint32_t read_data_for_scratch_pad_for_data_access;     // 0x14
    uint32_t scratch_pad_set_write_address;                 // 0x18
    uint32_t scratch_pad_set_read_address;                  // 0x1c
    uint8_t  reserved_0x20[992];
} BCM_SCRATCH_PAD; // Space: 1024 bytes

typedef struct
{
    uint32_t zmodp_configuration_register;                  // 0x0
    uint32_t zmodp_control_register;                        // 0x4
    uint32_t zmodp_command_register;                        // 0x8
    uint32_t zmodp_status_register;                         // 0xc
    uint32_t zmodp_interrupt_sources_register;              // 0x10
    uint32_t zmodp_interrupt_sources_enable_register;       // 0x14
    uint32_t zmodp_register_array[195];                     // 0x18
    uint8_t  reserved_0x324[220];
} BCM_ZMODP; // Space: 1024 bytes

typedef struct
{
    uint32_t zmp_configuration_register;                    // 0x0
    uint32_t zmp_control_register;                          // 0x4
    uint32_t zmp_command_register;                          // 0x8
    uint32_t zmp_status_register;                           // 0xc
    uint32_t zmp_interrupt_source_register;                 // 0x10
    uint32_t zmp_interrupt_source_enable_register;          // 0x14
    uint32_t zmp_internal_memory[195];                      // 0x18
    uint8_t  reserved_0x324[220];
} BCM_ECRSA_ZMP; // Space: 1024 bytes

typedef struct
{
    uint32_t monotonic_counter_control_register;            // 0x0
    uint32_t monotonic_counter_command_register;            // 0x4
    uint32_t monotonic_counter_status_register;             // 0x8
    uint32_t mct_interrupt_source_register;                 // 0xc
    uint32_t mct_interruption_mask_register;                // 0x10
    uint32_t monotonic_counter_value_word_0;                // 0x14
    uint32_t monotonic_counter_value_word_1;                // 0x18
    uint32_t monotonic_counter_value_word_2;                // 0x1c
    uint32_t monotonic_counter_value_word_3;                // 0x20
    uint32_t monotonic_counter_value_word_4;                // 0x24
    uint8_t  reserved_0x28[984];
} BCM_MCT; // Space: 1024 bytes

typedef struct
{
    uint32_t control;                                       // 0x0
    uint32_t otp_interrupt;                                 // 0x4
    uint32_t otp_interrupt_mask;                            // 0x8
    uint8_t  reserved_0xc[4];
    uint32_t otp_access_0;                                  // 0x10
    uint32_t otp_access_1;                                  // 0x14
    uint32_t otp_access_2;                                  // 0x18
    uint32_t otp_access_3;                                  // 0x1c
    uint32_t otp_access_4;                                  // 0x20
    uint32_t otp_access_5;                                  // 0x24
    uint32_t otp_access_6;                                  // 0x28
    uint32_t otp_access_7;                                  // 0x2c
    uint32_t otp_access_8;                                  // 0x30
    uint32_t otp_test_ctrl;                                 // 0x34
    uint32_t test_efuse_optimize;                           // 0x38
    uint32_t user_register_0;                               // 0x3c
    uint8_t  reserved_0x40[4];
    uint32_t match_code;                                    // 0x44
    uint32_t voltage_control;                               // 0x48
    uint8_t  reserved_0x4c[948];
} BCM_OTP_EFUSE; // Space: 1024 bytes

typedef struct
{
    uint32_t ebg_control_register;                          // 0x0
    uint32_t ebg_entropy_value_register;                    // 0x4
    uint8_t  reserved_0x8[1016];
} BCM_EBG; // Space: 1024 bytes

typedef struct
{
    uint32_t reserved[64];
    uint32_t rng_imr;
    uint32_t rng_isr;
    uint32_t rng_icr;
    uint32_t trng_config;
    uint32_t trng_valid;
    uint32_t ehr_data[6];
    uint32_t rng_source_enable;
    uint32_t sample_cnt1;
    uint32_t autocorr_statistic;
    uint32_t trng_debug_control;
    uint32_t reserved1;
    uint32_t trng_sw_reset;
    uint32_t reserved2[29];
    uint32_t trng_busy;
    uint32_t rst_bits_counter;
    uint32_t reserved3[8];
    uint32_t rng_bist_cntr[3];
} BCM_EBG_EXT;

typedef struct
{
    uint32_t core_memory_timing_control;                    // 0x0
    uint32_t bcm_monitor_module_select;                     // 0x4
    uint32_t bcm_monitor_signal_select;                     // 0x8
    uint32_t bcm_monitor;                                   // 0xc
    uint32_t processor_memory_timing_control;               // 0x10
    uint32_t power_control;                                 // 0x14
    uint8_t  reserved_0x18[8];
    uint32_t leakage_control_core;                          // 0x20
    uint32_t leakage_control_processor;                     // 0x24
    uint32_t bist_control_core;                             // 0x28
    uint32_t bist_control_processor;                        // 0x2c
    uint8_t  reserved_0x30[976];
} BCM_RCTRL; // Space: 1024 bytes

typedef struct
{
    uint32_t ahb_slave_control_register;                    // 0x0
    uint32_t ahb_slave_interrupt_register;                  // 0x4
    uint32_t ahb_slave_interrupt_mask;                      // 0x8
    uint32_t apb_bus_error_address;                         // 0xc
    uint8_t  reserved_0x10[1008];
} BCM_SAHB; // Space: 1024 bytes

typedef struct
{
    uint32_t apb_access[128];                               // 0x0
    uint32_t apb_base_address;                              // 0x200
    uint32_t apb_interface_select;                          // 0x204
    uint32_t apb_interrupt_status;                          // 0x208
    uint32_t apb_interrupt_mask;                            // 0x20c
    uint32_t apb_reset;                                     // 0x210
    uint8_t  reserved_0x214[492];
} BCM_APB; // Space: 1024 bytes

typedef struct
{
    uint32_t aes_cfg_reg;                                   // 0x0
    uint32_t aes_ctrl_reg;                                  // 0x4
    uint32_t aes_cmd_reg;                                   // 0x8
    uint32_t aes_status_reg;                                // 0xc
    uint32_t aes_intrpt_reg;                                // 0x10
    uint32_t aes_intrpt_src_en_reg;                         // 0x14
    uint32_t aes_stream_size;                               // 0x18
    uint8_t  reserved_0x1c[60];
    uint32_t key2_word0;                                    // 0x58
    uint32_t key2_word1;                                    // 0x5c
    uint32_t key2_word2;                                    // 0x60
    uint32_t key2_word3;                                    // 0x64
    uint32_t key2_word4;                                    // 0x68
    uint32_t key2_word5;                                    // 0x6c
    uint32_t key2_word6;                                    // 0x70
    uint32_t key2_word7;                                    // 0x74
    uint32_t key1_word0;                                    // 0x78
    uint32_t key1_word1;                                    // 0x7c
    uint32_t key1_word2;                                    // 0x80
    uint32_t key1_word3;                                    // 0x84
    uint32_t key1_word4;                                    // 0x88
    uint32_t key1_word5;                                    // 0x8c
    uint32_t key1_word6;                                    // 0x90
    uint32_t key1_word7;                                    // 0x94
    uint32_t iv_word0;                                      // 0x98
    uint32_t iv_word1;                                      // 0x9c
    uint32_t iv_word2;                                      // 0xa0
    uint32_t iv_word3;                                      // 0xa4
    uint32_t engine_select_reg;                             // 0xa8
    uint32_t aes_sector_size;                               // 0xac
    uint32_t lba_word0;                                     // 0xb0
    uint32_t lba_word1;                                     // 0xb4
    uint8_t  reserved_0xb8[840];
} BCM_AES2; // Space: 1024 bytes

typedef struct
{
    uint32_t ghash_cfg;                                     // 0x0
    uint32_t ghash_ctrl;                                    // 0x4
    uint32_t ghash_cmd;                                     // 0x8
    uint32_t ghash_status;                                  // 0xc
    uint32_t ghash_intrpt;                                  // 0x10
    uint32_t ghash_intrpt_src_en;                           // 0x14
    uint32_t ghash_stream_size;                             // 0x18
    uint8_t  reserved_0x1c[4];
    uint32_t key_word0;                                     // 0x20
    uint32_t key_word1;                                     // 0x24
    uint32_t key_word2;                                     // 0x28
    uint32_t key_word3;                                     // 0x2c
    uint32_t answer_word0;                                  // 0x30
    uint32_t answer_word1;                                  // 0x34
    uint32_t answer_word2;                                  // 0x38
    uint32_t answer_word3;                                  // 0x3c
    uint8_t  reserved_0x40[960];
} BCM_GHASH; // Space: 1024 bytes

#endif
