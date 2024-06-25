
#include "manual.h"

#define M_MANUAL_RUNNING_AUTO_STOP_TIME "auto_stop_s"

enum manual_state
{
    M_MANUAL_STATTE_IDLE,
    M_MANUAL_STATTE_SELECT,
    M_MANUAL_STATTE_RUNNING,
    M_MANUAL_STATTE_PAUSE,
    M_MANUAL_STATTE_STOP,
    M_MANUAL_STATTE_ALL,//全部区域运行
};

stat_m (*m_callable_manual_handle)(enum operate_event event_id, uint8_t pre_channel, uint8_t next_channel, uint32_t has_benn_runnint_time, uint64_t current_time);

struct _manual
{
    /**最大时长**/
    uint64_t Maximum_duration[50];
    /* 是否在运行 */
    enum manual_state status;
    /* 自动停止的时间 */
    uint32_t auto_stop_s;
    /* 上一个运行的区域 */
    uint8_t pre_channel;
    /* 当前运行的区域 */
    uint8_t channel;
    /* 当前运行的开始时间 */
    uint64_t runnint_time_start;
    /* 当前运行的时间 */
    uint64_t runnint_time_current;
    /**上次时长**/
    uint64_t runnint_time_last;
    /** 当前的ms */
    uint64_t global_current_time_ms;
    /** 记录操作 */
    enum operate_event sol_even;
    /** 设备类型 区域最大值*/
    uint8_t max_channel;

    int minx;
};
struct _manual manual = {0};
/**
 * @brief 手动运行初始化
 * 
 * @return stat_m 
 */
stat_m m_callable_manual_init(void)
{
    stat_m stat = fail_r;
    /**初始化 Button*/
    manual.max_channel = m_static_drive_get_max_channel();
    manual.minx = 254;
    manual.auto_stop_s = 0;
    m_callable_drive_flash_read(M_TYPE_U32, M_MANUAL_RUNNING_AUTO_STOP_TIME, &manual.auto_stop_s);
    if (manual.auto_stop_s <= 0)
        manual.auto_stop_s = 100 * manual.max_channel * 3;
    for (int i = 0; i < manual.max_channel; ++i) {
        manual.Maximum_duration[i] = 10;
    }
    return stat;
}
/**
 * @brief 设置自动停止时间
 * 
 * @param time_s 
 * @return stat_m 
 */
stat_m m_callable_manual_set_auto_stop_s(uint32_t time_s)
{
    stat_m stat = fail_r;
    if (time_s > 0)
    {
        stat = succ_r;
        manual.auto_stop_s = time_s * 100;
        m_callable_drive_flash_write(M_TYPE_U32, M_MANUAL_RUNNING_AUTO_STOP_TIME, &manual.auto_stop_s);
    }
    return stat;
}
uint64_t acc_trig = 0;
/**
 * @brief 外部相关事件处理  电流等
 * 
 * @param event_id 事件id
 * @param evt 事件数据
 * @param current_time_s 触发时间
 * @return stat_m 
 */
