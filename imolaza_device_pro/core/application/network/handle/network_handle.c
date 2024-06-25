

#include "network_handle.h"

void (*fun_callback)(enum connect_progress progress, enum net_cause cause) = NULL;

/**
 * @brief 注册连接过程函数
 *
 * @param func 需要注册的回调函数
 * @param progress 连接状态
 * @param cause  原因
 * @return stat_m
 */
stat_m m_callable_network_registered_progress_callback(void (*func)(enum connect_progress progress, enum net_cause cause))
{
    stat_m stat = succ_r;
    fun_callback = func;
    return stat;
}
/**
 * @brief 连接诶过程触发处理中间函数
 * 
 * @param progress 
 * @param cause 
 * @return stat_m 
 */
stat_m m_static_network_handle_progress_call(enum connect_progress progress, enum net_cause cause)
{
    switch (progress)
    {
    case M_NETWORK_CONNECT_PROGRESS_CONNECT_READY:
        DEBUG_TEST( DB_I,"Not Initialized");
        break;
    case M_NETWORK_CONNECT_PROGRESS_START_LOGIN:
        DEBUG_TEST( DB_I,"Connected，Not Login");
        break;
    case M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS:
        DEBUG_TEST( DB_I,"Login And Connect Full Success");
        break;
    
    default:
        break;
    }
    if (fun_callback == NULL)
        return succ_r;
    else
        fun_callback(progress, cause);
    return succ_r;
}
