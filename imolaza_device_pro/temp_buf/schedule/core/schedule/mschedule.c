/*
    计划运行核心内容

    包含计划运行的相关逻辑，举个例子
        计划开始需要执行的变量和状态
        （不包含端口操作和灯光刷新，只表示计划状态运行标志更新，也不会通知服务器，有上层调用来决定上述操作 或者在运行时提供一些特殊时刻的回调函数，用户实现该函数来达到该有的目的效果。

    注意
        会有一个宏定义控制该计划的通知方式是哪一种
        关键函数以 m_eigenfunction 开头
*/

// #include "tool.h"
// #include "text.h"
#include "core/fset_lib.h"
// #include "core/timer/mtime.h"
#include "core/utils/text.h"
#include "core/utils/util.h"
#include "core/notify/notify.h"
#include "core/schedule/mschedule.h"
#include "core/schedule/option/option.h"

stat_m m_core_schedule_notify_handler(m_fsm_events in_event_id, int in_event_data_type, void *in_event_data, uint64_t in_time);

uint64_t m_calculate_next_state_switching_time(uint64_t *in_WSClone, uint16_t schoud_chanTime, uint16_t soakTime, uint16_t real_chanTime, uint32_t pre_hhmmss);

/**
    m_schedule_fsm_prepare = 0,
    m_schedule_fsm_running,
    m_schedule_fsm_sacking,
    m_schedule_fsm_will_stop,
    m_schedule_fsm_idle
 */
m_schedule_event_prepare_callback
{
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    DEBUG_TEST(DB_I,"m_action_schedule__prepare");
    if (wobj != NULL)
    {
        if (m_ext_eigenfunction_should_run(wobj, 0) == succ_r)
        {
            if (m_ext_eigenfunction_should_skip(wobj, 0) == succ_r)
            {
                // m_ext_eigenfunction_prepare(wobj, m_callable_get_device_time_t());
                create_send_data_to_server_22_06_13_plan_start(M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME,
                                                               wobj->in_run_id,                                                          // 计划id
                                                               wobj->in_channel[wobj->in_running_buff.rb_pc].id,                         // 区域id
                                                               wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime,             // 已经运行时间
                                                               wobj->in_channel[wobj->in_running_buff.rb_pc].cruuent_round_running_time, // 当前轮回运行时间，就是刚刚
                                                               wobj->in_instance_id,                                                     // 计划结束时间
                                                               wobj->in_running_buff.chan_already_RunTimeSum);
            }
        }
    }
    return 0;
}
/**
 * @brief 计划开始函数
 *
 */
