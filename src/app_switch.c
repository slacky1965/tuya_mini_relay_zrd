#include "app_main.h"

#define DEBOUNCE_SWITCH     128     /* number of polls for debounce                 */
#define FR_COUNTER_MAX      10      /* number for factory reset                     */

typedef enum {
    SWITCH_OFF = 0,
    SWITCH_ON,
    SWITCH_FLOAT,
} switch_status_t;

typedef struct {
    ev_timer_event_t *timerFrCounterEvt;
    ev_timer_event_t *timerMfCounterEvt;
    switch_status_t   status;
    bool              hold;
    uint8_t           debounce;
    uint8_t           counter;
//    uint8_t           fr_counter;             /* factory reset counter                        */
    uint32_t          time_hold;
} app_switch_t;

static app_switch_t app_switch_cfg[AMT_RELAY] = {
    {
        .status = SWITCH_FLOAT,
        .debounce = (DEBOUNCE_SWITCH / 2),
        .hold = false,
        .counter = 0,
        .timerFrCounterEvt = NULL,
        .timerMfCounterEvt = NULL,
    },
};

static app_switch_t *app_switch = app_switch_cfg;

static int32_t net_steer_start_offCb(void *args) {

    g_appCtx.net_steer_start = false;

    light_blink_stop();

    return -1;
}

static void switch_factory_reset_start(void *args) {

//    printf("Factory reset\r\n");

    zb_factoryReset();

    g_appCtx.net_steer_start = true;
    TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
    light_blink_start(90, 250, 750);

}


static int32_t switch_counterFrCb(void *args) {

    uint8_t i = (uint8_t)((uint32_t)args);

//    printf("counter click: %d\r\n", app_switch[i].counter);
    if (app_switch[i].counter >= FR_COUNTER_MAX) {
//        printf("Factory reset\r\n");
        TL_SCHEDULE_TASK(switch_factory_reset_start, NULL);
    }

    if (app_switch[i].status == SWITCH_OFF && relay_settings.switchType [i] != ZCL_SWITCH_TYPE_MULTIFUNCTION) {
        if (relay_settings.switchType[i] == ZCL_SWITCH_TYPE_MOMENTARY) {
#if UART_PRINTF_MODE && DEBUG_SWITCH
            printf("FR Switch is OFF\r\n");
#endif
            if (relay_settings.switchActions[i] == ZCL_SWITCH_ACTION_OFF_ON) {
                remoteCmdOnOff(dev_relay.unit_relay[i].ep, ZCL_CMD_ONOFF_OFF);
                if (relay_settings.switch_decoupled[i] == CUSTOM_SWITCH_DECOUPLED_OFF) {
                    cmdOnOff_off(dev_relay.unit_relay[i].ep);
                }
            } else {
                remoteCmdOnOff(dev_relay.unit_relay[i].ep, ZCL_CMD_ONOFF_ON);
                if (relay_settings.switch_decoupled[i] == CUSTOM_SWITCH_DECOUPLED_OFF) {
                    cmdOnOff_on(dev_relay.unit_relay[i].ep);
                }
            }
        }
    }

    app_switch[i].counter = 0;
    app_switch[i].timerFrCounterEvt = NULL;

    return -1;
}

static int32_t switch_counterMfCb(void *args) {

    uint8_t i = (uint8_t)((uint32_t)args);
    bool report = false;

    zcl_msInputAttr_t *msInputAttr = zcl_msInputAttrsGet();
    msInputAttr += i;

//    printf("counter: %d\r\n", app_switch[i].counter);

    if (app_switch[i].hold) {
        msInputAttr->value = ACTION_HOLD;                               // 0
        report = true;
#if UART_PRINTF_MODE && DEBUG_SWITCH
        printf("Switch %d is Hold\r\n", dev_relay.unit_relay[i].ep);
#endif
    } else {
        switch(app_switch[i].counter) {
            case ACTION_SINGLE:                                         // 1
            case ACTION_DOUBLE:                                         // 2
            case ACTION_TRIPLE:                                         // 3
            case ACTION_QUADRUPLE:                                      // 4
            case ACTION_QUINTUPLE:                                      // 5
                msInputAttr->value = app_switch[i].counter;
                report = true;
#if UART_PRINTF_MODE && DEBUG_SWITCH
                printf("Switch %d click %d times\r\n", dev_relay.unit_relay[i].ep, msInputAttr->value);
#endif
                break;
            case ACTION_CLEAR:                                          // 250
                msInputAttr->value = ACTION_EMPTY;                      // 300
                report = true;
                break;
            default:
                if (app_switch[i].counter >= FR_COUNTER_MAX) {
                    TL_SCHEDULE_TASK(switch_factory_reset_start, NULL);
                }
                break;
        }
    }

    if (report) {
        app_forcedReport(dev_relay.unit_relay[i].ep, ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC, ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE);
    }

    app_switch[i].counter = 0;
    app_switch[i].timerMfCounterEvt = NULL;

    return -1;
}

