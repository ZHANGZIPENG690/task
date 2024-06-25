/**
 * @file manual.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */



#ifndef __MUANUAL__H__
#define  __MUANUAL__H__
#include "core/fset_lib.h"



/**
 * @brief 手动运行初始化
 * 
 * @return stat_m 
 */
stat_m m_callable_manual_init(void);
/**
 * @brief 
 * 
 * @param event_id 
 * @param pre_channel 
 * @param next_channel 
 * @param current_time_ms 
 * @return stat_m 
 */
stat_m m_callable_manual_event_input(enum key_id event_id , uint8_t pre_channel , uint8_t next_channel , uint64_t current_time_ms);


/**
 * @brief 事件触发检测 轮询
 * 
 * @param current_time_ms 时间
 * @return stat_m 
 */
stat_m m_callable_manual_function_monitor( uint64_t current_time_ms);
/**
 * @brief 设置自动停止时间
 * 
 * @param time_s 
 * @return stat_m 
 */
stat_m m_callable_manual_set_auto_stop_s(uint32_t time_s );

/**
 * @brief 事件处理
 *
 * @param event_id 事件id
 * @param pre_channel 上一个区域
 * @param next_channel 下一个 当前区域
 * @param has_benn_runnint_time 已经运行时间
 * @param current_time 当前时间
 * @return stat_m
 */
stat_m m_ext_manual_event_handle(enum operate_event event_id,  uint8_t pre_channel, uint8_t next_channel, uint32_t has_benn_runnint_time,  uint64_t current_time);



/**
 * @brief 外部相关事件处理  电流等
 * 
 * @param event_id 事件id
 * @param evt 事件数据
 * @param current_time_s 触发时间
 * @return stat_m 
 */
stat_m m_callable_manual_event_export_input(int event_id,int evt, uint64_t current_time_s);

/**
 * @brief 获取运行区域
 * 
 * @param in_zone 
 * @return stat_m 
 */
stat_m m_callable_manual_running_zone_get(int *in_zone);

/**
 * @brief 修改时长
 * 
 * @param kid 
 * @param currentTempVue 
 * @return stat_m 
 */
stat_m m_callable_manual_adjust_time(enum key_id kid,uint8_t currentTempVue);

/**
 * @brief 获取记录操作
 * 
 * @param 
 * @param  
 * @return stat_m 
 */
int m_callable_manual_get_sol_even(void);

#endif  /* __MUANUAL__H__ */