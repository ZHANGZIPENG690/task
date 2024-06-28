#include "sensor_flow.h"
#include <stdint.h>
#include <string.h>
// #include "math.h"

uint64_t s_pulse_1_timestamp = 0;
uint64_t s_pulse_2_timestamp = 0;
uint64_t s_pulse_1_timestamp_minu = 0;
uint64_t s_pulse_2_timestamp_minu = 0;
uint8_t current_channel = 0;
uint8_t previous_channel = 0;
bool pulse_monitor = false;

// // 自定义的四舍五入函数
// double custom_round(double value, int decimal_places)
// {
//     double factor = pow(10, decimal_places);
//     return round(value * factor) / factor;
// }

/**
 * @brief 获取当前运行区域的脉冲周期
 * @param zone_stop_time 当前区域本次运行停止时间
 * @param current_time 当前时间
 * @param clear 是否重置数据标志位
 * @return uint64_t
 */
uint16_t m_static_pulse_period_detection_monitor(uint64_t current_time, uint8_t channel_id, bool clear)
{
    uint16_t result_value = 0;
    if (clear == true)
    {
        flow_man.s_pulese_detection = 1;
        if (flow_man.s_pulse_1_num == 1)
        {
            flow_man.s_pulse_1_num = 2;
            s_pulse_1_timestamp = current_time;
            DEBUG_TEST(DB_E, "s_pulse_1_timestamp : %lld", s_pulse_1_timestamp);
        }
        if (flow_man.s_pulse_2_num == 2 && s_pulse_1_timestamp != 0)
        {
            flow_man.s_pulse_2_num = 3;
            s_pulse_2_timestamp = current_time;
            DEBUG_TEST(DB_E, "s_pulse_2_timestamp : %lld", s_pulse_2_timestamp);
        }
        if (s_pulse_2_timestamp != 0 && s_pulse_1_timestamp != 0)
        {
            flow_man.s_pulese_detection = 0;
            DEBUG_TEST(DB_E, "s_pulse_2_timestamp-s_pulse_1_timestamp : %d", (uint16_t)(s_pulse_2_timestamp - s_pulse_1_timestamp));
            result_value = (uint16_t)(s_pulse_2_timestamp - s_pulse_1_timestamp); // 单位 s  秒
            if (result_value != sen_man.s_pulse_time[channel_id])
            {
                /*更新流速  此时默认值可能发送改变，不是 10*/
                sen_man.s_pulse_time[channel_id] = result_value;
            }
            s_pulse_1_timestamp = 0;
            s_pulse_2_timestamp = 0;
        }
        else
        {
            /*返回当前区域脉冲周期----流速*/
            result_value = sen_man.s_pulse_time[channel_id];
        }
    }
    else
    {
        /*重置*/
        flow_man.s_pulese_detection = 0;
        flow_man.s_pulse_1_num = 0;
        flow_man.s_pulse_2_num = 0;
        s_pulse_1_timestamp = 0;
        s_pulse_2_timestamp = 0;
        /*返回当前区域脉冲周期----流速*/
        result_value = sen_man.s_pulse_time[channel_id];
    }
    return result_value;
}

/**
 * @brief 获取当前运行区域的脉冲周期
 * @param zone_stop_time 当前区域本次运行停止时间
 * @param current_time 当前时间
 * @param clear 是否重置数据标志位
 * @return uint64_t
 */
uint16_t m_static_pulse_period_detection_monitor_minu(uint64_t current_time, uint8_t channel_id, bool clear)
{
    uint16_t result_value = 0;
    if (clear == true)
    {
        flow_man.s_pulese_detection_minu = 1;
        if (flow_man.s_pulse_1_num_minu == 1)
        {
            flow_man.s_pulse_1_num_minu = 2;
            s_pulse_1_timestamp_minu = current_time;
            DEBUG_TEST(DB_E, "s_pulse_1_timestamp_minu : %lld", s_pulse_1_timestamp_minu);
        }
        if (flow_man.s_pulse_2_num_minu == 2 && s_pulse_1_timestamp_minu != 0)
        {
            flow_man.s_pulse_2_num_minu = 3;
            s_pulse_2_timestamp_minu = current_time;
            DEBUG_TEST(DB_E, "s_pulse_2_timestamp_minu : %lld", s_pulse_2_timestamp_minu);
        }
        if (s_pulse_1_timestamp_minu != 0 && s_pulse_2_timestamp_minu != 0)
        {
            flow_man.s_pulese_detection_minu = 0;
            DEBUG_TEST(DB_E, "s_pulse_2_timestamp_minu-s_pulse_1_timestamp_minu : %d", (uint16_t)(s_pulse_2_timestamp_minu - s_pulse_1_timestamp_minu));
            result_value = (uint16_t)(s_pulse_2_timestamp_minu - s_pulse_1_timestamp_minu); // 单位 s  秒
            if (result_value != sen_man.s_pulse_time[channel_id])
            {
                /*更新流速  此时默认值可能发送改变，不是 10*/
                sen_man.s_pulse_time[channel_id] = result_value;
            }
            s_pulse_1_timestamp_minu = 0;
            s_pulse_2_timestamp_minu = 0;
        }
        else
        {
            /*返回当前区域脉冲周期----流速*/
            result_value = sen_man.s_pulse_time[channel_id];
        }
    }
    else
    {
        /*重置*/
        flow_man.s_pulese_detection_minu = 0;
        flow_man.s_pulse_1_num_minu = 0;
        flow_man.s_pulse_2_num_minu = 0;
        s_pulse_1_timestamp_minu = 0;
        s_pulse_2_timestamp_minu = 0;
        /*返回当前区域脉冲周期----流速*/
        result_value = sen_man.s_pulse_time[channel_id];
    }
    return result_value;
}

