

#ifndef __NEW_lawn__H__
#define __NEW_lawn__H__

#include "core/fset_lib.h"

#define MAX_TIME_NUM 24
#define MAX_SCHEDU_NUM 16
/**
 * Add a new lawn pattern to a start schedule
 */
stat_m m_static_new_lawn_time_list_add_time(uint64_t in_id, uint32_t *in_time, int in_len);
/**
 * Remove the new lawn mode from a start schedule
 */
stat_m m_static_new_lawn_time_list_rm_one_time(uint64_t in_id);
/**
 * @brief 根据初始化的参数 查找最近上一次的运行时间
 *
 * @param in_id
 * @param pre_time
 * @return uint64_t
 */
uint64_t m_static_new_lawn_srech_pretime(uint64_t in_id, uint64_t pre_time);

/**
 * @brief 根据初始化的参数 查找最近下一次的运行时间
 * Find a recent time based on the current time ( Next Time )
 */
uint64_t m_static_new_lawn_srech_nexttime(uint64_t in_id, uint64_t pre_time);

/**
 * @brief 解析拆婚如的新草坪时间 ，并且更新到全局
 * 
 * @param in_id 计划ID
 * @param consStr 计划参数
 */
void m_static_new_lawn_parse_parameters(uint64_t in_id , char * consStr);



/**
 * @brief 更新计划参数 并且返回下一次时间
 *
 * @param in_id 计划 ID 
 * @param in_param 示例参数
 * @param out_time 输出下一次最接近的时间
 * @param in_present_time 现在当前的时间  HHMMSS 格式
 * @return uint64_t
 */
stat_m m_static_new_lawn_object_update(uint64_t in_id, char *in_param, uint64_t *out_pretime, uint64_t *out_nexttime, uint64_t in_present_time);
/**
 * @brief 返回附近的两个时间
 * 
 * @param in_id 计划id
 * @param out_pre_time 返回的上一次最近的时间
 * @param out_next_time 返回的下一次最近的时间
 * @param pre_time 当前的时间
 * @return uint64_t 
 */
stat_m m_static_new_lawn_srech_near_time(uint64_t in_id,uint64_t *out_pre_time ,uint64_t *out_next_time,  uint64_t pre_time);

/**
 * @brief 从flash中加载洗脑平时间 ，当让如果有的话 没有返回失败
 * 
 * @param in_id  根据计划id  添加
 * @return stat_m 
 */
stat_m m_static_new_lawn_object_load(uint64_t in_id);
// /**
//  * @brief 为一个已经存在的新草坪计划设置时间参数，并且返回距离最近的下次运行时间
//  *
//  * @param in_param 时间参数信息
//  * @param out_time  输出最近的参数 可以为 NULL
//  * @param in_time_s 当前的时间
//  * @return stat_m
//  */
// stat_m m_callable_new_lawn_fix_and_add_time(uint64_t inst_id, char *in_param, uint64_t *out_time, uint64_t in_time_s);

#endif
