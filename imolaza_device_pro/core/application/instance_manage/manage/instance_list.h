


/**
 * @file schedule_list.h
 * @brief 
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @date 2023-07-10
 */

#ifndef __INSTANCE_LIST__H__
#define __INSTANCE_LIST__H__


#include "core/fset_lib.h"
#include "../instance.h"

/**
 * @brief Find a plan from the list of plans in the plan ID by plan ID
 * @todo Uint Input ...
 * 
 * @param schedule_id schedule id 
 * @param schedule_obj output schedule object
 * @return stat_m 
 */
stat_m m_static_instance_list_find_by_id(uint64_t schedule_id , instance_doc_t **schedule_obj);
/**
 * @brief The list adds a new schedule item
 * @todo Uint Input ...
 * 
 * @param sch_doc_id schedule obiect
 * @return stat_m 
 */
instance_doc_t * m_static_instance_list_add(instance_doc_t *sch_doc_item);
/**
 * @brief Remove a schedule item by the schedule ID
 * @todo Uint Input ...
 * 
 * @param sch_doc_id schedule id 
 * @return stat_m 
 */
stat_m m_static_instance_list_remove_by_id(uint64_t sch_doc_id);
/**
 * @brief remove all schedule item
 * @todo Uint Input ...
 * 
 * @return stat_m 
 */
stat_m m_static_instance_list_remove_all(void);
/**
 * @brief 
 * @todo Uint Out ...
 * 
 * @return stat_m 
 */
stat_m m_static_instance_list_get_all(char * info);

/**
 * @brief remove all schedule item
 * @todo Uint Input ...
 * 
 * @return stat_m 
 */
stat_m m_static_instance_list_enable(uint64_t in_uid);
/**
 * @brief 
 * @todo Uint Out ...
 * 
 * @return stat_m 
 */
stat_m m_static_instance_list_enable(uint64_t in_uid);






#endif










