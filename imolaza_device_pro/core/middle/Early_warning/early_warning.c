#include "early_warning.h"
#include "core/middle/headware/sensor/sensor.h"

#define EARLY_WARNING_TEST 0

// #define RTC_POWER_ALARM_TIME 3600 * 24
#define RTC_POWER_ALARM_TIME 5

#if !EARLY_WARNING_TEST
int time_num = 3600;
#else
int time_num = 1;
#endif

uint32_t count_minu = 0;
bool clear_flag_minu = true;
uint32_t count = 0;
bool clear_flag = true;

const char *M_SENSOR_RAIN_TIME = "rant";
const char *M_SENSOR_FLOW_TIME = "fltm";
const char *M_SENSOR_FLOW_VALUE = "fval";
const char *M_SENSOR_FLOW_VALUE_MINU = "fvam";

struct m_sensor_warning m_sen_warning;
struct m_rtc_power_warning m_rtc_warning;
/**
 * @brief 流量计 漏水检测之 脉冲计数
 * @return stat_m
 */
stat_m m_callable_flow_sensor_meter_pulse_counting(void)
{
    m_sen_warning.sensor_pulsecount_leak++;
    m_sen_warning.sensor_pulsecount_leak_minu++;

    return succ_r;
}

/**
 * @brief 流量传感器数据清空
 * @return stat_m
 */
stat_m m_callable_flow_meter_leakage_detection_data_cleared(void)
{
    m_sen_warning.flow_start_flag = true;
    m_sen_warning.sensor_flow_value_sum = 0;
    m_sen_warning.sensor_pulsecount_leak = 0;

    return succ_r;
}

/**
 * @brief 流量传感器数据清空(间隔分钟)
 * @return stat_m
 */
stat_m m_callable_flow_meter_leakage_detection_data_cleared_minu(void)
{
    m_sen_warning.flow_start_flag_minu = true;
    m_sen_warning.sensor_flow_value_sum_minu = 0;
    m_sen_warning.sensor_pulsecount_leak_minu = 0;

    return succ_r;
}

/**
 * @brief 传感器数据清空
 * @return stat_m
 */
stat_m m_callable_rain_gauge_damage_detection_data_cleared(bool sensor_open_flag, enum seneor_type sen_type_get, uint8_t channel, enum is_effective is_effective_get_sensor)
{

    if (channel == 1)
    {
        m_sen_warning.sensor1_open_flag = sensor_open_flag;
        m_sen_warning.sensor1_type_get = sen_type_get;
        m_sen_warning.is_effective_get_sensor1 = is_effective_get_sensor;
    }
    else if (channel == 2)
    {
        m_sen_warning.sensor2_open_flag = sensor_open_flag;
        m_sen_warning.sensor2_type_get = sen_type_get;
        m_sen_warning.is_effective_get_sensor2 = is_effective_get_sensor;
    }
    m_sen_warning.sensor_first_trigger_time = m_callable_timer_manage_get_device_time();
    return succ_r;
}

/**
 * @brief 设置传感器损坏检测时间
 * @param poweron_time
 * @param poweroff_time
 * @return stat_m
 */
stat_m m_callable_device_sensor_damage_detection_time_set(uint8_t poweron_time, uint8_t poweroff_time)
{
    stat_m stat = fail_r;
    uint16_t sensor_time = (poweron_time << 8) | poweroff_time;
    stat = m_callable_drive_flash_write(M_TYPE_U16, M_SENSOR_RAIN_TIME, &sensor_time);
    return stat;
}

void m_callable_device_sensor_damage_detection_time_get(uint8_t *poweron_time, uint8_t *poweroff_time)
{
    uint16_t sensor_time = 0;
    m_callable_drive_flash_read(M_TYPE_U16, M_SENSOR_RAIN_TIME, &sensor_time);
    *poweron_time = (sensor_time >> 8) & 0xFF;
    *poweroff_time = sensor_time & 0xFF;
}

