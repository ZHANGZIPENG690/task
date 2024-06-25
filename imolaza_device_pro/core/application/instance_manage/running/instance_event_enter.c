#include "instance_running.h"
#include "../instance.h"

/**
 * @brief The call start execute new schedule
 * @param schedule_instance schedule instance object
 * @param start_time triggering time
 * @todo Unit Input ...
 *
 * @warning " in_instance_id " must be the exact time when the schedule ends, or the unpredictable results will occur
 * @return stat_m
 *          @retval fail_r fail
 *          @retval succ_r success
 */
stat_m m_callable_instance_running_execute_start(instance_obj *running_instance, uint64_t start_time)
{
    stat_m stat = succ_r;
    if (VALUE_IS_INVATE(running_instance->sio_running_id) || (running_instance->sio_run_type > 1 && running_instance->sio_run_type < 0) ||
        running_instance->sio_channel == NULL)
        stat = fail_r;
    if (stat == succ_r)
    {
        m_static_instance_manage_set_current_instance(running_instance);
        // m_static_instance_running_fsm_post(M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, M_BASE_STATUS_RUNNING, (void *)running_instance, start_time);
    }
    return stat;
}
/** 短路的区域集合，保证区域不会重复 */
static int short_array_channel[35] = {0};
/** 短路实例的有效时间 */
static uint64_t short_time_channel = 0;

/**
 * @brief 短路ID 一般设置成有效时间 计划的结束时间  没有的话加入返回succ_r     有的话 返回 fail_r
 *
 * @param in_short_id
 * @param in_ch_id
 * @return stat_m
 */
stat_m m_static_instance_enreg_instance_short_channel(uint64_t in_short_id, int in_ch_id, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    int yX = 0;
    if (short_time_channel != in_short_id)
    {
        memset(short_array_channel, 0, sizeof(short_array_channel));
        short_time_channel = in_short_id;
    }

    for (int i = 0; i < 35; i++)
    {
        // 有的话 返回 fail_r
        if (short_array_channel[i] == 0 && yX == 0)
            yX = i;
        if (short_array_channel[i] == in_ch_id)
        {
            stat = fail_r;
            break;
        }
    }
    if (stat == succ_r)
    {
        short_array_channel[yX] = in_ch_id;
    }

    return stat;
}

/**
 * @brief Event trigger entry when scheduled at runtime
 * @param event_id Event id
 * @param trigger_time triggering time
 * @todo Unit Input ...
 *
 * @return stat_m
 *          @retval fail_r Execution was successful but may not take effect
 *          @retval succ_r Execution was successful
 */
instance_obj *current_instance_fs = NULL;
/** 防止恶意触发 */
int tim = 0;
/** 时间ABC */
uint64_t abb_trig = 0;
char sKeyss[40] = {0};

