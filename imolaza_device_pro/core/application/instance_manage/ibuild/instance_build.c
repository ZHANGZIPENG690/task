
// #include "hot_so_fast.h"
#include "instance_build.h"
#include "../../../middle/schedule/smart_schedule/smart_schedule.h"

const int M_DONE_NUM = 2;
const int M_NOT_DONE_NUM = 1;

// extern plan_manager _plan;

/* 区域被胡烈 */
static int instance_zone_ignore_num = 0;
/* 区域被禁用 */
static int instance_zone_disable_num = 0;

/**
 * @brief 根据 (instance_doc_t *in_doc) 生成   (instance_obj *out_inst_obj ) 实例
 *
 * @param in_doc 对象描述
 * @param out_inst_obj 实例
 * @param dis_chan 关闭的区域列表
 * @return stat_m
 */
stat_m m_instance_schedule_gener_doc_to_obj(instance_doc_t *in_doc, instance_obj *out_inst_obj, uint8_t *dis_chan, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    /** 实际运行区域计数 最大 与 sio_max_channel 相同*/
    int real_run_channel_max_count = 0;
    /** boo表示区域是否需要使用 ，is_cul_max 表示区域是否需要节水计算  */
    bool boo = true, is_cul_max = false;
    uint32_t out_time = 0;
    out_inst_obj->sio_running_id = in_doc->sch_info.cu_id;
    out_inst_obj->sio_run_type = in_doc->exfun.fn_run_type;
    int count = 100;
    // 获取当前的设备状态
    // printf(" 0  0 0 0 0 0 0 0 %d\n", count);
    // out_inst_obj->sio_channel = in_doc->cmg_channel;

    for (int i = 0; i < in_doc->exfun.fn_max_channel; i++)
    {
        boo = true;
        if (in_doc->cmg_channel[i].cd_cahnnel_id <= 0 || in_doc->cmg_channel[i].cd_cahnnel_id >= 33)
        {
            DEBUG_TEST(DB_W, "zone inva : %hhd", in_doc->cmg_channel[i].cd_cahnnel_id);
            continue;
        }

        // 离线 调整
        if (m_callable_network_manage_get_server_login_status() != succ_r)
        {
            m_callable_channel_manage_get_offline_running_percent(&count);
            if (count <= 0)
            {
                DEBUG_TEST(DB_W, "offline count : %d", count);
                boo = false;
            }
        }
        else
            count = 100;

        /** 智能计划忽略 区域禁用 加载*/
        if (m_callable_smart_running_storage_is_ignore(in_doc->sch_info.cu_id, in_doc->cmg_channel[i].cd_cahnnel_id, in_time_s) == fail_r)
        {
            DEBUG_TEST(DB_W, "zone ignore : %hhd", in_doc->cmg_channel[i].cd_cahnnel_id);
            instance_zone_ignore_num++;
            boo = false;
        }
        if (m_callable_channel_manage_current_is_effective(in_doc->cmg_channel[i].cd_cahnnel_id, in_time_s) == succ_r)
        {
            DEBUG_TEST(DB_W, "zone En/Dis : %hhd", in_doc->cmg_channel[i].cd_cahnnel_id);
            stat = all_channel_close_r;
            instance_zone_disable_num++;
            boo = false;
        }

        if (boo)
        {
            out_inst_obj->sio_channel[real_run_channel_max_count].cd_channel_type = in_doc->cmg_channel[i].cd_channel_type;
            /** 泵模式 时间加载 */
            if (out_inst_obj->sio_channel[real_run_channel_max_count].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP)
            {
                if (m_callable_device_attribute_get_soledold(NULL, NULL) != succ_r)
                {
                    out_inst_obj->sio_channel[real_run_channel_max_count].cd_channel_type = M_STATIC_CHANNEL_TYPE_NORMAL;
                    DEBUG_TEST(DB_I, "pump mode is close ");
                }
            }
            out_inst_obj->sio_channel[real_run_channel_max_count].cd_cahnnel_id = in_doc->cmg_channel[i].cd_cahnnel_id;
            /** 加载动态修改的时间 */
            if (m_callable_smart_running_storage_is_change(in_doc->sch_info.cu_id,
                                                           in_doc->cmg_channel[i].cd_cahnnel_id,
                                                           &out_time, in_time_s) == succ_r)
            {
                out_inst_obj->sio_channel[real_run_channel_max_count].cd_should_run_time = out_time;
            }
            else
                out_inst_obj->sio_channel[real_run_channel_max_count].cd_should_run_time = ((uint32_t)((in_doc->cmg_channel[i].cd_should_run_time * count) / 100));

            if (in_doc->exfun.fn_sack_type == M_INSTANCE_SACK_TYPE_FOR_CONFIG_A || in_doc->exfun.fn_sack_type == M_INSTANCE_SACK_TYPE_FOR_CONFIG_B)
            {
                m_callable_channel_manage_get_channel_param(out_inst_obj->sio_channel[i].cd_cahnnel_id,
                                                            in_doc->exfun.fn_run_type,
                                                            &out_inst_obj->sio_channel[i].cd_channel_type,
                                                            &out_inst_obj->sio_channel[i].cd_sack_should_water_time,
                                                            &out_inst_obj->sio_channel[i].cd_sack_should_sack_time,
                                                            &out_inst_obj->sio_channel[i].cd_ar,
                                                            &out_inst_obj->sio_channel[i].cd_ir,
                                                            &out_inst_obj->sio_channel[i].cd_asa);
                if (out_inst_obj->sio_channel[i].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
                {
                    is_cul_max = true;
                }
            }

            //  (uint32_t)(in_doc->cmg_channel[i].cd_should_run_time * (count / 100));
            out_inst_obj->sio_channel[real_run_channel_max_count].cd_sack_should_water_time = in_doc->cmg_channel[i].cd_sack_should_water_time;
            out_inst_obj->sio_channel[real_run_channel_max_count].cd_sack_should_sack_time = in_doc->cmg_channel[i].cd_sack_should_sack_time;
            /** 智能浸泡参数 */
            out_inst_obj->sio_channel[real_run_channel_max_count].cd_ar = in_doc->cmg_channel[i].cd_ar;
            out_inst_obj->sio_channel[real_run_channel_max_count].cd_ir = in_doc->cmg_channel[i].cd_ir;
            out_inst_obj->sio_channel[real_run_channel_max_count].cd_asa = in_doc->cmg_channel[i].cd_asa;
            out_inst_obj->sio_max_channel++;
            real_run_channel_max_count++;
        }
    }
    /** 最大区域大于 0  */
    if (real_run_channel_max_count)
    {
        /** 有智能区域  */
        if (is_cul_max)
        {
            // m_static_instance_sack_algorithm_pro_max(in_doc->exfun.fn_max_channel, in_doc->exfun.fn_max_channel, in_time_s);
            m_static_instance_sack_algorithm_pro_max(in_doc->cmg_channel, in_doc->exfun.fn_max_channel, in_time_s);
        }

        for (int i = 0; i < real_run_channel_max_count; i++)
        {
            DEBUG_TEST(DB_I, "instance should rt: %d ->  ( %d - %d)",
                       out_inst_obj->sio_channel[i].cd_should_run_time,
                       out_inst_obj->sio_channel[i].cd_sack_should_water_time,
                       out_inst_obj->sio_channel[i].cd_sack_should_sack_time);
        }
        stat = succ_r;
    }
    else
    {
        stat = fail_r;
        /** 有区域饱和 并且全部不执行 */
        if (instance_zone_ignore_num > 0)
        {
            stat = all_channel_ignore_r;
        }
        else if (instance_zone_disable_num > 0)
            stat = all_channel_close_r;
    }
    return stat;
}
/** 总的运行时间 */
uint64_t final_time_sum = 0;
/**
 * @brief 根据已有的区域参数生成运行队列
 *
 * @param arr 区域信息
 * @param max_channel 最大运行区域
 * @param water_start_time 浇水开始时间
 * @param out_sequences 生成的队列
 * @param in_tim_s 当前时间
 * @return int
 */
int m_static_instance_sack_algorithm_queue_gener(struct channel_config *arr, int max_channel, int water_start_time,
                                                 struct instance_running_sequences *root_sequences,
                                                 uint64_t *final_max_time,
                                                 bool is_build,
                                                 uint64_t in_tim_s)
{
    struct instance_running_unit sign_running_unit;
    int result = 0, pc = 0;
    // 填充单元序列
    // 根据前面的市场推算出执行步骤 应该是没有问题的
    int loop_limit = 10000;
    // 泵模式用来运行的标记参数
    bool cd_pump_done = false;
    /**  当前区域 */
    int presentMaxChan = 0,
        /** 运行耗费总时间 */
        final_water_sum = 0, dist = in_tim_s - water_start_time;
    // 计划开始的时间到推算的时间 （根据计划内容不断推进，直到当前时间，或者计划应该结束时间）
    long temp_present_time = in_tim_s;
    // 进度
    int uint_progress = 0;
    DEBUG_TEST(DB_W, "m_static_instance_sack_algorithm_queue_gener");
    while (presentMaxChan < max_channel)
    {
        // if (arr[presentMaxChan].cd_channel_type != M_STATIC_CHANNEL_TYPE_NORMAL)
        {
            loop_limit--;
            if (!loop_limit)
                break;
            for (int n = 0; n < max_channel;) //_device.hardware.type;)
            {

                if ((arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_NORMAL ||
                     arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP) &&
                    arr[n].cd_should_run_time > arr[n].cd_reality_run_time && !cd_pump_done)

                {
                    // DEBUG_TEST(DB_W,"o-o-o");
                    {
                        arr[n].cd_reality_run_time = arr[n].cd_should_run_time;
                        final_water_sum += arr[n].cd_should_run_time;
                        temp_present_time += arr[n].cd_should_run_time;
                        // 累积时间
                        arr[n].cd_running_end_time = temp_present_time;

                        if (is_build)
                        { // 加入、加入、加入
                            DEBUG_TEST(DB_I, "NOR---> Zone %hhd Water %d Already -%d    (%d , %d)   ", arr[n].cd_cahnnel_id, arr[n].cd_sack_should_water_time,
                                       arr[n].cd_should_run_time, arr[n].cd_reality_run_time, final_water_sum);
                            sign_running_unit.unit_running_channel = arr[n].cd_cahnnel_id;                  // 区域
                            sign_running_unit.unit_should_running_time = arr[n].cd_should_run_time;         // 应该运行时间
                            sign_running_unit.unit_this_time_running_end_time = arr[n].cd_running_end_time; // 结束时间
                            sign_running_unit.unit_already_running_time = arr[n].cd_reality_run_time;
                            sign_running_unit.unit_this_time_will_running_time = arr[n].cd_should_run_time;
                            sign_running_unit.unit_running_type = M_BASE_STATUS_RUNNING;
                            arr[n].cd_curr_will_run_time = 0;
                            sign_running_unit.unit_zone_real_total_time = &arr[n].cd_curr_will_run_time;

                            if (arr[n].cd_should_run_time <= arr[n].cd_reality_run_time)
                                sign_running_unit.unit_done = M_DONE_NUM;
                            else
                                sign_running_unit.unit_done = M_NOT_DONE_NUM;
                            sign_running_unit.unit_progress = uint_progress++;
                            // 加入队列
                            m_static_instance_runnning_queue_append(root_sequences, &sign_running_unit);
                        }
                        // 泵模式标记，下一次一定运行浸泡
                        if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP)
                            cd_pump_done = true;
                        presentMaxChan++;
                    }
                }
                else if ((arr[n].cd_should_run_time > 0 && arr[n].cd_should_run_time > arr[n].cd_reality_run_time) || (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && cd_pump_done))
                {
                    // 运行还是浸泡
                    // 计划开始的时间到推算的时间 - 上一次运行的时间标记  大于浸泡时间的花 那么应该运行     否则 应该浸泡
                    if (!cd_pump_done && (arr[n].cd_running_end_time == 0 || temp_present_time - arr[n].cd_running_end_time >= arr[n].cd_sack_should_sack_time))
                    {
                        // 剩下的应该运行的时间 > 浸泡应该浇水的时间     =>>  直接浇水  cd_sack_should_water_time
                        // 运行完整时长还是断续时长
                        // DEBUG_TEST( DB_I,%hhd" , arr[n].cd_sack_should_water_time);
                        // DEBUG_TEST( DB_I,%hhd" , arr[n].cd_sack_should_sack_time);
                        if (arr[n].cd_reality_run_time + arr[n].cd_sack_should_water_time <= arr[n].cd_should_run_time &&
                            arr[n].cd_reality_run_time + arr[n].cd_last_running_time < arr[n].cd_should_run_time)
                        {
                            arr[n].cd_reality_run_time += arr[n].cd_sack_should_water_time;
                            final_water_sum += arr[n].cd_sack_should_water_time;
                            temp_present_time += arr[n].cd_sack_should_water_time;
                            // 累积时间
                            arr[n].cd_running_end_time = temp_present_time;

                            if (is_build)
                            { // 加入、加入、加入
                                DEBUG_TEST(DB_I, "---> Zone %hhd This %d Total-%d    (Already %d , Already rt%d)   ",
                                           arr[n].cd_cahnnel_id,
                                           arr[n].cd_sack_should_water_time,
                                           arr[n].cd_should_run_time,
                                           arr[n].cd_reality_run_time,
                                           final_water_sum);
                                sign_running_unit.unit_running_channel = arr[n].cd_cahnnel_id;                  // 区域
                                sign_running_unit.unit_should_running_time = arr[n].cd_should_run_time;         // 应该运行时间
                                sign_running_unit.unit_this_time_running_end_time = arr[n].cd_running_end_time; // 结束时间
                                sign_running_unit.unit_already_running_time = arr[n].cd_reality_run_time;
                                sign_running_unit.unit_this_time_will_running_time = arr[n].cd_sack_should_water_time;
                                sign_running_unit.unit_running_type = M_BASE_STATUS_RUNNING;
                                arr[n].cd_curr_will_run_time = 0;
                                sign_running_unit.unit_zone_real_total_time = &arr[n].cd_curr_will_run_time;

                                if (arr[n].cd_should_run_time <= arr[n].cd_reality_run_time)
                                    sign_running_unit.unit_done = M_DONE_NUM;
                                else
                                    sign_running_unit.unit_done = M_NOT_DONE_NUM;
                                sign_running_unit.unit_progress = uint_progress++;
                                // 加入队列
                                m_static_instance_runnning_queue_append(root_sequences, &sign_running_unit);
                            }
                            // if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_WATERSAVING)
                            //     m_static_instance_algorithm_sack_time(arr[n].cd_cahnnel_id, arr[n].cd_ar, arr[n].cd_ir, arr[n].cd_sack_should_water_time, &arr[n].cd_sack_should_sack_time);
                            if (arr[n].cd_should_run_time <= arr[n].cd_reality_run_time)
                                presentMaxChan++;
                            // 运行一次计算下浸泡的时间
                        }
                        else
                        {
                            // 算出需要进步的，加上
                            final_water_sum += (arr[n].cd_should_run_time - arr[n].cd_reality_run_time);
                            temp_present_time += (arr[n].cd_should_run_time - arr[n].cd_reality_run_time);
                            if (is_build)
                            {
                                DEBUG_TEST(DB_I, "LAST---> Zone %hhd This %d Total-%d    (Already %d , Already rt%d)   ",
                                           // DEBUG_TEST( DB_I,LAST---> 区域 %hhd 本次浇水 %d 总-%d    (已经%d , 已经运算时间%d) ",
                                           arr[n].cd_cahnnel_id,
                                           arr[n].cd_should_run_time - arr[n].cd_reality_run_time,
                                           arr[n].cd_should_run_time,
                                           arr[n].cd_reality_run_time,
                                           final_water_sum);
                                // sign_running_unit.pre_running_unit =
                                sign_running_unit.unit_this_time_will_running_time = arr[n].cd_should_run_time - arr[n].cd_reality_run_time;
                                if (sign_running_unit.unit_this_time_will_running_time < FIX_TIME_MIN)
                                    sign_running_unit.unit_this_time_will_running_time = arr[n].cd_last_running_time;
                                // 更新
                            }
                            arr[n].cd_reality_run_time = arr[n].cd_should_run_time;
                            if (is_build)
                            {
                                sign_running_unit.unit_running_channel = arr[n].cd_cahnnel_id;
                                sign_running_unit.unit_should_running_time = arr[n].cd_should_run_time;
                                sign_running_unit.unit_this_time_running_end_time = arr[n].cd_running_end_time;
                                sign_running_unit.unit_already_running_time = arr[n].cd_reality_run_time;
                                sign_running_unit.unit_running_type = M_BASE_STATUS_RUNNING;
                                arr[n].cd_curr_will_run_time = 0;
                                sign_running_unit.unit_zone_real_total_time = &arr[n].cd_curr_will_run_time;
                                if (arr[n].cd_should_run_time <= arr[n].cd_reality_run_time)
                                    sign_running_unit.unit_done = M_DONE_NUM;
                                else
                                    sign_running_unit.unit_done = M_NOT_DONE_NUM;
                                sign_running_unit.unit_progress = uint_progress++;
                                m_static_instance_runnning_queue_append(root_sequences, &sign_running_unit);
                            }

                            arr[n].cd_running_end_time = temp_present_time;
                            // m_static_instance_algorithm_sack_time(arr[n].cd_cahnnel_id , arr[n].cd_ar , arr[n].cd_ir , arr[n].cd_sack_should_water_time , &arr[n].cd_sack_should_sack_time);
                            if (arr[n].cd_should_run_time <= arr[n].cd_reality_run_time)
                                presentMaxChan++;
                            // 恢复相关
                            if (result == 0 && dist > 0)
                            {
                                // waterObj->running->rb_WSTime_clone += (temp_chan_should_water * 60);
                                // DEBUG_TEST(DB_I,"WSTime_clone : %lld", waterObj->WSTime_clone);

                                if (dist < (arr[n].cd_should_run_time)) // 如果现在的结果还没出来，但是开始时间的差距小于当前应该浇水的时间和，那么取差值，计算到精确的时间
                                {
                                    pc = n;
                                    result = 1; // 应该运行
                                    arr[n].cd_reality_run_time += dist;
                                    // DEBUG_TEST(DB_I,"(%hhd) watering : %d", waterObj->chaninfo[n].chanID, waterObj->chaninfo[n].realityRunTime);
                                }
                                else if (dist >= (arr[n].cd_should_run_time)) // 之间的差值大于当前的 ， 直接加上应该浇水的时间和
                                {
                                    arr[n].cd_reality_run_time += (arr[n].cd_should_run_time);
                                    // DEBUG_TEST(DB_I,"(%hhd) watering : %d", waterObj->chaninfo[n].chanID, waterObj->chaninfo[n].realityRunTime);
                                    dist -= (arr[n].cd_should_run_time);
                                }
                            }
                        }
                        n++;
                        // temp_chan_should_water = arr[n].cd_should_run_time;
                    }
                    else
                    {
                        // 要浸泡
                        if (arr[n].cd_should_run_time - arr[n].cd_reality_run_time > 0 || (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && cd_pump_done))
                        {
                            // int sct = (arr[n].cd_sack_should_sack_time - (temp_present_time - arr[n].cd_running_end_time));
                            int sct = 0;
                            { // 找一找 浸泡时间
                                if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP && arr[n].cd_sack_should_sack_time > 0)
                                {
                                    sct = arr[n].cd_sack_should_sack_time;
                                    // arr[n].cd_sack_should_sack_time = 0;
                                    if (arr[n + 1].cd_cahnnel_id <= 0)
                                    {
                                        break;
                                    }
                                    cd_pump_done = false;
                                }
                                else
                                {
                                    sct = (arr[n].cd_sack_should_sack_time - (temp_present_time - arr[n].cd_running_end_time));
                                }
                            }
                            if (is_build)
                            {
                                DEBUG_TEST(DB_I, "SACK---> Zone %hhd Sack   %d   (%d,%lld) ",
                                           arr[n].cd_cahnnel_id,
                                           sct,
                                           final_water_sum,
                                           arr[n].cd_running_end_time /** arr[n].cd_sack_should_sack_time - (final_water_sum - arr[n].cd_running_end_time) **/);
                                sign_running_unit.unit_this_time_will_running_time = sct;
                                sign_running_unit.unit_running_channel = arr[n].cd_cahnnel_id;
                                sign_running_unit.unit_should_running_time = arr[n].cd_should_run_time;
                                sign_running_unit.unit_this_time_running_end_time = arr[n].cd_running_end_time;
                                sign_running_unit.unit_already_running_time = arr[n].cd_reality_run_time;
                                sign_running_unit.unit_running_type = M_BASE_STATUS_SACK;
                                arr[n].cd_curr_will_run_time = 0;
                                sign_running_unit.unit_zone_real_total_time = &arr[n].cd_curr_will_run_time;
                                sign_running_unit.unit_done = M_NOT_DONE_NUM;
                                sign_running_unit.unit_progress = uint_progress++;
                                m_static_instance_runnning_queue_append(root_sequences, &sign_running_unit);
                            }
                            if (arr[n].cd_channel_type == M_STATIC_CHANNEL_TYPE_PUMP)
                                n++;
                            final_water_sum += sct;
                            // final_water_sum += sct;
                            // final_water_sum += (arr[n].cd_sack_should_sack_time - (final_water_sum - arr[n].cd_running_end_time));
                            temp_present_time += sct;
                        }
                    }
                }
                else
                    n++; //, temp_chan_should_water = arr[n].cd_should_run_time;
            }
        }
    }
    DEBUG_TEST(DB_I, "sum %d", final_water_sum);
    final_time_sum = final_water_sum;
    *final_max_time = final_water_sum;
    return result;
}

