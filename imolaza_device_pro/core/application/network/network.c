

#include "network.h"
#include "./transform/network_transmission.h"
#include "../device_proper/device_attribute.h"
#include "./manage/network_manage.h"

/** 连接次数触发 最大 */
#define NET_CONNECT_OFFLING_COUNT_MAX 10
/** 为反馈Ping的最大超时时间 s */
#define NET_CONNECT_REPING_COUNT_OUT_MAX 60 * 1000

// enum running_mode  pre_temp_run_mode = 0;

static char rx_buffer[RECE_BUF_SIZE];

struct connect_man
{
    /** 举例的连接模式 */
    enum connect_mode pre_temp_mode;
    /** 保证Ping超时 上一次接收到数据或者Ping的时间*/
    uint64_t server_ping_timeout;
    /** 用来计算服务器连接次数 */
    int server_connect_count_out;
    /** 连接服务器计数 */
    uint32_t net_connect_offling_count;
    /** Ping 发送时间，发送最大时间默认是 最大超时时间的 1/4 超过就会发送ping */
    uint64_t server_requ_ping_time;
};
/** 连接管理 */
struct connect_man conn_manage = {0};

/**
 * @brief 连接服务管理
 */
void m_static_link_server_manage_loop(void *args);

void *server_loop = NULL;
void *tx_loop = NULL;
void *rx_loop = NULL;

/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_callable_network_init(void)
{
    stat_m stat = fail_r;
    /** 初始化连接模式 */
    m_callable_middle_network_adapter_init();
    stat = m_static_network_manage_init();
    /** 连接管理 */
    mTaskCreate(server_loop, m_static_link_server_manage_loop, "s_link_server", 1024 * 10, (void *)3, 5, NULL); //--sock tcp link 5
    /** TX发送和处理 */
    mTaskCreate(tx_loop, m_static_network_tx_handle_loop, "s_link_tx_handle", 1024 * 4, (void *)3, 5, NULL); //--sock tcp link
    /** RX处理 */
    mTaskCreate(rx_loop, m_static_network_rx_handle_loop, "s_link_rx_handle", 512 * 7, (void *)3, 5, NULL); //--sock tcp link

    stat = m_callable_network_transmisson_init();
    return stat;
}

/**
 * @brief 设置连接方式
 *
 * @param nm_mode   网络连接方式
 * @param sm_mode   服务器连接方式
 * @return stat_m
 */
stat_m m_callable_network_set_connect_mode(enum connect_mode nm_mode, enum connect_mode sm_mode)
{
    stat_m stat = fail_r;
    conn_manage.pre_temp_mode = sm_mode;
    DEBUG_TEST(DB_W, "Info Connect Mode :%d   %d", nm_mode, sm_mode);
    stat = m_static_network_manage_set_connect_mode(nm_mode, sm_mode);
    return stat;
}
/**
 * @brief 连接网络
 *
 * @return stat_m
 */
// stat_m m_callable_network_connect(enum connect_mode mode)
stat_m m_callable_network_connect()
{
    stat_m stat = fail_r;
    // pre_temp_mode = mode;

    stat = m_static_network_manage_connect();
    return stat;
}
/**
 * @brief auto wifi ap
 * @warning In order to ensure reliable links, it is recommended to call in the timer
 * @return stat_m
 */
stat_m m_callable_wifi_auto_connect(void)
{
    stat_m stat = fail_r;
    // pre_temp_mode = mode;

    stat = m_static_network_net_connect();
    return stat;
}
/**
 * @brief 设置开启或者关闭自动连接
 *
 * @return stat_m
 */
stat_m m_callable_network_set_net_auto_connect(bool enable)
{
    return m_static_network_set_net_auto_connect(enable);
}
/**
 * @brief 设置服务器开启或者关闭自动连接
 *
 * @return stat_m
 */
