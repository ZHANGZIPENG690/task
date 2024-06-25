

#ifndef __INSTANCE_ALGORITHM__H__
#define __INSTANCE_ALGORITHM__H__

#include "core/fset_lib.h"

#include "../instance.h"
#include "../../device_proper/tools/tools.h"

uint64_t m_static_instance_tools_calculate_next_state_switching_time(enum m_funcation m_fun, uint64_t *in_WSClone, uint16_t schoud_chanTime, uint16_t soakTime, uint16_t real_chanTime, uint32_t pre_hhmmss);

/**
 * @brief 很简单的计算实例一次的状态，在当前状态刚开始的时候计算  就是预测未来的状态
 *
 * @param cur_pc
 * @param mode
 * @param max_chann
 * @param channels
 * @param next_pc
 * @param next_state
 * @return stat_m
 */

stat_m m_static_instance_tools_calculate_next_running_status(
    uint8_t cur_pc,
    enum m_funcation mode,
    uint8_t max_chann,
    struct channel_config *channels,
    uint64_t WSTime_clone,
    uint8_t *next_pc,
    enum base_state *next_state,
    uint64_t current_time);
/**
 * @brief 获取下一个运行的区域
 *
 * @param cur_pc
 * @param max_chann
 * @param channels
 * @param next_pc
 * @return stat_m
 */
stat_m m_static_instance_tools_calculate_next_running_pc(
    uint8_t cur_pc,
    uint8_t max_chann,
    struct channel_config *channels,
    uint8_t *next_pc);
/**
 * @brief
 *
 * @param cur_pc
 * @param max_chann
 * @param channels
 * @param pre_pc
 * @return stat_m
 */
stat_m m_static_instance_tools_calculate_pre_running_pc(
    uint8_t cur_pc,
    uint8_t max_chann,
    struct channel_config *channels,
    uint8_t *pre_pc);

/**
 * @brief  把一個子夫差un的數據解析爲一個對象
 *
 * @param src_str
 * @param out_obj
 * @param in_time_s
 * @return stat_m
 */
// stat_m m_static_tools_instance_obj_explain(char *src_str, instance_doc_t *out_obj, uint64_t in_time_s);
stat_m m_static_tools_instance_obj_explain(char *src_str, instance_doc_t *out_obj, bool *Edable, uint64_t in_time_s);

/**
 * @brief 老设备 初始化 和 转换计划的函数 
 * 
 * @param src_str 
 * @param out_obj 
 * @param Edable 
 * @param in_time_s 
 * @return stat_m 
 */
stat_m m_static_tools_instance_obj_explain_A0012(char *src_str, instance_doc_t *out_obj, bool *Edable, uint64_t in_time_s);


/**
 * @brief  这个函数很重要   生成下一次的运行时间
 *
 * @param in_info   实例详细信息
 * @param out_pre_time 上一次应该运行的时间
 * @param in_time_s 当前的时间
 * @return stat_m
 */
stat_m m_static_tools_instance_obj_refresh_start_time(instance_doc_t *in_info,uint64_t *out_pre_time, uint64_t in_time_s);

/**
 * @brief
 *
 * @param in_obj
 * @param format
 * @param zseparate
 * @param in_src
 * @param out_should_time_array
 * @return stat_m
 */
stat_m m_static_tools_instance_obi_explain_zone(instance_doc_t *in_obj, char *format, char zseparate, char *in_src);

// /**
//  * @brief Used to calculate whether the current plan is watered or soaked at some time, it can also be used to calculate the total duration of the area(When the update is a false)
//  *
//  * @param [in] waterObj Plan objects
//  * @param [in] sockWater How long it should be watered
//  * @param [in] sockTime How long it should be sacked
//  * @param [in] tempChanWater Regional details of the current program,
//  * @param [in] waterSum The total length of watering throughout the program
//  * @param [in] present_time The time of the device now
//  * @param [out] out_running_time
//  * @return stat_m
//  */
// stat_m m_static_instance_obj_tools_calcute_instance_time_sum_and_current_staus(instance_doc_t *waterObj, int sockWater, int sockTime, struct channel_config *tempChanWater, int waterSum, uint32_t present_time, uint32_t *out_running_time);

/**
 * @brief 根据当前的时间 计算计划总时长和状态
 *
 * @param arr 区域信息
 * @param max_channel 最大运行区域
 * @param water_start_time 浇水开始时间
 * @param out_sequences 生成的队列
 * @param out_cost_time 计算出的总时长
 * @param in_tim_s 当前时间
 * @return int
 */
int m_static_instance_tools_calcute_instance_time_sum_and_current_staus(struct channel_config *arr, int max_channel, int water_start_time, uint64_t *out_cost_time, long in_tim_s);

/**
 * @brief 找到下一个可以运行的区域，并把当前指针指向他
 *
 * @param root 根节点
 * @return stat_m
 */
