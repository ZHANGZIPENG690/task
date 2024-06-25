
#include "core/fset_lib.h"
#include "core/utils/util.h"
// #include "core/timer/mtime.h"
#include "core/notify/notify.h"
#include "option.h"

stat_m m_operation_schedule_next_running_time(m_watering_schedule *in_Obj, uint64_t present_hhmmss)
{
    stat_m state = succ_r;
    // 停掉和io操作不在这里
    // 1、判断是不是小时间隔和新草坪  拿到下一次运行时间 -> 普通计划直接是时间
    // 2、修改时间节点
    // 3、计划清理，运行数据 ， 清除计划运行Object
    // 4、通知计划挂起恢复

    DEBUG_TEST(DB_I,"m_operation_schedule_next_running_time [%lld]" , present_hhmmss);
    if(in_Obj->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
    {
        in_Obj->WSTime += in_Obj->sch_info.cu_interval_hour_or_day * 20 * 60;
        in_Obj->WSTime %= NUM_OF_DAY_SEC_SUM;
    }
    if(in_Obj->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_NEW_LAWN)
    {

    }
    in_Obj->running.rb_WSTime_clone =  in_Obj->WSTime;
    // 2
    m_change_schedule_run_notice_node(in_Obj->sch_info.cu_id , in_Obj->WSTime , m_schedule__idle );

    return state;
}

stat_m m_operation_sole_recover_schedule_time_and_status()
{
    stat_m state = succ_r;

    return state;
}
stat_m m_operation_normal_recover_schedule_time_and_status(m_watering_schedule *in_Obj, uint32_t present_time)
{
    stat_m re_state = not_should_r;
    uint32_t dicpt = present_time - in_Obj->running.rb_WSTime_clone, count = 0;

    if (present_time < in_Obj->running.rb_WSTime_clone) // 现在的时间小于开始执行的时间
        return re_state;

    if (!in_Obj->exfun.fun_soak_use || in_Obj->exfun.fun_soak_water == 0 ||
        in_Obj->exfun.fun_soak_cycle == 0) //  没有使用浸泡
    {
        for (uint8_t i = 0; i < in_Obj->exfun.maxChan; i++) // 最大时间的区域计算
        {
            in_Obj->cmg_channel[i].realityRunTime = 0;
            count += in_Obj->cmg_channel[i].shouldRunTime;                            // 机上当前第一个区域的运行时间
            in_Obj->sch_info.cu_run_time_sum += in_Obj->cmg_channel[i].shouldRunTime; // 机上当前第一个区域的运行时间

            if (dicpt < count && dicpt > 0)
            {
                in_Obj->running.rb_pc = i;
                in_Obj->running.rb_WSTime_clone += count;
                in_Obj->cmg_channel[i].realityRunTime = (uint32_t)(((in_Obj->cmg_channel[i].shouldRunTime - (in_Obj->running.rb_WSTime_clone - present_time)) / 10) * 10);
                DEBUG_TEST(DB_W,"realityRunTime : %d", in_Obj->cmg_channel[i].realityRunTime);
                // _plan.plan_runnning_obj = in_Obj;
                re_state = should_running_r;
                return re_state;
            }
            // temp_save_plan_channel_running_time(in_Obj->schedule_id, i, &in_Obj->chaninfo[i].realityRunTime, false);
            in_Obj->cmg_channel[i].realityRunTime = in_Obj->cmg_channel[i].shouldRunTime;
            // DEBUG_TEST(DB_I,"channel %hhd , next tim?e :%lld", i, in_water->WSTime_clone + count);
        }
    }
    // DEBUG_TEST(DB_I,"result = %s", TransStrState(re_state));
    in_Obj->sch_info.cu_run_time_sum /= 60;
    return re_state;
}
stat_m m_operation_sack_recover_schedule_time_and_status(m_watering_schedule *waterObj, int sockWater,
                                                         int sockTime, uint32_t *tempChanWater, int waterSum, uint32_t present_time, uint32_t *out_running_time)
// 浸泡浇水时间 浸泡时间、总的浇水时间、最大通道数
{
    stat_m result = unknow_r;
    DEBUG_TEST(DB_I,"m_schedule_operation_time_and_status ( %d&%d ) " ,sockWater , sockTime );

    int presentMaxChan = 0, finalWaterSum = 0;
    int runtineTemparr[16] = {0};
    int dist = 0, pc = -1;
    int sct = 0;

    // 计算现在的时间 ，
    if (waterObj != NULL) // 关于dist 时间转换成秒
    {
        waterObj->running.rb_WSTime_clone = waterObj->WSTime;
        dist = present_time - waterObj->WSTime;

        if (dist <= 0)
        {
            result = not_should_r;
        }
        if (dist > 0)
            dist += 1;
        // DEBUG_TEST(DB_W,"dist :\033[0;30m%d", dist);
        DEBUG_TEST(DB_W,"dist :\033[0;31m%d\033[0;", dist);
    }

    // printf("\r\n第 %d \r\n", sockWater);
    // printf("\r\n第 %d \r\n", sockTime);
    for (int j = 0; j < waterSum; j++)
    {
        // printf("\r\n第 %d 次统计\r\n", j);
        for (int n = 0; n < 8;)//_device.hardware.type;)
        {
            if (tempChanWater[n] > 0)
            {
                presentMaxChan++;
                if (tempChanWater[n] > 0)
                {
                    if (finalWaterSum - runtineTemparr[n] >= sockTime || runtineTemparr[n] == 0) // 用现在的时间 - 上一次运行的时间 判断是不是应该浸泡 应该浸泡多长时间     应该浇水还是应该浸泡
                    {
                        if (tempChanWater[n] >= sockWater)
                        {
                            // printf("\t区域 %d 浇水 %d (%d , %d)   \r\n", n + 1, sockWater , runtineTemparr[n] , finalWaterSum);
                            tempChanWater[n] -= sockWater;
                            finalWaterSum += sockWater;
                            runtineTemparr[n] = finalWaterSum;
                            { // ---------------------------------------------------
                                if (waterObj != NULL && pc == -1 && dist > 0)
                                {
                                    // DEBUG_TEST(DB_I,"finalWaterSum : %d", finalWaterSum);
                                    if (result == unknow_r)
                                    {
                                        waterObj->running.rb_WSTime_clone += (sockWater * 60);
                                        // DEBUG_TEST(DB_I,"WSTime_clone : %lld", waterObj->WSTime_clone);

                                        if (dist < (sockWater * 60)) // 如果现在的结果还没出来，但是开始时间的差距小于当前应该浇水的时间和，那么取差值，计算到精确的时间
                                        {
                                            pc = n;
                                            result = should_running_r;
                                            waterObj->cmg_channel[n].realityRunTime += dist;
                                            // DEBUG_TEST(DB_I,"(%hhd) watering : %d", waterObj->chaninfo[n].chanID, waterObj->chaninfo[n].realityRunTime);
                                        }
                                        else if (dist >= (sockWater * 60)) // 之间的差值大于当前的 ， 直接加上应该浇水的时间和
                                        {
                                            waterObj->cmg_channel[n].realityRunTime += (sockWater * 60);
                                            // DEBUG_TEST(DB_I,"(%hhd) watering : %d", waterObj->chaninfo[n].chanID, waterObj->chaninfo[n].realityRunTime);
                                            dist -= (sockWater * 60);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            // printf("\t区域 %d 浇水 %d  \r\n", n + 1, tempChanWater[n]);
                            finalWaterSum += tempChanWater[n];
                            runtineTemparr[n] = finalWaterSum;
                            { // ---------------------------------------------------
                                if (waterObj != NULL && pc == -1 && dist > 0)
                                {
                                    if (result == unknow_r)
                                    {
                                        waterObj->running.rb_WSTime_clone += (tempChanWater[n] * 60);
                                        // DEBUG_TEST(DB_I,"WSTime_clone : %lld", waterObj->WSTime_clone);

                                        if (dist < (tempChanWater[n] * 60)) // 如果现在的结果还没出来，但是开始时间的差距小于当前应该浇水的时间和，那么取差值，计算到精确的时间
                                        {
                                            pc = n;
                                            result = should_running_r;
                                            waterObj->cmg_channel[n].realityRunTime += dist;
                                            // DEBUG_TEST(DB_I,"(%hhd) watering : %d", waterObj->chaninfo[n].chanID, waterObj->chaninfo[n].realityRunTime);
                                        }
                                        else if (dist >= (tempChanWater[n] * 60)) // 之间的差值大于当前的 ， 直接加上应该浇水的时间和
                                        {
                                            waterObj->cmg_channel[n].realityRunTime += (tempChanWater[n] * 60);
                                            // DEBUG_TEST(DB_I,"(%hhd) watering : %d", waterObj->chaninfo[n].chanID, waterObj->chaninfo[n].realityRunTime);
                                            dist -= (tempChanWater[n] * 60);
                                        }
                                    }
                                }
                            }
                            tempChanWater[n] = 0;
                        }
                        n++;
                    }
                    else
                    { // 要浸泡
                        if (tempChanWater[n] > 0)
                        {
                            sct = (sockTime - (finalWaterSum - runtineTemparr[n]));
                            if (waterObj != NULL && (pc == -1) && dist > 0)
                            {
                                waterObj->running.rb_WSTime_clone += (sct * 60);
                                // DEBUG_TEST(DB_I,"()jinapo WSTime_clone : %lld", waterObj->WSTime_clone);

                                if (result == unknow_r && (dist < sct * 60)) // 如果现在的时间差 小于 浸泡时间
                                {
                                    pc = n;
                                    result = should_sack_r; // 还应该浸泡多长时间
                                }
                                else if (dist >= sct * 60) //
                                {
                                    dist -= sct * 60;
                                }
                            }
                            // printf(" %d 浸泡 %d    \r\n", n + 1, sockTime - (finalWaterSum - runtineTemparr[n]));
                            finalWaterSum += sct;
                        }
                    }
                }
            }
            else
                n++;
        }
        if (presentMaxChan == 0)
            break;
        presentMaxChan = 0;

        // 计算多少通道浇水
        //  printf("\r\n第%d次 剩余 %d 个区域在运行 ，应该运行 %d 分钟 ， 浸泡 %d 分钟 。(%d) \r\n",j, presentMaxChan, shouldRunTime , (sock_time + fristRunTime - shouldRunTime > 0) ? sock_time + fristRunTime - shouldRunTime : 0 , WTime);
    }

    if(out_running_time != NULL)
        *out_running_time = finalWaterSum * 60;
    // if (pc != -1)
    //     waterObj->pc = pc;
    DEBUG_TEST(DB_I,"finalWaterSum :%d", finalWaterSum);
    return result;
}

/**
 * @brief 计算和执行恢复的新状态和执行操作
 *
 * @param out_plan_obj 对象
 * @param in_state  之前的状态
 * @return enum en_link_tab
 */
int m_operation_recovery_needs_to_run_or_change_the_state(m_watering_schedule *out_plan_obj, stat_m in_state)
{
    m_status st = m_schedule__running;
    if (in_state == should_sack_r)
    {
        DEBUG_TEST(DB_I,".(%lld)...(%hhd).\r\n", out_plan_obj->sch_info.cu_id, out_plan_obj->cmg_channel[out_plan_obj->running.rb_pc].id);
        // in_sta = m_schedule__sack;
        out_plan_obj->state = m_schedule__sack;
        put_in(STAT_IDLE, M_ERR);
        put_in(STAT_KEY, M_OKK);
        out_plan_obj->state = M_PLAN_SACK;
        // out_plan_obj->channel_watering_time_sum = 0;

        // if (!_plan.is_backstage && !_plan.skip)
        // {
        //     TrunOnDevice(false);
        //     showOneChannel(out_plan_obj->chaninfo[out_plan_obj->pc].chanID);
        // }
        // else if (_plan.skip)
        // {
        //     plan_backstage_become_silent_add(out_plan_obj, out_plan_obj->schedule_id, out_plan_obj->chaninfo[out_plan_obj->pc].chanID);
        // }
        // else
        //     Sensor_check(out_plan_obj, out_plan_obj->schedule_id, out_plan_obj->chaninfo[out_plan_obj->pc].chanID, _time.time_get_timestamp_t() + out_plan_obj->chanRunTimeSum);
    }
    else if (in_state == should_running_r)
    {
        // DEBUG_TEST(DB_I,".(%lld)...(%hhd).\r\n", out_plan_obj->sch_info.cu_id, out_plan_obj->chaninfo[out_plan_obj->pc].id);
        // in_sta = m_schedule__running;
        out_plan_obj->state = m_schedule__running,
        // _plan.plan_running_channel = out_plan_obj->chaninfo[out_plan_obj->pc].chanID;
            out_plan_obj->state = M_PLAN_RUNNING;

        // if (!_plan.is_backstage && !_plan.skip)
        // {
        //     TrunOnDevice(true);
        //     _plan.plan_channel(true,
        //                        out_plan_obj->schedule_id, out_plan_obj->chaninfo[out_plan_obj->pc].chanID,
        //                        out_plan_obj->chaninfo[out_plan_obj->pc].shouldRunTime);
        //     out_plan_obj->chaninfo[out_plan_obj->pc].thisOperationTime = 0;
        // }
        // else if (_plan.skip)
        // {
        //     plan_backstage_become_silent_add(out_plan_obj, out_plan_obj->schedule_id, out_plan_obj->chaninfo[out_plan_obj->pc].chanID);
        // }
        // else
        //     Sensor_check(out_plan_obj, out_plan_obj->schedule_id, out_plan_obj->chaninfo[out_plan_obj->pc].chanID, _time.time_get_timestamp_t() + out_plan_obj->chanRunTimeSum);

        // out_plan_obj->chaninfo[out_plan_obj->pc].isRun = true;
        // out_plan_obj->channel_watering_time_sum = 1;
    }
    else
    {
        // out_plan_obj->pc = 0;
        // DEBUG_W(TEMP_SYNTAGMAA_DEBUG_TASK_NOT_RUN);
    }

    return st;
}

// const char ZONE_FORMAN_END = ';';

stat_m m_operation_schedule_compute_zone_distill(m_watering_schedule *in_obj, char *format, char zseparate, char *in_src, uint32_t *out_should_time_array)
{
    stat_m stat = fail_r;
    // int len = strlen(in_src);
    int zpc = 0;
    bool get_ed = true;

    // for (int i = 0; i < len; i++)+
    memset(in_obj->cmg_channel, 0, sizeof(in_obj->cmg_channel));
    // printf("%d"  ,  sizeof(in_obj->cmg_channel));
    in_obj->exfun.max_should_runtime = 0;
    while (true)
    {
#if CONFIG_NEW_CHANNEL_INFO

#else
        if (get_ed && in_src != NULL)
        {
            // printf("--> %s  %s (%d)\n", in_src, format, zpc);
            // in_obj->cmg_channel[zpc ].id = 1, in_obj->cmg_channel[3].shouldRunTime = 5;
            if (in_obj->cmg_channel[zpc].shouldRunTime != 0)
                printf("6+++++++++++5++5++5+5555++");
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, &in_obj->cmg_channel[zpc].id, &in_obj->cmg_channel[zpc].shouldRunTime);
            if (in_obj->cmg_channel[zpc].id > 0 && in_obj->cmg_channel[zpc].shouldRunTime > 0)
            {
                in_obj->cmg_channel[zpc].enable = true;
                in_obj->cmg_channel[zpc].realityRunTime = 0;
                in_obj->cmg_channel[zpc].shouldRunTime *= 60;
                in_obj->exfun.max_should_runtime += in_obj->cmg_channel[zpc].shouldRunTime;
                if (out_should_time_array != NULL)
                    out_should_time_array[zpc] = in_obj->cmg_channel[zpc].shouldRunTime / 60;
                DEBUG_TEST(DB_I,"..shouldRunTime: %d", in_obj->cmg_channel[zpc].shouldRunTime);
                zpc += 1;
                get_ed = false;
                stat = succ_r;
            }
            // else
            //     break;
        }
        in_src++;
        if (*in_src == zseparate)
            get_ed = true, in_src++;

        if (*in_src == '\0')
            break;
#endif
    }
    in_obj->exfun.maxChan = zpc;
    // M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, arg1, arg2);
    DEBUG_TEST(DB_I,"max_should_runtime... %d", in_obj->exfun.max_should_runtime);
    return stat;
}

/**
 * @brief 根据计划开始时间和计算间隔5小时的开始时间
 *
 * @param in_present_time 传入现在的时间点
 * @param in_present_timespeam 传入现在的时间戳
 * @param inver_hour 运行的时间
 * @param planTimesp 计划开始时间戳
 * @param [out] out_waterTime 计算得出的结果存放
 */
void m_operation_schedule_calculate_the_interval_hour(uint32_t in_present_time, uint32_t in_present_timespeam, uint8_t inver_hour, uint64_t planTimesp, uint64_t *out_waterTime) // 传入计划的时间戳 ，
{
    DEBUG_TEST(DB_I,"m_operation_schedule_calculate_the_interval_hour %hhd" , inver_hour);
    int tcc = inver_hour * 3600;
    tcc %= NUM_OF_DAY_SEC_SUM;
    if (tcc == 18000)
    { /** 现在的时间大于计划开始时间*/
        if (in_present_timespeam > planTimesp)
        {
            DEBUG_TEST(DB_W,"*waterTime :%d ", in_present_timespeam);
            DEBUG_TEST(DB_W,"inHour :%lld ", planTimesp);

            uint64_t vsTemp = *out_waterTime % 3600;
            uint64_t inHour = (uint64_t)(((in_present_timespeam - (planTimesp + ((*out_waterTime) / 3600) * 3600))) / 3600);

            if (inHour % 5 == 0)
                *out_waterTime = ((in_present_time / 3600)) * 3600 + vsTemp;
            else
                inHour %= 5, *out_waterTime = ((in_present_time / 3600) + (5 - inHour)) * 3600 + vsTemp;

            DEBUG_TEST(DB_W,"*waterTime :%lld ", *out_waterTime);
            DEBUG_TEST(DB_W,"inHour :%lld ", inHour);

            DEBUG_TEST(DB_W,"%lld\r\n", *out_waterTime);
        }
    }
    else
    {
        DEBUG_TEST(DB_W,"..%d..%lld..%hhd..\r\n", in_present_time, *out_waterTime, tcc);
        if (in_present_time > *out_waterTime)
        {
            while ((*out_waterTime) < in_present_time)
                *out_waterTime = ((*out_waterTime + tcc));
        }
        else
            while ((*out_waterTime) - in_present_time > tcc)
                *out_waterTime = ((*out_waterTime - tcc));
        // DEBUG_TEST(DB_W,"...\r\n");
        *out_waterTime %= NUM_OF_DAY_SEC_SUM;
    }
}

// 计算本次计划开始的时间，时间戳
uint64_t m_operation_schedule_calculate_near_start_timesp(uint64_t in_start_time, uint32_t in_wstime, uint32_t watert_time_sum , uint64_t *in_schedule_start)
{
    // 知道加护本次最近运行的时间点但是不知道最近的时间戳，需要计算一下
    DEBUG_TEST(DB_I,"compute_plan_last_start_timestamp");
    uint64_t result = 0;
    if (*in_schedule_start != 0)
        return in_schedule_start;
    if (watert_time_sum < NUM_OF_DAY_SEC_SUM)
    {
        while ((m_callable_get_device_time_t() > (in_start_time + in_wstime + result)))
            result += NUM_OF_DAY_SEC_SUM;
        result += in_start_time + in_wstime;
    }
    else
    {
    }
    DEBUG_TEST(DB_I,"in_start_time %lld , in_wstime %d , watert_time_sum %d  result : %lld", in_start_time, in_wstime, watert_time_sum, result);
    *in_schedule_start = result;
    return result;
}

/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/**
 * @brief Count the next execution between the scheduled hours
 *
 * @param planTimesp Time stamp for the scheduled start
 * @param waterTime Current watering time buffer
 */
void m_operation_calculate_the_interval_5_hours(uint64_t planTimesp, uint64_t *waterTime, uint32_t hhmmss) // 传入计划的时间戳 ，
{
    if (/*_time.time_stamp*/ 0 > planTimesp)
    {
        // planTimesp += *waterTime;
        // uint64_t invDay = (uint64_t)((_time.time_stamp - planTimesp - 8 * 3600) / NUM_OF_DAY_SEC_SUM);
        DEBUG_TEST(DB_W,"*waterTime :%lld ", 0/*_time.time_stamp*/);
        DEBUG_TEST(DB_W,"inHour :%lld ", planTimesp);

        uint64_t vsTemp = *waterTime % 3600;
        uint64_t inHour = (uint64_t)(((/*_time.time_stamp */0- (planTimesp + ((*waterTime) / 3600) * 3600))) / 3600);
        if (inHour % 5 == 0)
            *waterTime = ((/*_time.hhmmss*/0  / 3600)) * 3600 + vsTemp;
        else
            inHour %= 5, *waterTime = ((hhmmss / 3600) + (5 - inHour)) * 3600 + vsTemp;

        DEBUG_TEST(DB_W,"*waterTime :%lld ", *waterTime);
        DEBUG_TEST(DB_W,"inHour :%lld ", inHour);

        DEBUG_TEST(DB_W,"%lld\r\n", *waterTime);
    }
}
/**  */
bool m_operation_nextruntime_interval_hour(uint64_t *wsTime_clo, uint64_t *wsTime, uint32_t hour, uint32_t hhmmss)
{
    DEBUG_TEST(DB_W,"..%d..%lld..%d..\r\n", hhmmss, *wsTime, hour);
    if(hour != 0)
    {
        if (hhmmss > *wsTime)
        {
            while ((*wsTime) < hhmmss)
                *wsTime_clo = *wsTime = ((*wsTime + hour));
        }
        else
            while ((*wsTime) - hhmmss > hour)
                *wsTime_clo = *wsTime = ((*wsTime - hour));
        // DEBUG_TEST(DB_W,"...\r\n");
        *wsTime_clo %= NUM_OF_DAY_SEC_SUM;
        *wsTime %= NUM_OF_DAY_SEC_SUM;
        // printf("%lld , %lld" ,  *wsTime_clo ,*wsTime );
    }
    return true;
}

/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/

/**
 * @brief Count the next execution between the scheduled hours
 *
 * @param in_obj source obj
 * @param in_presenttime Current time
 * @param in_reserver If there is a skip information to feed back to the serve
 */
bool m_operation_schedule_skip_checkout(m_watering_schedule *in_obj, uint64_t in_presenttime, bool in_reserver)
{
    bool result = false;
    /**休眠模式 */
    /**全局跳过 */
    /**计划跳过 */
    if (in_presenttime > in_obj->skip_range_time.tr_start_time && in_presenttime < in_obj->skip_range_time.tr_end_time)
    {
        result = false;
        if (in_reserver)
        {
            /** 发送到服务器 或者怎么样 */
        }
    }
    return result;
}
bool m_operation_date_subtract(uint64_t in_planstart, uint64_t in_planstop, uint8_t cy_day)
{
    bool boo = false;
    if (!cy_day)
    {
        DEBUG_E("Parameters of the interval time !!!!! value : %hhd", cy_day);
        boo = false;
    }
    else
    {
        // DEBUG_TEST(DB_I,"+++ : %lld", _time.diff_time(in_planstop, in_planstart));

        if (((in_planstop - in_planstart) == NUM_OF_DAY_SEC_SUM) || (((in_planstop - in_planstart) / NUM_OF_DAY_SEC_SUM) % cy_day == 0))
            boo = true;
    }
    return boo;
}
stat_m m_operation_analyze_sack_str(m_watering_schedule *waterObj, char *in_str)
{
    stat_m stat = fail_r;

    if (strstr(in_str, "&") != NULL)
    {
        stat = succ_r;
    }
    return stat;
}

char *toname(int en)
{
    if (en == m_schedule__running)
        return "m_schedule__running";
    if (en == m_schedule__prepare)
        return "m_schedule__prepare";
    if (en == m_schedule__start)
        return "m_schedule__start";
    if (en == m_schedule__stop)
        return "m_schedule__stop";
    if (en == m_schedule__idle)
        return "m_schedule__idle";
    if (en == m_schedule__sack)
        return "m_schedule__sack";

    if (en == should_running_r)
        return "should_running_r";
    if (en == should_sack_r)
        return "should_sack_r";
    if (en == not_should_r)
        return "not_should_r";

    if (en == SCHEDULE_RUN_TYPE_AT)
        return "SCHEDULE_RUN_TYPE_AT";
    if (en == SCHEDULE_RUN_TYPE_BEFROE)
        return "SCHEDULE_RUN_TYPE_BEFROE";
    if (en == SCHEDULE_RUN_TYPE_SUNSET)
        return "SCHEDULE_RUN_TYPE_SUNSET";
    if (en == SCHEDULE_RUN_TYPE_SUNRISE)
        return "SCHEDULE_RUN_TYPE_SUNRISE";
    if (en == SCHEDULE_RUN_RHYTHM_SPEC_DAYS)
        return "SCHEDULE_RUN_RHYTHM_SPEC_DAYS";
    if (en == SCHEDULE_RUN_RHYTHM_EVEN_DAY)
        return "SCHEDULE_RUN_RHYTHM_EVEN_DAY";
    if (en == SCHEDULE_RUN_RHYTHM_ODD_DAY)
        return "SCHEDULE_RUN_RHYTHM_ODD_DAY";
    if (en == SCHEDULE_RUN_RHYTHM_EVERY_DAYS)
        return "SCHEDULE_RUN_RHYTHM_EVERY_DAYS";
    if (en == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
        return "SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR";
    if (en == SCHEDULE_RUN_RHYTHM_INVERTAL_DAY)
        return "SCHEDULE_RUN_RHYTHM_INVERTAL_DAY";
    return "unkown";
}

void sch_print(m_watering_schedule *water_d)
{
    DEBUG_TEST(DB_W,"-------\r\n\t\t \
            task_id : %lld\r\n\t\t \
            tsk_saveid : %lld \r\n\t\t \
            smart : %hhd\r\n\t\t \
            sack : %s\r\n\t\t \
            start time 1 : %s\r\n\t\t \
\
            available_start_time : %lld\r\n\t\t \
            available_end_time : %lld\r\n\t\t \
            desc_water_start_type : %s\r\n\t\t \
            desc_water_cycle_type : %s\r\n\t\t \
\
            week list : %s\r\n\t\t \
            interval_hour_or_day : %hhd\r\n\t\t \
            cu_run_time_sum : %d\r\n\t\t \
            state : %s\r\n\t\t \
            tag : %hhd\r\n\t\t \
            zones: %s\r\n\t\t \
            maxChan: %hhd\r\n\t\t \
            WSTime time :%lld \r\n \
            present time :%d   ----> %02dh %02dm %02ds \r\n \
            ----------\r\n",
            water_d->sch_info.cu_id,
            water_d->sch_info.cu_id,
            water_d->sch_info.cu_smart,
            water_d->sch_info.cu_sack,
            water_d->sch_info.cu_temp_param,

            water_d->sch_info.available_time.tr_start_time,
            water_d->sch_info.available_time.tr_end_time,
            toname(water_d->sch_info.cu_execute_type),
            toname(water_d->sch_info.cu_run_interval_type),

            water_d->sch_info.cu_week_list,
            water_d->sch_info.cu_interval_hour_or_day,
            water_d->sch_info.cu_run_time_sum,
            toname(water_d->state),
            water_d->running.rb_pc,
            water_d->sch_info.cu_zone,
            water_d->exfun.maxChan,
            water_d->running.rb_WSTime_clone,
            /*_time.hhmmss*/0 ,/*_time.hhmmss/3600*/0 ,/*(_time.hhmmss%3600)/60 */ 0, /**(_time.hhmmss%3600)%60*/0);
}