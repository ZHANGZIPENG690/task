#if !defined(MRTC_H)
#define MRTC_H
#include "core/fset_lib.h"

/**
 * @brief RTC init
 * @return stat_m
 */
stat_m m_callable_midllle_rtc_init(void);

/**
 * @brief Write time to RTC
 *
 * @param timestamp
 * @return stat_m
 */
stat_m m_callable_midlle_rtc_set_time(int year, int month, int tm_day, int tm_hour, int tm_min, int tm_sec, int tm_wday);
/**
 * @brief Get the time from the RTC
 *
 * @param timestamp
 * @return stat_m
 */
stat_m m_callable_midlle_rtc_get_time(uint64_t *timestamp);

/**
 * @brief RTC 线程  内部自己调用
 * @param a
 * @return void
 */
void m_static_rtc_time_runing_task(void *pvParameters);


/**
 * @brief 获取RTC电压值
 * @return float
 */
float m_callable_get_voltage_rtc(void);


/**
 * @brief RTC电量预警功能
 * @return int
 */
int m_callable_rtc_power_warning(char *percentage_battery_get);

#endif // MRTC_H
