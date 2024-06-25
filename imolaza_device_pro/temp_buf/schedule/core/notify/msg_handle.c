
// #include "core/schedule/option/option.h"

// #include "core/utils/util.h"
// #include "core/fset_lib.h"
// #include "core/notify/notify.h"
// // #include "core/net/mnet.h"
// #include "core/timer/mtime.h"
// #include "core/schedule/mschedule.h"
// // #include "core/fastrun/fastrun.h"
// #include "core/utils/text.h"

// extern struct fastRunManager _fastRunMan;

// static m_watering_schedule *gl_obj;

// void m_msg_init()
// {
// }

// m_watering_schedule wm_sch;
// char t_var_str_a[100] = {0}; /** cahr []*/
// char t_var_str_b[20] = {0};  /** cahr []*/
// uint64_t t_var_u64_a = 0;    /** 计划id*/
// uint64_t t_var_u64_b = 0;    /** 计划id*/
// uint64_t t_var_u64_c = 0;    /** 计划id*/
// int t_var_int_a = 0;         /** 计划id*/
// uint8_t cu_smart;            /** 智能计划使用参数*/
// char *cu_sack;

// stat_m m_server_message_handle(int cmd, char *msg)
// {
//     DEBUG_TEST(DB_W,"%d\n", cmd);
//     switch (cmd)
//     {

//     case 8: /** 8 传感器开关*/
//         m_insi_foramt_value("%d", &t_var_int_a, msg);

//         break;

//     case M_CMD_TWOWAY_SET_DEVICE_TIME: /** 时间*/
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg);
//         m_insi_foramt_value("%d", &t_var_int_a, msg);
//         m_callable_device_time_set(t_var_u64_a, t_var_int_a);
//         _device.state.onLine = true;
//         // if (_device.state.ReConnectServer)
//             create_send_data_to_server(18, U16, &_device.extreinfo.schedule_sum, null, NULL, null, NULL);
//         // _device.state.ReConnectServer = true;

//         break;
//     case M_CMD_TOWAY_DEVICE_CHANGEWIFI: /** 15*/
//         m_insi_foramt_str(t_var_str_a, msg);
//         m_insi_foramt_str(t_var_str_b, msg);
//         m_change_net_set_ssid(t_var_str_a, strlen(t_var_str_a));
//         m_change_net_set_psw(t_var_str_b, strlen(t_var_str_b));
//         m_change_net_start();

//         break;
//     case M_CMD_NOTIFY_TO_DEVICE_RESTART: /** 17*/
//         mReboot();
//         break;
//     case 18: /** 18 ota*/

//         DEBUG_TEST(DB_I,"core/ { %s }", msg);
//         memset(t_var_str_a, 0, sizeof(t_var_str_a));

//         m_insi_foramt_str(t_var_str_a, msg);
//         m_insi_foramt_str(t_var_str_b, msg);

//         if (strcmp(t_var_str_b, "null") == 0)
//         {
//             if (strlen(t_var_str_a) > 10 && strstr(t_var_str_a, ".bin") != NULL && strstr(t_var_str_a, "http") != NULL && strstr(t_var_str_a, ":") != NULL)
//             {

//                 m_callable_ota_start(t_var_str_a);

//                 break;
//             }
//             else
//                 DEBUG_E("inva addr {%s}", t_var_str_a);
//         }
//         else
//         {
//             // M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(t_var_str_b, "%lld", &y);
//         }
//         if (out_check(FRAST_START_UP) != OKK && !_device.state.ReConnectServer)
//         {
//             // create_send_data_to_server(ID_GET_DEVICE_TYPE, null, NULL, null, NULL, null, NULL);
//             m_splan_parse_all();
//             // nvs_plan_extract();
//         }
//         if (!_device.state.ReConnectServer)
//             _device.state.ReConnectServer = true;

//         break;

