
/**
 * @file fset_lib.h
 * @brief 状态接口
 * @author 毛威鹏、陈前治
 * @version 1.0.0
 * @date 2023-05-10
 * @warning : m_ext 需要实现的函数 m_callable :给调用的函数
 */

#ifndef __FSET_LIB__H
#define __FSET_LIB__H

#include "core.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

#define schedule_doc_t int

#define RECE_BUF_SIZE 512

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Test ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/**
 * @brief 测试的调试信息
 *
 * @param en
 * @return char*
 */
extern char *toname(int en);

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Key ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/**
 * @brief Construct a new m callable key init object
 * @todo Unit Input ...
 *
 */
extern stat_m m_callable_key_init(uint8_t device_typr);
/**
 * @brief Construct a new m callable key init object
 * @todo Unit Input ...
 *
 */
extern stat_m m_static_drive_key_function_monitor(uint64_t current_time_ms);
/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Manual Run************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/

/**
 * @brief 手动运行初始化
 *
 * @return stat_m
 */
extern stat_m m_callable_manual_init(void);
/**
 * @brief 获取设备类型
 *
 * @param b 得到的类型
 * @return stat_m
 */
/**
 * @brief 手动运行检测 需要实施调用
 *
 * @param current_time_ms 当前运行的ms
 * @return stat_m
 */
stat_m m_callable_manual_function_monitor(uint64_t current_time_ms);
/**
 * @brief 手动运行获取运行区域
 *
 * @param in_zone
 * @return stat_m
 */
extern stat_m m_callable_manual_running_zone_get(int *in_zone);

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Flash ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/**
 * @brief 初始化数据存储
 *
 */
/** ------------------------------------------------------------------------------------------------- */
stat_m m_ext_flash_init(void);
/**
 * @brief 移除
 *
 * @param in_key
 * @return m_stat
 */
stat_m m_ext_flash_remove(const char *in_key);
/**
 * @brief 移除
 *
 * @param in_key
 * @return m_stat
 */
stat_m m_ext_flash_remove_all(void);
/**
 * @brief 恢复出厂
 *
 * @return stat_m
 */
stat_m m_ext_flash_factory_reset(void);
/**
 * @brief 在存储中添加一个新的值
 *
 * @param in_type 添加的值的类型
 * @param in_key 添加的值的key
 * @param in_data  待存入的值
 * @param in_resp_u64 待存入的64值
 * @return m_stat
 */
stat_m m_ext_flash_write(enum m_paramete_enum in_type, const char *in_key, void *in_data);
/**
 * @brief 在存储中读取一个类型的key值
 *
 * @param in_type 要读取的类型
 * @param in_key 读取的key
 * @param out_resp 得到的值
 * @param out_resp_u64 如果是64参数的返回值   如果不是可以选择填入 NULL
 * @return m_stat
 */
stat_m m_ext_flash_read(enum m_paramete_enum in_type, const char *in_key, void *out_resp);

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Callable Flash ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/**
 * @brief Initialize the data store
 */
stat_m m_callable_drive_flash_init(void);

/**
 * @brief Use the key to remove the stored value
 * @todo Unit Input ...
 *
 * @param in_key The key to remove
 * @return m_stat
 */
stat_m m_callable_drive_flash_remove(const char *in_key);
/**
 * @brief Add a new value to the store, corresponding to the key passed in
 * @todo Unit Input ...
 *
 * @param in_type The type of value added
 * @param in_key The key of value added
 * @param in_data  The alue of value added
 * @return m_stat
 */
stat_m m_callable_drive_flash_write(enum m_paramete_enum in_type, const char *in_key, void *in_data);

/**
 * @brief The value is obtained in the storage from the existing key
 * @todo Unit Out ...
 *
 * @param in_type The type to read
 * @param in_key The key to read
 * @param out_resp The value returned
 * @return m_stat
 */
stat_m m_callable_drive_flash_read(enum m_paramete_enum in_type, const char *in_key, void *out_resp);

/**
 * @brief 操作一个存储标记
 *
 * @param in_flag 句柄
 * @param in_status 设置/获取/删除
 * @return stat_m
 */
extern stat_m m_callable_flash_rescue_message_flag(const char *in_flag, enum m_stroage_rescue_message in_status);

/**
 * @brief Store/Delete/Get a short message for a given key value
 *
 * @param in_flag Key
 * @param inout_vue vue
 * @param in_status (Store/Delete/Get )
 * @return stat_m
 */
stat_m m_callable_drive_flash_flag_special_value_str(char *in_flag, char *inout_vue, int in_status);
/**
 * @brief Store/Delete/Get a short message for a given key value(u32_t)
 *
 * @param in_flag Key
 * @param inout_vue vue
 * @param in_status (Store/Delete/Get )
 * @return stat_m
 */
stat_m m_callable_drive_flash_flag_special_value_u32(char *in_flag, uint32_t *inout_vue, int in_status);

/**
 * @brief Use the key to remove the all
 * @todo Unit Input ...
 *
 * @return m_stat
 */
stat_m m_callable_drive_flash_remove_all(void);
/**
 * @brief Use the key to remove the factory
 * @todo Unit Input ...
 *
 * @return m_stat
 */
stat_m m_callable_drive_flash_factory_reset(void);

/**
 * @brief 从存储中恢复所有
 *
 * @return stat_m
 */
extern stat_m m_callable_schedule_stroage_recover_item_all(void);
/**
 * @brief 打印缓冲区内的全部计划信息
 *
 */
extern void m_callable_schedule_storage_buffer_print_to_remote(void);

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Monitor  *************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/

/**
 * @brief 状态检测函数原型初始化
 *
 * @return stat_m
 */
extern stat_m m_callable_global_monitor_init(void);
/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Timer ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/***********************************
 *
 * @brief 定时器初始化(外部实现)
 *
 * @param void
 *
 * @return void
 *************************************/
void m_ext_drive_timer_init(void);
/**
 * @brief 定时器初始化
 *
 * @return stat_m
 */
extern stat_m m_callable_timer_manage_time_init(void);
/**
 * @brief 时间走时
 *
 * @return stat_m
 */
extern stat_m m_callable_timer_manage_time_update(void);

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/******************Device Time And TimeZone ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/**
 * @brief 直接设置设备时间
 *
 * @param time
 * @return stat_m
 */
stat_m m_callable_timer_manage_time_direct_set(uint64_t time, int in_tz);
/**
 * @brief 设置设备的时间
 * @todo Unit Input ...
 *
 * @param time   设备时间
 * @param tz 时区
 * @return stat_m
 */
extern stat_m m_callable_timer_manage_time_set(uint64_t time, int tz, int type);
/**
 * @brief 季节调整，，简单的意思就是 在  time_inav1 到  time_inav2 之间的时间区间范围内 是 offset_tz
 *                      其他的是 offset_tz2
 * @todo Unit Input ...
 *
 * @param time_inav1 第一个时区开始的时间
 * @param offset_tz 时区1
 * @param time_inav2 第一个时区结束的时间 第二个时区开始的时间
 * @param offset_tz2 时区2
 * @return stat_m
 */
extern stat_m m_callable_timer_manage_timezone_switch_set(uint64_t time_inav1, int offset_tz, uint64_t time_inav2, int offset_tz2);

/**
 * @brief 获取设备时间
 *
 * @return uint64_t
 */
extern uint64_t m_callable_timer_manage_get_device_time(void);
/**
 * @brief 获取UTC时间
 *
 * @return uint64_t
 */
extern uint64_t m_callable_timer_manage_get_utc_time(void);
/**
 * @brief 获取当天的时间 小于等于86400
 *
 * @return uint32_t
 */
extern uint32_t m_callable_timer_manage_get_hhmmss(void);

/**
 * @brief 获取当天的星期
 *
 * @return uint32_t
 */
extern uint32_t m_callable_timer_manage_get_week(uint64_t in_time_s);
/**
 * @brief 获取当天的星期
 *
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_day(uint64_t timp);
/**
 * @brief 获取当天的设备 零点 时间
 *
 * @return uint64_t
 */
extern uint64_t m_callable_timer_manage_get_device_zero_time(void);
/**
 * @brief 解析一个时间戳的全部记录  年月日时分秒
 *
 * @param in_tim 时间
 * @param out_tm 输出
 * @return stat_m
 */
extern stat_m m_callable_timer_manage_get_ymd_hms(uint64_t in_tim, int *out_tm);
/**
 * @brief 获取两个时间戳之间的小时数
 *
 * @param timpA
 * @param timpB
 * @return uint32_t
 */
extern uint32_t m_callable_timer_manage_get_diff_hour_num(uint64_t timpA, uint64_t timpB);

/**
 * @brief 返回的时间经过了设备时间的确认
 *
 * @param timep 当前的时间戳
 * @param year
 * @param month
 * @param day
 * @return stat_m
 */

extern stat_m m_callable_timer_manage_get_ymd(uint64_t timep, uint32_t *year, uint32_t *month, uint32_t *day);

/**
 * @brief 时间的时间分类
 *
 * @param in_timestamp
 * @return stat_m
 */
stat_m m_callable_event_monitor(uint64_t in_timestamp);
/**
 * @brief Get Date - TimeStamp
 *
 * @param mohth Month
 * @param day Day
 * @param hour Hour
 * @return uint64_t
 */
extern uint64_t m_callable_timer_manage_get_spec_date_time(int mohth, int day, int hour);
/*******************************************************/

/**
 * @brief 更新时间链表的函数 需要每秒调用 在运行计划的时候
 *
 * @param current_timestamp
 * @return stat_m
 */
stat_m m_callable_event_distribution_advancing(uint64_t current_timestamp);

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Sensor ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/

/**
 * @brief 传感器初始化 加载传感器配置
 * @todo Unit Input ...
 *
 *
 */
extern stat_m m_callable_sensor_init(int sen1_pin, int sen2_pin);

/**
 * @brief 以特定的格式设置传感器，开关和类型
 * @todo Unit Input ...
 *
 * @param tp_vue 开关和类型
 * @param boo 区域存储  远程设置必须true
 */
extern stat_m m_callable_sensor_set(int tp_vue, enum sensor_type_m sensor_type, bool boo);
/**
 * @brief 设置传感器检测时间间隔
 *
 * @param in_water_type_time 雨量传感器时间间隔
 * @param in_flow_type_time 流量传感器时间间隔
 * @return stat_m
 */
extern stat_m m_callable_sensor_set_check_interval_time(uint32_t in_water_type_time, uint32_t in_flow_type_time);
/**
 * @brief 以指定类型开启传感器
 * @todo Unit Input ...
 *
 * @param channel
 * @param type
 */
stat_m m_callable_sensor_set_open(uint8_t channel, uint8_t type);
/**
 * @brief 关闭传感器
 * @todo Unit Input ...
 *
 * @param channel
 * @param type
 */
stat_m m_callable_sensor_set_close(uint8_t channel);

/**
 * @brief 传感器监控，需要循环调用 以保证在传感器主动动作后 设备能在指定的某个合适的时间内发现并且触发
 * @todo Unit Input ...
 *
 * @param current_time 当前的时间
 */
stat_m m_callable_sensor_monitor(uint64_t current_time);
/**
 * @brief 检测当前是否处于传感器触发
 *
 * @param sen_chan 如果触发，参数返回的是触发的通道
 * @param sen_type 如果触发，参数返回的是触发的传感器类型
 * @param current_time 当前的时间
 * @return stat_m succ_r 表示未触发 否则否则
 */
stat_m m_callable_sensor_current_is_effective(enum seneor_chann *sen_chan, enum seneor_type *sen_type, uint64_t current_time);

/**
 * @brief 计算当前运行区域的中断次数
 *
 * @param flag  反映当前爱你区域的因较编号，可以根据缓存查询到对应的通道id
 * @return stat_m
 */
stat_m m_callable_sensor_interrupt_count(int flag);

/**
 * @brief 传感器触发处理函数  作简单的清除 但是不会释放数据 ，等待恢复才会被释放 或者 超出运行时间
 *
 * @param in_obj 被触发的对象
 * @param in_time_s 这个不是触发的时间 而是 触发通道和触发类型的结合
 * @return stat_m
 */
extern stat_m m_callable_sensor_staging_trig_handle(void *in_obj, uint64_t in_time_s);

