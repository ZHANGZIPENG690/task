
#include "instance_algorithm.h"
#include "../instance.h"
// #include "hot_so_fast.h"

/*  Plan the time of the next area  */
uint64_t m_static_instance_tools_calculate_next_state_switching_time(enum m_funcation m_fun,
                                                                     uint64_t *in_WSClone,
                                                                     uint16_t schoud_chanTime,
                                                                     uint16_t soakTime,
                                                                     uint16_t real_chanTime,
                                                                     uint32_t pre_hhmmss)
{
    /* 计算 */
    // DEBUG_TEST(DB_W,"718 %hhd  %lld \r\n", soakTime, *in_WSClone);

    if (m_fun == M_M_FUNCATION_NONE)
    {
        *in_WSClone += schoud_chanTime;
    }
    else if (m_fun == M_M_FUNCATION_SACK)
    {
        if ((soakTime) < schoud_chanTime && soakTime != 0)
        {
            if (((*in_WSClone + soakTime) - pre_hhmmss /*get_hh_mm_ss*/) > schoud_chanTime - real_chanTime)
                *in_WSClone += schoud_chanTime - real_chanTime;
            else
                *in_WSClone += soakTime;
        }
        else
            *in_WSClone += schoud_chanTime;
    }
    else if (m_fun == M_M_FUNCATION_PUMP)
        ;

    *in_WSClone %= NUM_OF_DAY_SEC_SUM;
    printf("Next :%lld\n", *in_WSClone);

    return *in_WSClone;
}

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

// *(a + index * 5 + 1)

// typedef unsigned char uint8_t;
// typedef unsigned int uint32_t;
// typedef unsigned long uint64_t;
// typedef int bool;

// #define true 1
// #define false 0

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
/**
 * @brief 计算浸泡时间
 *
 * @param in_ar 输入的喷头参数 AR
 * @param in_ir 基本吸土率    IR
 * @param in_sack_should_run_time  单次应该运行的时间
 * @param ou_sack_time 输出的应该浸泡时间
 * @return int 结果
 */
