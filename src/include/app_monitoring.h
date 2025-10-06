#ifndef SRC_INCLUDE_APP_MONITORING_H_
#define SRC_INCLUDE_APP_MONITORING_H_

#define HEAD            0x55
#define DATA_MAX_LEN    32

typedef struct __attribute__((packed)) {
    uint8_t     head;
    uint8_t     i_rms_0;
    uint8_t     i_rms_1;
    uint8_t     i_rms_2;
    uint8_t     v_rms_0;
    uint8_t     v_rms_1;
    uint8_t     v_rms_2;
    uint8_t     i_fast_rms_0;
    uint8_t     i_fast_rms_1;
    uint8_t     i_fast_rms_2;
    uint8_t     watt_0;
    uint8_t     watt_1;
    uint8_t     watt_2;
    uint8_t     cf_cnt_0;
    uint8_t     cf_cnt_1;
    uint8_t     cf_cnt_2;
    uint8_t     freq_0;
    uint8_t     freq_1;
    uint8_t     freq_2;
    uint8_t     status_0;
    uint8_t     status_1;
    uint8_t     status_2;
    uint8_t     crc;
} app_monitoring_t;

#define PKT_SIZE    sizeof(app_monitoring_t)

void send_uart_commandCb(void *args);
int32_t app_monitoringCb(void *arg);
void monitoring_handler();

#endif /* SRC_INCLUDE_APP_MONITORING_H_ */
