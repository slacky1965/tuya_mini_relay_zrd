#ifndef SRC_INCLUDE_APP_UART_H_
#define SRC_INCLUDE_APP_UART_H_

#define UART_DATA_LEN   188
#define RING_BUFF_SIZE  512               /* size ring buffer  */
#define RING_BUFF_MASK  RING_BUFF_SIZE-1  /* mask ring buffer  */

typedef enum {
    UART_TX_SUCCESS,
    UART_TX_BUFFERFULL,
    UART_TX_BUSY,
    UART_TX_FAILED,
    UART_TX_TOO_LONG,
} uartTx_err;

typedef struct {
    uint32_t dma_len;        // dma len must be 4 byte
    uint8_t  data[UART_DATA_LEN];
} uart_data_t;

void app_uart_init();
size_t write_bytes_to_uart(uint8_t *data, size_t len);
uint8_t read_byte_from_ring_buff();
size_t read_bytes_from_buff(uint8_t *str, size_t len);
uint8_t available_ring_buff();
size_t get_queue_len_ring_buff();
void flush_ring_buff();

#endif /* SRC_INCLUDE_APP_UART_H_ */
