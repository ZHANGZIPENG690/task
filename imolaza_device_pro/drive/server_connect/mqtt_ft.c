
#include "../drive.h"
struct receive_data re_data = {{0}, 0, false};
#ifdef M_GLOBAL_NETWORK_CONNECT_PRO_SELECT_MQTT

#define Topic "imolaza"      // OneNet平台鉴权信息
#define PRODUCT_ID "Linux"   // OneNet平台产品ID
#define AUTH_INFO "OpenSUSE" // OneNet平台鉴权信息

// #define PRODUCT_ID "imolaza" // OneNet平台产品ID
// #define AUTH_INFO "87654321"    // OneNet平台鉴权信息

// char data2;
// int data_len2;
static bool is_connected = false;
static bool is_conne_ecp = true;

char in_cliect_id[40] = {0};

#if !UNIT_TEST
#include "mqtt_client.h"



extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
extern const uint8_t server_cert_pem_start[] asm("_binary_ca_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_crt_end");

esp_mqtt_client_handle_t mqttclient;
// esp_mqtt_client_handle_t tclient;
/** 配置 */
esp_mqtt_client_config_t mqtt_cfg = {0};
/** 登陆数据整理 */
char my_token_get[240] = {0};
// extern int ota_updata_flagm;
static esp_err_t mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    // DEBUG_TEST( DB_I,"Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    DEBUG_TEST( DB_I,"Event dispatched from event loop , event_id=%d", event_id);
    esp_mqtt_event_handle_t event = event_data;
    if (event == NULL || event_id > 2000)
        return 1;
    // tclient = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_BEFORE_CONNECT: // MQTT连上事件
        // m_ext_network_config_user_login_cmd(my_token_get, 0);
        //     mqtt_cfg.password = my_token_get;

        //     DEBUG_TEST( DB_I,"MQTT_EVENT_BEFORE_CONNECT %s", mqtt_cfg.password);

        // esp_mqtt_set_config( mqttclient, &mqtt_cfg);

        break;

    case MQTT_EVENT_CONNECTED: // MQTT连上事件
        DEBUG_TEST( DB_I,"MQTT_EVENT_CONNECTED");

        is_connected = true;
        esp_mqtt_client_subscribe(mqttclient, "imolaza_1", 2);
        is_conne_ecp = false;
        m_callable_system_printf_heap_free_info(__FUNCTION__,__LINE__);
        // ota_updata_flagm = 1;
        // m_callable_network_set_server_auto_connect
        // xEventGroupSetBits(mqtt_event_group, CONNECTED_BIT);
        // 发布主题
        //  printf("Connecting to MQTT broker OK");
        //  msg_id = esp_mqtt_client_publish(client, "/topic/maodou", "data_3", 6, 1, 0);
        //  DEBUG_TEST( "sent publish successful, msg_id=%d", msg_id);
        // 发送订阅
        // msg_id = esp_mqtt_client_subscribe(client, "maodou", 2);
        // DEBUG_TEST( "sent subscribe successful, msg_id=%d", msg_id);
        // //取消订阅
        // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        // DEBUG_TEST( "sent unsubscribe successful, msg_id=%d", msg_id);

        break;
    case MQTT_EVENT_DISCONNECTED: // MQTT断开连接事件
        DEBUG_TEST( DB_I,"MQTT_EVENT_DISCONNECTED");
        is_connected = false;
        // mqtt连上事件
        // xEventGroupClearBits(mqtt_event_group, CONNECTED_BIT);
        // esp_err_t err = esp_mqtt_client_reconnect(tclient);
        m_callable_system_printf_heap_free_info(__FUNCTION__,__LINE__);
        // if (err != ESP_OK)
        // {
        //     DEBUG_TEST( "Failed to reconnect to MQTT server");
        // }
        break;

    case MQTT_EVENT_SUBSCRIBED: // MQTT发送订阅事件
        // DEBUG_TEST( "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        // DEBUG_TEST( "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED: // MQTT取消订阅事件
        DEBUG_TEST( DB_I,"MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED: // MQTT发布事件
        DEBUG_TEST( DB_I,"MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA: // MQTT接受数据事件
        // DEBUG_TEST( DB_I,"MQTT_EVENT_DATA");
        // printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        // if (strstr(event->data, "ping") != NULL)
        // {
        //     DEBUG_TEST(DB_W,"Reping");
        //     break;
        // }

        // m_callable_network_data_rx(event->data, event->data_len);
        if (!re_data.is_receive)
        {
            memset(re_data.inout_data, 0, sizeof(re_data.inout_data));
            strncpy(re_data.inout_data, event->data, event->data_len);
            re_data.inout_data_len = event->data_len;

            re_data.is_receive = true;
            memset(event->data, 0, event->data_len);
            // stat_m_callable_network_manage_data_rx(event->data, event->data_len);
            DEBUG_TEST( DB_I,"DATA=%d  %s", event->data_len, re_data.inout_data);
        }
        // mDelay_ms(150);
        break;
    case MQTT_EVENT_ERROR: // MQTT错误事件
        DEBUG_TEST( DB_I,"MQTT_EVENT_ERROR");
        // xEventGroupClearBits(mqtt_event_group, CONNECTED_BIT);
        // if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {

        // DEBUG_TEST( DB_I,"Last errno string (%s)", strerror(event->error_handle->esp_tls_last_esp_err));
        // DEBUG_TEST( DB_I,"Last errno string (%s)", strerror(event->error_handle->esp_tls_stack_err));
        // DEBUG_TEST( DB_I,"Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        //}
        break;
    default:
        DEBUG_TEST( DB_I,"Other event id:%d", event_id);
        break;
    }
    return 0;
}
#endif
/**
 * @brief mqtt 服务器连接
 *
 */
