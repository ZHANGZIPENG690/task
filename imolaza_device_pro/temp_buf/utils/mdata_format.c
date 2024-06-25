
#include "core/utils/util.h"
// #include "core/timer/mtime.h"
#include "core/fset_lib.h"

char buf[180] = {0};

uint64_t  m_callable_get_utc_time_t()
{
    return m_callable_get_device_time_t();
}


stat_m m_cmd_finish(char *in_data, int in_data_len, int cpcmd)
{
    stat_m stat = fail_r;
    if (in_data != NULL && in_data_len > 0 && cpcmd > 0)
    {
        m_callable_spec_scene_net_connext_tx(in_data, in_data_len, cpcmd, cpcmd, 0, true);
    }
    return stat;
}
stat_m m_cmd_finish_dir(char *in_data, int in_data_len, int cpcmd)
{
    stat_m stat = fail_r;
    if (in_data != NULL && in_data_len > 0 && cpcmd > 0)
    {
        m_callable_spec_scene_net_connext_tx(in_data, in_data_len, cpcmd, cpcmd, 0, false);
    }
    return stat;
}

stat_m create_send_data_to_server_int(int cmd, int cv1t)
{
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, cmd);
    m_mjson_append_new_element(buf, Int, (void *)&cv1t);
    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    // m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());m_cmd_finish(buf,strlen(buf),cmd);
    return 0;
}
// stat_m create_send_data_to_server_debug(int cmd, enum debug_code code, uint64_t info)
// {
//     if (err_state_check(STAT_SOCK_CONNECT) != M_ERR && _device.state.onLine)
//     {
//         m_mjson_create_new_table(buf);
//         m_mjson_prepare_cmd(buf, cmd);
//         m_mjson_append_new_element(buf, Int, &code);
//         m_mjson_append_new_element(buf, U64, info);
//         m_mjson_append_new_element(buf, U64, info);
//         // new_debug_finish((uint64_t)cmd);
//         m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
//         m_cmd_finish(buf, strlen(buf), cmd);
//         // if (_device.onLine)
//     }
//     return 0;
// }
stat_m create_send_data_to_server_22_07_29(int cmd, uint64_t in_schid, 
                                            uint8_t stc, uint32_t in_chanShouldRunningSum,
                                            uint32_t in_chanRunningSum, uint32_t in_plan_stop_time,
                                            uint8_t stype, uint8_t sensor_zone, uint64_t runtime_intermediate)
{

    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    m_mjson_append_new_element(buf, U64, (&in_schid));
    m_mjson_append_new_element(buf, U16, (&stc));
    m_mjson_append_new_element(buf, U32, (&in_chanShouldRunningSum));
    m_mjson_append_new_element(buf, U32, (&in_chanRunningSum));
    m_mjson_append_new_element(buf, U64, (&runtime_intermediate));
    m_mjson_append_new_element(buf, U32, (&in_plan_stop_time));
    m_mjson_append_new_element(buf, U16, (&stype));
    m_mjson_append_new_element(buf, U8, (&sensor_zone));
    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return 0;
}

// stat_m create_send_data_to_server_debug_timenode(int cmd, enum debug_code code, uint64_t info, uint64_t v2, uint64_t tag)
// {
//     if (err_state_check(STAT_SOCK_CONNECT) != M_ERR && _device.state.onLine) // _device.debug
//     {
//         m_mjson_create_new_table(buf);
//         m_mjson_prepare_cmd(buf, (cmd));
//         m_mjson_append_new_element(buf, Int, (&code));
//         m_mjson_append_new_element(buf, U64, (&info));
//         m_mjson_append_new_element(buf, U64, (&v2));
//         m_mjson_append_new_element(buf, U64, (&tag));
//         // new_debug_finish((uint64_t)cmd);
//         if (_device.state.onLine)
//             m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
//         m_cmd_finish(buf, strlen(buf), cmd);
//         // else
//         //     memset(json_buf, 0, sizeof(json_buf));
//     }
//     return 0;
// }
// This function is only for A, used to indicate the start or end of the plan

