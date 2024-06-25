

/**
 *
 *     一般的事件条件触发之后会把对应数据进行一次中转
 *     这里是中转文件*
 *
 *
 */
#include "notify.h"
#include "core/fset_lib.h"
#include "main.h"

char *_toname_plus(int name);

stat_m m_callable_achedule_end(void *pdata, uint64_t in_dev_time)
{
    stat_m stat = fail_r;
    schedule_instance_m *wobj = (schedule_instance_m *)pdata;

    if (wobj != NULL)
    {
        for (int i = 0; i < wobj->in_running_buff.maxChan; i++)
        {
            if(wobj->in_channel[i].id > 0)
            {
                DEBUG_E("id %hhd readTime:%d shoulddown %d (%lld)" ,wobj->in_channel[i].id ,  wobj->in_channel[i].realityRunTime  ,wobj->in_channel[i].shouldRunTime , in_dev_time - wobj->in_channel[i].runtime.tr_end_time);
                if(wobj->in_channel[i].shouldRunTime > wobj->in_channel[i].realityRunTime )
                {
                    stat = fail_r;
                    if(i == wobj->in_running_buff.rb_pc  &&
                     in_dev_time - wobj->in_channel[i].runtime.tr_start_time + wobj->in_channel[i].realityRunTime >= wobj->in_channel[i].shouldRunTime )
                        stat = succ_r; 
                }
                // else 
                //     stat = succ_r;

                // if (in_dev_time - wobj->in_instance_id < 50)
                //     stat = succ_r;
                // else
                //     stat = fail_r;
            }
        }

        DEBUG_E("present_timsp %lld   tr_end_time %lld", in_dev_time, wobj->in_instance_id);
    }
    // wobj->running.current_running_time_region.tr_end_time);
    return stat;
}

stat_m m_event_transit_processing_schedule(void *event_data, uint8_t sta_tag, uint64_t in_dev_time)
{
    stat_m stat = fail_r;
    switch (sta_tag)
    {
    case m_schedule__prepare:
        sta_tag = 0;
        break;
        break;
    case m_schedule__idle:
        sta_tag = 1;
        break;
    case m_schedule__start:
        sta_tag = 1;
        break;
    case m_schedule__sack:
        sta_tag = 3;
        break;
    case m_schedule__running:
        sta_tag = 2;
        if (m_callable_achedule_end(event_data, in_dev_time) == succ_r)
            sta_tag = 5;

        break;
    case m_schedule__pause:
    case m_schedule__stop:
        sta_tag = 5;
    case m_schedule__water_hammer:
        sta_tag = 6;
        break;

    default:
        stat = fail_r;
        break;
    }
    if (event_data != NULL)
    {
        stat = succ_r;

        m_callable_schedule_fsm_post_event(sta_tag, 0, (void *)event_data, in_dev_time);
    }

    return stat;
}

// 停止应该分为 直接停止(5)和隐式停止(7)及静默停止(9)
#define M_EVENT_RUNNING_TRIGGER_DIRECT_STOP 5
#define M_EVENT_RUNNING_TRIGGER_SECTION_STOP 7
#define M_EVENT_RUNNING_TRIGGER_CATLIKE_STOP 9

