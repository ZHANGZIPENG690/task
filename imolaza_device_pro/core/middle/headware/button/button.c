

#include "button.h"

struct limitButtonOperation
{
    /** 是否在更改时间 */
    bool adjusttime;
    /** 是否状态更新 */
    bool stateUpdate;
    /** 设备类型 最大通道数*/
    uint8_t max_channel;
    /** 当前操作的事件 */
    enum key_id event;
    /** 次数统计*/
    uint32_t pre_count;
    /** 上一个IO值*/
    uint8_t preTempVue;
    /** 当前IO值*/
    uint8_t currentTempVue;
    /** 第一次或者重新计算的时间 */
    uint64_t tranf_key_pressdown_time_ms;
    /**按键按下的长短按类型**/
    enum key_event key_event;

};

struct limitButtonOperation lmo = {0};
/** 全局当前时间,用于辅助按键时间间隔测量 */
uint64_t global_current_time_ms = 0;
/** 设备类型 最大IO  */
uint32_t device_hearware_type = 0;
// extern void m_static_drive_key_interrupt_handle(void *args);
// extern stat_m m_static_drive_key_ebouncing(uint32_t key_index);

stat_m (*m_callable_key_handle)(enum key_event key_id, uint8_t pre_key_index, uint8_t current_key_index, uint64_t current_time_ms);
/** 计数的缓存 累积变量 */
int global_press_count_int = 0;
stat_m m_callable_drive_button_init(uint8_t device_type)
{
    lmo.adjusttime = false;
    lmo.max_channel = device_type;
    lmo.currentTempVue = 1;
    m_callable_device_attribute_get_hearward_version(&device_hearware_type);
    global_press_count_int = 0;
    return m_ext_drive_button_gpio_and_interrupt_init();
}
/**
 * @brief 直接停止一个触发
 *
 * @return stat_m
 */
stat_m m_callable_drive_button_direct_stop(void)
{
    m_callable_manual_event_input(M_KEY_EVENT_STOP, lmo.preTempVue, lmo.currentTempVue, global_current_time_ms);
    return succ_r;
}
/**
 * @brief 直接驱动时间的调用 而不是硬件响应
 *
 * @param key_event_id
 * @return stat_m
 */
stat_m m_callable_drive_button_event_direct_input(enum key_id key_event_id)
{
    lmo.stateUpdate = false;
    lmo.event = key_event_id;
    if (lmo.currentTempVue == lmo.preTempVue || lmo.pre_count <= 1)
        lmo.pre_count++;
    return succ_r;
}
// post --> this
stat_m m_callable_drive_button_event_input(enum key_id key_event_id)
{
    // 只去抖和改变参数
    lmo.event = key_event_id;
    lmo.stateUpdate = true;
    // global_press_count_int++;
    return 0;
}

/**---------------------------------------------------------*/
/**---------------------------------------------------------*/
stat_m m_static_drive_button_tranfmoss(uint64_t current_time_ms);

uint64_t gl_zone_select = 0;
uint64_t gl_zone_select_add_add = 0;
//
int out_ste = 0;

int tran_port(enum key_id kid)
{
    switch (kid)
    {

    case M_KEY_EVENT_START:
        return 26;
        break;
    case M_KEY_EVENT_STOP:
        return 27;
        break;
    case M_KEY_EVENT_LEFT:
        return 32;
        break;
    case M_KEY_EVENT_RIGHT:
        return 33;
        break;
    case M_KEY_EVENT_RESET:
        return 34;
        break;

    default:
        break;
    }
    return 0;
}
/**
 * @brief 这里简单区抖
 *
 * @param current_time_ms
 * @return stat_m
 */
