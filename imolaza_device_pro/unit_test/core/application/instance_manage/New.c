
/**
 * @file New.c
 * @author your name (you@domain.com)
 * @brief
 * 队列大致结构
 *  区域A队列        区域B队列    区域～...
 *    [A]             [B]
 *    [A]             [B]
 *    [A]             [B]
 *   ...            ...
 * 每一个区域都有一个运行队列，类型不同但是生成的队列是一致的   最后是汇总(根据运行时间和规律来)
 *
 *  总 [A]-[B]-[浸泡]-[A]-..........
 *
 * 或者
 *    直接运算出最佳运行时间，然后根据实施推算出运行队列  这个挺好的
 *
 *
 * @version 0.1
 * @date 2023-09-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// *(a + index * 5 + 1)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
// typedef int bool;

#define bool int
#define true 1
#define false 0

#define M_MALLOC malloc
#define M_FREE free

#define GET_CHANNEL_ID(a, index) a[index].cd_cahnnel_id
#define GET_CHANNEL_SHOULD_RUN_TIME(a, index) a[index].cd_should_run_time
#define GET_CHANNEL_SHOULD_ALREADY_RUNNING_TIME(a, index) a[index].cd_reality_run_time
#define GET_CHANNEL_SACK_SHOULD_WATER_TIME(a, index) a[index].cd_sack_should_water_time
#define GET_CHANNEL_SACK_SHOULD_SACK_TIME(a, index) a[index].cd_sack_should_sack_time

#define SET_CHANNEL_SACK_SHOULD_WATER_TIME(a, index, param) a[index].cd_sack_should_water_time = param;
#define SET_CHANNEL_SHOULD_WATER_TIME(a, index, param) a[index].cd_should_run_time = param;

#define FIX_TIME_MIN 4
#define M_MAX_ALLOW_CHANNEL 30

#define DEBUG_TEST printf

enum unit_status
{
    M_STATIC_UNIT_STATUS_NEW,
    M_STATIC_UNIT_STATUS_RUNNING,
    M_STATIC_UNIT_STATUS_RECPVER
};
enum channel_type
{
    M_STATIC_CHANNEL_TYPE_NORMAL,
    M_STATIC_CHANNEL_TYPE_SACK,
    M_STATIC_CHANNEL_TYPE_PUMP,
    M_STATIC_CHANNEL_TYPE_WATERSAVING,
};
enum running_type
{
    M_STATIC_UNIT_RUNNING_TYPE_RUNNING,
    M_STATIC_UNIT_RUNNING_TYPE_SACK,
};

// 单个算法应该准备的是当前所有区域应该运行的时间
// 每次计算都会填充每一个单元
// 单元有 开始的区域   开始的时间  本次应该运行的时间  本次已经运行时间  结束时间
struct instance_running_unit
{
    // 区域的状态
    enum unit_status state;
    // 区域的通道
    uint8_t unit_running_channel;
    // 区域的应该运行时间
    uint32_t unit_should_running_time;
    // 区域的已经运行时间
    uint32_t unit_already_running_time;

    // 区域的本次运行开始时间
    uint64_t unit_this_time_running_start_time;
    // 区域的本次运行运行时间
    uint32_t unit_this_time_will_running_time;
    // 区域的本次运行结束时间
    uint64_t unit_this_time_running_end_time;
    // 相似单元的数量
    uint64_t unit_similar_num;
    // 运行类型
    enum running_type unit_running_type;
    // 下一个运行单元体
    struct instance_running_unit *next_running_unit;
    // 上一个运行单元体
    struct instance_running_unit *pre_running_unit;
};

struct channel_config
{
    /** channel id */
    uint8_t cd_cahnnel_id;
    /** channel is enable */
    uint8_t cd_enable;
    /** channel is opened sack */
    uint8_t cd_channel_type;
    /** channel use sack : water time*/
    uint8_t cd_sack_should_water_time;
    /** channel use sack : sack time */
    uint8_t cd_sack_should_sack_time;
    /** channel should running time sum*/
    uint32_t cd_should_run_time;
    /** channel reality running time sum*/
    uint32_t cd_reality_run_time;
    /** 將要運行時間*/
    uint32_t cd_curr_will_run_time;
    /** 运行时间范围*/
    uint64_t cd_running_end_time;
    /** 最后一个区域运行的时间*/
    uint32_t cd_last_running_time;
    /** 单独的区域喷头效率*/
    float cd_ar;
    /** 单独区域的基本吸土率*/
    float cd_ir;
    /** 单独区域的ASA 允许表层累积*/
    float cd_asa;
    // m_time_range channel_running_time_region;
    //     /** 暫停的時間 開始暫停和結束暫停的時間*/
    // m_time_range pause_recover_time;
};

