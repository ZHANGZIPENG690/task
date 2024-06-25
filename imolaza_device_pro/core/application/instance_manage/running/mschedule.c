// /*
//     计划运行核心内容

//     包含计划运行的相关逻辑，举个例子
//         计划开始需要执行的变量和状态
//         （不包含端口操作和灯光刷新，只表示计划状态运行标志更新，也不会通知服务器，有上层调用来决定上述操作 或者在运行时提供一些特殊时刻的回调函数，用户实现该函数来达到该有的目的效果。

//     注意
//         会有一个宏定义控制该计划的通知方式是哪一种
//         关键函数以 m_eigenfunction 开头
// */

// // #include "tool.h"
// // #include "text.h"
// #include "core/fset_lib.h"
// #include "instance_running.h"
// #include "../fsm/instance_fsm.h"
// #include "../manage/instance.h"
// #include "../../../middle/event_distribution/time_link.h"
// #include "../tools/instance_algorithm.h"

// stat_m m_static_instance_running_idleing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
// stat_m m_static_instance_running_pauseing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
// stat_m m_static_instance_running_sacking_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
// stat_m m_static_instance_running_running_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
// stat_m m_static_instance_running_hanguping_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

// stat_m m_static_instance_running_running_to_pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
// stat_m m_static_instance_running_sacking_to_pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

// stat_m m_static_instance_running_running_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
// stat_m m_static_instance_running_pauseing_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
// stat_m m_static_instance_running_hanguping_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

// stat_m m_static_instance_running_running_to_hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);
// stat_m m_static_instance_running_sacking_to_hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

// stat_m m_static_instance_running_running_to_run_switch(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

// stat_m m_static_instance_running_all_to_idle(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s);

// /**********************************************************************************************/
// /**********************************************************************************************/
// /**********************************************************************************************/
// /**********************************************************************************************/

// /**
//  * @brief
//  *
//  * @return stat_m
//  */
// stat_m m_static_instance_running_idleing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     enum base_state next_status = M_BASE_STATUS_RUNNING;
//     instance_obj *wobj = (instance_obj *)pdata;
//     wobj->running_buff->rb_pre_stated = M_BASE_STATUS_IDLE;
//     wobj->running_buff->rb_execution_staus = M_INSTANCE_EXECUTION_STATUS_START;

//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_idleing_to_run (%d) ", in_time_s, wobj->running_buff->rb_pre_stated);

//     if (wobj != NULL)
//     {
//         // 有没有区域需要运行 或者 区域下一次运行是那一个
//         wobj->sio_status = M_BASE_STATUS_RUNNING;
//         wobj->running_buff->current_running_time_region.tr_start_time = in_time_s;
//         // wobj->sio_channel[wobj->running_buff-> rb_pc].runtime.tr_start_time = in_time_s;
//         // wobj->sio_channel[wobj->running_buff-> rb_pc].runtime.tr_end_time = in_time_s;
//         // DEBUG_TEST(DB_W,"m_schedule_event_start_callback %d ", wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time);
//         //  更新上一个区域
//         wobj->running_buff->rb_pre_pc = wobj->running_buff->rb_pc;
//         // 找下一个开始运行的区域
//         // m_collide_next_zone();
//         // 计算计划开始和结束时间
//         { // 开始和计划下一次需要处理的时间计算
//             // 先算一下 下一个区域是浸泡还是运行
//             // 如果是浸泡 不找下一个运行的区域
//             // 如果不是浸泡那就找下一个运行的区域
//             // 以上如果两个 的情况都是最后一个区域的话 那就说明下一次就是结束
//             // {
//             // 如果使用了浸泡需要计算下一次需要处理的时间 ，
//             m_static_instance_tools_calculate_next_state_switching_time(wobj->sio_pump_or_sack,
//                                                                         &wobj->running_buff->rb_WSTime_clone,
//                                                                         wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time,
//                                                                         wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time,
//                                                                         wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time,
//                                                                         /*_time.hhmmss*/ in_time_s % NUM_OF_DAY_SEC_SUM);
//             m_static_instance_tools_calculate_next_running_status(wobj->running_buff->rb_pc, wobj->sio_pump_or_sack,
//                                                                   wobj->running_buff->maxChan, wobj->sio_channel,
//                                                                   wobj->running_buff->rb_WSTime_clone,
//                                                                   &wobj->running_buff->rb_next_pc,
//                                                                   &next_status,
//                                                                   in_time_s);

