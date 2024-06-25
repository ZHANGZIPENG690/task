#include "../drive.h"

#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MBEDTLS
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
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/esp_debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "esp_crt_bundle.h"
#endif

/** 连接信号 */
int mbedtls_is_connected = false;

/** Cer */
extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
extern const uint8_t server_cert_pem_start[] asm("_binary_ca_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_crt_end");

#define WEB_SERVER "www.howsmyssl.com"
#define WEB_PORT "443"
#define WEB_URL "https://www.howsmyssl.com/a/check"

static const char *TAG = "example";

static const char *REQUEST = "GET " WEB_URL " HTTP/1.0\r\n"
                             "Host: " WEB_SERVER "\r\n"
                             "User-Agent: esp-idf/1.0 esp32\r\n"
                             "\r\n";

/**
 * @brief
 *
 */
mbedtls_ssl_context ssl;
mbedtls_ssl_config conf;
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_x509_crt cacert;
stat_m m_ext_software_drive_server_mbedtls_init(void)
{
    DEBUG_TEST( DB_I,"Init MbedTLS...");

    int ret = 0;
    // memset(&ssl, 0, sizeof(ssl));
    // mbedtls_ssl_init(&ssl);

    DEBUG_TEST( DB_I,"Performing the SSL/TLS handshake...");
    return succ_r;
}
// bool ahpp = false;
/**
 * @brief 连接 WolfSSL 的软件驱动
 *
 * @param in_addr
 * @param in_port
 */
mbedtls_net_context server_fd;
bool adbbbbb = true;
char buf[512];
stat_m m_ext_software_drive_server_mbedtls_connect(char *in_addr, int in_port)
{
    stat_m stat = fail_r;
    int ret = 0, flags = 0;
    if (adbbbbb)
    {
        DEBUG_TEST( DB_I,"Seeding the random number generator");
        mbedtls_ssl_init(&ssl);
        mbedtls_x509_crt_init(&cacert);
        mbedtls_ctr_drbg_init(&ctr_drbg);
        mbedtls_ssl_config_init(&conf);

        mbedtls_entropy_init(&entropy);
        if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                         NULL, 0)) != 0)
        {
            ESP_LOGE(TAG, "mbedtls_ctr_drbg_seed returned %d", ret);
            abort();
        }

        DEBUG_TEST( DB_I,"Attaching the certificate bundle...");

        ret = esp_crt_bundle_attach(&conf);

        if (ret < 0)
        {
            ESP_LOGE(TAG, "esp_crt_bundle_attach returned -0x%x\n\n", -ret);
            abort();
        }

        DEBUG_TEST( DB_I,"Setting hostname for TLS session...");

        /* Hostname set here should match CN in server certificate */
        if ((ret = mbedtls_ssl_set_hostname(&ssl, WEB_SERVER)) != 0)
        {
            ESP_LOGE(TAG, "mbedtls_ssl_set_hostname returned -0x%x", -ret);
            abort();
        }

        DEBUG_TEST( DB_I,"Setting up the SSL/TLS structure...");

        if ((ret = mbedtls_ssl_config_defaults(&conf,
                                               MBEDTLS_SSL_IS_CLIENT,
                                               MBEDTLS_SSL_TRANSPORT_STREAM,
                                               MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
        {
            ESP_LOGE(TAG, "mbedtls_ssl_config_defaults returned %d", ret);
            // goto exit;
        }
        mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
        mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
        mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
        if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0)
        {
            ESP_LOGE(TAG, "mbedtls_ssl_setup returned -0x%x\n\n", -ret);
            // goto exit;
        }
    }
    mbedtls_net_init(&server_fd);

    DEBUG_TEST( DB_I,"Connecting to %s:%s...", WEB_SERVER, WEB_PORT);

    if ((ret = mbedtls_net_connect(&server_fd, WEB_SERVER,
                                   WEB_PORT, MBEDTLS_NET_PROTO_TCP)) != 0)
    {
        ESP_LOGE(TAG, "mbedtls_net_connect returned -%x", -ret);
    }
    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    DEBUG_TEST( DB_I,"Connected.");

    DEBUG_TEST( DB_I,"Performing the SSL/TLS handshake...");
    stat = succ_r;
    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            ESP_LOGE(TAG, "mbedtls_ssl_handshake returned -0x%x", -ret);
            stat = fail_r;
            break;
        }
        mDelay_ms(100);
    }

    DEBUG_TEST( DB_I,"Verifying peer X.509 certificate...");

    if (stat == succ_r)
    {
        if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0)
        {
            /* In real life, we probably want to close connection if ret != 0 */
            ESP_LOGW(TAG, "Failed to verify peer certificate!");
            bzero(buf, sizeof(buf));
            mbedtls_x509_crt_verify_info(buf, sizeof(buf), "  ! ", flags);
            ESP_LOGW(TAG, "verification info: %s", buf);
        }
        else
        {
            DEBUG_TEST( DB_I,"Certificate verified.");
        }

        stat = succ_r;
        mbedtls_is_connected = true;
    }
    m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
    adbbbbb = false;
    return stat;
}

/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_mbedtls_rx(char *out_data, int *len, int in_time_out_s)
{
    // DEBUG_TEST( DB_I,"Tcpp Send ... 000000000 ");?..
    stat_m stat = fail_r;
    uint64_t in_soc_time = m_callable_timer_manage_get_utc_time();
    memset(out_data, 0, 1024);

    while (1)
    {
        // mDelay_ms(20);
        // continue;
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
        if ((*len = mbedtls_ssl_read(&ssl, (unsigned char *)out_data, 1024)) >= 0)
        {
            DEBUG_TEST(DB_W,"m_ext_net_link_mqtt_receive_data -> %s", out_data);
            stat = succ_r;
            break;
        }
        printf("------");
#if !UNIT_TEST
        if (!mbedtls_is_connected)
        {
            stat = fail_r;
            *len = -1;
            DEBUG_TEST_E("Be Close-> %s", out_data);
            mbedtls_ssl_session_reset(&ssl);

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
stat_m m_ext_network_link_transformission_mbedtls_tx(char *data, int data_len)
{
    // if (ssl != NULL)
    {
        mbedtls_ssl_write(&ssl,
                          (const unsigned char *)REQUEST ,
                          strlen(REQUEST) );
        DEBUG_TEST( DB_I,"WolfSSL Send ... %s", data);
    }
    // else
    //     DEBUG_TEST( DB_I,"Send ... SSL NULL");
    return 0;
}
/**
 * @brief 主动断开 SSL 服务器连接
 *
 * @return stat_m
 */
stat_m m_ext_network_server_mbedtls_disconnect(void)
{
    DEBUG_TEST_E("m_ext_network_server_mbedTLS_disconnect .... ");
    // close(sock_fd);
    // CloseSocket(sock_fd);
    // // ssl = NULL;
    mbedtls_is_connected = false;
    mDelay_ms(1000);
    mbedtls_ssl_close_notify(&ssl);
    // mbedtls_ssl_session_reset(&ssl);

    // mDelay_ms(1000);

    mbedtls_net_free(&server_fd);
    mDelay_ms(1000);
    mDelay_ms(1000);
    mDelay_ms(1000);
    // sock_fd = 0;
    return succ_r;
}
/**
 * @brief 设置超时
 *
 * @param time_out 超时时间
 * @return stat_m
 */
stat_m m_ext_network_server_mbedtls_set_timeout(int time_out)
{
    stat_m stat = fail_r;

    return stat;
}

#endif