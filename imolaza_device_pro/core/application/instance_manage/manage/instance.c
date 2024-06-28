

#include "../instance.h"
#include "core/fset_lib.h"
#include "../fsm/instance_fsm.h"
#include "../tools/instance_algorithm.h"

#define M_INSTANCE_MAX 1

static instance_obj *current_instance[M_INSTANCE_MAX] = {NULL};
int inst_num = 0;

#define TEMP_WORD_PUBLIC_NULL "NULL"
#define M_HD "%hd"
#define M_INV7 "&"

/** 这个参数的作用很重要，如果单纯判断对象实例是否NULL：不准确、有问题，用变量控制 就很好了 */
static bool limit_instance_object_is_null = false;

void m_static_instance_manage_monitor(void *args);

stat_m m_static_instance_manage_init(void)
{
    stat_m stat = fail_r;
    current_instance[0] = NULL;
    limit_instance_object_is_null = false;
#if !UNIT_TEST
    mTaskCreate(NULL, m_static_instance_manage_monitor, "__fsm", 512 * 6, NULL, 2, NULL);
#endif
    return stat;
}

void m_static_instance_manage_monitor(void *args)
{
    current_instance[0] = NULL;
    limit_instance_object_is_null = false;
    m_static_instance_running_fsm_init();
    while (1)
    {
        m_static_instance_running_fsm_monitor();
        mDelay_ms(50);
    }
}

stat_m m_static_instance_manage_set_current_instance(instance_obj *instance)
{
    stat_m stat = fail_r;
    // int fag_i = -1;
    // if (instance != NULL)
    // {
    //     for (int i = 0; i < M_INSTANCE_MAX; i++)
    //     {
    //         if (current_instance[i] == NULL && fag_i == -1)
    //         {
    //             fag_i = i;
    //         }
    //         if (current_instance[i] != NULL && current_instance[i]->sio_running_id == instance->sio_running_id)
    //         {
    //             fag_i = i;
    //             break;
    //         }
    //     }
    // }
    // if (fag_i != -1)
    // {
    //     current_instance[fag_i] = instance;
    //     stat = succ_r;
    // }
    current_instance[0] = instance;
    limit_instance_object_is_null = true;

    return stat;
}
/**
 * @brief 查看当前有没有运行计划
 *
 * @param is_schedule 想知道是计划还是快速运行 true 判断计划 false 判断快速运行
 * @return stat_m succ_r 是的
 */
stat_m m_static_instance_manage_current_instance_is_effective(bool is_schedule)
{
    stat_m stat = fail_r;

    if (limit_instance_object_is_null && current_instance[0] != NULL)
    {
        if (current_instance[0]->sio_run_type == M_INSTANCE_RUN_TYPE_FAST_RUN && !is_schedule)
            stat = succ_r;
        if (current_instance[0]->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE && is_schedule)
            stat = succ_r;
    }
    return stat;
}
/**
 * @brief 查看当前运行的计划是不是他
 *
 * @param in_uid 他
 * @return stat_m succ_r 是的
 */
stat_m m_static_instance_manage_current_instance_id_is_effective(uint64_t in_uid)
{
    stat_m stat = fail_r;
    if (limit_instance_object_is_null && current_instance[0] != NULL)
    {
        if (in_uid == current_instance[0]->sio_running_id)
            stat = succ_r;
    }
    return stat;
}
/**
 * @brief 查看当前为区域是影响
 *
 * @param in_uid 查询区域
 * @return stat_m
 */
stat_m m_static_instance_manage_current_channel_is_effective(int in_uid)
{
    stat_m stat = fail_r;
    if (limit_instance_object_is_null && current_instance[0] != NULL)
    {
        for (int i = 0; i < current_instance[0]->sio_max_channel; i++)
        {
            if (in_uid == current_instance[0]->sio_channel[i].cd_cahnnel_id)
                stat = succ_r;
        }
    }
    return stat;
}
/**
 * @brief 获取当前实例的运行 id 和 区域运行 id
 *
 * @param out_sh_id 输出的实例 id
 * @param out_ch_id 输出的区域 id
 * @return stat_m
 */
