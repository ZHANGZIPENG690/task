
/**
 * @file event_distribution.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */
#ifndef __EVENT_DISTRBUTION_H__
#define __EVENT_DISTRBUTION_H__

#include "core/fset_lib.h"
#include "time_link.h"
#include "../../application/instance_manage/fsm/instance_fsm.h"





enum event_distribution_id
{
    M_EVENT_TIME_6_SEC,
    M_EVENT_TIME_10_SEC,
    M_EVENT_TIME_30_SEC,
    M_EVENT_TIME_1_MIN,
    M_EVENT_TIME_45_MIN,
};








/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_callable_event_distribution_init(void);

/**
 * @brief  The event transformation is then sent through a message notification
 * @todo Unit Input ...
 *
 * @param event_id Notification of the tag of the carry
 * @param node_time Defined point in time
 * @param node_tag Notification of the tag of the carry
 * @param node_obj Notification of the object of the carry
 * @param happen_timestamp Defined point in time
 *
 * @return stat_m
 */
stat_m m_callable_event_distribution_post(uint8_t event_id, uint64_t node_timestamp, uint8_t node_tag, void *in_obj, uint64_t happen_timestamp);

/**
 * @brief
 *
 * @param current_timestamp
 * @return stat_m
 */
stat_m m_callable_event_distribution_advancing(uint64_t current_timestamp);

/**
 * @brief  Event Transit Distribution Center
 * @todo Unit Out ...
 *
 * @param event_id Notification of the tag of the carry
 * @param node_obj Notification of the object of the carry
 * @param happen_timestamp Defined point in time
 *
 * @return stat_m
 */
stat_m m_callable_event_distribution_tiansit(uint8_t event_fag ,uint8_t event_id, enum fsm_change_cause cause, void *in_obj, uint64_t happen_timestamp);

/**
 * @brief 获取一个节点的时间
 * 
 * @param id 
 * @param out_time 
 * @return stat_m 
 */
stat_m m_static_time_link_notece_node_id_get_by_time(uint64_t id , uint64_t *out_time);


/**
 * @brief
 *
 * @param in_id
 * @return stat_m
 */
extern stat_m m_static_time_link_notece_node_enable(uint64_t in_id);
/**
 * @brief
 *
 * @param in_id
 * @return stat_m
 */
extern stat_m m_static_time_link_notece_node_disable(uint64_t in_id);



stat_m m_static_event_handle(enum event_distribution_id event_id);



extern stat_m m_static_instance_running_hammer_handle(uint8_t event_fag, enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
#endif /* __EVENT_DISTRBUTION_H__ */
