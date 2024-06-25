#include "../drive.h"

#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_WOLFSSL
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
#else
#include <wolfssl/wolfcrypt/settings.h>
#include "wolfssl/ssl.h"
#endif

struct sockaddr_in dest_addr;
int sock_fd = 0;
/** 连接信号 */
int wolfssl_is_connected = false;
/** ctx */
WOLFSSL_CTX *ctx = NULL;
/** ssl */
WOLFSSL *ssl = NULL;
/**  */
WOLFSSL_SESSION * ssl_session = NULL;
/** Cer */
extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
extern const uint8_t server_cert_pem_start[] asm("_binary_ca_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_crt_end");

/**
 * @brief
 *
 */
stat_m m_ext_software_drive_server_wolfSSL_init(void)
{
    DEBUG_TEST( DB_I,"Init wolfSSL...");

    if (wolfSSL_Init() != WOLFSSL_SUCCESS)
    {
        DEBUG_TEST(DB_W,"Init wolfSSL failed ...");
    }
    ctx = (void *)wolfSSL_CTX_new(wolfTLSv1_2_client_method());
    // ctx = (void *)wolfSSL_CTX_new(wolfTLSv1_2_client_method());

    wolfSSL_CTX_set_cipher_list(ctx, "AES128-SHA");

    if (!ctx)
    {
        DEBUG_TEST(DB_W,"Set wolfSSL ctx failed...");
    }

    wolfSSL_CTX_use_certificate_buffer(ctx, client_cert_pem_start, client_cert_pem_end - client_cert_pem_start, WOLFSSL_FILETYPE_PEM);
    wolfSSL_CTX_use_PrivateKey_buffer(ctx, client_key_pem_start, client_key_pem_end - client_key_pem_start, WOLFSSL_FILETYPE_PEM);
    wolfSSL_CTX_load_verify_buffer(ctx, server_cert_pem_start, server_cert_pem_end - server_cert_pem_start, WOLFSSL_FILETYPE_PEM);

    wolfSSL_CTX_set_verify(ctx, WOLFSSL_VERIFY_PEER, NULL);

    /** Create SSL */

    return succ_r;
}
bool ahpp = false;
struct linger linkb = {1, 0};
/**
 * @brief 连接 WolfSSL 的软件驱动
 *
 * @param in_addr
 * @param in_port
 */
stat_m m_ext_software_drive_server_wolfSSL_connect(char *in_addr, int in_port)
{
    stat_m stat = fail_r;
    /** 本地地址 */
    // struct sockaddr_in local_addr;
    int addr_family = 0;
    addr_family = AF_INET;
    // struct hostent *h = NULL;
    m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
    // memset(&local_addr, 0, sizeof(local_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));
    DEBUG_TEST( DB_I," m_ext_software_drive_server_tcp_connect (%s-%d) ", in_addr, in_port);
    dest_addr.sin_addr.s_addr = inet_addr(in_addr);
    dest_addr.sin_port = htons(in_port);
    dest_addr.sin_family = AF_INET;

    m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
    sock_fd = socket(addr_family, SOCK_STREAM, IPPROTO_IP);
    // fcntl(sock_fd, F_SETFL, O_NONBLOCK);
        setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, (const char *)&linkb, sizeof(struct linger));
    ssl = wolfSSL_new(ctx);
    if (!ssl)
    {
        DEBUG_TEST(DB_W,"Create wolfSSL failed...");
    }
    // // 将套接字的接收操作设置为非阻塞模式
    // int flags = fcntl(sock_fd, F_GETFL, 0);
    // if (flags == -1) {
    //     perror("Failed to get socket flags");
    //     exit(EXIT_FAILURE);
    // }
    // if (fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    //     perror("Failed to set socket receive to non-blocking");
    //     exit(EXIT_FAILURE);
    // }
    if(ahpp)
    {
            // wolfSSL_set_session(ssl,ssl_session);
            // wolfSSL_restore_session_cache("A");
    } 

        wolfSSL_set_fd(ssl, sock_fd);
    // fcntl(sock_fd,F_SETFL,SOCK_NONBLOCK);
    // sock_fd = socket(AF_INET, SOCK_STREAM, 0);
#if !UNIT_TEST
    if (sock_fd < 0)
    {
        DEBUG_E("Unable to create socket: errno %d", errno);
    }

    // lwip_bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in6));
#endif

    DEBUG_TEST( DB_I," ...");