//             // put_in(STAT_NEED_WIFI_BREAK, M_ERR); // 打断WiFi连接动画
//             // if (wobj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
//             {
//                 m_static_time_link_notece_node_change(wobj->sio_running_id, wobj->running_buff->rb_WSTime_clone, next_status); // 消化状态 修改下一个节点的状态
//             }
//             // wobj->sio_channel[wobj->running_buff-> rb_pc].runtime.tr_start_time = in_time_s;
//             wobj->sio_channel[wobj->running_buff->rb_pc].cd_curr_will_run_time = wobj->running_buff->rb_WSTime_clone - in_time_s;

//             m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);
//             // /** 驱动端口 */
//             wobj->running_buff->rb_execution_staus = M_INSTANCE_EXECUTION_STATUS_RUNNING;
//         }
//     }
//     return 0;
// }
// /**
//  * @brief running running running running
//  *
//  * @return stat_m
//  */
// stat_m m_static_instance_running_running_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     instance_obj *wobj = (instance_obj *)pdata;
//     uint64_t present_timesp = in_time_s;
//     enum base_state next_status = M_BASE_STATUS_RUNNING;
//     wobj->running_buff->rb_pre_stated = wobj->sio_status;

//     stat_m stat = fail_r;
//     // uint32_t present_time_hhmmss = /*_time.hhmmss*/ in_time_s & NUM_OF_DAY_SEC_SUM;
//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_running_to_run  ", in_time_s);

//     wobj->sio_status = M_BASE_STATUS_RUNNING;
//     wobj->running_buff->rb_pre_pc = wobj->running_buff->rb_pc;
//     // 记下时间
//     wobj->sio_channel[wobj->running_buff->rb_pre_pc].channel_running_time_region.tr_end_time = in_time_s;

//     // 根据一些参数 推断出下次运行区域
//     m_static_instance_tools_calculate_next_running_pc(wobj->running_buff->rb_pc, wobj->running_buff->maxChan, wobj->sio_channel, &wobj->running_buff->rb_pc);

//     // printf("--> %d > %d + %d\n" , wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time , wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time , wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time);
//     // 根据一些参数 推断出下次运行时间
//     // m_static_instance_tools_calculate_next_state_switching_time(wobj->sio_pump_or_sack,
//     //                                                             &wobj->running_buff->rb_WSTime_clone,
//     //                                                             wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time,
//     //                                                             wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time,
//     //                                                             wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time,
//     //                                                             /*_time.hhmmss*/ in_time_s % NUM_OF_DAY_SEC_SUM);
//     if (wobj->sio_pump_or_sack == M_M_FUNCATION_SACK)
//         wobj->running_buff->rb_WSTime_clone += ((wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time) > (wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time + wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time)
//                                                     ? wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time
//                                                     : (wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time - wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time));
//     else if (wobj->sio_pump_or_sack == M_M_FUNCATION_NONE)
//         wobj->running_buff->rb_WSTime_clone += wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time;

