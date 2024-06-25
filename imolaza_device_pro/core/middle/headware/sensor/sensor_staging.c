/**
 * @file sensor_staging.c
 * @author your name (you@domain.com)
 * @brief  传感器触发的计划暂存文件 即恢复
 * @version 0.1
 * @date 2023-11-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "core/fset_lib.h"
#include "core/application/instance_manage/manage/instance_list.h"
#include "core/application/instance_manage/instance.h"
#include "core/middle/event_distribution/time_link.h"
#include "core/application/instance_manage/tools/instance_algorithm.h"
#include "./sensor.h"

/**
 * @brief 在运行的传感器被触发
 *
 * @param in_obj 触发的对象
 * @param in_time_s 触发时间
 * @return stat_m
 */

struct _hide_global_obj
{
    instance_obj *hide_obj;
    uint64_t obj_end_time;
    /** 剩余运行时间 ( WSTime 和当前时间之间的差距 ) */
    uint64_t one_rema_runn_time;
};
struct _hide_global_obj hide_global_obj;

/**
 * @brief 某些计划可能需要被刷新继续的开始时间
 *
 * @param in_obj
 * @return stat_m
 */
stat_m m_static_sensor_flush(instance_obj *in_obj);

/**
 * @brief 直接添加一个传感器
 *
 * @param inobj 对象
 * @param in_time_s  时间
 * @return stat_m
 */
stat_m m_callable_sensor_staging_add_trig(void *inobj, uint64_t in_time_s)
{
    instance_obj *in_obj = (instance_obj *)inobj;

    if (hide_global_obj.hide_obj != NULL)
    {
        // m_static_sensor_flush(hide_global_obj.hide_obj);
        DEBUG_TEST( DB_I,"add_trig staging_clean %lld", hide_global_obj.hide_obj->sio_running_id);
        m_callable_sensor_staging_clean(M_CONST_NONE_NUM, None);
    }
    hide_global_obj.hide_obj = in_obj;
    memcpy(hide_global_obj.hide_obj, in_obj, sizeof(*in_obj));
    hide_global_obj.obj_end_time = in_obj->sio_instance_id;
    hide_global_obj.one_rema_runn_time = in_time_s;
    DEBUG_TEST(DB_W,"m_callable_sensor_staging_add_trig %lld , end time %lld", in_time_s, hide_global_obj.obj_end_time);
    // hide_global_obj.one_rema_runn_time += m_callable_timer_manage_get_device_zero_time();
    return succ_r;
}
/**
 * @brief 检查被触发的id 是不是传入ID
 *
 * @param in_chk_id ID
 * @return stat_m
 */
stat_m m_callable_sensor_staging_check_obj_id(uint64_t in_chk_id)
{
    stat_m stat = fail_r;
    if (hide_global_obj.hide_obj != NULL)
    {
        if (hide_global_obj.hide_obj->sio_running_id == in_chk_id)
            stat = succ_r;
    }
    return stat;
}

/**
 * @brief 传感器触发处理函数  作简单的清除 但是不会释放数据 ，等待恢复才会被释放 或者 超出运行时间
 *
 * @param in_obj 被触发的对象
 * @param in_time_s 这个不是触发的时间 而是 触发通道和触发类型的结合
 * @return stat_m
 */
