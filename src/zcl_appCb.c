/********************************************************************************************************
 * @file    zcl_appCb.c
 *
 * @brief   This is the source file for zcl_appCb
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

/**********************************************************************
 * INCLUDES
 */
#include "app_main.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */



/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
#ifdef ZCL_READ
static void app_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void app_zclWriteReqCmd(uint8_t epId, uint16_t clusterId, zclWriteCmd_t *pWriteReqCmd);
static void app_zclWriteRspCmd(zclWriteRspCmd_t *pWriteRspCmd);
#endif
#ifdef ZCL_REPORT
static void app_zclCfgReportCmd(uint8_t endPoint, uint16_t clusterId, zclCfgReportCmd_t *pCfgReportCmd);
static void app_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void app_zclReportCmd(uint16_t clusterId, zclReportCmd_t *pReportCmd, aps_data_ind_t aps_data_ind);
#endif
static void app_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd);


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif

uint8_t count_no_service = 0;


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      app_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void app_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
//  printf("app_zclProcessIncomingMsg\n");

    uint16_t cluster = pInHdlrMsg->msg->indInfo.cluster_id;
    uint8_t endPoint = pInHdlrMsg->msg->indInfo.dst_ep;
    aps_data_ind_t aps_data_ind = pInHdlrMsg->msg->indInfo;

    switch(pInHdlrMsg->hdr.cmd)
    {
#ifdef ZCL_READ
        case ZCL_CMD_READ_RSP:
            app_zclReadRspCmd(pInHdlrMsg->attrCmd);
            break;
#endif
#ifdef ZCL_WRITE
        case ZCL_CMD_WRITE:
            app_zclWriteReqCmd(endPoint, pInHdlrMsg->msg->indInfo.cluster_id, pInHdlrMsg->attrCmd);
            break;
        case ZCL_CMD_WRITE_RSP:
            app_zclWriteRspCmd(pInHdlrMsg->attrCmd);
            break;
#endif
#ifdef ZCL_REPORT
        case ZCL_CMD_CONFIG_REPORT:
            app_zclCfgReportCmd(endPoint, cluster, pInHdlrMsg->attrCmd);
            break;
        case ZCL_CMD_CONFIG_REPORT_RSP:
            app_zclCfgReportRspCmd(pInHdlrMsg->attrCmd);
            break;
        case ZCL_CMD_REPORT:
            app_zclReportCmd(cluster, pInHdlrMsg->attrCmd, aps_data_ind);
            break;
#endif
        case ZCL_CMD_DEFAULT_RSP:
            app_zclDfltRspCmd(pInHdlrMsg->attrCmd);
            break;
        default:
            break;
    }
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      app_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd) {
//    printf("app_zclReadRspCmd\n");

    uint8_t numAttr = pReadRspCmd->numAttr;
    zclReadRspStatus_t *attrList = pReadRspCmd->attrList;
    uint32_t time_local;
    bool time_sent = false;

    for (uint8_t i = 0; i < numAttr; i++) {
        if (attrList[i].attrID == ZCL_ATTRID_LOCAL_TIME && attrList[i].status == ZCL_STA_SUCCESS) {
            time_local = attrList[i].data[0] & 0xff;
            time_local |= (attrList[i].data[1] << 8)  & 0x0000ffff;
            time_local |= (attrList[i].data[2] << 16) & 0x00ffffff;
            time_local |= (attrList[i].data[3] << 24) & 0xffffffff;
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_TIME, ZCL_ATTRID_LOCAL_TIME, (uint8_t*)&time_local);
            time_sent = true;
#if UART_PRINTF_MODE && DEBUG_TIME
            printf("Sync Local Time: %d\r\n", time_local+UNIX_TIME_CONST);
#endif
        }
    }

    if (time_sent) {
//        set_time_sent();
    }
}
#endif

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      app_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param
 *
 * @return  None
 */
