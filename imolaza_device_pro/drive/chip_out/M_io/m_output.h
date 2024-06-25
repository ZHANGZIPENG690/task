#if !defined(M_OUTPUT_H)
#define M_OUTPUT_H

#include "core/fset_lib.h"
#if !UNIT_TEST
#include "driver/gpio.h"
#endif
#include "../chip_out.h"



/**
 * @brief M端子初始化 
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_M_init(void);


/**
 * @brief M端子打开
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_M_open(void);


/**
 * @brief M端子关闭
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_M_close(void);


#endif // M_OUTPUT_H
