

#include "mschedule.h"


schedule_instance_m *current_inst_obj = NULL;


stat_m m_instance_schedule_gener_obj_to_obj(m_watering_schedule *in_wobj, schedule_instance_m *out_inst_obj)
{
    stat_m stat = succ_r;

    out_inst_obj->in_instance_id = in_wobj->sch_info.cu_id;
    out_inst_obj->in_run_id = in_wobj->sch_info.cu_id;
    out_inst_obj->in_run_type = 1;
    out_inst_obj->in_sack_type = 1;
    out_inst_obj->in_sack_param_run_time = 1;
    out_inst_obj->in_sack_param_sack_time = 1;
    out_inst_obj->in_instance_id = in_wobj->running.current_running_time_region.tr_end_time;
    out_inst_obj->mobj = in_wobj;
    // out_inst_obj.in_channel[0] =
    //     {
    //         .id = channel_id,
    //         .shouldRunTime = run_time,
    //     },
    // out_inst_obj.in_running_buff = {
    //     .rb_WSTime_clone = wstime,
    // }};
    return stat;
}


stat_m m_instance_schedule_set( schedule_instance_m *in_inst_obj)
{
    stat_m stat = fail_r;
    if(in_inst_obj != NULL)
        stat = succ_r;
    current_inst_obj = in_inst_obj;

    return stat;
}
stat_m m_instance_schedule_is_running_and_get( schedule_instance_m **out_inst_obj)
{
    stat_m stat = fail_r;
    if(current_inst_obj != NULL)
        stat = succ_r;
    *out_inst_obj = current_inst_obj;
    return stat;
}







