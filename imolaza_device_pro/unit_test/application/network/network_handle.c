

// #include "core/fset_lib.h"

// // #include "../drive/drive.h"

// void m_user_network_config_login_cmd(char *out_cmd, int count)
// {
//     // char out_resp[120] = {0};
//     char out_temp_char[20] = {0};
//     int temp_int_a = 0;
//     memset(out_cmd, 0, strlen(out_cmd));
//     enum running_mode out_rmode;
//     m_callable_device_attribute_get_running_mode(&out_rmode);
//     m_callable_device_attribute_get_key(out_temp_char);
//     // 还是第一次登陆
//     strcat(out_cmd, "[");
//     if ((count == 1 || count == -1) && out_rmode == M_DEVICE_RUNNING_MODE_CONFIG)
//     {
//         if (count == -1)
//             strcat(out_cmd, "15,");
//         else
//             strcat(out_cmd, "10,");
//         strcat(out_cmd, out_temp_char);
//         strcat(out_cmd, ",");
//         bzero(out_temp_char, 20);
//         m_callable_device_attribute_get_current_connect_net_name(out_temp_char);
//         strcat(out_cmd, out_temp_char);
//         strcat(out_cmd, ",");
//         bzero(out_temp_char, 20);
//         m_callable_device_attribute_get_app_version(out_temp_char);
//         strcat(out_cmd, out_temp_char);
//         strcat(out_cmd, ",");
//         bzero(out_temp_char, 20);
//         m_callable_device_attribute_get_schedule_num(&temp_int_a);
//         sprintf(out_temp_char, "%d", temp_int_a);
//         strcat(out_cmd, out_temp_char);
//         strcat(out_cmd, "]");
//         // strcpy(out_cmd, "[10,230205A001,HCTJ12_m,123567456,1.6.001]");
//     }
//     // 不是第一次登陆
//     else if (count > 1)
//     {
//         strcpy(out_cmd, "[67,1.6.001]");
//     }
//     else
//     {
//         strcat(out_cmd, "11,");
//         strcat(out_cmd, out_temp_char);
//         bzero(out_temp_char, 20);
//         strcat(out_cmd, ",");
//         m_callable_device_attribute_get_app_version(out_temp_char);
//         strcat(out_cmd, out_temp_char);
//         strcat(out_cmd, ",");
//         bzero(out_temp_char, 20);
//         m_callable_device_attribute_get_schedule_num(&temp_int_a);
//         sprintf(out_temp_char, "%d", temp_int_a);
//         strcat(out_cmd, out_temp_char);
//         strcat(out_cmd, "]");
//     }
// }

// /**
//  * @brief 注意这个个函数，返回如果是成功，会被内部判定为认证成功，没有认证成功 会不断重复 在一段时间之内
//  *
//  * @return stat_m
//  */
// stat_m m_ext_network_manage_login(enum connect_mode sm_mode, int count)
// {
//     char arr[50] = {0};
//     switch (sm_mode)
//     {
//     case M_NETWORK_MANAGE_CONNECT_MODE_MQTT:
//         DEBUG_TEST( DB_I,"MQTT 登陆");

//         break;
//     case M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL:
//         DEBUG_TEST( DB_I,"蓝牙 登陆");

//         break;
//     case M_NETWORK_MANAGE_CONNECT_MODE_TCP:
//         DEBUG_TEST( DB_I,"TCP 登陆");
//         // 这里需要区分 配网登陆 还是 改wifi   还是第一登陆
//         m_user_network_config_login_cmd(arr, count);

//         m_callable_network_data_tx(false, arr, strlen(arr));

//         break;
//     default:
//         break;
//     }
//     return succ_r;
// }