/**
 * @brief 通过获取到的脉冲周期计算出 水 流速
 * @return float
 */
float m_static_get_flow_rate(uint8_t channel)
{
    // float speed = 6.23;
    /*每120s 更新一次  流速 ------>  脉冲周期*/
    if (channel >= 0)
    {
        DEBUG_TEST(DB_E, "sen_man.k_value(加仑/脉冲): %.03f   sen_man.s_pulse_time(脉冲周期): %d", sen_man.k_value, sen_man.s_pulse_time[channel]);

        /*如果sen_man.s_pulse_time[channel] != 0  表示此时的流速 是基于设备检测到的两个脉冲来计算出的脉冲周期，则更新流速并以此值作为流速 */
        if (sen_man.s_pulse_time[channel] != 0)
        {
            if (sen_man.flow_speed[channel] != m_static_round_threedecimal(m_static_divideInt_by_uint16(sen_man.k_value, sen_man.s_pulse_time[channel])))
            {

                sen_man.flow_speed[channel] = m_static_round_threedecimal(m_static_divideInt_by_uint16(sen_man.k_value, sen_man.s_pulse_time[channel]));
                // sen_man.flow_speed[channel] = sen_man.k_value / sen_man.s_pulse_time[channel];
                DEBUG_E("******************sen_man.flow_speed[%d]:%f", channel, sen_man.flow_speed[channel]);
                m_static_flow_rate_stored_value_set(channel);
            }
        }
        else
        {
            if (channel != 33 && (channel != 34)) // 区域号 33 是个特殊值、用来漏水检测时的计算
            {
                // sen_man.flow_speed[channel] = speed / 60.0;
                sen_man.flow_speed[channel] = 0.103;
                // DEBUG_E("*****************sen_man.flow_speed[channel] :%f*******", sen_man.flow_speed[channel]);
            }
            else
            {
                sen_man.flow_speed[channel] = 0.0;
            }
            // /*如果sen_man.s_pulse_time[channel] == 0  表示 设备此时并没有检测到两个脉冲 来计算流速，则用原始设置的作为流速*/
            // if (sen_man.flow_speed[channel] == 0 && sen_man.sensor_type >= HC_100_FLOW_B)
            // {
            //     /*如果 sen_man.flow_speed[channel] == 0  说明初始化时 获取到的该区域流速初始值为0，防止后续反馈的流量值为的0的情况，则默认给个 一般值*/
            //     /* 官方手册数据、该类型的Hunter流量计参数是   10L/pulse   然后正常一般运行120s有一个脉冲，则 一般流速为 0.022 GPS */
            //     // sen_man.flow_speed[channel] = 0.022;

            //     // 6.23加仑/分

            //     sen_man.flow_speed[channel] = 6.23 / 60.0;
            // }
            // else if (sen_man.flow_speed[channel] == 0 && sen_man.sensor_type < HC_100_FLOW_B)
            // {

            //     /* 官方手册数据、该类型的Hunter流量计参数是   1L/pulse   然后正常一般运行120s有一个脉冲，则 一般流速为 0.0022 GPS */
            //     // sen_man.flow_speed[channel] = 0.0022;
            //     sen_man.flow_speed[channel] = 6.23 / 60.0;
            // }
        }

        return sen_man.flow_speed[channel];
    }
    else
    {
        DEBUG_TEST(DB_E, "The regional run started for the first time");
        return 0.0;
    }
}

/**
 * @brief 通过计算出的流速以及提供的当前区域运行时间  算出  水 流量
 * @param zone_should_runtime 当前区域本次运行时间
 * @return float
 */
float m_static_get_flow_value(uint64_t zone_should_runtime, bool flag)
{
    float result = 0.0;
    if (flag == true)
    {
        result = m_static_get_flow_rate(previous_channel);
    }
    else
    {
        result = m_static_get_flow_rate(current_channel);
    }

    DEBUG_TEST(DB_E, "本次运行的流量值： %.03f 加仑", result * zone_should_runtime);
    return result * zone_should_runtime;
}

