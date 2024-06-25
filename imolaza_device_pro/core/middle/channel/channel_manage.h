#include "core/fset_lib.h"
/**
 * @file channel_manage.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */

#ifndef __CHANNEL_MANAGE__H__
#define __CHANNEL_MANAGE__H__

#define M_SMART_SACK_AR 0
#define M_SMART_SACK_IR 1
#define M_SMART_SACK_ASA 2

#define DEFAULT_AR 1.5f
#define DEFAULT_IR 0.1f
#define DEFAULT_ASA 0.2f

/** 禁用和启用 M_STATIC_CHANNEL_STATE_DISABLE */
enum channel_state
{
    // 禁用
    M_STATIC_CHANNEL_STATE_DISABLE,
    // 启用
    M_STATIC_CHANNEL_STATE_ENABLE,
};
// enum channel_param_config
// {
//     // 配置A 的信息
//     M_STATIC_CHANNEL_CONFIG_A,
//     // 配置B 的信息
//     M_STATIC_CHANNEL_CONFIG_B,
// };

struct channel_manage_conf
{
    /** 通道 id*/
    uint8_t channel_id;
    /** 通道状态 */
    enum channel_state channel_state;
    /** 如果是禁用的话 禁用的开始时间*/
    uint64_t disable_start_time;
    /** 如果是禁用的话 禁用的结束时间*/
    uint64_t disable_end_time;
    /** 区域的参数配置 A */
    uint32_t channel_config_a[2]; // sack 0   water 1
    /** 区域的参数配置 B */
    float channel_config_b[3]; // sack 0   water 1
    /** 区域配置选项 ,根据这个表示使用 A B 还是 不用 */
    int channel_config_choose;

    /*流速*/
    float flow_speed_f;
    /** 离线区域百分比 */
    // int offline_channel_percent[13][1];
    // /** 离线百分比启用 */
    //  channel_percent_enable;
};
/**
 * @brief 区域管理初始化
 *
 * @param device_type 是被通道数量
 * @return stat_m
 */
stat_m m_callable_channel_manage_init(int device_type);
/**
 * @brief 关闭一个区域
 * @todo Unit Input ...
 *
 * @param channel 区域
 * @param start_time 关闭有效的有效开始时间
 * @param end_time 有效的结束时间
 * @return stat_m
 */
stat_m m_callable_channel_manage_disable(uint8_t channel, uint64_t start_time, uint64_t end_time);

/**
 * @brief 启用一个区域
 * @todo Unit Input ...
 *
 * @param channel 启用一个区域
 * @return stat_m
 */
stat_m m_callable_channel_manage_enable(uint8_t channel);
/**
 * @brief 设置一个区域的状态  开启或者关闭
 *
 * @param enable 状态
 * @param channel 区域
 * @param start_time 关闭有效的有效开始时间
 * @param end_time 有效的结束时间
 * @return stat_m
 */
stat_m m_callable_channel_manage_state_set(bool enable, uint8_t channel, uint64_t start_time, uint64_t end_time);

/**
 * @brief  判断当前的时间是否在区域运行的范围之内
 *
 * @param channel 查询的区域
 * @param current_time 当前的时间
 * @return stat_m
 */
stat_m m_callable_channel_manage_current_is_effective(uint8_t channel, uint64_t current_time);

/**
 * @brief 离线百分比设置，针对于所有区域
 * @todo Unit Input ...
 *
 * @param param
 * @return stat_m
 */
stat_m m_callable_channel_manage_offline_running_percent(uint8_t channel_id, int percent);

/**
 * @brief 设置一个区域的运行配置 多个浸泡时间配置信息 （配置A 和配置B）
 *
 * @param channel_id 区域id
 * @param cconfig  配置A 还是B
 * @param src_str 参数  格式 A*B
 * @return stat_m
 */
// stat_m m_callable_channel_manage_set_channel_sack_param(uint8_t channel,uint16_t running_time , uint16_t sack_time);
// stat_m m_callable_channel_manage_set_channel_sack_param(uint8_t channel, enum channel_param_config cconfig, char *src_str);
stat_m m_callable_channel_manage_set_channel_sack_param(uint8_t channel, enum channel_param_config cconfig, uint32_t config_water, uint32_t config_sack, int in_enable);

// stat_m m_callable_channel_manage_get_channel_param(uint8_t channel_id,uint16_t *running_time , uint16_t *sack_time);
/**
 * @brief 获取区域浸泡配置信息
 * @todo Unit Out ...
 *
 * @param channel_id 区域id
 * @param cconfig A 或者 B
 * @param running_time  返回的运行时间
 * @param sack_time 返回的浸泡时间
 * @return stat_m
 */
stat_m m_callable_channel_manage_get_channel_param(uint8_t channel_id, enum channel_param_config cconfig, uint8_t *out_sack_type, uint32_t *running_time, uint32_t *sack_time,
                                                   float *out_ar, float *out_ir, float *out_asa);
/**
 * @brief 配置修改，需要传递到flash
 *
 * @param config 新的配置 敬爱唔会覆盖旧的配置
 * @return stat_m
 */
stat_m m_static_channel_manage_config_change_config(struct channel_manage_conf *config);
/**
 * @brief 删除一个配置
 *
 * @param chan_id 根据区域ID删除
 * @return stat_m
 */
stat_m m_static_channel_manage_config_change_config_del(uint8_t chan_id);
/**
 * @brief  旧爱再配置初始化
 *
 * @param config 配置的信息
 * @param device_type 设备类型  根据设备类型来判断需要加载再多少配置
 * @return stat_m
 */
stat_m m_static_channel_manage_config_init(struct channel_manage_conf *config, int device_type);

/**
 * @brief 调试信息
 *
 */
void m_test_channel_manage_get_param(void);

/**
 * @brief 设置区域流速   服务器改流速
 * @param channel
 * @param flow_speed
 * @return stat_m
 */
stat_m m_callable_channel_flow_speed_set(uint8_t channel, float flow_speed);

/**
 * @brief 获取该区域流速
 * @param channel
 * @param flow_speed
 * @return stat_m
 */
stat_m m_callable_channel_flow_speed_get(uint8_t channel, float *flow_speed);
/**
 * @brief 获取多个区域在运行时的 智能配置参数
 *
 * @param in_channel_info Zone Example ： 1#2#5#6#9
 * @param out_ar_info 1#60&60 # 2#1.2&3.2&1.1
 * @return stat_m
 */
stat_m m_callable_channel_smart_config_a_b_get(char *conff_a_b, char *in_channel_info, char *out_ar_info);
#endif