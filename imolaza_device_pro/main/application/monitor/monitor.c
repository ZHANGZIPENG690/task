

#include "core/fset_lib.h"

#if !UNIT_TEST
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#endif

const int MANUAL_OPERTAB_CODE = 254;

#if UNIT_TEST
uint64_t t = 0;
uint64_t xTaskGetTickCount()
{
    return t++;
}
#endif
void m_ext_global_monitor_loop(void *args)
{
    int device_status = -1;

    int nmode = 0;
    m_callable_device_attribute_get_running_mode(&nmode);
    while (1)
    {
        if (nmode != M_DEVICE_RUNNING_MODE_CONFIG && nmode != M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG)
        {
            m_callable_sensor_monitor(xTaskGetTickCount());
        }
        m_callable_drive_button_function_monitor(xTaskGetTickCount());
        // DEBUG_TEST( DB_I,%lld"  ,(uint64_t) xTaskGetTickCount());

        m_callable_manual_function_monitor(xTaskGetTickCount());

        m_calllable_current_monitor(device_status);
        mDelay_ms(9);
    }
}
/**
 * @brief 提取电流事件处理
 *
 * @param device_state 设备状态
 * @param interrupt_evt 断开事件
 * @param inter_zone 区域
 * @param in_time_s 触发时间
 * @return stat_m
 */
stat_m m_static_current_monitor_transf(enum mschedule_event ev_id, int device_state, int current_vue, int interrupt_evt, int inter_zone, uint64_t in_time_s)
{
    int temp_id = 0;
    uint32_t temp_int_cmd = 0;
    uint64_t cur_info = 0;

    switch (interrupt_evt)
    {
    /** 优先VT */
    case M_CURRENT_SHORT_MODE_VT:
        m_callable_device_attribute_get_hearward_version(&temp_int_cmd);
        if (temp_int_cmd != DEVICE_HEARWARE_A003 && temp_int_cmd != DEVICE_HEARWARE_B11 && temp_int_cmd != DEVICE_HEARWARE_B12)
            break;
        temp_int_cmd = 254;
        temp_id = MAX_SHORT_CURRENT;
        if (device_state == M_DEVICE_GLOBAL_STATUS_IDLE)
        {
            m_callable_display_status(M_DISPLAY_VT_SHORT_CIRCUIT_MODE, 0);
        }
        m_callable_local_resp_to_remote(
            M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK,
            M_TYPE_Int, &temp_int_cmd,
            M_TYPE_Int, &inter_zone,
            M_TYPE_Int, &temp_id,
            M_TYPE_Int, &interrupt_evt,
            in_time_s, false);
        break;
        /** 校准 更新灯光 ， 更新电流 */
    case M_CURRENT_SHORT_MODE_CALIBRATION:
        m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, inter_zone);
        break;
        /** 其他 */

    default:
        if (device_state == M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING ||
            device_state == M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING)
        {
            inter_zone *= 10;
            cur_info += inter_zone;
            cur_info += interrupt_evt;
            cur_info += current_vue * 1000;
            m_callable_instance_running_event_input(ev_id, cur_info);
        }
        else if (device_state == M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING)
        {
            m_callable_manual_event_export_input(ev_id, interrupt_evt, in_time_s);
        }
        else if (device_state == M_DEVICE_GLOBAL_STATUS_IDLE)
        {
            if (interrupt_evt == M_CURRENT_SHORT_MODE_PUMP)
                m_callable_display_status(M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE, inter_zone);
            else
                m_callable_display_status(M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE, inter_zone);
        }

        break;
    }
    return 0;
}

/**
 * @brief 電流監控和中斷觸發反饋含糊三
 *
 * @param event_id 發送的事件類型
 * @param current_vue 電流值
 * @param is_interrupt 是否觸發短路
 */
