
// #include "esp32/fun/fun.h"
// #include "esp32/fun/text.h"
// #include "conn_eth.h"
// #include "esp_wifi.h"
// #include "hot_so_fast.h"
// #include "esp_netif.h"
// #include "tool.h"
// #include "esp_wpa2.h"

// #include <stdlib.h>

// extern time_manage _time;
// extern wifi_manager _wifi;

// void m_net_global_adapter_init(esp_netif_t *netif);
// void m_wifi_set_static_ip(void);
// void m_wifi_unset_static_ip(void);
// void m_impl_getip_event(void *event_data);
// void m_net_global_adapter_disable_static_ip();
// void m_net_global_adapter_set_dns(uint32_t main_dns, uint32_t backup_dns);
// void m_wifi_set_try_ip(void);

// // 关于网络连接部分的代码自动适配

// static_sta_info_change_m m_static_dec = {M_CLEAN, M_CLEAN, IP_BACKUP_TRY, M_CLEAN, M_CLEAN, 0, undef, NULL};

// /*     Event *     */

// void m_impl_getip_event(void *event_data)
// {
//     put_in(STAT_STA_GOT_IP, OKK);
//     if (event_data == NULL)
//         return;
//     const ip_event_got_ip_t *event = (const ip_event_got_ip_t *)event_data;
//     // 得到IP第一步应该是 判断IP类型 判断有没有   一样不一样
//     if (m_static_dec.status == undef && rescue_message_flag(STATIC_IP_IN_FLAG, M_RESCUE_MESSAGE_GET) == succ_m)
//         m_static_dec.status = static_mode;

//     if (event != NULL && m_static_dec.status == undef)
//     {
//         // GET INFO
//         IP_TO_ARRAY(m_static_dec.ip_temp[0], m_static_dec.ip_temp[1], m_static_dec.ip_temp[2], m_static_dec.ip_temp[3], &event->ip_info.ip);
//         // DEBUG_I(I2P, m_static_dec.ip_temp[0], m_static_dec.ip_temp[1], m_static_dec.ip_temp[2], m_static_dec.ip_temp[3]);
//         if (m_static_dec.ip_temp[0] == 169 &&
//             m_static_dec.ip_temp[1] == 254)
//             m_static_dec.status = try_mode;
//         else
//             m_static_dec.status = change_mode;
//     }
//     if (m_static_dec.status == change_mode)
//     {
//         // DEBUG_TEST(DB_I,"3");
//         IP_TO_ARRAY(m_static_dec.ip_info_num[0], m_static_dec.ip_info_num[1], m_static_dec.ip_info_num[2], m_static_dec.ip_info_num[3], &event->ip_info.ip);
//         IP_TO_ARRAY(m_static_dec.ip_info_netmask[0], m_static_dec.ip_info_netmask[1], m_static_dec.ip_info_netmask[2], m_static_dec.ip_info_netmask[3], &event->ip_info.netmask);
//         IP_TO_ARRAY(m_static_dec.ip_info_gw[0], m_static_dec.ip_info_gw[1], m_static_dec.ip_info_gw[2], m_static_dec.ip_info_gw[3], &event->ip_info.gw);
//         /* Adding hardcore tags indicates that the current ah wifi is available next time  */
//         rescue_message_wifi(STATIC_IP_IN_IPADDR, m_static_dec.ip_info_num, M_RESCUE_MESSAGE_SET);
//         rescue_message_wifi(STATIC_IP_IN_IPMASK, m_static_dec.ip_info_netmask, M_RESCUE_MESSAGE_SET);
//         rescue_message_wifi(STATIC_IP_IN_IPGW, m_static_dec.ip_info_gw, M_RESCUE_MESSAGE_SET);
//         DEBUG_TEST(DB_W," Save  ip: " IPSTR ", mask: " IPSTR ", gw: " IPSTR,
//                 IP2STR(&event->ip_info.ip),
//                 IP2STR(&event->ip_info.netmask),
//                 IP2STR(&event->ip_info.gw));
//         rescue_message_flag(STATIC_IP_IN_FLAG, M_RESCUE_MESSAGE_SET);
//     }
//     else if (m_static_dec.status == try_mode)
//     {
//         printf("Router DHCP Server Fail !! ");
//         // m_wifi_set_static_ip();
//         DEFAULT_IP_ADAPTER(m_static_dec.ip_info_num[0], m_static_dec.ip_info_num[1], m_static_dec.ip_info_num[2], m_static_dec.ip_info_num[3], m_static_dec.ip_info_host_name_num_back[0]);
//         DEFAULT_NK_ADAPTER(m_static_dec.ip_info_netmask[0], m_static_dec.ip_info_netmask[1], m_static_dec.ip_info_netmask[2], m_static_dec.ip_info_netmask[3]);
//         DEFAULT_GW_ADAPTER(m_static_dec.ip_info_gw[0], m_static_dec.ip_info_gw[1], m_static_dec.ip_info_gw[2], m_static_dec.ip_info_gw[3], m_static_dec.ip_info_host_name_num_back[0]);
//         // _wifi.wifi_dis_connect();
//         _wifi.wifi_need_auto_connect = false;
//         // esp_wifi_deinit();
//         m_wifi_set_try_ip();
//         _wifi.wifi_need_auto_connect = true;
//     }
// }

