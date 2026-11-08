/* SPDX-License-Identifier: GPL-2.0
 * Copyright (C) 2026 Synaptics Incorporated
 *
 */

#ifndef _TEE_CA_OVP_H_
#define _TEE_CA_OVP_H_

#include <linux/types.h>

#define	OVP_UNINITIALIZED_VALUE	0xdeadbeef
#define	TAOVP_PASS_SHMSIZE	(1024 * 4)
#define OVPD_INSTID		0
#define INSTID_MASK		0x00FF0000
#define CMDID_MASK		0x0000FFFF
#define INSTID_POS		16
#define CREATE_CMD_ID(cmd_id, inst_id)	\
		(((((inst_id)) << INSTID_POS) & INSTID_MASK) | (((cmd_id)) & CMDID_MASK))
#define GETINST_ID(cmd_id) ((((cmd_id)) & INSTID_MASK) >> 16)
#define GETCMD_ID(cmd_id)  (((cmd_id)) & CMDID_MASK)

#define OVP_SHM_4K_ALIGN_SIZE 4096
#define OVP_SHM_4K_ALIGN_ROUNDUP(size)  (((size) + OVP_SHM_4K_ALIGN_SIZE - 1) & \
						(~(OVP_SHM_4K_ALIGN_SIZE - 1)))

/* enum for OVP commands */
typedef enum {
	OVP_CREATE,
	OVP_DESTROY,
	OVP_GETFRAMESWAIT,
	OVP_PASSSHM,
	OVP_GET_CLR_INTR_STS,
	OVP_SUSPEND,
	OVP_RESUME,
} OVP_CMD_ID;

/* enum for VPP shm ID */
enum {
	/* ID for push frame shared memory */
	OVP_SHM_PUSHFRAME,
	/* ID for process frame shared memory */
	OVP_SHM_PROCESSFRAME,
	/* ID for release buffer shared memory */
	OVP_SHM_RELEASEBUFF
};

/**
 * @m_uch_buff_id: Buffer Id to identify client this buffer belongs to
 * @m_uch_output_whole_frame: Output as a frame or interpolated field
 * @m_uch_output_one_field: Output single/two frames/fields
 * @m_uch_in_bit_depth: Bit-depth of input data (bits per pixel-8/10)
 * @m_uch_out_bit_depth: Bit Depth of generated data.
 * @m_uch_priority: Priority of this client
 * @pst_frame_desc: Input Frame descriptors.
 * @m_ui_error_code: Error code of the processed frame
 */
struct ovp_msg_header {
	u8 m_uch_buff_id;
	u8 m_uch_output_whole_frame;
	u8 m_uch_output_one_field;
	u8 m_uch_in_bit_depth;
	u8 m_uch_out_bit_depth;
	u8 m_uch_priority;
	void *pst_frame_desc;
	int m_ui_error_code;
};

/**
 * @user_data: User-defined data, not used by the video decoder
 * @internal_data: Reserved. For video decoder internal use only.
 * @rsvd1: Reserved. For video decoder internal use only.
 * @base_addr: Base address of the frame buffer. (Physical address)
 * @base_addr_virtual: Base address of the frame buffer. (Virtual Address)
 * @size: buffer size
 * @rsvd4: Reserved. For video decoder internal use only.
 * @id: Buffer ID assigned by the buffer allocator, not used by the video decoder.
 * @sn: Reserved. For video decoder internal use only.
 * @rsvd5: Reserved. For video decoder internal use only.
 * @rsvd6: Reserved. For video decoder internal use only.
 * @used_flag: (FLAG_USE_FOR_REF = 0x1, FLAG_USE_FOR_DIS = 0x2)
 * @base_addr_uv: Base address of the uv frame buffer. (Physical address)
 * @base_addr_uv_virtual: Base address of the uv frame buffer. (Virtual Address)
 * @size_uv: uv buffer size
 * @tctx_addr: Base address of tctx buffer address (from bg7)
 * @tctx_addr_virtual: virtual address of tctx buffer address
 * @tctx_buf_size: size of tctx buffer
 * @meta_addr: Base address of meta data address, offset from base
 * @meta_buf_size: size of meta data
 * @meta_addr_uv: Base address of meta data address, offset from base
 * @meta_buf_uv_size: size of meta data
 * @base_addr_ch1:  Base address of the frame buffer for channel 1. (Physical address)
 * @base_addr_virtual_ch1: Base address of the frame buffer for channel 1. (Virtual Address)
 * @size_ch1: buffer size for channel 1
 * @base_addr_uv_ch1: Base address of the uv frame buffer for channel 1. (Physical address)
 * @base_addr_uv_virtual_ch1: Base address of the uv frame buffer for channel 1. (Virtual Address)
 * @size_uv_ch1: uv buffer size for channel 1
 */
struct ovp_vid_buf_desc_t {
	void           *user_data;
	void           *internal_data;
	unsigned int    rsvd1;
	unsigned int    base_addr;
	void           *base_addr_virtual;
	unsigned int    size;
	unsigned int    rsvd4;
	unsigned int    id;
	unsigned int    sn;
	unsigned int    rsvd5;
	void           *rsvd6;
	unsigned int    used_flag;
	unsigned int    base_addr_uv;
	void           *base_addr_uv_virtual;
	unsigned int    size_uv;
	unsigned int    tctx_addr;
	void           *tctx_addr_virtual;
	unsigned int    tctx_buf_size;
	unsigned int    meta_addr;
	unsigned int    meta_buf_size;
	unsigned int    meta_addr_uv;
	unsigned int    meta_buf_uv_size;
	/* for multi channel case */
	unsigned int    base_addr_ch1;
	void           *base_addr_virtual_ch1;
	unsigned int    size_ch1;
	unsigned int    base_addr_uv_ch1;
	void           *base_addr_uv_virtual_ch1;
	unsigned int    size_uv_ch1;
};

int syna_ovpd_ca_initialize(void);
void syna_ovpd_ca_deinitialize(void);
int syna_ovpd_ca_push_frame(struct ovp_msg_header *pst_ovp_msg_header);
int syna_ovpd_ca_process_frame(struct ovp_msg_header *pst_ovp_msg_header, u32 ui_intr_sts);
int syna_ovpd_ca_release_buffer(u32 ui_buff_id, struct ovp_msg_header *pst_ovp_msg_header);
int syna_ovpd_ca_get_no_of_frames_waiting(u32 ui_buf_id, u32 *ptr_ui_frames_waiting);
int syna_ovpd_ca_get_clr_intr_sts(u32 *ptr_ui_intr_sts);
int syna_ovpd_ca_create(unsigned int ui_shm_PA, unsigned int ui_shm_size);
int syna_ovpd_ca_destroy(void);
int syna_ovpd_ca_suspend(bool optimize);
int syna_ovpd_ca_resume(bool optimize);
#endif /* _TEE_CA_OVP_H_ */
