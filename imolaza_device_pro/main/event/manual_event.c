#include "core/fset_lib.h"
#include "../application/include.h"

/**
 * @brief 事件处理
 *
 * @param event_id 事件id
 * @param pre_channel 上一个区域
 * @param next_channel 下一个 当前区域
 * @param has_benn_runnint_time 已经运行时间
 * @param current_time 当前时间
 * @return stat_m
 */
stat_m m_ext_manual_event_handle(enum operate_event event_id, uint8_t pre_channel, uint8_t next_channel, uint32_t has_benn_runnint_time, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    int resp = 0;
    int resp_man = 254;
    float temp_float = 0.0f;
    DEBUG_TEST(DB_W,"m_ext_manual_event_handle (%d) .. ", event_id);
    in_time_s = m_callable_timer_manage_get_device_time();
    /** 电流  */
    int temp_int = 0;
    temp_int = m_callable_current_check_get_final_value();
    
    m_callable_flow_sensor_get_total_value(next_channel, has_benn_runnint_time, &temp_float, true);

    switch (event_id)
    {

    case M_OPERATE_EVENT_START:
        DEBUG_TEST(DB_W,"Manual Start .. %d", next_channel);
        resp = M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_START;

        m_callable_solenoid_manage_open(next_channel);
        m_callable_device_proper_status_update_to_manual_running();
        m_callable_local_resp_to_remote(resp, M_TYPE_Int, (void *)&resp_man,
                                        M_TYPE_U8,
                                        (void *)&next_channel,
                                        M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, in_time_s, true);

        break;
    case M_OPERATE_EVENT_START_MANY:
        DEBUG_TEST(DB_W,"M_OPERATE_EVENT_START_MANY .. ");

        break;
    case M_OPERATE_EVENT_START_SELECT:

        // m_callable_solenoid_manage_open(next_channel);

        break;
    case M_OPERATE_EVENT_RUNNING_SWITCH:

        m_callable_solenoid_manage_continuously(pre_channel, next_channel);
        // m_callable_solenoid_manage_open(next_channel);

        DEBUG_TEST(DB_W,"Manual Swit ..  Pre: %hhd Time: %d Next: %hhd ", pre_channel, has_benn_runnint_time, next_channel);
        resp = M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_SWITCH;
        resp_man = 2;

        m_callable_local_resp_to_remote_pro(resp,
                                            M_TYPE_U8, (void *)&next_channel,
                                            M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                            M_TYPE_U8, (void *)&pre_channel,
                                            M_TYPE_U32, (void *)&has_benn_runnint_time,
                                            M_TYPE_Int, (void *)&resp_man,

                                            M_TYPE_Int, (void *)&temp_int,
                                            M_TYPE_Float, (void *)&temp_float, // 流量
                                            in_time_s);
        // DEBUG_TEST(DB_W,"M_OPERATE_EVENT_RUNNING_SWITCH .. ");
        break;
    case M_OPERATE_EVENT_CHANNEL_SWITCH:

        DEBUG_TEST(DB_W,"M_OPERATE_EVENT_CHANNEL_SWITCH .. ");
        // m_callable_solenoid_manage_continuously(pre_channel, next_channel);
        break;
    case M_OPERATE_EVENT_PAUSE:

        resp = M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_STOP;
        DEBUG_TEST(DB_W,"M_OPERATE_EVENT_PAUSE .. ");
        // m_callable_flow_sensor_get_total_value(&temp_float, true);
        m_callable_solenoid_manage_close(next_channel);
        m_callable_local_resp_to_remote_pro(resp,
                                            M_TYPE_Int, (void *)&resp_man,
                                            M_TYPE_U8, (void *)&next_channel,
                                            M_TYPE_U32, (void *)&has_benn_runnint_time,
                                            M_TYPE_Int, (void *)&temp_int,
                                            M_TYPE_Float, (void *)&temp_float, // 流量
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            in_time_s);

        m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, 0);
        m_callable_device_proper_status_update_to_idle();
        break;
    case M_OPERATE_EVENT_STOP:

        // m_callable_solenoid_manage_close(next_channel);
        DEBUG_TEST(DB_W,"M_OPERATE_EVENT_STOP .. %d  %d", temp_int , next_channel);
        resp = M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_STOP;
        // m_callable_flow_sensor_get_total_value(&temp_float, true);
        m_callable_solenoid_manage_close(next_channel);
        m_callable_local_resp_to_remote_pro(resp,
                                            M_TYPE_Int, (void *)&resp_man,
                                            M_TYPE_U8, (void *)&next_channel,
                                            M_TYPE_U32, (void *)&has_benn_runnint_time,
                                            M_TYPE_Int, (void *)&temp_int,
                                            M_TYPE_Float, (void *)&temp_float, // 流量
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            in_time_s);
        m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
        m_callable_device_proper_status_update_to_idle();
        // m_callable_drive_io_pin12_init();
        break;
    case M_OPERATE_EVENT_STOP_SELECT:

        DEBUG_TEST(DB_W,"M_OPERATE_EVENT_STOP_SELECT .. ");
        break;
    case M_OPERATE_EVENT_STOP_ALL:

        DEBUG_TEST(DB_W,"M_OPERATE_EVENT_STOP_ALL .. ");
        break;

    default:
        break;
    }

    return stat;
}
