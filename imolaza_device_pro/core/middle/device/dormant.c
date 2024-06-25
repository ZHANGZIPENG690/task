
/**
 * @file dormant.c
 * @author your name (you@domain.com)
 * @brief  发送和存储都是最原始的数据        当比较的时候   会转换成特定的格式
 *              比如 忽略年的话     月 *1000 + 日 * 100 +小时
 *              比如 不忽略年的话     直接比较
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "dormant.h"

/**    休眠模式*/
#define M_FLASH_KEY_OLD_SDORMANT "Sdormant"
#define M_FLASH_KEY_OLD_EDORMANT "Edormant"
/**    休眠模式*/
#define M_FLASH_KEY_DORMANT "dormant"
// struct m_date_t
// {
//     int year;
//     int month;
//     int day;
//     int hour;
// };
struct dormant_t
{
    int mode;
    uint64_t start_time;
    uint64_t end_time;
};
struct dormant_t main_dormant = {0};

char temp_str_i[80] = {0};

uint64_t m_old_u64_vue_a = 0;
uint64_t m_old_u64_vue_b = 0;

stat_m m_static_dormant_timestamp_transform_mdh(uint64_t in_time, uint64_t *out_num);

/**
 * @brief
 * @todo Uint Input ...
 *
 * @return stat_m
 */
stat_m m_callable_device_dormant_init(void)
{
    stat_m stat = succ_r;
    char temp_str__[60] = "2*1659995*17659820";
    // flash 加载
    if (m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DORMANT, temp_str__) == succ_r)
    {
        stat = M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_str__,"%d*%lld*%lld", &main_dormant.mode, &main_dormant.start_time, &main_dormant.end_time);
        DEBUG_TEST(DB_W,"Load Dormant Mode :  %s - " , temp_str__);
    }
    else
    {
        if (m_callable_drive_flash_read(M_TYPE_U64, M_FLASH_KEY_OLD_SDORMANT, (void *)&m_old_u64_vue_a) == succ_r &&
            m_callable_drive_flash_read(M_TYPE_U64, M_FLASH_KEY_OLD_EDORMANT, (void *)&m_old_u64_vue_b) == succ_r)
        // Find Dor Info OK !
        {
            if (m_old_u64_vue_a > 0 && m_old_u64_vue_b > 0)
            { // 旧设备 Format
              // 100100 is   1(Mothon)  0(NU) 01(Day)  00(Hour)
              // 1201223 is   12(Mothon)  0(NU) (Day)  00(Hour)
                main_dormant.mode = 1;
                if (m_old_u64_vue_a > 0100100 && m_old_u64_vue_b < 1201223) // 判断是否是特殊格式日期(跨年)
                {
                    // main_dormant.start_time = (m_old_u64_vue_a / 100000) *  ;
                    main_dormant.start_time = m_callable_timer_manage_get_spec_date_time(m_old_u64_vue_a / 100000, (m_old_u64_vue_a % 100000) / 100, (m_old_u64_vue_a % 100));
                    m_old_u64_vue_a = main_dormant.start_time;
                    main_dormant.end_time = m_callable_timer_manage_get_spec_date_time(m_old_u64_vue_b / 100000, (m_old_u64_vue_b % 100000) / 100, (m_old_u64_vue_b % 100));
                    m_old_u64_vue_b = main_dormant.end_time;
                    main_dormant.mode = 2;
                }
                else if (m_old_u64_vue_a > 010100 && m_old_u64_vue_b < 123123) // 不跨年
                {
                    main_dormant.start_time = m_callable_timer_manage_get_spec_date_time(m_old_u64_vue_a / 10000, (m_old_u64_vue_a % 10000) / 100, (m_old_u64_vue_a % 100));
                    m_old_u64_vue_a = main_dormant.start_time;
                    main_dormant.end_time = m_callable_timer_manage_get_spec_date_time(m_old_u64_vue_b / 10000, (m_old_u64_vue_b % 10000) / 100, (m_old_u64_vue_b % 100));
                    m_old_u64_vue_b = main_dormant.end_time;
                    main_dormant.mode = 2;
                }
                // Else  Spec Time
                main_dormant.start_time = m_old_u64_vue_a;
                main_dormant.end_time = m_old_u64_vue_b;
                DEBUG_TEST(DB_W,"Old Dormant Mode ： %lld <-> %lld  %d", main_dormant.start_time, main_dormant.end_time, main_dormant.mode);
                m_callable_device_dormant_set(main_dormant.start_time, main_dormant.end_time, main_dormant.mode);
                m_callable_drive_flash_remove(M_FLASH_KEY_OLD_SDORMANT);
                m_callable_drive_flash_remove(M_FLASH_KEY_OLD_EDORMANT);
            }
        }
    }
    return stat;
}
/**
 * @brief Test Dormant
 *
 * @param a Start Time
 * @param b End Time
 */
