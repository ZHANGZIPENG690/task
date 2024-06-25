/**
 * @brief 关于指针指向问题    最新的方法是 当前运行哪个区域 那就指向哪个区域    当切换完了之后 再指向下一个区域
 *
 *
 */

/**
 * @file instance_to_run_queue.c
 * @author your name (you@domain.com)
 * @brief  计划运行
 * @version 0.1
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "instance_running.h"
#include "../manage/instance_list.h"
#include "../ibuild/instance_build.h"
#include "../../../middle/schedule/skip_manage.h"

// 一键停止 计划运行（传入 Bool Bool）
//                      表示是否是手动停止 记录
//                      并表示是否需要反馈服务器

// 一键刷新信息   清空信息
#define WEATHER_SATURATION_SKIP 10

stat_m m_static_instance_running_clear(instance_obj *in_obj, uint64_t in_time_s);

stat_m m_static_instance_running_running_to_run_switch(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    instance_obj *in_obj = (instance_obj *)pdata;

    stat_m stat = fail_r;
    enum base_state next_ste = M_BASE_STATUS_IDLE, pre_ste = M_BASE_STATUS_IDLE;
    // 拿到运行单元
    struct instance_running_unit *wobj = in_obj->instance_running_root_unit->current_running_unit;
    struct instance_running_unit *pre_wobj = wobj;
    in_obj->sio_status = M_BASE_STATUS_RUNNING;

    DEBUG_TEST(DB_W,"m_static_instance_running_running_to_run_switch");

    // 记下当前区域的区域类型 用来作事件处理
    pre_ste = wobj->unit_running_type;
    // printf("%hhd\n\n" , wobj->unit_running_channel);

    // 判断下 当前单元是否完成 如果没有的话 记录下
    if (wobj->unit_this_time_will_running_time < in_time_s - wobj->unit_this_time_running_start_time)
    {
        wobj->state = M_STATIC_UNIT_STATUS_INTERRUPT;
    }
    if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_LEFT)
    { // 上一个
        // 找到上一个运行区域，并且把他指定为当前区域
        stat = m_static_instance_manage_tool_find_next_sequence(in_obj->instance_running_root_unit);
    }
    else if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_RIGHT)
    { // 下一个
        // 找到下一个运行区域，并且把他指定为当前区域
        // stat = succ_r;
        stat = m_static_instance_manage_tool_find_pre_sequence(in_obj->instance_running_root_unit);
    }
    if (stat == succ_r)
    {
        wobj = in_obj->instance_running_root_unit->current_running_unit;
        wobj->state = M_STATIC_UNIT_STATUS_RUNNING;
        if (wobj->next_running_unit == NULL)
            next_ste = M_BASE_STATUS_STOP; // 下一个是运行还是浸泡呢
        else
            next_ste = wobj->next_running_unit->unit_running_type; // 下一个是运行还是浸泡呢
        // 忘记还得修改下节点时间  将要运行的理论时间 减去 实际运行或者耗费的时间
        // printf("%hhd\n\n" , wobj->unit_running_channel);

        in_obj->WSTime = in_time_s + (wobj->unit_this_time_will_running_time - wobj->unit_this_time_cost_running_time);
        wobj->unit_this_time_real_water_statistics = 0;
        m_static_time_link_notece_node_change(in_obj->sio_running_id, None, M_BASE_STATUS_RUNNING);
        m_static_time_link_notece_node_change(in_obj->sio_running_id, in_obj->WSTime, next_ste);
        // 做事件处理
        m_static_instance_running_event_handle(cause, pre_ste, wobj->unit_running_type, pre_wobj, in_obj, in_time_s);
        // // 切换下
        // if (wobj->next_running_unit != NULL)
        //     in_obj->instance_running_root_unit->current_running_unit = wobj->next_running_unit;
    }
    return 0;
}

/**
 * @brief 根据 30 返回的参数生成对象的运行实例
 *
 * @param in_uid 对象的id
 * @param in_time_s 发生的时间
 * @return stat_m 成功或者失败
 */
