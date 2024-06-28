/**
 * @file http_requ.c
 * @author your name (you@domain.com)
 * @brief  实现HTTP稳定请求
 * @version 0.1
 * @date 2024-05-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "core/fset_lib.h"

#if !UNIT_TEST
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_http_client.h"
#include "esp_http_server.h"
#include "esp_https_ota.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#endif

/**
 * @brief 通过官方HTTP获取
 *
 * @param out_addr 地址
 * @param out_port 端口
 * @return stat_m
 */
stat_m m_ext_static_network_http_requ_addr(char *out_addr, int *out_port);
/**
 * @brief 通过SOCKET获取
 *
 * @param out_addr 地址
 * @param out_port 端口
 * @return stat_m
 */
stat_m m_ext_static_network_socket_requ_addr(char *out_addr, int *out_port);

/**
 * @brief Get the net and hostname object
 *
 * @param in_hostname 主机名
 * @param out_addr 输出地址
 * @return stat_m
 */
extern stat_m m_callable_drive_dns_get_net_and_hostname(unsigned char *in_hostname, void *out_addr);

static char hostname_addr[50] = {"device.imolaza.com"};
// #define SERVER_ADDR "device.imolaza.com"

// #define IpaddrS "http://device.imolaza.com/device/network"
static char NetWorkTemp[256] = {0};
static char ipaddrTemp[150] = {0};

#if !UNIT_TEST
esp_http_client_handle_t http_cli;
#endif

/** 不需要重复初始化*/
bool is_init = true;
/** 请求地址的时候，10次请求一次HTTP ，记下数 */
static int http_requ_count = 0;

#define WEB_PORT "80"

// static const char *REQUEST = "GET "
//                              "/device/network?version=8.5.100&mac=c4dee21f9e80"
//                              " HTTP/1.0\r\n"
//                              "Host: " SERVER_ADDR ":" WEB_PORT "\r\n"
//                              "User-Agent: esp-idf/1.0 esp32\r\n"
//                              "\r\n";

#if !UNIT_TEST
static void http_cleanup(esp_http_client_handle_t client)
{
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    DEBUG_E("http  free ( - ) !");
}
#endif

/**
 * @brief 初始哈u 请求地址
 *
 * @return stat_m
 */
stat_m m_ext_static_network_requ_addr_init(void)
{
    if (strstr(ipaddrTemp, "network?") == NULL)
    {
        memset(ipaddrTemp, 0, 100);
        // strcat(ipaddrTemp, "http://");
        // strcat(ipaddrTemp, SERVER_ADDR);
        // strcat(ipaddrTemp, "/device/network?version=");

        // m_callable_device_attribute_get_app_version(NetWorkTemp);
        // strcat(ipaddrTemp, NetWorkTemp);

        // // strcat(ipaddrTemp, "1.7.001");

        // m_callable_device_attribute_get_mac_addr(NetWorkTemp);
        // strcat(ipaddrTemp, "&mac=");
        // if ((NetWorkTemp[1] >= '0' && NetWorkTemp[1] <= '9') || (NetWorkTemp[1] >= 'a' && NetWorkTemp[1] <= 'z'))
        //     strcat(ipaddrTemp, NetWorkTemp);
        // else
        //     strcat(ipaddrTemp, "abcd00efgc");
        strcat(ipaddrTemp, "GET ");
        strcat(ipaddrTemp, "/device/network?version=");
        m_callable_device_attribute_get_app_version(NetWorkTemp);
        strcat(ipaddrTemp, NetWorkTemp);
        m_callable_device_attribute_get_mac_addr(NetWorkTemp);
        strcat(ipaddrTemp, "&mac=");
        if ((NetWorkTemp[1] >= '0' && NetWorkTemp[1] <= '9') || (NetWorkTemp[1] >= 'a' && NetWorkTemp[1] <= 'z'))
            strcat(ipaddrTemp, NetWorkTemp);
        else
            strcat(ipaddrTemp, "abcd00efgc");

        strcat(ipaddrTemp, " HTTP/1.0\r\nHost: ");

        strcat(ipaddrTemp, hostname_addr);
        strcat(ipaddrTemp, ":");
        strcat(ipaddrTemp, WEB_PORT);
        strcat(ipaddrTemp, "\r\n");
        strcat(ipaddrTemp, "User-Agent: Linux/99.99 Mwp--\r\n\r\n");

        // strcat(ipaddrTemp, "1.7.001");
    }
    return succ_r;
}

