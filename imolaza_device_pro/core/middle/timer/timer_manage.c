

#include "timer_manage.h"

#include <time.h>

/** 表示时间的结构体 */
struct tim_tz
{
    bool timezone_update;
    /** 当前的 utc 时间 */
    uint64_t utc_time_s;
    /** 监控每次的增量 */
    int time_inv;
    /** 时区 */
    int time_curr_zone;
    /** 设置的时区 1*/
    int time_zone_1;
    /** 设置的时区 2*/
    int time_zone_2;
    /** 冬令时、夏令时 切换的依据， */
    int time_type;

    /** 时区 1 有效开始时间*/
    uint64_t inv_time_1;
    /** 时区 1 有效结束时间 和 时区 2 有效开始时间*/
    uint64_t inv_time_2;
    /** 时间 */
    int tim_s[6];
};

struct tim_tz time_root;

char temp_tim_flash_buf[100];

#define JIJIE "ko"

stat_m m_callable_timer_manage_get_ymd(uint64_t timep, uint32_t *year, uint32_t *month, uint32_t *day);
/**
 * @brief 时间走时
 *
 * @return stat_m
 */
stat_m m_callable_timer_manage_time_monitor(void *args);

stat_m m_static_time_manage_save(uint64_t time_inav1, int offset_tz, uint64_t time_inav2, int offset_tz2)
{
    stat_m stat = fail_r;
    sprintf(temp_tim_flash_buf, "%lld,%d,%lld,%d", time_inav1, offset_tz, time_inav2, offset_tz2);

    stat = m_callable_drive_flash_write(M_TYPE_Str, JIJIE, temp_tim_flash_buf);
    return stat;
}

stat_m m_callable_timer_manage_timezone_switch_load(void)
{
    stat_m stat = fail_r;
    stat = m_callable_drive_flash_read(M_TYPE_Str, JIJIE, temp_tim_flash_buf);
    if (stat == succ_r)
    {
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_tim_flash_buf, "%lld,%d,%lld,%d",
                                                  &time_root.inv_time_1,
                                                  &time_root.time_zone_1,
                                                  &time_root.inv_time_2,
                                                  &time_root.time_zone_2);
    }
    else
        time_root.time_zone_1 = 100, time_root.time_zone_2 = 100;
    return stat;
}

/**
 * @brief 定时器初始化
 *
 * @return stat_m
 */
stat_m m_callable_timer_manage_time_init(void)
{
    stat_m stat = fail_r;
    uint64_t tim = time_root.utc_time_s;
    memset(&time_root, 0, sizeof(time_root));
    time_root.utc_time_s = tim;
    time_root.time_curr_zone = 0;

    time_root.time_inv = 1;
    m_ext_drive_timer_init();
    // 加载时间和时区

    // 加载季节调整的时间
    mTaskCreate(NULL, m_callable_timer_manage_time_monitor, "m_tim_event", 1024 * 4, (void *)3, 5, NULL); //--sock tcp link

    return stat;
}
/**
 * @brief 直接设置设备时间
 *
 * @param time
 * @return stat_m
 */
stat_m m_callable_timer_manage_time_direct_set(uint64_t time, int in_tz)
{
    stat_m stat = fail_r;
    time_root.time_inv = 1;
    time_root.utc_time_s = time;
    time_root.time_curr_zone = in_tz;
    return stat;
}
/**
 * @brief 设置设备的时间
 * @todo Unit Input ...
 *
 * @param time   设备时间
 * @param tz 时区
 * @return stat_m
 */