/**
 * @brief 设置 流量漏水检测时间
 * @param time_set
 * @param flow_value_set
 * @return stat_m
 */
stat_m m_callable_device_flow_leakage_detection_time_set(uint8_t time_set, float flow_value_set)
{
    stat_m stat = fail_r;
    int flow = 0;
    if (time_set != 0)
    {
        m_sen_warning.flow_leakage_time = time_set;
        m_sen_warning.flow_value = flow_value_set;
        stat = m_callable_drive_flash_write(M_TYPE_U8, M_SENSOR_FLOW_TIME, &m_sen_warning.flow_leakage_time);
        flow = (int)(flow_value_set * 1000);
        stat = m_callable_drive_flash_write(M_TYPE_U32, M_SENSOR_FLOW_VALUE, &flow);
    }
    else
    {
        m_sen_warning.flow_value_minu = flow_value_set;
        flow = (int)(flow_value_set * 1000);
        stat = m_callable_drive_flash_write(M_TYPE_U32, M_SENSOR_FLOW_VALUE_MINU, &flow);
    }

    return stat;
}

stat_m m_callable_Flow_meter_leakage_detection_monitor_minutes(uint64_t current_time)
{
    stat_m stat = succ_r;
    uint8_t temp_use_int_vue_a = 0;
    char str[30];
    int device_status = -1;
    m_callable_device_proper_status_get(&device_status);
    if (device_status == M_DEVICE_GLOBAL_STATUS_IDLE)
    {
        clear_flag_minu = true;
        if (m_sen_warning.flow_start_flag_minu)
        {
            m_sen_warning.flow_sensor_first_time_minu = current_time;
            m_sen_warning.flow_start_flag_minu = false;
        }
        if (current_time >= 60 + m_sen_warning.flow_sensor_first_time_minu && m_sen_warning.flow_sensor_flag_minu)
        {
            /*如果是Hunter流量计 则*/
            if (m_sen_warning.sensor_type >= HC_075_FLOW_B && m_sen_warning.sensor_type <= HC_200_FLOW_B)
            {
                m_callable_leakage_velocity_detection(0, &m_sen_warning.sensor_flow_value_sum_minu, current_time, 2, true);
            }
            m_sen_warning.flow_sensor_first_time_minu = current_time;
            m_sen_warning.sensor_pulsecount_leak_minu = 0;
            m_sen_warning.flow_sensor_flag_minu = false;
        }
        if (current_time >= (60 + m_sen_warning.flow_sensor_first_time_minu))
        {
            /*如果是Hunter流量计 则*/
            if (m_sen_warning.sensor_type >= HC_075_FLOW_B && m_sen_warning.sensor_type <= HC_200_FLOW_B)
            {

                m_callable_leakage_velocity_detection(60, &m_sen_warning.sensor_flow_value_sum_minu, current_time, 2, true);
            }
            else /*如果是其他一般流量计*/
            {
                m_sen_warning.sensor_flow_value_sum_minu = ((m_sen_warning.sensor_pulsecount_leak_minu + sen_man.offset_value) * sen_man.k_value) / 60;
            }

            if (m_sen_warning.sensor_flow_value_sum_minu > m_sen_warning.flow_value_minu)
            {
                count_minu++;
                temp_use_int_vue_a = FLOW_LEAKAGE_MINU;
                sprintf(str, "%u|%.3f", 60, m_sen_warning.sensor_flow_value_sum_minu);
                DEBUG_TEST(DB_I, "flow str:    %s", str);
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL,
                                                M_TYPE_U8, (void *)&temp_use_int_vue_a,
                                                M_TYPE_Str, (void *)&str,
                                                M_TYPE_U32, (void *)&count_minu,
                                                M_TYPE_NULL, NULL, current_time, true);
            }
            m_callable_flow_meter_leakage_detection_data_cleared_minu();
        }
    }
    else
    {
        if (clear_flag_minu)
        {
            clear_flag_minu = false;
            m_sen_warning.flow_sensor_flag_minu = true;
            count_minu = 0;
            m_callable_flow_meter_leakage_detection_data_cleared_minu();
            m_callable_leakage_velocity_detection(0, &m_sen_warning.sensor_flow_value_sum, current_time, 2, false);
        }
    }
    return succ_r;
}