/**
 * @brief 解除触发处理函数，用于计算恢复 如果不需要恢复 将会被清理释放
 *
 * @param in_time_s 触发通道和触发类型的结合
 * @return stat_m
 */
extern stat_m m_callable_sensor_staging_distrig_handle(uint64_t in_time_s);
/**
 * @brief 直接添加一个传感器
 *
 * @param inobj 对象
 * @param in_time_s  时间
 * @return stat_m
 */
extern stat_m m_callable_sensor_staging_add_trig(void *inobj, uint64_t in_time_s);
/**
 * @brief 检查被触发的id 是不是传入ID
 *
 * @param in_chk_id ID
 * @return stat_m
 */
extern stat_m m_callable_sensor_staging_check_obj_id(uint64_t in_chk_id);
/**
 * @brief
 *
 * @param sensor_channel 触发的通道
 * @param sen_type 触发的传感器类造型
 * @param is_eff 操作是断开还是触发
 * @param num_sum 总的检测次数
 * @param num_curr 检测次数内被触发的次数
 * @return stat_m
 */
stat_m m_ext_sensor_opened_event_handle(enum seneor_chann sensor_channel, enum seneor_type sen_type, enum is_effective is_eff, uint64_t num_sum, uint64_t num_curr);
/**
 * @brief
 *
 * @param sensor_channel 触发的通道
 * @param sen_type 触发的传感器类造型
 * @param is_eff 操作是断开还是触发
 * @param num_sum 总的检测次数
 * @param num_curr 检测次数内被触发的次数
 * @return stat_m
 */
stat_m m_ext_sensor_not_open_event_handle(enum seneor_chann sensor_channel, enum seneor_type sen_type, enum is_effective is_eff, uint64_t num_sum, uint64_t num_curr);
/**
 * @brief 获取接口触发状态
 *
 * @param pin 引角
 * @param type 类型
 * @return stat_m
 */
stat_m m_ext_sensor_get_interface_status(int pin, enum seneor_type type);
/**
 * @brief 获取自上次到现在的流量值，并且指定是否清除缓存并且重新开始计算，如果流量传感器未打开 返回0
 *
 * @param is_clear 是否清除缓存
 * @return stat_m
 */
// extern stat_m m_callable_flow_sensor_get_total_value(float *out_final_vue, bool is_clear);

extern stat_m m_callable_flow_sensor_get_total_value(uint8_t channel_id, uint32_t running_time, float *out_final_vue, bool is_clear);
/**
 * @brief 外部的传感器初始化 (引脚初始化)
 *
 * @param s1
 * @param s2
 * @return stat_m
 */
stat_m m_ext_drive_sensor_init(int s1, int s2);

// /**
//  * @brief 流量计监测
//  * @return void
//  */
// void m_ext_sensor_flow_value_monitor(void);

/**
 * @brief 流量计选型
 * @param sensor_type 流量计类型
 * @return void
 */
void m_callable_sensor_type_set(enum sensor_type_m sensor_type);

// void m_callable_sensor_flow_value_monitor(void *args);

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Data Parse ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/

/**
 * @brief
 *
 * @return stat_m
 */
extern stat_m m_callable_data_parse_init(void);
/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉'[' 提取下一个到 ',' 为止的int参数
 * @param out_targat 解析的cmd
 * @param in_src 源数据
 * @return stat_m
 */
// extern stat_m m_callable_data_parse_format_cmd(char *out_targat, char *in_src, int *len );
extern stat_m m_callable_data_parse_format_cmd(int *out_targat, char *in_src);

/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉',' 提取下一个到 ',' 为止的String参数
 *
 * @param out_targat  提取到的目标将从这里输出
 * @param in_src  输入提取源
 * @return stat_m 提取失败或者成功
 */
extern stat_m m_callable_data_parse_foramt_str(char *out_targat, char *in_src);
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
extern stat_m m_callable_data_parse_format_vue(char *format, void *out_targat, char *in_src);

/**
 * @brief 服务器数据分割
 *
 * @param out_targat 输出目标
 * @param in_src 输入
 * @param len 分割后子串长度
 * @return stat_m
 */
extern stat_m m_callable_data_parse_foramt_handle_server_sub_str(char *out_targat, char *in_src, int *len);
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
stat_m m_callable_data_parse_foramt_param_pro_max(char *format, void *arg1, void *arg2, void *arg3, void *args, char *in_src);
/**
 * @brief 把所有区域运行剩余时长 进行字符串拼接
 * @param  area
 * @param time
 * @param num_areas
 * @param output
 * @return void
 */
extern void m_callable_generatestring(int area[], uint64_t time[], int num_areas, char output[]);
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

/*************-----------------------------------------------------------------------------------------------------------******************/
/*************************************************************************************************************************************************/
/****************** Net Connect  ************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/

/**
 * @brief 初始化加载一些配置
 *
 * @return stat_m
 */
extern stat_m m_callable_network_init(void);

/**
 * @brief 设置连接方式
 *
 * @param nm_mode   网络连接方式
 * @param sm_mode   服务器连接方式
 * @return stat_m
 */
extern stat_m m_callable_network_set_connect_mode(enum connect_mode nm_mode, enum connect_mode sm_mode);

/**
 * @brief 直接连接网络
 *
 * @param sm_mode 网络连接的模式     -1 无效
 * @param nm_mode 服务连接模式      -1 无效
 * @return stat_m
 */
extern stat_m m_callable_network_connect(void);
/**
 * @brief 设置开启或者关闭自动连接
 *
 * @return stat_m
 */
extern stat_m m_callable_network_set_net_auto_connect(bool);
/**
 * @brief 设置开启或者关闭自动连接
 *
 * @return stat_m
 */
extern stat_m m_callable_network_set_server_auto_connect(bool);
/**
 * @brief 主动断开网络连接
 *
 * @return stat_m
 */
stat_m m_callable_network_disconnect(void);
/**
 * @brief 主动断开服务器连接
 *
 * @return stat_m
 */
extern stat_m m_callable_network_server_disconnect(void);
/**
 * @brief 保持连接活跃
 *
 * @return stat_m
 */

extern stat_m m_callable_network_keep_active(void);
/**
 * @brief 设置网络信息
 *
 * @param user_name  用户认证名
 * @param user_token 用户token
 * @return stat_m
 */
extern stat_m m_callable_network_set_net_info(char *user_name, char *user_token);
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
 * @brief 指定发送模式发送数据
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
extern stat_m m_callable_network_specify_mode_data_tx(enum connect_mode mode, bool is_queue, char *data, int datalen);
/**
 * @brief 接收数据
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
extern stat_m m_callable_network_data_rx(char *data, int datalen);
/**
 * @brief
 *
 * @param data
 * @param datalen
 * @return stat_m  M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL
 */
extern stat_m m_ext_network_transmisson_specify_mode_tx(enum connect_mode mode, char *data, int datalen);
/**
 * @brief 蓝牙发送数据
 *
 * @param mode
 * @param data
 * @param datalen
 * @return stat_m
 */
extern stat_m m_ext_network_transmisson_bluetool_tx(char *data, int datalen);
/**
 * @brief 主动断开服务器连接
 *
 * @return stat_m
 */
stat_m m_ext_network_server_disconnect(void);
/**
 * @brief 主动断开tcp服务器连接
 *
 * @return stat_m
 */
stat_m m_ext_network_server_tcp_disconnect(void);

/***************************  WolfSSL ******************************/
/**
 * @brief
 *
 */
stat_m m_ext_software_drive_server_wolfSSL_init(void);
/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_wolfSSL_rx(char *out_data, int *len, int in_outtime);
/**
 * @brief 连接 WolfSSL 的软件驱动
 *
 * @param in_addr
 * @param in_port
 */
stat_m m_ext_software_drive_server_wolfSSL_connect(char *in_addr, int in_port);
/**
 * @brief WolfSSL 发送数据
 *
 * @param data 数据
 * @param data_len 数据长度
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_wolfSSL_tx(char *data, int data_len);
/**
 * @brief 主动断开 SSL 服务器连接
 *
 * @return stat_m
 */
stat_m m_ext_network_server_wolfSSL_disconnect(void);
/**
 * @brief 设置超时
 *
 * @param time_out 超时时间
 * @return stat_m
 */
stat_m m_ext_network_server_wolfSSL_set_timeout(int time_out);
/*******************************************************************/
/***************************  MbedTLS ******************************/
/**
 * @brief
 *
 */
stat_m m_ext_software_drive_server_mbedtls_init(void);
/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_mbedtls_rx(char *out_data, int *len, int in_outtime);
/**
 * @brief 连接 WolfSSL 的软件驱动
 *
 * @param in_addr
 * @param in_port
 */
stat_m m_ext_software_drive_server_mbedtls_connect(char *in_addr, int in_port);
/**
 * @brief WolfSSL 发送数据
 *
 * @param data 数据
 * @param data_len 数据长度
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_mbedtls_tx(char *data, int data_len);
/**
 * @brief 主动断开 SSL 服务器连接
 *
 * @return stat_m
 */
stat_m m_ext_network_server_mbedtls_disconnect(void);
/**
 * @brief 设置超时
 *
 * @param time_out 超时时间
 * @return stat_m
 */
stat_m m_ext_network_server_mbedtls_set_timeout(int time_out);
/*******************************************************************/
/**
 * @brief data send
 *
 * @param in_tx data
 * @param in_len length
 * @return stat_m
 */
extern stat_m m_ext_network_link_transformission_mqtt_tx(const char *in_tx, int in_len);
/**  tcp 发送的函数  */
extern stat_m m_ext_network_link_transformission_tx(char *data, int data_len);
/**
 * @brief 传输接收 ,存入队列
 *
 * @param data 数据
 * @param datalen 长度
 * @return stat_m
 */
extern stat_m m_callable_network_transmisson_rx_msg_handle(bool is_queue, char *data, int datalen);
/**
 * @brief 获取连接次数
 *
 * @return stat_m
 */
extern stat_m m_callable_network_get_connect_count(int *count);

/**
 * @brief 注册连接过程函数
 *
 * @param func 需要注册的回调函数
 * @param progress 连接状态
 * @param cause  原因
 * @return stat_m
 */
// extern stat_m m_callable_network_registered_progress_callback(void (*func)(enum connect_progress progress , enum net_cause cause));
/**
 * @brief 设置服务器的连接状态
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_set_server_status(int stus);
/**
 * @brief 设置服务器的登陆状态
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_set_server_login_status(bool stus);

/**
 * @brief 获取连接状态
 *
 * @return stat_m
 */
extern stat_m m_callable_network_get_net_connect_status(void);

/**
 * @brief 设置当前 wifi 连接的状态
 *
 * @return stat_m
 */
stat_m m_callable_network_set_connect_status(bool stus);
/**
 * @brief 获取服务器的登陆
 *
 * @return stat_m
 */
stat_m m_callable_network_manage_get_server_login_status(void);

/**
 * @brief 返回登陆相关信息
 *
 * @param out_cmd 指令
 * @param count 次数
 * @return stat_m
 */
stat_m m_ext_network_config_user_login_cmd(char *out_cmd, int count);

// /**
//  * @brief 获取服务器的连接状态
//  *
//  * @param connect_progress
//  * @return stat_m
//  */
// stat_m m_callable_network_manage_get_server_status(enum connect_progress *connect_progress);

/**
 * @brief Get the address and port (possibly http)
 *
 * @param in_addr address
 * @param in_port port
 * @return stat_m
 */
stat_m m_ext_network_get_target_addr(char *in_addr, int *in_port);
/**
 * @brief 获取是否已经恢复计划
 *
 * @return stat_m
 */
stat_m m_ext_network_get_device_is_recover_schedule();
/**
 * @brief 修改爲在線改變wifi的狀態
 *
 * @return stat_m
 */
extern stat_m m_callable_network_set_status_change_net(void);

/**
 * @brief network linking
 *
 * @param in_addr address
 * @param in_port port
 * @return stat_m
 */
stat_m m_ext_net_link_connection(char *in_addr, int in_port);

/**
 * @brief data send
 *
 * @param in_tx data
 * @param in_len length
 * @return stat_m
 */
