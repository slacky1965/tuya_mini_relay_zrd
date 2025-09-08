#ifndef SRC_INCLUDE_APP_DEV_CONFIG_H_
#define SRC_INCLUDE_APP_DEV_CONFIG_H_

typedef struct __attribute__((packed)) {
    uint8_t status_onoff;
    uint8_t startUpOnOff;
    uint8_t switchType;         // 0x00 - toggle, 0x01 - momentary
    uint8_t switchActions;
} dev_config_t;

extern dev_config_t *config;
nv_sts_t config_restore();
nv_sts_t config_save();

#endif /* SRC_INCLUDE_APP_DEV_CONFIG_H_ */
