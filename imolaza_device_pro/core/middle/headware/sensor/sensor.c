

#include "sensor.h"
// #include <stdio.h>
// #include <stdint.h>

// const char *M_SENSOR_FLAG = "Sensor";
const char *SENSOR_BASE = "Sensor";
#define SENSOR_BASE "Sensor"
#define SENSOR_1 "1"
#define SENSOR_IS_EFF_1 "is_eff_1"
#define SENSOR_2 "2"
#define SENSOR_IS_EFF_2 "is_eff_2"

#define SENSOR_CHECK__WATER_TYPE_INTERVAL "water_inv"
#define SENSOR_CHECK__FLOW_TYPE_INTERVAL "flow_inv"

#define SENSOR_TYPE_RAIN "Rain"
#define SENSOR_TYPE_FLOW_RATE "Flow_Rate"
#define SENSOR_TYPE_NOR_OPEN "Nor_Open"
#define SENSOR_TYPE_NOR_CLOSE "Nor_Close"

#define SENSOR_CHECK_DEFAULT_WATER_TIME_INTERVAL 10
#define SENSOR_CHECK_DEFAULT_FLOW_TIME_INTERVAL 10

/** Old Sensor */
#define Old_Sensor "SS0"

/** 是否需要传感器 1 动态修改 如果是的会触发两次*/
int dynamic_change_1 = -1;
/** 是否需要传感器 2 动态修改 */
int dynamic_change_2 = -1;
// 基准时间 基于设备时间的 作用是 用于判断检测时间的间隔
uint64_t ext_time = 0;
/** 触发计数总量，不一定是流量传感器 */
uint64_t flow_sensor_count_sum = 0;
/** 传感器触发计数*/
volatile int sensor_pulsecount = 0;

void m_static_sensor_type_select_m(struct m_sensor_man *sensor_stats_z);
stat_m m_static_sensor_event_handle(enum seneor_chann sensor_channel, bool is_open, enum is_effective is_eff, enum seneor_type sen_type, uint64_t num_sum, uint64_t num_curr);
/**
 * @brief 加载老设备的配置，
 *
 * @param out_sen_chid 老设备配置 传感器区域id
 * @param out_is_open 是否打开
 * @return stat_m
 */
stat_m m_static_sensor_load_old_conf(int *out_sen_chid, int *out_is_open);

/**
 * @brief 传感器初始化 加载传感器配置
 * @todo Unit Input ...
 *
 *
 */
stat_m m_callable_sensor_init(int sen1_pin, int sen2_pin)
{
    stat_m stat = fail_r;
    int type_s = 0;
    // 初始化GPIO
    memset(&sen_man, 0, sizeof(sen_man));
    sen_man.sen1_pin = sen1_pin;
    sen_man.sen2_pin = sen2_pin;
    m_ext_drive_sensor_init(sen1_pin, sen2_pin);

    // 赋值
    { /** 传感器开启关闭  */
        if (m_callable_drive_flash_read(M_TYPE_U32, SENSOR_BASE SENSOR_1, (void *)&type_s) == succ_r)
        {
            if (type_s - 10 >= BADGER_228PV15 && type_s - 10 <= K_OTHER_SENSOR)
            {
                m_callable_sensor_set(1, type_s - 10, false);
                DEBUG_TEST(DB_I, "传感器1======》 类型 :%d ", type_s - 10);
            }
            else
            {
                m_callable_sensor_set(type_s / 10, type_s % 10, false);
            }
            //  m_callable_sensor_set(type_s / 10, type_s % 10, false);
        }
        if (m_callable_drive_flash_read(M_TYPE_U32, SENSOR_BASE SENSOR_2, (void *)&type_s) == succ_r)
        {
            if (type_s - 20 >= BADGER_228PV15 && type_s - 20 <= K_OTHER_SENSOR)
            {
                m_callable_sensor_set(2, type_s - 20, false);
                DEBUG_TEST(DB_I, "传感器2======》 类型 :%d ", type_s - 20);
            }
            else
            {
                m_callable_sensor_set(type_s / 10, type_s % 10, false);
            }
            //  m_callable_sensor_set(type_s / 10, type_s % 10, false);
        }
    }
    { /** 开启之后的标记 触发log */
        if (m_callable_drive_flash_read(M_TYPE_U32, SENSOR_BASE SENSOR_IS_EFF_1, (void *)&type_s) == succ_r)
            sen_man.sen_1_is_effective = type_s;
        if (m_callable_drive_flash_read(M_TYPE_U32, SENSOR_BASE SENSOR_IS_EFF_2, (void *)&type_s) == succ_r)
            sen_man.sen_2_is_effective = type_s;
    }
    { // 加载时间 间隔
        if (m_callable_drive_flash_read(M_TYPE_U32, SENSOR_BASE SENSOR_CHECK__WATER_TYPE_INTERVAL, (void *)&type_s) == succ_r)
            sen_man.sen_water_check_time_interval = type_s;
        else
            sen_man.sen_water_check_time_interval = SENSOR_CHECK_DEFAULT_WATER_TIME_INTERVAL;
        if (m_callable_drive_flash_read(M_TYPE_U32, SENSOR_BASE SENSOR_CHECK__FLOW_TYPE_INTERVAL, (void *)&type_s) == succ_r)
            sen_man.sen_flow_check_time_interval = type_s;
        else
            sen_man.sen_flow_check_time_interval = SENSOR_CHECK_DEFAULT_FLOW_TIME_INTERVAL;
    }
    /** 加载老设备的配置， */
    m_static_sensor_load_old_conf(NULL, NULL);
    m_callable_rain_gauge_damage_detection_data_cleared(sen_man.sen_1_is_opened, sen_man.sen1_type, 1, sen_man.sen_1_is_effective);
    m_callable_rain_gauge_damage_detection_data_cleared(sen_man.sen_2_is_opened, sen_man.sen2_type, 2, sen_man.sen_2_is_effective);
    m_callable_early_warning_data_init();
    sen_man.sen_1_trigger_count = 0;
    sen_man.sen_2_trigger_count = 0;
    // ext_time = 0;

    return stat;
}
void m_static_sensor_trig_log(enum seneor_chann sensor_channel, enum is_effective is_eff)
{
    if (sensor_channel == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1)
        m_callable_drive_flash_write(M_TYPE_U32, SENSOR_BASE SENSOR_IS_EFF_1, (void *)&is_eff);
    else if (sensor_channel == M_SENSOR_CHANNEL_MANAGE_CHANNEL_2)
        m_callable_drive_flash_write(M_TYPE_U32, SENSOR_BASE SENSOR_IS_EFF_2, (void *)&is_eff);
}

