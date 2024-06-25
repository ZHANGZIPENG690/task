/**
 * @file channel_manage.c
 * @author Mao (you@domain.com)
 * @brief  区域管理，存储区域的有效时间 区域的多个浇水浸泡时间
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../channel_manage.h"
#include "../../tool/data_parse/data_parse.h"
#include "../../../application/instance_manage/tools/instance_algorithm.h"

struct channel_manage_conf cmc[32] = {0};

int device_max_channel = 30;
/** 离线浇水调整汇总 */
int off_running_change[13] = {100};
/** 离线月份 百分比 */
const char *M_FLASH_FLAG_SMART_CHANNEL_OFFLIN_CHANGE = "off_c";

// 存储格式     EN/DIS,A|A,B|B,StartTime,EndTime

stat_m m_callable_channel_manage_init(int device_type)
{
    stat_m stat = fail_r;
    // flash加载
    // char arr_config[40] = {0};
    char arr_config_vue[100] = {0};
    stat = m_static_channel_manage_config_init(cmc, device_type);
    // memset(off_running_change, 100, 13);
    if (m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_FLAG_SMART_CHANNEL_OFFLIN_CHANGE, arr_config_vue) == succ_r)
    {
        DEBUG_TEST(DB_W, "off_c: %s", arr_config_vue);
        for (size_t i = 1; i < 13; i++)
        {
            m_callable_time_str_to_num_decrypt_decode(arr_config_vue, &off_running_change[i], 2, strlen(arr_config_vue));
        }
    }
    else
        for (size_t i = 1; i < 13; i++)
            off_running_change[i] = 100;

    return stat;
}
/**
 * @brief 设置一个区域开启或者关闭
 * @todo Unit Input ...
 *
 * @param channel
 * @return stat_m
 */
stat_m m_callable_channel_manage_state_set(bool enable, uint8_t channel, uint64_t start_time, uint64_t end_time)
{
    stat_m stat = fail_r;
    if (enable)
    {
        if (channel > 0 && channel <= device_max_channel)
        {
            stat = succ_r;
            cmc[channel].channel_state = M_STATIC_CHANNEL_STATE_ENABLE;
            cmc[channel].disable_end_time = 0;
            cmc[channel].channel_id = channel;
            cmc[channel].disable_start_time = 0;
            // m_static_channel_manage_config_change_config_del(channel);
            m_static_channel_manage_config_change_config(&cmc[channel]);
            m_callable_device_attribute_config_change_notify_event_callable(
                M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_CHANNEL_OPEN,
                (void *)&channel,
                m_callable_timer_manage_get_device_time());
        }
        else
            DEBUG_TEST(DB_I, "enable param err!");
    }
    else
    {
        if (channel > 0 && channel <= device_max_channel && end_time > start_time && start_time > 9999)
        {
            stat = succ_r;
            cmc[channel].channel_state = M_STATIC_CHANNEL_STATE_DISABLE;
            cmc[channel].disable_start_time = start_time;
            cmc[channel].channel_id = channel;
            cmc[channel].disable_end_time = end_time;
            m_static_channel_manage_config_change_config(&cmc[channel]);
            m_callable_device_attribute_config_change_notify_event_callable(
                M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_CHANNEL_CLOSE,
                (void *)&channel,
                m_callable_timer_manage_get_device_time());
        }
        else
            DEBUG_TEST(DB_I, "dis param err!");
    }
    return stat;
}
/**
 * @brief 启用一个区域
 * @todo Unit Input ...
 *
 * @param channel
 * @return stat_m
 */
stat_m m_callable_channel_manage_enable(uint8_t channel)
{
    stat_m stat = fail_r;
    if (channel > 0 && channel <= device_max_channel)
    {
        stat = succ_r;
        cmc[channel].channel_state = M_STATIC_CHANNEL_STATE_ENABLE;
        cmc[channel].disable_end_time = 0;
        cmc[channel].channel_id = channel;
        cmc[channel].disable_start_time = 0;
        m_static_channel_manage_config_change_config(&cmc[channel]);
        m_callable_device_attribute_config_change_notify_event_callable(
            M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_CHANNEL_OPEN,
            (void *)&channel,
            m_callable_timer_manage_get_device_time());
    }
    return stat;
}