// /**
//  * @brief 初始化网络方式   wifi还是蓝牙
//  *
//  * @param mode
//  * @return stat_m
//  */
// stat_m m_ext_drive_net_manage_init(enum running_mode nm, enum connect_mode sm)
// {
//     /** 设备运行方式 */
//     switch (nm)
//     {
//     case M_DEVICE_RUNNING_MODE_CONFIG:
//         DEBUG_TEST( DB_I," M_DEVICE_RUNNING_MODE_CONFIG ");
//         // 如果是配网模式 直接初始化 蓝牙 和 wifi
//         m_ext_soft_drive_net_wifi_init();
//         // 如果是配网模式 直接初始化 蓝牙 和 wifi
//         m_ext_soft_drive_net_bluetool_init();
//         mDelay_ms(1000);
//         m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS1_MODE, 0);
//         m_callable_device_proper_status_update_to_idle();

//         break;
//     case M_DEVICE_RUNNING_MODE_DERECT:
//         DEBUG_TEST( DB_I," M_DEVICE_RUNNING_MODE_DERECT ");
//         m_ext_soft_drive_net_wifi_init();
//         mDelay_ms(1000);
//         m_callable_network_connect();
//         m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 2);
//         m_callable_device_proper_status_update_to_idle();

//         break;
//     case M_DEVICE_RUNNING_MODE_OFFLINE:
//         DEBUG_TEST( DB_I," M_DEVICE_RUNNING_MODE_OFFLINE ");
//         m_ext_soft_drive_net_bluetool_init();
//         mDelay_ms(1000);
//         m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
//         break;
//     default:
//         DEBUG_TEST( DB_I," None ");
//         break;
//     }

//     /** 服务器连接方式 */
//     switch (sm)
//     {
//     case M_NETWORK_MANAGE_CONNECT_MODE_MQTT:
//         DEBUG_TEST( DB_I," M_NETWORK_MANAGE_CONNECT_MODE_MQTT ");
//         m_ext_software_drive_server_mqtt_init();
//         break;
//     case M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL:
//         DEBUG_TEST( DB_I," M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL ");
//         break;
//     case M_NETWORK_MANAGE_CONNECT_MODE_TCP:
//         DEBUG_TEST( DB_I," M_NETWORK_MANAGE_CONNECT_MODE_TCP ");
//         m_ext_software_drive_server_tcp_init();
//         break;

//     default:
//         DEBUG_TEST( DB_I," None ");
//         break;
//     }

//     return 0;
// }

// /**
//  * @brief 网络连接
//  *
//  * @param username  网络用户名
//  * @param token 认证密钥
//  * @return stat_m
//  */
// // stat_m m_ext_network_server_connect_to(char *username, int port)
// // {
// //     DEBUG_TEST( DB_I," m_ext_network_server_connect_to [ %s , %d ] ", username, port);
// //     return 0;
// // }
// /**
//  * @brief 网络连接
//  *
//  * @param username  网络用户名
//  * @param token 认证密钥
//  * @return stat_m
//  */
// stat_m m_ext_network_net_connect_to(char *username, char *token)
// {
//     DEBUG_TEST( DB_I," m_ext_network_net_connect_to [ %s , %s ] ", username, token);

//     return m_ext_software_drive_net_wifi_sta_connect_to(username, token);
// }

// /** 临时使用变量*/
// int temp_use_int_vue_a = 0;
// int temp_use_int_vue_b = 0;
// uint64_t temp_use_u64_vue_a = 0;
// uint64_t temp_use_u64_vue_b = 0;
// uint64_t temp_use_u64_vue_c = 0;
// /** ** */
// char tmp_sid[30];

// /**
//  * @brief 收到的消息处理
//  *
//  * @param cmd 消息CMD
//  * @param buf 消息内容
//  * @return stat_m
//  */
// stat_m m_ext_server_message_handle(int cmd, char *buf)
// {
//     stat_m stat = fail_r;
//     DEBUG_TEST( DB_I,"消息处理 %d   %s", cmd, buf);
//     int WSTime = 0;
//     enum m_paramete_enum mpe = M_TYPE_NULL;

//     switch (cmd)
//     {
//         //     /* 传感器加入 */
//         // case M_CMD_TWOWAY_SENSOR_TRIGE: /* 6 */

