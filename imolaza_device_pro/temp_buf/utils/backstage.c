// #include "fun.h"
// #include "tool.h"
// #include "text.h"
// #include "hot_so_fast.h"

// extern time_manage _time;
// extern plan_manager _plan;
// extern json_manager _json;
// extern struct _viuis_key viuis_key;
// extern struct light_state_control_bit light_scb;
// extern struct fastRunManager _fastRunMan;
// extern struct m_io_contr io_contr;

// extern void showOneChannel(uint8_t);
// extern uint64_t plan_calculate_the_remaining_runtime(water_task_date_m *);

// #define IS_PLAN_SACK 2

// static uint64_t temp_id = 0;

// // 后台列表
// struct backstrage_list
// {
//     water_task_date_m *wtd_obj; // 后台对象
//     uint64_t wtd_timsp;         // 加入后台的时间戳
//     uint8_t trigger_type;
//     uint8_t trigger_chann;
// };

// extern uint64_t compute_plan_last_start_timestamp(water_task_date_m *,uint64_t, uint32_t, uint32_t);

// struct backstrage_list bk_obj = {NULL, 0, 0, 0};

// void plan_backstage_reset(void)
// {
//     {
//         bk_obj.wtd_obj = NULL;
//         bk_obj.wtd_timsp = 0;
//         bk_obj.trigger_chann = 0;
//         bk_obj.trigger_type = 0;
//     }
// }

// void plan_backstage_become_silent_add(water_task_date_m *wtd, uint8_t s_type, uint8_t s_chann)
// {
//     bk_obj.wtd_obj = wtd;
//     bk_obj.wtd_timsp = _time.time_stamp;
//     _plan.is_backstage = true;
//     bk_obj.trigger_type = s_type;
//     bk_obj.trigger_chann = s_chann;
// }
// water_task_date_m *plan_backstage_become_silent_dele(void)
// {
//     bk_obj.wtd_timsp = 0;
//     if (!io_contr.s2_trigger && !io_contr.s1_trigger)
//         _plan.is_backstage = false;
//     bk_obj.trigger_type = 0;
//     bk_obj.trigger_chann = 0;
//     return bk_obj.wtd_obj;
// }

// //计划加入后台
// void plan_backstage_add(water_task_date_m *wtd, uint8_t s_type, uint8_t s_chann, int in_cmd)
// {
//     DEBUG_TEST(DB_I,"plan_backstage_add");
//     if (wtd != NULL)
//     {
//         // 已经有一个后台还是 新的后台 不需要 直接换
//         bk_obj.wtd_obj = wtd;
//         bk_obj.wtd_timsp = _time.time_stamp;
//         bk_obj.trigger_type = s_type;
//         bk_obj.trigger_chann = s_chann;
//         DEBUG_E("%d", s_chann);
//         _plan.is_backstage = true;
//         // wtd->is_backstage = true; //后台标志 ，当所有的区域跳动的时候都会判断该标识位
//         {
//             //还有一个操作是当现在是由计划在运行的时候，我们可能再做一些操作
//             // 1、停止计划(bu需要停止计划)，发送通知即可
//             if (_plan.plan_runnning_obj != NULL)
//             {
//                 uint8_t channel = _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].chanID;
//                 if (_plan.plan_runnning_obj->running_state == PLAN_SACK)
//                     channel = None;
//                 TrunOnDevice(false);
//                 _plan.plan_channel(false, _plan.plan_runnning_obj->schedule_id, _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].chanID,
//                                    _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].realityRunTime);
//                 put_in(STAT_KEY_RUN, ERR);
//                 put_in(STAT_KEY, OKK);
//                 light_scb.spec_auto_idle = _time.new_get_hhmmss();

//                 uint64_t plan_start_times = compute_plan_last_start_timestamp(_plan.plan_runnning_obj ,_plan.plan_runnning_obj->planinfo.start_date_time, _plan.plan_runnning_obj->WSTime, _plan.plan_runnning_obj->chanRunTimeSum);

//                 create_send_data_to_server_22_07_29(in_cmd,
//                                                     _plan.plan_runnning_obj->schedule_id,
//                                                     channel,
//                                                     None,
//                                                     _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].thisOperationTime, //当前批次区域已经运行时间
//                                                     (plan_start_times) + _plan.plan_runnning_obj->chanRunTimeSum,                     //计划结束时间戳
//                                                     2,                                                                                //状态
//                                                     bk_obj.trigger_chann,                                                             // 传感器通道
//                                                     _plan.plan_runnning_obj->chan_already_RunTimeSum                                  // 已经运行总时间
//                 );

