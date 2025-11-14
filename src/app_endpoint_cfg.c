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
const uint16_t app_ep1_inClusterList[] = {
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
    ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
    ZCL_CLUSTER_SE_METERING,
    ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep1_outClusterList[] = {
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
//    ZCL_CLUSTER_GEN_TIME,
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_EP1_IN_CLUSTER_NUM      (sizeof(app_ep1_inClusterList)/sizeof(app_ep1_inClusterList[0]))
#define APP_EP1_OUT_CLUSTER_NUM     (sizeof(app_ep1_outClusterList)/sizeof(app_ep1_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t app_ep1_simpleDesc =
{
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_ONOFF_LIGHT,		                /* Application device identifier */
    APP_ENDPOINT1,                          /* Endpoint */
    1,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP1_IN_CLUSTER_NUM,                     /* Application input cluster count */
    APP_EP1_OUT_CLUSTER_NUM,                    /* Application output cluster count */
    (uint16_t *)app_ep1_inClusterList,          /* Application input cluster list */
    (uint16_t *)app_ep1_outClusterList,         /* Application output cluster list */
};


///**
// *  @brief Definition for Incoming cluster / Sever Cluster
// */
//const uint16_t app_ep2_inClusterList[] = {
//#ifdef ZCL_GROUP
//    ZCL_CLUSTER_GEN_GROUPS,
//#endif
//#ifdef ZCL_SCENE
//    ZCL_CLUSTER_GEN_SCENES,
//#endif
//#ifdef ZCL_ON_OFF
//    ZCL_CLUSTER_GEN_ON_OFF,
//    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
//#endif
//    ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
//};
//
///**
// *  @brief Definition for Outgoing cluster / Client Cluster
// */
//const uint16_t app_ep2_outClusterList[] = {
//#ifdef ZCL_ON_OFF
//    ZCL_CLUSTER_GEN_ON_OFF,
//#endif
//};
//
///**
// *  @brief Definition for Server cluster number and Client cluster number
// */
//#define APP_EP2_IN_CLUSTER_NUM      (sizeof(app_ep2_inClusterList)/sizeof(app_ep2_inClusterList[0]))
//#define APP_EP2_OUT_CLUSTER_NUM     (sizeof(app_ep2_outClusterList)/sizeof(app_ep2_outClusterList[0]))
//
///**
// *  @brief Definition for simple description for HA profile
// */
//const af_simple_descriptor_t app_ep2_simpleDesc =
//{
//    HA_PROFILE_ID,                          /* Application profile identifier */
//    HA_DEV_ONOFF_LIGHT,                     /* Application device identifier */
//    APP_ENDPOINT2,                          /* Endpoint */
//    1,                                      /* Application device version */
//    0,                                      /* Reserved */
//    APP_EP2_IN_CLUSTER_NUM,                     /* Application input cluster count */
//    APP_EP2_OUT_CLUSTER_NUM,                    /* Application output cluster count */
//    (uint16_t *)app_ep2_inClusterList,          /* Application input cluster list */
//    (uint16_t *)app_ep2_outClusterList,         /* Application output cluster list */
//};

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

//zcl_timeAttr_t g_zcl_timeAttrs = {
//    .time_utc   = 0xffffffff,
//    .time_local = 0xffffffff,
//    .time_status = 0,
//};
//
//const zclAttrInfo_t time_attrTbl[] =
//{
//    { ZCL_ATTRID_TIME,                      ZCL_UTC,        RWR,    (uint8_t*)&g_zcl_timeAttrs.time_utc         },
//    { ZCL_ATTRID_LOCAL_TIME,                ZCL_UINT32,     R,      (uint8_t*)&g_zcl_timeAttrs.time_local       },
//    { ZCL_ATTRID_TIME_STATUS,               ZCL_BITMAP8,    RW,     (uint8_t*)&g_zcl_timeAttrs.time_status      },
//
//    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision  },
//};
//
//#define ZCL_TIME_ATTR_NUM    sizeof(time_attrTbl) / sizeof(zclAttrInfo_t)

zcl_msInputAttr_t g_zcl_msInputAttrs[AMT_RELAY] = {
    {
        .value = ACTION_EMPTY,
        .num = 8,
        .out_of_service = 0,
        .status_flag = 0,
    },
//    {
//        .value = ACTION_EMPTY,
//        .num = 8,
//        .out_of_service = 0,
//        .status_flag = 0,
//    }
};

const zclAttrInfo_t msInput1_attrTbl[] = {
        { ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,   ZCL_BOOLEAN,    RW,     (uint8_t*)&g_zcl_msInputAttrs[0].out_of_service },
        { ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,    ZCL_UINT16,     RWR,    (uint8_t*)&g_zcl_msInputAttrs[0].value          },
        { ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,     ZCL_BITMAP8,    RR,     (uint8_t*)&g_zcl_msInputAttrs[0].status_flag    },
        { ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,    ZCL_UINT16,     R,      (uint8_t*)&g_zcl_msInputAttrs[0].num            },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },

};

#define ZCL_MSINPUT1_ATTR_NUM   sizeof(msInput1_attrTbl) / sizeof(zclAttrInfo_t)

//const zclAttrInfo_t msInput2_attrTbl[] = {
//        { ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,   ZCL_BOOLEAN,    RW,     (uint8_t*)&g_zcl_msInputAttrs[1].out_of_service },
//        { ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,    ZCL_UINT16,     RWR,    (uint8_t*)&g_zcl_msInputAttrs[1].value          },
//        { ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,     ZCL_BITMAP8,    RR,     (uint8_t*)&g_zcl_msInputAttrs[1].status_flag    },
//        { ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,    ZCL_UINT16,     R,      (uint8_t*)&g_zcl_msInputAttrs[1].num            },
//
//        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },
//
//};
//
//#define ZCL_MSINPUT2_ATTR_NUM   sizeof(msInput2_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_GROUP
/* Group */
zcl_groupAttr_t g_zcl_groupAttrs[AMT_RELAY] =
{
    {.nameSupport = 0},
//    {.nameSupport = 0}
};

const zclAttrInfo_t group1_attrTbl[] =
{
    { ZCL_ATTRID_GROUP_NAME_SUPPORT,        ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_groupAttrs[0].nameSupport     },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_GROUP1_ATTR_NUM    sizeof(group1_attrTbl) / sizeof(zclAttrInfo_t)

//const zclAttrInfo_t group2_attrTbl[] =
//{
//    { ZCL_ATTRID_GROUP_NAME_SUPPORT,        ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_groupAttrs[1].nameSupport     },
//
//    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
//};
//
//#define ZCL_GROUP2_ATTR_NUM    sizeof(group2_attrTbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_SCENE
/* Scene */
zcl_sceneAttr_t g_zcl_sceneAttrs[AMT_RELAY] =
{
    {
        .sceneCount     = 0,
        .currentScene   = 0,
        .currentGroup   = 0x0000,
        .sceneValid     = FALSE,
        .nameSupport    = 0,
    },
//    {
//        .sceneCount     = 0,
//        .currentScene   = 0,
//        .currentGroup   = 0x0000,
//        .sceneValid     = FALSE,
//        .nameSupport    = 0,
//    }
};

const zclAttrInfo_t scene1_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[0].sceneCount     },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[0].currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs[0].currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs[0].sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs[0].nameSupport    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE1_ATTR_NUM   sizeof(scene1_attrTbl) / sizeof(zclAttrInfo_t)

//const zclAttrInfo_t scene2_attrTbl[] = {
//    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[1].sceneCount     },
//    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[1].currentScene   },
//    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs[1].currentGroup   },
//    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs[1].sceneValid     },
//    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs[1].nameSupport    },
//
//    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
//};
//
//#define ZCL_SCENE2_ATTR_NUM   sizeof(scene2_attrTbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_ON_OFF

/* On/Off */
zcl_onOffAttr_t g_zcl_onOffAttrs[AMT_RELAY] = {
    {
        //
        .onOff              = 0x00,
        .globalSceneControl = 1,
        .onTime             = 0x0000,
        .offWaitTime        = 0x0000,
        .startUpOnOff       = ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF,
    },
//    {
//        .onOff              = 0x00,
//        .globalSceneControl = 1,
//        .onTime             = 0x0000,
//        .offWaitTime        = 0x0000,
//        .startUpOnOff       = ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF,
//    }
};

const zclAttrInfo_t onOff1_attrTbl[] = {
    { ZCL_ATTRID_ONOFF,                     ZCL_BOOLEAN,    RR,     (uint8_t*)&g_zcl_onOffAttrs[0].onOff               },
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,      ZCL_BOOLEAN,    R,      (uint8_t*)&g_zcl_onOffAttrs[0].globalSceneControl  },
    { ZCL_ATTRID_ON_TIME,                   ZCL_UINT16,     RW,     (uint8_t*)&g_zcl_onOffAttrs[0].onTime              },
    { ZCL_ATTRID_OFF_WAIT_TIME,             ZCL_UINT16,     RW,     (uint8_t*)&g_zcl_onOffAttrs[0].offWaitTime         },
    { ZCL_ATTRID_START_UP_ONOFF,            ZCL_ENUM8,      RW,     (uint8_t*)&g_zcl_onOffAttrs[0].startUpOnOff        },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },
};

