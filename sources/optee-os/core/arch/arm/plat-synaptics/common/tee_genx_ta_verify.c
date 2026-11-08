// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <util.h>
#include <trace.h>
#include <types_ext.h>
#include <kernel/tee_ta_manager.h>
#include <kernel/cache_helpers.h>
#include <string.h>
#include <malloc.h>

#include "bcm_genx_verify.h"
#include "image.h"
#include "ta_info.h"
#include "otp_prop.h"
#include "hal_soc.h"

#include "tee_ta_perm_ctrl.h"

//#include "linker.h"
//#include "tee_ta_drm_client.h"
//#include <schedule.h>
//#include "tz_mgr.h"
//#include "compress.h"
//#include "syscall.h"
//#include "hal_plat.h"
//#include "tee_ta_perm_ctrl.h"
//#include "cpu.h"
#if 0
int free_library(soinfo *si);

extern struct ta_info ta30_info;
extern int hwrand(uint8_t *randBuffer, uint32_t randLength);

#define SYNA_UUID_MIN 0x03e8
#define SYNA_UUID_MAX 0x0467
static TEE_UUID syna_ta_uuid_mask = {0x1316a183, 0x894d, 0x43fe, {0x98, \
						0x93, 0xbb, 0x94, 0x6a, 0xe1, 0xff, 0xff}};

static bool is_syna_ta_uuid(const TEE_UUID *uuid)
{
	uint16_t id = 0;

	if(!uuid)
		return false;

	if (!memcmp(uuid, &syna_ta_uuid_mask, sizeof(TEE_UUID) - 2)) {
		id = uuid->clockSeqAndNode[7] | (uuid->clockSeqAndNode[6] << 8);
		switch (id) {
		case SYNA_UUID_MIN ... SYNA_UUID_MAX:
			return true;
		default:
			return false;
		}
	}

	return false;
}

static TEE_Result _get_ta_img_uuid_info(soinfo *si, void *ta_info_buffer)
{
	TEE_TADesc tad;
	ta_export_info_t *ta_exp_info = ta_info_buffer;

	if (si == NULL)
		return TZ_ERROR_BAD_PARAMETERS;

	Elf_Sym *sym = soinfo_lookup(si, "__ThisTA");
	if (!sym) {
		EMSG("can't find TA definition\n");
		return TZ_ERROR_BAD_FORMAT;
	}
	memset(&tad, 0, sizeof(tad));
	tad.ta = (void *)(si->base + sym->st_value);

	if (ta_exp_info != NULL) {
		ta_img_info_t *ta_img_info = &ta_exp_info->ta_img_info;
		memcpy(ta_img_info->vendor_uuid, &tad.ta->uuid, 16);
	}

	return TEE_SUCCESS;
}

static void * get_reloc_base(void)
{
#ifdef CONFIG_ENABLE_PIE
	int i = 0;
	uint32_t random_value = 0;
	unsigned char *p = NULL;

	while (1) {
		i++;
		if (i > 2)
			reset_soc();

		hwrand((uint8_t *)(&random_value), sizeof(uint32_t));
		p = (unsigned char *)(&random_value);
		if ((*p == *(p+1) && *p == *(p+2) && *p == *(p+3)) || random_value == 0)
			continue;
		else
			break;
	}

	return (void *)(uintptr_t)((random_value % 256 + 1) << PAGE_SHIFT);
#else
	return (void *)USER_TASK_START;
#endif
}

static TEE_Result register_ta(soinfo *si, void *ta_all_permission,
		void *ta_permission, struct ta_header *ta_header)
{
	TEE_TADesc tad;
	TEE_Result result = TEE_ERROR_GENERIC;

	if (si == NULL)
		return TZ_ERROR_BAD_PARAMETERS;

	Elf_Sym *sym = soinfo_lookup(si, "__ThisTA");
	if (!sym) {
		EMSG("can't find TA definition\n");
		result = TZ_ERROR_BAD_FORMAT;
		return result;
	}
	memset(&tad, 0, sizeof(tad));
	tad.ta = (void *)(si->base + sym->st_value);
	tad.ta_user = (void *)(si->vbase + sym->st_value);
	tad.size = si->size;
	tad.kBase = (void *)si->base;
	tad.uBase = (void *)si->vbase;
	tad.pBase = (void *)kern_virt_to_phys((unsigned long)tad.kBase);
	tad.si = si;
	sym = soinfo_lookup(si, "_start");
	if (!sym) {
		EMSG("can't find TA entry\n");
		return TZ_ERROR_BAD_FORMAT;
	}
	tad.uEntry = (void *)(si->vbase + sym->st_value);

	if (is_syna_ta_uuid(&tad.ta->uuid)){
		if(ta_header && (ta_header->ta_vendor_gid != 1))
			reset_soc();
	}

	if (ta_all_permission && ta_permission) {
		result = ta_perm_set_group_info(
				(ta_group_header_t *)ta_all_permission,
				(ta_group_header_t *)ta_permission,
				&tad);
		if (result != TEE_SUCCESS) {
			EMSG("fail to set group\n");
			return result;
		}
	}

	result = TEE_TARegister(&tad, NULL);
	if (result != TEE_SUCCESS && result != TEE_ERROR_ACCESS_CONFLICT){
		EMSG("Rgister TA failed with result = %08x\n", result);
		return result;
	} else {
		trace("Load ta %s baseaddr= 0x%08x size= 0x%08x.\n", si->name,
			si->base, si->size);
	}

	return result;
}
#endif
/*
 * params[0]: nw. TA code binary start (encrypted).
 *            Currently, it's the TEE_TA structure.
 *
 * params[1]:	1 to register TA, 0 to get TA info
 */
