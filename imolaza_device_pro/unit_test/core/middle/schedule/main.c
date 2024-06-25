

#include "main.h"

stat_m m_static_smart_running_analyse_cha0r(uint32_t *p1, uint32_t (*p2)[2], char *src);

void main()
{
    DEBUG_TEST( DB_I,"Start ..\n");

    { // waterdelay 测试
      // m_callable_water_delay_init();
      // m_callable_water_delay_set(102323, 1685850);
      // if (m_callable_water_delay_current_is_effective(102323) == succ_r)
      // {
      //     DEBUG_TEST( DB_I,"可以");
      // }
    }

    { // schedule skip 测试
      // m_callable_schedule_skip_init();
      // m_callable_schedule_skip_set(888, 102323, 1685850);
      // if (m_callable_schedule_skip_current_is_effective(880, 102325) == succ_r)
      // {
      // DEBUG_TEST( DB_I,"可以");
      // }
    }
    {
        /** 智能计划测试 */
        // uint32_t p1[30] = {0};
        // uint32_t p2[30][2] = {0};
        // char src[100] = "5|6|8|15|20|14,1#20;2#5;15#100;6#2000;30#9";
        // int sch_id = 555;
        // int zone  = 8;
        // /** 给一个计划设置智能参数 */
        // if(m_callable_smart_running_storage_append(sch_id, src, 0) != succ_r)
        //     DEBUG_TEST( DB_I,"智能计划添加失败");
        // /** 快用的时候 查一下 */
        // if (m_callable_smart_running_storage_is_ignore(sch_id ,zone ,0) == fail_r)
        //     DEBUG_TEST( DB_I,"区域%d被忽略了" ,zone );
        // else
        //     DEBUG_TEST( DB_I,"区域%d没有忽略" , zone);

        // int relx = 0;
        // zone = 8;
        // // 查看智能计划有没有被调整
        // if(m_callable_smart_running_storage_is_change(sch_id , zone ,&relx  , 0) == succ_r)
        // {
        //     DEBUG_TEST( DB_I,"%d区域有修改%d" ,zone, relx);
        // }
    }

    DEBUG_TEST( DB_I,"Done ..\n");
}

// stat_m m_static_smart_running_analyse_cha0r(uint32_t *p1, uint32_t (*p2)[2], char *src)
// {
//     stat_m stat = fail_r;
//     bool ccflag = true;
//     int c = 0;
//     for (int i = 0; i < 300; i++)
//     {
//         if (ccflag)
//         {
//             M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(src, "%d", &p1[c]);
//             c++;
//             ccflag = false;
//         }
//         if (*src == '|')
//             ccflag = true;
//         else if (*src == ',' || *src == '\0')
//             break;
//         else
//             *src = ' ';
//         src++;
//     }
//     if (*src == ',')
//         *src = ' ';
//     ccflag = true;
//     c = 0;
//     for (int i = 0; i < 300; i++)
//     {
//         if (ccflag)
//         {
//             M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(src, "%d#%d", &p2[c][0], &p2[c][1]);
//             c++;
//             ccflag = false;
//         }
//         if (*src == ';')
//             ccflag = true;
//         else if (*src == ',' || *src == '\0')
//             break;
//         else
//             *src = ' ';
//         src++;
//     }
//     return stat;
// }
