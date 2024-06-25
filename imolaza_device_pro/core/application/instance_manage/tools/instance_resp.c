

#include "instance_algorithm.h"

char remote_buf[1536] = {0};

const char M_INST_RESP_OK[10] = "ok";
const char M_INST_RESP_FAIL[10] = "fail";

stat_m m_static_instance_running_able(
    int cmd,
    uint64_t instance_running_id,              // 计划ID
    uint8_t start_channel,                     // 开始区域
    uint32_t start_channel_time_sum,           // 开始总时间
    uint32_t start_channel_will_running_time,  // 开始将要运行
    uint8_t end_channel,                       // 结束区域
    uint32_t end_channel_reality_time,         // 结束区域已经运行
    uint32_t end_channel_current_running_time, // 结束区域本次
    uint32_t running_once_sum,                 // 未中断的本批次运行
    uint32_t running_sum,                      // 总的已经运行
    uint32_t end_channel_state,                // 结束区域状态
    uint64_t instance_id,                      // 实例ID
    uint64_t in_time_s)                        // 当前时间
{
    /*,%d,%d,%d*/
    sprintf(remote_buf, "[%d,%lld,%d,%d,%d,%d,%d,%lld,%d,%lld]#@#\n", cmd, instance_running_id,
            start_channel,
            start_channel_time_sum,
            start_channel_will_running_time,
            end_channel,
            end_channel_reality_time,
            // end_channel_current_running_time,
            // running_once_sum,
            // running_sum,
            instance_id,
            end_channel_state,
            in_time_s);
    // m_callable_network_data_tx(true, remote_buf, strlen(remote_buf));
    m_callable_spec_scene_net_connext_tx(remote_buf, strlen(remote_buf), cmd, 0, M_TIMEOUT_LONG_LIMIT_MIN, true);

    // printf("CMD %d ID %lld 开始区域 %hhd 开始总时间 %d 将运行 %d  结束区域 %hhd 结束已经 %d 结束本次 %d 批次 %d 总运行 %d 实例ID %d, 结束区域状态 %d 当前时间 %lld\n",
    //        cmd,
    //        instance_running_id,
    //        start_channel,
    //        start_channel_time_sum,
    //        start_channel_will_running_time,
    //        end_channel,
    //        end_channel_reality_time,
    //        end_channel_current_running_time,
    //        running_once_sum,
    //        running_sum,
    //        instance_id,
    //        end_channel_state,
    //        in_time_s

    // );

    return 0;
}

uint64_t cmd_count = 1;

char temp_ace[512] = {0};
void m_static_pce_str_to_str(enum m_paramete_enum ty1, void *arg1, char *out_str, bool is_end)
{
    memset(temp_ace, 0, sizeof(temp_ace));
    switch (ty1)
    {
    case M_TYPE_U8:
        sprintf(temp_ace, "%hhd", *(uint8_t *)arg1);
        strcat(temp_ace, ",");
        break;
    case M_TYPE_U16:
        sprintf(temp_ace, "%hd", *(uint16_t *)arg1);
        strcat(temp_ace, ",");
        break;
    case M_TYPE_U32:
        sprintf(temp_ace, "%d", *(uint32_t *)arg1);
        strcat(temp_ace, ",");
        break;
    case M_TYPE_Int:
        sprintf(temp_ace, "%d", *(int *)arg1);
        if (!is_end)
            strcat(temp_ace, ",");
        break;
    case M_TYPE_Float:
        sprintf(temp_ace, "%.3f", *(float *)arg1);
        if (!is_end)
            strcat(temp_ace, ",");
        break;
    case M_TYPE_U64:
        sprintf(temp_ace, "%lld", *(uint64_t *)arg1);
        if (!is_end)
            strcat(temp_ace, ",");
        break;
    case M_TYPE_Str:
        // sprintf(temp_ace, "%lld", *(uint64_t *)arg1);
        strcpy(temp_ace, arg1);
        if (!is_end)
            strcat(temp_ace, ",");
        break;
    case M_TYPE_Str_OK:
        strcat(temp_ace, M_INST_RESP_OK);
        if (!is_end)
            strcat(temp_ace, ",");
        break;
    case M_TYPE_Str_FAIL:
        strcat(temp_ace, M_INST_RESP_FAIL);
        if (!is_end)
            strcat(temp_ace, ",");
        break;

    default:
        return;
        break;
    }

    strcat(out_str, temp_ace);
}

