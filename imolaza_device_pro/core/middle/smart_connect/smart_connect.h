

#ifndef __SMART__H__
#define __SMART__H__

#include "core/fset_lib.h"


enum smart_connect_type
{
    M_SMART_TYPE_WIFI = 0,
    M_SMART_TYPE_HTTP,
    M_SMART_TYPE_TCP,
};



/**
 * @brief 智能处理连接频率，在需要连接某个类型前调用即可 目前支持 WIFI、Http、TCP三种 需要智能处理 in_need_pro = true 否则false会删除传入的依据：smart_count
 *
 * @param type 连接类型
//  * @param smart_count 连接智能依据
 * @param in_need_pro 需要使用或者清空依据
 * @return stat_m
 */
stat_m m_callable_middle_smart_connect_pro(enum smart_connect_type type, bool in_need_pro);





#endif /* __SMART__H__ */