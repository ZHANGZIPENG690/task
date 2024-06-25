

#include "instance_manage.h"

#include "../fsm/instance_fsm.h"
#include "../tools/instance_algorithm.h"
#include "instance_manage.h"
#include "instance_list.h"
#include "instance_stroage.h"
#include "../instance.h"

stat_m m_callable_instance_manage_init(void)
{
    stat_m stat = fail_r;
    stat = m_static_instance_manage_init();
    return stat;
}

void sch_print(instance_doc_t *water_d, uint64_t current_time);
//
instance_doc_t inst_expl_obj = {0};

char temp_sch[500] = {0};

// 计划恢复和计划添加都可以调用这个函数    萨和ibushi恢复的计划
stat_m m_callable_instance_manage_add(char *src_str, enum inst_add_mode mode, uint64_t in_time_s, uint64_t *out_uid)
{
    stat_m stat = fail_r;
    instance_doc_t *wobj = NULL;
    bool enable = true;
    uint64_t pre_running_time = 0;

    if (out_uid != NULL)
        *out_uid = 0;
    // 解析
    strcpy(temp_sch, src_str);
    stat = m_static_tools_instance_obj_explain(src_str, &inst_expl_obj, &enable, in_time_s);
    if (stat == succ_r)
    {
        /** 解析成功 利马存储 */
        m_callable_schedule_stroage_write_string(inst_expl_obj.sch_info.cu_id, temp_sch, enable);

        m_callable_schedule_stroage_read_by_param(inst_expl_obj.sch_info.cu_id, &inst_expl_obj.sch_info.cu_src_execute_time_str, 5);
        m_callable_schedule_stroage_read_by_param(inst_expl_obj.sch_info.cu_id, &inst_expl_obj.sch_info.cu_zone, 6);

        // DEBUG_TEST( DB_I,%s", inst_expl_obj.sch_info.cu_zone);

        if (out_uid != NULL)
            *out_uid = inst_expl_obj.sch_info.cu_id;
        // 参数校对 - 如果都正常 那就加入列表或者下一个参数继续  如果返回成功表示计划内容是有效的
        stat = m_static_instance_manage_explain_instance_obj(&inst_expl_obj, in_time_s);
        if (inst_expl_obj.sch_info.cu_id < 10)
        {
            DEBUG_TEST(DB_W," Fail + ++ + %lld", inst_expl_obj.sch_info.cu_id);
            return 0;
        }
        // 接入队列
        wobj = m_static_instance_list_add(&inst_expl_obj);
        // 先判断是不是 恢复的计划
        if (mode == M_INSTANCE_ADD_MODE_ONLINE)
        {
            wobj->sch_info.cu_enable = enable;
            m_callable_exce_schedule_stroage_en_dis_able(wobj->sch_info.cu_id, wobj->sch_info.cu_enable);
        }
        // 刷新下开始时间
        // 存一下 temp_sch

        m_static_tools_instance_obj_refresh_start_time(wobj, &pre_running_time, in_time_s);
        if (mode == M_INSTANCE_ADD_MODE_ONLINE)
        {
            m_static_instance_enreg_schedule_end_time(wobj->sch_info.cu_id, pre_running_time + wobj->exfun.fn_max_coust_time);
        }
        // 计划启用和不启用 都会刷新最新的启动时间
        if (wobj->sch_info.cu_enable)
            m_static_time_link_notece_node_add(wobj->sch_info.cu_id, wobj->WSTime, M_BASE_STATUS_IDLE, (void *)wobj);
        else
        {
            m_static_time_link_notece_node_disable(wobj->sch_info.cu_id);
            m_callable_schedule_stroage_disable(wobj->sch_info.cu_id, in_time_s);
        }
        // 在启用和不启用的区别应该就是一个bool参数和正确时间节点    缺一不可   两个参数都有效才是一个可以运行的计划
        /** Next : Determine the scheduled start time */

        // stat = m_recover_determine_scheduled_start_time(ws_item, in_time_s);
        /** Next : schedule recover */
        /** 所有需要恢复的计划都将放在这个队列里面 （这个后期我想改成挂起队列 挺好的哈哈哈）*/
        // m_callable_recover_queue_add_one(&mreRoot, ws_item );
        // m_static_time_link_notece_node_add(wobj->sch_info.cu_id ,  wobj->WSTime , wobj);
        // m_operation_normal_recover_schedule_time_and_status(ws_item , _time.hhmmss);
        m_callable_sensor_staging_clean(wobj->sch_info.cu_id, None);

        // m_callable_add_notice_schedule_run_node(ws_item->sch_info.cu_id, ws_item->WSTime, m_schedule__idle, ws_item);
        /**
         * 梳理一下逻辑
         * 1 、找出所有计划，判断是否运行，是否应该恢复
         * 2 、放入队列，不需要排序
         * 3 、所有的计划都完了之后，找到最先应该运行的计划
         * 4 、根据当前的时间，恢复该计划
         *
         */
        sch_print(wobj, in_time_s);
    }
    else
    {
        DEBUG_TEST( DB_I,"Parameter error, initialization failed !");
    }
    return stat;
}

