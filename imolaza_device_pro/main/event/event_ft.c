

#include "../application/include.h"

#define SSID "ssid"
#define PSW "password"
#define TYPE "type"
#define HTTP_PROXY "http_proxy"

#define TOKEN "token"
#define BINDING "binding"
/**
 * @brief  在线模式 或者说是正常模式 配网模式   的事件处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_online_config_mode_event_handle(enum event_ft event_id, char *in_data, int in_len)
{
    stat_m stat = fail_r;
    char tmp_sid[30];
    int context_cmd = 0;
    // DEBUG_TEST( DB_I,"m_ext_online_config_mode_event_handle  %s", in_data);
    switch (event_id)
    {
        /** 收到数据 */
    case M_EVENT_FT_REVICE_REMOTE_DATA:

        DEBUG_TEST(DB_I, "Online Confnig Net Event: Handle Rece Data ->  %s", in_data);

        if (strstr(in_data, SSID) != NULL && strstr(in_data, PSW) != NULL)
        {
            m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_CONFIG_CONFIG_TRYING_TO_NET);
            m_callable_middle_network_adapter_clear();
            m_callable_device_proper_status_update_to_config_net();
            stat = m_callable_json_data_parse_foramt_one_key(SSID, tmp_sid, in_data);
            m_callable_config_net_set_username(tmp_sid);
            m_callable_device_attribute_set_current_connect_net_name(tmp_sid);
            stat = m_callable_json_data_parse_foramt_one_key(PSW, tmp_sid, in_data);
            m_callable_config_net_set_token(tmp_sid);
            stat = m_callable_json_data_parse_foramt_one_key(TYPE, tmp_sid, in_data);
            if (strstr(tmp_sid, "0") != NULL)
            {
                m_callable_device_attribute_all_clear();
            }
            else if (strstr(tmp_sid, "1") != NULL)
            {
                m_ext_network_set_device_login_state();
            }
            stat = m_callable_json_data_parse_foramt_one_key(HTTP_PROXY, tmp_sid, in_data);
            if(strstr(tmp_sid, "."))
            {
                m_callable_device_attribute_http_proxy_set_addr(tmp_sid);
            }
            stat = m_callable_config_net_start();

            m_callable_config_net_resp(stat);
            if (stat == succ_r)
            {
                // m_callable_device_attribute_running_mode_set_wifi_info(tmp_sid,);
                // m_callable_config_net_save();
                DEBUG_TEST(DB_I, "Config Net Success ! "); // M_EVNBIT_SERVER_INIT
                // 准备开始连接服务器
                m_callable_device_attribute_running_set_mode(M_DEVICE_RUNNING_MODE_CONFIG, M_NETWORK_MANAGE_CONNECT_MODE_TCP);
                m_callable_network_set_connect_mode(M_NETWORK_MANAGE_CONNECT_MODE_WIFI, M_NETWORK_MANAGE_CONNECT_MODE_TCP);
                m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS2_MODE, 0);
                m_callable_network_set_auto_connect(true);
                m_callable_network_set_connect_status(true);
                m_callable_network_set_server_start_connect_server();
            }
            else
            {

                DEBUG_TEST(DB_I, "Config Net Fail ! "); // M_EVNBIT_SERVER_INIT
                m_callable_display_status(M_DISPLAY_WIFI_CONNET_FAIL_MODE, 0);
            }
        }

        else if (strstr(in_data, "[") != NULL)
        {
            if (m_callable_data_parse_format_cmd(&context_cmd, in_data) == succ_r)
            {
                if (context_cmd != 0 && (context_cmd == M_CMD_TOWAY_DEVICE_RECONNECT_TIPS || context_cmd == M_CMD_TWOWAY_SET_DEVICE_TIME))
                    m_ext_server_message_handle(context_cmd, in_data);
            }
        }
        else
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_CONFIG_RECEIVE_DATA_INVALID);
        break;
        /** 网络连接成功 */
    case M_EVENT_FT_LOCAL_NET_CONNECT:
        DEBUG_TEST(DB_I, "Online Confnig Net Event:    ->   Nwtwork Connect Ok");
        // m_callable_display_status(M_DISPLAY_CONNECTING_TO_SERVER_MODE, 0);
        m_callable_network_set_server_start_connect_server();
        break;
        /** 网络连接断开 */
    case M_EVENT_FT_LOCAL_NET_DIS_CONNECT:
        DEBUG_TEST(DB_I, "Online Confnig Net Event:    ->   Network Disconnect !");
        // m_callable_display_status(M_DISPLAY_SERVER_CONNET_FAIL_MODE, 0);
        m_callable_config_net_recode();

        break;
        /** 连接成功服务器 */
    case M_EVENT_FT_LOCAL_SERVER_CONNECT:
        DEBUG_TEST(DB_I, "Online Confnig Net Event:    ->   Sever Connect Success !");
        m_callable_display_status(M_DISPLAY_CONNECTING_TO_SERVER_MODE, 0);
        // m_callable_device_attribute_running_mode_set_direct();

        break;
        /** 断开服务器 */
    case M_EVENT_FT_LOCAL_SERVER_DIS_CONNECT:
        DEBUG_TEST(DB_I, "Online Confnig Net Event:    ->   Server Dis");
        m_callable_display_status(M_DISPLAY_SERVER_CONNET_FAIL_MODE, 0);

        break;

        /** 登陆服务器 */
    case M_EVENT_FT_LOCAL_SERVER_LOGIN:
        DEBUG_TEST(DB_I, "Online Confnig Net Event:    ->   logined");
        m_callable_display_status(M_DISPLAY_DISTRIBUTION_NETWORK_SUCCESS_MODE, 0);
        m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_SERVER_LOGIN_AND_CONNECT_OK);
        m_callable_device_attribute_running_save_mode(M_DEVICE_RUNNING_MODE_DERECT);
        mDelay_ms(2000);
        mReboot(M_RESTART_CAUSE_PREMEDITATED);
        break;
        /** 登陆失败服务器 */
    case M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN:
        DEBUG_TEST(DB_I, "Online Confnig Net Event:    ->   Login fail");

        break;
    case M_EVENT_FT_LOCAL_HTTP_REQU_FAIL:
        DEBUG_TEST(DB_I, "Online Confnig Net Event:    ->   http/ser fail");

        break;
    }
    return stat;
}
/**
 * @brief  离线 绑定模式   的事件处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_offline_config_mode_event_handle(enum event_ft event_id, char *in_data, int in_len)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_I, "离线 事件处理  -> %d   ", event_id);
    char tmp_sid[30];
    int rmode = 0;
    int context_cmd = 0;
    switch (event_id)
    {
        /** 收到数据 */
    case M_EVENT_FT_REVICE_REMOTE_DATA:

        DEBUG_TEST(DB_I, "Offline Confnig Net Event: Handle Rece Data ->  %s", in_data);
        if (strstr(in_data, "binding") != NULL)
        {
            m_callable_offline_bluetooth_running_mode_getting(&rmode);
            if (m_callable_offline_bluetooth_login_flag() == succ_r)
            {
                DEBUG_TEST(DB_I, "设备添加绑定成功");
                if (m_callable_offline_bluetooth_running_mode_setting(M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT) == succ_r)
                {
                    m_callable_display_status(M_DISPLAY_DISTRIBUTION_NETWORK_SUCCESS_MODE, 0);
                    mDelay_ms(2000);
                }
                mReboot(M_RESTART_CAUSE_PREMEDITATED);
            }
        }
        if (strstr(in_data, TOKEN) != NULL)
        {
            /*离线蓝牙模式添加设备认证*/
            m_callable_json_data_parse_foramt_one_key(TOKEN, tmp_sid, in_data);
            m_callable_offline_bluetooth_login(tmp_sid);
            m_callable_json_data_parse_foramt_one_key(TYPE, tmp_sid, in_data);

            if (m_callable_offline_bluetooth_login_flag() == succ_r)
            {
                mDelay_ms(2000);

                if (strstr(tmp_sid, "2") != NULL)
                {
                    m_callable_device_attribute_all_clear();
                }
                m_ext_network_transmisson_specify_mode_tx(M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL, "blu_login_succ", 15);
            }
            else
            {
                mDelay_ms(500);
                m_ext_network_transmisson_specify_mode_tx(M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL, "blu_login_fail", 15);
                DEBUG_TEST(DB_I, "未进行添加设备认证..");
            }
        }
        else if (strstr(in_data, "[101,") != NULL)
        {
            m_ext_network_transmisson_specify_mode_tx(M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL, "101fail", 10);
        }

        // if (strstr(in_data, "[") != NULL)
        // {
        //     if (m_callable_data_parse_format_cmd(&context_cmd, in_data) == succ_r)
        //     {
        //         // if (context_cmd != 0 && (context_cmd == M_CMD_TWOWAY_SET_DEVICE_TIME || context_cmd == M_CMD_LOGIN_AUTHENTICATION || context_cmd == M_CMD_CONFIRM_BINDING))
        //         if (m_callable_offline_bluetooth_login_flag() != succ_r)
        //         {
        //             if (context_cmd != M_CMD_LOGIN_AUTHENTICATION)
        //             {
        //                 DEBUG_TEST(DB_I, "NOT LOGIN_AUTHENTICATION...");
        //                 break;
        //             }
        //         }
        //         else
        //         {
        //             if (m_callable_offline_bluetooth_unbind_flag_get() != true)
        //             {
        //                 if (context_cmd != M_CMD_CONFIRM_BINDING)
        //                 {
        //                     DEBUG_TEST(DB_I, "NOT BINDING...");
        //                     break;
        //                 }
        //             }
        //         }
        //     }
        //     m_ext_server_message_handle(context_cmd, in_data);
        // }
        break;
        /** 网络连接成功 */
    case M_EVENT_FT_LOCAL_NET_CONNECT:

        break;
        /** 网络连接断开 */
    case M_EVENT_FT_LOCAL_NET_DIS_CONNECT:

        break;
        /** 连接成功服务器 */
    case M_EVENT_FT_LOCAL_SERVER_CONNECT:

        break;
        /** 断开服务器 */
    case M_EVENT_FT_LOCAL_SERVER_DIS_CONNECT:

        break;
        /** 登陆服务器 */
    case M_EVENT_FT_LOCAL_SERVER_LOGIN:

        break;
        /** 登陆失败服务器 */
    case M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN:
        DEBUG_TEST(DB_I, "M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN FAIL")
        // m_callable_network_server_disconnect();
        // m_callable_network_set_server_status(M_NETWORK_CONNECT_PROGRESS_);
        break;
    case M_EVENT_FT_LOCAL_HTTP_REQU_FAIL:

        break;
    }
    return stat;
}
/** 设备断线统计 */
int net_connect_count = 0;