stat_m m_callable_local_resp_to_remote(int cmd,
                                       enum m_paramete_enum ty1, void *arg1,
                                       enum m_paramete_enum ty2, void *arg2,
                                       enum m_paramete_enum ty3, void *arg3,
                                       enum m_paramete_enum ty4, void *arg4,
                                       uint64_t in_time_s, bool is_queue)
{
    memset(remote_buf, 0, sizeof(remote_buf));
    stat_m stat = fail_r;
    strcat(remote_buf, "[");
    m_static_pce_str_to_str(M_TYPE_Int, ((void *)&cmd), remote_buf, false);
    m_static_pce_str_to_str(ty1, arg1, remote_buf, false);

    m_static_pce_str_to_str(ty2, arg2, remote_buf, false);

    m_static_pce_str_to_str(ty3, arg3, remote_buf, false);

    m_static_pce_str_to_str(ty4, arg4, remote_buf, false);

    m_static_pce_str_to_str(M_TYPE_U64, ((void *)&in_time_s), remote_buf, true);
    strcat(remote_buf, "]#@#\n\0");
#ifdef M_GLOBAL_TRANSFORM_USE_TEA_ENCRYPT
    m_callable_crypt_xxtea_encrypt(remote_buf, strlen(remote_buf), remote_buf);
#endif /* M_GLOBAL_TRANSFORM_USE_TEA_ENCRYPT */
    // m_callable_network_data_tx(is_queue, remote_buf, strlen(remote_buf));
    m_callable_spec_scene_net_connext_tx(remote_buf, strlen(remote_buf), cmd, 0, M_TIMEOUT_LONG_LIMIT_MIN, is_queue);

    DEBUG_TEST(DB_I, " low ------     >    %s|", remote_buf);
    return stat;
}
/**
 * @brief 反馈数据到远端 pro版本
 *
 * @param cmd
 * @param ty1
 * @param arg1
 * @param ty2
 * @param arg2
 * @param ty3
 * @param arg3
 * @param ty4
 * @param arg4
 * @param ty5
 * @param arg5
 * @param ty6
 * @param arg6
 * @param ty7
 * @param arg7
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
                                           uint64_t in_time_s)
{
    memset(remote_buf, 0, sizeof(remote_buf));
    stat_m stat = fail_r;
    strcat(remote_buf, "[");
    m_static_pce_str_to_str(M_TYPE_Int, ((void *)&cmd), remote_buf, false);
    m_static_pce_str_to_str(ty1, arg1, remote_buf, false);

    m_static_pce_str_to_str(ty2, arg2, remote_buf, false);

    m_static_pce_str_to_str(ty3, arg3, remote_buf, false);

    m_static_pce_str_to_str(ty4, arg4, remote_buf, false);
    m_static_pce_str_to_str(ty5, arg5, remote_buf, false);
    m_static_pce_str_to_str(ty6, arg6, remote_buf, false);
    m_static_pce_str_to_str(ty7, arg7, remote_buf, false);

    m_static_pce_str_to_str(M_TYPE_U64, ((void *)&in_time_s), remote_buf, true);
    strcat(remote_buf, "]#@#\n\0");
    // m_callable_network_data_tx(true, remote_buf, strlen(remote_buf));
    m_callable_spec_scene_net_connext_tx(remote_buf, strlen(remote_buf), cmd, 0, M_TIMEOUT_LONG_LIMIT_MIN, true);
    DEBUG_TEST(DB_I, "pro %d------     >    %s|", strlen(remote_buf), remote_buf);
    return stat;
}
/**
 * @brief 反馈参数到远端 pro-max版本
 *
 * @param cmd
 * @param ty1
 * @param arg1
 * @param ty2
 * @param arg2
 * @param ty3
 * @param arg3
 * @param ty4
 * @param arg4
 * @param ty5
 * @param arg5
 * @param ty6
 * @param arg6
 * @param ty7
 * @param arg7
 * @param ty8
 * @param arg8
 * @param ty9
 * @param arg9
 * @param ty10
 * @param arg10
 * @param ty11
 * @param arg11
 * @param ty12
 * @param arg12
 * @param in_time_s
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
                                               uint64_t in_time_s)
{
    memset(remote_buf, 0, sizeof(remote_buf));
    stat_m stat = fail_r;
    strcat(remote_buf, "[");
    m_static_pce_str_to_str(M_TYPE_Int, ((void *)&cmd), remote_buf, false);
    m_static_pce_str_to_str(ty1, arg1, remote_buf, false);

    m_static_pce_str_to_str(ty2, arg2, remote_buf, false);

    m_static_pce_str_to_str(ty3, arg3, remote_buf, false);

    m_static_pce_str_to_str(ty4, arg4, remote_buf, false);
    m_static_pce_str_to_str(ty5, arg5, remote_buf, false);
    m_static_pce_str_to_str(ty6, arg6, remote_buf, false);
    m_static_pce_str_to_str(ty7, arg7, remote_buf, false);
    m_static_pce_str_to_str(ty8, arg8, remote_buf, false);
    m_static_pce_str_to_str(ty9, arg9, remote_buf, false);
    m_static_pce_str_to_str(ty10, arg10, remote_buf, false);

    if (cmd != M_CMD_NOTIFY_TO_SERVER_DEVICE_OFFLINE_TO_ONLINE)
        m_static_pce_str_to_str(ty11, arg11, remote_buf, false);
    else
        m_static_pce_str_to_str(ty11, arg11, remote_buf, true);

    m_static_pce_str_to_str(ty12, arg12, remote_buf, false);

    if (cmd == M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH || cmd == M_CMD_TWOWAY_TO_SERVER_FAST_RUN_SWITCH)
    {
        m_static_pce_str_to_str(M_TYPE_U64, (void *)&cmd_count, remote_buf, false);
        cmd_count++;
    }
    if (cmd != M_CMD_NOTIFY_TO_SERVER_DEVICE_OFFLINE_TO_ONLINE)
        m_static_pce_str_to_str(M_TYPE_U64, ((void *)&in_time_s), remote_buf, true);

    strcat(remote_buf, "]#@#\n\0");
    if (cmd != M_CMD_NOTIFY_TO_SERVER_DEVICE_OFFLINE_TO_ONLINE)
        m_callable_spec_scene_net_connext_tx(remote_buf, strlen(remote_buf), cmd, 0, M_TIMEOUT_LONG_LIMIT_MIN, true);
    else
        m_callable_spec_scene_net_connext_tx(remote_buf, strlen(remote_buf), cmd, 0, M_TIMEOUT_LONG_LIMIT_MIN, false);
    DEBUG_TEST(DB_I, " pro-max ------     >    %s", remote_buf);
    return stat;
}
