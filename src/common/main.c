/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for main
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#include "zb_common.h"

#define ID_BOOTABLE             0x544c4e4b

#define OTA1_FADDR              0x000000
#define OTA2_FADDR              0x020000
#define BIG_OTA1_FADDR          0x000000 // Big OTA1
#define BIG_OTA2_FADDR          0x040000 // Big OTA2
#define ZIGBEE_BOOT_OTA_FADDR   0x008000
#define ZIGBEE_MAC_FADDR        0x0ff000
#define BIN_SIZE_MAX_OTA        0x036000
#define BLE_MAC_FADDR           0x076000

#define BOOTLOAD_MARKER         ZCL_BASIC_MFG_NAME
#define BOOTLOAD_MARKER_ADDR    0x7ff0
#define BOOTLOAD_MARKER_SECTOR  0x7000

static const u32 flag_addr_ok = 0x33CC55AA;


extern void user_init(bool isRetention);

/* Reformat low OTA to big OTA, Zigbee BootLoader to Zigbee no BootLoader */

_attribute_ram_code_ void tuya_zigbee_ota(void) {
    u32 id = ID_BOOTABLE;
    u32 size;
    u32 faddrr = ZIGBEE_BOOT_OTA_FADDR; // 0x008000
    u32 faddrw = BIG_OTA1_FADDR;    // 0x000000
    u32 buf_blk[64];    // max 256 bytes
    flash_write_status(0, 0);
    // search for start firmware address 0x008000 or 0x020000 ?
    flash_read_page(faddrr, 16, (unsigned char *) &buf_blk);
    if(buf_blk[2] != id) { // 0x008000 != bootable
        faddrr = OTA2_FADDR;
        flash_read_page(faddrr, 16, (unsigned char *) &buf_blk);
        if(buf_blk[2] != id) // 0x020000 != bootable
            return;
    }
    // faddrr: 0x008000 == bootable || 0x020000 == bootable
    flash_read_page(faddrr, sizeof(buf_blk), (unsigned char *) &buf_blk);
    if(buf_blk[2] == id && buf_blk[6] > FLASH_SECTOR_SIZE && buf_blk[6] < BIN_SIZE_MAX_OTA) {
        buf_blk[2] &= 0xffffffff; // clear id "bootable"
        size = buf_blk[6];
        size += FLASH_SECTOR_SIZE - 1;
        size &= ~(FLASH_SECTOR_SIZE - 1);
        size += faddrw;
        flash_erase_sector(faddrw); // 45 ms, 4 mA
        flash_write_page(faddrw, sizeof(buf_blk), (unsigned char *) &buf_blk);
        faddrr += sizeof(buf_blk);
        // size += faddrw;
        faddrw += sizeof(buf_blk);
        while(faddrw < size) {
            if((faddrw & (FLASH_SECTOR_SIZE - 1)) == 0)
                flash_erase_sector(faddrw); // 45 ms, 4 mA
                // rd-wr 4kB - 20 ms, 4 mA
                flash_read_page(faddrr, sizeof(buf_blk), (unsigned char *) &buf_blk);
            faddrr += sizeof(buf_blk);
            flash_write_page(faddrw, sizeof(buf_blk), (unsigned char *) &buf_blk);
            faddrw += sizeof(buf_blk);
        }
        // set id "bootable" to new segment
        flash_write_page(BIG_OTA1_FADDR+8, sizeof(id), (unsigned char *) &id);
        while(1)
            reg_pwdn_ctrl = BIT(5);
    }
}



int flash_main(void){
	startup_state_e state = drv_platform_init();

//    if (state != SYSTEM_DEEP_RETENTION) {
//        tuya_zigbee_ota();
//    }

	u8 isRetention = (state == SYSTEM_DEEP_RETENTION) ? 1 : 0;

    tuya_zigbee_ota();

	os_init(isRetention);

#if 0
	extern void moduleTest_start(void);
	moduleTest_start();
#endif

	user_init(isRetention);

	drv_enable_irq();

#if (MODULE_WATCHDOG_ENABLE)
	drv_wd_setInterval(600);
    drv_wd_start();
#endif

#if VOLTAGE_DETECT_ENABLE
    u32 tick = clock_time();
#endif

	while(1){
#if VOLTAGE_DETECT_ENABLE
		if(clock_time_exceed(tick, 200 * 1000)){
			voltage_detect(0);
			tick = clock_time();
		}
#endif

    	ev_main();

#if (MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif

		tl_zbTaskProcedure();

#if	(MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif
	}

	return 0;
}

_attribute_ram_code_ int main(void) {

    if(*(u32 *)(ZIGBEE_BOOT_OTA_FADDR + 8) == ID_BOOTABLE || flag_addr_ok != 0x33CC55AA) {
        // clock_init(SYS_CLK_24M_Crystal);
        tuya_zigbee_ota();
    }

//    tl_id = *(u32 *)(BIG_OTA1_FADDR + 8);
    return flash_main();
}


