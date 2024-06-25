

#include "instance_recover.h"
#include "../manage/instance_manage.h"
#include "../manage/instance_list.h"
#include "../tools/instance_algorithm.h"
#include "../../../middle/tool/data_parse/data_parse.h"
#include "../../../middle/event_distribution/time_link.h"
#include "../ibuild/instance_build.h"

char res_temp_sch[512] = {0};
instance_doc_t inst_resp_obj = {0};

stat_m m_callable_instance_manage_recover_queue_append(instance_doc_t *in_wtd, uint64_t in_time_s);

#define DEVICE_OLD_AAA "OA"
/** 如果已经成新设备了  ， 那么就是 False */
bool old_device_to_new = false;
/**
 * @brief 计划恢复
 *
 * @param src_str
 * @param en_dis_able
 * @param in_time_s
 */
void m_static_instance_manage_instance_recover(char *src_str, bool en_dis_able, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    instance_doc_t *wobj = NULL;
    bool enable = true;
    // 恢复的第一部应该是初始化
    if (strlen(src_str) < 30)
        return ;
    // 解析
    strcpy(res_temp_sch, src_str);
    memset(&inst_resp_obj, 0, sizeof(inst_resp_obj));
    /** 没有， 老设备*/
    if (strstr(res_temp_sch, ",") == NULL)
    {
        DEBUG_TEST(DB_W, "old_device_to_new ... ");
        old_device_to_new = true;
    }
    else
        old_device_to_new = false;

    if (!old_device_to_new)
        stat = m_static_tools_instance_obj_explain(res_temp_sch, &inst_resp_obj, &enable, in_time_s);
    else
        stat = m_static_tools_instance_obj_explain_A0012(res_temp_sch, &inst_resp_obj, &enable, in_time_s);
    enable = en_dis_able;

    m_callable_schedule_stroage_read_by_param(inst_resp_obj.sch_info.cu_id, &inst_resp_obj.sch_info.cu_src_execute_time_str, 5);
    m_callable_schedule_stroage_read_by_param(inst_resp_obj.sch_info.cu_id, &inst_resp_obj.sch_info.cu_zone, 6);

    // 参数校对 - 如果都正常 那就加入列表或者下一个参数继续  如果返回成功表示计划内容是有效的
    stat = m_static_instance_manage_explain_instance_obj(&inst_resp_obj, in_time_s);

    if (stat == succ_r)
    {
        // 接入队列
        wobj = m_static_instance_list_add(&inst_resp_obj);

        // 先判断是不是 恢复的计划
        // if (mode == M_INSTANCE_ADD_MODE_ONLINE)
        {
            // m_callable_exce_schedule_stroage_en_dis_able(wobj->sch_info.cu_id, wobj->sch_info.cu_enable);
            wobj->sch_info.cu_enable = enable;
            // m_callable_schedule_stroage_write_string(wobj->sch_info.cu_id, temp_sch, wobj->sch_info.cu_enable);
        }
        stat = m_callable_schedule_skip_init(wobj->sch_info.cu_id);

        // 刷新下开始时间
        // 存一下 temp_sch
        stat = m_static_tools_instance_obj_refresh_start_time(wobj, &wobj->BufTime, in_time_s);
        // stat = m_static_tools_instance_obj_refresh_start_time(wobj, in_time_s);
        // DEBUG_TEST( DB_I, -- - wobj->BufTime %lld", wobj->BufTime);

        if (wobj->sch_info.cu_enable)
        {
            m_static_time_link_notece_node_add(wobj->sch_info.cu_id, wobj->WSTime, M_BASE_STATUS_IDLE, (void *)wobj);
            wobj->exfun.fn_water_skip = M_BASE_STATUS_IDLE;
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                            M_TYPE_U32, (void *)(&wobj->exfun.fn_water_skip),
                                            M_TYPE_U64, (void *)(&wobj->sch_info.cu_id),
                                            M_TYPE_U64, (void *)(&wobj->WSTime),
                                            M_TYPE_NULL, NULL,
                                            in_time_s, false);
            wobj->exfun.fn_water_skip = 0;
        }
        else
        {
            m_static_time_link_notece_node_add(wobj->sch_info.cu_id, wobj->WSTime, M_BASE_STATUS_IDLE, (void *)wobj);
            m_static_time_link_notece_node_disable(wobj->sch_info.cu_id);
        }
        if (wobj->sch_info.cu_enable && stat == need_r)
        {
            m_callable_instance_manage_recover_queue_append(wobj, in_time_s);
        }

        // 计划启用和不启用 都会刷新最新的启动时间

        // 在启用和不启用的区别应该就是一个bool参数和正确时间节点    缺一不可   两个参数都有效才是一个可以运行的计划
        /** Next : Determine the scheduled start time */

        // stat = m_recover_determine_scheduled_start_time(ws_item, in_time_s);
        /** Next : schedule recover */
        /** 所有需要恢复的计划都将放在这个队列里面 （这个后期我想改成挂起队列 挺好的哈哈哈）*/
        // m_callable_recover_queue_add_one(&mreRoot, ws_item );
        // m_static_time_link_notece_node_add(wobj->sch_info.cu_id ,  wobj->WSTime , wobj);
        // m_operation_normal_recover_schedule_time_and_status(ws_item , _time.hhmmss);

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
        printf("000000\n");
    // return stat;
}

