

#if 1
#include "../drive.h"
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_TCP

#if UNIT_TEST
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#endif

struct sockaddr_in dest_addr;
int sock__ = 0;
/**  */
bool server_is_connected = false;

// struct linger
// {
//     int l_onoff;  // 0 = off, nozero = on
//     int l_linger; // linger time
// };
/**
 * @brief
 *
 */
stat_m m_ext_software_drive_server_tcp_init(void)
{
    // struct sockaddr_in local_addr;
    // int addr_family = 0;
    // addr_family = AF_INET;
    // struct hostent *h = NULL;

    // memset(&local_addr, 0, sizeof(local_addr));
    // memset(&dest_addr, 0, sizeof(dest_addr));

    // sock__ = socket(addr_family, SOCK_STREAM, IPPROTO_IP);
    // lwip_bind(sock__, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in6));
    // if (sock__ <= 0)
    // {
    //     DEBUG_TEST( DB_I," m_ext_software_drive_server  ERR ");
    //     return fail_r;
    // }
    // else
    // {
    //     DEBUG_TEST( DB_I," m_ext_software_drive_server_tcp_init ");
    return succ_r;
    // }
}
struct linger linkz = {1, 0};
/**
 * @brief 连接 TCP 的软件驱动
 *
 * @param in_addr
 * @param in_port
 */
stat_m m_ext_software_drive_server_tcp_connect(char *in_addr, int in_port)
{
    // struct sockaddr_in local_addr;
    stat_m stat = fail_r;
    int addr_family = 0;
    addr_family = AF_INET;
    // struct hostent *h = NULL;

    // memset(&local_addr, 0, sizeof(local_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));

    DEBUG_TEST(DB_I, " m_ext_software_drive_server_tcp_connect (%s-%d) ", in_addr, in_port);
    dest_addr.sin_addr.s_addr = inet_addr(in_addr);
    dest_addr.sin_port = htons(in_port);
    dest_addr.sin_family = AF_INET;

    sock__ = socket(addr_family, SOCK_STREAM, IPPROTO_IP);
    stat = succ_r;
#if !UNIT_TEST
    if (sock__ <= 0)
    {
        stat = fail_r;
        DEBUG_E("Unable to create socket: errno %d", errno);
        m_callable_middle_err_handle_set_code(M_ERROR_CODE_NETWORK__CAUSE_SERVER_FAIL_CREATE_SOCKET_FAIL);
        m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_SOCTET_CREATE_FAIL);
        mDelay_ms(1500);
    }
// lwip_bind(sock__, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in6));
#endif

#if !UNIT_TEST
    setsockopt(sock__, SOL_SOCKET, SO_LINGER, (const char *)&linkz, sizeof(struct linger));
    if (stat == succ_r)
    {
        errno = connect(sock__, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
        int nagle_status = 1;
        // setsockopt(sock__, // socket的文件描述符
        //            IPPROTO_TCP,
        //            TCP_NODELAY,
        //            (char *)&nagle_status,
        //            sizeof(int)); // 1 - on, 0 - off
        DEBUG_TEST(DB_I, "%s , %d", esp_err_to_name(errno), errno);
        if (errno == ESP_OK)
        {
            server_is_connected = true;
            m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_SOCKET_CONNECT_SUCCESS);
            return succ_r;
        }
    }
    shutdown(sock__, SHUT_RDWR);
    close(sock__);
    DEBUG_TEST(DB_W, "Sock Free .. ");
    m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_SOCTET_CONNECT_FAIL);

#else
    if (connect(sock__, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
        return fail_r;
#endif
    server_is_connected = false;
    return fail_r;
}

/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
stat_m m_ext_net_link_receive_data(char *out_data, int *len, uint64_t in_time_out_s)
{
    stat_m stat = fail_r;
    memset(out_data, 0, 512);

    uint64_t in_soc_time = m_callable_timer_manage_get_utc_time();
    while (1)
    {
#if !UNIT_TEST
        if (in_time_out_s != M_TIMEOUT_FOREVER && m_callable_timer_manage_get_utc_time() > in_soc_time + in_time_out_s)
        {
            stat = fail_r;
            *len = -1;
            m_callable_middle_err_handle_set_code(M_ERROR_CODE_NETWORK_DISCONNECT_CAUSE_RECEIVE_RETURN_1);
            // m_ext_network_server_disconnect();
            DEBUG_E("in_time_out_s: %lld - utc %lld   insoc %lld", in_time_out_s, m_callable_timer_manage_get_utc_time(), in_soc_time);
            break;
        }
#endif
        if ((*len = recv(sock__, out_data, 512, 0)) > 0)
        {
            DEBUG_TEST(DB_W, "m_ext_net_link_tcp_receive_data -> %s", out_data);
            stat = succ_r;
            break;
        }
#if !UNIT_TEST
        if (!server_is_connected || *len == 0)
        {
            stat = fail_r;
            *len = -1;
            // m_ext_network_server_disconnect();
            break;
        }
#endif
        mDelay_ms(20);
    }

    return stat;
}
/**  mqtt接收的函数  */
stat_m m_ext_network_link_transformission_rx()
{
    return succ_r;
}
stat_m m_ext_network_link_transformission_tx(char *data, int data_len)
{
    send(sock__, data, data_len, 0); /// 发送
    DEBUG_TEST(DB_I, "Tcpp Send ... %s %d", data, data_len);

    return 0;
}
/**
 * @brief 主动断开tcp服务器连接
 *
 * @return stat_m
 */
stat_m m_ext_network_server_tcp_disconnect(void)
{
    close(sock__);
    // close(sock__);
    sock__ = 0;
    server_is_connected = false;
    DEBUG_TEST_E("m_ext_network_server_tcp_disconnect ...");
    return succ_r;
}

#endif
#endif