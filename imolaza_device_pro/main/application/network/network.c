

#include "core/fset_lib.h"

/**
 * @brief 连接验证
 *
 * @param out_cmd
 */
extern void m_ext_network_config_user_reconnect_cmd(char *out_cmd);

/**
 * @brief 初始化网络方式   wifi还是蓝牙
 *
 * @param mode
 * @return stat_m
 */
stat_m m_ext_drive_net_manage_init(enum running_mode nm, enum connect_mode sm)
{
    /** 设备运行方式 */
    switch (nm)
    {
    case M_DEVICE_RUNNING_MODE_NONE:
    case M_DEVICE_RUNNING_MODE_CONFIG:
        DEBUG_TEST(DB_I, " M_DEVICE_RUNNING_MODE_CONFIG ");
        // 如果是配网模式 直接初始化 蓝牙 和 wifi
        m_ext_soft_drive_net_wifi_init();
        // 如果是配网模式 直接初始化 蓝牙 和 wifi
        m_ext_soft_drive_net_bluetool_init();
        mDelay_ms(1000);
        m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS1_MODE, 0);
        m_callable_device_proper_status_update_to_idle();

        break;
    case M_DEVICE_RUNNING_MODE_DERECT:
        DEBUG_TEST(DB_I, " M_DEVICE_RUNNING_MODE_DERECT ");
        m_ext_soft_drive_net_wifi_init();
        mDelay_ms(1000);
        m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 2);
        m_callable_network_connect();
        m_callable_device_proper_status_update_to_idle();

        break;
    case M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT:
        DEBUG_TEST(DB_I, " M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT ");
        m_ext_soft_drive_net_bluetool_init();
        mDelay_ms(1000);
        m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
        m_callable_device_proper_status_update_to_idle();
        break;
    case M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG:
        DEBUG_TEST(DB_I, " M_DEVICE_RUNNING_MODE_OFFLINE ");
        m_ext_soft_drive_net_bluetool_init();
        mDelay_ms(1000);
        m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
        m_callable_device_proper_status_update_to_idle();

        break;
    default:
        DEBUG_TEST(DB_I, " None ");
        break;
    }

    /** 服务器连接方式 */
    switch (sm)
    {
    case M_NETWORK_MANAGE_CONNECT_MODE_MQTT:
        DEBUG_TEST(DB_I, " M_NETWORK_MANAGE_CONNECT_MODE_MQTT ");
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MQTT
        m_ext_software_drive_server_mqtt_init();
#endif /* M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MQTT */
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL:
        DEBUG_TEST(DB_I, " M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL ");
        m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP:
        DEBUG_TEST(DB_I, " M_NETWORK_MANAGE_CONNECT_MODE_TCP ");
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP
        m_ext_software_drive_server_tcp_init();
#endif /* M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP */
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL:
        DEBUG_TEST(DB_I, " M_NETWORK_MANAGE_CONNECT_MODE_TCP ");
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL
        m_ext_software_drive_server_wolfSSL_init();
#endif /* M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL */
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS:
        DEBUG_TEST(DB_I, " M_NETWORK_MANAGE_CONNECT_MODE_MbedTLS ");
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MBEDTLS
        m_ext_software_drive_server_mbedtls_init();
#endif /* M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MBEDTLS */
        break;

    default:
        DEBUG_TEST(DB_I, " None ");
        break;
    }

    return 0;
}

/**
 * @brief 注意这个个函数，返回如果是成功，会被内部判定为认证成功，没有认证成功 会不断重复 在一段时间之内
 *
 * @return stat_m
 */
