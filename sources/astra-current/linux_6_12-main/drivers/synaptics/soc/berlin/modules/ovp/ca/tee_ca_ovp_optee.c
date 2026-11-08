// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2026 Synaptics Incorporated */

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/tee_drv.h>
#include <linux/interrupt.h>
#include <tee_client_api.h>

#include "avio_type.h"
#include "vpp_vbuf.h"
#include "vbuf.h"
#include "tee_ca_ovp.h"

struct ovp_ca_context {
	int initialized;
	struct tee_context *context;
};

struct ovp_ca_session {
	int initialized;
	bool mutex_initialized;
	u32 session;
	struct tee_shm *shm_msg;
	/* Protects shared memory access between concurrent API calls */
	struct mutex shm_mutex;
};

/* Mutex to protect initialization sequence */
static DEFINE_MUTEX(g_init_lock);

static struct ovp_ca_context g_ovp_ca_context = {0};
static struct ovp_ca_session ta_ovp_instance = {0};

static const uuid_t ta_ovp_uuid = UUID_INIT(0x1316a183, 0x894d, 0x43fe,
						0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1,
						0x03, 0xf4);

static int optee_ctx_match(struct tee_ioctl_version_data *ver, const void *data)
{
	return (ver->impl_id == TEE_IMPL_ID_OPTEE);
}

static int syna_ovpd_tee_alloc(u32 len, void **shm)
{
	struct tee_shm *shm_tmp;

	shm_tmp = tee_shm_alloc_kernel_buf(g_ovp_ca_context.context, len);
	if (IS_ERR(shm_tmp)) {
		pr_err("fail to allocate share memory: size 0x%x\n", len);
		return -ENOMEM;
	}
	shm_tmp->size = len;

	*shm = shm_tmp;

	return 0;
}

static int syna_ovpd_tee_release(void *shm)
{
	if (shm)
		tee_shm_free(shm);
	else
		pr_err("invalid shm %p\n", shm);

	return 0;
}

int syna_ovpd_ca_initialize(void)
{
	int ret;
	struct tee_param param[4];
	struct tee_ioctl_open_session_arg sess_arg;

	mutex_lock(&g_init_lock);
	if (g_ovp_ca_context.initialized) {
		mutex_unlock(&g_init_lock);
		return 0;
	}

	/* ========================================================================
	 *  [1] Connect to TEE
	 * ========================================================================
	 */
	g_ovp_ca_context.context =
	    tee_client_open_context(NULL, optee_ctx_match, NULL, NULL);
	if (IS_ERR(g_ovp_ca_context.context)) {
		pr_err("fail to initialize optee context\n");
		ret = -ENODEV;
		goto cleanup1;
	}

	g_ovp_ca_context.initialized = true;

	/* ========================================================================
	 *  [2] Load TA, If required during ampless boot.
	 * ========================================================================
	 */

	/* ========================================================================
	 * [3] Open session with TEE application
	 * ========================================================================
	 */
	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT;
	param[0].u.value.a = 0;
	param[0].u.value.b = 2;

	memset(&sess_arg, 0, sizeof(sess_arg));
	memcpy(sess_arg.uuid, ta_ovp_uuid.b, TEE_IOCTL_UUID_LEN);
	sess_arg.clnt_login = TEE_IOCTL_LOGIN_PUBLIC;
	sess_arg.num_params = 1;

	ret = tee_client_open_session(g_ovp_ca_context.context, &sess_arg, param);
	if (ret < 0 || sess_arg.ret != 0) {
		pr_err("tee_client_open_session failed with tee_ctx 0x%p code 0x%x origin 0x%x",
		       g_ovp_ca_context.context, ret, sess_arg.ret);
		/* OP-TEE not ready yet or transient communication failure.
		 * Treat as retryable so callers can defer probe or retry later.
		 */
		if (ret == -EIO || sess_arg.ret == TEEC_ERROR_COMMUNICATION)
			ret = -EAGAIN;
		else if (ret == 0)
			ret = -EINVAL;

		goto cleanup2;
	}

	ta_ovp_instance.session = sess_arg.session;

	ret = syna_ovpd_tee_alloc(TAOVP_PASS_SHMSIZE, (void **)&ta_ovp_instance.shm_msg);
	if (ret < 0) {
		pr_err("fail to alloc shm Msg for param passing %x", TAOVP_PASS_SHMSIZE);
		goto cleanup3;
	}

	if (!ta_ovp_instance.mutex_initialized) {
		mutex_init(&ta_ovp_instance.shm_mutex);
		ta_ovp_instance.mutex_initialized = true;
	}
	ta_ovp_instance.initialized = true;

	mutex_unlock(&g_init_lock);
	return 0;
cleanup3:
	tee_client_close_session(g_ovp_ca_context.context, ta_ovp_instance.session);
cleanup2:
	g_ovp_ca_context.initialized = false;
	tee_client_close_context(g_ovp_ca_context.context);
	g_ovp_ca_context.context = NULL;
cleanup1:
	mutex_unlock(&g_init_lock);
	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_initialize);