bool resp_rtc_ads = false;
/** 已经恢复 步伐 28 */
bool resp_ads = false;
/**
 * @brief RTC 模式不请求 30
 *
 * @param in_flag  设置或者获取的 FLAG
 * @return true
 * @return false
 */
bool m_static_recover_instance_rtc_mode_flag_set_get(int in_flag)
{
    bool retu_bool = false;
    if (in_flag == None)
    {
        /** 如果是获取 那么当前的 flag 是什么状态就是什么状态 */
        retu_bool = resp_rtc_ads;
        resp_rtc_ads = false;
    }
    else
        resp_rtc_ads = in_flag;

    return retu_bool;
}

instance_doc_t *iwtd = NULL;
/** 已经恢复了，其他计划不重新发送 */
bool recover_ed = true;
/** 是否被传感器跳过*/
bool is_sensor_skip = false;
enum seneor_chann sen_chan;
enum skip_manage_type skip_type;
enum seneor_type sen_type;
stat_m m_callable_instance_manage_recover_queue_append(instance_doc_t *in_wtd, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    char sKeyss[30] = {0};
    int weather = 0;
    int weather_code = 0;

    sprintf(sKeyss, "%s%lld", "Ss", in_wtd->sch_info.cu_id);
    if (m_callable_drive_flash_read(M_TYPE_U64, sKeyss, (void *)&in_wtd->deadlineofsch) == succ_r)
    {
        if (in_wtd->deadlineofsch < in_time_s)
        {
            stat = succ_r;
        }
        else
            iwtd = NULL,
            is_sensor_skip = false;
    }
    else
        stat = succ_r;

    if (!recover_ed)
        stat = fail_r;

    if (stat == succ_r)
    {
        stat = fail_r;
        m_callable_weather_manage_get_is_effective(in_wtd->sch_info.cu_id, &weather);
        if (weather <= 0)
        {
            // stat = m_callable_skip_manage_query_is_effective(in_wtd->sch_info.cu_id, (enum skip_manage_type *)&weather, in_time_s);
            stat = m_callable_skip_manage_query_is_effective(in_wtd->sch_info.cu_id, &skip_type, &sen_chan, &sen_type, &weather_code, in_time_s);

            if (stat == succ_r || skip_type == M_SKIP_MANAGE_TYPE_SENSOR_SKIP)
            {
                iwtd = in_wtd;
                if (skip_type == M_SKIP_MANAGE_TYPE_SENSOR_SKIP)
                    is_sensor_skip = true;
                stat = succ_r;
            }
            else
            {
                // m_static_instance_running_skip_notify_event_handle(in_wtd, NULL,
                //                                                    0, 0, skip_type,
                //                                                    sen_chan, sen_type, weather_code, in_time_s);
            }
        }
    }
    if (stat == succ_r)
    {
        uint64_t itime = iwtd->WSTime + iwtd->exfun.fn_max_coust_time;
        iwtd->inst_end_buf_time = itime;
        iwtd->is_genger_info = M_INSTANCE_IS_GENER_INFO_WAIT;
        recover_ed = false;
        /** 如果是RTC恢复 那么  不发 30 直接恢复 */
        if (m_static_recover_instance_rtc_mode_flag_set_get(None))
        {
            m_callable_instance_manage_recover_queue_get_and_exec(iwtd->sch_info.cu_id, in_time_s);
        }
        else
            m_callable_local_resp_to_remote(M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME, M_TYPE_U64, &iwtd->sch_info.cu_id,
                                            M_TYPE_U64, &itime, M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
    }
    else
    {
        // m_callable_local_resp_to_remote(M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME,
        //                                 M_TYPE_NULL, NULL,
        //                                 M_TYPE_NULL, NULL,
        //                                 M_TYPE_NULL, NULL,
        //                                 M_TYPE_NULL, NULL, in_time_s, true);
    }
    DEBUG_TEST(DB_W, "deadlineofsch %lld", in_wtd->deadlineofsch);
    return stat;
}
/**
 * @brief 获取待恢复计划 并且执行
 *
 * @param in_time_s
 * @return stat_m
 */
stat_m m_callable_instance_manage_recover_queue_get_and_exec(uint64_t in_id, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    instance_obj *in_obj = NULL;
    enum base_state next_ste = M_BASE_STATUS_IDLE;
    enum base_state pre_ste = M_BASE_STATUS_RUNNING;
    /** （单元）剩余运行时间 */
    int temp_int_sy = 0;
    /** 已经运行时间和 */
    uint64_t shia_sum = 0;
    /** 剩余运行总时间 */
    uint64_t sy_sum = 0;
    uint64_t schedule_start_time = 0;

    int skip_type_aaa = 0;
    int weather_code = 0;
    DEBUG_TEST(DB_W, "m_callable_instance_manage_recover_queue_get_and_exec .. ");
    if ((stat = m_callable_sensor_current_is_effective(&sen_chan, &sen_type, in_time_s)) == fail_r)
        is_sensor_skip = true;
    if (iwtd != NULL) // 需要恢复
    {
        if (in_id != iwtd->sch_info.cu_id)
        {
            iwtd = NULL;
            is_sensor_skip = false;
            return fail_r;
        }
        // 当前运行队列
        struct instance_running_unit *unit_next = NULL;
        // 电流
        uint32_t temp_int = 0;
        // 今天零点
        uint64_t this_day_zero_time_timep = m_callable_timer_manage_get_device_zero_time();
        // 准备运行队列队列
        char temp_queue[512] = {0};
        // 填充
        in_obj = M_MALLOC(sizeof(instance_obj));
        memset(in_obj, 0, sizeof(instance_obj));
        // 时间推理
        DEBUG_TEST(DB_W, "Start time for Restoring Calculations BUf Time: %lld  WS %lld - %lld", iwtd->BufTime, iwtd->WSTime, this_day_zero_time_timep);
        if (iwtd->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR)
        // iwtd->WSTime = m_recover_pre_time;
        {
            iwtd->WSTime %= NUM_OF_DAY_SEC_SUM;
            iwtd->WSTime = (iwtd->WSTime + NUM_OF_DAY_SEC_SUM) - iwtd->sch_info.cu_interval_hour_or_day * 3600;
        }
        if (iwtd->sch_info.frequency_type == SCHEDULE_RUN_RHYTHM_NEW_LAWN)
            iwtd->WSTime = iwtd->BufTime;

        /** 你问当时下面为什么加上零点时间戳   因为这里取余了 */
        iwtd->WSTime %= NUM_OF_DAY_SEC_SUM;

        DEBUG_TEST(DB_W, "Start time for Restoring Calculations WS %lld - %lld", iwtd->WSTime, this_day_zero_time_timep);
        // 记录开始时间
        schedule_start_time = iwtd->WSTime + this_day_zero_time_timep;
        DEBUG_TEST(DB_I, "Start time %lld =   WS %lld + %lld", schedule_start_time, iwtd->WSTime, this_day_zero_time_timep);
        // 查看/获取  30 反馈的天气跳过信息
        m_callable_weather_manage_get_is_effective(iwtd->sch_info.cu_id, &weather_code);
        DEBUG_TEST(DB_I, "-->  Weather Code : %d", weather_code)

        in_obj->sio_instance_id = iwtd->inst_end_buf_time;
        shia_sum = iwtd->WSTime;
        // 实例生成 可能不需要执行而是跳过
        stat = m_callable_instance_manage_instance_gener(iwtd, in_obj, NULL, &weather_code, in_time_s);
        iwtd->WSTime = shia_sum;
        shia_sum = 0;
        if (stat == succ_r)
        {
            iwtd->is_genger_info = M_INSTANCE_IS_GENER_INFO_INIT;
            in_obj->is_genger_info = M_INSTANCE_IS_GENER_INFO_DONG;
            // stat = m_callable_sensor_current_is_effective(NULL, NULL, in_time_s);
            // printf("ws %lld" ,  iwtd->WSTime);
            // 找到执行单元
            unit_next = in_obj->instance_running_root_unit->head_running_unit;
            while (unit_next != NULL)
            {
                iwtd->WSTime += unit_next->unit_this_time_will_running_time;
                iwtd->WSTime %= NUM_OF_DAY_SEC_SUM;
                // DEBUG_TEST(DB_W,"in_time_s %lld    iwtd->WSTime %lld", in_time_s % 3600, iwtd->WSTime);
                // DEBUG_TEST(DB_W,"in_time_s %lld  ", shia_sum);

                if (in_time_s % NUM_OF_DAY_SEC_SUM < iwtd->WSTime)
                {
                    temp_int_sy = iwtd->WSTime - (in_time_s % NUM_OF_DAY_SEC_SUM);
                    // printf("ws %lld" ,  iwtd->WSTime);
                    shia_sum += (unit_next->unit_should_running_time - temp_int_sy);

                    // DEBUG_TEST(DB_W," WS: %lld , in: %lld  temp_int_sy : %d   =    shia_sum: %lld  ", iwtd->WSTime, (in_time_s % NUM_OF_DAY_SEC_SUM), temp_int_sy, shia_sum);
                    // DEBUG_TEST( DB_I,%d   %d", unit_next->unit_should_running_time, temp_int_sy);

                    if (unit_next->unit_running_type == M_BASE_STATUS_SACK)
                        stat = should_sack_r;
                    else if (unit_next->unit_running_type == M_BASE_STATUS_RUNNING)
                    {
                        stat = should_running_r;
                    }
                    break;
                }
                else
                    pre_ste = unit_next->unit_running_type;
                // if (unit_next->unit_running_type == M_BASE_STATUS_RUNNING)
                shia_sum += unit_next->unit_this_time_will_running_time;
                unit_next = unit_next->next_running_unit;
            }
        }
        else
        {
            // skip_type_aaa = M_SKIP_MANAGE_TYPE_SATURATION_SKIP;
        }

        if (stat == should_sack_r || stat == should_running_r)
        {
            in_obj->WSTime = iwtd->WSTime + this_day_zero_time_timep;
            // DEBUG_TEST( DB_I,Start time %lld =   WS %lld + %lld",schedule_start_time, iwtd->WSTime, this_day_zero_time_timep);
            DEBUG_TEST(DB_W, "WST %lld   max coust :%lld   result %lld", in_obj->WSTime, iwtd->exfun.fn_max_coust_time, in_obj->sio_instance_id);
            if (in_obj->sio_instance_id < 86400)
                in_obj->sio_instance_id = schedule_start_time + iwtd->exfun.fn_max_coust_time;
            DEBUG_TEST(DB_W, "WST %lld   max coust :%lld   result %lld", in_obj->WSTime, iwtd->exfun.fn_max_coust_time, in_obj->sio_instance_id);
        }

        if ((stat == should_sack_r || stat == should_running_r))
        {
            skip_type_aaa = M_SKIP_MANAGE_TYPE_SENSOR_SKIP;
            if (!is_sensor_skip)
            {
                if (stat == should_sack_r || stat == should_running_r)
                {
                    { //  取消其他运行
                        int device_state = 0;
                        // 在这里应之一切运行 包含快速运行和手动运行
                        m_callable_device_proper_status_get(&device_state);
                        if (device_state != M_DEVICE_GLOBAL_STATUS_IDLE)
                        {
                            DEBUG_TEST(DB_W, "Schedule to Start , Stop other runs ...");
                            if (device_state == M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING)
                                m_callable_instance_running_forced_stop(M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, true, in_time_s);
                            if (device_state == M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING)
                                m_callable_drive_button_direct_stop();
                        }
                    }
                    schedule_start_time = 0;
                    if (unit_next->next_running_unit != NULL)
                        next_ste = unit_next->next_running_unit->unit_running_type;
                    else
                        // 可能只有一个序列,快结束了
                        next_ste = M_BASE_STATUS_STOP;

                    in_obj->instance_running_root_unit->current_running_unit = NULL;
                    in_obj->instance_running_root_unit->current_running_unit = unit_next;
                    in_obj->instance_running_root_unit->current_running_unit = unit_next;

                    // m_static_inst_fsm_pre_state_change(pre_ste);
                    m_static_instance_manage_set_current_instance(in_obj);
                    m_callable_device_proper_status_update_to_schedule_running();
                    int rep_sack_run = unit_next->unit_running_channel;
                    if (stat == should_running_r)
                    {
                        m_callable_solenoid_manage_open(unit_next->unit_running_channel);
                        in_obj->sio_status = M_BASE_STATUS_RUNNING;
                        pre_ste = M_BASE_STATUS_RUNNING;
                    }
                    else
                    {
                        in_obj->sio_status = M_BASE_STATUS_SACK;
                        m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, rep_sack_run);
                        pre_ste = M_BASE_STATUS_SACK;
                        rep_sack_run = 254;
                    }
                    m_static_time_link_notece_node_change_obj(iwtd->sch_info.cu_id, this_day_zero_time_timep + iwtd->WSTime, pre_ste, in_obj, M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL);
                    m_static_time_link_notece_node_change_obj(iwtd->sch_info.cu_id, this_day_zero_time_timep + iwtd->WSTime, next_ste, in_obj, M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL);
                    m_static_instance_runnning_queue_str_format_get(in_obj, temp_queue);
                    /** 电流改成单元剩余运行时间 */
                    // temp_int = m_callable_current_check_get_final_value();
                    DEBUG_TEST(DB_I, "- - -sio_max_should_runtime : %lld  shia_sum: %lld", in_obj->sio_max_should_runtime, shia_sum);
                    sy_sum = in_obj->sio_max_coust_time - shia_sum;
                    m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME,
                                                            M_TYPE_U64, (void *)&in_obj->sio_running_id,              // 计划 ID
                                                            M_TYPE_Int, (void *)&rep_sack_run,                        // 开始运行区域
                                                            M_TYPE_U32, (void *)&unit_next->unit_should_running_time, // 区域应该运行时间
                                                            M_TYPE_U32, (void *)&temp_int_sy,                         // 单元剩余运行时间
                                                            M_TYPE_U64, (void *)&sy_sum,                              // 剩余总运行时间

                                                            M_TYPE_U64, (void *)&in_obj->sio_instance_id, // 结束时间
                                                            // 队列
                                                            M_TYPE_Str, (void *)temp_queue,
                                                            // 单元剩余运行时间
                                                            M_TYPE_Int, (void *)&temp_int_sy,
                                                            // 进度
                                                            M_TYPE_Int, (void *)&unit_next->unit_progress,
                                                            M_TYPE_NULL, NULL,
                                                            M_TYPE_NULL, NULL,
                                                            M_TYPE_NULL, NULL,
                                                            in_time_s);
                    stat = succ_r;
                    resp_ads = true;
                }
                else // 释放
                {
                    if (in_obj != NULL)
                    {
                        if (in_obj->instance_running_root_unit != NULL)
                        {
                            m_static_instance_runnning_queue_clear(in_obj->instance_running_root_unit);
                            in_obj->instance_running_root_unit = NULL;
                        }
                        M_FREE(in_obj);
                        in_obj = NULL;
                    }
                }
            }
            else
            {
                uint16_t None_254 = 254;
                m_static_instance_running_skip_notify_event_handle(NULL, in_obj,
                                                                   in_obj->sio_instance_id,
                                                                   None_254,
                                                                   skip_type_aaa,
                                                                   sen_chan, sen_type, None, in_time_s);
                schedule_start_time = 0;
            }
        }
        else
        {
            if (in_obj != NULL)
            {
                // 如果是需要跳过
                if (weather_code > 0)
                {
                    if (skip_type_aaa == 10)
                        skip_type_aaa = M_SKIP_MANAGE_TYPE_SATURATION_SKIP;
                    else
                        skip_type_aaa = M_SKIP_MANAGE_TYPE_WEATHER_SKIP;

                    in_obj->sio_instance_id = schedule_start_time + iwtd->exfun.fn_max_coust_time;

                    m_static_instance_running_skip_notify_event_handle(iwtd, in_obj, in_obj->sio_instance_id,
                                                                       0, skip_type_aaa,
                                                                       sen_chan, sen_type, weather_code, in_time_s);
                    m_static_instance_enreg_schedule_end_time(iwtd->sch_info.cu_id, in_obj->sio_instance_id);
                    m_callable_weather_manage_clean(iwtd->sch_info.cu_id);

                    iwtd->is_genger_info = M_INSTANCE_IS_GENER_INFO_INIT;
                    m_static_time_link_notece_node_change_obj(iwtd->sch_info.cu_id, None, None, iwtd, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
                }
                // if (in_obj->instance_running_root_unit != NULL)
                // {
                //     m_static_instance_runnning_queue_clear(in_obj->instance_running_root_unit);
                // }
                // if (in_obj != NULL)
                //     M_FREE(in_obj);
                // in_obj = NULL;
                m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME,
                                                        M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                        M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                        M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                        M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        in_time_s);
            }
            if (iwtd != NULL)
                m_static_time_link_notece_node_change_obj(iwtd->sch_info.cu_id, None, None, iwtd, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE);
        }
        // m_callable_sensor_staging_add_trig(in_obj, in_time_s);

        iwtd = NULL;
        is_sensor_skip = false;
        stat = unknow_r;
    }
    else
    {
        stat = fail_r;
        iwtd = NULL;
        is_sensor_skip = false;
        // m_callable_local_resp_to_remote(M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME,
        //                                 M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
        //                                 M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
        //                                 M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
        //                                 M_TYPE_NULL, NULL, in_time_s, true);
        // m_callable_local_resp_to_remote(M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME,
        //                                 M_TYPE_NULL, NULL,
        //                                 M_TYPE_NULL, NULL,
        //                                 M_TYPE_NULL, NULL,
        //                                 M_TYPE_NULL, NULL, in_time_s);
    }
    return stat;
}
/**
 * @brief 计划恢复 全部完成
 *
 * @param in_time_s 时间
 * @return stat_m
 */