stat_m create_send_data_to_server_22_02_23(int cmd, uint8_t cv1, uint32_t cv1t, uint8_t cv2, uint32_t cvt, bool Tpause_Fstop)
{
    uint8_t in_pause = 1, in_stop = 2;
    uint16_t in_null = 254;
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    {
        if (cv1 == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U8, (&cv1));
    }
    m_mjson_append_new_element(buf, U32, (cv1t));
    {
        if (cv2 == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U8, (&cv2));
    }
    m_mjson_append_new_element(buf, U32, (cvt));
    if (Tpause_Fstop)
        m_mjson_append_new_element(buf, U8, (in_pause));
    else
        m_mjson_append_new_element(buf, U8, (in_stop));
    // m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());m_cmd_finish(buf,strlen(buf),cmd);
    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return 0;
}
// [64,开始计划ID,开始通道,开始通道运行总时长, 开始通道本次将运行时间,结束通道,结束通道累积运行时间,结束通道本次运行时间,通道结束状态,计划ID结束时间,时间戳]
stat_m create_send_data_to_server_22_06_13(int cmd, uint64_t in_schid, uint8_t stc, uint32_t in_chanRunningSum, uint32_t in_currChanWillt,
                                           uint8_t enc, uint32_t enct, uint32_t in_end_chanRunningt, bool Tpause_Fstop, uint64_t in_end_plant , uint64_t pre_utc_time)
{
    uint8_t in_pause = 1, in_stop = 2;
    uint16_t in_null = 254;
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    m_mjson_append_new_element(buf, U64, (&in_schid));
    {
        if (stc == 0)
        {
            m_mjson_append_new_element(buf, U16, (&in_null));
            m_mjson_append_new_element(buf, U16, (&in_null));
            m_mjson_append_new_element(buf, U16, (&in_null));
        }
        else
        {
            m_mjson_append_new_element(buf, U8, (&stc));
            m_mjson_append_new_element(buf, U32, (&in_chanRunningSum));
            m_mjson_append_new_element(buf, U32, (&in_currChanWillt));
        }
    }
    {
        if (enc == 0)
        {
            m_mjson_append_new_element(buf, U16, (&in_null));
            m_mjson_append_new_element(buf, U16, (&in_null));
            m_mjson_append_new_element(buf, U16, (&in_null));
        }
        else
        {
            m_mjson_append_new_element(buf, U8, (&enc));
            m_mjson_append_new_element(buf, U32, (&enct));
            m_mjson_append_new_element(buf, U32, (&in_end_chanRunningt));
        }
    }
    if (Tpause_Fstop)
        m_mjson_append_new_element(buf, U8, (&in_pause));
    else
        m_mjson_append_new_element(buf, U8, (&in_stop));

    // m_mjson_append_new_element(buf, U32,(ct);
    m_mjson_append_new_element(buf, U64, (&in_end_plant));
    m_mjson_declare_table_finish(buf, pre_utc_time);
    m_cmd_finish(buf, strlen(buf), cmd);
    DEBUG_OUT_TO_FILE_COLOR("\033[0;32m%s\n" ,buf);
    DEBUG_TEST(DB_W,"---> %s" , buf);
        // DEBUG_OUT_TO_FILE("[%d,%lld,%hhd,%d,%d,%hhd,%d,%d,%d,%lld]", cmd,  in_schid,  stc,
        //                                  in_chanRunningSum,  in_currChanWillt,
        //                                  enc,  enct,  in_end_chanRunningt,
        //                                  Tpause_Fstop,  in_end_plant);
    return succ_r;
}

/**
 * @brief Create a send data to server 22 05 18 sync time object     28
 *
 * @param cmd
 * @param pid
 * @param cid
 * @param ctime
 * @param state
 * @param ctime2
 * @param cv3t
 * @return stat_m
 */

stat_m create_send_data_to_server_22_05_18_sync_time(int cmd,
                                                     uint64_t pid,
                                                     uint16_t cid,
                                                     uint32_t ctime,
                                                     uint32_t state,
                                                     uint32_t ctime2,
                                                     uint64_t cv3t,
                                                     uint64_t chan_already_RunTimeSum)
{
    uint16_t in_null = 254;
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    {
        if (pid == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U64, (&pid));
        if (cid == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U8, (&cid));
        if (ctime == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U32, (&ctime));
    }

    m_mjson_append_new_element(buf, U32, (&state));
    m_mjson_append_new_element(buf, U32, (&ctime2));
    m_mjson_append_new_element(buf, U64, (&chan_already_RunTimeSum));
    m_mjson_append_new_element(buf, U64, (&cv3t));
    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return 0;
}
stat_m create_send_data_to_server_22_05_18(int cmd,
                                           uint64_t pid,
                                           uint16_t cid,
                                           uint32_t ctime,
                                           uint32_t state)
{
    uint16_t in_null = 254;
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    {
        if (pid == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U64, (&pid));
        if (cid == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U8, (&cid));
        if (ctime == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U32, (&ctime));
    }

    m_mjson_append_new_element(buf, U32, (&state));

    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return 0;
}
stat_m create_send_data_to_server_22_07_07_query_decvice_status(int cmd,
                                                                char *ver,
                                                                char *snc,
                                                                uint16_t psum,
                                                                uint32_t offline_time,
                                                                uint64_t pid,
                                                                uint32_t pcid,
                                                                uint32_t prt,
                                                                uint32_t prc)
{
    uint16_t in_null = 254;
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    m_mjson_append_new_element(buf, Str, (ver));
    m_mjson_append_new_element(buf, Str, (snc));
    m_mjson_append_new_element(buf, U16, (&psum));
    m_mjson_append_new_element(buf, U32, (&offline_time));
    m_mjson_append_new_element(buf, U64, (&pid));
    m_mjson_append_new_element(buf, U32, (&pcid));
    m_mjson_append_new_element(buf, U32, (&prt));
    m_mjson_append_new_element(buf, U32, (&prc));
    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return 0;
}
stat_m create_send_data_to_server_22_03_11(int cmd,
                                           uint64_t pid,
                                           uint8_t cid,
                                           uint32_t ctime,
                                           uint64_t tsp,
                                           uint32_t state,
                                           uint64_t acc)
{
    uint16_t in_null = 254;
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    {
        if (pid == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U64, (&pid));
        if (cid == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U8, (&cid));
    }
    m_mjson_append_new_element(buf, U32, (&ctime));

    m_mjson_append_new_element(buf, U64, (&acc));
    m_mjson_append_new_element(buf, U64, (&tsp));
    m_mjson_append_new_element(buf, U32, (&state));

    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return 0;
}