stat_m m_callable_drive_button_function_monitor(uint64_t current_time_ms)
{
    stat_m stat = fail_r;
    global_current_time_ms = current_time_ms;
    m_static_drive_button_tranfmoss(current_time_ms);
    if (lmo.stateUpdate && lmo.tranf_key_pressdown_time_ms == 0)
        lmo.tranf_key_pressdown_time_ms = global_current_time_ms;

    // DEBUG_TEST(DB_I,"%d - %d - %d - %d " , m_ext_drive_gpio_get_level(26) , m_ext_drive_gpio_get_level(27 ) , m_ext_drive_gpio_get_level(33) , m_ext_drive_gpio_get_level(32));
    // if (lmo.stateUpdate && (current_time_ms > lmo.tranf_key_pressdown_time_ms + M_KEY_LIMIT_TIME && lmo.tranf_key_pressdown_time_ms > 0))
    // {
    //     lmo.stateUpdate = false;
    //     // lmo.tranf_key_pressdown_time_ms = current_time_ms;
    //     // 按下计数
    // DEBUG_TEST( DB_I,"Interrupt Count: %d   %d -> Level %d", global_press_count_int , tran_port(lmo.event) , m_ext_drive_gpio_get_level(tran_port(lmo.event)));
    //     if (
    //         m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 1 && global_press_count_int > 2 &&(global_press_count_int < 12 || global_press_count_int > 30) && global_press_count_int < 260)
    //         // m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 1 && global_press_count_int > 2 &&(global_press_count_int < 12 || global_press_count_int > 30) && global_press_count_int < 260)
    //     {
    //         if (lmo.currentTempVue == lmo.preTempVue || lmo.pre_count <= 1)
    //             lmo.pre_count++;
    //     }
    //         global_press_count_int= 0;
    //     // lmo.pre_count = 0;
    // DEBUG_TEST( DB_I,"%dUp: %d   %lld -> Level %d", lmo.event, lmo.stateUpdate , lmo.tranf_key_pressdown_time_ms , m_ext_drive_gpio_get_level(tran_port(lmo.event)));
    // if (lmo.stateUpdate && lmo.tranf_key_pressdown_time_ms > 0 && m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 0)
    // {
    //     global_press_count_int = 99;
    //     // DEBUG_TEST( DB_I,"Interrupt Count: %d   %d -> Level %d", global_press_count_int , tran_port(lmo.event) , m_ext_drive_gpio_get_level(tran_port(lmo.event)));
    // }
    // if (global_press_count_int == 99 && m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 1)
    // {
    //     global_press_count_int = 0;
    //     lmo.stateUpdate = false;
    //     if (lmo.currentTempVue == lmo.preTempVue || lmo.pre_count <= 1)
    //         lmo.pre_count++;
    // }
    if (lmo.stateUpdate && (current_time_ms > lmo.tranf_key_pressdown_time_ms + M_KEY_LIMIT_TIME && lmo.tranf_key_pressdown_time_ms > 0))
    {
        if(m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 1)
        {
            mDelay_ms(10);
            if(m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 1)
            {
                lmo.pre_count = 1;
                if(current_time_ms  > lmo.tranf_key_pressdown_time_ms + 480)
                    lmo.key_event = M_KEY_EVENT_LONG_PRESS;
                else
                    lmo.key_event = M_KEY_EVENT_SIGN_CLICK;
            }
        }
    }  
    {
        // lmo.tranf_key_pressdown_time_ms = current_time_ms;
        // 按下计数
        // if (
        //     m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 1 && global_press_count_int > 2 &&(global_press_count_int < 12 || global_press_count_int > 30) && global_press_count_int < 260)
        //     // m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 1 && global_press_count_int > 2 &&(global_press_count_int < 12 || global_press_count_int > 30) && global_press_count_int < 260)
        // {
        //     if (lmo.currentTempVue == lmo.preTempVue || lmo.pre_count <= 1)
        //         lmo.pre_count++;
        // }
        //     global_press_count_int= 0;
        // lmo.pre_count = 0;
    }
    // lmo.tranf_key_pressdown_time_ms = 0;
    // else if (lmo.stateUpdate && m_ext_drive_gpio_get_level(tran_port(lmo.event)) == 1 &&  lmo.tranf_key_pressdown_time_ms + 3 >= current_time_ms )
    // {
    //     lmo.stateUpdate = false;
    // }
    // printf("%lld\n" , current_time_ms);

    if (m_callable_display_status_get() == M_DISPLAY_ZONE_SELECT_MODE)
    {
        gl_zone_select++;
        if (gl_zone_select > 100 * 60 * 3)
        {
            m_callable_device_proper_status_get(&out_ste);
            gl_zone_select = 0;
            if (out_ste == M_DEVICE_GLOBAL_STATUS_IDLE)
            {
                m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
            }
        }

    }
    else
        gl_zone_select_add_add = gl_zone_select = 0;

    return stat;
}
/**
 * @brief 按下之后 开始判断是按下多少次 来判断是单击 双击  还是长按
 *
 * @param current_time_ms
 * @return stat_m
 */
