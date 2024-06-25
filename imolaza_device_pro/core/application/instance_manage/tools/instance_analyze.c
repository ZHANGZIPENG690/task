// #include "core/fset_lib.h"
// #include "../instance.h"
// #include "instance_algorithm.h"

// instance_obj *m_current_instance = NULL;

// /** 计算计划在任意时间点的计算和恢复 */
// stat_m m_instnce_recover_and_build_arbitrarily()
// {
//     return 0;
// }

// #define TEMP_WORD_PUBLIC_NULL "NULL"
// #define M_HD "%hd"
// #define M_INV7 "&"

// stat_m m_static_instance_manage_explain_instance_obj(instance_doc_t *in_info, uint64_t in_time_s)
// {

//     stat_m stat = succ_r;
//     char temp_ac[20] = {0};

//     // int temp_var_a = 0;
//     // int temp_var_b = 0;
//     DEBUG_TEST(DB_I,"plan_virtual_init %lld...", in_info->sch_info.cu_id);

//     if (in_info->sch_info.cu_id <= 0 ||
//         in_info->sch_info.available_time.tr_start_time < 1000 ||
//         in_info->sch_info.available_time.tr_end_time < 1000 ||
//         strlen(in_info->sch_info.cu_zone) < 2)
//     {
//         stat = fail_r;
//     }
//     if (stat != fail_r)
//     {

//         { /** Next : null or 1&2 (Sack) or 2|10 (Pump Mode)*/
//             if (strstr(in_info->sch_info.cu_sack, M_INV7) != NULL)
//             {
//                 // in_info->exfun->fun_soak_use = true;
//                 m_callable_data_parse_foramt_param(M_STRM_STR_3(M_HD, "&", M_HD),
//                                                    &in_info->exfun.fn_soak_water,
//                                                    &in_info->exfun.fn_soak_cycle,
//                                                    in_info->sch_info.cu_sack);
//                 printf("{%s}%d - %d" ,in_info->sch_info.cu_sack, in_info->exfun.fn_soak_water,in_info->exfun.fn_soak_cycle);
//                 in_info->exfun.fn_run_type = M_M_FUNCATION_SACK;
//             }
//             else if (strstr(in_info->sch_info.cu_sack, "|") != NULL)
//             {
//                 // in_info->exfun.fun_soak_use = false;
//                 m_callable_data_parse_foramt_param(M_STRM_STR_3(M_HD, "|", M_HD),
//                                                    &in_info->exfun.fn_solenoid_mode_time,
//                                                    &in_info->exfun.fn_solenoid_mode_time,
//                                                    in_info->sch_info.cu_sack);
//                 in_info->exfun.fn_run_type = M_M_FUNCATION_PUMP;
//             }
//             else if(in_info->sch_info.cu_sack == M_STATIC_CHANNEL_CONFIG_A)
//             {

//             }
//             else if(in_info->sch_info.cu_sack == M_STATIC_CHANNEL_CONFIG_B)
//             {

//             }
//             else
//             {
//                 // in_info->exfun.fun_soak_use = false;
//                 DEBUG_E("sch_info.cu_sack Err --> (%s)", in_info->sch_info.cu_sack);
//                 in_info->exfun.fn_run_type = M_M_FUNCATION_NONE;
//             }
//             in_info->sch_info.cu_src_execute_time_str[3] = ' ';
//         }

//         /** Next : 100 08:00*/
//         char temp_str_1[10] = {0};
//         int temp_var_a = 0;
//         int temp_var_b = 0;
//         m_callable_data_parse_foramt_param("%hhd %s",
//                                            &in_info->execu_type,
//                                            temp_str_1,
//                                            in_info->sch_info.cu_src_execute_time_str);
//         if (in_info->execu_type == SCHEDULE_RUN_TYPE_AT || in_info->execu_type == SCHEDULE_RUN_TYPE_BEFROE)
//         {
//             printf("\n%s ," ,temp_str_1 );
//             m_callable_data_parse_foramt_param("%d:%d",
//                                                &temp_var_a,
//                                                &temp_var_b,
//                                                temp_str_1);
//             printf(" %d %d," , temp_var_a , temp_var_b );
//             in_info->WSTime = temp_var_a * 3600 + temp_var_b*60;
//         }
//         char ZONE_FORMAN_SEPA[10] = "%hhd#%hhd"; // 区域格式
//         char ZONE_FORMAN_END = ';';               // 区域结束