#if !UNIT_TEST
    errno = connect(sock_fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    // DEBUG_TEST( DB_I,"%s , %d", esp_err_to_name(errno), errno);
    m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
    if (errno == ESP_OK)
    {
        DEBUG_TEST( DB_I,"Performing the SSL/TLS handshake...");
        if (WOLFSSL_SUCCESS != wolfSSL_connect(ssl))
        {
            DEBUG_TEST( DB_I,"Performing the SSL/TLS handshake failed: ... ");
        }
        else
        {
            stat = succ_r;
            wolfssl_is_connected = true;
            // ssl_session = wolfSSL_get1_session(ssl);
            // wolfSSL_save_session_cache("A");
            ahpp = true;
        }
    }
    m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
#else
    if (connect(sock_fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
        return fail_r;
#endif
    return stat;
}

/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_wolfSSL_rx(char *out_data, int *len, int in_time_out_s)
{
    // DEBUG_TEST( DB_I,"Tcpp Send ... 000000000 ");?..
    memset(out_data, 0, 1024);
    // int n = 1;
    // m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
    // if (ssl != NULL && (*len = wolfSSL_recv(ssl, out_data, 1024, 0)) >= 0) // M_TIMEOUT_35_SEC
    // // if (ssl != NULL && (*len = wolfSSL_recv(ssl, out_data, 1024, 1)) >= 0) // M_TIMEOUT_35_SEC
    // // if (ssl != NULL && (*len = wolfSSL_read(ssl, out_data, 1024)) >= 0) // M_TIMEOUT_35_SEC
    // {
    //     // printf("%s", out_data);
    //     return succ_r;
    // }
    // else
    // {
    //     // wolfSSL_clear(ssl);
    //     n = wolfSSL_get_error(ssl, 0);
    //     DEBUG_TEST(DB_W,"Be Remote Close .. %d  -  (%d)%s", *len, n, wolfSSL_ERR_error_string(n, NULL));
    //     return fail_r;
    // }

    stat_m stat = fail_r;
    uint64_t in_soc_time = m_callable_timer_manage_get_utc_time();
    while (1)
    {
#if !UNIT_TEST
        if (in_time_out_s != M_TIMEOUT_FOREVER && m_callable_timer_manage_get_utc_time() > in_soc_time + in_time_out_s)
        {
            stat = fail_r;
            *len = -1;
            // m_ext_network_server_disconnect();
            DEBUG_E("in_time_out_s: %d - utc %lld   insoc %lld", in_time_out_s, m_callable_timer_manage_get_utc_time(), in_soc_time);
            break;
        }
#endif
        if (ssl != NULL && (*len = wolfSSL_recv(ssl, out_data, 1024, 0)) >= 0)
        {
            DEBUG_TEST(DB_W,"m_ext_net_link_mqtt_receive_data -> %s", out_data);
            stat = succ_r;
            break;
        }
#if !UNIT_TEST
        if (!wolfssl_is_connected)
        {
            stat = fail_r;
            *len = -1;
            // m_ext_network_server_disconnect();
            break;
        }
#endif
        mDelay_ms(20);
    }

    m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
    return stat;
}

/**
 * @brief WolfSSL 发送数据
 *
 * @param data 数据
 * @param data_len 数据长度
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_wolfSSL_tx(char *data, int data_len)
{
    if (ssl != NULL)
    {
        // wolfSSL_send(ssl, data, data_len, 0);
        wolfSSL_write(ssl, data, data_len);
        // wolfSSL_write(ssl, "[11,4941cc5a02034686063cbb914cbec10a,8.5.100,0,0,1,HCTJ12_m,-56]", 67);
        DEBUG_TEST( DB_I,"WolfSSL Send ... %s", data);
    }
    else
        DEBUG_TEST( DB_I,"Send ... SSL NULL");
    return 0;
}
/**
 * @brief 主动断开 SSL 服务器连接
 *
 * @return stat_m
 */
stat_m m_ext_network_server_wolfSSL_disconnect(void)
{
    DEBUG_TEST_E("m_ext_network_server_wolfSSL_disconnect .... ");
    wolfssl_is_connected = false;
    close(sock_fd);
    CloseSocket(sock_fd);
    wolfSSL_clear(ssl);
    wolfSSL_shutdown(ssl);
    wolfSSL_free(ssl);
    ssl = NULL;
    mDelay_ms(3000);
    // wolfSSL_Cleanup();
// ssl->session
    // wolfSSL_clear(ssl);
    sock_fd = 0;
    return succ_r;
}
/**
 * @brief 设置超时
 *
 * @param time_out 超时时间
 * @return stat_m
 */
stat_m m_ext_network_server_wolfSSL_set_timeout(int time_out)
{
    stat_m stat = fail_r;
    if (time_out == M_TIMEOUT_35_SEC)
    {
        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = time_out;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                       sizeof(receiving_timeout)) < 0)
        {
            DEBUG_TEST_E("TimeOut set Fail !");
        }
    }
    else
        setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, NULL, 0);
    return stat;
}
#endif