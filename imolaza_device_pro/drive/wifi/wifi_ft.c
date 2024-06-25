

#include "../drive.h"

#define IP_TO_ARRAY(a, b, c, d, ipaddr) a = esp_ip4_addr1_16(ipaddr), \
                                        b = esp_ip4_addr2_16(ipaddr), \
                                        c = esp_ip4_addr3_16(ipaddr), \
                                        d = esp_ip4_addr4_16(ipaddr)

// #define IP_STR(adr) adr[0],adr[1],adr[2] adr[3]

stat_m m_ext_software_drive_net_wifi_sta_connect_to(const char *SSID, const char *PASSW);
#if !UNIT_TEST
esp_netif_t *m_netif = NULL;
uint32_t ip_addr[8] = {0};
uint32_t gw_addr[4] = {0};
void wifi_event(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = NULL;
    switch (event_id)
    {
    case IP_EVENT_STA_GOT_IP:

        event = (ip_event_got_ip_t *)event_data;
        // 得到IP第一步应该是 判断IP类型 判断有没有   一样不一样

        // GET INFO
        IP_TO_ARRAY(ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3], &event->ip_info.ip);
        IP_TO_ARRAY(gw_addr[0], gw_addr[1], gw_addr[2], gw_addr[3], &event->ip_info.gw);

        // _wifi.base.wifi_status.wifi_is_connected = true;
        DEBUG_TEST(DB_I, "tips: STA connected success !");
        m_callable_network_set_connect_status(true);

        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        m_callable_network_set_connect_status(false);
        // m_callable_middle_connect_handle_set_code(M_CONNECT__CODE__WIFI_DISCONNECT);

        DEBUG_TEST(DB_I, "tips: STA connected DIS !");
        if (mDeviceSystemTime() < 12000)
        {
            DEBUG_TEST(DB_I, "Smart Connect (One) ... ");
            m_callable_wifi_auto_connect();
        }
        break;
    }
}
#endif
/**
 * @brief wifi ip 地址 更新
 *
 * @return stat_m
 */
stat_m m_ext_software_drive_net_wifi_ip_update(void)
{
    m_callable_middle_network_adapter_set_info(ip_addr, gw_addr);
    return 0;
}
stat_m m_ext_wifi_core_init(void)
{
#if !UNIT_TEST

    esp_netif_init();
    esp_event_loop_create_default();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event,
                                                        NULL,
                                                        NULL));
#if USE_CONFIGNET_WIFI_FOR_APP || USE_CONFIGNET_WIFI_AND_AP_FOR_APP
    if (out_check(FRAST_START_UP) == OKK)
    {
        DEBUG_TEST(DB_I, "wifi.c tip : register IP_EVENT and SC_EVENT");
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &smartconfig_event_handler, NULL));
        ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &smartconfig_event_handler, NULL));
    }
    else
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event, NULL));
#endif
#endif

    return succ_r;
}
/**
 * @brief 自主动设计的静态IP信息
 *
 * @param in_ip_add 192.168.0.1
 * @return stat_m
 */
// stat_m m_callable_middle_network_adapter_set_static_ip(char *in_ip_add)
/**
 * @brief 设置静态参数相关
 *
 * @param in_glo_ip_add IP地址
 * @param in_glo_sub_net_mask 子网掩吗
 * @param in_glo_gateway 网关
 * @param in_glo_dns DNS
 * @return stat_m
 */
