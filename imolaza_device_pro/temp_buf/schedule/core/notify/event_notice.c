// #include "hot_so_fast.h"
#include "core/fset_lib.h"
#include "core/schedule/mschedule.h"
#include "core/utils/util.h"
// #include "core/net/mnet.h"

// #include "core/timer/mtime.h"
#include "notify.h"
#include "main.h"

// volatile uint64_t timc = 0;

void TestPoint_ABC_schedule_out(schedule_instance_m *wobj, int event, uint64_t present_time_hhmmss);

extern bool Variables_Interrupt_Animation;

void m_wifi_event_notice(void *arg)
// void m_wifi_event_notice(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    printf("m_wifi_event_notice %d", 0);
}
stat_m m_ext_schedule_switch_action(uint8_t in_ch, uint8_t in_ch2, uint64_t in_times)
{
    Variables_Interrupt_Animation = true;
    m_callable_contr_watering(in_ch2, true, in_times);
    put_in(STAT_KEY_RUN, M_OKK);
    put_in(STAT_IDLE, M_ERR);
    put_in(STAT_KEY, M_ERR);
    return 0;
}
stat_m m_ext_schedule_sack_show_one(uint8_t in_ch, bool on_off, uint64_t in_times)
{
    Variables_Interrupt_Animation = true;
    m_callable_contr_watering(in_ch, on_off, in_times);
    if (!on_off)
        put_in(STAT_KEY_RUN, M_ERR);

    return 0;
}
stat_m m_ext_schedule_action(uint8_t in_ch, bool on_off, uint64_t in_times)
{
    Variables_Interrupt_Animation = true;
    m_callable_contr_watering(in_ch, on_off, in_times);
    if (on_off)
    {
        put_in(STAT_KEY_RUN, M_OKK);
        put_in(STAT_KEY, M_ERR);
    }
    else
    {
        put_in(STAT_KEY_RUN, M_ERR);
        put_in(STAT_KEY, M_OKK);
    }

    put_in(STAT_IDLE, M_ERR);
    return 0;
}
void m_event_notice(int event_id, void *in_data) // 时间相关
{
    /*   时间相关事件   跳转到时间外部文件处理*/
    bool notice = false;
    // if (event_id == M_STAT_TIME_1_SEC)
    {
        notice = true;
    }
    // if (_time.tim_s % 5 == 0)
    {
        notice = true;
        // event_id = M_STAT_TIME_5_SEC;
    }
    // if (_time.tim_s % 30 == 0)
    {
        notice = true;
        // event_id = M_STAT_TIME_30_SEC;
    }
    // if (_time.tim_s % 60 == 0)
    {
        notice = true;
        // event_id = M_STAT_TIME_1_MIN;
    }

    // if(notice)
    //     m_ext_event_time_notice(event_id, in_data);
    /*   其他相关事件   */
}
stat_m m_test_ext_schedule_action(schedule_instance_m *wobj, uint8_t in_ch, bool on_off, uint64_t in_times)
{
    // if( on_off )
    //     {DEBUG_OUT_TO_FILE_COLOR("channel ( %hhd ) opened" ,in_ch);}
    // else
    //     DEBUG_OUT_TO_FILE_COLOR("channel ( %hhd ) close" ,in_ch);
    m_callable_contr_watering(in_ch, on_off, in_times);
}
stat_m m_test_ext_schedule_action_plus(schedule_instance_m *wobj, uint8_t in_ch, bool on_off, uint64_t in_times)
{
    // if( on_off )
    //     {DEBUG_OUT_TO_FILE_COLOR("channel ( %hhd ) Switch" ,in_ch);}
    // else
    //     DEBUG_OUT_TO_FILE_COLOR("channel ( %hhd ) close" ,in_ch);
    m_callable_contr_watering_plus(in_ch, on_off, in_times);
}

// 打算用他做一些驱动io，发送信息到服务器的工作   计划相关
/**
    m_schedule__prepare = 0, // 29-24
    m_schedule__start,       // x-254
    m_schedule__running,     // 24-x
    m_schedule__sack,        // x-254
    m_schedule__pause,       // x-50
    m_schedule__stop,        // x-50
    m_schedule__idle,
 */
