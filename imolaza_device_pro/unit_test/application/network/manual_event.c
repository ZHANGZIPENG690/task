// #include "core/fset_lib.h"
// // #include "../application/include.h"

// /**
//  * @brief 事件处理
//  *
//  * @param event_id
//  * @param pre_channel
//  * @param next_channel
//  * @param has_benn_runnint_time
//  * @param current_time
//  * @return stat_m
//  */
// stat_m m_ext_manual_event_handle(enum operate_event event_id, uint8_t pre_channel, uint8_t next_channel, uint32_t has_benn_runnint_time, uint64_t current_time)
// {
//     stat_m stat = fail_r;
//     int resp = 0;
//     int resp_man = 254;
//     DEBUG_TEST(DB_W,"m_ext_manual_event_handle (%d) .. ", event_id);

//     switch (event_id)
//     {

//     case M_OPERATE_EVENT_START:
//         DEBUG_TEST(DB_W,"开始运行 .. %d", next_channel);
        
//         resp = M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_START;
//         m_callable_solenoid_manage_open(next_channel);
//         m_callable_local_resp_to_remote(resp,
//                                         M_TYPE_Int, (void *)&resp_man,
//                                         M_TYPE_U8, (void *)&next_channel,
//                                         M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, current_time);

//         break;
//     case M_OPERATE_EVENT_START_MANY:
//         DEBUG_TEST(DB_W,"M_OPERATE_EVENT_START_MANY .. ");

//         break;
//     case M_OPERATE_EVENT_START_SELECT:

//         // m_callable_solenoid_manage_open(next_channel);

//         break;
//     case M_OPERATE_EVENT_RUNNING_SWITCH:

//         DEBUG_TEST(DB_W,"M_OPERATE_EVENT_RUNNING_SWITCH .. ");
//         m_callable_solenoid_manage_continuously(pre_channel, next_channel);
//         DEBUG_TEST(DB_W,"手动运行切换 ..  上一个: %hhd 时间: %d 下一个: %hhd ", pre_channel, has_benn_runnint_time, next_channel);
//         resp = M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_SWITCH;
//         resp_man = 0;
//         m_callable_local_resp_to_remote(resp,
//                                         M_TYPE_U8, (void *)&next_channel,
//                                         M_TYPE_Int, (void *)&resp_man,
//                                         M_TYPE_U8, (void *)&pre_channel, M_TYPE_U32,
//                                         (void *)&has_benn_runnint_time, current_time);
//         break;
//     case M_OPERATE_EVENT_CHANNEL_SWITCH:

//         DEBUG_TEST(DB_W,"M_OPERATE_EVENT_CHANNEL_SWITCH .. ");
//         // m_callable_solenoid_manage_continuously(pre_channel, next_channel);
//         break;
//     case M_OPERATE_EVENT_PAUSE:

//         DEBUG_TEST(DB_W,"M_OPERATE_EVENT_PAUSE .. ");
//         // break;
//     case M_OPERATE_EVENT_STOP:

//         // m_callable_solenoid_manage_close(next_channel);
//         DEBUG_TEST(DB_W,"M_OPERATE_EVENT_STOP .. ");
//         resp = M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_STOP;

//         m_callable_solenoid_manage_close(next_channel);
//         m_callable_local_resp_to_remote(resp, M_TYPE_Int, (void *)&resp_man,
//                                         M_TYPE_U8, (void *)&next_channel,
//                                         M_TYPE_U32, (void *)&has_benn_runnint_time,
//                                         M_TYPE_NULL, NULL, current_time);
//         break;
//     case M_OPERATE_EVENT_STOP_SELECT:

//         DEBUG_TEST(DB_W,"M_OPERATE_EVENT_STOP_SELECT .. ");
//         break;
//     case M_OPERATE_EVENT_STOP_ALL:

//         DEBUG_TEST(DB_W,"M_OPERATE_EVENT_STOP_ALL .. ");
//         break;

//     default:
//         break;
//     }

//     return stat;
// }