stat_m m_callable_network_set_auto_connect(bool enable)
{
    stat_m stat = fail_r;
    // m_static_network_set_net_auto_connect
    // stat = m_static_network_set_net_auto_connect(enable);
    stat = m_static_network_set_auto_connect(enable);
    return stat;
}
int ping = 0;
char Pings[10][10] = {"ping1#@#", "ping2#@#", "ping3#@#", "ping4#@#", "ping5#@#", "ping6#@#", "ping7#@#", "ping8#@#", "ping9#@#"};
/**
 * @brief 保持连接活跃,如果喜爱年在不再连接状态的话 或自动连接    否则是心跳数据 每次执行时间 30s
 *
 * @return stat_m
 */
stat_m m_callable_network_keep_active(void)
{
    stat_m stat = fail_r;
    // enum connect_progress cnp_state = m_static_network_get_cnnect_state();
    // 如果现在是连接成功
    if ((conn_manage.pre_temp_mode == M_NETWORK_MANAGE_CONNECT_MODE_TCP ||
         conn_manage.pre_temp_mode == M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL) &&
        m_static_network_manage_get_server_login_status() == succ_r)
    {
        if (mDeviceSystemTime() > conn_manage.server_requ_ping_time + NET_CONNECT_REPING_COUNT_OUT_MAX / 3)
        {
            // 发送和维持心跳  如果需要的话
            m_callable_network_specify_mode_data_tx(conn_manage.pre_temp_mode, false, Pings[(ping++) % 9], 8);
            DEBUG_TEST_SHORT_PRINT("[%d]PING Send (%lld)  And Max (%ds) ..", ping, mDeviceSystemTime() - conn_manage.server_requ_ping_time, NET_CONNECT_REPING_COUNT_OUT_MAX / 3000);
        }
        if (mDeviceSystemTime() > conn_manage.server_ping_timeout + NET_CONNECT_REPING_COUNT_OUT_MAX && m_static_network_manage_get_server_login_status() == succ_r)
        {
            DEBUG_TEST_SHORT_PRINT("NO Ping Resp -> Disconnect !");
            m_callable_middle_err_handle_set_code(M_ERROR_CODE_NETWORK_DISCONNECT_CAUSE_NO_PING_RETURN);
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_SERVER_LOGIN_NO_RESPONSE);
            m_callable_network_server_disconnect();
            ping = 0;
        }
    }
    // else if (cnp_state != M_NETWORK_CONNECT_PROGRESS_NOT_INIT)
    // {
    //     // 如果现在是自动连接
    //     if (m_static_network_get_auto_connect() == succ_r)
    //     {
    //         stat = m_callable_network_connect();
    //     }
    // }
    return stat;
}

