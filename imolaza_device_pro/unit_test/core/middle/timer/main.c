



















#include "core/fset_lib.h"



/**
 * @brief 定时器初始化
 * 
 * @return stat_m 
 */
stat_m m_callable_timer_manage_time_init(void);
/**
 * @brief 时间走时
 * 
 * @return stat_m 
 */
stat_m m_callable_timer_manage_time_monitor(void);

/**
 * @brief 设置设备的时间
 * @todo Unit Input ...
 *
 * @param time   设备时间
 * @param tz 时区
 * @return stat_m
 */
stat_m m_callable_timer_manage_time_set(uint64_t time, int tz);
/**
 * @brief 季节调整，，简单的意思就是 在  time_inav1 到  time_inav2 之间的时间区间范围内 是 offset_tz
 *                      其他的是 offset_tz2
 * @todo Unit Input ...
 *
 * @param time_inav1 第一个时区开始的时间
 * @param offset_tz 时区1
 * @param time_inav2 第一个时区结束的时间 第二个时区开始的时间
 * @param offset_tz2 时区2
 * @return stat_m
 */
stat_m m_callable_timer_manage_timezone_switch_set(uint64_t time_inav1, int offset_tz, uint64_t time_inav2, int offset_tz2);

void main()
{

}