stat_m m_ext_net_link_connection_tx(const char *in_tx, int in_len);
/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
stat_m m_ext_net_link_receive_data(char *out_data, int *len, uint64_t in_time_out_s);
/**
 * @brief data send
 *
 * @param in_tx data
 * @param in_len length
 * @return stat_m
 */
stat_m m_ext_network_link_transformission_mqtt_tx(const char *in_tx, int in_len);
/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
extern stat_m m_ext_net_link_mqtt_receive_data(char *out_data, int *len, uint64_t in_time_out_s);

/**
 * @brief 注意这个个函数，返回如果是成功，会被内部判定为认证成功，没有认证成功 会不断重复 在一段时间之内
 *
 * @return stat_m
 */
stat_m m_ext_network_manage_login(enum connect_mode sm_mode, int count);
/**
 * @brief 网络层初始化   仅仅初始化即可
 *
 * @param nm  网络初始化方式
 * @param sm  服务器连接初始化方式
 * @return stat_m
 */
stat_m m_ext_drive_net_manage_init(enum running_mode nm, enum connect_mode sm);

/**
 * @brief Enable the link service (if the device does not support the network can take this function as... Bluetooth initialization)
 *
 */
void m_callable_net_connect_server_enable(void);

/**
 * @brief 收到的消息处理
 *
 * @param cmd 消息CMD
 * @param buf 消息内容
 * @return stat_m
 */
stat_m m_ext_server_message_handle(int cmd, char *buf);

/**
 * @brief 发送指令到服务器
 *
 * @param in_tx_data 数据内容，这个是需要包含【CMD,】#@#
 * @param in_tx_len 长度
 * @param in_spec_cmd 单独的指令int格式
 * @param in_spec_cmd_flag 这个忘记了
 * @param in_timeout 指令发送超时。不加入队列，该参数无效
 * @param in_tx_queue 是否加入队列发送
 * @example  m_callable_spec_scene_net_connext_tx(cmd_data, cmd_data_len, cmd, cpcmd, 0, true);
 * @return stat_m
 */
stat_m m_callable_spec_scene_net_connext_tx(const char *in_tx_data, int in_tx_len, uint64_t in_spec_cmd, uint64_t in_spec_cmd_flag, uint64_t in_timeout, bool in_tx_queue);
/** 参数类型 */
enum m_paramete_enum;
/**
 * @brief 向远端发送一个数据 以多个参数的形式
 *
 * @param cmd 指令
 * @param ty1 参数类型 1
 * @param arg1 参数 1
 * @param ty2  类推
 * @param arg2 类推
 * @param ty3 类推
 * @param arg3 类推
 * @param ty4 类推
 * @param arg4 类推
 * @param in_time_s 类推
 * @return stat_m
 */
stat_m m_callable_local_resp_to_remote(int cmd,
                                       enum m_paramete_enum ty1, void *arg1,
                                       enum m_paramete_enum ty2, void *arg2,
                                       enum m_paramete_enum ty3, void *arg3,
                                       enum m_paramete_enum ty4, void *arg4,
                                       uint64_t in_time_s, bool is_queue);

/**
 * @brief 反馈数据到远端 pro版本
 *
 * @param cmd 指令
 * @param ty1 参数1类型
 * @param arg1 参数1数据
 * @param ty2 参数2类型
 * @param arg2 参数2数据
 * @param ty3 参数3类型
 * @param arg3 参数3数据
 * @param ty4 参数4类型
 * @param arg4 参数4数据
 * @param ty5 参数5类型
 * @param arg5 参数5数据
 * @param ty6 参数6类型
 * @param arg6 参数6数据
 * @param ty7 参数7类型
 * @param arg7 参7数据
 * @param in_time_s
 * @return stat_m
 */
stat_m m_callable_local_resp_to_remote_pro(int cmd,
                                           enum m_paramete_enum ty1, void *arg1,
                                           enum m_paramete_enum ty2, void *arg2,
                                           enum m_paramete_enum ty3, void *arg3,
                                           enum m_paramete_enum ty4, void *arg4,
                                           enum m_paramete_enum ty5, void *arg5,
                                           enum m_paramete_enum ty6, void *arg6,
                                           enum m_paramete_enum ty7, void *arg7,
                                           uint64_t in_time_s);
/**
 * @brief 反馈参数到远端 pro-max版本
 *
 * @param cmd 指令
 * @param ty1 参数1类型
 * @param arg1 参数1数据
 * @param ty2 参数2类型
 * @param arg2 参数2数据
 * @param ty3 参数3类型
 * @param arg3 参数3数据
 * @param ty4 参数4类型
 * @param arg4 参数4数据
 * @param ty5 参数5类型
 * @param arg5 参数5数据
 * @param ty6 参数6类型
 * @param arg6 参数6数据
 * @param ty7 参数7类型
 * @param arg7 参7数据
 * @param ty8 参数8类型
 * @param arg8  参数8数
 * @param ty9 参数9类型
 * @param arg9  参数9数
 * @param ty10 参数10类型
 * @param arg10  参数10数
 * @param ty11 参数11类型
 * @param arg11  参数11数
 * @param ty12 参数12类型
 * @param arg12  参数12数
 * @param in_time_s 时间
 * @return stat_m
 */
stat_m m_callable_local_resp_to_remote_pro_max(int cmd,
                                               enum m_paramete_enum ty1, void *arg1,
                                               enum m_paramete_enum ty2, void *arg2,
                                               enum m_paramete_enum ty3, void *arg3,
                                               enum m_paramete_enum ty4, void *arg4,
                                               enum m_paramete_enum ty5, void *arg5,
                                               enum m_paramete_enum ty6, void *arg6,
                                               enum m_paramete_enum ty7, void *arg7,
                                               enum m_paramete_enum ty8, void *arg8,
                                               enum m_paramete_enum ty9, void *arg9,
                                               enum m_paramete_enum ty10, void *arg10,
                                               enum m_paramete_enum ty11, void *arg11,
                                               enum m_paramete_enum ty12, void *arg12,
                                               uint64_t in_time_s);
/*******************************************************/
/*******************************************************/
/******************* WIFI ****************************/
/*******************************************************/
/*******************************************************/

/**
 * @brief wifi 软件驱动初始化
 *
 * @return stat_m
 */
stat_m m_ext_soft_drive_net_wifi_init(void);
/**
 * @brief wifi ip 地址 更新
 *
 * @return stat_m
 */
stat_m m_ext_software_drive_net_wifi_ip_update(void);
/**
 * @brief 断开wifi连接
 *
 * @return stat_m
 */
stat_m m_ext_soft_drive_net_wifi_dis_connect(void);
/**
 * @brief 扫描一个wifi
 *
 * @param in_ssid wifi ssid
 * @return stat_m
 */
stat_m m_ext_soft_drive_net_wifi_sta_scan_spec(char *in_ssid);
/**
 * @brief 连接到指定的wifi
 *
 * @param SSID  ssid
 * @param PASSW  password
 * @return stat_m
 */
stat_m m_ext_software_drive_net_wifi_sta_connect_to(const char *SSID, const char *PASSW);
/**
 * @brief  dhcp 服务
 *
 * @return stat_m
 */
stat_m m_ext_soft_drive_dhcp_option();
/**
 * @brief 设置静态参数相关
 *
 * @param in_glo_ip_add IP地址
 * @param in_glo_sub_net_mask 子网掩吗
 * @param in_glo_gateway 网关
 * @param in_glo_dns DNS
 * @return stat_m
 */
stat_m m_ext_soft_drive_net_wifi_set_static_ip(uint32_t *in_glo_ip_add, uint32_t *in_glo_sub_net_mask, uint32_t *in_glo_gateway, uint32_t *in_glo_dns);

/**
 * @brief auto wifi ap
 * @warning In order to ensure reliable links, it is recommended to call in the timer
 * @return stat_m
 */
stat_m m_callable_wifi_auto_connect(void);
/**
 * @brief 设置开启或者关闭自动连接
 *
 * @return stat_m
 */
extern stat_m m_callable_network_set_auto_connect(bool enable);
/**
 * @brief 获取wifi信号强度
 *
 * @param sign_strage
 * @return stat_m
 */
stat_m m_ext_soft_drive_net_wifi_get_sign_strage(int *sign_strage);
/*******************************************************/
/*******************************************************/
/******************* NwtWork Adapter *******************/
/*******************************************************/
/*******************************************************/
/**
 * @brief 网络适配器初始化
 *
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_init(void);
/**
 * @brief 清除网络适配器
 *
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_clear(void);
/**
 * @brief 获取网络适配器内容
 *
 * @param in_ip_add
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_get_info(uint32_t *in_ip_add);
/**
 * @brief 设置网络适配器
 *
 * @param in_ip_add
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_set_info(uint32_t *in_ip_add, uint32_t *in_gw_info);
/**
 * @brief 自主动设计的静态IP信息
 *
 * @param in_ip_add 192.168.0.1
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_set_static_ip(char *in_ip_add, char *in_sub_mask_add, char *in_getway_add, char *in_in_dns);
/**
 * @brief 获取静态IP信息
 *
 * @param in_ip_add 输出的信息
 * @return stat_m
 */
stat_m m_callable_middle_network_adapter_get_static_ip(char *out_ip_add, char *out_sub_mask_add, char *out_getway_add, char *in_in_dns);

/**
 * @brief
 *
 * @param connect_option
 * @return int
 */
void m_callable_middle_network_net_connection_option_set_info(int connect_option);
/**
 * @brief 获取
 *
 * @return int
 */
int m_callable_middle_network_net_connection_option_get_info();
/*******************************************************/
/*******************************************************/
/******************* Bluetool **************************/
/*******************************************************/
/*******************************************************/

extern stat_m m_ext_soft_drive_net_bluetool_init(void);
/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
extern stat_m m_ext_net_link_bluetool_receive_data(char *out_data, int *len, uint64_t in_time_out_s);
/**
 * @brief 离线蓝牙模式 手机与设备 进行 登录认证
 * @param token 登录认证密钥
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login_authentication(char *token);

/**
 * @brief 离线蓝牙模式 手机登录
 * @param tmp_sid 登录认证密钥
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login(char *tmp_sid);
/**
 * @brief 离线蓝牙模式  设备向手机返回设备状态
 * @param void
 * @return device_status
 */
enum device_status m_callable_offline_bluetooth_get_device_status(void);

/**
 * @brief 离线蓝牙模式  蓝牙登录认证是否成功 标志位
 * @param void
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login_flag(void);

/**
 * @brief 离线蓝牙模式 手机与设备 解除绑定
 * @param token 登录认证密钥
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_unbind(void);

/**
 * @brief 离线蓝牙模式 手机与设备 解除绑定 标志位设置
 * @return stat_m
 */
bool m_callable_offline_bluetooth_unbind_flag_get(void);

/**
 * @brief 离线蓝牙模式 手机与设备 绑定
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_bind_set(void);

/**
 * @brief   筛选出在离线蓝牙模式中，需要直接返回结果的指令
 * @param in_spec_cmd
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_instruction_filtering(uint64_t in_spec_cmd);

/**
 * @brief 在离线蓝牙模式下，判断是否处于登录状态
 * @param void
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_logged_or_not_logged(void);
/*******************************************************/
/*******************************************************/
/******************* Config Net ************************/
/*******************************************************/
/*******************************************************/
/**
 * @brief 配置设备的入网 username
 *
 * @param in_psw
 * @param len
 * @return stat_m
 */

stat_m m_callable_config_net_set_username(char *in_username);

/**
 * @brief 配置设备的token（密码）
 *
 * @param in_psw 密码
 * @param len 长度
 * @return stat_m
 */

stat_m m_callable_config_net_set_token(char *in_token);

/**
 * @brief 开始配网
 *
 */
stat_m m_callable_config_net_start(void);

/**
 * @brief 配置网络成功或者失败的反馈
 *
 * @param stat
 * @return stat_m
 */
extern stat_m m_callable_config_net_resp(stat_m stat);
/**
 * @brief 配网断开连接
 *
 * @return stat_m
 */
stat_m m_callable_config_net_recode(void);
/**
 * @brief 保存
 *
 * @return stat_m
 */
stat_m m_callable_config_net_save(void);
/**
 * @brief 吧指定字符替换
 *
 * @param strc
 * @return stat_m
 */
extern stat_m m_callable_str_transf_str_resp(char *strc);
/**
 * @brief 设置设备状态 为离线改Wifi
 *
 */