void m_static_link_server_manage_loop(void *args)
{
    // static char final_buf[200] = {0};
    enum running_mode nmode;
    enum connect_mode smode = conn_manage.pre_temp_mode;
    enum connect_progress connect_prog;
    stat_m stat = succ_r;


    // 等待服务器初始化完成
    m_callable_event_group_wait_bits(server_loop, M_EVNBIT_CONNRCT_SERVER, true);
    m_static_network_manage_set_server_connect_status(M_NETWORK_CONNECT_PROGRESS_CONNECT_READY);

    while (1)
    {
        while (1)
        {
            /** 如果wifi断开 或者 服务器没有登陆 */
            if (m_callable_network_get_net_connect_status() == fail_r || m_static_network_manage_get_server_login_status() == fail_r)
                break;
            mDelay_ms(800);
        }
        // 模式是在线 如果wifi连接上了 并且允许连接服务 通过http获取连接地址  连接成功了 开始认证
        // 模式是离线 如果有人连接上了  发送测试认证数据等待认证
        // 目的 是连接上服务器、认证完成、可以正常传输数据 算完成
        /** 1、获取设备连接模式 */
        m_callable_device_attribute_get_running_mode(&nmode);
        m_static_network_manage_get_server_connect_status(&connect_prog);

        //  0 : 错误诊断     但是必须是网络错误 而不是连接不上 Wifi
        if (conn_manage.net_connect_offling_count > NET_CONNECT_OFFLING_COUNT_MAX)
        {
            DEBUG_TEST(DB_W, ">（X not used X） Failed more than once (MAX: %d) Trigger network adjustment ... ", conn_manage.net_connect_offling_count);
            conn_manage.net_connect_offling_count = 0;
            // (m_ext_soft_drive_net_wifi_sta_scan_spec(nmode.))
            m_callable_network_disconnect();
            // m_callable_middle_network_adapter_clear();
            // mDelay_ms(5000);
        }

        //  0.5 : 保持基础网络连接
        if (nmode != M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT && nmode != M_DEVICE_RUNNING_MODE_NONE)
        {
            if (m_static_network_manage_get_net_connect_status() != succ_r)
            {
                // if (nmode == M_DEVICE_RUNNING_MODE_CONFIG)
                m_static_online_config_mode_event_handle(nmode, M_EVENT_FT_LOCAL_NET_DIS_CONNECT, NULL, 0);
                DEBUG_TEST(DB_I, "get_net_connect_status() != succ_r ------>  m_callable_network_connect()");
                mDelay_ms(200);
                m_callable_network_connect();
                conn_manage.net_connect_offling_count++;
                continue;
            }
        }
        else
        {
            // DEBUG_TEST(DB_W, "运行模式:M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT");
            mDelay_ms(500);
            continue;
        }
        //  1、准备连接服务器
        if (connect_prog == M_NETWORK_CONNECT_PROGRESS_CONNECT_READY &&
            m_static_network_get_auto_connect() == succ_r)
        {
            stat = fail_r;
            if (nmode != M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT && nmode != M_DEVICE_RUNNING_MODE_NONE)
            {
                DEBUG_TEST(DB_I, "Online Mode connecting server ...  %d", m_static_network_manage_get_net_connect_status());
            }
            else
            {
                DEBUG_TEST(DB_I, "Offline waiting connect ...   %d", m_static_network_manage_get_net_connect_status());
                continue;
            }
            // 连接服务器 他会有获取http地址    会直接返回失败或者成功 阻塞
            conn_manage.net_connect_offling_count++;
            m_static_network_transform_status_rx_able_set(true);
            mDelay_ms(100);
            DEBUG_TEST(DB_I, "nect_prog == M_NETWORK_CONNECT_PROGRESS_CONNECT_READY  --------> will connect server 2");
            if (m_callable_network_connect() == succ_r)
            {
                // 不管是哪种模式 道连接成功 就是连接成功了
                stat = succ_r;
                conn_manage.net_connect_offling_count = 0;
                conn_manage.server_connect_count_out = 0;
                m_callable_network_set_server_status(M_NETWORK_CONNECT_PROGRESS_START_LOGIN);
                // if (nmode == M_DEVICE_RUNNING_MODE_CONFIG)
                m_static_online_config_mode_event_handle(nmode, M_EVENT_FT_LOCAL_SERVER_CONNECT, NULL, 0);
            }
            else
            {
                conn_manage.server_connect_count_out++;
                m_static_online_config_mode_event_handle(nmode, M_EVENT_FT_LOCAL_HTTP_REQU_FAIL, NULL, 0);
            }
        }

        //  2、登陆服务器
        if (connect_prog == M_NETWORK_CONNECT_PROGRESS_START_LOGIN)
        {
            stat = fail_r;
            mDelay_ms(100);
            m_static_network_transform_status_rx_able_set(true);
            m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_SERVER_LOGIN_LOGGING);
            if (m_callable_network_manage_get_server_login_status() == succ_r)
            {
                stat = succ_r;
                m_static_network_manage_set_server_connect_status(M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS);
            }
            // 没有登陆就登陆
            else if (m_static_network_manage_login() == succ_r)
            {
               
                ping = 0;
                mDelay_ms(100);
                // 可能是连接断开
                // if (nmode == M_DEVICE_RUNNING_MODE_CONFIG)
                // m_static_online_config_mode_event_handle(nmode, M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN, NULL, 0);
                m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);

                {
                    m_static_network_manage_get_server_connect_status(&connect_prog);
                    DEBUG_TEST(DB_W, "%s Try Login .. .. Wait...(%d) ", __FUNCTION__, connect_prog);
                    stat = fail_r;
                    for (size_t i = 0; i < 30; i++)
                    {
                        mDelay_ms(800);
                        m_static_network_manage_get_server_connect_status(&connect_prog);
                        if (connect_prog == M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS)
                        {
                            stat = succ_r;
                            mDelay_ms(800);
                            // M_NETWORK_CONNECT_PROGRESS_START_LOGIN
                            DEBUG_TEST(DB_W, "Login Success...");
                            m_callable_middle_smart_connect_pro(M_SMART_TYPE_TCP, false);
                            m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_SERVER_LOGIN_SUCCESS);
                            break;
                        }
                        else if (connect_prog == M_NETWORK_CONNECT_PROGRESS_START_LOGIN)
                            ;
                        else
                            break;
                    }
                    if (stat == fail_r)
                    {
                        DEBUG_TEST(DB_W, "Login Fail...");
                        m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_SERVER_LOGIN_TIMEOUT_FAIL);
                        m_callable_network_server_disconnect();
                    }
                    m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
                }
            }
        }
        else
            stat = fail_r;
        if (conn_manage.net_connect_offling_count > NET_CONNECT_OFFLING_COUNT_MAX / 2)
        {
            mDelay_ms(2500);
        }
        // 3、收取数据 、 管理连接
        if (connect_prog == M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS)
        {
            DEBUG_TEST(DB_I, "Successfully connected and Login, the next step is to manage the chain status ...");
            // if (nmode == M_DEVICE_RUNNING_MODE_CONFIG)
            m_static_online_config_mode_event_handle(nmode, M_EVENT_FT_LOCAL_SERVER_LOGIN, NULL, 0);
            mEventGroupReSetBits(server_loop, M_EVNBIT_CONNRCT_SERVER);
        }
        mDelay_ms(200);
    }
}

