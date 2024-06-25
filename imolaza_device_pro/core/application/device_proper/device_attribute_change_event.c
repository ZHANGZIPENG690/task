/**
 * @file device_attribute_change_event.c
 * @author your name (you@domain.com)
 * @brief 当所有的配置发生改变的时候 将由他 进行处理
 * @version 0.1
 * @date 2023-10-18
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "core/fset_lib.h"
#include "../../middle/event_distribution/event_distribution.h"
#include "../instance_manage/manage/instance_list.h"

instance_doc_t *current_instance_ms = NULL;

/**
 * @brief  当所有的配置发生改变的时候 将由他 进行处理
 *
 * @param event_id 事件ID
 * @param event_data 事件数据
 * @param in_time_s 触发时间
 * @return stat_m
 */
stat_m m_callable_device_attribute_config_change_notify_event_callable(enum device_function_change_notify_event event_id, void *event_data, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    uint64_t temp_id = 0;
    int temp_int = 0;
    switch (event_id)
    {
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_DORMANT: // 休眠模式
        stat = m_callable_device_dormant_current_is_effective(in_time_s);
        if (stat == fail_r) // 有影响
        {
            m_callable_instance_running_event_input(DORMANT_MODE_STOP, in_time_s);
            /** 看下传感器跳过的话 */
            m_callable_sensor_staging_clean(M_CONST_NONE_NUM, None);
        }

        break;

    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_A_SKIP:
        temp_id = *(uint64_t *)event_data;
        stat = m_static_instance_manage_current_instance_id_is_effective(temp_id);
        if (stat == succ_r && m_callable_schedule_skip_current_is_effective(temp_id, in_time_s) == fail_r) // 是
        {
            // 有影响
            m_callable_instance_running_event_input(A_SIGNLE_SKIP_STOP, in_time_s);
        }
        else
        {
            m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC,
                                                    M_TYPE_U64, (uint64_t *)&temp_id,
                                                    M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                    M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                    M_TYPE_NULL, NULL,
                                                    M_TYPE_NULL, NULL,
                                                    M_TYPE_NULL, NULL,
                                                    M_TYPE_NULL, NULL,
                                                    in_time_s);
            if (m_callable_sensor_staging_check_obj_id(temp_id) == succ_r)
            {
                /** 看下传感器跳过的话 */
                m_callable_sensor_staging_clean(M_CONST_NONE_NUM, None);
            }
        }

        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_GLOBAL_SKIP:
        stat = m_callable_water_delay_current_is_effective(in_time_s);

        if (stat == fail_r && m_static_instance_manage_current_instance_is_effective(true) == succ_r) // 有影响
        {
            m_callable_instance_running_event_input(GLOBAL_SKIP_STOP, in_time_s);
        }
        else
            m_callable_local_resp_to_remote_pro(M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC,
                                                M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                in_time_s);

        /** 看下传感器跳过的话 */
        m_callable_sensor_staging_clean(M_CONST_NONE_NUM, None);

        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_ABORT_GLOBAL_SKIP:
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_ABORT_A_SKIP:
        temp_id = *(uint64_t *)event_data;

        current_instance_ms = NULL;
        m_static_instance_list_find_by_id(temp_id, &current_instance_ms);
        if (current_instance_ms != NULL)
        {
            current_instance_ms->is_genger_info = M_INSTANCE_IS_GENER_INFO_INIT;
        }
        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_WEATHER_SKIP:
        temp_id = *(uint64_t *)event_data;
        // stat = m_callable_weather_manage_get_is_effective(temp_id, &temp_int);
        stat = m_static_instance_manage_current_instance_id_is_effective(temp_id);

        if (stat == succ_r) // 有影响
        {
            m_callable_instance_running_event_input(WIND_RAIN_SKIP_STOP, in_time_s);
        }
        break;
        /** 30 */
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SMART_CONFIG:
        temp_id = *(uint64_t *)event_data;
        m_callable_instance_running_gener_obj(temp_id, in_time_s);

        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SENSOR_OPEN_SKIP:
        temp_int = *(int *)event_data;
        stat = m_callable_instance_running_event_input(SENSOR_TRIGGER, temp_int);

        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SENSOR_CLOSE_SKIP:
        in_time_s = *(uint64_t *)event_data;
        m_callable_instance_running_event_input(SENSOR_DIS_TRIGGER, in_time_s);

        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_TIME:
        // 更新 日出日落
        // m_callable_drive_flash_write();
        temp_id = *(uint64_t *)event_data;
        m_callable_device_attribute_set_time(in_time_s, temp_id);
        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_OPEN:
        /** 计划操作开启 */

        temp_id = *(uint64_t *)event_data;
        stat = m_static_instance_manage_current_instance_id_is_effective(temp_id);
        if (stat != succ_r)
        {
            m_static_time_link_notece_node_enable(temp_id);
            m_callable_exce_schedule_stroage_en_dis_able(temp_id, true);
            m_callable_instance_running_event_input(SCHEDULE_DISABLE_STOP, in_time_s);
        }
        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_CLOSE:
        /** 计划操作关闭 */
        // 先做
        temp_id = *(uint64_t *)event_data;
        stat = m_static_instance_manage_current_instance_id_is_effective(temp_id);
        if (stat == succ_r)
        {
            m_callable_instance_running_event_input(SCHEDULE_DISABLE_STOP, in_time_s);
        }
        else
        {
            /** 看下传感器跳过的话 */
            if (m_callable_sensor_staging_check_obj_id(temp_id) == succ_r)
            {
                m_callable_sensor_staging_clean(M_CONST_NONE_NUM, None);
            }
        }
        m_static_time_link_notece_node_disable(temp_id);
        m_callable_exce_schedule_stroage_en_dis_able(temp_id, false);

        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_CHANNEL_CLOSE:
        temp_int = *(uint8_t *)event_data;
        if (m_callable_channel_manage_current_is_effective(temp_int, in_time_s) == succ_r)
        {
            if (m_static_instance_manage_current_channel_is_effective(temp_int) == succ_r)
                m_callable_instance_running_event_input(CHANNEL_DISABLE_TRIGGER, in_time_s);
            else
            {

                DEBUG_TEST( DB_I, "m_static_instance_manage_current_channel_is_effective 141 None (%d)", temp_int);
            }
        }
        else
        {
            DEBUG_TEST( DB_I,"m_callable_channel_manage_current_is_effective 146 None (%d)", temp_int);
        }

        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_DELE:
        temp_id = *(uint64_t *)event_data;
        stat = m_static_instance_manage_current_instance_id_is_effective(temp_id);
        if (stat == succ_r)
        {
            m_callable_instance_running_event_input(SCHEDULE_DELETE_STOP, m_callable_timer_manage_get_device_time());
        }
        else if (m_callable_sensor_staging_check_obj_id(temp_id) == succ_r)
        {
            /** 看下传感器跳过的话 */
            m_callable_sensor_staging_clean(M_CONST_NONE_NUM, None);
        }
        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_CHANGE:
        temp_id = *(uint64_t *)event_data;
        stat = m_static_instance_manage_current_instance_id_is_effective(temp_id);
        if (stat == succ_r)
        {
            m_callable_instance_running_event_input(SCHEDULE_CHANGE_STOP, m_callable_timer_manage_get_device_time());
        }
        else if (m_callable_sensor_staging_check_obj_id(temp_id) == succ_r)
        {
            /** 看下传感器跳过的话 */
            m_callable_sensor_staging_clean(M_CONST_NONE_NUM, None);
        }
        break;
    case M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_LOCAL_CHANGE:
        DEBUG_TEST(DB_W,"M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_LOCAL_CHANGE");
        m_callable_instance_running_event_input(SCHEDULE_CHANGE_STOP, m_callable_timer_manage_get_device_time());
        m_callable_instance_manage_update_spec_schedule_time(1);
        break;

    default:
        break;
    }
    return stat;
}
