/**
 * 
 * 相关的计划恢复都在这里
*/


#include "core/schedule/option/option.h"
// #include "core/timer/mtime.h"
#include "core/fset_lib.h"
#include "core/utils/util.h"
#include "mschedule.h"

struct m_re_queue
{
    uint64_t target_id;
    uint32_t comp_time;
    m_watering_schedule * target_obj;
    struct m_re_queue *pre;
    struct m_re_queue *next;

};

struct m_re_queue mreRoot = {0,0,NULL,NULL,NULL};

uint32_t sh_time_arr[16] = {0}; // 获得所有区域运行时间

stat_m m_recover_determine_scheduled_start_time();

/* 添加一个到对列*/
stat_m m_callable_recover_queue_add_one(struct m_re_queue *in_root ,m_watering_schedule * in_target_obj);

/** 获取下一个计划内容*/
stat_m m_callable_recover_queue_next_obj(struct m_re_queue *in_root ,m_watering_schedule * out_target_obj);


stat_m m_sch_forthwith_add(m_watering_schedule *ws_item , bool boo)
{
    stat_m stat = fail_r;
    // int temp_var_a = 0, temp_var_b = 0;
    m_status m_ste = m_schedule__idle;

    m_callable_schedule_virtual_init(ws_item);


    m_callable_schedule_list_add(ws_item);
    

    /** Next : Determine the scheduled start time */
    stat = m_recover_determine_scheduled_start_time(ws_item , m_callable_get_device_time_t()) ;

    /** Next : schedule recover */
    if(stat == M_OKK && boo)
    { 
        for (int i = 0; i < ws_item->exfun.maxChan; i++)
        {
                sh_time_arr[i] = ws_item->cmg_channel[i].shouldRunTime / 60;
        }
        
        // 计算总花费时间
        if(ws_item->exfun.fun_soak_use)
            m_ste =  m_operation_sack_recover_schedule_time_and_status(ws_item,
                                                                ws_item->exfun.fun_soak_water,
                                                                ws_item->exfun.fun_soak_cycle,
                                                                sh_time_arr,
                                                                ws_item->exfun.max_should_runtime,
                                                                /*_time.hhmmss*/0, &ws_item->sch_info.cu_run_time_sum);
        else if(ws_item->sch_info.cu_execute_type == SCHEDULE_RUN_RHYTHM_NEW_LAWN) // 新草坪计划
        {//找找计划开始的时间
            // m_schedule_newlwan_mode_load(ws_item->sch_info.cu_id);
            //  m_newlwan_load(new_water_d->schedule_id);
            // m_newlwan_schedule_enable(new_water_d, _time.new_get_hhmmss(), true);
        }
        else 
            ws_item->sch_info.cu_run_time_sum = ws_item->exfun.max_should_runtime;

        DEBUG_TEST(DB_I,"it schedule :\033[0;31m %s(%d)" , toname(m_ste) , m_ste);

        /** 所有需要恢复的计划都将放在这个队列里面 （这个后期我想改成挂起队列 挺好的哈哈哈）*/
        m_callable_recover_queue_add_one(&mreRoot, ws_item );
    }else DEBUG_TEST(DB_I,"No ...");

    // m_operation_normal_recover_schedule_time_and_status(ws_item , _time.hhmmss);

    m_callable_add_notice_schedule_run_node(ws_item->sch_info.cu_id, ws_item->WSTime, m_schedule__idle, ws_item);
    
    /**
     * 梳理一下逻辑 
     * 1 、找出所有计划，判断是否运行，是否应该恢复
     * 2 、放入队列，不需要排序
     * 3 、所有的计划都完了之后，找到最先应该运行的计划
     * 4 、根据当前的时间，恢复该计划
     * 
    */
    sch_print(ws_item);
    return succ_r;
}

void m_callable_recove_schedule_comple(void)
{
    DEBUG_TEST(DB_I,"m_callable_recove_schedule_comple");
    // m_callable_recover_queue_next_obj(mreRoot , );

}

/**
 * @brief After the device is called on, all the plans must go through the function, 
 *        he will determine the start time, when he returns successfully, so ,
 *        the plan needs to be restored, I will put it in a queue after all the plans are restored, I start to choose which plan to restore,
 *           perfect !
 * @param new_water_d source obj
 * @param pretime Current time 
 * @return M_ERR : Not need  OKK : Need
 */
