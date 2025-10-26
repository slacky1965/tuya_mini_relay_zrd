#include "tl_common.h"
#include "zcl_include.h"
#include "smart_energy/zcl_metering.h"
#include "zcl_se_metering.h"
#include "app_monitoring.h"

_CODE_ZCL_ static status_t zcl_metering_getProfilePrc(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    u8 *pData = pInMsg->pData;

    if (pInMsg->clusterAppCb) {
        zcl_metering_getProfileCmd_t getProfileCmd;
        getProfileCmd.intervalChannel = *pData++;
        getProfileCmd.endTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        pData += 4;
        getProfileCmd.numberOfPeriods = *pData++;

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &getProfileCmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_requestMirrorRspPrc(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    u8 *pData = pInMsg->pData;

    if (pInMsg->clusterAppCb) {
        zcl_metering_requestMirrorRspCmd_t requestMirrorRspCmd;
        requestMirrorRspCmd.endPointId = BUILD_U16(pData[0], pData[1]);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &requestMirrorRspCmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_mirrorRemovedPrc(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    u8 *pData = pInMsg->pData;

    if (pInMsg->clusterAppCb) {
        zcl_metering_mirrorRemovedCmd_t mirrorRemovedCmd;
        mirrorRemovedCmd.removedEndPointId = BUILD_U16(pData[0], pData[1]);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &mirrorRemovedCmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_requestFastPollModePrc(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    u8 *pData = pInMsg->pData;

    if (pInMsg->clusterAppCb) {
        zcl_metering_requestFastPollModeCmd_t requestFastPollModeCmd;
        requestFastPollModeCmd.fastPollUpdatePeriod = *pData++;
        requestFastPollModeCmd.duration = *pData++;

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &requestFastPollModeCmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_getProfileRspPrc(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    u8 *pData = pInMsg->pData;

    if (pInMsg->clusterAppCb) {
        zcl_metering_getProfileRspCmd_t getProfileRspCmd;
        getProfileRspCmd.endTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        pData += 4;
        getProfileRspCmd.status = *pData++;
        getProfileRspCmd.profileIntervalPeriod = *pData++;
        getProfileRspCmd.numberOfPeriodsDelived = *pData++;
        getProfileRspCmd.intervals = pData;

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &getProfileRspCmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_requestMirrorPrc(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    //u8 *pData = pInMsg->pData; //this command has no payload.

    if (pInMsg->clusterAppCb) {
        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, NULL);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_removeMirrorPrc(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    //u8 *pData = pInMsg->pData; //this command has no payload.

    if (pInMsg->clusterAppCb) {
        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, NULL);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_requestFastPollModeRspPrc(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    u8 *pData = pInMsg->pData;

    if (pInMsg->clusterAppCb) {
        zcl_metering_requestFastPollModeRspCmd_t requestFastPollModeRspCmd;
        requestFastPollModeRspCmd.appliedUpdatePeriod = *pData++;
        requestFastPollModeRspCmd.fastPollModeEndTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &requestFastPollModeRspCmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_clientCmdHandler(zclIncoming_t *pInMsg) {

//    printf("zcl_metering_clientCmdHandler\r\n");

    u8 status = ZCL_STA_SUCCESS;

    switch (pInMsg->hdr.cmd) {
    case ZCL_CMD_GET_PROFILE:
        status = zcl_metering_getProfilePrc(pInMsg);
        break;
    case ZCL_CMD_REQUEST_MIRROR_RESPONSE:
        status = zcl_metering_requestMirrorRspPrc(pInMsg);
        break;
    case ZCL_CMD_MIRROR_REMOVED:
        status = zcl_metering_mirrorRemovedPrc(pInMsg);
        break;
    case ZCL_CMD_REQUEST_FAST_POLL_MODE:
        status = zcl_metering_requestFastPollModePrc(pInMsg);
        break;
    case ZCL_CMD_REMOVE_ENERGY:
        energy_remove();
        break;
    default:
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_serverCmdHandler(zclIncoming_t *pInMsg) {

//    printf("zcl_metering_serverCmdHandler\r\n");

    u8 status = ZCL_STA_SUCCESS;

    switch (pInMsg->hdr.cmd) {
    case ZCL_CMD_GET_PROFILE_RESPONSE:
        status = zcl_metering_getProfileRspPrc(pInMsg);
        break;
    case ZCL_CMD_REQUEST_MIRROR:
        status = zcl_metering_requestMirrorPrc(pInMsg);
        break;
    case ZCL_CMD_REMOVE_MIRROR:
        status = zcl_metering_removeMirrorPrc(pInMsg);
        break;
    case ZCL_CMD_REQUEST_FAST_POLL_MODE_RESPONSE:
        status = zcl_metering_requestFastPollModeRspPrc(pInMsg);
        break;
    default:
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    }

    return status;
}

_CODE_ZCL_ static status_t app_zcl_metering_cmdHandler(zclIncoming_t *pInMsg) {

//    printf("app_zcl_metering_cmdHandler\r\n");

    if (pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR) {
        return zcl_metering_clientCmdHandler(pInMsg);
    } else {
        return zcl_metering_serverCmdHandler(pInMsg);
    }
}

_CODE_ZCL_ status_t app_zcl_metering_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_SE_METERING, manuCode, attrNum, attrTbl, app_zcl_metering_cmdHandler, cb);
}