void m_ext_network_set_device_login_state();
/*******************************************************/
/*******************************************************/
/********************Server Connect ********************/
/*******************************************************/
/*******************************************************/

/**
 * @brief mqtt 服务器连接
 *
 */
void m_ext_software_drive_server_mqtt_init(void);
/**
 * @brief mqtt 服务器连接
 *
 */
stat_m m_ext_software_drive_server_tcp_init(void);

/**
 * @brief 连接 TCP 的软件驱动
 *
 * @param in_addr
 * @param in_port
 */
stat_m m_ext_software_drive_server_tcp_connect(char *in_addr, int in_port);
/**
 * @brief 连接 Mqtt 的软件驱动
 *
 * @param in_addr
 * @param in_port
 */
stat_m m_ext_software_drive_server_mqtt_connect(char *in_addr, int in_port);

/*******************************************************/
/*******************************************************/
/**********************Event Loop***********************/
/*******************************************************/
/*******************************************************/

/**
 * @brief create event group
 * @param func
 */
void m_callable_event_group_loop_enable(void (*func)(int, void *));
/**
 * @brief post notify event
 *
 * @param event_id event id
 * @param in_data  parameter
 * @return stat_m
 */
stat_m m_callable_event_post(int event_id, void *in_data);

/*******************************************************/
/*******************************************************/
/********************Global State***********************/
/*******************************************************/
/*******************************************************/
/**
 * @brief If you want to record a state
 *
 * @param defin The status of the label
 * @param stat state
 */
void put_in(uint16_t defin, stat_m stat);

/**
 * @brief Query for multiple states at a time
 *
 * @param left start
 * @param right end
 * @return stat_m
 */
stat_m info_check(uint8_t left, uint8_t right);

/**
 * @brief Query a state
 *
 * @param vue id
 * @return stat_m
 */
stat_m out_check(uint8_t vue);

/*******************************************************/
/*******************************************************/
/**********************OTA******************************/
/*******************************************************/
/*******************************************************/

/**
 * @brief Set the ota update url
 *
 * @param vue url
 * @return stat_m
 */
stat_m m_ext_ota_set_url(char *in_url);

/**
 * @brief ota start (ready)
 *
 * @return stat_m
 */
stat_m m_callable_ota_start(void);

/**
 * @brief ota  (ready)
 *
 * @return stat_m
 */
stat_m m_ext_ota_prepare(void);

/**
 * @brief ota  (ready)
 *
 * @return stat_m
 */
stat_m m_ext_ota_start(void);
/*******************************************************/
/*******************************************************/
/**********************Err_Handle*****************************/
/*******************************************************/
/*******************************************************/
/**
 * @brief 设置错误信息
 *
 * @param ecd
 * @return stat_m
 */
stat_m m_callable_middle_err_handle_set_code(enum error_net_code ecd);
/**
 * @brief 输出信息到服务器
 *
 * @return stat_m
 */
stat_m m_callable_middle_err_handle_printf(void);
/*******************************************************/
/*******************************************************/
/**********************Http*****************************/
/*******************************************************/
/*******************************************************/

/**
 * @brief Read the data from the http stream
 *
 * @return stat_m
 */
int m_ext_http_read_data(char *out_data, int in_len);

/**
 * @brief Open an http link
 *
 * @return stat_m
 */
stat_m m_ext_http_openlink(char *in_url, char *in_head, char *in_vue, uint64_t *online_ota_range_sum, uint64_t in_timeout_ms);

////////////////////
/**
 * @brief schedule steoage
 *
 * @param in_info 传入标准的存储结构
 *
 * @return stat_m
 */
stat_m m_splan_add_item(m_schedule_info *in_info);

/*******************************************************/
/*******************************************************/
/*******************Config Net**************************/
/*******************************************************/
/*******************************************************/

///      udp
/**
 * @brief Enable distribution udp  network function
 *
 * @return stat_m
 */
stat_m m_callable_config_net_to_udp_connect_enable(void);

/**
 * @brief External implementation of the udp creation
 *
 * @return stat_m
 */
stat_m m_callable_config_net_to_udp_connect_receive_handle(char *in_data, int in_len);

stat_m m_ext_config_net_connect_resp(char *in_resp);
/**
 * @brief External implementation of the udp creation
 *
 * @return stat_m
 */
stat_m m_ext_config_net_to_udp_connect_create(int in_port);

/*******************************************************/
/*******************************************************/
/*******************System******************************/
/*******************************************************/
/*******************************************************/
/**
 * @brief A task is abandoned for a period of time
 *
 * @return stat_m
 */
void mDelay_ms(uint32_t in_ms);
/**
 * @brief 电流检测延时
 *
 * @param in_ms
 */
void mDelay_precise_ms(uint32_t in_ms);
/**
 * @brief 获取时间戳
 *
 * @return uint64_t
 */
uint64_t mDeviceSystemTime(void);
/**
 * @brief Create a task
 * @param fun task
 * @param pcName task name
 * @param stack_depth stack size
 * @param pvParameters param
 * @param uxPriority priority
 * @param pvCreatedTask handle
 * @return stat_m
 */
void *mTaskCreate(void *handle, void (*fun)(void *), const char *pcName, const uint32_t stack_depth, void *const pvParameters, int uxPriority, void *pvCreatedTask);
/**
 * @brief 重启函数
 *
 * @param cause 重启原因
 */
void mReboot(enum restart_cause_t cause);
/**
 * @brief 删除任务
 *
 * @return void
 */
void mTaskDelete(void);
/**
 * @brief bit  set
 *
 * @return stat_m
 */
void mEventGroupSetBits(void *event_loop, const int thisBit);

/**
 * @brief bit wait
 *
 * @return stat_m
 */
int m_callable_event_group_wait_bits(void *event_loop, const int thisBit, int waitForAllBits);
/**
 * @brief bit reset
 *
 * @return stat_m
 */
void mEventGroupReSetBits(void *event_loop, const int thisBit);
/**
 * @brief printf heap info
 *
 */
void m_callable_system_printf_heap_free_info(const char *fun, int line);
/**
 * @brief 打印资源占用 任务
 *
 */
void m_callable_system_printf_task_info();
/**
 * @brief 获取堆栈使用情况
 *
 * @param out_heap_size 栈内存
 * @param min_heap_size 堆内存
 */
void m_callable_system_printf_to_remote_heap_free_info(int *out_heap_size, int *min_heap_size);
/*******************************************************/
/*******************************************************/
/******************State machine************************/
/*******************************************************/
/*******************************************************/

/**
 * @brief fsm init
 *
 * @param in_trig_acti_fun_handle
 * @return stat_m
 */

stat_m m_callable_schedule_fsm_init(m_fsm_event_handle_fun in_trig_acti_fun_handle);
/**
 * @brief post event
 *
 * @param evt event
 * @param in_sub_event_id sub-class event id
 * @param pdata data
 * @return stat_m
 */
stat_m m_callable_schedule_fsm_post_event(m_fsm_events evt, int in_sub_event_id, void *pdata, uint64_t);

/*******************************************************/
/*******************************************************/
/****************** Schedule ***************************/
/*******************************************************/
/*******************************************************/

/**
 * @brief Event trigger entry when scheduled at runtime
 * @param event_id Event id
 * @param trigger_time triggering time
 * @todo Unit Input ...
 *
 * @return stat_m
 *          @retval fail_r Execution was successful but may not take effect
 *          @retval succ_r Execution was successful
 */
stat_m m_callable_instance_running_event_input(enum mschedule_event event_id, uint64_t trigger_time);
/**
 * @brief 清除运行数据
 *
 * @return stat_m
 */
stat_m m_static_instance_manage_clean_current_instance(void);

/**
 * @brief Read all existing schedule from flash, Contains disabled and enabled schedules,Start from 0 to MAX_SCHEDU_NUM, after which it returns failure, and the pointer returns to 0 after receiving the failure
 * @todo Unit Out ...
 *
 * @return stat_m M_ERR: ：Indicates that the content has been restored
 */
extern stat_m m_callable_schedule_stroage_load_all_from_flash(void);
;

// /**
//  * @brief 表示计划应该请求运行参数和跳过信息了
//  *
//  * @param in_obj 计划
//  * @param in_trigger_timing 触发时机
//  * @return stat_m
//  */
// stat_m m_ext_eigenfunction_prepare(instance_doc_t *in_obj, int in_trigger_timing);

// /**
//  * @brief 该运行吗，奇数偶数参数
//  *
//  * @param in_obj
//  * @param in_trigger_timing
//  * @return stat_m
//  */
// stat_m m_ext_eigenfunction_should_run(instance_doc_t *in_obj, int in_trigger_timing);

// /**
//  * @brief 检测跳过需要
//  *
//  * @param in_wobj
//  * @param in_trigger_timing
//  * @return stat_m
//  */
// stat_m m_ext_eigenfunction_should_skip(instance_doc_t *in_wobj, int in_trigger_timing);

/**
 * @brief 停掉在运行的快速手动运行
 *
 * @param in_wobj
 * @param in_trigger_timing
 * @return stat_m
 */
stat_m m_ext_eigenfunction_program_conflict_processing(instance_doc_t *in_wobj, int in_trigger_timing);
/**
 * @brief
 *
 * @return stat_m
 */
extern stat_m m_callable_instance_manage_init(void);
/**
 * @brief 计划的初始化
 *
 * @param in_info
 * @return stat_m
 */
stat_m m_callable_schedule_virtual_init(instance_doc_t *in_info);
/**
 * @brief 计划添加到列表
 *
 * @param mwc_d 计划内容
 * @return stat_m
 */
stat_m m_callable_schedule_list_add(instance_doc_t *mwc_d);
/**
 * @brief 資格是監控計劃執行的，必須循環調用 比如開個喜愛年成每個1s一次
 *
 * @param current_time
 * @return stat_m
 */
stat_m m_callable_instance_running_monitor(uint64_t current_time);
// /**
//  * @brief 计划从列表移除全部
//  *
//  * @param in_uid 计划id
//  * @return stat_m
//  */
// stat_m  m_callable_schedule_list_remove_all(void);
/**
 * @brief 计划添加
 *
 * @param src_str
 * @param in_time_s
 * @return stat_m
 */
extern stat_m m_callable_instance_manage_add(char *src_str, enum inst_add_mode of_line, uint64_t in_time_s, uint64_t *out_uid);
/**
 * @brief 获取当前实例的运行 id 和 区域运行 id
 *
 * @param out_sh_id 输出的实例 id
 * @param out_ch_id 输出的区域 id
 * @return stat_m
 */
extern stat_m m_callable_instance_manage_get_current_instance_sid_abd_cid(uint64_t *out_sh_id, int *out_ch_state);

/**
 * @brief 获取当前实例的运行 id 和 区域运行 id
 *
 * @param device_state 传入设备状态
 * @param out_sh_id 输出的计划 id
 * @param out_instance_id 输出的实例 id
 * @param ch_id 输出的区域 id
 * @param remainder_time 剩余运行时间
 * @param out_ch_state 输出的状态
 * @param out_running_queue 运行队列
 * @param out_running_queue_local  运行队列的位置
 * @return stat_m
 */
stat_m m_callable_instance_manage_get_current_instance_sid_abd_cid_adb_remainder_run_time(int device_state,
                                                                                          uint64_t *out_sh_id,
                                                                                          uint64_t *out_instance_id,
                                                                                          int *ch_id,
                                                                                          uint64_t *remainder_time,
                                                                                          int *out_ch_state,
                                                                                          char *out_running_queue,
                                                                                          int *out_running_queue_local,
                                                                                          uint64_t in_time_s);

/**
 * @brief 一键清除所有计划
 *
 */
extern stat_m m_callable_instance_manage_remove_all(void);
/**
 * @brief
 *
 * @param sch_id
 * @return stat_m
 */
extern stat_m m_callable_instance_manage_remove(uint64_t sch_id, uint64_t trig_time);
/**
 * @brief 计划从列表移除
 *
 * @param in_uid 计划id
 * @return stat_m
 */
stat_m m_callable_schedule_list_remove(uint64_t in_uid);
/**
 * @brief 计划数据清除
 *
 * @param in_uid 计划id
 * @return stat_m
 */