stat_m m_callable_instance_running_gener_obj(uint64_t in_uid, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    instance_obj *in_obj = NULL;
    instance_doc_t *wtd = NULL;
    enum base_state ste = M_BASE_STATUS_IDLE;
    int weather = 0;

    if (m_callable_instance_manage_recover_queue_get_and_exec(in_uid, in_time_s) == fail_r)
    {
        stat = m_static_instance_list_find_by_id(in_uid, &wtd);

        if (wtd != NULL && stat == succ_r && wtd->is_genger_info == M_INSTANCE_IS_GENER_INFO_WAIT)
        {
            wtd->exfun.fn_water_skip = 0;
            DEBUG_TEST(DB_W,"Not a recovery plan that needs to be executed ,%d", wtd->is_genger_info);
            in_obj = M_MALLOC(sizeof(instance_obj));
            memset(in_obj, 0, sizeof(instance_obj));
            // 查看/获取  30 反馈的天气跳过信息
            m_callable_weather_manage_get_is_effective(in_uid, &weather);
            DEBUG_TEST(DB_W,"30 -> m_callable_weather_manage_get_is_effective: %d", weather);
            // 生成数据
            {
                if (weather && weather != WEATHER_SATURATION_SKIP)
                    stat = fail_r;
                else
                {
                    in_obj->sio_skip_weather = weather;
                    stat = m_callable_instance_manage_instance_gener(wtd, in_obj, NULL, &weather, in_time_s);
                    wtd->exfun.fn_water_skip = weather;
                }
                DEBUG_TEST( DB_I,"Skip Code  ------ > %d\n ", weather);
                // if (stat == fail_r)
                // {
                // if (weather == 0)
                // wtd->exfun.fn_water_skip = M_SKIP_MANAGE_TYPE_SATURATION_SKIP;
                // else
                // ste = M_BASE_STATUS_IDLE;
                // }

                ste = M_BASE_STATUS_RUNNING;
                // 改掉对象
                if (stat == succ_r)
                {
                    DEBUG_TEST( DB_I,"Stop Next entering .. ");
                    wtd->is_genger_info = M_INSTANCE_IS_GENER_INFO_INIT;
                    if (in_obj != NULL)
                        in_obj->is_genger_info = M_INSTANCE_IS_GENER_INFO_DONG;
                    m_static_time_link_notece_node_change_obj(in_uid, None, ste, in_obj, M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL);
                }
                else
                    m_static_time_link_notece_node_change_obj(in_uid, None, ste, NULL, M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL);
            }
            // else
            //     wtd->exfun.fn_water_skip = weather;
        }

        else
            stat = fail_r;
    }
    return stat;
}

stat_m m_static_instance_running_idle_to_prepare(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    DEBUG_TEST( DB_I,"m_static_instance_running_idle_to_prepare");
    stat_m stat = fail_r;
    // instance_obj *in_obj = NULL;
    instance_doc_t *wtd = (instance_doc_t *)pdata;
    enum skip_manage_type skip_type;
    enum seneor_chann sen_chan;
    enum seneor_type sen_type;
    int weather_code = 0;

    // in_obj = M_MALLOC(sizeof(instance_obj));
    // memset(in_obj, 0, sizeof(instance_obj));
    // 在运行之前 检查一下
    instance_obj *current_instance_ab = NULL;
    stat = m_static_instance_manage_get_current_instance(&current_instance_ab);
    if (current_instance_ab != NULL)
        DEBUG_TEST(DB_W,"%lld    %lld   %d", current_instance_ab->sio_running_id, wtd->sch_info.cu_id, wtd->is_genger_info);

    if (wtd->is_genger_info == M_INSTANCE_IS_GENER_INFO_INIT && ((current_instance_ab != NULL && current_instance_ab->sio_running_id != wtd->sch_info.cu_id) ||
                                                                 current_instance_ab == NULL))
    {
        stat = m_callable_skip_manage_query_is_effective(wtd->sch_info.cu_id, &skip_type, &sen_chan, &sen_type, &weather_code, in_time_s);
        if (stat == fail_r)
        { // 说明跳过了 , 触发事件   在这 不触发
            DEBUG_TEST(DB_W," Need Skip -> %d  But dnot send 30  Handle ...", skip_type);

            ste = M_BASE_STATUS_RUNNING;
            // m_static_instance_running_skip_notify_event_handle(wtd->sch_info.cu_id , skip_type , in_time_s);
            // m_static_time_link_notece_node_change(wtd->sch_info.cu_id, None, M_BASE_STATUS_RUNNING);
            // m_static_time_link_notece_node_change(wtd->sch_info.cu_id, None, M_BASE_STATUS_PREPARE);
            m_static_time_link_notece_node_change(wtd->sch_info.cu_id, None, M_BASE_STATUS_PREPARE);
        }
        else
        {
            ste = M_BASE_STATUS_PREPARE;
            // 事件触发
            m_static_instance_running_event_handle(cause, M_BASE_STATUS_IDLE, M_BASE_STATUS_PREPARE, NULL, wtd, in_time_s);
            //  m_static_instance_running_event_handle(cause, M_BASE_STATUS_IDLE, wobj->unit_running_type, in_obj, in_time_s);
        }
        m_static_time_link_notece_node_change(wtd->sch_info.cu_id, None, ste);
    }
    return stat;
}

/**
 * @brief 一开始直接运行
 *
 * @return stat_m
 */