void m_ext_software_drive_server_mqtt_init(void)
{
    // esp_mqtt_client_config_t mqtt_cfg = {
    //     .host = "m7d98997.ala.cn-hangzhou.emqxsl.cn",
    //     .port = 8084,
    //     .client_id = "emqx_cloud8a2d55",
    //     .disable_auto_reconnect = false,
    //     .keepalive = 10,
    //     .transport = MQTT_TRANSPORT_OVER_TCP,
    //     .out_buffer_size = 200,
    //     .buffer_size = 2048,
    //     .event_handle = mqtt_event_handler,
    //     .username = PRODUCT_ID, // OneNet平台产品ID和设备名称
    //     .password = AUTH_INFO,  // OneNet平台鉴权信息
    // };

    // // 通过esp_mqtt_client_init获取一个MQTT客户端结构体指针，参数是MQTT客户端配置结构体
    // mqttclient = esp_mqtt_client_init(&mqtt_cfg);
    // // 注册MQTT事件
    // esp_mqtt_client_register_event(mqttclient, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    // 开启MQTT功能

}
int loop_checkout_connect = 100;
// extern const char *M_FLASH_KEY_DEVICE_EN_KEY;

stat_m m_ext_software_drive_server_mqtt_connect(char *username, int port)
{
    stat_m stat = fail_r;
    loop_checkout_connect = 150;
    // stat = m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_EN_KEY, my_key_get);

#if !UNIT_TEST
    m_ext_network_config_user_login_cmd(my_token_get, 0);

    DEBUG_TEST(DB_W,"Mqtt  %s(%d) ...", username, port);
    mqtt_cfg.host = username;
    mqtt_cfg.port = port;
    mqtt_cfg.password = my_token_get; // OneNet平台鉴权信息

       if (is_conne_ecp)
    //    if (true)
    {
        DEBUG_TEST(DB_W,"Mqtt  Init ...");
        mqtt_cfg.username = "imolaza"; // OneNet平台产品ID和设备名称
        is_conne_ecp = false;

        m_callable_device_attribute_get_mac_addr(in_cliect_id);
        mqtt_cfg.client_id = in_cliect_id;

        mqtt_cfg.event_handle = mqtt_event_handler,


        mqtt_cfg.client_cert_pem = (const char *)client_cert_pem_start; // client.crt
        mqtt_cfg.client_key_pem = (const char *)client_key_pem_start;   // client.key
        mqtt_cfg.cert_pem = (const char *)server_cert_pem_start;        // ca.crt

        mqtt_cfg.transport = MQTT_TRANSPORT_OVER_SSL;                   // 必须显式指明用ssl,
        mqtt_cfg.skip_cert_common_name_check = true;                    // 不要校验服务器的域名

        mqtt_cfg.disable_auto_reconnect = true;
        mqtt_cfg.keepalive = 70;
        mqtt_cfg.out_buffer_size = 200;
        mqtt_cfg.buffer_size = 2048;

        // // 通过esp_mqtt_client_init获取一个MQTT客户端结构体指针，参数是MQTT客户端配置结构体
        mqttclient = esp_mqtt_client_init(&mqtt_cfg);
        // // 注册MQTT事件
        esp_mqtt_client_register_event(mqttclient, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
        // esp_mqtt_set_config(mqttclient , &mqtt_cfg);
        is_connected = false;
        // esp_mqtt_set_config(mqttclient, &mqtt_cfg);

        esp_mqtt_client_start(mqttclient);
    }
    else
    {
        // set_if_config(my_token_get, &mqttclient.connect_info.password);
        // mqttclient->config->clientkey_password = my_token_get;
       esp_set_config_psw(mqttclient, username, port, my_token_get);
        // mqtt_cfg.password = "mjmm"; // OneNet平台鉴权信息
        // esp_err_t er = esp_mqtt_set_config(mqttclient, &mqtt_cfg);
        // mqttclient.
        // printf("%s  ,  %s  %s", mqtt_cfg.username, mqtt_cfg.password, esp_err_to_name(er));
        // esp_mqtt_client_stop(mqttclient);
        // // esp_mqtt_client_disconnect(mqttclient);
        if (esp_mqtt_client_start(mqttclient) == ESP_OK)
        {
            // DEBUG_TEST( DB_I,"%s - %d",esp_err_to_name( esp_mqtt_client_start(mqttclient)),loop_checkout_connect);
            // mDelay_ms(100);
            is_connected = false;
        }
        // esp_mqtt_client_reconnect(mqttclient);
    }
    while (loop_checkout_connect--)
    {
        mDelay_ms(200);
        if (is_connected)
        {
            stat = succ_r;
            break;
        }
    }
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
stat_m m_ext_net_link_mqtt_receive_data(char *out_data, int *len, uint64_t in_time_out_s)
{
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
            DEBUG_E("in_time_out_s: %lld - utc %lld   insoc %lld", in_time_out_s, m_callable_timer_manage_get_utc_time(), in_soc_time);
            break;
        }
#endif
        if (re_data.is_receive)
        {
            strcpy(out_data, re_data.inout_data);
            *len = re_data.inout_data_len;
            re_data.is_receive = false;
            DEBUG_TEST(DB_W,"m_ext_net_link_mqtt_receive_data -> %s", out_data);
            stat = succ_r;
            break;
        }
#if !UNIT_TEST
        if (!is_connected)
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

/**
 * @brief data send
 *
 * @param in_tx data
 * @param in_len length
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_mqtt_tx(const char *in_tx, int in_len)
{
#if !UNIT_TEST
    if (mqttclient != NULL && in_tx != NULL && in_len > 0)
    {
        DEBUG_TEST( DB_I,"Mq ... " );
        esp_mqtt_client_publish(mqttclient, Topic, in_tx, in_len, 0, 0);
    }else {
        DEBUG_TEST(DB_W,"Mqtt Send Data Err ! / ( Client/TxData is Null )[%d]" ,in_len);
    }
#else
    DEBUG_TEST( DB_I,"Mqtt Send ....  .... ");
#endif
    return 0;
}
/**
 * @brief 主动断开服务器连接
 *
 * @return stat_m
 */
stat_m m_ext_network_server_disconnect(void)
{
    stat_m stat = succ_r;
#if !UNIT_TEST
    if (mqttclient == NULL)
        stat = fail_r;

    if (stat == succ_r)
    {
        esp_mqtt_client_disconnect(mqttclient) == ESP_OK ? (stat = succ_r) : (stat = fail_r);
        esp_mqtt_client_stop(mqttclient);
        is_connected = false;
        // esp_mqtt_client_destroy(mqttclient);
        // mqttclient = NULL;
    }
#endif
    return stat;
}
#endif