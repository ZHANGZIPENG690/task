#ifndef __SENSOR_FLOW__H__
#define __SENSOR_FLOW__H__

#include "core/fset_lib.h"
#include "sensor.h"

// /**
//  * @brief 存储每个区域的各自流速
//  * @return stat_m
//  */
// stat_m m_static_flow_pulse_time_save(void);

/**
 * @brief 获取当前运行区域的脉冲周期
 * @param zone_stop_time 当前区域本次运行停止时间
 * @param current_time 当前时间
 * @param clear 是否重置数据标志位
 * @return uint64_t
 */
uint16_t m_static_pulse_period_detection_monitor(uint64_t current_time, uint8_t channel_id, bool clear);

/**
 * @brief 判断当前时间是否在本次区域运行时间范围内
 * @param currenttime 当前时间
 * @return statm
 */
stat_m m_callable_whether_current_time_range(uint64_t currenttime);

/**
 * @brief 通过计算出的流速以及提供的当前区域运行时间  算出  水 流量
 * @param zone_should_runtime 当前区域本次运行时间
 * @return float
 */
float m_callable_get_flow_value(uint64_t zone_should_runtime);

/**
 * @brief 用来去中断 获取 脉冲
 * @return statm
 */
stat_m m_callable_pulse_recording(void);

/**
 * @brief 在外部 检测函数中调用, 检测 相邻且是同一个区域的 两个脉冲对应的时间戳  计算出脉冲周期，从而算出 流速
 * @param channel_id 下一个将要运行的区域id
 * @param running_time 当前运行区域的   运行时间
 * @param out_final_vue 获取到的流量值
 * @param in_times 当前时间
 * @return statm
 */
stat_m m_callable_zone_transfore_get(uint8_t channel_id, uint32_t running_time, float *out_final_vue);

/**
 * @brief 计算得到的各个区域流速值  存储传递  向外部传递
 * @param channel_id 需要存储的 该区域  流速
 * @return float
 */
stat_m m_static_flow_rate_stored_value_set(uint8_t channel_id);

/**
 * @brief 向外部存储的地方   获取 各个区域的流速值
 * @param channel_id 需要存储的 该区域  流速
 * @return uint16_t
 */
stat_m m_static_flow_rate_stored_value_get(uint8_t channel_id);

/**
 * @brief 在外部 检测函数中调用, 检测 相邻且是同一个区域的 两个脉冲对应的时间戳  计算出脉冲周期，从而算出 流速
 * @param detection_cycle 当前漏水检测周期
 * @param out_final_vue 获取到的流量值
 * @param in_times 当前时间
 * @param time_interval 间隔小时1   间隔分钟2
 * @param flag 是否清除数据
 * @return statm
 */
stat_m m_callable_leakage_velocity_detection(uint32_t detection_cycle, float *out_final_vue, uint64_t in_times, uint8_t time_interval, bool flag);

#endif