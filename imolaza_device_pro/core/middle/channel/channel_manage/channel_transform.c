/**
 * @file channel_transform.c
 * @author your name (you@domain.com)
 * @brief  区域参数解析和配置
 * @version 0.1
 * @date 2023-09-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../channel_manage.h"
#include "../../tool/data_parse/data_parse.h"
// #include "../../drive/soft_drive/flash.h"

#define INVE_CHANN "*"

// pz01 没用  pz02是区域1 改不了了   pz00是 区域16

// 现在版本的句柄
char mz_able[33][5] = {"Nul", "PZ01", "PZ02", "PZ03", "PZ04", "PZ05", "PZ06", "PZ07", "PZ08", "PZ09", "PZ10", "PZ11", "PZ12", "PZ13", "PZ14", "PZ15", "PZ16",
                       "PZ17", "PZ18", "PZ19", "PZ20", "PZ21", "PZ22", "PZ23", "PZ24", "PZ25", "PZ26", "PZ27", "PZ28", "PZ29", "PZ30", "PZ31"};
char new_mz_able[33][5] = {"Nul", "Zc01", "Zc02", "Zc03", "Zc04", "Zc05", "Zc06", "Zc07", "Zc08", "Zc09", "Zc10", "Zc11", "Zc12", "Zc13", "Zc14", "Zc15", "Zc16",
                           "Zc17", "Zc18", "Zc19", "Zc20", "Zc21", "Zc22", "Zc23", "Zc24", "Zc25", "Zc26", "Zc27", "Zc28", "Zc29", "Zc30", "Zc31"};

stat_m m_static_channel_manage_config_load_config(char *src_str, struct channel_manage_conf *config);

stat_m m_static_channel_manage_config_init(struct channel_manage_conf *config, int device_type)
{
    stat_m stat = succ_r;
    char temp_str[100] = {0};
    int ste = 0;
    for (uint8_t i = 1; i <= device_type && i < 32; i++)
    {
        // DEBUG_TEST( DB_I,"loop(%d)-%d\n", i, device_type);
        // 最后开始的配置初始化
        memset(temp_str, 0, sizeof(temp_str));
        // 1、第一部是先加载新配置，后面的就配置如果有的话 会替换新的配置，没有就没有
        stat = m_callable_drive_flash_read(M_TYPE_Str, new_mz_able[i], temp_str);
        if (stat == succ_r)
        {
            DEBUG_TEST(DB_W,"ZC%d - %s", i, temp_str);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_str, "%d,%d|%d,%f|%f|%f,%lld,%lld,%d,%f",

                   &ste, &config[i].channel_config_a[0], &config[i].channel_config_a[1],
                   &config[i].channel_config_b[0], &config[i].channel_config_b[1], &config[i].channel_config_b[2],
                   &config[i].disable_start_time, &config[i].disable_end_time, &config[i].channel_config_choose, &config[i].flow_speed_f);

            config[i].channel_state = ste;

            // DEBUG_TEST( DB_I,"(m_static_channel_manage_config %hhd: ) status: %d A0: %d A1: %d B0: %.02f B1: %.02f B2: %.02f Start: %lld End: %lld Config Choose%d",
            //            ste, config[i].channel_config_a[0], config[i].channel_config_a[1],
            //            con   fig[i].channel_config_b[M_SMART_SACK_AR], config[i].channel_config_b[M_SMART_SACK_IR], config[i].channel_config_b[M_SMART_SACK_ASA],
            //            config[i].disable_start_time, config[i].disable_end_time, config[i].channel_config_choose);
        }
        else
        {
            config[i].channel_state = M_STATIC_CHANNEL_STATE_ENABLE;
            config[i].disable_end_time = 0;
            config[i].disable_start_time = 0;
            if (m_callable_flash_rescue_message_flag(mz_able[i+1 ], M_GLO_STROAGE_RESCUE_MESSAGE_GET) == succ_r)
            {
                config[i].disable_start_time = 1561514;
                config[i].disable_end_time = 4952356454;
                config[i].channel_id = i;
                config[i].channel_state = M_STATIC_CHANNEL_STATE_DISABLE;
                m_static_channel_manage_config_change_config(&config[i]);               
                m_callable_flash_rescue_message_flag(mz_able[i ], M_GLO_STROAGE_RESCUE_MESSAGE_DEL);
            }
            config[i].channel_config_choose = 0;
            config[i].channel_config_b[M_SMART_SACK_AR] = DEFAULT_AR;
            config[i].channel_config_b[M_SMART_SACK_IR] = DEFAULT_IR;
            config[i].channel_config_b[M_SMART_SACK_ASA] = DEFAULT_ASA;
            config[i].flow_speed_f = 0.63f;
        }

        // // if (m_callable_rescue_message_str(new_mz_able[i], temp_str, M_RESCUE_MESSAGE_GET) == succ_r)
        // {
        //     m_static_channel_manage_config_load_config(temp_str, &config[i]);
        //     config[i].channel_id = i;
        //     // DEBUG_TEST( DB_I,"disable %hhd (%d)", config[i].channel_id, config[i].channel_state);
        // }

        // 2、加载旧配置 替换为新的配置（禁用启用）
        // if (m_callable_rescue_message_flag(mz_able[i + 1], M_RESCUE_MESSAGE_GET) != fail_r)
        // // config[i].channel_state = M_STATIC_CHANNEL_STATE_ENABLE, printf("en(%hhd)\n", i + 1);
        // // else
        // {
        //     // 3、如果替换成功 删掉旧配置 存下新的配置
        //     config[i].channel_state = M_STATIC_CHANNEL_STATE_DISABLE;
        //     config[i].channel_id = i + 1;
        //     // 以前的配置都是 永久性的 所以给一个默认永久的时间
        //     config[i].disable_start_time = 19536200;
        //     config[i].disable_end_time = 4093470397;

        //     // rescue_message_flag(mz_able[i], M_RESCUE_MESSAGE_DEL);

        //     m_static_channel_manage_config_change_config(&config[i]);
        //     // DEBUG_TEST( DB_I,"disable %hhd", config[i].channel_id);
        // }
    }
    return stat;
}
stat_m m_static_channel_manage_config_load_config(char *src_str, struct channel_manage_conf *config)
{
    stat_m stat = fail_r;
    // printf("%s\n", src_str);
    // En/Dis able
    // m_callable_data_parse_format_vue("%d", &config->channel_state, src_str);
    // // if ()
    // // Config A
    // m_callable_data_parse_foramt_param("%d" INVE_CHANN "%d", &config->channel_config_a[0], &config->channel_config_a[1], src_str);
    // // Config B
    // m_callable_data_parse_foramt_param("%d" INVE_CHANN "%d", &config->channel_config_b[0], &config->channel_config_b[1], src_str);
    // // Start / End  Time
    // m_callable_data_parse_format_vue("%lld", &config->disable_start_time, src_str);
    // m_callable_data_parse_format_vue("%lld", &config->disable_end_time, src_str);

    return stat;
}

stat_m m_static_channel_manage_config_change_config_del(uint8_t chan_id)
{
    // return rescue_message_str(new_mz_able[chan_id - 1], NULL, M_RESCUE_MESSAGE_DEL);
    DEBUG_TEST( DB_I,"delete ...");
    return 0;
}
stat_m m_static_channel_manage_config_change_config(struct channel_manage_conf *config)
{
    stat_m stat = fail_r;
    char strc[100] = "";
    sprintf(strc, "%d,%d|%d,%.02f|%.02f|%.02f,%lld,%lld,%d,%.03f",
            config->channel_state, config->channel_config_a[0], config->channel_config_a[1],
            config->channel_config_b[0], config->channel_config_b[1], config->channel_config_b[2],
            config->disable_start_time, config->disable_end_time, config->channel_config_choose, config->flow_speed_f);
    DEBUG_TEST( DB_I,"(m_static_channel_manage_config_change_config)  result : %s", strc);
    // m_ext_flash_write(Str, mz_able[config->channel_id], strc);
    stat = m_callable_drive_flash_write(M_TYPE_Str, new_mz_able[config->channel_id], strc);
    // rescue_message_str(new_mz_able[config->channel_id - 1], strc, M_RESCUE_MESSAGE_SET);
    return stat;
}