//         //     break;
//         //     /* 传感器离开 */
//         // case M_CMD_TWOWAY_SENSOR_LEAVE: /* 7 */

//         //     break;
//         /**
//          * @brief 设置传感器 8
//          *
//          * @name [ 8 , a ]
//          * @param  a 传感器参数格式
//          */
//     case M_CMD_TWOWAY_SET_SENSOR:

//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
//         // mDelay_ms(200);
//         m_callable_sensor_set(temp_use_int_vue_a , true);

//         break;
//         /** 查询设备状态*/
//     case M_CMD_TWOWAY_GET_DEVICE_STATUS: /* 9 */

//         break;
//     //     /** 设备配网上线*/
//     // case M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIG: /* 10 */
//     //     break;
//     //     /** 设备断电上线*/
//     // case M_CMD_NOTIFY_TO_SERVER_DEVICE_ONLINE: /* 11 */
//     //     break;
//     // 设置时间和向服务器获取时间
//     case M_CMD_TWOWAY_SET_DEVICE_TIME: /* 12 */
//         /**
//          * @brief 时间-登陆标记
//          *
//          * @name [ 12 , time , time_zone ]
//          * @param  time 时间
//          * @param  time_zone 时区
//          */
//         // m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%lld,%d", &temp_use_u64_vue_a, &temp_use_int_vue_a);
//         // m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
//         // m_callable_timer_manage_time_set(1668114920, -6);
//         m_callable_timer_manage_time_set(temp_use_u64_vue_a, temp_use_int_vue_a);
//         m_callable_network_set_server_login_status(true);

//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_GET_OR_DEMAND_DEVICE_UPDTAE, M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         break;
//         // 在线改 WiFi
//     case M_CMD_TOWAY_DEVICE_CHANGEWIFI: /* 15 */
//         // [15 ,  ssid   psw]
//         m_callable_data_parse_foramt_str(tmp_sid, buf);
//         m_callable_config_net_set_username(tmp_sid);
//         m_callable_data_parse_foramt_str(tmp_sid, buf);
//         m_callable_config_net_set_token(tmp_sid);
//         stat = m_callable_config_net_start();
//         m_callable_network_set_status_change_net();
//         break;
//     //     // 同步计划
//     // case M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE: /* 16 */
//     //     break;
//     // 远程重启设备
//     case M_CMD_NOTIFY_TO_DEVICE_RESTART: /* 17 */
//         mReboot();
//         break;
//         // 请求更新设备或者要求新设备
//     case M_CMD_TWOWAY_GET_OR_DEMAND_DEVICE_UPDTAE: /* 18*/

//         m_callable_device_attribute_get_schedule_num(&temp_use_int_vue_a);
//         m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE, M_TYPE_Int, &temp_use_int_vue_a,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         m_callable_schedule_stroage_recover_item_all();
//         break;
//         // 设备向服务器通知更新开始
//     case M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIRM_UPDATA: /* 19 */

//         break;
//         /* PLAN */
//         /** 添加一个计划到设备*/
//     case M_CMD_TWOWAY_ADD_A_SCHEDULE: /* 20 */
//         temp_use_u64_vue_a = 0;
//         stat = m_callable_instance_manage_add(buf, M_INSTANCE_ADD_MODE_ONLINE, m_callable_timer_manage_get_device_time(), &temp_use_u64_vue_a);
//         if (stat == succ_r)
//             mpe = M_TYPE_Str_OK;
//         else
//             mpe = M_TYPE_Str_FAIL;
//         m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_ADD_SCH_RESP, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
//                                         mpe, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         break;
//         //     /** 反馈计划添加成功与否*/
//         // case M_CMD_NOTIFY_TO_SERVER_ADD_SCH_RESP: /* 21 */
//         //     break;
//         /** 删除计划*/
//     case M_CMD_TWOWAY_DELE_SCHEDULE: /* 22 */
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         stat = m_callable_instance_manage_remove(temp_use_u64_vue_a, 0);
//         if (stat == succ_r)
//             mpe = M_TYPE_Str_OK;
//         else
//             mpe = M_TYPE_Str_FAIL;
//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_DELE_SCHEDULE_RESP, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
//                                         mpe, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         break;
//         //     /** 反馈计划删除成功与否*/
//         // case M_CMD_TWOWAY_DELE_SCHEDULE_RESP: /* 23 */
//         //     break;
//         //     /** 通知计划开始*/
//         // case M_CMD_NOTIFY_TO_SERVER_SCHEDULE_START: /* 24 */
//         //     break;
//         //     /** 通知计划开始切换区域运行*/
//         // case M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH: /* 64 */
//         //     break;
//         //     /** 通知计划结束*/
//         // case M_CMD_NOTIFY_TO_SERVER_SCHEDULE_END: /* 25 */