/**
 * @brief 设置一个区域的运行配置 隐藏的时间配置信息
 *
 * @param channel_id 区域id
 * @param cconfig  配置A 还是B
 * @param src_str 参数
 * @return stat_m
 */
// stat_m m_callable_channel_manage_set_channel_sack_param(uint8_t channel, enum channel_param_config cconfig, char *src_str)
// {
//     stat_m stat = fail_r;
//     if (cconfig == M_STATIC_CHANNEL_CONFIG_A)
//     {
//         m_callable_data_parse_foramt_param("%d*%d", &cmc[channel ].channel_config_a[0], &cmc[channel ].channel_config_a[1], src_str);
//     }
//     else
//     {
//         m_callable_data_parse_foramt_param("%d*%d", &cmc[channel ].channel_config_b[0], &cmc[channel ].channel_config_b[1], src_str);
//     }
//     if ((channel > 0 && channel <= device_max_channel && (cmc[channel ].channel_config_b[0] > 0 && cmc[channel ].channel_config_b[1] > 0)) ||
//         (cmc[channel ].channel_config_a[0] > 0 && cmc[channel ].channel_config_a[1] > 0))
//     {
//         stat = succ_r;
//         cmc[channel ].channel_id = channel;
//         m_static_channel_manage_config_change_config(&cmc[channel ]);
//     }
//     return stat;
// }
stat_m m_callable_channel_manage_set_channel_sack_param(uint8_t channel, enum channel_param_config cconfig, uint32_t config_water, uint32_t config_sack, int in_enable)
{
    stat_m stat = fail_r;
    // if (channel > 0 && channel < 32)
    // {
    //     if (cconfig == M_STATIC_CHANNEL_CONFIG_A)
    //     {
    //         cmc[channel].channel_id = channel;
    //         cmc[channel].channel_config_a[0] = config_water;
    //         cmc[channel].channel_config_a[1] = config_sack;
    //         cmc[channel].channel_config_a[2] = in_enable;
    //     }
    //     else
    //     {
    //         cmc[channel].channel_id = channel;
    //         cmc[channel].channel_config_b[0] = config_water;
    //         cmc[channel].channel_config_b[1] = config_sack;
    //         cmc[channel].channel_config_b[2] = in_enable;
    //     }
    // }
    return stat;
}
/**
 * @brief 配置区域信息     格式 ：10,10*20,5*60,1]
 *
 * @param in_channel 区域
 * @param src_str 格式 ：10,10*20,5*60,1]
 *            状态： 0:关闭 1： 使用配置A（手动设置） 2：使用智能计划
 *
 * @return stat_m
 */