#define ZCL_ONOFF1_ATTR_NUM   sizeof(onOff1_attrTbl) / sizeof(zclAttrInfo_t)

//const zclAttrInfo_t onOff2_attrTbl[] = {
//    { ZCL_ATTRID_ONOFF,                     ZCL_BOOLEAN,    RR,     (uint8_t*)&g_zcl_onOffAttrs[1].onOff               },
//    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,      ZCL_BOOLEAN,    R,      (uint8_t*)&g_zcl_onOffAttrs[1].globalSceneControl  },
//    { ZCL_ATTRID_ON_TIME,                   ZCL_UINT16,     RW,     (uint8_t*)&g_zcl_onOffAttrs[1].onTime              },
//    { ZCL_ATTRID_OFF_WAIT_TIME,             ZCL_UINT16,     RW,     (uint8_t*)&g_zcl_onOffAttrs[1].offWaitTime         },
//    { ZCL_ATTRID_START_UP_ONOFF,            ZCL_ENUM8,      RW,     (uint8_t*)&g_zcl_onOffAttrs[1].startUpOnOff        },
//
//    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },
//};
//
//#define ZCL_ONOFF2_ATTR_NUM   sizeof(onOff2_attrTbl) / sizeof(zclAttrInfo_t)

#endif


#ifdef ZCL_ON_OFF_SWITCH_CFG
/* On/Off Config */