stat_m m_callable_instance_manage_get_current_instance_sid_abd_cid(uint64_t *out_sh_id, int *out_ch_state)
{
    stat_m stat = fail_r;
    if (limit_instance_object_is_null && current_instance[0] != NULL)
    {
        *out_sh_id = current_instance[0]->sio_running_id;
        *out_ch_state = current_instance[0]->instance_running_root_unit->current_running_unit->unit_running_type;
        if (*out_ch_state == M_BASE_STATUS_SACK)
            *out_ch_state = 254;
        else
            *out_ch_state = 1;
        stat = succ_r;
    }
    return stat;
}
int A = 20;
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
                                                                                          uint64_t in_time_s)
{
    int zonenum_buf = 0;
    stat_m stat = fail_r;
    memset(m_run_time.output_str, 0, sizeof(m_run_time.output_str));
    DEBUG_TEST(DB_W, "eiurgnueignuitegnu   %d , %d", limit_instance_object_is_null, limit_instance_object_is_null);
    if (limit_instance_object_is_null < A && current_instance[0] != NULL &&
        current_instance[0]->instance_running_root_unit != NULL &&
        current_instance[0]->instance_running_root_unit->current_running_unit != NULL)
    {
        *out_sh_id = current_instance[0]->sio_running_id;

        *out_instance_id = current_instance[0]->sio_instance_id;

        /** 获取运行队列数据 */
        m_static_instance_runnning_queue_get(out_running_queue);
        /** 运行位置 */
        *out_running_queue_local = current_instance[0]->instance_running_root_unit->current_running_unit->unit_progress;
        if (ch_id != NULL)
            *ch_id = current_instance[0]->instance_running_root_unit->current_running_unit->unit_running_channel;
        /** 单元剩余运行时间 */
        if (remainder_time != NULL)
        {
            m_static_time_link_notece_node_id_get_by_time(*out_sh_id, remainder_time);
            *remainder_time -= in_time_s;
            if (*remainder_time <= 0)
            {
                *remainder_time = current_instance[0]->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time - current_instance[0]->instance_running_root_unit->current_running_unit->unit_this_time_real_water_statistics;
            }

            if (m_callable_offline_bluetooth_login_flag() == succ_r)
            {
                if (current_instance[0]->sio_run_type == M_INSTANCE_RUN_TYPE_FAST_RUN) // 快速运行
                {
                    if (current_instance[0]->sio_status == M_BASE_STATUS_PAUSE) // 暂停
                    {
                        *remainder_time = m_run_time.total_duration_current_area[m_run_time.channel_number[*out_running_queue_local + 1]] - m_run_time.puase_duration_current_area[m_run_time.channel_number[*out_running_queue_local + 1]];
                    }
                }

                for (int i = 1; i <= m_run_time.zonenum; i++)
                {
                    if (i == *out_running_queue_local + 1)
                    {
                        m_run_time.remain_duration_current_area[m_run_time.channel_number[i]] = *remainder_time;
                    }
                    else
                    {
                        m_run_time.remain_duration_current_area[m_run_time.channel_number[i]] = m_run_time.total_duration_current_area[m_run_time.channel_number[i]] - m_run_time.puase_duration_current_area[m_run_time.channel_number[i]];
                    }
                    // DEBUG_TEST(DB_I, "区域 %d  剩余时长 %d", m_run_time.channel_number[i], m_run_time.remain_duration_current_area[m_run_time.channel_number[i]])
                }
                zonenum_buf = m_run_time.zonenum;
                m_callable_generatestring(m_run_time.channel_number, m_run_time.remain_duration_current_area, m_run_time.zonenum, m_run_time.output_str);
                m_run_time.zonenum = zonenum_buf;

                // DEBUG_TEST(DB_I, "output---》 %s ", m_run_time.output_str);
            }
            DEBUG_TEST(DB_I, "unit rasmin time: %lld", *remainder_time);
        }

        // 当前区域运行类型
        *out_ch_state = current_instance[0]->instance_running_root_unit->current_running_unit->unit_running_type;
        // 当前实例运行类型
        if (current_instance[0]->sio_run_type == M_INSTANCE_RUN_TYPE_FAST_RUN) // 快速运行
        {
            if (m_callable_current_batch_area_power_calibration_flag_get() == 1)
            {
                *out_ch_state = 8;
            }
            else if (m_callable_current_batch_area_power_calibration_flag_get() == 2)
            {
                *out_ch_state = 9;
            }
            else
            {
                if (current_instance[0]->sio_status == M_BASE_STATUS_PAUSE) // 暂停
                    *out_ch_state = 6;
                else if (*out_ch_state == M_BASE_STATUS_SACK) // 浸泡
                    *out_ch_state = 7;
                else if (*out_ch_state == M_BASE_STATUS_RUNNING) // 运行
                    *out_ch_state = 5;
                else
                    *out_ch_state = 5;
            }
        }
        else if (current_instance[0]->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE) // 计划
        {
            if (*out_ch_state == M_BASE_STATUS_RUNNING) // 运行
                *out_ch_state = 4;
            else if (*out_ch_state == M_BASE_STATUS_SACK) // 浸泡
                *out_ch_state = 3;
        }
        stat = succ_r;
    }
    else if (device_state == M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING)
    {
        *out_ch_state = 1;
        *out_sh_id = 254;
        if (ch_id != NULL)
            m_callable_manual_running_zone_get(ch_id);
        *remainder_time = 0;
    }
    else
    {
        *out_sh_id = 254;
        *out_ch_state = 2;
        *remainder_time = 0;
    }
    return stat;
}
stat_m m_static_instance_manage_get_current_instance(instance_obj **instance)
{
    stat_m stat = fail_r;
    // for (int i = 0; i < M_INSTANCE_MAX; i++)
    // {
    //     if ((current_instance[i] != NULL))
    //     {
    //         // current_instance[i] = instance;
    //         *instance = current_instance[i];
    //         stat = succ_r;
    //         DEBUG_TEST( DB_I,获取实例成功 > %d", i);
    //     }
    // }
    if (limit_instance_object_is_null && current_instance[0] != NULL)
        *instance = current_instance[0], stat = succ_r;
    return stat;
}
/**
 * @brief 清除运行数据
 *
 * @return stat_m
 */