stat_m m_static_instance_running_idleing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    // enum base_state next_status = M_BASE_STATUS_RUNNING;
    instance_obj *in_obj = NULL;
    instance_doc_t *wtd = NULL;
    enum base_state next_ste = M_BASE_STATUS_IDLE;
    stat_m stat = fail_r;
    int skip_type = 0;
    // enum skip_manage_type skip_type = 0;
    enum seneor_chann sen_chan = 0;
    enum seneor_type sen_type = 0;
    int weather_code = 0;
    uint64_t tp_uid = 0;
    int tp_skip = 0;

    // printf("%d\n" ,sizeof(instance_obj) );
    // printf("%d\n" ,sizeof(instance_doc_t) );
    // printf("%d\n" ,strlen(pdata) );
    // printf("    ..    %d\n" ,cause );
    m_callable_device_proper_status_get(&tp_skip);
    if (tp_skip == M_DEVICE_GLOBAL_STATUS_UPDATEING)
    {
        tp_skip = debug_schedule_running_but_device_update_status;
        m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                        M_TYPE_Int, (void *)&tp_skip,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL,
                                        in_time_s, true); //_time.new_get_hhmmss()
        DEBUG_TEST(DB_W,"m_callable_device_proper_status_get() == M_DEVICE_GLOBAL_STATUS_UPDATEING");
        return fail_r;
    }
    tp_skip = 0;

    if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL)
    {
        DEBUG_TEST( DB_I,"instance_obj");
        in_obj = (instance_obj *)pdata;
        tp_uid = in_obj->sio_running_id;
        stat = succ_r;
        in_obj->is_genger_info = M_INSTANCE_IS_GENER_INFO_INIT;
    }
    else
    {
        DEBUG_TEST( DB_I,"instance_doc_t");
        wtd = (instance_doc_t *)pdata;
        in_obj = malloc(sizeof(instance_obj));
        memset(in_obj, 0, sizeof(instance_obj));
        tp_uid = wtd->sch_info.cu_id;
        wtd->is_genger_info = in_obj->is_genger_info = M_INSTANCE_IS_GENER_INFO_INIT;
        tp_skip = wtd->exfun.fn_water_skip;
        wtd->exfun.fn_water_skip = 0;
        m_static_time_link_notece_node_change(tp_uid, None, M_BASE_STATUS_RUNNING);
        // m_static_time_link_notece_node_change(wtd->sch_info.cu_id, None, ste);
    }
    /** 不官🛕情况  看下跳过*/
    {
        // 在运行之前 检查一下
        if (wtd != NULL)
            in_obj->sio_running_id = wtd->sch_info.cu_id;
        // stat = m_callable_skip_manage_query_is_effective(tp_uid, &skip_type, in_time_s);
        stat = m_callable_skip_manage_query_is_effective(in_obj->sio_running_id, &skip_type, &sen_chan, &sen_type, &weather_code, in_time_s);
        // stat = m_callable_skip_manage_query_is_effective(wtd->sch_info.available_time.tr_start_time, wtd->sch_info.available_time.tr_end_time, in_obj->sio_running_id, &skip_type, &sen_chan, &sen_type, &weather_code, in_time_s);
        // if (tp_skip != 0)
        //     skip_type = tp_skip;
        DEBUG_TEST(DB_W,"Skip old %d -- Skip New %d  -- WeatherCode %d", tp_skip, skip_type, weather_code);

        if (stat == succ_r && tp_skip == M_SKIP_MANAGE_TYPE_SATURATION_SKIP)
        {
            stat = fail_r;
            skip_type = M_SKIP_MANAGE_TYPE_SATURATION_SKIP;
        }
        if (tp_skip == M_SKIP_MANAGE_TYPE_ALL_CHANNEL_DISABLE)
        {
            stat = fail_r;
            skip_type = M_SKIP_MANAGE_TYPE_ALL_CHANNEL_DISABLE;
        }
        if (stat == fail_r && skip_type != M_SKIP_MANAGE_TYPE_SENSOR_SKIP)
        { // 说明跳过了 , 触发事件   在这 不触发
            stat = fail_r;

            // in_obj->sio_instance_id = in_time_s + in_obj->sio_max_coust_time;
            DEBUG_TEST(DB_W,"Event Skip -> %s ", toname(skip_type));
            if (wtd != NULL)
                m_static_instance_running_skip_notify_event_handle(wtd, in_obj, wtd->exfun.fn_max_coust_time + in_time_s,
                                                                   wtd->cmg_channel[0].cd_cahnnel_id, skip_type,
                                                                   sen_chan, sen_type, weather_code, in_time_s);
            else
                m_static_instance_running_skip_notify_event_handle(wtd, in_obj, in_obj->sio_instance_id,
                                                                   in_obj->sio_channel[0].cd_cahnnel_id, skip_type,
                                                                   sen_chan, sen_type, weather_code, in_time_s);
            m_callable_weather_manage_clean(tp_uid);
        }
        else if (cause != M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL)
        {
            stat = succ_r;
            // 临时生成数据
            stat = m_callable_instance_manage_instance_gener(wtd, in_obj, NULL, NULL, in_time_s);
            // 改掉对象
            if (stat != succ_r)
                stat = fail_r;
            else
                m_static_time_link_notece_node_change_obj(in_obj->sio_running_id, None, None, in_obj, M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL);
        }
    }
    if (skip_type == M_SKIP_MANAGE_TYPE_SENSOR_SKIP)
    {
        in_obj->instance_running_root_unit->current_running_unit = in_obj->instance_running_root_unit->head_running_unit;
        m_static_instance_running_skip_notify_event_handle(wtd, in_obj, in_obj->sio_instance_id,
                                                           in_obj->instance_running_root_unit->current_running_unit->unit_running_channel,
                                                           skip_type,
                                                           sen_chan, sen_type, weather_code, in_time_s);
    }
    else
    {
        if (stat == succ_r)
        {
            /** 一定开始运行。在这里标记不会恢复的计划 比如部分饱和跳过 */
            if (in_obj->sio_skip_weather == WEATHER_SATURATION_SKIP)
                m_static_instance_enreg_schedule_end_time(in_obj->sio_running_id, in_obj->sio_instance_id);

            // 第一步应该是酸下这个状态应该保持的时间，定一个时间节点
            in_obj->instance_running_root_unit->current_running_unit = in_obj->instance_running_root_unit->head_running_unit;

            int device_state = 0;
            // 在这里应之一切运行 包含快速运行和手动运行
            m_callable_device_proper_status_get(&device_state);
            if (device_state != M_DEVICE_GLOBAL_STATUS_IDLE)
            {
                DEBUG_TEST(DB_W,"Schedule to Start , Stop other runs ...");
                if (device_state == M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING)
                    // m_callable_drive_button_event_direct_input(M_KEY_EVENT_STOP);
                    m_callable_drive_button_direct_stop();
                if (device_state == M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING)
                    m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, true, in_time_s);

                // m_callable_drive_button_event_input(M_KEY_EVENT_STOP);
                // m_ext_drive_button_event_handle(M_KEY_EVENT_STOP, M_KEY_EVENT_STOP, 0, 0, in_time_s);
                // while (m_callable_display_status_get() == M_DISPLAY_ZONE_RUNNING_MODE)
                //     mDelay_ms(30);
            }
            // 更新设备状态
            m_callable_device_proper_status_update_to_schedule_running();
            // 拿到对象
            struct instance_running_unit *wobj = in_obj->instance_running_root_unit->head_running_unit;
            // 我在运行,,填充计划运行对象
            m_static_instance_manage_set_current_instance(in_obj);
            // 时间调整下
            in_obj->WSTime += wobj->unit_this_time_will_running_time;
            in_obj->WSTime += in_time_s;
            in_obj->sio_status = M_BASE_STATUS_RUNNING;
            // /** 驱动端口 */
            if (wobj->next_running_unit != NULL)
                next_ste = wobj->next_running_unit->unit_running_type;
            else
                // 可能只有一个序列,快结束了
                next_ste = M_BASE_STATUS_STOP;
            // 消化状态 修改下一个节点的状态
            m_static_time_link_notece_node_change(in_obj->sio_running_id, in_obj->WSTime, next_ste);
            // 事件触发
            m_static_instance_running_event_handle(cause, M_BASE_STATUS_PREPARE, wobj->unit_running_type, NULL, in_obj, in_time_s);
            // // 执行完了 指向下一个区域 2
            // in_obj->instance_running_root_unit->current_running_unit = wobj->next_running_unit;
        }
        else
        {

            DEBUG_TEST( DB_I,"Update this schedule time  ...");
            char sKeyss[30] = {0};
            tp_uid = in_time_s + wtd->exfun.fn_max_coust_time;
            // sprintf(sKeyss, "%s%lld", "Ss", wtd->sch_info.cu_id);
            // m_callable_drive_flash_write(M_TYPE_U64, sKeyss, (void *)&tp_uid);
            m_static_instance_enreg_schedule_end_time(wtd->sch_info.cu_id, tp_uid);

            m_callable_weather_manage_clean(wtd->sch_info.cu_id);
            m_callable_smart_running_storage_clean(wtd->sch_info.cu_id);

            stat = m_static_tools_instance_obj_refresh_start_time(wtd, NULL, tp_uid);
            M_FREE(in_obj);
            // stat = m_static_time_link_notece_node_change_obj(wtd->sch_info.cu_id, wtd->WSTime, M_BASE_STATUS_IDLE, wtd, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
            stat = m_static_time_link_notece_node_change_obj(wtd->sch_info.cu_id, wtd->WSTime, M_BASE_STATUS_IDLE, wtd, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
            weather_code = M_BASE_STATUS_IDLE;
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                            M_TYPE_U64, (void *)(&weather_code),
                                            M_TYPE_U64, (void *)(&wtd->sch_info.cu_id),
                                            M_TYPE_U64, (void *)(&wtd->WSTime),
                                            M_TYPE_Int, (void *)&skip_type,
                                            in_time_s, false); //_time.new_get_hhmmss()

            in_obj = NULL;
            // 先停止
            // stat = m_static_instance_list_find_by_id(in_obj->sio_running_id, &wtd);

            // m_static_inst_fsm_clear();
            // stat = m_static_tools_instance_obj_refresh_start_time(wtd, in_time_s);

            /**
             * @brief 这里可以避免在清除的函数里面 搜索list对象 wtd
             *
             */
            // stat = m_static_instance_running_clear(in_obj, in_time_s);
        }
    }
    return 0;
}
/**
 * @brief running running running to running
 *
 * @param cause 触发原因
 * @param ste 当前状态
 * @param pdata 数据
 * @param in_time_s 触发时间
 * @return stat_m
 */