//     // DEBUG_TEST(DB_I,"cu_id %lld -- tr_start_time %lld -- cd_reality_run_time %d -> shouldRunTime %d rb_WSTime_clone %lld", wobj->sio_running_id, wobj->running_buff-> current_running_time_region.tr_start_time, wobj->sio_channel[wobj->running_buff-> rb_pc].cd_reality_run_time, wobj->sio_channel[wobj->running_buff-> rb_pc].shouldRunTime, wobj->running_buff-> rb_WSTime_clone);
//     // wobj->running_buff->rb_WSTime_clone %= NUM_OF_DAY_SEC_SUM;
//     // m_static_time_link_notece_node_change(wobj->sio_running_id, wobj->running_buff-> rb_WSTime_clone, m_next_state);
//     // 根据一些参数 推断出下次运行状态
//     m_static_instance_tools_calculate_next_running_status(wobj->running_buff->rb_pc, wobj->sio_pump_or_sack,
//                                                           wobj->running_buff->maxChan, wobj->sio_channel,
//                                                           wobj->running_buff->rb_WSTime_clone,
//                                                           &wobj->running_buff->rb_next_pc,
//                                                           &next_status, in_time_s);

//     // put_in(STAT_NEED_WIFI_BREAK, M_ERR); // 打断WiFi连接动画
//     {
//         m_static_time_link_notece_node_change(wobj->sio_running_id, wobj->running_buff->rb_WSTime_clone, next_status); // 消化状态 修改下一个节点的状态
//     }
//     wobj->sio_channel[wobj->running_buff->rb_pc].cd_curr_will_run_time = wobj->running_buff->rb_WSTime_clone - in_time_s;
//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);
//     // wobj->sio_channel[wobj->running_buff-> rb_pc].runtime.tr_start_time = present_timesp;
//     return stat;
// }
// stat_m m_static_instance_running_sacking_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = fail_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     enum base_state next_status = M_BASE_STATUS_RUNNING;
//     wobj->running_buff->rb_pre_stated = wobj->sio_status;
//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_sacking_to_run  ", in_time_s);

//     // 修改下狀態和上個區域指針
//     wobj->sio_status = M_BASE_STATUS_RUNNING;
//     wobj->running_buff->rb_pre_pc = wobj->running_buff->rb_pc;

//     // 根据一些参数 推断出下次运行区域
//     m_static_instance_tools_calculate_next_running_pc(wobj->running_buff->rb_pc, wobj->running_buff->maxChan, wobj->sio_channel, &wobj->running_buff->rb_pc);
//     DEBUG_E("%hhd ", wobj->sio_channel[wobj->running_buff->rb_pc].cd_cahnnel_id);
//     // 根据一些参数 推断出下次运行时间
//     wobj->running_buff->rb_WSTime_clone += ((wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time) > (wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time + wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time)
//                                                 ? wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time
//                                                 : (wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time - wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time));
//     // 根据一些参数 推断出下次运行狀態
//     m_static_instance_tools_calculate_next_running_status(wobj->running_buff->rb_pc, wobj->sio_pump_or_sack,
//                                                           wobj->running_buff->maxChan, wobj->sio_channel,
//                                                           wobj->running_buff->rb_WSTime_clone,
//                                                           &wobj->running_buff->rb_next_pc,
//                                                           &next_status, in_time_s);
//     m_static_time_link_notece_node_change(wobj->sio_running_id, wobj->running_buff->rb_WSTime_clone, next_status);
//     wobj->sio_channel[wobj->running_buff-> rb_pc].cd_curr_will_run_time = wobj->running_buff->rb_WSTime_clone - in_time_s;
//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);
//     return stat;
// }
// stat_m m_static_instance_running_hanguping_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s) { return 0; }
// stat_m m_static_instance_running_pauseing_to_recover(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = fail_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_end_time = in_time_s;
//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_pauseing_to_recover  ", in_time_s);
//     // 所有區域結束加上 暫停的時間
//     for (int i = 0; i < wobj->running_buff->maxChan; i++)
//     {
//         wobj->sio_channel[i].channel_running_time_region.tr_end_time += wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_end_time - wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_start_time;
//     }
//     // 根据一些参数 推断出下次运行时间
//     wobj->running_buff->rb_WSTime_clone += wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_end_time - wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_start_time;
//     // wobj->running_buff->rb_WSTime_clone %= NUM_OF_DAY_SEC_SUM;