stat_m m_callable_instance_running_event_input(enum mschedule_event event_id, uint64_t trigger_time)
{

    stat_m stat = fail_r;
    int temp_int_cmd = 0;
    int temp_int_a = 0;
    int temp_int_b = 0;
    current_instance_fs = NULL;
    stat = m_static_instance_manage_get_current_instance(&current_instance_fs);
    DEBUG_TEST(DB_I, "  %d", event_id);
    // DEBUG_TEST( DB_I,%d", stat);
    if (current_instance_fs != NULL && current_instance_fs->sio_running_id > 0) // && current_instance_fs->sio_status == M_BASE_STATUS_RUNNING
    {
        // enum base_state cur_ste = current_instance_fs->sio_status;
        if (event_id >= CHANGING_OVER_START_TRIGGER && event_id <= CHANGING_OVER_RIGHT_TRIGGER)
        {
            if (true || current_instance_fs->sio_run_type == M_INSTANCE_RUN_TYPE_FAST_RUN)
            {
                DEBUG_TEST(DB_W, "Quick operation Event_id %s\n", toname(event_id));
                switch (event_id)
                {
                case CHANGING_OVER_START_TRIGGER:
                    if (current_instance_fs->sio_status == M_BASE_STATUS_PAUSE)
                        m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, M_BASE_STATUS_RUNNING, (void *)current_instance_fs, trigger_time);
                    else if (current_instance_fs->sio_status == M_BASE_STATUS_RUNNING)
                        m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, M_BASE_STATUS_PAUSE, (void *)current_instance_fs, trigger_time);
                    else if (current_instance_fs->sio_status == M_BASE_STATUS_SACK)
                        DEBUG_TEST(DB_W, "The soaking state does not support this operation");

                    break;
                case SOCKET_CMD_STOP:
                case CHANGING_OVER_STOP_TRIGGER:
                    m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                    if (current_instance_fs->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
                    {
                        memset(sKeyss, 0, sizeof(sKeyss));
                        sprintf(sKeyss, "%s%lld", "Ss", current_instance_fs->sio_running_id);
                        m_callable_drive_flash_write(M_TYPE_U64, sKeyss, (void *)&current_instance_fs->sio_instance_id);
                    }
                    break;
                    /** 切換 */
                case CHANGING_OVER_LEFT_TRIGGER:
                    if (trigger_time < tim + 1)
                    {
                        ;
                    }
                    else if (current_instance_fs->sio_status == M_BASE_STATUS_PAUSE || current_instance_fs->sio_status == M_BASE_STATUS_SACK || current_instance_fs->sio_max_channel == 1 || current_instance_fs->sio_clone_max_channel == 1)
                    {
                        DEBUG_TEST(DB_W, "The soaking state does not support this operation,  Insufficient area");
                    }
                    else
                        m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_LEFT, M_BASE_STATUS_RUNNING, (void *)current_instance_fs, trigger_time);
                    break;
                case CHANGING_OVER_RIGHT_TRIGGER:
                    if (trigger_time < tim + 1)
                    {
                        ;
                    }
                    else if (current_instance_fs->sio_status == M_BASE_STATUS_PAUSE || current_instance_fs->sio_status == M_BASE_STATUS_SACK || current_instance_fs->sio_max_channel == 1 || current_instance_fs->sio_clone_max_channel == 1)
                    {
                        DEBUG_TEST(DB_W, "The soaking state does not support this operation ,Insufficient area");
                    }
                    else
                        m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_RIGHT, M_BASE_STATUS_RUNNING, (void *)current_instance_fs, trigger_time);
                    break;
                default:
                    break;
                }
                tim = trigger_time;
            }
        }
        else
        {
            switch (event_id)
            {
                /** 停止 */
            case MANUAL_STOP:
            case SCHEDULE_DELETE_STOP:
                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, true, trigger_time);
                // m_static_instance_running_fsm_post( M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;
            case SOCKET_CMD_STOP:
                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, true, trigger_time);
                // m_static_instance_running_fsm_post(  M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;
            case SCHEDULE_CHANGE_STOP:
                DEBUG_TEST(DB_I, "Schedule modification Stop Schedule");

                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, true, trigger_time);
                // m_static_instance_running_fsm_post(  M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;
            case SCHEDULE_DISABLE_STOP:
                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, true, trigger_time);
                // m_static_instance_running_fsm_post(  M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;
            case A_SIGNLE_SKIP_STOP:
                DEBUG_TEST(DB_I, "Schedule Single  Skip Stop");
                temp_int_a = m_callable_current_check_get_final_value();

                m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC,
                                                        M_TYPE_U64, (void *)&current_instance_fs->sio_running_id,                                                                         // 计划ID
                                                        M_TYPE_U8, (void *)&current_instance_fs->instance_running_root_unit->current_running_unit->unit_running_channel,                  // 运行通道
                                                        M_TYPE_U32, (void *)current_instance_fs->instance_running_root_unit->current_running_unit->unit_zone_real_total_time,             // 区域已经运行总时间
                                                        M_TYPE_U32, (void *)&current_instance_fs->instance_running_root_unit->current_running_unit->unit_this_time_real_water_statistics, // 单次浇水时间
                                                        M_TYPE_U32, (void *)&current_instance_fs->cb_already_run_time_sum,                                                                // 计划总的已经运行时间
                                                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,                                                                                            // code
                                                        M_TYPE_U64, (void *)&current_instance_fs->sio_instance_id,                                                                        // 实例ID
                                                        M_TYPE_Int, (void *)&temp_int_a,                                                                                                  // 电流
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        trigger_time);
                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, false, trigger_time);
                // m_static_instance_running_fsm_post(  M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;
            case GLOBAL_SKIP_STOP:
                DEBUG_TEST(DB_I, "Schedule Global  Skip Stop");

                temp_int_a = m_callable_current_check_get_final_value();
                m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC,
                                                        M_TYPE_U64, (void *)&current_instance_fs->sio_running_id,
                                                        M_TYPE_U8, (void *)&current_instance_fs->instance_running_root_unit->current_running_unit->unit_running_channel,
                                                        M_TYPE_U32, (void *)&current_instance_fs->instance_running_root_unit->current_running_unit->unit_this_time_real_water_statistics,
                                                        M_TYPE_U32, (void *)current_instance_fs->instance_running_root_unit->current_running_unit->unit_zone_real_total_time,
                                                        M_TYPE_U32, (void *)&current_instance_fs->cb_already_run_time_sum,
                                                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                        M_TYPE_U64, (void *)&current_instance_fs->sio_instance_id,
                                                        M_TYPE_Int, (void *)&temp_int_a,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        trigger_time);
                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, false, trigger_time);

                // m_static_instance_running_fsm_post(  M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;
            case WIND_RAIN_SKIP_STOP:
                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, true, trigger_time);
                // m_static_instance_running_fsm_post(  M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;
            case DORMANT_MODE_STOP:
                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, true, trigger_time);
                // m_static_instance_running_fsm_post(  M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;

                /** 觸發 */
            case RUNNING_PAUSE:
                m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, M_BASE_STATUS_PAUSE, (void *)current_instance_fs, trigger_time);
                break;
            case RUNNING_RECOVER:
                if (current_instance_fs->sio_status != M_BASE_STATUS_RUNNING)
                    m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, M_BASE_STATUS_RUNNING, (void *)current_instance_fs, trigger_time);
                break;
            case SENSOR_TRIGGER:
                if (current_instance_fs->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
                    stat = m_callable_sensor_staging_trig_handle(current_instance_fs, trigger_time);
                else
                    stat = fail_r;
                // m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, M_BASE_STATUS_IDLE, (void *)current_instance_fs, trigger_time);
                break;
            case SENSOR_DIS_TRIGGER:
                // m_callable_sensor_staging_distrig_handle(current_instance_fs , trigger_time);
                // m_static_instance_running_fsm_post(current_instance_fs->sio_status, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, M_BASE_STATUS_RUNNING, (void *)current_instance_fs, trigger_time);
                break;
            case CHANNEL_ENABLE_TRIGGER:

                break;
            case CHANNEL_DISABLE_TRIGGER:
                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE, true, trigger_time);

                break;

            case M_INSTANCE_RUNNING_UPDATE_WEATHER:
                temp_int_cmd = M_CMD_TWOWAY_REQU_WEATHER_BUFF;
                m_callable_local_resp_to_remote(
                    temp_int_cmd, M_TYPE_U64, (void *)&current_instance_fs->sio_running_id,
                    M_TYPE_NULL, NULL,
                    M_TYPE_NULL, NULL,
                    M_TYPE_NULL, NULL, trigger_time, false);
                break;
            case M_INSTANCE_RUNNING_CURRENT_NOTIFY:
                if (current_instance_fs->sio_status != M_BASE_STATUS_SACK && current_instance_fs->sio_status != M_BASE_STATUS_PAUSE)
                {
                    temp_int_cmd = (uint64_t)m_callable_current_check_get_final_value();
                    /** 区域 */
                    temp_int_a = trigger_time / 10;
                    /** 事件 */
                    temp_int_b = trigger_time % 10;
                    trigger_time = m_callable_timer_manage_get_device_time();
                    m_callable_local_resp_to_remote(
                        M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK,
                        M_TYPE_U64, (void *)&current_instance_fs->sio_running_id,
                        M_TYPE_U8, (void *)&temp_int_a,
                        M_TYPE_Int, (void *)&temp_int_cmd,
                        M_TYPE_Int, (void *)&temp_int_b, trigger_time, false);
                }
                break;
            case M_INSTANCE_RUNNING_CURRENT_NOTIFY_VALUE_OUT:
                /** 区域 */
                temp_int_a = trigger_time % 1000 / 10;
                /** 电流值 */
                temp_int_cmd = trigger_time / 1000;
                /** 事件 */
                temp_int_b = trigger_time % 10;
                trigger_time = m_callable_timer_manage_get_device_time();
                m_callable_local_resp_to_remote(
                    M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK, M_TYPE_U64, (void *)&current_instance_fs->sio_running_id,
                    M_TYPE_U8, (void *)&temp_int_a,                       // 区域
                    M_TYPE_Int, (void *)&temp_int_cmd,                    // 电流值
                    M_TYPE_Int, (void *)&temp_int_b, trigger_time, true); // 模式-时间
                break;
            case M_INSTANCE_RUNNING_CURRENT_NOTIFY_INTERRUPT:
                temp_int_cmd = MAX_SHORT_CURRENT;
                /** 区域 */
                temp_int_a = trigger_time % 1000 / 10;
                /** 事件 */
                temp_int_b = trigger_time % 10;
                trigger_time = m_callable_timer_manage_get_device_time();
                DEBUG_TEST(DB_W, "m_callable_display_status_get %d , zone %d  , Evt %d", m_callable_display_status_get(), temp_int_a, temp_int_b);
                if (temp_int_b == M_CURRENT_SHORT_MODE_SIGN_ZONE)
                    m_callable_display_status(M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE, temp_int_a);
                else if (temp_int_b == M_CURRENT_SHORT_MODE_PUMP)
                    m_callable_display_status(M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE, temp_int_a);
                // if (abb_trig == trigger_time)
                //     break;
                // abb_trig = trigger_time;
                current_instance_fs->invalid = true;
                /** 没有的话加入 有的话 返回false  */
                if (m_static_instance_enreg_instance_short_channel(current_instance_fs->sio_instance_id, temp_int_a, trigger_time) == succ_r)
                {
                    m_callable_local_resp_to_remote(
                        M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK, M_TYPE_U64, (void *)&current_instance_fs->sio_running_id,
                        M_TYPE_U8, (void *)&temp_int_a,                       // 区域
                        M_TYPE_Int, (void *)&temp_int_cmd,                    // 电流值
                        M_TYPE_Int, (void *)&temp_int_b, trigger_time, true); // 模式-时间
                }
                break;
            default:
                break;
            }
        }
    }

    return stat;
}

