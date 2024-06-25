/**
 * @file water_delay.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */
#include "core/fset_lib.h"
#include "../../tool/data_parse/data_parse.h"


#ifndef __WATERDELAY__H__
#define __WATERDELAY__H__

/**
 * @brief Load information from memory
 * @todo Unit Input ...
 *
 * @return stat_m
 */
stat_m m_callable_water_delay_init(void);

/**
 * @brief Configure the start time and end time of water-delay
 * @todo Unit Input ...
 *
 * @param start_time start time
 * @param end_time end time
 * @return stat_m
 */
stat_m m_callable_water_delay_set(uint64_t start_time, uint64_t end_time, uint64_t in_time_s);

/**
 * @brief Determine whether the current time is within the water-delay range
 * @todo Unit Out ...
 *
 * @param current_time  Current time
 * @return stat_m
 */
// bool m_callable_water_delay_current_is_effective(uint64_t current_time);
stat_m m_callable_water_delay_current_is_effective(uint64_t current_time);
/**
 * @brief 清除计划跳过
 * 
 * @return stat_m 
 */
stat_m m_callable_water_delay_clean(void);


#endif
