

#include "main.h"
#include "TestOut.h"
#include "core/notify/notify.h"

// #include </home/ornai/anaconda3/include/python3.9/Python.h>

void pt(schedule_instance_m *wobj, int pin_pc, uint64_t present_time)
{
    uint64_t nexttime = 0;
    uint32_t thistime = 0;

    if (wobj->in_running_buff.rb_pc == pin_pc)
    {
        DEBUG_OUT_TO_FILE_COLOR("\033[0;34mcurrent--> ");
        nexttime = (wobj->in_running_buff.rb_WSTime_clone + NUM_OF_DAY_SEC_SUM - /*_time.hhmmss*/ present_time) % NUM_OF_DAY_SEC_SUM;
        thistime = present_time - wobj->in_channel[pin_pc].runtime.tr_start_time;
    }
    else if (wobj->in_running_buff.rb_pre_pc == pin_pc)
    {
        DEBUG_OUT_TO_FILE_COLOR("\033[0;35m  pre  --> ");
    }
    else
    {
        DEBUG_OUT_TO_FILE_COLOR("\033[0;33m  nor  --> ");
    }

    DEBUG_OUT_TO_FILE("channel %hhd , state : %lld , nextShouldRunTime : %lld, shouldRunTimeSum : %d , thisOperationTime : %d , realityRunTime %d",

                      wobj->in_channel[pin_pc].id,
                      wobj->in_channel[pin_pc].enable,
                      nexttime,
                      wobj->in_channel[pin_pc].shouldRunTime,
                      thistime,
                      wobj->in_channel[pin_pc].realityRunTime);


}
char *_toname(int name)
{
    switch (name)
    {
    case M_FSM_EVENT_PREPARE_RUN:
        return "PREPARE_RUN 30";
        break;
    case M_FSM_EVENT_WILL_START:
        return "START 24";
        break;
    case M_FSM_EVENT_RUNNING:
        return "RUNNING ";
        break;
    case M_FSM_EVENT_SACKING:
        return "SACKING ";
        break;
    case M_FSM_EVENT_SACKING_RECOVER:
        return "SACKING_RECOVER ";
        break;
    case M_FSM_EVENT_PAUSE:
        return "PAUSE ";
        break;
    case M_FSM_EVENT_STOP:
        return "STOP ";
        break;
    case M_FSM_EVENT_IDLE:
        return "IDLE ";
        break;
    default:
        break;
    }
}

void TestPoint_ABC_schedule_out(schedule_instance_m *wobj, int event, uint64_t present_time)
{
    int a = event;
    if (event == 11)
        event = 24;
    else if (event == 12)
        event = 64;
    else if (event == 13)
        event = 64;
    else if (event == 17)
        event = 64;
    else if (event == 15)
        event = 25;

    DEBUG_OUT_TO_FILE(
        "\n---------------------------\n"
        "event id :%d (%s)\n"
        "scuedule id :%lld\n"
        "instance id :%lld\n"
        "run time Total :%d\n"
        "run time Sum :%lld\n"
        "current time :%lld\n"
        "---------------------------",
        event, _toname(a),
        wobj->in_run_id,
        wobj->in_instance_id,
        wobj->in_running_buff.chan_already_RunTimeSum,
        wobj->in_running_buff.maxTime,
        present_time);
    for (size_t i = 0; i < wobj->in_running_buff.maxChan; i++)
    {
        pt(wobj, i, present_time);
    }
    printf("\n");
}
// void create_send_data_to_server_22_06_13(int cmd, uint64_t in_schid, uint8_t stc,
//                                         uint32_t in_chanRunningSum, uint32_t in_currChanWillt,
//                                         uint8_t enc, uint32_t enct, uint32_t in_end_chanRunningt,
//                                         bool Tpause_Fstop, uint64_t in_end_plant)

// {
//     DEBUG_OUT_TO_FILE("[%d,%lld,%hhd,%d,%d,%hhd,%d,%d,%d,%lld]", cmd,  in_schid,  stc,
//                                          in_chanRunningSum,  in_currChanWillt,
//                                          enc,  enct,  in_end_chanRunningt,
//                                          Tpause_Fstop,  in_end_plant);
// }


schedule_instance_m skm;

stat_m StartSchedule(
    uint64_t in_schedule_id,
    uint64_t in_instance_id,
    uint8_t runtype,
    uint16_t pump_sack,
    uint16_t water_hammer,
    uint16_t waterTime,
    uint16_t soakTime,
    uint64_t WStime,
    uint32_t (*channel)[2],
    uint32_t (*sack_channel)[2]
    )
{
    stat_m recode = succ_r;
    static schedule_instance_m sik;
    if (VALUE_IS_INVATE(in_schedule_id) || VALUE_IS_INVATE(runtype) ||
        VALUE_IS_INVATE(pump_sack) || VALUE_IS_INVATE(waterTime) ||
        VALUE_IS_INVATE(soakTime) ||
        channel == NULL || VALUE_IS_INVATE(WStime))
        recode = fail_r;
    if (recode == succ_r)
    {
        sik.in_run_id = in_schedule_id;
        sik.WSTime = WStime;
        sik.in_running_buff.rb_WSTime_clone = WStime;
        sik.in_run_type = runtype;
        sik.in_sack_type = pump_sack; 
        sik.in_water_hammer = water_hammer;
        if (pump_sack == 1)
        {
            sik.in_sack_param_run_time = waterTime;
            sik.in_sack_param_sack_time = soakTime;
        }

        for (int i = 0; i < 16; i++)
        {
            if (channel[i][0] > 0 && channel[i][0] <= 16 && channel[i][1] > 0)
            {
                sik.in_channel[i].id = channel[i][0];
                sik.in_channel[i].shouldRunTime = channel[i][1];
                sik.in_channel[i].enable = 1;
                if( sik.in_sack_type != 3)
                {
                    sik.in_channel[i].in_sack_param_run_time = waterTime;  // 只要是浸泡这两个参数一定是有效的
                    sik.in_channel[i].in_sack_param_sack_time = soakTime; // 只要是浸泡这两个参数一定是有效的
                }else 
                {
                    sik.in_channel[i].in_sack_param_run_time = sack_channel[i][0];  // 只要是浸泡这两个参数一定是有效的
                    sik.in_channel[i].in_sack_param_sack_time = sack_channel[i][1]; // 只要是浸泡这两个参数一定是有效的
                }
                sik.in_running_buff.maxChan++;
                sik.in_running_buff.maxTime += sik.in_channel[i].shouldRunTime;
                // printf("%hhd  %lld\n", sik.in_channel[i].id, sik.in_channel[i].shouldRunTime);
            }
        }
        sik.in_instance_id = in_instance_id; // WStime + sik.in_running_buff.maxTime;

        // memcpy(&skm, &sik, sizeof(sik));
        // skm.in_running_buff.maxTime = skm.in_running_buff.maxChan * run_time;
        m_callable_add_notice_schedule_run_node(sik.in_run_id,
                                                sik.in_running_buff.rb_WSTime_clone,
                                                m_schedule__idle,
                                                &sik);
    }
    return recode;
}


stat_m ScheduleEventTriggerEncry(enum mschedule_event event_id  , uint64_t trigger_time )
{
    stat_m stat = fail_r;
    schedule_instance_m *out_inst_obj = NULL;
    // 获取正在在运行的实例
    stat = m_instance_schedule_is_running_and_get( &out_inst_obj);
    if(stat == succ_r)
    {
        stat = m_event_transit_processing_running_trigger( out_inst_obj, out_inst_obj->in_run_type , event_id , trigger_time);
    }
    return stat;
}











