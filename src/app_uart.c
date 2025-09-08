#include "app_main.h"

uart_data_t rec_buff = {0,  {0, } };
uint8_t  ring_buff[RING_BUFF_SIZE];
uint16_t ring_head, ring_tail;

uint8_t available_ring_buff() {
    if (ring_head != ring_tail) {
        return true;
    }
    return false;
}

size_t get_queue_len_ring_buff() {
   return (ring_head - ring_tail) & (RING_BUFF_MASK);
}

static size_t get_freespace_ring_buff() {
    return (sizeof(ring_buff)/sizeof(ring_buff[0]) - get_queue_len_ring_buff());
}

void flush_ring_buff() {
    ring_head = ring_tail = 0;
    memset(ring_buff, 0, RING_BUFF_SIZE);
}

uint8_t read_byte_from_ring_buff() {
    uint8_t ch = ring_buff[ring_tail++];
    ring_tail &= RING_BUFF_MASK;
    return ch;

}

size_t read_bytes_from_buff(uint8_t *str, size_t len) {

    size_t i = 0;

    while (i < len) {
        /* check for empty buffer */
        if (get_queue_len_ring_buff()) {
            str[i++] = read_byte_from_ring_buff();
        } else {
            break;
        }
    }

//    for (i = 0; i < len; i++) {
//        str[i] = read_byte_from_ring_buff();
//    }

//    printf("read_bytes_from_buff(). len: %d, i: %d\r\n", len, i);

    return i;
}

static size_t write_bytes_to_ring_buff(uint8_t *data, size_t len) {

    size_t free_space = get_freespace_ring_buff();
    size_t put_len;

//    printf("free_space: %d, len: %d\r\n", free_space, len);

    if (free_space >= len) put_len = len;
    else put_len = free_space;

    for (int i = 0; i < put_len; i++) {
        ring_buff[ring_head++] = data[i];
        ring_head &= RING_BUFF_MASK;
    }

    return put_len;
}

static void app_uartRecvCb() {

    uint8_t write_len;

    write_len = write_bytes_to_ring_buff(rec_buff.data, rec_buff.dma_len);

    if (write_len != 0) {
        if (write_len == rec_buff.dma_len) {
            rec_buff.dma_len = 0;
        } else if (write_len < rec_buff.dma_len) {
            memcpy(rec_buff.data, rec_buff.data+write_len, rec_buff.dma_len-write_len);
            rec_buff.dma_len -= write_len;
        }
    }
}

void app_uart_init() {

    uint32_t baudrate = BAUDRATE_UART;

    flush_ring_buff();
    drv_uart_pin_set(GPIO_UART_TX, GPIO_UART_RX);

    drv_uart_init(baudrate, (uint8_t*)&rec_buff, sizeof(uart_data_t), app_uartRecvCb);
}

void app_uart_reinit() {

    uint32_t baudrate = BAUDRATE_UART;

    drv_uart_pin_set(GPIO_UART_TX, GPIO_UART_RX);

    drv_uart_init(baudrate, (uint8_t*)&rec_buff, sizeof(uart_data_t), app_uartRecvCb);
}


size_t write_bytes_to_uart(uint8_t *data, size_t len) {

    if (len > UART_DATA_LEN) len = UART_DATA_LEN;

    if (drv_uart_tx_start(data, len)) return len;

    return 0;
}

void app_uart_rx_off() {

    drv_gpio_input_en(GPIO_UART_RX, false);
}

