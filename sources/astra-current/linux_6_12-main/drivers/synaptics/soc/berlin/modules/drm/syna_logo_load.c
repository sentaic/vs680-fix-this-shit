// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 Synaptics Incorporated
 *
 */

#include <linux/io.h>
#include <linux/module.h>
#include <linux/string.h>

#include "drm_syna_drv.h"
#include "vpp_mem.h"
#include "syna_vpp.h"
#include "vpp_api.h"
#include "syna_drm_priv.h"

typedef struct _FL_INFO_ {
	unsigned long long addr;
	unsigned int wid;
	unsigned int hgt;
	unsigned int stride;
	unsigned int size;
	unsigned int valid;
} FL_INFO;

static FL_INFO fl_info[MAX_CRTC];

/* Module parameter for bootlogo info from U-Boot */
static char *logo_info;
module_param(logo_info, charp, 0444);
MODULE_PARM_DESC(logo_info,
	"Bootlogo info: addr@wxh-stride[,addr@wxh-stride]");

/*
 * Parse and validate bootlogo information from U-Boot
 * synatx format: #address@#widthx#height-#stride[,#address@#widthx#height-#stride]
 * Example format: 0x8000000@1920x1080-7680,0x9000000@1280x720-5120
 *
 * sscanf returns the number of successfully parsed values:
 *   - ret >= 4: First display valid (addr, width, height, stride)
 *   - ret >= 8: Both displays valid (4 values × 2 displays)
 */
static bool bootlogo_is_available(int disp_id)
{
	int ret;

	if (disp_id < 0 || disp_id > 1 || !logo_info)
		return false;

	if (!fl_info[0].addr) {
		ret = sscanf(logo_info, "%llx@%xx%x-%x,%llx@%xx%x-%x",
				&fl_info[0].addr, &fl_info[0].wid,
				&fl_info[0].hgt,  &fl_info[0].stride,
				&fl_info[1].addr, &fl_info[1].wid,
				&fl_info[1].hgt,  &fl_info[1].stride);

		fl_info[0].valid = ret >= 4 ? 1 : 0;
		fl_info[1].valid = ret >= 8 ? 1 : 0;

		if (fl_info[0].valid)
			fl_info[0].size = fl_info[0].hgt * fl_info[0].stride;
		if (fl_info[1].valid)
			fl_info[1].size = fl_info[1].hgt * fl_info[1].stride;
	}

	return fl_info[disp_id].valid;
}

int syna_load_uboot_logo(VPP_MEM_LIST *vpp_mem_list,
			 VPP_MEM *vpp_mem_handle,
			 int disp_id,
			 VPP_WIN *vpp_res_info)
{
	void *uboot_logo_virt;
	int ret;

	ret = bootlogo_is_available(disp_id);
	if (!ret) {
		pr_err("VPP: No boot logo available for display %d\n", disp_id);
		return -ENOENT;
	}

	if (!vpp_mem_list || !vpp_mem_handle) {
		pr_err("VPP: Invalid parameters for logo loading\n");
		return -EINVAL;
	}

	/* Map U-Boot logo physical address to virtual address */
	uboot_logo_virt = memremap(fl_info[disp_id].addr, fl_info[disp_id].size, MEMREMAP_WB);
	if (IS_ERR_OR_NULL(uboot_logo_virt)) {
		pr_err("VPP: Failed to remap U-Boot logo memory at 0x%llx (size: 0x%x)\n",
			(u64)fl_info[disp_id].addr, fl_info[disp_id].size);
		return -1;
	}

	/* Allocate VPP memory for logo buffer */
	memset(vpp_mem_handle, 0, sizeof(VPP_MEM));
	vpp_mem_handle->size = VPP_SHM_4K_ALIGN_ROUNDUP(fl_info[disp_id].size);
	ret = VPP_MEM_AllocateMemory(vpp_mem_list, VPP_MEM_TYPE_DMA, vpp_mem_handle, 0);
	if (ret) {
		pr_err("VPP: Failed to alloc mem for display W[%d]H[%d]\n",
			fl_info[disp_id].wid, fl_info[disp_id].hgt);
		ret = -ENOMEM;
		goto out1;
	}

	//copy logo into new allocated kernel buffer
	memcpy(vpp_mem_handle->k_addr, uboot_logo_virt, vpp_mem_handle->size);

	vpp_res_info->width = fl_info[disp_id].wid;
	vpp_res_info->height = fl_info[disp_id].hgt;

	pr_debug("VPP: Loaded U-Boot logo: addr=0x%llx size=%u wxh=%ux%u\n",
		(u64)fl_info[disp_id].addr, fl_info[disp_id].size,
		fl_info[disp_id].wid, fl_info[disp_id].hgt);

out1:
	memunmap(uboot_logo_virt);

	return 0;
}