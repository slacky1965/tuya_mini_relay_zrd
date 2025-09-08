#ifndef SRC_INCLUDE_APP_TIME_H_
#define SRC_INCLUDE_APP_TIME_H_

#define UNIX_TIME_CONST 946684800

#define JD0 2451911-366 // days until 01.01.2001

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t wday;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} ftime_t;

typedef enum {
    FT_WDAY_SUN = 0,
    FT_WDAY_MON,
    FT_WDAY_TUE,
    FT_WDAY_WED,
    FT_WDAY_THU,
    FT_WDAY_FRI,
    FT_WDAY_SAT,
} ft_wday_t;

void set_time_sent();
int32_t app_time_cmdCb(void *arg);
bool get_time_sent();
ftime_t *get_ftime();
void print_time();

#endif /* SRC_INCLUDE_APP_TIME_H_ */
