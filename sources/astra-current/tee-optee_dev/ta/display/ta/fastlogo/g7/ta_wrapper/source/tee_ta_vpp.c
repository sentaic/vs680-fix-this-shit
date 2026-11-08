/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2025 Synaptics Incorporated */

////////////////////////////////////////////////////////////////////////////////
//! \file tee_ta_vpp.c
//!
//! \brief Implementation of VPP commands for TrustZone.
//!
//! Purpose: This is used for controlling access to VPP API in TrustZone.
//!
//!     Version    Date                     Author
//!     V 1.00,    Nov 12 2013,             Jiankang Yu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#include <trace.h>
#include "tee_ta_vpp.h"
#include "tee_ta_vpp_be.h"
#include "vpp_obj.h"
#include "vpp_be_hdmitx.h"
#include "vpp_be.h"

#define TA_OUTCTRL_UUID {0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x03, 0xfc}}
TEE_UUID output_control_ta_uuid=TA_OUTCTRL_UUID;
//////////////////////////////////////////////////////
//   VPP module pipeline connection configuration
//   from input ports to output ports for different
//   profiles
//////////////////////////////////////////////////////
/* input plane->channel connection, copy from disp_soc_map.c*/
/* TODO: VPP_SW_BRINGUP: Make all this configurations user configurable */
const INT32 gVinPortConfig[] = {
    /* PLANE_MAIN   */ CHAN_MAIN,
    /* PLANE_PIP    */ CHAN_PIP,
    /* PLANE_GFX1   */ CHAN_GFX1,
    /* PLANE_GFX2   */ CHAN_GFX2,
    /* PLANE_AUX   */  CHAN_AUX,
    /* PLANE_OVP_EL */ CHAN_OVP_EL,
    /* PLANE_VMX    */ CHAN_VMX,
};

const INT32 gDVConfig[] = {
    /* CHAN_MAIN   */ CPCB_1,
    /* CHAN_PIP    */ CPCB_1,
    /* CHAN_GFX1   */ CPCB_1,
    /* CHAN_GFX2   */ CPCB_INVALID,
    /* CHAN_AUX    */ CPCB_INVALID,
    /* CHAN_OVP_EL */ CPCB_1,
    /* CHAN_VMX     */CPCB_1,
};

const INT32 gDVBypassConfig[] = {
    /* CHAN_MAIN   */ CPCB_1,
    /* CHAN_PIP    */ -1,
    /* CHAN_GFX1   */ -1,
};

const INT32 gZorderConfig[] = {
    /* CHAN_MAIN   */ CPCB_ZORDER_1,
    /* CHAN_PIP    */ CPCB_ZORDER_2,
    /* CHAN_GFX1   */ CPCB_ZORDER_3,
    /* CHAN_GFX2   */ CPCB_ZORDER_INVALID,
    /* CHAN_AUX    */ CPCB_ZORDER_INVALID,
    /* CHAN_OVP_EL */ CPCB_ZORDER_INVALID,
    /* CHAN_VMX    */ CPCB_ZORDER_INVALID,
};

const INT32 gVoutPortConfig[] = {
    /* VOUT_HDMI   */ CPCB_1,
    /* VOUT_HD     */ CPCB_1,
    /* VOUT_SD     */ CPCB_1,
    /* VOUT_DSI    */ CPCB_1,
};

//TODO: VPP_SW_BRINGUP: Check this
const INT32 gVoutPortBypassConfig[] = {
    /* VOUT_HDMI   */ CPCB_1,
    /* VOUT_HD     */ -1,
    /* VOUT_SD     */ -1,
};

const INT gVppFeatureCfg[MAX_NUM_FEATURE_CFG] = { VPP_FEATURE_HDMITX };

/* ***************************************************
 * Create a new instance for the trusted application *
 * ***************************************************/
#include "../../../../../../../../../../tools/optee-port/hardware-capabilities/display-dma.c"
#include "../../../../../../../../../../tools/optee-port/hardware-capabilities/display-ddc-pins.c"

