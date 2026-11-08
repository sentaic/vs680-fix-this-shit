// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#ifndef _SZC_MST_H_
#define _SZC_MST_H_

#define MST_ID_ACPU0		   0
#define MST_ID_ACPU1		   1
#define MST_ID_2_unused		   2
#define MST_ID_3_unused		   3
#define MST_ID_SCPU_SPROT	   4
#define MST_ID_SM_ACRYPTO	   5
#define MST_ID_SM_OTHERS	   6
#define MST_ID_VIO			   7
#define MST_ID_AIO			   8
#define MST_ID_CIO			   9
#define MST_ID_10_unused	   10
#define MST_ID_11_unused	   11
#define MST_ID_12_unused	   12
#define MST_ID_NPU			   13
#define MST_ID_GPU			   14
#define MST_ID_MMU			   15
#define MST_ID_16_unused	   16
#define MST_ID_17_unused	   17
#define MST_ID_GE0			   18
#define MST_ID_GE1			   19
#define MST_ID_EMMC			   20
#define MST_ID_SDIO0		   21
#define MST_ID_SDIO1		   22
#define MST_ID_USB0			   23
#define MST_ID_USB1			   24
#define MST_ID_25_unused	   25
#define MST_ID_MTEST		   26
#define MST_ID_JTAG2AHB		   27
#define MST_ID_28_unused	   28
#define MST_ID_29_unused	   29
#define MST_ID_CORESIGHT	   30
#define MST_ID_PDMA			   31


/* refer to MasterGroup in soc.h */
enum {
	RF_MST_ACPU,
	RF_MST_SM_SPROT,
	RF_MST_SM_ACRYP,
	RF_MST_SM_OTHER,
	RF_MST_GE0,
	RF_MST_GE1,
	RF_MST_EMMC,
	RF_MST_SDIO0,
	RF_MST_SDIO1,
	RF_MST_USB0,
	RF_MST_USB1,
	RF_MST_MTEST,
	RF_MST_JTAG2AHB,
	RF_MST_CORESIGHT,
	RF_MST_PDMA,
	RF_MST_MAX,
};

enum {
	RF_GROUP0,
	RF_GROUP1,
	RF_GROUP2,
	RF_GROUP3,
	RF_GROUP4,
	RF_GROUP5,
	RF_GROUP6,
	RF_GROUP7,
	RF_GROUP_MAX,
};
#endif /* _SZC_MST_H_ */