stat_m m_callable_timer_manage_time_set(uint64_t time, int tz, int type)
{
    stat_m stat = fail_r;
    uint32_t y = 0, m = 0, d = 0;
    uint64_t hhmmss = 0;

    time_root.time_inv = 1;
    time_root.utc_time_s = time;
    time_root.time_curr_zone = tz;

    DEBUG_TEST(DB_W, "m_callable_timer_manage_time_set111111");
    m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_TIME,
                                                                    (void *)&tz, time_root.utc_time_s + tz * 3600);
    DEBUG_TEST(DB_W, "m_callable_timer_manage_time_set222222");
    m_callable_timer_manage_get_ymd(time_root.utc_time_s + tz * 3600, &y, &m, &d);
    hhmmss = (time_root.utc_time_s + tz * 3600) % 86400;
    DEBUG_TEST(DB_W, "m_callable_timer_manage_time_set333333");
    m_callable_midlle_rtc_set_time(y, m, d, hhmmss / 3600, (hhmmss % 3600) / 60, hhmmss % 60, 0);
    DEBUG_TEST(DB_W, "m_callable_timer_manage_time_set4444444");
    
    DEBUG_TEST(DB_I, "Setting device time  %lld , %d", time_root.utc_time_s, time_root.time_curr_zone);
    return stat;
}
/**
 * @brief Get Date - TimeStamp
 *
 * @param mohth Month
 * @param day Day
 * @param hour Hour
 * @return uint64_t
 */
uint64_t m_callable_timer_manage_get_spec_date_time(int mohth, int day, int hour)
{
    struct tm t = {0};
    time_t timestamp = 0;
    DEBUG_TEST(DB_I, "%d %d %d", mohth, day, hour);

    /* 日期修正 */
    t.tm_year = 70;
    t.tm_mon = mohth - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = 0;
    t.tm_sec = 0;

    timestamp = mktime(&t);
    DEBUG_TEST(DB_I, "%ld ", timestamp);
    return timestamp;
}
/**
 * @brief 获取设备时间
 *
 * @return uint64_t
 */
uint64_t m_callable_timer_manage_get_device_time(void)
{
    // DEBUG_TEST(DB_I,DB_W,"rfemo %lld  utc %lld , Tz :%d",time_root.utc_time_s + time_root.time_curr_zone * 3600, time_root.utc_time_s, time_root.time_curr_zone);
    return time_root.utc_time_s + time_root.time_curr_zone * 3600;
}
/**
 * @brief 获取UTC时间
 *
 * @return uint64_t
 */
uint64_t m_callable_timer_manage_get_utc_time(void)
{
    return time_root.utc_time_s;
}
/**
 * @brief 获取当天的时间 小于等于86400
 *
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_hhmmss(void)
{
    return ((time_root.utc_time_s + time_root.time_curr_zone * 3600) % NUM_OF_DAY_SEC_SUM);
}
/**
 * @brief 获取当天的设备 零点 时间
 *
 * @return uint64_t
 */
uint64_t m_callable_timer_manage_get_device_zero_time(void)
{
    // DEBUG_TEST(DB_I, DB_I,"gg %lld", time_root.utc_time_s);
    // DEBUG_TEST(DB_I, DB_I,"%lld   %d ", time_root.utc_time_s + time_root.time_curr_zone * 3600,
    //             time_root.time_curr_zone);

    // DEBUG_TEST(DB_I, DB_I,"%lld  ",
    //             (((time_root.utc_time_s + time_root.time_curr_zone * 3600) % NUM_OF_DAY_SEC_SUM)));
    return
        // ((time_root.utc_time_s - time_root.utc_time_s % NUM_OF_DAY_SEC_SUM) +time_root.time_curr_zone * 3600);
        ((time_root.utc_time_s + time_root.time_curr_zone * 3600) -
         ((time_root.utc_time_s + time_root.time_curr_zone * 3600) % NUM_OF_DAY_SEC_SUM));
}

