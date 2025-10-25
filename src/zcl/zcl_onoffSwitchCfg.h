#ifndef SRC_ZCL_ZCL_ONOFFSWITCHCFG_H_
#define SRC_ZCL_ZCL_ONOFFSWITCHCFG_H_

#define ZCL_ATTRID_SWITCH_TYPE              0x0000
#define ZCL_ATTRID_SWITCH_ACTION            0x0010

#define ZCL_SWITCH_TYPE_TOGGLE              0x00
#define ZCL_SWITCH_TYPE_MOMENTARY           0x01
#define ZCL_SWITCH_TYPE_MULTIFUNCTION       0x02

#define ZCL_SWITCH_ACTION_ON_OFF            0x00
#define ZCL_SWITCH_ACTION_OFF_ON            0x01
#define ZCL_SWITCH_ACTION_TOGGLE            0x02

#define CUSTOM_SWITCH_DECOUPLED_OFF         0x00
#define CUSTOM_SWITCH_DECOUPLED_ON          0x01

#define CUSTOM_ATTRID_SWITCH_TYPE           0xF000
#define CUSTOM_ATTRID_DECOUPLED             0xF001

status_t zcl_onoffCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);
nv_sts_t zcl_onOffCfgAttr_save(void);
nv_sts_t zcl_onOffCfgAttr_restore(void);


#endif /* SRC_ZCL_ZCL_ONOFFSWITCHCFG_H_ */
