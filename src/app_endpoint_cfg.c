#include "app_main.h"

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME     		{6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID	   		{8,'T','L','S','R','8','2','x','x'}
#endif
#ifndef ZCL_BASIC_SW_BUILD_ID
#define ZCL_BASIC_SW_BUILD_ID     	{10,'0','1','2','2','0','5','2','0','1','7'}
#endif

#define R               ACCESS_CONTROL_READ
#define RW              ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE
#define RR              ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE
#define RWR             ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE | ACCESS_CONTROL_REPORTABLE

#define ZCL_UINT8       ZCL_DATA_TYPE_UINT8
#define ZCL_UINT16      ZCL_DATA_TYPE_UINT16
#define ZCL_UINT24      ZCL_DATA_TYPE_UINT24
#define ZCL_UINT32      ZCL_DATA_TYPE_UINT32
#define ZCL_UINT48      ZCL_DATA_TYPE_UINT48
#define ZCL_INT8        ZCL_DATA_TYPE_INT8
#define ZCL_INT16       ZCL_DATA_TYPE_INT16
#define ZCL_ENUM8       ZCL_DATA_TYPE_ENUM8
#define ZCL_ENUM16      ZCL_DATA_TYPE_ENUM16
#define ZCL_BITMAP8     ZCL_DATA_TYPE_BITMAP8
#define ZCL_BITMAP16    ZCL_DATA_TYPE_BITMAP16
#define ZCL_BITMAP32    ZCL_DATA_TYPE_BITMAP32
#define ZCL_BOOLEAN     ZCL_DATA_TYPE_BOOLEAN
#define ZCL_CHAR_STR    ZCL_DATA_TYPE_CHAR_STR
#define ZCL_OCTET_STR   ZCL_DATA_TYPE_OCTET_STR
#define ZCL_UTC         ZCL_DATA_TYPE_UTC
#define ZCL_IEEE_ADDR   ZCL_DATA_TYPE_IEEE_ADDR

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_inClusterList[] = {
    ZCL_CLUSTER_GEN_BASIC,
    ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
    ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
    ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
#ifdef ZCL_POLL_CTRL
    ZCL_CLUSTER_GEN_POLL_CONTROL,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
#endif
    ZCL_CLUSTER_SE_METERING,
    ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_outClusterList[] = {
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_IN_CLUSTER_NUM      (sizeof(app_inClusterList)/sizeof(app_inClusterList[0]))
#define APP_OUT_CLUSTER_NUM     (sizeof(app_outClusterList)/sizeof(app_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t app_simpleDesc =
{
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_ONOFF_LIGHT,		                /* Application device identifier */
    APP_ENDPOINT1,                          /* Endpoint */
    1,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_IN_CLUSTER_NUM,                     /* Application input cluster count */
    APP_OUT_CLUSTER_NUM,                    /* Application output cluster count */
    (uint16_t *)app_inClusterList,          /* Application input cluster list */
    (uint16_t *)app_outClusterList,         /* Application output cluster list */
};

/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
{
    .zclVersion     = 0x03,
    .appVersion     = APP_RELEASE,
    .stackVersion   = (STACK_RELEASE|STACK_BUILD),
    .hwVersion      = HW_VERSION,
    .manuName       = ZCL_BASIC_MFG_NAME,
    .modelId        = ZCL_BASIC_MODEL_ID,
    .dateCode       = ZCL_BASIC_DATE_CODE,
    .powerSource    = POWER_SOURCE_MAINS_1_PHASE,
    .swBuildId      = ZCL_BASIC_SW_BUILD_ID,
    .deviceEnable   = TRUE,
};

uint8_t zclVersionServer;

const zclAttrInfo_t basic_attrTbl[] =
{
    { ZCL_ATTRID_BASIC_ZCL_VER,             ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.zclVersion      },
    { ZCL_ATTRID_BASIC_APP_VER,             ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.appVersion      },
    { ZCL_ATTRID_BASIC_STACK_VER,           ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.stackVersion    },
    { ZCL_ATTRID_BASIC_HW_VER,              ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.hwVersion       },
    { ZCL_ATTRID_BASIC_MFR_NAME,            ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.manuName         },
    { ZCL_ATTRID_BASIC_MODEL_ID,            ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.modelId          },
    { ZCL_ATTRID_BASIC_DATE_CODE,           ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.dateCode         },
    { ZCL_ATTRID_BASIC_POWER_SOURCE,        ZCL_ENUM8,      R,  (uint8_t*)&g_zcl_basicAttrs.powerSource     },
    { ZCL_ATTRID_BASIC_DEV_ENABLED,         ZCL_BOOLEAN,    RW, (uint8_t*)&g_zcl_basicAttrs.deviceEnable    },
    { ZCL_ATTRID_BASIC_SW_BUILD_ID,         ZCL_CHAR_STR,   R,  (uint8_t*)&g_zcl_basicAttrs.swBuildId       },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_BASIC_ATTR_NUM    sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)

uint8_t zclVersionServer;

const zclAttrInfo_t version_attrTbl[] =
{
    { ZCL_ATTRID_BASIC_ZCL_VER,             ZCL_UINT8,      R,  (uint8_t*)&zclVersionServer                 },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_BASIC_SERVER_ATTR_NUM    sizeof(version_attrTbl) / sizeof(zclAttrInfo_t)


/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,     RW, (uint8_t*)&g_zcl_identifyAttrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_IDENTIFY_ATTR_NUM    sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_GROUP
/* Group */
zcl_groupAttr_t g_zcl_groupAttrs =
{
    .nameSupport    = 0,
};

const zclAttrInfo_t group_attrTbl[] =
{
    { ZCL_ATTRID_GROUP_NAME_SUPPORT,        ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_groupAttrs.nameSupport     },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_GROUP_ATTR_NUM    sizeof(group_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_SCENE
/* Scene */
zcl_sceneAttr_t g_zcl_sceneAttrs =
{
    .sceneCount     = 0,
    .currentScene   = 0,
    .currentGroup   = 0x0000,
    .sceneValid     = FALSE,
    .nameSupport    = 0,
};

const zclAttrInfo_t scene_attrTbl[] =
{
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs.sceneCount     },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs.currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs.currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs.sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs.nameSupport    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE_ATTR_NUM   sizeof(scene_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_ON_OFF

/* On/Off */
zcl_onOffAttr_t g_zcl_onOffAttrs = {
    .onOff              = 0x00,
    .globalSceneControl = 1,
    .onTime             = 0x0000,
    .offWaitTime        = 0x0000,
    .startUpOnOff       = ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF,
};

const zclAttrInfo_t onOff_attrTbl[] = {
    { ZCL_ATTRID_ONOFF,                     ZCL_BOOLEAN,    RR,     (uint8_t*)&g_zcl_onOffAttrs.onOff               },
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,      ZCL_BOOLEAN,    R,      (uint8_t*)&g_zcl_onOffAttrs.globalSceneControl  },
    { ZCL_ATTRID_ON_TIME,                   ZCL_UINT16,     RW,     (uint8_t*)&g_zcl_onOffAttrs.onTime              },
    { ZCL_ATTRID_OFF_WAIT_TIME,             ZCL_UINT16,     RW,     (uint8_t*)&g_zcl_onOffAttrs.offWaitTime         },
    { ZCL_ATTRID_START_UP_ONOFF,            ZCL_ENUM8,      RW,     (uint8_t*)&g_zcl_onOffAttrs.startUpOnOff        },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },
};

#define ZCL_ONOFF_ATTR_NUM   sizeof(onOff_attrTbl) / sizeof(zclAttrInfo_t)

#endif


#ifdef ZCL_ON_OFF_SWITCH_CFG
/* On/Off Config */

zcl_onOffCfgAttr_t g_zcl_onOffCfgAttrs = {
    .switchType        = ZCL_SWITCH_TYPE_MOMENTARY,
    .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
    .custom_swtichType = ZCL_SWITCH_TYPE_MOMENTARY,
};

const zclAttrInfo_t onOffCfg_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs.switchType         },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs.switchActions      },
    { CUSTOM_ATTRID_SWITCH_TYPE,            ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs.custom_swtichType  },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision                 },
};

#define ZCL_ON_OFF_CFG_ATTR_NUM       sizeof(onOffCfg_attrTbl) / sizeof(zclAttrInfo_t)

#endif //ZCL_ON_OFF_SWITCH_CFG

zcl_seAttr_t g_zcl_seAttrs = {
    .unit_of_measure = 0x00,                                        // kWh
    .summation_formatting = 0xFA,                                   // bit7 - 1, bit6-bit3 - 15, bit2-bit0 - 2 (b11111010)
    .status = 0,
    .device_type = 0,                                               // 0 - Electric Metering
};

const zclAttrInfo_t se_attrTbl[] = {
    {ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD,         ZCL_UINT48,     RR, (uint8_t*)&g_zcl_seAttrs.cur_sum_delivered      },
    {ZCL_ATTRID_STATUS,                             ZCL_BITMAP8,    RR,  (uint8_t*)&g_zcl_seAttrs.status                },
    {ZCL_ATTRID_UNIT_OF_MEASURE,                    ZCL_UINT8,      R,  (uint8_t*)&g_zcl_seAttrs.unit_of_measure        },
    {ZCL_ATTRID_SUMMATION_FORMATTING,               ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_seAttrs.summation_formatting   },
    {ZCL_ATTRID_METERING_DEVICE_TYPE,               ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_seAttrs.device_type            },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision      },
};

