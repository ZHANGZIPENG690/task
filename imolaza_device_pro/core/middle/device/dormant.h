



/**
 * @file self_test.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */


#ifndef __DORMANT__H__
#define __DORMANT__H__

#include "core/fset_lib.h"
#include "../tool/data_parse/data_parse.h"


/**
 * @brief 
 * @todo Uint Input ...
 * 
 * @return stat_m 
 */
stat_m m_callable_device_dormant_init(void);




/**
 * @brief 设置休眠模式
 * @todo Uint Input ...
 * 
 * @param start_time 
 * @param end_time 
 * @param mode 
 * @return stat_m 
 */
stat_m m_callable_device_dormant_set(uint64_t start_time ,uint64_t end_time , uint8_t  mode);

/**
 * @brief 清除休眠模式
 * 
 * @return stat_m 
 */
stat_m m_callable_device_dormant_clean(void);

/**
 * @brief 
 * @todo Uint Out ...
 * 
 * @param current_time 
 * @return true 
 * @return false 
 */
stat_m m_callable_device_dormant_current_is_effective(uint64_t current_time);

/**
 * @brief Test Dormant
 * 
 * @param a Start Time
 * @param b End Time
 */
void m_test_dormant_set(uint64_t a, uint64_t b);
#endif