m_schedule_event_start_callback
{
    m_status next_status = m_schedule__running;
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    if (wobj != NULL)
    {
        // 有没有区域需要运行 或者 区域下一次运行是那一个
        if (m_callable_zone_global_available(wobj->in_channel, &wobj->in_running_buff.rb_pc, 0, wobj->in_running_buff.maxChan, m_callable_get_device_time_t()) == succ_r &&
            m_ext_eigenfunction_should_run(wobj, /*_time.hhmmss*/ in_time & NUM_OF_DAY_SEC_SUM) == succ_r)
        {
            if (m_ext_eigenfunction_should_skip(wobj, /*_time.hhmmss*/ in_time & NUM_OF_DAY_SEC_SUM) == succ_r)
            {
                if (1) // 会停掉在运行的其他运行，
                // if (m_ext_eigenfunction_program_conflict_processing(wobj, /*_time.hhmmss*/0) == succ_r) // 会停掉在运行的其他运行，
                {
                    m_instance_schedule_set(wobj);
                    wobj->in_running_buff.rb_state = M_FSM_EVENT_RUNNING;
                    wobj->in_running_buff.current_running_time_region.tr_start_time = in_time;
                    // wobj->in_running_buff.current_running_time_region.tr_end_time = wobj->in_instance_id;
                    // wobj->in_running_buff.current_running_time_region.tr_end_time = ;
                    wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time = in_time;
                    wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time = in_time;
                    DEBUG_TEST(DB_W,"m_schedule_event_start_callback %d ", wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime);
                    //  更新上一个区域
                    wobj->in_running_buff.rb_pre_pc = wobj->in_running_buff.rb_pc;
                    // 找下一个开始运行的区域
                    // m_collide_next_zone();
                    // 计算计划开始和结束时间
                    // wobj->in_running_buff.chan_already_RunTimeSum = in_time; // m_callable_get_utc_time_t();
                    // wobj->in_running_buff.current_running_time_region.tr_start_time = in_time ; // m_callable_get_utc_time_t();
                    // wobj->in_running_buff.current_running_time_region.tr_end_time = in_time + wobj->sch_info.cu_run_time_sum;
                    // m_operation_schedule_calculate_near_start_timesp(wobj->sch_info.available_time.tr_start_time,
                    //                                                                              wobj->WSTime, wobj->sch_info.cu_run_time_sum);
                    { // 开始和计划下一次需要处理的时间计算
                        // 先算一下 下一个区域是浸泡还是运行
                        // 如果是浸泡 不找下一个运行的区域
                        // 如果不是浸泡那就找下一个运行的区域
                        // 以上如果两个 的情况都是最后一个区域的话 那就说明下一次就是结束
                        // {
                        // 如果使用了浸泡需要计算下一次需要处理的时间 ，
                        m_calculate_next_state_switching_time(&wobj->in_running_buff.rb_WSTime_clone,
                                                              wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime,
                                                              wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time,
                                                              wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime,
                                                              /*_time.hhmmss*/ in_time % NUM_OF_DAY_SEC_SUM);
                        put_in(STAT_NEED_WIFI_BREAK, M_ERR); // 打断WiFi连接动画

                        m_change_schedule_run_notice_node(wobj->in_run_id, wobj->in_running_buff.rb_WSTime_clone, next_status); // 消化状态 修改下一个节点的状态
                        wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time = in_time;

                        /** 驱动端口 */
                        if (!wobj->in_hangup)
                        { /** 通知服务器 */
                            // m_ext_schedule_has_begun();
                            m_ext_schedule_event_handle(ste, wobj, in_time);
                        };
                    }
                }
            }
        }
    }
    return 0;
}
/**
 * @brief 计划运行时
 *
 */