// This function is only for A, used to indicate the start or end of the fastrun
stat_m create_send_data_to_server_22_06_23_ota_start(int cmd)
{
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return succ_r;
}
stat_m create_send_data_to_server_22_03_02_fastRun(int cmd,
                                                   uint64_t tid,
                                                   uint8_t cv1,
                                                   uint32_t cv1t,
                                                   uint64_t tid2,
                                                   uint8_t cv2,
                                                   uint32_t cvt,
                                                   uint32_t curt,
                                                   bool Tpause_Fstop)
{
    uint8_t in_pause = 1, in_stop = 2, in_sock = 3;
    uint16_t in_null = 254;
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    {
        if (tid == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U64, (&tid));
        if (cv1 == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U8, (&cv1));
    }
    m_mjson_append_new_element(buf, U32, (&cv1t));
    {
        if (tid2 == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U64, (&tid2));

        if (cv2 == 0)
            m_mjson_append_new_element(buf, U16, (&in_null));
        else
            m_mjson_append_new_element(buf, U8, (&cv2));
    }
    m_mjson_append_new_element(buf, U32, (&cvt));
    m_mjson_append_new_element(buf, U32, (&curt));
    if (Tpause_Fstop == in_sock)
        m_mjson_append_new_element(buf, U8, (&in_sock));
    if (Tpause_Fstop)
        m_mjson_append_new_element(buf, U8, (&in_pause));
    else
        m_mjson_append_new_element(buf, U8, (&in_stop));
    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return 0;
}
stat_m create_send_data_to_server_22_06_20_fastrun(int cmd,
                                                   uint32_t fast_id,
                                                   uint8_t start_channel_id,
                                                   uint32_t fast_running_time,
                                                   uint32_t this_operation_run_time)
{
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    m_mjson_append_new_element(buf, U64, (&fast_id));
    m_mjson_append_new_element(buf, U8, (&start_channel_id));
    m_mjson_append_new_element(buf, U32, (&fast_running_time));
    m_mjson_append_new_element(buf, U32, (&this_operation_run_time));
    m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
    m_cmd_finish(buf, strlen(buf), cmd);
    return 0;
}
// [24,schedule_id,start_channel_id,channel_total_will_runtime,current_will_run_time,schedule_end_time,timestamp]
/**
 * @brief Create a send data to server 22 06 13 plan start object(2022-09-12 edit .. add chan_already_RunTimeSum)
 * @param cmd
 * @param schedule_id schedule_id
 * @param start_channel_id start_channel_id
 * @param shouldRunTime shouldRunTime
 * @param current_will_run_time current_will_run_time
 * @param schedule_end_time schedule_end_time
 * @return stat_m
 */
