// #include "core/timer/mtime.h"
#include "core/fset_lib.h"
#include "core/utils/util.h"
#include "mschedule.h"
#include "core/notify/notify.h"



#include "schedule/main.h"



#define M_SCHEDULE_ENABLE "PAE"

static char M_FLAG_BUFF[20] = {0};

// 计划运行开始类型(是不是bef) ，计划间隔类型 （是不是间隔小时）， 计划运行总需要的时间 ， 现在的时间
stat_m m_callable_today_need_run_odev(uint8_t in_run_type, uint8_t in_inva_type, uint32_t runtime_sum, uint32_t presenttime)
{
    stat_m should_run = M_ERR;

    return should_run;
}

stat_m m_callable_schedule_avliable(uint64_t in_obj_id)
{
    return 0;
}

stat_m m_callable_schedule_flag_disable(uint64_t in_obj_id)
{
    sprintf(M_FLAG_BUFF, "%s%lld", M_SCHEDULE_ENABLE, in_obj_id);
    m_callable_flash_flag_rescue_message(M_FLAG_BUFF, M_RESCUE_MESSAGE_SET);
    return 0;
}

stat_m m_callable_schedule_disable(uint64_t in_obj_id)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_I,"m_callable_schedule_disable");
    stat = m_callable_schedule_list_remove(in_obj_id);

    // m_splan_remove(in_obj_id);
    sprintf(M_FLAG_BUFF, "%s%lld", M_SCHEDULE_ENABLE, in_obj_id);
    m_callable_flash_flag_rescue_message(M_FLAG_BUFF, M_RESCUE_MESSAGE_SET);

    m_remove_schedule_run_notice_node(in_obj_id, 9);

    DEBUG_TEST(DB_I,"schedule %lld Disable Done .. ", in_obj_id);
    return stat;
}
stat_m m_callable_schedule_enable(uint64_t in_obj_id)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_I,"m_callable_schedule_enable");
    sprintf(M_FLAG_BUFF, "%s%lld", M_SCHEDULE_ENABLE, in_obj_id);
    m_callable_flash_flag_rescue_message(M_FLAG_BUFF, M_RESCUE_MESSAGE_DEL);

    // stat = m_callable_schedule_virtual_init(in_info);
    m_splan_parse_one(in_obj_id);

    DEBUG_TEST(DB_I,"schedule %lld Enable Done .. ", in_obj_id);
    return stat;
}

/**
 * 
 * 
 * ****************************************************
 * *********************************************************
 * ***************************************************************
 * *********************************************************
 * ****************************************************
 * 
 * 
*/

m_schedule_event_switch_pre_callback
{
    return m_action_schedule__switch_pre(ste, pdata, in_time);
    
}
m_schedule_event_switch_next_callback
{
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    stat_m stat = fail_r;
    bool boo = ((ste == M_FSM_EVENT_NEXT) ? true : false);
    if (wobj != NULL)
    {
        ste = M_FSM_EVENT_NEXT;
        wobj->in_running_buff.rb_state = M_FSM_EVENT_RUNNING;
        wobj->in_running_buff.rb_pre_pc = wobj->in_running_buff.rb_pc;
        stat =  m_callable_zone_quick_run_switch_channel(wobj->in_channel, &wobj->in_running_buff.rb_pc, boo, wobj->in_running_buff.maxChan, in_time) ;
        if(stat == succ_r)
        {
            // 保存状态
            wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time = in_time;
            wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime += in_time - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time;
            wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time = in_time;
            wobj->in_running_buff.chan_already_RunTimeSum += in_time - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time; // m_callable_get_utc_time_t()
         
            // 如果切换有效，调整一下时间
            int tim = wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime - wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime;
            if (tim >= wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time)
                wobj->in_running_buff.rb_WSTime_clone += (((wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time)));
            else
                wobj->in_running_buff.rb_WSTime_clone += (((tim)));
            m_change_schedule_run_notice_node(wobj->in_run_id, wobj->in_running_buff.rb_WSTime_clone, m_schedule__running);
            m_ext_schedule_event_handle(ste, wobj, in_time);        
        }
    }
    return stat;
}
m_schedule_event_switch_pause_callback 
{
    /**  暂停 
     * 1、保存状态
     * 2、保存状态
     **/
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;
    stat_m stat = fail_r;
    if (wobj != NULL)
    {
        ste = M_FSM_EVENT_NEXT;
        wobj->in_running_buff.rb_state = M_FSM_EVENT_RUNNING;
        // 保存状态
        wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time = in_time;
        wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime += in_time - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time;
        wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_end_time = in_time;
        wobj->in_running_buff.chan_already_RunTimeSum += in_time - wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time; // m_callable_get_utc_time_t()

        m_remove_schedule_run_notice_node(wobj->in_run_id, m_schedule__running);

        stat =  m_callable_zone_quick_run_switch_channel(wobj->in_channel, &wobj->in_running_buff.rb_pc, false, wobj->in_running_buff.maxChan, in_time) ;
        if(stat == succ_r)
        {
            // // 如果切换有效，调整一下时间
            // wobj->in_channel[wobj->in_running_buff.rb_pc].runtime.tr_start_time = in_time;
            // int tim = wobj->in_channel[wobj->in_running_buff.rb_pc].shouldRunTime - wobj->in_channel[wobj->in_running_buff.rb_pc].realityRunTime;
            // if (tim >= wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time)
            //     wobj->in_running_buff.rb_WSTime_clone += (((wobj->in_channel[wobj->in_running_buff.rb_pc].in_sack_param_run_time)));
            // else
            //     wobj->in_running_buff.rb_WSTime_clone += (((tim)));
            m_ext_schedule_event_handle(ste, wobj, in_time);        
        }
    }
    return stat;
}
m_schedule_event_switch_start_recove_callback
{
    
}