//     // 恢復下狀態
//     wobj->sio_status = wobj->running_buff->rb_pre_stated;
//     wobj->running_buff->rb_pre_stated = M_BASE_STATUS_PAUSE;
//     // 恢復下節點
//     m_static_time_link_notece_node_change(wobj->sio_running_id, wobj->running_buff->rb_WSTime_clone, 0);
//     m_static_time_link_notece_node_enable(wobj->sio_running_id);
//     wobj->sio_channel[wobj->running_buff-> rb_pc].cd_curr_will_run_time = wobj->running_buff->rb_WSTime_clone - in_time_s;
//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);
//     return stat;
// }
// stat_m m_static_instance_running_pauseing_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s) { return 0; }

// stat_m m_static_instance_running_running_to_pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = fail_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     wobj->running_buff->rb_pre_stated = wobj->sio_status;
//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_running_to_pause  ", in_time_s);

//     wobj->sio_status = M_BASE_STATUS_PAUSE;
//     wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_start_time = in_time_s;

//     // m_static_time_link_notece_node_remove_by_id(wobj->sio_running_id);

//     m_static_time_link_notece_node_disable(wobj->sio_running_id);

//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);

//     return stat;
// }
// stat_m m_static_instance_running_sacking_to_pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = fail_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     wobj->running_buff->rb_pre_stated = wobj->sio_status;
//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_sacking_to_pause  ", in_time_s);

//     wobj->sio_status = M_BASE_STATUS_PAUSE;
//     wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_start_time = in_time_s;

//     // m_static_time_link_notece_node_remove_by_id(wobj->sio_running_id);

//     m_static_time_link_notece_node_disable(wobj->sio_running_id);

//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);
//     return stat;
// }

// stat_m m_static_instance_running_pauseing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s) { return 0; }
// stat_m m_static_instance_running_hanguping_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s) { return 0; }

// stat_m m_static_instance_running_running_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = succ_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     enum base_state next_status = M_BASE_STATUS_RUNNING;
//     wobj->running_buff->rb_pre_stated = wobj->sio_status;
//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_running_to_sack  ", in_time_s);

//     // 修改下狀態和上個區域指針 結算上個區域激素時間
//     wobj->sio_status = M_BASE_STATUS_SACK;
//     wobj->sio_channel[wobj->running_buff->rb_pc].channel_running_time_region.tr_end_time = in_time_s;
//     // wobj->running_buff->rb_pre_pc = wobj->running_buff->rb_pc;

//     // 根据一些参数 推断出下次运行区域
//     m_static_instance_tools_calculate_next_running_pc(wobj->running_buff->rb_pc, wobj->running_buff->maxChan, wobj->sio_channel, &wobj->running_buff->rb_next_pc);
//     // 根据一些参数 推断出下次运行时间
//     wobj->running_buff->rb_WSTime_clone +=
//         wobj->sio_channel[wobj->running_buff->rb_next_pc].cd_sack_time -
//         (in_time_s - wobj->sio_channel[wobj->running_buff->rb_next_pc].channel_running_time_region.tr_end_time);
//     // 根据一些参数 推断出下次运行狀態
//     m_static_instance_tools_calculate_next_running_status(wobj->running_buff->rb_pc, wobj->sio_pump_or_sack,
//                                                           wobj->running_buff->maxChan, wobj->sio_channel,
//                                                           wobj->running_buff->rb_WSTime_clone,
//                                                           &wobj->running_buff->rb_next_pc,
//                                                           &next_status, in_time_s);
//     m_static_time_link_notece_node_change(wobj->sio_running_id, wobj->running_buff->rb_WSTime_clone, next_status);
//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);
//     return stat;
// }
// stat_m m_static_instance_running_running_to_hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = fail_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     wobj->running_buff->rb_pre_stated = wobj->sio_status;
//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_running_to_hangup  ", in_time_s);

//     wobj->sio_status = M_BASE_STATUS_HANG_UP;
//     wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_start_time = in_time_s;

//     // m_static_time_link_notece_node_remove_by_id(wobj->sio_running_id);