void sch_print(instance_doc_t *water_d, uint64_t current_time)
{

    DEBUG_TEST( DB_I,"\033[0;33m--------->");
    DEBUG_TEST( DB_I,"schedule id :%lld", water_d->sch_info.cu_id);
    DEBUG_TEST( DB_I, "smart :%hhd", water_d->sch_info.cu_smart);
    DEBUG_TEST( DB_I, "cu_sack :%s", water_d->sch_info.cu_sack);
    DEBUG_TEST( DB_I, "tr_start_time :%lld", water_d->sch_info.available_time.tr_start_time);    /* 开始时间*/
    DEBUG_TEST( DB_I, "tr_end_time :%lld", water_d->sch_info.available_time.tr_end_time);        /* 结束时间*/
    DEBUG_TEST( DB_I, "cu_src_execute_time_str :%s", water_d->sch_info.cu_src_execute_time_str); /* 关于开始时间参数 */
    DEBUG_TEST( DB_I, "cu_zone :%s", water_d->sch_info.cu_zone);                                 /** 运行区域*/
    DEBUG_TEST( DB_I, "frequency_type :%s", toname(water_d->sch_info.frequency_type));           /* 间隔类型 INVA_HOUR SUNSET*/

    DEBUG_TEST( DB_I, "cu_week_list :%s", water_d->sch_info.cu_week_list);                         /* 星期几参数 */
    DEBUG_TEST( DB_I, "cu_interval_hour_or_day :%hhd", water_d->sch_info.cu_interval_hour_or_day); /* 间隔小时的参数 */

    DEBUG_TEST( DB_I, "------------------------ ");

    DEBUG_TEST( DB_I, "execu_type :%s", toname(water_d->execu_type));                     /* 执行类型 AT BEF*/
    DEBUG_TEST( DB_I, "fn_max_should_runtime :%d", water_d->exfun.fn_max_should_runtime); /* 运行浇水时间总和 */
    DEBUG_TEST( DB_I, "fn_soak_water :%d", water_d->exfun.fn_soak_water);                 /* 浇水时间 */
    DEBUG_TEST( DB_I,"fn_soak_cycle :%d", water_d->exfun.fn_soak_cycle);                 /* 浸泡时间 */
    DEBUG_TEST( DB_I, "fn_max_coust_time :%lld", water_d->exfun.fn_max_coust_time);       /* 运行时间总和 */
    DEBUG_TEST( DB_I, "fn_max_channel :%d", water_d->exfun.fn_max_channel);               /* 最大运行区域*/
    DEBUG_TEST( DB_I, "WSTime : %lld\033[0;37m", water_d->WSTime);                        /* 运行时间 */

    // current_time%3600 ,(current_time%3600)/3600 ,((current_time%3600)%3600)/60  , ((current_time%3600)%3600)%60);
}

/**
 * @brief
 *
 * @param instance
 * @return stat_m
 */
