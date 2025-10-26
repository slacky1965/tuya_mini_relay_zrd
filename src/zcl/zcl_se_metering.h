#ifndef SRC_ZCL_ZCL_SE_METERING_H_
#define SRC_ZCL_ZCL_SE_METERING_H_

#define ZCL_CMD_REMOVE_ENERGY   0x80

_CODE_ZCL_ status_t app_zcl_metering_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

#endif /* SRC_ZCL_ZCL_SE_METERING_H_ */