stat_m m_callable_channel_manage_congfig_sack_param(int in_channel, char *src_str)
{
    stat_m stat = fail_r;
    // 配置A
    char arr_config_a[40] = {0};
    // 配置B
    char arr_config_b[40] = {0};

    m_callable_data_parse_foramt_str(arr_config_a, src_str);
    m_callable_data_parse_foramt_str(arr_config_b, src_str);
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(src_str, "%*[ ]%d", &cmc[in_channel].channel_config_choose);
    cmc[in_channel].channel_id = in_channel;
    DEBUG_TEST(DB_W, "Chann: %d  Congfig A: %s  Congfig B: %s Enable: %d", in_channel, arr_config_a, arr_config_b, cmc[in_channel].channel_config_choose);
    if (strstr(arr_config_a, "null") == NULL)
    {
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(arr_config_a,
                                                  "%d&%d",
                                                  &cmc[in_channel].channel_config_a[0], &cmc[in_channel].channel_config_a[1]);
        // m_callable_channel_manage_set_channel_sack_param(in_channel, M_STATIC_CHANNEL_CONFIG_A, b, c, arr_config_enable);
    }
    if (strstr(arr_config_b, "null") == NULL)
    {
        if (strstr(arr_config_b, "##") != NULL)
        {
            cmc[in_channel].channel_config_b[M_SMART_SACK_AR] = 0;
            cmc[in_channel].channel_config_b[M_SMART_SACK_IR] = 0;
            cmc[in_channel].channel_config_b[M_SMART_SACK_ASA] = 0;
        }
        else
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(arr_config_b,
                                                      "%f#%f#%f#%f",
                                                      &cmc[in_channel].channel_config_b[M_SMART_SACK_AR],
                                                      &cmc[in_channel].channel_config_b[M_SMART_SACK_IR],
                                                      &cmc[in_channel].channel_config_b[M_SMART_SACK_ASA],
                                                      &cmc[in_channel].flow_speed_f);

        // m_callable_channel_manage_set_channel_sack_param(in_channel, M_STATIC_CHANNEL_CONFIG_B, b, c, arr_config_enable);
    }
    // else if (cmc[in_channel].channel_config_b[M_SMART_SACK_AR] == 0 &&
    // cmc[in_channel].channel_config_b[M_SMART_SACK_IR] == 0 && cmc[in_channel].channel_config_b[M_SMART_SACK_ASA] == 0)
    // {
    //     cmc[in_channel].channel_config_b[M_SMART_SACK_AR] = DEFAULT_AR;
    //     cmc[in_channel].channel_config_b[M_SMART_SACK_IR] = DEFAULT_IR;
    //     cmc[in_channel].channel_config_b[M_SMART_SACK_ASA] = DEFAULT_ASA;
    // }

    m_static_channel_manage_config_change_config(&cmc[in_channel]);
    // m_callable_data_parse_foramt_param("%d*%d", &cmc[channel ].channel_config_a[0], &cmc[channel ].channel_config_a[1], src_str);

    return stat;
}

/**
 * @brief 判断区域在当前时间是否应该有效（启用）
 *
 * @param channel 区域id
 * @param current_time 当前时间
 * @return stat_m
 */
stat_m m_callable_channel_manage_current_is_effective(uint8_t channel, uint64_t current_time)
{
    stat_m stat = fail_r;
    if (current_time >= cmc[channel].disable_start_time && current_time <= cmc[channel].disable_end_time)
    {
        stat = succ_r;
    }
    return stat;
}
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

stat_m m_callable_channel_manage_get_channel_param(uint8_t channel_id,
                                                   enum channel_param_config cconfig, uint8_t *out_sack_type, uint32_t *running_time, uint32_t *sack_time,
                                                   float *out_ar, float *out_ir, float *out_asa)
{
    stat_m stat = fail_r;
    // cmc[channel_id ].channel_id = channel_id;
    DEBUG_TEST(DB_W, "A/B:  %d - -- Choose: %d", cconfig, cmc[channel_id].channel_config_choose);

    if (cconfig == M_STATIC_CHANNEL_CONFIG_A)
    {
        if (cmc[channel_id].channel_config_choose == 1)
        {
            *running_time = cmc[channel_id].channel_config_a[0];
            *sack_time = cmc[channel_id].channel_config_a[1];
            *out_sack_type = M_STATIC_CHANNEL_TYPE_SACK;
        }
        else if (cmc[channel_id].channel_config_choose == 2)
        {
            *running_time = 0;
            *sack_time = 0;
            *out_ar = cmc[channel_id].channel_config_b[M_SMART_SACK_AR];
            *out_ir = cmc[channel_id].channel_config_b[M_SMART_SACK_IR];
            *out_asa = cmc[channel_id].channel_config_b[M_SMART_SACK_ASA];
            *out_sack_type = M_STATIC_CHANNEL_TYPE_WATERSAVING;
            if (*out_ar == 0 || *out_ir == 0 || *out_asa == 0)
                *out_sack_type = M_STATIC_CHANNEL_TYPE_NORMAL;
        }
        else
        {
            *running_time = 0;
            *sack_time = 0;
            *out_sack_type = M_STATIC_CHANNEL_TYPE_NORMAL;
        }
        DEBUG_TEST(DB_I, "M_STATIC_CHANNEL_CONFIG_A   %hhd %hhd    ar: %.02f ir: %.02f asa: %.02f", *running_time, *sack_time, *out_ar, *out_ir, *out_asa);
        stat = succ_r;
    }
    else if (cconfig == M_STATIC_CHANNEL_CONFIG_B)
    {
        *running_time = 0;
        *sack_time = 0;
        *out_ar = cmc[channel_id].channel_config_b[M_SMART_SACK_AR];
        *out_ir = cmc[channel_id].channel_config_b[M_SMART_SACK_IR];
        *out_asa = cmc[channel_id].channel_config_b[M_SMART_SACK_ASA];
        *out_sack_type = M_STATIC_CHANNEL_TYPE_WATERSAVING;

        DEBUG_TEST(DB_I, "M_STATIC_CHANNEL_CONFIG_B   ar: %.02f ir: %.02f asa: %.02f", *out_ar, *out_ir, *out_asa);
        stat = succ_r;
    }

    return stat;
}