stat_m m_ext_soft_drive_net_wifi_set_static_ip(uint32_t *in_glo_ip_add,
                                               uint32_t *in_glo_sub_net_mask,
                                               uint32_t *in_glo_gateway,
                                               uint32_t *in_glo_dns)
{
    esp_netif_ip_info_t ip_info = {
        .ip = {.addr = ESP_IP4TOADDR(in_glo_ip_add[0],
                                     in_glo_ip_add[1],
                                     in_glo_ip_add[2],
                                     in_glo_ip_add[3])},
        .gw = {.addr = ESP_IP4TOADDR(
                   in_glo_gateway[0],
                   in_glo_gateway[1],
                   in_glo_gateway[2],
                   in_glo_gateway[3])},
        .netmask = {.addr = ESP_IP4TOADDR(in_glo_sub_net_mask[0],
                                          in_glo_sub_net_mask[1],
                                          in_glo_sub_net_mask[2],
                                          in_glo_sub_net_mask[3])},
    };

    if (m_netif != NULL)
    {
        esp_netif_dhcpc_stop(m_netif);
        (tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_STA));
        if (esp_netif_set_ip_info(m_netif, &ip_info) != ESP_OK)
        {
            esp_netif_dhcpc_stop(m_netif);
            (tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_STA));
            esp_netif_set_ip_info(m_netif, NULL);
            esp_netif_dhcpc_start(m_netif);
            m_callable_middle_network_net_connection_option_set_info(M_NET_CONNECT_OPTION_DHCP);
            DEBUG_TEST_W("\t\t\tUSE Static FAIL !    ( %d.%d.%d.%d )", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
        }
        else
        {
            // DEBUG_TEST(DB_W,"\t\t\tUSE Static IP    ( %d.%d.%d.%d )", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
            DEBUG_TEST_W("\tUSE Static IP( %d.%d.%d.%d) Geteway( %d.%d.%d.%d ) DNS( %d.%d.%d.%d )",
                         in_glo_ip_add[0], in_glo_ip_add[1], in_glo_ip_add[2], in_glo_ip_add[3],
                         in_glo_gateway[0], in_glo_gateway[1], in_glo_gateway[2], in_glo_gateway[3],
                         in_glo_dns[0], in_glo_dns[1], in_glo_dns[2], in_glo_dns[3]);
            // DEBUG_TEST_W("\t\t\tUSE DNS\t      ( %d.%d.%d.%d )", IP_STR(in_glo_dns));
        }
        // esp_netif_dhcpc_start(m_netif);
        {
            tcpip_adapter_dns_info_t dns_info = {0};
            memset(&dns_info, 0, sizeof(dns_info));
            // // 首选
            // IP_ADDR4(&dns_info.ip, in_glo_dns[0], in_glo_dns[1], in_glo_dns[2], in_glo_dns[3]);
            // IP_ADDR4(&dns_info.ip, in_glo_dns[0], in_glo_dns[1], in_glo_dns[2], in_glo_dns[3]);
            (dns_info.ip).u_addr.ip4.addr = (uint32_t)PP_HTONL(LWIP_MAKEU32(in_glo_dns[0], in_glo_dns[1], in_glo_dns[2], in_glo_dns[3]));
            ESP_ERROR_CHECK(tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_MAIN, &dns_info));
            ESP_ERROR_CHECK(tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_BACKUP, &dns_info));
        }
    }
    return succ_r;
}
/**
 * @brief 获取静态IP信息
 *
 * @param in_ip_add 输出的信息
 * @return stat_m
 */
// stat_m m_callable_middle_network_adapter_get_static_ip(char *out_ip_add)

stat_m m_ext_wifi_sta_init()
{
#if !UNIT_TEST

    esp_err_t ret = 0;
    wifi_config_t wifi_config;
    m_ext_wifi_core_init();
    m_netif = esp_netif_create_default_wifi_sta();

    wifi_sta_config_t sta;

    memset(&sta, 0, sizeof(wifi_sta_config_t));

    wifi_config.sta = sta;

    if (m_callable_middle_network_adapter_get_info(ip_addr) == succ_r)
    {
        if (m_callable_middle_network_net_connection_option_get_info() != M_NET_CONNECT_OPTION_MANUAL_STATIC_IP &&
            m_callable_middle_network_net_connection_option_get_info() != M_NET_CONNECT_OPTION_AUTO_STATIC_IP)
            m_callable_middle_network_net_connection_option_set_info(M_NET_CONNECT_OPTION_AUTO_STATIC_IP);
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_set_ps(WIFI_PS_NONE);
    return (ret == ESP_OK ? succ_r : fail_r);
#endif
}

stat_m m_ext_soft_drive_net_wifi_init(void)
{
    stat_m stat = fail_r;
#if !UNIT_TEST

    stat = m_ext_wifi_core_init();
    stat = m_ext_wifi_sta_init();
#endif
    return stat;
}

/**
 * @brief  dhcp 服务
 *
 * @return stat_m
 */
stat_m m_ext_soft_drive_dhcp_option()
{
#if !UNIT_TEST
    // if(boo)
    m_callable_middle_network_net_connection_option_set_info(M_NET_CONNECT_OPTION_DHCP);
    if (m_netif != NULL)
    {
        esp_netif_dhcpc_stop(m_netif);
        (tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_STA));
        esp_netif_set_ip_info(m_netif, NULL);
        esp_netif_dhcpc_start(m_netif);
    }
#endif
    return succ_r;
}
/**
 * @brief 获取wifi信号强度
 *
 * @param sign_strage
 * @return stat_m
 */