// void m_wifi_set_try_ip(void)
// {
//     // _wifi.wifi_init();
//     // _wifi.wifi_sta_init(STA_CONN.ssid, STA_CONN.psw);
//     // _wifi.wifi_need_auto_connect = true;
//     esp_netif_ip_info_t ip_info = {
//         .ip = {.addr = ESP_IP4TOADDR(m_static_dec.ip_info_num[0],
//                                      m_static_dec.ip_info_num[1],
//                                      m_static_dec.ip_info_num[2],
//                                      m_static_dec.ip_info_num[3])},
//         .gw = {.addr = ESP_IP4TOADDR(m_static_dec.ip_info_gw[0],
//                                      m_static_dec.ip_info_gw[1],
//                                      m_static_dec.ip_info_gw[2], 1)},
//         .netmask = {.addr = ESP_IP4TOADDR(m_static_dec.ip_info_netmask[0],
//                                           m_static_dec.ip_info_netmask[1],
//                                           m_static_dec.ip_info_netmask[2],
//                                           m_static_dec.ip_info_netmask[3])},
//     };
//     if (m_static_dec.netif != NULL)
//     {
//         esp_netif_dhcpc_stop(m_static_dec.netif);
//         ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_STA));

//         if (esp_netif_set_ip_info(m_static_dec.netif, &ip_info) != ESP_OK)
//         {
//             esp_netif_set_ip_info(m_static_dec.netif, NULL);
//             // esp_netif_dhcpc_start(m_static_dec.netif);
//         }
//     }
// }
// void m_wifi_unset_static_ip(void)
// {
//     // 清除配置信息
//     if (m_static_dec.ip_info_num[0] != 0 || m_static_dec.ip_info_netmask[0] != 0 || m_static_dec.ip_info_gw[0] != 0)
//     {
//         memset(m_static_dec.ip_info_num, 0, sizeof(m_static_dec.ip_info_num));
//         memset(m_static_dec.ip_info_netmask, 0, sizeof(m_static_dec.ip_info_netmask));
//         memset(m_static_dec.ip_info_gw, 0, sizeof(m_static_dec.ip_info_gw));
//         // m_wifi_set_static_ip();
//     }
//     ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_STA));
//     //  esp_netif_set_ip_info(m_static_dec.netif, NULL);
//     ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_STA));
//     rescue_message_flag(STATIC_IP_IN_FLAG, M_RESCUE_MESSAGE_DEL);
//     // 开启DHCP服务、
// }
// void m_wifi_set_static_ip(void)
// {

//     // 想使用静态ip但是没有静态IP
//     if (rescue_message_wifi(STATIC_IP_IN_IPADDR, m_static_dec.ip_info_num, M_RESCUE_MESSAGE_GET) != succ_m ||
//         rescue_message_wifi(STATIC_IP_IN_IPMASK, m_static_dec.ip_info_netmask, M_RESCUE_MESSAGE_GET) != succ_m ||
//         rescue_message_wifi(STATIC_IP_IN_IPGW, m_static_dec.ip_info_gw, M_RESCUE_MESSAGE_GET) != succ_m)
//     {
//         rescue_message_flag(STATIC_IP_IN_FLAG, M_RESCUE_MESSAGE_DEL);
//         ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_STA));
//         return;
//     }

//     if (m_static_dec.status == undef)
//         m_static_dec.status = static_mode;

