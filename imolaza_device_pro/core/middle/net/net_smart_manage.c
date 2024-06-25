/**
 * @file net_smart_manage.c网络智能管理单元
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "core/fset_lib.h"
/*
9、加入智能网络管理单元，会根据网络环境请求时间频率和信号强度，对网络请求作方式和频率控制
        在连接WIFI的时候需要智能
        在连接服务器的时候需要智能
        在HTTP请求的时候需要智能
        在发送数据的时候需要智能
*/
struct net_smart
{
    /** WiFi 智能计数 */
    uint32_t net_smart_wifi_count;
    /** 状态持续时间 */
    uint64_t net_smart_wifi_time;
    /** Server 智能 */
    uint32_t net_smart_server_count;
    /** Http 智能 */
    uint32_t net_smart_http_count;
    /** 数据发送 智能 */
    uint32_t net_smart_data_send_count;
};
struct net_smart net_smart_m = {0};

stat_m m_callable_net_smart_manage_init()
{
    return succ_r;
}





















/**
 * 
 * 
 * @brief 连接WiFi的操作  操作连接WIFi成功还是失败，失败如何处理，成功如何处理
 *          这种函数一般一对出现 分别 Option 和 Notify
 *          Option 一般处理异常情况
 *          Notify  一般调整连接频率
 *
 * @param in_is_ok 连接成功还是失败  WiFi
 * @return stat_m
 */
stat_m m_callable_net_smart__manage_wifi_net_option(bool in_is_ok, uint64_t in_time_s)
{
    if (in_is_ok)
    { /** 连接网络成功 */
        if (net_smart_m.net_smart_server_count == 0)
        {/** 连接成功 并且 服务器也没有很长时间处理 蓝色*/

        }
        net_smart_m.net_smart_wifi_count = 0;
    }
    else
    {/** 网络WiFi失败 */

        if (in_time_s - net_smart_m.net_smart_wifi_time >= 120 && net_smart_m.net_smart_wifi_count > 1)
        {/** 超过120s 断开次数超过1次 */
            
            net_smart_m.net_smart_wifi_time = in_time_s;
        }
        // else
        //     net_smart_m.net_smart_wifi_time = in_time_s;
        net_smart_m.net_smart_wifi_count++;
    }
    return succ_r;
}
stat_m m_callable_net_smart__manage_wifi_net_notify( uint64_t in_time_s)
{
    if (1)
    { /** 连接网络成功 */
        if (net_smart_m.net_smart_server_count == 0)
        {/** 连接成功 并且 服务器也没有很长时间处理 蓝色*/

        }
        net_smart_m.net_smart_wifi_count = 0;
    }
    else
    {
        if (net_smart_m.net_smart_wifi_count == 0)
        {

        }
        net_smart_m.net_smart_wifi_count++;
    }
    return succ_r;
}
