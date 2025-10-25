#include "app_main.h"

uint32_t last_timeReportMsi[AMT_RELAY];     // time of the last attribute report ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE
uint8_t  last_seqNum[AMT_RELAY];

static int32_t resetMsiTimerCb(void *args) {

    uint8_t i = (uint8_t)((uint32_t)args) - 1;

//    printf("resetMsiTimerCb. i: %d\r\n", i);

    zcl_msInputAttr_t *msInputAttr = zcl_msInputAttrsGet();
    msInputAttr += i;
    msInputAttr->value = ACTION_EMPTY;

//    app_forcedReport(dev_relay.unit_relay[i].ep, ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC, ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE);

    return -1;
}

void app_all_forceReporting(void *args) {

    if (zb_isDeviceJoinedNwk()) {
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CUSTOM_DEVICE_MODEL);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_GEN_DEVICE_TEMP_CONFIG, ZCL_ATTRID_DEV_TEMP_CURR_TEMP);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_METER_SERIAL_NUMBER);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CUSTOM_DATE_RELEASE);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_MULTIPLIER);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_DIVISOR);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_DELIVERD);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_DELIVERD);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_DELIVERD);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_DELIVERD);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_AC_VOLTAGE_MULTIPLIER);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_AC_VOLTAGE_DIVISOR);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_RMS_VOLTAGE);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_AC_POWER_MULTIPLIER);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_AC_POWER_DIVISOR);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_ACTIVE_POWER);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_AC_CURRENT_MULTIPLIER);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_AC_CURRENT_DIVISOR);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_RMS_CURRENT);
//        app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_REMAINING_BATTERY_LIFE);
    }

}

void app_forcedReport(uint8_t endpoint, uint16_t claster_id, uint16_t attr_id) {

    if (zb_isDeviceJoinedNwk()) {

        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        status_t ret = 0;
        uint8_t i = endpoint - 1;
        dstEpInfo.profileId = HA_PROFILE_ID;
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

        zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, claster_id, attr_id);

        if (!pAttrEntry) {
            //should not happen.
            ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_ZCL_ENTRY);
            return;
        }

        if (attr_id == ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE) {
            last_timeReportMsi[i] = clock_time();
            last_seqNum[i] = ZCL_SEQ_NUM;
//            printf("MSI report. seqNum: %d\r\n", last_seqNum[i]);
            ret = zcl_report(endpoint, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR, last_seqNum[i],
                    MANUFACTURER_CODE_NONE, claster_id, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
            TL_ZB_TIMER_SCHEDULE(resetMsiTimerCb, (void*)((uint32_t)endpoint), TIMEOUT_750MS);
        } else {
            ret = zcl_sendReportCmd(endpoint, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                    claster_id, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
        }


#if UART_PRINTF_MODE && DEBUG_REPORTING
        printf("ret: %d, forceReportCb. endpoint: 0x%x, claster_id: 0x%x, attr_id: 0x%x\r\n", ret, endpoint, claster_id, attr_id);
#endif
    }


}

int32_t forcedReportCb(void *arg) {

    TL_SCHEDULE_TASK(app_all_forceReporting, NULL);

    return -1;
}