/***/
/**    固定的区域IR参数 不可修改  */
/** IR */
static const float M_STATIC_FIXED_IR_FOR_CHANNEL[6] = {0.35f, 0.15f, 0.5f, 0.4f, 0.2f, 0.1f};
/** AR */
static const float M_STATIC_FIXED_AR_FOR_CHANNEL[6] = {1.6f, 1.6f, 1.4f, 1.4f, 0.2f, 0.35f};
/** ASA */
static const float M_STATIC_FIXED_ASA_FOR_CHANNEL[6] = {0.25f, 0.16f, 0.26f, 0.24f, 0.26f, 0.1f};

static const float M_STATIC_FIXED_IR_FOR_CHANNEL_1 = 0.35f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_2 = 0.15f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_3 = 0.5f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_4 = 0.4f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_5 = 0.2f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_6 = 0.1f;

static const float M_STATIC_FIXED_AR_FOR_CHANNEL_1 = 1.6f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_2 = 1.6f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_3 = 1.4f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_4 = 1.4f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_5 = 0.2f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_6 = 0.35f;

static const float M_STATIC_FIXED_ASA_FOR_CHANNEL_1 = 0.25f;
static const float M_STATIC_FIXED_ASA_FOR_CHANNEL_2 = 0.16f;
static const float M_STATIC_FIXED_ASA_FOR_CHANNEL_3 = 0.26f;
static const float M_STATIC_FIXED_ASA_FOR_CHANNEL_4 = 0.24f;
static const float M_STATIC_FIXED_ASA_FOR_CHANNEL_5 = 0.26f;
static const float M_STATIC_FIXED_ASA_FOR_CHANNEL_6 = 0.1f;

/***/

//
// 定义一下 所有的区域的根节点指针
// 简单理解就是所有节点的集合    区域1 -> 分片1 分片2 分片3
//                           区域2 -> 分片1 分片2 分片3
struct instance_running_unit *p_root_array[M_MAX_ALLOW_CHANNEL][M_MAX_ALLOW_CHANNEL];
// 还需要一个单元序列的队列   用来存储和改变计划的与运行顺序
// 保证长度可变
struct instance_running_sequences
{
    struct instance_running_unit *head_running_unit;
    struct instance_running_unit *current_running_unit;
    struct instance_running_unit *tail_running_unit;
};
int m_static_instance_runnning_queue_append(struct instance_running_sequences *root_inst_queue, struct instance_running_unit *in_running_unit);
void m_static_instance_runnning_queue_print(struct instance_running_sequences *root_inst_queue);
// int m_static_instance_algorithm_sack_time(int in_ar, int in_ir, uint32_t in_sack_should_run_time, uint8_t *ou_sack_time);
/**
 * @brief 计算浸泡时间
 *
 * @param in_ar 输入的喷头参数 AR
 * @param in_ir 基本吸土率    IR
 * @param in_sack_should_run_time  单次应该运行的时间
 * @param ou_sack_time 输出的应该浸泡时间
 * @return int 结果
 */
int m_static_instance_algorithm_sack_time(uint8_t chann_id, float in_ar, float in_ir, uint32_t in_sack_should_run_time, uint8_t *ou_sack_time);
/**
 * @brief 计算应该浇水的时间
 *
 * @param in_asa 允许表面累积最大   ASA
 * @param in_ar  输入的喷头参数 AR
 * @param in_ir  基本吸土率    IR
 * @param ou_water_time 输出的应该浇水时间
 * @return int 结果
 */
int m_static_instance_algorithm_water_time(uint8_t chann_id, float in_asa, float in_ar, float in_ir, uint8_t *ou_water_time);

/**
 * @brief 根据已有的区域参数生成运行队列
 *
 * @param arr 区域信息
 * @param max_channel 最大运行区域
 * @param water_start_time 浇水开始时间
 * @param out_sequences 生成的队列
 * @param in_tim_s 当前时间
 * @return int
 */