TEE_Result verify_ta(const TEE_UUID * uuid __maybe_unused, const uint8_t *nw, size_t nw_size, void *sw, size_t *sw_ta_size)
{

	TEE_Result result = TEE_ERROR_GENERIC;
	void *buffer = NULL;
	unsigned int usize = 0;
	void *pta_all_permission = NULL;
	void *pta_permission = NULL;
	void *ta;
	struct ta_header *ta_header = NULL;
	struct ta_cert_header *cert_header = NULL;
	struct ta_root_cert_header *root_cert_header = NULL;
	unsigned int ta_size = 0;
	unsigned int all_permission_size = 0;
	unsigned int subset_permission_size = 0;
	unsigned int otp_dolby_vision_enable;

	usize = nw_size;

	if (usize < sizeof(struct ta_header)) {
		EMSG("input params[0].memref.size(0x%lx) less than ta_header size\n", nw_size);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	buffer = sw;

	/* copying data to secure memory */
	memcpy((char *)buffer, nw, usize);

	// get pta_all_permission size from root cert, and set root_cert_header address as
	// the pta_all_permission decryption address
	root_cert_header = (struct ta_root_cert_header *)buffer;
	if (root_cert_header->image_type != IMAGE_TYPE_TA_ROOT_CERT) {
		EMSG("invaild root cert header.\n");
		result = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}
	assert(root_cert_header->length >= sizeof(sizeof(*root_cert_header)));
	all_permission_size = root_cert_header->length - sizeof(*root_cert_header);
	pta_all_permission = (void *)root_cert_header;

	// get pta_permission size from ta cert, and set ta_cert_header address as
	// the pta_permission decryption address

	cert_header = (struct ta_cert_header *)((uintptr_t)buffer + root_cert_header->length);
	if (cert_header->image_type != IMAGE_TYPE_TA_CERT) {
		EMSG("invaild ta cert header.\n");
		result = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}
	assert(cert_header->length >= sizeof(sizeof(*cert_header)));
	subset_permission_size = cert_header->length - sizeof(*cert_header);
	pta_permission = (void *)cert_header;

	// get ta size from ta header, and set ta header address as
	// the ta decryption address
	ta_header = (struct ta_header *)((uintptr_t)cert_header + cert_header->length);
	if (ta_header->image_type != IMAGE_TYPE_TA) {
		EMSG("invaild ta header.\n");
		result = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}
	assert(ta_header->length >= sizeof(sizeof(*ta_header)));
	ta_size = ta_header->length - sizeof(*ta_header);
	ta = ta_header->data;

	/*
	 * If TA Type == 0, proceed to asking BCM to verify the TA
	 * If TA Type == 1, check Dolby_Vision_Disabled OTP
	 * 		If Dolby_Vision_Disabled = 0, proceed to asking BCM to verify the TA
	 * 		If Dolby_Vision_Disabled = 1, stop processing the TA and return error to REE caller
	 * TA Type == 2 or others are reserved for future use. In case Dolby Audio also requires the feature,
	 *		Dolby Audio TA could be assigned to TA Type == 2.
	 * TA Type == 0xFFFFFFFF, DEBUG TA, if Production_Image_Flag == 1, stop processing the TA and return error to REE caller
	 */
	switch (ta_header->ta_type) {
	case TA_TYPE_NORMAL:
		// do nothing, proceed to asking BCM to verify the TA
		break;
	case TA_TYPE_DOLBY_VISION:
		result = soc_get_otp_prop(OTP_DOLBY_VISION_ENABLE, &otp_dolby_vision_enable, 4);
		if (result != TEE_SUCCESS) {
			EMSG("fail to get OTP_DOLBY_VISION_ENABLE, result = %08x\n", result);
			goto out;
		}
		if (1 != otp_dolby_vision_enable) {
			EMSG("Dolby Vision is disabled, stop processing the TA!\n");
			result = TEE_ERROR_ACCESS_DENIED;
			goto out;
		}
		break;
	case TA_TYPE_DEBUG:
		if (ta_header->production_image_flag == 1) {
			EMSG("Debug TA is not supported in production release.\n");
			result = TEE_ERROR_BAD_PARAMETERS;
			goto out;
		}
		break;
	default:
		EMSG("unsupported ta type 0x%x.\n", ta_header->ta_type);
		result = TEE_ERROR_BAD_PARAMETERS;
		goto out;
	}

		/*verify ta image */
		result = verify_ta_image(buffer, usize, ta, ta_size,
					pta_all_permission, all_permission_size,
					pta_permission, subset_permission_size);

		if (result != TEE_SUCCESS) {
			EMSG("verify_ta_image fail result = %08x\n", result);
			goto out;
		}

#ifdef CFG_TA_PERM_CHECK
		result = ta_perm_set_group_info(pta_all_permission, pta_permission, uuid);
		if (result != TEE_SUCCESS) {
			EMSG("ta_perm_set_group_info fail result = %08x\n", result);
			goto out;
		}
#endif
out:
	if (result != TEE_SUCCESS) {
		return TEE_ERROR_SECURITY;
	}

	if (buffer)
		memcpy(buffer, ta, ta_size);

	if (sw_ta_size)
		*sw_ta_size =  ta_size;
	return TEE_SUCCESS;
}
