

#include "../manage/instance_manage.h"

#include "../fsm/instance_fsm.h"
#include "instance_algorithm.h"
#include "../manage/new_lawn/new_lawn.h"

#include "../manage/instance_list.h"
#include "../manage/instance_stroage.h"

uint64_t t_var_u64_a = 0; /** 计划id*/

stat_m m_static_tools_instance_obj_explain(char *src_str, instance_doc_t *out_obj, bool *Edable, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    char temp_char_aa_c[250] = {0};
    int temp_frp = 0;

    /*/ id */
    m_callable_data_parse_format_vue("%lld", &out_obj->sch_info.cu_id, src_str);
    /*/ 智 */
    m_callable_data_parse_format_vue("%hhd", &out_obj->sch_info.cu_smart, src_str);
    /*/ 浸泡参数 */
    m_callable_data_parse_foramt_str(out_obj->sch_info.cu_sack, src_str);
    /*/ 开始时间 */
    m_callable_data_parse_format_vue("%lld", &out_obj->sch_info.available_time.tr_start_time, src_str);
    /*/ 结束时间 */
    m_callable_data_parse_format_vue("%lld", &out_obj->sch_info.available_time.tr_end_time, src_str);
    // /*/ 执行时间  100 10:24 */
    // m_callable_data_parse_foramt_str(out_obj->sch_info.cu_src_execute_time_str, src_str);
    // DEBUG_TEST( DB_I," - - %s", src_str);
    m_callable_data_parse_foramt_str(temp_char_aa_c, src_str);
    /*/ 区域 */
    // m_callable_data_parse_foramt_str(out_obj->sch_info.cu_zone, src_str);
    m_callable_data_parse_foramt_str(temp_char_aa_c, src_str);
    /*/ 频率类型 */
    m_callable_data_parse_format_vue("%d", &out_obj->sch_info.frequency_type, src_str);
    temp_frp = out_obj->sch_info.frequency_type;
    /*/ 星期 */
    m_callable_data_parse_foramt_str(out_obj->sch_info.cu_week_list, src_str);
    DEBUG_TEST(DB_W, "cu_week_list %s", out_obj->sch_info.cu_week_list);
    /*/ 启用 */
    if (strstr(src_str, "1") != NULL)
        *Edable = true;
    else
        *Edable = false;
    out_obj->sch_info.frequency_type = temp_frp;
    DEBUG_TEST(DB_W, "frequency_type %d", out_obj->sch_info.frequency_type);
    if (out_obj->sch_info.frequency_type > 1000)
        stat = fail_r;

    return stat;
}
stat_m m_static_expl_tools_zone_inform(char *in_str)
{
    stat_m stat = fail_r;
    char chr_guf[315] = {0};
    char chr_temp_a[40] = {0};
    char *i_src = in_str;
    int len = strlen(in_str);
    int zone_ss = 0, time_ss = 0;

    for (int i = 0; i < len; i++)
    {
        if (stat == fail_r)
        {
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_str, "%d#%d", &zone_ss, &time_ss);
            if (zone_ss > 0 && time_ss > 0)
            {
                sprintf(chr_temp_a, "%d#%d;", zone_ss, time_ss * 60);
                strcat(chr_guf, chr_temp_a);
            }
            zone_ss = 0;
            time_ss = 0;
            stat = succ_r;
        }
        if (*in_str == ';')
            stat = fail_r;
        in_str++;
    }
    memset(i_src, 0, len);
    strcpy(i_src, chr_guf);
    return stat;
}

