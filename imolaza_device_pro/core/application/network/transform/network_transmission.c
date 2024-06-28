
// #include "hot_so_fast.h"
#include "core/fset_lib.h"
#include "network_transmission.h"
#include "../network.h"
#include "../manage/network_manage.h"

#define M_SERVICE_RESP_TIME_OUT 5
/** 记录设备与服务器的超时时间  */
static uint32_t service_resp_time_out = 0;
/** 发现设备一直没有初始化  这个记录初始化超时时间  */
static uint32_t device_init_check_time_out = 0;

// static char cmd_buf[RECE_BUF_SIZE] = {0};

// static char Server_Address_Buffer[25] = {0};

static queue_root rx_qroot = {"Recv", NULL, NULL, 0};

static queue_root tx_qroot = {"Send", NULL, NULL, 0};

extern void *server_loop;
extern void *tx_loop;
extern void *rx_loop;

struct x_transmission
{
    /** 能够发送 */
    bool x_can;
    /** 快速发送 */
    bool x_quick;
    /** 正在发送的特征 */
    uint64_t x_sending_profile;
    /** 发送超时 */
    uint64_t x_active_send_timeout;
    /** 数据携带的超时时间 */
    uint64_t x_data_timeout;
    /** 发送缓冲 */
    char x_buf[512];
    /** 发送数据长度 */
    int x_len;
};
struct transform_root
{

    struct x_transmission tx;
    struct x_transmission rx;
};
/**
 * @brief 接收函数
 *
 * @param args
 */
void m_static_network_rx_loop(void *args);

struct transform_root transform_data_root = {0};

/** 是否恢复了计划 标记  false 恢复了*/
extern bool login_recover_schedule;

stat_m m_callable_network_transmisson_init(void)
{
    stat_m stat = fail_r;
    m_static_net_queue_init(&rx_qroot);
    m_static_net_queue_init(&tx_qroot);
    memset(&transform_data_root, 0, sizeof(struct transform_root));
    /** RX接收 */
    mTaskCreate(rx_loop, m_static_network_rx_loop, "s_link_rx", 512 * 7, (void *)3, 5, NULL); //--sock tcp link
    return stat;
}
/**
 * @brief 超强发送函数
 *
 * @param in_tx_data 发送的数据
 * @param in_tx_len 数据长度
 * @param in_spec_cmd 发送的指令信息
 * @param in_spec_cmd_flag 发送计数信息 可以是 0
 * @param in_timeout  超时时间爱你 单位 分钟
 * @param in_tx_queue 是否需要就爱如队列
 * @return stat_m
 */
