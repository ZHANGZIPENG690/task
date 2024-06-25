
#include "include.h"
#include <stdio.h>
#include "core/fset_lib.h"



// #include <Python.h>

// #define EXANOLE_DATA "[40,1#1#60;2#2#60]"

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

int m_callable_device_attribute_device_type_get()
{
	return 16;
}
int m_ext_drive_gpio_get_level()
{
	return 0;
}

//
// extern  stat_m (*m_callable_key_handle)(enum key_event key_id, uint8_t pre_key_index, uint8_t current_key_index, uint64_t current_time_ms);
// void add(stat_m (*dd)(enum key_event key_id, uint8_t pre_key_index, uint8_t current_key_index, uint64_t current_time_ms))
// {
//     m_callable_key_handle = dd;
// }

void main()
{
	char m_static_channel_config_a[] = "3*5";
	DEBUG_TEST( DB_I,"Start ..\n");
	// 1、区域初始化
	m_callable_channel_manage_init(8);
	
	
	
	
	// { // 区域禁用和启用测试
	// 	// 2、关闭一个区域
	// 	m_callable_channel_manage_disable(5, 1999565, 492054102);
	// 	// 3、查询所有区域状态
	// 	m_test_channel_manage_get_param();
	// 	// 4、开启一个区域
	// 	m_callable_channel_manage_enable(5, 0, 0);
	// 	// 5、查询所有区域状态
	// 	m_test_channel_manage_get_param();
	// }




	// { // 区域多个浸泡时间测试
	// 	// 2、设置一个区域的时间
	// 	// m_callable_channel_manage_disable(5, 1999565, 492054102);
	// 	m_callable_channel_manage_offline_running_param(2, M_STATIC_CHANNEL_CONFIG_A, m_static_channel_config_a);
	// 	// 3、查询所有区域状态
	// 	m_test_channel_manage_get_param();
	// 	// 4、设置一个区域的时间
	// 	m_callable_channel_manage_offline_running_param(2, M_STATIC_CHANNEL_CONFIG_B, m_static_channel_config_a);
	// 	// 5、查询所有区域状态
	// 	m_test_channel_manage_get_param();
	// }







	{ // 区域禁用时间段测试

		// 2、在指定时间段关闭区域
		m_callable_channel_manage_disable(5, 1999565, 492054102);
		// 3、查询所有区域状态
		m_test_channel_manage_get_param();
		{
			// 3.5 如果我在中间开启的话。会得道不同的结果
			m_callable_channel_manage_enable(5, 1999565, 492054102);
		}
		// 3、查一下 看是不是真的有效
		if( m_callable_channel_manage_current_is_effective(5, 5672555) == succ_r)
		{
			printf("区域属于禁用时间段");
		}

	}
	DEBUG_TEST( DB_I,"End ..\n");
}