stat_m m_callable_sensor_staging_trig_handle(void *inobj, uint64_t in_time_s)
{
    stat_m stat = fail_r;

    DEBUG_TEST( DB_I," m_callable_sensor_staging_trig_handle ( %lld ) ", in_time_s);

    instance_obj *in_obj = (instance_obj *)inobj;
    /** 触发区域 */
    int trig_channel = in_time_s / 100;
    /** 触发传感器类型 */
    int trig_sensor_type = in_time_s % 100;
    /** 触发类型 (是否跳过了计划)*/
    int trig_type = 2;
    float temp_float = 0.0f;
    /** 电流 */
    int temp_int = 0;
    in_time_s = m_callable_timer_manage_get_device_time();
    /**
         [6,计划ID，区域ID, 保留位，当前批次已浇水时间，计划本次有运行已浇水时长,计划结束时间，触发类型，传感器通道,时间戳]
                触发类型：0 传感器活跃       1 有计划被跳过 （不区分运行中和开始，运行中和刚刚开始区别在于区域开始时间 xx/0）
     */
    if (in_obj != NULL)
    {
        if (in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_FAST_RUN)
        {
            return 0;
        }
        stat = succ_r;
        hide_global_obj.hide_obj = in_obj;
        hide_global_obj.hide_obj->sio_status = M_BASE_STATUS_IDLE;
        hide_global_obj.obj_end_time = in_obj->sio_instance_id;
        hide_global_obj.one_rema_runn_time = (in_obj->sio_instance_id - in_obj->sio_max_coust_time);
        temp_int = m_callable_current_check_get_final_value();
        // hide_global_obj.one_rema_runn_time = (m_callable_timer_manage_get_hhmmss() - hide_global_obj.hide_obj->cb_already_run_time_sum);
        // hide_global_obj.one_rema_runn_time += m_callable_timer_manage_get_device_zero_time();

        // ((hide_global_obj.hide_obj->WSTime % NUM_OF_DAY_SEC_SUM) - in_obj->cb_already_run_time_sum );

        m_static_instance_manage_clean_current_instance();
        m_callable_device_proper_status_update_to_idle();
        m_callable_solenoid_manage_close(in_obj->instance_running_root_unit->current_running_unit->unit_running_channel);
        m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
        m_static_time_link_notece_node_disable(hide_global_obj.hide_obj->sio_running_id);

        m_callable_flow_sensor_get_total_value(0, in_obj->instance_running_root_unit->current_running_unit->unit_this_time_real_water_statistics,
                                               &temp_float, true);

 m_run_time.sensor_mode = M_CMD_TWOWAY_SENSOR_TRIGE;
        m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_SENSOR_TRIGE,
                                                M_TYPE_U64, (void *)&in_obj->sio_running_id,
                                                M_TYPE_U8, (void *)&in_obj->instance_running_root_unit->current_running_unit->unit_running_channel,
                                                M_TYPE_Int, (void *)&trig_sensor_type, // 0 常闭

                                                M_TYPE_U32, (void *)&in_obj->instance_running_root_unit->current_running_unit->unit_this_time_real_water_statistics,
                                                M_TYPE_U32, (void *)&in_obj->cb_already_run_time_sum,

                                                M_TYPE_U64, (void *)&in_obj->sio_instance_id,
                                                M_TYPE_Int, (void *)&trig_type,    // 类型
                                                M_TYPE_Int, (void *)&trig_channel, // 通道
                                                // M_TYPE_NULL, NULL,
                                                M_TYPE_Float, (void *)&temp_float, // 流量
                                                M_TYPE_Int, (void *)&temp_int,     // 电流
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                in_time_s);
        in_obj->cb_total_already_run_time_one_round = 0;
    }
    return stat;
}
/**
 * @brief 解除触发处理函数，用于计算恢复 如果不需要恢复 将会被清理释放
 *
 * @param in_obj 解除触发对象
 * @param in_time_s 触发通道和触发类型的结合
 * @return stat_m
 */