stat_m m_callable_spec_scene_net_connext_tx(const char *in_tx_data, int in_tx_len, uint64_t in_spec_cmd, uint64_t in_spec_cmd_flag, uint64_t in_timeout, bool in_tx_queue)
{
    stat_m stat = fail_r;
    if (in_tx_data != NULL && in_tx_len > 0)
    {
        // if (in_tx_queue)
        // {
        //     if (m_static_net_queue_push(&tx_qroot, in_tx_data, in_tx_len, in_spec_cmd, in_spec_cmd_flag, in_timeout))
        //     {
        //         // mEventGroupSetBits(M_EVNBIT_TXHANDLE);
        //         mEventGroupSetBits(tx_loop, M_EVNBIT_TXHANDLE);
        //         stat = succ_r;
        //     }
        // }
        // else
        // {
        //     int mode = 0;
        //     m_callable_device_attribute_get_net_connect_mode(&mode);
        //     m_static_network_transmisson_specify_mode_tx(mode, false, in_tx_data, in_tx_len);
        //     // m_ext_net_link_connection_tx(in_tx_data, in_tx_len);
        // }
        /*如果是离线蓝牙模式认证后，并且其指令是些需要直接返回结果 则把in_tx_queue 置为false*/
        if ((m_callable_offline_bluetooth_instruction_filtering(in_spec_cmd) == succ_r) && (m_callable_offline_bluetooth_unbind_flag_get() == true))
        {
            DEBUG_TEST(DB_I, "在离线蓝牙模式下,该指令直接发送,不进入队列");
            in_tx_queue = false;
        }

        if ((m_callable_offline_bluetooth_instruction_filtering(in_spec_cmd) == M_ERR) && (m_callable_offline_bluetooth_unbind_flag_get() == true))
        {
            DEBUG_TEST(DB_I, "在离线蓝牙模式下,该指令不允许发送");
            return succ_r;
        }

        if (in_tx_queue)
        {
            if (m_callable_offline_bluetooth_logged_or_not_logged() == fail_r)
            {
                DEBUG_TEST(DB_I, "离线蓝牙模式断开了连接、该指令进丢弃");
                return succ_r;
            }
            DEBUG_TEST(DB_I, "该指令进入队列");
            if (m_static_net_queue_push(&tx_qroot, in_tx_data, in_tx_len, in_spec_cmd, in_spec_cmd_flag, in_timeout))
            {
                // mEventGroupSetBits(M_EVNBIT_TXHANDLE);
                mEventGroupSetBits(tx_loop, M_EVNBIT_TXHANDLE);
                stat = succ_r;
            }
        }
        else
        {
            DEBUG_TEST(DB_I, "该指令直接发送");
            int mode = 0;
            m_callable_device_attribute_get_net_connect_mode(&mode);
            m_static_network_transmisson_specify_mode_tx(mode, false, in_tx_data, in_tx_len);
            // m_ext_net_link_connection_tx(in_tx_data, in_tx_len);
        }
    }

    return stat;
}
/**
 * @brief
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
// stat_m m_static_network_transmisson_tx(bool is_queue, char *data, int datalen)
// {
//     stat_m stat = fail_r;

//     if (is_queue)
//         m_static_net_queue_push(&tx_qroot, data, datalen, 0, 0, 30);
//     else
//         m_ext_network_link_transformission_tx(data, datalen);

//     return stat;
// }
#ifdef M_GLOBAL_TRANSFORM_USE_TEA_ENCRYPT
char tx_buf[512] = {0};
#endif /* M_GLOBAL_TRANSFORM_USE_TEA_ENCRYPT */
/**
 * @brief 底层发送函数
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
stat_m m_static_network_transmisson_specify_mode_tx(enum connect_mode mode, bool is_queue, char *data, int datalen)
{
    stat_m stat = fail_r;
    if (is_queue)
        m_static_net_queue_push(&tx_qroot, data, datalen, 0, 0, 30);
    else if (m_callable_network_manage_get_server_login_status() /** 是连接上的 */)
    {
#ifdef M_GLOBAL_TRANSFORM_USE_TEA_ENCRYPT
        m_callable_crypt_xxtea_encrypt(data, &datalen, tx_buf);
        stat = m_ext_network_transmisson_specify_mode_tx(mode, tx_buf, datalen);
#else
        stat = m_ext_network_transmisson_specify_mode_tx(mode, data, datalen);
#endif
    }

    return stat;
}
// char format_s[3][50] = {{"%[^#@#]"}, {"%*[^#]#@#%[^#]"}, {"%*[^#]#@#%*[^#]#@#%[^#]"}};
char format_s[3][50] = {{"%[^]]"},
                        {"%*[^]]%*[^[]%[^]]"},
                        {"%*[^]]%*[^[]%*[^]]%*[^[]%[^]]"}};
stat_m m_static_network_rx_cut(char *in_buf, char *out_buf, int *offic)
{
    stat_m stat = fail_r;

    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_buf, format_s[*offic], out_buf);
    if (strlen(out_buf) > 0)
    {
        stat = succ_r;
    }
    DEBUG_TEST(DB_I, "%s    len: %d", out_buf, strlen(out_buf));
    return stat;
}

/**
 * @brief 接收函数
 *
 * @param args
 */
