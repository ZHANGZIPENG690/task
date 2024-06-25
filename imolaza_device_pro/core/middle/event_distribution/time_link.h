#include "core/fset_lib.h"
/**
 * @file time_link.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */

/**
 * @brief To run, you must call this function in a loop （unit 1s）
 * @todo Unit Input ...
 *
 * @param current_timestamp The timestamp of the current device
 * @return stat_m
 */
stat_m m_static_time_link_notece_node_advancing(uint64_t current_timestamp);

/**
 * @brief  Add a time node
 * @todo Unit Input ...
 *
 * @param node_id node id ( Recommendations and program IDs are aligned )
 * @param node_time Defined point in time
 * @param node_tag Notification of the tag of the carry
 * @param node_obj Notification of the object of the carry
 *
 * @return stat_m
 */
stat_m m_static_time_link_notece_node_add(uint64_t node_id, uint64_t node_timestamp, uint8_t node_tag, void *in_obj);

/**
 * @brief Instead of deleting but changing the node state and time
 * @todo Unit Input ...
 *
 * @param node_id node id ( Recommendations and program IDs are aligned )
 * @param node_time Defined point in time
 * @param node_tag Modify the tag of the notification node
 * @return stat_m
 */
stat_m m_static_time_link_notece_node_change(uint64_t node_id, uint64_t node_timestamp, uint8_t node_tag);

/**
 * @brief Remove all notifications for this node based on the id
 * @todo Unit Input ...
 *
 * @param id node id ( Recommendations and program IDs are aligned )
 * @return stat_m
 */
stat_m m_static_time_link_notece_node_remove_by_id(uint64_t node_id);
/**
 * @brief Remove all notifications
 * @todo Unit Input ...
 *
 * @return stat_m
 *
 */
stat_m m_static_time_link_notece_node_remove_all(void);

/**
 * @brief 修改区域对象
 *
 * @param id 计划id
 * @param data  需要修改的计划对象
 * @return stat_m
 */
stat_m m_static_time_link_notece_node_change_obj(uint64_t id, uint64_t in_notify_time,  uint8_t state, void *data , int in_change_cause);

/**
 * @brief
 *
 * @param in_id
 * @return stat_m
 */
stat_m m_static_time_link_notece_node_enable(uint64_t in_id);
/**
 * @brief
 *
 * @param in_id
 * @return stat_m
 */
stat_m m_static_time_link_notece_node_disable(uint64_t in_id);

/**
 * @brief Event callbacks for time notification
 * @todo Unit Out ...
 *
 * @param node_id node id ( Recommendations and program IDs are aligned )
 * @param node_time Defined point in time
 * @param node_tag Notification of the tag of the carry
 * @param node_obj Notification of the object of the carry
 *
 * @return stat_m
 */
stat_m m_static_time_link_notice_node_event_cb(uint64_t node_id, uint64_t node_time, uint8_t node_tag, void *in_obj);
/**
 * @brief 获取一个节点的时间
 * 
 * @param id 
 * @param out_time 
 * @return stat_m 
 */
stat_m m_static_time_link_notece_node_id_get_by_time(uint64_t id , uint64_t *out_time);