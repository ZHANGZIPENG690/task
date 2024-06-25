

/**
 * @file instance_manage.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @date 2023-07-10
 */
#ifndef __INSTANCE_MANAGE__H__
#define __INSTANCE_MANAGE__H__

#include "core/fset_lib.h"
#include "../instance.h"

/** 主要包含了计划实例生成(10)，运行冲突管理，跳过管理通知管理、下次运行更新，*/

/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_init(void);
/**
 * @brief 添加一个计划
 *
 * @param instance_manage
 * @return stat_m
 */
stat_m m_callable_instance_manage_add(char *src_str, enum inst_add_mode of_line, uint64_t in_time_s, uint64_t *out_uid);
/**
 * @brief 添加快速计划 （快速运行）
 *
 * @param instance_manage
 * @return stat_m
 */
stat_m m_callable_instance_manage_quick_add(char *src_str, uint64_t in_time_s);
/**
 * @brief
 *
 * @param instance
 * @return stat_m
 */
stat_m m_callable_instance_manage_change(instance_doc_t *in_doc, uint64_t in_time_s);
/**
 * @brief
 *
 * @param sch_id
 * @return stat_m
 */
stat_m m_callable_instance_manage_remove(uint64_t sch_id, uint64_t trig_time);
/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_remove_all(void);
/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_read_all(char *sch_info);

/**
 * @brief 启用一个计划
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_enable(uint64_t in_uid);
/**
 * @brief 禁用一个计划
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_disable(uint64_t in_uid);

// /**
//  * @brief 为一个已经存在的新草坪计划设置时间参数，并且返回距离最近的下次运行时间
//  *
//  * @param in_param 时间参数信息
//  * @param out_time  输出最近的参数 可以为 NULL
//  * @param in_time_s 当前的时间
//  * @return stat_m
//  */
// stat_m m_callable_new_lawn_fix_and_add_time(uint64_t inst_id, char *in_param, uint64_t *out_time, uint64_t in_time_s);

stat_m m_static_instance_list_update_spec_type(int sch_type, stat_m *(fun)(void *));

/**
 * @brief 刷新指定计划内容的时间
 *
 * @param in_type 指定类型
 * @return stat_m
 */
stat_m m_callable_instance_manage_update_spec_schedule_time(int in_type);
#endif