//     m_static_time_link_notece_node_disable(wobj->sio_running_id);

//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);
//     return stat;
//     return 0;
// }
// stat_m m_static_instance_running_sacking_to_hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = fail_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     wobj->running_buff->rb_pre_stated = wobj->sio_status;
//     DEBUG_TEST(DB_W,"[%lld] m_static_instance_running_sacking_to_hangup  ", in_time_s);

//     wobj->sio_status = M_BASE_STATUS_HANG_UP;
//     wobj->sio_channel[wobj->running_buff->rb_pc].pause_recover_time.tr_start_time = in_time_s;

//     // m_static_time_link_notece_node_remove_by_id(wobj->sio_running_id);

//     m_static_time_link_notece_node_disable(wobj->sio_running_id);

//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);
//     return stat;
//     return 0;
// }

// /**
//  * @brief all to stop
//  *
//  * @return stat_m
//  */
// stat_m m_static_instance_running_all_to_idle(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = succ_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     if (wobj != NULL)
//     {
//         enum base_state pre_ste = wobj->sio_status;
//         wobj->sio_status = M_BASE_STATUS_IDLE;
//         // wobj->running_buff->rb_pre_pc = -1;
//         if (wobj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
//         {
//             m_static_time_link_notece_node_change(wobj->sio_running_id, wobj->WStime, M_BASE_STATUS_IDLE); // 消化状态 修改下一个节点的状态
//         }
//         // wobj->running_buff-> chan_already_RunTimeSum += in_time_s - wobj->sio_channel[wobj->running_buff-> rb_pc].runtime.tr_start_time; // m_callable_get_utc_time_t()
//         // // wobj->running_buff-> chan_already_RunTimeSum += in_time_s- wobj->running_buff-> chan_already_RunTimeSum; // m_callable_get_utc_time_t()
//         // wobj->sio_channel[wobj->running_buff-> rb_pc].cd_reality_run_time += in_time_s - wobj->sio_channel[wobj->running_buff-> rb_pc].runtime.tr_start_time;
//         // if (!wobj->in_hangup)
//             wobj->sio_channel[wobj->running_buff-> rb_pc].cd_curr_will_run_time = wobj->running_buff->rb_WSTime_clone - in_time_s;
//         m_ext_instance_running_event_handle(cause, pre_ste, wobj->sio_status, wobj, in_time_s);
//     }
//     return stat;
// }

// /**
//  * @brief 计划运行时
//  *
//  */
// // m_schedule_event_running_callback
// stat_m m_ext_instance_fsm__run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = succ_r;
//     uint64_t present_timesp = in_time_s;
//     uint32_t present_time_hhmmss = /*_time.hhmmss*/ in_time_s & NUM_OF_DAY_SEC_SUM;
//     /***
//      *  1、 idleing to run
//      *  2、 sacking to run
//      *  3、 pauseing to run
//      *  4、 running to run
//      *
//      *  A、 LEFT to run
//      *  B、 RIGHT to run
//      *
//      *  C、 pause to run
//      */
//     DEBUG_E("[%lld]m_ext_instance_fsm_(%d)_to_run  ", in_time_s, ste);

//     if (pdata != NULL)
//     {
//         // 1、 idle_to_runnig
//         if (ste == M_BASE_STATUS_IDLE)
//         {
//             stat = m_static_instance_running_idleing_to_run(cause, ste, pdata, in_time_s);
//         }
//         // 2、 sack_to_runnig
//         else if (ste == M_BASE_STATUS_PAUSE)
//         {
//             stat = m_static_instance_running_pauseing_to_recover(cause, ste, pdata, in_time_s);
//         }
//         // 3、 pause_to_runnig
//         else if (ste == M_BASE_STATUS_SACK)
//         {
//             stat = m_static_instance_running_sacking_to_run(cause, ste, pdata, in_time_s);
//         }
//         // 4、 running_to_runnig
//         else if (ste == M_BASE_STATUS_RUNNING)
//         {
//             if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_RIGHT)
//                 stat = m_static_instance_running_running_to_run_switch(cause, ste, pdata, in_time_s);
//             else if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_LEFT)
//                 stat = m_static_instance_running_running_to_run_switch(cause, ste, pdata, in_time_s);
//             else
//                 stat = m_static_instance_running_running_to_run(cause, ste, pdata, in_time_s);
//         }
//         else if (ste == M_BASE_STATUS_HANG_UP)
//         {
//             stat = m_static_instance_running_hanguping_to_run(cause, ste, pdata, in_time_s);
//         }
//     }