stat_m m_ext_static_http_res_head_any(char *in_src, char **out_str)
{
    stat_m stat = fail_r;
    /** 起始点记录 和 .计数 */
    int int_sc = 0, int_dot_count = 0;

    /** 如果是数字 王后推8个 ，必须包含2个以上. 就是目标*/
    for (int i = 0; i < 200; i++)
    {
        if (*in_src > 48 && *in_src <= 57)
        {
            int_sc = i;
            *out_str = in_src;
            // printf("+.  %s  .-" ,*out_str);
            for (; i < int_sc + 15; i++)
            {
                in_src++;
                if (*in_src == '.')
                    int_dot_count++;
                else if (*in_src > 57 || *in_src < 48)
                    break;
                // printf("%c(%d)       ", in_src[i] , i);
                if (int_dot_count >= 3)
                {
                    stat = succ_r;
                    break;
                }
            }
            int_dot_count = 0;
            if (stat == succ_r)
                break;
        }
        in_src++;
    }

    return stat;
}

/**
 * @brief 获取目标连接服务器地址
 *
 * @param out_addr
 * @param out_port
 * @return stat_m
 */
stat_m m_ext_network_get_target_addr(char *out_addr, int *out_port)
{
    stat_m stat = fail_r;
    // if (*out_port != 0 && http_requ_count < 1024 )
    // {
    //     http_requ_count++;
    //     stat = succ_r;
    //     DEBUG_TEST(DB_W,"-> Get Old (%s - %d)", out_addr, *out_port);
    // }
    // else
    {
        // stat = m_ext_static_network_http_requ_addr(         out_addr , out_port);
        stat = m_ext_static_network_socket_requ_addr(out_addr, out_port);
    }
    return stat;
}

/**
 * @brief 通过官方HTTP获取
 *
 * @param out_addr 地址
 * @param out_port 端口
 * @return stat_m
 */
stat_m m_ext_static_network_http_requ_addr(char *out_addr, int *out_port)
{
    stat_m stat = fail_r;
    int re_code = 3;
    DEBUG_TEST(DB_I, " m_ext_network_get_target_addr  %d", *out_port);

#if 1
    // if (is_init)
    // {
    // }
    m_ext_static_network_requ_addr_init();

    if (http_cli == NULL)
    {
        esp_http_client_config_t config = {
            .url = ipaddrTemp,
            .timeout_ms = 25 * 1000,
            .keep_alive_enable = true,
        };
        http_cli = esp_http_client_init(&config);
        DEBUG_TEST(DB_I, "NetWork Test URL :%s", config.url);
        is_init = false;
        // sprintf(ipaddrTemp, "http://device.imolaza.com/device/network?version=%s&mac=%s", _device.device_version, _device.mac);
    }
    esp_http_client_perform(http_cli);
    esp_http_client_open(http_cli, 0);
    DEBUG_TEST(DB_I, "Addr  :%s", hostname_addr);
    // esp_http_client_config_t config;
    // memset(config, 0, sizeof(config));
    // strcpy(config.url, ipaddrTemp);
    // config.timeout_ms = 20 * 1000;

    esp_http_client_fetch_headers(http_cli);
    while (re_code--)
    {
        if (esp_http_client_get_status_code(http_cli) == 200)
        {
            bzero(out_addr, sizeof(*out_addr));
            bzero(NetWorkTemp, sizeof(NetWorkTemp));
            esp_http_client_read(http_cli, NetWorkTemp, sizeof(NetWorkTemp));
            DEBUG_TEST(DB_I, "%s", NetWorkTemp);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(NetWorkTemp, "%[^:]:%d", out_addr, out_port);
            DEBUG_TEST(DB_W, "Success(%s:%d)\r\n", out_addr, *out_port);
            stat = succ_r;
            http_cleanup(http_cli);
            http_cli = NULL;
            http_requ_count = 1;
            // is_init = true;
            break;
        }
        else
        {
            DEBUG_E("http fail(%d)", re_code);
            // is_init = true;
        }
        mDelay_ms(500);
    }
    // http_cleanup(http_cli);
    //         http_cli = NULL;
    if (stat != succ_r)
        esp_http_client_close(http_cli);

#else
    stat = succ_r;
    strcpy(out_addr, "54.204.104.7");
    *out_port = 9511;
#endif
    return stat;
}

/** APT嘎ungbo地址缓存信息 */
struct addrinfo *res;
/** 地址 */
struct in_addr *addr;
/** IP地址信息 */
const struct addrinfo hints = {
    .ai_family = AF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_flags = O_NONBLOCK,
};
/** 超时时间设置 */
struct timeval receiving_timeout = {30, 0};

/** IP快速释放 */
struct linger linka = {1, 0};
int socket_ft = 0;
struct hostent *h = NULL;
extern void adsc(char *name);

struct sockaddr_in http_dest_addr;

/**
 * @brief 通过过SOCKET获取 地址信息
 *
 * @param out_addr 地址
 * @param out_port 端口
 * @return stat_m
 */