stat_m m_static_instance_running_running_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    // 运行到运行 就是当前的区域到下一个区域都是运行
    instance_obj *wtd = (instance_obj *)pdata;
    enum base_state next_ste = M_BASE_STATUS_IDLE;
    //  记录下 将要结束 马上结束的 区域
    struct instance_running_unit *pre_wobj = wtd->instance_running_root_unit->current_running_unit;
    if (pre_wobj == NULL)
    {
        DEBUG_TEST( DB_I,"GGGGGGGGG... \n");
    }
    //  更新区域状态
    pre_wobj->state = M_STATIC_UNIT_STATUS_DONE;
    DEBUG_TEST( DB_I,"%lld m_static_instance_running_running_to_run(%d) ", wtd->sio_instance_id, ste);

    // 切换下 找到本次运行的序列
    while (wtd->instance_running_root_unit->current_running_unit->next_running_unit != NULL)
    {
        wtd->instance_running_root_unit->current_running_unit = wtd->instance_running_root_unit->current_running_unit->next_running_unit;
        if (wtd->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time > wtd->instance_running_root_unit->current_running_unit->unit_this_time_cost_running_time + 2)
        {
            break;
        }
    }

    // 拿到运行单元
    struct instance_running_unit *wobj = wtd->instance_running_root_unit->current_running_unit;
    // 把状态改下
    wtd->sio_status = M_BASE_STATUS_RUNNING;
    // 时间
    DEBUG_TEST( DB_I,"%d    %d  ", wobj->unit_this_time_will_running_time, wobj->unit_this_time_cost_running_time);

    wtd->WSTime += (wobj->unit_this_time_will_running_time - wobj->unit_this_time_cost_running_time);
    // 判断下 当前序列是否是完整的 ，正常进来应该都是完整的，不正常进不来
    // 是否结束
    if (wobj->next_running_unit == NULL)
        next_ste = M_BASE_STATUS_STOP;
    else
        next_ste = wobj->next_running_unit->unit_running_type;

    // 记下 开始时间 后面恢复用得上  XXX
    // wobj->unit_this_time_running_start_time = in_time_s;
    wobj->unit_this_time_real_water_statistics = 0;

    // 节点修改
    m_static_time_link_notece_node_change(wtd->sio_running_id, wtd->WSTime, next_ste); // 消化状态 修改下一个节点的状态
    // 事件触发通知
    m_static_instance_running_event_handle(cause, ste, wobj->unit_running_type, pre_wobj, wtd, in_time_s);
    return 0;
}
/**
 * @brief    sack to running
 *
 * @param cause 触发原因
 * @param ste 当前状态
 * @param pdata 数据
 * @param in_time_s 触发时间
 */
