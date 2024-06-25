#include "core/fset_lib.h"

#include "../application/include.h"

/**
 * @brief
 *

 * @param sensor_channel 触发的通道
 * @param sen_type 触发的传感器类造型
 * @param is_eff 操作是断开还是触发
 * @param num_sum 总的检测次数
 * @param num_curr 检测次数内被触发的次数
 * @return stat_m
 */
stat_m m_ext_sensor_opened_event_handle(enum seneor_chann sensor_channel, enum seneor_type sen_type, enum is_effective is_eff, uint64_t num_sum, uint64_t num_curr)
{
    stat_m stat = succ_r;
    float temp_float_a = 0.0f;
    sensor_channel += 1;
    int cmd = 0, in_value = sensor_channel * 100 + sen_type;
    DEBUG_TEST(DB_W, "%lld", num_curr);
    float flow_result = num_curr / 100;
    DEBUG_TEST(DB_W, "%.02f", flow_result);
    uint64_t in_time_s = m_callable_timer_manage_get_device_time();

    if (sen_type != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
    {
        if (sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH)
        {
            DEBUG_TEST(DB_I, "常闭");
        }
        else
        {
            DEBUG_TEST(DB_I, "常开");
        }
        if (is_eff == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG)
        {
            m_callable_rain_gauge_damage_detection_data_cleared(true, sen_type, sensor_channel, is_eff);

            DEBUG_TEST(DB_I, "Opened Sensor Trig %d", sensor_channel);
            cmd = M_CMD_TWOWAY_SENSOR_TRIGE;
             m_run_time.sensor_mode = M_CMD_TWOWAY_SENSOR_TRIGE;
            stat = m_callable_device_attribute_config_change_notify_event_callable(
                M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SENSOR_OPEN_SKIP,
                (void *)&in_value,
                m_callable_timer_manage_get_device_time());
        }
        else if (is_eff == M_SENSOR_IS_EFFECTIVE_NOT_TRIG)
        {
            m_callable_rain_gauge_damage_detection_data_cleared(true, sen_type, sensor_channel, is_eff);

            DEBUG_TEST(DB_I, "Opened Sensor Dis %d", sensor_channel);
            stat = m_callable_sensor_staging_distrig_handle(in_value);

            // m_callable_device_attribute_config_change_notify_event_callable(
            //     M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SENSOR_CLOSE_SKIP,
            //     (void *)&(in_value),
            //     m_callable_timer_manage_get_device_time());
            cmd = M_CMD_TWOWAY_SENSOR_LEAVE;
            m_run_time.sensor_mode = M_CMD_TWOWAY_SENSOR_LEAVE;
        }

        else if (is_eff == M_SENSOR_IS_EFFECTIVE_NONE)
            DEBUG_TEST(DB_I, "Opened Sensor Swit %d", sensor_channel);

        if (stat == fail_r)
        {

            m_callable_flow_sensor_get_total_value(0, 0, &temp_float_a, true);
            m_callable_local_resp_to_remote_pro_max(cmd,
                                                    M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                    M_TYPE_Int, (void *)&sen_type,
                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,

                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM,

                                                    M_TYPE_Int, (void *)&M_CONST_NONE_NUM, // 五计划跳过
                                                    M_TYPE_Int, (void *)&sensor_channel,

                                                    (cmd == M_CMD_TWOWAY_SENSOR_TRIGE) ? M_TYPE_Int : M_TYPE_NULL,
                                                    (cmd == M_CMD_TWOWAY_SENSOR_TRIGE) ? (void *)&M_CONST_NONE_NUM : NULL, // 流
                                                    M_TYPE_Float, (void *)&temp_float_a,                                   // 流量
                                                    M_TYPE_NULL, NULL,
                                                    M_TYPE_NULL, NULL,
                                                    in_time_s);
        }
    }
    else
    {

        cmd = M_CMD_NOTIFY_TO_SERVER_FLOW_SENSOR_FEEDBACK;

        m_callable_local_resp_to_remote(cmd,
                                        M_TYPE_Int, (void *)&sensor_channel,
                                        M_TYPE_Float, (void *)&flow_result,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL, in_time_s, true);
    }
    return succ_r;
}
/**
 * @brief
 *

 * @param sensor_channel 触发的通道
 * @param sen_type 触发的传感器类造型
 * @param is_eff 操作是断开还是触发
 * @param num_sum 总的检测次数
 * @param num_curr 检测次数内被触发的次数
 * @return stat_m
 */
stat_m m_ext_sensor_not_open_event_handle(enum seneor_chann sensor_channel, enum seneor_type sen_type, enum is_effective is_eff, uint64_t num_sum, uint64_t num_curr)
{
    int cmd = 0;
    sensor_channel += 1;

    if (sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH)
    {
        DEBUG_TEST(DB_I, "常闭");
    }
    else
    {
        DEBUG_TEST(DB_I, "常开");
    }

    if (is_eff == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG)
    {
        DEBUG_TEST(DB_I, "Not Open Sensor Trig %d", sensor_channel);
        /*传入-1 ，相当于关闭传感器的损坏或者漏水检测*/
        cmd = M_CMD_NOTIFY_TO_SERVER_SENSOR_CONNECT;
    }
    else if (is_eff == M_SENSOR_IS_EFFECTIVE_NOT_TRIG)
    {
        DEBUG_TEST(DB_I, "Not Open Sensor Dis %d", sensor_channel); // M_CMD_NOTIFY_TO_SERVER_SENSOR_CONNECT

        cmd = M_CMD_NOTIFY_TO_SERVER_SENSOR_DIS;
    }

    else if (is_eff == M_SENSOR_IS_EFFECTIVE_NONE)
        DEBUG_TEST(DB_I, "Not Open Sensor Swit %d", sensor_channel);

    m_callable_rain_gauge_damage_detection_data_cleared(false, sen_type, sensor_channel, is_eff);

    m_callable_local_resp_to_remote(cmd,
                                    M_TYPE_Int, (void *)&sensor_channel,
                                    M_TYPE_Int, (void *)&sen_type,
                                    M_TYPE_NULL, NULL,
                                    M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time(), false);
    return succ_r;
}

stat_m m_ext_sensor_get_interface_status(int pin, enum seneor_type type)
{
    stat_m stat = fail_r;
    // if (type == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH || type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
    // {
    //     if (m_ext_drive_gpio_get_level(pin) == 0)
    //         stat = succ_r;
    // }
    // else if (type == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH)
    if (m_ext_drive_gpio_get_level(pin) == 1)
        stat = succ_r;
    // DEBUG_TEST(DB_W,"%d", m_ext_drive_gpio_get_level(pin));
    return stat;
}
