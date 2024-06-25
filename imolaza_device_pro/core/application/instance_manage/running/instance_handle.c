/**
 * @file instance_handle.c
 * @author your name (you@domain.com)
 * @brief 处理关于实例的问题，运行事件
 * @version 0.1
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "core/fset_lib.h"
#include "../instance.h"
#include "../fsm/instance_fsm.h"
#include "instance_running.h"
#include "../tools/instance_algorithm.h"

#define M_WATER_DELAY_CODE 2
#define M_SIGN_SKIP_CODE 1

stat_m m_static_instance_running_skip_notify_event_handle(instance_doc_t *in_wtd, instance_obj *in_obj, uint64_t in_instance_id, uint16_t in_chan_id, enum skip_manage_type skip_typr,
                                                          enum seneor_chann sen_chan, enum seneor_type sen_type, int weather_code, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    int rep = 0;
    // 62 和 70 区别  70 1 -  62 2
    int water_delay_skip_and_sign_type = 0;
    float temp_float = 0.0f;
    int temp_int = 0;
    switch (skip_typr)
    {
    case M_SKIP_MANAGE_TYPE_ALL_CHANNEL_DISABLE:

        water_delay_skip_and_sign_type = debug_schedule_running_but_channel_all_disable;
        m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                        M_TYPE_Int, (void *)&water_delay_skip_and_sign_type,
                                        M_TYPE_U64, in_wtd == NULL ? (void *)&in_obj->sio_running_id : (void *)&in_wtd->sch_info.cu_id,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL,
                                        in_time_s, false); //_time.new_get_hhmmss()
        break;
    case M_SKIP_MANAGE_TYPE_DORMANT_SKIP:
        stat = succ_r;
        water_delay_skip_and_sign_type = plan_dorman_skip_run;
        rep = M_CMD_TWOWAY_HIBERNATION_MODE;
        m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                        M_TYPE_Int, (void *)&water_delay_skip_and_sign_type,
                                        M_TYPE_U64, in_wtd == NULL ? (void *)&in_obj->sio_running_id : (void *)&in_wtd->sch_info.cu_id,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL,
                                        in_time_s, false); //_time.new_get_hhmmss()
        break;

    case M_SKIP_MANAGE_TYPE_WATERDELAY_SKIP:
        water_delay_skip_and_sign_type = M_WATER_DELAY_CODE;
    case M_SKIP_MANAGE_TYPE_SIGN_SKIP:
        rep = M_CMD_NOTIFY_TO_SERVER_A_SCHEDULE_BE_SKIP;
        stat = succ_r;
        if (water_delay_skip_and_sign_type != M_WATER_DELAY_CODE)
            water_delay_skip_and_sign_type = M_SIGN_SKIP_CODE;

        m_callable_local_resp_to_remote(rep, M_TYPE_U64,
                                        in_wtd == NULL ? (void *)&in_obj->sio_running_id : (void *)&in_wtd->sch_info.cu_id,
                                        M_TYPE_U64, (void *)&in_instance_id,
                                        M_TYPE_Int, (void *)&water_delay_skip_and_sign_type,
                                        M_TYPE_NULL, NULL, in_time_s, true);
        break;

    case M_SKIP_MANAGE_TYPE_SENSOR_SKIP:
        // [6,1500,4,0,0,0,1698952380,1,1,1698964680]
        rep = M_CMD_TWOWAY_SENSOR_TRIGE;
        m_run_time.sensor_mode = M_CMD_TWOWAY_SENSOR_TRIGE;
        stat = 1;
        m_callable_sensor_staging_add_trig(in_obj, in_time_s);
        m_static_time_link_notece_node_disable(in_obj->sio_running_id);

        m_callable_flow_sensor_get_total_value(0, 0, &temp_float, true);
        temp_int = m_callable_current_check_get_final_value();
        stat = succ_r;
        // if (weather_code == None)
        // {
        // if (m_callable_sensor_current_is_effective(NULL, NULL, 0) == fail_r)
        //     {
        //         break;
        //     }
        // }
        m_callable_local_resp_to_remote_pro_max(rep, M_TYPE_U64,
                                                in_wtd == NULL ? (void *)&in_obj->sio_running_id : (void *)&in_wtd->sch_info.cu_id,
                                                // M_TYPE_U64, (void *)&in_obj->sio_running_id,
                                                M_TYPE_U16, (void *)&in_chan_id,
                                                M_TYPE_Int, (void *)&sen_type, // 类型 0 常闭 1 常开
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_U64, (void *)&in_instance_id,
                                                M_TYPE_Int, (void *)&stat, // 有无跳过计划
                                                M_TYPE_Int, (void *)&sen_chan,
                                                // M_TYPE_NULL, NULL,
                                                M_TYPE_Float, (void *)&temp_float, // 流量
                                                M_TYPE_Int, (void *)&temp_int,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                in_time_s);
        break;
    case M_SKIP_MANAGE_TYPE_SATURATION_SKIP:
        weather_code = 10;
    case M_SKIP_MANAGE_TYPE_WEATHER_SKIP:

        stat = succ_r;
        rep = M_CMD_NOTIFY_TO_SERVER_SCHEdULE_SKIP;
        // [cmd,schedule_id,channel_id,结束时间,计划本次运行总浇水时长,计划结束时间,跳过类型,时间搓]
        // [31,3943,254,0,0,1699019040,254,10,1699014000]
        // [31,4453,254,0,0,1699008420,254,3,1699008360]
        if (in_wtd == NULL)
            water_delay_skip_and_sign_type = in_obj->sio_max_should_runtime;
        else
            water_delay_skip_and_sign_type = in_wtd->exfun.fn_max_should_runtime;

        m_callable_local_resp_to_remote_pro_max(rep, M_TYPE_U64,
                                                in_wtd == NULL ? (void *)&in_obj->sio_running_id : (void *)&in_wtd->sch_info.cu_id,
                                                // M_TYPE_U8, (void *)&in_wtd->sch_info.cu_id,
                                                M_TYPE_U8, (void *)&in_chan_id,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,

                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&water_delay_skip_and_sign_type,
                                                M_TYPE_U64, (void *)&in_instance_id,

                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&weather_code,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,

                                                in_time_s);
        break;
    default:
        break;
    }

    return stat;
}

/**
 * @brief 计划运行时,根据状态机分配计划运行
 *
 */