//                 DEBUG_E("%d", bk_obj.trigger_chann);
//                 rescue_message_value(PLAN_SENSOR_SKIP_FLAG, &_plan.plan_runnning_obj->schedule_id, (plan_start_times) + _plan.plan_runnning_obj->chanRunTimeSum, M_RESCUE_MESSAGE_SET);

//                 _plan.plan_runnning_obj->chan_already_RunTimeSum = 0;
//             }
//             // }
//         }
//     }
//     else
//         return;
// }
// uint64_t plan_backstage_get_id(void)
// {
//     if (bk_obj.wtd_obj != NULL)
//         return bk_obj.wtd_obj->schedule_id;
//     else
//         return 0;
// }

// void plan_backstage_dele(int in_cmd)
// {
//     DEBUG_TEST(DB_W,"plan_backstage_dele");
//     // 当前的计划是？
//     _plan.is_backstage = false;
//     if (_plan.plan_runnning_obj != NULL && bk_obj.wtd_obj != NULL)
//     {
//         DEBUG_E("%d", bk_obj.trigger_chann);

//         uint64_t plan_start_times = compute_plan_last_start_timestamp(_plan.plan_runnning_obj , _plan.plan_runnning_obj->planinfo.start_date_time, _plan.plan_runnning_obj->WSTime, _plan.plan_runnning_obj->chanRunTimeSum);
//         if (_plan.plan_runnning_obj->running_state == PLAN_RUNNING || _plan.plan_runnning_obj->running_state == PLAN_SACK)
//         {
//             if (_fastRunMan.FastRunroot->isRun || _fastRunMan.FastRunroot->isPause)
//                 _fastRunMan.fastRunStop();
//             else if (viuis_key.isruning)
//             {
//                 DEBUG_TEST(DB_I,"plan _key...");
//                 _plan.key_channel(false, None, viuis_key.channel, viuis_key.runtimeed, true);
//             }
//         }
//         // if (/*当前是不是计划结束时间之前,,计划应不应该运行*/) // 应该
//         {
//             _plan.plan_runnning_obj = bk_obj.wtd_obj;
//             // wtd->is_backstage = true;
//             if (_plan.plan_runnning_obj->running_state == PLAN_RUNNING)
//             {
//                 DEBUG_TEST(DB_I,"PLAN_RUNNING");
//                 TrunOnDevice(true);

//                 create_send_data_to_server_22_07_29(in_cmd,
//                                                     _plan.plan_runnning_obj->schedule_id,
//                                                     _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].chanID,
//                                                     _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].shouldRunTime,       // 区域总运行时间
//                                                     ((_plan.plan_runnning_obj->WSTime_clone + 86400) - _time.new_get_hhmmss()) % 86400, // 区域将要运行时间
//                                                     (plan_start_times) + _plan.plan_runnning_obj->chanRunTimeSum,                       // 计划结束时间戳
//                                                     PLAN_RUNNING,                                                                       // 状态
//                                                     bk_obj.trigger_chann,                                                               // 触发区域
//                                                     plan_calculate_the_remaining_runtime(_plan.plan_runnning_obj)                       // 剩余运行总时间
//                 );

//                 _plan.plan_channel(true, _plan.plan_runnning_obj->schedule_id, _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].chanID,
//                                    _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].realityRunTime);
//                 put_in(STAT_KEY_RUN, OKK), put_in(STAT_KEY, ERR);
//                 put_in(STAT_IDLE, ERR);
//             }
//             else if (_plan.plan_runnning_obj->running_state == PLAN_SACK)
//             {
//                 DEBUG_TEST(DB_I,"PLAN_SACK");
//                 TrunOnDevice(false);
//                 create_send_data_to_server_22_07_29(in_cmd,
//                                                     _plan.plan_runnning_obj->schedule_id,
//                                                     None,
//                                                     _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].shouldRunTime,
//                                                     None,
//                                                     (plan_start_times) + _plan.plan_runnning_obj->chanRunTimeSum,
//                                                     IS_PLAN_SACK, //(2) 浸泡
//                                                     bk_obj.trigger_chann,
//                                                     plan_calculate_the_remaining_runtime(_plan.plan_runnning_obj));

//                 showOneChannel(_plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].chanID);
//                 put_in(STAT_KEY, OKK);
//                 put_in(STAT_KEY_RUN, ERR);
//                 put_in(STAT_IDLE, ERR);
//             }
//             // _plan.plan_runnning_obj
//             else
//                 DEBUG_E("backstage running_state unknow");
//             // else
//             {
//                 //    plan_backstage_reset();
//             }
//         }
//         _plan.plan_runnning_obj->chan_already_RunTimeSum = 0;
//     }
//     else
//     {
//         DEBUG_TEST(DB_I,"backstage reset");
//         plan_backstage_reset();
//     }
// }
