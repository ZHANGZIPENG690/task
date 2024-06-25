

// #include "../network.h"
#include "network_connect.h"

// stat_m m_static_connect_mqtt(char *addr, int port)
// {
//     stat_m stat = fail_r;
//     m_ext_network_link_to(addr , port);
//     return stat;
// }
// stat_m m_static_connect_tcp(char *addr, int port)
// {
//     stat_m stat = fail_r;

//     return stat;
// }
// stat_m m_static_connect_bluetool(void)
// {
//     stat_m stat = fail_r;

//     return stat;
// }

// int ota_updata_flagm;

stat_m m_static_network_server_connect_to(struct network_man *nt_man)
{
    stat_m stat = succ_r;
    DEBUG_TEST( DB_I,"Will be selected based on the connection method ...  ");
    /** 先找到需要连接的方式地址 */
    if (nt_man->server_con_conf.sm_mode == M_NETWORK_MANAGE_CONNECT_MODE_MQTT)
    {
        DEBUG_TEST( DB_I,"mqtt Connect ... ");
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MQTT
        stat = m_ext_software_drive_server_mqtt_connect(nt_man->server_con_conf.sm_addr, nt_man->server_con_conf.sm_port);
#endif /*  M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP */
    }
    else if (nt_man->server_con_conf.sm_mode == M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL)
    {
        DEBUG_TEST( DB_I,"bluetool Connect ... ");
        // stat = m_ext_software_drive_server_blue_connect(nt_man->server_con_conf.sm_addr , nt_man->server_con_conf.sm_port);
    }
    else if (nt_man->server_con_conf.sm_mode == M_NETWORK_MANAGE_CONNECT_MODE_TCP)
    {
        DEBUG_TEST( DB_I,"TCP Connect ... ");
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP
        stat = m_ext_software_drive_server_tcp_connect(nt_man->server_con_conf.sm_addr, nt_man->server_con_conf.sm_port);
#endif /*  M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP */
    }
    else if (nt_man->server_con_conf.sm_mode == M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL)
    {
        DEBUG_TEST( DB_I,"WolfSSL Connect ... ");
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL
        stat = m_ext_software_drive_server_wolfSSL_connect(nt_man->server_con_conf.sm_addr, nt_man->server_con_conf.sm_port);
#endif /*  M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL */
    }
    else if (nt_man->server_con_conf.sm_mode == M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS)
    {
        DEBUG_TEST( DB_I,"MbedTLS Connect ... ");
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MBEDTLS
        stat = m_ext_software_drive_server_mbedtls_connect(nt_man->server_con_conf.sm_addr, nt_man->server_con_conf.sm_port);
#endif /*  M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MBEDTLS */
    }
    if (stat == succ_r)
    {
        DEBUG_TEST( DB_I,"Server Success ...  ");
        nt_man->server_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_START_LOGIN;
        // ota_updata_flagm=1;
    }
    return stat;
}
stat_m m_static_network_net_connect_to(struct network_man *nt_man)
{
    stat_m stat = succ_r;
    /** 先找到需要连接的方式地址 */
    stat = m_ext_network_net_connect_to(nt_man->net_con_conf.nm_name, nt_man->net_con_conf.nm_token);
    // printf("%s", nt_man->net_con_conf.nm_name);
    if (stat == succ_r)
    {
        nt_man->net_con_conf.state.active = M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS;
    }
    return stat;
}