//         //     break;
//         /** 区域设置 */
//     case M_CMD_TWOWAY_SET_ZONE: /* 26 */
//         /**
//          * @brief 区域设置 开关 [26,15,1,1697109978,1697109978]   --> [26,15,11697109978,1697109978,1697109978]
//          *
//          * @name [ 26 , zone , enable , start , end ]
//          * @param  zone 区域 id
//          * @param  enable 启用禁用 enable
//          * @param  start 如果是关闭的话 这个是时间
//          * @param  end 如果是关闭的话 这个是时间
//          */
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
//         stat = m_callable_channel_manage_state_set(temp_use_int_vue_b, temp_use_int_vue_a, temp_use_u64_vue_a, temp_use_u64_vue_b);

//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_SET_ZONE, M_TYPE_Int, (void *)&temp_use_int_vue_a,
//                                         M_TYPE_Int, (void *)&temp_use_int_vue_b,
//                                         M_TYPE_U64, (void *)&temp_use_u64_vue_a,
//                                         M_TYPE_U64, (void *)&temp_use_u64_vue_b, m_callable_timer_manage_get_device_time());

//         break;
//         /** 计划启用和禁用 */
//     case M_CMD_TWOWAY_SET_SCHEDULE_ABLE: /* 27*/
//         /**
//          * @brief 计划启用禁用    [27, 3885 , 1 , 1655321995]
//          *
//          * @name [ 27 , id , state ]
//          * @param  id 计划 id
//          * @param  state 启用禁用 enable
//          */
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
//         if (temp_use_int_vue_a)
//             stat = m_callable_instance_manage_enable(temp_use_u64_vue_a);
//         else
//             stat = m_callable_instance_manage_disable(temp_use_u64_vue_a);

//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_SET_SCHEDULE_ABLE, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
//                                         M_TYPE_Int, (void *)&temp_use_int_vue_a,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         break;
//         //     /** 同步浇水时间 */
//         // case M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME: /* 28 */

//         //     break;
//         //     /** 废弃 */
//         // case M_CMD_TWOWAY_POWEROFF_ONLINE_SE: /* 29*/

//         //     break;
//         /** 提前 9 分 30 秒 或者 1 分 30 秒 请求 浇水时间 */
//     case M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME: /* 30 */
//         /**
//          * @brief 同步请求信息
//          *
//          * @name [ 30 , id , skip_zone , time , weather ]
//          * @param  id 计划 id
//          * @param  skip_zone 1|2|5
//          * @param  time 1#30;2#40
//          * @param  weather 0 - 1 - 2 - 3
//          */
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         m_callable_smart_running_storage_append(temp_use_u64_vue_a, buf, m_callable_timer_manage_get_device_time());

//         break;
//         //     /** 计划被分域跳过s */
//         // case M_CMD_NOTIFY_TO_SERVER_SCHEdULE_SKIP: /* 31 */
//         //                                            /** 设备更新天气缓存 */

//         //     break;
//         // case M_CMD_TWOWAY_REQU_WEATHER_BUFF: /* 32 */