/**
 * @brief 获取当天的星期
 *
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_week(uint64_t in_time_s)
{
    uint32_t year, month, day;
    m_callable_timer_manage_get_ymd(in_time_s, &year, &month, &day);
    {
        if (month == 1 || month == 2) // åˆ¤æ–­monthæ˜¯å<90>¦ä¸º1æˆ–2ã€€
            year--, month += 12;
        int c = year / 100;
        int y = year - c * 100;
        int week = y + y / 4 + c / 4 - 2 * c + 26 * (month + 1) / 10 + day - 1;
        while (week < 0)
            week += 7;
        week %= 7;
        DEBUG_TEST(DB_I, "week %d ", week);
        return week;
    }
    return 0;
}
/**
 * @brief 获取两个时间戳之间的小时数
 *
 * @param timpA
 * @param timpB
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_diff_hour_num(uint64_t timpA, uint64_t timpB)
{
    int resu = 0;
    DEBUG_TEST(DB_I, "timpA: %lld - timpB %lld  ", timpA, timpB);
    //  (difftime(timpA, timpB) / 3600);
    if (timpA > timpB)
        resu = ((timpA - timpB) / (NUM_OF_DAY_SEC_SUM / 24));
    else
        resu = ((timpB - timpA) / (NUM_OF_DAY_SEC_SUM / 24));

    return resu;
}
/**
 * @brief 获取当天的星期
 *
 * @return uint32_t
 */
uint32_t m_callable_timer_manage_get_day(uint64_t timp)
{
    uint32_t result = 0;
    m_callable_timer_manage_get_ymd(timp, NULL, NULL, &result);
    return result;
}

/**
 * @brief 季节调整，，简单的意思就是 在  time_inav1 到  time_inav2 之间的时间区间范围内 是 offset_tz
 *                      其他的是 offset_tz2
 * @todo Unit Input ...
 *
 * @param time_inav1 第一个时区开始的时间
 * @param offset_tz 时区1
 * @param time_inav2 第一个时区结束的时间 第二个时区开始的时间
 * @param offset_tz2 时区2
 * @return stat_m
 */
stat_m m_callable_timer_manage_timezone_switch_set(uint64_t time_inav1, int offset_tz, uint64_t time_inav2, int offset_tz2)
{
    stat_m stat = fail_r;
    // 先判断时间和合理性
    if (time_inav1 > 0 && time_inav2 > 0)
    {
        DEBUG_TEST(DB_I, "m_callable_timer_manage_timezone_switch_set %lld -> %d - %lld %d", time_inav1, offset_tz, time_inav2, offset_tz2);
        // 第二个是 存起来
        stat = m_static_time_manage_save(time_inav1, offset_tz, time_inav2, offset_tz2);
        time_root.timezone_update = true;
        // 第三个是 加载到内存
        time_root.inv_time_1 = time_inav1;
        time_root.inv_time_2 = time_inav2;
        time_root.time_zone_1 = offset_tz;
        time_root.time_zone_2 = offset_tz2;
    }

    return stat;
}
/**
 * @brief 解析一个时间戳的全部记录  年月日时分秒
 *
 * @param in_tim 时间
 * @param out_tm 输出
 * @return stat_m
 */
stat_m m_callable_timer_manage_get_ymd_hms(uint64_t in_tim, int *out_tm)
{
    struct tm tm = *gmtime(&in_tim);
    out_tm[0] = tm.tm_year + 1900;
    out_tm[1] = tm.tm_mon + 1;
    out_tm[2] = tm.tm_mday;
    out_tm[3] = tm.tm_hour;
    out_tm[4] = tm.tm_min;
    out_tm[5] = tm.tm_sec;
    return succ_r;
}
stat_m m_callable_timer_manage_time_update(void)
{
    time_root.utc_time_s += time_root.time_inv;
    m_callable_event_distribution_advancing(time_root.utc_time_s + time_root.time_curr_zone * 3600);
    m_callable_instance_running_monitor(time_root.utc_time_s + time_root.time_curr_zone * 3600);
    // m_callable_instance_running_monitor(time_root.utc_time_s + time_root.time_curr_zone * 3600);
    // DEBUG_TEST(DB_I, DB_I,"%lld", time_root.utc_time_s);
    return succ_r;
}
/**
 * @brief 时间走时
 *
 * @return stat_m
 */