/**
 * @brief 以特定的格式设置传感器，开关和类型
 * @todo Unit Input ...
 * @param channel
 * @param sensor_type
 * @param boo
 */

stat_m m_callable_sensor_set(int tp_vue, enum sensor_type_m sensor_type, bool boo)
{
    stat_m stat = fail_r;
    ext_time = m_callable_timer_manage_get_device_time();
    flow_sensor_count_sum = 0;
    sen_man.sen_1_trigger_count = 0;
    sen_man.sen_2_trigger_count = 0;
    switch (tp_vue)
    {
        /** 开启传感器 雨1*/ // 常闭
    case 1:
        stat = succ_r;
        if (sensor_type == 0)
        {
            m_callable_sensor_set_close(M_SENSOR_CHANNEL_MANAGE_CHANNEL_1);

            DEBUG_TEST(DB_W, "close sensor 1 ");
        }
        else if (sensor_type == 1)
        {
            m_callable_sensor_set_open(M_SENSOR_CHANNEL_MANAGE_CHANNEL_1, M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH);

            DEBUG_TEST(DB_W, "use sensor 1 Sensoer_Rain  ( Normal Connect )");
        }
        else if (sensor_type == 2)
        {
            m_callable_sensor_set_open(M_SENSOR_CHANNEL_MANAGE_CHANNEL_1, M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH);
            DEBUG_TEST(DB_W, "use sensor 1 Sensoer_Rain  ( Normal DisConnect )");
        }
        else if (sensor_type > 2)
        {
            m_callable_sensor_set_open(M_SENSOR_CHANNEL_MANAGE_CHANNEL_1, M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE);
            DEBUG_TEST(DB_W, "use sensor 1 Sensoer_FLOW start ");
            // printf("*******************%d**************\r\n", sensor_type);
            // 设置传感器类型为 BADGER_228PV15
            sen_man.sensor_type = sensor_type;

            // sen_man.sen_1_is_opened = true;
            // 调用函数来设置 k_value 和 offset_value
            m_static_sensor_type_select_m(&sen_man);
        }

        break;
        /** 开启传感器 流1*/
    case 2:
        stat = succ_r;
        if (sensor_type == 0)
        {
            m_callable_sensor_set_close(M_SENSOR_CHANNEL_MANAGE_CHANNEL_2);
            DEBUG_TEST(DB_W, "close sensor 2 ");
        }
        else if (sensor_type == 1)
        {
            m_callable_sensor_set_open(M_SENSOR_CHANNEL_MANAGE_CHANNEL_2, M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH);
            DEBUG_TEST(DB_W, "use sensor 2 Sensoer_Rain  ( Normal Connect )");
        }
        else if (sensor_type == 2)
        {
            m_callable_sensor_set_open(M_SENSOR_CHANNEL_MANAGE_CHANNEL_2, M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH);
            DEBUG_TEST(DB_W, "use sensor 2 Sensoer_Rain  ( Normal DisConnect )");
        }
        else if (sensor_type > 2)
        {
            m_callable_sensor_set_open(M_SENSOR_CHANNEL_MANAGE_CHANNEL_2, M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE);
            DEBUG_TEST(DB_W, "use sensor 2 Sensoer_FLOW start ");
            // 设置传感器类型为 BADGER_228PV15
            sen_man.sensor_type = sensor_type;

            // sen_man.sen_1_is_opened = true;

            // 调用函数来设置 k_value 和 offset_value
            m_static_sensor_type_select_m(&sen_man);
        }
        break;
    default:
        break;
    }
    if (stat == succ_r && boo)
    {

        if (tp_vue == 2)
        {
            if (sensor_type == 0)
                m_callable_drive_flash_remove(SENSOR_BASE SENSOR_2 /* SENSOR_TYPE_RAIN SENSOR_TYPE_NOR_CLOSE */),
                    m_callable_drive_flash_remove(SENSOR_BASE SENSOR_IS_EFF_2 /* SENSOR_TYPE_RAIN SENSOR_TYPE_NOR_CLOSE */);
            else
            {
                tp_vue *= 10;
                tp_vue += sensor_type;
                m_callable_drive_flash_write(M_TYPE_U32, SENSOR_BASE SENSOR_2 /* SENSOR_TYPE_RAIN SENSOR_TYPE_NOR_CLOSE */, (void *)&tp_vue);
            }
        }
        else if (tp_vue == 1)
        {
            if (sensor_type == 0)
                m_callable_drive_flash_remove(SENSOR_BASE SENSOR_1 /* SENSOR_TYPE_RAIN SENSOR_TYPE_NOR_CLOSE */),
                    m_callable_drive_flash_remove(SENSOR_BASE SENSOR_IS_EFF_1 /* SENSOR_TYPE_RAIN SENSOR_TYPE_NOR_CLOSE */);
            else
            {
                tp_vue *= 10;
                tp_vue += sensor_type;
                m_callable_drive_flash_write(M_TYPE_U32, SENSOR_BASE SENSOR_1 /* SENSOR_TYPE_RAIN SENSOR_TYPE_NOR_CLOSE */, (void *)&tp_vue);
            }
        }
        // m_callable_drive_flash_write(M_TYPE_Int, M_SENSOR_FLAG, (void *)&tp_vue);
    }

    m_callable_flow_sensor_type_gets_m();
    return 0;
}
/**
 * @brief 计算当前运行区域的中断次数
 *
 * @param flag  反映当前爱你区域的因较编号，可以根据缓存查询到对应的通道id
 * @return stat_m
 */
