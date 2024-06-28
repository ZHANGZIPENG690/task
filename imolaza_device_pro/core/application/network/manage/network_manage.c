

#include "../network.h"
#include "network_manage.h"
#include "../handle/network_handle.h"

struct network_man nwkm;

/**
 * @brief 网络管理初始化
 *
 * @return stat_m
 */
stat_m m_static_network_manage_init(void)
{
    // memset(&nwkm, 0, sizeof(struct network_man));
    // nwkm.con_conf.nm_topic = "Top";
    // 应该根据对应的配置初始化对应的设备
    // m_static_network_tr
    nwkm.net_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_CONNECT_READY;
    m_ext_drive_net_manage_init(nwkm.net_con_conf.nm_mode, nwkm.server_con_conf.sm_mode);
    nwkm.server_con_conf.state.auto_connect = true;

    return succ_r;
}
/**
 * @brief 设置网络的配置
 *
 * @param user_name 用户名
 * @param user_token  认证
 * @return stat_m
 */
stat_m m_static_newwork_manage_set_net_conf(char *user_name, char *user_token)
{
    if (user_name != NULL && strlen(user_name) > 0)
        strcpy(nwkm.net_con_conf.nm_name, user_name);
    // strcpy(nwkm.net_con_conf.nm_name, "RT-LINK");
    if (user_token != NULL && strlen(user_token) > 0)
        strcpy(nwkm.net_con_conf.nm_token, user_token);
    // strcpy(nwkm.net_con_conf.nm_token, "123567456");
    nwkm.net_con_conf.state.auto_connect = true;
    // nwkm.net_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_CONNECT_READY;
    DEBUG_TEST(DB_W, "Conf- Change WIFI --%s---%s--", user_name, user_token);

    return succ_r;
}
/**
 * @brief 设置自动连接
 *
 * @param auto_conn
 * @return stat_m
 */
stat_m m_static_network_set_auto_connect(bool auto_conn)
{
    nwkm.server_con_conf.state.auto_connect = auto_conn;
    return succ_r;
}
/**
 * @brief 设置网络信息
 *
 * @param user_name  用户认证名
 * @param user_token 用户token
 * @return stat_m
 */
// stat_m m_callable_network_set_net_inf0(char *user_name, char *user_token)
// {
//     if (user_name != NULL)
//         strcpy(nwkm.net_con_conf.nm_name, user_name);
//     if (user_token != NULL)
//         strcpy(nwkm.net_con_conf.nm_token, user_token);
//     return succ_r;
// }
stat_m m_static_network_net_connect(void)
{
    stat_m stat = fail_r;
    if (nwkm.net_con_conf.state.active != M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS)
    {
        if (nwkm.net_con_conf.state.auto_connect)
        {
            DEBUG_TEST(DB_I, "m_static_network_manage_connect connecting ...");
            stat = m_static_network_net_connect_to(&nwkm);
        }
    }
    return stat;
}
/**
 * @brief 设置连接状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_set_connect_success(void)
{
    nwkm.net_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS;
    DEBUG_TEST(DB_W, "Dir- Network Change --%d-----", 1);
    return succ_r;
}

/**
 * @brief 连接成功开始认证
 *
 * @return stat_m
 */
stat_m m_static_network_manage_login(void)
{
    DEBUG_TEST(DB_I, "Try Login (%d)...,", nwkm.server_con_conf.state.connent_count);
    // if(nwkm.server_con_conf.state.connent_count == 100)

    return m_ext_network_manage_login(nwkm.server_con_conf.sm_mode, nwkm.server_con_conf.state.connent_count); // enum connect_mode sm_mode
}

// stat_m m_static_network_manage_set_connect_mode(enum connect_mode sm_mode , enum connect_mode nm_mode)
stat_m m_static_network_manage_set_connect_mode(int nm_mode, int sm_mode)
{
    nwkm.server_con_conf.sm_mode = sm_mode;
    nwkm.net_con_conf.nm_mode = nm_mode;
    return succ_r;
}

/**
 * @brief 获取服务器的ip地址和端口   当处于非离线模式的时候
 *          获取到的地址有三次的保质期 意思是三次调用都会返回同义词的缓存结果
 *                之后会重新获取一次 节省资源
 *
 * @return stat_m
 */
stat_m m_static_network_get_server_target_addr(void)
{
    return m_ext_network_get_target_addr(nwkm.server_con_conf.sm_addr, &nwkm.server_con_conf.sm_port);
}
/**
 * @brief 离线模式的 在登陆之前试探性认证口令消息
 *
 * @return stat_m
 */
stat_m m_static_network_offline_test_login_msg(void)
{
    return succ_r;
}
/**
 * @brief 修改爲在線改變wifi的狀態
 *
 * @return stat_m
 */
stat_m m_callable_network_set_status_change_net()
{
    nwkm.server_con_conf.state.connent_count = 1;
    return succ_r;
}
/**
 * @brief 获取连接次数
 *
 * @return stat_m
 */
stat_m m_callable_network_get_connect_count(int *count)
{
    *count = nwkm.server_con_conf.state.connent_count;
    return succ_r;
}

