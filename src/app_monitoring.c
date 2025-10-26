#include "app_main.h"

// This REF get from https://github.com/esphome/esphome/blob/dev/esphome/components/bl0942/bl0942.h
#define BL0942_POWER_REF        596
#define BL0942_VOLTAGE_REF      15873.35944299
#define BL0942_CURRENT_REF      251213.46469622
#define BL0942_ENERGY_REF       3304.61127328

#define ID_ENERGY               0x0FED1410
#define TOP_MASK                0xFFFFFFFF

static uint8_t  pkt_out[2] = {0x58, 0xAA};
static uint8_t  pkt_in[PKT_SIZE] = {0};
static uint16_t current, voltage, freq;
static int16_t  power;
static uint64_t cur_sum_delivered;
static uint32_t new_energy, old_energy = 0;
static uint8_t  default_energy_cons = false;
static uint8_t  first_start = true;
static energy_cons_t energy_cons = {0};
static bool new_energy_save = false;

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

    for(uint8_t i = 0; i < (length - 1); i++) {
        crc8 += data[i];
    }

    crc8 += 0x58;

    return ~crc8;
}

static void send_uart_commandCb(void *args) {

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

static void clear_user_data(uint32_t flash_addr) {

    uint32_t flash_data_size = flash_addr + USER_DATA_SIZE;

    while(flash_addr < flash_data_size) {
        flash_erase_sector(flash_addr);
        flash_addr += FLASH_SECTOR_SIZE;
    }
}

static void energy_saveCb(void *args) {

    light_blink_start(1, 250, 250);

    if (default_energy_cons) {
        energy_cons.crc = checksum((uint8_t*)&(energy_cons), sizeof(energy_cons_t));
        flash_erase(energy_cons.flash_addr_start);
        flash_write(energy_cons.flash_addr_start, sizeof(energy_cons_t), (uint8_t*)&(energy_cons));
        default_energy_cons = false;
#if UART_PRINTF_MODE && DEBUG_SAVE
        printf("Save energy_cons to flash address - 0x%x\r\n", energy_cons.flash_addr_start);
#endif /* UART_PRINTF_MODE */
    } else {
        energy_cons.flash_addr_start += FLASH_PAGE_SIZE;
        if (energy_cons.flash_addr_start == END_USER_DATA) {
            energy_cons.flash_addr_start = BEGIN_USER_DATA;
        }
        if (energy_cons.flash_addr_start % FLASH_SECTOR_SIZE == 0) {
            flash_erase(energy_cons.flash_addr_start);
        }
        energy_cons.top++;
        energy_cons.top &= TOP_MASK;
        energy_cons.crc = checksum((uint8_t*)&(energy_cons), sizeof(energy_cons_t));
        flash_write(energy_cons.flash_addr_start, sizeof(energy_cons_t), (uint8_t*)&(energy_cons));
#if UART_PRINTF_MODE && DEBUG_SAVE
        printf("Save energy_cons to flash address - 0x%x\r\n", energy_cons.flash_addr_start);
#endif /* UART_PRINTF_MODE */

    }

    new_energy_save = false;
}


static void init_default_energy_cons() {
    clear_user_data(BEGIN_USER_DATA);
    memset(&energy_cons, 0, sizeof(energy_cons_t));
    energy_cons.id = ID_ENERGY;
    energy_cons.flash_addr_start = BEGIN_USER_DATA;
    energy_cons.flash_addr_end = END_USER_DATA;
    g_zcl_seAttrs.cur_sum_delivered = 0;
    default_energy_cons = true;
    energy_saveCb(NULL);
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
            if (checksum(pkt_in, PKT_SIZE) == pkt->crc) {
                current = (uint16_t)((float)(pkt->i_rms/BL0942_CURRENT_REF*100.0));
                voltage = (uint16_t)((float)(pkt->v_rms/BL0942_VOLTAGE_REF*100.0));
                power = (uint16_t)((float)(pkt->watt/BL0942_POWER_REF*100.0));
                freq = (uint16_t)((float)(1000000.0/pkt->freq*100.0));
                new_energy = (uint32_t)((float)(pkt->cf_cnt/BL0942_ENERGY_REF*100.0));

#if UART_PRINTF_MODE && DEBUG_MONITORING
                printf("current_adc: %d,%s current: %d\r\n", pkt->i_rms, pkt->i_rms > 9?"\t":"\t\t", current);
                printf("voltage_adc: %d,%s voltage: %d\r\n", pkt->v_rms, pkt->v_rms > 9?"\t":"\t\t", voltage);
                printf("power_adc:   %d,%s power:   %d\r\n", pkt->watt, pkt->watt > 9?"\t":"\t\t", power);
                printf("freq_adc:    %d,%s freq:    %d\r\n", pkt->freq, pkt->freq > 9?"\t":"\t\t", freq);
                printf("energy_adc:  %d,%s energy:  %d\r\n", pkt->cf_cnt, pkt->cf_cnt > 9?"\t":"\t\t", new_energy);
                printf("new_energy:  %d,%s old_en:  %d\r\n", new_energy, new_energy > 9?"\t":"\t\t", old_energy);
#endif
                if (first_start) {
#if UART_PRINTF_MODE && DEBUG_MONITORING
                    printf("first start\r\n");
#endif
                    first_start = false;
                    old_energy = new_energy;
                    return;
                }

                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_RMS_VOLTAGE, (uint8_t*)&voltage);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_RMS_CURRENT, (uint8_t*)&current);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_ACTIVE_POWER, (uint8_t*)&power);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_ATTRID_AC_FREQUENCY, (uint8_t*)&freq);

                if (new_energy > old_energy) {
//                    printf("new_energy: %d > old_energy: %d\r\n", new_energy, old_energy);
                    cur_sum_delivered = energy_cons.energy + (new_energy - old_energy);
                    old_energy = new_energy;
                    energy_cons.energy = cur_sum_delivered;
                    energy_save();
                    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SE_METERING, ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD, (uint8_t*)&cur_sum_delivered);
                }
            }
        }
    }
}

