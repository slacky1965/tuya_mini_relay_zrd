#include "app_main.h"

relay_settings_t relay_settings;

static uint8_t checksum(uint8_t *data, uint16_t length) {

    uint8_t crc8 = 0;

    for(uint8_t i = 0; i < length; i++) {
        crc8 += data[i];
    }

    return crc8;
}

bool get_relay_status() {
    return drv_gpio_read(RELAY_GPIO)?true:false;
}

void set_relay_status(uint8_t status) {
    drv_gpio_write(RELAY_GPIO, status);
//    if (status) {
//        printf("Relay is on: %d\r\n", get_relay_status());
//        relay_settings.running_state = relay_settings.running_state | (1 << RUN_STATE_HEAT_BIT);
//    } else {
//        printf("Relay is off: %d\r\n", get_relay_status());
//        relay_settings.running_state = relay_settings.running_state & ~(1 << RUN_STATE_HEAT_BIT);
//    }
//
//    zcl_setAttrVal(APP_ENDPOINT1,
//                   ZCL_CLUSTER_HAVC_THERMOSTAT,
//                   ZCL_ATTRID_HVAC_THERMOSTAT_RUNNING_STATE,
//                   (uint8_t*)&relay_settings.running_state);
}

static void print_setting_sr(nv_sts_t st, relay_settings_t *relay_settings_tmp, bool save) {

#if UART_PRINTF_MODE && DEBUG_SAVE

    printf("Settings %s. Return: %s\r\n", save?"saved":"restored", st==NV_SUCC?"Ok":"Error");

    printf("status_onoff   0x%02x\r\n", relay_settings_tmp->status_onoff);
    printf("startUpOnOff:  0x%02x\r\n", relay_settings_tmp->startUpOnOff);
    printf("switchActions: 0x%02x\r\n", relay_settings_tmp->switchActions);
    printf("switchType:    0x%02x\r\n", relay_settings_tmp->switchType);
#endif
}

nv_sts_t relay_settings_save() {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

#if UART_PRINTF_MODE
    printf("Saved relay settings\r\n");
#endif

    relay_settings.crc = checksum((uint8_t*)&relay_settings, sizeof(relay_settings_t)-1);
    st = nv_flashWriteNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(relay_settings_t), (uint8_t*)&relay_settings);

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t relay_settings_restore() {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    relay_settings_t relay_settings_tmp;

    st = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(relay_settings_t), (uint8_t*)&relay_settings_tmp);

    if (st == NV_SUCC && relay_settings_tmp.crc == checksum((uint8_t*)&relay_settings_tmp, sizeof(relay_settings_t)-1)) {

#if UART_PRINTF_MODE
        printf("Restored relay settings\r\n");
#if DEBUG_SAVE
        print_setting_sr(st, &relay_settings_tmp, false);
#endif
#endif

    } else {
        /* default config */
#if UART_PRINTF_MODE
        printf("Default relay settings \r\n");
#endif
        relay_settings_tmp.startUpOnOff = ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF;
        relay_settings_tmp.status_onoff = ZCL_ONOFF_STATUS_OFF;
        relay_settings_tmp.switchActions = ZCL_SWITCH_ACTION_OFF_ON;
        relay_settings_tmp.switchType = ZCL_SWITCH_TYPE_MOMENTARY;
    }

    memcpy(&relay_settings, &relay_settings_tmp, (sizeof(relay_settings_t)));
    g_zcl_onOffAttrs.onOff = relay_settings.status_onoff;
    g_zcl_onOffAttrs.startUpOnOff = relay_settings.startUpOnOff;
    g_zcl_onOffCfgAttrs.custom_swtichType = g_zcl_onOffCfgAttrs.switchType = relay_settings.switchType;
    g_zcl_onOffCfgAttrs.switchActions = relay_settings.switchActions;

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

