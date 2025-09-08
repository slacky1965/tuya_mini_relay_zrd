#include "app_main.h"

static bool time_sent = false;
uint8_t str_time[32] = {0};
static ftime_t ftime;
static uint32_t local_time = 0;
static uint32_t attr_data;

void get_time(uint32_t *local_time) {

    uint16_t attr_len;

    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_TIME, ZCL_ATTRID_LOCAL_TIME, &attr_len, (uint8_t*)&attr_data);

    if (attr_data != 0xFFFFFFFF) {
        *local_time = attr_data;
    } else {
        local_time = NULL;
    }
}

static int32_t app_clockCb(void *arg) {

    uint32_t *p_local_time = &local_time;

    if (time_sent) {
        get_time(p_local_time);

        if (p_local_time) {
            local_time++;
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_TIME, ZCL_ATTRID_LOCAL_TIME, (uint8_t*)&local_time);

#if UART_PRINTF_MODE && DEBUG_TIME
            printf("Local time: %d\r\n", (local_time+UNIX_TIME_CONST));
#endif
        }
    }

    TL_SCHEDULE_TASK(send_uart_commandCb, NULL);

    return 0;
}

int32_t app_time_cmdCb(void *arg) {

//    printf("app_time_cmdCb(). time_sent: %d\r\n", time_sent);

    if(zb_isDeviceJoinedNwk()) {
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
            pReadCmd->attrID[0] = ZCL_ATTRID_LOCAL_TIME;

            zcl_read(APP_ENDPOINT1, &dstEpInfo, ZCL_CLUSTER_GEN_TIME, MANUFACTURER_CODE_NONE, 0, 0, 0, pReadCmd);

            ev_buf_free((uint8_t *)pReadCmd);

            if (time_sent) return TIMEOUT_60MIN;
        }

    }

    return 0;
}

void set_time_sent() {

    if (!time_sent) {
        TL_ZB_TIMER_SCHEDULE(app_clockCb, NULL, TIMEOUT_1SEC);
    }

    time_sent = true;

}

bool get_time_sent() {

    return time_sent;
}

ftime_t *get_ftime() {

    if (local_time == 0) return NULL;

    uint32_t counter = local_time;

    uint32_t ace;
    uint8_t b;
    uint8_t d;
    uint8_t m;
    uint16_t y;

    ace = (counter / 86400) + 32044 + JD0;
    b = (4 * ace + 3) / 146097;
    ace = ace - ((146097 * b) / 4);
    d = (4 * ace + 3) / 1461;
    ace = ace - ((1461 * d) / 4);
    m = (5 * ace + 2) / 153;
    ftime.day = ace - ((153 * m + 2) / 5) + 1;
    ftime.month = m + 3 - (12 * (m / 10));
    ftime.year = 100 * b + d - 4800 + (m / 10);
    ftime.hour = (counter / 3600) % 24;
    ftime.minute = (counter / 60) % 60;
    ftime.second = (counter % 60);

    y = ftime.year % 100;
    ftime.wday = (y / 12) + (y % 12) + ((y % 12) / 4);
    uint8_t m_idx[] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    ftime.wday += m_idx[ftime.month - 1];
    if ((y % 44) == 0) {
      if ((ftime.month == 1) || (ftime.month == 2)) ftime.wday--;
    }
    ftime.wday += ftime.day;
    ftime.wday = (ftime.wday % 7);

    return &ftime;
}


void print_time() {
    ftime_t ftime;
    uint32_t ace;
    uint8_t b;
    uint8_t d;
    uint8_t m;

    uint16_t attr_len;
    uint32_t counter;

    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_TIME, ZCL_ATTRID_LOCAL_TIME, &attr_len, (uint8_t*)&counter);

    if (counter != 0xFFFFFFFF) {
        ace = (counter / 86400) + 32044 + JD0;
        b = (4 * ace + 3) / 146097;
        ace = ace - ((146097 * b) / 4);
        d = (4 * ace + 3) / 1461;
        ace = ace - ((1461 * d) / 4);
        m = (5 * ace + 2) / 153;
        ftime.day = ace - ((153 * m + 2) / 5) + 1;
        ftime.month = m + 3 - (12 * (m / 10));
        ftime.year = 100 * b + d - 4800 + (m / 10);
        ftime.hour = (counter / 3600) % 24;
        ftime.minute = (counter / 60) % 60;
        ftime.second = (counter % 60);
#if UART_PRINTF_MODE
        printf("[%d-%s%d-%s%d %s%d:%s%d:%s%d] ",
                    ftime.year,
                    ftime.month<10?"0":"", ftime.month,
                    ftime.day<10?"0":"", ftime.day,
                    ftime.hour<10?"0":"", ftime.hour,
                    ftime.minute<10?"0":"", ftime.minute,
                    ftime.second<10?"0":"", ftime.second);
#endif
    }
}