stat_m m_callable_timer_manage_time_monitor(void *args)
{
    stat_m stat = fail_r;
    uint64_t ins_tts = 0;
    while (1)
    {
        if (time_root.utc_time_s != ins_tts)
        {
            ins_tts = time_root.utc_time_s;

            // DEBUG_TEST(DB_I, DB_I,"%d", time_root.time_curr_zone);
            /** 网络连接 和 处理 */
            m_callable_event_monitor(time_root.utc_time_s + time_root.time_curr_zone * 3600);
            // 在这里会监控当前每一刻的时间 看下是否需要调整时区 , 是否启用时区调整
            // if (time_root.timezone_update)
            // {
            //     if (time_root.utc_time_s > time_root.inv_time_1 &&
            //         time_root.utc_time_s < time_root.inv_time_2)
            //     {
            //         if (time_root.time_zone_1 != time_root.time_curr_zone)
            //         {
            //             time_root.time_curr_zone = time_root.time_zone_1;
            //         }
            //     }
            //     else if (time_root.time_zone_2 != time_root.time_curr_zone)
            //     {
            //         time_root.time_curr_zone = time_root.time_zone_2;
            //     }
            // }
        }
        mDelay_ms(200);
    }
    return stat;
}
/**
 * @brief 返回的时间经过了设备时间的确认
 *
 * @param timep 当前的时间戳
 * @param year
 * @param month
 * @param day
 * @return stat_m
 */