void syna_ovpd_ca_deinitialize(void)
{
	mutex_lock(&g_init_lock);
	if (!g_ovp_ca_context.initialized) {
		mutex_unlock(&g_init_lock);
		return;
	}

	g_ovp_ca_context.initialized = 0;

	if (ta_ovp_instance.initialized) {
		syna_ovpd_tee_release(ta_ovp_instance.shm_msg);
		tee_client_close_session(g_ovp_ca_context.context, ta_ovp_instance.session);
		ta_ovp_instance.initialized = false;
		ta_ovp_instance.mutex_initialized = false;
		memset(&ta_ovp_instance, 0, sizeof(ta_ovp_instance));
	}
	tee_client_close_context(g_ovp_ca_context.context);
	g_ovp_ca_context.context = NULL;
	mutex_unlock(&g_init_lock);
}
EXPORT_SYMBOL(syna_ovpd_ca_deinitialize);

static int invoke_command_helper(u32 *session_id, int command_id,
				 struct tee_param *param, int n_param)
{
	int result = 0;
	int cmd_id = command_id;

	/* Validate parameter count */
	if (n_param < 0 || n_param > 4) {
		pr_err("Invalid parameter count: %d\n", n_param);
		return -EINVAL;
	}

	command_id = CREATE_CMD_ID(cmd_id, OVPD_INSTID);

	if (g_ovp_ca_context.initialized) {
		struct tee_ioctl_invoke_arg arg;

		memset(&arg, 0, sizeof(arg));
		arg.func = command_id;
		arg.num_params = n_param;
		arg.session = *session_id;
		result = tee_client_invoke_func(g_ovp_ca_context.context, &arg, param);
		if (result < 0 || arg.ret != 0) {
			pr_err("fail to invoke command(%x): ret = %x,TEE err: 0x%x\n",
			       command_id, result, arg.ret);
			result = -EINVAL;
		}
	} else {
		result = -ENODEV;
	}

	return result;
}

int syna_ovpd_ca_create(unsigned int ui_shm_PA, unsigned int ui_shm_size)
{
	int ret;
	struct tee_param param[2];
	u32 *ptr_session;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	ptr_session = &ta_ovp_instance.session;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT;

	/* clear result */
	param[0].u.value.a = OVP_UNINITIALIZED_VALUE;

	param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT;
	param[1].u.value.a = ui_shm_PA;
	param[1].u.value.b = ui_shm_size;

	ret = invoke_command_helper(ptr_session, OVP_CREATE, param, 2);
	if (!ret)
		ret = param[0].u.value.b;

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_create);

int syna_ovpd_ca_destroy(void)
{
	int ret;
	struct tee_param param[1];
	u32 *ptr_session;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	ptr_session = &ta_ovp_instance.session;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT;

	/* clear result */
	param[0].u.value.a = OVP_UNINITIALIZED_VALUE;

	ret = invoke_command_helper(ptr_session, OVP_DESTROY, param, 1);
	if (!ret)
		ret = param[0].u.value.b;

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_destroy);