stat_m m_static_tools_instance_obj_explain_A0012(char *src_str, instance_doc_t *out_obj, bool *Edable, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    char pp_temp[50] = {0};
    char vv_temp[200] = {0};
    char temp_char_aa_b[50] = {0};
    char temp_char_aa_c[250] = {0};
    uint64_t in_temp_time_a = 0, in_temp_time_b = 0;

    int a = 0, b = 0, c = 0, d = 0;
    // 3549 1 30&1 1676851200 2147483647 102 1#43;2#5515#61;16#61 7;1;2;3;4;5;6 0 104 7566

    DEBUG_TEST(DB_W, "m_static_tools_instance_obj_explain_A0012");
    DEBUG_TEST(DB_W, "[-|%s|-]", src_str);
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(src_str, "%lld %hhd %s %lld %lld %s %s %s %d %d", //  5 2 2021-09-28:08-00-00 2021-10-29:19-00-00 0 2#1,3#1 0 S 1,2,5 AT 08:02 NULL 0 0
                                                                                                /*/ id */
                                              &out_obj->sch_info.cu_id,
                                              /*/ 智 */
                                              &out_obj->sch_info.cu_smart,
                                              /*/ 浸泡参数 */
                                              out_obj->sch_info.cu_sack,
                                              /*/ 开始时间 */
                                              &out_obj->sch_info.available_time.tr_start_time,
                                              /*/ 结束时间 */
                                              &out_obj->sch_info.available_time.tr_end_time,
                                              //    /*/ 执行时间  100 10:24 */
                                              //    out_obj->sch_info.cu_src_execute_time_str,
                                              //    /*/ 区域 */
                                              //    out_obj->sch_info.cu_zone,
                                              temp_char_aa_b,
                                              temp_char_aa_c,
                                              /*/ 星期 */
                                              out_obj->sch_info.cu_week_list,
                                              &a,
                                              &b
                                              /*/ 频率类型 */
                                              //    &in_doc.sch_info.cu_smart,                     // or 1 hour
                                              //    &in_doc.exfun.fn_max_should_runtime
    );
    printf("%lld  \n ", out_obj->sch_info.cu_id);

    // m_callable_schedule_stroage_read_by_param(out_obj->sch_info.cu_id, &out_obj->sch_info.cu_src_execute_time_str, 5);
    // m_callable_schedule_stroage_read_by_param(out_obj->sch_info.cu_id, &out_obj->sch_info.cu_zone, 6);

    out_obj->sch_info.cu_src_execute_time_str = temp_char_aa_b;

    // 区域初始化
    m_static_expl_tools_zone_inform(temp_char_aa_c);
    out_obj->sch_info.cu_zone = temp_char_aa_c;
    // 频率类型
    out_obj->sch_info.frequency_type = b;
    // 去除
    if (strstr(temp_char_aa_b, "&") != NULL)
    {
        out_obj->sch_info.cu_src_execute_time_str[3] = ' ';
    }

    if (out_obj->sch_info.frequency_type == 120)
    {
        sprintf(pp_temp, "%s%lld", "nl", out_obj->sch_info.cu_id);
        m_callable_drive_flash_read(M_TYPE_Str, pp_temp, vv_temp);
        strcpy(temp_char_aa_b, vv_temp);
    }
    // 浸泡参数
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(out_obj->sch_info.cu_sack, "%d&%d", &c, &d);
    if (c && d)
        sprintf(out_obj->sch_info.cu_sack, "%d&%d", c * 60, d * 60);
    DEBUG_TEST(DB_W, "cu_week_list %s", out_obj->sch_info.cu_week_list);

    sprintf(pp_temp, "%s%lld", "PA", out_obj->sch_info.cu_id);

    if (m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_GET) != succ_r) // 有
        *Edable = true;
    else
        *Edable = false;

    // 2222,2,null,1702252800,4070995199,101 10:24,1#60,109,null,1]
    sprintf(src_str, "%lld,%hhd,%s,%lld,%lld,%s,%s,%hhd,%s,%d]",

            out_obj->sch_info.cu_id,
            /*/ 智 */
            out_obj->sch_info.cu_smart,
            /*/ 浸泡参数 */
            out_obj->sch_info.cu_sack,
            /*/ 开始时间 */
            out_obj->sch_info.available_time.tr_start_time,
            /*/ 结束时间 */
            out_obj->sch_info.available_time.tr_end_time,
            /*/ 执行时间  100 10:24 */
            out_obj->sch_info.cu_src_execute_time_str,
            /*/ 区域 */
            /*s*/ out_obj->sch_info.cu_zone,
            /*/ 频率类型 */
            /*s*/ out_obj->sch_info.frequency_type,
            /*/ 星期 */
            out_obj->sch_info.cu_week_list,
            *Edable);
    if (out_obj->sch_info.cu_id > 0 && out_obj->sch_info.cu_id < 99999)
    {
        m_callable_schedule_stroage_write_string(out_obj->sch_info.cu_id, src_str, *Edable);
        // PKA:5352)set :1711065600 - 1711151999 (1711065600:1711151999)
        //     sprintf(sKeyss, "%s:%lld", "PKA", skip_id);
        // sprintf(sVuess, "%lld:%lld", tsp, tep);
        sprintf(pp_temp, "%s:%lld", "PKA", out_obj->sch_info.cu_id);
        if (m_callable_drive_flash_read(M_TYPE_Str, pp_temp, vv_temp) == succ_r)
        {
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(vv_temp, "%lld:%lld", &in_temp_time_a, &in_temp_time_b);
            m_callable_schedule_skip_set(out_obj->sch_info.cu_id, in_temp_time_a, in_temp_time_b, 0);
        }

        // sprintf(vv_temp, "%lld:%lld", tsp, tep);
    }
    // m_callable_schedule_stroage_read_by_param(out_obj->sch_info.cu_id, &out_obj->sch_info.cu_src_execute_time_str, 5);
    // m_callable_schedule_stroage_read_by_param(out_obj->sch_info.cu_id, &out_obj->sch_info.cu_zone, 6);
    DEBUG_TEST(DB_W, "frequency_type %d", out_obj->sch_info.frequency_type);
    return stat;
}

char tpp_queue_char_arr[512] = {0};
void m_stat_instance_queue_code_encrypt(char *out_str, uint8_t a, uint32_t b, uint32_t c, uint32_t d)
{
    char tep[5] = {0};
    m_callable_time_num_to_str_encrypt_encode(tep, a, 1);
    strcat(out_str, tep);
    m_callable_time_num_to_str_encrypt_encode(tep, b, 3);
    strcat(out_str, tep);
    // m_callable_time_num_to_str_encrypt_encode(tep, c, 3);
    // strcat(out_str, tep);
    // m_callable_time_num_to_str_encrypt_encode(tep, d, 3);
    // strcat(out_str, tep);
}
/**
 * @brief 获取已经存在的运行队列
 *
 * @param inst_obj
 * @param out_str
 */
void m_static_instance_runnning_queue_get(char *out_str)
{
    strcpy(out_str, tpp_queue_char_arr);
}
instance_doc_t *wtd = NULL;
/**
 * @brief 把当前的实例以一定格式的字符串输出
 *
 * @param inst_obj 队列
 * @param out_str 输出字符
 */