//     // DEBUG_TEST(DB_W,"m_action_schedule__running -- > %d", /*_time.hhmmss*/ in_time_s & NUM_OF_DAY_SEC_SUM);
//     // uint64_t plan_start_times = m_operation_schedule_calculate_near_start_timesp(wobj->sch_info.available_time.tr_start_time, wobj->WSTime, wobj->exfun.max_should_runtime);
//     return 0;
// }
// stat_m m_ext_instance_fsm__sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = succ_r;
//     /**
//      *  1、 running to sack
//      *  2、 pauseing to sack
//      *  3、hanguping to sack
//      *
//      *  4、 pause to run
//      */
//     DEBUG_E("[%lld]m_ext_instance_fsm_(%d)_to_sack  ", in_time_s, ste);
//     if (pdata != NULL)
//     {
//         if (ste == M_BASE_STATUS_RUNNING)
//         {
//             stat = m_static_instance_running_running_to_sack(cause, ste, pdata, in_time_s);
//         }
//         else if (ste == M_BASE_STATUS_PAUSE)
//         {
//             stat = m_static_instance_running_pauseing_to_recover(cause, ste, pdata, in_time_s);
//         }
//         else if (ste == M_BASE_STATUS_HANG_UP)
//         {
//             stat = m_static_instance_running_hanguping_to_sack(cause, ste, pdata, in_time_s);
//         }
//     }
//     return stat;
// }
// stat_m m_ext_instance_fsm__hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = succ_r;
//     /**
//      *  1、 sacking to hangup
//      *  2、 running to hangup
//      */
//     DEBUG_E("[%lld]m_ext_instance_fsm_(%d)_to_hangup  ", in_time_s, ste);

//     if (ste == M_BASE_STATUS_SACK)
//     {
//         stat = m_static_instance_running_running_to_hangup(cause, ste, pdata, in_time_s);
//     }
//     else if (ste == M_BASE_STATUS_RUNNING)
//     {
//         stat = m_static_instance_running_sacking_to_hangup(cause, ste, pdata, in_time_s);
//     }
//     return stat;
// }
// stat_m m_ext_instance_fsm__pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = succ_r;
//     /**
//      *  1、 sack to pause
//      *  2、 running to pause
//      */
//     DEBUG_E("[%lld]m_ext_instance_fsm_(%d)_to_pause  ", in_time_s, ste);

//     if (ste == M_BASE_STATUS_SACK)
//     {
//         stat = m_static_instance_running_sacking_to_pause(cause, ste, pdata, in_time_s);
//     }
//     else if (ste == M_BASE_STATUS_RUNNING)
//     {
//         stat = m_static_instance_running_running_to_pause(cause, ste, pdata, in_time_s);
//     }
//     return stat;
// }

// stat_m m_static_instance_running_running_to_run_switch(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     stat_m stat = succ_r;
//     instance_obj *wobj = (instance_obj *)pdata;
//     uint64_t present_timesp = in_time_s;
//     enum base_state next_status = M_BASE_STATUS_RUNNING;
//     wobj->running_buff->rb_pre_stated = wobj->sio_status;

//     DEBUG_E("[%lld]m_static_instance_running_running_to_run_switch  ", in_time_s, ste);

//     wobj->sio_status = M_BASE_STATUS_RUNNING;
//     wobj->running_buff->rb_pre_pc = wobj->running_buff->rb_pc;
//     // 记下时间
//     wobj->sio_channel[wobj->running_buff->rb_pre_pc].channel_running_time_region.tr_end_time = in_time_s;

