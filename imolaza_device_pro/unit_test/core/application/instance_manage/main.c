

#include "core/fset_lib.h"
#include "core/application/instance_manage/running/instance_running.h"
#include "core/application/instance_manage/manage/instance_manage.h"
#include "core/middle/event_distribution/event_distribution.h"

#include "main.h"

long time_s = 0;

#if !MANAGE_AND_RUNNING

void tmie_buv(long in_time_s)
{
    m_callable_instance_running_monitor(in_time_s);
    {
        m_callable_event_distribution_advancing(in_time_s);
    }
}

// void Task0(void *args)
// {
//     while (1)
//     {
//         mDelay_ms(10);
//         time_s++;
//         tmie_buv(time_s);
//         printf("%d\n", time_s);
//         if (time_s > 50000)
//             break;
//     }
// }
// stat_m (*m_ext_instance_running_able)(
//     int cmd,
//     uint64_t instance_running_id,
//     uint8_t start_channel,
//     uint32_t start_channel_time_sum,
//     uint32_t start_channel_will_running_time,
//     uint8_t end_channel,
//     uint32_t end_channel_reality_time,
//     uint32_t end_channel_current_running_time,
//     uint32_t end_channel_state,
//     uint32_t running_once_sum,
//     uint32_t running_sum,
//     uint64_t instance_id,
//     uint64_t in_time_s);

// stat_m m_ext_instance_running_able_cb(
//     int cmd,
//     uint64_t instance_running_id,
//     uint8_t start_channel,
//     uint32_t start_channel_time_sum,
//     uint32_t start_channel_will_running_time,
//     uint8_t end_channel,
//     uint32_t end_channel_reality_time,
//     uint32_t end_channel_current_running_time,
//     uint32_t end_channel_state,
//     uint32_t running_once_sum,
//     uint32_t running_sum,
//     uint64_t instance_id,
//     uint64_t in_time_s)
// {

//     printf("[%d,%lld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lld,%lld]\n", cmd, instance_running_id,
//            start_channel,
//            start_channel_time_sum,
//            start_channel_will_running_time,
//            end_channel,
//            end_channel_reality_time,
//            end_channel_current_running_time,
//            running_once_sum,
//            running_sum,
//            end_channel_state,
//            instance_id,
//            in_time_s);
// }

// void add(int (*m_ext_instance_running)(
//     int cmd,
//     uint64_t instance_running_id,
//     uint8_t start_channel,
//     uint32_t start_channel_time_sum,
//     uint32_t start_channel_will_running_time,
//     uint8_t end_channel,
//     uint32_t end_channel_reality_time,
//     uint32_t end_channel_current_running_time,
//     uint32_t end_channel_state,
//     uint32_t running_once_sum,
//     uint32_t running_sum,
//     uint64_t instance_id,
//     uint64_t in_time_s))
// {
//     m_ext_instance_running_able = m_ext_instance_running;
// }

void main()
{
    printf("Nihao... \n");

    stat_m stat = fail_r;

    //     测试计划运行逻辑的 （包括事件）
    {
        // int start_time = 100;
        // instance_doc_t in_doc = {
        //     .sch_info.cu_id = 555,
        // };
        // in_doc.exfun.fn_max_channel = 3;
        // for (size_t i = 0; i < in_doc.exfun.fn_max_channel; i++)
        // {
        //     in_doc.cmg_channel[i].cd_cahnnel_id = i + 1;
        //     in_doc.cmg_channel[i].cd_channel_type = M_STATIC_CHANNEL_TYPE_SACK; // 浸泡模式
        //     in_doc.cmg_channel[i].cd_should_run_time = 30;                      // 应该 s
        //     in_doc.cmg_channel[i].cd_sack_should_water_time = 10;               // 浇水 s
        //     in_doc.cmg_channel[i].cd_sack_should_sack_time = 30;                // 浸泡 s
        // }

        // // 测试实例运行 第一部是初始化运行实例管理
        // m_callable_instance_manage_init();
        // // 添加一个实例运行节点，到时间会自动运行
        // m_static_time_link_notece_node_add(in_doc.sch_info.cu_id, start_time, M_BASE_STATUS_IDLE, (void *)&in_doc);
        // while (1)
        // {
        //     // 测试计划事件
        //     {
        //         if (time_s == start_time + 5)
        //         {
        //             DEBUG_TEST( DB_I,"CHANGING_OVER_LEFT_TRIGGER + 5 %d " , time_s);
        //             m_callable_instance_running_event_input(CHANGING_OVER_RIGHT_TRIGGER, time_s);
        //         }
        //         if (time_s == start_time + 10)
        //         {
        //             DEBUG_TEST( DB_I,"CHANGING_OVER_LEFT_TRIGGER + 10  %d " , time_s);
        //             m_callable_instance_running_event_input(CHANGING_OVER_RIGHT_TRIGGER, time_s);
        //         }
        //         {
        //             // DEBUG_TEST( DB_I,"CHANGING_OVER_LEFT_TRIGGER + 45  ");
        //             // m_callable_instance_running_event_input(CHANGING_OVER_LEFT_TRIGGER, time_s);
        //         }
        //     }
        //     time_s++;
        //     tmie_buv(time_s);
        //     if (time_s > 300)
        //         break;
        // }
    }

    {
        // 计划添加
        /* 普通计划*/
        // char schedule_info[100] = {"814,0,2&1,1674008936,1684108936,100 15:16,1#3;2#3;3#4;4#2;5#3;6#3;7#3;8#3,108,1,1]"};
        /* 新草坪计划*/
        char schedule_info[100] = {"814,0,2&1,1674008936,1684108936,120,1#3;2#3;3#4;4#2;5#3;6#3;7#3;8#3,108,1,1]"};
        char schedule_param[40] = {"A0B0C0D0E0F0"};
        uint64_t WSTime = 0;
        m_callable_instance_manage_add(schedule_info,false, 0);
        /** 新草坪计划 添加时间*/
        m_callable_new_lawn_fix_and_add_time(814, schedule_param, &WSTime, 0);
        printf("%lld\n", WSTime);
    }
}

#else
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

void mai0n()
{
    printf("Nihao... \n");

    stat_m stat = fail_r;
    char schedule_str[80] = "522,1,1&1,1652000,1655445588,100 20:30,2#20;3#10,108,1,1,]\0";
    printf("%s\n", schedule_str);
    m_callable_instance_manage_add(schedule_str, true, 10);
    // while (1)
    // {
    //     // scanf("%d", &event);
    //     // mDelay_ms(10);

    // }
}

#endif