#define ZCL_SE_ATTR_NUM    sizeof(se_attrTbl) / sizeof(zclAttrInfo_t)

zcl_msAttr_t g_zcl_msAttrs = {
    .type = 0x09,               // bit0: Active measurement (AC). bit3: Phase A measurement
    .freq = 0xffff,
    .current = 0xffff,
    .voltage = 0xffff,
    .power = 0xffff,
};

const zclAttrInfo_t ms_attrTbl[] = {
    {ZCL_ATTRID_MEASUREMENT_TYPE,           ZCL_BITMAP32, R,    (uint8_t*)&g_zcl_msAttrs.type               },
    {ZCL_ATTRID_AC_FREQUENCY,               ZCL_UINT16,   RR,   (uint8_t*)&g_zcl_msAttrs.freq               },
    {ZCL_ATTRID_RMS_CURRENT,                ZCL_UINT16,   RR,   (uint8_t*)&g_zcl_msAttrs.current            },
    {ZCL_ATTRID_RMS_VOLTAGE,                ZCL_UINT16,   RR,   (uint8_t*)&g_zcl_msAttrs.voltage            },
    {ZCL_ATTRID_ACTIVE_POWER,               ZCL_INT16,    RR,   (uint8_t*)&g_zcl_msAttrs.power              },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_MS_ATTR_NUM    sizeof(ms_attrTbl) / sizeof(zclAttrInfo_t)

/**
 *  @brief Definition for mini relay ZCL specific cluster
 */
const zcl_specClusterInfo_t g_appClusterList[] =
{
    {ZCL_CLUSTER_GEN_BASIC,                 MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,         basic_attrTbl,      zcl_basic_register,     app_basicCb     },
    {ZCL_CLUSTER_GEN_IDENTIFY,              MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,      identify_attrTbl,   zcl_identify_register,  app_identifyCb  },
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, ZCL_GROUP_ATTR_NUM,         group_attrTbl,      zcl_group_register,     NULL            },
#endif
#ifdef ZCL_SCENE
    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE_ATTR_NUM,         scene_attrTbl,      zcl_scene_register,     app_sceneCb     },
#endif
#ifdef ZCL_ON_OFF
    {ZCL_CLUSTER_GEN_ON_OFF,                MANUFACTURER_CODE_NONE, ZCL_ONOFF_ATTR_NUM,         onOff_attrTbl,      zcl_onOff_register,     app_onOffCb     },
#endif
#ifdef ZCL_ON_OFF_SWITCH_CFG
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF_CFG_ATTR_NUM,    onOffCfg_attrTbl,   zcl_onoffCfg_register,  NULL            },
#endif
    {ZCL_CLUSTER_SE_METERING,               MANUFACTURER_CODE_NONE, ZCL_SE_ATTR_NUM,            se_attrTbl,         zcl_metering_register,  app_meteringCb  },
    {ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, MANUFACTURER_CODE_NONE, ZCL_MS_ATTR_NUM,            ms_attrTbl,         zcl_electricalMeasure_register, NULL    },
};

uint8_t APP_CB_CLUSTER_NUM = (sizeof(g_appClusterList)/sizeof(g_appClusterList[0]));