void m_static_network_rx_loop(void *args)
{
    /** 连接模式 */
    int con_mode = 0;
    /** 连接状态 */
    int con_status = 0;

    // transform_data_root.rx.x_buf = (char *)M_MALLOC(sizeof(char) * 512);
    // transform_data_root.rx.x_can = true;
    transform_data_root.rx.x_len = 1;
    // m_callable_event_group_wait_bits(server_loop, M_EVNBIT_CONNRCT_SERVER, true);
    while (1)
    {
        if (transform_data_root.rx.x_can)
        {
            m_callable_device_attribute_get_net_connect_mode(&con_mode);
            m_static_network_manage_get_server_connect_status(&con_status);

            if (con_status == M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS)
            {
                if (m_callable_network_ext_tcp_mqtt_offline_data_rx(con_mode, transform_data_root.rx.x_buf, &transform_data_root.rx.x_len, M_TIMEOUT_LONG_LIMIT_MIN) == succ_r)
                {
                    {
                        DEBUG_TEST(DB_W, "is m_network_connect_progress_LOGIN_SUCCESS %s- %d", transform_data_root.rx.x_buf, transform_data_root.rx.x_len);
                        if (transform_data_root.rx.x_len > 1)
                        {
                            /** 存入接收队列 */
                            m_callable_network_transmisson_rx_msg_handle(true, transform_data_root.rx.x_buf, transform_data_root.rx.x_len);
                            // QueuePush(&rx_qroot, final_buf, len, 0, 0, 0);
                        }
                    }
                    // DEBUG_TEST(DB_W,"%s", rx_buffer);
                    mEventGroupSetBits(rx_loop, M_EVNBIT_RXHANDLE);
                }
            }
            /** 登陆 会超时 ...  */
            else if (m_callable_network_ext_tcp_mqtt_offline_data_rx(con_mode, transform_data_root.rx.x_buf, &transform_data_root.rx.x_len, M_TIMEOUT_35_SEC) == succ_r)
            {
                /** 直接处理 */
                m_callable_network_data_rx(transform_data_root.rx.x_buf, transform_data_root.rx.x_len);
                transform_data_root.tx.x_quick = true;
            }
            if (transform_data_root.rx.x_len <= 0)
            {
                DEBUG_TEST(DB_I, " Rx %d  Server Disconnected", transform_data_root.rx.x_len);
                m_callable_network_set_server_login_status(false);
                m_callable_middle_connect_handle_set_code(M_CONNECT__CODE__WIFI_SUCCESS);
                transform_data_root.tx.x_can = false;
                transform_data_root.rx.x_can = false;
                m_callable_network_server_disconnect();
            }
        }
        mDelay_ms(100);
    }
}

/**
 * @brief 接收处理线程
 *
 * @param args
 */
void m_static_network_rx_handle_loop(void *args)
{
    int context_cmd = 0;
    static char cmd_buf[RECE_BUF_SIZE] = {0};
    static char cmd_buf_re[RECE_BUF_SIZE] = {0};
    int reindex = 0;
    // stat_m m_static_tranfmiss_guarantee_set_value(int in_wait_value);
    /**
     * @brief 数据校对
     *
     * @param in_value 收到的值  一般 1
     * @param in_buf 收到的缓存 只取第一个
     * @return stat_m
     */
    // stat_m m_static_tranfmiss_guarantee_proofread(int in_wait_value, char *in_buf);
    // mDelay_ms(200);
    while (1)
    {
        // if (m_callable_offline_bluetooth_logged_or_not_logged() == fail_r)
        // {
        //     mDelay_ms(500);
        // }
        // else
        // {
        memset(cmd_buf, 0, 512);
        m_callable_event_group_wait_bits(rx_loop, M_EVNBIT_RXHANDLE, false);
        mDelay_ms(100);
        DEBUG_TEST(DB_I, "Rx Data.. 👇");
        while (m_static_net_queue_pop(&rx_qroot, cmd_buf, NULL, NULL, NULL))
        {
            mDelay_ms(100);
            DEBUG_TEST(DB_I, "m_static_network_rx_handle_loop :%s", cmd_buf);
            reindex = 0;
            while (m_static_network_rx_cut(cmd_buf, cmd_buf_re, &reindex) == succ_r)
            {
                if (m_callable_data_parse_format_cmd(&context_cmd, cmd_buf_re) == succ_r)
                {
                    DEBUG_TEST(DB_I, "%d", context_cmd);
                    reindex++;
                    m_static_net_queue_dele(&rx_qroot);
                    m_static_tranfmiss_guarantee_set_value(context_cmd, cmd_buf_re);
                    if (context_cmd > 1)
                    {
                        // m_static_tranfmiss_guarantee_set_value(context_cmd, NULL);
                        m_ext_server_message_handle(context_cmd, cmd_buf_re);
                    }
                    context_cmd = 0;
                }
                mDelay_ms(100);
                memset(cmd_buf_re, 0, 512);
            }
            memset(cmd_buf, 0, 512);
            // memset(cmd_buf, 0, strlen(cmd_buf));
        }
        mEventGroupReSetBits(NULL, M_EVNBIT_RXHANDLE);
    }
    // }
}
/**
 * @brief 发送处理线程
 *
 * @param args
 */