stat_m m_callable_manual_event_export_input(int event_id, int evt, uint64_t current_time_s)
{
    int temp_int = 0, temp_int_a = evt;
    manual.minx = 254;
    if (manual.status == M_MANUAL_STATTE_RUNNING)
    {
        switch (event_id)
        {
        case M_INSTANCE_RUNNING_CURRENT_NOTIFY:
            temp_int = (int)m_callable_current_check_get_final_value();
            DEBUG_TEST(DB_W,"电流事件 值%d .. ", temp_int);
            m_callable_local_resp_to_remote(
                M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK, M_TYPE_Int, (void *)&manual.minx,
                M_TYPE_U8, (void *)&(manual.channel),
                M_TYPE_Int, (void *)&temp_int,
                M_TYPE_Int, (void *)&temp_int_a, current_time_s, false);
            break;
        case M_INSTANCE_RUNNING_CURRENT_NOTIFY_VALUE_OUT:
            temp_int = 1195;
            m_callable_local_resp_to_remote(
                M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK, M_TYPE_Int, (void *)&manual.minx,
                M_TYPE_U8, (void *)&(manual.channel),
                M_TYPE_Int, (void *)&temp_int,
                M_TYPE_Int, (void *)&temp_int_a, current_time_s, true);
            break;

        case M_INSTANCE_RUNNING_CURRENT_NOTIFY_INTERRUPT:
            temp_int = MAX_SHORT_CURRENT;
            DEBUG_TEST(DB_W,"m_callable_display_status_get  %d", m_callable_display_status_get());
            // if (m_callable_display_status_get() != M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE)
            if (evt == M_CURRENT_SHORT_MODE_SIGN_ZONE)
                m_callable_display_status(M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE, manual.channel);
            else if (evt == M_CURRENT_SHORT_MODE_PUMP)
                m_callable_display_status(M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE, manual.channel);

            DEBUG_TEST(DB_W,"m_callable_display_status_get  %d", m_callable_display_status_get());
            // temp_int_a = 1;
            if (acc_trig == current_time_s)
                break;
            acc_trig = current_time_s;
            m_callable_local_resp_to_remote(
                M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK, M_TYPE_Int, (void *)&manual.minx,
                M_TYPE_U8, (void *)&(manual.channel),
                M_TYPE_Int, (void *)&temp_int,
                M_TYPE_Int, (void *)&temp_int_a, current_time_s, true);

            break;

        default:
            break;
        }
    }
    return succ_r;
}
/**
 * @brief 外部事件输入04:13 PM
 *
 * @param event_id 事件id
 * @param pre_channel 上一个区域
 * @param next_channel 下一个区域
 * @param current_time_ms 当前的时间搓
 * @return stat_m
 */
stat_m m_callable_manual_event_input(enum key_id event_id, uint8_t pre_channel, uint8_t next_channel, uint64_t current_time_ms)
{
    stat_m stat = fail_r;
    // enum operate_event sol_even = 0;
    current_time_ms = manual.global_current_time_ms;
    switch (event_id)
    {
    case M_KEY_EVENT_WATERED_ALL:
        //printf("全部浇灌\n");
        if (manual.status != M_MANUAL_STATTE_ALL)
        {
            /** 如果暂停或者选择 那么继续开始 更新下参数*/
            manual.sol_even = M_OPERATE_EVENT_RUNNING_SWITCH_ALL;
            //m_callable_device_proper_status_update_to_manual_running();
            manual.status = M_MANUAL_STATTE_ALL;
            manual.pre_channel = 1;
            manual.channel = 1;
            next_channel = manual.channel;
            manual.runnint_time_start = current_time_ms;
            m_callable_manual_set_auto_stop_s(manual.Maximum_duration[manual.channel-1]);
            m_ext_manual_event_handle(manual.sol_even, pre_channel, next_channel, manual.runnint_time_current /100, current_time_ms);
        }else
        {
            manual.sol_even = M_OPERATE_EVENT_RUNNING_SWITCH_ALL;//任务切换
            /** 新区域开始计数，更新时间参数 */
            manual.runnint_time_current = (current_time_ms - manual.runnint_time_start);        
            manual.channel = next_channel;
            manual.runnint_time_start = current_time_ms;
            m_callable_manual_set_auto_stop_s(manual.Maximum_duration[manual.channel-1]);
            m_ext_manual_event_handle(manual.sol_even, pre_channel, next_channel, manual.runnint_time_current /100, current_time_ms);
        }
    break;
    case M_KEY_EVENT_LEFT:
    case M_KEY_EVENT_RIGHT:
        /** 如果 左右 在运行 */
        if (manual.status == M_MANUAL_STATTE_RUNNING)
        {
            manual.sol_even = M_OPERATE_EVENT_RUNNING_SWITCH;
            /** 新区域开始计数，更新时间参数 */
            manual.channel = next_channel;
            manual.runnint_time_current = (current_time_ms - manual.runnint_time_start);
            manual.runnint_time_start = current_time_ms;
        }
        /** 都是选择和切换，不改变参数 */
        break;
        /** 开始： 开始和暂停*/
    case M_KEY_EVENT_START:
        // if (!manual.status == M_MANUAL_STATTE_IDLE)
        {
            if (manual.status == M_MANUAL_STATTE_RUNNING)
            {
                manual.sol_even = M_OPERATE_EVENT_PAUSE;
                manual.status = M_OPERATE_EVENT_PAUSE;
                m_callable_device_proper_status_update_to_idle();
                manual.runnint_time_current = current_time_ms - manual.runnint_time_start;
            }
            else
            // if (manual.status == M_MANUAL_STATTE_PAUSE || manual.status == M_MANUAL_STATTE_SELECT)
            {
                /** 如果暂停或者选择 那么继续开始 更新下参数*/
                manual.sol_even = M_OPERATE_EVENT_START;
                m_callable_device_proper_status_update_to_manual_running();
                manual.status = M_MANUAL_STATTE_RUNNING;
            }
            manual.pre_channel = pre_channel;
            manual.channel = next_channel;
            manual.runnint_time_start = current_time_ms;
            m_ext_manual_event_handle(manual.sol_even, pre_channel, next_channel, manual.runnint_time_current / 100, current_time_ms);
            m_callable_manual_set_auto_stop_s(manual.Maximum_duration[next_channel-1]);
        }
        break;
    case M_KEY_EVENT_STOP:
        // if (!manual.status == M_MANUAL_STATTE_IDLE)
        {
            if (next_channel == M_CONST_SACK_NUM)
                next_channel = manual.channel;
            if (manual.status == M_MANUAL_STATTE_RUNNING || manual.status == M_MANUAL_STATTE_ALL)
            {
                manual.sol_even = M_OPERATE_EVENT_STOP;
                manual.status = M_OPERATE_EVENT_STOP;
                manual.runnint_time_current = current_time_ms - manual.runnint_time_start;
            }
            else if (manual.status == M_MANUAL_STATTE_PAUSE || manual.status == M_MANUAL_STATTE_SELECT)
            {
                manual.status = M_OPERATE_EVENT_STOP;
                manual.sol_even = M_OPERATE_EVENT_STOP_SELECT;
                manual.runnint_time_current = 0;
            }
            else
                manual.sol_even = M_OPERATE_EVENT_STOP;
            m_ext_manual_event_handle(manual.sol_even, pre_channel, next_channel, manual.runnint_time_current / 100, current_time_ms);
            m_callable_device_proper_status_update_to_idle();
        }
        break;
    default:
        break;
    }
    return stat;
}
/**
 * @brief 获取运行区域
 *
 * @param in_zone
 * @return stat_m
 */
