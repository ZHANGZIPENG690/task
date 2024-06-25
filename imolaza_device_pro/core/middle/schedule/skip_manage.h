/**
 * @file skip_manage.h
 * @author your name (you@domain.com)
 * @brief  关于计划跳过的集合查询，在计划跳过之前可以通过这个文件提供的函数进行查询
 *          文件整合了所有印象计划跳过的函数，但是不包含配置  仅供查询
 * @version 0.1
 * @date 2023-10-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __SKIP_MANAGE__H__
#define __SKIP_MANAGE__H__



#include "core/fset_lib.h"

// /**
//  * @brief  一共分为 6 种
//  * 
//     1、是否有休眠模式
//     2、是否有waterdelay
//     3、是否有计划跳过
//     4、是否有传感器跳过
//     5、是否有天气跳过
//     6、是否有饱和跳过
//  */
// enum skip_manage_type
// {
//     /** 休眠模式*/
//     M_SKIP_MANAGE_TYPE_DORMANT_SKIP,
//     /** water delay */
//     M_SKIP_MANAGE_TYPE_WATERDELAY_SKIP,
//     /** 单次计划 */
//     M_SKIP_MANAGE_TYPE_SIGN_SKIP,
//     /** 传感器 */
//     M_SKIP_MANAGE_TYPE_SENSOR_SKIP,
//     /** 天气 */
//     M_SKIP_MANAGE_TYPE_WEATHER_SKIP,
//     /** 饱和 */
//     M_SKIP_MANAGE_TYPE_SATURATION_SKIP,
// };




/**
 * @brief 跳过管理初始化，这个函数在射比初始化必须被调用 ，因为他会调用其他包括休眠计划跳过和waterdelay的数据加载
 * 
 * @return stat_m 
 */
stat_m m_callable_skip_manage_init(void);


/**
 * @brief   全局查询计划是否应该跳过   
 * @param in_id 计划id
 * @param skip_type
 *              包含
 *                  1、是否有休眠模式
                    2、是否有waterdelay
                    3、是否有计划跳过
                    4、是否有传感器跳过
                    5、是否有天气跳过
                    6、是否有饱和跳过
 * 
 * @param sen_chan 传感器通道
 * @param sen_type 传感器类型
 * @param weather_code 天气跳过代码
 * @param in_time_s 发生的时间
 * @return stat_m
 */
extern stat_m m_callable_skip_manage_query_is_effective(uint64_t in_id, enum skip_manage_type *skip_type, enum seneor_chann *sen_chan, enum seneor_type *sen_type, int *weather_code, uint64_t in_time_s);


/**
 * @brief 一次清除所有的跳过类型，在删除计划的时候可能会用到
 * 
 * @param in_id 计划id
 * @param in_time_s 发生的时间
 * @return stat_m 
 */
stat_m m_callable_skip_manage_clean_all(uint64_t in_id ,uint64_t in_time_s);


/**
 * @brief 删除一个
 *
 * @param in_uid
 * @return stat_m
 */
extern stat_m m_callable_smart_running_storage_clean(uint64_t in_uid);



#endif