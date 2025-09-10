#ifndef SRC_INCLUDE_APP_RELAY_H_
#define SRC_INCLUDE_APP_RELAY_H_

typedef struct __attribute__((packed)) {
    uint8_t status_onoff;
    uint8_t startUpOnOff;
    uint8_t switchType;         // 0x00 - toggle, 0x01 - momentary
    uint8_t switchActions;
    uint8_t crc;
} relay_settings_t;

extern relay_settings_t relay_settings;

bool get_relay_status();
void set_relay_status(uint8_t status);

nv_sts_t relay_settings_save();
nv_sts_t relay_settings_restore();

#endif /* SRC_INCLUDE_APP_RELAY_H_ */
