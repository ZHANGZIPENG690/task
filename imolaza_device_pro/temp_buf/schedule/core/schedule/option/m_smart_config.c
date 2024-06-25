#include "core/fset_lib.h"

#include "option.h"



struct sz {
    uint64_t smart_schedule_id; 
    uint8_t smart_zone_id[17];
};



struct sz m_smart = {0,{0}};



stat_m m_callable_zone_global_clear( void)
{
    stat_m stat = fail_r;

    return stat;
}
stat_m m_callable_zone_global_load( void)
{
    stat_m stat = fail_r;

    return stat;
}



stat_m m_callable_smartschedule_config(char * in_src , uint64_t  in_skip_flag)
{
    stat_m stat = fail_r;

    int j = 0, c = 0;
    bool ccflag = true;
    DEBUG_TEST(DB_I,"smartconfig ");
    if ((in_skip_flag == 0 || in_skip_flag == 10)  && in_src != NULL && strstr(in_src, "null") == NULL) // 说明使用智能计划
    {
        DEBUG_TEST(DB_I,"%s", in_src);
        while (1)
        {
            if (ccflag)
            {
                c = 0;
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, "%d", &c);
                ccflag = false;
                m_smart.smart_zone_id[c] = 1;
            }
            if (*in_src == '|')
            {
                ccflag = true;
            }
            in_src++;
            if (*in_src == '\0')
                break;
        }
            //  wtd->chanRunTimeSum_Clone = wtd->chanRunTimeSum; 
        // 下一步在这里
        for (int i = 0; i <= 16; i++)
        {
            // printf("-%d", wtd->chaninfo[i].enable);
            // if (wtd->chaninfo[i].chanID != 0 && wtd->chaninfo[i].chanID == sm_zone[j])
            // {
                if(m_smart.smart_zone_id[i] == 1)
                DEBUG_TEST(DB_I,"skip zone-->%d",i);
                // j++;
            //     wtd->chaninfo[i].enable = false;
            //     wtd->chanRunTimeSum -= wtd->chaninfo[i].shouldRunTime;
            // }
            // else
            //    {
            //     wtd->chaninfo[i].enable = true;
            //     // DEBUG_TEST(DB_I,"%hhd   %hhd kl %d" ,wtd->chaninfo[i].chanID  , j , sm_zone[j]);
                //  }
        }
    }
    // if (in_skip_flag != 0) // 说明使用 跳过信息 处理标记
    // {
    //     wtd->wind_water_ice_skip = in_skip_flag;
    // }
    // else
    // {
    //     if (_plan.is_backstage && !io_contr.s1_trigger && !io_contr.s2_trigger &&
    //         _plan.plan_runnning_obj != NULL && _plan.plan_runnning_obj->schedule_id != wtd->schedule_id)
    //     // 下一个计划需要运行  但是现在的计划在后台运行
    //     { // 停掉后台的计划
    //         DEBUG_TEST(DB_I,"9999");
    //         delete_time_node(_plan.plan_runnning_obj->schedule_id, 9);
    //         water_plan_end_is_Interval_hours(plan_backstage_become_silent_dele());
    //         plan_backstage_reset();
    //     }
    // }
    return stat;
}
stat_m m_callable_smartschedule_check( uint8_t in_pc)
{
    stat_m stat = succ_r;
    if(m_smart.smart_zone_id[in_pc] == 1)
        stat = fail_r;
    return stat;
}
// stat_m m_callable_zone_global_enable( uint8_t *in_pc)
// {
//     stat_m stat = fail_r;

//     return stat;
// }
stat_m m_callable_zone_global_available(m_chan_managec * in_mg , uint8_t *out_pc,uint32_t in_sack , uint16_t in_max_zone , uint64_t pre_timesp)
{
    stat_m stat = fail_r;
    uint8_t x = *out_pc;
    for (int i = 0 ;  i  <= in_max_zone ; i ++)
    {
        x = i + *out_pc;
        if(x  >= in_max_zone)
            x -= in_max_zone ;
        if(in_mg[x].enable && in_mg[x].id != 0 && m_global_zone_query(in_mg[x].id) == enable_r && 
             in_mg[x].shouldRunTime - in_mg[x].realityRunTime > 50 &&
             pre_timesp - in_mg[x].runtime.tr_end_time >= in_sack * 60) // 查询计划启用情况        
        {
            in_mg[i].id = in_mg[i].id ;
            *out_pc = x;
            DEBUG_TEST(DB_I,"should zone %hhd " , in_mg[x].id);
            stat = succ_r;
            break;
        }else
            DEBUG_TEST(DB_I,"not should zone %hhd : %d-%d(%lld)" , in_mg[x].id ,  in_mg[x].shouldRunTime , in_mg[x].realityRunTime ,in_mg[x].runtime.tr_end_time);
    }

    return stat;
}