static void app_zclWriteReqCmd(uint8_t epId, uint16_t clusterId, zclWriteCmd_t *pWriteReqCmd)
{

    uint8_t numAttr = pWriteReqCmd->numAttr;
    zclWriteRec_t *attr = pWriteReqCmd->attrList;
    bool save = false;
    uint8_t idx = epId - 1;
    zcl_onOffCfgAttr_t *pOnOffCfg = zcl_onOffCfgAttrsGet();
    pOnOffCfg += idx;

    printf("app_zclWriteReqCmd\r\n");

    if (clusterId == ZCL_CLUSTER_GEN_ON_OFF) {
        for (uint32_t i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_START_UP_ONOFF) {
                uint8_t startup = attr[i].attrData[0];
//                printf("startup: 0x%02x, ep: %d\r\n", startup, epId);
                relay_settings.startUpOnOff[idx] = startup;
                save = true;
            }
        }
    } else if (clusterId == ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG) {
        for (uint32_t i = 0; i < numAttr; i++) {
            if (attr[i].attrID == CUSTOM_ATTRID_SWITCH_TYPE) {
                uint8_t type = attr[i].attrData[0];
//                printf("type: 0x%02x\r\n", type);
                relay_settings.switchType[idx] = type;
                pOnOffCfg->switchType = type;
                if (type == ZCL_SWITCH_TYPE_MULTIFUNCTION) {
                    cmdOnOff_off(epId);
                    relay_settings.switch_decoupled[idx] = CUSTOM_SWITCH_DECOUPLED_ON;
                    pOnOffCfg->custom_decoupled = CUSTOM_SWITCH_DECOUPLED_ON;
                }
                save = true;
            } else if (attr[i].attrID == ZCL_ATTRID_SWITCH_ACTION) {
                uint8_t action = attr[i].attrData[0];
//                printf("action: 0x%02x, ep: %d\r\n", action, epId);
                relay_settings.switchActions[idx] = action;
                save = true;
            } else if (attr[i].attrID == CUSTOM_ATTRID_DECOUPLED) {
                uint8_t decoupled = attr[i].attrData[0];
//                printf("decoupled: 0x%02x\r\n", decoupled);
                cmdOnOff_off(epId);
                if (decoupled == CUSTOM_SWITCH_DECOUPLED_OFF && relay_settings.switchType[idx] == ZCL_SWITCH_TYPE_MULTIFUNCTION) {
                    decoupled = CUSTOM_SWITCH_DECOUPLED_ON;
                    pOnOffCfg->custom_decoupled = decoupled;
//                    app_forcedReport(epId, clusterId, CUSTOM_ATTRID_DECOUPLED);
                }
                relay_settings.switch_decoupled[idx] = decoupled;
                save = true;
            }
        }
    }

    if (save) relay_settings_save();

#ifdef ZCL_POLL_CTRL
    if(clusterId == ZCL_CLUSTER_GEN_POLL_CONTROL){
        for(int32_t i = 0; i < numAttr; i++){
            if(attr[i].attrID == ZCL_ATTRID_CHK_IN_INTERVAL){
                app_zclCheckInStart();
                return;
            }
        }
    }
#endif
}

