#include "core/fset_lib.h"
#include "option.h"

struct m_zone_able mz_able = {{"PZ01", "PZ02", "PZ03", "PZ04", "PZ05", "PZ06", "PZ07", "PZ08", "PZ09", "PZ10", "PZ11", "PZ12", "PZ13", "PZ14", "PZ15", "PZ16"}, {0}};

GLOBAL_OPTION _option;

// extern struct m_io_contr io_contr;

// extern plan_manager _plan;
// extern int power_break_restore_wait;

// uint8_t sm_zone[16] = {0}; // 智能计划的区域禁用
// // static int gd_zone[16] = {0}; // 全局的区域禁用

// extern uint64_t compute_plan_last_start_timestamp(uint64_t, uint32_t, uint32_t);

// /*   主要是配置计划的区域智能信息 、 和 计划跳过  计划跳过估计需要一个特殊的标记来实现（如果需要准点通知的话）   */
// void plan_smart_reconfig(water_task_date_m *wtd, const char *smart_zone, uint32_t skip_flag)
// {
//     int j = 0;
//     if (skip_flag == 0 && wtd != NULL && smart_zone != NULL && strstr(smart_zone, "null") == NULL) // 说明使用智能计划
//     {
//         DEBUG_TEST(DB_I,"%s", smart_zone);
//         M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(smart_zone, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d",
//                &wtd->chaninfo[0].enable, &wtd->chaninfo[1].enable, &wtd->chaninfo[2].enable, &wtd->chaninfo[3].enable, &wtd->chaninfo[4].enable,
//                &wtd->chaninfo[5].enable, &wtd->chaninfo[6].enable, &wtd->chaninfo[7].enable, &wtd->chaninfo[8].enable, &wtd->chaninfo[9].enable,
//                &wtd->chaninfo[10].enable, &wtd->chaninfo[11].enable, &wtd->chaninfo[12].enable, &wtd->chaninfo[13].enable, &wtd->chaninfo[14].enable,
//                &wtd->chaninfo[15].enable);
//         for (int i = 0; i < 16; i++)
//         {
//             if (wtd->chaninfo[i].chanID != 0 && wtd->chaninfo[i].chanID == wtd->chaninfo[j].enable)
//             {
//                 DEBUG_TEST(DB_I,"skip zone-->%hhd", wtd->chaninfo[i].chanID);
//                 j++;
//                 wtd->chaninfo[j].enable = 0;
//                 wtd->chanRunTimeSum -= wtd->chaninfo[i].shouldRunTime;
//             }
//             else
//                 wtd->chaninfo[i].enable = 1;
//         }
//     }
//     if (skip_flag != 0) // 说明使用 跳过信息 处理标记
//     {
//         wtd->wind_water_ice_skip = skip_flag;
//     }
//     else
//     {
//         if (_plan.is_backstage && !io_contr.s1_trigger && !io_contr.s2_trigger &&
//             _plan.plan_runnning_obj != NULL && _plan.plan_runnning_obj->schedule_id != wtd->schedule_id)
//         // 下一个计划需要运行  但是现在的计划在后台运行
//         { // 停掉后台的计划
//             DEBUG_TEST(DB_I,"9999");
//             delete_time_node(_plan.plan_runnning_obj->schedule_id, 9);
//             water_plan_end_is_Interval_hours(plan_backstage_become_silent_dele());
//             plan_backstage_reset();
//         }
//     }
// }
// void plan_flushed_weather_info(uint64_t in_id, int in_state)
// {
//     if (in_state > 0 && _plan.plan_runnning_obj != NULL && _plan.plan_runnning_obj->schedule_id == in_id)
//     {
//         uint64_t plan_start_times = compute_plan_last_start_timestamp(
//             _plan.plan_runnning_obj->planinfo.start_date_time, _plan.plan_runnning_obj->WSTime, _plan.plan_runnning_obj->chanRunTimeSum);
//         _plan.plan_runnning_obj->deadlineOfp-lan = (plan_start_times) + _plan.plan_runnning_obj->chanRunTimeSum;
//         create_send_data_to_server_22_07_29(SCH_WEATHER_SKIP,
//                                             _plan.plan_runnning_obj->schedule_id,
//                                             _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].chanID,
//                                             _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].realityRunTime,    // (当前区域已经运行时间)
//                                             _plan.plan_runnning_obj->chaninfo[_plan.plan_runnning_obj->pc].thisOperationTime, // 当前批次区域已经运行时间
//                                             (plan_start_times) + _plan.plan_runnning_obj->chanRunTimeSum,                     // 计划结束时间戳
//                                             None,                                                                             //(保留)
//                                             (uint32_t)in_state,
//                                             _plan.plan_runnning_obj->chan_already_RunTimeSum // 计划运行总时间(不含浸泡)

