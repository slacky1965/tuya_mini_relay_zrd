#ifndef SRC_INCLUDE_APP_UTILITY_H_
#define SRC_INCLUDE_APP_UTILITY_H_

/* for clock_time_exceed() */
#define TIMEOUT_TICK_250MS  (250*1000)          /* timeout 250 ms   */
#define TIMEOUT_TICK_500MS  (500*1000)          /* timeout 500 ms   */
#define TIMEOUT_TICK_1SEC   (1000*1000)         /* timeout 1 sec    */
#define TIMEOUT_TICK_5SEC   (5*1000*1000)       /* timeout 5 sec    */
#define TIMEOUT_TICK_10SEC  (10*1000*1000)      /* timeout 10 sec   */
#define TIMEOUT_TICK_15SEC  (15*1000*1000)      /* timeout 15 sec   */
#define TIMEOUT_TICK_30SEC  (30*1000*1000)      /* timeout 30 sec   */
#define TIMEOUT_TICK_60SEC  (60*1000*1000)      /* timeout 60 sec   */
#define TIMEOUT_TICK_5MIN   (300*1000*1000)     /* timeout 5  min   */
#define TIMEOUT_TICK_30MIN  (1800*1000*1000)    /* timeout 30 min   */

/* for TL_ZB_TIMER_SCHEDULE() */
#define TIMEOUT_100MS              100          /* timeout 100 ms   */
#define TIMEOUT_250MS              250          /* timeout 250 ms   */
#define TIMEOUT_500MS              500          /* timeout 500 ms   */
#define TIMEOUT_750MS              750          /* timeout 750 ms   */
#define TIMEOUT_1SEC        (1    * 1000)       /* timeout 1 sec    */
#define TIMEOUT_2SEC        (2    * 1000)       /* timeout 2 sec    */
#define TIMEOUT_3SEC        (3    * 1000)       /* timeout 3 sec    */
#define TIMEOUT_4SEC        (4    * 1000)       /* timeout 4 sec    */
#define TIMEOUT_5SEC        (5    * 1000)       /* timeout 5 sec    */
#define TIMEOUT_7_5SEC      (75   * 100)        /* timeout 7.5 sec  */
#define TIMEOUT_10SEC       (10   * 1000)       /* timeout 10 sec   */
#define TIMEOUT_15SEC       (15   * 1000)       /* timeout 15 sec   */
#define TIMEOUT_30SEC       (30   * 1000)       /* timeout 30 sec   */
#define TIMEOUT_1MIN        (60   * 1000)       /* timeout 1 min    */
#define TIMEOUT_1MIN30SEC   (90   * 1000)       /* timeout 1.5 min  */
#define TIMEOUT_2MIN        (120  * 1000)       /* timeout 2 min    */
#define TIMEOUT_5MIN        (300  * 1000)       /* timeout 5 min    */
#define TIMEOUT_10MIN       9600  * 1000)       /* timeout 10 min   */
#define TIMEOUT_15MIN       9900  * 1000)       /* timeout 15 min   */
#define TIMEOUT_30MIN       (1800 * 1000)       /* timeout 30 min   */
#define TIMEOUT_60MIN       (3600 * 1000)       /* timeout 60 min   */

int32_t delayedMcuResetCb(void *arg);
int32_t delayedFactoryResetCb(void *arg);
int32_t delayedFullResetCb(void *arg);
uint32_t itoa(uint32_t value, uint8_t *ptr);
uint64_t atoi(uint16_t len, uint8_t *data);
uint32_t from24to32(const uint8_t *str);
uint64_t fromPtoInteger(uint16_t len, uint8_t *data);
uint8_t set_zcl_str(uint8_t *str_in, uint8_t *str_out, uint8_t len);
uint64_t reverse64(uint64_t in);
uint32_t reverse32(uint32_t in);
uint16_t reverse16(uint16_t in);
uint8_t *print_str_zcl(uint8_t *str_zcl);
void start_message();
double nat_log(double x);

#endif /* SRC_INCLUDE_APP_UTILITY_H_ */