// /**
//  * @brief
//  *
//  * @param data
//  * @param datalen
//  * @return stat_m
//  */
// stat_m m_callable_network_data_tx(const char *in_tx_data, int in_tx_len, uint64_t in_spec_cmd, uint64_t in_spec_cmd_flag, uint64_t in_timeout, bool in_tx_queue)
// {
//     stat_m stat = fail_r;
//     if (in_tx_data != NULL)
//     {
//         stat = m_callable_spec_scene_net_connext_tx(in_tx_data, in_tx_len, in_spec_cmd, in_spec_cmd_flag, in_timeout, in_tx_queue);

//     }
//     return stat;
// }
/**
 * @brief
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
stat_m m_callable_network_specify_mode_data_tx(enum connect_mode mode, bool is_queue, char *data, int datalen)
{
    stat_m stat = fail_r;
    if (data != NULL)
    {
        stat = m_static_network_transmisson_specify_mode_tx(mode, is_queue, data, datalen);
        if (is_queue)
            mEventGroupSetBits(tx_loop, M_EVNBIT_TXHANDLE);
    }
    return stat;
}
#ifdef M_GLOBAL_TRANSFORM_USE_TEA_ENCRYPT
char rx_buf[512] = {0};
#endif
/**
 * @brief 调用收取数据
 *
 * @param smode
 * @param data
 * @param datalen
 * @param in_time_out
 * @return stat_m
 */