int m_static_instance_sack_algorithm_queue_gener(struct channel_config *arr, int max_channel, int water_start_time, struct instance_running_sequences *root_sequences, long in_tim_s)
{
    struct instance_running_unit sign_running_unit;
    int result = 0, pc = 0;
    // 填充单元序列

    // 泵模式用来运行的标记参数
    bool cd_pump_done = false;
    // 当前区域
    int presentMaxChan = 0,
        // 运行耗费总时间
        final_water_sum = 0;
    // 计划开始的时间到推算的时间 （根据计划内容不断推进，直到当前时间，或者计划应该结束时间）
    long temp_present_time = in_tim_s;
    while (presentMaxChan < max_channel)
    {
        // if (arr[presentMaxChan].cd_channel_type != M_STATIC_CHANNEL_TYPE_NORMAL)
        {
            for (int loop_channel_index = 0; loop_channel_index < max_channel;) //_device.hardware.type;)
            {
                // DEBUG_TEST( DB_I,"opop   %d" , loop_channel_index);
                if ((arr[loop_channel_index].cd_channel_type == M_STATIC_CHANNEL_TYPE_NORMAL ||
                     arr[loop_channel_index].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP) &&
                    arr[loop_channel_index].cd_should_run_time > arr[loop_channel_index].cd_reality_run_time && !cd_pump_done)
                {
                    {
                        arr[loop_channel_index].cd_reality_run_time = arr[loop_channel_index].cd_should_run_time;
                        final_water_sum += arr[loop_channel_index].cd_should_run_time;
                        temp_present_time += arr[loop_channel_index].cd_should_run_time;
                        // 累积时间
                        arr[loop_channel_index].cd_running_end_time = temp_present_time;
                        DEBUG_TEST( DB_I,"Nor ---> 区域 %d 浇水 %d 完成-%d    (%d , %d)   \r\n", arr[loop_channel_index].cd_cahnnel_id, arr[loop_channel_index].cd_should_run_time,
                                   arr[loop_channel_index].cd_should_run_time, arr[loop_channel_index].cd_reality_run_time, final_water_sum);
                        {                                                                                                    // 加入、加入、加入
                            sign_running_unit.unit_running_channel = arr[loop_channel_index].cd_cahnnel_id;                  // 区域
                            sign_running_unit.unit_should_running_time = arr[loop_channel_index].cd_should_run_time;         // 应该运行时间
                            sign_running_unit.unit_this_time_running_end_time = arr[loop_channel_index].cd_running_end_time; // 结束时间
                            sign_running_unit.unit_already_running_time = arr[loop_channel_index].cd_reality_run_time;
                            sign_running_unit.unit_this_time_will_running_time = arr[loop_channel_index].cd_should_run_time;
                            sign_running_unit.unit_running_type = M_STATIC_UNIT_RUNNING_TYPE_RUNNING;
                            // 加入队列
                            m_static_instance_runnning_queue_append(root_sequences, &sign_running_unit);
                        }
                        // 泵模式标记，下一次一定运行浸泡
                        if (arr[loop_channel_index].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP)
                            cd_pump_done = true;
                        presentMaxChan++;
                    }
                }
                else if ((arr[loop_channel_index].cd_should_run_time > 0 && arr[loop_channel_index].cd_should_run_time > arr[loop_channel_index].cd_reality_run_time) || (arr[loop_channel_index].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && cd_pump_done))
                {
                    // 运行还是浸泡
                    // 计划开始的时间到推算的时间 - 上一次运行的时间标记  大于浸泡时间的花 那么应该运行     否则 应该浸泡
                    if (!cd_pump_done && arr[loop_channel_index].cd_running_end_time == 0 || temp_present_time - arr[loop_channel_index].cd_running_end_time >= arr[loop_channel_index].cd_sack_should_sack_time)
                    {
                        // 剩下的应该运行的时间 > 浸泡应该浇水的时间     =>>  直接浇水  cd_sack_should_water_time
                        // 运行完整时长还是断续时长
                        if (arr[loop_channel_index].cd_reality_run_time + arr[loop_channel_index].cd_sack_should_water_time <= arr[loop_channel_index].cd_should_run_time &&
                            arr[loop_channel_index].cd_reality_run_time + arr[loop_channel_index].cd_last_running_time < arr[loop_channel_index].cd_should_run_time)
                        {
                            arr[loop_channel_index].cd_reality_run_time += arr[loop_channel_index].cd_sack_should_water_time;
                            final_water_sum += arr[loop_channel_index].cd_sack_should_water_time;
                            temp_present_time += arr[loop_channel_index].cd_sack_should_water_time;
                            // 累积时间
                            arr[loop_channel_index].cd_running_end_time = temp_present_time;

                            DEBUG_TEST( DB_I,".   ---> 区域 %d 浇水 %d 完成-%d    (%d , %d)   \r\n", arr[loop_channel_index].cd_cahnnel_id, arr[loop_channel_index].cd_sack_should_water_time,
                                       arr[loop_channel_index].cd_should_run_time, arr[loop_channel_index].cd_reality_run_time, final_water_sum);
                            {                                                                                                    // 加入、加入、加入
                                sign_running_unit.unit_running_channel = arr[loop_channel_index].cd_cahnnel_id;                  // 区域
                                sign_running_unit.unit_should_running_time = arr[loop_channel_index].cd_should_run_time;         // 应该运行时间
                                sign_running_unit.unit_this_time_running_end_time = arr[loop_channel_index].cd_running_end_time; // 结束时间
                                sign_running_unit.unit_already_running_time = arr[loop_channel_index].cd_reality_run_time;
                                sign_running_unit.unit_this_time_will_running_time = arr[loop_channel_index].cd_sack_should_water_time;
                                sign_running_unit.unit_running_type = M_STATIC_UNIT_RUNNING_TYPE_RUNNING;
                                // 加入队列
                                m_static_instance_runnning_queue_append(root_sequences, &sign_running_unit);
                            }
                            if (arr[loop_channel_index].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
                                m_static_instance_algorithm_sack_time(arr[loop_channel_index].cd_cahnnel_id, arr[loop_channel_index].cd_ar, arr[loop_channel_index].cd_ir, arr[loop_channel_index].cd_sack_should_water_time, &arr[loop_channel_index].cd_sack_should_sack_time);
                            if (arr[loop_channel_index].cd_should_run_time <= arr[loop_channel_index].cd_reality_run_time)
                                presentMaxChan++;
                            // 运行一次计算下浸泡的时间
                            // 恢复相关
                        }
                        else
                        {

                            DEBUG_TEST( DB_I,";   ---> 区域 %d 浇水 %d 完成-%d    (%d , %d) \r\n", arr[loop_channel_index].cd_cahnnel_id, arr[loop_channel_index].cd_should_run_time - arr[loop_channel_index].cd_reality_run_time,
                                       arr[loop_channel_index].cd_should_run_time, arr[loop_channel_index].cd_reality_run_time, final_water_sum);
                            {
                                // sign_running_unit.pre_running_unit =
                                sign_running_unit.unit_this_time_will_running_time = arr[loop_channel_index].cd_should_run_time - arr[loop_channel_index].cd_reality_run_time;
                                if (sign_running_unit.unit_this_time_will_running_time < FIX_TIME_MIN)
                                    sign_running_unit.unit_this_time_will_running_time = arr[loop_channel_index].cd_last_running_time;
                                // 算出需要进步的，加上
                                final_water_sum += sign_running_unit.unit_this_time_will_running_time;
                                temp_present_time += sign_running_unit.unit_this_time_will_running_time;
                                // 更新
                                arr[loop_channel_index].cd_reality_run_time = arr[loop_channel_index].cd_should_run_time;
                                sign_running_unit.unit_running_channel = arr[loop_channel_index].cd_cahnnel_id;
                                sign_running_unit.unit_should_running_time = arr[loop_channel_index].cd_should_run_time;
                                sign_running_unit.unit_this_time_running_end_time = arr[loop_channel_index].cd_running_end_time;
                                sign_running_unit.unit_already_running_time = arr[loop_channel_index].cd_reality_run_time;
                                sign_running_unit.unit_running_type = M_STATIC_UNIT_RUNNING_TYPE_RUNNING;
                                m_static_instance_runnning_queue_append(root_sequences, &sign_running_unit);
                            }

                            arr[loop_channel_index].cd_running_end_time = temp_present_time;
                            // m_static_instance_algorithm_sack_time(arr[loop_channel_index].cd_cahnnel_id , arr[loop_channel_index].cd_ar , arr[loop_channel_index].cd_ir , arr[loop_channel_index].cd_sack_should_water_time , &arr[loop_channel_index].cd_sack_should_sack_time);
                            if (arr[loop_channel_index].cd_should_run_time <= arr[loop_channel_index].cd_reality_run_time)
                                presentMaxChan++;
                        }
                        loop_channel_index++;
                        // temp_chan_should_water = arr[loop_channel_index].cd_should_run_time;
                    }
                    else
                    { // 要浸泡
                        if (arr[loop_channel_index].cd_should_run_time - arr[loop_channel_index].cd_reality_run_time > 0 || (arr[loop_channel_index].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && cd_pump_done))
                        {

                            int sct = 0;
                            { // 找一找 浸泡时间
                                if (arr[loop_channel_index].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && arr[loop_channel_index].cd_sack_should_sack_time > 0)
                                {
                                    sct = arr[loop_channel_index].cd_sack_should_sack_time;
                                    arr[loop_channel_index].cd_sack_should_sack_time = 0;
                                    cd_pump_done = false;
                                }
                                else
                                {
                                    sct = (arr[loop_channel_index].cd_sack_should_sack_time - (temp_present_time - arr[loop_channel_index].cd_running_end_time));
                                }
                            }
                            { // 添加进入队列
                                sign_running_unit.unit_this_time_will_running_time = sct;
                                sign_running_unit.unit_running_channel = arr[loop_channel_index].cd_cahnnel_id;
                                sign_running_unit.unit_should_running_time = arr[loop_channel_index].cd_should_run_time;
                                sign_running_unit.unit_this_time_running_end_time = arr[loop_channel_index].cd_running_end_time;
                                sign_running_unit.unit_already_running_time = arr[loop_channel_index].cd_reality_run_time;
                                sign_running_unit.unit_running_type = M_STATIC_UNIT_RUNNING_TYPE_SACK;
                                m_static_instance_runnning_queue_append(root_sequences, &sign_running_unit);
                            }
                            if (arr[loop_channel_index].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP)
                                loop_channel_index++;
                            final_water_sum += sct;
                            temp_present_time += sct;
                        }
                    }
                }
                else
                    loop_channel_index++; //, temp_chan_should_water = arr[loop_channel_index].cd_should_run_time;
            }
        }
    }
    DEBUG_TEST( DB_I,"sum %d\n", final_water_sum);
    return result;
}
/**
 * @brief 计算应该浇水的时间
 *
 * @param in_asa 允许表面累积最大   ASA
 * @param in_ar  输入的喷头参数 AR
 * @param in_ir  基本吸土率    IR
 * @param ou_water_time 输出的应该浇水时间
 * @return int 结果
 */
int m_static_instance_algorithm_water_time(uint8_t chann_id, float in_asa, float in_ar, float in_ir, uint8_t *ou_water_time)
{
    int result = 0;
    if (in_asa == 0 && chann_id <= 6)
        in_asa = M_STATIC_FIXED_ASA_FOR_CHANNEL[chann_id - 1];

    if (in_ir == 0 && chann_id <= 6)
        in_ir = M_STATIC_FIXED_IR_FOR_CHANNEL[chann_id - 1];
    if (in_ar == 0 && chann_id <= 6)
        in_ar = M_STATIC_FIXED_AR_FOR_CHANNEL[chann_id - 1];

    if (in_asa)
    {
        // printf("%.02f / ( %.02f -  %.02f )\n", in_asa, in_ar, in_ir);
        int resul = (int)((60 * in_asa) / (in_ar - in_ir));
        *ou_water_time = (uint8_t)resul;
        // DEBUG_TEST( DB_I,"*-----%d\n\n", resul);
        // DEBUG_TEST( DB_I,"( %.02f / %.02f - 1 ) * %d  result  = %d\n", in_ar, in_ir, in_sack_should_run_time, resul);
    }
    return result;
}
/**
 * @brief 计算浸泡时间
 *
 * @param in_ar 输入的喷头参数 AR
 * @param in_ir 基本吸土率    IR
 * @param in_sack_should_run_time  单次应该运行的时间
 * @param ou_sack_time 输出的应该浸泡时间
 * @return int 结果
 */
int m_static_instance_algorithm_sack_time(uint8_t chann_id, float in_ar, float in_ir, uint32_t in_sack_should_run_time, uint8_t *ou_sack_time)
{
    int result = 0;

    if (in_ir == 0 && chann_id <= 6)
        in_ir = M_STATIC_FIXED_IR_FOR_CHANNEL[chann_id - 1];
    if (in_ar == 0 && chann_id <= 6)
        in_ar = M_STATIC_FIXED_AR_FOR_CHANNEL[chann_id - 1];

    if (in_ir && in_ar)
    {
        float resul = (((in_ar / in_ir) - 1) * in_sack_should_run_time);
        if (resul + 0.9999999 <= ((int)resul) + 1) // 如果整除了
            *ou_sack_time = (uint8_t)resul;
        else
            *ou_sack_time = ((uint8_t)resul) + 1;
        // DEBUG_TEST( DB_I,"( %.02f / %.02f - 1 ) * %d  result  = %.02f\n", in_ar, in_ir, in_sack_should_run_time, resul);
        // DEBUG_TEST( DB_I,"%d" , *ou_sack_time);
    }
    return result;
}

/**
 * @brief 作为计划浸泡模型生成的函数,如果需要恢复的话 一直都会计算出应该处于的位置根据in_tim_s
 * @param arr 区域信息
 * @param max_channel 最大区域
 * @param out_sequences  输出序列
 * @param in_tim_s  当前的时间
 */
int m_static_instance_sack_algorithm_pro_max_A(struct channel_config *arr, int max_channel, long in_tim_s)
{

    int result = 0;
    //  准备单元序列
    // struct instance_running_unit new_running_unit = {0};

    if (arr == NULL || max_channel <= 0)
        result = 1;

    if (!result)
    {
        //  每个区域均衡算法
        for (int loop_channel_index_i = 0; loop_channel_index_i < max_channel; loop_channel_index_i++)
        {
            // 均衡的是各个区域的浇水时间，范围应该是 ( 4 到 设置的应该浇水时间 )
            // 递减推进计算出最合适的浇水时间和轮数
            // remainder 计算出来的余数    approaching_zero 整除不了但是最接近的余数
            double remainder = 0.0f;
            // int approaching_zero = 9999;
            int sum = 0;
            int result_x = 0;
            //  填充单元序列
            // memset(new_running_unit , 0 , sizeof(new_running_unit));
            // 如果是节水模式的话
            if (arr[loop_channel_index_i].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
            {

                for (int p = 1; p <= GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - 3; p++) // GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr , loop_channel_index_i)不会小于4
                {
                    // 计算运行总时间
                    // m_static_algorithm_instance_cost(GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i), GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1, &sum);
                    //    应该运行时间 对  浸泡应该运行时间  取余
                    remainder = (GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) * 1.0) / ((GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) * 1.0) - p + 1);
                    DEBUG_TEST( DB_I,"\t(%d/%d)   %.02f <= %d\n", GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i), (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1), remainder, ((int)remainder) + 1);

                    if (remainder + 0.9999999 <= ((int)remainder) + 1) // 如果整除了
                    {
                        result_x = (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1);
                        sum = (int)remainder * result_x;
                        DEBUG_TEST( DB_I,"%d*%d\n", (int)remainder, result_x);

                        break;
                        // 总运行时间 取余 应该运行时间（7.6.5.4）  大于上一个 小于 应该运行时间
                    }
                    // 那就计算下总运行时间 总运行时间小获胜 比如 664 和 554 明显 554 更适合
                    // 如果 轮数*计算的最佳时间 + FIX_TIME_MIN < 应该运行的总时间
                    else if (
                        ((int)remainder - 1) * (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1) + FIX_TIME_MIN < GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i))
                    {
                        // approaching_zero = ((int)remainder - 1); // GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - ((int)remainder-1) * (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1);
                        result_x = (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1);
                        DEBUG_TEST( DB_I,"%d*%d+%d\n", (int)remainder, result_x, GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - ((GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1)) * ((int)remainder));
                        arr[loop_channel_index_i].cd_last_running_time = GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - ((GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1)) * ((int)remainder);
                        // 如果 应该运行的时间 - 轮数 * 最佳时间 大于 FIX_TIME_MIN   (最好不用减法，会出现bug)
                        // 如果  轮数 * 最佳时间 + FIX_TIME_MIN 大于 应该运行的时间
                        if (((int)remainder) * (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) - p + 1) + FIX_TIME_MIN <= GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i))
                            break;
                        sum = (int)remainder * result_x + GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - ((int)remainder * result_x);
                    }
                }
                // 有结果了 设置一下
                if (result_x != 0)
                    SET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i, result_x);
            }
            else
            {
                // 其他基本不变
            }
            if (arr[loop_channel_index_i].cd_channel_type != M_STATIC_CHANNEL_TYPE_NORMAL)
                DEBUG_TEST( DB_I,"区域%d ->  最佳浇水时长 %d  浸泡%d（不变）\n\n", GET_CHANNEL_ID(arr, loop_channel_index_i), GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i), GET_CHANNEL_SACK_SHOULD_SACK_TIME(arr, loop_channel_index_i));
            else
                DEBUG_TEST( DB_I,"区域%d ->  最佳浇水时长 %d  浸泡%d（不变）\n\n", GET_CHANNEL_ID(arr, loop_channel_index_i), GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i));
        }
        // 高定
    }
    return 0;
}
int (*m_ext_instance_running_able)(
    int cmd,
    uint64_t instance_running_id,
    uint8_t start_channel,
    uint32_t start_channel_time_sum,
    uint32_t start_channel_will_running_time,
    uint8_t end_channel,
    uint32_t end_channel_reality_time,
    uint32_t end_channel_current_running_time,
    uint32_t end_channel_state,
    uint32_t running_once_sum,
    uint32_t running_sum,
    uint64_t instance_id,
    uint64_t in_time_s);