stat_m m_recover_determine_scheduled_start_time(m_watering_schedule *new_water_d , uint64_t pretime)
{
    stat_m stat = M_ERR; // 表示需不需要恢复  
    bool biu = false; //表示本次需不需要被恢复
    // 2021-10-09:10-23-33 , 2021-11-29:19-00-00
    if ((pretime <= new_water_d->sch_info.available_time.tr_end_time && pretime >= new_water_d->sch_info.available_time.tr_start_time) ) 
    {
        // 上次被中断，不能恢复
        if(pretime >= new_water_d->deadlineofsch.tr_end_time)
        {
            // 传感器什么的不需要在这里进行
            // 喜爱面的判断包含两个方面  一个是确定开始时间   一个是确定当天需不需要运行和本次需不需要恢复
            /* 1 、 开始时间确定   */
            biu = true;
            // {   /** AT BEF SUN  */

            //     if ((new_water_d->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_AT || new_water_d->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_BEFROE))
            //     {
            //         uint8_t mm = 0, hh = 0;
            //         M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(new_water_d->sch_info.cu_temp_param, "%*d&%hhd:%hhd", &hh, &mm);
            //         new_water_d->WSTime = new_water_d->running.rb_WSTime_clone = hh * 60 * 60 + mm * 60;
            //         if(new_water_d->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_BEFROE)
            //             new_water_d->WSTime = (new_water_d->running.rb_WSTime_clone -= new_water_d->sch_info.cu_run_time_sum);
            //         DEBUG_TEST(DB_I,"AT - - BEF(%s) (%lld)", new_water_d->sch_info.cu_temp_param, new_water_d->WSTime);
            //     }
            //     /*
            //         The following questions are dedicated to solving the problem of planned recovery,
            //         on whether there is a planned restart time requiring rewatering, specifically calculated based on time and planned time
            //     */
            //     if (new_water_d->sch_info.cu_run_interval_type != SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
            //     {   /** SUNSET  SUNRISE  */
            //         if (new_water_d->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNSET)
            //         {
            //             new_water_d->running.rb_WSTime_clone = new_water_d->WSTime = (_time.today_sunset);
            //         }
            //         if (new_water_d->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNRISE)
            //         {
            //             new_water_d->running.rb_WSTime_clone = new_water_d->WSTime = (_time.today_sunrise - new_water_d->sch_info.cu_run_time_sum);
            //         }
            //     }
            //     if (new_water_d->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_SUNRISE_PLUS)
            //     {
            //         /* 待补充*/
            //     }
            //     if (new_water_d->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_SUNSET_PLUS)
            //     {
            //         /* 待补充*/
            //     }
            //     if (new_water_d->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_NEW_LAWN)
            //     {
            //         /* 待补充*/
            //     }
            //     /** 特殊 间隔小时  */
            //     if (new_water_d->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
            //     {
            //         M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF( new_water_d->sch_info.cu_week_list ,"%hhd" , &new_water_d->sch_info.cu_interval_hour_or_day );
            //         /* 需要找到 计划创建当天的日出日落时间 */
            //         if (new_water_d->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNSET)
            //         {
            //             new_water_d->running.rb_WSTime_clone = new_water_d->WSTime = (_time.today_sunset);
            //         }
            //         if (new_water_d->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNRISE)
            //         {
            //             new_water_d->running.rb_WSTime_clone = new_water_d->WSTime = (_time.today_sunrise);
            //         }
            //         //确定开始时间
            //         if (new_water_d->sch_info.cu_interval_hour_or_day == 5)
            //             m_operation_calculate_the_interval_5_hours(new_water_d->sch_info.available_time.tr_start_time, &new_water_d->WSTime , _time.hhmmss);
            //         else
            //             m_operation_nextruntime_interval_hour(&new_water_d->running.rb_WSTime_clone, &new_water_d->WSTime, 
            //                                                  ((new_water_d->sch_info.cu_interval_hour_or_day * 60 * 60) % NUM_OF_DAY_SEC_SUM) , _time.hhmmss);
            //         new_water_d->running.rb_WSTime_clone = new_water_d->WSTime;
            //     }
            // }/* time 确定*/

            {/** 需不需要运行 */

                /**
                 *  ODD  And  Even  这个确定今天需不需要运行，开始时间在前面确定了
                */
                if ((new_water_d->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_ODD_DAY ) ||
                    (new_water_d->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_EVEN_DAY ))
                    {
                        if(m_callable_today_need_run_odev(new_water_d->sch_info.cu_execute_type , 
                                                            new_water_d->sch_info.cu_run_interval_type ,
                                                            new_water_d->sch_info.cu_run_time_sum , 
                                                            /*_time.hhmmss*/0) == M_OKK)
                            biu = true;
                        else 
                            biu = false;

                    }
                if (new_water_d->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERTAL_DAY)
                {
                    biu = m_operation_date_subtract(new_water_d->sch_info.available_time.tr_start_time , /*_time.time_stamp*/0 , new_water_d->sch_info.cu_interval_hour_or_day);
                }
                if (new_water_d->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_SPEC_DAYS)
                {
                    biu = false;
                    for (size_t i = 0; i < 7; i++)
                        if(/*_time.ctime.day*/0 == new_water_d->sch_info.cu_week_list[i])
                            biu = true;
                    
                }

                if(biu ) // 如果前面那没有被档下来,那就检查一下跳过
                {
                    if(new_water_d->deadlineofsch.tr_end_time > /*_time.time_stamp*/0)
                        biu = false;

                    // biu = m_operation_schedule_skip_checkout(new_water_d , _time.time_stamp , false);

                    if(biu)// 如果都行，那就看看时间会不会超出跳过时间了
                    {
                        /* Analysis of interval hours:
                            * If the current time is greater than the start time + the total watering time, then calculate forward.
                            * If the current time is less than the start time + the total watering time, then calculate backwards.
                        */
                        // 37440 48274 120  ||  WS: 37440 , pre: 48213 , sum :120
                        
                        // 如果有计划的总时间  并且计划开始的时间加上总时间小于现在的时间  ，才会需要恢复，，
                        //（有个BUG 我用当天的点去计算的 而不是计划开始或者计划结束的时间戳，如果表示当天需要运行的话 那这没有什么问题的对吧）
                        if( ((/*_time.hhmmss*/0 + NUM_OF_DAY_SEC_SUM ) - new_water_d->running.rb_WSTime_clone) 
                                           % NUM_OF_DAY_SEC_SUM < new_water_d->sch_info.cu_run_time_sum)
                        { // 说明通过层层关卡，开始了
                            stat = M_OKK;
                        DEBUG_TEST(DB_W,"need recovr");
                        }
                    }
                }

            }/** 需不需要运行 */


        }else 
            DEBUG_TEST(DB_W,"not need recovr");
    }else
        new_water_d->state = 0;
    return stat;
}



