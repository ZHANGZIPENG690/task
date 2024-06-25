
#include "include.h"
#include <stdio.h>
#include "core/fset_lib.h"

// #include <Python.h>


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

// int m_callable_device_attribute_device_type_get()
// {
// 	return 16;
// }
// int m_ext_drive_gpio_get_level()
// {
// 	return 0;
// }

// //
// extern  stat_m (*m_callable_key_handle)(enum key_event key_id, uint8_t pre_key_index, uint8_t current_key_index, uint64_t current_time_ms);
// void add(stat_m (*dd)(enum key_event key_id, uint8_t pre_key_index, uint8_t current_key_index, uint64_t current_time_ms))
// {
//     m_callable_key_handle = dd;
// }

#include "core/fset_lib.h"

void main()
{
	// printf("jj");
	// printf("Start..\n");
	// printf("Start..%d\n" , boo);

	// printf("Start..\n");
	// pr
	// intf("Start..\n");
	// printf("Start..\n");
	// printf("Start..\n");
	// printf("Start..\n");
	// m_network_link_wifi_init();
	// // struct mda_tx_t da_tx
	// // {
	// // 	.in_tx_data = EXANOLE_DATA,
	// // 	.in_tx_len = strlen(EXANOLE_DATA)
	// // };
	// // m_callable_spec_scene_net_connext_tx(&da_tx);
	// m_callable_spec_scene_net_connext_tx(EXANOLE_DATA , strlen(EXANOLE_DATA) , 40 , 40 , 300 , true);

	// FILE *f;
	// f = popen("./niu.py 99" , "r");

	// fclose(f);
	// while (1)
	// 	;
	char src_da[30] = "[45,1#1#60;2#2#60,0,1,100]";
	
	m_callable_data_parse_init();
	char *out_str;
	int out_int;
	m_callable_data_parse_format_cmd(&out_int, src_da);
	printf("cmd :%d\n", out_int);
	m_callable_data_parse_foramt_str(	out_str, src_da);
	printf("str :%s\n", out_str);
	m_callable_data_parse_foramt_str(	out_str, src_da);
	printf("str :%s\n", out_str);
	m_callable_data_parse_foramt_str(	out_str, src_da);
	printf("str :%s\n", out_str);
	printf("src_da :%s\n", src_da);
}