m_schedule_event_running_callback
{
    stat_m status = succ_r;
    m_status m_next_state = m_schedule__running;
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    uint64_t present_timesp = in_time;
    uint32_t present_time_hhmmss = /*_time.hhmmss*/ in_time & NUM_OF_DAY_SEC_SUM;
    DEBUG_TEST(DB_W,"m_action_schedule__running -- > %d", /*_time.hhmmss*/ in_time & NUM_OF_DAY_SEC_SUM);
    // uint64_t plan_start_times = m_operation_schedule_calculate_near_start_timesp(wobj->sch_info.available_time.tr_start_time, wobj->WSTime, wobj->exfun.max_should_runtime);

    if (wobj != NULL)
    {
        /*   就两个状态， 一个是运行切换，一个是浸泡开始  */
        /*   40 和 35 都是  */
        /*     */
        if (true /** 运行切换 */) // 如果是正常切换   40
        {
            wobj->in_running_buff.rb_pre_pc = wobj->in_running_buff.rb_pc;
            wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime += present_timesp - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time;
            wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time = present_timesp;
            wobj->in_running_buff.chan_already_RunTimeSum += in_time - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time; // m_callable_get_utc_time_t()
            // printf("-->%lld %lld ,%d\n" ,m_callable_get_device_time_t()  ,  wobj->in_channel[wobj->in_running_buff.rb_pc].startTime, wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime );

            // printf("pre zone %hhd(%lld)\n", wobj->in_channel[wobj->in_running_buff.rb_pc].id, wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time);
            m_callable_zone_global_next(wobj->in_channel, &wobj->in_running_buff.rb_pc, wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_sack_time, wobj->in_running_buff.maxChan, present_timesp);
            // printf("next zone %hhd\n", wobj->in_channel[wobj->in_running_buff.rb_pc].id);
            /** 如果使用浸泡的话，下一个状态可能是浸泡也有可能是浇水 */
            if (wobj->in_sack_type) // 使用浸泡
            {
                if (present_timesp - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time < (wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_sack_time))
                { /** 浸泡   1 、计算浸泡花费时间 2 、目标灯光切换 3、修改节点 4、发送服务器 5、清理和计算状态 */
                    DEBUG_TEST(DB_I,"SACK STATE current: %lld end : %lld sack_cycle :%hd", present_timesp, wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time, (wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_sack_time));
                    // int tim = present_timesp - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time ; //上一个区域到现在 也就是没有运行的时间总和，他如果大于浸泡时间就不需要浸泡 如果小于浸泡时间

                    wobj->in_running_buff.rb_WSTime_clone += ((wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_sack_time - (present_timesp - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time)));

                    // 记录这一次运行时间，
                    m_next_state = m_schedule__sack;
                    ste = M_FSM_EVENT_SACKING;
                    wobj->in_running_buff.rb_state = M_FSM_EVENT_SACKING;
                }
                else
                { /** 切换   1 、计算浸泡花费时间 2 、目标灯光切换 3、修改节点 4、发送服务器 5、清理和计算状态 */
                    int tim =
                        wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime - wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime;
                    //  present_timesp - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time;
                    DEBUG_TEST(DB_I,"RUNNING STATE current: %lld end : %lld sack_cycle :%hd (tim %d)", present_timesp, wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time, (wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_sack_time), tim);
                    if (tim >= wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time)
                        wobj->in_running_buff.rb_WSTime_clone += (((wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time)));
                    else
                        wobj->in_running_buff.rb_WSTime_clone += (((tim)));
                    wobj->in_running_buff.rb_state = M_FSM_EVENT_RUNNING;
                    m_next_state = m_schedule__running;
                }
            }
            else
            { /** 浸泡恢复-需要  单独开启一个区域，开启泵 发送数据到服务器  35 */
                // m_callable_zone_global_next(wobj->in_channel , &wobj->in_running_buff.rb_pc , wobj->in_sack_param_sack_time );
                wobj->in_running_buff.rb_WSTime_clone += ((wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime - wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime));
                m_next_state = m_schedule__running;
            }
            DEBUG_TEST(DB_I,"cu_id %lld -- tr_start_time %lld -- realityRunTime %d -> shouldRunTime %d rb_WSTime_clone %lld", wobj->in_run_id, wobj->in_running_buff.current_running_time_region.tr_start_time, wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime, wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime, wobj->in_running_buff.rb_WSTime_clone);
            wobj->in_running_buff.rb_WSTime_clone %= NUM_OF_DAY_SEC_SUM;
            m_change_schedule_run_notice_node(wobj->in_run_id, wobj->in_running_buff.rb_WSTime_clone, m_next_state);
            // 事件触发
            if (!wobj->in_hangup)
                m_ext_schedule_event_handle(wobj->in_running_buff.rb_state, wobj, in_time);
            wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time = present_timesp;
        }
    }
    return 0;
}
m_schedule_event_sack_callback
{
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;

    if (wobj != NULL)
    {
        ste = M_FSM_EVENT_SACKING_RECOVER;
        wobj->in_running_buff.rb_state = M_FSM_EVENT_RUNNING;
        // DEBUG_TEST(DB_I,"RUNNING STATE current: %lld end : %lld sack_cycle :%hd" , 0 , wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time ,(wobj->in_sack_param_sack_time * 60) );
        wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time = in_time;
        int tim = wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime - wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime;
        if (tim >= wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time)
            wobj->in_running_buff.rb_WSTime_clone += (((wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time)));
        else
            wobj->in_running_buff.rb_WSTime_clone += (((tim)));
        m_change_schedule_run_notice_node(wobj->in_run_id, wobj->in_running_buff.rb_WSTime_clone, m_schedule__running);
        m_ext_schedule_event_handle(ste, wobj, in_time);
    }
    return 0;
}
m_watering_schedule *tp_obj = 50;
m_schedule_event_stop_callback
{
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    DEBUG_TEST(DB_I,"m_schedule_event_stop_callback");
    if (wobj != NULL)
    {
        wobj->in_running_buff.rb_state = M_FSM_EVENT_IDLE;
        wobj->in_running_buff.rb_pre_pc = -1;
        wobj->in_running_buff.chan_already_RunTimeSum += in_time - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time; // m_callable_get_utc_time_t()
        // wobj->in_running_buff.chan_already_RunTimeSum += in_time - wobj->in_running_buff.chan_already_RunTimeSum; // m_callable_get_utc_time_t()
        wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime += in_time - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time;
        if (!wobj->in_hangup)
            m_ext_schedule_event_handle(ste, wobj, in_time);
        m_change_schedule_run_notice_node(wobj->in_run_id, wobj->WSTime, m_schedule__idle);
        // m_callable_schedule_list_find(wobj->in_run_id  ,&tp_obj);
        // m_operation_schedule_next_running_time(tp_obj, /*_time.hhmmss*/in_time % NUM_OF_DAY_SEC_SUM );
        // m_callable_schedule_running_buf_clear(tp_obj);
        // m_core_schedule_notify_handler(M_FSM_EVENT_IDLE , wobj ,in_time);
    }
    return 0;
}
m_schedule_event_idle_callback
{
    stat_m stat = fail_r;
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;

    if (wobj != NULL)
    {
        DEBUG_TEST(DB_I,"water_plan_end_is_Interval_hours");

        // if (/*** 是否在后台运行*/ true)
        //     TrunOnDevice(false);

        // if (wobj->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
        // {
        //     wobj->WSTime += wobj->sch_info.cu_interval_hour_or_day * 60 * 60;
        //     // DEBUG_TEST(DB_I,"interval hour Next Runtiem :%lld", item->WSTime);
        //     wobj->in_running_buff.rb_WSTime_clone = wobj->WSTime %= NUM_OF_DAY_SEC_SUM;
        // }
        // m_schedule_state_flushed(wobj);
        // // rescue_message_value(PLAN_SENSOR_SKIP_FLAG, NULL, None, M_RESCUE_MESSAGE_DEL);

        // // m_schedule_think_wakeup();

        // m_change_schedule_run_notice_node(wobj->in_run_id, wobj->in_running_buff.rb_WSTime_clone + 60 , 50);

        // // create_send_data_to_server_debug_timenode(DEVICE_DEBUG, plan_next_runtime, item->schedule_id, item->WSTime, _time.new_get_hhmmss());
        // put_in(STAT_NEED_WIFI_BREAK, M_OKK);

        // // _plan.plan_runnning_obj = NULL;
        // { /** 调整全局状态参数*/
        // }
        // _plan
    }
    return stat;
}
m_schedule_event_water_hammer_callback
{
    stat_m stat = fail_r;
    bool boo = false;
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    if (wobj != NULL && !wobj->in_hangup && wobj->in_water_hammer)
    {
        DEBUG_TEST(DB_I,"m_action_schedule__water_hammer");
        // next = wobj->in_running_buff.rb_pc;
        // curre = wobj->in_channel[next].id;
        if (m_callable_zone_global_next(wobj->in_channel, &wobj->in_running_buff.rb_next_pc,
                                        wobj->in_sack_param_sack_time, wobj->in_running_buff.maxChan, in_time) == succ_r)
        {
            if (wobj->in_sack_type == SACK_TYPE_SACK)
            {
                // The end time of the last channel was less than the soaking time, indicating the immersion     -    1020   < 180
                if (in_time - wobj->in_channel[wobj->in_running_buff.rb_next_pc].startTime < ((wobj->in_sack_param_sack_time * 60) - 10))
                {
                }
                else if (wobj->in_channel[wobj->in_running_buff.rb_next_pc].shouldRunTime - wobj->in_channel[wobj->in_running_buff.rb_next_pc].realityRunTime > 10)
                {
                    boo = true;
                }
            }
            else
            {
                boo = true;
            }
        }
        if (boo)
        {
            if (wobj->in_running_buff.rb_next_pc != wobj->in_running_buff.rb_pc)
                m_ext_schedule_event_handle(ste, wobj, in_time);
        }
    }
    return stat;
}
stat_m m_action_schedule__hangup(m_status ste, void *pdata, uint64_t in_time)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_W,"m_action_schedule__hangup");
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    stat = m_ext_schedule_event_handle(ste, wobj, in_time);
    // m_change_schedule_run_notice_node(wobj->in_run_id, wobj->WSTime, m_schedule__idle);
    wobj->in_hangup = true;
    return stat;
}
stat_m m_action_schedule__hangup_recover(m_status ste, void *pdata, uint64_t in_time)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_W,"m_action_schedule__hangup_recover");
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    stat = m_ext_schedule_event_handle(ste, wobj, in_time);

    // m_change_schedule_run_notice_node(wobj->in_run_id, wobj->WSTime, m_schedule__idle);
    wobj->in_hangup = false;
    return stat;
}















