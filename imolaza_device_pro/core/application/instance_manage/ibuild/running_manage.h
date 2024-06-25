


/**
 * @file instance_manage.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @date 2023-07-10
 */

#include "core/fset_lib.h"
#include "core/application/schedule_manage/manage/instance.h"







// 内部


/**
 * @brief 运行预处理，10 min请求发送，直接由事件分发处理调用
 * @brief 1、收到消息解析出参数
 * @brief 2、生成实例
 * 
 * @param sch_doc_item 
 * @param current_time 
 * @return stat_m 
 */
stat_m m_callable_running_manage_pretreatment(schedule_doc_t *sch_doc_item, uint64_t current_time);


/**
 * @brief According to the scheduled instance content:
 * @brief 1、Distribute events
 * @brief 2、Stop all other runs
 * 
 * @param schedule_instance The resulting running_manage instance
 * @param current_time current time
 * @return stat_m  success OK
 */
stat_m m_static_running_manage_decision(schedule_instance_obj *schedule_instance, uint64_t current_time);



/**
 * @brief Scheduled uptime monitoring( [45 min] m_ext_event_handle())
 * 
 * @param current_time current timestamp
 * @return stat_m 
 */
stat_m m_callable_running_manage_long_time_monitor(uint64_t current_time);



/**
 * @brief 
 * 
 * @param schedule_instance 
 * @param current_time 
 * @return stat_m 
 */
stat_m m_callable_running_manage_update_next_running_time(schedule_instance_obj *schedule_instance, uint64_t current_time);


/**
 * @brief 
 * 
 * @param schedule_instance 
 * @param current_time 
 * @return stat_m 
 */
stat_m m_callable_running_manage_notify_manage(schedule_instance_obj *schedule_instance, uint64_t current_time);