/**
 * @brief 流量传感器漏水检测以及反馈
 * @param current_time
 * @return stat_m
 */
stat_m m_callable_Flow_meter_leakage_detection_monitor(uint64_t current_time)
{
    stat_m stat = succ_r;
    uint8_t temp_use_int_vue_a = 0;

    int device_status = -1;
    char str[30];
    m_callable_device_proper_status_get(&device_status);
    /*在所有端口是关闭的情况下测量流量传感器的脉冲，如果脉冲大于0，则漏水*/
    // 这里的 1  需要换成 所有IO关闭的状态
    if (device_status == M_DEVICE_GLOBAL_STATUS_IDLE)
    {
        clear_flag = true;
        if (m_sen_warning.flow_start_flag)
        {
            m_sen_warning.flow_sensor_first_time = current_time;
            m_sen_warning.flow_start_flag = false;
        }
        if ((current_time >= 60 + m_sen_warning.flow_sensor_first_time) && m_sen_warning.flow_sensor_flag)
        {
            /*如果是Hunter流量计 则*/
            if (m_sen_warning.sensor_type >= HC_075_FLOW_B && m_sen_warning.sensor_type <= HC_200_FLOW_B)
            {
                m_callable_leakage_velocity_detection(0, &m_sen_warning.sensor_flow_value_sum, current_time, 1, true);
            }
            m_sen_warning.flow_sensor_first_time = current_time;
            m_sen_warning.sensor_pulsecount_leak = 0;
            m_sen_warning.flow_sensor_flag = false;
        }
        if (current_time >= (m_sen_warning.flow_leakage_time * time_num + m_sen_warning.flow_sensor_first_time) && m_sen_warning.flow_leakage_time != 0)
        {
            /*如果是Hunter流量计 则*/
            if (m_sen_warning.sensor_type >= HC_075_FLOW_B && m_sen_warning.sensor_type <= HC_200_FLOW_B)
            {
                m_callable_leakage_velocity_detection(m_sen_warning.flow_leakage_time * time_num, &m_sen_warning.sensor_flow_value_sum, current_time, 1, true);
            }
            else /*如果是其他一般流量计*/
            {
                m_sen_warning.sensor_flow_value_sum = ((m_sen_warning.sensor_pulsecount_leak + sen_man.offset_value) * sen_man.k_value) / 60;
            }

            if (m_sen_warning.sensor_flow_value_sum > m_sen_warning.flow_value)
            {

                count++;
                temp_use_int_vue_a = FLOW_LEAKAGE;
                sprintf(str, "%u|%.3f", m_sen_warning.flow_leakage_time, m_sen_warning.sensor_flow_value_sum);
                DEBUG_TEST(DB_I, "flow str:    %s", str);
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL,
                                                M_TYPE_U8, (void *)&temp_use_int_vue_a,
                                                M_TYPE_Str, (void *)&str,
                                                M_TYPE_U32, (void *)&count,
                                                M_TYPE_NULL, NULL, current_time, true);
            }
            m_callable_flow_meter_leakage_detection_data_cleared();
        }
    }
    else
    {
        if (clear_flag)
        {
            clear_flag = false;
            m_sen_warning.flow_sensor_flag = true;
            count = 0;
            m_callable_flow_meter_leakage_detection_data_cleared();
            m_callable_leakage_velocity_detection(0, &m_sen_warning.sensor_flow_value_sum, current_time, 1, false);
        }
    }
    return succ_r;
}