int m_static_instance_algorithm_sack_time(uint8_t chann_id, float in_ar, float in_ir, float in_asa, uint32_t in_sack_should_run_time, uint32_t *ou_sack_time)
{
    int result = 0;

    if ((in_ir && in_ar && in_ar > in_ir) || (in_ar == 0.35 && in_ir == 0.1 && in_asa == 0.1))
    {
        int resul = (int)(((in_ar / in_ir) - 1) * (in_sack_should_run_time));
        if ((in_ar / in_ir) + 0.99999 > (int)(in_ar / in_ir) + 1)
            resul += 1;

        *ou_sack_time = resul;
        /**   */
        *ou_sack_time *= 60;
        /**   */
        DEBUG_TEST(DB_I, "( %.02f / %.02f - 1 ) * %d  sack result  = %d min", in_ar, in_ir, in_sack_should_run_time, resul);
    }
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
int m_static_instance_algorithm_water_time(uint8_t chann_id, float in_asa, float in_ar, float in_ir, uint32_t *ou_water_time)
{
    int result = 0;
    if (in_ar <= in_ir)
    {
        *ou_water_time = 540;
    }
    else if (in_asa > 0)
    {
        printf("%.02f / ( %.02f -  %.02f )\n", in_asa, in_ar, in_ir);
        int resul = (int)((60 * in_asa) / (in_ar - in_ir));
        *ou_water_time = resul;
        // *ou_water_time *= 60;
        // DEBUG_TEST( DB_I,"( %.02f / %.02f - 1 ) * %d  water result  = %d min", in_ar, in_ir, in_sack_should_run_time, resul);
        DEBUG_TEST(DB_I, "*max: - %d", resul);
    }
    // DEBUG_TEST( DB_I,"*max: - %d", result);
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
        for (int i = 0; i < max_channel; i++)
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
            if (arr[i].cd_channel_type == /*M_STATIC_CHANNEL_TYPE_WATERSAVING*/ 0)
            {

                for (int p = 1; p <= GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - 3; p++) // GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr , i)不会小于4
                {
                    // 计算运行总时间
                    // m_static_algorithm_instance_cost(GET_CHANNEL_SHOULD_RUN_TIME(arr, i), GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1, &sum);
                    //    应该运行时间 对  浸泡应该运行时间  取余
                    remainder = (GET_CHANNEL_SHOULD_RUN_TIME(arr, i) * 1.0) / ((GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) * 1.0) - p + 1);
                    printf("\t(%d/%d)   %.02f <= %d\n", GET_CHANNEL_SHOULD_RUN_TIME(arr, i), (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1), remainder, ((int)remainder) + 1);

                    if (remainder + 0.9999999 <= ((int)remainder) + 1) // 如果整除了
                    {
                        result_x = (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1);
                        sum = (int)remainder * result_x;
                        printf("%d*%d\n", (int)remainder, result_x);

                        break;
                        // 总运行时间 取余 应该运行时间（7.6.5.4）  大于上一个 小于 应该运行时间
                    }
                    // 那就计算下总运行时间 总运行时间小获胜 比如 664 和 554 明显 554 更适合
                    // 如果 轮数*计算的最佳时间 + FIX_TIME_MIN < 应该运行的总时间
                    else if (
                        ((int)remainder - 1) * (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1) + FIX_TIME_MIN < GET_CHANNEL_SHOULD_RUN_TIME(arr, i))
                    {
                        // approaching_zero = ((int)remainder - 1); // GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - ((int)remainder-1) * (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1);
                        result_x = (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1);
                        printf("%d*%d+%d\n", (int)remainder, result_x, GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - ((GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1)) * ((int)remainder));
                        arr[i].cd_last_running_time = GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - ((GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1)) * ((int)remainder);
                        // 如果 应该运行的时间 - 轮数 * 最佳时间 大于 FIX_TIME_MIN   (最好不用减法，会出现bug)
                        // 如果  轮数 * 最佳时间 + FIX_TIME_MIN 大于 应该运行的时间
                        if (((int)remainder) * (GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) - p + 1) + FIX_TIME_MIN <= GET_CHANNEL_SHOULD_RUN_TIME(arr, i))
                            break;
                        sum = (int)remainder * result_x + GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - ((int)remainder * result_x);
                    }
                }
                // 有结果了 设置一下
                if (result_x != 0)
                    SET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i, result_x);
            }
            else
            {
                // 其他基本不变
            }
            // if(arr[i].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)

            printf("区域%d ->  最佳浇水时长 %d  浸泡%d（不变）\n\n", GET_CHANNEL_ID(arr, i), GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i), GET_CHANNEL_SACK_SHOULD_SACK_TIME(arr, i));
        }
        // 高定
    }
    return 0;
}

// /**
//  * @brief 作为计划浸泡模型生成的函数 （根据参数生成应该运行的数据，可以单独使用）
//  * @param arr 区域信息
//  * @param max_channel 最大区域
//  * @param out_sequences  输出序列
//  * @param in_tim_s  当前的时间
//  */
// int m_static_instance_sack_algorithm_pro_max(struct channel_config *arr, int max_channel, long in_tim_s)
// {

//     int result = 0;
//     int pc = 0;
//     //  准备单元序列
//     // struct instance_running_unit new_running_unit = {0};

//     if (arr == NULL || max_channel <= 0 || max_channel > 16)
//         printf("区域或者最大区域数不正确 ！\n"), result = 1;