// m_schedule_event_running_callback
stat_m m_ext_instance_fsm__run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    // uint64_t present_timesp = in_time_s;
    uint32_t present_time_hhmmss = /*_time.hhmmss*/ (in_time_s % NUM_OF_DAY_SEC_SUM);
    /***
     *  1、 idleing to run
     *  2、 sacking to run
     *  3、 pauseing to run
     *  4、 running to run
     *
     *  A、 LEFT to run
     *  B、 RIGHT to run
     *
     *  C、 pause to run
     */
    DEBUG_TEST(DB_I, "[%lld]m_ext_instance_fsm_(%d)_to_run  ", in_time_s, ste);

    if (pdata != NULL)
    {
        // 1、 idle_to_runnig
        if (ste == M_BASE_STATUS_PREPARE)
        {
            stat = m_static_instance_running_idleing_to_run(cause, ste, pdata, in_time_s);
        }
        // 2、 sack_to_runnig
        else if (ste == M_BASE_STATUS_PAUSE)
        {
            stat = m_static_instance_running_pauseing_to_run(cause, ste, pdata, in_time_s);
        }
        // 3、 pause_to_runnig
        else if (ste == M_BASE_STATUS_SACK)
        {
            stat = m_static_instance_running_sacking_to_run(cause, ste, pdata, in_time_s);
        }
        // 4、 running_to_runnig
        else if (ste == M_BASE_STATUS_RUNNING)
        {
            if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_RIGHT)
                stat = m_static_instance_running_running_to_run_switch(cause, ste, pdata, in_time_s);
            else if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_LEFT)
                stat = m_static_instance_running_running_to_run_switch(cause, ste, pdata, in_time_s);
            else
                stat = m_static_instance_running_running_to_run(cause, ste, pdata, in_time_s);
        }
        else if (ste == M_BASE_STATUS_HANG_UP)
        {
            stat = m_static_instance_running_hanguping_to_run(cause, ste, pdata, in_time_s);
        }
    }

    // DEBUG_TEST(DB_W,"m_action_schedule__running -- > %d", /*_time.hhmmss*/ in_time_s & NUM_OF_DAY_SEC_SUM);
    // uint64_t plan_start_times = m_operation_schedule_calculate_near_start_timesp(wobj->sch_info.available_time.tr_start_time, wobj->WSTime, wobj->exfun.max_should_runtime);
    return stat;
}