stat_m m_ext_static_network_socket_requ_addr(char *out_addr, int *out_port)
{
    stat_m stat = fail_r;
    int resp_code = 0;

    m_callable_device_attribute_http_proxy_get_addr(hostname_addr);
    m_ext_static_network_requ_addr_init();
    DEBUG_TEST(DB_I, "%s ... ", ipaddrTemp);

    m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_DNS_RESOLVEING);

    {
        // hints.ai_flags = O_NONBLOCK;
        DEBUG_TEST(DB_I, "m_ext_static_network_socket_requ_addr [ %s ]... ", hostname_addr);
        stat = m_callable_drive_dns_get_net_and_hostname((unsigned char *)hostname_addr, (void *)&http_dest_addr.sin_addr.s_addr);
        DEBUG_TEST(DB_I, "result ... [%s] ", hostname_addr);
        mDelay_ms(100);

        if (stat == fail_r)
        {
            DEBUG_TEST_E("DNS lookup failed err=%d\n", stat);
            m_callable_middle_err_handle_set_code(M_ERROR_CODE_NETWORK__CAUSE_HOSTNAME_FAIL);
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_DNS_RESOLVE_FAIL);
            // m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE__WIFI_NOT_NAT);
            mDelay_ms(1000);
        }
        else
        {
            stat = succ_r;
        }
    }
    /** 有网络 直接连接 */
    if (stat == succ_r)
    {
        stat = succ_r;
        if (*out_port != 0 && http_requ_count < 1024)
        {
            http_requ_count++;
            DEBUG_TEST(DB_W, "-> Get Old (%s - %d)", out_addr, *out_port);
            return stat;
        }
    }
    DEBUG_TEST(DB_I, "http_requ_count : %d OutPort : %d", http_requ_count, *out_port);
    /* Code to print the resolved IP.

       Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */

    if (stat == succ_r)
    {
        http_dest_addr.sin_port = htons(80);
        http_dest_addr.sin_family = AF_INET;
        // addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        DEBUG_TEST(DB_I, "Http Requ. IP=%s :80 ", inet_ntoa(http_dest_addr.sin_addr.s_addr));

        socket_ft = socket(AF_INET, SOCK_STREAM, 0);

        if (socket_ft < 0)
        {
            DEBUG_TEST_E("... Failed to allocate socket.\n");
            m_callable_middle_err_handle_set_code(M_ERROR_CODE_NETWORK__CAUSE_HTTP_FAIL_CREATE_SOCKET_FAIL);
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_HTTP_SOCKET_CREATE_FAIL);
            mDelay_ms(1000);
            stat = fail_r;
        }
    }
    if (stat == succ_r)
    {
        setsockopt(socket_ft, SOL_SOCKET, SO_LINGER, (const char *)&linka, sizeof(struct linger));
        DEBUG_TEST(DB_I, "... set socket receiving timeout success");
        if (connect(socket_ft, (struct sockaddr *)&http_dest_addr, sizeof(struct sockaddr_in)) < 0)
        {
            DEBUG_TEST_E("... socket connect failed errno=%d", errno);
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_HTTP_CONNECT_FAIL);
            stat = fail_r;
        }
    }

    if (stat == succ_r && write(socket_ft, ipaddrTemp, strlen(ipaddrTemp)) < 0)
    {
        DEBUG_TEST_E("... socket send failed");
        stat = fail_r;
    }
    if (stat == succ_r)
    {
        receiving_timeout.tv_sec = 30;
        if (setsockopt(socket_ft, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                       sizeof(receiving_timeout)) < 0)
        {
            DEBUG_TEST(DB_I, "... failed to set socket receiving timeout");
            stat = fail_r;
        }

        m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
        int r = recv(socket_ft, NetWorkTemp, sizeof(NetWorkTemp) - 1, 0);

        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(NetWorkTemp, "%*[^ ] %d", &resp_code);
        DEBUG_TEST(DB_I, "... done (- %s -)reading from socket. Last read return=%d errno=%d.", NetWorkTemp, r, errno);
        if (resp_code == 200 && r > 0)
        {
            // r = recv(socket_ft, NetWorkTemp, sizeof(NetWorkTemp) - 1  , 0);
            char *adrc = NetWorkTemp + 150;
            m_ext_static_http_res_head_any(NetWorkTemp, &adrc);
            DEBUG_TEST(DB_I, "Resu -> : %s", adrc);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(adrc, "%[^:]:%d", out_addr, out_port);
            DEBUG_TEST(DB_W, "Success(%s:%d)\r\n", out_addr, *out_port);
            m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_HTTP_CONNECT_SUCCESS);
            // m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_SERVER_LOGIN_LOGGING);

            stat = succ_r;
        }
        else
        {
            stat = fail_r;
            DEBUG_TEST_E("Http resp_code: %d  ", resp_code);
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_HTTP_RESP_PARSE_FAIL);
        }

        m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);

        DEBUG_TEST(DB_I, "Starting again!");
    }
    close(socket_ft);

    return stat;
}