stat_m m_callable_instance_manage_recover_done(uint64_t in_time_s)
{
    DEBUG_TEST(DB_W, "m_callable_instance_manage_recover_done  %d", old_device_to_new);
    // [20,5496,2,null,1704326400,4070995199,RtSUT5TeUFUo,1#540;2#540;3#540,120,null,1]
    if (old_device_to_new)
    {
        if (m_callable_flash_rescue_message_flag(DEVICE_OLD_AAA, M_GLO_STROAGE_RESCUE_MESSAGE_GET) != succ_r) // 有
        {
            m_callable_flash_rescue_message_flag(DEVICE_OLD_AAA, M_GLO_STROAGE_RESCUE_MESSAGE_SET);
            mReboot(M_RESTART_CAUSE_PREMEDITATED);
        }
    }
    if (iwtd == NULL && !resp_ads)
    {
        m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME,
                                                M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_SACK_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_Int, (void *)&M_CONST_NONE_NUM,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                in_time_s);
    }
    return succ_r;
}
/**

         else if (mode == M_INSTANCE_ADD_MODE_RECOVER_ENABLE)
        {
            wobj->sch_info.cu_enable = true;
            // 找一下 是不是需要恢复 或者是被断电中断的，如果是的话   那就加入恢复队列
            // 不然的话，不需要
        }
        else if (mode == M_INSTANCE_ADD_MODE_RECOVER_DISABLE)
        {
            wobj->sch_info.cu_enable = false;
            m_callable_exce_schedule_stroage_en_dis_able(wobj->sch_info.cu_id);
        }
 *
 */