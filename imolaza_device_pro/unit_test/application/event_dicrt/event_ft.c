

// // #include "../application/include.h"
// #include "core/fset_lib.h"

// #define SSID "ssid"
// #define PSW "password"
// /**
//  * @brief  在线模式 或者说是正常模式 配网模式   的事件处理
//  *
//  * @param in_data 收到的数据
//  * @param in_len 数据长度
//  * @return stat_m
//  */
// stat_m m_ext_online_config_mode_event_handle(enum event_ft event_id, char *in_data, int in_len)
// {
//     stat_m stat = fail_r;
//     char tmp_sid[30];
//     int context_cmd = 0;

//     // DEBUG_TEST( DB_I,"m_ext_online_config_mode_event_handle  %s", in_data);
//     switch (event_id)
//     {
//         /** 收到数据 */
//     case M_EVENT_FT_REVICE_REMOTE_DATA:

//         DEBUG_TEST( DB_I,"在线配网事件处理 收到数据 ->  %s", in_data);
//         if (strstr(in_data, SSID) != NULL &&  strstr(in_data, PSW) != NULL )
//         {
//             m_callable_device_proper_status_update_to_config_net();
//             stat = m_callable_json_data_parse_foramt_one_key(SSID, tmp_sid, in_data);
//             m_callable_config_net_set_username(tmp_sid);
//             stat = m_callable_json_data_parse_foramt_one_key(PSW, tmp_sid, in_data);
//             m_callable_config_net_set_token(tmp_sid);
//             stat = m_callable_config_net_start();

//             m_callable_config_net_resp(stat);
//             if (stat == succ_r)
//             {
//                 DEBUG_TEST( DB_I,"配网成功 "); // M_EVNBIT_SERVER_INIT
//                 // 准备开始连接服务器
//                 m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS2_MODE, 0);
//                 m_callable_config_net_save();
//                 m_callable_network_set_server_start_connect_server();
//             }
//             else
//             {
//                 DEBUG_TEST( DB_I,"配网失败 "); // M_EVNBIT_SERVER_INIT
//                 m_callable_display_status(M_DISPLAY_WIFI_CONNET_FAIL_MODE, 0);
//             }
//         }
//         else if (strstr(in_data, "[") != NULL)
//         {
//             if (m_callable_data_parse_format_cmd(&context_cmd, in_data) == succ_r)
//             {
//                 if (context_cmd != 0 && context_cmd == M_CMD_TWOWAY_SET_DEVICE_TIME)
//                     m_ext_server_message_handle(context_cmd, in_data);
//             }
//         }
//         break;
//         /** 网络连接成功 */
//     case M_EVENT_FT_LOCAL_NET_CONNECT:
//         DEBUG_TEST( DB_I,"在线配网事件处理   ->   网络连接成功");
//         // m_callable_display_status(M_DISPLAY_CONNECTING_TO_SERVER_MODE, 0);

//         break;
//         /** 网络连接断开 */
//     case M_EVENT_FT_LOCAL_NET_DIS_CONNECT:
//         DEBUG_TEST( DB_I,"在线配网事件处理   ->   网络断开连接");
//         // m_callable_display_status(M_DISPLAY_SERVER_CONNET_FAIL_MODE, 0);
//          m_callable_config_net_recode();

//         break;
//         /** 连接成功服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_CONNECT:
//         DEBUG_TEST( DB_I,"在线配网事件处理   ->   服务器连接成功");
//         m_callable_display_status(M_DISPLAY_CONNECTING_TO_SERVER_MODE, 0);
//         // m_callable_device_attribute_running_mode_set_direct();

//         break;
//         /** 断开服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_DIS_CONNECT:
//         DEBUG_TEST( DB_I,"在线配网事件处理   ->   服务器断开连接");
//         m_callable_display_status(M_DISPLAY_SERVER_CONNET_FAIL_MODE, 0);

//         break;