/**
 * @brief 作为计划浸泡模型生成的函数 （根据参数生成应该运行的数据，可以单独使用）
 * @param arr 区域信息
 * @param max_channel 最大区域
 * @param out_sequences  输出序列
 * @param in_tim_s  当前的时间
 */
int m_static_instance_sack_algorithm_pro_max(struct channel_config *arr, int max_channel, long in_tim_s)
{

    int result = 0;
    int pc = 0;
    //  准备单元序列
    // struct instance_running_unit new_running_unit = {0};

    if (arr == NULL || max_channel <= 0 || max_channel > 16)
        DEBUG_TEST( DB_I,"区域或者最大区域数不正确 ！\n"), result = 1;

    if (!result)
    {
        //  每个区域均衡算法
        for (int loop_channel_index_i = 0; loop_channel_index_i < max_channel; loop_channel_index_i++)
        {
            // 均衡的是各个区域的浇水时间，范围应该是 ( 4 到 设置的应该浇水时间 )
            // 递减推进计算出最合适的浇水时间和轮数
            // remainder 计算出来的除数
            double remainder = 0.0f;
            //  approaching_zero 余数
            int approaching_zero = 9999;
            int sum = 0;
            int result_x = 0;
            //  填充单元序列
            if (arr[loop_channel_index_i].cd_should_run_time <= 0 || arr[loop_channel_index_i].cd_cahnnel_id <= 0 ||
                (arr[loop_channel_index_i].cd_channel_type == M_STATIC_CHANNEL_TYPE_SACK && (arr[loop_channel_index_i].cd_sack_should_water_time <= 0 || arr[loop_channel_index_i].cd_sack_should_sack_time <= 0)) ||
                (arr[loop_channel_index_i].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING && arr[loop_channel_index_i].cd_cahnnel_id > 6 && (arr[loop_channel_index_i].cd_ar == 0 || arr[loop_channel_index_i].cd_asa == 0 || arr[loop_channel_index_i].cd_ir == 0)))
            {
                DEBUG_TEST( DB_I,"该区域参数错误 ！\n");
                return 1;
            }

            // 如果是节水模式的话
            if (arr[loop_channel_index_i].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
            {
                m_static_instance_algorithm_water_time(arr[loop_channel_index_i].cd_cahnnel_id, arr[loop_channel_index_i].cd_asa, arr[loop_channel_index_i].cd_ar, arr[loop_channel_index_i].cd_ir, &arr[loop_channel_index_i].cd_sack_should_water_time);
                DEBUG_TEST( DB_I,"计算出区域ASA: %hhd \n", arr[loop_channel_index_i].cd_sack_should_water_time);

                for (int p = 1; p <= GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - 3; p++) // GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr , loop_channel_index_i)不会小于4
                {
                    // 除数 14/2 = 7   13 /2 = 6
                    remainder = (GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) * 1.0) / (p * 1.0 + 1.0);
                    // 余数
                    approaching_zero = GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) % ((int)remainder);
                    if (remainder >= FIX_TIME_MIN)
                    {
                        if (remainder + 0.99999 <= ((int)remainder) + 1 && remainder < GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i)) // 如果整除了
                        {
                            // DEBUG_TEST( DB_I,"ok 最佳%d   -%d-   最后%d\n", (int)remainder, p + 1, (int)remainder);
                            DEBUG_TEST( DB_I,"根据总浇水时长 %d   计算出最佳 (%d * %d)\n", arr[loop_channel_index_i].cd_should_run_time, (int)remainder, p + 1);
                            arr[loop_channel_index_i].cd_last_running_time = 0;
                            result_x = (int)remainder;
                            break;
                        }
                        else if ((GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - (p) * ((int)remainder)) <= GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i) &&
                                 remainder <= GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i))
                        {
                            // DEBUG_TEST( DB_I,"最佳%d   -%d-   最后%d\n", (int)remainder, p , GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - (p) * ((int)remainder ));
                            arr[loop_channel_index_i].cd_last_running_time = GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - (p) * ((int)remainder);
                            DEBUG_TEST( DB_I,"根据总浇水时长 %d   计算出最佳 (%d * %d + %d)\n", arr[loop_channel_index_i].cd_should_run_time, (int)remainder, p, arr[loop_channel_index_i].cd_last_running_time);
                            result_x = (int)remainder;
                            break;
                        }
                    }
                    else
                    {
                        arr[loop_channel_index_i].cd_last_running_time = GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) - p * (FIX_TIME_MIN);
                        if (arr[loop_channel_index_i].cd_last_running_time < FIX_TIME_MIN)
                            arr[loop_channel_index_i].cd_last_running_time = FIX_TIME_MIN;
                        DEBUG_TEST( DB_I,"根据总浇水时长 %d   计算出最佳 (%d * %d + %d)\n", FIX_TIME_MIN, p, arr[loop_channel_index_i].cd_last_running_time);

                        result_x = FIX_TIME_MIN;
                        SET_CHANNEL_SHOULD_WATER_TIME(arr, loop_channel_index_i, FIX_TIME_MIN * p + arr[loop_channel_index_i].cd_last_running_time);
                        break;
                    }
                    // DEBUG_TEST( DB_I,"l%d ",GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i));
                }
                // 有结果了 设置一下
                if (result_x != 0)
                    SET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i, result_x);
            }
            else
            {
                // 其他基本不变
            }
            if (arr[loop_channel_index_i].cd_channel_type != M_STATIC_CHANNEL_TYPE_NORMAL)
                DEBUG_TEST( DB_I,"区域%d ->  最佳浇水时长 %d  浸泡%d（不变）\n\n", GET_CHANNEL_ID(arr, loop_channel_index_i), GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i), GET_CHANNEL_SACK_SHOULD_SACK_TIME(arr, loop_channel_index_i));
            else
                DEBUG_TEST( DB_I,"区域%d ->  (无浸泡)最佳浇水时长 %d  浸泡%d（不变）\n\n", GET_CHANNEL_ID(arr, loop_channel_index_i), GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i), 0);
        }
        // 高定
    }
    return result;
}

