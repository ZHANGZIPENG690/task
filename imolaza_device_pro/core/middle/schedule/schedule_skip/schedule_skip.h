/**
 * @file schedule_skip.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */


#ifndef __SCHEDULE_H__
#define  __SCHEDULE_H__

#include "core/fset_lib.h"
#include "schedule_skip.h"
#include "../../tool/data_parse/data_parse.h"


/**
 * @brief Load information from memory
 * @todo Unit Input ...
 *
 * @return stat_m
 */
stat_m m_callable_schedule_skip_init(uint64_t in_sid);

/**
 * @brief Configure the start time and end time of water-delay
 * @todo Unit Input ...
 *
 * @param sch_id schedule id
 * @param start_time start time
 * @param end_time end time
 * @return stat_m
 */
stat_m m_callable_schedule_skip_set(uint64_t sch_id, uint64_t start_time, uint64_t end_time,uint64_t in_time_s);

/**
 * @brief Determine whether the current time is within the water-delay range
 * @todo Unit Out ...
 *
 * @param sch_id schedule id
 * @param current_time  Current time
 * @return stat_m
 */
// bool m_callable_schedule_skip_current_is_effective(uint64_t sch_id, uint64_t current_time);
stat_m m_callable_schedule_skip_current_is_effective(uint64_t sch_id, uint64_t in_time_s);
/**
 * @brief 清除计划跳过信息
 * 
 * @param sch_id 计划id
 * @return stat_m 
 */
stat_m m_callable_schedule_skip_clean(uint64_t sch_id);






#endif