/*********************************************************************
 * @fn      app_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclWriteRspCmd(zclWriteRspCmd_t *pWriteRspCmd)
{
//    printf("app_zclWriteRspCmd\n");

}
#endif


/*********************************************************************
 * @fn      app_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd)
{
//  printf("app_zclDfltRspCmd\n");
#ifdef ZCL_OTA
    if( (pDftRspCmd->commandID == ZCL_CMD_OTA_UPGRADE_END_REQ) &&
        (pDftRspCmd->statusCode == ZCL_STA_ABORT) ){
        if(zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_DOWNLOAD_COMPLETE){
            ota_upgradeAbort();
        }
    }
#endif
}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      app_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclCfgReportCmd(uint8_t endPoint, uint16_t clusterId, zclCfgReportCmd_t *pCfgReportCmd)
{
    //printf("app_zclCfgReportCmd\r\n");
    reportAttrTimerStop();
}

/*********************************************************************
 * @fn      app_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
//    printf("app_zclCfgReportRspCmd\n");

}

/*********************************************************************
 * @fn      app_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclReportCmd(uint16_t clusterId, zclReportCmd_t *pReportCmd, aps_data_ind_t aps_data_ind) {
//    printf("app_zclReportCmd\r\n");

//    uint8_t numAttr = pReportCmd->numAttr;
//    zclReport_t *attrList = pReportCmd->attrList;
//
//    uint8_t ret;
//    uint16_t addr = aps_data_ind.src_short_addr;
//
//    for (uint8_t i = 0; i < numAttr; i++) {
//        if (clusterId == ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT &&
//                attrList[i].dataType == ZCL_DATA_TYPE_INT16 &&
//                attrList[i].attrID == ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE) {
//            int16_t temp;
//
//            temp = attrList[i].attrData[0] & 0xFF;
//            temp |= (attrList[i].attrData[1] << 8) & 0xFFFF;
//
////            printf("temp: 0x%04x\r\n", (uint16_t)temp);
//
//            ret = bind_outsise_proc(addr, clusterId);
//
//            if (ret != OUTSIDE_SRC_CLUSTER_OK) {
//                continue;
//            }
//
//            app_set_remote_temperature(temp);
//            bind_remote_update_timer();
//        }
//    }

}
#endif

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      app_basicCb
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_basicCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{
    if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
        //Reset all the attributes of all its clusters to factory defaults
        //zcl_nv_attr_reset();
    }

    return ZCL_STA_SUCCESS;
}
#endif

#ifdef ZCL_IDENTIFY
int32_t app_zclIdentifyTimerCb(void *arg)
{
    if(g_zcl_identifyAttrs.identifyTime <= 0){
        light_blink_stop();

        identifyTimerEvt = NULL;
        return -1;
    }
    g_zcl_identifyAttrs.identifyTime--;
    return 0;
}

void app_zclIdentifyTimerStop(void)
{
    if(identifyTimerEvt){
        TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
    }
}

/*********************************************************************
 * @fn      app_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void app_zclIdentifyCmdHandler(uint8_t endpoint, uint16_t srcAddr, uint16_t identifyTime)
{
    g_zcl_identifyAttrs.identifyTime = identifyTime;

    if(identifyTime == 0){
        app_zclIdentifyTimerStop();
        light_blink_stop();
    }else{
        if(!identifyTimerEvt){
            light_blink_start(identifyTime, 500, 500);
            identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(app_zclIdentifyTimerCb, NULL, 1000);
        }
    }
}

/*********************************************************************
 * @fn      app_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void app_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
    uint8_t effectId = pTriggerEffect->effectId;
//  uint8_t effectVariant = pTriggerEffect->effectVariant;


    switch (effectId) {
        case IDENTIFY_EFFECT_BLINK:
            light_blink_start(1, 500, 500);
            break;
        case IDENTIFY_EFFECT_BREATHE:
            light_blink_start(15, 300, 700);
            break;
        case IDENTIFY_EFFECT_OKAY:
            light_blink_start(2, 250, 250);
            break;
        case IDENTIFY_EFFECT_CHANNEL_CHANGE:
            light_blink_start(1, 500, 7500);
            break;
        case IDENTIFY_EFFECT_FINISH_EFFECT:
            light_blink_start(1, 300, 700);
            break;
        case IDENTIFY_EFFECT_STOP_EFFECT:
            light_blink_stop();
            break;
        default:
            break;
    }
}

/*********************************************************************
 * @fn      app_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{
    if(pAddrInfo->dstEp == APP_ENDPOINT1){
        if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
            switch(cmdId){
                case ZCL_CMD_IDENTIFY:
                    app_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
                    break;
                case ZCL_CMD_TRIGGER_EFFECT:
                    app_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
                    break;
                default:
                    break;
            }
        }
    }

    return ZCL_STA_SUCCESS;
}
#endif

#ifdef ZCL_GROUP
/*********************************************************************
 * @fn      app_zclAddGroupRspCmdHandler
 *
 * @brief   Handler for ZCL add group response command.
 *
 * @param   pAddGroupRsp
 *
 * @return  None
 */
static void app_zclAddGroupRspCmdHandler(uint8_t ep, zcl_addGroupRsp_t *pAddGroupRsp) {

//    printf("app_zclAddGroupRspCmdHandler. ep: %d, status: %d, gid: %d\r\n", ep, pAddGroupRsp->status, pAddGroupRsp->groupId);
}

/*********************************************************************
 * @fn      app_zclViewGroupRspCmdHandler
 *
 * @brief   Handler for ZCL view group response command.
 *
 * @param   pViewGroupRsp
 *
 * @return  None
 */
static void app_zclViewGroupRspCmdHandler(zcl_viewGroupRsp_t *pViewGroupRsp)
{

}

/*********************************************************************
 * @fn      app_zclRemoveGroupRspCmdHandler
 *
 * @brief   Handler for ZCL remove group response command.
 *
 * @param   pRemoveGroupRsp
 *
 * @return  None
 */
static void app_zclRemoveGroupRspCmdHandler(zcl_removeGroupRsp_t *pRemoveGroupRsp)
{

}