stat_m m_callable_instance_manage_change(instance_doc_t *in_doc, uint64_t in_time_s)
{
    stat_m stat = succ_r;

    stat = m_callable_instance_running_event_input(SCHEDULE_CHANGE_STOP, in_time_s);

    return stat;
}
/**
 * @brief
 *
 * @param sch_id
 * @return stat_m
 */
stat_m m_callable_instance_manage_remove(uint64_t sch_id, uint64_t trig_time)
{
    stat_m stat = succ_r;

    m_callable_device_attribute_config_change_notify_event_callable(
        M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_DELE,
        (void *)&sch_id,
        m_callable_timer_manage_get_device_time());
    // stat = m_callable_instance_running_event_input(SCHEDULE_DELETE_STOP, trig_time);

    // 先删除列表
    stat = m_static_instance_list_remove_by_id(sch_id);
    // 再删除存储
    stat = m_static_schedule_stroage_remove_by_id(sch_id);
    /** 清除跳过信息 */
    stat = m_callable_schedule_skip_clean(sch_id);
    return stat;
}
/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_remove_all(void)
{
    stat_m stat = succ_r;
    stat = m_callable_instance_running_event_input(SCHEDULE_DELETE_STOP, m_callable_timer_manage_get_device_time());
    // 先删除列表
    stat = m_static_instance_list_remove_all();
    // 再删除存储
    stat = m_static_schedule_stroage_remove_all();
    // m_static_time_link_notece_node_remove_all();
    return stat;
}

/**
 * @brief 启用一个计划
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_enable(uint64_t in_uid)
{
    stat_m stat = succ_r;
    instance_doc_t *wobj = NULL;

    DEBUG_TEST(DB_W,"m_callable_instance_manage_enable");
    // 存储标记
    m_callable_schedule_stroage_enable(in_uid, m_callable_timer_manage_get_device_time());
    // 列表标记  (不需要)
    // 发送 计划该改变的 事件    --   因为需要改变 计划是否在运行 来停止   所以发送事件需要
    // m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_OPEN,
    //                                                             (void *)&in_uid, m_callable_timer_manage_get_device_time());
    m_callable_exce_schedule_stroage_en_dis_able(in_uid, true);
    m_static_time_link_notece_node_enable(in_uid);
    stat = m_static_instance_list_find_by_id(in_uid, &wobj);
    if (stat == succ_r)
    {
        m_static_tools_instance_obj_refresh_start_time(wobj, NULL, m_callable_timer_manage_get_device_time());
        m_static_time_link_notece_node_change_obj(in_uid, wobj->WSTime, M_BASE_STATUS_IDLE, wobj, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
    }
    // m_callable_instance_running_event_input(SCHEDULE_DISABLE_STOP, in_time_s);
    return stat;
}
/**
 * @brief 禁用一个计划
 *
 * @return stat_m
 */
stat_m m_callable_instance_manage_disable(uint64_t in_uid)
{
    stat_m stat = succ_r;
    DEBUG_TEST(DB_W,"m_callable_instance_manage_disable");
    // 存储标记
    m_callable_schedule_stroage_disable(in_uid, m_callable_timer_manage_get_device_time());
    // 列表标记  (不需要)
    // 发送 计划该改变的 事件    --   因为需要改变 计划是否在运行 来停止   所以发送事件需要
    m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_CLOSE,
                                                                    (void *)&in_uid, m_callable_timer_manage_get_device_time());

    return stat;
}