static void switch_first_start(uint8_t i, switch_status_t status) {

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
    app_switch[i].status = status;
}

static void read_switch_toggle(uint8_t i) {
    if (!drv_gpio_read(dev_relay.unit_relay[i].sw)) {
        if (app_switch[i].status != SWITCH_ON) {
            if (app_switch[i].debounce != DEBOUNCE_SWITCH) {
                app_switch[i].debounce++;
                if (app_switch[i].debounce == DEBOUNCE_SWITCH) {
//                    printf("switch toggle\r\n");
                    if (app_switch[i].status == SWITCH_FLOAT) {
                        switch_first_start(i, SWITCH_ON);
                    } else {
                        app_switch[i].status = SWITCH_ON;
                        app_switch[i].counter++;
                        ev_timer_event_t *timerFrCounterEvt = app_switch[i].timerFrCounterEvt;
                        if (app_switch[i].timerFrCounterEvt) {
                            TL_ZB_TIMER_CANCEL(&timerFrCounterEvt);
                        }
                        app_switch[i].timerFrCounterEvt = TL_ZB_TIMER_SCHEDULE(switch_counterFrCb, (void*)((uint32_t)i), TIMEOUT_1SEC);
                        if (app_switch[i].counter == 1) {
#if UART_PRINTF_MODE && DEBUG_SWITCH
                            printf("Switch %d is ON\r\n", dev_relay.unit_relay[i].ep);
#endif
                            if (relay_settings.switchType[i] == ZCL_SWITCH_TYPE_MOMENTARY) {
                                if (relay_settings.switchActions[i] == ZCL_SWITCH_ACTION_OFF_ON) {
                                    remoteCmdOnOff(dev_relay.unit_relay[i].ep, ZCL_CMD_ONOFF_ON);
                                    if (relay_settings.switch_decoupled[i] == CUSTOM_SWITCH_DECOUPLED_OFF) {
                                        cmdOnOff_on(dev_relay.unit_relay[i].ep);
                                    }
                                }
                                else {
                                    remoteCmdOnOff(dev_relay.unit_relay[i].ep, ZCL_CMD_ONOFF_OFF);
                                    if (relay_settings.switch_decoupled[i] == CUSTOM_SWITCH_DECOUPLED_OFF) {
                                        cmdOnOff_off(dev_relay.unit_relay[i].ep);
                                    }
                                }
                            } else {
                                remoteCmdOnOff(dev_relay.unit_relay[i].ep, ZCL_CMD_ONOFF_TOGGLE);
                                if (relay_settings.switch_decoupled[i] == CUSTOM_SWITCH_DECOUPLED_OFF) {
                                    cmdOnOff_toggle(dev_relay.unit_relay[i].ep);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (app_switch[i].status != SWITCH_OFF) {
            if (app_switch[i].debounce != 1) {
                app_switch[i].debounce--;
                if (app_switch[i].debounce == 1) {
                    if (app_switch[i].status == SWITCH_FLOAT) {
                        switch_first_start(i, SWITCH_OFF);
                    } else {
                        app_switch[i].status = SWITCH_OFF;
                        if (!app_switch[i].timerFrCounterEvt) {
#if UART_PRINTF_MODE && DEBUG_SWITCH
                            printf("Switch %d is OFF\r\n", dev_relay.unit_relay[i].ep);
#endif
                            if (relay_settings.switchType[i] == ZCL_SWITCH_TYPE_MOMENTARY) {
                                if (relay_settings.switchActions[i] == ZCL_SWITCH_ACTION_OFF_ON) {
                                    remoteCmdOnOff(dev_relay.unit_relay[i].ep, ZCL_CMD_ONOFF_OFF);
                                    if (relay_settings.switch_decoupled[i] == CUSTOM_SWITCH_DECOUPLED_OFF) {
                                        cmdOnOff_off(dev_relay.unit_relay[i].ep);
                                    }
                                } else {
                                    remoteCmdOnOff(dev_relay.unit_relay[i].ep, ZCL_CMD_ONOFF_ON);
                                    if (relay_settings.switch_decoupled[i] == CUSTOM_SWITCH_DECOUPLED_OFF) {
                                        cmdOnOff_on(dev_relay.unit_relay[i].ep);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static void read_switch_multifunction(uint8_t i) {

    zcl_msInputAttr_t *msInputAttr = zcl_msInputAttrsGet();
    msInputAttr += i;

    if (!drv_gpio_read(dev_relay.unit_relay[i].sw)) {
        if (app_switch[i].status == SWITCH_ON) {
            if (clock_time_exceed(app_switch[i].time_hold, TIMEOUT_TICK_500MS)) {
                if (!app_switch[i].hold) {
                    app_switch[i].hold = true;
//                    printf("on. hold: %d\r\n", app_switch[i].time_hold);
                }
            }
        } else {
            if (app_switch[i].debounce != DEBOUNCE_SWITCH) {
                app_switch[i].debounce++;
                if (app_switch[i].debounce == DEBOUNCE_SWITCH) {
                    if (app_switch[i].status == SWITCH_FLOAT) {
                        app_switch[i].counter = ACTION_CLEAR;
                        app_switch[i].status = SWITCH_ON;
                        switch_counterMfCb((void*)((uint32_t)i));
                    } else {
                        app_switch[i].status = SWITCH_ON;
                        app_switch[i].counter++;
                        ev_timer_event_t *timerMfCounterEvt = app_switch[i].timerMfCounterEvt;
                        if (app_switch[i].timerMfCounterEvt) {
                            TL_ZB_TIMER_CANCEL(&timerMfCounterEvt);
                        }
                        app_switch[i].timerMfCounterEvt = TL_ZB_TIMER_SCHEDULE(switch_counterMfCb, (void*)((uint32_t)i), TIMEOUT_500MS);
                    }
                }
            }
        }
    } else {
        if (app_switch[i].status == SWITCH_OFF) {
            app_switch[i].time_hold = clock_time();
            if (app_switch[i].hold) {
                app_switch[i].hold = false;
                msInputAttr->value = ACTION_RELEASE;
#if UART_PRINTF_MODE && DEBUG_SWITCH
                printf("Switch %d is Release\r\n", dev_relay.unit_relay[i].ep);
#endif
                app_forcedReport(dev_relay.unit_relay[i].ep, ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC, ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE);
            }
        } else {
            if (app_switch[i].debounce != 1) {
                app_switch[i].debounce--;
                if (app_switch[i].debounce == 1) {
                    if (app_switch[i].status == SWITCH_FLOAT) {
                        app_switch[i].counter = ACTION_CLEAR;
                        app_switch[i].status = SWITCH_OFF;
                        switch_counterMfCb((void*)((uint32_t)i));
                    } else {
                        app_switch[i].status = SWITCH_OFF;
                    }
                }
            }
        }
    }

}

void switch_handler() {

    for (uint8_t idx = 0; idx < AMT_RELAY; idx ++) {
        if (relay_settings.switchType[idx] == ZCL_SWITCH_TYPE_MULTIFUNCTION) {
            read_switch_multifunction(idx);
        } else {
            read_switch_toggle(idx);
        }
    }

}

bool switch_idle() {
    for (uint8_t i = 0; i < dev_relay.amt; i++) {
        if (app_switch[i].debounce != 1 && app_switch[i].debounce != DEBOUNCE_SWITCH) return true;
        if (app_switch[i].timerFrCounterEvt) return true;
    }

    return false;
}