stat_m m_static_instance_running_sacking_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    instance_obj *wtd = (instance_obj *)pdata;
    enum base_state next_ste = M_BASE_STATUS_IDLE;
    wtd->sio_status = M_BASE_STATUS_RUNNING;

    DEBUG_TEST( DB_I,"[%lld] m_static_instance_running_sacking_to_run  ", in_time_s);
    // 切换下 找到本次运行的序列

    // if (wtd->instance_running_root_unit->current_running_unit->next_running_unit != NULL)
    //     wtd->instance_running_root_unit->current_running_unit = wtd->instance_running_root_unit->current_running_unit->next_running_unit;
    while (wtd->instance_running_root_unit->current_running_unit->next_running_unit != NULL)
    {
        wtd->instance_running_root_unit->current_running_unit = wtd->instance_running_root_unit->current_running_unit->next_running_unit;
        if (wtd->instance_running_root_unit->current_running_unit->unit_this_time_will_running_time > wtd->instance_running_root_unit->current_running_unit->unit_this_time_cost_running_time + 2)
        {
            break;
        }
    }

    // 拿到运行单元
    struct instance_running_unit *wobj = wtd->instance_running_root_unit->current_running_unit;
    // 替换下当前指向的序列
    wtd->WSTime += (wobj->unit_this_time_will_running_time - wobj->unit_this_time_cost_running_time);
    // 判断下 当前序列是否是完整的 ，正常进来应该都是完整的，不正常进不来
    wobj->state = M_STATIC_UNIT_STATUS_DONE;
    // 记下 开始时间 后面恢复用得上 XXXX
    // wobj->unit_this_time_running_start_time = in_time_s;
    // 是否结束
    if (wobj->next_running_unit == NULL)
        next_ste = M_BASE_STATUS_STOP;
    else
        next_ste = wobj->next_running_unit->unit_running_type;

    // 节点修改
    m_static_time_link_notece_node_change(wtd->sio_running_id, wtd->WSTime, next_ste); // 消化状态 修改下一个节点的状态
                                                                                       // 事件触发通知
    m_static_instance_running_event_handle(cause, ste, wobj->unit_running_type, NULL, wtd, in_time_s);
    return stat;
}
stat_m m_static_instance_running_hanguping_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s) { return 0; }