// /**
//  * @brief 预警检测反馈 everyone
//  * @return stat_m
//  */
// stat_m m_callable_early_warning_feedback_monitor(uint64_t current_time)
// {
//     stat_m stat = succ_r;
//     uint8_t temp_use_int_vue_a = 0;
//     m_sen_warning.last_trigger_time = current_time;

//     if (m_sen_warning.sensor1_open_flag == true)
//     {
//         if (m_sen_warning.sensor1_type_get != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
//         {
//             if (m_sen_warning.sensor_first_trigger_time == 0)
//             {
//                 m_sen_warning.sensor_first_trigger_time = current_time;
//             }
//             if (m_sen_warning.rain_poweron_time != 0 && (m_sen_warning.last_trigger_time >= (m_sen_warning.rain_poweron_time * 24 * 3600 + m_sen_warning.sensor_first_trigger_time)))
//             {

//                 if ((m_sen_warning.is_effective_get_sensor1 == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG && m_sen_warning.sensor1_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) || (m_sen_warning.is_effective_get_sensor1 == M_SENSOR_IS_EFFECTIVE_NOT_TRIG && m_sen_warning.sensor1_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
//                 {
//                     temp_use_int_vue_a = CURRENT_FOR_A_LONG_TIME;
//                     /*向服务器发送雨量传感器预警消息*/
//                     m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL,
//                                                     M_TYPE_U8, (void *)&temp_use_int_vue_a,
//                                                     M_TYPE_U8, (void *)&m_sen_warning.rain_poweron_time,
//                                                     M_TYPE_NULL, NULL,
//                                                     M_TYPE_NULL, NULL, current_time, true);
//                     m_sen_warning.sensor_first_trigger_time = current_time;
//                 }
//             }
//             if (m_sen_warning.rain_poweroff_time != 0 && (m_sen_warning.last_trigger_time >= (m_sen_warning.rain_poweroff_time * 24 * 3600 + m_sen_warning.sensor_first_trigger_time)))
//             {

//                 if ((m_sen_warning.is_effective_get_sensor1 == M_SENSOR_IS_EFFECTIVE_NOT_TRIG && m_sen_warning.sensor1_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) || (m_sen_warning.is_effective_get_sensor1 == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG && m_sen_warning.sensor1_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
//                 {
//                     temp_use_int_vue_a = NO_CURRENT_FOR_A_LONG_TIME;
//                     /*向服务器发送雨量传感器预警消息*/
//                     m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL,
//                                                     M_TYPE_U8, (void *)&temp_use_int_vue_a,
//                                                     M_TYPE_U8, (void *)&m_sen_warning.rain_poweroff_time,
//                                                     M_TYPE_NULL, NULL,
//                                                     M_TYPE_NULL, NULL, current_time, true);
//                     m_sen_warning.sensor_first_trigger_time = current_time;
//                 }
//             }
//         }
//         /*如果通道一是流量计 */
//         else
//         {
//             m_callable_Flow_meter_leakage_detection_monitor(current_time);
//         }
//     }

//     if (m_sen_warning.sensor2_open_flag == true)
//     {
//         if (m_sen_warning.sensor2_type_get != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
//         {
//             if (m_sen_warning.sensor_first_trigger_time == 0)
//             {
//                 m_sen_warning.sensor_first_trigger_time = current_time;
//             }
//             if (m_sen_warning.rain_poweron_time != 0 && m_sen_warning.last_trigger_time >= (m_sen_warning.rain_poweron_time * 24 * 3600 + m_sen_warning.sensor_first_trigger_time))
//             {

