

/**
 * @file schedule_stroage.h
 * @brief Connect the flash driver downwards and be related applications upwards
 * @public (application) stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note enter : 
 * @date 2023-07-10
 */

#ifndef __INSTANCE_STROAGE__H__
#define __INSTANCE_STROAGE__H__


#include "core/fset_lib.h"
#include "../recover/instance_recover.h"
#include "../../../middle/event_distribution/time_link.h"

// #define M_SCHEDULE_MODE_NEWLWAN_FLAG "nl"
#define MAX_SCHEDU_NUM 16
#define MAX_TIME_NUM 25

/** A single schedule stores structure and critical information */
struct flash_schedule_list_t
{
    /** The schedule ID for the location */
    uint64_t ft_schedult_id;
    /** The flash index schedule in the list */
    uint64_t ft_flash_position_index;
    /** The scheduled status of the location, enabled or disabled */
    bool ft_schedult_enable;
    /** Feature string */
    char * ft_schedult_str;

};

// http://c.biancheng.net/ml_alg/env.html
stat_m m_callable_schedule_stroage_init(void);
/**
 * @brief Deposit a string that represents the contents of the schedule in flash_schedule_list_t
 * @todo Unit Input ...
 * 
 * @param in_schedule_str A string that represents the contents of the plan
 * @param enable schedule enable
 * @return stat_m  success
 */
stat_m m_callable_schedule_stroage_write_string(uint64_t in_id, const char *in_schedule_str, bool enable);

/**
 * @brief Delete the storage item based on the schedule id
 * @todo Unit Input ...
 * 
 * @param schedule_id schedule id
 * @return stat_m M_OOK success
 */
stat_m m_static_schedule_stroage_remove_by_id(uint64_t schedule_id);





stat_m m_static_schedule_stroage_remove_all(void);

/**
 * @brief Extract the schedule content characters from the existing list by id
 * @todo Unit Out ...
 *
 * @param schedule_id schedule id
 * @param sch_str output schedule string
 * @param postion postion
 * @return stat_m
 */
stat_m m_callable_schedule_stroage_read_by_param(uint64_t schedule_id, char **out_str, int postion);
/**
 * @brief Read all existing schedule from flash, Contains disabled and enabled schedules,Start from 0 to MAX_SCHEDU_NUM, after which it returns failure, and the pointer returns to 0 after receiving the failure
 * @todo Unit Out ...
 *
 * @return stat_m M_ERR: ：Indicates that the content has been restored
 */
stat_m m_callable_schedule_stroage_load_all_from_flash(void);

// stat_m m_callable_stroage_schedule_print_all();


/**
 * @brief 启用
 * 
 * @param schedule_id 
 * @param in_time_s 
 * @return stat_m 
 */
stat_m m_callable_schedule_stroage_enable(uint64_t schedule_id, uint64_t in_time_s);

/**
 * @brief 关闭一个
 * 
 * @param schedule_id 
 * @param in_time_s 
 * @return stat_m 
 */
stat_m m_callable_schedule_stroage_disable(uint64_t schedule_id, uint64_t in_time_s);

/**
 * @brief 恢复计划内容
 * 
 * @return stat_m 
 */
stat_m m_callable_schedule_stroage_recover_item_all(void);
/**
 * @brief 打印全部的信息
 *
 */
void m_callable_schedule_storage_buffer_print_to_remote(void);
/**
 * @brief Test Add Recovery
 * 
 * @param dec 
 * @return stat_m 
 */
// stat_m m_rest_schedule_stroage_add( char* dec);
#endif /* __INSTANCE_STROAGE__H__ */