stat_m m_callable_manual_running_zone_get(int *in_zone)
{
    *in_zone = manual.channel;
    return succ_r;
}
/**
 * @brief 事件触发检测 轮询
 * 
 * @param current_time_ms 时间
 * @return stat_m 
 */
stat_m m_callable_manual_function_monitor(uint64_t current_time_ms)
{
    stat_m stat = fail_r;
    manual.global_current_time_ms = current_time_ms;

    // DEBUG_TEST(DB_W,"[%d]    [%lld] > [%lld]", manual.auto_stop_s, current_time_ms, manual.runnint_time_start);
    manual.runnint_time_current = manual.global_current_time_ms - manual.runnint_time_start;
    if(manual.channel != 0 && ((manual.runnint_time_current/100) <= manual.Maximum_duration[manual.channel-1]) && manual.sol_even !=  M_OPERATE_EVENT_STOP && manual.sol_even != M_OPERATE_EVENT_PAUSE && manual.sol_even != M_OPERATE_EVENT_STOP_SELECT)
    {
        if((manual.runnint_time_current/100) != (manual.runnint_time_last/100) && (manual.runnint_time_current/100) != 0)
        {
            manual.runnint_time_last = manual.runnint_time_current;
            //printf("区域%d运行了%lld秒\n",manual.channel,manual.runnint_time_current/100);
        }
    }

    if (manual.sol_even != M_OPERATE_EVENT_START && manual.sol_even != M_OPERATE_EVENT_PAUSE &&
        current_time_ms == manual.runnint_time_start + 40 && manual.runnint_time_start != 0)
    {
        // manual.runnint_time_start -= 60;

        m_ext_manual_event_handle(manual.sol_even, manual.pre_channel, manual.channel, manual.runnint_time_current / 100, current_time_ms);
        manual.pre_channel = manual.channel;
    }

    if ((manual.status == M_MANUAL_STATTE_RUNNING || manual.status == M_MANUAL_STATTE_ALL)  && current_time_ms > manual.auto_stop_s + manual.runnint_time_start)
    {
        if( manual.status == M_MANUAL_STATTE_ALL)
        {
            if(manual.channel == manual.max_channel)
            {
                manual.status = M_MANUAL_STATTE_STOP;
                manual.sol_even = M_OPERATE_EVENT_STOP;
                manual.runnint_time_current = current_time_ms - manual.runnint_time_start;
                m_ext_manual_event_handle(manual.sol_even, manual.pre_channel, manual.channel, manual.runnint_time_current / 100, current_time_ms);
            }else
            {
                manual.pre_channel = manual.channel;
                
                manual.runnint_time_current = current_time_ms - manual.runnint_time_start;
                //printf("当前区域%d运行了%lld秒了,准备切换下一区域\n",manual.channel,manual.runnint_time_current/100);
                manual.channel = manual.channel+1;
                m_callable_manual_event_input(M_KEY_EVENT_WATERED_ALL,manual.pre_channel,manual.channel,current_time_ms);
                manual.runnint_time_current = 1;
            }
            //m_callable_manual_set_auto_stop_s(manual.Maximum_duration[manual.channel-1]);
        }else
        {
            manual.status = M_MANUAL_STATTE_STOP;
            manual.sol_even = M_OPERATE_EVENT_STOP;
            manual.runnint_time_current = current_time_ms - manual.runnint_time_start;
            m_ext_manual_event_handle(manual.sol_even, manual.pre_channel, manual.channel, manual.runnint_time_current / 100, current_time_ms); 
        }
        // manual.sol_even = M_OPERATE_EVENT_STOP;
        // manual.status = M_OPERATE_EVENT_STOP;
        // manual.runnint_time_current = current_time_ms - manual.runnint_time_start;
        // m_ext_manual_event_handle(manual.sol_even, manual.pre_channel, manual.channel, manual.runnint_time_current / 100, current_time_ms);
    }
    // if (current_time_ms > manual.auto_stop_s + manual.runnint_time_start && manual.runnint_time_start != 0)
    // {
    //     enum operate_event sol_even = 0;
    //     if (manual.status == M_MANUAL_STATTE_RUNNING)
    //     {
    //         sol_even = M_OPERATE_EVENT_STOP;
    //         manual.runnint_time_current = current_time_ms - manual.runnint_time_start;
    //     }
    //     if (manual.status == M_MANUAL_STATTE_SELECT)
    //     {
    //         sol_even = M_OPERATE_EVENT_STOP_SELECT;
    //         manual.runnint_time_current = 0;
    //     }
    //     m_ext_manual_event_handle(sol_even, manual.pre_channel, manual.channel, manual.runnint_time_current / 1000, current_time_ms);
    // }
    return stat;
}