//         /** 登陆服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_LOGIN:
//         DEBUG_TEST( DB_I,"在线配网事件处理   ->   已经登陆");
//         m_callable_display_status(M_DISPLAY_DISTRIBUTION_NETWORK_SUCCESS_MODE, 0);
//         mDelay_ms(2000);
//         mReboot();
//         break;
//         /** 登陆失败服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN:
//         DEBUG_TEST( DB_I,"在线配网事件处理   ->   登陆失败");

//         break;
//     }
//     return stat;
// }
// /**
//  * @brief  离线 配网模式   的事件处理
//  *
//  * @param in_data 收到的数据
//  * @param in_len 数据长度
//  * @return stat_m
//  */
// stat_m m_ext_offline_config_mode_event_handle(enum event_ft event_id, char *in_data, int in_len)
// {
//     stat_m stat = fail_r;
//     DEBUG_TEST( DB_I,"离线配网事件处理  -> %d   %s", event_id, in_data);
//     //
//     switch (event_id)
//     {
//         /** 收到数据 */
//     case M_EVENT_FT_REVICE_REMOTE_DATA:

//         break;
//         /** 网络连接成功 */
//     case M_EVENT_FT_LOCAL_NET_CONNECT:

//         break;
//         /** 网络连接断开 */
//     case M_EVENT_FT_LOCAL_NET_DIS_CONNECT:

//         break;
//         /** 连接成功服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_CONNECT:

//         break;
//         /** 断开服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_DIS_CONNECT:

//         break;
//         /** 登陆服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_LOGIN:

//         break;
//         /** 登陆失败服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN:

//         break;
//     }
//     return stat;
// }

// stat_m m_ext_derect_config_mode_event_handle(enum event_ft event_id,char *in_data, int in_len)
// {
//     stat_m stat = fail_r;
//     DEBUG_TEST( DB_I,"直接连接事件处理  -> %d   ", event_id);
//     //
//     switch (event_id)
//     {
//         /** 收到数据 */
//     case M_EVENT_FT_REVICE_REMOTE_DATA:
//         DEBUG_TEST(DB_W,"不对了 检查代码问题");
//         break;
//         /** 网络连接成功 */
//     case M_EVENT_FT_LOCAL_NET_CONNECT:
//         m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
//         // mEventGroupSetBits(0x04);
//         m_callable_network_set_server_start_connect_server();
//         break;
//         /** 网络连接断开 */
//     case M_EVENT_FT_LOCAL_NET_DIS_CONNECT:
//         m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 2);
//         DEBUG_TEST( DB_I,"Net dis");
//         // m_callable_network_connect();

//         break;
//         /** 连接成功服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_CONNECT:

//         break;
//         /** 断开服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_DIS_CONNECT:

//         break;

//         /** 登陆服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_LOGIN:

//         break;
//         /** 登陆失败服务器 */
//     case M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN:

//         break;
//     }
//     return stat;

// }
// /**
//  * @brief  直接连接模式的处理
//  *
//  * @param in_data 收到的数据
//  * @param in_len 数据长度
//  * @return stat_m
//  */
// stat_m m_ext_net_server_event_handle(char *in_data, int in_len)
// {
//     stat_m stat = fail_r;
//     /** 看一下 是什么消息    比如 ping */
//     /** 如果是的话   直接加入队列   */
//     stat = m_callable_network_transmisson_rx_msg_handle(true, in_data, in_len);

//     return stat;
// }
// /**
//  * @brief
//  *
//  * @param data
//  * @param datalen
//  * @return stat_m  M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL
//  */
// stat_m m_ext_network_transmisson_specify_mode_tx(enum connect_mode mode, char *data, int datalen)
// {
//     switch (mode)
//     {
//     case M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL:
//         // m_ext_network_transmisson_bluetool_tx(data , datalen);
//         break;
    
//     default:
//         break;
//     }
//     return succ_r;
// }