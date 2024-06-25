#include "mrtc.h"

// 实现设置时间函数
stat_m m_callable_midlle_rtc_set_time(int year, int month, int tm_day, int tm_hour, int tm_min, int tm_sec, int tm_wday)
{
    return m_ext_drive_rtc_set_time(year, month, tm_day, tm_hour, tm_min, tm_sec, tm_wday);
}

// 实现获取时间函数
stat_m m_callable_midlle_rtc_get_time(uint64_t* out_time_s)
{
    // stat_m stat = fail_r;
    // pcf8563_time_t  timestamp ;
    return m_ext_drvie_rtc_get_time(out_time_s);
}

// void m_static_rtc_time_runing_task(void *pvParameters)
// {
//     while (1)
//     {
//         pcf8563_time_t time;
//         m_ext_drvie_rtc_get_time(&time);
//         DEBUG_TEST( DB_I,"Current time is: %04d-%02d-%02d %02d:%02d:%02d, weekday %d\n",
//                    time.year, time.month, time.day,
//                    time.hour, time.minute, time.second, time.weekday);
//         mDelay_ms(1000);
//     }
// }

stat_m m_callable_midllle_rtc_init()
{
    m_callable_iic_init();

    // pcf8563_time_t time;
    // time.year = 2023;
    // time.month = 11;
    // time.day = 7;
    // time.hour = 14;
    // time.minute = 50;
    // time.second = 0;
    // time.weekday = 2; // 周五
    // if (m_ext_drive_rtc_set_time(&time) == false)
    // {
    //     DEBUG_TEST( DB_I,"m_callable_midlle_rtc_set_time(&time)==fail_r");
    // }

    // DEBUG_TEST( DB_I,"Current time is: %04d-%02d-%02d %02d:%02d:%02d, weekday %d\n",
    //            time.year, time.month, time.day,
    //            time.hour, time.minute, time.second, time.weekday);

    // mTaskCreate(NULL, m_static_rtc_time_runing_task, "m_static_rtc_time_runing_task", 2048, NULL, 2, NULL);
    return succ_r;
}