void m_static_network_tx_handle_loop(void *args)
{
    int len = 0;
    /** 无用 */
    uint64_t abc = 0;
    // QueueInit(&tx_qroot);
    int smode = 0, device_ste = 0, dev_run_mode = 0;

    transform_data_root.tx.x_active_send_timeout = 0;
    // transform_data_root.tx.x_buf = M_MALLOC(512);

    m_callable_device_attribute_get_net_connect_mode(&smode);
    m_callable_device_attribute_get_running_mode(&dev_run_mode);
    while (1)
    {
        // busc =
        m_callable_event_group_wait_bits(tx_loop, M_EVNBIT_TXHANDLE, false);
        mDelay_ms(500);

        // if (_device.state.onLine)
        {
            memset(transform_data_root.tx.x_buf, 0, 512);
            while (m_static_net_queue_pop(&tx_qroot,
                                          transform_data_root.tx.x_buf,
                                          &transform_data_root.tx.x_sending_profile,
                                          &abc,
                                          &transform_data_root.tx.x_data_timeout)) // timeout > m_callable_timer_manage_get_device_time() ||
            {
                mDelay_ms(100);
                /** 2.1 连接突然断开，或无连接 不发送 */
                if (!transform_data_root.tx.x_can)
                {
                    mDelay_ms(500);
                    continue;
                }
                transform_data_root.tx.x_len = strlen(transform_data_root.tx.x_buf);
                m_ext_network_transmisson_specify_mode_tx(smode, transform_data_root.tx.x_buf, transform_data_root.tx.x_len);

                for (size_t i = 0; i < 2 * 13; i++)
                {
                    if (!transform_data_root.tx.x_can)
                        break;
                    if (m_static_tranfmiss_guarantee_proofread(transform_data_root.tx.x_sending_profile) == succ_r)
                    {
                        // m_callable_spec_scene_net_connext_tx();
                        m_static_net_queue_dele(&tx_qroot);
                        transform_data_root.tx.x_active_send_timeout = 0;
                        transform_data_root.tx.x_active_send_timeout = 0;
                        break;
                    }
                    else
                        mDelay_ms(600);

                    if (i == 25)
                    {
                        DEBUG_TEST(DB_W, "Timeout Retransmission ...%s.. ", transform_data_root.tx.x_buf);
                        if (transform_data_root.tx.x_active_send_timeout++ > M_SERVICE_RESP_TIME_OUT)
                        {
                            DEBUG_TEST(DB_W, "Send Timeout  And  Proactively disconnect  ... ");
                            m_callable_network_server_disconnect();
                            device_init_check_time_out = 0;
                            transform_data_root.tx.x_active_send_timeout = 0;
                        }
                    }
                }
                transform_data_root.tx.x_sending_profile = 0;
            }
            mEventGroupReSetBits(tx_loop, M_EVNBIT_TXHANDLE);
        }
    }
}
// if (smode == M_NETWORK_MANAGE_CONNECT_MODE_TCP)
//     m_ext_network_link_transformission_tx(pop, len);
// else if (smode == M_NETWORK_MANAGE_CONNECT_MODE_MQTT)
// {
//     m_ext_network_link_transformission_mqtt_tx(pop, len);
// }
/**
 * @brief 传输接收 ,存入队列
 *
 * @param data 数据
 * @param datalen 长度
 * @return stat_m
 */
stat_m m_callable_network_transmisson_rx_msg_handle(bool is_queue, char *data, int datalen)
{
    stat_m stat = fail_r;
    // m_ext_network_link_transformission_rx();
    if (m_static_net_queue_push(&rx_qroot, data, datalen, 0, 0, 0))
    {
        stat = succ_r;
    }
    mEventGroupSetBits(rx_loop, M_EVNBIT_RXHANDLE);

    return stat;
}
/**
 * @brief 传输接收 ,存入队列
 *
 * @param data 数据
 * @param datalen 长度
 * @return stat_m
 */
stat_m m_static_network_transmisson_rx(bool is_queue, char *data, int datalen)
{
    stat_m stat = fail_r;
    // m_ext_network_link_transformission_rx();
    m_static_net_queue_push(&rx_qroot, data, datalen, 0, 0, 0);
    return stat;
}
/**
 * @brief 设置连接接收时机 保证在网络无连接诶和无网络不露数据
 *
 * @param ste
 * @return stat_m
 */
stat_m m_static_network_transform_status_rx_able_set(bool ste)
{
    transform_data_root.rx.x_can = ste;
    return succ_r;
}
/**
 * @brief 设置连接发送时机 保证在网络无连接诶和无网络不发送空数据
 *
 * @param ste
 * @return stat_m
 */
stat_m m_static_network_transform_status_tx_able_set(bool ste)
{
    transform_data_root.tx.x_can = ste;
    return succ_r;
}