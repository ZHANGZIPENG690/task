



#include "core/fset_lib.h"
/**
 * @file cmd_parse.h
 * @brief 
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write 
 * @date 2023-07-10
 */

#ifndef __DATA_PARSE__H__
#define __DATA_PARSE__H__

/**
 * @brief 
 * 
 * @return stat_m 
 */
stat_m m_callable_data_parse_init(void);
/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉'[' 提取下一个到 ',' 为止的int参数
 * @param out_targat 解析的cmd
 * @param in_src 源数据
 * @return stat_m
 */
// stat_m m_callable_data_parse_format_cmd(char *out_targat, char *in_src, int *len );
stat_m m_callable_data_parse_format_cmd(int *out_targat, char *in_src);

/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉',' 提取下一个到 ',' 为止的String参数
 *
 * @param out_targat  提取到的目标将从这里输出
 * @param in_src  输入提取源
 * @return stat_m 提取失败或者成功
 */
stat_m m_callable_data_parse_foramt_str(char *out_targat, char *in_src);
/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉',' 提取下一个到 ',' 为止的&{format}参数
 *
 * @param format  example : %lld %hhd %d
 * @param out_targat  提取到的目标将从这里输出
 * @param in_src  输入提取源
 *
 * @return stat_m 提取失败或者成功
 */
stat_m m_callable_data_parse_format_vue(char *format, void *out_targat, char *in_src);

/**
 * @brief  按照指定个格式 一次性解析多个参数 2个
 * 
 */
stat_m m_callable_data_parse_foramt_param(char *format, void *arg1, void *arg2, char *in_src);
/**
 * @brief  按照指定个格式  一次性解析多个参数 3个
 * 
 * @param format 
 * @param arg1 
 * @param arg2 
 * @param arg3 
 * @param in_src 
 * @return stat_m 
 */
stat_m m_callable_data_parse_foramt_param_pro(char *format, void *arg1, void *arg2, void *arg3, char *in_src);
/**
 * @brief  按照指定个格式  一次性解析多个参数 4个
 * 
 * @param format 
 * @param arg1 
 * @param arg2 
 * @param arg3 
 * @param arg4
 * @param in_src 
 * @return stat_m 
 */
stat_m m_callable_data_parse_foramt_param_pro_max(char *format, void *arg1, void *arg2, void *arg3, void *args4,char *in_src);


/**
 * @brief 得到一个 JSON 相关的 键值
 *
 * @param in_key 键
 * @param out_resp 解析出的值
 * @param in_src 源 json
 * @return stat_m
 */
stat_m m_callable_json_data_parse_foramt_one_key(char *in_key, char *out_resp, char *in_src);

/**
 * @brief 解析 第一到第三个参数
 *
 * @param format
 * @param arg1
 * @param arg2
 * @param in_src
 * @return stat_m
 */
stat_m m_callable_data_parse_foramt_quick_time(char *format, void *arg1, void *arg2, void *arg3, char *in_src);


/**
 * @brief 解析星期
 * 
 * @param format 
 * @param span 
 * @param pragma1 
 * @param in_src 
 * @return stat_m 
 */
stat_m m_callable_data_parse_foramt_week_c(char *format, char span, char *pragma1, char *in_src);


/**
 * @brief 解析 wifiname
 *
 * @param json
 * @param wifiName
 * @return stat_m
 */
stat_m m_callable_json_extract_wifiname(const char *json, char *wifiName);

/**
 * @brief 解析 password
 *
 * @param json
 * @param password
 * @return stat_m
 */
stat_m m_callable_json_extractpassword(const char *json, char *password);

/**
 * @brief 解析 connect type
 *
 * @param json
 * @param type
 * @return stat_m
 */
stat_m m_callable_json_extract_type(const char *json, char *type);


/**
 * @brief 蓝牙配网阶段解析 wifi名称、密码、和联网类型
 *
 * @param str
 * @param len
 * @return stat_m
 */
stat_m m_callable_json_config_net_set_par(char *str, int len);

/**
 * @brief 把所有区域运行剩余时长 进行字符串拼接
 * @param  area
 * @param time
 * @param num_areas
 * @param output
 * @return void
 */
void m_callable_generatestring(int area[], uint64_t time[], int num_areas, char output[]);


/**
 * @brief 把带有&的字符串进行分割并且把浮点数进行返回
 * @param  str
 * @param num1
 * @param num2
 * @return stat_m
 */
stat_m m_callable_parsestring(const char *str, float *num1, float *num2);

// 字符拼接函数
void m_callable_concatenatestring(const char *input, char *output);
#endif