stat_m m_static_drive_button_tranfmoss(uint64_t current_time_ms)
{
    // DEBUG_TEST( DB_I,"klk");
    if (m_ext_drive_button_reset_trig(current_time_ms) == succ_r)
    {
        m_ext_drive_button_event_handle(M_KEY_EVENT_RESET, M_KEY_EVENT_LONG_PRESS, M_KEY_EVENT_RESET, lmo.currentTempVue, current_time_ms);
    }

    if (lmo.pre_count!=0 && current_time_ms > lmo.tranf_key_pressdown_time_ms + M_KEY_LIMIT_TIME_HANDLE_MAX)
    {
        if(lmo.key_event == M_KEY_EVENT_LONG_PRESS)//长按
        {
            if(lmo.event == M_KEY_EVENT_START)
            {
                if(m_callable_display_status_get() == M_DISPLAY_ZONE_SELECT_MODE)//区域是否被选中
                {
                    //区域被选中，进去修改时长
                    lmo.adjusttime = true;
                }else if(m_callable_display_status_get() == M_DISPLAY_IDLE_STATUS_MODE)//是否为空闲
                {
                    lmo.event = M_KEY_EVENT_WATERED_ALL;
                }
            }
        }else if(lmo.key_event == M_KEY_EVENT_SIGN_CLICK)//短按
        {
            gl_zone_select_add_add = gl_zone_select = 0;

            /** 普通设备和Pro设备相反，这里取反下*/
            if (device_hearware_type != DEVICE_HEARWARE_A003)
            {
                if (lmo.event == M_KEY_EVENT_LEFT)
                    lmo.event = M_KEY_EVENT_RIGHT;
                else if (lmo.event == M_KEY_EVENT_RIGHT)
                    lmo.event = M_KEY_EVENT_LEFT;
            }
            // DEBUG_TEST("单击！");
            if (lmo.event == M_KEY_EVENT_LEFT)
            {
                if(lmo.adjusttime == true)
                {
                    m_callable_manual_adjust_time(lmo.event,lmo.currentTempVue);
                }else if(m_callable_manual_get_sol_even() != M_OPERATE_EVENT_RUNNING_SWITCH_ALL)
                {
                     lmo.preTempVue = lmo.currentTempVue;
                    if (m_callable_display_status_get() != M_DISPLAY_IDLE_STATUS_MODE && m_callable_display_status_get() != M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE)
                        lmo.currentTempVue++;
                    if (lmo.currentTempVue > lmo.max_channel)
                        lmo.currentTempVue = 1;
                }
            }
            else if (lmo.event == M_KEY_EVENT_RIGHT)
            {
                if(lmo.adjusttime == true)
                {
                    m_callable_manual_adjust_time(lmo.event,lmo.currentTempVue);
                }else if(m_callable_manual_get_sol_even() != M_OPERATE_EVENT_RUNNING_SWITCH_ALL)
                {
                    // lmo.preTempVue = lmo.currentTempVue;
                    // if (lmo.currentTempVue <= 1)
                    //     lmo.currentTempVue = lmo.max_channel + 1;
                    // if (m_callable_display_status_get() != M_DISPLAY_IDLE_STATUS_MODE && m_callable_display_status_get() != M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE)
                    //     lmo.currentTempVue--;

                    lmo.preTempVue = lmo.currentTempVue;
                    if (m_callable_display_status_get() != M_DISPLAY_IDLE_STATUS_MODE && m_callable_display_status_get() != M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE)
                        lmo.currentTempVue--;
                    if (lmo.currentTempVue < 1)
                        lmo.currentTempVue = lmo.max_channel;
                    // printf("lmo.currentTempVue %d   max_channel %d  \n", lmo.currentTempVue, lmo.max_channel);
                }
            }else if(lmo.event == M_KEY_EVENT_START)
            {
                if(lmo.adjusttime == true)
                    lmo.adjusttime = false;
            }
        }
        m_ext_drive_button_event_handle(lmo.event, lmo.key_event, lmo.preTempVue, lmo.currentTempVue, current_time_ms);
        lmo.stateUpdate = false;
        lmo.preTempVue = lmo.currentTempVue;
        lmo.tranf_key_pressdown_time_ms = 0;
        lmo.pre_count = 0;
    }
    return 0;
}
/**
 * @brief 获取最大个通道数发给manual_run.c中存放时间的数组
 *
 * @param current_time_ms
 * @return stat_m
 */
