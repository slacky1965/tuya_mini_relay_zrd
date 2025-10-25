#ifndef SRC_INCLUDE_APP_RELAY_H_
#define SRC_INCLUDE_APP_RELAY_H_

#define AMT_RELAY   1

typedef struct __attribute__((packed)) {
    uint8_t  status_onoff[AMT_RELAY];
    uint8_t  startUpOnOff[AMT_RELAY];
    uint8_t  switchType[AMT_RELAY];         // 0x00 - toggle, 0x01 - momentary, 0x02 - multifunction
    uint8_t  switchActions[AMT_RELAY];
    uint8_t  switch_decoupled[AMT_RELAY];
    uint8_t  ota;
    uint64_t energy;
    uint8_t  crc;
} relay_settings_t;

typedef struct {
    GPIO_PinTypeDef sw;
    GPIO_PinTypeDef rl;
    uint8_t         ep;
} unit_relay_t;

typedef struct {
    uint8_t         amt;
    unit_relay_t    unit_relay[AMT_RELAY];
} dev_relay_t;

extern relay_settings_t relay_settings;
extern dev_relay_t      dev_relay;

bool get_relay_status();
void set_relay_status(uint8_t i, uint8_t status);

nv_sts_t relay_settings_save();
nv_sts_t relay_settings_restore();

void dev_relay_init();

#endif /* SRC_INCLUDE_APP_RELAY_H_ */
