

#include "skip_manage.h"
#include "../device/dormant.h"
#include "../weather/weather.h"
#include "./water_delay/water_delay.h"
#include "../headware/sensor/sensor.h"
#include "./schedule_skip/schedule_skip.h"

/**
 * @brief 跳过管理初始化，这个函数在射比初始化必须被调用 ，因为他会调用其他包括休眠计划跳过和waterdelay的数据加载
 *
 * @return stat_m
 */
stat_m m_callable_skip_manage_init(void)
{
    stat_m stat = fail_r;
    stat = m_callable_water_delay_init();
    return stat;
}

/**
 * @brief   全局查询计划是否应该跳过
 * @param in_id 计划id
 * @param skip_type
 *              包含
 *                  1、是否有休眠模式
                    2、是否有waterdelay
                    3、是否有计划跳过
                    4、是否有传感器跳过
                    5、是否有天气跳过
                    6、是否有饱和跳过
 *
 * @param sen_chan 传感器通道
 * @param sen_type 传感器类型
 * @param weather_code 天气跳过代码
 * @param in_time_s 发生的时间
 * @return stat_m
 */
stat_m m_callable_skip_manage_query_is_effective(uint64_t in_id, enum skip_manage_type *skip_type, enum seneor_chann *sen_chan, enum seneor_type *sen_type, int *weather_code, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    // enum seneor_chann sen_chan;
    // enum seneor_type sen_type;
    // int weather_code = 0;
    // 是不是 有休眠模式
    if ((stat = m_callable_device_dormant_current_is_effective(in_time_s)) == succ_r)
    {
        if ((stat = m_callable_water_delay_current_is_effective(in_time_s)) == succ_r)
        {
            if ((stat = m_callable_schedule_skip_current_is_effective(in_id, in_time_s)) == succ_r)
            {
                if ((stat = m_callable_sensor_current_is_effective(sen_chan, sen_type, in_time_s)) != fail_r)
                {
                    if ((stat = m_callable_weather_manage_get_is_effective(in_id, weather_code)) == succ_r)
                    {
                        stat = succ_r; // 全通过
                    }
                    else
                        *skip_type = M_SKIP_MANAGE_TYPE_WEATHER_SKIP;
                }
                else
                    *skip_type = M_SKIP_MANAGE_TYPE_SENSOR_SKIP;
            }
            else
                *skip_type = M_SKIP_MANAGE_TYPE_SIGN_SKIP;
        }
        else
            *skip_type = M_SKIP_MANAGE_TYPE_WATERDELAY_SKIP;
    }
    else
        *skip_type = M_SKIP_MANAGE_TYPE_DORMANT_SKIP;
    DEBUG_TEST(DB_I, "%s", toname(*skip_type));
    return stat;
}

/**
 * @brief 一次清除所有的跳过类型，在删除计划的时候可能会用到
 *
 * @param in_id 计划id
 * @param in_time_s 发生的时间
 * @return stat_m
 */
stat_m m_callable_skip_manage_clean_all(uint64_t in_id, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    stat = m_callable_device_dormant_clean();
    stat = m_callable_water_delay_clean();
    stat = m_callable_schedule_skip_clean(in_id);
    return stat;
}

/**
 * @brief 跳过的分配中心
 *                  1、是否有休眠模式     （不需要）
                    2、是否有waterdelay  （不需要）
                    3、是否有计划跳过      （不需要）
                    4、是否有传感器跳过    （不应该）
                    5、是否有天气跳过      （不需要）
                    6、是否有饱和跳过       （不需要）
                    7、智能计划(饱和)            （需要）   [30,id,1|2|3,1#10;2#2]
                    8、智能时间调整            （需要）   [30,id,1|2|3,1#10;2#2]
                    9、离线或者在线季节调整       （不需要） [XX,80]
 * @param in_uid
 * @param in_cid
 * @param sk_src
 * @return stat_m
 */
stat_m m_callable_skip_dispatch_all(uint64_t in_uid, uint8_t in_cid, char *sk_src)
{
    stat_m stat = fail_r;
    if (sk_src != NULL)
    {
    }
    return stat;
}
