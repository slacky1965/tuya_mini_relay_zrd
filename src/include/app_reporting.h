#ifndef SRC_INCLUDE_APP_REPORTING_H_
#define SRC_INCLUDE_APP_REPORTING_H_


int32_t forcedReportCb(void *arg);
void app_forcedReport(uint8_t endpoint, uint16_t claster_id, uint16_t attr_id);
void app_all_forceReporting(void *args);

#endif /* SRC_INCLUDE_APP_REPORTING_H_ */