stat_m m_ext_instance_fsm__sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    /**
     *  1、 running to sack
     *  2、 pauseing to sack
     *  3、hanguping to sack
     *
     *  4、 pause to run
     */
    DEBUG_TEST(DB_I, "[%lld]m_ext_instance_fsm_(%s)_to_sack  ", in_time_s, toname(ste));
    if (pdata != NULL)
    {
        if (ste == M_BASE_STATUS_RUNNING)
        {
            stat = m_static_instance_running_running_to_sack(cause, ste, pdata, in_time_s);
        }
        else if (ste == M_BASE_STATUS_PAUSE)
        {
            // stat = m_static_instance_running_pauseing_to_recover(cause, ste, pdata, in_time_s);
        }
        else if (ste == M_BASE_STATUS_HANG_UP)
        {
            // stat = m_static_instance_running_hanguping_to_sack(cause, ste, pdata, in_time_s);
        }
    }
    return stat;
}

stat_m m_ext_instance_fsm__prepare(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    /**
     *  1、 idle to prepare
     */
    DEBUG_TEST(DB_I, "[%lld]m_ext_instance_fsm_(%d)_to_prepare  ", in_time_s, ste);
    if (pdata != NULL)
    {
        if (ste == M_BASE_STATUS_IDLE)
        {
            stat = m_static_instance_running_idle_to_prepare(cause, ste, pdata, in_time_s);
        }
    }
    return stat;
}

// m_watering_schedule *tp_obj = 50;
stat_m m_ext_instance_fsm__idle(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    // instance_obj *wobj = (instance_obj *)pdata;
    DEBUG_TEST(DB_I, "m_schedule_event_stop_callback (%lld)  ste %d", in_time_s, ste);
    m_static_instance_running_all_to_idle(cause, ste, pdata, in_time_s);

    return 0;
}

char temp_queue[512] = {0};