instance_obj *current_instance_cu = NULL;
/** 电流信息缓存 */
int temp_current_int = 0;
/** 流量信息缓存 */
float temp_flow_float = 0;

stat_m m_callable_instance_running_monitor(uint64_t current_time)
{
    stat_m stat = fail_r;
    int in_cause = 0;
    stat = m_static_instance_manage_get_current_instance(&current_instance_cu);
    // if (current_instance_cu != NULL && stat == succ_r)
    //     printf("klklkl(%d) - %lld", 10 , current_instance_cu->sio_status );
    if (stat == succ_r && current_instance_cu != NULL && current_instance_cu->sio_status == M_BASE_STATUS_RUNNING)
    {
        if (!current_instance_cu->invalid)
        {
            // DEBUG_TEST( DB_I,- - - - - -- - - - - - - -  current_instance_cu->sio_status  %lld" ,  current_instance_cu->sio_status );
            //  wobj->unit_this_time_real_water_statistics = 0;
            //      只是为了记录真实的浇水时间
            current_instance_cu->cb_total_already_run_time_one_round++;
            current_instance_cu->cb_already_run_time_sum++;
            //  当前运行的单元也在计数
            if (current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time >
                current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_real_water_statistics)
                current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_real_water_statistics++;

            {
                if (current_instance_cu->instance_running_root_unit->current_running_unit->unit_zone_real_total_time != NULL &&
                    *current_instance_cu->instance_running_root_unit->current_running_unit->unit_zone_real_total_time <
                        current_instance_cu->instance_running_root_unit->current_running_unit->unit_should_running_time)
                    (*current_instance_cu->instance_running_root_unit->current_running_unit->unit_zone_real_total_time)++;
            }
            // printf("\nklklkl(%d - %d)\n", current_instance_cu->instance_running_root_unit != NULL , current_instance_cu->instance_running_root_unit->current_running_unit != NULL );

            if (current_instance_cu != NULL &&
                current_instance_cu->instance_running_root_unit != NULL &&
                current_instance_cu->instance_running_root_unit->current_running_unit != NULL)
            {

                DEBUG_TEST(DB_I, ".%lld - %hhd(one %d - ch_sum %d - real %d  ) ", current_instance_cu->sio_running_id,
                           current_instance_cu->instance_running_root_unit->current_running_unit->unit_running_channel,
                           current_instance_cu->cb_total_already_run_time_one_round,
                           current_instance_cu->cb_already_run_time_sum,
                           current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_real_water_statistics);

                /** 运行反馈   超过 120 秒*/
                if (current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time > 120 &&
                    /** 是第一次 */
                    *current_instance_cu->instance_running_root_unit->current_running_unit->unit_zone_real_total_time == 120)
                {
                    m_callable_current_is_interrupt();
                    temp_current_int = m_callable_current_check_get_final_value();

                    m_callable_flow_sensor_get_total_value(current_instance_cu->instance_running_root_unit->current_running_unit->unit_running_channel,
                                                           120,
                                                           &temp_flow_float,
                                                           false);
                    m_callable_local_resp_to_remote(
                        M_CMD_NOTIFY_TO_DEVICE_RUNNING_RESP_CURRENT_AND_FLOW, M_TYPE_U64, (void *)&current_instance_cu->sio_running_id,
                        M_TYPE_U8, (void *)&current_instance_cu->instance_running_root_unit->current_running_unit->unit_running_channel,

                        M_TYPE_Int, (void *)&temp_current_int,
                        M_TYPE_Float, (void *)&temp_flow_float,
                        current_time, false);
                }
            }
        }
        // 可以在这加一个判断防止浇水过量
        if (current_instance_cu != NULL &&
            current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_cost_running_time <=
                current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time + 7)
            current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_cost_running_time++;

        else if (current_instance_cu != NULL && current_instance_cu->sio_status == M_BASE_STATUS_RUNNING &&
                 current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_cost_running_time >
                     current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time + 6)
        {
            DEBUG_E("Time exceeds, it will be stopped directly %d > Should %d)",
                    current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_cost_running_time,
                    current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time);
            in_cause = plan_unknow_stop;
            // 数据超出运行时间
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO, M_TYPE_Int, (void *)&in_cause,
                                            M_TYPE_U64, &current_instance_cu->sio_running_id,
                                            M_TYPE_U32, &current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_cost_running_time,
                                            M_TYPE_U32, &current_instance_cu->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time,
                                            M_TYPE_NULL, NULL);
            m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, true, current_time);
            // m_static_instance_running_event_handle( M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE ,
            //  M_BASE_STATUS_RUNNING, M_BASE_STATUS_STOP, void *pre_obj, void *in_data, uint64_t in_time_s);
        }
    }
    current_instance_cu = NULL;
    return stat;
}
stat_m m_callable_instance_running_event_out(enum mschedule_event event_id, uint64_t trigger_time)
{

    return succ_r;
}