/**
 * @brief 用来去中断 获取 脉冲
 * @return statm
 */
stat_m m_callable_pulse_recording(void)
{
    if ((sen_man.sensor_type >= HC_075_FLOW_B && sen_man.sensor_type <= HC_200_FLOW_B) || (sen_man.sensor_type == P_OTHER_SENSOR))
    {
        if (flow_man.s_pulese_detection == 1)
        {
            flow_man.s_pulese_detection = 0;
            flow_man.s_pulse_1_num++;
            flow_man.s_pulse_2_num++;
        }
        if (flow_man.s_pulese_detection_minu == 1)
        {
            flow_man.s_pulese_detection_minu = 0;
            flow_man.s_pulse_1_num_minu++;
            flow_man.s_pulse_2_num_minu++;
        }
    }
    return succ_r;
}

/**
 * @brief 在外部 检测函数中调用, 检测 相邻且是同一个区域的 两个脉冲对应的时间戳  计算出脉冲周期，从而算出 流速
 * @param currenttime 当前时间
 * @return statm
 */
stat_m m_callable_whether_current_time_range(uint64_t currenttime)
{
    int device_status = -1;
    m_callable_device_proper_status_get(&device_status);
    if (device_status == M_DEVICE_GLOBAL_STATUS_IDLE)
    {
        sen_man.s_pulse_time[34] = m_static_pulse_period_detection_monitor_minu(currenttime, 34, true);
        sen_man.s_pulse_time[33] = m_static_pulse_period_detection_monitor(currenttime, 33, true);
    }
    else
    {
        sen_man.s_pulse_time[current_channel] = m_static_pulse_period_detection_monitor(currenttime, current_channel, true);
    }

    return succ_r;
}

/**
 * @brief 在外部 检测函数中调用, 检测 相邻且是同一个区域的 两个脉冲对应的时间戳  计算出脉冲周期，从而算出 流速
 * @param channel_id 下一个将要运行的区域id
 * @param running_time 当前运行区域的   运行时间
 * @param out_final_vue 获取到的流量值
 * @param in_times 当前时间
 * @return statm
 */
stat_m m_callable_zone_transfore_get(uint8_t channel_id, uint32_t running_time, float *out_final_vue)
{

    DEBUG_TEST(DB_E, "?????????????????????????????????当前区域 : %hhd", channel_id);

    stat_m stat = succ_r;
    /*通过 channel_id 与 全局缓存的 当前运行区域id比较  是否一致*/
    /*第一次进来，一定是true*/
    if (current_channel == 0)
    {
        current_channel = channel_id;
    }

    /*如果不同，则检测数据重置、开始新的检测*/
    if (current_channel != channel_id)
    {
        DEBUG_TEST(DB_E, "下一个准备运行区域ID为:%d  当前区域区域运行时长为%d ", channel_id, running_time);
        previous_channel = current_channel;
        current_channel = channel_id;
        *out_final_vue = m_static_get_flow_value(running_time, true);
    }
    else if (current_channel == channel_id)
    {
        DEBUG_TEST(DB_E, "当前区域ID为:%d  当前区域运行时长为%d ", channel_id, running_time);
        *out_final_vue = m_static_get_flow_value(running_time, false);
    }
    m_static_pulse_period_detection_monitor(0, channel_id, false);
    /*如果一致，状态不变 继续检测*/

    /*在每次区域切换的时候，可以更新一次所有区域的流速，避免在区域运行的时候，将该区域流速进行更新改变*/
    m_static_flow_rate_stored_value_get(channel_id);

    return stat;
}

/**
 * @brief 计算得到的各个区域流速值  存储传递  向外部传递
 * @param channel_id 需要存储的 该区域  流速
 * @return float
 */
stat_m m_static_flow_rate_stored_value_set(uint8_t channel_id)
{
    stat_m stat = fail_r;

    DEBUG_TEST(DB_E, "存储--------》");
    DEBUG_TEST(DB_E, "区域id :%d    该区域流速为：%.03f", channel_id, sen_man.flow_speed[channel_id]);

    if (sen_man.flow_speed[channel_id] != 0)
    {
        m_callable_channel_flow_speed_set(channel_id, sen_man.flow_speed[channel_id] * 60.0);
        stat = succ_r;
    }
    return stat;
}

/**
 * @brief 向外部存储的地方获取  各个区域的流速值
 * @param channel_id 需要存储的 该区域  流速
 * @return uint16_t
 */