zcl_onOffCfgAttr_t g_zcl_onOffCfgAttrs[AMT_RELAY] = {
    {
        .switchType        = ZCL_SWITCH_TYPE_MOMENTARY,
        .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
        .custom_swtichType = ZCL_SWITCH_TYPE_MOMENTARY,
        .custom_decoupled  = CUSTOM_SWITCH_DECOUPLED_OFF,
    },
//    {
//        .switchType        = ZCL_SWITCH_TYPE_MOMENTARY,
//        .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
//        .custom_swtichType = ZCL_SWITCH_TYPE_MOMENTARY,
//        .custom_decoupled  = CUSTOM_SWITCH_DECOUPLED_OFF,
//    }
};

const zclAttrInfo_t onOffCfg1_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs[0].switchType         },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[0].switchActions      },
    { CUSTOM_ATTRID_SWITCH_TYPE,            ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[0].custom_swtichType  },
    { CUSTOM_ATTRID_DECOUPLED,              ZCL_ENUM8,    RWR,(u8*)&g_zcl_onOffCfgAttrs[0].custom_decoupled   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision           },
};

#define ZCL_ON_OFF1_CFG_ATTR_NUM       sizeof(onOffCfg1_attrTbl) / sizeof(zclAttrInfo_t)

//const zclAttrInfo_t onOffCfg2_attrTbl[] =
//{
//    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs[1].switchType         },
//    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[1].switchActions      },
//    { CUSTOM_ATTRID_SWITCH_TYPE,            ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[1].custom_swtichType  },
//    { CUSTOM_ATTRID_DECOUPLED,              ZCL_ENUM8,    RWR,(u8*)&g_zcl_onOffCfgAttrs[1].custom_decoupled   },
//
//    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision           },
//};
//
//#define ZCL_ON_OFF2_CFG_ATTR_NUM       sizeof(onOffCfg2_attrTbl) / sizeof(zclAttrInfo_t)

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
    .power = 0x8000,
    .power_max = DEFAULT_POWER_MAX,
    .current_max = DEFAULT_CURRENT_MAX,
    .voltage_min = DEFAULT_VOLTAGE_MIN,
    .voltage_max = DEFAULT_VOLTAGE_MAX,
    .time_reload = DEFAULT_TIME_RELOAD,
    .protect_control = DEFAULT_PROTECT_CONTROL,
    .auto_restart = DEFAULT_AUTORESTART,
};

