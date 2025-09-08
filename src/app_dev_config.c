#include "app_main.h"

static dev_config_t dev_config;

dev_config_t *config = &dev_config;

nv_sts_t config_restore() {
    nv_sts_t st = NV_SUCC;

    config->status_onoff = OFF;

    return st;
}

nv_sts_t config_save() {
    nv_sts_t st = NV_SUCC;

    return st;
}