stat_m m_static_flow_rate_stored_value_get(uint8_t channel_id)
{

    m_callable_channel_flow_speed_get(channel_id, &sen_man.flow_speed[channel_id]);
    DEBUG_TEST(DB_E, "获取--------》");
    DEBUG_TEST(DB_E, "区域id :%d    该区域流速为：%.03f", channel_id, sen_man.flow_speed[channel_id]);
    return succ_r;
}

/**
 * @brief 通过计算出的流速以及提供的 漏水检测周期算出 漏水值
 * @param detection_cycle 当前区域本次运行时间
 * @return float
 */
float m_static_get_leakage_value(uint64_t detection_cycle, uint8_t number)
{
    if (sen_man.s_pulse_time[number] != 0)
    {
        sen_man.flow_speed[number] = sen_man.k_value / sen_man.s_pulse_time[number];
    }
    // DEBUG_TEST(DB_E," 漏水速度： %.03f         漏水流量值： %.03f 加仑", sen_man.flow_speed[number], sen_man.flow_speed[number] * detection_cycle);
    return sen_man.flow_speed[number] * detection_cycle;
}

/*用来控制 分钟漏水检测  长期不重置  导致一直获取不到结果(一般情况下是每分钟可以获取到流量)*/
int minu_count = 0;
/**
 * @brief 在外部 检测函数中调用, 检测 相邻且是同一个区域的 两个脉冲对应的时间戳  计算出脉冲周期，从而算出 流速
 * @param detection_cycle 当前漏水检测周期
 * @param out_final_vue 获取到的流量值
 * @param in_times 当前时间
 * @param time_interval 间隔小时1   间隔分钟2
 * @param flag 是否清除数据
 * @return statm
 */
stat_m m_callable_leakage_velocity_detection(uint32_t detection_cycle, float *out_final_vue, uint64_t in_times, uint8_t time_interval, bool flag)
{
    stat_m stat = succ_r;
    if (flag == true)
    {
        // if (detection_cycle != 0)
        // {
        //     DEBUG_TEST(DB_E,"漏水检测周期为 %d ", detection_cycle);
        // }

        if (time_interval == 1)
        {
            *out_final_vue = m_static_get_leakage_value(detection_cycle, 33);
            // DEBUG_TEST(DB_E,"间隔小时周期单位漏水量  : %lf", *out_final_vue);
            m_static_pulse_period_detection_monitor(in_times, 33, false);
            sen_man.s_pulse_time[33] = 0.0;
            sen_man.flow_speed[33] = 0.0;
        }
        else if (time_interval == 2)
        {
            *out_final_vue = m_static_get_leakage_value(detection_cycle, 34);
            // m_static_pulse_period_detection_monitor_minu(in_times, 34, false);
            minu_count++;
            if (minu_count >= 3) // 这里的数值3  可调节
            {
                m_static_pulse_period_detection_monitor_minu(in_times, 34, false);
            }
            if (*out_final_vue != 0)
            {
                // DEBUG_TEST(DB_E,"间隔分钟周期单位漏水量  : %lf", *out_final_vue);
                m_static_pulse_period_detection_monitor_minu(in_times, 34, false);
                minu_count = 0;
            }
            sen_man.s_pulse_time[34] = 0.0;
            sen_man.flow_speed[34] = 0.0;
        }
    }
    else
    {
        if (time_interval == 1)
        {
            sen_man.s_pulse_time[33] = 0.0;
            sen_man.flow_speed[33] = 0.0;
        }
        else if (time_interval == 2)
        {
            sen_man.s_pulse_time[34] = 0.0;
            sen_man.flow_speed[34] = 0.0;
        }
    }

    return stat;
}

/* 未处理的潜在问题
1.停水了没脉冲  和 有水(水流小)没检测到脉冲 如何区分
2.浇水中途断电，数据丢失情况  (记录到的脉冲 时间戳)
3.重置恢复出厂设置，调用默认流速值，每个区域统一 一个值  该值待确定
*/

// //*******************************************************************//
// /**
//  * @brief 存储每个区域的各自流速
//  * @return stat_m
//  */
// stat_m m_static_flow_pulse_time_save(void)
// {
//     stat_m stat = fail_r;
//     uint16_t data[32] = {1, 2, 3, 4, 5, 6, 7, 8,
//                          9, 10, 11, 12, 13, 14, 15, 16,
//                          17, 18, 19, 20, 21, 22, 23, 24,
//                          25, 26, 27, 28, 29, 30, 31, 32};
//     uint16_t data_get[32] = {0};

//     stat = m_callable_drive_flash_write(M_TYPE_Array, M_SENSOR_FLOW_PULSE_ERIOD, data);
//     mDelay_ms(2000);
//     stat = m_callable_drive_flash_read(M_TYPE_Array, M_SENSOR_FLOW_PULSE_ERIOD, data_get);
//     for (int i = 0; i < 32; i++)
//     {
//         DEBUG_TEST(DB_E,"**%d**", data_get[i]);
//     }

//     return stat;
// }
