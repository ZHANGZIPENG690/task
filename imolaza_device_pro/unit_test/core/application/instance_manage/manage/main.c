

#include "core/fset_lib.h"
#include "core/application/instance_manage/running/instance_running.h"
#include "core/application/instance_manage/manage/instance_manage.h"
#include "core/middle/event_distribution/event_distribution.h"




// #define MANAGE_AND_RUNNING 1


#if !true
long time_s = 0;
stat_m (*m_ext_instance_running_able)(
    int cmd,
    uint64_t instance_running_id,
    uint8_t start_channel,
    uint32_t start_channel_time_sum,
    uint32_t start_channel_will_running_time,
    uint8_t end_channel,
    uint32_t end_channel_reality_time,
    uint32_t end_channel_current_running_time,
    uint32_t end_channel_state,
    uint32_t running_once_sum,
    uint32_t running_sum,
    uint64_t instance_id,
    uint64_t in_time_s);

stat_m m_ext_instance_running_able_cb(
    int cmd,
    uint64_t instance_running_id,
    uint8_t start_channel,
    uint32_t start_channel_time_sum,
    uint32_t start_channel_will_running_time,
    uint8_t end_channel,
    uint32_t end_channel_reality_time,
    uint32_t end_channel_current_running_time,
    uint32_t end_channel_state,
    uint32_t running_once_sum,
    uint32_t running_sum,
    uint64_t instance_id,
    uint64_t in_time_s)
{

    printf("[%d,%lld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lld,%lld]\n", cmd, instance_running_id,
           start_channel,
           start_channel_time_sum,
           start_channel_will_running_time,
           end_channel,
           end_channel_reality_time,
           end_channel_current_running_time,
           running_once_sum,
           running_sum,
           end_channel_state,
           instance_id,
           in_time_s);
}

void add(int (*m_ext_instance_running)(
    int cmd,
    uint64_t instance_running_id,
    uint8_t start_channel,
    uint32_t start_channel_time_sum,
    uint32_t start_channel_will_running_time,
    uint8_t end_channel,
    uint32_t end_channel_reality_time,
    uint32_t end_channel_current_running_time,
    uint32_t end_channel_state,
    uint32_t running_once_sum,
    uint32_t running_sum,
    uint64_t instance_id,
    uint64_t in_time_s))
{
    // m_ext_instance_running_able = m_ext_instance_running;
}

void main()
{
    printf("Nihao... \n");

    stat_m stat = fail_r;
    char schedule_str[30] = "[20,522,1,1&20,1652000,1655445588,2&20;3&10,108,1,0,1]";
    m_callable_instance_manage_add(schedule_str, true, 10);
    while (1)
    {
        // scanf("%d", &event);
        // mDelay_ms(10);

    }
}
#else



#endif