//     if (!result)
//     {
//         //  每个区域均衡算法
//         for (int i = 0; i < max_channel; i++)
//         {
//             // 均衡的是各个区域的浇水时间，范围应该是 ( 4 到 设置的应该浇水时间 )
//             // 递减推进计算出最合适的浇水时间和轮数
//             // remainder 计算出来的除数
//             double remainder = 0.0f;
//             //  approaching_zero 余数
//             int approaching_zero = 9999;
//             int sum = 0;
//             int result_x = 0;
//             //  填充单元序列
//             if (arr[i].cd_should_run_time <= 0 || arr[i].cd_sack_should_water_time <= 0 || arr[i].cd_sack_should_sack_time <= 0 || arr[i].cd_cahnnel_id <= 0)
//             {
//                 printf("该区域参数错误 ！\n");
//                 return 1;
//             }
//             // 如果是节水模式的话
//             if (arr[i].cd_channel_type == /*M_STATIC_CHANNEL_TYPE_WATERSAVING*/ 1)
//             {
//                 for (int p = 1; p <= GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - 3; p++) // GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr , i)不会小于4
//                 {
//                     // 除数 14/2 = 7   13 /2 = 6
//                     remainder = (GET_CHANNEL_SHOULD_RUN_TIME(arr, i) * 1.0) / (p * 1.0 + 1.0);
//                     // 余数
//                     approaching_zero = GET_CHANNEL_SHOULD_RUN_TIME(arr, i) % ((int)remainder);
//                     if (remainder >= FIX_TIME_MIN)
//                     {
//                         if (remainder + 0.99999 <= ((int)remainder) + 1 && remainder < GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i)) // 如果整除了
//                         {
//                             // printf("ok 最佳%d   -%d-   最后%d\n", (int)remainder, p + 1, (int)remainder);
//                             printf("%d * %d\n", (int)remainder, p + 1);
//                             arr[i].cd_last_running_time = 0;
//                             result_x = (int)remainder;
//                             break;
//                         }
//                         else if ((GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - (p) * ((int)remainder)) <= GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i) &&
//                                  remainder <= GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i))
//                         {
//                             // printf("最佳%d   -%d-   最后%d\n", (int)remainder, p , GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - (p) * ((int)remainder ));
//                             arr[i].cd_last_running_time = GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - (p) * ((int)remainder);
//                             printf("%d * %d + %d\n", (int)remainder, p, arr[i].cd_last_running_time);
//                             result_x = (int)remainder;
//                             break;
//                         }
//                     }
//                     else
//                     {
//                         arr[i].cd_last_running_time = GET_CHANNEL_SHOULD_RUN_TIME(arr, i) - p * (FIX_TIME_MIN);
//                         if (arr[i].cd_last_running_time < FIX_TIME_MIN)
//                             arr[i].cd_last_running_time = FIX_TIME_MIN;
//                         printf("%d * %d + %d\n", FIX_TIME_MIN, p, arr[i].cd_last_running_time);

//                         result_x = FIX_TIME_MIN;
//                         SET_CHANNEL_SHOULD_WATER_TIME(arr, i, FIX_TIME_MIN * p + arr[i].cd_last_running_time);
//                         break;
//                     }
//                     // printf("l%d ",GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i));
//                 }
//                 // 有结果了 设置一下
//                 if (result_x != 0)
//                     SET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i, result_x);
//             }
//             else
//             {
//                 // 其他基本不变
//             }