stat_m m_callable_sensor_interrupt_count(int flag)
{
    stat_m stat = fail_r;
    if (flag == sen_man.sen1_pin)
        sen_man.sen_1_trigger_count++;
    else if (flag == sen_man.sen2_pin)
        sen_man.sen_2_trigger_count++;
    return stat;
}

volatile bool rising_edge_detected = false;
void m_callable_sensor_flow_value_interrupt(uint8_t channel)
{
    if ((channel == 1 && sen_man.sen1_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE) || (channel == 2 && sen_man.sen2_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE))
    {

        if (rising_edge_detected)
        {
            sensor_pulsecount++; // 每次下降沿后的上升沿触发中断时，脉冲计数加一
            m_callable_pulse_recording();
            m_callable_flow_sensor_meter_pulse_counting();
            rising_edge_detected = false;
        }
        else
        {
            rising_edge_detected = true;
        }
    }
}
void m_static_sensor_status_change_trig_again(bool *sen__is_opened, int *dynamic_change, enum is_effective *sen__is_effective)
{
    if (*dynamic_change != -1 && *dynamic_change != *sen__is_opened)
    {
        *sen__is_opened = *dynamic_change;
        *sen__is_effective = M_SENSOR_IS_EFFECTIVE_NONE;
        *dynamic_change = -1;
    }
}
/**
 * @brief 设置传感器检测时间间隔
 *
 * @param in_water_type_time 雨量传感器时间间隔
 * @param in_flow_type_time 流量传感器时间间隔
 * @return stat_m
 */
stat_m m_callable_sensor_set_check_interval_time(uint32_t in_water_type_time, uint32_t in_flow_type_time)
{
    stat_m stat = fail_r;

    if (in_water_type_time != 0)
    {
        sen_man.sen_water_check_time_interval = in_water_type_time;
        stat = m_callable_drive_flash_write(M_TYPE_U32, SENSOR_BASE SENSOR_CHECK__WATER_TYPE_INTERVAL, (void *)&sen_man.sen_water_check_time_interval);
    }
    if (in_flow_type_time != 0)
    {
        sen_man.sen_flow_check_time_interval = in_flow_type_time;
        stat = m_callable_drive_flash_write(M_TYPE_U32, SENSOR_BASE SENSOR_CHECK__FLOW_TYPE_INTERVAL, (void *)&sen_man.sen_flow_check_time_interval);
    }

    return stat;
}

/**
 * @brief 以指定类型开启传感器
 * @todo Unit Input ...
 *
 * @param channel
 * @param type
 */
