#include "app_main.h"

#define BL0942_POWER_REF        596
#define BL0942_VOLTAGE_REF      15873.35944299
#define BL0942_CURRENT_REF      251213.46469622
#define BL0942_ENERGY_REF       3304.61127328

static uint8_t pkt_out[2] = {0x58, 0xAA};
static uint8_t pkt_in[PKT_SIZE] = {0};
static uint32_t current_adc, voltage_adc, power_adc, freq_adc, energy_adc;
static uint16_t current, voltage, power, freq;
static uint64_t tariff_summ;

#if UART_PRINTF_MODE && DEBUG_PACKAGE
void static print_package(uint8_t *head, uint8_t *buff, size_t len) {

    uint8_t ch;

    if (len) {
        printf("%s. len: %d, data: 0x", head, len);

        for (int i = 0; i < len; i++) {
            ch = buff[i];
            if (ch < 0x10) {
                printf("0%x", ch);
            } else {
                printf("%x", ch);
            }
        }
    } else {
        printf("%s. len: %d", head, len);
    }
    printf("\r\n");

}
#endif


static uint8_t checksum(uint8_t *data, uint16_t length) {

    uint8_t crc8 = 0;

    for(uint8_t i = 0; i < length; i++) {
        crc8 += data[i];
    }

    crc8 += 0x58;

    return ~crc8;
}

void send_uart_commandCb(void *args) {

    uint8_t buff_len = sizeof(pkt_out), len = 0;

    flush_ring_buff();

    /* three attempts to write to uart */
    for (uint8_t attempt = 0; attempt < 3; attempt++) {
        len = write_bytes_to_uart(pkt_out, buff_len);
        if (len == buff_len) {
            break;
        } else {
            len = 0;
        }
#if UART_PRINTF_MODE
        printf("Attempt to send data to uart: %d\r\n", attempt+1);
#endif
        sleep_ms(50);
    }

#if UART_PRINTF_MODE && DEBUG_PACKAGE
    if (len == 0) {
        uint8_t head[] = "write to uart error";
        print_package(head, pkt_out, buff_len);
    } else {
        uint8_t head[] = "write to uart";
        print_package(head, pkt_out, len);
    }
#endif

//    return len;
}

int32_t app_monitoringCb(void *arg) {

    TL_SCHEDULE_TASK(send_uart_commandCb, NULL);

    return 0;
}


void monitoring_handler() {

    size_t load_size = 0;
    uint8_t ch, complete = false;

    app_monitoring_t *pkt = (app_monitoring_t*)pkt_in;

    memset(pkt_in, 0, PKT_SIZE);

    if (available_ring_buff()) {
        load_size = 0;

        while (available_ring_buff() && load_size < DATA_MAX_LEN) {

            ch = read_byte_from_ring_buff();

            if (load_size == 0) {
                if (ch != HEAD) {
                    continue;
                }
            }

            pkt_in[load_size++] = ch;

            if (load_size == 1) {

                load_size += read_bytes_from_buff(pkt_in+load_size, PKT_SIZE-load_size);

                if (load_size == PKT_SIZE) {
#if UART_PRINTF_MODE && DEBUG_PACKAGE
                    uint8_t head[] = "read from uart";
                    print_package(head, pkt_in, load_size);
#endif
                    complete = true;
                    break;
                } else {
                    load_size = 0;
                    continue;
                }
            }
        }

        if (complete) {
            if (checksum(pkt_in, PKT_SIZE-1) == pkt->crc) {
                current_adc = 0x00000000 | (uint32_t)pkt->i_rms_2 << 16 | (uint32_t)pkt->i_rms_1 << 8 | (uint32_t)pkt->i_rms_0;
                voltage_adc = 0x00000000 | (uint32_t)pkt->v_rms_2 << 16 | (uint32_t)pkt->v_rms_1 << 8 | (uint32_t)pkt->v_rms_0;
                power_adc = 0x00000000 | (uint32_t)pkt->watt_2 << 16 | (uint32_t)pkt->watt_1 << 8 | (uint32_t)pkt->watt_0;
                freq_adc = 0x00000000 | (uint32_t)pkt->freq_2 << 16 | (uint32_t)pkt->freq_1 << 8 | (uint32_t)pkt->freq_0;
                energy_adc = 0x00000000 | (uint32_t)pkt->cf_cnt_2 << 16 | (uint32_t)pkt->cf_cnt_1 << 8 | (uint32_t)pkt->cf_cnt_0;

                current = (uint16_t)((float)(current_adc/BL0942_CURRENT_REF*100.0));
                voltage = (uint16_t)((float)(voltage_adc/BL0942_VOLTAGE_REF*100.0));
                power = (uint16_t)((float)(power_adc/BL0942_POWER_REF*100.0));
                freq = (uint16_t)((float)(1000000.0/freq_adc*100.0));
                tariff_summ = (uint64_t)((float)(energy_adc/BL0942_ENERGY_REF*100.0));

//                printf("current_adc: %d, current: %d\r\n", current_adc, current);
//                printf("voltage_adc: %d, voltage: %d\r\n", voltage_adc, voltage);
//                printf("power_adc: %d, power: %d\r\n", power_adc, power);
//                printf("freq_adc: %d, freq: %d\r\n", freq_adc, freq);
//                printf("energy_adc: %d, energy: %d\r\n", energy_adc, tariff_summ);

                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_RMS_VOLTAGE, (uint8_t*)&voltage);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_RMS_CURRENT, (uint8_t*)&current);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_ACTIVE_POWER, (uint8_t*)&power);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_AC_FREQUENCY, (uint8_t*)&freq);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD, (uint8_t*)&tariff_summ);
            }
        }
    }
}
