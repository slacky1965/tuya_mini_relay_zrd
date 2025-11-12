#ifndef SRC_INCLUDE_APP_RELAY_H_
#define SRC_INCLUDE_APP_RELAY_H_

#define AMT_RELAY   1

#define PROTECT_CONTROL_OFF     0x00
#define PROTECT_CONTROL_ON      0x01
#define AUTORESTART_OFF         0x00
#define AUTORESTART_ON          0x01
#define TIME_RELOAD_MIN         5                   // sec
#define TIME_RELOAD_MAX         60                  // sec
#define VOLTAGE_MID             22000               // 220V

#define DEFAULT_CURRENT_MAX     1600                // 16A
#define DEFAULT_POWER_MAX       3600                // 3600W
#define DEFAULT_VOLTAGE_MIN     18000               // 180V
#define DEFAULT_VOLTAGE_MAX     26000               // 260V
#define DEFAULT_TIME_RELOAD     TIME_RELOAD_MIN     // 5sec
#define DEFAULT_PROTECT_CONTROL PROTECT_CONTROL_OFF // 0 - off, 1 - on
#define DEFAULT_AUTORESTART     AUTORESTART_OFF     // 0 - off, 1 - on

typedef struct __attribute__((packed)) {
    uint8_t  status_onoff[AMT_RELAY];
    uint8_t  startUpOnOff[AMT_RELAY];
    uint8_t  switchType[AMT_RELAY];                 // 0x00 - toggle, 0x01 - momentary, 0x02 - multifunction
    uint8_t  switchActions[AMT_RELAY];
    uint8_t  switch_decoupled[AMT_RELAY];
    uint16_t current_max;
    int16_t  power_max;
    uint16_t voltage_min;
    uint16_t voltage_max;
    uint16_t time_reload;
    uint8_t  protect_control;
    uint8_t  auto_restart;
    uint8_t  crc;
} relay_settings_t;

typedef struct {
    GPIO_PinTypeDef sw;
    GPIO_PinTypeDef rl;
    uint8_t         ep;
} unit_relay_t;

typedef struct {
    ev_timer_event_t *timerAutoRestartEvt; //[AMT_RELAY];
    uint8_t         amt;
    unit_relay_t    unit_relay[AMT_RELAY];
} dev_relay_t;

extern relay_settings_t relay_settings;
extern dev_relay_t      dev_relay;

bool get_relay_status(uint8_t i);
void set_relay_status(uint8_t i, uint8_t status);

nv_sts_t relay_settings_save();
nv_sts_t relay_settings_restore();
void relay_settints_default();

void dev_relay_init();

#endif /* SRC_INCLUDE_APP_RELAY_H_ */