stat_m m_callable_schedule_running_buf_clear(instance_doc_t *wobj);
/**
 * @brief 启用一个计划
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_enable(uint64_t in_uid);
/**
 * @brief 禁用一个计划
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_disable(uint64_t in_uid);
/**
 * @brief 添加计划
 *
 * @param in_info 计划内容
 * @param in_enable 显式还是隐式添加
 * @return stat_m
 */
stat_m m_ext_schedule_add(instance_doc_t *in_info, bool in_enable);
/**
 * @brief 删除计划
 *
 * @param in_id 计划id
 * @return stat_m
 */
stat_m m_ext_schedule_remove(uint64_t in_id);

/**
 * @brief one sec cb
 *
 * @return stat_m
 */
stat_m m_ext_device_time_cb(void);

/**
 * @brief one sec cb
 *
 * @param in_obj_id 计划id
 * @return stat_m
 */
stat_m m_callable_schedule_disable(uint64_t in_obj_id);
/**
 * @brief one sec cb
 *
 * @param in_obj_id 计划id
 * @return stat_m
 */
stat_m m_callable_schedule_enable(uint64_t in_obj_id);

stat_m m_callable_schedule_flag_disable(uint64_t in_obj_id);

/**
 * @brief 打印所有
 *
 * @return stat_m
 */
stat_m m_static_instance_list_query_print_all(void);

/**
 * @brief 根据 30 返回的参数生成对象的运行实例
 *
 * @param in_uid 对象的id
 * @param in_time_s 发生的时间
 * @return stat_m 成功或者失败
 */
extern stat_m m_callable_instance_running_gener_obj(uint64_t in_uid, uint64_t in_time_s);

/**
 * @brief 刷新指定计划内容的时间
 *
 * @param in_type 指定类型
 * @return stat_m
 */
extern stat_m m_callable_instance_manage_update_spec_schedule_time(int in_type);
/*******************************************************/
/*******************************************************/
/****************** Event ********************************/
/*******************************************************/
/*******************************************************/

/**
 * @brief  在线模式 或者说是正常模式 配网模式   的事件处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_online_config_mode_event_handle(enum event_ft event_id, char *in_data, int in_len);
/**
 * @brief  离线 配网模式   的事件处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_offline_config_mode_event_handle(enum event_ft event_id, char *in_data, int in_len);
/**
 * @brief  直接模式   的事件处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_derect_config_mode_event_handle(enum event_ft event_id, char *in_data, int in_len);
/**
 * @brief  离线模式   的事件处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_derect_offline_mode_event_handle(enum event_ft event_id, char *in_data, int in_len);

/**
 * @brief  直接连接模式的处理
 *
 * @param in_data 收到的数据
 * @param in_len 数据长度
 * @return stat_m
 */
stat_m m_ext_net_server_event_handle(char *in_data, int in_len);

/**
 * @brief 快速搭建一个实例 运行
 *
 * @param in_src
 * @param in_time_s
 * @return stat_m
 */
stat_m m_callable_instance_manage_quick_gener(char *in_src, uint64_t in_time_s);

/**
 * @brief  当所有的配置发生改变的时候 将由他 进行处理
 *
 * @param event_id 事件ID
 * @param event_data 事件数据
 * @param in_time_s 触发时间
 * @return stat_m
 */
stat_m m_callable_device_attribute_config_change_notify_event_callable(enum device_function_change_notify_event event_id, void *event_data, uint64_t in_time_s);

/**
 * @brief 重置设备
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_all_clear(void);
/**
 * @brief 获取设备类型
 *
 * @param b 得到的类型 A001
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_device_type(int *);
/**
 * @brief 设置开始连接服务器
 *
 * @param stus
 * @return stat_m
 */
extern stat_m m_callable_network_set_server_start_connect_server(void);
/*******************************************************/
/*******************************************************/
/****************** Key ********************************/
/*******************************************************/
/*******************************************************/

/**
 * @brief Construct a new m callable key init object
 * @todo Unit Input ...
 *
 */
extern stat_m m_callable_drive_button_init(uint8_t device_type);
/**
 * @brief
 * @todo Unit Input ...
 *
 * @param time_s
 * @return stat_m
 */
extern stat_m m_callable_manual_set_auto_stop_s(uint32_t time_s);

/**
 * @brief Construct a new m callable key init object
 * @todo Unit Input ...
 *
 */
extern stat_m m_callable_drive_button_function_monitor(uint64_t current_time_ms);

/**
 * @brief 事件入口
 * @todo Unit Input ...
 *
 * @param kry_event_id
 * @param current_time_ms
 * @return stat_m
 */
extern stat_m m_callable_drive_button_event_input(enum key_id kry_event_id);
/**
 * @brief 直接驱动时间的调用 而不是硬件响应
 *
 * @param key_event_id
 * @return stat_m
 */
extern stat_m m_callable_drive_button_event_direct_input(enum key_id key_event_id);
/**
 * @brief 直接停止一个触发
 *
 * @return stat_m
 */
extern stat_m m_callable_drive_button_direct_stop(void);
/**
 * @brief  按键事件处理
 *
 * @param key_id 按键id
 * @param pre_key_index 操作前的按键id
 * @param current_key_index 操作后的按键id
 * @param current_time_ms 当前的时间
 * @return stat_m
 */
stat_m m_ext_drive_button_event_handle(enum key_id key_id, enum key_event key_event, uint8_t pre_key_index, uint8_t current_key_index, uint64_t current_time_ms);

/**
 * @brief 按键的GPIO初始化
 *
 * @return stat_m
 */
extern stat_m m_ext_drive_button_gpio_and_interrupt_init(void);
/**
 * @brief 获取GPIO的高低
 *
 * @return int
 */
int m_ext_drive_gpio_get_level();
/**
 * @brief 重置触发
 *
 * @return stat_m
 */
stat_m m_ext_drive_button_reset_trig(uint64_t);
/*******************************************************/
/*******************************************************/
/****************** Manual  *************************/
/*******************************************************/
/*******************************************************/
/**
 * @brief
 *
 * @param event_id
 * @param pre_channel
 * @param next_channel
 * @param current_time_ms
 * @return stat_m
 */
stat_m m_callable_manual_event_input(enum key_id event_id, uint8_t pre_channel, uint8_t next_channel, uint64_t current_time_ms);
/**
 * @brief
 *
 * @param event_id
 * @param pre_channel
 * @param next_channel
 * @param has_benn_runnint_time
 * @param current_time
 * @return stat_m
 */
stat_m m_ext_manual_event_handle(enum operate_event event_id, uint8_t pre_channel, uint8_t next_channel, uint32_t has_benn_runnint_time, uint64_t current_time);

/**
 * @brief 事件输入
 *
 * @param event_id
 * @param current_time_s
 * @return stat_m
 */
stat_m m_callable_manual_event_export_input(int event_id, int evt, uint64_t current_time_s);
/*******************************************************/
/*******************************************************/
/****************** Date parse *************************/
/*******************************************************/
/*******************************************************/

/**
 * @brief 得到一个 JSON 相关的 键值
 *
 * @param in_key 键
 * @param out_resp 解析出的值
 * @param in_src 源 json
 * @return stat_m
 */
stat_m m_callable_json_data_parse_foramt_one_key(char *in_key, char *out_resp, char *in_src);
/*******************************************************/
/*******************************************************/
/****************** Fsat Run ***************************/
/*******************************************************/
/*******************************************************/
/**
 * @brief 动作发生的事件
 *
 * @param cmd
 * @param in_id
 * @param in_chid
 * @param in_shouldtime
 * @param in2_id
 * @param in2_chid
 * @param in2_shouldtime
 * @param auutime
 * @return stat_m
 */
stat_m m_ext_fast_run_event_noticy(int cmd, uint64_t in_id, uint8_t in_chid, uint32_t in_shouldtime, uint64_t in2_id, uint8_t in2_chid, uint32_t in2_shouldtime, uint64_t auutime);
/**
 * @brief 控制电磁阀的动作
 *
 * @param in_ch
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_fast_run_action(uint8_t in_ch, bool on_off);

/**
 * @brief
 *
 * @param in_fastsrc
 * @param in_len
 * @return stat_m
 */
stat_m m_ext_fast_run_create(char *in_fastsrc, int in_len);

/**
 * @brief
 *
 * @param in_fastsrc
 * @param in_len
 * @return stat_m
 */
stat_m m_callable_fastRunInit(char *in_fastsrc, size_t in_len);

/**
 * @brief
 *
 * @param in_fastsrc
 * @param in_len
 * @return stat_m
 */
void m_callable_recove_schedule_comple(void);

/**   global zone  */

/**
 * @brief Load the global zone properties
 *
 * @return stat_m
 */
stat_m m_callable_properties_zone_global_load(void);

/**
 * @brief Global zone properties are disable
 *
 * @param in_pc zone id
 * @return stat_m
 */
stat_m m_callable_properties_zone_global_disable(uint8_t in_pc);
/**
 * @brief Global zone properties are enable
 *
 * @param in_id zone id
 * @return stat_m
 */
stat_m m_callable_properties_zone_global_enable(uint8_t in_pc);

stat_m m_callable_global_zone_init(void);

/*******************************************************/
/*******************************************************/
/****************** device profile *********************/
/*******************************************************/
/*******************************************************/
enum running_mode;
/**
 * @brief Loads device properties from flash，He calls the set function for various properties
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_init(void);
/**
 * @brief 设置设备的运行模式
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_running_set_mode(int mode, int connection_option);
/**
 * @brief 获取设备的运行模式
 *
 * @param out_rmode
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_running_mode(int *out_rmode);
/**
 * @brief 确定保存设备运行模式，保证下次设备灯光和连接方式
 *
 * @param mode 模式
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_running_save_mode(int mode);
/**
 * @brief 获取设备属性 连接模式
 *
 * @param out_rmode 输出的连接模式
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_net_connect_mode(int *out_rmode);

/**
 * @brief 设备时间设置
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_time(uint64_t loical_time, int tz);

/**
 * @brief 设备mac地址设置
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_mac_addr(char *in_mac_addr);
/**
 * @brief 设备mac地址获取
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_mac_addr(char *out_mac_addr);
/**
 * @brief 设备mac地址设置
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_mac_suffix_addr(char *in_mac_addr);
/**
 * @brief 设备mac地址获取
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_mac_suffix_addr(char *out_mac_addr);
/**
 * @brief 设置当前的硬件芯片输出的类型
 *
 * @param in_out_chip
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_out_chip(int in_out_chip);
/**
 * @brief 获取当前的硬件芯片输出的类型
 *
 * @param out_out_chip 类型
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_out_chip(int *out_out_chip);
/**
 * @brief 设置应用程序版本信息
 *
 * @param in_app_version 输入
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_app_version(char *in_app_version);
/**
 * @brief 获取应用程序版本信息
 *
 * @param out_app_version 输出
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_app_version(char *out_app_version);
extern stat_m m_callable_device_attribute_get_app_version_ext(char *out_app_version);

/**
 * @brief 获取当前的key
 *
 * @param out_app_version 输出
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_key(char *out_sn_version);

/**
 * @brief 设置当前的key吗
 *
 * @param out_app_version 输出
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_key(char *in_key_version);
/**
 * @brief 设置当前的sn吗
 *
 * @param out_app_version 输出
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_sn(char *in_sn);
/**
 * @brief 设置当前的sn吗
 *
 * @param out_app_version 输出
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_sn(char *out_sn);

/**
 * @brief 获取当前连接的wifi ssid
 *
 * @param out_app_version 输出
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_current_connect_net_name(char *net_name);
/**
 * @brief 判断当前WiFi是否和传入一致
 *
 * @param net_name
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_current_is_net_name_same(char *net_name);
/**
 * @brief 设置当前连接的wifi ssid
 *
 * @param out_app_version 输出
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_current_connect_net_name(char *net_name);
/**
 * @brief 获取当前的硬件版本  A001
 *
 * @param out_out_chip 类型
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_hearward_version(uint32_t *out_hear_ware);

/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_sign_strage(int a);
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_schedule_num(int a);
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_sign_strage(int *a);
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_schedule_num(int *a);
/**
 * @brief 改变设备的计划数量
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_change_schedule_num(int in_u_d);
/**
 * @brief 获取地址信息
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_local(double *lon, double *lat);
/**
 * @brief
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_local(double lon, double lat, int tz); /*  设备地址设置  */
/**
 * @brief 获取当前的时区
 *
 * @param b 得到的时区
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_device_timezone(int *tz);
/**
 * @brief 加载设备时区
 *
 * @param tz
 * @return stat_m
 */