//                 if ((m_sen_warning.is_effective_get_sensor2 == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG && m_sen_warning.sensor2_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) || (m_sen_warning.is_effective_get_sensor2 == M_SENSOR_IS_EFFECTIVE_NOT_TRIG && m_sen_warning.sensor2_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
//                 {
//                     temp_use_int_vue_a = CURRENT_FOR_A_LONG_TIME;
//                     /*向服务器发送雨量传感器预警消息*/
//                     m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL,
//                                                     M_TYPE_U8, (void *)&temp_use_int_vue_a,
//                                                     M_TYPE_U8, (void *)&m_sen_warning.rain_poweron_time,
//                                                     M_TYPE_NULL, NULL,
//                                                     M_TYPE_NULL, NULL, current_time, true);
//                     m_sen_warning.sensor_first_trigger_time = current_time;
//                 }
//             }
//             if (m_sen_warning.rain_poweroff_time != 0 && (m_sen_warning.last_trigger_time >= (m_sen_warning.rain_poweroff_time * 24 * 3600 + m_sen_warning.sensor_first_trigger_time)) && m_sen_warning.sensor_first_trigger_time != 0)
//             {

//                 if ((m_sen_warning.is_effective_get_sensor2 == M_SENSOR_IS_EFFECTIVE_NOT_TRIG && m_sen_warning.sensor2_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) || (m_sen_warning.is_effective_get_sensor2 == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG && m_sen_warning.sensor2_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
//                 {
//                     temp_use_int_vue_a = NO_CURRENT_FOR_A_LONG_TIME;
//                     /*向服务器发送雨量传感器预警消息*/
//                     m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL,
//                                                     M_TYPE_U8, (void *)&temp_use_int_vue_a,
//                                                     M_TYPE_U8, (void *)&m_sen_warning.rain_poweroff_time,
//                                                     M_TYPE_NULL, NULL,
//                                                     M_TYPE_NULL, NULL, current_time, true);
//                     m_sen_warning.sensor_first_trigger_time = current_time;
//                 }
//             }
//         }
//         /*如果通道二是流量计 */
//         else
//         {
//             m_callable_Flow_meter_leakage_detection_monitor(current_time);
//         }
//     }

//     //****************************************************************************************************************************//

//     return stat;
// }

stat_m m_callable_rtc_power_alarm_detection(uint64_t current_time)
{
    bool flag = false;
    if (m_rtc_warning.rtc_detection_first_time <= NUM_OF_DAY_SEC_SUM)
    {
        m_rtc_warning.rtc_detection_first_time = current_time;
    }
    if (current_time >= m_rtc_warning.rtc_detection_first_time + RTC_POWER_ALARM_TIME) //
    {
        flag = true;
    }
    if (flag)
    {
        if (m_callable_rtc_power_warning() != succ_r)
        {
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_DEVICE_RTC_POWER_CHECK,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, current_time, true);
        }
        m_rtc_warning.rtc_detection_first_time = current_time;
    }
    return succ_r;
}

/**
 * @brief 预警检测反馈 everyone
 * @return stat_m
 */