//         );
//         lay_plan_stop_a_run(true, _plan.plan_runnning_obj->schedule_id, _plan.plan_runnning_obj->deadlineOfplan, NULL);
//         plan_stop_directly(_plan.plan_runnning_obj);
//         _plan.plan_runnning_obj = NULL;
//         TrunOnDevice(false);
//     }
//     else if (power_break_restore_wait == fail_r)
//     {
//         if (in_state > 0)
//             power_break_restore_wait = in_state;
//         else
//             power_break_restore_wait = 0;
//     }
// }

// 一个计划和区域的队列    用来存储当前的计划是否应该被恢复和启用
//   可以使用计划的启用和禁用    可以使用计划当前区域的启用和禁用
//   计划可以在任何时候被启用
//   计划还有适配的功能适配低版本

/**
 * 区域禁用 ： 当使用区域禁用的时候 直接把区域禁用的区域在恢复之后去掉   ，在计划恢复之后的初始化区域之前
 *
 * 区域启用 ： 区域启用   直接启用区域肯设计到所有计划的初始化（）
 *
 *
 * 选择全局区域禁用   全局属性存在一个单独的文件夹中，会包含一些参数
 *                          区域禁用开启（区域）（一个一个设置）
 *                                  保存区域，及相关参数
 *                          区域禁用关闭（区域）
 *                                  移除区域，及相关参数
 *                          区域禁用查询（在计划选择区域的时候，基本会查询该计划的区域是否被禁用，我提供统一的接口来查询）
 *                          网络接口查询
 *                              在线调试查询有哪些区域是被禁用的
 *
 *
 */

stat_m m_global_zone_disable(int in_zone)
{
    stat_m stat = succ_r;
    DEBUG_TEST(DB_I,"disable zone(%d) ...", in_zone);
    in_zone -= 1;
    m_callable_flash_flag_rescue_message(_option.mz_able.zone_list[in_zone], M_RESCUE_MESSAGE_SET);
    _option.mz_able.zone_state[in_zone] = disable_r;
    return stat;
}

stat_m m_global_zone_enable(int in_zone)
{
    stat_m stat = succ_r;
    DEBUG_TEST(DB_I,"enable zone(%d) ...", in_zone);
    in_zone -= 1;
    m_callable_flash_flag_rescue_message(mz_able.zone_list[in_zone], M_RESCUE_MESSAGE_DEL);
    mz_able.zone_state[in_zone] = enable_r;
    return stat;
}
/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_global_zone_query_available(void)
{
    stat_m stat = enable_r;
    for (int i = 0; i < 16; i++)
    {
        if (m_callable_flash_flag_rescue_message(_option.mz_able.zone_list[i], M_RESCUE_MESSAGE_GET) == succ_r)
        {
            stat = disable_r;
        }
    }
    return stat;
}
/**
 * @brief The area of the query plan is disabled
 *
 * @param in_zone query zone
 * @return stat_m
 */
stat_m m_global_zone_query(int in_zone)
{
    stat_m stat = enable_r;

    // if (_option.mz_able.zone_state[in_zone] == 1)
    if (_option.mz_able.zone_state[in_zone - 1] == disable_r)
    {
        stat = disable_r;
        DEBUG_TEST(DB_I,"disable :m_global_zone_query :need skip:%d", in_zone);
    }
    else
        DEBUG_TEST(DB_I,"disable :m_global_zone_query :need run:%d", in_zone);
    return stat;
}

