
// #include "esp32/core/fset_lib.h"
// #include "esp32/core/utils/util.h"
// #include "mschedule.h"


// static const int M_THINK_HANGUP_MAX_NUN = 5;

// m_watering_schedule *hangup_table[M_THINK_HANGUP_MAX_NUN] = {NULL};

// // stat_m m_think_wakeup( m_watering_schedule *in_obj)
// // {
// //         stat_m stat = fail_r;
// //     if (in_obj != NULL)
// //     {

// //         stat = succ_r;
// //     }
// //     return stat;
// // }

// stat_m m_schedule_think_wakeup(void)
// {
//     stat_m stat = fail_r;

//     for (int i = 0; i < M_THINK_HANGUP_MAX_NUN; i++)
//     {
//         if (hangup_table[i] != NULL)
//         {
//             /** 恢复操作*/
//             stat = succ_r;
//         }
//     }

//     return stat;
// }
// stat_m m_schedule_think_hangup(m_watering_schedule *in_obj)
// {
//     stat_m stat = fail_r;
//     if (in_obj != NULL)
//     {
//         for (int i = 0; i < M_THINK_HANGUP_MAX_NUN; i++)
//         {
//             if (hangup_table[i] == NULL)
//             {
//                 /**  后续根据运行介素时间排序 ，时间短和先结束的计划放在前面*/
//                 hangup_table[i] = in_obj;
//                 stat = succ_r;
//                 break;
//             }
//         }
//         // stat =
//     }
//     return stat;
// }