stat_m m_ext_network_manage_login(enum connect_mode sm_mode, int count)
{
    char arr[520] = {0};
    switch (sm_mode)
    {
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS:
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL:
        m_ext_network_config_user_login_cmd(arr, 0);
        // strcpy(arr , "年哦ibdew 额的iewjbdieded叠 俄的为单位额的为的俄文的俄文的的");
        m_callable_spec_scene_net_connext_tx(arr, strlen(arr), 10, 0, M_TIMEOUT_LONG_LIMIT_MIN, false);
        DEBUG_TEST(DB_I, "WolfSSL Login .. %s ", arr);
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_MQTT:
        // strcpy(arr, "[14]#@#\r\n");

        m_ext_network_config_user_reconnect_cmd(arr);
        // if (m_user_network_config_login_cmd(arr, count) == succ_r)

        // m_callable_network_data_tx(false, arr, strlen(arr));
        // m_callable_network_specify_mode_data_tx(M_NETWORK_MANAGE_CONNECT_MODE_MQTT, false, arr, strlen(arr));
        DEBUG_TEST(DB_I, "MQTT Login .. %s ", arr);

        m_callable_spec_scene_net_connext_tx(arr, strlen(arr), 10, 0, M_TIMEOUT_LONG_LIMIT_MIN, false);

        // m_ext_network_link_transformission_mqtt_tx(arr, strlen(arr));
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL:
        DEBUG_TEST(DB_I, "Blue Login");
        m_ext_network_transmisson_specify_mode_tx(M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL, arr, strlen(arr));
        break;
    case M_NETWORK_MANAGE_CONNECT_MODE_TCP:
        DEBUG_TEST(DB_I, "TCP Login");
        // 这里需要区分 配网登陆 还是 改wifi   还是第一登陆
        // m_user_network_config_login_cmd(arr, count);
        m_ext_network_config_user_login_cmd(arr, count);

        // m_callable_network_data_tx(false, arr, strlen(arr));
        m_callable_spec_scene_net_connext_tx(arr, strlen(arr), 10, 0, M_TIMEOUT_LONG_LIMIT_MIN, false);

        break;
    default:
        break;
    }
    return succ_r;
}

/**
 * @brief 网络连接
 *
 * @param username  网络用户名
 * @param token 认证密钥
 * @return stat_m
 */
int mmmloop_checkout_connect = 0;
stat_m m_ext_network_net_connect_to(char *username, char *token)
{
    DEBUG_TEST(DB_I, " m_ext_network_net_connect_to [ %s , %s ] ", username, token);
    m_ext_software_drive_net_wifi_sta_connect_to(username, token);
    // m_ext_software_drive_net_wifi_sta_connect_to("HCTJ12_m", "123567456");
    // :whosyourdaddy;S:HUAWEI_B316_2D01
    // m_ext_software_drive_net_wifi_sta_connect_to("HUAWEI_B316_2D01", "whosyourdaddy");
    // m_ext_software_drive_net_wifi_sta_connect_to("LQJUSA", "123456789");
    // m_ext_software_drive_net_wifi_sta_connect_to("HUAWEI_B316_AB84" , "sunnykong5555sdo");
    // m_ext_software_drive_net_wifi_sta_connect_to("RT-LINK", "123567456");
    // m_ext_software_drive_net_wifi_sta_connect_to("Hi-Link", "123567456");
    // m_ext_software_drive_net_wifi_sta_connect_to("HCTJ04", "123567456");
    DEBUG_TEST(DB_I, "%d", mmmloop_checkout_connect);
    return 0;
}

// /**
//  * @brief Get the address and port (possibly http)
//  *
//  * @param in_addr address
//  * @param in_port port
//  * @return stat_m
//  */
// stat_m m_ext_net_target_addr(char *in_addr, int *in_port)
// {
//     return succ_r;
// }

// /**
//  * @brief network linking
//  *
//  * @param in_addr address
//  * @param in_port port
//  * @return stat_m
//  */
// stat_m m_ext_net_link_connection(char *in_addr, int in_port)
// {
//     return succ_r;
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
// /**
//  * @brief Data is received from the link
//  *
//  * @param out_data The data received
//  * @param len length
//  * @return stat_m
//  */
// stat_m m_ext_net_link_receive_data(char *out_data, int len)
// {
//     return succ_r;
// }