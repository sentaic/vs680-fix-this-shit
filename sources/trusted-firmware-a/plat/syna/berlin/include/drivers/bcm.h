/*
 * Copyright (c) 2024, Synaptics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BCM_H__
#define __BCM_H__

#define CODETYPE_SYS_INIT			1
#define CODETYPE_MINILOADER			1
#define CODETYPE_TZK				4
#define CODETYPE_BOOTLOADER			5
#define CODETYPE_PT				5
#define CODETYPE_BLI				5
#define CODETYPE_KERNEL				5
#define CODETYPE_KEYSTORE			5

#define MAX_SECURE_BOOT_PARA_SIZE		1440

#define MAX_DDR_REG_NUM			2
#define HASH_LEN				32

struct ddr_reg_item
{
	unsigned int base;
	unsigned int size;
};

struct ddr_reg_group
{
	int ddr_reg_num;
	struct ddr_reg_item item_cfg[MAX_DDR_REG_NUM];
};

struct ddr_reg_info
{
	//0x01:ddr hash value; 0x02:ddr region
	unsigned int ddr_info_type;
	union {
		//128byte, now just used 32byte for sha256.
		unsigned char hash_value[HASH_LEN * MAX_DDR_REG_NUM];
		struct ddr_reg_item ddr_reg;
	} ddr_info_data;
	unsigned int reserve[15]; //make the size 64byte-aligned
};

struct scs_pub_info
{
	union {
		unsigned int data[16];
		struct {
			unsigned int miniloader_start_addr;
			unsigned int miniloader_size;
			unsigned int layout_sign_header_start_addr;
			unsigned int layout_sign_header_size;
			unsigned int size_of_SCS_total_area;
			unsigned int miniloader_loading_addr;
			unsigned int kernel_base;
			unsigned int kernel_length;
			unsigned int kernel_attr;
			unsigned int pci_win_lower_addr;
			unsigned int pci_win_upper_addr;
			unsigned int previlege_mode;
		}pub_info;
	}scs_data;
};

struct otp_srv_resp {
	uint32_t bitmap;
	uint8_t rkekid[8];
	uint8_t retrymap[8];
	uint8_t failmap[8];
};

#define virt_to_phy(x)	(x)

void bcm_suspend_init(void);
void bcm_suspend(void);
void bcm_resume(void);
int bcm_kernel_genx_store_hash(void *hash_buff, unsigned int size);
int bcm_kernel_store_hash(struct ddr_reg_info *ddr_reg,
				unsigned int *tee_sha_buf,
				unsigned char *tee_buf,
				unsigned int tee_size);
int bcm_calculate_and_store_hmac(struct ddr_reg_info *ddr_reg, unsigned int ddr_reg_num);
int bcm_verify(unsigned int type, unsigned long src, unsigned long dst);
int bcm_erom_verify_miniloader(unsigned long src, unsigned int size);
int bcm_erom_cfg_ddr_scramble_key(void);
int bcm_erom_clean_up(void);
int bcm_erom_query_tee_config(void *tee_buf_out, int size, void *scs_pub_info,
				int* warmboot);
int bcm_erom_check_ddr_hmac(struct ddr_reg_info *ddr_reg);
int bcm_erom_load_bcm_kernel(void);
void bcm_parse_mem_region(void *, void *);
int bcm_check_priviledge_mode(void *);
int bcm_hwrand(void *rand_addr, unsigned int rand_len);
int bcm_erom_load_usb_image(uint64_t image_type, uint64_t dest_addr,
			uint64_t file_name_addr, uint64_t file_name_len);
int bcm_erom_program_otp(uint64_t pphy_otp_layout, uint64_t size_opt_layout);
int bcm_otp_read_otp(uint32_t index, uint32_t *pdata, uint32_t *pmask);
int bcm_otp_write_otp(uint32_t index, uint32_t data, uint32_t mask);
int bcm_erom_read_public_otp(uint64_t otp_buff, uint64_t size);
int bcm_access_secure_sram(uint32_t type, uint32_t idx, uint64_t *pdata);
int bcm_cmd_handle(uint32_t cmd_id, uint32_t arg0, uint32_t arg1, uint32_t arg2,
			uint32_t arg3, uint32_t arg4 , uint32_t arg5);
bool bcm_sip_cmd_is_supported(uint32_t cmd_id);

#endif