//         /** Next : Zone info */
//         // 解析及区域参数，运行时间总和 区域单数
//         m_static_tools_instance_obi_explain_zone(in_info,
//                                                  ZONE_FORMAN_SEPA,
//                                                  ZONE_FORMAN_END,
//                                                  in_info->sch_info.cu_zone);
//         // // 计算总花费时间
//         // if (in_info->exfun.fn_run_type == M_M_FUNCATION_SACK || in_info->exfun.fn_run_type == M_M_FUNCATION_PUMP)
//         //     m_static_instance_obj_tools_calcute_instance_time_sum_and_current_staus(NULL,
//         //                                                                             in_info->exfun.fn_soak_water,
//         //                                                                             in_info->exfun.fn_soak_cycle,
//         //                                                                             in_info->cmg_channel,
//         //                                                                             in_info->exfun.fn_max_should_runtime,
//         //                                                                             in_time_s, &in_info->exfun.fn_max_coust_time);
//         // else
//         //     in_info->exfun.fn_max_coust_time = in_info->exfun.fn_max_should_runtime;

//         /** Next : Determine Start Time  [sch_info.cu_execute_type] 100 - 101 103 104 */
//         if (
//             in_info->sch_info.frequency_type == SCHEDULE_RUN_TYPE_SUNRISE ||
//             in_info->sch_info.frequency_type == SCHEDULE_RUN_TYPE_SUNSET)
//         {
//             /* 根据计划开始时间，计算当天的日出和日落时间*/
//             // m_suninfo_specity_date(&certain_day_sunset,&certain_day_sunrise , in_info->sch_info.cu_execute_time.tr_start_time);
//             // in_info->WSTime = in_info->running.rb_WSTime_clone = 47000;
//         }

//         /** interval hour and (sunset / sunrise)  ==  AT + interval hour */
//         if (in_info->sch_info.frequency_type == SCHEDULE_RUN_TYPE_SUNSET &&
//             in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
//             in_info->sch_info.frequency_type = SCHEDULE_RUN_TYPE_AT;

//         if (in_info->sch_info.frequency_type == SCHEDULE_RUN_TYPE_SUNRISE &&
//             in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
//             in_info->execu_type = SCHEDULE_RUN_TYPE_BEFROE;
//         if (in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR || in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERTAL_DAY)
//         {
//             M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_week_list, "%hhd", &in_info->sch_info.cu_interval_hour_or_day);
//         }
//         /** Next : Determine Start Time 2 */
//     }

//     return stat;
// }

// // instance_obj *current_instance = NULL;

// stat_m m_static_instance_manage_set_current_instance(instance_obj *instance)
// {
//     stat_m stat = fail_r;
//     m_current_instance = instance;
//     return stat;
// }

// stat_m m_static_instance_manage_get_current_instance(instance_obj **instance)
// {
//     stat_m stat = fail_r;
//     *instance = m_current_instance;
//     if (instance != NULL)
//         stat = succ_r;
//     return stat;
// }

#include "core/fset_lib.h"
#include "../instance.h"
#include <math.h>

/**
 * @brief 找到下一个可以运行的区域，并把当前指针指向他
 *
 * @param root 根节点
 * @return stat_m
 */
