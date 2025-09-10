#include "app_main.h"

void cmdOnOff_toggle(uint8_t ep) {

#if UART_PRINTF_MODE && DEBUG_ONOFF
    printf("cmdOnOff_toggle\r\n");
#endif

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    relay_settings.status_onoff = !relay_settings.status_onoff;
    relay_settings_save();
    pOnOff->onOff = relay_settings.status_onoff;
    uint8_t status = RELAY_OFF;

    if (pOnOff->onOff) {
        status = RELAY_ON;
    }

    set_relay_status(status);
}

void cmdOnOff_on(uint8_t ep) {

#if UART_PRINTF_MODE && DEBUG_ONOFF
    printf("cmdOnOff_on\r\n");
#endif

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    relay_settings.status_onoff = ZCL_ONOFF_STATUS_ON;
    relay_settings_save();
    pOnOff->onOff = relay_settings.status_onoff;
    uint8_t status = RELAY_ON;

    set_relay_status(status);
}

void cmdOnOff_off(uint8_t ep) {

#if UART_PRINTF_MODE && DEBUG_ONOFF
    printf("cmdOnOff_off\r\n");
#endif

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    relay_settings.status_onoff = ZCL_ONOFF_STATUS_OFF;
    relay_settings_save();
    pOnOff->onOff = relay_settings.status_onoff;
    uint8_t status = RELAY_OFF;

    set_relay_status(status);
}
