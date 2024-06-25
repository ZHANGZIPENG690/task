
#include "core/fset_lib.h"
#include "core/utils/util.h"
#include "mnetwork.h"


static  QueRoot rx_qroot = {"Recv", NULL, NULL, 0};

static  QueRoot tx_qroot = {"Send", NULL, NULL, 0};





/** 数据接收*/
void link_rx_loop(void *args)
{
    // int busc = 0;
    int context_cmd = 0;
    static char cmd_buf[RECE_BUF_SIZE] = {0};

    // mDelay_ms(200);
    while (1)
    {
        // busc = 
        m_callable_event_group_wait_bits(M_EVNBIT_RXHANDLE, false);

        while (m_static_queue_pro_pop(&rx_qroot, cmd_buf, NULL, NULL, NULL))
        { // 这里有一个超时逻辑没加入

            m_static_queue_pro_rm(&rx_qroot);
        //     // if (m_insi_foramt_int_cmd(&context_cmd, cmd_buf) == succ_r)
        //     // {
        //     //     // m_server_message_handle(context_cmd, cmd_buf);
        //     // }
        //     // memset(cmd_buf, 0, strlen(cmd_buf));
        }
    }
}
#define PING_COUNT 70
bool queue_send_done = false;
int sock_timeout = 0;



/** 發送邏輯需要管理下 */

/** 数据发送*/
void link_tx_loop(void *args)
{
    // char hu[20] = "898989898";
    // int busc = 0;
    char pop[100] = {0};
    int len = 0;
    int abc = 0;
    int timeout = 0;
    int reSendCount = 0;
    // QueueInit(&tx_qroot);

    while (1)
    {
        // busc = 
        m_callable_event_group_wait_bits(M_EVNBIT_TXHANDLE, false);

        // if (_device.state.onLine)
        {
            memset(pop, 0, sizeof(pop));
            while (m_static_queue_pro_pop(&tx_qroot, pop, &_link.buf_flag_top.buf_flag_cmd, &_link.buf_flag_top.buf_flag_tx_count, &timeout))
            {
                // printf("--kk faspon %s" , pop);
                // if(m_callable_get_device_time_t() > )
                if (sock_timeout <= PING_COUNT * 2 && queue_send_done)
                {
                    len = strlen(pop);
                    reSendCount = 0;
                    queue_send_done = false;
                    DEBUG_TEST(DB_I,"send ..(%s %lld %lld %d)", pop, _link.buf_flag_top.buf_flag_cmd, _link.buf_flag_top.buf_flag_tx_count, timeout);
                    // DEBUG_TEST(DB_I,"send ing....." );
                    m_ext_net_link_connection_tx(pop, len);
                }
                // else
                // {
                //     DEBUG_TEST(DB_W,"..(%d)...%d", sock_timeout, queue_send_done);
                // }
                // DEBUG_TEST(DB_I,"sock_timeout: %d", sock_timeout);
                // mDelay_ms(50);
                if (sock_timeout < PING_COUNT * 2 && _link.buf_flag_top.allow_deletion)
                {
                    _link.buf_flag_top.allow_deletion = false;
                    queue_send_done = true;
                    m_static_queue_pro_rm(&tx_qroot);
                    // DEBUG_TEST(DB_W,"**> %s\r\n", pop);

                    DEBUG_TEST(DB_W,"(%s)SEND_OK", pop);
                    _link.buf_flag_top.buf_flag_cmd = 0;
                    _link.buf_flag_top.buf_flag_tx_count = 0;
                }
                else
                {
                    reSendCount += 1;
                    if (reSendCount >= 30 && reSendCount < 40)
                    {
                        // lwip_send(sock__, pop, len, 2);

                        reSendCount = 0;
                        DEBUG_TEST(DB_W,"wait...(%lld.%lld).%s", _link.buf_flag_top.buf_flag_cmd, _link.buf_flag_top.buf_flag_tx_count, pop);
                        mDelay_ms(1500);
                        // queue_send_done = true;
                    }
                    // break;
                }
            }
            // if (ping_send_flag)
            {
                // reSendCount = 0;
                // ping_send_flag = false;
                // while (sock_timeout > 2)
                // // {
                // if (ping_tail_count > 9)
                //     ping_tail_count = 1;

                // sprintf(ping_pop, "%s%d#@#\n", PING, ping_tail_count);
                // abc = lwip_send(sock__, PING[ping_tail_count], 6, 2);
                // m_ext_net_link_connection_tx(PING[ping_tail_count], 6);

                // DEBUG_TEST(DB_I,"ping_1(%d) {%d} --- %d", sock_timeout, abc, pings);

                // if (abc < 1)
                //     DEBUG_E("send fail !");
                // pings++;
                // ping_tail_count++;
            }
            // send(sock__, PING, strlen(PING), 2);
        }
        mDelay_ms(3000);
        // quick_handle_send = false;
    }
}

//  数据发送，直接发送或者存入队列
stat_m m_callable_spec_scene_net_connext_tx(const char *in_tx_data, int in_tx_len, uint64_t in_spec_cmd, uint64_t in_spec_cmd_flag, uint64_t in_timeout, bool in_tx_queue)
// stat_m m_callable_spec_scene_net_connext_tx(const char *in_tx_data, int in_tx_len, uint64_t in_spec_cmd, uint64_t in_spec_cmd_flag, uint64_t in_timeout, bool in_tx_queue)
{
    stat_m stat = fail_r;
    if (in_tx_data != NULL && in_tx_len > 0)
    {
        if (in_tx_queue)
        {
            if (m_static_queue_pro_push(&tx_qroot, in_tx_data, in_tx_len, in_spec_cmd, in_spec_cmd_flag, in_timeout))
            {
                mEventGroupSetBits(M_EVNBIT_TXHANDLE);
                stat = succ_r;
            }
        }
        else
        {
            // m_ext_net_link_connection_tx(in_tx_data, in_tx_len);
        }
    }
    return stat;
}
