//     /**
//      * @brief A plan will be added to the device, and the added plan will be initialized again
//      * @param[in] schedule_id      scheming id
//      * @param[in] fun_smarty_cycle Smart plan, smart plan will dynamically change the area watering time, when the plan is smart,
//      *
//      * @param[in] fun_soak_str     Soak options :  1&2
//      *                             Soak the structure :  item.planinfo.planfun { fun_soak_use , fun_soak_str , fun_soak_water , fun_soak_cycle }
//      *                             Soak parameters  :  1&2 {  fun_soak_water & fun_soak_cycle  }
//      *
//      * @param[in] start_date_time  Date options :   2021-02-24 08:00:00
//      *                             Date the structure :  item.planinfo
//      *                             *Note :  The received time is converted to a timestamp and stored in memory or in a flash .
//      *
//      * @param[in] finish_date_time On the same  ( Validity period end time  )
//      *
//      * @param[in] water_start_time Time options :  AT 10:24 \ SUNSET \ ...
//      *                             Time the structure :  item.planinfo
//      *                             *Note :  More format parameters, specific at the socket instruction table
//      *
//      * @param[in] zones            zones options :  1#5;6#4
//      *                             zones the structure : item.planinfo
//      *                             zones parameters :  ChannId#WateringTime;..#..;..#..
//      *
//      * @param[in] desc_water_cycle_type  cyType options :   104 - 107
//      *                                   cyType the structure : item.planinfo
//      *                                   *Note : More format parameters, specific at the socket instruction table
//      *
//      * @param[in] water_cycle_walue      cyType value options :  1;2;3;4 or 1
//      *                                   cyType value the structure : item.planinfo
//      *                                   *Note : More format parameters, specific at the socket instruction table
//      *
//      *
//      * @example [20,   3111,0,null,1668028320,4101494399,103,1#60,104,1;2;3;4;5;6;7,1]
//      */
//     case M_CMD_TWOWAY_ADD_A_SCHEDULE: /**20 计划添加*/
//         memset(&wm_sch, 0, sizeof(m_watering_schedule));
//         m_insi_foramt_value("%lld", &wm_sch.sch_info.cu_id, msg);
//         m_insi_foramt_value("%hhd", &wm_sch.sch_info.cu_smart, msg);
//         m_insi_foramt_str(wm_sch.sch_info.cu_sack, msg);
//         m_insi_foramt_value("%lld", &wm_sch.sch_info.available_time.tr_start_time, msg);
//         m_insi_foramt_value("%lld", &wm_sch.sch_info.available_time.tr_end_time, msg);
//         m_insi_foramt_str(wm_sch.sch_info.cu_temp_param, msg);
//         m_insi_foramt_str(wm_sch.sch_info.cu_zone, msg);
//         m_insi_foramt_value("%hhd", &wm_sch.sch_info.cu_run_interval_type, msg);
//         m_insi_foramt_str(wm_sch.sch_info.cu_week_list, msg);
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg);
//         if (m_ext_schedule_add(&wm_sch, t_var_u64_a) == succ_r)
//             sch_print(&wm_sch);
//         else
//             DEBUG_E("add_fail...");

//         break;

//     /**
//      * @brief   Remove the device plan
//      * @param[in] flag              If for null it is all otherwise id 
//      */
//     case M_CMD_TWOWAY_DELE_SCHEDULE: // 22
//         m_insi_foramt_str(t_var_str_b, msg);
//         if (strcmp(t_var_str_b, TEMP_WORD_PUBLIC_NULL) == 0)
//             t_var_u64_a = None;
//         else
//             m_insi_foramt_value("%lld", &t_var_u64_a, t_var_str_b);
        
//         m_ext_schedule_remove(t_var_u64_a);
//         break;


//     /**
//      * @brief   Area disabled
//      * @param[in] flag              If for null it is all otherwise id 
//      */
//     case M_CMD_TWOWAY_SET_ZONE:
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // 区域
//         m_insi_foramt_value("%lld", &t_var_u64_b, msg); // zhaungtai
//         if (t_var_u64_b)  // enable
//             m_global_zone_enable(t_var_u64_a);
//         else
//             m_global_zone_disable(t_var_u64_a);
//         break;


//     /**
//      * @brief   schedule disabled  27
//      * @param[in] ID              If for null it is all otherwise id 
//      * @param[in] state              If for null it is all otherwise id 
//      */
//     case M_CMD_TWOWAY_SET_SCHEDULE_ABLE:
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // ID
//         m_insi_foramt_value("%lld", &t_var_u64_b, msg); // state
//         if (t_var_u64_b) // enable
//             m_callable_schedule_enable(t_var_u64_a);
//         else
//             m_callable_schedule_disable(t_var_u64_a);
//         break;


//     case M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME:
//         break;

//     /**
//      * @brief   Request smart plans and weather feedback
//      * @param[in] flag              If for null it is all otherwise id 
//      * @example [30,id,2|3|4|5|6|7,10]    or    [30,id,null,0]
//      */
//     case M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME: /** 30*/

//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // ID
//         m_insi_foramt_str(t_var_str_b, msg);
//         m_insi_foramt_value("%lld", &t_var_u64_b, msg); // ID

//         m_callable_smartschedule_config(t_var_str_b , t_var_u64_b);

//         break;

//     /**
//      * @brief   Time of the new lawn mode
//      * @param[in] ID              If for null it is all otherwise id 
//      * @param[in] time              The maximum length is (32 * 2)
//      * @example [30,id,2|3|4|5|6|7,10]    or    [35,id,A0B0]
//      */
//     case M_CMD_TWOWAY_NEW_LAWN_TIME_INFO: /** 35*/

//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // ID
//         m_insi_foramt_str(t_var_str_a, msg);

