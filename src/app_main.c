#include "app_main.h"

//uint8_t device_online = false;
uint8_t resp_time = false;

app_ctx_t g_appCtx = {
        .timerFactoryReset = NULL,
        .timerLedEvt = NULL,
};

#ifdef ZCL_OTA
extern ota_callBack_t app_otaCb;

//running code firmware information
ota_preamble_t app_otaInfo = {
	.fileVer 			= FILE_VERSION,
	.imageType 			= IMAGE_TYPE,
	.manufacturerCode 	= MANUFACTURER_CODE_TELINK,
};
#endif

//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
    bdb_zdoStartDevCnf,//start device cnf cb
    NULL,//reset cnf cb
    NULL,//device announce indication cb
    app_leaveIndHandler,//leave ind cb
    app_leaveCnfHandler,//leave cnf cb
    app_nwkUpdateIndicateHandler,//nwk update ind cb
    NULL,//permit join ind cb
    NULL,//nlme sync cnf cb
    NULL,//tc join ind cb
    NULL,//tc detects that the frame counter is near limit
};


/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
    .linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
    .linkKey.tcLinkKey.key = (uint8_t *)tcLinkKeyCentralDefault,             //can use unique link key stored in NV

    .linkKey.distributeLinkKey.keyType = MASTER_KEY,
    .linkKey.distributeLinkKey.key = (uint8_t *)linkKeyDistributedMaster,    //use linkKeyDistributedCertification before testing

    .linkKey.touchLinkKey.keyType = MASTER_KEY,
    .linkKey.touchLinkKey.key = (uint8_t *)touchLinkKeyMaster,               //use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
    .touchlinkEnable = 1,                                               /* enable touch-link */
#else
    .touchlinkEnable = 0,                                               /* disable touch-link */
#endif
    .touchlinkChannel = DEFAULT_CHANNEL,                                /* touch-link default operation channel for target */
    .touchlinkLqiThreshold = 0xA0,                                      /* threshold for touch-link scan req/resp command */
};



/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
	/* Initialize ZB stack */
	zb_init();

	/* Register stack CB */
    zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
	af_nodeDescManuCodeUpdate(MANUFACTURER_CODE_TELINK);

    /* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
    zcl_init(app_zclProcessIncomingMsg);

	/* Register endPoint */
    af_endpointRegister(APP_ENDPOINT1, (af_simple_descriptor_t *)&app_ep1_simpleDesc, zcl_rx_handler, NULL);
//    af_endpointRegister(APP_ENDPOINT2, (af_simple_descriptor_t *)&app_ep2_simpleDesc, zcl_rx_handler, NULL);

	zcl_reportingTabInit();

	start_message();
	relay_settings_restore();
	energy_restore();

	/* Register ZCL specific cluster information */
    zcl_register(APP_ENDPOINT1, APP_CB_CLUSTER_NUM1, (zcl_specClusterInfo_t *)g_appClusterList1);
//    zcl_register(APP_ENDPOINT2, APP_CB_CLUSTER_NUM2, (zcl_specClusterInfo_t *)g_appClusterList2);

    dev_relay_init();

    #if ZCL_GP_SUPPORT
	/* Initialize GP */
	gp_init(APP_ENDPOINT1);
#endif

#if ZCL_OTA_SUPPORT
	/* Initialize OTA */
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&app_ep1_simpleDesc, &app_otaInfo, &app_otaCb);
#endif

#if ZCL_WWAH_SUPPORT
    /* Initialize WWAH server */
    wwah_init(WWAH_TYPE_SERVER, (af_simple_descriptor_t *)&app_simpleDesc);
#endif

    app_uart_init();

    TL_ZB_TIMER_SCHEDULE(app_monitoringCb, NULL, TIMEOUT_1SEC);

//    printf("FLASH_ADDR_OF_OTA_IMAGE: 0x%08x\r\n", FLASH_ADDR_OF_OTA_IMAGE);
//    printf("BEGIN_USER_DATA: 0x%x\r\n", BEGIN_USER_DATA);
//    printf("END_USER_DATA:   0x%x\r\n", END_USER_DATA);
//    printf("USER_DATA_SIZE:  0x%x\r\n", USER_DATA_SIZE);
}

void app_task(void) {

    button_handler();
    switch_handler();
    monitoring_handler();

    if (BDB_STATE_GET() == BDB_STATE_IDLE && !button_idle() && !switch_idle() && !available_ring_buff()) {
        report_handler();
    }
}


extern volatile u16 T_evtExcept[4];

static void app_sysException(void) {

#if UART_PRINTF_MODE
    printf("app_sysException, line: %d, event: %d, reset\r\n", T_evtExcept[0], T_evtExcept[1]);
#endif

#if 1
    SYSTEM_RESET();
#else
    led_on(LED_STATUS);
    while(1);
#endif
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
    (void)isRetention;

    /* Initialize LEDs*/
    light_init();

#ifdef CHECK_BOOTLOADER
    bootloader_check();
#endif

    /* Initialize Stack */
    stack_init();

    /* Initialize user application */
    user_app_init();

    /* Register except handler for test */
    sys_exceptHandlerRegister(app_sysException);


    /* User's Task */
#if ZBHCI_EN
    zbhciInit();
    ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
    ev_on_poll(EV_POLL_IDLE, app_task);

    /* Read the pre-install code from NV */
    if(bdb_preInstallCodeLoad(&g_appCtx.tcLinkKey.keyType, g_appCtx.tcLinkKey.key) == RET_OK){
        g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_appCtx.tcLinkKey.keyType;
        g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_appCtx.tcLinkKey.key;
    }

    /* Set default reporting configuration */
    uint8_t reportableChange = 0x01;

    /* OnOff */
    bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF,
            0, 65000, (uint8_t *)&reportableChange);
    bdb_defaultReportingCfg(APP_ENDPOINT2, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF,
            0, 65000, (uint8_t *)&reportableChange);

    /* OnOffCfg */
    bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, CUSTOM_ATTRID_DECOUPLED,
            0, 65000, (uint8_t *)&reportableChange);
    bdb_defaultReportingCfg(APP_ENDPOINT2, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, CUSTOM_ATTRID_DECOUPLED,
            0, 65000, (uint8_t *)&reportableChange);

    /* MultistateInput */
    uint16_t reportableChange_u16 = 0x01;
    bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
            ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE, 10, 0, (uint8_t *)&reportableChange_u16);
    bdb_defaultReportingCfg(APP_ENDPOINT2, HA_PROFILE_ID, ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
            ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE, 10, 0, (uint8_t *)&reportableChange_u16);


    /* Initialize BDB */
    bdb_init((af_simple_descriptor_t *)&app_ep1_simpleDesc, &g_bdbCommissionSetting, &g_zbBdbCb, 1);

    rf_setTxPower(MY_RF_POWER_INDEX);
}

