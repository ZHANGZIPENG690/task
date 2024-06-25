

/**
 * @file weather.h
 * @author your name (you@domain.com)
 * @brief 天气相关的 所有集合函数
 * @version 0.1
 * @date 2023-10-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "core/fset_lib.h"




#ifndef __WEATHER_H__
#define __WEATHER_H__








/**
 * @brief 天气相关初始化
 * 
 * @return stat_m 
 */
stat_m m_callable_weather_manage_init(void );

/**
 * @brief 设置一个带id的天气信息
 * 
 * @param in_uid id
 * @param weath_info 对应的天气信息 
 * @return stat_m 
 */
stat_m m_callable_weather_manage_set( uint64_t in_uid , int weath_info,uint64_t in_time_s);

/**
 * @brief 45 min 请求计数
 * 
 * @param in_time_s  当前的时间
 * @return stat_m 
 */
stat_m m_callable_weather_manage_monitor(uint64_t in_time_s);


/**
 * @brief 查看计划的天气运行信息
 * 
 * @param in_id 计划id
 * @param weather_code 表征天气信息的代码 
 * @return stat_m 
 */
stat_m m_callable_weather_manage_get_is_effective(uint64_t in_id ,int *weather_code);



/**
 * @brief 清除天气缓存信息
 * 
 * @param in_id 
 * @return stat_m 
 */
stat_m m_callable_weather_manage_clean(uint64_t in_id);




#endif