stat_m create_send_data_to_server_22_06_13_plan_start(int cmd,
                                                      uint64_t schedule_id,
                                                      uint8_t start_channel_id,
                                                      uint64_t shouldRunTime,
                                                      uint32_t current_will_run_time,
                                                      uint64_t chan_already_RunTimeSum,
                                                      uint64_t schedule_end_time,
                                                      uint64_t pre_utc_time
                                                      )
{
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    m_mjson_append_new_element(buf, U64, (&schedule_id));
    m_mjson_append_new_element(buf, U8, (&start_channel_id));
    m_mjson_append_new_element(buf, U64, (&shouldRunTime));
    m_mjson_append_new_element(buf, U32, (&current_will_run_time));
    m_mjson_append_new_element(buf, U64, (&chan_already_RunTimeSum));
    if (schedule_end_time > 0)
        m_mjson_append_new_element(buf, U64, (&schedule_end_time));
    m_mjson_declare_table_finish(buf, pre_utc_time);
    m_cmd_finish(buf, strlen(buf), cmd);
    DEBUG_TEST(DB_W,"---> %s" , buf);
    DEBUG_OUT_TO_FILE_COLOR("\033[0;32m%s\n" ,buf);
    return 0;
}
stat_m create_send_data_to_server(int cmd, type_m type_1, void *cv1, type_m type_2, void *cv2, type_m type_3, void *cv3)
{
    // printf("******");
    // if (err_state_check(STAT_SOCK_CONNECT) != M_ERR)
    {
        m_mjson_create_new_table(buf);
        m_mjson_prepare_cmd(buf, (cmd));
            m_mjson_append_new_element(buf, type_1, cv1);
            m_mjson_append_new_element(buf, type_2, cv2);
            m_mjson_append_new_element(buf, type_3, cv3);
        m_mjson_declare_table_finish(buf, m_callable_get_utc_time_t());
        return m_cmd_finish(buf, strlen(buf), cmd);
        // return 0;
    }
    // else
    //     return fail_m;
}
// void json_clean_buff(void)
// {
//     memset(json_temp, 0, sizeof(json_temp));
//     if (json_aplist_buf != NULL)
//         memset(json_aplist_buf, 0, strlen(json_aplist_buf));
//     memset(json_buf, 0, sizeof(json_buf));
// }
// [11,a641f26d84b862d26a9888acc422b3d8,8.0.077,3]
// [10,a641f26d84b862d26a9888acc422b3d8,HUAWEI_B316_2D01,8.0.074]
// [67,a641f26d84b862d26a9888acc422b3d8,8.0.074,1,0,254,2,-1,2]
// int sn                               char * int uint64_t int int int int
// [67,sn码,固件版本,计划数量,离线时间,计划id,运行状态,剩余运行时间,离线原因]

stat_m create_send_data_to_server_login_in(int cmd, char *name, char *version, int off_line_time,
                                           uint64_t sch_id, int running_state, int remain_time, int off_line_resu, int sch_sum)
{
    m_mjson_create_new_table(buf);
    m_mjson_prepare_cmd(buf, (cmd));
    m_mjson_append_new_element(buf, Str, "_device.encrypt_code.key");
    if (cmd == 15 && name != NULL)
    {
        m_mjson_append_new_element(buf, Str, name);
    }

    m_mjson_append_new_element(buf, Str, version);

    m_mjson_append_new_element(buf, U32, (&sch_sum));
    if (cmd == 67)
    {
        m_mjson_append_new_element(buf, U64, (&sch_id));
        m_mjson_append_new_element(buf, Int, (&running_state));
        m_mjson_append_new_element(buf, Int, (&remain_time));
        m_mjson_append_new_element(buf, Int, (&off_line_resu));
    }
    m_mjson_done_a_table(buf);
    m_cmd_finish_dir(buf, strlen(buf), cmd);
    return succ_r;
}

// stat_m createe_send_data_to_server(char *cc, const char *ck1, type_m type_1, void *cv1, const char *ck2, type_m type_2, void *cv2)
// {
//     // json_simple_create();
//     // json_add_Stritem("cmd", cc);
//     // if (type_1 == Str)
//     //     json_add_Stritem(ck1, (char *)cv1);
//     // else if (type_1 == U32)
//     //     json_add_u32item(ck1, (uint32_t)(cv1));
//     // else if (type_1 == U8)
//     //     json_add_u8item(ck1, (uint8_t)(cv1));
//     // if (type_2 == Str)
//     //     json_add_Stritem(ck2, (&char *)cv2);
//     // else if (type_2 == U32)
//     //     json_add_u32item(ck2, (&uint32_t)(cv2));
//     // else if (type_2 == U8)
//     //     json_add_u8item(ck2, (&uint8_t)(cv2));
//     return json_finish();
// }
// {
//     // json_simple_create();
//     // json_add_Stritem("cmd", cc);
//     // if (type_1 == Str)
//     //     json_add_Stritem(ck1, (char *)cv1);
//     // else if (type_1 == U32)
//     //     json_add_u32item(ck1, (uint32_t)(cv1));
//     // else if (type_1 == U8)
//     //     json_add_u8item(ck1, (uint8_t)(cv1));
//     // if (type_2 == Str)
//     //     json_add_Stritem(ck2, (&char *)cv2);
//     // else if (type_2 == U32)
//     //     json_add_u32item(ck2, (&uint32_t)(cv2));
//     // else if (type_2 == U8)
//     //     json_add_u8item(ck2, (&uint8_t)(cv2));
//     return json_finish();
// }
