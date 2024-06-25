#include "core/fset_lib.h"

// #include "../application/include.h"

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
stat_m m_ext_sensor_opened_event_handle(enum seneor_chann sensor_channel, enum seneor_type sen_type, bool is_eff, uint64_t num_sum, uint64_t num_curr)
{
    if (is_eff)
    {
        DEBUG_TEST( DB_I,"Opened Sensor Trig %d", sensor_channel + 1);
    }
    else
    {
        DEBUG_TEST( DB_I,"Opened Sensor Dis %d", sensor_channel + 1);
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
stat_m m_ext_sensor_not_open_event_handle(enum seneor_chann sensor_channel, enum seneor_type sen_type, bool is_eff, uint64_t num_sum, uint64_t num_curr)
{
    if (is_eff)
    {
        DEBUG_TEST( DB_I,"未开传感器触发 %d", sensor_channel + 1);
    }
    else
    {
        DEBUG_TEST( DB_I,"未开传感器断开 %d", sensor_channel + 1);
    }

    return succ_r;
}

stat_m m_ext_sensor_get_interface_status(int pin, enum seneor_type type)
{
    stat_m stat = fail_r;
    if (type == M_SENSOR_MANAGE_TYPE_NORMALLY_OPEN_SWITCH)
    {
        if (m_ext_drive_gpio_get_level(pin) == 0)
            stat = succ_r;
    }
    else if (type == M_SENSOR_MANAGE_TYPE_NORMALLY_CLOSED_SWITCH || type == M_SENSOR_MANAGE_TYPE_NORMALLY_FLOW_RATE)
        if (m_ext_drive_gpio_get_level(pin) == 1)
            stat = succ_r;
    return stat;
}
stat_m m_ext_drive_sensor_init(int s1, int s2){}