stat_m m_static_instance_manage_tool_find_next_sequence(struct instance_running_sequences *root)
{
    stat_m stat = fail_r;
    struct instance_running_unit *temp = NULL;
    int loop_max = 1000;
    if (root != NULL)
    {
        temp = root->current_running_unit;

        while (temp != NULL)
        {
            loop_max--;
            if (!loop_max)
                break;
 
            if (temp->next_running_unit == NULL)
                temp = root->head_running_unit;
            else
                temp = temp->next_running_unit;
                
            if (temp->unit_this_time_will_running_time > temp->unit_this_time_cost_running_time + 2 &&
                temp->unit_running_type != M_BASE_STATUS_SACK)
            {
                // 是你
                root->current_running_unit = temp;
                stat = succ_r;
                break;
            }
        }
        DEBUG_TEST(DB_W,"Find Next is %hhd", root->current_running_unit->unit_running_channel);
    }
    return stat;
}
/**
 * @brief 找到上一个可以运行的区域，并把当前指针指向他
 *
 * @param root 根节点
 * @return stat_m
 */
stat_m m_static_instance_manage_tool_find_pre_sequence(struct instance_running_sequences *root)
{
    stat_m stat = fail_r;
    struct instance_running_unit *temp = NULL;
    int loop_max = 1000;

    if (root != NULL)
    {
        temp = root->current_running_unit;
        while (temp != NULL)
        {
            loop_max--;
            if (!loop_max)
                break;
            if (temp->pre_running_unit != NULL) // 如果不是NULL
            {
                temp = temp->pre_running_unit;
                //  ，怎么判断当前区域运行完成，，，，需要运行中设置一下     有Done
                // if (temp->state == M_STATIC_UNIT_STATUS_INTERRUPT || temp->state == M_STATIC_UNIT_STATUS_NEW)
                if (temp->unit_this_time_will_running_time > temp->unit_this_time_cost_running_time + 2 && temp->unit_running_type != M_BASE_STATUS_SACK)
                {
                    // 是你
                    root->current_running_unit = temp;
                    stat = succ_r;
                    break;
                }
            }
            else // 否则的话，回到头节点
            {
                temp = root->tail_running_unit;
                // if (temp != NULL)
                // {
                //     root->current_running_unit = temp;
                //     stat = succ_r;
                // }
                break;
            }
        }
        DEBUG_TEST(DB_W,"Find Pre is %hhd", root->current_running_unit->unit_running_channel);
    }
    return stat;
}
static const char M_STR_EN_DE_CODE[] = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx"};

/**
 * @brief 字符编码  把数字编码成 字符
 *
 * @param conchr 返回对应的字符
 * @param in_num 传入数字
 * @param encode_num 编码位数   最大 5 位
 * @return int
 */
void m_callable_time_num_to_str_encrypt_encode(char *conchr, int in_num, int encode_num)
{
    memset(conchr, 0, sizeof(*conchr));

    for (int i = 5; i >= 0; i--)
    {
        if (i < encode_num)
        {
            if (i == 0)
            {
                // printf(". %d      %d\n", i, in_num % 60);
                conchr[encode_num - i - 1] = M_STR_EN_DE_CODE[in_num % 60];
            }
            else if (i == 1)
            {
                // printf("%d      %d\n", i, in_num / (int)pow(60, i));
                conchr[encode_num - i - 1] = M_STR_EN_DE_CODE[(in_num / (int)pow(60, i))%60];
            }
            else if (i == 2)
            {
                // printf("%d      %d\n", i, in_num / (int)pow(60, i));
                conchr[encode_num - i - 1] = M_STR_EN_DE_CODE[(in_num / (int)pow(60, i))];
            }
        }
    }
}

void m_callable_time_str_to_num_decrypt_decode(char *contr, int *out_num, int bit_num, int len)
{
    int count_bit = 0;
    int bt_sum = bit_num;
    *out_num = 0;
    for (int i = 0; i <= len; i++)
    {
        if (*contr != ' ' && *contr != '\0')
        {
            for (int x = 0; x < 60; x++)
            {
                if (*contr == M_STR_EN_DE_CODE[x])
                {
                    bit_num--;
                    *out_num += x * (int)pow(60, bit_num);
                    count_bit++;
                    break;
                }
            }
            *contr = ' ';
            if (count_bit >= bt_sum)
                break;
        }
        else
            contr++;
    }
}
