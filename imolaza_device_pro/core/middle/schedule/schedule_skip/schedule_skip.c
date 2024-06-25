

#include "schedule_skip.h"

const char *M_CONST_FLASH_FLAG_A_SKIP = "a_skip";

struct inst_schedule_skip
{
    uint64_t sch_id;
    uint64_t start_time;
    uint64_t end_time;
};
struct inst_schedule_skip sch_skip_array[16] = {0};

char temp_str[80] = {0};
/**
 * @brief Load information from memory
 * @todo Unit Input ...
 *
 * @return stat_m
 */
stat_m m_callable_schedule_skip_init(uint64_t in_sid)
{
    stat_m stat = succ_r;
    // 假设读取到一个参数
    char temp_str__[60] = {0};
    sprintf(temp_str__, "a_skip%lld", in_sid);
    stat = m_callable_drive_flash_read(M_TYPE_Str, temp_str__, (void *)temp_str);
    if (stat == succ_r)
    {
        DEBUG_TEST(DB_I, "Loading Schedule %s", temp_str);
        for (int i = 0; i < 16; i++)
            if (sch_skip_array[i].sch_id == 0 || sch_skip_array[i].sch_id == in_sid)
            {
                sch_skip_array[i].sch_id = in_sid;
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_str, "%lld*%lld", &sch_skip_array[i].start_time, &sch_skip_array[i].end_time);
                DEBUG_TEST(DB_I, "Setting Schedule %lld-%lld-%lld", sch_skip_array[i].sch_id, sch_skip_array[i].start_time, sch_skip_array[i].end_time);
                break;
            }
    }

    return stat;
}

/**
 * @brief Configure the start time and end time of water-delay
 * @todo Unit Input ...
 *
 * @param sch_id schedule id
 * @param start_time start time
 * @param end_time end time
 * @return stat_m
 */
stat_m m_callable_schedule_skip_set(uint64_t sch_id, uint64_t start_time, uint64_t end_time, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    char temp_str__[60] = {0};
    // DEBUG_TEST( DB_I,"%s-----------------------", temp_str);
    if (start_time > 0 && end_time > 0 && end_time > start_time)
    {
        for (int i = 0; i < 16; i++)
        {
            if (sch_id == sch_skip_array[i].sch_id || sch_skip_array[i].sch_id == 0 || i == 15)
            {
                DEBUG_TEST(DB_I, "YES %d", i);
                sch_skip_array[i].sch_id = sch_id;
                sch_skip_array[i].start_time = start_time;
                sch_skip_array[i].end_time = end_time;
                m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_A_SKIP,
                                                                                (void *)&sch_id, in_time_s);
                break;
            }
        }
        sprintf(temp_str__, "a_skip%lld", sch_id);
        sprintf(temp_str, "%lld*%lld", start_time, end_time);
        stat = m_callable_drive_flash_write(M_TYPE_Str, temp_str__, (void *)temp_str);
        DEBUG_TEST(DB_I, "%s", temp_str);
    }

    return stat;
}

/**
 * @brief Determine whether the current time is within the water-delay range
 * @todo Unit Out ...
 *
 * @param sch_id schedule id
 * @param current_time  Current time
 * @return stat_m
 */
stat_m m_callable_schedule_skip_current_is_effective(uint64_t sch_id, uint64_t in_time_s)
{
    stat_m stat = succ_r;

    for (int i = 0; i < 16; i++)
    {
        if (sch_id == sch_skip_array[i].sch_id && in_time_s >= sch_skip_array[i].start_time && in_time_s < sch_skip_array[i].end_time)
        {
            // printf("a %lld b %lld ", sch_id, sch_skip_array[i].sch_id);
            stat = fail_r;
        }
    }
    // printf("a %lld b ", sch_id);
    return stat;
}

stat_m m_callable_schedule_skip_clean(uint64_t sch_id)
{
    stat_m stat = fail_r;
    char temp_str__[60] = {0};

    for (int i = 0; i < 16; i++)
    {
        if (sch_id == sch_skip_array[i].sch_id)
        {
            sprintf(temp_str__, "a_skip%lld", sch_id);
            stat = m_callable_drive_flash_remove(temp_str__);
            m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_ABORT_A_SKIP,
                                                                            (void *)&sch_id, 0);
            sch_skip_array[i].sch_id = 0;
            sch_skip_array[i].start_time = 0;
            sch_skip_array[i].end_time = 0;
            stat = succ_r;
            break;
        }
    }
    return stat;
}