static int ovp_msg_to_params(struct ovp_msg_header *pst_ovp_msg_header, struct tee_param *param)
{
	int ret;
	u32 *ptr_session;
	struct tee_shm *shm;
	struct ovp_vid_buf_desc_t *ptr_desc;
	VBUF_INFO *ptr_vbuf_info;
	struct ovp_msg_header *pst_temp_msg_buf;
	void *va;

	if (!pst_ovp_msg_header || !param) {
		pr_err("%s: Bad args, pst_ovp_msg_header: %p, param:%p",
		       __func__, pst_ovp_msg_header, param);
		return -EINVAL;
	}

	ptr_session = &ta_ovp_instance.session;

	/* Read the frame descriptor and vbuf from the MsgHeader */
	if (pst_ovp_msg_header->pst_frame_desc) {
		ptr_desc = (struct ovp_vid_buf_desc_t *)(pst_ovp_msg_header->pst_frame_desc);
		ptr_vbuf_info = ptr_desc->user_data;
	} else {
		ptr_desc = NULL;
		ptr_vbuf_info = NULL;
	}

	mutex_lock(&ta_ovp_instance.shm_mutex);
	/* Use the shared mem for the OVP header */
	shm = ta_ovp_instance.shm_msg;
	if (IS_ERR(shm)) {
		mutex_unlock(&ta_ovp_instance.shm_mutex);
		return PTR_ERR(shm);
	}

	/* Get the virtual address to pass hdr back to caller */
	va = tee_shm_get_va(shm, 0);
	if (IS_ERR(va) || !va) {
		mutex_unlock(&ta_ovp_instance.shm_mutex);
		return IS_ERR(va) ? PTR_ERR(va) : -EINVAL;
	}
	memcpy(va, pst_ovp_msg_header, sizeof(struct ovp_msg_header));

	param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT;
	param[1].u.memref.shm = shm;
	param[1].u.memref.size = sizeof(struct ovp_msg_header);
	param[1].u.memref.shm_offs = 0;

	if (ptr_vbuf_info) {
		/* Non-null desc case: Use the SHM passed by Caller*/
		param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT;
		param[2].u.value.a = ptr_vbuf_info->pVppVbufInfo_phy;
		param[2].u.value.b = OVP_SHM_4K_ALIGN_ROUNDUP(sizeof(VPP_VBUF));
	}

	ret = invoke_command_helper(ptr_session, OVP_PASSSHM, param, 3);
	/* Check actual TA response if request success (ret = 0) */
	if (!ret)
		ret = param[0].u.value.b;

	/* Copy back the OVP header if TA response success */
	if (!ret) {
		/* Expected pst_ovp_msg_header is already in the passed shm (va) */
		pst_temp_msg_buf = (struct ovp_msg_header *)va;
		/* Preserve pst_frame_desc (set by caller) and copy other fields from TA */
		pst_ovp_msg_header->m_uch_buff_id = pst_temp_msg_buf->m_uch_buff_id;
		pst_ovp_msg_header->m_uch_output_whole_frame =
			pst_temp_msg_buf->m_uch_output_whole_frame;
		pst_ovp_msg_header->m_uch_output_one_field =
			pst_temp_msg_buf->m_uch_output_one_field;
		pst_ovp_msg_header->m_uch_in_bit_depth = pst_temp_msg_buf->m_uch_in_bit_depth;
		pst_ovp_msg_header->m_uch_out_bit_depth = pst_temp_msg_buf->m_uch_out_bit_depth;
		pst_ovp_msg_header->m_uch_priority = pst_temp_msg_buf->m_uch_priority;
		pst_ovp_msg_header->m_ui_error_code = pst_temp_msg_buf->m_ui_error_code;
	} else {
		pr_err("%s: fail to invoke command(%llu): ret = %x\n", __func__,
		       (unsigned long long)param[0].u.value.a, ret);
	}

	mutex_unlock(&ta_ovp_instance.shm_mutex);

	return ret;
}

