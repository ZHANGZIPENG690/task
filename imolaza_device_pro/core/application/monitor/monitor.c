

// #include "monitor.h"
#include "core/fset_lib.h"

// #include "core/fset_lib.h"

// void m_callae_global_monitor_loop(void *args);




/**
 * @brief 状态检测函数原型初始化
 *
 * @return stat_m
 */
stat_m m_callable_global_monitor_init(void)
{
    mTaskCreate(NULL, m_ext_global_monitor_loop, "monitor", 1024 * 3, NULL, 2, NULL);
   // mTaskCreate(NULL, m_callable_sensor_flow_value_monitor, "m_callable_sensor_flow_value_monitor", 1024*2, (void *)3, 5, NULL); //--sock tcp link
    return succ_r;
}

