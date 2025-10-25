#include "app_main.h"

relay_settings_t relay_settings;
dev_relay_t      dev_relay;

static uint8_t checksum(uint8_t *data, uint16_t length) {

    uint8_t crc8 = 0;

    for(uint8_t i = 0; i < length; i++) {
        crc8 += data[i];
    }

    return crc8;
}

static void check_first_start(uint8_t i) {

    switch(relay_settings.startUpOnOff[i]) {
        case ZCL_START_UP_ONOFF_SET_ONOFF_TO_PREVIOUS:
            if (relay_settings.status_onoff[i]) cmdOnOff_on(dev_relay.unit_relay[i].ep);
            else cmdOnOff_off(dev_relay.unit_relay[i].ep);
            break;
        case ZCL_START_UP_ONOFF_SET_ONOFF_TOGGLE:
            cmdOnOff_toggle(dev_relay.unit_relay[i].ep);
            break;
        case ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON:
            cmdOnOff_on(dev_relay.unit_relay[i].ep);
            break;
        case ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF:
            cmdOnOff_off(dev_relay.unit_relay[i].ep);
            break;
        default:
            cmdOnOff_off(dev_relay.unit_relay[i].ep);
            break;
    }
}



//bool get_relay_status() {
//    return drv_gpio_read(RELAY1_GPIO)?true:false;
//}

void set_relay_status(uint8_t i, uint8_t status) {
//    printf("set_relay_status(i = %d, status = %d). GPIO: %d\r\n", i, status, dev_relay.unit_relay[i].rl);
    drv_gpio_write(dev_relay.unit_relay[i].rl, status);
}

static void print_setting_sr(nv_sts_t st, relay_settings_t *relay_settings_tmp, bool save) {

#if UART_PRINTF_MODE && DEBUG_SAVE

    printf("Settings %s. Return: %s\r\n", save?"saved":"restored", st==NV_SUCC?"Ok":"Error");

    printf("status_onoff1:      0x%02x\r\n", relay_settings_tmp->status_onoff[0]);
//    printf("status_onoff2:      0x%02x\r\n", relay_settings_tmp->status_onoff[1]);
    printf("startUpOnOff1:      0x%02x\r\n", relay_settings_tmp->startUpOnOff[0]);
//    printf("startUpOnOff2:      0x%02x\r\n", relay_settings_tmp->startUpOnOff[1]);
    printf("switchActions1:     0x%02x\r\n", relay_settings_tmp->switchActions[0]);
//    printf("switchActions2:     0x%02x\r\n", relay_settings_tmp->switchActions[1]);
    printf("switchType1:        0x%02x\r\n", relay_settings_tmp->switchType[0]);
//    printf("switchType2:        0x%02x\r\n", relay_settings_tmp->switchType[1]);
    printf("switch_decoupled1:  0x%02x\r\n", relay_settings_tmp->switch_decoupled[0]);
//    printf("switch_decoupled2:  0x%02x\r\n", relay_settings_tmp->switch_decoupled[1]);

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
        relay_settings_tmp.startUpOnOff[0] = ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF;
//        relay_settings_tmp.startUpOnOff[1] = ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF;
        relay_settings_tmp.status_onoff[0] = ZCL_ONOFF_STATUS_OFF;
//        relay_settings_tmp.status_onoff[1] = ZCL_ONOFF_STATUS_OFF;
        relay_settings_tmp.switchActions[0] = ZCL_SWITCH_ACTION_OFF_ON;
//        relay_settings_tmp.switchActions[1] = ZCL_SWITCH_ACTION_OFF_ON;
        relay_settings_tmp.switchType[0] = ZCL_SWITCH_TYPE_MOMENTARY;
//        relay_settings_tmp.switchType[1] = ZCL_SWITCH_TYPE_MOMENTARY;
        relay_settings_tmp.switch_decoupled[0] = CUSTOM_SWITCH_DECOUPLED_OFF;
//        relay_settings_tmp.switch_decoupled[1] = CUSTOM_SWITCH_DECOUPLED_OFF;
    }

    memcpy(&relay_settings, &relay_settings_tmp, (sizeof(relay_settings_t)));
    g_zcl_onOffAttrs[0].onOff = relay_settings.status_onoff[0];
//    g_zcl_onOffAttrs[1].onOff = relay_settings.status_onoff[1];
    g_zcl_onOffAttrs[0].startUpOnOff = relay_settings.startUpOnOff[0];
//    g_zcl_onOffAttrs[1].startUpOnOff = relay_settings.startUpOnOff[1];
    g_zcl_onOffCfgAttrs[0].custom_swtichType = g_zcl_onOffCfgAttrs[0].switchType = relay_settings.switchType[0];
//    g_zcl_onOffCfgAttrs[1].custom_swtichType = g_zcl_onOffCfgAttrs[1].switchType = relay_settings.switchType[1];
    g_zcl_onOffCfgAttrs[0].switchActions = relay_settings.switchActions[0];
//    g_zcl_onOffCfgAttrs[1].switchActions = relay_settings.switchActions[1];
    g_zcl_onOffCfgAttrs[0].custom_decoupled = relay_settings.switch_decoupled[0];
//    g_zcl_onOffCfgAttrs[1].custom_decoupled = relay_settings.switch_decoupled[1];

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

void dev_relay_init() {
    dev_relay.amt = AMT_RELAY;
    dev_relay.unit_relay[0].ep = APP_ENDPOINT1;
    dev_relay.unit_relay[0].sw = SWITCH1_GPIO;
    dev_relay.unit_relay[0].rl = RELAY1_GPIO;

    if (relay_settings.switchType[0] == ZCL_SWITCH_TYPE_MULTIFUNCTION) {
        check_first_start(0);
    }

//    if (relay_settings.switchType[1] == ZCL_SWITCH_TYPE_MULTIFUNCTION) {
//        check_first_start(1);
//    }
}