//         if(m_callable_schedule_list_find(t_var_u64_a , &gl_obj) == succ_r && gl_obj != NULL)
//         {
                
//             m_newlwan_mode_schedule_update_time_list(t_var_u64_a , t_var_str_a ,true);  
            
//             m_newlwan_mode_schedule_flush(t_var_u64_a , _time.hhmmss ,&t_var_u64_b , false);
//             gl_obj->WSTime = gl_obj->running.rb_WSTime_clone = t_var_u64_b;  
//             m_callable_add_notice_schedule_run_node(t_var_u64_a ,gl_obj->WSTime ,  m_schedule__idle , gl_obj );
//         }
//         else 
//             DEBUG_E("Fail ..");


//         break;
//     /**
//      * @brief   This mode is normal except for scheduled operation
//      * @param[in] flag              If for null it is all otherwise id 
//      * @example [37,i676525000,16943555510,10]    
//      */
//     case M_CMD_TWOWAY_HIBERNATION_MODE: /** 37*/

//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // start time
//         m_insi_foramt_value("%lld", &t_var_u64_b, msg); // end time
//         if(t_var_u64_b > t_var_u64_a)
//         {
//             // _device.extreinfo.debug
//             printf("<%lld - %lld>",  t_var_u64_a , t_var_u64_b);
//         }
//         // m_callable_smartschedule_config(t_var_str_b , t_var_u64_b);

//         break;
//     case M_CMD_TWOWAY_PUMP_MODE: /** 38*/

//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // mode

//         break;


//     case M_CMD_TWOWAY_FASTRUN_START /**40 快速运行添加*/:
//         m_ext_fast_run_create(msg, strlen(msg));
//         break;
//     case M_CMD_TWOWAY_FASTRUN_SWITCH_PRE /** 快速运行上一个*/:
//         _fastRunMan.fastRunPreChan();
//         break;
//     case M_CMD_TWOWAY_FASTRUN_SWITCH_NEXT /** 快速运行下一个*/:
//         _fastRunMan.fastRunNextChan();
//         break;
//     case M_CMD_TWOWAY_FASTRUN_PAUSE: /** 快速运行暂停 */
//         _fastRunMan.fastRunPause();
//         break;
//     case M_CMD_TWOWAY_FASTRUN_RECOVER: /** 快速运行恢复 */
//         _fastRunMan.fastRunRecover();
//         break;
//     case M_CMD_TWOWAY_GLOBAL_RUN_STOP: /** 全局停止运行 */

//         break;

//         /***
//          * 计划跳过有很多 跳过单个计划（70）、跳过计划时间范围（62）、还有休眠时间段  （...）
//          *
//          */
//     case M_CMD_TWOWAY_RESET_DEVICE: // 52
//         break;
//     case M_CMD_TWOWAY_DEVICE_LIGHT_CHANGE: /** 调节设备灯光亮度 56 */
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg);
//         if (t_var_u64_a)

//             break;
//     case M_CMD_TWOWAY_CONTROL_DEVICE_KEY:               /** 远程控制设备按键 58 */
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // id
//         break;
//         /**
//          * @brief The server announces that a plan should not be execute
//          *
//          * @param[in] y  start time
//          * @param[in] z  end time
//          * @param[in] code  code
//          */
//     case M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC: /** 跳过一个时间区域的计划 */ // 62 The server announces that a plan should not be executed
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // id
//         m_insi_foramt_value("%lld", &t_var_u64_b, msg); // start
//         if(t_var_u64_b > t_var_int_a && t_var_u64_b > 1000 && t_var_int_a > 1000)
//         {
            
//         }

//         break;
//     case M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC: /** 跳过一个计划 */ // 70
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg);               // id
//         m_insi_foramt_value("%lld", &t_var_u64_b, msg);               // start
//         m_insi_foramt_value("%lld", &t_var_u64_c, msg);               // end
//         // m_global_schedule_skip_alone_set(t_var_u64_a, t_var_u64_b, t_var_u64_c);
//         break;
//     case M_CMD_NOTIFY_TO_SERVER_A_SCHEDULE_BE_SKIP: /** 计划跳过通知 */ // 74
//         break;
//     case M_CMD_TWOWAY_UNDO_NOT_RUN_A_SCHEDULE_TIMESPC: /** 取消跳过一个计划 */ // 75
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg);                        // id
//         break;
//     case M_CMD_TWOWAY_UNDO_NOT_RUN_SCHEDULE_TIMESPC: /** 取消跳过一段时间 */
//         break;

//     default:
//         m_insi_foramt_value("%lld", &t_var_u64_a, msg); // id
//         if_allow_dele(t_var_u64_a, 0);

//         break;
//     }
//     return 0;
// }

// stat_m m_msg_handle(char *in_msg)
// {
//     DEBUG_TEST(DB_I,"handle (%d){%s}", 0, in_msg);
//     return 0;
// }