//     esp_netif_ip_info_t ip_info = {
//         .ip = {.addr = ESP_IP4TOADDR(m_static_dec.ip_info_num[0],
//                                      m_static_dec.ip_info_num[1],
//                                      m_static_dec.ip_info_num[2],
//                                      m_static_dec.ip_info_num[3])},
//         .gw = {.addr = ESP_IP4TOADDR(m_static_dec.ip_info_gw[0],
//                                      m_static_dec.ip_info_gw[1],
//                                      m_static_dec.ip_info_gw[2], 1)},
//         .netmask = {.addr = ESP_IP4TOADDR(m_static_dec.ip_info_netmask[0],
//                                           m_static_dec.ip_info_netmask[1],
//                                           m_static_dec.ip_info_netmask[2],
//                                           m_static_dec.ip_info_netmask[3])},
//     };
//     if (m_static_dec.ramdom_offset >= 5 || m_static_dec.status == try_mode)
//     {
//         srand(_time.time_get_timestamp_t());

//         if (m_static_dec.ip_info_num[0] == 0)
//             m_static_dec.ramdom_offset = 5;
//         ip_info.ip.addr = ESP_IP4TOADDR(m_static_dec.ip_info_host_name_num_back[0],
//                                         m_static_dec.ip_info_host_name_num_back[1],
//                                         m_static_dec.ip_info_host_name_num_back[m_static_dec.ramdom_offset - 5],
//                                         rand() % 200 + 50 + m_static_dec.ramdom_offset);
//         ip_info.gw.addr = ESP_IP4TOADDR(m_static_dec.ip_info_gw[0],
//                                         m_static_dec.ip_info_gw[1],
//                                         m_static_dec.ip_info_host_name_num_back[m_static_dec.ramdom_offset - 5],
//                                         1);
//         DEBUG_E(" ip:" I2P "mask" I2P "gw" I2P, m_static_dec.ip_info_host_name_num_back[0],
//                 m_static_dec.ip_info_host_name_num_back[1],
//                 m_static_dec.ip_info_host_name_num_back[m_static_dec.ramdom_offset - 5],
//                 50 + m_static_dec.ramdom_offset,
//                 m_static_dec.ip_info_netmask[0],
//                 m_static_dec.ip_info_netmask[1],
//                 m_static_dec.ip_info_netmask[2],
//                 m_static_dec.ip_info_netmask[3],
//                 m_static_dec.ip_info_gw[0],
//                 m_static_dec.ip_info_gw[1],
//                 m_static_dec.ip_info_host_name_num_back[m_static_dec.ramdom_offset - 5],
//                 1);
//     }
//     // _wifi.wifi_need_auto_connect = true;
//     m_static_dec.ramdom_offset++;
//     if (m_static_dec.netif != NULL)
//     {
//         esp_netif_dhcpc_stop(m_static_dec.netif);
//         if (esp_netif_set_ip_info(m_static_dec.netif, &ip_info) != ESP_OK)
//         {
//             esp_netif_set_ip_info(m_static_dec.netif, NULL);
//             esp_netif_dhcpc_start(m_static_dec.netif);
//         }
//     }

//     DEBUG_TEST(DB_W,"USE static IP");
// }

// void m_net_global_adapter_init(esp_netif_t *netif)
// {

//     if (netif != NULL)
//     {
//         m_static_dec.netif = netif;
//         _wifi.m_static_dec = &m_static_dec;
//         m_wifi_set_static_ip();
//     }
// }
// void m_net_global_adapter_set_dns(uint32_t main_dns, uint32_t backup_dns)
// {
//     tcpip_adapter_dns_info_t dns_info = {0};
//     memset(&dns_info, 0, sizeof(dns_info));
//     // 首选
//     IP_ADDR4(&dns_info.ip,
//              m_static_dec.ip_info_gw[0],
//              m_static_dec.ip_info_gw[1],
//              m_static_dec.ip_info_gw[2], 1);
//     ESP_ERROR_CHECK(tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_MAIN, &dns_info));
//     // 备选
//     memset(&dns_info, 0, sizeof(dns_info));
//     IP_ADDR4(&dns_info.ip, 8, 8, 8, 8);
//     ESP_ERROR_CHECK(tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_BACKUP, &dns_info));
// }
// void m_net_global_adapter_disable_static_ip()
// {
//     return m_wifi_unset_static_ip();
// }