//     if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_LEFT)
//     {
//         // 根据一些参数 推断出下次运行区域
//         m_static_instance_tools_calculate_next_running_pc(wobj->running_buff->rb_pc, wobj->running_buff->maxChan, wobj->sio_channel, &wobj->running_buff->rb_pc);
//     }
//     else if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_RIGHT)
//     {
//         // 根据一些参数 推断出下次运行区域
//         m_static_instance_tools_calculate_pre_running_pc(wobj->running_buff->rb_pc, wobj->running_buff->maxChan, wobj->sio_channel, &wobj->running_buff->rb_pc);
//     }

//     wobj->running_buff->rb_WSTime_clone = in_time_s;
//     // 根据一些参数 推断出下次运行时间
//     wobj->running_buff->rb_WSTime_clone += ((wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time) > (wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time + wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time)
//                                                 ? wobj->sio_channel[wobj->running_buff->rb_pc].cd_water_time
//                                                 : (wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time - wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time));
//     printf(":: %d\n", wobj->running_buff->rb_WSTime_clone);
//     // 根据一些参数 推断出下次运行状态
//     m_static_instance_tools_calculate_next_running_status(wobj->running_buff->rb_pc, wobj->sio_pump_or_sack,
//                                                           wobj->running_buff->maxChan, wobj->sio_channel,
//                                                           wobj->running_buff->rb_WSTime_clone,
//                                                           &wobj->running_buff->rb_next_pc,
//                                                           &next_status, in_time_s);

//     // put_in(STAT_NEED_WIFI_BREAK, M_ERR); // 打断WiFi连接动画
//     {
//         m_static_time_link_notece_node_change(wobj->sio_running_id, wobj->running_buff->rb_WSTime_clone, next_status); // 消化状态 修改下一个节点的状态
//     }
//     m_ext_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status , wobj, in_time_s);

//     return stat;
// }
// // m_watering_schedule *tp_obj = 50;
// stat_m m_ext_instance_fsm__idle(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
// {
//     // instance_obj *wobj = (instance_obj *)pdata;
//     DEBUG_TEST(DB_I,"m_schedule_event_stop_callback (%lld)", in_time_s);
//     m_static_instance_running_all_to_idle(cause, ste, pdata, in_time_s);

//     return 0;
// }

// /**
//     m_schedule_fsm_prepare = 0,
//     m_schedule_fsm_running,
//     m_schedule_fsm_sacking,
//     m_schedule_fsm_will_stop,
//     m_schedule_fsm_idle
//  */
// // m_schedule_event_prepare_callback
// // {
// //     instance_obj *wobj = (instance_obj *)pdata;
// //     DEBUG_TEST(DB_I,"m_action_schedule__prepare");
// //     if (wobj != NULL)
// //     {
// //         if (m_ext_eigenfunction_should_run(wobj, 0) == succ_r)
// //         {
// //             if (m_ext_eigenfunction_should_skip(wobj, 0) == succ_r)
// //             {
// //                 // m_ext_eigenfunction_prepare(wobj, m_callable_get_device_time_t());
// //                 create_send_data_to_server_22_06_13_plan_start(M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME,
// //                                                                wobj->sio_running_id,                                                          // 计划id
// //                                                                wobj->sio_channel[wobj->running_buff-> rb_pc].id,                         // 区域id
// //                                                                wobj->sio_channel[wobj->running_buff-> rb_pc].cd_reality_run_time,             // 已经运行时间
// //                                                                wobj->sio_channel[wobj->running_buff-> rb_pc].cruuent_round_running_time, // 当前轮回运行时间，就是刚刚
// //                                                                wobj->in_instance_id,                                                     // 计划结束时间
// //                                                                wobj->running_buff-> chan_already_RunTimeSum);
// //             }
// //         }
// //     }
// //     return 0;
// // }