//
// 会根据传入的原始计划数据对象来生成运行实例   最高设定是在 10min 分钟或者 30s 的请求来到的时候
/**
 * @brief 根据十分钟参数来进行实例生成
 *
 * @param instance_id 计划id
 * @param dis_channel 需要关闭的区域
 * @param skip 是否跳过
 */
stat_m m_callable_instance_manage_instance_gener(instance_doc_t *in_doc, instance_obj *in_obj, uint8_t *dis_channel, int *skip, uint64_t in_time_s)
{
    DEBUG_TEST(DB_W, "m_callable_instance_manage_instance_gener");
    stat_m stat = fail_r;

    {
        // 找到计划之后  把计划的所有都转换成 运行单元 根据除去的区域和应该跳过
        // 表示不需要跳过计划,饱和跳过可能只是部分，所以需要进一步区分
        if (skip == NULL || (*skip <= 0 || *skip == 10))
        {
            // 先填充 in_obj  form  in_doc   剔除 dis_channel
            in_obj->instance_running_root_unit = M_MALLOC(sizeof(struct instance_running_sequences));
            memset(in_obj->instance_running_root_unit, 0, sizeof(struct instance_running_sequences));

            // 区域转换
            stat = m_instance_schedule_gener_doc_to_obj(in_doc, in_obj, dis_channel, in_time_s);
            if (stat == succ_r)
            {
                in_obj->instance_running_root_unit->head_running_unit = NULL;
                in_obj->sio_run_type = M_INSTANCE_RUN_TYPE_SCHEDULE;
                // 根据已有的区域参数生成运行队列 这个在提前十分中或者其他的时间开始计算 ,有两个参数因为不是恢复所以
                m_static_instance_sack_algorithm_queue_gener(in_obj->sio_channel, in_obj->sio_max_channel, 0, in_obj->instance_running_root_unit,
                                                             &final_time_sum,
                                                             true, in_time_s);

                /** 30 没有请求的时候 ID不存在 */
                if (in_obj->sio_instance_id < 86400)
                    in_obj->sio_instance_id = in_doc->WSTime + in_doc->exfun.fn_max_coust_time;
                in_obj->sio_clone_max_channel = in_obj->sio_max_channel;
                // Err Handle
                if (in_doc->WSTime <= 2000)
                {
                    m_static_time_link_notece_node_id_get_by_time(in_doc->sch_info.cu_id, &in_doc->WSTime);
                    in_obj->sio_instance_id = in_doc->WSTime + in_doc->exfun.fn_max_coust_time;
                }
                in_obj->sio_max_should_runtime = in_doc->exfun.fn_max_should_runtime;
                in_obj->sio_max_coust_time = in_doc->exfun.fn_max_coust_time;
                // DEBUG_TEST( DB_I,---------------");
                DEBUG_TEST(DB_I, "--> WS  %lld", in_doc->WSTime);
                // DEBUG_TEST( DB_I,--> Cou  %lld", in_doc->exfun.fn_max_coust_time);
                // DEBUG_TEST( DB_I,--> so-id %lld", in_obj->sio_instance_id);
                DEBUG_TEST(DB_I, "---------------");
                m_static_instance_runnning_queue_print(in_obj->instance_running_root_unit);
                if (skip != NULL)
                    *skip = 0;
                m_callable_weather_manage_clean(in_obj->sio_running_id);
            }
            else
            {
                M_FREE(in_obj->instance_running_root_unit);
                if (stat == all_channel_close_r)
                {
                    DEBUG_TEST(DB_I, "Should  (Chann All Dis)   Skip... %lld", in_doc->sch_info.cu_id);
                    /** 所有区域被关闭 */
                    if (skip != NULL)
                        *skip = M_SKIP_MANAGE_TYPE_ALL_CHANNEL_DISABLE;
                }
                else
                {
                    if (skip != NULL)
                        *skip = M_SKIP_MANAGE_TYPE_SATURATION_SKIP;
                    DEBUG_TEST(DB_I, "Should (Saturation)  / (Month Count : 0) Skip... %lld", in_doc->sch_info.cu_id);
                }
            }
        }
        else
        {
            // 如果没有的 不需要生成实例
            if (skip != NULL)
                DEBUG_TEST(DB_I, "Should Weather Skip... %d", *skip);
        }
        // m_static_instance_runnning_queue_print(temp_water_a_run->instance_running_root_unit);
    }
    return stat;
}