//             printf("区域%d ->  最佳浇水时长 %d  浸泡%d（不变）\n\n", GET_CHANNEL_ID(arr, i), GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, i), GET_CHANNEL_SACK_SHOULD_SACK_TIME(arr, i));
//         }
//         // 高定
//     }
//     return result;
// }
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

    if (arr == NULL || max_channel <= 0 || max_channel > 40)
    {
        DEBUG_TEST(DB_I, "区域或者最大区域数不正确 ！\n");
        result = 1;
    }

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
            int approaching_zero = 0;
            int sum = 0;
            /** 浸泡应该浇水时间的结果 */
            int result_x = 0;
            //  填充单元序列
            if (arr[loop_channel_index_i].cd_should_run_time <= 0 || arr[loop_channel_index_i].cd_cahnnel_id <= 0 ||
                (arr[loop_channel_index_i].cd_channel_type == M_STATIC_CHANNEL_TYPE_SACK && (arr[loop_channel_index_i].cd_sack_should_water_time <= 0 || arr[loop_channel_index_i].cd_sack_should_sack_time <= 0)) ||
                (arr[loop_channel_index_i].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING && arr[loop_channel_index_i].cd_cahnnel_id > 6 && (arr[loop_channel_index_i].cd_ar == 0 || arr[loop_channel_index_i].cd_asa == 0 || arr[loop_channel_index_i].cd_ir == 0)))
            {
                DEBUG_TEST(DB_I, "该区域参数错误 ！\n");
                return 1;
            }

            // 如果是节水模式的话
            if (arr[loop_channel_index_i].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
            {
                // if (arr[loop_channel_index_i].cd_should_run_time % 60 == 0)
                arr[loop_channel_index_i].cd_should_run_time /= 60;

                m_static_instance_algorithm_water_time(arr[loop_channel_index_i].cd_cahnnel_id, arr[loop_channel_index_i].cd_asa, arr[loop_channel_index_i].cd_ar, arr[loop_channel_index_i].cd_ir, &arr[loop_channel_index_i].cd_sack_should_water_time);
                //   if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
                bool is_ok = false;
                if (GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) > arr[loop_channel_index_i].cd_sack_should_water_time)
                {
                    for (int p = GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) / 2; p > 4; p--) // GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr , loop_channel_index_i)不会小于4
                    {
                        remainder = GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) / (p);
                        // { // 除数 14/2 = 7   13 /2 = 6
                        if (GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) % (p) == 0 &&
                            p <= arr[loop_channel_index_i].cd_sack_should_water_time)
                        {
                            approaching_zero = 0;
                            result_x = p;
                            is_ok = true;
                            break;
                        }
                        else if (GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) % (p) > approaching_zero &&
                                 p <= arr[loop_channel_index_i].cd_sack_should_water_time)
                        {
                            result_x = p;
                            approaching_zero = GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i) % (p);
                            // if(approaching_zero < )
                            // DEBUG_TEST( DB_I," ./(%d) =  %d  \%= %d\n", p,remainder, approaching_zero);
                        }
                    }
                }
                else
                {
                    result_x = arr[loop_channel_index_i].cd_sack_should_water_time;
                    is_ok = true;
                    // DEBUG_TEST( DB_I,"Not Sack Because -> %d > %d  ", result_x, GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i));
                }

                // 有结果了 设置一下
                if (result_x != 0)
                    SET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i, result_x);
                /** 浸泡 最后算下，根据浇水时间 */
                m_static_instance_algorithm_sack_time(arr[loop_channel_index_i].cd_cahnnel_id, arr[loop_channel_index_i].cd_ar, arr[loop_channel_index_i].cd_ir, arr[loop_channel_index_i].cd_asa, arr[loop_channel_index_i].cd_sack_should_water_time, &arr[loop_channel_index_i].cd_sack_should_sack_time);
                arr[loop_channel_index_i].cd_sack_should_water_time *= 60;
                DEBUG_TEST(DB_I, "计算出区域浸泡浇水: Watrt: %d min - S*60: %d min ", result_x, arr[loop_channel_index_i].cd_sack_should_sack_time / 60);
                arr[loop_channel_index_i].cd_should_run_time *= 60;
            }
            else
            {
                // 其他基本不变
            }
            if (arr[loop_channel_index_i].cd_channel_type != M_STATIC_CHANNEL_TYPE_NORMAL)
            {
                // DEBUG_TEST( DB_I,"区域%d ->  最佳浇水时长 %d  浸泡%d（不变）\n\n", GET_CHANNEL_ID(arr, loop_channel_index_i), GET_CHANNEL_SACK_SHOULD_WATER_TIME(arr, loop_channel_index_i), GET_CHANNEL_SACK_SHOULD_SACK_TIME(arr, loop_channel_index_i));
            }
            else
            {
                // DEBUG_TEST( DB_I,"区域%d ->  (无浸泡)最佳浇水时长 %d  浸泡%d（不变）\n\n", GET_CHANNEL_ID(arr, loop_channel_index_i), GET_CHANNEL_SHOULD_RUN_TIME(arr, loop_channel_index_i), 0);
            }
        }
        // 高定
    }
    return result;
}

// void main()
// {
//     struct instance_running_sequences inst_queue;
//     int max_channel_num = 0;
//     int ste = 0;
//     /** 1、 定义运行区域 (每一个参数必填) */
//     struct channel_config Array[16] =
//         {
//             {
//                 .cd_cahnnel_id = 1,                                   // 区域
//                 .cd_should_run_time = 20,                             // 总运行时间
//                 .cd_sack_should_water_time = 5,                       // 应该运行时间
//                 .cd_channel_type = M_STATIC_CHANNEL_TYPE_WATERSAVING, // 区域类型
//                 .cd_sack_should_sack_time = 4,                        // 应该浸泡时间
//             },
//             {
//                 .cd_cahnnel_id = 2,                                   // 区域
//                 .cd_should_run_time = 6,                              // 总运行时间
//                 .cd_channel_type = M_STATIC_CHANNEL_TYPE_WATERSAVING, // 区域类型
//                 .cd_sack_should_water_time = 4,                       // 应该运行时间
//                 .cd_sack_should_sack_time = 4,                        // 应该浸泡时间
//             },
//         };
//     /** 2、 根据区域填写区域数量*/
//     max_channel_num = 2;