stat_m m_ext_derect_config_mode_event_handle(enum event_ft event_id, char *in_data, int in_len)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_I, "Direct Mode Event Handle :  -> %d   ", event_id);
    int device_status = 0;
    m_callable_device_proper_status_get(&device_status);
    switch (event_id)
    {
        /** 收到数据 */
    case M_EVENT_FT_REVICE_REMOTE_DATA:
        DEBUG_TEST(DB_W, "不对了 检查代码问题");
        break;
        /** 网络连接成功 */
    case M_EVENT_FT_LOCAL_NET_CONNECT:

        DEBUG_TEST(DB_I, "Direct Mode Event Handle : Network OK");
        if (
            // device_status == M_DEVICE_GLOBAL_STATUS_IDLE
            // &&
            //     device_status != M_DEVICE_GLOBAL_STATUS_CONFIG_NER
            //     // &&device_status != M_DEVICE_GLOBAL_STATUS_INIT
            // &&
            m_callable_display_status_get() == M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE)
        {
            /** 这里判断是不是 找网络状态   但是主要的一点 要判断是不是长时间没有连接上网络 是的话 还是黄灯光 */
            if (net_connect_count >= 2)
                m_callable_display_status(M_DISPLAY_NETWORK_PROBLEM_MODE, 0);
            else
                m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
        }

        // mEventGroupSetBits(0x04);
        m_callable_network_set_server_start_connect_server();
        // if (m_callable_display_status_get() == M_DISPLAY_IDLE_STATUS_MODE)
        break;
        /** 网络连接断开 */
    case M_EVENT_FT_LOCAL_NET_DIS_CONNECT:
        DEBUG_TEST(DB_I, "M_EVENT_FT_LOCAL_NET_DIS_CONNECT  Net dis");
        if (device_status == M_DEVICE_GLOBAL_STATUS_IDLE && device_status != M_DEVICE_GLOBAL_STATUS_CONFIG_NER &&
            device_status != M_DEVICE_GLOBAL_STATUS_INIT && m_callable_display_status_get() != M_DISPLAY_ZONE_SELECT_MODE)
            m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 2);
        ++net_connect_count;
        // if (net_connect_count == 1)
        //     m_callable_network_connect();
        if (net_connect_count == 10)
        {
            DEBUG_TEST(DB_W, "Direct Mode Event Handle : Net Change  ... ，%d", net_connect_count);
            // m_callable_middle_network_adapter_clear();
        }
        m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);

        break;
        /** 连接成功服务器 */
    case M_EVENT_FT_LOCAL_SERVER_CONNECT:
        net_connect_count = 0;
        if (device_status == M_DEVICE_GLOBAL_STATUS_IDLE &&
            device_status != M_DEVICE_GLOBAL_STATUS_CONFIG_NER &&
            device_status != M_DEVICE_GLOBAL_STATUS_INIT &&
            m_callable_display_status_get() != M_DISPLAY_ZONE_SELECT_MODE)
            m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
        break;
        /** 断开服务器 */
    case M_EVENT_FT_LOCAL_SERVER_DIS_CONNECT:

        break;

        /** 登陆服务器 */
    case M_EVENT_FT_LOCAL_SERVER_LOGIN:
        if (device_status == M_DEVICE_GLOBAL_STATUS_IDLE &&
            device_status != M_DEVICE_GLOBAL_STATUS_CONFIG_NER &&
            m_callable_display_status_get() != M_DISPLAY_ZONE_SELECT_MODE &&
            device_status != M_DEVICE_GLOBAL_STATUS_INIT)
            m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);

        break;
        /** 登陆失败服务器 */
    case M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN:
        DEBUG_TEST(DB_W, "Direct Mode Event Handle : Login Fail  ... ");
        m_callable_network_server_disconnect();
        m_callable_network_set_server_status(M_NETWORK_CONNECT_PROGRESS_);
        //     m_static_event_handle(M_EVENT_TIME_6_SEC);
        break;
    case M_EVENT_FT_LOCAL_HTTP_REQU_FAIL:
        ++net_connect_count;
        DEBUG_TEST(DB_W, "Direct Mode Event Handle : Http / (Connect Server )  Fail -> %d  ... ", net_connect_count);
        /** 黄灯不轻易 后利马断网重新连接  重新连接之后还是无网络 第一次就黄灯 */
        if (net_connect_count >= 1)
        {
            if (net_connect_count == 2)
            {
                m_callable_network_disconnect();
                m_callable_middle_network_adapter_clear();
                DEBUG_TEST(DB_W, " M_CALLABLE_MIDDLE_NETWORK_ADAPTER_CLEAR IP ADD    ");
                m_callable_network_set_connect_status(false);
            }
            else if (net_connect_count > 2 && net_connect_count % 2 == 0)
            {
                m_callable_network_disconnect();
                m_callable_network_set_connect_status(false);
            }
            else if (m_callable_display_status_get() == M_DISPLAY_IDLE_STATUS_MODE)
            {
                m_callable_display_status(M_DISPLAY_NETWORK_PROBLEM_MODE, 0);
            }
        }

        // if ((net_connect_count >= 1 && net_connect_count % 3 == 0)) //|| (net_connect_count > 3 && net_connect_count % 2 == 1))

        break;
    }
    return stat;
}
/**
 * @brief  离线模式   的事件处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_derect_offline_mode_event_handle(enum event_ft event_id, char *in_data, int in_len)
{
    return 0;
}
/**
 * @brief  直接连接模式的处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_net_server_event_handle(char *in_data, int in_len)
{
    stat_m stat = fail_r;
    /** 看一下 是什么消息    比如 ping */
    /** 如果是的话   直接加入队列   */
    stat = m_callable_network_transmisson_rx_msg_handle(true, in_data, in_len);

    return stat;
}
/**
 * @brief
 *
 * @param data
 * @param datalen
 * @return stat_m  M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL
 */
stat_m m_ext_network_transmisson_specify_mode_tx(enum connect_mode mode, char *data, int datalen)
{

    switch (mode)
    {
    case M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL:
        m_ext_network_transmisson_bluetool_tx(data, datalen);
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP:
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP
        m_ext_network_link_transformission_tx(data, datalen);
#endif
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL:
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL
        m_ext_network_link_transformission_wolfSSL_tx(data, datalen);
#endif
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS:
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MBEDTLS
        m_ext_network_link_transformission_mbedtls_tx(data, datalen);
#endif
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_MQTT:
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MQTT
        m_ext_network_link_transformission_mqtt_tx(data, datalen);
#endif

        break;
    default:
        break;
    }
    return succ_r;
}