//         //     break;
//         //     /** 未作任何事的传感器接入 */
//         // case M_CMD_NOTIFY_TO_SERVER_SENSOR_CONNECT: /* 34 */

//         //     break;
//         //     /** 新草坪模式 后续时间相关的计划参数 */
//         // case M_CMD_TWOWAY_NEW_LAWN_TIME_INFO: /* 35 */

//         //     break;
//         //     /** 未作任何事的传感器断开 */
//         // case M_CMD_NOTIFY_TO_SERVER_SENSOR_DIS: /* 36 */

//         //     break;
//         /** 设备休眠模式 */
//     case M_CMD_TWOWAY_HIBERNATION_MODE: /* 37 */
//         /**
//          * @brief 设备休眠模式
//          *
//          * @name [ 37 , start , end , mode ]
//          * @param  start 计划 id
//          * @param  end 1|2|5
//          * @param  mode 1#30;2#40
//          */
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_c, buf);

//         m_callable_device_dormant_set(temp_use_u64_vue_a, temp_use_u64_vue_b, temp_use_u64_vue_c);

//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_HIBERNATION_MODE, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
//                                         M_TYPE_U64, (void *)&temp_use_u64_vue_b,
//                                         M_TYPE_U64, (void *)&temp_use_u64_vue_c,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         break;
//         /** 泵模式开启或者关闭   */
//     case M_CMD_TWOWAY_PUMP_MODE: /* 38 */
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
//         m_callable_device_attribute_set_soledold(temp_use_int_vue_a, temp_use_int_vue_b);
//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_PUMP_MODE, M_TYPE_Int,
//                                         (void *)&temp_use_int_vue_a,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         break;
//         /** 快速运行上一个 */
//     case M_CMD_TWOWAY_FASTRUN_SWITCH_PRE: /* 39*/

//         break;
//         /** 快速运行开始 */
//     case M_CMD_TWOWAY_FASTRUN_START: /* 40 */
//         /** 添加一个快速运行*/
//         // m_callable_instance_manage_quick_add(buf, 0);
//         m_callable_instance_manage_quick_gener(buf, m_callable_timer_manage_get_device_time());
//         break;
//         //     /** 快速运行完成 */
//         // case M_CMD_TWOWAY_FASTRUN_COMPLETE: /* 42 */
//         break;
//         /** 快速运行下一个 */
//     case M_CMD_TWOWAY_FASTRUN_SWITCH_NEXT: /* 43 */
//         m_callable_instance_running_event_input(CHANGING_OVER_RIGHT_TRIGGER, 0);

//         break;
//         /** 快速运行暂停 */
//     case M_CMD_TWOWAY_FASTRUN_PAUSE: /* 44 */
//         m_callable_instance_running_event_input(CHANGING_OVER_START_TRIGGER, 0);
//         break;
//         /** 快速运行恢复 */
//     case M_CMD_TWOWAY_FASTRUN_RECOVER: /* 45 */
//         m_callable_instance_running_event_input(CHANGING_OVER_START_TRIGGER, 0);
//         break;
//         /** 全局停止运行 */
//     case M_CMD_TWOWAY_GLOBAL_RUN_STOP: /* 46 */
//         m_callable_instance_running_event_input(CHANGING_OVER_STOP_TRIGGER, 0);
//         break;
//         /** 手动运行开始 */
//     case M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_START: /* 48 */
//         break;
//         /** 手动运行开始 */
//     case M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_STOP: /* 49 */
//         break;
//         /** 手动运行切换 */
//     case M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_SWITCH: /* 51 */

//         break;
//         /** 清除设备信息 */
//     case M_CMD_TWOWAY_RESET_DEVICE: /* 52 */
//         m_callable_device_attribute_all_clear();
//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_RESET_DEVICE, M_TYPE_Str_OK,
//                                         NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         mReboot();
//         break;
//         /** 调节设备灯光亮度 */
//     case M_CMD_TWOWAY_DEVICE_LIGHT_CHANGE: /* 56 */
//         break;
//         /** 远程控制设备按键 */
//     case M_CMD_TWOWAY_CONTROL_DEVICE_KEY: /* 58 */