/**
 * @brief 修改时长
 * 
 * @param kid 
 * @param currentTempVue 
 * @return stat_m 
 */
stat_m m_callable_manual_adjust_time(enum key_id kid,uint8_t currentTempVue)
{
    stat_m stat = fail_r;
    switch(kid)
    {
        case M_KEY_EVENT_RIGHT:
        if(manual.Maximum_duration[currentTempVue-1] <= 5)
        {
            //printf("时间不足5分钟,无法减去5分钟\n");
        }
        else
        {
            manual.Maximum_duration[currentTempVue-1] -=5;
            //printf("成功减少5分钟\n");
            //printf("区域%d的时间为%lld",currentTempVue,manual.Maximum_duration[currentTempVue-1]);
        }     
        break;
        case M_KEY_EVENT_START:
        return true;
        break;
        case M_KEY_EVENT_LEFT:
        if(manual.Maximum_duration[currentTempVue-1] == 60)
        {
            //printf("时间最大值为60分钟,无法继续增加\n");
        }else
        {
            manual.Maximum_duration[currentTempVue-1] +=5;
            //printf("成功增加5分钟\n");
            //printf("区域%d的时间为%lld",currentTempVue,manual.Maximum_duration[currentTempVue-1]);
        }
        break;
        default:
        break;
        
    }
    return stat;
}

/**
 * @brief 获取记录操作
 * 
 * @param 
 * @param  
 * @return stat_m 
 */
int m_callable_manual_get_sol_even(void)
{
    return manual.sol_even;
}

