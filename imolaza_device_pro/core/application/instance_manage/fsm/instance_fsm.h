



#ifndef __INSTANCE_FSM__H__
#define __INSTANCE_FSM__H__

#include "core/fset_lib.h"
// #include "instance_running.h"
#include "../instance.h"
#include "../running/instance_running.h"
#include "../../../middle/event_distribution/event_distribution.h"

stat_m m_static_instance_running_fsm_monitor(void);

stat_m m_static_instance_running_fsm_init(void);

stat_m m_static_instance_running_fsm_post(uint8_t event_fag , enum fsm_change_cause cause, enum base_state ste, void *data, uint64_t time_s);

/**
 * @brief
 *
 * @param cause
 * @param ste
 * @param pdata
 * @param in_time_s
 * @return stat_m
 */
stat_m m_ext_instance_fsm__idle(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
/**
 * @brief
 *
 * @param cause
 * @param ste
 * @param pdata
 * @param in_time_s
 * @return stat_m
 */
stat_m m_ext_instance_fsm__run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
/**
 * @brief 当下一个状态是浸泡的状态的时候 切换需要做的事情
 *
 * @param cause
 * @param ste
 * @param pdata
 * @param in_time_s
 * @return stat_m
 */
stat_m m_ext_instance_fsm__sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
/**
 * @brief 当下一个状态是暂停状态的时候  切换需要做的事情
 *
 * @param cause
 * @param ste
 * @param pdata
 * @param in_time_s
 * @return stat_m
 */
stat_m m_ext_instance_fsm__pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
/**
 * @brief 当前下一个状态是准备的状态的时候  切换需要做的事情
 *
 * @param cause
 * @param ste
 * @param pdata
 * @param in_time_s
 * @return stat_m
 */
stat_m m_ext_instance_fsm__prepare(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);


/**
 * @brief all to stop
 * 
 * @param cause 
 * @param ste 
 * @param pdata 
 * @param in_time_s 
 * @return stat_m 
 */
stat_m m_ext_instance_fsm__stop(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

void m_static_inst_fsm_pre_state_change(enum base_state in_pre_state);

#endif