//     printf("Start..\n");
//     // 节水数据计算
//     ste = m_static_instance_sack_algorithm_pro_max(Array, max_channel_num, 0);
//     if (!ste)
//     {
//         // 运行数据生成
//         m_static_instance_sack_algorithm_queue_gener(Array, max_channel_num, 0, &inst_queue, 0);
//         // 运行数据输出
//         m_static_instance_runnning_queue_print(&inst_queue);
//     }
//     printf("Done..\n");
// }

/*********/
/*********/
/*********/
/*********/
/*********/
/*********/
/*********/

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
int m_static_instance_tools_calcute_instance_time_sum_and_current_staus(struct channel_config *arr, int max_channel, int water_start_time, uint64_t *out_cost_time, long in_tim_s)
{
    struct instance_running_unit sign_running_unit;
    int result = 0, pc = 0;
    // 泵模式用来运行的标记参数
    bool cd_pump_done = false;
    // 填充单元序列
    // 根据前面的市场推算出执行步骤 应该是没有问题的
    int presentMaxChan = 0, final_water_sum = 0, dist = in_tim_s - water_start_time;
    // 计划开始的时间到推算的时间 （根据计划内容不断推进，直到当前时间，或者计划应该结束时间）
    long temp_present_time = 0;
    // long temp_present_time = in_tim_s;
    while (presentMaxChan < max_channel)
    {
        // if (arr[presentMaxChan].cd_channel_type != M_STATIC_CHANNEL_TYPE_NORMAL)
        {
            for (int n = 0; n < max_channel;) //_device.hardware.type;)
            {
                if ((arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_NORMAL ||
                     arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP) &&
                    arr[n].cd_should_run_time > arr[n].cd_reality_run_time &&
                    !cd_pump_done)
                {
                    {
                        arr[n].cd_reality_run_time = arr[n].cd_should_run_time;
                        final_water_sum += arr[n].cd_should_run_time;
                        temp_present_time += arr[n].cd_should_run_time;
                        // 累积时间
                        arr[n].cd_running_end_time = temp_present_time;

                        // DEBUG_TEST( DB_I,"NOR---> 区域 %d 浇水 %d 完成-%d    (%d , %d)   ", arr[n].cd_cahnnel_id, arr[n].cd_sack_should_water_time,
                        //            arr[n].cd_should_run_time, arr[n].cd_reality_run_time, final_water_sum);
                        // 泵模式标记，下一次一定运行浸泡
                        if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP)
                            cd_pump_done = true;
                        presentMaxChan++;
                    }
                }
                // else if ((arr[n].cd_should_run_time > 0 && arr[n].cd_should_run_time > arr[n].cd_reality_run_time))
                else if ((arr[n].cd_should_run_time > 0 && arr[n].cd_should_run_time > arr[n].cd_reality_run_time) || (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && cd_pump_done))
                {
                    // 运行还是浸泡
                    // 计划开始的时间到推算的时间 - 上一次运行的时间标记  大于浸泡时间的花 那么应该运行     否则 应该浸泡
                    // if (arr[n].cd_cahnnel_id == 13)
                    // DEBUG_TEST(DB_W," pr t: %lld end t: %lld, sack time: %d", temp_present_time, arr[n].cd_running_end_time, arr[n].cd_sack_should_sack_time);
                    // if (!cd_pump_done && (arr[n].cd_running_end_time == 0 || temp_present_time - arr[n].cd_running_end_time >= arr[n].cd_sack_should_sack_time) || (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && cd_pump_done))
                    if (!cd_pump_done && (arr[n].cd_running_end_time == 0 || temp_present_time - arr[n].cd_running_end_time >= arr[n].cd_sack_should_sack_time))

                    {
                        // 剩下的应该运行的时间 > 浸泡应该浇水的时间     =>>  直接浇水  cd_sack_should_water_time
                        // 运行完整时长还是断续时长
                        // DEBUG_TEST( DB_I,"%hhd" , arr[n].cd_sack_should_water_time);
                        // DEBUG_TEST( DB_I,"%hhd" , arr[n].cd_sack_should_sack_time);
                        if (arr[n].cd_reality_run_time + arr[n].cd_sack_should_water_time <= arr[n].cd_should_run_time &&
                            arr[n].cd_reality_run_time + arr[n].cd_last_running_time < arr[n].cd_should_run_time)
                        {
                            arr[n].cd_reality_run_time += arr[n].cd_sack_should_water_time;
                            final_water_sum += arr[n].cd_sack_should_water_time;
                            temp_present_time += arr[n].cd_sack_should_water_time;
                            // DEBUG_TEST(DB_W,"quyu %d , zuihou end %lld " ,  arr[n].cd_cahnnel_id ,temp_present_time);
                            // 累积时间
                            arr[n].cd_running_end_time = temp_present_time;
                            // if (arr[n].cd_cahnnel_id == 13)

                            { // 加入、加入、加入
                              // DEBUG_TEST( DB_I,"---> 区域 %d 本次浇水 %d 总-%d    (已经%d , 已经运行时间%d)   ", arr[n].cd_cahnnel_id, arr[n].cd_sack_should_water_time,
                              //            arr[n].cd_should_run_time, arr[n].cd_reality_run_time, final_water_sum);
                            }
                            if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
                                m_static_instance_algorithm_sack_time(arr[n].cd_cahnnel_id, arr[n].cd_ar, arr[n].cd_ir, arr[n].cd_asa, arr[n].cd_sack_should_water_time, &arr[n].cd_sack_should_sack_time);
                            // m_static_instance_algorithm_sack_time(arr[loop_channel_index_i].cd_cahnnel_id, arr[loop_channel_index_i].cd_ar, arr[loop_channel_index_i].cd_ir, arr[loop_channel_index_i].cd_sack_should_water_time, &arr[loop_channel_index_i].cd_sack_should_sack_time);

                            if (arr[n].cd_should_run_time <= arr[n].cd_reality_run_time)
                                presentMaxChan++;
                        }
                        else
                        {

                            // DEBUG_TEST( DB_I,"LAST---> 区域 %d 本次浇水 %d 总-%d    (已经%d , 已经运算时间%d) ", arr[n].cd_cahnnel_id, arr[n].cd_should_run_time - arr[n].cd_reality_run_time,
                            //            arr[n].cd_should_run_time, arr[n].cd_reality_run_time, final_water_sum);
                            if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING && arr[n].cd_should_run_time - arr[n].cd_reality_run_time < 4 * 60)
                            {
                                final_water_sum += 4 * 60;
                                temp_present_time += 4 * 60;
                                arr[n].cd_reality_run_time = arr[n].cd_should_run_time;
                                arr[n].cd_running_end_time = temp_present_time;
                            }
                            else
                            {
                                // 算出需要进步的，加上
                                final_water_sum += arr[n].cd_should_run_time - arr[n].cd_reality_run_time;
                                temp_present_time += arr[n].cd_should_run_time - arr[n].cd_reality_run_time;
                                arr[n].cd_reality_run_time = arr[n].cd_should_run_time;
                                arr[n].cd_running_end_time = temp_present_time;
                            }
                            // m_static_instance_algorithm_sack_time(arr[n].cd_cahnnel_id , arr[n].cd_ar , arr[n].cd_ir , arr[n].cd_sack_should_water_time , &arr[n].cd_sack_should_sack_time);
                            if (arr[n].cd_should_run_time <= arr[n].cd_reality_run_time)
                                presentMaxChan++;
                        }
                        n++;
                        // temp_chan_should_water = arr[n].cd_should_run_time;
                    }
                    else
                    { // 要浸泡
                        int sct = 0;
                        if (arr[n].cd_should_run_time - arr[n].cd_reality_run_time > 0 || (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && cd_pump_done))

                        {

                            if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && arr[n].cd_sack_should_sack_time > 0)
                            {
                                sct = arr[n].cd_sack_should_sack_time;
                                // arr[n].cd_sack_should_sack_time = 0;
                                if (arr[n + 1].cd_cahnnel_id <= 0)
                                {
                                    break;
                                }
                                cd_pump_done = false;
                            }
                            else
                            {
                                sct = (arr[n].cd_sack_should_sack_time - (temp_present_time - arr[n].cd_running_end_time));
                            }
                            {
                                //  sct = (arr[n].cd_sack_should_sack_time - (temp_present_time - arr[n].cd_running_end_time));
                                if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP)
                                    n++;
                                // DEBUG_TEST( DB_I,"SACK---> 区域 %d 浸泡    (%d,%d)  end %lld", arr[n].cd_cahnnel_id,
                                //            sct, final_water_sum, arr[n].cd_running_end_time /** arr[n].cd_sack_should_sack_time - (final_water_sum - arr[n].cd_running_end_time) **/);

                                final_water_sum += sct;
                                // final_water_sum += sct;
                                // final_water_sum += (arr[n].cd_sack_should_sack_time - (final_water_sum - arr[n].cd_running_end_time));
                                temp_present_time += sct;
                            }
                        }
                    }
                }
                else
                    n++; //, temp_chan_should_water = arr[n].cd_should_run_time;
            }
            // if (presentMaxChan == 0)
            //     break;
            // presentMaxChan = 0;
        }
    }
    DEBUG_TEST(DB_W, "comp: max coust  %d\n", final_water_sum);
    *out_cost_time = final_water_sum;
    return result;
}