stat_m m_event_transit_processing_running_trigger(void *in_event_data ,uint8_t in_event_data_type ,uint8_t in_event_id , uint64_t in_dev_time)
{
    stat_m stat = fail_r;
    // enum _schedule_run_type t_run_type = RUN_TYPE_UNIVERSAL;

    schedule_instance_m *wobj = (schedule_instance_m *)in_event_data;
    DEBUG_OUT_TO_FILE_COLOR("\033[0;31m m_event_transit_processing_running_trigger -> * %s (id %lld - %lld)\n", _toname_plus(in_event_id), wobj->in_run_id,in_dev_time);
    // int sub_id = 0;
    switch (in_event_id)
    {
    case MANUAL_STOP:
        // 记录停止发生时间和实例id
        in_event_id = 5;
        break;
        // 远程停止
    case SOCKET_CMD_STOP:
        in_event_id = 5;
        break;
        // 区域改变
    case SCHEDULE_CHANGE_STOP:
        in_event_id = 5;
        break;
        // 计划删除
    case SCHEDULE_DELETE_STOP:
        in_event_id = 5;
        break;
        // 计划关闭
    case SCHEDULE_DISABLE_STOP:
        in_event_id = 5;
        break;
        // 计划跳过
    case A_SIGNLE_SKIP_STOP:
        in_event_id = 5;
        break;
        // 全局跳过
    case GLOBAL_SKIP_STOP:
        in_event_id = 5;
        break;
        // 天气跳过
    case WIND_RAIN_SKIP_STOP:
        in_event_id = 5;
        break;
        // 静默模式跳过
    case DORMANT_MODE_STOP:
        in_event_id = 5;
        break;
        // 传感器触发 隐式停止
    case SENSOR_TRIGGER:
        in_event_id = 7;
        // sub_id = M_EVENT_RUNNING_TRIGGER_SECTION_STOP;
        break;
        // 传感器触发 重新开始
    case SENSOR_DIS_TRIGGER:
        in_event_id = 8;
        break;
        // 区域启用 直接停止
    case CHANNEL_ENABLE_TRIGGER:
        in_event_id = 5;
        break;
        // 区域禁用 直接停止
    case CHANNEL_DISABLE_TRIGGER:
        in_event_id = 5;
        break;
        // 外部按键触发 开始/暂停 按键
    case CHANGING_OVER_START_TRIGGER:
        break;
        // 外部按键触发 停止按键
    case CHANGING_OVER_STOP_TRIGGER:
        in_event_id = 5;
        break;
        // 外部按键触发 左按键
    case CHANGING_OVER_LEFT_TRIGGER:
        in_event_id = 10;
        break;
        // 外部按键触发 右按键
    case CHANGING_OVER_RIGHT_TRIGGER:
        in_event_id = 11;
        break;
    }
    // Event classification （Schedule）
        m_callable_schedule_fsm_post_event(in_event_id, in_event_data_type, (void *)in_event_data, in_dev_time);
    // Event classification （Quick）

    return stat;
}

char *_toname_plus(int name)
{
    switch (name)
    {
    case MANUAL_STOP:
        return "MANUAL_STOP ";
        break;
    case SOCKET_CMD_STOP:
        return "SOCKET_CMD_STOP ";
        break;
    case SCHEDULE_CHANGE_STOP:
        return "SCHEDULE_CHANGE_STOP ";
        break;
    case SCHEDULE_DELETE_STOP:
        return "SCHEDULE_DELETE_STOP ";
        break;
    case SCHEDULE_DISABLE_STOP:
        return "SCHEDULE_DISABLE_STOP ";
        break;
    case A_SIGNLE_SKIP_STOP:
        return "A_SIGNLE_SKIP_STOP ";
        break;
    case GLOBAL_SKIP_STOP:
        return "GLOBAL_SKIP_STOP ";
        break;
    case WIND_RAIN_SKIP_STOP:
        return "WIND_RAIN_SKIP_STOP ";
        break;
    case DORMANT_MODE_STOP:
        return "IDDORMANT_MODE_STOPLE ";
        break;
    case SENSOR_TRIGGER:
        return "SENSOR_TRIGGER ";
        break;
    case SENSOR_DIS_TRIGGER:
        return "SENSOR_DIS_TRIGGER ";
        break;
    case CHANNEL_ENABLE_TRIGGER:
        return "CHANNEL_ENABLE_TRIGGER ";
        break;
    case CHANNEL_DISABLE_TRIGGER:
        return "CHANNEL_DISABLE_TRIGGER ";
        break;
    case CHANGING_OVER_START_TRIGGER:
        return "CHANGING_OVER_START_TRIGGER ";
        break;
    case CHANGING_OVER_STOP_TRIGGER:
        return "CHANGING_OVER_STOP_TRIGGER ";
        break;
    case CHANGING_OVER_LEFT_TRIGGER:
        return "CHANGING_OVER_LEFT_TRIGGER ";
        break;
    case CHANGING_OVER_RIGHT_TRIGGER:
        return "CHANGING_OVER_RIGHT_TRIGGER ";
        break;
    default:
        break;
    }
}