//         break;
//         /** 跳过一个时间区域的计划 */
//     case M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC: /* 62 */
//         /**
//          * @brief water delay
//          *
//          * @name [ 62 , id , start , end , e_code ]
//          * @param  start 开始时间
//          * @param  end 结束时间
//          */
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_c, buf);
//         m_callable_water_delay_set(temp_use_u64_vue_a, temp_use_u64_vue_b, m_callable_timer_manage_get_device_time());

//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
//                                         M_TYPE_U64, (void *)&temp_use_u64_vue_b,
//                                         M_TYPE_U64, (void *)&temp_use_u64_vue_c,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         break;
//         /** 水锤 */
//     case M_CMD_TWOWAY_WATER_HAMMER: /* 63 */
//                                     /**
//                                      * @brief water hammer
//                                      *
//                                      * @name [ 63 , Enable  ]
//                                      * @param  Enable 开启/关闭 1/0
//                                      */
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
//         m_callable_device_attribute_set_hammer(temp_use_int_vue_a);
//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_WATER_HAMMER, M_TYPE_Int, (void *)&temp_use_int_vue_a,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());

//         break;
//         /** 跳过一个计划 */
//     case M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC: /* 70 */
//                                                   /**
//                                                    * @brief water delay
//                                                    *
//                                                    * @name [ 70 , id , 2 , code , start , end  ]
//                                                    * @param  start 开始时间
//                                                    * @param  end 结束时间
//                                                    */
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_c, buf);
//         m_callable_schedule_skip_set(temp_use_u64_vue_a, temp_use_u64_vue_b, temp_use_u64_vue_c, m_callable_timer_manage_get_device_time());

//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC,
//                                         M_TYPE_U64, (void *)&temp_use_u64_vue_a,
//                                         M_TYPE_Int, (void *)&temp_use_int_vue_b,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         m_callable_timer_manage_get_device_time());
//         break;
//         //     /** 计划跳过通知 */
//         // case M_CMD_NOTIFY_TO_SERVER_A_SCHEDULE_BE_SKIP: /* 74 */

//         //     break;
//         /** 取消跳过一个计划 */
//     case M_CMD_TWOWAY_UNDO_NOT_RUN_A_SCHEDULE_TIMESPC: /* 75 */
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);

//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_UNDO_NOT_RUN_A_SCHEDULE_TIMESPC,
//                                         M_TYPE_U64, (void *)&temp_use_u64_vue_a,
//                                         M_TYPE_Str_OK,NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         m_callable_schedule_skip_clean(temp_use_u64_vue_a);
//         break;
//         /** 取消跳过一段时间 */
//     case M_CMD_TWOWAY_UNDO_NOT_RUN_SCHEDULE_TIMESPC: /* 76 */
//         m_callable_water_delay_clean();
//         m_callable_local_resp_to_remote(M_CMD_TWOWAY_UNDO_NOT_RUN_SCHEDULE_TIMESPC, M_TYPE_Str_OK,
//                                         NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());

//         break;
//         /** 地址设置 */
//     case M_CMD_NOTIFY_TO_SERVER_REQUEST_LOCAL_INFO: /* 82 */
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
//         m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
//         m_callable_device_attribute_set_local(temp_use_u64_vue_a, temp_use_u64_vue_b);
//         m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_REQUEST_LOCAL_INFO, M_TYPE_Str_OK,
//                                         NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());

//         break;
//         /** 配置AB设置  */
//     case M_CMD_NOTIFY_TO_SERVER_ZONE_CONFIG_ABC: /** 87 */
//         m_callable_channel_manage_congfig_sack_param(buf);
//         m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_ZONE_CONFIG_ABC, M_TYPE_Str_OK,
//                                         NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time());
//         break;
//         /** 离线月份的浇水量调整  */
//     case M_CMD_NOTIFY_TO_SERVER_OFFLINE_ADJUST:  /** 88 */
//         m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
//         m_callable_channel_manage_set_offline_running_percent(temp_use_int_vue_a);
//         m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_OFFLINE_ADJUST, M_TYPE_Str_OK,
//                                         NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL,
//                                         M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time(),true);