stat_m m_static_instance_manage_tool_find_next_sequence(struct instance_running_sequences *root);

/**
 * @brief 找到上一个可以运行的区域，并把当前指针指向他
 *
 * @param root 根节点
 * @return stat_m
 */
stat_m m_static_instance_manage_tool_find_pre_sequence(struct instance_running_sequences *root);

/**
 * @brief 根据计划开始时间和计算间隔5小时的开始时间
 *
 * @param in_present_time 传入现在的时间点
 * @param in_present_timespeam 传入现在的时间戳
 * @param inver_hour 运行的时间
 * @param planTimesp 计划开始时间戳
 * @param [out] out_waterTime 计算得出的结果存放
 */
void m_static_instance_tools_time_calculate_the_interval_hour(uint32_t in_present_time, uint32_t in_present_timespeam, uint8_t inver_hour, uint64_t planTimesp, uint64_t *out_waterTime); // 传入计划的时间戳 ，;

/**
 * @brief 反馈
 *
 * @param cmd // 计划ID
 * @param instance_running_id // 开始区域
 * @param start_channel // 开始总时间
 * @param start_channel_time_sum // 开始将要运行
 * @param start_channel_will_running_time // 结束区域已经运行
 * @param end_channel // 结束区域
 * @param end_channel_reality_time // 结束区域本次
 * @param end_channel_current_running_time // 未中断的本批次运行
 * @param running_once_sum // 总的已经运行
 * @param running_sum // 实例ID
 * @param instance_id // 结束区域状态
 * @param end_channel_state // 当前时间
 * @param in_time_s
 * @return stat_m
 */
stat_m m_static_instance_running_able(
    int cmd,
    uint64_t instance_running_id,
    uint8_t start_channel,
    uint32_t start_channel_time_sum,
    uint32_t start_channel_will_running_time,
    uint8_t end_channel,
    uint32_t end_channel_reality_time,
    uint32_t end_channel_current_running_time,
    uint32_t running_once_sum,
    uint32_t running_sum,
    uint32_t end_channel_state,
    uint64_t instance_id,
    uint64_t in_time_s);

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
                                        uint64_t in_time_s,bool is_queue);


/**
 * @brief 获取已经存在的运行队列
 *
 * @param inst_obj
 * @param out_str
 */
void m_static_instance_runnning_queue_get(char *out_str);
/**
 * @brief 把当前的实例以一定格式的字符串输出
 *
 * @param root_inst_queue 队列
 * @param out_str 输出字符
 */
void m_static_instance_runnning_queue_str_format_get(instance_obj *inst_obj, char *out_str);
/**
 * @brief 时间改变的时候需要重新刷新时间
 * 
 * @param in_data 改变对象
 * @param in_time_s 根据当前的时间
 * @return stat_m 
 */
stat_m m_static_tools_instance_obj_will_refresh_time(void *in_data, uint64_t *noti_time, int *tag, uint64_t in_time_s);

/**
 * @brief 字符编码  把数字编码成 字符
 *
 * @param conchr 返回对应的字符
 * @param in_num 传入数字
 * @param encode_num 编码位数   最大 5 位
 * @return int
 */
void m_callable_time_num_to_str_encrypt_encode(char *conchr, int in_num, int encode_num);
/**
 * @brief 字符串到数字的解码 仅仅以为解码 会损坏原始内容
 * 
 * @param contr 待码数据
 * @param out_num 解码数值 
 * @param len 待解码数据总长度
 */
// void m_callable_time_str_to_num_decrypt_decode(char *contr , int* out_num , int len);
void m_callable_time_str_to_num_decrypt_decode(char *contr, int *out_num, int bit_num, int len);


/**
 * @brief 作为计划浸泡模型生成的函数 （根据参数生成应该运行的数据，可以单独使用）
 * @param arr 区域信息
 * @param max_channel 最大区域
 * @param out_sequences  输出序列
 * @param in_tim_s  当前的时间
 */
int m_static_instance_sack_algorithm_pro_max(struct channel_config *arr, int max_channel, long in_tim_s);
/**
 * @brief 输入待解析的字符串,输出解析完成的时间数组
 * 
 * @param in_str 源字符串
 * @param out_array 数组
 * @return stat_m 
 */
stat_m m_callable_install_tools_start_time_encode_into_array(char *in_str , uint32_t *out_array);

/**
 * @brief 根据当前的时间 取出最近运行的时间
 *
 * @param in_arr_list 时间范围列表
 * @param pre_time 输出的上一个时间
 * @param next_time 输出的下一个时间
 * @param curr_time 当前的时间
 * @return stat_m
 */
stat_m m_static_instance_tools_time_calculate_near_time(uint32_t *in_arr_list, uint64_t *pre_time, uint64_t *next_time, uint64_t curr_time)
;
#endif /* __INSTANCE_ALGORITHM__H__ */