stat_m m_static_instance_running_event_handle(enum fsm_change_cause cause, enum base_state pre_ste,
                                              enum base_state curr_ste, void *pre_obj,
                                              void *in_data, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    /** 处理的指令 */
    int in_cmd = 24;
    /** 缓存 Int  pre*/
    int temp_int_pre = 0;
    /** 流量 */
    float temp_float = 0.0f;
    /** 单元完成 */
    int unit_sone = 0;
    /** 将要运行时间*/
    uint32_t will_running_time = 0;

    instance_obj *in_obj = (instance_obj *)in_data;
    struct instance_running_unit *wobj = NULL;

    if (curr_ste != M_BASE_STATUS_PREPARE)
    {
            wobj = in_obj->instance_running_root_unit->current_running_unit;
    }

    if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_WATER_HAMMER)
    {
        // m_callable_current_monitor_stop();
        m_callable_current_monitor_pause_not_clear();
        DEBUG_TEST(DB_I, "Hammer66 Notify: Channel %hhd will open ", wobj->next_running_unit->unit_running_channel);
        m_callable_current_set_checkout_zone(wobj->next_running_unit->unit_running_channel);
        if (m_callable_display_status_get() != M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE)
        {
            // in_obj->invalid = false;
            m_callable_solenoid_manage_M_open();
            m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_WATER_HAMMER);
            m_callable_solenoid_manage_direct_open(wobj->next_running_unit->unit_running_channel);
        }
    }
    else
    {
        struct instance_running_unit *pre_wobj = (struct instance_running_unit *)pre_obj;
        DEBUG_TEST(DB_I, "(%d-%d)  %s -> %s ", pre_ste, curr_ste, toname(pre_ste), toname(curr_ste));

        switch (curr_ste)
        {
        case M_BASE_STATUS_PREPARE:
            if (pre_ste == M_BASE_STATUS_IDLE)
            {
                in_cmd = M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME;
                instance_doc_t *obj_doc = (instance_doc_t *)in_data;
                uint64_t itime = obj_doc->WSTime + obj_doc->exfun.fn_max_coust_time;

                // printf("haha meishi \n");
                m_callable_local_resp_to_remote(in_cmd, M_TYPE_U64, &obj_doc->sch_info.cu_id,
                                                M_TYPE_U64, &itime, M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time(), false);
                obj_doc->is_genger_info = M_INSTANCE_IS_GENER_INFO_WAIT;
            }
            break;
        case M_BASE_STATUS_RUNNING:
            if (pre_ste == M_BASE_STATUS_PREPARE || pre_ste == M_BASE_STATUS_IDLE)
            {
                m_callable_device_proper_status_update_to_schedule_running();

                m_callable_solenoid_manage_open(wobj->unit_running_channel);
                // 计划开始
                if (in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
                    in_cmd = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_START;
                else
                    in_cmd = M_CMD_TWOWAY_FASTRUN_START;
                memset(temp_queue, 0, 512);
                // sprintf(temp_queue , "%d#%d" , );
                // temp_int = m_callable_current_check_get_final_value();
                temp_int_pre = 0;
                m_callable_flow_sensor_get_total_value(wobj->unit_running_channel, 0, &temp_float, true);
                m_static_instance_runnning_queue_str_format_get(in_obj, temp_queue);

                m_run_time.total_duration_current_area[wobj->unit_running_channel] = wobj->unit_should_running_time;
                DEBUG_TEST(DB_I, "0000m_run_time.total_duration_current_area %lld", m_run_time.total_duration_current_area[wobj->unit_running_channel]);

                m_callable_local_resp_to_remote_pro_max(in_cmd, M_TYPE_U64, (void *)&in_obj->sio_running_id,
                                                        M_TYPE_U8, (void *)&wobj->unit_running_channel,
                                                        M_TYPE_U32, (void *)&wobj->unit_should_running_time,

                                                        M_TYPE_U32, (void *)&wobj->unit_this_time_will_running_time, // 本次将要
                                                        M_TYPE_U64, (void *)&in_obj->sio_instance_id,                // 结束时间

                                                        M_TYPE_Str, (void *)temp_queue,           // 队列
                                                        M_TYPE_Int, (void *)&temp_int_pre,        // 电流
                                                        M_TYPE_Int, (void *)&wobj->unit_progress, // 进度
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        in_time_s);
            }
            /** 手动运行切换 */

            else if (pre_ste == M_BASE_STATUS_RUNNING || cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_RIGHT || cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_RIGHT)
            {
                // temp_int = m_callable_current_check_get_final_value();
                temp_int_pre = m_callable_current_check_get_final_value();
                if (m_callable_display_status_get() != M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE)
                {
                    if (*pre_wobj->unit_zone_real_total_time == 1 && in_obj->invalid)
                        *pre_wobj->unit_zone_real_total_time = 0;
                    if (pre_wobj->unit_this_time_real_water_statistics == 1 && in_obj->invalid)
                        pre_wobj->unit_this_time_real_water_statistics = 0;

                    in_obj->invalid = false;
                    m_callable_solenoid_manage_open(wobj->unit_running_channel);
                }
                // 计划切换
                if (in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
                {
                    in_cmd = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH;
                    m_callable_device_proper_status_update_to_schedule_running();
                }
                else
                {
                    in_cmd = M_CMD_TWOWAY_TO_SERVER_FAST_RUN_SWITCH;
                    m_callable_device_proper_status_update_to_fast_running();
                }

                m_callable_flow_sensor_get_total_value(wobj->unit_running_channel, pre_wobj->unit_this_time_real_water_statistics, &temp_float, true);
                // 将要运行时间  重新计算
                will_running_time = wobj->unit_this_time_will_running_time - wobj->unit_this_time_cost_running_time;
                if (pre_wobj->unit_this_time_will_running_time <= pre_wobj->unit_this_time_cost_running_time)
                    unit_sone = 2, in_obj->sio_clone_max_channel--;
                else
                    unit_sone = 1;
                {
                    if (pre_wobj->unit_this_time_real_water_statistics + 1 == pre_wobj->unit_should_running_time)
                    {
                        DEBUG_TEST(DB_I, "Info: Manual Calibration %d -> %d", pre_wobj->unit_this_time_real_water_statistics, pre_wobj->unit_should_running_time);
                        pre_wobj->unit_this_time_real_water_statistics = pre_wobj->unit_should_running_time;
                        *pre_wobj->unit_zone_real_total_time += 1;
                    }
                }
                m_run_time.puase_duration_current_area[pre_wobj->unit_running_channel] = *pre_wobj->unit_zone_real_total_time;

                DEBUG_TEST(DB_I, "1111m_run_time.puase_duration_current_area %lld", m_run_time.puase_duration_current_area[pre_wobj->unit_running_channel]);

                if (in_obj->sio_running_id > 0)
                    m_callable_local_resp_to_remote_pro_max(
                        in_cmd, M_TYPE_U64, (void *)&in_obj->sio_running_id,
                        M_TYPE_U8, (void *)&wobj->unit_running_channel,
                        M_TYPE_U32, (void *)&wobj->unit_should_running_time,

                        M_TYPE_U32, (void *)&will_running_time,                              // 本次将要
                        M_TYPE_U8, (void *)&pre_wobj->unit_running_channel,                  // 结束区域
                        M_TYPE_U32, (void *)pre_wobj->unit_zone_real_total_time,             // 结束区域已经运    0
                        M_TYPE_U32, (void *)&pre_wobj->unit_this_time_real_water_statistics, // 结束区域本次
                        M_TYPE_Int, (void *)&pre_wobj->unit_done,                            // 完成与否
                        M_TYPE_U64, (void *)&in_obj->sio_instance_id,                        // 结束时间
                        M_TYPE_Int, (void *)&temp_int_pre,                                   // 电流
                        M_TYPE_Int, (void *)&wobj->unit_progress,                            // 进度
                        M_TYPE_Float, (void *)&temp_float,                                   // 流量
                        in_time_s);
            }
            else if (pre_ste == M_BASE_STATUS_SACK || pre_ste == M_BASE_STATUS_PAUSE) // 浸泡恢复  或者暂停恢复
            {
                // temp_int = m_callable_current_check_get_final_value();
                // temp_int_pre = m_callable_current_check_get_final_value();
                if (m_callable_display_status_get() != M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE)
                {
                    in_obj->invalid = false;
                    m_callable_solenoid_manage_open(wobj->unit_running_channel);
                }
                if (in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
                {
                    in_cmd = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH;
                }
                else
                    in_cmd = M_CMD_TWOWAY_TO_SERVER_FAST_RUN_SWITCH;

                m_callable_flow_sensor_get_total_value(wobj->unit_running_channel, 0, &temp_float, true);
                // 将要运行时间  重新计算
                will_running_time = wobj->unit_this_time_will_running_time - wobj->unit_this_time_cost_running_time;

                m_run_time.total_duration_current_area[wobj->unit_running_channel] = wobj->unit_should_running_time;
                DEBUG_TEST(DB_I, "2222m_run_time.puase_duration_current_area %lld", m_run_time.total_duration_current_area[wobj->unit_running_channel]);

                if (in_obj->sio_running_id > 0)
                    m_callable_local_resp_to_remote_pro_max(
                        in_cmd, M_TYPE_U64, (void *)&in_obj->sio_running_id,
                        M_TYPE_U8, (void *)&wobj->unit_running_channel,
                        M_TYPE_U32, (void *)&wobj->unit_should_running_time,
                        M_TYPE_U32, (void *)&will_running_time,
                        M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM, // 结束区域
                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM, // 结束区域
                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                        M_TYPE_U64, (void *)&in_obj->sio_instance_id,
                        M_TYPE_Int, (void *)&temp_int_pre,
                        // 进度
                        M_TYPE_Int, (void *)&wobj->unit_progress,
                        // M_TYPE_NULL, NULL,
                        M_TYPE_Float, (void *)&temp_float, // 流量

                        in_time_s);
            }
            // else if (pre_ste == M_BASE_STATUS_PAUSE)
            // {
            //     m_callable_solenoid_manage_open(wobj->unit_running_channel);
            //     in_cmd = M_CMD_TWOWAY_FASTRUN_RECOVER;
            //     // temp_int = (wobj->unit_this_time_will_running_time - wobj->unit_real_water_statistics);
            //     m_callable_local_resp_to_remote(in_cmd, M_TYPE_U64, (void *)&in_obj->sio_running_id,
            //                                     M_TYPE_U8, (void *)&wobj->unit_running_channel,
            //                                     M_TYPE_U32, (void *)&wobj->unit_should_running_time,
            //                                     M_TYPE_U32, (void *)&wobj->unit_this_time_real_water_statistics,
            //                                     in_time_s, true);
            // }
            break;
        case M_BASE_STATUS_PAUSE:
            // in_cmd = M_CMD_TWOWAY_FASTRUN_PAUSE;
            unit_sone = 1;
            // m_callable_solenoid_manage_close(wobj->unit_running_channel);
            // [61,254,254,0,2,2,6,6,1,1701071630,2]
            // temp_int = (wobj->unit_this_time_will_running_time - wobj->unit_real_water_statistics);
            // m_callable_local_resp_to_remote(in_cmd, M_TYPE_U64, (void *)&in_obj->sio_running_id,
            //                                 M_TYPE_U8, (void *)&wobj->unit_running_channel,
            //                                 M_TYPE_U32, (void *)&wobj->unit_this_time_real_water_statistics,
            //                                 M_TYPE_NULL, NULL,
            //                                 in_time_s, true);

            // break;

        case M_BASE_STATUS_SACK:

            temp_int_pre = m_callable_current_check_get_final_value();

            // 计划暂停的时候
            if (in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
            {
                in_cmd = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH;
                unit_sone = pre_wobj->unit_done;
                if (unit_sone == 2)
                    in_obj->sio_clone_max_channel--;
            }
            else
                in_cmd = M_CMD_TWOWAY_TO_SERVER_FAST_RUN_SWITCH;

            m_callable_flow_sensor_get_total_value(0, pre_wobj->unit_this_time_real_water_statistics, &temp_float, true);

            // 将要运行时间  重新计算
            // will_running_time = wobj->unit_this_time_will_running_time - wobj->unit_this_time_cost_running_time;

            m_run_time.puase_duration_current_area[pre_wobj->unit_running_channel] = *pre_wobj->unit_zone_real_total_time;
            DEBUG_TEST(DB_I, "33333m_run_time.puase_duration_current_area %lld", m_run_time.puase_duration_current_area[pre_wobj->unit_running_channel]);

            if (in_obj->sio_running_id > 0)
                m_callable_local_resp_to_remote_pro_max(
                    in_cmd, M_TYPE_U64, (void *)&in_obj->sio_running_id,
                    M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM, // 结束区域
                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM, // 结束区域

                    M_TYPE_U8, (void *)&pre_wobj->unit_running_channel,                  // 结束区域
                    M_TYPE_U32, (void *)pre_wobj->unit_zone_real_total_time,             // 结束区域已经运    0
                    M_TYPE_U32, (void *)&pre_wobj->unit_this_time_real_water_statistics, // 结束区域本次
                    M_TYPE_Int, (void *)&unit_sone,
                    M_TYPE_U64, (void *)&in_obj->sio_instance_id,
                    M_TYPE_Int, (void *)&temp_int_pre,
                    // 进度
                    M_TYPE_Int, (void *)&wobj->unit_progress,
                    // M_TYPE_NULL, NULL,
                    M_TYPE_Float, (void *)&temp_float, // 流量

                    in_time_s);
            // printf("wobj->unit_this_time_real_water_statistics   %d  pre_wobj->unit_this_time_real_water_statistics   %d" ,
            //         wobj->unit_running_channel ,     pre_wobj->unit_running_channel);
            in_obj->cb_total_already_run_time_one_round = 0;

            m_callable_solenoid_manage_close(pre_wobj->unit_running_channel);
            if (wobj != NULL)
                m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, wobj->unit_running_channel);
            wobj->unit_this_time_real_water_statistics = 0; // 结束区域本次
            break;
        case M_BASE_STATUS_IDLE:

            if (pre_ste == M_BASE_STATUS_PREPARE)
            {
                m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
                m_callable_device_proper_status_update_to_idle();
            }
            break;
        case M_BASE_STATUS_HANG_UP:
            break;
        case M_BASE_STATUS_STOP:

            temp_int_pre = m_callable_current_check_get_final_value();

            if (pre_ste == 0)
            {
                m_callable_solenoid_manage_close_all();
            }
            else
            {

                m_callable_flow_sensor_get_total_value(0, 0, &temp_float, true);
                // m_callable_flow_sensor_get_total_value(0, wobj->unit_this_time_real_water_statistics, &temp_float, true);

                if (cause != M_STATIC_FSM_CHANGE_STATE_CAUSE_NOOE_CALLBACK)
                {
                    if (in_obj->cb_already_run_time_sum % 10 == 9)
                        in_obj->cb_already_run_time_sum += 1;
                    if (*wobj->unit_zone_real_total_time % 10 == 9)
                        *wobj->unit_zone_real_total_time += 1;
                    if (wobj->unit_this_time_real_water_statistics % 10 == 9)
                        wobj->unit_this_time_real_water_statistics += 1;

                    if (in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
                        in_cmd = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_END;
                    else
                        in_cmd = M_CMD_TWOWAY_FASTRUN_COMPLETE;
                    m_callable_local_resp_to_remote_pro_max(
                        // 计划ID
                        in_cmd, M_TYPE_U64, (void *)&in_obj->sio_running_id,
                        // 当前结束区域
                        M_TYPE_U8, (void *)&wobj->unit_running_channel,
                        // 当前区域总累积运行时间
                        M_TYPE_U32, (void *)wobj->unit_zone_real_total_time,
                        // 当前区域本次单次运行时间
                        M_TYPE_U32, (void *)&wobj->unit_this_time_real_water_statistics,
                        // 计划总的累积运行时间
                        M_TYPE_U32, (void *)&in_obj->cb_already_run_time_sum,
                        // 实例ID  (设备时间)
                        M_TYPE_U64, (void *)&in_obj->sio_instance_id,
                        // 当前区域最大运行电流
                        M_TYPE_Int, (void *)&temp_int_pre,
                        // 结束区域对应单元进度
                        M_TYPE_Int, (void *)&wobj->unit_progress,
                        M_TYPE_Float, (void *)&temp_float, // 流量

                        M_TYPE_NULL, NULL,
                        M_TYPE_NULL, NULL,
                        M_TYPE_NULL, NULL,
                        // 指令时间 设备时间
                        in_time_s);
                }
                m_callable_solenoid_manage_close(wobj->unit_running_channel);
            }
            m_callable_device_proper_status_update_to_idle();
            m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);

            break;
        default:
            break;
        }
    }
    return stat;
}
stat_m m_ext_instance_fsm__stop(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    printf("m_ext_instance_fsm__stop( %d )", ste);
    if (ste != M_BASE_STATUS_IDLE)
    {
        stat = m_static_instance_running_all_to_stop(cause, ste, pdata, in_time_s);
    }

    return stat;
}

stat_m m_ext_instance_fsm__pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    /**
     *  1、 sack to pause
     *  2、 running to pause
     */
    DEBUG_TEST(DB_I, "[%lld]m_ext_instance_fsm_(%d)_to_pause  ", in_time_s, ste);

    if (ste == M_BASE_STATUS_SACK)
    {
        stat = m_static_instance_running_sacking_to_pause(cause, ste, pdata, in_time_s);
    }
    else if (ste == M_BASE_STATUS_RUNNING)
    {
        stat = m_static_instance_running_running_to_pause(cause, ste, pdata, in_time_s);
    }
    return stat;
}
