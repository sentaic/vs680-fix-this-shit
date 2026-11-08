/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright 2025 Synaptics Incorporated */

#ifndef __SYNA_V4L2_EXTERNTON_H
#define __SYNA_V4L2_EXTERNTON_H

#include <linux/types.h>
#include <linux/videodev2.h>

/*synaptics specific controls*/
#define SYNA_V4L2_CID_VENC_DS_W (V4L2_CID_USER_BASE + 0x1170)
#define SYNA_V4L2_CID_VENC_DS_H (V4L2_CID_USER_BASE + 0x1171)
#define SYNA_V4L2_CID_VENC_ENABLE_SIMULCAST (V4L2_CID_USER_BASE + 0x1172)

#define SYNA_V4L2_CID_VENC_METADATA (V4L2_CID_USER_BASE + 0x1173)
//enable encoder motion vector data
#define SYNA_V4L2_CID_VENC_ENABLE_METADATA_MV (V4L2_CID_USER_BASE + 0x1175)

#define MAX_MBS_PER_FRAME ((H1_MAX_W * H1_MAX_H) >> 8) //8160
#define MB_INFO_SIZE (sizeof(MVInfo_t)) //16bytes
#define MAX_MV_BUFFER_SIZE (MAX_MBS_PER_FRAME * MB_INFO_SIZE) //128kbytes

#ifndef V4L2_META_FMT_MV
#define V4L2_META_FMT_MV    v4l2_fourcc('M', 'O', 'V', 'E')
#endif

typedef struct __MVInfo_t {
    /* Byte 0: Motion vector metadata flags */
    __u8 mbType    : 4;   /* Macroblock type (bits 0-3, LSB)
                           * 0: I16x16_V
                           * 1: I16x16_H
                           * 2: I16x16_DC
                           * 3: I16x16_PLANE
                           * 4: I4x4
                           * 6: P16x16
                           * 7: P16x8
                           * 8: P8x16
                           * 9: P8x8
                           */
    __u8 chrIType  : 2;   /* Chroma intra pred type (bits 4-5) */
    __u8 refFrmIdx : 2;   /* Reference frame index[0-2] (bits 6-7, MSB) */
    /* Byte 1-3: Reserved for hw alignment */
    __u8 reserved;
    __u16 reserved2;
    /* Byte 4-7: Motion vector Y components (4 blocks) */
    __s8 mvY[4];
    /* Byte 8-15: Motion vector X components (4 blocks) */
    __s16 mvX[4];
} __attribute__((packed)) MVInfo_t;

#define SYNA_V4L2_CID_DEC_OPT_DOVI          (V4L2_CID_USER_BASE + 0x1200)
#define SYNA_V4L2_CID_DEC_OPT_DOVI_VES_MODE (V4L2_CID_USER_BASE + 0x1201)
#define SYNA_V4L2_CID_DEC_OPT_HDR10_PLUS    (V4L2_CID_USER_BASE + 0x1202)
#define SYNA_V4L2_CID_DEC_OPT_NO_REORDER    (V4L2_CID_USER_BASE + 0x1203)
#define SYNA_V4L2_CID_DEC_OPT_LOW_LATENCY   (V4L2_CID_USER_BASE + 0x1204)

enum DOVI_VES_MODE {
    SYNA_DOVI_VES_AUTO,
    SYNA_DOVI_VES_BL_ONLY,
};

#endif