/**
 * @brief 使用指定的连接方式连接服务器
 *
 * @param mode
 * @return stat_m
 */
stat_m m_static_network_manage_connect(void)
{
    stat_m stat = fail_r;
    int smode;
    int loop_x = 20;
    stat = m_callable_device_attribute_get_net_connect_mode(&smode);

    if (smode == M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL)
    {
        if (nwkm.server_con_conf.state.active == M_NETWORK_CONNECT_PROGRESS_START_LOGIN)
            stat = succ_r;
    }
    else
    {

        // 没有连接wifi 就连接wifi
        if (nwkm.net_con_conf.state.active != M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS)
        {
            if (nwkm.net_con_conf.state.auto_connect && m_callable_display_status_get() != M_DISPLAY_ZONE_RUNNING_MODE)
            {
                DEBUG_TEST(DB_I, "m_static_network_manage_connect connecting ...");
                m_callable_middle_smart_connect_pro(M_SMART_TYPE_WIFI, true);
                stat = m_static_network_net_connect_to(&nwkm);
            }
        }
        else
        {
            stat = succ_r;
            m_callable_middle_smart_connect_pro(M_SMART_TYPE_WIFI, false);
        }
        // DEBUG_TEST( DB_I,"m_static_network_manage_connect connecting %d ..." , nwkm.net_con_conf.state.active );

        /** 先找到需要连接的方式地址 */
        if (stat == succ_r && nwkm.server_con_conf.state.active == M_NETWORK_CONNECT_PROGRESS_CONNECT_READY)
        // if (stat == succ_r && nwkm.server_con_conf.state.auto_connect)
        {
            switch (smode)
            {
            case M_NETWORK_MANAGE_CONNECT_MODE_MQTT:
            case M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL:
            case M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS:
            case M_NETWORK_MANAGE_CONNECT_MODE_TCP:
                DEBUG_TEST(DB_I, "M_NETWORK_MANAGE_CONNECT_MODE   TCP / MQTT  %d", nwkm.server_con_conf.sm_port);
                // 断开原因是主动断开   第一次连接   或者每隔十次 连接 就获取一次 地址和连接端口
                if (nwkm.server_con_conf.state.connent_count <= 1 || nwkm.server_con_conf.state.connent_count % 10 == 0 ||
                    nwkm.server_con_conf.sm_port == 0)
                {
                    m_callable_middle_smart_connect_pro(M_SMART_TYPE_HTTP, true);
                    stat = m_ext_network_get_target_addr(nwkm.server_con_conf.sm_addr, &nwkm.server_con_conf.sm_port);

                    // if (nwkm.server_con_conf.state.connent_count % 10 == 0 && nwkm.server_con_conf.state.connent_count % 10 > 0)
                    // {
                    //     m_ext_soft_drive_net_wifi_dis_connect();
                    //     m_static_network_manage_set_connect_status(false);
                    // }
                }
                if (stat == succ_r)
                {
                    m_callable_middle_smart_connect_pro(M_SMART_TYPE_HTTP, false);
                    m_callable_middle_smart_connect_pro(M_SMART_TYPE_TCP, true);
                    nwkm.server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_CONNECT_READY;
                    // m_static_network_handle_progress_call(nwkm.server_con_conf.state.active, 0);
                    stat = m_static_network_server_connect_to(&nwkm);
                    if (stat == succ_r)
                    {
                        nwkm.server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_START_LOGIN;

                        // if (nwkm.server_con_conf.state.connent_count == -1)
                        nwkm.server_con_conf.state.connent_count = 0;
                        nwkm.server_con_conf.state.net_err_count = 0;
                        // m_static_network_handle_progress_call(nwkm.server_con_conf.state.active, 0);
                    }
                    else
                    {
                        nwkm.server_con_conf.state.net_err_count++;
                    }
                }
                else
                {
                    ++nwkm.server_con_conf.state.net_err_count;
                    nwkm.server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_CONNECT_READY;
                    DEBUG_TEST(DB_W, "Http Fail ... ");
                    // mDelay_ms(1000);
                    // m_static_network_handle_progress_call(nwkm.server_con_conf.state.active, 0);
                }
                if (stat == fail_r)
                {
                    while (loop_x--)
                    {
                        mDelay_ms(100);
                        if (m_static_network_manage_get_net_connect_status() != succ_r)
                            break;
                    }
                }
                break;
            case M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL:
                stat = m_static_network_server_connect_to(&nwkm);
                break;

            default:
                break;
            }
        }
    }
    return stat;
}

/**
 * @brief 主动断开网络连接
 *
 * @return stat_m
 */
stat_m m_callable_network_disconnect(void)
{
    m_ext_soft_drive_net_wifi_dis_connect();
    // nwkm.server_con_conf.sm_port = 0;
    return m_static_network_manage_set_connect_status(false);
}
/**
 * @brief 主动断开服务器连接
 *
 * @return stat_m
 */