stat_m m_callable_early_warning_feedback_monitor(uint64_t current_time)
{
    stat_m stat = succ_r;
    uint8_t rain_sensor_flag_status = false;
    uint8_t flow_sensor_flag_status = false;
    // uint8_t rain_sensor_triggle_status = false;
    uint8_t rain_sensor_triggle_type = 0;
    uint8_t temp_use_int_vue_a = 0;
    m_sen_warning.last_trigger_time = current_time;

    /*RTC电量预警   目前是检测周期一天一次*/
    // m_callable_rtc_power_alarm_detection(current_time);
    if (m_sen_warning.sensor1_open_flag == true)
    {
        if (m_sen_warning.sensor1_type_get != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
        {
            rain_sensor_flag_status = true;
            if ((m_sen_warning.is_effective_get_sensor1 == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG && m_sen_warning.sensor1_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) || (m_sen_warning.is_effective_get_sensor1 == M_SENSOR_IS_EFFECTIVE_NOT_TRIG && m_sen_warning.sensor1_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
            {
                // rain_sensor_triggle_status = true;
                rain_sensor_triggle_type = 1;
            }
            else if ((m_sen_warning.is_effective_get_sensor1 == M_SENSOR_IS_EFFECTIVE_NOT_TRIG && m_sen_warning.sensor1_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) || (m_sen_warning.is_effective_get_sensor1 == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG && m_sen_warning.sensor1_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
            {
                // rain_sensor_triggle_status = true;
                rain_sensor_triggle_type = 0;
            }
        }
        else
        {
            flow_sensor_flag_status = true;
        }
    }

    if (m_sen_warning.sensor2_open_flag == true)
    {
        if (m_sen_warning.sensor2_type_get != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
        {
            rain_sensor_flag_status = true;
            if ((m_sen_warning.is_effective_get_sensor2 == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG && m_sen_warning.sensor2_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) || (m_sen_warning.is_effective_get_sensor2 == M_SENSOR_IS_EFFECTIVE_NOT_TRIG && m_sen_warning.sensor2_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
            {
                // rain_sensor_triggle_status = true;
                rain_sensor_triggle_type = 1;
            }
            else if ((m_sen_warning.is_effective_get_sensor2 == M_SENSOR_IS_EFFECTIVE_NOT_TRIG && m_sen_warning.sensor2_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) || (m_sen_warning.is_effective_get_sensor2 == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG && m_sen_warning.sensor2_type_get == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
            {
                // rain_sensor_triggle_status = true;
                rain_sensor_triggle_type = 0;
            }
        }
        else
        {
            flow_sensor_flag_status = true;
        }
    }

    if (rain_sensor_flag_status == true)
    {
        if (m_sen_warning.sensor_first_trigger_time <= NUM_OF_DAY_SEC_SUM)
        {
            m_sen_warning.sensor_first_trigger_time = current_time;
        }
        if (m_sen_warning.rain_poweron_time != 0 && (m_sen_warning.last_trigger_time >= (m_sen_warning.rain_poweron_time * 24 * time_num + m_sen_warning.sensor_first_trigger_time)))
        {
            if (rain_sensor_triggle_type == 1)
            {
                temp_use_int_vue_a = CURRENT_FOR_A_LONG_TIME;
                /*向服务器发送雨量传感器预警消息*/
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL,
                                                M_TYPE_U8, (void *)&temp_use_int_vue_a,
                                                M_TYPE_U8, (void *)&m_sen_warning.rain_poweron_time,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, current_time, true);
                m_sen_warning.sensor_first_trigger_time = current_time;
            }
        }
        if (m_sen_warning.rain_poweroff_time != 0 && (m_sen_warning.last_trigger_time >= (m_sen_warning.rain_poweroff_time * 24 * time_num + m_sen_warning.sensor_first_trigger_time)))
        {
            DEBUG_TEST(DB_I, "rain_poweroff_time: %d , last_trigger_time : %lld >=  rain_poweroff_time %d * 24 * %d + %lld", m_sen_warning.rain_poweroff_time, m_sen_warning.last_trigger_time, m_sen_warning.rain_poweroff_time, time_num, m_sen_warning.sensor_first_trigger_time);

            if (rain_sensor_triggle_type == 0)
            {
                temp_use_int_vue_a = NO_CURRENT_FOR_A_LONG_TIME;
                /*向服务器发送雨量传感器预警消息*/
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL,
                                                M_TYPE_U8, (void *)&temp_use_int_vue_a,
                                                M_TYPE_U8, (void *)&m_sen_warning.rain_poweroff_time,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, current_time, true);
                m_sen_warning.sensor_first_trigger_time = current_time;
            }
        }
    }
    if (flow_sensor_flag_status == true)
    {
        if (m_sen_warning.flow_value != 0)
        {
            m_callable_Flow_meter_leakage_detection_monitor(current_time);
        }
        if (m_sen_warning.flow_value_minu != 0)
        {
            m_callable_Flow_meter_leakage_detection_monitor_minutes(current_time);
        }
    }
    return stat;
}

// /**
//  * @brief 传感器损坏以及漏水检测 是否需要打开或者关闭
//  *  @param channel
//  * @param flag
//  * @return stat_m
//  */
// stat_m m_callable_early_warning_monitor_open_or_close(bool flag)
// {
//     m_sen_warning.sensor_monitor_open_or_close = flag;
//     m_callable_drive_flash_write(M_TYPE_U8, M_SENSOR_MONITOR_OPEN_OR_CLOSE, &m_sen_warning.sensor_monitor_open_or_close);

//     return succ_r;
// }

/**
 * @brief 传感器损坏以及漏水检测 数据初始化
 * @return stat_m
 */
stat_m m_callable_flow_sensor_type_gets_m(void)
{
    return m_callable_flow_sensor_type_gets(&m_sen_warning.sensor_type);
}

stat_m m_callable_early_warning_data_init(void)
{
    int flow_value_int = -1;
    int flow_value_int_minu = -1;
    m_sen_warning.flow_value = 0;      // 30.0
    m_sen_warning.flow_value_minu = 0; // 2.64

    m_callable_device_sensor_damage_detection_time_get(&m_sen_warning.rain_poweron_time, &m_sen_warning.rain_poweroff_time);
    m_callable_drive_flash_read(M_TYPE_U8, M_SENSOR_FLOW_TIME, &m_sen_warning.flow_leakage_time);
    if (m_callable_drive_flash_read(M_TYPE_U32, M_SENSOR_FLOW_VALUE, &flow_value_int) == succ_r)
    {
        if (flow_value_int != 0)
        {
            m_sen_warning.flow_value = flow_value_int / 1000.0;
        }
    }

    if (m_callable_drive_flash_read(M_TYPE_U32, M_SENSOR_FLOW_VALUE_MINU, &flow_value_int_minu) == succ_r)
    {
        if (flow_value_int_minu != 0)
        {
            m_sen_warning.flow_value_minu = flow_value_int_minu / 1000.0;
        }
    }

    m_callable_flow_sensor_type_gets(&m_sen_warning.sensor_type);

    if (m_sen_warning.rain_poweron_time == 0)
    {
        m_sen_warning.rain_poweron_time = 15;
    }
    if (m_sen_warning.rain_poweroff_time == 0)
    {
        m_sen_warning.rain_poweroff_time = 15;
    }
    if (m_sen_warning.flow_leakage_time == 0)
    {
        m_sen_warning.flow_leakage_time = 1;
    }

    DEBUG_TEST(DB_I, "rain_poweron_time   :%d", m_sen_warning.rain_poweron_time);
    DEBUG_TEST(DB_I, "rain_poweroff_time  :%d", m_sen_warning.rain_poweroff_time);
    DEBUG_TEST(DB_I, "sensor1_open_flag :%d", m_sen_warning.sensor1_open_flag);
    DEBUG_TEST(DB_I, "sensor2_open_flag :%d", m_sen_warning.sensor2_open_flag);
    DEBUG_TEST(DB_I, "flow_leakage_time :%d", m_sen_warning.flow_leakage_time);
    DEBUG_TEST(DB_I, "is_effective_get_sensor1 :%d", m_sen_warning.is_effective_get_sensor1);
    DEBUG_TEST(DB_I, "is_effective_get_sensor2 :%d", m_sen_warning.is_effective_get_sensor2);
    DEBUG_TEST(DB_I, "m_sen_warning.sensor_type :%d", m_sen_warning.sensor_type);
    DEBUG_TEST(DB_I, "m_sen_warning.flow_value :%.02f", m_sen_warning.flow_value);
    DEBUG_TEST(DB_I, "m_sen_warning.flow_value_minu :%.02f", m_sen_warning.flow_value_minu);
    m_callable_flow_meter_leakage_detection_data_cleared();
    m_callable_flow_meter_leakage_detection_data_cleared_minu();
    m_sen_warning.flow_sensor_flag = true;
    m_sen_warning.flow_sensor_flag_minu = true;
    return succ_r;
}