stat_m m_ext_soft_drive_net_wifi_get_sign_strage(int *sign_strage)
{
#if !UNIT_TEST
    wifi_ap_record_t apa;
    esp_wifi_sta_get_ap_info(&apa);
    *sign_strage = apa.rssi;
    DEBUG_TEST(DB_I, "ssid:%s , rssi :%d bssid: %hhn\n", apa.ssid, apa.rssi, apa.bssid);
#endif
    return succ_r;
}

/**
 * @brief 断开当前的 wifi 连接
 *
 * @return stat_m
 */
stat_m m_ext_soft_drive_net_wifi_dis_connect(void)
{
#if !UNIT_TEST
    DEBUG_TEST_W("m_ext_soft_drive_net_wifi_dis_connect");
    esp_wifi_disconnect();

    mDelay_ms(50);

#endif
    return succ_r;
}

stat_m wifi_sta_scan(char *in_ssid)
{

#if !UNIT_TEST
    if (in_ssid != NULL)
    {
        DEBUG_TEST(DB_I, "wifi scan(%s)..", in_ssid);
        wifi_scan_config_t scanconf = {.ssid = (uint8_t *)in_ssid};
        esp_wifi_scan_start(&scanconf, true);
    }
#endif
    return succ_r;
}

stat_m m_ext_soft_drive_net_wifi_sta_scan_spec(char *in_ssid)
{
    stat_m stat = fail_r;
    uint16_t wifi_num = 0;
#if !UNIT_TEST
    wifi_sta_scan(in_ssid);
    mDelay_ms(500);
    esp_wifi_scan_get_ap_num(&wifi_num);
    if(wifi_num > 0)
        stat = succ_r;
#endif
    return stat;
}
wifi_config_t wifi_config;
wifi_sta_config_t sta;
// bool re_connect = true;
/**
 * @brief 连接到某一个网络
 *
 * @param SSID ssid
 * @param PASSW 密码
 * @return stat_m
 */
stat_m m_ext_software_drive_net_wifi_sta_connect_to(const char *SSID, const char *PASSW)
{
    stat_m stat = fail_r;
#if !UNIT_TEST
    if (SSID == NULL || strlen(SSID) < 1)
    {
        return stat;
    }
    // if (re_connect)
    DEBUG_TEST(DB_I, "**** %s    %s", (char *)sta.ssid, SSID);
    DEBUG_TEST(DB_I, "************** %s    %s", (char *)sta.password, PASSW);
    // if ((strcmp((char *)sta.ssid, SSID) != 0) || (strcmp((char *)sta.password, PASSW) != 0))
    {
        DEBUG_TEST(DB_I, "--> %s,%s", SSID, PASSW);
        memset(&sta, 0, sizeof(wifi_sta_config_t));
        memset(&wifi_config, 0, sizeof(wifi_config_t));
        memcpy(sta.ssid, SSID, strlen(SSID));
        if (strlen(PASSW) > 3)
        {
            DEBUG_TEST(DB_I, "have psw connection wifi...");
            memcpy(sta.password, PASSW, strlen(PASSW));
            // strcpy((char *)sta.password, PASSW);
        }
        else
        {
            DEBUG_TEST(DB_I, "not  psw connection wifi...");
            memset(sta.password, 0, 64);
        }
        // , printf("-noPsw :--%s", PASSW);
        // ESP_ERR_WIFI_NOT_STARTED
        // memcpy(sta.password, PASSW, strlen(PASSW));
        wifi_config.sta = sta;
        wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
        DEBUG_TEST(DB_I, "将会连接 ssid [%s]  psw [%s]", wifi_config.sta.ssid, wifi_config.sta.password);
        (esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        esp_wifi_set_inactive_time(ESP_IF_WIFI_STA, 60);
        // re_connect = false;
    }
    // else
    // {
    //     DEBUG_TEST(DB_I, "reConnect %s - %s", sta.ssid, sta.password);
    // }
    esp_wifi_connect();



#endif
    return stat;
}
