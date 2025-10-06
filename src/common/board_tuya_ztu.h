//#ifndef SRC_INCLUDE_BOARD_TUYA_ZTU_H_
//#define SRC_INCLUDE_BOARD_TUYA_ZTU_H_

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/************************* Configure SWITCH GPIO ***************************************/
#define SWITCH_GPIO             GPIO_PD2
#define PD2_INPUT_ENABLE        ON
//#define PD2_DATA_OUT            OFF
#define PD2_OUTPUT_ENABLE       OFF
#define PD2_FUNC                AS_GPIO
//#define PULL_WAKEUP_SRC_PD4     PM_PIN_PULLDOWN_100K


/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM  1

#define BUTTON                  GPIO_PB5
#define PB5_INPUT_ENABLE        ON
#define PB5_DATA_OUT            OFF
#define PB5_OUTPUT_ENABLE       OFF
#define PB5_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PB5     PM_PIN_PULLUP_10K

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON}

/************************** Configure LED ****************************************/

#define LED_ON                  0
#define LED_OFF                 1

#define LED_GPIO                GPIO_PB4
#define PB4_FUNC                AS_GPIO
#define PB4_OUTPUT_ENABLE       ON
#define PB4_INPUT_ENABLE        OFF

#if (__PROJECT_TL_BOOT_LOADER__)

#define LED_POWER               GPIO_PA0
#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       ON
#define PA0_INPUT_ENABLE        OFF

#define LED_PERMIT              LED_GPIO

#endif

/********************* Configure Relay ***************************/

#define RELAY_ON                1
#define RELAY_OFF               0

#define RELAY_GPIO              GPIO_PC3
#define PC3_FUNC                AS_GPIO
#define PC3_OUTPUT_ENABLE       ON
#define PC3_INPUT_ENABLE        ON
#define PC3_DATA_OUT            RELAY_OFF

/**************************** Configure UART for BL0942 ***********************************/
#define BAUDRATE_UART           4800
#define GPIO_UART_TX            UART_TX_PB1
#define GPIO_UART_RX            UART_RX_PB7

/********************* Configure printf UART ***************************/

#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       UART_TX_PD3    //printf
#define DEBUG_BAUDRATE          115200

#endif /* UART_PRINTF_MODE */

//#endif /* SRC_INCLUDE_BOARD_TUYA_ZTU_H_ */

