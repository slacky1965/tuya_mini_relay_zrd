#ifndef SRC_INCLUDE_APP_ENDPOINT_CFG_H_
#define SRC_INCLUDE_APP_ENDPOINT_CFG_H_

#define APP_ENDPOINT1 0x01
#define APP_ENDPOINT2 0x02
#define APP_ENDPOINT3 0x03

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct {
    uint8_t  zclVersion;
    uint8_t  appVersion;
    uint8_t  stackVersion;
    uint8_t  hwVersion;
    uint8_t  manuName[ZCL_BASIC_MAX_LENGTH];
    uint8_t  modelId[ZCL_BASIC_MAX_LENGTH];
    uint8_t  dateCode[ZCL_BASIC_MAX_LENGTH];
    uint8_t  powerSource;
    uint8_t  genDevClass;                        //attr 8
    uint8_t  genDevType;                         //attr 9
    uint8_t  deviceEnable;
    uint8_t  swBuildId[ZCL_BASIC_MAX_LENGTH];    //attr 4000
} zcl_basicAttr_t;


/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
    uint16_t identifyTime;
}zcl_identifyAttr_t;

/**
 *  @brief Defined for group cluster attributes
 */
typedef struct{
    uint8_t  nameSupport;
}zcl_groupAttr_t;

/**
 *  @brief Defined for scene cluster attributes
 */
typedef struct{
    uint8_t   sceneCount;
    uint8_t   currentScene;
    uint8_t   nameSupport;
    bool sceneValid;
    uint16_t  currentGroup;
}zcl_sceneAttr_t;

typedef struct {
    uint32_t time_utc;
    uint32_t time_local;
    uint8_t  time_status;
} zcl_timeAttr_t;

/**
 *  @brief Defined for on/off cluster attributes
 */
typedef struct {
    uint16_t onTime;
    uint16_t offWaitTime;
    uint8_t  startUpOnOff;
    bool     onOff;
    bool     globalSceneControl;
} zcl_onOffAttr_t;

typedef struct {
    uint8_t switchType;
    uint8_t switchActions;
    uint8_t custom_swtichType; // custom RW
    uint8_t custom_decoupled;
} zcl_onOffCfgAttr_t;

/**
 *  @brief Defined for multistate input clusters attributes
 */
typedef struct {
    uint8_t     out_of_service;
    uint16_t    value;
    uint16_t    num;
    uint8_t     status_flag;
} zcl_msInputAttr_t;

typedef struct {
    uint64_t cur_sum_delivered;
    uint8_t  unit_of_measure;       // 0x00 - kWh
    uint8_t  status;
    uint8_t  summation_formatting;  // Bits 0 to 2: Number of Digits to the right of the Decimal Point
                                    // Bits 3 to 6: Number of Digits to the left of the Decimal Point
                                    // Bit  7:      If set, suppress leading zeros
    uint8_t  device_type;
} zcl_seAttr_t;

typedef struct {
    uint32_t type;
    uint16_t freq;
    uint16_t current;
    uint16_t voltage;
    uint16_t power;
} zcl_msAttr_t;

extern uint8_t APP_CB_CLUSTER_NUM1;
//extern uint8_t APP_CB_CLUSTER_NUM2;
extern const zcl_specClusterInfo_t  g_appClusterList1[];
//extern const zcl_specClusterInfo_t  g_appClusterList2[];
extern const af_simple_descriptor_t app_ep1_simpleDesc;
//extern const af_simple_descriptor_t app_ep2_simpleDesc;

/* Attributes */
extern zcl_basicAttr_t              g_zcl_basicAttrs;
extern zcl_identifyAttr_t           g_zcl_identifyAttrs;
extern zcl_groupAttr_t              g_zcl_groupAttrs[];
extern zcl_sceneAttr_t              g_zcl_sceneAttrs[];
extern zcl_onOffAttr_t              g_zcl_onOffAttrs[];
extern zcl_onOffCfgAttr_t           g_zcl_onOffCfgAttrs[];
extern zcl_msInputAttr_t            g_zcl_msInputAttrs[];
extern zcl_seAttr_t                 g_zcl_seAttrs;
extern zcl_msAttr_t                 g_zcl_msAttrs;

#define zcl_groupAttrsGet()         g_zcl_groupAttrs
#define zcl_sceneAttrGet()          g_zcl_sceneAttrs
#define zcl_onOffAttrsGet()         g_zcl_onOffAttrs;
#define zcl_onOffCfgAttrsGet()      g_zcl_onOffCfgAttrs;
#define zcl_msInputAttrsGet()       g_zcl_msInputAttrs;
#define zcl_seAttrsGet()            &g_zcl_seAttrs;
#define zcl_msAttrsGet()            &g_zcl_msAttrs;

#endif /* SRC_INCLUDE_APP_ENDPOINT_CFG_H_ */