/*********************************************************************
 * @fn      app_zclGetGroupMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get group membership response command.
 *
 * @param   pGetGroupMembershipRsp
 *
 * @return  None
 */
static void app_zclGetGroupMembershipRspCmdHandler(zcl_getGroupMembershipRsp_t *pGetGroupMembershipRsp)
{

}

/*********************************************************************
 * @fn      app_groupCb
 *
 * @brief   Handler for ZCL Group command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_groupCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload) {

//    printf("app_groupCb. ep: %d\r\n", pAddrInfo->dstEp);

	if(pAddrInfo->dstEp == APP_ENDPOINT1) {
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_GROUP_ADD_GROUP_RSP:
					app_zclAddGroupRspCmdHandler(pAddrInfo->dstEp, (zcl_addGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_VIEW_GROUP_RSP:
					app_zclViewGroupRspCmdHandler((zcl_viewGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_REMOVE_GROUP_RSP:
					app_zclRemoveGroupRspCmdHandler((zcl_removeGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_GET_MEMBERSHIP_RSP:
					app_zclGetGroupMembershipRspCmdHandler((zcl_getGroupMembershipRsp_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_GROUP */

#ifdef ZCL_SCENE
/*********************************************************************
 * @fn      app_zclAddSceneRspCmdHandler
 *
 * @brief   Handler for ZCL add scene response command.
 *
 * @param   cmdId
 * @param   pAddSceneRsp
 *
 * @return  None
 */