const zclAttrInfo_t ms_attrTbl[] = {
    {ZCL_ATTRID_MEASUREMENT_TYPE,           ZCL_BITMAP32, R,    (uint8_t*)&g_zcl_msAttrs.type               },
    {ZCL_ATTRID_AC_FREQUENCY,               ZCL_UINT16,   RR,   (uint8_t*)&g_zcl_msAttrs.freq               },
    {ZCL_ATTRID_RMS_CURRENT,                ZCL_UINT16,   RR,   (uint8_t*)&g_zcl_msAttrs.current            },
    {ZCL_ATTRID_RMS_VOLTAGE,                ZCL_UINT16,   RR,   (uint8_t*)&g_zcl_msAttrs.voltage            },
    {ZCL_ATTRID_ACTIVE_POWER,               ZCL_INT16,    RR,   (uint8_t*)&g_zcl_msAttrs.power              },
    {ZCL_ATTRID_RMS_EXTREME_UNDER_VOLTAGE,  ZCL_INT16,    RW,   (uint8_t*)&g_zcl_msAttrs.voltage_min        },
    {ZCL_ATTRID_RMS_EXTREME_OVER_VOLTAGE,   ZCL_INT16,    RW,   (uint8_t*)&g_zcl_msAttrs.voltage_max        },
    {ZCL_ATTRID_CUSTOM_CURRENT_MAX,         ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_msAttrs.current_max        },
    {ZCL_ATTRID_CUSTOM_POWER_MAX,           ZCL_INT16,    RW,   (uint8_t*)&g_zcl_msAttrs.power_max          },
    {ZCL_ATTRID_CUSTOM_TIME_RELOAD,         ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_msAttrs.time_reload        },
    {ZCL_ATTRID_CUSTOM_PROTECT_CONTROL,     ZCL_BOOLEAN,  RW,   (uint8_t*)&g_zcl_msAttrs.protect_control    },
    {ZCL_ATTRID_CUSTOM_AUTORESTART,         ZCL_BOOLEAN,  RW,   (uint8_t*)&g_zcl_msAttrs.auto_restart       },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_MS_ATTR_NUM    sizeof(ms_attrTbl) / sizeof(zclAttrInfo_t)

/**
 *  @brief Definition for mini relay ZCL specific cluster
 */
const zcl_specClusterInfo_t g_appClusterList1[] =
{
    {ZCL_CLUSTER_GEN_BASIC,                 MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,         basic_attrTbl,      zcl_basic_register,     app_basicCb     },
    {ZCL_CLUSTER_GEN_IDENTIFY,              MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,      identify_attrTbl,   zcl_identify_register,  app_identifyCb  },
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, ZCL_GROUP1_ATTR_NUM,        group1_attrTbl,      zcl_group_register,     NULL            },
#endif
#ifdef ZCL_SCENE
    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE1_ATTR_NUM,        scene1_attrTbl,      zcl_scene_register,     app_sceneCb     },
#endif
//    {ZCL_CLUSTER_GEN_TIME,                  MANUFACTURER_CODE_NONE, ZCL_TIME_ATTR_NUM,          time_attrTbl,       zcl_time_register,      app_timeCb      },
#ifdef ZCL_ON_OFF
    {ZCL_CLUSTER_GEN_ON_OFF,                MANUFACTURER_CODE_NONE, ZCL_ONOFF1_ATTR_NUM,        onOff1_attrTbl,      zcl_onOff_register,     app_onOffCb     },
#endif
#ifdef ZCL_ON_OFF_SWITCH_CFG
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF1_CFG_ATTR_NUM,   onOffCfg1_attrTbl,   zcl_onoffCfg_register,  NULL            },
#endif
    {ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,MANUFACTURER_CODE_NONE, ZCL_MSINPUT1_ATTR_NUM,      msInput1_attrTbl,    zcl_multistate_input_register,     app_msInputCb},
    {ZCL_CLUSTER_SE_METERING,               MANUFACTURER_CODE_NONE, ZCL_SE_ATTR_NUM,            se_attrTbl,          app_zcl_metering_register,         app_meteringCb  },
    {ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, MANUFACTURER_CODE_NONE, ZCL_MS_ATTR_NUM,            ms_attrTbl,          zcl_electricalMeasure_register,    NULL    },
};

uint8_t APP_CB_CLUSTER_NUM1 = (sizeof(g_appClusterList1)/sizeof(g_appClusterList1[0]));

//const zcl_specClusterInfo_t g_appClusterList2[] =
//{
//#ifdef ZCL_GROUP
//    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, ZCL_GROUP2_ATTR_NUM,         group2_attrTbl,      zcl_group_register,     NULL            },
//#endif
//#ifdef ZCL_SCENE
//    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE2_ATTR_NUM,         scene2_attrTbl,      zcl_scene_register,     app_sceneCb     },
//#endif
//#ifdef ZCL_ON_OFF
//    {ZCL_CLUSTER_GEN_ON_OFF,                MANUFACTURER_CODE_NONE, ZCL_ONOFF2_ATTR_NUM,         onOff2_attrTbl,      zcl_onOff_register,     app_onOffCb     },
//#endif
//#ifdef ZCL_ON_OFF_SWITCH_CFG
//    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF2_CFG_ATTR_NUM,    onOffCfg2_attrTbl,   zcl_onoffCfg_register,  NULL            },
//#endif
//    {ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,MANUFACTURER_CODE_NONE, ZCL_MSINPUT2_ATTR_NUM,       msInput2_attrTbl,    zcl_multistate_input_register,  app_msInputCb},
//};
//
//uint8_t APP_CB_CLUSTER_NUM2 = (sizeof(g_appClusterList2)/sizeof(g_appClusterList2[0]));
//