stat_m m_callable_timer_manage_get_ymd(uint64_t timep, uint32_t *year, uint32_t *month, uint32_t *day)
{
    // time_t tick = (&timep);
    // DEBUG_TEST(DB_I, DB_I,"DIDI   %lld" , tick);
    struct tm tm = *gmtime(&timep);
    if (year != NULL)
        *year = tm.tm_year + 1900;
    if (month != NULL)
        *month = tm.tm_mon + 1;
    if (day != NULL)
        *day = tm.tm_mday;
    // if (time_root.time_curr_zone >= 0 && time_root.time_curr_zone + tm.tm_hour >= 24)
    // {
    //     if (day != NULL)
    //         *day = tm.tm_mday + 1;
    // }
    // else if (time_root.time_curr_zone < 0 && tm.tm_hour < (time_root.time_curr_zone * -1))
    // {
    //     if (day != NULL)
    //         *day = tm.tm_mday - 1;
    // }

    DEBUG_TEST(DB_I, "%d-%d-%d , %d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return 0;
}

// 函数用于判断某一年是否为闰年
int m_static_time_is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 函数用于获取某年某月的天数
int m_static_time_get_days_in_month(int year, int month)
{
    if (month == 2)
    {
        return m_static_time_is_leap_year(year) ? 29 : 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    else
    {
        return 31;
    }
}

// 函数用于计算某年某月第n个星期x的日期的时间戳，返回指定小时和分钟时间戳（通过美国纽约时区）
time_t m_static_time_get_weekday_of_month_with_time_in_newyork(int year, int month, int n, int day_of_week, int hour, int minute, int time_type)
{
    struct tm timeinfo = {0};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = minute;
    time_t time = mktime(&timeinfo);
    timeinfo = *localtime(&time);

    int daysInMonth = m_static_time_get_days_in_month(year, month);
    int weekdayCount = 0;

    if (time_type == 2)
    {
        timeinfo.tm_mday = daysInMonth;
        while (timeinfo.tm_wday != 0)
        {
            timeinfo.tm_mday--;
            time = mktime(&timeinfo);
            timeinfo = *localtime(&time);
        }
        timeinfo.tm_hour = hour;
        timeinfo.tm_min = minute;
        // 通过设置时区为伦敦 (GMT+0)
        setenv("TZ", "GMT0", 1);
        tzset();

        struct timeval tv = {
            mktime(&timeinfo),
            0};
        return tv.tv_sec;
    }
    else
    {
        while (timeinfo.tm_mday <= daysInMonth)
        {
            if (timeinfo.tm_wday == day_of_week)
            {
                weekdayCount++;
                if (weekdayCount == n)
                {
                    break;
                }
            }
            timeinfo.tm_mday++;
            mktime(&timeinfo);
        }
        // 通过设置时区为美国纽约 (EST)
        setenv("TZ", "EST5EDT", 1);
        tzset();

        struct timeval tv = {
            mktime(&timeinfo),
            0};
        return tv.tv_sec;
    }
}

stat_m m_callable_get_next_dst_transition_type(time_t timestamp, int timezone_offset, int time_type, time_t *next_transition_timestamp, int *next_timezone_offset)
{
    stat_m stat = fail_r;
    time_t inputTimestamp;
    struct tm *timeinfo;

    /*设置是当月的第几个星期天、当天的几点*/
    int start_day = 0;
    int end_day = 0;
    int start_hour = 0;
    int end_hour = 0;

    inputTimestamp = timestamp;

    // DEBUG_TEST(DB_I,"请输入一个时间戳：");
    //  scanf("%ld", &inputTimestamp);
    // DEBUG_TEST(DB_I,"时间戳：%ld", inputTimestamp);
    timeinfo = localtime(&inputTimestamp);
    int month = timeinfo->tm_mon + 1;

    switch (time_type)
    {
    case 1:
        start_day = 2;
        end_day = 1;
        start_hour = 2;
        end_hour = 2;
        break;
    case 2:
        start_day = 0;
        end_day = 0;
        start_hour = 1;
        end_hour = 2;
        break;
    case 3:
        start_day = 3;
        end_day = 2;
        start_hour = 2;
        end_hour = 2;
        break;

    default:
        break;
    }
    if (time_type != 0)
    {
        time_t daylight_time_start = m_static_time_get_weekday_of_month_with_time_in_newyork(timeinfo->tm_year + 1900, 3, start_day, 0, start_hour, 0, time_type);
        time_t daylight_time_end = m_static_time_get_weekday_of_month_with_time_in_newyork(timeinfo->tm_year + 1900, 11, end_day, 0, end_hour, 0, time_type);

        DEBUG_TEST(DB_I, "三月的第%d个星期天的0%d:00(xx时区)的时间戳为：%ld", start_day, start_hour, daylight_time_start);
        DEBUG_TEST(DB_I, "十一月的第%d个星期天的0%d:00(xx时区)的时间戳为：%ld", end_day, end_hour, daylight_time_end);

        /*如果 当前时间处于夏令时开始之后，夏令时结束之前*/
        if (inputTimestamp >= daylight_time_start && inputTimestamp < daylight_time_end)
        {
            DEBUG_TEST(DB_I, "当前处于夏令时");
            *next_transition_timestamp = daylight_time_end;
            *next_timezone_offset = timezone_offset + 1; // 下一个时区偏移量+1
            DEBUG_TEST(DB_I, " 时区需要加一:   %d ", timezone_offset + 1);
            stat = succ_r;
            // return 0; // 下一个时间点  切换到标准时区
        }
        else
        {
            DEBUG_TEST(DB_I, "当前处于冬令时");
            // 如果timestamp在今年夏令时开始之前，则下一个切换时区为今年的夏令时开始时间点
            if (inputTimestamp < daylight_time_start)
            {
                *next_transition_timestamp = daylight_time_start;
                DEBUG_TEST(DB_I, "下一次切换时间时 今年的夏令时开始 :%ld", daylight_time_start);
            }
            else
            {
                time_t inputTimestamp_next;
                /*如果当前时间是今年夏令时结束之后，则下一次夏令时的开始是 第二年的 夏令时开始时间点
                所以需要加 31536000秒，为一年，并且是在当年的夏令时开始的时间基础上加一年，这样就不用去判断时平年还是闰年了*/
                inputTimestamp_next = inputTimestamp + 31536000;
                timeinfo = localtime(&inputTimestamp_next);
                time_t next_year_dst_start_time = m_static_time_get_weekday_of_month_with_time_in_newyork(timeinfo->tm_year + 1900, 3, start_day, 0, start_hour, 0, time_type);
                *next_transition_timestamp = next_year_dst_start_time;

                DEBUG_TEST(DB_I, "下一次切换时间是 明年的夏令时开始:%ld", next_year_dst_start_time);
            }

            *next_timezone_offset = timezone_offset - 1;
            DEBUG_TEST(DB_I, " 时区需要减一 :  %d ", timezone_offset - 1);
            // return 1; // 下一个时间点为夏令时切换（下一年的夏令时开始）
            stat = succ_r;
        }
    }
    return stat;
}