stat_m m_callable_device_attribute_load_timezone_info();
/**
 * @brief 单独设置时区
 *
 * @param tz 时区
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_timezone(int tz);
/**
 * @brief Device channel configuration settings
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_hammer(int);
/**
 * @brief Device water hammer configuration get
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_hammer(void);
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_soledold(int a, int in_time);
/**
 * @brief Equipment pump configuration get
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_soledold(int *out_mode, int *out_time);

/**
 * @brief 设置设备的mac地址
 *
 * @param in_mac
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_set_mac(char *in_mac);
/**
 * @brief 获取设备的mac地址
 *
 * @param out_mac
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_mac(char *out_mac);
/**
 * @brief 获取设备rtc的RTC时间
 *
 * @return uint64_t
 */
uint64_t m_callable_device_attribute_get_rtc_time(void);

/**
 * @brief 确定保存设备代理服务器地址，保证下次连接正常
 *
 * @param mode 模式
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_save_addr(void);
/**
 * @brief 设置设备的代理服务器地址
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_set_addr(char *in_http_proxy_addr);
/**
 * @brief 获取设备的代理服务器地址
 *
 * @param out_rmode
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_get_addr(char *out_addr);
/**
 * @brief Find the channel that should run when using quickrun
 *
 * @param in_img schedule zone param
 * @param out_pc  next zone id
 * @param in_next_or_pre  true -> next  ; false  -> pre
 * @param in_max_zone  max run channel
 * @param pre_timesp  present time
 * @return stat_m
 */
// stat_m m_callable_zone_quick_run_switch_channel(m_chan_managec *in_mg, uint8_t *out_pc, bool in_next_or_pre, uint16_t in_max_zone, uint64_t pre_timesp);
// /**
//  * @brief
//  *
//  * @param in_img schedule zone param
//  * @param out_pc  next zone id
//  * @param in_max_zone  max run channel
//  * @param pre_timesp  present time
//  * @return stat_m
//  */
// stat_m m_callable_zone_global_next(m_chan_managec *in_mg, uint8_t *out_pc, uint32_t in_sack, uint16_t in_max_zone, uint64_t pre_timesp);

// stat_m m_callable_zone_global_available(m_chan_managec *in_mg, uint8_t *out_pc, uint32_t in_sack, uint16_t in_max_zone, uint64_t pre_timesp);

/** 判断是否还有下一个区域*/
stat_m m_callable_zone_have_next(instance_doc_t *mobj, uint64_t present_timesp);

/** */
stat_m m_callable_smartschedule_config(char *in_src, uint64_t in_in_skip_flag);

/** */
void m_rgb_all_clear(void);

/** */
void clearOneChannel(uint8_t channel);
/** */
void showOneChannel(uint8_t channel);

/***********************************************/
/**
 * @brief 设备生产模式
 *
 * @return stat_m
 */
stat_m m_callable_device_device_factoryconfig(void);

/***********************************************/
/***********************************************/
/**
 * @brief 设置一个区域开启或者关闭
 * @todo Unit Input ...
 *
 * @param channel
 * @return stat_m
 */
stat_m m_callable_channel_manage_state_set(bool enable, uint8_t channel, uint64_t start_time, uint64_t end_time);
/***********************************************/
/***********************************************/

/***********************************************/
/***********************************************/
/***********************************************/
/**
 * @brief 追加一个参数到smart 数组，  如果是数组的0
 *
 * @param in_uid 计划id
 * @param in_src 源格式数据   1|2|3|4,2#10;3#5
 * @param in_time_s 时间 （这个时间是很有必要的，超过15min的都会被清除）
 * @return stat_m
 */
extern stat_m m_callable_smart_running_storage_append(uint64_t in_uid, char *in_src, uint64_t in_time_s);

/**
 * @brief 获取区域浸泡配置信息
 * @todo Unit Out ...
 *
 * @param channel_id 区域id
 * @param cconfig A 或者 B
 * @param running_time  返回的运行时间
 * @param sack_time 返回的浸泡时间
 * @return stat_m
 */
extern stat_m m_callable_channel_manage_get_channel_param(uint8_t channel_id, enum channel_param_config cconfig, uint8_t *out_sack_type, uint32_t *running_time, uint32_t *sack_time,
                                                          float *out_ar, float *out_ir, float *out_asa);

/***********************************************/
/***********************************************/
/**
 * @brief 配置区域信息     格式 ：17*10*20,5*5*60,1]
 *
 * @param src_str 格式 ：17*10*20,5*5*60,1]
 * @return stat_m
 */
stat_m m_callable_channel_manage_congfig_sack_param(int in_channel, char *src_str);
/**
 * @brief 离线百分比设置，针对于所有区域
 * @todo Unit Input ...
 *
 * @param param
 * @return stat_m
 */

stat_m m_callable_channel_manage_set_offline_running_percent(char *percent);
/**
 * @brief 获取离线百分比参数
 * @todo Unit Input ...
 *
 * @param param
 * @return stat_m
 */

stat_m m_callable_channel_manage_get_offline_running_percent(int *percent);

/**
 * @brief 区域管理初始化
 *
 * @param device_type 是被通道数量
 * @return stat_m
 */
extern stat_m m_callable_channel_manage_init(int device_type);
/**
 * @brief 判断区域在当前时间是否应该有效（启用）
 *
 * @param channel 区域id
 * @param current_time 当前时间
 * @return stat_m
 */
stat_m m_callable_channel_manage_current_is_effective(uint8_t channel, uint64_t current_time);
/***********************************************/
/***********************************************/
/***********************************************/
/**
 * @brief
 * @todo Uint Input ...
 *
 * @return stat_m
 */
extern stat_m m_callable_device_dormant_init(void);
/**
 * @brief 设置休眠模式
 * @todo Uint Input ...
 *
 * @param start_time
 * @param end_time
 * @param mode
 * @return stat_m
 */
extern stat_m m_callable_device_dormant_set(uint64_t start_time, uint64_t end_time, uint8_t mode);
/**
 * @brief
 * @todo Uint Out ...
 *
 * @param current_time
 * @return true
 * @return false
 */
extern stat_m m_callable_device_dormant_current_is_effective(uint64_t current_time);
/**
 * @brief Test Dormant
 *
 * @param a Start Time
 * @param b End Time
 */
extern void m_test_dormant_set(uint64_t a, uint64_t b);
/***********************************************/
/***********************************************/
/**
 * @brief 设备状态提更新到 初始化状态
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_init(void);
/**
 * @brief 设备状态提更新到 在线状态
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_idle(void);

/**
 * @brief 设备状态提更新到 快速运行状态
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_fast_running(void);
/**
 * @brief 设备状态提更新到 计划状态
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_schedule_running(void);

/**
 * @brief 设备状态提更新到 手动运行状态
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_manual_running(void);
/**
 * @brief 设备状态提更新到 更新状态
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_update(void);
/**
 * @brief 设备状态提更新到 短路
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_short_M_pump();
/**
 * @brief 设备状态提更新到 更新状态
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_test(void);
/**
 * @brief 设备状态提更新到 配置状态
 *
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_update_to_config_net(void);

/**
 * @brief 查询当前设备的状态
 *
 * @param out_ste 输出
 * @return stat_m
 */
extern stat_m m_callable_device_proper_status_get(int *out_ste);
/***********************************************/
/***********************************************/

/**
 * @brief TEA-加密数据，输入代价数据，输出base64
 *
 * @param in_enda 输入数据
 * @param in_len 数据长度
 * @param out_dat 数据数据 bate64
 * @return stat_m
 */
stat_m m_callable_crypt_xxtea_encrypt(char *in_enda, int *in_len, char *out_dat);

/**
 * @brief TEA-解密数据，输入加密后的数据，输出字符
 *
 * @param in_deda 输入待解密数据
 * @param in_len 数据长度
 * @param out_dat 输出数据
 * @return stat_m
 */
stat_m m_callable_crypt_xxtea_decrypt(char *in_deda, int *in_len, char *out_dat);

/***********************************************/
/***********************************************/
/***********************************************/
/**
 * @brief Load information from memory
 * @todo Unit Input ...
 *
 * @return stat_m
 */
extern stat_m m_callable_schedule_skip_init(uint64_t in_sid);
/**
 * @brief 跳过管理初始化，这个函数在射比初始化必须被调用 ，因为他会调用其他包括休眠计划跳过和waterdelay的数据加载
 *
 * @return stat_m
 */
extern stat_m m_callable_skip_manage_init(void);
/**
 * @brief Configure the start time and end time of water-delay
 * @todo Unit Input ...
 *
 * @param sch_id schedule id
 * @param start_time start time
 * @param end_time end time
 * @return stat_m
 */
extern stat_m m_callable_schedule_skip_set(uint64_t sch_id, uint64_t start_time, uint64_t end_time, uint64_t in_time_s);

/**
 * @brief Determine whether the current time is within the water-delay range
 * @todo Unit Out ...
 *
 * @param sch_id schedule id
 * @param current_time  Current time
 * @return stat_m
 */
extern stat_m m_callable_schedule_skip_current_is_effective(uint64_t sch_id, uint64_t in_time_s);
/**
 * @brief   全局查询计划是否应该跳过
 * @param in_id 计划id
 * @param skip_type
 *              包含
 *                  1、是否有休眠模式
                    2、是否有waterdelay
                    3、是否有计划跳过
                    4、是否有传感器跳过
                    5、是否有天气跳过
                    6、是否有饱和跳过
 *
 * @param sen_chan 传感器通道
 * @param sen_type 传感器类型
 * @param weather_code 天气跳过代码
 * @param in_time_s 发生的时间
 * @return stat_m
 */
extern stat_m m_callable_skip_manage_query_is_effective(uint64_t in_id, enum skip_manage_type *skip_type, enum seneor_chann *sen_chan, enum seneor_type *sen_type, int *weather_code, uint64_t in_time_s);
/**
 * @brief 清除计划跳过信息
 *
 * @param sch_id 计划id
 * @return stat_m
 */
extern stat_m m_callable_schedule_skip_clean(uint64_t sch_id);
/***********************************************/
/***********************************************/
/**
 * @brief Determine whether the current time is within the water-delay range
 * @todo Unit Out ...
 *
 * @param current_time  Current time
 * @return stat_m
 */
stat_m m_callable_water_delay_current_is_effective(uint64_t current_time);

/**
 * @brief Configure the start time and end time of water-delay
 * @todo Unit Input ...
 *
 * @param start_time start time
 * @param end_time end time
 * @return stat_m
 */
stat_m m_callable_water_delay_set(uint64_t start_time, uint64_t end_time, uint64_t in_time_s);
/**
 * @brief 清除计划跳过
 *
 * @return stat_m
 */
stat_m m_callable_water_delay_clean(void);

/***********************************************/
/***********************************************/
/**
 * @brief 查看计划的天气运行信息
 *
 * @param in_id 计划id
 * @param weather_code 表征天气信息的代码
 * @return stat_m
 */
stat_m m_callable_weather_manage_get_is_effective(uint64_t in_id, int *weather_code);
/**
 * @brief 清除天气缓存信息
 *
 * @param in_id
 * @return stat_m
 */
extern stat_m m_callable_weather_manage_clean(uint64_t in_id);
/**
 * @brief 设置一个带id的天气信息
 *
 * @param in_uid id
 * @param weath_info 对应的天气信息
 * @return stat_m
 */
stat_m m_callable_weather_manage_set(uint64_t in_uid, int weath_info, uint64_t in_time_s);

/***********************************************/

/**
 * @brief 计算某一天的日出日落时间
 *
 * @param lng 经度
 * @param lat 纬度
 * @param tz 时区
 * @param year 年
 * @param month  月份
 * @param day 日
 * @param sunset 输出日落
 * @param sunrise 输出日出
 */
void m_callable_suninfo_get_sunset_and_rise(float lng, float lat, int tz, int year, int month, int day, uint32_t *sunset, uint32_t *sunrise);
/***********************************************/