void main()
{
    struct instance_running_sequences inst_queue;
    int max_channel_num = 0;
    int ste = 0;
    /** 1、 定义运行区域 (每一个参数必填) */
    struct channel_config Array[M_MAX_ALLOW_CHANNEL] =
        {
            /**
             * @brief 区域类型是不浸泡的话，相关参数可以不填写或任意
             * M_STATIC_CHANNEL_TYPE_NORMAL        不使用浸泡
             * M_STATIC_CHANNEL_TYPE_SACK          只浸泡，不使用节水
             * M_STATIC_CHANNEL_TYPE_WATERSAVING   节水
             */
            {
                /** 普通运行区域参数示例 */
                .cd_cahnnel_id = 1,                              // 区域
                .cd_should_run_time = 30,                        // 总运行时间
                .cd_channel_type = M_STATIC_CHANNEL_TYPE_NORMAL, // 区域类型
            },
            {
                /** 普通浸泡区域参数示例 */
                .cd_cahnnel_id = 2,                            // 区域
                .cd_should_run_time = 30,                      // 总运行时间
                .cd_channel_type = M_STATIC_CHANNEL_TYPE_SACK, // 区域类型
                .cd_sack_should_water_time = 10,               // 应该运行时间
                .cd_sack_should_sack_time = 30,                // 应该浸泡时间
            },
            {
                /** 泵模式区域参数示例 */
                .cd_cahnnel_id = 8,                            // 区域
                .cd_should_run_time = 30,                      // 总运行时间
                .cd_channel_type = M_STATIC_CHANNEL_TYPE_PUMP, // 区域类型
                .cd_sack_should_water_time = 10,               // 应该运行时间 (无效)
                .cd_sack_should_sack_time = 30,                // 应该浸泡时间
            },
            // {
            //     /** 智能浸泡区域参数示例 */
            //     .cd_cahnnel_id = 6,                                   // 区域
            //     .cd_should_run_time = 30,                             // 总运行时间
            //     .cd_channel_type = M_STATIC_CHANNEL_TYPE_WATERSAVING, // 区域类型

            //     // .cd_ar = M_STATIC_FIXED_AR_FOR_CHANNEL_3,   // AR 喷头效率
            //     // .cd_ir = M_STATIC_FIXED_IR_FOR_CHANNEL_3,   // IR 基本吸土率
            //     // .cd_asa = M_STATIC_FIXED_ASA_FOR_CHANNEL_3, // ASA
            // },
        };
    /** 2、 根据区域填写区域数量    必须和区域数量匹配*/
    max_channel_num = 3;

    DEBUG_TEST( DB_I,"Start..\n\n");
    // 节水数据计算
    ste = m_static_instance_sack_algorithm_pro_max(Array, max_channel_num, 0);
    if (!ste)
    {
        // 运行数据生成
        m_static_instance_sack_algorithm_queue_gener(Array, max_channel_num, 0, &inst_queue, 0);
        // 运行数据输出
        m_static_instance_runnning_queue_print(&inst_queue);
    }
    DEBUG_TEST( DB_I,"Done..\n\n");
}

