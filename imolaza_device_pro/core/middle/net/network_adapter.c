

#include "core/fset_lib.h"

typedef struct
{
    /* The ip address assigned by the router  */
    // uint32_t ip_info_num[8];
    /* Use the cache before the address  */
    // uint32_t ip_temp[8];
    /* List of host alternatives for addresses, for using different network segments  */
    // uint32_t ip_info_host_name_num_back[8];
    /* subnet mask */
    // uint32_t ip_info_netmask[8];
    /* gateway */
    // uint32_t ip_info_gw[8];
    /* The attempted address offset, and the default is 50 + ramdom_offset ( 192.168.8.50 + ramdom_offset) */
    // uint32_t ramdom_offset;
    /* The device uses the state of the network connection */
    // enum static_ip_net_info status;
    /* None
     */
    // esp_netif_t *netif;
    /** 全局静态IP地址 */
    uint32_t glo_ip_add[8];
    /** 网关 */
    uint32_t glo_ip_geteway[8];
    /** 子网掩码 */
    uint32_t glo_ip_sub_net_mask[8];
    /** DNS */ 
    uint32_t glo_ip_dns[8];
    /** 连接方式记录 */
    uint32_t glo_ip_connection;

} static_sta_info_change_m;

enum net_connect_option m_net_connect_option = M_NET_CONNECT_OPTION_DHCP;
/** 当前爱你网络适应的IP连接诶方式 */
#define IP_CONNECTION_OPTION "STIP_OP"
#define IP_ADDR "STIP"
#define IP_IN_IPADDR "IPip"
#define IP_IN_GWADDR "IPgw"