stat_m m_static_instance_running_pauseing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_W,"m_static_instance_running_pauseing_to_run");
    instance_obj *in_obj = (instance_obj *)pdata;
    enum base_state next_ste = M_BASE_STATUS_IDLE;

    if (in_obj != NULL)
    {
        in_obj->sio_status = M_BASE_STATUS_RUNNING;
        // 拿到本次应该运行单元
        struct instance_running_unit *wobj = in_obj->instance_running_root_unit->current_running_unit;
        if (wobj->unit_this_time_will_running_time == wobj->unit_this_time_cost_running_time)
        {
            if (wobj->next_running_unit == NULL)
            {
                next_ste = M_BASE_STATUS_STOP;
                DEBUG_E("Err");
                wobj->unit_this_time_will_running_time -= wobj->unit_this_time_cost_running_time;
                m_static_time_link_notece_node_remove_by_id(in_obj->sio_running_id);
                m_static_instance_running_clear(in_obj, in_time_s);
                return 0;
            }
            else
            {
                wobj = wobj->next_running_unit;
                in_obj->instance_running_root_unit->current_running_unit = wobj;
            }
        }

        DEBUG_TEST(DB_W,".. .. . . .. %d -  unit_this_time_cost_running_time %d", wobj->unit_this_time_will_running_time, wobj->unit_this_time_cost_running_time);
        // wobj->unit_this_time_will_running_time -= wobj->unit_real_water_statistics;
        // 算下时间
        in_obj->WSTime = in_time_s;
        in_obj->WSTime += (wobj->unit_this_time_will_running_time - wobj->unit_this_time_cost_running_time);
        // 更新 或者朝霞下一个运行状态
        if (wobj->next_running_unit == NULL)
            next_ste = M_BASE_STATUS_STOP;
        else
            next_ste = wobj->next_running_unit->unit_running_type;
        m_static_time_link_notece_node_enable(in_obj->sio_running_id);
        // 节点修改
        m_static_time_link_notece_node_change(in_obj->sio_running_id, in_obj->WSTime, M_BASE_STATUS_RUNNING); // 消化状态 修改下一个节点的状态
        m_static_time_link_notece_node_change(in_obj->sio_running_id, in_obj->WSTime, next_ste);              // 消化状态 修改下一个节点的状态
                                                                                                              // 事件触发通知
        m_static_instance_running_event_handle(cause, ste, wobj->unit_running_type, NULL, in_obj, in_time_s);
        wobj->unit_this_time_real_water_statistics = 0;
        // m_static_instance_running_event_handle(cause, ste, wobj->unit_running_type, NULL, in_obj, in_time_s);
    }
    return stat;
}
stat_m m_static_instance_running_pauseing_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s) { return 0; }

stat_m m_static_instance_running_running_to_pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_W,"m_static_instance_running_running_to_pause");
    instance_obj *in_obj = (instance_obj *)pdata;
    if (in_obj != NULL)
    {
        in_obj->sio_status = M_BASE_STATUS_PAUSE;
        struct instance_running_unit *pre_wobj = in_obj->instance_running_root_unit->current_running_unit;
        //  更新区域状态
        pre_wobj->state = M_STATIC_UNIT_STATUS_DONE;
        // 拿到运行单元
        struct instance_running_unit *wobj = in_obj->instance_running_root_unit->current_running_unit;

        m_static_time_link_notece_node_disable(in_obj->sio_running_id);
        m_static_instance_running_event_handle(cause, ste, in_obj->sio_status, pre_wobj, in_obj, in_time_s);
        wobj->unit_this_time_real_water_statistics = 0;
        in_obj->cb_total_already_run_time_one_round = 0;

        //  m_static_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status, wobj, in_time_s);
    }

    return stat;
}
stat_m m_static_instance_running_sacking_to_pause(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = fail_r;

    instance_obj *wtd = (instance_obj *)pdata;
    // 拿到运行单元
    struct instance_running_unit *wobj = wtd->instance_running_root_unit->current_running_unit;

    //  m_static_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status, wobj, in_time_s);
    return stat;
}