stat_m m_ext_schedule_event_handle(int event_id, void *in_data, uint64_t present_time_hhmmss)
{
    schedule_instance_m *wobj = (schedule_instance_m *)in_data;
    DEBUG_TEST(DB_I,"m_ext_schedule_event_handle %d :(%lld)", event_id, wobj->in_run_id);
    int DYNAMIC_CMD_RESP = 0;
    // present_time_hhmmss %= NUM_OF_DAY_SEC_SUM;
    {
        /** 在这里的时候区分计划和快速运行*/
        // if(wobj->in_run_type == 0) // 计划
        // if(wobj->in_run_type == 1) // 快速运行
    }
    int cmd_cb = 0;
    // if(event_id != M_FSM_EVENT_WATER_HAMMER)
    //     TestPoint_ABC_schedule_out(wobj, event_id, present_time_hhmmss);

    switch (event_id)
    {
    case M_FSM_EVENT_PREPARE_RUN:
    case M_FSM_EVENT_WILL_START:
        // DEBUG_E("schedule start %lld *(%lld)*" , wobj->in_run_id , wobj->in_instance_id);
        if (!wobj->in_hangup)
        {
            if(wobj->in_run_type == RUN_TYPE_FAST_RUN)
                DYNAMIC_CMD_RESP = M_CMD_TWOWAY_FASTRUN_START;
            else
                DYNAMIC_CMD_RESP = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_START;
            create_send_data_to_server_22_06_13_plan_start(DYNAMIC_CMD_RESP,
                                                           wobj->in_run_id,
                                                           wobj->in_channel[wobj->in_running_buff.rb_pc].id,
                                                           wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime,                                                                                    // 区域应该运行时间
                                                           (wobj->in_running_buff.rb_WSTime_clone + NUM_OF_DAY_SEC_SUM - /*_time.hhmmss */ present_time_hhmmss % NUM_OF_DAY_SEC_SUM) % NUM_OF_DAY_SEC_SUM, // 区域本次应该运行时间
                                                           wobj->in_running_buff.maxTime,                                                                                                                  // 计划本次应该运行总时间
                                                           wobj->in_running_buff.current_running_time_region.tr_end_time, present_time_hhmmss);                                                            // 计划结束时间 (UTC)
            m_test_ext_schedule_action(wobj, wobj->in_channel[wobj->in_running_buff.rb_pc].id, true, present_time_hhmmss);
        }

        break;
    case M_FSM_EVENT_RUNNING:
        // DEBUG_E("schedule running %lld *(%lld)*" , wobj->in_run_id , wobj->in_instance_id);
        // DEBUG_TEST(DB_W,"%lld" , wobj->in_running_buff.rb_WSTime_clone );
        // [64,开始计划ID,开始通道,开始通道运行总时长, 开始通道本次将运行时间,结束通道,结束通道累积运行时间,结束通道本次运行时间,通道结束状态,计划ID结束时间,时间戳]
        if (!wobj->in_hangup)
        {
        if(wobj->in_run_type == RUN_TYPE_FAST_RUN)
            DYNAMIC_CMD_RESP = M_CMD_TWOWAY_FASTRUN_CHANGE_STATUS;
        else
            DYNAMIC_CMD_RESP = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH;
            create_send_data_to_server_22_06_13(DYNAMIC_CMD_RESP,
                                                wobj->in_run_id,
                                                wobj->in_channel[wobj->in_running_buff.rb_pc].id,
                                                wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime,
                                                (wobj->in_running_buff.rb_WSTime_clone + NUM_OF_DAY_SEC_SUM - /*_time.hhmmss*/ present_time_hhmmss % NUM_OF_DAY_SEC_SUM) % NUM_OF_DAY_SEC_SUM,
                                                wobj->in_channel[wobj->in_running_buff.rb_pre_pc].id,
                                                present_time_hhmmss - wobj->in_channel[wobj->in_running_buff.rb_pre_pc].runtime.tr_start_time,
                                                wobj->in_channel[wobj->in_running_buff.rb_pre_pc].realityRunTime,
                                                true,
                                                wobj->in_running_buff.current_running_time_region.tr_end_time,
                                                present_time_hhmmss);
            m_test_ext_schedule_action(wobj, wobj->in_channel[wobj->in_running_buff.rb_pc].id, true, present_time_hhmmss);
        }
        break;
    case M_FSM_EVENT_SACKING:
        if (!wobj->in_hangup)
        {
        if(wobj->in_run_type == RUN_TYPE_FAST_RUN)
            DYNAMIC_CMD_RESP = M_CMD_TWOWAY_FASTRUN_CHANGE_STATUS;
        else
            DYNAMIC_CMD_RESP = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH;
            create_send_data_to_server_22_06_13(DYNAMIC_CMD_RESP,
                                                wobj->in_run_id,
                                                0,
                                                0,
                                                0,
                                                wobj->in_channel[wobj->in_running_buff.rb_pre_pc].id,
                                                present_time_hhmmss - wobj->in_channel[wobj->in_running_buff.rb_pre_pc].runtime.tr_start_time,
                                                wobj->in_channel[wobj->in_running_buff.rb_pre_pc].realityRunTime,
                                                true,
                                                wobj->in_running_buff.current_running_time_region.tr_end_time,
                                                present_time_hhmmss);   
            m_test_ext_schedule_action(wobj, wobj->in_channel[wobj->in_running_buff.rb_pre_pc].id, false, present_time_hhmmss);
        }
        break;
    case M_FSM_EVENT_SACKING_RECOVER:
        // DEBUG_E("schedule re sack %lld *(%lld)*" , wobj->in_run_id , wobj->in_instance_id);

        // // [64,开始计划ID,开始通道,开始通道运行总时长, 开始通道本次将运行时间,结束通道,结束通道累积运行时间,结束通道本次运行时间,通道结束状态,计划ID结束时间,时间戳]
        if (!wobj->in_hangup)
        {
        if(wobj->in_run_type == RUN_TYPE_FAST_RUN)
            DYNAMIC_CMD_RESP = M_CMD_TWOWAY_FASTRUN_CHANGE_STATUS;
        else
            DYNAMIC_CMD_RESP = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH;
            create_send_data_to_server_22_06_13(DYNAMIC_CMD_RESP,
                                                wobj->in_run_id,
                                                wobj->in_channel[wobj->in_running_buff.rb_pc].id,
                                                wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime,
                                                (wobj->in_running_buff.rb_WSTime_clone + NUM_OF_DAY_SEC_SUM - /*_time.hhmmss*/ present_time_hhmmss % NUM_OF_DAY_SEC_SUM) % NUM_OF_DAY_SEC_SUM,
                                                0,
                                                0,
                                                0,
                                                true,
                                                wobj->in_running_buff.current_running_time_region.tr_end_time,
                                                present_time_hhmmss);
            m_test_ext_schedule_action(wobj, wobj->in_channel[wobj->in_running_buff.rb_pc].id, true, present_time_hhmmss);
        }
        break;
    case M_FSM_EVENT_WATER_HAMMER:
        if (!wobj->in_hangup)
        {
            m_test_ext_schedule_action_plus(wobj, wobj->in_channel[wobj->in_running_buff.rb_next_pc].id, true, present_time_hhmmss); // 无条件加持一个区域
            DEBUG_TEST(DB_W,"M_FSM_EVENT_WATER_HAMMER (m_test_ext_schedule_action_plus)");
        }
        break;
    case M_FSM_EVENT_PAUSE:
        break;
    case M_FSM_EVENT_HANGUP:
        if (!wobj->in_hangup)
        {
            create_send_data_to_server_22_06_13(M_CMD_TWOWAY_SENSOR_TRIGE,
                                                wobj->in_run_id,
                                                wobj->in_channel[wobj->in_running_buff.rb_pc].id,
                                                wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime,
                                                (wobj->in_running_buff.rb_WSTime_clone + NUM_OF_DAY_SEC_SUM - /*_time.hhmmss*/ present_time_hhmmss % NUM_OF_DAY_SEC_SUM) % NUM_OF_DAY_SEC_SUM,
                                                wobj->in_channel[wobj->in_running_buff.rb_pre_pc].id,
                                                present_time_hhmmss - wobj->in_channel[wobj->in_running_buff.rb_pre_pc].runtime.tr_start_time,
                                                wobj->in_channel[wobj->in_running_buff.rb_pre_pc].realityRunTime,
                                                true,
                                                wobj->in_running_buff.current_running_time_region.tr_end_time,
                                                present_time_hhmmss);
            m_test_ext_schedule_action(wobj, wobj->in_channel[wobj->in_running_buff.rb_pc].id, false, present_time_hhmmss);
        }
        break;
    case M_FSM_EVENT_HANGUP_RECOVER:
        if (!wobj->in_hangup)
        {
            create_send_data_to_server_22_06_13(M_CMD_TWOWAY_SENSOR_LEAVE,
                                                wobj->in_run_id,
                                                wobj->in_channel[wobj->in_running_buff.rb_pc].id,
                                                wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime,
                                                (wobj->in_running_buff.rb_WSTime_clone + NUM_OF_DAY_SEC_SUM - /*_time.hhmmss*/ present_time_hhmmss % NUM_OF_DAY_SEC_SUM) % NUM_OF_DAY_SEC_SUM,
                                                wobj->in_channel[wobj->in_running_buff.rb_pre_pc].id,
                                                present_time_hhmmss - wobj->in_channel[wobj->in_running_buff.rb_pre_pc].runtime.tr_start_time,
                                                wobj->in_channel[wobj->in_running_buff.rb_pre_pc].realityRunTime,
                                                true,
                                                wobj->in_running_buff.current_running_time_region.tr_end_time,
                                                present_time_hhmmss);
            m_test_ext_schedule_action(wobj, wobj->in_channel[wobj->in_running_buff.rb_pc].id, false, present_time_hhmmss);

            if (wobj->in_running_buff.rb_state == M_FSM_EVENT_RUNNING)
                m_test_ext_schedule_action(wobj, wobj->in_channel[wobj->in_running_buff.rb_pc].id, true, present_time_hhmmss);
        }
        break;
    case M_FSM_EVENT_STOP:
        //  //  25
        if (!wobj->in_hangup)
        {
            if(wobj->in_run_type == RUN_TYPE_FAST_RUN)
                DYNAMIC_CMD_RESP = M_CMD_TWOWAY_FASTRUN_COMPLETE;
            else
                DYNAMIC_CMD_RESP = M_CMD_NOTIFY_TO_SERVER_SCHEDULE_END;
            create_send_data_to_server_22_06_13_plan_start(DYNAMIC_CMD_RESP,
                                                           wobj->in_run_id,                                                          // 计划id
                                                           wobj->in_channel[wobj->in_running_buff.rb_pc].id,                         // 区域id
                                                           wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime,             // 已经运行时间
                                                           wobj->in_channel[wobj->in_running_buff.rb_pc].cruuent_round_running_time, // 当前轮回运行时间，就是刚刚
                                                           wobj->in_running_buff.current_running_time_region.tr_end_time,            // 计划结束时间
                                                           wobj->in_running_buff.chan_already_RunTimeSum,
                                                           present_time_hhmmss); // 已经运行总时间
            m_test_ext_schedule_action(wobj, wobj->in_channel[wobj->in_running_buff.rb_pc].id, false, present_time_hhmmss);
        }
        // m_callable_light_auto_idle();
        // DEBUG_E("schedule stop %lld *(%lld)*" , wobj->in_run_id , wobj->in_instance_id);

        break;
    case M_FSM_EVENT_IDLE:
        break;

    default:
        break;
    }

    return 0;
}
/*
{
        计划id
        实例id
        当前时间
        事件类型：
            计划开始、计划区域切换、计划浸泡、浸泡恢复、计划结束

        累计运行时间

        区域信息 ·「16」：{
            （当前）区域id 、总运行时间、本次已经运行时间、区域已经运行时间、

            （上一个）区域id 、总运行时间、本次已经运行时间、区域已经运行时间、

            （其他）区域id 、总运行时间、本次已经运行时间、区域已经运行时间、
            。。。。。
        }
        水锤

        电磁阀信息   开/关
        {0,1,0,1,0,0,0,0}
        灯光信息（灯光状态）
            运行、浸泡、暂停、停止。
}
*/