/**
 * @brief 离线百分比设置，针对于所有区域
 * @todo Unit Input ...
 *
 * @param param
 * @return stat_m
 */

stat_m m_callable_channel_manage_set_offline_running_percent(char *in_percent)
{
    stat_m stat = fail_r;
    // int
    DEBUG_TEST(DB_W, "Offline Percentage adjustment %s", in_percent);
    /** 直接存储   */
    m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_FLAG_SMART_CHANNEL_OFFLIN_CHANGE, in_percent);
    for (size_t i = 1; i < 13; i++)
    {
        m_callable_time_str_to_num_decrypt_decode(in_percent, &off_running_change[i], 2, strlen(in_percent));
        // DEBUG_TEST(DB_W,"Offline Percentage -->  %d", off_running_change[i]);
    }

    // off_running_change

    return stat;
}

/**
 * @brief 获取离线百分比参数
 * @todo Unit Input ...
 *
 * @param param
 * @return stat_m
 */

stat_m m_callable_channel_manage_get_offline_running_percent(int *percent)
{
    stat_m stat = fail_r;
    uint32_t month = 0;
    m_callable_timer_manage_get_ymd(m_callable_timer_manage_get_device_time(), NULL, &month, NULL);
    *percent = off_running_change[month];
    DEBUG_TEST(DB_I, "m_callable_channel_manage_get_offline_running_percent  %d", *percent);
    return stat;
}

/**
 * @brief 清除离线百分比参数
 * @todo Unit Input ...
 *
 * @param param
 * @return stat_m
 */

stat_m m_callable_channel_manage_clean_offline_running_percent(uint8_t channel_id)
{
    stat_m stat = fail_r;
    for (size_t i = 0; i < 16; i++)
    {
        // cmc[i].channel_percent = 0;
        stat = succ_r;
    }

    return stat;
}

void m_test_channel_manage_get_param(void)
{
    // for (int i = 0; i < 8; i++)
    // {
    //     DEBUG_TEST( DB_I,"id   %hhd", cmc[i].channel_id);
    //     DEBUG_TEST( DB_I,"enable   %hhd", cmc[i].channel_state);
    //     DEBUG_TEST( DB_I,"disable_start_time   %lld", cmc[i].disable_start_time);
    //     DEBUG_TEST( DB_I,"disable_end_time   %lld", cmc[i].disable_end_time);
    //     DEBUG_TEST( DB_I,"channel_config_a   %d - %d  En: %d", cmc[i].channel_config_a[0], cmc[i].channel_config_a[1], cmc[i].channel_config_a[2]);
    //     DEBUG_TEST( DB_I,"channel_config_b   %d - %d  En: %d", cmc[i].channel_config_b[0], cmc[i].channel_config_b[1], cmc[i].channel_config_b[2]);
    //     DEBUG_TEST( DB_I,"-----------------------");
    // }
}
// void main()
// {
//     m_callable_channel_manage_init();
// }

