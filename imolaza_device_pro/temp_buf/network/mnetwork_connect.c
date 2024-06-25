
#include "core/fset_lib.h"
#include "mnetwork.h"


char rx_buffer[200]; 

struct mnetwork _link;

extern void link_tx_loop(void *args);
extern void link_rx_loop(void *args);


void link_server_loop(void *args);

stat_m m_network_link_wifi_init(void )
{

    mTaskCreate(link_server_loop, "s_link_server", 1024 * 3, (void *)3, 5, NULL); //--sock tcp link
    mTaskCreate(link_rx_loop, "s_link_rx", 1024 * 6, (void *)3, 5, NULL);         //--sock tcp link
    mTaskCreate(link_tx_loop, "s_link_tx", 1024 * 3, (void *)3, 5, NULL);         //--sock tcp link
}

static stat_m m_static_net_link_login_in(void)
{
    stat_m stat = fail_r;
    if (_link.link_is_connected)
    { // 67
        // create_send_data_to_server_login_in(67, NULL, "_device.hardware.version", 10, 0, 254, 0, 13, /*_device.extreinfo.schedule_sum*/0);
    }
    else
    { // 11  10  15
        // create_send_data_to_server_login_in(11, NULL, "_device.hardware.version", 0, 0, 0, 0, 0,  /*_device.extreinfo.schedule_sum*/0);
    }
    // QueuePush(&tx_qroot,);
    stat = succ_r;
    // _device.state.ReConnectServer = true;
    return stat;
}


/** 连接服务器*/
void link_server_loop(void *args)
{
    int len = 0;
    stat_m stat = fail_r;
    static char final_buf[200] = {0};
    /**
     * @brief Wait for the network link
     */
    // esp_netif_init();

    // set_system_normal();
    strcpy(_link.base.addr_buf, _link.base.addr);

    // nvs_extract_plan_num();
    m_callable_event_group_wait_bits(M_EVNBIT_CONNRCT_SERVER, true);
    while (1)
    {
        // if (out_check(STAT_OTA_START) != M_OKK)
        // && _tcp.auto_connect)
        {
            // if (_tcp.auto_connect && _wifi.wifi_is_connected)
            if (!_link.link_is_connected)
            {
                // err_net_count = 0;
                // if (m_ext_net_target_addr(_link.base.addr, &_link.base.port) == succ_r) // 3.56.1.112
                {
                    if (m_ext_net_link_connection(_link.base.addr, _link.base.port) == succ_r) // "device.imolaza.com"
                    {
                        // 连接成功，开始认证
                        stat = m_static_net_link_login_in();
                        while (stat == succ_r)
                        {
                            mEventGroupReSetBits(M_EVNBIT_CONNRCT_SERVER);
                            // 等待服务器接收指令 阻塞
                            // if (m_ext_net_link_receive_data(rx_buffer, 250) == succ_r)
                            {
                                // 分割指令 存入队列
                                // while (m_insi_foramt_handle_server_sub_str(final_buf, rx_buffer, &len) == succ_r)
                                // {
                                //     if (len > 1)
                                //     {
                                //         // QueuePush(&rx_qroot, final_buf, len, 0, 0, 0);
                                //     }
                                // }
                                mEventGroupSetBits(M_EVNBIT_RXHANDLE);
                            }
                            // else
                            //     break;
                        }
                    }
                    // else
                    //     mDelay_ms(2000);
                }
                // else
                // {
                //     mDelay_ms(5000);
                //     mEventGroupSetBits(M_EVNBIT_CONNRCT_SERVER);
                //     // _errTestInfo.offline_cause =  network_anomaly;
                // }
            }
            // else
            // {
            //     // err_net_count++;
            // }

            // m_net_link_prepare();
        }
        mDelay_ms(100);
        {
            // mEventGroupWaitBits(M_EVNBIT_CONNRCT_SERVER, true);
        }
    }
}


















