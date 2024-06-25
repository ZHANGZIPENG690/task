
/**
 * @file timer_manage.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */

#ifndef __TIMER_MANAGE_H__
#define __TIMER_MANAGE_H__

#include "core/fset_lib.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

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
stat_m m_callable_timer_manage_time_update(void);

/**
 * @brief 设置设备的时间
 * @todo Unit Input ...
 *
 * @param time   设备时间
 * @param tz 时区
 * @return stat_m
 */
stat_m m_callable_timer_manage_time_set(uint64_t time, int tz, int type);
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

/**
 * @brief 获取设备时间
 *
 * @return uint64_t
 */
uint64_t m_callable_timer_manage_get_device_time(void);
/**
 * @brief 获取UTC时间
 *
 * @return uint64_t
 */
uint64_t m_callable_timer_manage_get_utc_time(void);
/**
 * @brief 获取当天的时间 小于等于86400
 *
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_hhmmss(void);
/**
 * @brief 获取当天的星期
 *
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_week(uint64_t in_time_s);
/**
 * @brief 获取当天的天
 *
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_day(uint64_t timp);
/**
 * @brief 获取当天的设备 零点 时间
 *
 * @return uint64_t
 */
uint64_t m_callable_timer_manage_get_device_zero_time(void);

/**
 * @brief 获取两个时间戳之间的小时数
 *
 * @param timpA
 * @param timpB
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_diff_hour_num(uint64_t timpA, uint64_t timpB);
/**
 * @brief 解析一个时间戳的全部记录  年月日时分秒
 *
 * @param in_tim 时间
 * @param out_tm 输出
 * @return stat_m
 */
stat_m m_callable_timer_manage_get_ymd_hms(uint64_t in_tim, int *out_tm);
/**
 * @brief Get Date - TimeStamp
 *
 * @param mohth Month
 * @param day Day
 * @param hour Hour
 * @return uint64_t
 */
uint64_t m_callable_timer_manage_get_spec_date_time(int mohth, int day, int hour);

/**
 * @brief 按照时间类型对设备进行  冬令时夏令时切换
 * @param timestamp
 * @param timezone_offset
 * @param time_type
 * @param next_transition_timestamp
 * @param next_timezone_offset
 * @return stat_m
 */
stat_m m_callable_get_next_dst_transition_type(time_t timestamp, int timezone_offset, int time_type, time_t *next_transition_timestamp, int *next_timezone_offset);

#endif /** __TIMER_MANAGE_H__ */