stat_m m_callable_network_server_disconnect(void)
{
    nwkm.server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_CONNECT_READY;
    nwkm.server_con_conf.state.cause = M_NETWORK_HAPPEN_CAUSE_REMOTE_PROACTIVE_ACTION;
    if (nwkm.server_con_conf.sm_mode == M_NETWORK_MANAGE_CONNECT_MODE_TCP)
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP
        m_ext_network_server_tcp_disconnect();
#else
    {
        DEBUG_TEST_E("Err Config Option !");
    }
#endif /* M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP */
    else if (nwkm.server_con_conf.sm_mode == M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL)
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL
        m_ext_network_server_wolfSSL_disconnect();
#else
    {
        DEBUG_TEST_E("Err Config Option !");
    }
#endif /* M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL */
    else if (nwkm.server_con_conf.sm_mode == M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS)
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MBEDTLS
        m_ext_network_server_mbedtls_disconnect();
#else
    {
        DEBUG_TEST_E("Err Config Option !");
    }
#endif /* M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL */
    else
    {
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MQTT
        m_ext_network_server_disconnect();
#endif /* M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MQTT */
        DEBUG_TEST(DB_I, "6 6 6 ++ +");
    }
    if (nwkm.server_con_conf.sm_mode != M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL)
    {
        DEBUG_TEST(DB_I, "5 5 5 ++ +");
        m_static_network_manage_set_server_connect_status(M_NETWORK_CONNECT_PROGRESS_CONNECT_READY);
    }
    // m_static_network_manage_set_server_connect_status(M_NETWORK_CONNECT_PROGRESS_CONNECT_READY);

    return succ_r;
}

stat_m m_static_network_set_server_auto_connect(bool auto_conn)
{
    nwkm.server_con_conf.state.auto_connect = auto_conn;
    return succ_r;
}
/**
 * @brief 设置自动连接
 *
 * @param auto_conn
 * @return stat_m
 */
stat_m m_static_network_set_net_auto_connect(bool auto_conn)
{
    nwkm.net_con_conf.state.auto_connect = auto_conn;
    return succ_r;
}
/**
 * @brief 获取当前的自动连接诶
 *
 * @param auto_conn
 * @return stat_m
 */
stat_m m_static_network_get_auto_connect(void)
{
    if (nwkm.server_con_conf.state.auto_connect)
        return succ_r;
    else
        return fail_r;
}

/**
 * @brief 返回当前连接状态    和 状态处理回调函数 流程基本一致
 *
 * @return enum connect_progress
 */
enum connect_progress m_static_network_get_cnnect_state(void)
{
    return nwkm.server_con_conf.state.active;
}

/**
 * @brief 设置连接状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_set_connect_status(bool stus)
{
    DEBUG_TEST(DB_W, "- Network Change --%d-----", stus);
    if (stus)
    {
        m_callable_middle_connect_handle_set_code(M_CONNECT__CODE__WIFI_SUCCESS);
        (nwkm.net_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS);
        m_static_online_config_mode_event_handle(nwkm.net_con_conf.nm_mode, M_EVENT_FT_LOCAL_NET_CONNECT, NULL, 0);
    }
    else
    {
        // m_callable_middle_connect_handle_set_code(M_CONNECT__CODE__WIFI_DISCONNECT);
        (nwkm.net_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_CONNECT_READY);
    }
    return succ_r;
}
/**
 * @brief 获取连接状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_get_net_connect_status(void)
{
    stat_m stat = fail_r;
    if (nwkm.net_con_conf.state.active >= M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS)
        stat = succ_r;
    else
        stat = fail_r;
    return stat;
}
// /**
//  * @brief 设置服务器的扎ungtai
//  *
//  * @param progress
//  * @return stat_m
//  */
// stat_m m_static_network_manage_set_server_status(enum connect_progress progress)
// {
//     nwkm.server_con_conf.state.active = progress;
// }

/**
 * @brief 设置 服务器 连接状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_set_server_connect_status(int stus)
{
    nwkm.server_con_conf.state.active = stus;
    return succ_r;
}
// stat_m m_static_network_manage_set_server_connect_status(bool stus)
// {
//     if (stus)
//         nwkm.server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS;
//     else
//         nwkm.server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_CONNECT_READY;
//     return succ_r;
// }
/**
 * @brief 设置 服务器登陆 状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_set_server_login_status(bool stus)
{
    if (stus)
        nwkm.server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS,
        nwkm.server_con_conf.state.connent_count++;
    // printf("在干苏木 ？？？、%d\n", nwkm.server_con_conf.state.connent_count);
    else
        nwkm.server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_START_LOGIN;
    return succ_r;
}
/**
 * @brief 获取 服务器 连接状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_get_server_login_status(void)
{
    // DEBUG_TEST(DB_W,"nwkm.server_con_conf.state.active   : %d", nwkm.server_con_conf.state.active);
    if (nwkm.server_con_conf.state.active >= M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS)
        return succ_r;
    else
        return fail_r;
}
/**
 * @brief 获取服务器的连接状态
 *
 * @param connect_progress
 * @return stat_m
 */
stat_m m_static_network_manage_get_server_connect_status(enum connect_progress *connect_progress)
{
    stat_m stat = fail_r;
    *connect_progress = nwkm.server_con_conf.state.active;
    return stat;
}
