
/**
 * @file instance_running.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */
#ifndef __INSTANCE_RUNNING_H__
#define __INSTANCE_RUNNING_H__

#include "core/fset_lib.h"
#include "../instance.h"
#include "../fsm/instance_fsm.h"
#include "../../../middle/event_distribution/time_link.h"
#include "../tools/instance_algorithm.h"

/**
 * @brief The call start execute new schedule
 * @param schedule_instance schedule instance object
 * @param start_time triggering time
 * @todo Unit Input ...
 *
 * @warning " in_instance_id " must be the exact time when the schedule ends, or the unpredictable results will occur
 * @return stat_m
 *          @retval fail_r fail
 *          @retval succ_r success
 */
stat_m m_callable_instance_running_execute_start(instance_obj *instance_running_instance, uint64_t start_time);

/**
 * @brief Event trigger entry when scheduled at runtime
 * @param event_id Event id
 * @param trigger_time triggering time
 * @todo Unit Input ...
 *
 * @return stat_m
 *          @retval fail_r Execution was successful but may not take effect
 *          @retval succ_r Execution was successful
 */
stat_m m_callable_instance_running_event_input(enum mschedule_event event_id, uint64_t trigger_time);

/**
 * @brief 資格是監控計劃執行的，必須循環調用 比如開個喜愛年成每個1s一次
 *
 * @param current_time
 * @return stat_m
 */
stat_m m_callable_instance_running_monitor(uint64_t current_time);
// /**
//  * @brief The event processing in the plan runs, if you want to use some notification server and so on, will pass the event ID and the plan itself
//  * @todo Unit Out ...
//  *
//  * @param event_id event id
//  * @param in_data instance_running obj
//  * @return stat_m  if return fail Causes a plan abnormal
//  */
// stat_m m_ext_instance_running_event_handle(enum fsm_change_cause cause, enum base_state pre_ste, enum base_state curr_ste, void *in_data, uint64_t in_time_s);

/**
 * @brief 執行數據反饋
 *
 * @param cmd 操作的指令
 * @param instance_running_id 實例id 這裏指的是計劃或者快速運行id
 * @param start_channel 當前開始區域
 * @param start_channel_time_sum 當前區域應該運行總時間
 * @param start_channel_will_running_time 當前區域將要運行時間
 * @param end_channel  上一個區域
 * @param end_channel_reality_time 上一個區域總共運行時間
 * @param end_channel_current_running_time  上一個區域本次運行時間
 * @param end_channel_state 上一個區域是否結束 是：1
 * @param running_once_sum 當前愛你批次運行時間
 * @param running_sum 到目前總的運行時間
 * @param instance_id 結束時間，一般爲0 ，計劃之子嗯不涉及實例id生成
 * @param in_time_s 發生的時間搓
 * @return stat_m
 */
// extern stat_m (*m_ext_instance_running_able)(
//     int cmd,
//     uint64_t instance_running_id,
//     uint8_t start_channel,
//     uint32_t start_channel_time_sum,
//     uint32_t start_channel_will_running_time,
//     uint8_t end_channel,
//     uint32_t end_channel_reality_time,
//     uint32_t end_channel_current_running_time,
//     uint32_t end_channel_state,
//     uint32_t running_once_sum,
//     uint32_t running_sum,
//     uint64_t instance_id,
//     uint64_t in_time_s);

stat_m m_static_instance_running_idleing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
stat_m m_static_instance_running_pauseing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
stat_m m_static_instance_running_sacking_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
stat_m m_static_instance_running_running_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
stat_m m_static_instance_running_hanguping_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

stat_m m_static_instance_running_running_to_pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
stat_m m_static_instance_running_sacking_to_pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

stat_m m_static_instance_running_running_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
stat_m m_static_instance_running_pauseing_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
stat_m m_static_instance_running_hanguping_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

stat_m m_static_instance_running_running_to_hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
stat_m m_static_instance_running_sacking_to_hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

stat_m m_static_instance_running_running_to_run_switch(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

/**
 * @brief all to stop
 *
 * @return stat_m
 */
stat_m m_static_instance_running_all_to_stop(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

/**
 * @brief 从空闲进入到准备运行状态
 *
 * @param cause
 * @param ste
 * @param pdata
 * @param in_time_s
 * @return stat_m
 */
stat_m m_static_instance_running_idle_to_prepare(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

/**
 * @brief 任意状态到空闲   可以作为强行停止
 *
 * @param cause
 * @param ste
 * @param pdata
 * @param in_time_s
 * @return stat_m
 */
stat_m m_static_instance_running_all_to_idle(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

// 反馈函数
/**
 * @brief 事件处理函数包含很多对服务器的反馈和IO驱动事件，中间层，调试是直接底层用
 *
 * @param cause 触发的原因  手动的还是事件到了自动触发   还能表示是左切换还是右切换
 * @param pre_ste 上一个区域的状态    用来和下一个（也就是当前区域）来准确描述变化细节
 * @param curr_ste 下一个 （当前） 区域状态
 * @param pre_obj 上一个序列的对象   本来是可以不用的 但是在时候手动切换的时候 上一个区域可能是任意一个没有完成的区域 ，所以这个比较有必要的
 * @param in_data  当前正在运行的实例对象   （不是序列对象）
 * @param in_time_s  当前的时间
 * @return stat_m 返回值
 */
stat_m m_static_instance_running_event_handle(enum fsm_change_cause cause, enum base_state pre_ste, enum base_state curr_ste, void *pre_obj, void *in_data, uint64_t in_time_s);
/**
 * @brief 跳过通知
 *
 * @param in_id 跳过计划id
 * @param skip_typr  跳过类型
 * @param in_time_s 触发时间
 * @return stat_m
 */
stat_m m_static_instance_running_skip_notify_event_handle(instance_doc_t* in_wtd, instance_obj *in_obj, uint64_t in_instance_id, uint16_t in_chan_id , enum skip_manage_type skip_typr,
                                                          enum seneor_chann sen_chan, enum seneor_type sen_type, int weather_code, uint64_t in_time_s);
// stat_m m_callable_instance_running_monitor(uint64_t current_time);

/**
 * @brief 清除实例队列
 *
 * @param root_inst_queue 队列 Root
 * @return int
 */
int m_static_instance_runnning_queue_clear(struct instance_running_sequences *root_inst_queue);

#endif /* __INSTANCE_RUNNING_H__ */
