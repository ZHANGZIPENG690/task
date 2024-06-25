/**
 * @file smart_schedule.h
 * @author your name (you@domain.com)
 * @brief 智能计划相关函数、包括解析和
 * @version 0.1
 * @date 2023-10-05
 *
 * @copyright Copyright (c) 2023
 *
 */

/**
 *       源    1|2|3|4,2#10;3#5
 *
 * @brief 智能计划 直接存放的缓存
 *          会有 31 个存储位置
 */

#ifndef SMART_SCHEDULE__H__
#define SMART_SCHEDULE__H__

#include "core/fset_lib.h"

/** 智能计划的区域调整id */
struct m_smart_channel
{
    /** 智能计划的区域调整id */
    uint32_t id;
    /** 智能计划的区域调整时间 */
    uint32_t time;
};

struct m_smart_array
{
    /** 对应的 id */
    uint64_t uid;
    /** 运行中也会更新 所以需要这个 */
    bool is_update;
    /** 每一个计划都会有最多 32 个新的区域浇水时间信息 */
    struct m_smart_channel new_water_msg[32];
    /** 每一个计划都会有最多 32 个不执行的区域列表 */
    uint32_t new_ignore_msg[32];
    /** 15min 过期  记录添加的时间 */
    uint64_t inflag_time_s;
    /** pc指针 ，循环找到最近的信息   */
    uint8_t pc;
};

/**
 * @brief 这个初始化的目的是 取出存在flash的智能计划数据
 *
 *
 * @return stat_m
 */
stat_m m_callable_smart_running_storage_init(void);

/**
 * @brief 追加一个参数到smart 数组，  如果是数组的0
 *
 * @param in_uid 计划id
 * @param in_src 源格式数据
 * @param in_time_s 时间 （这个时间是很有必要的，超过15min的都会被清除）
 * @return stat_m
 */
stat_m m_callable_smart_running_storage_append(uint64_t in_uid, char *in_src, uint64_t in_time_s);

/**
 * @brief 删除一个
 *
 * @param in_uid
 * @return stat_m
 */
stat_m m_callable_smart_running_storage_clean(uint64_t in_uid);

/**
 * @brief 查询计划的区域是否有被忽略
 *
 * @param in_uid
 * @param in_cid
 * @return stat_m
 */
stat_m m_callable_smart_running_storage_is_ignore(uint64_t in_uid, uint8_t in_cid, uint64_t in_time_s);
/**
 * @brief 查询区域是否有更改目标时间
 *
 * @param in_uid 计划id
 * @param in_cid 查询的区域id
 * @return stat_m
 */
stat_m m_callable_smart_running_storage_is_change(uint64_t in_uid, uint8_t in_cid, uint32_t *out_time, uint64_t in_time_s);

#endif /* SMART_SCHEDULE__H__ */