stat_m m_callable_network_ext_tcp_mqtt_offline_data_rx(enum connect_mode smode, char *data, int *datalen, uint64_t in_time_out)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_I, "m_callable_network_ext_tcp_mqtt_offline_data_rx");
    switch (smode)
    {
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS:
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MBEDTLS
        if (m_ext_network_link_transformission_mbedtls_rx(rx_buffer, datalen, in_time_out) == succ_r)
        {
            stat = succ_r;
        }
#endif
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP:
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP
    ReRecv:
        if (m_ext_net_link_receive_data(data, datalen, in_time_out) == succ_r)
        {

#ifdef M_GLOBAL_TRANSFORM_USE_TEA_ENCRYPT
            memset(rx_buf, 0, 512);
            m_callable_crypt_xxtea_decrypt(rx_buffer, &datalen, rx_buf);
#endif
            /** 收到了数据，清除Ping */
            conn_manage.server_ping_timeout = conn_manage.server_requ_ping_time = mDeviceSystemTime();
            if (data != NULL && data[0] != '[')
            {
                DEBUG_TEST_SHORT_PRINT("Resp: %s", data);
                goto ReRecv;
            }
            stat = succ_r;
        }

        /* code */
#endif
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_MQTT:
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MQTT
        if (m_ext_net_link_mqtt_receive_data(rx_buffer, datalen, in_time_out) == succ_r)
        {
            stat = succ_r;
        }
#else
        mDelay_ms(3000);
#endif
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL:
        if (m_ext_net_link_bluetool_receive_data(data, datalen, in_time_out) == succ_r)
        {
            stat = succ_r;
        }
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL:
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL
    ReRecv:
        if (m_ext_network_link_transformission_wolfSSL_rx(rx_buffer, datalen, in_time_out) == succ_r)
        {
            /** 收到了数据，清除Ping */
            conn_manage.server_ping_timeout = mDeviceSystemTime();
            if (rx_buffer != NULL && rx_buffer[0] != '[')
            {
                DEBUG_TEST_SHORT_PRINT("Resp: %s", rx_buffer);
                goto ReRecv;
            }
            stat = succ_r;
        }

#endif
        break;
    default:
#if UNIT_TEST
        while (1)
            ;
        stat = succ_r;
#endif
        break;
    }
    return stat;
}
/**
 * @brief 收到了数据,只有在 登陆会调用
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
stat_m m_callable_network_data_rx(char *data, int datalen)
{
    stat_m stat = fail_r;
    // 判断是不是ping
    int nmode = 0, smode = 0;
    stat = m_callable_device_attribute_get_running_mode(&nmode);

    stat = m_callable_device_attribute_get_net_connect_mode(&smode);
    /** 发送的肯定是请求配置的数据 会表明是蓝牙还是wifi */
    if (nmode == M_DEVICE_RUNNING_MODE_NONE)
    {
        if (strstr(data, CONFIG_NET_OR_BIND_TEXT_SSID) != NULL && strstr(data, CONFIG_NET_OR_BIND_TEXT_PSW) != NULL)
        {
            nmode = M_DEVICE_RUNNING_MODE_CONFIG;
            m_callable_device_attribute_running_set_mode(nmode, M_NETWORK_MANAGE_CONNECT_MODE_TCP);
        }
        else
        {
            m_callable_device_attribute_running_set_mode(nmode, M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL);
            nmode = M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG;
        }
    }
    switch (nmode)
    {
        /** 配网模式 */
    case M_DEVICE_RUNNING_MODE_CONFIG:
        // 县处理一下     ，，，， 不用处理
        m_ext_online_config_mode_event_handle(M_EVENT_FT_REVICE_REMOTE_DATA, data, datalen);
        break;
    /** 直连模式 */
    case M_DEVICE_RUNNING_MODE_DERECT:
        DEBUG_TEST(DB_I, "Direct Mode Rx %s", data);
        m_ext_net_server_event_handle(data, datalen);
        break;
    /** 离线绑定模式 */
    case M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG:
        m_ext_offline_config_mode_event_handle(M_EVENT_FT_REVICE_REMOTE_DATA, data, datalen);
        break;
    /** 离线运行模式 */
    case M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT:
        // m_ext_offline_config_mode_event_handle(M_EVENT_FT_REVICE_REMOTE_DATA, data, datalen);
        if (data != NULL && datalen > 2)
        {
            DEBUG_TEST(DB_I, "Offline Direct Mode Rx %s  %d", data, datalen);
            m_ext_net_server_event_handle(data, datalen);
        }

        break;

    default:
        break;
    }

    return stat;
}
/**
 * @brief 网络事件分发
 *
 * @param in_mode 运行模式
 * @param evn_id 网络事件类型
 * @param in_data 搜到的数据
 * @param in_len 长度
 * @return stat_m
 */