void energy_restore() {

    energy_cons_t energy_curr, energy_next;
    uint8_t find_config = false;

    uint32_t flash_addr = BEGIN_USER_DATA;

    flash_read_page(flash_addr, sizeof(energy_cons_t), (uint8_t*)&energy_curr);

    if (energy_curr.id != ID_ENERGY || checksum((uint8_t*)&energy_curr, sizeof(energy_cons_t)) != energy_curr.crc) {
#if UART_PRINTF_MODE && DEBUG_SAVE
        printf("No saved energy_cons! Init.\r\n");
#endif /* UART_PRINTF_MODE */
        init_default_energy_cons();
        return;
    }

    flash_addr += FLASH_PAGE_SIZE;

    while(flash_addr < END_USER_DATA) {
        flash_read_page(flash_addr, sizeof(energy_cons_t), (uint8_t*)&energy_next);
        if (energy_next.id == ID_ENERGY && checksum((uint8_t*)&energy_next, sizeof(energy_cons_t)) == energy_next.crc) {
            if ((energy_curr.top + 1) == energy_next.top || (energy_curr.top == TOP_MASK && energy_next.top == 0)) {
                memcpy(&energy_curr, &energy_next, sizeof(energy_cons_t));
                flash_addr += FLASH_PAGE_SIZE;
                continue;
            }
            find_config = true;
            break;
        }
        find_config = true;
        break;
    }

    if (find_config) {
        memcpy(&energy_cons, &energy_curr, sizeof(energy_cons_t));
        energy_cons.flash_addr_start = flash_addr-FLASH_PAGE_SIZE;
        g_zcl_seAttrs.cur_sum_delivered = energy_cons.energy;
#if UART_PRINTF_MODE && DEBUG_SAVE
        printf("Read config from flash address - 0x%x\r\n", energy_cons.flash_addr_start);
#endif /* UART_PRINTF_MODE */
    } else {
#if UART_PRINTF_MODE && DEBUG_SAVE
        printf("No active saved energy_cons! Reinit.\r\n");
#endif /* UART_PRINTF_MODE */
        init_default_energy_cons();
    }

}

void energy_save() {

    new_energy_save = true;
}

int32_t energy_timerCb(void *args) {

    if (new_energy_save) {
        TL_SCHEDULE_TASK(energy_saveCb, NULL);
    }

    return 0;
}

void energy_remove() {

#if UART_PRINTF_MODE && DEBUG_SAVE
        printf("Energy removed\r\n");
#endif /* UART_PRINTF_MODE */

    init_default_energy_cons();
}