int m_static_drive_get_max_channel(void)
{
    return lmo.max_channel;
}

// [50,813 0 3&2 1649635200 2147483647 116S0 1#15;2#15;3#20;4#15;5#15;6#15;7#12;8#13 null 0 109 7320]
// [50,828 0 5&1 1649635200 2147483647 102 1#14;2#16;3#16;4#14;5#15;6#20;7#10;8#15 null 0 109 7200]
// [50,3221 0 1&59 1685577600 1686873599 116E0 1#6;2#6;3#6;4#6;5#6;6#6;7#6;8#6 null 0 109 18480]
// [50,3220 0 3&57 1685577600 1686873599 115E0 1#24;2#24;3#24;4#24;5#24;6#24;7#24;8#24 7;1;2;3;4;6;5 0 104 26640]
// [50,3789 2 null 1691107200 1693526399 0 1#8;2#8;3#8;4#7;5#7;6#7;7#7;8#7 null 0 120 3540]
// [50,3788 2 1&3 1691107200 1693526399 0 1#1;2#1;3#1;4#1;5#1;6#1;7#2;8#2 null 0 120 720]
// [50,3790 2 3&1 1691107200 1693526399 0 1#8;2#7;3#7;4#8;5#7;6#8;7#7;8#7 null 0 120 3540]
// [50,3788 2 1&3 1679443200 1681171199 0 1#2;2#1;3#1;4#1;5#1;6#1;7#2;8#1 null 0 120 660]
// [50,4321 2 3&1 1688169600 1690847999 0 1#5;2#5;3#5;4#5;5#5;6#5 null 0 120 1800]
// [50,4322 2 3&1 1686873600 1688169599 0 7#3;8#3 null 0 120 360]
// [50,4573 0 8&30 1691107200 1693526399 100&09:45 1#1;2#1;3#1;4#1;5#1;6#1 7;1;2;3;4;5;6 0 104 360]
// [50,5759 0 6&30 1706745600 4070995199 100&20:00 2#7;1#7;3#7;4#7 null 0 109 3300]
// [50,5760 1 1&15 1706745600 4070995199 100&21:00 1#21;2#21;3#21;4#21 null 0 109 19440]
// [50,5761 2 1&10 1706745600 4070995199 0 1#15;3#15 null 0 120 9360]