stat_m m_static_online_config_mode_event_handle(enum running_mode in_mode, enum event_ft evn_id, char *in_data, int in_len)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_E, "%d", in_mode);
    switch (in_mode)
    {
    case M_DEVICE_RUNNING_MODE_CONFIG:
        stat = m_ext_online_config_mode_event_handle(evn_id, in_data, in_len);
        break;
    case M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG:
        stat = m_ext_offline_config_mode_event_handle(evn_id, in_data, in_len);
        break;
    case M_DEVICE_RUNNING_MODE_DERECT:
        stat = m_ext_derect_config_mode_event_handle(evn_id, in_data, in_len);
    case M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT:
        DEBUG_TEST(DB_I, "m_static_online_config_mode_event_handle M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT");
        stat = m_ext_derect_offline_mode_event_handle(evn_id, in_data, in_len);
        break;

    default:
        break;
    }
    return stat;
}

/**
 * @brief 设置服务器的连接状态
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_set_net_info(char *user_name, char *user_token)
{
    DEBUG_TEST(DB_W, "Setting Network :  %s - %s", user_name, user_token);
    return m_static_newwork_manage_set_net_conf(user_name, user_token);
}
/**
 * @brief 设置开始连接服务器
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_set_server_start_connect_server(void)
{
    stat_m stat = fail_r;
    mEventGroupSetBits(server_loop, M_EVNBIT_CONNRCT_SERVER);
    return stat;
}

/**
 * @brief 复位开始连接服务器  用于离线模式，不然线程会循环执行
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_reset_server_start_connect_server(void)
{
    stat_m stat = fail_r;
    mEventGroupReSetBits(server_loop, M_EVNBIT_CONNRCT_SERVER);
    return stat;
}

/**
 * @brief 设置服务器的连接状态
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_set_server_status(int stus)
{
    stat_m stat = fail_r;
    m_static_network_transform_status_rx_able_set(stus);
    stat = m_static_network_manage_set_server_connect_status(stus);
    return stat;
}

/**
 * @brief 获取服务器的连接状态
 *
 * @param connect_progress
 * @return stat_m
 */
stat_m m_callable_network_manage_get_server_status(enum connect_progress *connect_progress)
{
    return m_static_network_manage_get_server_connect_status(connect_progress);
}
/**
 * @brief 设置当前 wifi 连接的状态
 *
 * @return stat_m
 */
stat_m m_callable_network_set_connect_status(bool stus)
{
    stat_m stat = fail_r;
    stat = m_static_network_manage_set_connect_status(stus);
    return stat;
}
/**
 * @brief 设置服务器的登陆状态
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_set_server_login_status(bool stus)
{
    stat_m stat = fail_r;
    // if (stus)
    //     m_callable_device_proper_status_update_to_online();
    // else
    // {
    //     enum device_status out_ste;
    //     m_callable_device_proper_status_get(&out_ste);
    //     if (out_ste == M_DEVICE_GLOBAL_STATUS_ONLINE || out_ste == M_DEVICE_GLOBAL_STATUS_OFFLINE)
    //         m_callable_device_proper_status_update_to_offline();
    // }
    // m_callable_spec_scene_net_connext_tx();
    m_static_network_transform_status_tx_able_set(stus);
    stat = m_static_network_manage_set_server_login_status(stus);
    return stat;
}

/**
 * @brief 获取当前 wifi 连接的状态
 *
 * @return stat_m
 */
stat_m m_callable_network_get_net_connect_status(void)
{
    stat_m stat = fail_r;
    stat = m_static_network_manage_get_net_connect_status();

    return stat;
}
/**
 * @brief 获取连接状态
 *
 * @return stat_m
 */
stat_m m_callable_network_manage_get_server_login_status(void)
{
    stat_m stat = fail_r;
    stat = m_static_network_manage_get_server_login_status();
    return stat;
}
