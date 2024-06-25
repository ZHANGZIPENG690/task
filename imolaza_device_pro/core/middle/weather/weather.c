

#include "weather.h"

uint64_t weather_info[31][2] = {0};

uint64_t time_weather = 0;
/**
 * @brief 天气相关初始化
 *
 * @return stat_m
 */
stat_m m_callable_weather_manage_init(void)
{
    stat_m stat = fail_r;

    return stat;
}

/**
 * @brief 设置一个带id的天气信息
 *
 * @param in_uid id
 * @param weath_info 对应的天气信息
 * @return stat_m
 */
stat_m m_callable_weather_manage_set(uint64_t in_uid, int weath_info, uint64_t in_time_s)
{
    DEBUG_TEST(DB_W,"m_callable_weather_manage_set %d" , weath_info);
    stat_m stat = fail_r;
    if (in_uid > 0 && weath_info > 0)
    {
        stat = succ_r;
        for (int i = 0; i < 31; i++)
            if (weather_info[i][0] == in_uid || weather_info[i][0] == 0)
            {
                weather_info[i][0] = in_uid, weather_info[i][1] = weath_info;
                m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_WEATHER_SKIP,
                                                                                (void *)&in_uid, in_time_s);
                break;
            }
    }
    return stat;
}

/** 45 min 请求计数 */
stat_m m_callable_weather_manage_monitor(uint64_t in_time_s)
{
    stat_m stat = fail_r;
    if (in_time_s > time_weather + 60 * 45 + 15)
    {
        // 防止第一次误判
        time_weather = in_time_s;
    }
    else if (in_time_s > time_weather + 60 * 45)
    {
        // 时间到了
        time_weather = in_time_s;
        stat = succ_r;
    }
    return stat;
}
/**
 * @brief 清除天气缓存信息
 *
 * @param in_id
 * @return stat_m
 */
stat_m m_callable_weather_manage_clean(uint64_t in_id)
{
    stat_m stat = succ_r;
    for (int i = 30; i >= 0; i--)
    {
        if (weather_info[i][0] == in_id)
        {
            weather_info[i][0] = 0;
            weather_info[i][1] = 0;
        }
    }
    return stat;
}
/**
 * @brief 设置一个计划的天气信息
 *
 * @param in_id 计划id
 * @param weather_code 表征天气信息的代码
 * @return stat_m
 */
stat_m m_callable_weather_manage_get_is_effective(uint64_t in_id, int *weather_code)
{
    stat_m stat = succ_r;
    // printf("%lld ---  " , in_id);
    for (int i = 30; i >= 0; i--)
    {
        // printf("%lld   -  %lld\n" , weather_info[i][0] ,weather_info[i][1]);
        if (weather_info[i][0] == in_id && weather_info[i][1] != 0)
        {
            stat = fail_r;
            // printf("OK k k kkk k \n");
            *weather_code = weather_info[i][1];
        }
        else if (i > 15 && weather_info[i][0] > 0)
            weather_info[i][0] = 0, weather_info[i][1] = 0 ;
        // else 
    }
    return stat;
}