//************************************IO管理单元******************************************//
/**
 * @brief 仅仅打开一个区域不会影响其他区域
 *
 */
extern stat_m m_callable_solenoid_manage_direct_open(uint8_t channel);
/**
 * @brief 仅仅关闭一个区域不会影响其他区域
 *
 */
extern stat_m m_callable_solenoid_manage_direct_close(uint8_t channel);
/**
 * @brief solenoid init
 * @param _device_version_set 1、2、3  ==》 旧版子、新板子、pro机
 * @param number_of_channels_set 4、6、8、12、15、32
 * @param io_chip_set ahh5 or hc595
 * @param io_chip
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_init(uint32_t _device_version_set, int number_of_channels_set, int io_chip);

/**
 * @brief 打开某一个IO
 * @param channel
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_open(uint8_t channel);

/**
 * @brief 关闭某一个IO
 * @param channel
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_close(uint8_t channel);
/**
 * @brief Continuous operation solenoid {  opens next_pin_index after closing pre_pin_index  }
 *
 * @param pre_pin_index will be closed
 * @param next_pin_index will be opened
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_continuously(uint8_t pre_pin_index, uint8_t next_pin_index);

// /**
//  * @brief 同时打开 两个IO
//  *
//  * @param pre_pin_index will be opened
//  * @param next_pin_index will be opened too
//  * @return stat_m
//  */
// stat_m m_callable_solenoid_manage_open_together(uint8_t pre_pin_index, uint8_t next_pin_index);

// /**
//  * @brief 同时关闭 两个IO
//  *
//  * @param pre_pin_index will be closed
//  * @param next_pin_index will be closed too
//  * @return stat_m
//  */
// stat_m m_callable_solenoid_manage_close_together(uint8_t pre_pin_index, uint8_t next_pin_index);

// /**
//  * @brief IO管理  属性配置
//  *
//  * @param number_of_channels_set 通路数量设置
//  * @param _device_version_set  版本型号设置
//  * @param io_chip_set  芯片型号设置  ahh5 or hc595
//  * @return stat_m
//  */
// stat_m configuration_modification(int number_of_channels_set, int _device_version_set, int io_chip_set);

/**
 * @brief 关闭所有 IO 以及 M 端子
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_close_all(void);

//*************************************IO驱动***************************************//

/**
 * @brief IO驱动初始化
 *
 * @param _device_version_set 1、2、3  ==》 旧版子、新板子、pro机
 * @param number_of_channels_set 4、6、8、12、15、32
 * @param io_chip_set ahh5 or hc595
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_init(uint32_t _device_version_set, int number_of_channels_set, int io_chip_set);

/**
 * @brief 直接打开一个区域,其他区域会关闭
 *
 * @param pin_index
 * @return stat_m
 */
extern stat_m m_ext_drive_solenoid_open_and_close(int pin_index, int is_open);
/**
 * @brief 打开一个区域,不会影响其他区域
 *
 * @param pin_index
 * @return stat_m
 */
extern stat_m m_ext_drive_solenoid_direct_open_and_close(int pin_index, int is_open);

/**
 * @brief 关闭所有 IO 以及 M 端子
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_close_all(void);

//**************************************************M端子*********************************//
/**
 * @brief M端子初始化
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_M_init(void);

/**
 * @brief M端子打开
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_M_open(void);

/**
 * @brief M端子关闭
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_M_close(void);

//************************************灯光单元******************************************//

/**
 * @brief lighting set point color
 *
 * @return void
 */
void m_ext_drive_lighting_set_point_color(uint8_t point, uint32_t color);

/**
 * @brief 灯光单元初始化
 *
 * @param _device_version_set1
 * @param number_of_channels1
 * @param light_strength1
 * @param increment_lighting1
 * @return stat_m
 */
stat_m m_callable_light_display_initial(uint32_t _device_version_set1, uint8_t number_of_channels1, uint8_t light_strength1, uint8_t increment_lighting1);

/**
 * @brief 选择灯光显示模式
 *
 * @param light_mode
 * @param param
 * @return stat_m
 */
stat_m m_callable_display_status(enum light_anima_mode light_mode, uint8_t param);

// 灯光显示 线程
void m_static_display_run(void *pvParameters);

/**
 * @brief 获取灯光状态

 * @return light_anima_mode
 */
enum light_anima_mode m_callable_display_status_get();

/**
 * @brief 设置设备的运行模式
 *
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_running_mode_set_wifi_info(char *wifi_name, char *wifi_psw);

//***************************************灯光驱动**************************************//
/**
 * @brief 设置灯光颜色
 * @param pin
 * @param color
 * @return void
 */
void m_ext_rgb_pin_2(uint8_t pin, uint32_t color);

/**
 * @brief 设置灯光颜色
 * @param pin
 * @param red
 * @param green
 * @param blue
 * @return void
 */
void m_ext_rgb_pin_1(uint8_t pin, uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief 灯光 芯片驱动初始化
 * @return stat_m
 */
stat_m m_ext_drive_lighting_init(void);

/**
 * @brief 灯光 IIC驱动初始化
 * @return stat_m
 */
stat_m m_ext_drive_lighting_IIC_init(void);

/********************************************************/

/**
 * @brief IIC 协议初始化
 *
 * @return stat_m
 */
stat_m m_callable_iic_init(void);
/**
 * @brief IIC 写入
 *
 * @param u8I2cSlaveAddr
 * @param u8Cmd
 * @param u8Value
 * @param iic_select
 * @return stat_m
 */
stat_m m_callable_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value);

/**
 * @brief IIC 读取
 *
 * @param u8I2cSlaveAddr
 * @param u8Cmd
 * @param pBuff
 * @param u8Cnt
 * @param iic_select
 * @return stat_m
 */
stat_m m_callable_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt);

/**
 * @brief 相关监控触发函数
 *
 * @param args
 */
void m_ext_global_monitor_loop(void *args);

//*************************************电流检测*****************************

/**
 * @brief 电流检测初始化 同时打开了 检测电流线程
 * @return stat_m
 */
stat_m m_callable_current_monitor_init(void);

/**
 * @brief 开始电流检测
 * @return stat_m
 */
stat_m m_callable_current_monitor_start(void);

/**
 * @brief 停止电流检测
 * @return stat_m
 */
stat_m m_callable_current_monitor_stop(void);
/**
 * @brief 获取到检测的电流值
 *
 * @return float
 */
int m_callable_current_check_get_final_value(void);
/**
 * @brief 查询短路是否
 *
 * @return stat_m  succ 短路
 */
extern stat_m m_callable_current_is_interrupt();
//**********************************ADC 驱动*********************************************//
/**
 * @brief adc init
 * @return void
 */
void m_ext_drive_adc_init();
/**
 * @brief 電流監控和中斷觸發反饋含糊三
 *
 * @param event_id 發送的事件類型
 * @param current_vue 電流值
 * @param is_interrupt 是否觸發短路
 */
extern void m_ext_current_monitor_result(enum current_event_id event_id, int current_vue, int interrupt_evt, int inter_zone, uint64_t in_time_s);
/**
 * @brief adc read value
 * @return int
 */
int m_ext_drive_read_adc_value();

/**
 * @brief adc read value
 * @param adc_reading
 * @param voltage_float
 * @return float
 */
float m_ext_drive_adc_to_voltage(int adc_reading, float *voltage_float);
/**
 * @brief 获取一段时间内的最大值
 *
 * @return float
 */
int m_callable_current_check_get_max_value(void);
/**
 * @brief 电流中断短路通知
 *
 */
void m_callable_current_out_short_interrupt(void);

//************************************RTC   应用层**************************************//
/**
 * @brief RTC init
 * @return stat_m
 */
stat_m m_callable_midllle_rtc_init(void);

/**
 * @brief Write time to RTC
 *
 * @param timestamp
 * @return bool
 */
stat_m m_callable_midlle_rtc_set_time(int year, int month, int tm_day, int tm_hour, int tm_min, int tm_sec, int tm_wday);
/**
 * @brief Get the time from the RTC
 *
 * @param timestamp
 * @return stat_m
 */
stat_m m_callable_midlle_rtc_get_time(uint64_t *timestamp);

void rtc_time_runing_task(void);

//************************************RTC   驱动**************************************//
/**
 * @brief Write time to RTC
 *
 * @param timestamp
 * @return bool
 */
bool m_ext_drive_rtc_set_time(int year, int month, int tm_day, int tm_hour, int tm_min, int tm_sec, int tm_wday);
/**
 * @brief Get the time from the RTC
 *
 * @param timestamp
 * @return bool
 */
bool m_ext_drvie_rtc_get_time(uint64_t *out_timestamp);

//********************************************OTA  更新 应用层***********************************************************//

/**
 * @brief ota_init
 * @return stat_m
 */
stat_m m_callable_ota_init(void);

/**
 * @brief ota_enable_silent_update

 * @return stat_m
 */
stat_m m_callable_ota_enable_silent_updata(char *in_url);

// /**
//  * @brief ota_disable_silent_update
//  * @return stat_m
//  */
// stat_m m_callable_ota_disable_silent_updata(void);

// /**
//  * @brief ota_start
//  * @return stat_m
//  */
// stat_m m_callable_ota_start(void);

// /**
//  * @brief ota_stop
//  * @return stat_m
//  */
// stat_m m_callable_ota_stop(void);

// /**
//  * @brief ota_update_main_entrance  OTA更新 只需要要调用这一个接口就可以
//  * @param in_url
//  * @return stat_m
//  */
// stat_m m_callable_ota_update_main_entrance(char *in_url);

/**
 * @brief ota_update_main_entrance
 * @param in_url
 * @return stat_m
 */
stat_m m_callable_ota_check_app_version(void);

/**
 * @brief ota_can_start_updating_time
 * @param new_timestamp
 * @return stat_m
 */
stat_m m_callable_ota_can_start_updating_time_set(uint64_t new_timestamp);

/**
 * @brief ota_can_start_updating_time_get  如果返回succ_r,就可以进行静默更新否则不可以
 * @return stat_m
 */
stat_m m_callable_ota_can_start_updating_time_get(void);

/**
 * @brief m_callable_ota_silent_updata_start_init  
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_start_init(uint64_t in_time_s);


/**
 * @brief m_callable_ota_silent_updata_flag_get
 * @return stat_m
 */
extern bool m_callable_ota_silent_updata_flag_get(void);

/**
 * @brief ota_update_progress_get
 * @return float
 */

// stat_m m_callable_ota_event_handle(uint8_t event_id);

//***********************************OTA 驱动******************//
/**
 * @brief ota_init
 * @return stat_m
 */
stat_m m_ext_drive_ota_init(void);

/**
 * @brief ota_start
 * @return stat_m
 */
stat_m m_ext_drive_ota_start(void);

/**
 * @brief ota_enable_silent_update
 * @return stat_m
 */
stat_m m_ext_drive_ota_enable_silent_update(char *in_url);

/**
 * @brief ota_disable_silent_update
 * @return stat_m
 */
stat_m m_ext_drive_ota_disable_silent_update(void);

/**
 * @brief ota_client_cleanup
 * @return stat_m
 */
stat_m m_ext_drive_ota_client_cleanup(void);


/**
 * @brief m_callable_ota_client_cleanup
 * @return stat_m
 */
stat_m m_callable_ota_client_cleanup();

/**
 * @brief ota静默更新完成标记 移除    如果设备意外重启,则移除标记
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_clear_mark(void);


/**
 * @brief ota静默更新  停止
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_stop(void);


/**
 * @brief ota静默更新  停止标记位获取
 * @return stat_m
 */
bool m_callable_ota_silent_updata_stop_flag_get(void);

/**
 * @brief ota_stop
 * @return stat_m
 */
stat_m m_ext_drive_ota_stop(void);

/**
 * @brief ota_end
 * @return stat_m
 */
void m_ext_drive_ota_end(void);

/**
 * @brief OTA_check_app_version
 * @return stat_m
 */
