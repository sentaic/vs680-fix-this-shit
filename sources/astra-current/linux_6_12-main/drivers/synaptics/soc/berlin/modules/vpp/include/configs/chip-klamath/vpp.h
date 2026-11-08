// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2026 Synaptics Incorporated */

#ifndef _VPP_H_
#define _VPP_H_

#define SYNA_VPP_PRIMARY_INTR_NUM CPCB_2

// CPCB_1 is invalid for KLAMATH
#define SYNA_IS_VALID_CRTC_NDX(crtc_id)	((crtc_id == CPCB_2) ? true : false)
#define SYNA_GET_VIRT_CRTC_NDX(crtc_id)	CPCB_2

#endif //_VPP_H_