static_sta_info_change_m wifi_m;
/**
 * @brief 网络适配器初始化
 *
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_init(void)
{
    stat_m stat = fail_r;
    char temp_addr[20] = {0};
    
    if (m_callable_flash_rescue_message_flag(IP_ADDR, M_GLO_STROAGE_RESCUE_MESSAGE_GET) == succ_r)
    {
        stat = succ_r;
        stat = m_callable_drive_flash_read(M_TYPE_Str, IP_IN_IPADDR, temp_addr);
        DEBUG_TEST(DB_W,"Read IP Buf %s", temp_addr);
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_addr, "%d.%d.%d.%d", &wifi_m.glo_ip_add[0], &wifi_m.glo_ip_add[1], &wifi_m.glo_ip_add[2], &wifi_m.glo_ip_add[3]);
        wifi_m.glo_ip_sub_net_mask[0] = 255;
        wifi_m.glo_ip_sub_net_mask[1] = 255;
        wifi_m.glo_ip_sub_net_mask[2] = 255;
        wifi_m.glo_ip_sub_net_mask[3] = 0;
        wifi_m.glo_ip_dns[0] = 8;
        wifi_m.glo_ip_dns[1] = 8;
        wifi_m.glo_ip_dns[2] = 8;
        wifi_m.glo_ip_dns[3] = 8;
        stat = m_callable_drive_flash_read(M_TYPE_Str, IP_IN_GWADDR, temp_addr);
        if (stat == succ_r)
        {
            DEBUG_TEST(DB_W,"Read GW Buf %s", temp_addr);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_addr, "%d.%d.%d.%d", &wifi_m.glo_ip_geteway[0], &wifi_m.glo_ip_geteway[1], &wifi_m.glo_ip_geteway[2], &wifi_m.glo_ip_geteway[3]);
        }
        else
        {
            wifi_m.glo_ip_geteway[0] = wifi_m.glo_ip_add[0] ; 
            wifi_m.glo_ip_geteway[1] = wifi_m.glo_ip_add[1] ; 
            wifi_m.glo_ip_geteway[2] = wifi_m.glo_ip_add[2] ; 
            wifi_m.glo_ip_geteway[3]  = 1 ; 
        }
        m_callable_drive_flash_read(M_TYPE_U32, IP_CONNECTION_OPTION, &m_net_connect_option);

    }
    return stat;
}
/**
 * @brief 清除网络适配器
 *
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_clear(void)
{
    stat_m stat = fail_r;
    m_callable_network_server_disconnect();
    m_callable_middle_network_net_connection_option_set_info(M_NET_CONNECT_OPTION_DHCP);
    m_callable_flash_rescue_message_flag(IP_ADDR, M_GLO_STROAGE_RESCUE_MESSAGE_DEL);
    memset(wifi_m.glo_ip_add, 0, sizeof(wifi_m.glo_ip_add));
    m_ext_soft_drive_dhcp_option();
    return stat;
}
/**
 * @brief 自主动设计的静态IP信息
 *
 * @param in_ip_add 192.168.0.1
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_set_static_ip(char *in_ip_add, char *in_sub_mask_add, char *in_getway_add, char *in_dns)
{
    int a = 0, b = 0, c = 0, d = 0;
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_ip_add, "%d.%d.%d.%d", &a, &b, &c, &d);
    if (a > 0 && b > 0 && d > 0)
    {
        wifi_m.glo_ip_add[0] = a;
        wifi_m.glo_ip_add[1] = b;
        wifi_m.glo_ip_add[2] = c;
        wifi_m.glo_ip_add[3] = d;
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_sub_mask_add, "%d.%d.%d.%d", &a, &b, &c, &d);
        if (a <= 0 || a != 255)
            return fail_r;
        wifi_m.glo_ip_sub_net_mask[0] = a;
        wifi_m.glo_ip_sub_net_mask[1] = b;
        wifi_m.glo_ip_sub_net_mask[2] = c;
        wifi_m.glo_ip_sub_net_mask[3] = d;
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_getway_add, "%d.%d.%d.%d", &a, &b, &c, &d);
        if (a <= 0 && b <= 0 && d <= 0)
            return fail_r;
        wifi_m.glo_ip_geteway[0] = a;
        wifi_m.glo_ip_geteway[1] = b;
        wifi_m.glo_ip_geteway[2] = c;
        wifi_m.glo_ip_geteway[3] = d;
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_dns, "%d.%d.%d.%d", &a, &b, &c, &d);
        wifi_m.glo_ip_dns[0] = a;
        wifi_m.glo_ip_dns[1] = b;
        wifi_m.glo_ip_dns[2] = c;
        wifi_m.glo_ip_dns[3] = d;

        DEBUG_TEST(DB_W," +- Update IP Addr ---->   ( %s    %s     %s  DNS %s)", in_ip_add, in_sub_mask_add, in_getway_add, in_dns);
        m_callable_drive_flash_write(M_TYPE_Str, IP_IN_IPADDR, in_ip_add);
        m_callable_drive_flash_write(M_TYPE_Str, IP_IN_GWADDR, in_getway_add);
        m_callable_flash_rescue_message_flag(IP_ADDR, M_GLO_STROAGE_RESCUE_MESSAGE_SET);
        m_ext_soft_drive_net_wifi_set_static_ip(wifi_m.glo_ip_add, wifi_m.glo_ip_sub_net_mask, wifi_m.glo_ip_geteway, wifi_m.glo_ip_dns);
        
    }
    return succ_r;
}
/**
 * @brief 获取静态IP信息
 *
 * @param in_ip_add 输出的信息
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_get_static_ip(char *out_ip_add, char *out_sub_mask_add, char *out_getway_add, char *out_dns)
{
    memset(out_ip_add, 0, sizeof(*out_ip_add));
    sprintf(out_ip_add, "%d.%d.%d.%d", wifi_m.glo_ip_add[0], wifi_m.glo_ip_add[1], wifi_m.glo_ip_add[2], wifi_m.glo_ip_add[3]);
    sprintf(out_sub_mask_add, "%d.%d.%d.%d", wifi_m.glo_ip_sub_net_mask[0], wifi_m.glo_ip_sub_net_mask[1], wifi_m.glo_ip_sub_net_mask[2], wifi_m.glo_ip_sub_net_mask[3]);
    sprintf(out_getway_add, "%d.%d.%d.%d", wifi_m.glo_ip_geteway[0], wifi_m.glo_ip_geteway[1], wifi_m.glo_ip_geteway[2], wifi_m.glo_ip_geteway[3]);
    sprintf(out_dns, "%d.%d.%d.%d", wifi_m.glo_ip_dns[0], wifi_m.glo_ip_dns[1], wifi_m.glo_ip_dns[2], wifi_m.glo_ip_dns[3]);
    return succ_r;
}
/**
 * @brief 获取网络适配器内容
 *
 * @param in_ip_add
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_get_info(uint32_t *in_ip_add)
{
    stat_m stat = fail_r;
    int out_runmode = 0;
    m_callable_device_attribute_get_running_mode(&out_runmode);
    if (out_runmode != M_DEVICE_RUNNING_MODE_CONFIG)
    {
        if (wifi_m.glo_ip_add[0] != 0)
        {
            for (size_t i = 0; i < 8; i++)
            {
                in_ip_add[i] = wifi_m.glo_ip_add[i];
            }
            stat = succ_r;
            m_ext_soft_drive_net_wifi_set_static_ip(wifi_m.glo_ip_add, wifi_m.glo_ip_sub_net_mask, wifi_m.glo_ip_geteway, wifi_m.glo_ip_dns);
        }
    }
    return stat;
}
/**
 * @brief 设置网络适配器
 *
 * @param in_ip_add
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_set_info(uint32_t *in_ip_add, uint32_t *in_gw_info)
{
    stat_m stat = fail_r;
    char temp_addr[20] = {0};
    if (wifi_m.glo_ip_add[0] == 0)
    {
        // stat = succ_r;
        for (size_t i = 0; i < 8; i++)
        {
            if ((wifi_m.glo_ip_add[i] != in_ip_add[i] || wifi_m.glo_ip_geteway[i] != in_gw_info[i]) && in_ip_add[0] > 0 && in_ip_add[0] != 236)
            {
                wifi_m.glo_ip_add[i] = in_ip_add[i];
                wifi_m.glo_ip_geteway[i] = in_gw_info[i];
                stat = succ_r;
            }
        }
        if (stat == succ_r)
        {
            sprintf(temp_addr, "%d.%d.%d.%d", wifi_m.glo_ip_add[0], wifi_m.glo_ip_add[1], wifi_m.glo_ip_add[2], wifi_m.glo_ip_add[3]);
            DEBUG_TEST(DB_W,"Update IP Addr ---->   %s", temp_addr);
            m_callable_drive_flash_write(M_TYPE_Str, IP_IN_IPADDR, temp_addr);
            sprintf(temp_addr, "%d.%d.%d.%d", wifi_m.glo_ip_geteway[0], wifi_m.glo_ip_geteway[1], wifi_m.glo_ip_geteway[2], wifi_m.glo_ip_geteway[3]);
            m_callable_drive_flash_write(M_TYPE_Str, IP_IN_GWADDR, temp_addr);
            DEBUG_TEST(DB_W,"Update GW Addr ---->   %s", temp_addr);
            m_callable_flash_rescue_message_flag(IP_ADDR, M_GLO_STROAGE_RESCUE_MESSAGE_SET);
        }
    }
    return stat;
}
/**
 * @brief
 *
 * @param connect_option
 * @return int
 */
void m_callable_middle_network_net_connection_option_set_info(int connect_option)
{
    if (connect_option != NULL &&
        m_net_connect_option != connect_option)
    {
        m_net_connect_option = connect_option;
        m_callable_drive_flash_write(M_TYPE_U32, IP_CONNECTION_OPTION, &m_net_connect_option);
    }
}
/**
 * @brief 获取
 *
 * @return int
 */
int m_callable_middle_network_net_connection_option_get_info()
{
    return m_net_connect_option;
}