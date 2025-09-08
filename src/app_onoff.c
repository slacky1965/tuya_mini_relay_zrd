#include "app_main.h"

void cmdOnOff_toggle(uint8_t ep) {

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    config->status_onoff = !config->status_onoff;
    config_save();
    pOnOff->onOff = config->status_onoff;
    uint8_t status = RELAY_OFF;

    if (pOnOff->onOff) {
        status = RELAY_ON;
    }

    set_relay_status(status);
}

void cmdOnOff_on(uint8_t ep) {

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    config->status_onoff = ON;
    config_save();
    pOnOff->onOff = config->status_onoff;
    uint8_t status = RELAY_ON;

    set_relay_status(status);
}

void cmdOnOff_off(uint8_t ep) {

    zcl_onOffAttr_t *pOnOff = zcl_onOffAttrsGet();
    config->status_onoff = OFF;
    config_save();
    pOnOff->onOff = config->status_onoff;
    uint8_t status = RELAY_OFF;

    set_relay_status(status);
}