void m_test_dormant_set(uint64_t a, uint64_t b)
{
    DEBUG_TEST( DB_I,"J ()%lld %lld", a, b);
    m_ext_flash_write(M_TYPE_U64, M_FLASH_KEY_OLD_SDORMANT, &a);
    m_ext_flash_write(M_TYPE_U64, M_FLASH_KEY_OLD_EDORMANT, &b);
}

/**
 * @brief
 * @todo Uint Input ...
 *
 * @param start_time
 * @param end_time
 * @param mode
 * @return stat_m
 */
stat_m m_callable_device_dormant_set(uint64_t start_time, uint64_t end_time, uint8_t mode)
{
    stat_m stat = succ_r;
    if (start_time > 0 && end_time > 0)
    {
        main_dormant.mode = mode;
        main_dormant.start_time = start_time;
        main_dormant.end_time = end_time;
        // if (main_dormant.mode == 0)
        {
            sprintf(temp_str_i, "%d*%lld*%lld", mode, start_time, end_time);
            m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DORMANT, temp_str_i);
            m_callable_device_attribute_config_change_notify_event_callable(
                M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_DORMANT,
                (void *)&mode,
                m_callable_timer_manage_get_device_time());
        }
        // else
        //     ;
        DEBUG_TEST( DB_I,"%s", temp_str_i);
    }
    else
    {
        main_dormant.mode = 0;
        main_dormant.start_time = 0;
        main_dormant.end_time = 0;
        m_callable_drive_flash_remove(M_FLASH_KEY_DORMANT);
    }
    return stat;
}

/**
 * @brief
 * @todo Uint Out ...
 *
 * @param current_time
 * @return true 应该运行
 * @return false
 */
stat_m m_callable_device_dormant_current_is_effective(uint64_t current_time)
{
    stat_m stat = succ_r;
    if (main_dormant.start_time > 0 && main_dormant.end_time > 0)
    {
        // 先把时间转换成年月日

        // 根据模式判断当前的时间是否处于休眠
        if (main_dormant.mode == 1)
        {
            if (current_time >= main_dormant.start_time && current_time < main_dormant.end_time)
            {
                stat = fail_r;
            }
        }
        else if (main_dormant.mode == 2)
        {
            // 分贝是当前时间  开始时间和结束时间
            uint64_t cur_tms = current_time, str_tms = 0, end_tms = 0;
            m_static_dormant_timestamp_transform_mdh(current_time, &cur_tms);
            cur_tms += m_callable_timer_manage_get_hhmmss() % 60;
            m_static_dormant_timestamp_transform_mdh(main_dormant.start_time, &str_tms);
            m_static_dormant_timestamp_transform_mdh(main_dormant.end_time, &end_tms);
            end_tms += 23;
            // 跨年
            if (str_tms > end_tms)
            {
                if (cur_tms >= str_tms && cur_tms > end_tms)
                    stat = fail_r;
                if (cur_tms <= str_tms && cur_tms < end_tms)
                    stat = fail_r;
                DEBUG_TEST( DB_I,"Out year(%lld) %lld - %lld - > %d", cur_tms, str_tms, end_tms, stat);
            }
            // 不跨跨年
            else
            {
                if (cur_tms >= str_tms && cur_tms < end_tms)
                    stat = fail_r;
                DEBUG_TEST( DB_I,"Not out year(%lld) %lld - %lld -> %d ", cur_tms, str_tms, end_tms, stat);
            }
        }
    }
    return stat;
}
/**
 * @brief 把一个的时间转换成  时间和日期的指定数字
 *
 * @param in_time 传入的时间
 * @param out_num 传入的时间
 * @return stat_m
 */
stat_m m_static_dormant_timestamp_transform_mdh(uint64_t in_time, uint64_t *out_num)
{
    stat_m stat = fail_r;
    // time_t ptime = in_time;
    // struct tm *tps;
    uint64_t sytime = 0, eytime = 0;
    uint32_t step = 10000, a, b, c;

    m_callable_timer_manage_get_ymd(in_time, &a, &b, &c);
    // a = tps->tm_mon + 1;
    // b = tps->tm_mday;
    // c = tps->tm_hour;

    // *out_num = a * 10000 + b * 100 + c;
    *out_num = b * 10000 + c * 100; // 24 12 58

    return stat;
}

stat_m m_callable_device_dormant_clean(void)
{
    main_dormant.mode = 0;
    main_dormant.start_time = 0;
    main_dormant.end_time = 0;
    return 0;
}