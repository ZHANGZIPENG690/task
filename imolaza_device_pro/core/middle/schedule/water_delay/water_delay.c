

#include "water_delay.h"

uint64_t water_delay_start_time = 0;
uint64_t water_delay_end_time = 0;

char temp_str_a[80] = {0};

const char *M_CONST_FLASH_FLAG_WATER_DELAY = "water_delay";

#define SKIP_SOMETIME_As "As"
#define SKIP_SOMETIME_Ae "Ae"

/**
 * @brief Load information from memory
 * @todo Unit Input ...
 *
 * @return stat_m
 */
stat_m m_callable_water_delay_init(void)
{
    stat_m stat = fail_r;
    char temp_str__[60] = {0};
    uint64_t in_tmp_ats = 0, in_tmp_ate = 0;
    m_callable_drive_flash_read(M_TYPE_Str, M_CONST_FLASH_FLAG_WATER_DELAY, temp_str__);
    // flash 加载
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_str__ , "%lld*%lld", &water_delay_start_time, &water_delay_end_time);
    DEBUG_TEST(DB_W,"Load WaterDelay  start: %lld end；  %lld",
                 water_delay_start_time, water_delay_end_time);
    /** Find Old Device */
    if (m_callable_drive_flash_read(M_TYPE_U64, SKIP_SOMETIME_As, &in_tmp_ats) == succ_r)
    {
        if (m_callable_drive_flash_read(M_TYPE_U64, SKIP_SOMETIME_Ae, &in_tmp_ate) == succ_r)
        {
            m_callable_water_delay_set(in_tmp_ats, in_tmp_ate, 0);
            m_callable_drive_flash_remove(SKIP_SOMETIME_As);
            m_callable_drive_flash_remove(SKIP_SOMETIME_Ae);
            DEBUG_TEST(DB_W,"Old Device : Load  WaterDelay Time : %lld - %lld" , in_tmp_ats , in_tmp_ate);
        }
    }
    return stat;
}

/**
 * @brief Configure the start time and end time of water-delay
 * @todo Unit Input ...
 *
 * @param start_time start time
 * @param end_time end time
 * @return stat_m
 */
stat_m m_callable_water_delay_set(uint64_t start_time, uint64_t end_time, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    if (start_time > 0 && end_time > 0 && end_time > start_time)
    {
        water_delay_start_time = start_time;
        water_delay_end_time = end_time;
        // 更新一下 flash
        sprintf(temp_str_a, "%lld*%lld", start_time, end_time);
        m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_GLOBAL_SKIP,
                                                                        (void *)&start_time, in_time_s);
        stat = m_callable_drive_flash_write(M_TYPE_Str, M_CONST_FLASH_FLAG_WATER_DELAY, temp_str_a);

        DEBUG_TEST( DB_I,"%s", temp_str_a);
    }
    return stat;
}

/**
 * @brief Determine whether the current time is within the water-delay range
 * @todo Unit Out ...
 *
 * @param current_time  Current time
 * @return stat_m
 */
stat_m m_callable_water_delay_current_is_effective(uint64_t current_time)
{
    stat_m stat = succ_r;
    if (current_time >= water_delay_start_time && current_time < water_delay_end_time)
    {
        stat = fail_r;
    }
    return stat;
}

stat_m m_callable_water_delay_clean(void)
{
    stat_m stat = succ_r;
    water_delay_start_time = 0;
    water_delay_end_time = 0;
    m_callable_drive_flash_remove(M_CONST_FLASH_FLAG_WATER_DELAY);
    // m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_ABORT_A_SKIP,
    //                                                                 (void *)&, 0);
    return stat;
}
