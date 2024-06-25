

// #include "core/fset_lib.h"

// int sync = 0;

// int main_time = 30;
// int main_local = 30;
// int main_sch = 30;

// void (*m_callable_sync_unit_callback)(int event_id , uint64_t in_time_s );

// enum evntm
// {
//     M_SYNC_TIME,
//     M_SYNC_LOCAL,
//     M_SYNC_SCHEDULE,
// };

// void m_callable_sync_unit_set_time(uint64_t time)
// {
//     main_time = time;
// }
// void m_callable_sync_unit_set_local(uint64_t lon, uint64_t lat)
// {
//     main_local = 1;
// }
// void m_callable_sync_unit_set_schedule(uint64_t schedule_num)
// {
//     main_sch = schedule_num;
// }

// void m_callable_sync_unit_reg_callable(void (*m_callable_sync_unit_ca)(int event_id , uint64_t ))
// {
//         m_callable_sync_unit_callback = m_callable_sync_unit_ca;
// }
// void m_callable_sync_unit_init()
// {
// }

// void m_callable_sync_unit_monitor(uint64_t sync_time)
// {
//     if (main_local % 30 == 0)
//     {
//         m_callable_sync_unit_callback(M_SYNC_LOCAL, sync_time);
//     }
//     if (main_sch % 30 == 0)
//     {
//         m_callable_sync_unit_callback(M_SYNC_SCHEDULE, sync_time);
//     }
//     if (sync_time % 30 == 0 || main_time == 0)
//     {
//         if (m_callable_sync_unit_callback != NULL)
//         {
//             main_time = 0;
//             m_callable_sync_unit_callback(M_SYNC_TIME, sync_time);
//         }
//     }
//     mDelay_ms(1000);
// }