static void app_zclAddSceneRspCmdHandler(uint8_t cmdId, addSceneRsp_t *pAddSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclViewSceneRspCmdHandler
 *
 * @brief   Handler for ZCL view scene response command.
 *
 * @param   cmdId
 * @param   pViewSceneRsp
 *
 * @return  None
 */
static void app_zclViewSceneRspCmdHandler(uint8_t cmdId, viewSceneRsp_t *pViewSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclRemoveSceneRspCmdHandler
 *
 * @brief   Handler for ZCL remove scene response command.
 *
 * @param   pRemoveSceneRsp
 *
 * @return  None
 */
static void app_zclRemoveSceneRspCmdHandler(removeSceneRsp_t *pRemoveSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclRemoveAllSceneRspCmdHandler
 *
 * @brief   Handler for ZCL remove all scene response command.
 *
 * @param   pRemoveAllSceneRsp
 *
 * @return  None
 */
static void app_zclRemoveAllSceneRspCmdHandler(removeAllSceneRsp_t *pRemoveAllSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclStoreSceneRspCmdHandler
 *
 * @brief   Handler for ZCL store scene response command.
 *
 * @param   pStoreSceneRsp
 *
 * @return  None
 */
static void app_zclStoreSceneRspCmdHandler(storeSceneRsp_t *pStoreSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclGetSceneMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get scene membership response command.
 *
 * @param   pGetSceneMembershipRsp
 *
 * @return  None
 */
static void app_zclGetSceneMembershipRspCmdHandler(getSceneMemRsp_t *pGetSceneMembershipRsp)
{

}

/*********************************************************************
 * @fn      app_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == APP_ENDPOINT1){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_SCENE_ADD_SCENE_RSP:
				case ZCL_CMD_SCENE_ENHANCED_ADD_SCENE_RSP:
					app_zclAddSceneRspCmdHandler(cmdId, (addSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_VIEW_SCENE_RSP:
				case ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE_RSP:
					app_zclViewSceneRspCmdHandler(cmdId, (viewSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_REMOVE_SCENE_RSP:
					app_zclRemoveSceneRspCmdHandler((removeSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_REMOVE_ALL_SCENE_RSP:
					app_zclRemoveAllSceneRspCmdHandler((removeAllSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_STORE_SCENE_RSP:
					app_zclStoreSceneRspCmdHandler((storeSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_GET_SCENE_MEMSHIP_RSP:
					app_zclGetSceneMembershipRspCmdHandler((getSceneMemRsp_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_SCENE */


/*********************************************************************
 * @fn      app_timeCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_timeCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload) {

    //printf("app_timeCb. cmd: 0x%x\r\n", cmdId);

    return ZCL_STA_SUCCESS;
}



static int32_t checkRespTimeCb(void *arg) {

    if (device_online) {
        if (!resp_time) {
            if (count_no_service++ == 3) {
                device_online = false;
#if UART_PRINTF_MODE// && DEBUG_LEVEL
                printf("No service!\r\n");
#endif
            }
        } else {
            count_no_service = 0;
        }
    } else {
        if (resp_time) {
            device_online = true;
            count_no_service = 0;
#if UART_PRINTF_MODE// && DEBUG_LEVEL
            printf("Device online\r\n");
#endif
        }
    }

    resp_time = false;

    return -1;
}


int32_t getTimeCb(void *arg) {

    if(zb_isDeviceJoinedNwk()){
        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
        dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        dstEpInfo.dstEp = APP_ENDPOINT1;
        dstEpInfo.dstAddr.shortAddr = 0x0;
#endif
        zclAttrInfo_t *pAttrEntry;
        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_TIME, ZCL_ATTRID_TIME);

        zclReadCmd_t *pReadCmd = (zclReadCmd_t *)ev_buf_allocate(sizeof(zclReadCmd_t) + sizeof(uint16_t));
        if(pReadCmd){
            pReadCmd->numAttr = 1;
            pReadCmd->attrID[0] = ZCL_ATTRID_TIME;

            zcl_read(APP_ENDPOINT1, &dstEpInfo, ZCL_CLUSTER_GEN_TIME, MANUFACTURER_CODE_NONE, 0, 0, 0, pReadCmd);

            ev_buf_free((uint8_t *)pReadCmd);

            TL_ZB_TIMER_SCHEDULE(checkRespTimeCb, NULL, TIMEOUT_2SEC);
        }
    }

    return 0;
}

status_t app_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload) {

//    printf("app_onOffCb, dstEp: %d\r\n", pAddrInfo->dstEp);

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    pOnOff += pAddrInfo->dstEp - 1;

    if(pAddrInfo->dstEp == APP_ENDPOINT1 || pAddrInfo->dstEp == APP_ENDPOINT2) {
#if 0
        if (relay_settings.switchType[pAddrInfo->dstEp-1] != ZCL_SWITCH_TYPE_MULTIFUNCTION) {
#endif
            switch(cmdId){
                case ZCL_CMD_ONOFF_ON:
                    printf("pAddrInfo->dstEp: %d, cmd on\r\n", pAddrInfo->dstEp);
                    cmdOnOff_on(pAddrInfo->dstEp);
                    break;
                case ZCL_CMD_ONOFF_OFF:
                    printf("pAddrInfo->dstEp: %d, cmd off\r\n", pAddrInfo->dstEp);
                    cmdOnOff_off(pAddrInfo->dstEp);
                    break;
                case ZCL_CMD_ONOFF_TOGGLE:
                    printf("pAddrInfo->dstEp: %d, cmd toggle\r\n", pAddrInfo->dstEp);
                    cmdOnOff_toggle(pAddrInfo->dstEp);
                    break;
//                case ZCL_CMD_OFF_WITH_EFFECT:
//                    if(pOnOff->globalSceneControl == TRUE){
//                        /* TODO: store its settings in its global scene */
//
//                        pOnOff->globalSceneControl = FALSE;
//                    }
//                    sampleLight_onoff_offWithEffectProcess((zcl_onoff_offWithEffectCmd_t *)cmdPayload);
//                    break;
                case ZCL_CMD_ON_WITH_RECALL_GLOBAL_SCENE:
                    if(pOnOff->globalSceneControl == FALSE){
//                        app_onoff_onWithRecallGlobalSceneProcess();
                        pOnOff->globalSceneControl = TRUE;
                    }
                    break;
//                case ZCL_CMD_ON_WITH_TIMED_OFF:
//                    sampleLight_onoff_onWithTimedOffProcess((zcl_onoff_onWithTimeOffCmd_t *)cmdPayload);
//                    break;
                default:
                    break;
            }
#if 0
        } else {
            printf("pAddrInfo->dstEp: %d, cmd off\r\n", pAddrInfo->dstEp);
            cmdOnOff_off(pAddrInfo->dstEp);
        }
#endif
    }

    return ZCL_STA_SUCCESS;
}

status_t app_msInputCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload) {

//    printf("app_aInputCb(). pAddrInfo->dirCluster: %0x%x, cmdId: 0x%x\r\n", pAddrInfo->dirCluster, cmdId);

    status_t status = ZCL_STA_SUCCESS;

    return status;
}

/*********************************************************************
 * @fn      app_meteringCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_meteringCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{

    return ZCL_STA_SUCCESS;
}