//         break;
//     case 202: /* 202 */
//         m_static_instance_list_query_print_all();
//         break;
//     default:
//         break;
//     }
//     return stat;
// }

// /**
//  * @brief data send
//  *
//  * @param in_tx data
//  * @param in_len length
//  * @return stat_m
//  */
// stat_m m_ext_net_link_connection_tx(const char *in_tx, int in_len)
// {
//     return succ_r;
// }

// // #include "esp_netif.h"
// // #include "esp_system.h"
// // #include "esp_http_client.h"
// // #include "esp_http_server.h"
// // #include "esp_https_ota.h"

// // static void http_cleanup(esp_http_client_handle_t client)
// // {
// //     esp_http_client_close(client);
// //     esp_http_client_cleanup(client);
// // }

// #define SERVER_ADDR "device.imolaza.com"

// // #define IpaddrS "http://device.imolaza.com/device/network"
// static char NetWorkTemp[50] = {0};
// static char ipaddrTemp[100] = {0};
// /**
//  * @brief 获取目标连接服务器地址
//  *
//  * @param out_addr
//  * @param out_port
//  * @return stat_m
//  */
// stat_m m_ext_network_get_target_addr(char *out_addr, int *out_port)
// {
//     // stat_m stat = fail_r;
//     // int re_code = 3;
//     DEBUG_TEST( DB_I,"Unit Tets --》 m_ext_network_get_target_addr  ");
//     // // *out_addr = "192.168.13.144";
//     strcpy(out_addr, "192.168.13.227");
//     *out_port = 9503;
//     // m_callable_device_attribute_get_mac_addr(NetWorkTemp);
//     // memset(ipaddrTemp, 0, 100);
//     // strcat(ipaddrTemp, "http://");
//     // strcat(ipaddrTemp, SERVER_ADDR);
//     // strcat(ipaddrTemp, "/device/network?version=");
//     // strcat(ipaddrTemp, "1.7.001");
//     // strcat(ipaddrTemp, "&mac=");
//     // strcat(ipaddrTemp, NetWorkTemp);
//     // // sprintf(ipaddrTemp, "http://device.imolaza.com/device/network?version=%s&mac=%s", _device.device_version, _device.mac);
//     // esp_http_client_config_t config = {
//     //     .url = ipaddrTemp,
//     //     .timeout_ms = 3 * 1000,
//     // };
//     // DEBUG_TEST(DB_I,"Addr  :%s", SERVER_ADDR);
//     // DEBUG_TEST(DB_I,"NetWork Test URL :%s", config.url);
//     // esp_http_client_handle_t http_cli = esp_http_client_init(&config);
//     // esp_http_client_open(http_cli, 0);

//     // esp_http_client_fetch_headers(http_cli);

//     // while (re_code--)
//     // {
//     //     if (esp_http_client_get_status_code(http_cli) == 200)
//     //     {
//     //         bzero(out_addr, sizeof(*out_addr));
//     //         bzero(NetWorkTemp, sizeof(NetWorkTemp));
//     //         esp_http_client_read(http_cli, NetWorkTemp, sizeof(NetWorkTemp));
//     //         DEBUG_TEST(DB_I,"%s", NetWorkTemp);
//     //         M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(NetWorkTemp, "%[^:]:%d", out_addr, out_port);
//     //         DEBUG_TEST(DB_W,"Success(%s:%d)\r\n", out_addr, *out_port);
//     //         stat = succ_r;
//     //         break;
//     //     }
//     //     else
//     //     {
//     //         DEBUG_E("http fail(%d)", re_code);
//     //     }
//     //     mDelay_ms(500);
//     // }
//     // http_cleanup(http_cli);

//     return succ_r;
// }
