
#include "include.h"
#include <stdio.h>
#include "core/fset_lib.h"

// #include <Python.h>

#define EXANOLE_DATA "[40,1#1#60;2#2#60]"

struct mda_tx_t
{
	/** Data content 「cmd,」#@#*/
	const char *in_tx_data;
	/** Data len*/
	int in_tx_len;
	uint64_t in_spec_cmd;
	uint64_t in_spec_cmd_flag;
	uint64_t in_timeout;
	bool in_tx_queue;
};

/**
 *
 *       单元测试入口函数
 *
 */
// stat_m m_network_link_wifi_init(void);
// // 數據發送
// stat_m m_callable_spec_scene_net_connext_tx(const char *in_tx_data, int in_tx_len, uint64_t in_spec_cmd, uint64_t in_spec_cmd_flag, uint64_t in_timeout, bool in_tx_queue);

/**
 *
 *       单元测试出口函数
 *
 */


// 

extern  stat_m (*m_callable_manual_handle)(enum operate_event event_id, uint8_t pre_channel, uint8_t next_channel, uint32_t has_benn_runnint_time, uint64_t current_time);
void add(stat_m (*dd)(enum operate_event event_id, uint8_t pre_channel, uint8_t next_channel, uint32_t has_benn_runnint_time, uint64_t current_time))
{
    m_callable_manual_handle = dd;
}
 stat_m m_static_manual_handle(enum operate_event event_id, uint8_t pre_channel, uint8_t next_channel, uint32_t has_benn_runnint_time, uint64_t current_time)
 {
		printf("(%d) %hhd - %hhd  --> %d      --%lld \n" , event_id,  pre_channel,  next_channel,  has_benn_runnint_time,  current_time);
 }



void main()
{
	m_callable_manual_init();

	add(m_static_manual_handle);

	m_callable_manual_event_input(0  , 0 ,0 ,0);
	m_callable_manual_event_input(0  , 0 ,0 ,0);
	m_callable_manual_event_input(0  , 0 ,0 ,0);
	m_callable_manual_event_input(0  , 0 ,0 ,0);


}