stat_m m_ext_drive_ota_check_app_version(void);
//*************************************ahh5 芯片 *********************************************//
/**
 * @brief ahh5_init
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_init(void);

/**
 * @brief ahh5_reset
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_reset(void);

/**
 * @brief  通过ahh5 打开IO
 * @param bit
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_solenoidvalve_set(uint8_t bit);

/**
 * @brief ahh5 芯片复位
 * @param bit
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_solenoidvalve_reset(uint8_t bit);

/**
 * @brief 通过ahh5 同时打开两个IO
 * @param bito1
 * @param bit2ss
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_solenoidvalve_two_bit(PinBit bito1, PinBit bit2, uint8_t on_off);

/**
 * @brief 控制某一个IO 是打开还是关闭
 * @param bit
 * @param on_off
 * @return stat_m m_ext_drive_ahh5_solenoidvalve(PinBit bit, uint8_t on_off);

 */
stat_m m_ext_drive_ahh5_solenoidvalve(PinBit bit, uint8_t on_off);
/**
 * @brief 暂时没写 暂时没用上
 * @param pinBit
 * @param x
 * @return esp_err_t
 */
stat_m m_ext_drive_pca9557_setiostate(PinBit pinBit, uint8_t x);

//*********************************************HC595 驱动 ************************************************//
/**
 * @brief hc595 初始化
 * @return stat_m
 */
stat_m m_ext_drive_hc595_init(void);

/**
 * @brief  控制hc595某一个引脚的电平高低
 * @param value
 * @return stat_m
 */
stat_m m_ext_driver_hc595_shiftout(int value);

/**
 * @brief 控制某一个IO 打开与关闭
 * @param bit
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_hc595_solenoidvalve(PinBit bit, uint8_t on_off);

/**
 * @brief hc595 芯片复位  关闭所有IO
 * @return stat_m
 */
stat_m m_ext_drive_hc595_shiftout_reset(void);

/**
 * @brief hc595 水锤 控制
 * @param value
 * @return stat_m
 */
stat_m m_ext_drive_hc595_hammer_shiftout(int value);

/**
 * @brief 通过hc595 控制两个 IO同时打开或者关闭
 * @param bito1
 * @param bito2
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_hc595_solenoidvalve_two_bit(PinBit bito1, PinBit bito2, uint8_t on_off);

//*****************************************IIC 驱动***************************************//

/**
 * @brief IIC驱动初始化
 * @return stat_m
 */
extern stat_m m_ext_drive_iic_init(void);

/**
 * @brief IIC写入驱动
 * @param u8I2cSlaveAddr
 * @param u8Cmd
 * @param u8Value
 * @return stat_m
 */
extern stat_m m_ext_drive_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value);

/**
 * @brief IIC写入驱动
 * @param u8I2cSlaveAddr
 * @param u8Cmd
 * @param u8Value
 * @return stat_m
 */
extern stat_m m_ext_driver_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt);

// /**
//  * @brief IIC驱动初始化
//  * @return stat_m
//  */
// stat_m m_ext_drive_iic_init(void);

// /**
//  * @brief IIC写入驱动
//  * @param u8I2cSlaveAddr
//  * @param u8Cmd
//  * @param u8Value
//  * @return stat_m
//  */
// stat_m m_ext_driver_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value);

// /**
//  * @brief IIC 读取驱动
//  * @param u8I2cSlaveAddr
//  * @param u8Cmd
//  * @param pBuff
//  * @param u8Cnt
//  * @return stat_m
//  */
// stat_m m_ext_driver_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt);

/**
 * @brief 灯光亮度调节 范围：1-245
 * @param param
 * @return void
 */
void m_callable_display_set_light_strength(uint8_t param);

/**
 * @brief 流量计脉冲中断计数
 * @return void
 */
void m_callable_sensor_flow_value_interrupt(uint8_t channel);

/**
 * @brief 预警检测反馈 everyone
 * @return stat_m
 */
stat_m m_callable_early_warning_feedback_monitor(uint64_t current_time);

/**
 * @brief 设置 传感器损坏检测时间
 * @param Poweron_time_set
 * @param Poweroff_time_set
 * @return stat_m
 */
stat_m m_callable_device_sensor_damage_detection_time_set(uint8_t poweron_time, uint8_t poweroff_time);

/**
 * @brief 设置 流量漏水检测时间
 * @param time_set
 * @param flow_value_set
 * @return stat_m
 */
stat_m m_callable_device_flow_leakage_detection_time_set(uint8_t time_set, float flow_value_set);

/**
 * @brief 雨量传感器损坏 检测清空
 * @param sen_type_get
 * @param channel
 * @param is_effective_get_sensor
 * @return stat_m
 */
stat_m m_callable_rain_gauge_damage_detection_data_cleared(bool sensor_open_flag, enum seneor_type sen_type_get, uint8_t channel, enum is_effective is_effective_get_sensor);

/**
 * @brief 流量计 漏水检测之 脉冲计数
 * @return stat_m
 */
stat_m m_callable_flow_sensor_meter_pulse_counting(void);

/**
 * @brief 直接获取当前电流值
 * @return float
 */
float m_ext_mda_adc_get_current(void);

/**
 * @brief mda_adc init
 * @return void
 */
void m_ext_mda_adc_init(void);

/**
 * @brief 上电直接打开引脚12
 * @return stat_m
 */
stat_m m_callable_drive_io_pin12_init(void);
/**
 * @brief 上电直接打开引脚12
 * @return stat_m
 */
stat_m m_callable_drive_io_pin12_uninit(void);

/**
 * @brief 传感器损坏以及漏水检测 是否需要打开或者关闭
 *  @param channel
 * @param flag
 * @return stat_m
 */
stat_m m_callable_early_warning_monitor_open_or_close(bool flag);

/**
 * @brief 传感器损坏以及漏水检测设置 存储  防止断电数据清空
 * @return stat_m
 */
stat_m m_callable_early_warning_data_init(void);

// // 设置灯光打开与关闭
// void m_callable_display_set_light_close_or_open(uint8_t param);

// /**
//  * @brief 存储每个区域的各自流速
//  * @return stat_m
//  */
// stat_m m_static_flow_pulse_time_save(void);

/**
 * @brief 判断当前时间是否在本次区域运行时间范围内
 * @param currenttime 当前时间
 * @return statm
 */
stat_m m_callable_whether_current_time_range(uint64_t currenttime);

/**
 * @brief 用来去中断 获取 脉冲
 * @return statm
 */
stat_m m_callable_pulse_recording(void);

/**
 * @brief 在外部 检测函数中调用, 检测 相邻且是同一个区域的 两个脉冲对应的时间戳  计算出脉冲周期，从而算出 流速
 * @param channel_id 下一个将要运行的区域id
 * @param running_time 当前运行区域的   运行时间
 * @param out_final_vue 获取到的流量值
 * @param in_times 当前时间
 * @return statm
 */
stat_m m_callable_zone_transfore_get(uint8_t channel_id, uint32_t running_time, float *out_final_vue);

/**
 * @brief 设置区域流速   服务器改流速
 * @param channel
 * @param flow_speed
 * @return stat_m
 */
stat_m m_callable_channel_flow_speed_set(uint8_t channel, float flow_speed);

/**
 * @brief 获取该区域流速
 * @param channel
 * @param flow_speed
 * @return stat_m
 */
stat_m m_callable_channel_flow_speed_get(uint8_t channel, float *flow_speed);

/**
 * @brief 获取多个区域在运行时的 智能配置参数
 *
 * @param in_channel_info Zone Example ： 1#2#5#6#9
 * @param out_ar_info 1#60&60 # 2#1.2&3.2&1.1
 * @return stat_m
 */
stat_m m_callable_channel_smart_config_a_b_get(char *conff_a_b, char *in_channel_info, char *out_ar_info);
/**
 * @brief 漏水检测
 * @param detection_cycle 当前漏水检测周期
 * @param out_final_vue 获取到的流量值
 * @param in_times 当前时间
 * @return statm
 */
stat_m m_callable_leakage_velocity_detection(uint32_t detection_cycle, float *out_final_vue, uint64_t in_times, uint8_t time_interval, bool flag);

stat_m m_callable_flow_sensor_type_gets(uint8_t *senseortype);

void m_ext_drive_ota_overtime(void);

extern stat_m m_callable_flow_sensor_type_gets_m(void);

extern void m_ext_drive_adc_init_rtc();

// 读取ADC值的函数
extern int m_ext_drive_read_adc_value_rtc();

// ADC值转化成电压值
extern float m_ext_drive_adc_to_voltage_rtc(int adc_reading, float *voltage_float);

extern int m_callable_rtc_power_warning(char *percentage_battery_get);

extern stat_m m_calllable_current_monitor(int device_status);

extern int m_callable_current_calibration(void);

extern stat_m m_callable_current_calibration_channel_select(uint8_t channel);

extern stat_m m_callable_current_calibration_interrupt_processing(void);

extern stat_m m_callable_current_monitor_pause_not_clear(void);

/**
 * @brief 根据传入模式调整在短路发生时，对应的处理
 *
 * @param in_csm 模式
 * @return stat_m
 */
extern stat_m m_callable_current_set_short_mode(enum current_short_mode in_csm);
/**
 * @brief 设置电流反馈的当前检测区域，需要再每次区域开启更新，保持水锤更新
 *
 * @param in_zone
 * @return stat_m
 */
extern stat_m m_callable_current_set_checkout_zone(int in_zone);

/*批量区域电流校准标志位设置 0表示关闭、1表示电流校准 、2表示电流测试*/
extern stat_m m_callable_current_batch_area_power_calibration_flag_set(int flag);

/*批量区域电流校准标志位获取 0表示关闭、1表示电流校准 、2表示电流测试*/
extern int m_callable_current_batch_area_power_calibration_flag_get(void);

/**
 * @brief 获取自上次到现在的流量值，并且指定是否清除缓存并且重新开始计算，如果流量传感器未打开 返回0
 *
 * @param is_clear 是否清除缓存
 * @return stat_m
 */
extern float m_static_divideInt_by_uint32(int dividend, uint32_t divisor);

/**
 * @brief 获取自上次到现在的流量值，并且指定是否清除缓存并且重新开始计算，如果流量传感器未打开 返回0
 *
 * @param is_clear 是否清除缓存
 * @return stat_m
 */
extern float m_static_divideInt_by_uint16(float dividend, uint16_t divisor);

/**
 * @brief 获取 带有三位小数的 值
 *
 * @param value 需要转换的值
 * @return float
 */
extern float m_static_round_threedecimal(float value);

/**
 * @brief 按照时间类型对设备进行  冬令时夏令时切换
 * @param timestamp
 * @param timezone_offset
 * @param time_type
 * @param next_transition_timestamp
 * @param next_timezone_offset
 * @return stat_m
 */
extern stat_m m_callable_get_next_dst_transition_type(time_t timestamp, int timezone_offset, int time_type, time_t *next_transition_timestamp, int *next_timezone_offset);

extern void print_partition_table();

/*                    *******               */
/**
 * @brief 把设备通信模式更改为离线蓝牙模式
 * @param void
 * @param connect_mode_get
 * @return stat_m
 */
extern stat_m m_callable_offline_bluetooth_mode_setting(enum connect_mode connect_mode_get);

/**
 * @brief 把设备运行模式 更改为离线蓝牙模式
 * @param void
 * @param connect_mode_get
 * @return stat_m
 */
extern stat_m m_callable_offline_bluetooth_running_mode_setting(enum running_mode running_mode_get);

/**
 * @brief 获取设备通信模式
 * @param running_mode_get
 * @return stat_m
 */
extern stat_m m_callable_offline_bluetooth_running_mode_getting(enum running_mode *running_mode_get);

/**
 * @brief 复位开始连接服务器  用于离线模式，不然线程会循环执行
 *
 * @param stus
 * @return stat_m
 */
extern stat_m m_callable_network_reset_server_start_connect_server(void);

/**
 * @brief 流量计自定义值 设置
 * @param k_value K值或者是 升/脉冲
 * @param offset_value 偏移值
 * @return stat_m
 */
extern stat_m m_calllable_sensor_k_or_f_set(float k_value, float offset_value);

/**
 * @brief 把带有&的字符串进行分割并且把浮点数进行返回
 * @param  str
 * @param num1
 * @param num2
 * @return stat_m
 */
extern stat_m m_callable_parsestring(const char *str, float *num1, float *num2);

/**
 * @brief 字符拼接函数
 * @param  str
 * @param num1
 * @param num2
 * @return stat_m
 */
extern void m_callable_concatenatestring(const char *input, char *output);

#endif /* __FSET_LIB__H */