/*********/
/*********/
/*********/
/*********/
/*********/
/*********/
/*********/
/** 运行相关的队列 包括了 增加删除修改等等 */

int m_static_instance_runnning_queue_append(struct instance_running_sequences *root_inst_queue, struct instance_running_unit *in_running_unit)
{
    int state = 0;

    static struct instance_running_unit *new_unit_d = NULL;

    new_unit_d = M_MALLOC(sizeof(struct instance_running_unit));

    // memcpy(new_unit_d, in_running_unit, sizeof(in_running_unit));
    new_unit_d->unit_running_channel = in_running_unit->unit_running_channel;
    new_unit_d->unit_should_running_time = in_running_unit->unit_should_running_time;
    new_unit_d->unit_this_time_running_end_time = in_running_unit->unit_this_time_running_end_time;
    new_unit_d->unit_already_running_time = in_running_unit->unit_already_running_time;
    new_unit_d->unit_this_time_will_running_time = in_running_unit->unit_this_time_will_running_time;
    new_unit_d->unit_running_type = in_running_unit->unit_running_type;

    // DEBUG_TEST( DB_I,"id %d , should %d already %d \n" , new_unit_d->unit_running_channel , new_unit_d->unit_should_running_time , new_unit_d->unit_already_running_time);

    if (root_inst_queue->head_running_unit == NULL || root_inst_queue->tail_running_unit == NULL)
    {
        new_unit_d->pre_running_unit = NULL;
        root_inst_queue->head_running_unit = new_unit_d;
        root_inst_queue->tail_running_unit = new_unit_d;
    }
    else
    {
        new_unit_d->pre_running_unit = root_inst_queue->tail_running_unit;
        root_inst_queue->tail_running_unit->next_running_unit = new_unit_d;
    }
    root_inst_queue->tail_running_unit = new_unit_d;
    new_unit_d->next_running_unit = NULL;

    return state;
}
/** 打印队列信息 */
void m_static_instance_runnning_queue_print(struct instance_running_sequences *root_inst_queue)
{
    struct instance_running_unit *pc = NULL;

    if (root_inst_queue != NULL && root_inst_queue->head_running_unit != NULL)
    {
        pc = root_inst_queue->head_running_unit;
        while (pc != NULL)
        {
            if (pc->unit_running_type == M_STATIC_UNIT_RUNNING_TYPE_RUNNING)
                DEBUG_TEST( DB_I,"区域 %d , 应该运行 %d  将要运行 %d  完成后区域累积运行 %d \n", pc->unit_running_channel, pc->unit_should_running_time, pc->unit_this_time_will_running_time, pc->unit_already_running_time);
            else
                DEBUG_TEST( DB_I,"区域 %d , 将要浸泡 %d  完成后区域累积运行 %d \n", pc->unit_running_channel, pc->unit_this_time_will_running_time, pc->unit_already_running_time);
            pc = pc->next_running_unit;
        }
    }
}
