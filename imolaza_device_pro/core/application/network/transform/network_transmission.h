/**
 * @file network_transmission.h
 * @author your name (you@domain.com)
 * @brief  全部的数据发送和接受，包含三种方式
 * @version 0.1
 * @date 2023-09-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef NETWORK_TRANSMISSION__H__
#define NETWORK_TRANSMISSION__H__

#include "../network.h"

#include "./queue/net_queue.h"

/**
 * @brief 传输初始化
 *
 * @return stat_m
 */
stat_m m_callable_network_transmisson_init(void);
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
stat_m m_callable_spec_scene_net_connext_tx(const char *in_tx_data, int in_tx_len, uint64_t in_spec_cmd, uint64_t in_spec_cmd_flag, uint64_t in_timeout, bool in_tx_queue);
/**
 * @brief 传输接收 ,存入队列
 *
 * @param data 数据
 * @param datalen 长度
 * @return stat_m
 */
stat_m m_static_network_transmisson_rx(bool is_queue, char *data, int datalen);

/**
 * @brief
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
stat_m m_static_network_transmisson_specify_mode_tx(enum connect_mode mode,bool is_queue, char *data, int datalen);


/**
 * @brief 发送处理线程
 * 
 * @param args 
 */
void m_static_network_tx_handle_loop(void *args);
/**
 * @brief 接收处理线程
 * 
 * @param args 
 */
void m_static_network_rx_handle_loop(void *args);



/**
 * @brief 设置数据收发保障
 * 
 * @param in_wait_value 值
 * @return stat_m 
 */
stat_m m_static_tranfmiss_guarantee_set_value(int in_wait_value, char *in_buf);
/**
 * @brief 数据校对
 *
 * @param in_value 收到的值  一般 1
 * @param in_buf 收到的缓存 只取第一个
 * @return stat_m
 */
stat_m m_static_tranfmiss_guarantee_proofread(int in_wait_value);
/**
 * @brief 设置连接接收时机 保证在网络无连接诶和无网络不露数据
 *
 * @param ste 状态
 * @return stat_m
 */
stat_m m_static_network_transform_status_rx_able_set(bool ste);
/**
 * @brief 设置连接发送时机 保证在网络无连接诶和无网络不发送空数据
 * 
 * @param ste 状态
 * @return stat_m 
 */
stat_m m_static_network_transform_status_tx_able_set(bool ste);
#endif