stat_m m_callable_schedule_initial()
{
    m_schedule_fsm_enable();
    return m_callable_schedule_fsm_init(m_core_schedule_notify_handler);
}

/* 计划开始执行的实现 */
stat_m m_core_schedule_notify_handler(m_fsm_events in_event_id, int in_event_data_type, void *in_event_data, uint64_t in_time)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_I,"m_core_schedule_notify_handler %d", in_event_id);

    switch (in_event_id)
    {
    case M_FSM_EVENT_PREPARE_RUN: // 通知
        stat = m_action_schedule__prepare(in_event_id, in_event_data, in_time);

        break;
    case M_FSM_EVENT_WILL_START: // 计划开始

        stat = m_action_schedule__start(in_event_id, in_event_data, in_time);

        break;
    case M_FSM_EVENT_RUNNING: // 计划运行 切换

        stat = m_action_schedule__running(in_event_id, in_event_data, in_time);
        break;
    case M_FSM_EVENT_SACKING: // 计划运行 浸泡恢复，上一次算出应该浸泡

        stat = m_action_schedule__sack_recover(in_event_id, in_event_data, in_time);

        break;

    case M_FSM_EVENT_STOP: // 其他条件或者 运行中触发暂停 ， 或者直接停止 条件是真的停止操作
        stat = m_action_schedule__stop(in_event_id, in_event_data, in_time);

        break;
    case M_FSM_EVENT_IDLE: // 清理工作 和计算下一次运行时间

        stat = m_action_schedule__idle(in_event_id, in_event_data, in_time);
        break;
    case M_FSM_EVENT_WATER_HAMMER: // 水锤消除触发
        // printf("---- >  ");
        stat = m_action_schedule__water_hammer(in_event_id, in_event_data, in_time);
        break;
    case M_FSM_EVENT_HANGUP: // 挂起事件触发
        // printf("---- >  ");
        stat = m_action_schedule__hangup(in_event_id, in_event_data, in_time);
        break;
    case M_FSM_EVENT_HANGUP_RECOVER: // 挂起恢复事件触发
        // printf("---- >  ");
        stat = m_action_schedule__hangup_recover(in_event_id, in_event_data, in_time);
        break;


        
        /**
         *
         *   在事件转换哪里 把所有的事件都分了类  这个可以去掉   直接在这里进行判断就行
         *      比如如果是计划运行  那么不处理除了停止按键的所有按键事件 （如果计划支持暂停 那么计划执行就被打乱了 ，后续加入暂停也不是不可以）
         *   记得哈
         */

    case M_FSM_EVENT_NEXT: // 其他条件 或者 运行中下一个切换 ，
        if (in_event_data_type == RUN_TYPE_FAST_RUN)
        {
            stat = m_action_schedule__switch_next(in_event_id, in_event_data, in_time);
        }
        break;
    case M_FSM_EVENT_PRE: // 其他条件 或者 运行中上一个切换 ，
        if (in_event_data_type == RUN_TYPE_FAST_RUN)
        {
            stat = m_action_schedule__switch_pre(in_event_id, in_event_data, in_time);
        }
        break;
    case M_FSM_EVENT_RECOVER: // 其他条件 或者 （运行中暂停）恢复 ，
        if (in_event_data_type == RUN_TYPE_FAST_RUN)
        {
            stat = m_action_schedule__start_recove(in_event_id, in_event_data, in_time);
        }
        break;
    case M_FSM_EVENT_PAUSE: // 其他条件 或者 运行中（暂停）按下停止按键或者指令暂停 ，
        if (in_event_data_type == RUN_TYPE_FAST_RUN)
        {
            stat = m_action_schedule__pause(in_event_id, in_event_data, in_time);
        }
        break;
    default:
        break;
    }
    return stat;
}