int syna_ovpd_ca_push_frame(struct ovp_msg_header *pst_ovp_msg_header)
{
	int ret;
	struct tee_param param[3];
	struct tee_param *ptr_param;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT;
	param[0].u.value.a = OVP_SHM_PUSHFRAME;

	ptr_param = &param[0];
	ret = ovp_msg_to_params(pst_ovp_msg_header, ptr_param);

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_push_frame);

int syna_ovpd_ca_process_frame(struct ovp_msg_header *pst_ovp_msg_header, u32 ui_intr_sts)
{
	int ret;
	struct tee_param param[3];
	struct tee_param *ptr_param;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT;
	param[0].u.value.a = OVP_SHM_PROCESSFRAME;
	param[0].u.value.b = ui_intr_sts;

	ptr_param = &param[0];
	ret = ovp_msg_to_params(pst_ovp_msg_header, ptr_param);

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_process_frame);

int syna_ovpd_ca_release_buffer(u32 ui_buff_id, struct ovp_msg_header *pst_ovp_msg_header)
{
	int ret;
	struct tee_param param[3];
	struct tee_param *ptr_param;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT;
	param[0].u.value.a = OVP_SHM_RELEASEBUFF;
	param[0].u.value.b = ui_buff_id;

	ptr_param = &param[0];
	ret = ovp_msg_to_params(pst_ovp_msg_header, ptr_param);

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_release_buffer);

int syna_ovpd_ca_get_no_of_frames_waiting(u32 ui_buff_id, u32 *pui_frames_waiting)
{
	int ret;
	struct tee_param param[2];
	u32 *ptr_session;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	if (!pui_frames_waiting)
		return -EINVAL;

	ptr_session = &ta_ovp_instance.session;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT;
	param[0].u.value.a = ui_buff_id;

	param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT;

	ret = invoke_command_helper(ptr_session, OVP_GETFRAMESWAIT, param, 2);
	if (!ret)
		*pui_frames_waiting = param[0].u.value.b;

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_get_no_of_frames_waiting);

int syna_ovpd_ca_get_clr_intr_sts(u32 *ptr_ui_intrsts)
{
	int ret;
	struct tee_param param[1];
	u32 *ptr_session;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	if (!ptr_ui_intrsts)
		return -EINVAL;

	ptr_session = &ta_ovp_instance.session;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT;

	param[0].u.value.a = OVP_UNINITIALIZED_VALUE;
	param[0].u.value.b = OVP_UNINITIALIZED_VALUE;

	ret = invoke_command_helper(ptr_session, OVP_GET_CLR_INTR_STS, param, 1);
	if (!ret)
		*ptr_ui_intrsts = param[0].u.value.b;

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_get_clr_intr_sts);

int syna_ovpd_ca_suspend(bool optimize)
{
	int ret;
	struct tee_param param[1];
	u32 *ptr_session;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	ptr_session = &ta_ovp_instance.session;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT;

	/* clear result */
	param[0].u.value.a = OVP_UNINITIALIZED_VALUE;
	/* optimize Resume flag */
	param[0].u.value.b = optimize;

	ret = invoke_command_helper(ptr_session, OVP_SUSPEND, param, 1);
	if (!ret)
		ret = param[0].u.value.b;

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_suspend);

int syna_ovpd_ca_resume(bool optimize)
{
	int ret;
	struct tee_param param[1];
	u32 *ptr_session;

	if (!ta_ovp_instance.initialized)
		return -ENODEV;

	ptr_session = &ta_ovp_instance.session;

	memset(param, 0, sizeof(param));
	param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT;

	/* clear result */
	param[0].u.value.a = OVP_UNINITIALIZED_VALUE;
	/* optimize Resume flag */
	param[0].u.value.b = optimize;

	ret = invoke_command_helper(ptr_session, OVP_RESUME, param, 1);
	if (!ret)
		ret = param[0].u.value.b;

	return ret;
}
EXPORT_SYMBOL(syna_ovpd_ca_resume);