/* 快速构建 */
/**
 * @brief 快速搭建一个实例 运行
 *
 * @param in_src
 * @param in_time_s
 * @return stat_m
 */
stat_m m_callable_instance_manage_quick_gener(char *in_src, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    // 不会生成 实例  而是直接   添加一个在下一秒开始的通知
    int device_status = 0;
    m_callable_device_proper_status_get(&device_status);
    if (device_status == M_DEVICE_GLOBAL_STATUS_IDLE)
    {

        instance_obj *in_obj = M_MALLOC(sizeof(instance_obj));
        enum base_state next_ste;
        memset(in_obj, 0, sizeof(instance_obj));
        // instance_obj *in_obj
        m_static_tools_instance_obj_quick_explain(in_src, in_obj, in_time_s);

        in_obj->instance_running_root_unit = M_MALLOC(sizeof(struct instance_running_sequences));
        memset(in_obj->instance_running_root_unit, 0, sizeof(struct instance_running_sequences));

        in_obj->instance_running_root_unit->head_running_unit = NULL;
        in_obj->sio_run_type = M_INSTANCE_RUN_TYPE_FAST_RUN;

        stat = m_static_instance_sack_algorithm_queue_gener(in_obj->sio_channel, in_obj->sio_max_channel, 0,
                                                            in_obj->instance_running_root_unit,
                                                            &final_time_sum,
                                                            true,
                                                            in_time_s);

        m_static_instance_runnning_queue_print(in_obj->instance_running_root_unit);

        // 拿到对象
        struct instance_running_unit *wobj = in_obj->instance_running_root_unit->head_running_unit;
        // 我在运行
        m_static_instance_manage_set_current_instance(in_obj);
        // 时间调整下
        in_obj->sio_instance_id = in_time_s + final_time_sum;
        in_obj->sio_clone_max_channel = in_obj->sio_max_channel;

        // 时间调整下
        in_obj->WSTime += in_time_s;
        in_obj->WSTime += wobj->unit_this_time_will_running_time;
        in_obj->sio_status = M_BASE_STATUS_RUNNING;
        // 第一步应该是酸下这个状态应该保持的时间，定一个时间节点
        in_obj->instance_running_root_unit->current_running_unit = wobj;

        // /** 驱动端口 */
        if (wobj->next_running_unit != NULL)
            next_ste = wobj->next_running_unit->unit_running_type;
        else
            // 可能只有一个序列,快结束了
            next_ste = M_BASE_STATUS_STOP;
            
        if (m_callable_current_batch_area_power_calibration_flag_get() == 1)
        {
            in_obj->sio_running_id = 665;
        }
        else if (m_callable_current_batch_area_power_calibration_flag_get() == 2)
        {
            in_obj->sio_running_id = 664;
        }
        else
        {
            in_obj->sio_running_id = 666;
        }

        m_callable_device_proper_status_update_to_fast_running();
        // 消化状态 修改下一个节点的状态
        m_static_time_link_notece_node_add(in_obj->sio_running_id, in_obj->WSTime, M_BASE_STATUS_RUNNING, (void *)in_obj);

        m_static_time_link_notece_node_change_obj(in_obj->sio_running_id, None, next_ste, in_obj, M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL);
        // 事件触发
        m_static_instance_running_event_handle(M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL, M_BASE_STATUS_PREPARE, wobj->unit_running_type, NULL, in_obj, in_time_s);

        m_callable_device_proper_status_update_to_fast_running();
    }
    else
    {
        DEBUG_TEST(DB_W, "Warring:  Currently Running [ Manual/Fast/Scheduled %d ]", device_status);
    }

    return stat;
}