/*  Plan the time of the next area  */
uint64_t m_calculate_next_state_switching_time(uint64_t *in_WSClone, uint16_t schoud_chanTime, uint16_t soakTime, uint16_t real_chanTime, uint32_t pre_hhmmss)
{
    /* 计算 */
    // DEBUG_TEST(DB_W,"718 %hhd  %lld \r\n", soakTime, *in_WSClone);
    if ((soakTime) < schoud_chanTime && soakTime != 0)
    {
        if (((*in_WSClone + soakTime) - pre_hhmmss /*get_hh_mm_ss*/) > schoud_chanTime - real_chanTime)
            *in_WSClone += schoud_chanTime - real_chanTime;
        else
            *in_WSClone += soakTime;
    }
    else
        *in_WSClone += schoud_chanTime;

    *in_WSClone %= NUM_OF_DAY_SEC_SUM;

    return *in_WSClone;
}

stat_m m_callable_schedule_virtual_init(m_watering_schedule *in_info)
{
    stat_m stat = succ_r;
    char temp_ac[20] = {0};

    // uint32_t certain_day_sunset = 0;
    // uint32_t certain_day_sunrise = 0;

    int temp_var_a = 0;
    int temp_var_b = 0;
    DEBUG_TEST(DB_I,"plan_virtual_init %lld...", in_info->sch_info.cu_id);

    if (in_info->sch_info.cu_id <= 0 ||
        in_info->sch_info.available_time.tr_start_time < 1000 ||
        in_info->sch_info.available_time.tr_end_time < 1000 ||
        strlen(in_info->sch_info.cu_zone) < 2)
    {
        stat = fail_r;
    }
    if (stat != fail_r)
    {

        { /** Next : null or 1&2 (Sack) or 2|10 (Pump Mode)*/
            if (strcmp(in_info->sch_info.cu_sack, TEMP_WORD_PUBLIC_NULL) != 0)
            {
                if (strstr(in_info->sch_info.cu_sack, M_INV7) != NULL)
                {
                    in_info->exfun.fun_soak_use = true;
                    m_insi_foramt_param(M_STRM_STR_3(M_HD, "&", M_HD),
                                        &in_info->exfun.fun_soak_water,
                                        &in_info->exfun.fun_soak_cycle,
                                        in_info->sch_info.cu_sack);
                    printf("  use %s , " M_STRM_STR_3(M_HD, "&", M_HD),
                           in_info->sch_info.cu_sack, in_info->exfun.fun_soak_water, in_info->exfun.fun_soak_cycle);
                }
                else if (strstr(in_info->sch_info.cu_sack, "|") != NULL)
                {
                    in_info->exfun.fun_soak_use = false;
                    m_insi_foramt_param(M_STRM_STR_3(M_HD, "|", M_HD),
                                        &in_info->exfun.fun_solenoid_mode,
                                        &in_info->exfun.fun_solenoid_time,
                                        in_info->sch_info.cu_sack);
                }
                else
                {
                    in_info->exfun.fun_soak_use = false;
                    DEBUG_E("sch_info.cu_sack Err --> (%s)", in_info->sch_info.cu_sack);
                }
            }
            in_info->sch_info.cu_temp_param[3] = ' ';
        }
        /** Next : 100 08:00*/
        m_insi_foramt_param("%hhd %s",
                            &in_info->sch_info.cu_execute_type,
                            in_info->sch_info.cu_execute_time,
                            in_info->sch_info.cu_temp_param);
        // printf("%s...%hhd...%s ", in_info->sch_info.cu_temp_param, in_info->sch_info.cu_execute_type, in_info->sch_info.cu_execute_time);
        /** Next : 108 1*/
        /*
                {
                    SCHEDULE_RUN_TYPE_AT = 100,
                    SCHEDULE_RUN_TYPE_BEFROE,
                    SCHEDULE_RUN_TYPE_SUNSET,
                    SCHEDULE_RUN_TYPE_SUNRISE,
                };
        */

        char ZONE_FORMAN_SEPA[10] = "%hhd#%hhd";
        char ZONE_FORMAN_END = ';';
        uint32_t sh_time_arr[16] = {0}; // 获得所有区域运行时间

        /** Next : Zone info */
        // 解析及区域参数，运行时间总和 区域单数
        m_operation_schedule_compute_zone_distill(in_info,
                                                  ZONE_FORMAN_SEPA,
                                                  ZONE_FORMAN_END,
                                                  in_info->sch_info.cu_zone,
                                                  sh_time_arr);
        // 计算总花费时间
        if (in_info->exfun.fun_soak_use)
            m_operation_sack_recover_schedule_time_and_status(NULL,
                                                              in_info->exfun.fun_soak_water,
                                                              in_info->exfun.fun_soak_cycle,
                                                              sh_time_arr,
                                                              in_info->exfun.max_should_runtime,
                                                              m_callable_get_device_time_t(), &in_info->sch_info.cu_run_time_sum);
        else
            in_info->sch_info.cu_run_time_sum = in_info->exfun.max_should_runtime;

        /** Next : Determine Start Time  [sch_info.cu_execute_type] 100 - 101 103 104 */
        if (
            in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNRISE ||
            in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNSET)
        {
            /* 根据计划开始时间，计算当天的日出和日落时间*/
            // m_suninfo_specity_date(&certain_day_sunset,&certain_day_sunrise , in_info->sch_info.cu_execute_time.tr_start_time);
            in_info->WSTime = in_info->running.rb_WSTime_clone = 47000;
        }

        /** interval hour and (sunset / sunrise)  ==  AT + interval hour */
        if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNSET &&
            in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
            in_info->sch_info.cu_execute_type = SCHEDULE_RUN_TYPE_AT;

        if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNRISE &&
            in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
            in_info->sch_info.cu_execute_type = SCHEDULE_RUN_TYPE_BEFROE;

        /** Next : Determine Start Time 2 */
        // cu_execute_time
        if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_AT || in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_BEFROE)
        {
            m_insi_foramt_param("%d:%d",
                                &temp_var_a,
                                &temp_var_b,
                                in_info->sch_info.cu_execute_time);
            in_info->WSTime = in_info->running.rb_WSTime_clone = temp_var_a * 60 * 60 + temp_var_b * 60;
        }

        // m_insi_schedule_compute_zone_foramt(in_info , );
        // 假设你得到了，他的应该运行时间，那么，他的开始时间你应该也是可以得到的
        if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_BEFROE)
        {
            in_info->WSTime = in_info->running.rb_WSTime_clone - in_info->sch_info.cu_run_time_sum;
            in_info->running.rb_WSTime_clone = in_info->WSTime;
        }

        if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNSET)
        {
            in_info->WSTime = in_info->running.rb_WSTime_clone = /*_time.today_sunset*/ 0;
        }
        if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNRISE)
        {
            in_info->WSTime = in_info->running.rb_WSTime_clone = /*_time.today_sunrise*/ 0;
        }

        /** Next : Determine Start Time 2  [sch_info.cu_run_interval_type]  */
        /*
                {
                    SCHEDULE_RUN_RHYTHM_SPEC_DAYS,
                    SCHEDULE_RUN_RHYTHM_ODD_DAY,
                    SCHEDULE_RUN_RHYTHM_EVEN_DAY,
                    SCHEDULE_RUN_RHYTHM_INVERTAL_DAY,
                    SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR,
                    SCHEDULE_RUN_RHYTHM_EVERY_DAYS
                };
        */
        if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_SPEC_DAYS)
        {
            m_insi_foramt_week_c("%d", ';', in_info->exfun.week, in_info->sch_info.cu_week_list);
        }
        if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_NEW_LAWN)
        { // 如果是新草坪计划
            DEBUG_TEST(DB_I,"New Lwan Mode ...");
            in_info->running.rb_WSTime_clone = in_info->WSTime = 0;
            stat = special_newlwan_r;
        }
        if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_ODD_DAY || in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_EVEN_DAY)
        {
            DEBUG_TEST(DB_I,"ODD / EVENT ...");
        }
        if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERTAL_DAY)
        {
            DEBUG_TEST(DB_I,"Invertal Day ...");
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_week_list, "%hhd", &in_info->sch_info.cu_interval_hour_or_day);
        }
        if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
        {
            DEBUG_TEST(DB_I,"Invertal Hour ");
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_week_list, "%hhd", &in_info->sch_info.cu_interval_hour_or_day);
            if (in_info->sch_info.available_time.tr_start_time + in_info->WSTime <= m_callable_get_device_time_t())
            {
                m_operation_schedule_calculate_the_interval_hour(/*_time.hhmmss*/ 0, m_callable_get_device_time_t(),
                                                                 in_info->sch_info.cu_interval_hour_or_day, in_info->sch_info.available_time.tr_start_time, &in_info->WSTime);
                in_info->running.rb_WSTime_clone = in_info->WSTime;
            }
            // Not more than not processing  else
            // DEBUG_TEST(DB_W," cruuent :%d ,  CYCLE_HOUR_DAYS :%lld  ,%hhd ", _time.new_get_hhmmss(), in_water_item->WSTime, in_water_item->cycle_days_or_hours);
        }
        {
            DEBUG_TEST(DB_W,"rb_WSTime_clone :%lld", in_info->running.rb_WSTime_clone);
            // in_water_item->channel_state = plan_runing;
            // nvs_plan_save(in_water_item, plan_runing);

            // in_water_item->WSTime_clone = in_water_item->WSTime;
            // new_add_time_node(in_water_item->schedule_id, in_water_item->WSTime_clone, in_water_item->channel_state);
            // DEBUG_E("present time: %lld", _time.time_get_timestamp_t());
            // DEBUG_E("plan start_time: %lld", in_water_item->WSTime);
            // create_send_data_to_server(ID_RESP_ADD_PLAN_TO_DEVICE, U32, &in_water_item->schedule_id, Str, TEMP_WORD_PUBLIC_OK, null, NULL);
            // sch_print(in_info);
        }
    }

    return stat;
}
stat_m m_callable_schedule_running_buf_clear(m_watering_schedule *wobj)
{
    stat_m stat = fail_r;
    // if (wobj != NULL)
    // {
    //     wobj->in_running_buff.chan_already_RunTimeSum = 0;
    //     wobj->in_running_buff.rb_pc = 0;
    //     wobj->in_running_buff.rb_pre_pc = 0;
    //     wobj->in_running_buff.current_running_time_region.tr_end_time = 0;
    //     wobj->in_running_buff.current_running_time_region.tr_start_time = 0;
    //     wobj->in_running_buff.wind_water_ice_skip = 0;
    //     for (int i = 0; i < wobj->exfun.maxChan; i++)
    //     {
    //         wobj->in_channel[i].realityRunTime = 0;
    //         wobj->in_channel[i].has_been_power_watering_time = 0;
    //         wobj->in_channel[i].has_been_watering_time = 0;
    //         wobj->in_channel[i].runtime.tr_start_time = 0;
    //         wobj->in_channel[i].runtime.tr_end_time = 0;
    //     }
    // }

    return stat;
}

stat_m m_schedule_query_state_is_running(void)
{
    stat_m stat = fail_r;
    if (true)
    {
        stat = succ_r;
    }
    return stat;
}
stat_m m_schedule_state_flushed(m_watering_schedule *in_obj)
{
    stat_m stat = fail_r;
    if (in_obj != NULL)
    {
        in_obj->running.rb_pc = 0;
        in_obj->running.rb_WSTime_clone = 0;
        for (int i = 0; i < in_obj->exfun.maxChan; i++)
        {
            in_obj->cmg_channel[i].has_been_power_watering_time = 0;
            in_obj->cmg_channel[i].realityRunTime = 0;
            in_obj->cmg_channel[i].has_been_watering_time = 0;
            in_obj->cmg_channel[i].thisOperationTime = 0;
        }
        stat = succ_r;
    }
    return stat;
}