stat_m m_callable_sensor_staging_distrig_handle(uint64_t in_time_s)
{
    stat_m stat = fail_r;
    enum base_state pre_ste = M_BASE_STATUS_RUNNING;
    enum base_state next_ste = M_BASE_STATUS_RUNNING;

    DEBUG_TEST( DB_I," m_callable_sensor_staging_distrig_handle ( %lld ) ", in_time_s);

    instance_obj *in_obj = hide_global_obj.hide_obj;
    if (hide_global_obj.hide_obj == NULL)
        DEBUG_E("没有恢复可对象\n\n");
    if (in_obj != NULL)
    {
        /** 触发区域 */
        int trig_channel = in_time_s / 100;
        /** 触发传感器类型 */
        int trig_sensor_type = in_time_s % 100;
        /** 触发类型 (是否跳过了计划)*/
        int trig_type = 1;
        /** 单元剩余运行时间 */
        int temp_int_sy = 0;
        /** 本次开始的实例 开始运行时间 */
        uint64_t temp_u64_start_time = 0;
        // 剩余运行总时间
        uint32_t temp_u32_sy_sum_time = 0;
        // // 今天零点
        // uint64_t this_day_zero_time_timep = m_callable_timer_manage_get_device_zero_time();
        /** 电流 */
        uint32_t temp_int = 0;
        /** 准备运行队列队列 */
        char temp_queue[512] = {0};
        /** 偷偷计下 浸泡时间 */
        uint32_t temp_running_time = 0;

        in_time_s = m_callable_timer_manage_get_device_time();
        temp_u64_start_time = hide_global_obj.one_rema_runn_time;

        DEBUG_TEST(DB_W,"WSTime --->  %lld", in_obj->WSTime);
        /**  找到运行的计划单元，根据当前时间 当前时间很重要 */
        struct instance_running_unit *unit_next = NULL;
        unit_next = in_obj->instance_running_root_unit->head_running_unit;
        while (unit_next != NULL)
        {
            temp_u64_start_time += unit_next->unit_this_time_will_running_time;
            // DEBUG_TEST(DB_W,"in_time_s %lld    iwtd->WSTime %lld", in_time_s , temp_u64_start_time);
            /** 当前时间 小于 （计划开始时间 + 单元本次（叠加 ）就 = 每个小单元的结束时间） */
            /** 如果小于 说明 就应该在当前单元内恢复 */
            if (in_time_s < temp_u64_start_time)
            {
                temp_int_sy = temp_u64_start_time - (in_time_s);
                if (unit_next->unit_running_type == M_BASE_STATUS_SACK)
                    stat = should_sack_r;
                else if (unit_next->unit_running_type == M_BASE_STATUS_RUNNING)
                    stat = should_running_r;
                break;
            }
            else
                pre_ste = unit_next->unit_running_type;

            if (unit_next->unit_running_type == M_BASE_STATUS_RUNNING)
                temp_running_time += unit_next->unit_this_time_will_running_time;

            unit_next = unit_next->next_running_unit;
        }
        DEBUG_TEST(DB_W,"temp_u64_start_time --->  %lld", temp_u64_start_time);
        /*
            [7,计划ID，区域ID，总运行时间，区域将要运行时间，计划剩余总浇水时长,计划结束时间，运行状态，传感器通道，时间戳]
                    运行状态 ：1 运行 2浸泡
        */
        if (stat == should_running_r || stat == should_sack_r)
        {
            if (unit_next->next_running_unit != NULL)
                next_ste = unit_next->next_running_unit->unit_running_type;
            else
                // 可能只有一个序列,快结束了
                next_ste = M_BASE_STATUS_STOP;
            in_obj->instance_running_root_unit->current_running_unit = unit_next;
            // in_obj->WSTime = temp_int_sy + this_day_zero_time_timep;
            // in_obj->sio_instance_id = in_obj->WSTime + iwtd->exfun.fn_max_coust_time;

            // m_static_inst_fsm_pre_state_change(pre_ste);
            m_callable_device_proper_status_update_to_schedule_running();
            m_static_time_link_notece_node_enable(in_obj->sio_running_id);
            in_obj->WSTime = temp_u64_start_time;
            // m_static_time_link_notece_node_add(in_obj->sio_running_id, 2, M_BASE_STATUS_IDLE, in_obj);
            if (stat == should_running_r)
            {
                in_obj->sio_status = M_BASE_STATUS_RUNNING;
                pre_ste = M_BASE_STATUS_RUNNING;
                m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, unit_next->unit_running_channel);
                m_callable_solenoid_manage_open(unit_next->unit_running_channel);
                DEBUG_TEST( DB_I,"Recovery status: sRunning");
            }
            else
            {
                m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, unit_next->unit_running_channel);
                in_obj->sio_status = M_BASE_STATUS_SACK;
                pre_ste = M_BASE_STATUS_SACK;
                DEBUG_TEST( DB_I,"Recovery status: Sack");
            }
            m_static_time_link_notece_node_change_obj(in_obj->sio_running_id, temp_u64_start_time, pre_ste, in_obj, M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL);
            m_static_time_link_notece_node_change_obj(in_obj->sio_running_id, temp_u64_start_time, next_ste, in_obj, M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL);
            stat -= 9;
            hide_global_obj.hide_obj = in_obj;
            hide_global_obj.obj_end_time = in_obj->sio_instance_id;
            in_obj->instance_running_root_unit->current_running_unit = unit_next;
            m_static_instance_manage_set_current_instance(in_obj);

            DEBUG_TEST(DB_W,"should time %lld     run sum %d ", in_obj->sio_max_should_runtime, in_obj->cb_already_run_time_sum);
            // 计算剩余运行总时间，最大运行时间 - 本通道剩余 - 总已经运行
            temp_u32_sy_sum_time = (in_obj->sio_max_should_runtime - temp_int_sy) - temp_running_time;
            if (temp_u32_sy_sum_time < 0)
                temp_u32_sy_sum_time = temp_int_sy;
            m_static_instance_runnning_queue_str_format_get(in_obj, temp_queue);
            // 不需要电流
            // temp_int = m_callable_current_check_get_final_value();
            m_run_time.sensor_mode=M_CMD_TWOWAY_SENSOR_LEAVE;
            m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_SENSOR_LEAVE,
                                                    M_TYPE_U64, (void *)&in_obj->sio_running_id,              // 运行ID
                                                    M_TYPE_U8, (void *)&unit_next->unit_running_channel,      // 运行区域
                                                    M_TYPE_U32, (void *)&unit_next->unit_should_running_time, // 区域总运行时间
                                                    M_TYPE_Int, (void *)&temp_int_sy,                         // 区域剩余运行时间，区域总将要运行
                                                    M_TYPE_U32, (void *)&temp_u32_sy_sum_time,                // 计划剩余运行总时间
                                                    M_TYPE_U64, (void *)&in_obj->sio_instance_id,             // 计划结束时间
                                                    M_TYPE_Int, (void *)&stat,                                // 恢复状态
                                                    M_TYPE_Int, (void *)&trig_channel,                        // 传感器触发通道

                                                    M_TYPE_Str, (void *)temp_queue, // 队列

                                                    M_TYPE_Int, (void *)&temp_int_sy,              // 单元本次剩余运行/浸泡时间
                                                    M_TYPE_Int, (void *)&unit_next->unit_progress, // 进度
                                                    // M_TYPE_Int, (void *)&trig_type, // 新增 传感器类型
                                                    // M_TYPE_Int, (void *)&trig_sensor_type,
                                                    M_TYPE_NULL, NULL,
                                                    // M_TYPE_NULL, NULL,
                                                    in_time_s);
            stat = succ_r;
            hide_global_obj.hide_obj = NULL;
            hide_global_obj.obj_end_time = 0;
        }
        else
        {
            DEBUG_TEST( DB_I,"Should not be restored, direct clean up  ");
            m_callable_sensor_staging_clean(M_CONST_NONE_NUM, None);
        }
    }
    return stat;

    // @许凯君今天把白色继电器用许总的别墅拆下的pro设备验证了下，基础结论是用同批次pro设备开关白色继电器 只有许总拆的设备会异响，其他正常 差别比较明显
}
/**
 * @brief 清除传感器信息暂存
 *
 * @param in_time_s 依据
 * @return stat_m
 */