stat_m m_static_instance_manage_clean_current_instance(void)
{
    stat_m stat = succ_r;
    limit_instance_object_is_null = false;
    current_instance[0] = NULL;
    DEBUG_TEST(DB_I, "m_static_instance_manage_clean_current_instance   --> Clean.. Ok");
    return stat;
}

char temp_str_1[200] = {0};
// 区域格式
char ZONE_FORMAN_SEPA[10] = "%hhd#%d";
// 区域结束
char ZONE_FORMAN_END = ';';
stat_m m_static_instance_manage_explain_instance_obj(instance_doc_t *in_info, uint64_t in_time_s)
{

    stat_m stat = succ_r;
    // char temp_ac[20] = {0};
    int temp = 0, temp_var_a = 0;
    int temp_var_b = 0;
    // int temp_var_a = 0;
    // int temp_var_b = 0;
    DEBUG_TEST(DB_I, "plan_virtual_init %lld...", in_info->sch_info.cu_id);

    if (in_info->sch_info.cu_id <= 0 ||
        in_info->sch_info.available_time.tr_start_time < 1000 ||
        in_info->sch_info.available_time.tr_end_time < 1000 ||
        strlen(in_info->sch_info.cu_zone) < 2)
    {
        stat = fail_r;
        DEBUG_TEST(DB_W, "Err : The time or area size is incorrect %lld  %lld  %lld %s", in_info->sch_info.cu_id,
                   in_info->sch_info.available_time.tr_start_time,
                   in_info->sch_info.available_time.tr_end_time, in_info->sch_info.cu_zone);
    }
    if (stat != fail_r)
    {

        { /** Next : null or 1&2 (Sack) or 2|10 (Pump Mode)*/
            if (strstr(in_info->sch_info.cu_sack, M_INV7) != NULL)
            {
                // in_info->exfun->fun_soak_use = true;
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_sack,
                                                          M_STRM_STR_3(M_HD, "&", M_HD),
                                                          &in_info->exfun.fn_soak_water,
                                                          &in_info->exfun.fn_soak_cycle);
                // printf("{%s}%d - %d", in_info->sch_info.cu_sack, in_info->exfun.fn_soak_water, in_info->exfun.fn_soak_cycle);
                in_info->exfun.fn_run_type = M_M_FUNCATION_SACK;
            }
            else if (strstr(in_info->sch_info.cu_sack, "|") != NULL)
            {
                // in_info->exfun.fun_soak_use = false;
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_sack,
                                                          "%d|%d",
                                                          &temp,
                                                          &in_info->exfun.fn_solenoid_mode_time);
                in_info->exfun.fn_run_type = M_M_FUNCATION_PUMP;
            }
            else if (strstr(in_info->sch_info.cu_sack, "C1") != NULL)
            {
                in_info->exfun.fn_run_type = M_M_FUNCATION_CONFIG_A;
                // m_callable_channel_manage_get_channel_param(in_info->sch_info.cu_id,
                //                                             M_M_FUNCATION_CONFIG_A, &in_info->exfun.fn_soak_water, &in_info->exfun.fn_soak_cycle);
                DEBUG_TEST(DB_W, "Using configuration A ");
            }
            else if (strstr(in_info->sch_info.cu_sack, "C2") != NULL)
            {
                in_info->exfun.fn_run_type = M_M_FUNCATION_CONFIG_B;
                // m_callable_channel_manage_get_channel_param(in_info->sch_info.cu_id,
                //                                             M_M_FUNCATION_CONFIG_B, &in_info->exfun.fn_soak_water, &in_info->exfun.fn_soak_cycle);
                DEBUG_TEST(DB_W, "Using configuration B ");
            }
            else
            {
                // in_info->exfun.fun_soak_use = false;
                DEBUG_E("sch_info.cu_sack Err --> (%s)", in_info->sch_info.cu_sack);
                in_info->exfun.fn_run_type = M_M_FUNCATION_NONE;
            }
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_src_execute_time_str, "%d", &temp_var_a);
            in_info->execu_type = temp_var_a;

            // if (in_info->execu_type != SCHEDULE_RUN_TYPE_SUNSET_PLUS && in_info->execu_type != SCHEDULE_RUN_TYPE_SUNRISE_PLUS)
            //     in_info->sch_info.cu_src_execute_time_str[3] = ' ';
        }
        /** Next : 100 08:00*/
        temp_var_a = 0;
        if (in_info->execu_type != SCHEDULE_RUN_TYPE_SUNRISE_PLUS && in_info->execu_type != SCHEDULE_RUN_TYPE_SUNSET_PLUS)
        {
            if (in_info->sch_info.cu_src_execute_time_str[0] >= 'A' && in_info->sch_info.cu_src_execute_time_str[0] <= 'Z')
            {
                in_info->sch_info.cu_more_time = true;
                m_callable_install_tools_start_time_encode_into_array(in_info->sch_info.cu_src_execute_time_str , in_info->sch_info.cu_more_time_list);
               
            }
            else
            {
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_src_execute_time_str, "%d %[^,]",
                                                          &temp_var_a,
                                                          temp_str_1);
            }
            DEBUG_TEST(DB_I, "temp_str_1 : %s\n", temp_str_1);
        }
        temp_var_a = 0;

        if (in_info->execu_type == SCHEDULE_RUN_TYPE_AT || in_info->execu_type == SCHEDULE_RUN_TYPE_BEFROE)
        {
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_src_execute_time_str, "%d:%d",
                                                      &temp_var_a,
                                                      &temp_var_b);
        }

        /** Next : Zone info */
        // 解析及区域参数，运行时间总和 区域单数
        m_static_tools_instance_obi_explain_zone(in_info,
                                                 ZONE_FORMAN_SEPA,
                                                 ZONE_FORMAN_END,
                                                 in_info->sch_info.cu_zone);
        // 计算总花费时间
        if (in_info->exfun.fn_run_type != M_M_FUNCATION_NONE)
            // 计算总花费时间

            // m_static_instance_tools_calcute_instance_time_sum_and_current_staus(in_info->cmg_channel, in_info->exfun.fn_max_channel, in_info->WSTime, &in_info->exfun.fn_max_coust_time, in_time_s);
            m_static_instance_sack_algorithm_queue_gener(in_info->cmg_channel, in_info->exfun.fn_max_channel,
                                                         in_info->WSTime, NULL,
                                                         &in_info->exfun.fn_max_coust_time,
                                                         false, in_time_s);
        else
            in_info->exfun.fn_max_coust_time = in_info->exfun.fn_max_should_runtime;
        /** Next : Determine Start Time  [sch_info.cu_execute_type] 100 - 101 103 104 */
        if (
            in_info->sch_info.frequency_type == SCHEDULE_RUN_TYPE_SUNRISE ||
            in_info->sch_info.frequency_type == SCHEDULE_RUN_TYPE_SUNSET)
        {
            /* 根据计划开始时间，计算当天的日出和日落时间*/
            // m_suninfo_specity_date(&certain_day_sunset,&certain_day_sunrise , in_info->sch_info.cu_execute_time.tr_start_time);
            // in_info->WSTime = in_info->running.rb_WSTime_clone = 47000;
        }

        /** interval hour and (sunset / sunrise)  ==  AT + interval hour */
        // if (in_info->sch_info.frequency_type == SCHEDULE_RUN_TYPE_SUNSET &&
        //     in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
        //     in_info->sch_info.frequency_type = SCHEDULE_RUN_TYPE_AT;

        // if (in_info->sch_info.frequency_type == SCHEDULE_RUN_TYPE_SUNRISE &&
        //     in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
        //     in_info->execu_type = SCHEDULE_RUN_TYPE_BEFROE;
        if (in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR || in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERTAL_DAY)
        {
            // M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_week_list, "%hhd", &in_info->sch_info.cu_interval_hour_or_day);
        }
        /** Next : Determine Start Time 2 */
        DEBUG_TEST(DB_W, "execu_type %d", in_info->execu_type);
        DEBUG_TEST(DB_W, "frequency_type  %d", in_info->sch_info.frequency_type);
        // DEBUG_TEST(DB_W,"执行类型%d", in_info->sch_info.cu_enable);
    }

    return stat;
}