TEE_Result TA_EXPORT TA_CreateEntryPoint(void)
{
    TEE_Result result =TEE_SUCCESS;
#ifdef TZ_3_0
    result = VPP_TA_AddMemMap();
#endif
    VPP_TA_IncCreateInstanceCount();
    return result;
}

/* *************************************************
 * Destory the instance of the trusted application *
 * *************************************************/
void TA_EXPORT TA_DestroyEntryPoint(void)
{
    return;
}

/* ********************************************************************
 * open a session with the trusted application when a client requests *
 * ********************************************************************/
TEE_Result TA_EXPORT TA_OpenSessionEntryPoint(uint32_t paramTypes,
                                        TEE_Param params[4],
                                        void **sessionContext)
{
#ifdef ENABLE_BG4_NON_SUPPORTED_INTERFACE
    MV_VPPOBJ_FetchOtpConfig();
#endif
    return TEE_SUCCESS;
}

/* *****************************************************
 * Close a client session with the trusted application *
 * *****************************************************/
void TA_EXPORT TA_CloseSessionEntryPoint(void *sessionContext)
{
    return;
}

/* *****************************************
 * Invoke a command within a given session *
 * *****************************************/
TEE_Result TA_EXPORT TA_InvokeCommandEntryPoint(void *sessionContext,
                                          uint32_t commandID,
                                          uint32_t paramTypes,
                                          TEE_Param params[4])
{
    UINT32 VPPBase = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_VPP_REG_BASE;
    INT handle;
    TEE_Result res = TEE_SUCCESS;
    VPP_WIN win;
    VPP_WIN *pWin;
    VPP_WIN_ATTR attr;
    VPP_WIN_ATTR *pAttr;
    VPP_INIT_PARM vpp_init_parm;
    VBUF_INFO *vbuf_info;
#ifdef TZ_3_0
    int instanceID=0;
    instanceID=GETINST_ID(commandID);
    commandID = GETCMD_ID(commandID);
#endif
#if defined (HDMI2X_FEATURES) || defined (CONFIG_HDCP2X_SUPPORT)
    unsigned int Size;
#endif

#ifdef VPP_ENABLE_FRAME_CAPTURE
    VPP_CAPTURE_FRAME_SETTING Cap_Frm;
#endif
#ifdef CONFIG_FASTLOGO_ENABLE_DEBUG_LOG_COMMAND
    VPPAPI_DBG_INFO("%s, cmd:%d, para0a=%x, para0b=%x, para1a=%x, para1b=%x,\n",
        __FUNCTION__, commandID, params[0].value.a, params[0].value.b
        , params[1].value.a, params[1].value.b);
#endif

    /* 返回当前 TA 缓存的显示状态。 */
    if (commandID == 0x7f00) {
        uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
            TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);
        if (paramTypes != expected)
            return TEE_ERROR_BAD_PARAMETERS;
        if (!g_vpp || !g_vpp->vpp_objs[0])
            return TEE_ERROR_BAD_STATE;
        VPP_OBJ *object = g_vpp->vpp_objs[0];
        params[0].value.a = object->status;
        params[0].value.b = object->plane[PLANE_GFX1].status;
        params[1].value.a = object->dv[CPCB_1].vbi_num;
        params[1].value.b = object->dv[CPCB_1].status;
        return TEE_SUCCESS;
    }

    if (commandID == 0x7f07 || commandID == 0x7f08) {
        if (paramTypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
                TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE))
            return TEE_ERROR_BAD_PARAMETERS;
        return display_ddc_pins(params, commandID == 0x7f08);
    }

    if (commandID == 0x7f0a) {
        UINT32 values[3] = {0};
        if (paramTypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
                TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE))
            return TEE_ERROR_BAD_PARAMETERS;
        if (!g_vpp || !g_vpp->vpp_objs[0]) return TEE_ERROR_BAD_STATE;
        params[1].value.a = BE_HDMITX_GetDebugColor(g_vpp->vpp_objs[0]->be.hdmiTx, values);
        params[0].value.a = values[0];
        params[0].value.b = values[1];
        params[1].value.b = values[2];
        return TEE_SUCCESS;
    }

    if (commandID == 0x7f09) {
        if (paramTypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
                TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE) ||
            params[0].value.a > 0xffffffu || params[0].value.b > 1)
            return TEE_ERROR_BAD_PARAMETERS;
        if (!g_vpp || !g_vpp->vpp_objs[0]) return TEE_ERROR_BAD_STATE;
        params[1].value.a = BE_HDMITX_DebugColor(g_vpp->vpp_objs[0]->be.hdmiTx,
            params[0].value.a, params[0].value.b);
        return TEE_SUCCESS;
    }

    if (commandID == 0x7f01) {
        TEE_Param state[4] = {0};
        uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
            TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);
        if (paramTypes != expected)
            return TEE_ERROR_BAD_PARAMETERS;
        if (g_vpp && g_vpp->vpp_objs[0] && g_vpp->vpp_objs[0]->be.hdmiTx) {
            params[0].value.a = BE_HDMITX_DebugColor(g_vpp->vpp_objs[0]->be.hdmiTx, 0, FALSE);
            if (params[0].value.a) return TEE_SUCCESS;
        }
        res = loop_display_dma_command(0x7f1b, state);
        if (res != TEE_SUCCESS)
            return res;
        params[0].value.a = MV_VPP_ReleaseOwnedPool();
        return TEE_SUCCESS;
    }

    if (commandID >= 0x7f03 && commandID <= 0x7f06) {
        BE_HDMITX_DATA *tx;
        UINT32 values[4];
        if (!g_vpp || !g_vpp->vpp_objs[0] || !g_vpp->vpp_objs[0]->be.hdmiTx)
            return TEE_ERROR_BAD_STATE;
        tx = g_vpp->vpp_objs[0]->be.hdmiTx;
        if (commandID == 0x7f06) {
            if (paramTypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
                    TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE) ||
                !params[0].memref.buffer || params[0].memref.size != 18)
                return TEE_ERROR_BAD_PARAMETERS;
            TEE_MemFill(params[0].memref.buffer, 0, 18);
            params[1].value.a = BE_HDMITX_GetDdcConfig(tx, params[0].memref.buffer);
            return TEE_SUCCESS;
        }
        if (commandID == 0x7f05) {
            uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
                TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE);
            if (paramTypes != expected || !params[0].memref.buffer ||
                params[0].memref.size != VPP_SIZE_OF_EDID_BLK ||
                params[1].value.a > 1 || params[1].value.b > 1)
                return TEE_ERROR_BAD_PARAMETERS;
            TEE_MemFill(params[0].memref.buffer, 0, VPP_SIZE_OF_EDID_BLK);
            UINT32 bytes_read = 0;
            if (params[1].value.b) {
                UINT8 *bytes = params[0].memref.buffer;
                for (; bytes_read < 8; bytes_read++) {
                    params[2].value.a = BE_HDMITX_ReadEdidByte(tx,
                        params[1].value.a * VPP_SIZE_OF_EDID_BLK + bytes_read,
                        &bytes[bytes_read]);
                    if (params[2].value.a) break;
                }
            } else {
                params[2].value.a = BE_HDMITX_ReadEdidBlock(tx, params[1].value.a,
                    params[0].memref.buffer);
                if (!params[2].value.a) bytes_read = VPP_SIZE_OF_EDID_BLK;
            }
            if (BE_HDMITX_GetDdcStatus(tx, values) != MV_VPP_OK)
                return TEE_ERROR_BAD_STATE;
            params[2].value.b = (values[0] & 255u) | ((values[1] & 255u) << 8) |
                (bytes_read << 16);
            return TEE_SUCCESS;
        }
        if (commandID == 0x7f04) {
            uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
                TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);
            if (paramTypes != expected || !params[0].memref.buffer ||
                params[0].memref.size != sizeof(VPP_HDMI_RAW_EDID))
                return TEE_ERROR_BAD_PARAMETERS;
            params[1].value.a = VPP_BE_GetRawEdid(g_vpp->vpp_objs[0], params[0].memref.buffer);
            return TEE_SUCCESS;
        }
        if (paramTypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
            TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE))
            return TEE_ERROR_BAD_PARAMETERS;
        if (BE_HDMITX_GetLinkStatus(tx, values)) return TEE_ERROR_BAD_STATE;
        params[0].value.a = values[0]; params[0].value.b = values[1];
        params[1].value.a = values[2]; params[1].value.b = values[3];
        params[2].value.a = (!!tx->hdmiHpdControlOff) | (!!tx->hdmiConfig.enableRxsenseCheck << 1) |
                            (!!tx->rxConnected << 2) | (!!tx->edidInfo.validEdid << 3);
        params[2].value.b = tx->sinkConnChangeCount;
        return TEE_SUCCESS;
    }

    if (commandID == 0x7f02) {
        uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
            TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);
        UINT32 values[4];
        if (paramTypes != expected)
            return TEE_ERROR_BAD_PARAMETERS;
        if (!g_vpp || !g_vpp->vpp_objs[0])
            return TEE_ERROR_BAD_STATE;
        if (BE_HDMITX_GetVideoPathStatus(g_vpp->vpp_objs[0]->be.hdmiTx, values))
            return TEE_ERROR_BAD_STATE;
        params[0].value.a = values[0];
        params[0].value.b = values[1];
        params[1].value.a = values[2];
        params[1].value.b = values[3];
        return TEE_SUCCESS;
    }

    switch (commandID) {
        case VPP_INIT:
        memset(&vpp_init_parm, 0, sizeof(VPP_INIT_PARM));
        /*set default value to vpp_init_parm*/
        vpp_init_parm.iHDMIEnable = 1;
        vpp_init_parm.iVdacEnable = 1;
        /*get vpp_init_parm*/
        vpp_init_parm.iHDMIEnable = ((params[0].value.a == 1)? 1 : 0);
        vpp_init_parm.iVdacEnable = ((params[0].value.b == 1)? 1 : 0);
#if defined (TZ_3_0) && defined (VPP_ENABLE_INTERNAL_MEM_MGR)
        vpp_init_parm.phShm = params[2].value.a;
        vpp_init_parm.uiShmSize=params[2].value.b;
#endif
        params[1].value.a = MV_VPP_Init(&vpp_init_parm);
#ifdef TZ_3_0
        params[1].value.b = vpp_init_parm.g_vpp;
#ifdef VPP_ENABLE_INTERNAL_MEM_MGR
        params[0].value.b = vpp_init_parm.gMemhandle;
#endif
#endif
        break;

#ifdef TZ_3_0
        case VPP_INITIATE_VPPS:
        params[1].value.a = MV_VPP_INITVPPS(instanceID, params[0].value.a,params[0].value.b);
        break;
#endif
        case VPP_AVIORESET:
        params[0].value.a = MV_VPPOBJ_AVIO_Reset();
        break;

        case VPP_RESET:
        params[0].value.a = MV_VPPOBJ_Reset(0);
        break;

        case VPP_CREATE: {
        TEE_Param pins[4] = {0};
        res = display_ddc_pins(pins, 1);
        if (res != TEE_SUCCESS) return res;
        params[0].value.a = MV_VPPOBJ_Create(VPPBase,
                                             &handle);
        break;
        }
        case VPP_DESTROY:
        params[0].value.a = MV_VPPOBJ_Destroy(0);
        break;

        case VPP_PASSSHM:
        params[3].value.b = MV_VPPOBJ_BindSHM(params);
        break;

        case VPP_PASSPHY:
        params[3].value.b = MV_VPPOBJ_BindSHM_PHY(params);
        break;

#ifdef CONFIG_VPP_FASTLOGO_PUSHPAR_SUPPORT
        case VPP_PASSPAR:
        params[3].value.b = MV_VPPOBJ_BindSHM_PAR(params);
        break;
#endif //CONFIG_VPP_FASTLOGO_PUSHPAR_SUPPORT

        case VPP_CONFIG:
        params[0].value.a = MV_VPPOBJ_Config(0, gVinPortConfig,
                                gDVConfig, gZorderConfig, gVoutPortConfig, gVppFeatureCfg);
        break;

        case VPP_SETRES:
        params[2].value.a = MV_VPPOBJ_SetCPCBOutputResolution(0,
                                params[0].value.a, params[0].value.b,
                                params[1].value.a);
        break;

        case VPP_SETREFWIN:
        win.x = params[0].value.b;
        win.y = params[1].value.a;
        win.width = params[1].value.b;
        win.height = params[2].value.a;
        params[3].value.a = MV_VPPOBJ_SetRefWindow(0, params[0].value.a, &win);
        break;

        case VPP_SETREFWINFROMISR:
        win.x = params[0].value.b;
        win.y = params[1].value.a;
        win.width = params[1].value.b;
        win.height = params[2].value.a;
        params[3].value.a = MV_VPPOBJ_SetRefWindowFromISR(0, params[0].value.a, &win);
        break;

        case VPP_OPENDISPWIN:
        win.x = params[0].value.b;
        win.y = params[1].value.a;
        win.width = params[1].value.b;
        win.height = params[2].value.a;
        attr.bgcolor = params[2].value.b;
        attr.alpha= (params[3].value.a & 0xFFFF);
        attr.globalAlphaFlag = VPP_ATTR_PARAM_GET_GLOBALPHA(params[0].value.a);

        pAttr = ((attr.bgcolor == -1) && (attr.alpha == -1))? NULL : &attr;
        params[3].value.b = MV_VPPOBJ_OpenDispWindow(0, VPP_ATTR_PARAM_GET_PLANEID(params[0].value.a),
                                                     &win, pAttr);
        break;

        case VPP_CHANGEDISPWIN:
        win.x = params[0].value.b;
        win.y = params[1].value.a;
        win.width = params[1].value.b;
        win.height = params[2].value.a;
        attr.bgcolor = params[2].value.b;
        attr.alpha= (params[3].value.a & 0xFFFF);
        attr.globalAlphaFlag = VPP_ATTR_PARAM_GET_GLOBALPHA(params[0].value.a);
        pWin = ((win.x == -1)&&(win.y == -1)&&(win.width == -1)&&(win.height == -1))? NULL : &win;
        pAttr = ((attr.bgcolor == -1) && (attr.alpha == -1))? NULL : &attr;
        params[3].value.b = MV_VPPOBJ_ChangeDispWindow(0, VPP_ATTR_PARAM_GET_PLANEID(params[0].value.a),
                                                     pWin, pAttr);
        break;

        case VPP_CHANGEDISPWINFROMISR:
        win.x = params[0].value.b;
        win.y = params[1].value.a;
        win.width = params[1].value.b;
        win.height = params[2].value.a;
        attr.bgcolor = params[2].value.b;
        attr.alpha= (params[3].value.a & 0xFFFF);
        attr.globalAlphaFlag = VPP_ATTR_PARAM_GET_GLOBALPHA(params[0].value.a);
        pWin = ((win.x == -1)&&(win.y == -1)&&(win.width == -1)&&(win.height == -1))? NULL : &win;
        pAttr = ((attr.bgcolor == -1) && (attr.alpha == -1))? NULL : &attr;
        params[3].value.b = MV_VPPOBJ_ChangeDispWindowFromISR(0, VPP_ATTR_PARAM_GET_PLANEID(params[0].value.a),
                                                     pWin, pAttr);
        break;

        case VPP_GETDISPWINDOW:
        win.x = win.y = win.width = win.height = 0;
        attr.alpha = attr.bgcolor = 0;
        params[0].value.b = MV_VPPOBJ_GetDispWindow(0, params[0].value.a, &win, &attr);
        params[1].value.a = win.x;
        params[1].value.b = win.y;
        params[2].value.a = win.width;
        params[2].value.b = win.height;
        params[3].value.a = attr.bgcolor;
        params[3].value.b = attr.alpha;
        break;

        case VPP_GETREFWINDOW:
        win.x = win.y = win.width = win.height = 0;
        params[0].value.b = MV_VPPOBJ_GetRefWindow(0, params[0].value.a, &win);
        params[1].value.a = win.x;
        params[1].value.b = win.y;
        params[2].value.a = win.width;
        params[2].value.b = win.height;
        break;

        case VPP_HDMISETVIDFMT:
        params[2].value.a = MV_VPPOBJ_SetHdmiVideoFmt(0, params[0].value.a,
                                params[0].value.b, params[1].value.a);
        break;

        case VPP_HDMISETTXCTRL:
        params[1].value.a = MV_VPPOBJ_SetHdmiTxControl(0, params[0].value.a);
        break;

        case VPP_HDMIHPDCONTROL:
        params[1].value.a = MV_VPPOBJ_HdmiHpdControlOff(0, params[0].value.a);
        break;
#ifdef CONFIG_FASTLOGO_ENABLE_UNSUPPORTED_INTERFACE
        case VPP_HDMISETTMDSMUXSEL:
        params[0].value.b = MV_VPPOBJ_SetHdmiTmdsMuxSel(0, params[0].value.a);
        break;
#endif

        case VPP_HANDLEINT:
        VPP_ISR_Handler(params[0].value.a, params[0].value.b);
        //return success
        params[1].value.a = 0;
        break;

        case VPP_RECYCLEFRAME:
        vbuf_info = (VBUF_INFO *)MV_VPPOBJ_RecycleFrames(0, params[0].value.a);
        if (vbuf_info) {
#ifndef CONFIG_VPP_RECYLE_LOCAL_VBUF_IMMEDIATE
            params[1].value.a = vbuf_info->m_hDesc;
            VPP_FrameInfoPool_FreeFrameInfo((VBUF_INFO *)vbuf_info);
#else
            params[1].value.a = (UINT32)vbuf_info;
#endif
        } else {
            params[1].value.a = 0;
        }
        break;

        case VPP_SETDISPMODE:
        params[1].value.a = MV_VPPOBJ_SetDisplayMode(0, params[0].value.a,
                                                     params[0].value.b);
        break;
#ifdef CONFIG_FASTLOGO_ENABLE_UNSUPPORTED_INTERFACE
        case VPP_SETDISPMODEFROMISR:
        params[1].value.a = MV_VPPOBJ_SetDisplayModeFromISR(0, params[0].value.a,
                                                     params[0].value.b);
        break;
#endif
        case VPP_CLOSEDISPWINDOW:
        params[1].value.a = MV_VPPOBJ_CloseDispWindow(0, params[0].value.a);
        break;

        case VPP_SETPLANEMUTE:
        params[1].value.a = MV_VPPOBJ_SetPlaneMute(0, params[0].value.a, params[0].value.b);
        break;

        case VPP_ENABLEAUTOPUSH:
        {
            params[1].value.a = MV_VPPOBJ_EnableAutoPush(0, params[0].value.a);
        }
        break;

        case VPP_GETCPCBOUTPIXELCLOCK:
        {
            params[1].value.b = MV_VPPOBJ_GetCPCBOutputPixelClock(0, params[0].value.a, (INT *)&params[1].value.a);
        }
        break;

        case VPP_STOP:
        {
            params[1].value.b = MV_VPPOBJ_Stop(0);
        }
        break;

        case VPP_HDMIENABLEAUDIOFMT:
        {
            params[0].value.b = MV_VPPOBJ_EnableHdmiAudioFmt(0, params[0].value.a);
            break;
        }

        case VPP_GETHPDSTATUS:
        params[0].value.b = MV_VPPOBJ_GetHPDStatus(0, (INT *)&params[0].value.a);
        break;

        case VPP_GET_BLOCK_STATUS:
        {
            params[1].value.b = MV_VPPOBJ_GetBlockStatus(0, params[0].value.b, params[0].value.a, (INT *)&params[1].value.a);
        }
        break;

        case VPP_SUSPEND:
        {
            params[0].value.a = MV_VPPOBJ_Suspend(0, params[0].value.b);
        }
        break;

        case VPP_RESUME:
        {
            params[0].value.a = MV_VPPOBJ_Resume(0, params[0].value.b);
        }
        break;

        default:
        res = TEE_ERROR_NOT_SUPPORTED;
        break;
    }
    return res;
}

#ifndef CONFIG_OPTEE
TA_DEFINE_BEGIN
    .uuid              = TA_FASTLOGO_UUID,
    .name              = "fastlogo",
    .singleInstance    = false,
    .multiSession      = false,
    .instanceKeepAlive = false,
TA_DEFINE_END

#endif