// stat_m m_static_instance_running_pauseing_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s) { return 0; }
stat_m m_static_instance_running_hanguping_to_run(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s) { return 0; }

stat_m m_static_instance_running_running_to_sack(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;

    instance_obj *wtd = (instance_obj *)pdata;
    enum base_state next_ste = M_BASE_STATUS_IDLE;

    DEBUG_TEST( DB_I,"[%lld] m_static_instance_running_running_to_sack  ", in_time_s);

    wtd->sio_status = M_BASE_STATUS_SACK;
    //  记录下 将要结束 马上结束的 区域
    struct instance_running_unit *pre_wobj = wtd->instance_running_root_unit->current_running_unit;
    //  更新区域状态
    pre_wobj->state = M_STATIC_UNIT_STATUS_DONE;
    // 切换下 找到本次运行的序列
    if (wtd->instance_running_root_unit->current_running_unit->next_running_unit != NULL)
        wtd->instance_running_root_unit->current_running_unit = wtd->instance_running_root_unit->current_running_unit->next_running_unit;
    // 拿到运行单元
    struct instance_running_unit *wobj = wtd->instance_running_root_unit->current_running_unit;

    // 替换下当前指向的序列
    wtd->WSTime += wobj->unit_this_time_will_running_time;
    // 判断下 当前序列是否是完整的 ，正常进来应该都是完整的，不正常进不来
    // 下一个节点的状态
    next_ste = wobj->next_running_unit->unit_running_type;
    // 节点修改s
    m_static_time_link_notece_node_change(wtd->sio_running_id, wtd->WSTime, next_ste); // 消化状态 修改下一个节点的状态
                                                                                       // 事件触发通知
    m_static_instance_running_event_handle(cause, ste, wobj->unit_running_type, pre_wobj, wtd, in_time_s);
    return stat;
}
stat_m m_static_instance_running_running_to_hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = fail_r;

    instance_obj *wtd = (instance_obj *)pdata;
    // 拿到运行单元
    struct instance_running_unit *wobj = wtd->instance_running_root_unit->current_running_unit;

    //  m_static_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status, wobj, in_time_s);
    return stat;
}
stat_m m_static_instance_running_sacking_to_hangup(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = fail_r;

    instance_obj *wtd = (instance_obj *)pdata;
    // 拿到运行单元
    struct instance_running_unit *wobj = wtd->instance_running_root_unit->current_running_unit;

    //  m_static_instance_running_event_handle(cause, wobj->running_buff->rb_pre_stated, wobj->sio_status, wobj, in_time_s);
    m_static_instance_running_event_handle(cause, wobj->pre_running_unit->unit_running_type, wobj->unit_running_type, NULL, wtd, in_time_s);

    return stat;
    return 0;
}

/**
 * @brief all to stop
 *
 * @return stat_m
 */
stat_m m_static_instance_running_all_to_stop(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    // instance_obj *wobj = (instance_obj *)pdata;
    // if (wobj != NULL)
    DEBUG_TEST(DB_W,"m_static_instance_running_all_to_stop");
    {

        instance_obj *wtd = (instance_obj *)pdata;
        if (wtd->instance_running_root_unit == NULL)
            stat = fail_r;
        if (stat == succ_r)
        {
            struct instance_running_unit *wobj = wtd->instance_running_root_unit->current_running_unit;
            wtd->sio_status = M_BASE_STATUS_IDLE;
            //  m_static_instance_running_event_handle(cause, pre_ste, wobj->sio_status, wobj, in_time_s);
            m_static_instance_running_event_handle(cause, ste, M_BASE_STATUS_STOP, NULL, wtd, in_time_s);
            m_static_instance_running_clear(wtd, in_time_s);
        }
    }
    return stat;
}
/**
 * @brief all to stop
 *
 * @return stat_m
 */
stat_m m_static_instance_running_all_to_idle(enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    // instance_obj *wobj = (instance_obj *)pdata;
    // if (wobj != NULL)
    {

        instance_obj *wtd = (instance_obj *)pdata;
        if (wtd->instance_running_root_unit == NULL)
            stat = fail_r;
        if (stat == succ_r)
        {
            struct instance_running_unit *wobj = wtd->instance_running_root_unit->current_running_unit;
            wtd->sio_status = M_BASE_STATUS_IDLE;
            //  m_static_instance_running_event_handle(cause, pre_ste, wobj->sio_status, wobj, in_time_s);
            m_static_instance_running_event_handle(cause, ste, M_BASE_STATUS_STOP, NULL, wtd, in_time_s);
            m_static_instance_running_clear(wtd, in_time_s);
        }
    }
    return stat;
}