stat_m m_callable_sensor_staging_clean(uint64_t schid, uint64_t in_time_s)
{
    // if(hide_global_obj.hide_obj != NULL)
    // if (schid == M_CONST_NONE_NUM && in_time_s == 4999999999)
    // {
    //     DEBUG_TEST( DB_I,"a > b+15 %d. NULL?: %d  id: %d", in_time_s > hide_global_obj.obj_end_time + 15, hide_global_obj.hide_obj != NULL, schid == M_CONST_NONE_NUM);
    //     // hide_global_obj.hide_obj->instance_running_root_unit != NULL &&
    // }
    if ((in_time_s == None || in_time_s > hide_global_obj.obj_end_time + 15) &&
        hide_global_obj.hide_obj != NULL &&
        (schid == hide_global_obj.hide_obj->sio_running_id || M_CONST_NONE_NUM == schid))
    {
        DEBUG_TEST( DB_I,"Sensor Recover Time Out / Complete ,  Cleaning up !");
        // 释放 对象和资源
        if (hide_global_obj.hide_obj != NULL)
        {
            if (M_CONST_NONE_NUM == schid || schid == hide_global_obj.hide_obj->sio_running_id)
                m_static_sensor_flush(hide_global_obj.hide_obj);
            if (hide_global_obj.hide_obj->instance_running_root_unit != NULL)
            {
                m_static_instance_runnning_queue_clear(hide_global_obj.hide_obj->instance_running_root_unit);
                M_FREE(hide_global_obj.hide_obj);
            }
            hide_global_obj.hide_obj = NULL;
        }
    }
    return succ_r;
}
/**
 * @brief 某些计划可能需要被刷新继续的开始时间
 *
 * @param in_obj
 * @return stat_m
 */
stat_m m_static_sensor_flush(instance_obj *in_obj)
{
    stat_m stat = succ_r;
    instance_doc_t *in_wtd = NULL;

    if (in_obj != NULL)
    {
        stat = m_static_instance_list_find_by_id(in_obj->sio_running_id, &in_wtd);
        if (stat == succ_r && in_wtd != NULL && in_obj->sio_run_type == M_INSTANCE_RUN_TYPE_SCHEDULE)
        {
            in_wtd->exfun.fn_water_skip = 0;
            // in_wtd->is_genger_info = false;
            stat = m_static_tools_instance_obj_refresh_start_time(in_wtd, NULL, m_callable_timer_manage_get_device_time());
            m_static_time_link_notece_node_enable(in_wtd->sch_info.cu_id);
            stat = m_static_time_link_notece_node_change_obj(in_wtd->sch_info.cu_id, in_wtd->WSTime, M_BASE_STATUS_IDLE, in_wtd, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
            stat = m_static_time_link_notece_node_change_obj(in_wtd->sch_info.cu_id, in_wtd->WSTime, M_BASE_STATUS_IDLE, in_wtd, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
            in_wtd->exfun.fn_water_skip = M_BASE_STATUS_IDLE;
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                            M_TYPE_U32, (void *)(&in_wtd->exfun.fn_water_skip),
                                            M_TYPE_U64, (void *)(&in_wtd->sch_info.cu_id),
                                            M_TYPE_U64, (void *)(&in_wtd->WSTime),
                                            M_TYPE_NULL, NULL,
                                            m_callable_timer_manage_get_device_time(), false);
            in_wtd->exfun.fn_water_skip = 0;
            // m_static_tools_instance_obj_refresh_start_time(in_wtd, m_callable_timer_manage_get_device_time());
        }
    }
    return stat;
}