void m_ext_current_monitor_result(enum current_event_id event_id, int current_vue, int interrupt_evt, int inter_zone, uint64_t in_time_s)
{

    int device_state = 0, ev_id = M_INSTANCE_RUNNING_CURRENT_NOTIFY_INTERRUPT;
    // uint64_t cur_info = 0;
    in_time_s = m_callable_timer_manage_get_device_time();
    switch (event_id)
    {
    case M_CURRENT_EVENT_VALUE_OUT:
        m_callable_solenoid_manage_M_close();
        // m_ext_drive_solenoid_close_all();
        m_ext_drive_hc595_shiftout_reset();
        m_callable_drive_io_pin12_uninit();
        m_callable_display_status(M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE, inter_zone);
        ev_id = M_INSTANCE_RUNNING_CURRENT_NOTIFY_VALUE_OUT;
        interrupt_evt = M_CURRENT_OUT_VALUE_MODE;
    case M_CURRENT_EVENT_INTERRUPT:
        // 先断开

        // 再反馈
        // temp_int_cmd = (uint64_t)m_callable_current_check_get_final_value();

        if (interrupt_evt == M_CURRENT_SHORT_MODE_WATER_HAMMER)
            break;
        m_callable_device_proper_status_get(&device_state);
        m_static_current_monitor_transf(ev_id, device_state, current_vue, interrupt_evt, inter_zone, in_time_s);
        break;

    case M_CURRENT_EVENT_30_SEC:

        // m_callable_device_proper_status_get(&device_state);
        // if (device_state == M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING ||
        //     device_state == M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING)
        // {
        //     inter_zone *= 10;
        //     cur_info += inter_zone;
        //     cur_info += current_vue*100;
        //     m_callable_instance_running_event_input(M_INSTANCE_RUNNING_CURRENT_NOTIFY, cur_info);
        // }
        // else if (device_state == M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING)
        // {
        //     m_callable_manual_event_export_input(M_INSTANCE_RUNNING_CURRENT_NOTIFY, interrupt_evt, in_time_s);
        // }
        break;
    default:
        break;
    }
}

// stat_m m_ext_drive_solenoid_init(int _device_version_set, int number_of_channels_set,int io_chip)
// {}
/**
 * @brief 電流監控和中斷觸發反饋含糊三
 *
 * @param event_id 發送的事件類型
 * @param current_vue 電流值
 * @param is_interrupt 是否觸發短路
 */
// void m_ext_current_monitor_result(enum current_event_id event_id, int current_vue, uint8_t is_interrupt, uint64_t in_time_s)
// {
//     uint8_t channel_id = 0;
//     int temp_int_cmd = 0;
//     enum device_status device_state;
//     switch (event_id)
//     {
//     case M_CURRENT_EVENT_30_SEC:
//         m_callable_device_proper_status_get(&device_state);
//         // printf("lplp   -   %d", device_state);
//         if (device_state == M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING ||
//             device_state == M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING)
//         {
//             m_callable_instance_running_event_input(M_INSTANCE_RUNNING_CURRENT_NOTIFY, in_time_s);
//         }
//         if (device_state == M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING)
//             m_callable_manual_event_export_input(M_INSTANCE_RUNNING_CURRENT_NOTIFY, in_time_s);

//         break;
//     case M_CURRENT_EVENT_INTERRUPT:
//         // 先断开
//         // 再反馈
//         in_time_s = m_callable_timer_manage_get_device_time();
//         temp_int_cmd = (uint64_t)m_callable_current_check_get_final_value();
//         m_callable_device_proper_status_get(&device_state);
//         if (device_state == M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING ||
//             device_state == M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING)
//         {
//             m_callable_instance_running_event_input(M_INSTANCE_RUNNING_CURRENT_NOTIFY_INTERRUPT, in_time_s);
//         }
//         if (device_state == M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING)
//         {
//             m_callable_manual_event_export_input(M_INSTANCE_RUNNING_CURRENT_NOTIFY, in_time_s);
//         }
//         break;

//     default:
//         break;
//     }
// }
