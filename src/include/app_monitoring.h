#ifndef SRC_INCLUDE_APP_MONITORING_H_
#define SRC_INCLUDE_APP_MONITORING_H_

#define HEAD            0x55
#define DATA_MAX_LEN    32

typedef struct __attribute__((packed)) {
    uint8_t     head;
    uint32_t    i_rms       :24;
    uint32_t    v_rms       :24;
    uint32_t    i_fast_rms  :24;
    int32_t     watt        :24;
    uint32_t    cf_cnt      :24;
    uint16_t    freq;
    uint8_t     resv1;
    uint8_t     status;
    uint8_t     resv2;
    uint8_t     resv3;
    uint8_t     crc;
} app_monitoring_t;

#define PKT_SIZE    sizeof(app_monitoring_t)

void send_uart_commandCb(void *args);
int32_t app_monitoringCb(void *arg);
void monitoring_handler();

#endif /* SRC_INCLUDE_APP_MONITORING_H_ */