stat_m m_callable_sensor_set_open(uint8_t channel, uint8_t type)
{
    stat_m stat = fail_r;
    m_callable_rain_gauge_damage_detection_data_cleared(true, type, channel + 1, -1);
    DEBUG_TEST(DB_I, "1");
    if (channel == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1)
    {
        // 梅开 并且
        DEBUG_TEST(DB_I, "2");
        if (!sen_man.sen_1_is_opened || sen_man.sen1_type != type)
        {
            DEBUG_TEST(DB_I, "3");
            if (!sen_man.sen_1_is_opened && type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
                sen_man.sen_1_is_effective = M_SENSOR_IS_EFFECTIVE_NOT_TRIG;
            else
                sen_man.sen_1_is_effective = M_SENSOR_IS_EFFECTIVE_NONE;
            sen_man.sen_1_is_opened = true;
        }
        sen_man.sen1_type = type;
        m_callable_rain_gauge_damage_detection_data_cleared(true, type, channel + 1, sen_man.sen_1_is_effective);
    }
    else
    {
        if (!sen_man.sen_2_is_opened || sen_man.sen2_type != type)
        {

            if (!sen_man.sen_2_is_opened && type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
                sen_man.sen_2_is_effective = M_SENSOR_IS_EFFECTIVE_NOT_TRIG;
            else
                sen_man.sen_2_is_effective = M_SENSOR_IS_EFFECTIVE_NONE;
            sen_man.sen_2_is_opened = true;
        }
        sen_man.sen2_type = type;
        m_callable_rain_gauge_damage_detection_data_cleared(true, type, channel + 1, sen_man.sen_2_is_effective);
    }

    // m_callable_device_attribute_config_change_notify_event_callable(
    //     M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SENSOR_OPEN_SKIP,
    //     (void *)&channel,
    //     m_callable_timer_manage_get_device_time());
    return stat;
}
/**
 * @brief 关闭传感器
 * @todo Unit Input ...
 *
 * @param channel
 * @param type
 */
stat_m m_callable_sensor_set_close(uint8_t channel)
{
    stat_m stat = fail_r;
    m_callable_rain_gauge_damage_detection_data_cleared(false, -1, channel + 1, -1);
    if (channel == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1)
    {
        // DEBUG_TEST( DB_I,"77");
        // sen_man.sen1_type = M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH;
        if (sen_man.sen_1_is_opened)
        {
            // DEBUG_TEST( DB_I,"88");
            // DEBUG_TEST( DB_I,"%d-%d", sen_man.sen_1_is_effective, M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG);
            // dynamic_change_1 = false;
            if (sen_man.sen_1_is_effective == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG)
            {
                // DEBUG_TEST( DB_I,"99");
                dynamic_change_1 = false,
                sen_man.sen_1_is_effective = M_SENSOR_IS_EFFECTIVE_NONE;
            }
            else
                sen_man.sen_1_is_opened = false;
        }
        // sen_man.sen_1_is_opened = false;
        // if (sen_man.sen_1_is_effective == M_SENSOR_IS_EFFECTIVE_NOT_TRIG)
        m_callable_rain_gauge_damage_detection_data_cleared(false, -1, channel + 1, sen_man.sen_1_is_effective);
    }
    else
    {
        if (sen_man.sen_2_is_opened)
        {
            // DEBUG_TEST( DB_I,"%d-%d", sen_man.sen_1_is_effective, M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG);
            if (sen_man.sen_2_is_effective == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG)
            {
                dynamic_change_2 = false;
                sen_man.sen_2_is_effective = M_SENSOR_IS_EFFECTIVE_NONE;
            }
            else
                sen_man.sen_2_is_opened = false;
        }
        // if (sen_man.sen_2_is_effective == M_SENSOR_IS_EFFECTIVE_NOT_TRIG)
        m_callable_rain_gauge_damage_detection_data_cleared(false, -1, channel + 1, sen_man.sen_2_is_effective);
    }
    // m_callable_device_attribute_config_change_notify_event_callable(
    //     M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SENSOR_CLOSE_SKIP,
    //     (void *)&channel,
    //     m_callable_timer_manage_get_device_time());

    return stat;
}

/**
 * @brief 统一的传感器检测
 *
 * @param sen_trigger_count 检测到的次数
 * @param should_sen_trigger_count  应该检测到的次数
 * @param sen_chan 传感器通道
 * @param sen__is_effective 近来之前是否被触发
 * @param sen__is_opened 近来之前是否已经打开
 * @param sen_type 传感器的类型
 */
void m_static_sensor_check_pro_max(uint64_t sen_trigger_count, uint64_t should_sen_trigger_count,
                                   enum seneor_chann sen_chan, enum is_effective *sen__is_effective, bool *sen__is_opened, enum seneor_type sen_type)
{
    int dy_change = 0;

    // should_sen_trigger_count /= 2;
    // if ((sen_trigger_count >= 50 && sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH) /*== should_sen_trigger_count*/ // 有
    // ){
    if (
        // 完全接上 + 常开
        (sen_trigger_count + 5 >= should_sen_trigger_count && sen_trigger_count > 0 && sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) ||
        // 没有接上 + 常闭
        (sen_trigger_count + 5 >= should_sen_trigger_count && sen_trigger_count > 0 && sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH))
    {
        if ((sen_man.sen_1_is_opened || sen_man.sen_2_is_opened))
        {
            DEBUG_TEST(DB_I, "Have ->(%d) Check %lld ,Total %lld  - %d",
                       sen_chan + 1, sen_trigger_count, should_sen_trigger_count, *sen__is_effective);
        }
        /**     错误    有事件  1 */ // 满了的话 对于常开来说 是触发 下雨了
        // 满了的话(没检测到) 对于常闭来说 是触发 雨开始 && 避免重复
        if (sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH && (*sen__is_effective == M_SENSOR_IS_EFFECTIVE_NOT_TRIG ||
                                                                       *sen__is_effective == M_SENSOR_IS_EFFECTIVE_NONE))
        {
            *sen__is_effective = M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG;
            m_static_sensor_trig_log(sen_chan, *sen__is_effective);
            if ((dynamic_change_1 != -1 && sen_chan == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1) || (dynamic_change_2 != -1 && sen_chan == M_SENSOR_CHANNEL_MANAGE_CHANNEL_2))
                m_static_sensor_event_handle(sen_chan, *sen__is_opened, M_SENSOR_IS_EFFECTIVE_NOT_TRIG, sen_type, 0, 0);
            else
                m_static_sensor_event_handle(sen_chan, *sen__is_opened, *sen__is_effective, sen_type, 0, 0);
            // 状态修复再次触发
            if (dynamic_change_1 != -1 || dynamic_change_2 != -1)
            {
                if (sen_chan == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1)
                    dy_change = dynamic_change_1,
                    dynamic_change_1 = -1;
                else
                    dy_change = dynamic_change_2,
                    dynamic_change_2 = -1;

                m_static_sensor_status_change_trig_again(sen__is_opened, &dy_change, sen__is_effective);
            }
        }
        // 满了的话(没检测到) 对于常开来说 是断开 雨停止
        if ((sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH) && (*sen__is_effective == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG ||
                                                                            *sen__is_effective == M_SENSOR_IS_EFFECTIVE_NONE))
        {
            DEBUG_TEST(DB_W, " lp l lp l pl");
            *sen__is_effective = M_SENSOR_IS_EFFECTIVE_NOT_TRIG;
            m_static_sensor_trig_log(sen_chan, *sen__is_effective);
            // if (dynamic_change_1 != -1 || dynamic_change_2 != -1)
            if ((dynamic_change_1 != -1 && sen_chan == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1) || (dynamic_change_2 != -1 && sen_chan == M_SENSOR_CHANNEL_MANAGE_CHANNEL_2))
                m_static_sensor_event_handle(sen_chan, *sen__is_opened, M_SENSOR_IS_EFFECTIVE_NOT_TRIG, sen_type, 0, 0);
            else
                m_static_sensor_event_handle(sen_chan, *sen__is_opened, *sen__is_effective, sen_type, 0, 0);
            // 状态修复再次触发
            if (dynamic_change_1 != -1 || dynamic_change_2 != -1)
            {
                if (sen_chan == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1)
                    dy_change = dynamic_change_1,
                    dynamic_change_1 = -1;
                else
                    dy_change = dynamic_change_2,
                    dynamic_change_2 = -1;
                m_static_sensor_status_change_trig_again(sen__is_opened, &dy_change, sen__is_effective);
            }
        }
    }
    else
    {
        if (sen_man.sen_1_is_opened || sen_man.sen_2_is_opened)
            DEBUG_TEST(DB_I, "None ->(%d) Check %lld ,Total %lld    - %d", sen_chan + 1, sen_trigger_count, should_sen_trigger_count, *sen__is_effective);
        /**  有事件  1 */ // 没满的话 对于常开来说 是断开 雨停了    && 避免重复

        // 无满的话(有检测) 对于常闭来说 是断开 雨停止
        if ((sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH && (*sen__is_effective == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG ||
                                                                        *sen__is_effective == M_SENSOR_IS_EFFECTIVE_NONE)) ||
            sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
        {
            *sen__is_effective = M_SENSOR_IS_EFFECTIVE_NOT_TRIG;
            if (sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
                sen_type = M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH;
            m_static_sensor_trig_log(sen_chan, *sen__is_effective);
            m_static_sensor_event_handle(sen_chan, *sen__is_opened, *sen__is_effective, sen_type, 0, 0);
            // 状态修复再次触发
            if (dynamic_change_1 != -1 || dynamic_change_2 != -1)
            {
                if (sen_chan == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1)
                    dy_change = dynamic_change_1;
                else
                    dy_change = dynamic_change_2;
                m_static_sensor_status_change_trig_again(sen__is_opened, &dy_change, sen__is_effective);
            }
        }
        // 没满的话 对于常闭来说 是触发 下雨了    && 避免重复
        // 无满了的话(有检测) 对于常开来说 是触发 雨开始
        if (sen_type == M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH && (*sen__is_effective == M_SENSOR_IS_EFFECTIVE_NOT_TRIG ||
                                                                          *sen__is_effective == M_SENSOR_IS_EFFECTIVE_NONE))
        {
            *sen__is_effective = M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG;
            m_static_sensor_trig_log(sen_chan, *sen__is_effective);
            m_static_sensor_event_handle(sen_chan, *sen__is_opened, *sen__is_effective, sen_type, 0, 0);
            // 状态修复再次触发
            if (dynamic_change_1 != -1 || dynamic_change_2 != -1)
            {
                if (sen_chan == M_SENSOR_CHANNEL_MANAGE_CHANNEL_1)
                    dy_change = dynamic_change_1;
                else
                    dy_change = dynamic_change_2;
                m_static_sensor_status_change_trig_again(sen__is_opened, &dy_change, sen__is_effective);
            }
        }
    }
}

// 记录当前的时间
uint64_t in_time_s = 0;
/** 流量传感器 计数 */
uint64_t flow_sensor_count = 0;

int m_out_ste = 0;

/** 传感器的值 float类型 */
float sensor_flow_value = 0.0;

// uint64_t flow_time_10 = 0;
// bool flow_time_10_flag = true;
/**
 * @brief 传感器监控，需要循环调用 以保证在传感器主动动作后 设备能在指定的某个合适的时间内发现并且触发对应的事件
 *      不管是 流量还是 雨量
 *          触发方法应该都是   在一定时间内的触发次数
 * @todo Unit Input ...
 *
 * @param current_time
 */
stat_m m_callable_sensor_monitor(uint64_t current_time)
{
    stat_m stat = fail_r;
    in_time_s = m_callable_timer_manage_get_device_time();
    // m_callable_device_proper_status_get(&m_out_ste);
    m_callable_early_warning_feedback_monitor(in_time_s);
    m_callable_whether_current_time_range(in_time_s);

    // if (flow_time_10_flag)
    // {
    //     flow_time_10_flag = false;
    //     flow_time_10 = in_time_s;
    // }
    // if (in_time_s >= flow_time_10 + 2)
    // {
    //     flow_time_10_flag = true;
    //     DEBUG_E("sensor_pulsecount %d", sensor_pulsecount);
    //     sensor_pulsecount = 0;
    // }

    if (m_ext_sensor_get_interface_status(sen_man.sen1_pin, sen_man.sen1_type) == succ_r)
        m_callable_sensor_interrupt_count(sen_man.sen1_pin);
    if (m_ext_sensor_get_interface_status(sen_man.sen2_pin, sen_man.sen2_type) == succ_r)
        m_callable_sensor_interrupt_count(sen_man.sen2_pin);

    // if (current_time > ext_time + 100) // 隔一段时间 检测
    // DEBUG_TEST( DB_I,"%d  %d", sen_man.sen_1_is_opened , sen_man.sen_2_is_opened);
    // DEBUG_TEST( DB_I,"%lld  %lld", in_time_s, ext_time);
    if (m_ext_network_get_device_is_recover_schedule() == succ_r && in_time_s > ext_time + sen_man.sen_water_check_time_interval)
    {
        if (sen_man.sen2_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE && sen_man.sen_2_is_effective != M_SENSOR_IS_EFFECTIVE_NONE)
        {
        }
        else
        {
            m_static_sensor_check_pro_max(
                sen_man.sen_2_trigger_count,
                flow_sensor_count_sum,
                M_SENSOR_CHANNEL_MANAGE_CHANNEL_2,
                &sen_man.sen_2_is_effective,
                &sen_man.sen_2_is_opened,
                sen_man.sen2_type);
            sen_man.sen_2_trigger_count = 0;
            // if ((sen_man.sen2_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE  && !sen_man.sen_2_is_opened )||
            // (sen_man.sen2_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE &&  !sen_man.sen_2_is_opened)
            // )
            // {
            //     sen_man.sen2_type = M_SENSOR_MANAGE_TYPE_NORMAL_NONE;
            //     sen_man.sen1_type = M_SENSOR_MANAGE_TYPE_NORMAL_NONE;
            // }
        }
        if (sen_man.sen1_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE && sen_man.sen_1_is_effective != M_SENSOR_IS_EFFECTIVE_NONE)
        {

            // 隔一段时间 检测
        }
        else
        {

            m_static_sensor_check_pro_max(
                sen_man.sen_1_trigger_count,
                flow_sensor_count_sum,
                M_SENSOR_CHANNEL_MANAGE_CHANNEL_1,
                &sen_man.sen_1_is_effective,
                &sen_man.sen_1_is_opened,
                sen_man.sen1_type);
            sen_man.sen_1_trigger_count = 0;
            // printf("    %d  %d"  ,sen_man.sen2_type , sen_man.sen_2_is_opened );
        }
        ext_time = in_time_s;
        stat = succ_r;
    }

    // ||
    //  ((sen_man.sen1_type != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE) || (sen_man.sen2_type != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE))))
    // (sen_man.sen1_type != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE && sen_man.sen2_type != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE))

    // 流量计 一直在触发  (  取消一直触发 )
    /*
        if ((in_time_s > ext_time + sen_man.sen_flow_check_time_interval) &&
            (sen_man.sen1_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE || sen_man.sen2_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE))
        {
            // m_callable_exclusive_lock_enter();
            sensor_pulsecount /= 10;
            if (sensor_pulsecount > 3)
            {
                sensor_pulsecount -= 3;
                //   float frequency = count; // 频率 = 脉冲数 / 时间（秒）
                sen_man.sen_1_trigger_count = sen_man.sen_2_trigger_count = ((uint64_t)((sensor_pulsecount + sen_man.offset_value) * sen_man.k_value) * 100);
            }
            else
            {
                sen_man.sen_1_trigger_count = sen_man.sen_2_trigger_count = 0;
            }
            // m_callable_exclusive_lock_exit();

            if (sen_man.sen_1_is_opened && sen_man.sen1_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
            {
                // DEBUG_TEST( DB_I,"1 flow --> %lld", sen_man.sen_1_trigger_count);
                DEBUG_TEST( DB_I,"1 flow -->(cnt %d) %lld", sensor_pulsecount, sen_man.sen_1_trigger_count);
                sen_man.sen_1_is_effective = true;
                m_static_sensor_event_handle(M_SENSOR_CHANNEL_MANAGE_CHANNEL_1, sen_man.sen_1_is_opened,
                                             true, M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE,
                                             flow_sensor_count_sum, sen_man.sen_1_trigger_count);
                sen_man.sen_1_trigger_count = 0;
            }
            if (sen_man.sen_2_is_opened && sen_man.sen2_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
            {
                // DEBUG_TEST( DB_I,"2 flow --> %lld", sen_man.sen_2_trigger_count);
                DEBUG_TEST( DB_I,"2 flow -->(cnt %d) %lld", sensor_pulsecount, sen_man.sen_2_trigger_count);
                sen_man.sen_2_is_effective = true;
                m_static_sensor_event_handle(M_SENSOR_CHANNEL_MANAGE_CHANNEL_2, sen_man.sen_2_is_opened,
                                             true, M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE,
                                             flow_sensor_count_sum, sen_man.sen_1_trigger_count);
                sen_man.sen_2_trigger_count = 0;
            }
            sensor_pulsecount = 0;
            sensor_flow_value = 0;
            ext_time = in_time_s;
            stat = succ_r;
        }
            */
    flow_sensor_count_sum++;
    if (stat == succ_r)
        flow_sensor_count_sum = 0;
    m_callable_sensor_staging_clean(M_CONST_NONE_NUM, in_time_s);
    return stat;
}
/**
 * @brief 检测当前是否处于传感器触发
 *
 * @param sen_chan 如果触发，参数返回的是触发的通道
 * @param sen_type 如果触发，参数返回的是触发的传感器类型
 * @param current_time 当前的时间
 * @return stat_m fail_r 表示没有触发 否则否则
 */
stat_m m_callable_sensor_current_is_effective(enum seneor_chann *sen_chan, enum seneor_type *sen_type, uint64_t current_time)
{
    stat_m stat = succ_r;

    if (sen_man.sen_1_is_opened && sen_man.sen1_type != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
    {
        if (sen_man.sen_1_is_effective == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG)
        {
            if (sen_chan != NULL)
            {
                *sen_chan = M_SENSOR_CHANNEL_MANAGE_CHANNEL_1;
                *sen_chan += 1;
                *sen_type = sen_man.sen1_type;
            }
            stat = fail_r;
        }
    }
    if (sen_man.sen_2_is_opened && sen_man.sen2_type != M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE)
    {
        if (sen_man.sen_2_is_effective == M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG)
        {
            if (sen_chan != NULL)
            {
                *sen_chan = M_SENSOR_CHANNEL_MANAGE_CHANNEL_2;
                *sen_chan += 1;
                *sen_type = sen_man.sen2_type;
            }
            stat = fail_r;
        }
    }
    if ((sen_man.sen_1_is_opened == false) && (sen_man.sen_2_is_opened == false))
    {
        stat = M_ERR;
    }

    return stat;
}

/**
 * @brief 获取自上次到现在的流量值，并且指定是否清除缓存并且重新开始计算，如果流量传感器未打开 返回0
 *
 * @param is_clear 是否清除缓存
 * @return stat_m
 */
stat_m m_callable_flow_sensor_get_total_value(uint8_t channel_id, uint32_t running_time, float *out_final_vue, bool is_clear)
{
    stat_m stat = fail_r;
    float flow_value = 0.0;
    DEBUG_TEST(DB_I, "*******  channel_id :%hhd  running_time :%d ", channel_id, running_time);
    /*避免 在漏水检测中累计的脉冲数在这里 没清空*/
    if (running_time == 0)
    {
        sensor_pulsecount = 0;
    }
    if ((sen_man.sen_1_is_opened && sen_man.sen1_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE) || (sen_man.sen_2_is_opened && sen_man.sen2_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE))
    {
        stat = succ_r;

        if ((sen_man.sensor_type >= HC_075_FLOW_B && sen_man.sensor_type <= HC_200_FLOW_B) || (sen_man.sensor_type == P_OTHER_SENSOR))
        {
            m_callable_zone_transfore_get(channel_id, running_time, &flow_value);
            *out_final_vue = flow_value;
            DEBUG_TEST(DB_I, "1 or 2 flow -=> %.03f", *out_final_vue);
        }
        else
        {
            if (sensor_pulsecount > 0)
            {
                if (running_time != 0)
                {
                    float result_pul = m_static_divideInt_by_uint32(sensor_pulsecount, running_time);
                    float roundedresult_1 = m_static_round_threedecimal(result_pul);
                    float result_time = m_static_divideInt_by_uint32(running_time, 60);
                    float roundedresult_2 = m_static_round_threedecimal(result_time);
                    // DEBUG_E("roundedresult %.03f",roundedresult_1);
                    // DEBUG_E("roundedresult %.03f",roundedresult_2);
                    *out_final_vue = (roundedresult_1 + sen_man.offset_value) * sen_man.k_value * roundedresult_2;
                }
                else
                {
                    *out_final_vue = 0;
                }
            }
            else
                *out_final_vue = 0.0f;
            DEBUG_TEST(DB_I, "1 or 2 flow -->(cnt %d) %.03f", sensor_pulsecount, *out_final_vue);
            // m_callable_local_resp_to_remote(M_CMD__NUMBER_OF_TRAFFIC_DETECTION_PULSES,
            //                                 M_TYPE_Int, (void *)&sensor_pulsecount,
            //                                 M_TYPE_Int, (void *)&running_time,
            //                                 M_TYPE_NULL, NULL,
            //                                 M_TYPE_NULL, NULL, in_time_s, true);
            if (is_clear)
            {
                sensor_pulsecount = 0;
            }
        }
    }
    else
    {
        *out_final_vue = -1.0f;
        sensor_pulsecount = 0;
    }
    return stat;
}

// stat_m m_callable_flow_sensor_get_total_value(float *out_final_vue, bool is_clear)
// {
//     stat_m stat = fail_r;
//     float flow_value = 0.0;
//     if ((sen_man.sen_1_is_opened && sen_man.sen1_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE) || (sen_man.sen_2_is_opened && sen_man.sen2_type == M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE))
//     {
//         stat = succ_r;

//         if (sensor_pulsecount > 0)
//         {
//             //   float frequency = count; // 频率 = 脉冲数 / 时间（秒）
//             *out_final_vue = (((sensor_pulsecount + sen_man.offset_value) * sen_man.k_value)) / 60;
//         }
//         else
//             *out_final_vue = 0.0f;
//         DEBUG_TEST( DB_I,"1 or 2 flow -->(cnt %d) %.02f", sensor_pulsecount, *out_final_vue);

//         if (is_clear)
//             sensor_pulsecount = 0;
//     }
//     else
//     {
//         *out_final_vue = -1.0f;
//         sensor_pulsecount = 0;
//     }
//     return stat;
// }

/**
 * @brief 监控如果触发，会调用事件处理函数，你可以在里面做一些你想做的操作
 *
 * @param sensor_channel 触发的通道
 * @param is_open 通道是否被设置打开
 * @param is_eff 操作是断开还是触发
 * @param sen_type 触发的传感器类造型
 * @param num_sum 总的检测次数
 * @param num_curr 检测次数内被触发的次数
 * @return stat_m
 */
stat_m m_static_sensor_event_handle(enum seneor_chann sensor_channel, bool is_open,
                                    enum is_effective is_eff, enum seneor_type sen_type, uint64_t num_sum, uint64_t num_curr)
{
    stat_m stat = fail_r;
    num_sum /= 10;
    /** 没有开启触发了 */
    if (!is_open)
    {
        //       触发的通道 ， 触发的传感器类型   是触发还是断开   触发总数  触发数量
        m_ext_sensor_not_open_event_handle(sensor_channel, sen_type, is_eff, num_sum, num_curr);
    }
    else
    /** 已经开启后触发 */
    {
        m_ext_sensor_opened_event_handle(sensor_channel, sen_type, is_eff, num_sum, num_curr);
    }
    return stat;
}

//****************************************************************************************************//

/**
 * @brief 流量计类型获取
 * @param senseortype 类型
 * @return stat_m
 */
stat_m m_callable_flow_sensor_type_gets(uint8_t *senseortype)
{
    stat_m stat = fail_r;
    if (sen_man.sensor_type >= BADGER_228PV15)
    {

        *senseortype = sen_man.sensor_type;
        stat = succ_r;
    }
    return stat;
}

/**
 * @brief 流量计参数设置
 * @param sensor_stats_z 传感器参数
 * @return void
 */
void m_static_sensor_type_select_m(struct m_sensor_man *sensor_stats_z)
{
    switch (sensor_stats_z->sensor_type)
    {
    case BADGER_228PV15:
        sensor_stats_z->k_value = BADGER_228PV15_K_VALUE;
        sensor_stats_z->offset_value = BADGER_228PV15_OFFSET_VALUE;

        break;
    case BADGER_228PV20:
        sensor_stats_z->k_value = BADGER_228PV20_K_VALUE;
        sensor_stats_z->offset_value = BADGER_228PV20_OFFSET_VALUE;

        break;
    case BADGER_228PV30:
        sensor_stats_z->k_value = BADGER_228PV30_K_VALUE;
        sensor_stats_z->offset_value = BADGER_228PV30_OFFSET_VALUE;

        break;
    case BADGER_228PV40:
        sensor_stats_z->k_value = BADGER_228PV40_K_VALUE;
        sensor_stats_z->offset_value = BADGER_228PV40_OFFSET_VALUE;

        break;
    case TORO_TFS_050:
        sensor_stats_z->k_value = TORO_TFS_050_K_VALUE;
        sensor_stats_z->offset_value = TORO_TFS_050_OFFSET_VALUE;
        break;
    case TORO_TFS_075:
        sensor_stats_z->k_value = TORO_TFS_075_K_VALUE;
        sensor_stats_z->offset_value = TORO_TFS_075_OFFSET_VALUE;
        break;
    case TORO_TFS_100:
        sensor_stats_z->k_value = TORO_TFS_100_K_VALUE;
        sensor_stats_z->offset_value = TORO_TFS_100_OFFSET_VALUE;
        break;
    case TORO_TFS_150:
        sensor_stats_z->k_value = TORO_TFS_150_K_VALUE;
        sensor_stats_z->offset_value = TORO_TFS_150_OFFSET_VALUE;
        break;
    case TORO_TFS_200:
        sensor_stats_z->k_value = TORO_TFS_200_K_VALUE;
        sensor_stats_z->offset_value = TORO_TFS_200_OFFSET_VALUE;
        break;

    case TORO_TFS_300:
        sensor_stats_z->k_value = TORO_TFS_300_K_VALUE;
        sensor_stats_z->offset_value = TORO_TFS_300_OFFSET_VALUE;
        break;

    case TORO_TFS_400:
        sensor_stats_z->k_value = TORO_TFS_400_K_VALUE;
        sensor_stats_z->offset_value = TORO_TFS_400_OFFSET_VALUE;
        break;

    case CST_FSI_T10:
        sensor_stats_z->k_value = CST_FSI_T10_K_VALUE;
        sensor_stats_z->offset_value = CST_FSI_T10_OFFSET_VALUE;
        break;

    case CST_FSI_T15:
        sensor_stats_z->k_value = CST_FSI_T15_K_VALUE;
        sensor_stats_z->offset_value = CST_FSI_T15_OFFSET_VALUE;
        break;

    case CST_FSI_T20:
        sensor_stats_z->k_value = CST_FSI_T20_K_VALUE;
        sensor_stats_z->offset_value = CST_FSI_T20_OFFSET_VALUE;
        break;

    case EVERYBODY_1004:
        sensor_stats_z->k_value = EVERYBODY_1004_K_VALUE;
        sensor_stats_z->offset_value = EVERYBODY_1004_OFFSET_VALUE;
        break;
    case FLOMEC_QS200_10_SCH40:
        sensor_stats_z->k_value = FLOMEC_QS200_10_SCH40_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_10_SCH40_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_10_SCH80:
        sensor_stats_z->k_value = FLOMEC_QS200_10_SCH80_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_10_SCH80_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_15_SCH40:
        sensor_stats_z->k_value = FLOMEC_QS200_15_SCH40_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_15_SCH40_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_15_SCH80:
        sensor_stats_z->k_value = FLOMEC_QS200_15_SCH80_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_15_SCH80_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_20_SCH40:
        sensor_stats_z->k_value = FLOMEC_QS200_20_SCH40_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_20_SCH40_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_20_SCH80:
        sensor_stats_z->k_value = FLOMEC_QS200_20_SCH80_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_20_SCH80_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_30_SCH40:
        sensor_stats_z->k_value = FLOMEC_QS200_30_SCH40_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_30_SCH40_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_30_SCH80:
        sensor_stats_z->k_value = FLOMEC_QS200_30_SCH80_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_30_SCH80_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_40_SCH40:
        sensor_stats_z->k_value = FLOMEC_QS200_40_SCH40_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_40_SCH40_OFFSET_VALUE;
        break;

    case FLOMEC_QS200_40_SCH80:
        sensor_stats_z->k_value = FLOMEC_QS200_40_SCH80_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS200_40_SCH80_OFFSET_VALUE;
        break;

    case FLOMEC_QS100_SCH40:
        sensor_stats_z->k_value = FLOMEC_QS100_SCH40_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS100_SCH40_OFFSET_VALUE;
        break;

    case FLOMEC_QS100_SCH80:
        sensor_stats_z->k_value = FLOMEC_QS100_SCH80_K_VALUE;
        sensor_stats_z->offset_value = FLOMEC_QS100_SCH80_OFFSET_VALUE;
        break;

    case HC_075_FLOW_B:
        sensor_stats_z->k_value = 0.264;

        /*flash 存储读取还未添加，用这个先做模拟值*/
        for (int i = 0; i < 34; i++)
        {
            sensor_stats_z->s_pulse_time[i] = 0;
            sensor_stats_z->flow_speed[i] = 0.000;
        }

        break;

    case HC_100_FLOW_B:
    case HC_150_FLOW_B:
    case HC_200_FLOW_B:
        sensor_stats_z->k_value = 2.642;

        /*flash 存储读取还未添加，用这个先做模拟值*/
        for (int i = 0; i < 34; i++)
        {
            sensor_stats_z->s_pulse_time[i] = 0;
            sensor_stats_z->flow_speed[i] = 0.000;
        }
        break;

    case FG100:
        sensor_stats_z->k_value = FG100_K_VALUE;
        sensor_stats_z->offset_value = FG100_OFFSET_VALUE;
        break;

    case FS350B:
        sensor_stats_z->k_value = FS350B_K_VALUE;
        sensor_stats_z->offset_value = FS350B_OFFSET_VALUE;
        break;

    case P_OTHER_SENSOR:
    case K_OTHER_SENSOR:
        sensor_stats_z->k_value = sen_man.k_value;
        sensor_stats_z->offset_value = sen_man.offset_value;
        break;

    default:

        break;
    }
}

/**
 * @brief 流量计自定义值 设置
 * @param k_value K值或者是 升/脉冲
 * @param offset_value 偏移值
 * @return stat_m
 */
stat_m m_calllable_sensor_k_or_f_set(float k_value, float offset_value)
{
    sen_man.k_value = k_value;
    sen_man.offset_value = offset_value;
    return succ_r;
}

/**
 * @brief 加载老设备的配置，
 *
 * @param out_sen_chid 老设备配置 传感器区域id
 * @param out_is_open 是否打开
 * @return stat_m
 */
stat_m m_static_sensor_load_old_conf(int *out_sen_chid, int *out_is_open)
{
    stat_m stat = fail_r;
    uint8_t out_ve = 0;
    if (m_callable_drive_flash_read(M_TYPE_U8, Old_Sensor, &out_ve) == succ_r)
    {
        DEBUG_TEST(DB_I, "m_static_sensor_load_old_conf -> %d", out_ve);
        if (out_ve % 10 == 1)
        {
            stat = succ_r;
            if (out_sen_chid != NULL)
                *out_sen_chid = out_ve / 10;
            if (out_is_open != NULL)
                *out_is_open = 1;
            m_callable_drive_flash_remove(Old_Sensor);
            m_callable_sensor_set(out_ve / 10, 1, true);
        }
    }
    return stat;
}
