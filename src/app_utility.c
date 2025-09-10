#include "app_main.h"

#define EULER_CONST 2.718281828459045235
#define TAYLOR_ITERATIONS 30

double nat_log(double x) {
    // Trap illegal values
    if (x <= 0) {
        return 0.0/0.0;  // NaN
    }

    // Confine x to a sensible range
    int power_adjust = 0;
    while (x > 1.0) {
        x /= EULER_CONST;
        power_adjust++;
    }
    while (x < .25) {
        x *= EULER_CONST;
        power_adjust--;
    }

    // Now use the Taylor series to calculate the logarithm
    x -= 1.0;
    double t = 0.0, s = 1.0, z = x;
    for (int k=1; k<=TAYLOR_ITERATIONS; k++) {
        t += z * s / k;
        z *= x;
        s = -s;
    }

    // Combine the result with the power_adjust value and return
    return t + power_adjust;
}

int32_t delayedMcuResetCb(void *arg) {

    //printf("mcu reset\r\n");
    zb_resetDevice();
    return -1;
}

int32_t delayedFactoryResetCb(void *arg) {

    printf("delayedFactoryResetCb\r\n");
    zb_resetDevice2FN();
    zb_deviceFactoryNewSet(true);
//    zb_factoryReset();
//    zb_resetDevice();
    return -1;
}

int32_t delayedFullResetCb(void *arg) {

    //printf("full reset\r\n");
    return -1;
}

/* Function return size of string and convert signed  *
 * integer to ascii value and store them in array of  *
 * character with NULL at the end of the array        */

uint32_t itoa(uint32_t value, uint8_t *ptr) {
    uint32_t count = 0, temp;
    if(ptr == NULL)
        return 0;
    if(value == 0)
    {
        *ptr = '0';
        return 1;
    }

    if(value < 0)
    {
        value *= (-1);
        *ptr++ = '-';
        count++;
    }

    for(temp=value; temp>0; temp/=10, ptr++);
        *ptr = '\0';

    for(temp=value; temp>0; temp/=10)
    {
        *--ptr = temp%10 + '0';
        count++;
    }
    return count;
}

uint8_t *digit64toString(uint64_t value) {
    static uint8_t buff[32] = {0};
    uint8_t *buffer = buff;
    buffer += 17;
    *--buffer = 0;
    do {
        *--buffer = value % 10 + '0';
        value /= 10;
    } while (value != 0);

    return buffer;
}

uint64_t atoi(uint16_t len, uint8_t *data) {

    uint64_t value = 0, mp;
    uint8_t ch;

    if (len > 16) len = 16;

    uint16_t v_len = len - 1;

    for (uint8_t i = 0; i < len; i++, v_len--) {
        ch = data[i];
        if (ch >= '0' && ch <= '9') {
            mp = ch - '0';
            for (uint8_t ii = 0; ii < v_len; ii++) {
                mp = mp * 10;
            }
            value += mp;
        } else {
            break;
        }
    }
    return value;
}

uint32_t from24to32(const uint8_t *str) {

    uint32_t value;

    value = str[0] & 0xff;
    value |= (str[1] << 8) & 0xff00;
    value |= (str[2] << 16) & 0xff0000;

    return value;
}

uint64_t fromPtoInteger(uint16_t len, uint8_t *data) {

    uint64_t value = 0;

    if (len > 8) len = 8;

    for (uint8_t i = 0; i < len; i++) {
        value |= (uint64_t)data[i] << (i*8);
    }

    return value;
}

uint8_t set_zcl_str(uint8_t *str_in, uint8_t *str_out, uint8_t len) {
    uint8_t *data = str_out;
    uint8_t *str_len = data;
    uint8_t *str_data = data+1;

    for (uint8_t i = 0; *(str_in+i) != 0 && i < (len-1); i++) {
        *(str_data+i) = *(str_in+i);
        (*str_len)++;
    }

    return *str_len;
}

uint64_t reverse64(uint64_t in) {
    uint64_t out;
    uint8_t *source = (uint8_t*)&in;
    uint8_t *destination = (uint8_t*)&out;

    destination[7] = source[0];
    destination[6] = source[1];
    destination[5] = source[2];
    destination[4] = source[3];
    destination[3] = source[4];
    destination[2] = source[5];
    destination[1] = source[6];
    destination[0] = source[7];

    return out;
}

uint32_t reverse32(uint32_t in) {
    uint32_t out;
    uint8_t *source = (uint8_t*)&in;
    uint8_t *destination = (uint8_t*)&out;

    destination[3] = source[0];
    destination[2] = source[1];
    destination[1] = source[2];
    destination[0] = source[3];

    return out;
}

uint16_t reverse16(uint16_t in) {
    uint16_t out;
    uint8_t *source = (uint8_t*)&in;
    uint8_t *destination = (uint8_t*)&out;

    destination[1] = source[0];
    destination[0] = source[1];

    return out;
}

uint8_t *print_str_zcl(uint8_t *str_zcl) {
    static uint8_t str[32+1];
    uint8_t len = *str_zcl;

    if (len == 0) {
        *str = 0;
    } else {
        if (len > 32) len = 32;

        for (uint8_t i = 0; i < len; i++) {
            str[i] = str_zcl[i+1];
        }

        str[len] = 0;
    }

    return str;
}

#ifdef ZCL_OTA
uint32_t mcuBootAddrGet(void);
#endif

void start_message() {

#ifdef ZCL_OTA
#if UART_PRINTF_MODE
        printf("OTA mode enabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif /* UART_PRINTF_MODE */
#else
#if UART_PRINTF_MODE
    printf("OTA mode desabled. MCU boot from address: 0\r\n");
#endif /* UART_PRINTF_MODE */
#endif

#if UART_PRINTF_MODE
    const uint8_t version[] = ZCL_BASIC_SW_BUILD_ID;
    printf("Firmware version: %s\r\n", version+1);
#endif
}

