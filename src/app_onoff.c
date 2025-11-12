#include "app_main.h"

void cmdOnOff_toggle(uint8_t ep) {

#if UART_PRINTF_MODE && DEBUG_ONOFF
    printf("cmdOnOff_toggle. ep: %d\r\n", ep);
#endif

    uint8_t i = ep-1;

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    pOnOff += i;
    relay_settings.status_onoff[i] = !relay_settings.status_onoff[i];
    pOnOff->onOff = relay_settings.status_onoff[i];
    if (pOnOff->startUpOnOff != ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF && pOnOff->startUpOnOff != ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON) {
        relay_settings_save();
    }
    uint8_t status = RELAY_OFF;

    if (pOnOff->onOff) {
        status = RELAY_ON;
    }

    set_relay_status(i, status);
}

void cmdOnOff_on(uint8_t ep) {

#if UART_PRINTF_MODE && DEBUG_ONOFF
    printf("cmdOnOff_on. ep: %d\r\n", ep);
#endif

    uint8_t i = ep-1;

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    pOnOff += i;
    pOnOff->onOff = relay_settings.status_onoff[i] = ZCL_ONOFF_STATUS_ON;
    if (pOnOff->startUpOnOff != ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF && pOnOff->startUpOnOff != ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON) {
        relay_settings_save();
    }
    uint8_t status = RELAY_ON;

    set_relay_status(i, status);
}

void cmdOnOff_off(uint8_t ep) {

#if UART_PRINTF_MODE && DEBUG_ONOFF
    printf("cmdOnOff_off. ep: %d\r\n", ep);
#endif

    uint8_t i = ep-1;

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    pOnOff += i;
    pOnOff->onOff = relay_settings.status_onoff[i] = ZCL_ONOFF_STATUS_OFF;
    if (pOnOff->startUpOnOff != ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF && pOnOff->startUpOnOff != ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON) {
        relay_settings_save();
    }
    uint8_t status = RELAY_OFF;

    set_relay_status(i, status);
}

void remoteCmdOnOff(uint8_t ep, uint8_t cmd) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;

    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    /* command 0x00 - off, 0x01 - on, 0x02 - toggle */

    switch(cmd) {
        case ZCL_CMD_ONOFF_OFF:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: off\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_offCmd(ep, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_ON:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: on\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_onCmd(ep, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_TOGGLE:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: toggle\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_toggleCmd(ep, &dstEpInfo, FALSE);
            break;
        default:
            break;
    }
}