stat_m m_static_instance_running_hammer_handle(uint8_t event_fag, enum fsm_change_cause cause, enum base_state ste, void *pdata, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    // instance_obj *wobj = (instance_obj *)pdata;
    if (event_fag == M_BASE_STATUS_RUNNING && ste == M_BASE_STATUS_RUNNING)
    {
        DEBUG_TEST(DB_W,"m_static_instance_running_hammer_handle !");
        // instance_obj *in_obj = (instance_obj *)pdata;
        m_static_instance_running_event_handle(cause, M_BASE_STATUS_RUNNING,
                                               M_BASE_STATUS_RUNNING, NULL, pdata, in_time_s);
    }
    return stat;
}

// 一键停止 计划运行（传入 Bool Bool）
//                      表示是否是手动停止 记录
//                      并表示是否需要反馈服务器

// 一键刷新信息   清空信息

stat_m m_static_instance_running_clear(instance_obj *in_obj, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    instance_doc_t *in_wtd;
    if (in_obj != NULL)
    {
        if (in_obj->instance_running_root_unit != NULL)
        {
            m_static_instance_runnning_queue_clear(in_obj->instance_running_root_unit);
        }
        stat = succ_r;
    }
    m_callable_weather_manage_clean(in_obj->sio_running_id);
    m_callable_smart_running_storage_clean(in_obj->sio_running_id);
    stat = m_static_instance_list_find_by_id(in_obj->sio_running_id, &in_wtd);
    if (stat == succ_r && in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
    {
        in_wtd->exfun.fn_water_skip = 0;
        // in_wtd->is_genger_info = false;
        stat = m_static_tools_instance_obj_refresh_start_time(in_wtd, NULL, in_time_s);
        stat = m_static_time_link_notece_node_change_obj(in_wtd->sch_info.cu_id, in_wtd->WSTime, M_BASE_STATUS_IDLE, in_wtd, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
        stat = m_static_time_link_notece_node_change_obj(in_wtd->sch_info.cu_id, in_wtd->WSTime, M_BASE_STATUS_IDLE, in_wtd, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
        in_wtd->exfun.fn_water_skip = M_BASE_STATUS_IDLE;
        m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                        M_TYPE_U32, (void *)(&in_wtd->exfun.fn_water_skip),
                                        M_TYPE_U64, (void *)(&in_wtd->sch_info.cu_id),
                                        M_TYPE_U64, (void *)(&in_wtd->WSTime),
                                        M_TYPE_NULL, NULL,
                                        in_time_s, false);
        in_wtd->exfun.fn_water_skip = 0;
        m_static_instance_manage_clean_current_instance();
        M_FREE(in_obj);
        in_obj = NULL;
    }
    if (in_obj != NULL && in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_FAST_RUN)
    {
        m_static_instance_manage_clean_current_instance();
        m_static_time_link_notece_node_remove_by_id(in_obj->sio_running_id);
        M_FREE(in_obj);
        in_obj = NULL;
    }
    return stat;
}
extern void m_static_inst_fsm_clear(void);
/**
 * @brief 停止计划运行
 *
 * @param cause 如果是手动停止记录停止的时间
 * @param is_callback 是否需要反馈
 * @param in_time_s 触发的时间
 * @return stat_m
 */
stat_m m_callable_instance_running_forced_stop(enum fsm_change_cause cause, bool is_callback, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    instance_obj *in_obj_clone = NULL;
    instance_doc_t *wtd = NULL;

    m_static_instance_manage_get_current_instance(&in_obj_clone);
    if (in_obj_clone != NULL) // 当前有在运行的计划
    {
        in_obj_clone->sio_status = M_BASE_STATUS_IDLE;
        DEBUG_TEST(DB_W,"m_callable_instance_running_forced_stop %lld", in_obj_clone->sio_running_id);
        if (cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL || cause == M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE)
        {
            m_static_instance_enreg_schedule_end_time(in_obj_clone->sio_running_id, in_obj_clone->sio_instance_id);
        }
        if (!is_callback)
            cause = M_STATIC_FSM_CHANGE_STATE_CAUSE_NOOE_CALLBACK;
        // 先停止
        m_static_instance_running_event_handle(cause, M_BASE_STATUS_RUNNING,
                                               M_BASE_STATUS_STOP, NULL, in_obj_clone, in_time_s);
        m_static_inst_fsm_clear();

        stat = m_static_instance_running_clear(in_obj_clone, in_time_s);
    }

    return stat;
}

/**
 * @brief 记录计划终止时间，防止下次计算
 *
 */
void m_static_instance_enreg_schedule_end_time(uint64_t uid, uint64_t in_time)
{
    char sKeyss[40] = {0};
    DEBUG_TEST( DB_I,"end reg -> %lld - %lld", uid, in_time);
    
    sprintf(sKeyss, "%s%lld", "Ss", uid);
    if (in_time)
        m_callable_drive_flash_write(M_TYPE_U64, sKeyss, (void *)&in_time);
    else
        m_callable_drive_flash_remove(sKeyss);
}