#include "app_main.h"

#define DEBOUNCE_SWITCH     8       /* number of polls for debounce                 */
#define FR_COUNTER_MAX      8       /* number for factory reset                     */

typedef struct {
    ev_timer_event_t *timerCounterEvt;
    bool    status;                 /* true - switch is on, false - switch is off   */
    uint8_t debounce;
    uint8_t counter;
    uint8_t fr_counter;             /* factory reset counter                        */
} app_switch_t;

static app_switch_t app_switch_cfg = {
    .status = false,
    .debounce = 1,
    .counter = 0,
    .fr_counter = 0,
    .timerCounterEvt = NULL,
};

static app_switch_t *app_switch = &app_switch_cfg;
//static bool factory_reset = false;

static int32_t net_steer_start_offCb(void *args) {

    g_appCtx.net_steer_start = false;
//    factory_reset = false;

    light_blink_stop();

    return -1;
}

static void switch_factory_reset_start(void *args) {

    printf("button_factory_reset_start\r\n");

    zb_factoryReset();

    g_appCtx.net_steer_start = true;
    TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
    light_blink_start(90, 250, 750);

}


static int32_t switch_counterCb(void *args) {

    if (app_switch->counter >= FR_COUNTER_MAX) {
        printf("Factory reset\r\n");
        TL_SCHEDULE_TASK(switch_factory_reset_start, NULL);
    }

    if (!app_switch->status) {
        if (config->switchType == ZCL_SWITCH_TYPE_MOMENTARY) {
            printf("FR Switch is OFF\r\n");
            cmdOnOff_off(APP_ENDPOINT1);
        }
    }

    app_switch->counter = 0;
    app_switch->timerCounterEvt = NULL;

    return -1;
}

void switch_handler() {


    if (drv_gpio_read(SWITCH_GPIO)) {
        if (!app_switch->status) {
            if (app_switch->debounce != DEBOUNCE_SWITCH) {
                app_switch->debounce++;
                if (app_switch->debounce == DEBOUNCE_SWITCH) {
                    app_switch->status = true;
                    app_switch->counter++;
                    if (app_switch->timerCounterEvt) {
                        TL_ZB_TIMER_CANCEL(&app_switch->timerCounterEvt);
                    }
                    app_switch->timerCounterEvt = TL_ZB_TIMER_SCHEDULE(switch_counterCb, NULL, TIMEOUT_1SEC);
                    if (app_switch->counter == 1) {
#if UART_PRINTF_MODE && DEBUG_SWITCH
                        printf("Switch is ON\r\n");
#endif
                        if (config->switchType == ZCL_SWITCH_TYPE_MOMENTARY) {
                            cmdOnOff_on(APP_ENDPOINT1);
                        } else {
                            cmdOnOff_toggle(APP_ENDPOINT1);
                        }
                    }
                }
            }
        }
    } else {
        if (app_switch->status) {
            if (app_switch->debounce != 1) {
                app_switch->debounce--;
                if (app_switch->debounce == 1) {
                    app_switch->status = false;
                    if (!app_switch->timerCounterEvt) {
#if UART_PRINTF_MODE && DEBUG_SWITCH
                        printf("Switch is OFF\r\n");
#endif
                        if (config->switchType == ZCL_SWITCH_TYPE_MOMENTARY) {
                            cmdOnOff_off(APP_ENDPOINT1);
                        }
                    }
                }
            }
        }
    }
}

bool switch_idle() {
    uint8_t idle = false;;

    if (app_switch->debounce != 1 && app_switch->debounce != DEBOUNCE_SWITCH) idle = true;

    if (app_switch->timerCounterEvt) idle = true;

    return idle;
}