/**
 * @brief 根据计划开始时间和计算间隔5小时的开始时间
 *
 * @param in_present_time 传入现在的时间点 hhmmss
 * @param in_present_timespeam 传入现在的时间戳
 * @param inver_hour 运行的时间
 * @param planTimesp 计划开始时间戳
 * @param [out] out_waterTime 计算得出的结果存放
 */
void m_static_instance_tools_time_calculate_the_interval_hour(uint32_t in_present_time,
                                                              uint32_t in_present_timespeam, uint8_t inver_hour, uint64_t planTimesp, uint64_t *out_waterTime) // 传入计划的时间戳 ，
{
    DEBUG_TEST(DB_I, "m_operation_schedule_calculate_the_interval_hour %hhd", inver_hour);
    int tcc = inver_hour * 3600;
    tcc %= NUM_OF_DAY_SEC_SUM;
    if (tcc == 18000)
    { /** 现在的时间大于计划开始时间*/
        if (in_present_timespeam > planTimesp)
        {
            int inv = 0;
            DEBUG_TEST(DB_I, "*current Time :%d  planTimesp %lld", in_present_timespeam, planTimesp);
            /** 1、开始到现在的时间小时数 */
            // (in_present_timespeam - planTimesp) % 3600 ;
            /** 2、间隔小时 取余 少了加 inv-x 小时 */
            inv = ((in_present_timespeam - planTimesp) / tcc);
            DEBUG_TEST(DB_I, "in - Hour :%d ", inv);
            /** 加上计划开始时间 + 需要递进的时间 */
            // *out_waterTime += planTimesp;
            *out_waterTime += (inver_hour - inv) * 3600;
            DEBUG_TEST(DB_I, "%lld", *out_waterTime);
        }
    }
    else
    {
        DEBUG_TEST(DB_I, "..%d..%lld..%hhd..\r\n", in_present_time, *out_waterTime, tcc);
        if (in_present_time > *out_waterTime)
        {
            while ((*out_waterTime) < in_present_time)
                *out_waterTime = ((*out_waterTime + tcc));
        }
        else
            while ((*out_waterTime) > tcc + in_present_time)
                *out_waterTime = ((*out_waterTime - tcc));
        // DEBUG_TEST( DB_I,"...\r\n");
        *out_waterTime %= NUM_OF_DAY_SEC_SUM;
    }
}

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
{
    stat_m stat = fail_r;
    bool next_ok = false, pre_ok = false;
    for (int i = 0; i < 24; i++)
    {

        if (!next_ok && in_arr_list[i] > curr_time && next_time != NULL)
        {
            *next_time = in_arr_list[i];
            next_ok = true;
            stat = succ_r;
            break;
        }
        else if (!pre_ok && pre_time != NULL)
        {
            // if ((i > 0 && in_arr_list[i] > 0) || i == 0)
            {
                // if ((i > 0 && in_arr_list[i + 1] == 0))
                pre_ok = true;
                *pre_time = in_arr_list[i];
            }
        }
    }
    if (!pre_ok && pre_time != NULL)
    {
        *pre_time = in_arr_list[0];
    }
    if (!next_ok && next_time != NULL)
    {
        *next_time = in_arr_list[0];
    }
    return stat;
}

//
