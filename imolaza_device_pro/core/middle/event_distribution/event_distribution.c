

#include "event_distribution.h"

stat_m m_callable_event_distribution_advancing(uint64_t current_timestamp)
{
    stat_m stat = fail_r;
    stat = m_static_time_link_notece_node_advancing(current_timestamp);
    return stat;
}

stat_m m_callable_event_distribution_tiansit(uint8_t event_fag, uint8_t event_id, enum fsm_change_cause cause, void *in_obj, uint64_t happen_timestamp)
{
    stat_m stat = fail_r;
    // printf("%d\n" , pre);
    DEBUG_TEST( DB_I,"-- - - m_callable_event_distribution_tiansit(%d)" , event_id);

    if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_WATER_HAMMER)
        stat = m_static_instance_running_hammer_handle(event_fag, cause, event_id, in_obj, happen_timestamp);
    else if (event_id >= M_PRIVATE_BASE_STATUS_FSM)
        stat = m_static_instance_running_fsm_post(event_fag, cause, event_id, in_obj, happen_timestamp);

    // printf("------ %d\n" , event_id);
    return stat;
}
stat_m m_callable_event_monitor(uint64_t in_timestamp)
{
    stat_m stat = fail_r;
    // if (in_timestamp % 10 == 0)
    //     m_static_event_handle(M_EVENT_TIME_6_SEC);

    if (in_timestamp % 6 == 0)
        m_static_event_handle(M_EVENT_TIME_6_SEC);
    if (in_timestamp % 30 == 0)
        m_static_event_handle(M_EVENT_TIME_30_SEC);
    if (in_timestamp % 60 == 0)
        // {
        m_static_event_handle(M_EVENT_TIME_1_MIN);
    // }
    // DEBUG_TEST( DB_I,"cur  : %d", m_callable_current_check_get_final_value());
    return stat;
}
