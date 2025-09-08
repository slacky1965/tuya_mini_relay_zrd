#include "app_main.h"

bool get_relay_status() {
    return drv_gpio_read(RELAY_GPIO)?true:false;
}

void set_relay_status(uint8_t status) {
    drv_gpio_write(RELAY_GPIO, status);
//    if (status) {
//        printf("Relay is on: %d\r\n", get_relay_status());
//        thermostat_settings.running_state = thermostat_settings.running_state | (1 << RUN_STATE_HEAT_BIT);
//    } else {
//        printf("Relay is off: %d\r\n", get_relay_status());
//        thermostat_settings.running_state = thermostat_settings.running_state & ~(1 << RUN_STATE_HEAT_BIT);
//    }
//
//    zcl_setAttrVal(APP_ENDPOINT1,
//                   ZCL_CLUSTER_HAVC_THERMOSTAT,
//                   ZCL_ATTRID_HVAC_THERMOSTAT_RUNNING_STATE,
//                   (uint8_t*)&thermostat_settings.running_state);
}