void m_static_instance_runnning_queue_str_format_get(instance_obj *inst_obj, char *out_str)
{
    // char chr[20] = {0};
    stat_m stat = fail_r;

    if (inst_obj != NULL && inst_obj->sio_channel != NULL)
    {
        for (int i = 0; i < inst_obj->sio_max_channel; i++)
        {
            if (inst_obj->sio_channel[i].cd_cahnnel_id > 0 && inst_obj->sio_channel[i].cd_should_run_time > 0)
            {
                // memset(chr, 0, sizeof(chr));
                m_stat_instance_queue_code_encrypt(out_str, inst_obj->sio_channel[i].cd_cahnnel_id,
                                                   inst_obj->sio_channel[i].cd_should_run_time,
                                                   inst_obj->sio_channel[i].cd_sack_should_water_time,
                                                   inst_obj->sio_channel[i].cd_sack_should_sack_time);

                // strcat(out_str, chr);
            }
        }
    }
    enum base_state ste = M_BASE_STATUS_IDLE;
    int weather = 0;
    strcat(out_str, "*");

    if (inst_obj != NULL)
    {
        if (inst_obj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
        {
            if (m_static_instance_list_find_by_id(inst_obj->sio_running_id, &wtd) == succ_r)
                strcat(out_str, wtd->sch_info.cu_sack);
        }
        else if (inst_obj->sio_run_type == M_INSTANCE_RUN_TYPE_FAST_RUN)
        {
            if (inst_obj->sio_channel[0].cd_sack_should_sack_time > 0 && inst_obj->sio_channel[0].cd_sack_should_water_time > 0)
            {
                // char fask_sack_str[30] = {0};
                sprintf(tpp_queue_char_arr, "%d&%d", inst_obj->sio_channel[0].cd_sack_should_sack_time, inst_obj->sio_channel[0].cd_sack_should_water_time);
                strcat(out_str, tpp_queue_char_arr);
            }
            else
                strcat(out_str, "null");
        }
    }
    memset(tpp_queue_char_arr, 0, sizeof(tpp_queue_char_arr));
    strcpy(
        tpp_queue_char_arr,
        out_str);

    DEBUG_TEST(DB_I, "%s", out_str);
}

/**
 * @brief 快速解析一个实例 快速运行
 *
 * @param src_str 快速运行格式
 * @param out_obj 发明阔的实例对象
 * @param in_time_s 当前的时间
 * @return stat_m
 */
stat_m m_static_tools_instance_obj_quick_explain(char *src_str, instance_obj *out_obj, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    // stat_m m_callable_data_parse_foramt_quick_time(char *format, void *arg1, void *arg2, void *arg3, char *in_src);
    int a, b, c, mis = 0;
    m_run_time.zonenum = 0;
    while (m_callable_data_parse_foramt_quick_time("%d#%d#%d", &a, &b, &c, src_str) == succ_r)
    {
        DEBUG_TEST(DB_I, "%d  %d   %d", a, b, c);

        m_run_time.total_duration_current_area[a] = c;
        m_run_time.zonenum++;
        m_run_time.channel_number[m_run_time.zonenum] = a;
        m_run_time.puase_duration_current_area[a] = 0;

        out_obj->sio_channel[mis].cd_cahnnel_id = a;
        out_obj->sio_channel[mis].cd_should_run_time = c;
        out_obj->sio_channel[mis].cd_channel_type = M_STATIC_CHANNEL_TYPE_NORMAL;
        out_obj->sio_max_channel++;
        stat = succ_r;
        mis++;
        a = 0;
        b = 0;
        c = 0;
    }
    m_run_time.zonenum = mis;
    if (stat == succ_r)
    {
        stat = fail_r;
        for (size_t i = 0; i < 500; i++)
        {
            if (*src_str == ',')
            {
                stat = succ_r;
                *src_str = ' ';
                DEBUG_TEST(DB_I, "Find Sack  %s", src_str);
                break;
            }
            else if (*src_str == '\n')
                break;
            src_str++;
        }
    }
    if (stat == succ_r)
    {
        stat = fail_r;
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(src_str, "%d&%d", &a, &b);
        if (a > 0 && b > 0)
            for (size_t i = 0; i < mis; i++)
            {
                out_obj->sio_channel[i].cd_channel_type = M_STATIC_CHANNEL_TYPE_SACK;
                out_obj->sio_channel[i].cd_sack_should_water_time = a;
                out_obj->sio_channel[i].cd_sack_should_sack_time = b;
            }
    }
    if (stat == succ_r)
        DEBUG_TEST(DB_I, "Any Sack  %d & %d", a, b);

    return stat;
}

stat_m m_static_tools_instance_obi_explain_zone(instance_doc_t *in_obj, char *format, char zseparate, char *in_src)
{
    stat_m stat = fail_r;
    // int len = strlen(in_src);
    int zpc = 0;
    bool get_ed = true, is_cul_max = false;

    // for (int i = 0; i < len; i++)+
    memset(in_obj->cmg_channel, 0, sizeof(in_obj->cmg_channel));
    // printf("%d"  ,  sizeof(in_obj->cmg_channel));
    in_obj->exfun.fn_max_should_runtime = 0;
    DEBUG_TEST(DB_I, "Type: %d", in_obj->exfun.fn_run_type);
    while (true)
    {
#if CONFIG_NEW_CHANNEL_INFO

#else
        if (get_ed && in_src != NULL)
        {
            // printf("--> %s  %s (%d)\n", in_src, format, zpc);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, &in_obj->cmg_channel[zpc].cd_cahnnel_id, &in_obj->cmg_channel[zpc].cd_should_run_time);
            // printf("%hhd - %d\n" ,in_obj->cmg_channel[zpc].cd_cahnnel_id, in_obj->cmg_channel[zpc].cd_should_run_time) ;
            if (in_obj->cmg_channel[zpc].cd_cahnnel_id > 0 && in_obj->cmg_channel[zpc].cd_should_run_time > 0)
            {
                in_obj->cmg_channel[zpc].cd_reality_run_time = 0;
                // in_obj->cmg_channel[zpc].cd_should_run_time *= 60;
                in_obj->exfun.fn_max_should_runtime += in_obj->cmg_channel[zpc].cd_should_run_time;
                if (in_obj->exfun.fn_run_type == M_M_FUNCATION_PUMP)
                {
                    in_obj->cmg_channel[zpc].cd_channel_type = M_STATIC_CHANNEL_TYPE_PUMP;
                    in_obj->cmg_channel[zpc].cd_sack_should_sack_time = in_obj->exfun.fn_solenoid_mode_time;
                    in_obj->cmg_channel[zpc].cd_sack_should_water_time = in_obj->cmg_channel[zpc].cd_should_run_time;
                }
                else if (in_obj->exfun.fn_run_type == M_M_FUNCATION_SACK)
                // if (true)
                {
                    in_obj->cmg_channel[zpc].cd_channel_type = M_STATIC_CHANNEL_TYPE_SACK;
                    in_obj->cmg_channel[zpc].cd_sack_should_sack_time = in_obj->exfun.fn_soak_cycle;
                    in_obj->cmg_channel[zpc].cd_sack_should_water_time = in_obj->exfun.fn_soak_water;
                }
                else if (in_obj->exfun.fn_run_type == M_M_FUNCATION_CONFIG_A || in_obj->exfun.fn_run_type == M_M_FUNCATION_CONFIG_B)
                {
                    m_callable_channel_manage_get_channel_param(in_obj->cmg_channel[zpc].cd_cahnnel_id,
                                                                in_obj->exfun.fn_run_type,
                                                                &in_obj->cmg_channel[zpc].cd_channel_type,
                                                                &in_obj->cmg_channel[zpc].cd_sack_should_water_time,
                                                                &in_obj->cmg_channel[zpc].cd_sack_should_sack_time,
                                                                &in_obj->cmg_channel[zpc].cd_ar,
                                                                &in_obj->cmg_channel[zpc].cd_ir,
                                                                &in_obj->cmg_channel[zpc].cd_asa);
                    if (in_obj->cmg_channel[zpc].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
                        is_cul_max = true;
                    // in_obj->cmg_channel[zpc].cd_channel_type = M_STATIC_CHANNEL_TYPE_SACK;
                    // if (in_obj->cmg_channel[zpc].cd_sack_should_sack_time == 0 || in_obj->cmg_channel[zpc].cd_sack_should_water_time == 0)
                    // {
                    //     in_obj->cmg_channel[zpc].cd_channel_type = M_STATIC_CHANNEL_TYPE_NORMAL;
                    // }
                }
                else
                    in_obj->cmg_channel[zpc].cd_channel_type = M_STATIC_CHANNEL_TYPE_NORMAL;
                zpc += 1;
                get_ed = false;
                stat = succ_r;
            }
            // else
            //     break;
        }
        in_src++;
        if (*in_src == zseparate)
            get_ed = true, in_src++;

        if (*in_src == '\0')
            break;
#endif
    }
    if (zpc >= 1 && is_cul_max)
    {
        m_static_instance_sack_algorithm_pro_max(in_obj->cmg_channel, zpc, 0);
    }
    for (size_t i = 0; i < zpc; i++)
    {
        DEBUG_TEST(DB_I, "(Type: %d)..shouldRunTime: %d ... (%d-%d) [%.02f - %.02f - %.02f]", in_obj->cmg_channel[i].cd_channel_type,
                   in_obj->cmg_channel[i].cd_should_run_time,
                   in_obj->cmg_channel[i].cd_sack_should_water_time,
                   in_obj->cmg_channel[i].cd_sack_should_sack_time,
                   in_obj->cmg_channel[i].cd_ar,
                   in_obj->cmg_channel[i].cd_ir,
                   in_obj->cmg_channel[i].cd_asa);
    }
    in_obj->exfun.fn_max_channel = zpc;
    // M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, arg1, arg2);
    DEBUG_TEST(DB_I, "max_should_runtime... %d", in_obj->exfun.fn_max_should_runtime);
    return stat;
}

/** 经纬度信息 */
double lon, lat;
/** 暂时全部的日月年  和临时其他日期的日月年 */
uint32_t year = 0, month = 0, day = 0, week = 0, tp_year = 0, tp_month = 0, tp_day = 0;
/** tz */
int tz = 0;
/** 某天的日出日落  */
uint32_t sunrise = 0, sunset = 0, tp_sunrise = 0, tp_sunset = 0;
/** 临时变量 A */
int temp_var_a = 0;
/** 临时变量 B */
int temp_var_b = 0;
/** 临时变量 s */
uint64_t temp_var_u64_s = 0;
/** 临时 char 变量 A */
char temp_char_a[30] = {0};
/** 临时 char 变量 B */
char temp_char_c[10] = {0};
/** 当天的零点设备时间 */
uint64_t this_day_zero_time_timep = 0;

// 算出的是时间错
// 还要看下是不是在上一次的运行时间范围
stat_m m_static_tools_instance_obj_refresh_start_time(instance_doc_t *in_info, uint64_t *out_pre_time, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    // 计算时间和刷新的时候 顺带看下是不是在上一次的运行时间 也就是需不需要恢复计划
    bool is_resp_hhmmss = false, is_resp_day = false;

    if (m_callable_timer_manage_get_device_time() > in_info->sch_info.available_time.tr_end_time)
    {
        return stat;
    }
    memset(temp_char_a, 0, sizeof(temp_char_a));
    memset(temp_char_c, 0, sizeof(temp_char_c));
    /** 地址 */
    m_callable_device_attribute_get_local(&lon, &lat);

    if ((m_callable_timer_manage_get_device_zero_time() != this_day_zero_time_timep) || this_day_zero_time_timep == 0 || this_day_zero_time_timep < in_info->sch_info.available_time.tr_start_time || this_day_zero_time_timep > in_info->sch_info.available_time.tr_end_time)
    {
        /** 时区 */
        m_callable_device_attribute_get_device_timezone(&tz);
        // 当天的零点时间戳 (设备)
        this_day_zero_time_timep = m_callable_timer_manage_get_device_zero_time();

        // 如果是事件不再范围 比小
        // DEBUG_TEST( DB_I,"a a a a a (%lld    %lld )", this_day_zero_time_timep, in_info->sch_info.available_time.tr_start_time);
        if (this_day_zero_time_timep < in_info->sch_info.available_time.tr_start_time || this_day_zero_time_timep > in_info->sch_info.available_time.tr_end_time)
        {
            this_day_zero_time_timep = in_info->sch_info.available_time.tr_start_time -
                                       (in_info->sch_info.available_time.tr_start_time % NUM_OF_DAY_SEC_SUM);
            m_callable_timer_manage_get_ymd(in_info->sch_info.available_time.tr_start_time, &year, &month, &day);
            /** week */
            week = m_callable_timer_manage_get_week(in_info->sch_info.available_time.tr_start_time);
            DEBUG_TEST(DB_W, "Consistent processing according to start time in the past or future .. %lld ", this_day_zero_time_timep);
        }
        // 如果是事件不再范围 比大
        // else if (this_day_zero_time_timep > in_info->sch_info.available_time.tr_end_time)
        // {
        //     /** 年月日 */
        //     m_callable_timer_manage_get_ymd(m_callable_timer_manage_get_device_time(), &year, &month, &day);
        // }
        else
        // 这个正常
        {
            // 当天的零点时间戳 (设备)
            this_day_zero_time_timep = m_callable_timer_manage_get_device_zero_time();
            /** week */
            week = m_callable_timer_manage_get_week(m_callable_timer_manage_get_device_time());
            /** 年月日 */
            m_callable_timer_manage_get_ymd(m_callable_timer_manage_get_device_time(), &year, &month, &day);
        }
        /** 日出日落 */
        m_callable_suninfo_get_sunset_and_rise(lon, lat, tz, year, month, day, &sunset, &sunrise);
        // 计算 当前时间和计划相差太大的时候
        if ((in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR && in_info->sch_info.available_time.tr_start_time + 10 < this_day_zero_time_timep) ||
            (in_info->sch_info.available_time.tr_start_time > this_day_zero_time_timep + NUM_OF_DAY_SEC_SUM))
        {
            DEBUG_TEST(DB_W, "The time difference is too large -- ");
            m_callable_timer_manage_get_ymd(in_info->sch_info.available_time.tr_start_time, &tp_year, &tp_month, &tp_day);
            m_callable_suninfo_get_sunset_and_rise(lon, lat, tz, tp_year, tp_month, tp_day, &tp_sunset, &tp_sunrise);

            DEBUG_TEST(DB_I, "%d-%d-%d - Today Sunrise: %d:%d:00 Today Sunset: %d:%d:00", tp_year, tp_month, tp_day, tp_sunrise / 3600, (tp_sunrise % 3600) / 60, tp_sunset / 3600, (tp_sunset % 3600) / 60);
        }
        else
        {
            tp_year = year;
            tp_month = month;
            tp_day = day;
            tp_sunrise = sunrise;
            tp_sunset = sunset;
        }
    }
    else
        DEBUG_TEST(DB_W, "Time already exists -- ");

    /** */
    DEBUG_TEST(DB_I, "Year-Month-Day %d-%d-%d", year, month, day);
    DEBUG_TEST(DB_I, "Today Week %d", week);
    DEBUG_TEST(DB_I, "Today %d Day ", day);
    DEBUG_TEST(DB_I, "Today Zero Time Stamp %lld", this_day_zero_time_timep);
    DEBUG_TEST(DB_I, "Local lon: %.04f lat: %.04f  tz %d", lon, lat, tz);
    DEBUG_TEST(DB_I, "Today Sunrise: %d Today Sunset: %d", sunrise, sunset);
    DEBUG_TEST(DB_I, "Today Sunrise: %d:%d:00 Today Sunset: %d:%d:00", sunrise / 3600, (sunrise % 3600) / 60, sunset / 3600, (sunset % 3600) / 60);
    /** */

    in_info->exfun.fn_ahead_one_day_exe = false;

    DEBUG_TEST(DB_I, " -- - %d", in_info->execu_type);
    switch (in_info->execu_type)
    {
    case SCHEDULE_RUN_TYPE_AT:
    case SCHEDULE_RUN_TYPE_BEFROE:
        DEBUG_TEST(DB_I, "%s", in_info->sch_info.cu_src_execute_time_str);
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_src_execute_time_str, "%s %d:%d",
                                                  temp_char_c,
                                                  &temp_var_a,
                                                  &temp_var_b);
        // if (in_info->execu_type == SCHEDULE_RUN_TYPE_BEFROE)

        if (in_info->execu_type == SCHEDULE_RUN_TYPE_AT)
        {
            in_info->WSTime = temp_var_a * 60 * 60 + temp_var_b * 60;
        }
        else
        {
            if ((temp_var_a * 60 * 60 + temp_var_b * 60) < in_info->exfun.fn_max_coust_time)
                in_info->exfun.fn_ahead_one_day_exe = true;

            in_info->WSTime = ((((temp_var_a * 60 * 60 + temp_var_b * 60) + NUM_OF_DAY_SEC_SUM) - in_info->exfun.fn_max_coust_time) % NUM_OF_DAY_SEC_SUM);
        }
        break;
    // case SCHEDULE_RUN_TYPE_BEFROE:
    // in_info->WSTime = in_info->running.rb_WSTime_clone - in_info->sch_info.cu_run_time_sum;
    // in_info->running.rb_WSTime_clone = in_info->WSTime;
    // break;
    case SCHEDULE_RUN_TYPE_SUNSET:
        in_info->WSTime = sunset;
        // 如果 间隔小时和日出日落   那么  就等于 AT + 间隔小时
        if (in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
        {
            in_info->WSTime = tp_sunset;
            in_info->execu_type = SCHEDULE_RUN_TYPE_AT;
        }
        break;
    case SCHEDULE_RUN_TYPE_SUNRISE:

        if (sunrise < in_info->exfun.fn_max_coust_time)
            in_info->exfun.fn_ahead_one_day_exe = true;

        in_info->WSTime = ((sunrise + NUM_OF_DAY_SEC_SUM) - in_info->exfun.fn_max_coust_time) % NUM_OF_DAY_SEC_SUM;

        // 如果 间隔小时和日出日落   那么  就等于 AT + 间隔小时
        if (in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
        {
            in_info->WSTime = ((tp_sunrise + NUM_OF_DAY_SEC_SUM) - in_info->exfun.fn_max_coust_time) % NUM_OF_DAY_SEC_SUM;
            in_info->execu_type = SCHEDULE_RUN_TYPE_AT;
        }

        // in_info->WSTime = in_info->running.rb_WSTime_clone = _time.today_sunrise;
        break;
    case SCHEDULE_RUN_TYPE_SUNRISE_PLUS:
        if (in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
            temp_var_b = tp_sunrise,
            in_info->execu_type = SCHEDULE_RUN_TYPE_AT;
        else
            temp_var_b = tp_sunrise;

        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_src_execute_time_str, "%*[0-9]%*[A-Z]%d", &temp_var_a);
        DEBUG_TEST(DB_I, "%s --  %d", in_info->sch_info.cu_src_execute_time_str, temp_var_a);
        if (temp_var_a * 60 > temp_var_b)
            in_info->exfun.fn_ahead_one_day_exe = true;

        if (in_info->sch_info.cu_src_execute_time_str[3] == 'S') // 日出开始 时间之前或者之后
        {
            in_info->WSTime = ((temp_var_b + NUM_OF_DAY_SEC_SUM) + temp_var_a * 60);
        }
        else if (in_info->sch_info.cu_src_execute_time_str[3] == 'E') // 日出结束  时间之前或者之后
        {
            if (temp_var_a * 60 + in_info->exfun.fn_max_coust_time > temp_var_b)
                in_info->exfun.fn_ahead_one_day_exe = true;
            in_info->WSTime = ((temp_var_b + NUM_OF_DAY_SEC_SUM) + temp_var_a * 60 - in_info->exfun.fn_max_coust_time);
        }
        in_info->WSTime %= NUM_OF_DAY_SEC_SUM;
        DEBUG_TEST(DB_I, "SCHEDULE_RUN_TYPE_SUNRISE_PLUS :%lld", in_info->WSTime);
        break;
    case SCHEDULE_RUN_TYPE_SUNSET_PLUS:
        if (in_info->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
            temp_var_b = tp_sunset,
            in_info->execu_type = SCHEDULE_RUN_TYPE_AT;
        else
            temp_var_b = tp_sunset;
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_src_execute_time_str, "%*[0-9]%*[A-Z]%d", &temp_var_a);
        DEBUG_TEST(DB_I, "%s", in_info->sch_info.cu_src_execute_time_str);

        if (in_info->sch_info.cu_src_execute_time_str[3] == 'S')
        {
            in_info->WSTime = ((temp_var_b) + temp_var_a * 60);
        }
        else if (in_info->sch_info.cu_src_execute_time_str[3] == 'E')
        {
            if (temp_var_a * 60 + in_info->exfun.fn_max_coust_time > temp_var_b)
                in_info->exfun.fn_ahead_one_day_exe = true;
            in_info->WSTime = ((temp_var_b + NUM_OF_DAY_SEC_SUM) + temp_var_a * 60 - in_info->exfun.fn_max_coust_time);
        }
        in_info->WSTime %= NUM_OF_DAY_SEC_SUM;
        break;
    case SCHEDULE_RUN_RHYTHM_110:
    case SCHEDULE_RUN_RHYTHM_111:
    case SCHEDULE_RUN_RHYTHM_112:
    case SCHEDULE_RUN_RHYTHM_113:
    case SCHEDULE_RUN_RHYTHM_114:
    case SCHEDULE_RUN_RHYTHM_117:
    case SCHEDULE_RUN_RHYTHM_118:
    case SCHEDULE_RUN_RHYTHM_119:
        break;
    default:
        if (in_info->sch_info.cu_more_time)
        {
            DEBUG_TEST(DB_I, " -- -  -+ + ++  -- ");
        }
        if (in_info->sch_info.cu_more_time)
            m_static_instance_tools_time_calculate_near_time(in_info->sch_info.cu_more_time_list, out_pre_time,
                                                             &in_info->WSTime, in_time_s % NUM_OF_DAY_SEC_SUM);

        break;
    }
    DEBUG_TEST(DB_W, "on Cur: %d   WS: %lld  +  Coust: %lld", m_callable_timer_manage_get_hhmmss(), in_info->WSTime, in_info->exfun.fn_max_coust_time);
    if (in_info->WSTime + in_info->exfun.fn_max_coust_time % NUM_OF_DAY_SEC_SUM > m_callable_timer_manage_get_hhmmss() &&
        (m_callable_timer_manage_get_hhmmss() > in_info->WSTime || in_info->WSTime + in_info->exfun.fn_max_coust_time > NUM_OF_DAY_SEC_SUM))
    {
        DEBUG_TEST(DB_W, "In the previous time frame");
        is_resp_hhmmss = true;
    }

    // 上面的确定  具体时间
    DEBUG_TEST(DB_I, "Wish Today :  %lld ? frequency_type : %d", in_info->WSTime, in_info->sch_info.frequency_type);
    // 当前时间爱你和计划开始时间想拆毁的提阿美术
    int diff_day = 0;
    // 用来在计算中 缓存数据
    int temp_buf_int_a = 0;
    switch (in_info->sch_info.frequency_type)
    {
    case SCHEDULE_RUN_RHYTHM_SPEC_DAYS:
        temp_buf_int_a = week;
        if (temp_buf_int_a == 0)
            temp_buf_int_a = 7;
        DEBUG_TEST(DB_I,"week -> %d" , temp_buf_int_a);
        strcpy(temp_char_a, in_info->sch_info.cu_week_list);
        memset(in_info->sch_info.cu_week_list, 0, sizeof(in_info->sch_info.cu_week_list));
        m_callable_data_parse_foramt_week_c("%d", ';', in_info->sch_info.cu_week_list, temp_char_a);
        // 找到最接近的一个星期
        for (int find_week_this = 0; find_week_this < 8; find_week_this++)
        {
            // if (strstr((char)temp_buf_int_a + 48, in_info->sch_info.cu_week_list) != NULL)
            for (size_t i = 0; i < sizeof(in_info->sch_info.cu_week_list); i++)
                // （'week' + j ） == list[i]
                if (((((temp_buf_int_a + find_week_this) % 7) + 48 == in_info->sch_info.cu_week_list[i]) 
                ||((temp_buf_int_a + find_week_this == 7) && in_info->sch_info.cu_week_list[i] == '7')
                ))
                {
                    // 如果是当天的话 即得看时间是不是比现在的时间大
                    if (find_week_this == 0 && this_day_zero_time_timep + in_info->WSTime <= in_time_s)
                    {
                        is_resp_day = true;
                        continue;
                    }
                    DEBUG_TEST(DB_I, "week %d ，Execute after %d  days...", (temp_buf_int_a + find_week_this) % 7, find_week_this);
                    in_info->WSTime += (this_day_zero_time_timep + find_week_this * NUM_OF_DAY_SEC_SUM);
                    find_week_this = 80;
                    break;
                }
        }
        temp_buf_int_a = 0;
        break;
    case SCHEDULE_RUN_RHYTHM_ODD_DAY:
        DEBUG_TEST(DB_W, "ODD  ...");
        temp_buf_int_a = day;
        for (int find_odd_this_day = 0; find_odd_this_day < 3; find_odd_this_day++)
        {
            if (((!in_info->exfun.fn_ahead_one_day_exe && temp_buf_int_a % 2 != 0) || (in_info->exfun.fn_ahead_one_day_exe && temp_buf_int_a % 2 == 0)) &&
                temp_buf_int_a != 31)
            {
                if (temp_buf_int_a == day && this_day_zero_time_timep + find_odd_this_day * NUM_OF_DAY_SEC_SUM + in_info->WSTime + in_info->exfun.fn_max_coust_time > in_time_s)
                {
                    is_resp_day = true;
                }
                if (this_day_zero_time_timep + in_info->WSTime + find_odd_this_day * NUM_OF_DAY_SEC_SUM > in_time_s)
                {
                    in_info->WSTime += (this_day_zero_time_timep + find_odd_this_day * NUM_OF_DAY_SEC_SUM);
                    DEBUG_TEST(DB_I, "Execute after %d days ...", find_odd_this_day);
                    break;
                }
                else
                    temp_buf_int_a = m_callable_timer_manage_get_day(m_callable_timer_manage_get_device_time() +
                                                                     ((find_odd_this_day + 1) * NUM_OF_DAY_SEC_SUM));
            }
            else
                temp_buf_int_a = m_callable_timer_manage_get_day(m_callable_timer_manage_get_device_time() +
                                                                 ((find_odd_this_day + 1) * NUM_OF_DAY_SEC_SUM));
        }
        break;
    case SCHEDULE_RUN_RHYTHM_EVEN_DAY:
        DEBUG_TEST(DB_W, " EVENT ...");
        temp_buf_int_a = day;
        for (int find_event_this_day = 0; find_event_this_day < 3; find_event_this_day++)
        {
            if (((in_info->exfun.fn_ahead_one_day_exe && temp_buf_int_a % 2 != 0) || (!in_info->exfun.fn_ahead_one_day_exe && temp_buf_int_a % 2 == 0)) &&
                temp_buf_int_a != 31)
            {
                if (temp_buf_int_a == day && this_day_zero_time_timep + find_event_this_day * NUM_OF_DAY_SEC_SUM + in_info->WSTime + in_info->exfun.fn_max_coust_time > in_time_s)
                {
                    is_resp_day = true;
                }
                if (this_day_zero_time_timep + in_info->WSTime + find_event_this_day * NUM_OF_DAY_SEC_SUM > in_time_s)
                {
                    in_info->WSTime += (this_day_zero_time_timep + find_event_this_day * NUM_OF_DAY_SEC_SUM);
                    DEBUG_TEST(DB_I, "Execute after %d days ...", find_event_this_day);
                    break;
                }
                else
                    temp_buf_int_a = m_callable_timer_manage_get_day(m_callable_timer_manage_get_device_time() +
                                                                     ((find_event_this_day + 1) * NUM_OF_DAY_SEC_SUM));
            }
            else
                temp_buf_int_a = m_callable_timer_manage_get_day(m_callable_timer_manage_get_device_time() +
                                                                 ((find_event_this_day + 1) * NUM_OF_DAY_SEC_SUM));
        }

        break;
    case SCHEDULE_RUN_RHYTHM_INVERTAL_DAY:
        if (in_info->sch_info.cu_interval_hour_or_day == 0)
        {
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_week_list, "%hhd", &in_info->sch_info.cu_interval_hour_or_day);

            // if (in_info->WSTime < 864000)
            diff_day = m_callable_timer_manage_get_diff_hour_num(in_info->sch_info.available_time.tr_start_time,
                                                                 m_callable_timer_manage_get_device_time());
            DEBUG_TEST(DB_I, "day %d  WS %lld  . difference %d Hour %d Day", in_info->sch_info.cu_interval_hour_or_day, in_info->WSTime, diff_day, diff_day / 24);
            diff_day /= 24; // 到今天为止,算下 还差多少天到目标天

            if (diff_day % (in_info->sch_info.cu_interval_hour_or_day) > 0)
            {
                temp_buf_int_a = diff_day / in_info->sch_info.cu_interval_hour_or_day;
                temp_buf_int_a += 1;
            }
        }
        else
            temp_buf_int_a = in_info->sch_info.cu_interval_hour_or_day;

        in_info->WSTime += (this_day_zero_time_timep + (temp_buf_int_a)*NUM_OF_DAY_SEC_SUM);
        DEBUG_TEST(DB_W, "in_info->WSTime %lld += (this_day_zero_time_timep %lld + ( temp_buf_int_a ) %d * NUM_OF_DAY_SEC_SUM);",
                   in_info->WSTime, this_day_zero_time_timep, temp_buf_int_a * NUM_OF_DAY_SEC_SUM);
        if (in_info->WSTime < in_time_s)
            in_info->WSTime += ((in_info->sch_info.cu_interval_hour_or_day) * NUM_OF_DAY_SEC_SUM);

        if ((in_info->WSTime - NUM_OF_DAY_SEC_SUM * in_info->sch_info.cu_interval_hour_or_day) > this_day_zero_time_timep)
        {
            is_resp_day = true;
        }
        DEBUG_TEST(DB_I, "cala add  %d day Can be obtained(%hhd)  rt: %lld", temp_buf_int_a, in_info->sch_info.cu_interval_hour_or_day, in_info->WSTime);

        break;
    case SCHEDULE_RUN_RHYTHM_NEW_LAWN:
        // 加载并且解析
        // stat = m_static_new_lawn_object_load(in_info->sch_info.cu_id);
        // case SCHEDULE_RUN_RHYTHM_NEW_LAWN:
        // m_callable_new_lawn_object_init(in_info, in_time_s);
        stat = m_static_new_lawn_object_update(in_info->sch_info.cu_id,
                                               in_info->sch_info.cu_src_execute_time_str, &temp_var_u64_s,
                                               &in_info->WSTime, in_time_s % NUM_OF_DAY_SEC_SUM);
        DEBUG_TEST(DB_W, "new pre time %lld     stat : %d", temp_var_u64_s, stat);
        in_info->WSTime += (this_day_zero_time_timep);
        /** 记录上一次运行时间 */
        if (out_pre_time != NULL)
        {
            *out_pre_time = temp_var_u64_s;
        }
        // if (

        DEBUG_TEST(DB_W, " ( preTime: %lld + maxTime: %lld > 60 + in_time_s(hhmmss):%lld) ", temp_var_u64_s, in_info->exfun.fn_max_coust_time, in_time_s % NUM_OF_DAY_SEC_SUM);
        DEBUG_TEST(DB_W, "||  ( preTime: %lld + maxTime: %lld > 86400 && PreTime: %lld > in_time_s(hhmmss): %lld) ",
                   temp_var_u64_s % NUM_OF_DAY_SEC_SUM, in_info->exfun.fn_max_coust_time, (temp_var_u64_s + in_info->exfun.fn_max_coust_time) % NUM_OF_DAY_SEC_SUM, (in_time_s % NUM_OF_DAY_SEC_SUM));
        if (stat == succ_r &&
            /** 上一次时间 + 运行总时间 》 当前时间   并且   当前时间 》  上一次运行时间*/
            ((temp_var_u64_s + in_info->exfun.fn_max_coust_time > 60 + in_time_s % NUM_OF_DAY_SEC_SUM &&
              in_time_s % NUM_OF_DAY_SEC_SUM > temp_var_u64_s) ||
             /** 上一次 + 运行总时间 》 一天*/
             (temp_var_u64_s + in_info->exfun.fn_max_coust_time > NUM_OF_DAY_SEC_SUM &&
              /** 上一次 + 运行总时间 》 第二天的当前时间*/
              //   temp_var_u64_s <= (in_time_s % NUM_OF_DAY_SEC_SUM) + NUM_OF_DAY_SEC_SUM)))
              in_time_s % NUM_OF_DAY_SEC_SUM < (temp_var_u64_s + in_info->exfun.fn_max_coust_time) % NUM_OF_DAY_SEC_SUM)))
        {
            DEBUG_TEST(DB_W, "In the previous New Lawn time frame  ");
            is_resp_day = true,
            is_resp_hhmmss = true;
        }
        if (in_info->WSTime <= in_time_s && in_info->WSTime + NUM_OF_DAY_SEC_SUM > in_info->sch_info.available_time.tr_start_time &&
            in_info->WSTime + NUM_OF_DAY_SEC_SUM < in_info->sch_info.available_time.tr_end_time)
            in_info->WSTime += NUM_OF_DAY_SEC_SUM;
        break;
        // [3335,0,50&20,1674008936,1684108936,A0B0C0D0E0F0W0Q0K0L0P0O0I0U0Y0Z0M0N0V0,4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;4#30000;7#5400,120,1,1]
        // 添加时间节点

    case SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR:
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_week_list, "%hhd", &in_info->sch_info.cu_interval_hour_or_day);
        DEBUG_TEST(DB_I, "Invertal Hour %hhd", in_info->sch_info.cu_interval_hour_or_day);
        // 如果现在的时间 + 开始的时间  （就是计划在在当前的最近运行时间）  小于等于  现在的时间     那就酸
        if (in_info->sch_info.available_time.tr_start_time + in_info->WSTime <= in_time_s)
        // if (in_info->sch_info.available_time.tr_start_time + in_info->WSTime <= in_time_s)
        {
            m_static_instance_tools_time_calculate_the_interval_hour(in_time_s % NUM_OF_DAY_SEC_SUM, in_time_s,
                                                                     in_info->sch_info.cu_interval_hour_or_day,
                                                                     in_info->sch_info.available_time.tr_start_time, &in_info->WSTime);
        }
        in_info->WSTime += this_day_zero_time_timep;
        if ((in_time_s + in_info->sch_info.cu_interval_hour_or_day * 3600 < in_info->WSTime + in_info->exfun.fn_max_coust_time) ||
            ((in_info->WSTime + in_info->exfun.fn_max_coust_time > NUM_OF_DAY_SEC_SUM && in_info->WSTime <= in_time_s % NUM_OF_DAY_SEC_SUM + NUM_OF_DAY_SEC_SUM)))
        {
            DEBUG_TEST(DB_W, "In the previous Inveryal Hour time frame");
            is_resp_day = true, is_resp_hhmmss = true;
        }

        if (in_info->WSTime <= in_time_s && in_info->WSTime + NUM_OF_DAY_SEC_SUM > in_info->sch_info.available_time.tr_start_time &&
            in_info->WSTime + NUM_OF_DAY_SEC_SUM < in_info->sch_info.available_time.tr_end_time)
            in_info->WSTime += NUM_OF_DAY_SEC_SUM;
        if (out_pre_time != NULL)
            *out_pre_time = in_info->WSTime - in_info->sch_info.cu_interval_hour_or_day * 3600;
        break;
    case SCHEDULE_RUN_RHYTHM_EVERY_DAYS:
        in_info->WSTime += (this_day_zero_time_timep);
        DEBUG_TEST(DB_I, "Ws: %lld - Cur: %lld", in_info->WSTime, in_time_s);
        if (in_info->WSTime <= in_time_s ||
            (in_info->WSTime + in_info->exfun.fn_max_coust_time > NUM_OF_DAY_SEC_SUM && in_info->WSTime <= in_time_s % NUM_OF_DAY_SEC_SUM + NUM_OF_DAY_SEC_SUM))
        {
            DEBUG_TEST(DB_W, "In the previous Every Days time frame");
            is_resp_day = true,
            in_info->WSTime += NUM_OF_DAY_SEC_SUM;
        }
        break;

    default:
        break;
    }
    // 假设你得到了，他的应该运行时间，那么，他的开始时间你应该也是可以得到的
    if (in_info->WSTime > in_info->sch_info.available_time.tr_end_time)
    {
        in_info->WSTime = 0;
        stat = fail_r;
    }
    DEBUG_TEST(DB_W, "rb_WSTime_clone: %lld", in_info->WSTime);
    if (is_resp_day && is_resp_hhmmss)
    {
        DEBUG_TEST(DB_W, "Needs recovery ?? !! ??");
        stat = need_r;
    }
    return stat;
}
/**
 * @brief 时间改变的时候需要重新刷新时间
 *
 * @param in_data 改变对象
 * @param in_time_s 根据当前的时间
 * @return stat_m
 */
stat_m m_static_tools_instance_obj_will_refresh_time(void *in_data, uint64_t *noti_time, int *tag, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    int in_cause = 0;
    if (in_data != NULL)
    {
        instance_doc_t *in_wtd = (instance_doc_t *)in_data;
        if (in_time_s > in_wtd->sch_info.available_time.tr_start_time && in_time_s < in_wtd->sch_info.available_time.tr_end_time)
        {
            m_static_tools_instance_obj_refresh_start_time(in_wtd, NULL, in_time_s);
            if (in_wtd->WSTime < in_time_s)
            {
                DEBUG_TEST(DB_W, "Warring: It's still too small after the refresh is completed  !!  in_wtd->WSTime(%lld) < in_time_s(%lld) ", in_wtd->WSTime, in_time_s);
                m_static_time_link_notece_node_remove_by_id(in_wtd->sch_info.cu_id);
                in_cause = time_node_time_out_flush_fail;
                // 数据超出运行时间
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO, M_TYPE_Int, (void *)&in_cause,
                                                M_TYPE_U64, &in_wtd->sch_info.cu_id,
                                                M_TYPE_U64, &in_wtd->WSTime,
                                                M_TYPE_U64, &in_time_s,
                                                M_TYPE_NULL, NULL);
            }
            else
            {
                *noti_time = in_wtd->WSTime;
                DEBUG_TEST(DB_I, "change Time %lld", *noti_time);
                *tag = M_BASE_STATUS_IDLE;
            }
            // m_static_time_link_notece_node_change(in_wtd->sch_info.cu_id, in_wtd->WSTime, M_BASE_STATUS_IDLE);

            stat = succ_r;
        }
        else
        {
            DEBUG_TEST(DB_W, "Warring: Exceeding the refresh limit and running time range WS(%lld)   intime_s(%lld) >< (%lld)  >< %lld",
                       in_wtd->WSTime,
                       in_time_s, in_wtd->sch_info.available_time.tr_start_time, in_wtd->sch_info.available_time.tr_end_time);
            in_cause = time_node_time_out_schedule_range;
            m_static_time_link_notece_node_remove_by_id(in_wtd->sch_info.cu_id);
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO, M_TYPE_Int, (void *)&in_cause,
                                            M_TYPE_U64, &in_wtd->sch_info.cu_id,
                                            M_TYPE_U64, &in_wtd->WSTime,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL);
        }
    }

    return stat;
}