stat_m m_callable_schedule_action_recover(void)
{
    stat_m stat = fail_r;
    uint64_t present_time = /*_time.time_stamp*/0;
    m_watering_schedule * out_target_obj = NULL;
    stat = m_callable_recover_queue_next_obj(&mreRoot ,out_target_obj);
    if(stat == M_OKK)   //  present_time
    {
        // stat : 计划应该运行的状态
        if(!out_target_obj->exfun.fun_soak_use)
        {
            stat = m_operation_normal_recover_schedule_time_and_status(out_target_obj , present_time);
        }else if(out_target_obj->exfun.fun_soak_use)
            stat =  m_operation_sack_recover_schedule_time_and_status(out_target_obj,out_target_obj->exfun.fun_soak_water , out_target_obj->exfun.fun_soak_cycle ,
                                                                         sh_time_arr, out_target_obj->exfun.max_should_runtime , present_time , NULL);
        
        // else if(out_target_obj->sch_info.cu_run_interval_type = SCHEDULE_RUN_RHYTHM_NEW_LAWN) 
        //     stat = m_operation_sole_recover_schedule_time_and_status
        m_operation_recovery_needs_to_run_or_change_the_state(out_target_obj ,stat);

    }
    return succ_r;
}



stat_m m_callable_recover_queue_add_one(struct m_re_queue *in_root ,m_watering_schedule * in_target_obj)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_I,"m_callable_recover_queue_add_one .. (%lld) " , in_target_obj->sch_info.cu_id);
    if(in_target_obj != NULL && in_root != NULL)
    {
        struct m_re_queue *trmc = in_root;
        static struct m_re_queue *new_obj = NULL;
        new_obj = M_MALLOC(sizeof(struct m_re_queue));
        new_obj->target_obj = in_target_obj;
        new_obj->target_id = in_target_obj->sch_info.cu_id;
        new_obj->comp_time = in_target_obj->running.rb_WSTime_clone;
        new_obj->next = new_obj->pre = NULL;

        if(trmc->next == NULL)
            in_root->next = new_obj , new_obj->pre = in_root;
        else 
        {
            while(trmc->next != NULL)
            {
                trmc = trmc->next;
                if(trmc->next == NULL)
                {
                    trmc->next = new_obj;
                }
            }
        }
    }
    return stat;
}
stat_m m_callable_recover_queue_next_obj(struct m_re_queue *in_root ,m_watering_schedule * out_target_obj)
{
    stat_m stat = fail_r;
    struct m_re_queue *mrq = in_root->next;
    struct m_re_queue *mrc_free = NULL;
    struct m_re_queue *mrq_pre = NULL;
    while(mrq != NULL)
    {
        mrq_pre = mrq;
        if(out_target_obj != NULL && out_target_obj->running.rb_WSTime_clone < mrq->target_obj->running.rb_WSTime_clone )
        {
            out_target_obj = mrq->target_obj;
            mrc_free = mrq;
            stat = succ_r;
        }
        mrq = mrq->next;
    }
    if(mrc_free != NULL)
    {
        mrq_pre->next = mrc_free->next;
        mrq_pre->pre = mrc_free;
        mrc_free->next = NULL;
        mrc_free->pre = NULL;
        M_FREE(mrc_free);
        mrc_free = NULL;
    }
    return stat;
}