stat_m m_callable_global_zone_init(void)
{
    stat_m stat = succ_r;
    // _option.mz_able.zone_list = mz_able.zone_list;
    memcpy(_option.mz_able.zone_list, mz_able.zone_list, sizeof(mz_able.zone_list));
    for (int i = 0; i < 16; i++)
    {
        // printf("%s" ,_option.mz_able.zone_list[i]);
        if (m_callable_flash_flag_rescue_message(_option.mz_able.zone_list[i], M_RESCUE_MESSAGE_GET) == succ_r)
        {
            _option.mz_able.zone_state[i] = disable_r;
            DEBUG_TEST(DB_W,"m_global_zone disable:%d", i + 1);
            stat = succ_r;
        }
        else
            _option.mz_able.zone_state[i] = enable_r;
    }
    return stat;
}

stat_m m_callable_zone_have_next(m_watering_schedule *mobj, uint64_t present_timsp)
{
    stat_m stat = fail_r;
    if (present_timsp - mobj->running.current_running_time_region.tr_end_time < 50)
    {
        stat = succ_r;
    }
    DEBUG_TEST(DB_I,"present_timsp %lld   tr_end_time %lld", present_timsp, mobj->running.current_running_time_region.tr_end_time);
    return stat;
}
stat_m m_callable_zone_global_next(m_chan_managec *in_mg, uint8_t *out_pc, uint32_t in_sack, uint16_t in_max_zone, uint64_t pre_timesp)
{
    stat_m stat = fail_r;
    uint8_t x = *out_pc;
    for (int i = 1; i <= in_max_zone; i++)
    {
        x = i + *out_pc;
        if (x >= in_max_zone)
            x -= in_max_zone;
        if (in_mg[x].enable && in_mg[x].id != 0 && m_global_zone_query(in_mg[x].id) == enable_r &&
            in_mg[x].shouldRunTime - in_mg[x].realityRunTime > 50 // &&
            // pre_timesp - in_mg[x].runtime.tr_end_time >= in_sack * 60   这一条去掉了  原因是在需要浸泡的时候这个不成立 结束时间和浸泡的时间差小于浸泡的时间是正常的，所以他才需要浸泡，不知道写的时候是出于什么原因但是这里不该有
            ) // 查询计划启用情况
        {
            in_mg[i].id = in_mg[i].id;
            *out_pc = x;
            DEBUG_TEST(DB_I,"should zone %hhd ", in_mg[x].id);
            stat = succ_r;
            break;
        }
        else
            DEBUG_TEST(DB_I,"not should zone %hhd (%lld -> %lld)", in_mg[x].id, pre_timesp, in_mg[x].runtime.tr_end_time);
    }
    return stat;
}

stat_m m_callable_zone_quick_run_switch_channel(m_chan_managec *in_mg, uint8_t *out_pc, bool in_next_or_pre, uint16_t in_max_zone, uint64_t pre_timesp)
{
    stat_m stat = fail_r;
    int x = *out_pc;
    for (int i = 1; i <= in_max_zone; i++)
    {
        if(in_next_or_pre)
        {
            x = i + *out_pc;
            // 循环寻找
            if (x >= in_max_zone)
                x -= in_max_zone;
        }
        else 
        {
            x = *out_pc - i;
            // 循环寻找
            if (x < 0)
                x += in_max_zone;
        }
        // 区域有效
        if (in_mg[x].enable && in_mg[x].id != 0 &&
            // 区域启用
            m_global_zone_query(in_mg[x].id) == enable_r &&
            // 区域未运行完成
            in_mg[x].shouldRunTime > in_mg[x].realityRunTime // &&
            // // 区域还需要浸泡
            // in_mg[x].shouldRunTime > in_mg[x].realityRunTime
            // pre_timesp - in_mg[x].runtime.tr_end_time >= in_sack * 60   这一条去掉了  原因是在需要浸泡的时候这个不成立 结束时间和浸泡的时间差小于浸泡的时间是正常的，所以他才需要浸泡，不知道写的时候是出于什么原因但是这里不该有
            ) // 查询计划启用情况
        {
            in_mg[i].id = in_mg[i].id;
            *out_pc = x;
            DEBUG_TEST(DB_I,"m_callable_zone_quick_run_switch_channel zone %hhd ", in_mg[x].id);
            stat = succ_r;
            break;
        }
        else
            DEBUG_TEST(DB_W,"not m_callable_zone_quick_run_switch_channel zone %hhd (%lld -> %lld)", in_mg[x].id, pre_timesp, in_mg[x].runtime.tr_end_time);
    }
    return stat;
}