stat_m m_static_inst_reflash_time_callback(void *in_wtd)
{
    stat_m stat = fail_r;
    instance_doc_t *indoc = (instance_doc_t *)in_wtd;

    stat = m_static_instance_manage_current_instance_id_is_effective(indoc->sch_info.cu_id);
    if (stat == succ_r)
    {
        m_ext_drive_button_event_handle(M_KEY_EVENT_STOP, M_KEY_EVENT_SIGN_CLICK, 0, 0, m_callable_timer_manage_get_device_time());
    }

    m_static_tools_instance_obj_refresh_start_time(indoc, NULL, m_callable_timer_manage_get_device_time());
    m_static_time_link_notece_node_change_obj(indoc->sch_info.cu_id,
                                              indoc->WSTime, M_BASE_STATUS_IDLE, indoc, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
    return stat;
}
/**
 * @brief 刷新指定计划内容的时间
 *
 * @param in_type 指定类型
 * @return stat_m
 */
stat_m m_callable_instance_manage_update_spec_schedule_time(int in_type)
{
    return m_static_instance_list_update_spec_type(in_type, m_static_inst_reflash_time_callback);
}

// stat_m m_callable_schedule_virtual_init(instance_doc_t *in_info)
// {
//     stat_m stat = succ_r;
//     char temp_ac[20] = {0};

//     // uint32_t certain_day_sunset = 0;
//     // uint32_t certain_day_sunrise = 0;

//     int temp_var_a = 0;
//     int temp_var_b = 0;
//     DEBUG_TEST(DB_I,"plan_virtual_init %lld...", in_info->sch_info.cu_id);

//     if (in_info->sch_info.cu_id <= 0 ||
//         in_info->sch_info.available_time.tr_start_time < 1000 ||
//         in_info->sch_info.available_time.tr_end_time < 1000 ||
//         strlen(in_info->sch_info.cu_zone) < 2)
//     {
//         stat = fail_r;
//     }
//     if (stat != fail_r)
//     {

//         { /** Next : null or 1&2 (Sack) or 2|10 (Pump Mode)*/
//             if (strcmp(in_info->sch_info.cu_sack, TEMP_WORD_PUBLIC_NULL) != 0)
//             {
//                 if (strstr(in_info->sch_info.cu_sack, M_INV7) != NULL)
//                 {
//                     in_info->exfun.fun_soak_use = true;
//                     m_insi_foramt_param(M_STRM_STR_3(M_HD, "&", M_HD),
//                                         &in_info->exfun.fun_soak_water,
//                                         &in_info->exfun.fun_soak_cycle,
//                                         in_info->sch_info.cu_sack);
//                     printf("  use %s , " M_STRM_STR_3(M_HD, "&", M_HD),
//                            in_info->sch_info.cu_sack, in_info->exfun.fun_soak_water, in_info->exfun.fun_soak_cycle);
//                 }
//                 else if (strstr(in_info->sch_info.cu_sack, "|") != NULL)
//                 {
//                     in_info->exfun.fun_soak_use = false;
//                     m_insi_foramt_param(M_STRM_STR_3(M_HD, "|", M_HD),
//                                         &in_info->exfun.fun_solenoid_mode,
//                                         &in_info->exfun.fun_solenoid_time,
//                                         in_info->sch_info.cu_sack);
//                 }
//                 else
//                 {
//                     in_info->exfun.fun_soak_use = false;
//                     DEBUG_E("sch_info.cu_sack Err --> (%s)", in_info->sch_info.cu_sack);
//                 }
//             }
//             in_info->sch_info.cu_src_execute_time_str[3] = ' ';
//         }
//         /** Next : 100 08:00*/
//         m_insi_foramt_param("%hhd %s",
//                             &in_info->sch_info.cu_execute_type,
//                             in_info->sch_info.cu_execute_time,
//                             in_info->sch_info.cu_src_execute_time_str);
//         // printf("%s...%hhd...%s ", in_info->sch_info.cu_src_execute_time_str, in_info->sch_info.cu_execute_type, in_info->sch_info.cu_execute_time);
//         /** Next : 108 1*/
//         /*
//                 {
//                     SCHEDULE_RUN_TYPE_AT = 100,
//                     SCHEDULE_RUN_TYPE_BEFROE,
//                     SCHEDULE_RUN_TYPE_SUNSET,
//                     SCHEDULE_RUN_TYPE_SUNRISE,
//                 };
//         */

//         char ZONE_FORMAN_SEPA[10] = "%hhd#%hhd";
//         char ZONE_FORMAN_END = ';';
//         uint32_t sh_time_arr[16] = {0}; // 获得所有区域运行时间

//         /** Next : Zone info */
//         // 解析及区域参数，运行时间总和 区域单数
//         m_operation_schedule_compute_zone_distill(in_info,
//                                                   ZONE_FORMAN_SEPA,
//                                                   ZONE_FORMAN_END,
//                                                   in_info->sch_info.cu_zone,
//                                                   sh_time_arr);
//         // 计算总花费时间
//         if (in_info->exfun.fun_soak_use)
//             m_operation_sack_recover_schedule_time_and_status(NULL,
//                                                               in_info->exfun.fun_soak_water,
//                                                               in_info->exfun.fun_soak_cycle,
//                                                               sh_time_arr,
//                                                               in_info->exfun.max_should_runtime,
//                                                               m_callable_get_device_time_t(), &in_info->sch_info.cu_run_time_sum);
//         else
//             in_info->sch_info.cu_run_time_sum = in_info->exfun.max_should_runtime;

//         /** Next : Determine Start Time  [sch_info.cu_execute_type] 100 - 101 103 104 */
//         if (
//             in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNRISE ||
//             in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNSET)
//         {
//             /* 根据计划开始时间，计算当天的日出和日落时间*/
//             // m_suninfo_specity_date(&certain_day_sunset,&certain_day_sunrise , in_info->sch_info.cu_execute_time.tr_start_time);
//             in_info->WSTime = in_info->running.rb_WSTime_clone = 47000;
//         }

//         /** interval hour and (sunset / sunrise)  ==  AT + interval hour */
//         if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNSET &&
//             in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
//             in_info->sch_info.cu_execute_type = SCHEDULE_RUN_TYPE_AT;

//         if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNRISE &&
//             in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
//             in_info->sch_info.cu_execute_type = SCHEDULE_RUN_TYPE_BEFROE;

//         /** Next : Determine Start Time 2 */
//         // cu_execute_time
//         if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_AT || in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_BEFROE)
//         {
//             m_insi_foramt_param("%d:%d",
//                                 &temp_var_a,
//                                 &temp_var_b,
//                                 in_info->sch_info.cu_execute_time);
//             in_info->WSTime = in_info->running.rb_WSTime_clone = temp_var_a * 60 * 60 + temp_var_b * 60;
//         }

//         // m_insi_schedule_compute_zone_foramt(in_info , );
//         // 假设你得到了，他的应该运行时间，那么，他的开始时间你应该也是可以得到的
//         if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_BEFROE)
//         {
//             in_info->WSTime = in_info->running.rb_WSTime_clone - in_info->sch_info.cu_run_time_sum;
//             in_info->running.rb_WSTime_clone = in_info->WSTime;
//         }

//         if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNSET)
//         {
//             in_info->WSTime = in_info->running.rb_WSTime_clone = /*_time.today_sunset*/ 0;
//         }
//         if (in_info->sch_info.cu_execute_type == SCHEDULE_RUN_TYPE_SUNRISE)
//         {
//             in_info->WSTime = in_info->running.rb_WSTime_clone = /*_time.today_sunrise*/ 0;
//         }

//         /** Next : Determine Start Time 2  [sch_info.cu_run_interval_type]  */
//         /*
//                 {
//                     SCHEDULE_RUN_RHYTHM_SPEC_DAYS,
//                     SCHEDULE_RUN_RHYTHM_ODD_DAY,
//                     SCHEDULE_RUN_RHYTHM_EVEN_DAY,
//                     SCHEDULE_RUN_RHYTHM_INVERTAL_DAY,
//                     SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR,
//                     SCHEDULE_RUN_RHYTHM_EVERY_DAYS
//                 };
//         */
//         if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_SPEC_DAYS)
//         {
//             m_insi_foramt_week_c("%d", ';', in_info->exfun.week, in_info->sch_info.cu_week_list);
//         }
//         if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_NEW_LAWN)
//         { // 如果是新草坪计划
//             DEBUG_TEST(DB_I,"New Lwan Mode ...");
//             in_info->running.rb_WSTime_clone = in_info->WSTime = 0;
//             stat = special_newlwan_r;
//         }
//         if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_ODD_DAY || in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_EVEN_DAY)
//         {
//             DEBUG_TEST(DB_I,"ODD / EVENT ...");
//         }
//         if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERTAL_DAY)
//         {
//             DEBUG_TEST(DB_I,"Invertal Day ...");
//             M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_week_list, "%hhd", &in_info->sch_info.cu_interval_hour_or_day);
//         }
//         if (in_info->sch_info.cu_run_interval_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
//         {
//             DEBUG_TEST(DB_I,"Invertal Hour ");
//             M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_info->sch_info.cu_week_list, "%hhd", &in_info->sch_info.cu_interval_hour_or_day);
//             if (in_info->sch_info.available_time.tr_start_time + in_info->WSTime <= m_callable_get_device_time_t())
//             {
//                 m_operation_schedule_calculate_the_interval_hour(/*_time.hhmmss*/ 0, m_callable_get_device_time_t(),
//                                                                  in_info->sch_info.cu_interval_hour_or_day, in_info->sch_info.available_time.tr_start_time, &in_info->WSTime);
//                 in_info->running.rb_WSTime_clone = in_info->WSTime;
//             }
//             // Not more than not processing  else
//             // DEBUG_TEST(DB_W," cruuent :%d ,  CYCLE_HOUR_DAYS :%lld  ,%hhd ", _time.new_get_hhmmss(), in_water_item->WSTime, in_water_item->cycle_days_or_hours);
//         }
//         {
//             DEBUG_TEST(DB_W,"rb_WSTime_clone :%lld", in_info->running.rb_WSTime_clone);
//             // in_water_item->channel_state = plan_runing;
//             // nvs_plan_save(in_water_item, plan_runing);

//             // in_water_item->WSTime_clone = in_water_item->WSTime;
//             // new_add_time_node(in_water_item->schedule_id, in_water_item->WSTime_clone, in_water_item->channel_state);
//             // DEBUG_E("present time: %lld", _time.time_get_timestamp_t());
//             // DEBUG_E("plan start_time: %lld", in_water_item->WSTime);
//             // create_send_data_to_server(ID_RESP_ADD_PLAN_TO_DEVICE, U32, &in_water_item->schedule_id, Str, TEMP_WORD_PUBLIC_OK, null, NULL);
//             // sch_print(in_info);
//         }
//     }

//     return stat;
// }
// stat_m m_callable_schedule_running_buf_clear(instance_doc_t *wobj)
// {
//     stat_m stat = fail_r;
//     // if (wobj != NULL)
//     // {
//     //     wobj->running_buff.chan_already_RunTimeSum = 0;
//     //     wobj->running_buff.rb_pc = 0;
//     //     wobj->running_buff.rb_pre_pc = 0;
//     //     wobj->running_buff.current_running_time_region.tr_end_time = 0;
//     //     wobj->running_buff.current_running_time_region.tr_start_time = 0;
//     //     wobj->running_buff.wind_water_ice_skip = 0;
//     //     for (int i = 0; i < wobj->exfun.maxChan; i++)
//     //     {
//     //         wobj->sio_channel[i].realityRunTime = 0;
//     //         wobj->sio_channel[i].has_been_power_watering_time = 0;
//     //         wobj->sio_channel[i].has_been_watering_time = 0;
//     //         wobj->sio_channel[i].runtime.tr_start_time = 0;
//     //         wobj->sio_channel[i].runtime.tr_end_time = 0;
//     //     }
//     // }

//     return stat;
// }

char *toname(int en)
{
    // if (en == m_schedule__running)
    //     return "m_schedule__running";
    // // if (en == m_schedule__prepare)
    // //     return "m_schedule__prepare";
    // if (en == m_schedule__start)
    //     return "m_schedule__start";
    // if (en == m_schedule__stop)
    //     return "m_schedule__stop";
    // if (en == m_schedule__idle)
    //     return "m_schedule__idle";
    // if (en == m_schedule__sack)
    //     return "m_schedule__sack";

    if (en == M_BASE_STATUS_IDLE)
        return "IDLE";
    if (en == M_BASE_STATUS_RUNNING)
        return "RUNNING";
    if (en == M_BASE_STATUS_SACK)
        return "SACK";
    if (en == M_BASE_STATUS_PREPARE)
        return "PREPARE";
    if (en == M_BASE_STATUS_PAUSE)
        return "PAUSE";
    if (en == M_BASE_STATUS_HANG_UP)
        return "HANG_UP";
    if (en == M_BASE_STATUS_STOP)
        return "STOP";

    // if (en == should_running_r)
    //     return "should_running_r";
    // if (en == should_sack_r)
    //     return "should_sack_r";
    // if (en == not_should_r)
    //     return "not_should_r";

    if (en == SCHEDULE_RUN_TYPE_AT)
        return "SCHEDULE_RUN_TYPE_AT";
    if (en == SCHEDULE_RUN_TYPE_BEFROE)
        return "SCHEDULE_RUN_TYPE_BEFROE";
    if (en == SCHEDULE_RUN_TYPE_SUNSET)
        return "SCHEDULE_RUN_TYPE_SUNSET";
    if (en == SCHEDULE_RUN_TYPE_SUNRISE)
        return "SCHEDULE_RUN_TYPE_SUNRISE";
    if (en == SCHEDULE_RUN_TYPE_SUNSET_PLUS)
        return "SCHEDULE_RUN_TYPE_SUNSET_PLUS";
    if (en == SCHEDULE_RUN_RHYTHM_NEW_LAWN)
        return "SCHEDULE_RUN_RHYTHM_NEW_LAWN";
    if (en == SCHEDULE_RUN_TYPE_SUNRISE_PLUS)
        return "SCHEDULE_RUN_TYPE_SUNRISE_PLUS";
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

    if (en == CHANGING_OVER_STOP_TRIGGER)
        return "CHANGING_OVER_STOP_TRIGGER";
    if (en == CHANGING_OVER_START_TRIGGER)
        return "CHANGING_OVER_START_TRIGGER";
    if (en == CHANGING_OVER_LEFT_TRIGGER)
        return "CHANGING_OVER_LEFT_TRIGGER";
    if (en == CHANGING_OVER_RIGHT_TRIGGER)
        return "CHANGING_OVER_RIGHT_TRIGGER";

    if (en == M_SKIP_MANAGE_TYPE_DORMANT_SKIP)
        return "M_SKIP_MANAGE_TYPE_DORMANT_SKIP";
    if (en == M_SKIP_MANAGE_TYPE_WATERDELAY_SKIP)
        return "M_SKIP_MANAGE_TYPE_WATERDELAY_SKIP";
    if (en == M_SKIP_MANAGE_TYPE_SIGN_SKIP)
        return "M_SKIP_MANAGE_TYPE_SIGN_SKIP";
    if (en == M_SKIP_MANAGE_TYPE_SENSOR_SKIP)
        return "M_SKIP_MANAGE_TYPE_SENSOR_SKIP";
    if (en == M_SKIP_MANAGE_TYPE_WEATHER_SKIP)
        return "M_SKIP_MANAGE_TYPE_WEATHER_SKIP";
    if (en == M_SKIP_MANAGE_TYPE_SATURATION_SKIP)
        return "M_SKIP_MANAGE_TYPE_SATURATION_SKIP";
    if (en == M_KEY_EVENT_LEFT)
        return "M_KEY_EVENT_LEFT";
    if (en == M_KEY_EVENT_STOP)
        return "M_KEY_EVENT_STOP";
    if (en == M_KEY_EVENT_RIGHT)
        return "M_KEY_EVENT_RIGHT";
    if (en == M_KEY_EVENT_START)
        return "M_KEY_EVENT_START";
    DEBUG_TEST( DB_I,"%d", en);
    return "unkown";
}