/**
 * @brief 设置区域流速   服务器改流速
 * @param channel
 * @param flow_speed
 * @return stat_m
 */
stat_m m_callable_channel_flow_speed_set(uint8_t channel, float flow_speed)
{
    stat_m stat = fail_r;
    if (channel > 0 && channel <= 33 && flow_speed != 0)
    {
        cmc[channel].channel_id = channel;
        cmc[channel].flow_speed_f = flow_speed / 60.0;
        m_static_channel_manage_config_change_config(&cmc[channel]);
        DEBUG_TEST(DB_I, "该区域流速设置完成=====》区域id : %d, 流速 :%.03f ", cmc[channel].channel_id, cmc[channel].flow_speed_f);
        stat = succ_r;
    }

    return stat;
}

/**
 * @brief 获取该区域流速
 * @param channel
 * @param flow_speed
 * @return stat_m
 */
stat_m m_callable_channel_flow_speed_get(uint8_t channel, float *flow_speed)
{
    *flow_speed = cmc[channel].flow_speed_f;
    return succ_r;
}
/**
 * @brief 获取多个区域在运行时的 智能配置参数
 *
 * @param in_channel_info Zone Example ： 1#2#5#6#9
 * @param out_ar_info 1#60&60 # 2#1.2&3.2&1.1
 * @return stat_m
 */
stat_m m_callable_channel_smart_config_a_b_get(char *conff_a_b, char *in_channel_info, char *out_ar_info)
{
    stat_m stat = fail_r;
    char tmp_ttr[40] = {0};
    int channel = 0;
    memset(out_ar_info, 0, strlen(out_ar_info));

    if (strlen(in_channel_info) > 0)
    {
        while (in_channel_info != NULL && *in_channel_info != '\0' && *in_channel_info != ' ')
        {
            if (in_channel_info != '#')
            {
                stat = fail_r;
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_channel_info, "%d", &channel);
                if (channel > 0)
                {
                    if (strstr(conff_a_b, "C1"))
                    {
                        if (cmc[channel].channel_config_choose == 1)
                        {
                            sprintf(tmp_ttr, "%d#%d&%d", channel, cmc[channel].channel_config_a[0], cmc[channel].channel_config_a[1]);
                        }
                        else if (cmc[channel].channel_config_choose == 2)
                        {
                            sprintf(tmp_ttr, "%d#%.02f&%.02f&%.02f", channel, cmc[channel].channel_config_b[M_SMART_SACK_AR],
                                    cmc[channel].channel_config_b[M_SMART_SACK_IR], cmc[channel].channel_config_b[M_SMART_SACK_ASA]);
                        }
                        else
                        {
                            sprintf(tmp_ttr, "%d#null", channel);
                        }
                        stat = succ_r;
                    }
                    else if (strstr(conff_a_b, "C2"))
                    {
                        sprintf(tmp_ttr, "%d#%.02f&%.02f&%.02f", channel, cmc[channel].channel_config_b[M_SMART_SACK_AR],
                                cmc[channel].channel_config_b[M_SMART_SACK_IR], cmc[channel].channel_config_b[M_SMART_SACK_ASA]);
                        stat = succ_r;
                    }
                    if (stat == succ_r)
                    {
                        strcat(out_ar_info, tmp_ttr);
                        if (*(in_channel_info + 1) != '\0' && *(in_channel_info + 1) != ' ')
                            strcat(out_ar_info, ";");
                    }
                }
            }

            // printf("    . %s   %d .\n" , in_channel_info , channel);
            in_channel_info++;
            in_channel_info++;
            if (channel >= 10)
                in_channel_info++;
            channel = 0;
        }
    }

    return stat;
}