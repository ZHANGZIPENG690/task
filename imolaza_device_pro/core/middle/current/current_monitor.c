#include "current_monitor.h"

/** 控制区分感性负载接触触发的短路中中断还是真实的中断短路 */
static int interrupt_short_invalid_count = 0;

int area_power_calibration_flag = 0;

void m_calllable_current_monitor_task(void *pvParameters);

float m_callable_volatage_transform_current(void);

stat_m m_callable_current_get_voltage(float *max_voltage_get, float *min_voltage_get);

stat_m m_static_current_interrupt_handle();

/**
 * @brief 高电流检测处理
 *
 * @return stat_m
 */
stat_m m_static_height_current_handle(int in_current_vue, int evt_id, uint64_t in_time_s);

// 计算电压最大值的函数
float m_static_max_voltage(float voltage1, float voltage2)
{
    return (voltage1 > voltage2) ? voltage1 : voltage2;
}

// 计算电压最小值的函数
float m_static_min_voltage(float voltage1, float voltage2)
{
    return (voltage1 < voltage2) ? voltage1 : voltage2;
}

#define MAX_LIMIT_CURRENT_1S 1800
#define MAX_LIMIT_CURRENT_2S 1100
#define MAX_LIMIT_CURRENT_10S 1000

stat_m m_callable_current_monitor_init(void)
{
    m_ext_drive_adc_init();
    m_ext_drive_adc_init_rtc();
    current_monitor.max_voltage = 0.0;
    current_monitor.min_voltage = 999.99;
    current_monitor.cur_short_mode = M_CURRENT_SHORT_MODE_VT;
    current_monitor.m_global_short_interrupt = false;
    current_monitor.current_calibration_interrupt = false;
    mTaskCreate(NULL, m_calllable_current_monitor_task, "adc_task", 512 * 5, NULL, 2, NULL);

    return succ_r;
}

stat_m m_callable_current_monitor_start(void)
{
    current_monitor.first_time = m_callable_timer_manage_get_device_time();
    current_monitor.current_monitor_flag = 1;
    current_monitor.current_value1_averge = 0;
    current_monitor.current_value1_count = 0;
    m_static_current_interrupt_handle();
    DEBUG_TEST(DB_I, "Start Current Check");
    return succ_r;
}

stat_m m_callable_current_monitor_stop(void)
{
    current_monitor.current_monitor_flag = 0;
    current_monitor.current_value1_averge = 0;
    current_monitor.current_value1_count = 0;
    DEBUG_TEST(DB_I, "Stop Current Check");
    return succ_r;
}

stat_m m_callable_current_monitor_pause_not_clear(void)
{
    current_monitor.current_monitor_flag = 0;
    DEBUG_TEST(DB_I, "pause Current Check");
    return succ_r;
}
/**
 * @brief 设置电流反馈的当前检测区域，需要再每次区域开启更新，保持水锤更新
 *
 * @param in_zone
 * @return stat_m
 */
stat_m m_callable_current_set_checkout_zone(int in_zone)
{
    current_monitor.m_global_short_interrupt_zone = in_zone;
    return succ_r;
}

/**
 * @brief 获取一段时间内的最大值
 *
 * @return float
 */
int m_callable_current_check_get_max_value(void)
{
    while (1)
    {
        m_callable_current_get_voltage(&current_monitor.max_voltage, &current_monitor.min_voltage);

        if (current_monitor.read_adc_interval_time > 40) // 40
        {
            current_monitor.read_adc_interval_time = 9.8;
            if (current_monitor.current_monitor_flag == 0)
            {
                break;
            }
            current_monitor.current_value1 = m_callable_volatage_transform_current();

            if (current_monitor.current_value1 >= 300)

            {
                current_monitor.current_value1 = current_monitor.current_value1 - 20.0;
            }
            else if (current_monitor.current_value1 < 300 && current_monitor.current_value1 >= 150)
            {
                current_monitor.current_value1 = current_monitor.current_value1 - 15.0;
            }
            else if (current_monitor.current_value1 < 150 && current_monitor.current_value1 >= 70)
            {
                current_monitor.current_value1 = current_monitor.current_value1 - 10.0;
            }

            // current_monitor.current_monitor_flag = 0;
            break;
        }
        current_monitor.read_adc_interval_time = current_monitor.read_adc_interval_time + 0.08; // 0.06
    }
    // mDelay_ms(1);
    // DEBUG_E("current_monitor.current_value1  %f", current_monitor.current_value1);
    return (int)current_monitor.current_value1;
}

int m_callable_current_calibration(void)
{
    /** 最后采样得到的电流值  */
    float current_val = 0;

    if (current_monitor.current_value1_averge < 800 && !current_monitor.m_global_short_interrupt)
    {
        for (int i = 0; i < 60; i++)
        {
            current_val += m_callable_current_check_get_max_value();
            current_monitor.current_monitor_flag = 1;
        }
        current_val = current_val / 60;
        DEBUG_E("*-*******m_callable_display_status_get()  %d", m_callable_display_status_get());
        if (current_monitor.current_calibration_interrupt == true || m_callable_display_status_get() == M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE || m_callable_display_status_get() == M_DISPLAY_VT_SHORT_CIRCUIT_MODE)
        {
            current_monitor.current_calibration_interrupt = false;
            current_val = MAX_SHORT_CURRENT;
        }
    }
    else
    {
        current_val = current_monitor.current_value1_averge;
    }
    return current_val;
}
int mm = 0;
/**
 * @brief 获取到检测的电流值
 *
 * @return float
 */
int m_callable_current_check_get_final_value(void)
{

    int current_result = 0;
    // DEBUG_E("*******************%d            %d",current_monitor.current_value1_averge,current_monitor.current_value1_count);
    if (current_monitor.current_value1_count > 0)
    {
        DEBUG_TEST(DB_I, "current_monitor.current_value1_count %d ", current_monitor.current_value1_count);
        DEBUG_TEST(DB_I, "current_monitor.current_value1_averge %d ", current_monitor.current_value1_averge);
        current_result = current_monitor.current_value1_averge / (current_monitor.current_value1_count);
    }
    // DEBUG_TEST( DB_I,"current_monitor.current_value1_count %d ", current_monitor.current_value1_count);
    if (current_monitor.current_value1_count == 0)
    {
        current_result = -1;
    }
    current_monitor.current_value1_averge = 0;
    current_monitor.current_value1_count = 0;
    current_monitor.first_time = m_callable_timer_manage_get_device_time();

    // DEBUG_TEST(DB_I, "current_resul tcurrent_result %d ", current_result);
    return current_result;
}

stat_m m_callable_current_get_voltage(float *max_voltage_get, float *min_voltage_get)
{
    float voltage_float; // 定义一个电压值变量

    int adc_value = m_ext_drive_read_adc_value();
    voltage_float = m_ext_drive_adc_to_voltage(adc_value, &voltage_float);
    *max_voltage_get = (*max_voltage_get > voltage_float) ? *max_voltage_get : voltage_float;

    *min_voltage_get = (*min_voltage_get < voltage_float) ? *min_voltage_get : voltage_float;
    // DEBUG_E("*max_voltage_get %f   *min_voltage_get %f",*max_voltage_get,*min_voltage_get);
    return succ_r;
}
float m_callable_volatage_transform_current(void)
{
    current_monitor.max_voltage_output = current_monitor.max_voltage;
    current_monitor.min_voltage_output = current_monitor.min_voltage;

    current_monitor.max_voltage = 0.0;
    current_monitor.min_voltage = 99999.9;

    current_monitor.max_current = (current_monitor.max_voltage_output - VCC / 2.0) / AD / PRECISION * 1000;
    current_monitor.min_current = (current_monitor.min_voltage_output - VCC / 2.0) / AD / PRECISION * 1000;
    current_monitor.avg_current = (current_monitor.max_current - current_monitor.min_current) / 2.0;
    return current_monitor.avg_current;
}
// void m_ext_current_monitor_result(enum current_event_id event_id, int current_vue, uint8_t is_interrupt, uint8_t channel_id);
/**
 * @brief 查询短路是否
 *
 * @return stat_m  succ 短路
 */
stat_m m_callable_current_is_interrupt()
{
    if (current_monitor.m_global_short_interrupt)
    {
        current_monitor.m_global_short_interrupt = false;
        return succ_r;
    }
    else
        return fail_r;
}
/**
 * @brief 高电流检测处理
 *
 * @return stat_m
 */
stat_m m_static_height_current_handle(int in_current_vue, int evt_id, uint64_t in_time_s)
{
    // DEBUG_TEST( DB_I," - - - -%d", in_current_vue);
    if (in_current_vue > evt_id)
    {
        current_monitor.current_monitor_flag = false;

        m_ext_current_monitor_result(M_CURRENT_EVENT_VALUE_OUT, in_current_vue, current_monitor.cur_short_mode,
                                     current_monitor.m_global_short_interrupt_zone, in_time_s);
    }
    DEBUG_TEST(DB_I, "End Current 3[%d] ..", in_current_vue);
    return succ_r;
}
stat_m m_static_current_interrupt_handle()
{
    if (current_monitor.m_global_short_interrupt &&
        ((interrupt_short_invalid_count > 0 && interrupt_short_invalid_count < 50) || interrupt_short_invalid_count > 400))
    // if (m_ext_drive_gpio_get_level(19)==0)
    // if (current_monitor.m_global_short_interrupt && current_monitor.current_monitor_flag)
    {
        // DEBUG_TEST(DB_W,"&& m_ext_drive_gpio_get_level(19)==0 %d" ,  m_ext_drive_gpio_get_level(19));
        switch (current_monitor.cur_short_mode)
        {
        case M_CURRENT_SHORT_MODE_CALIBRATION:
        case M_CURRENT_SHORT_MODE_PUMP:
        case M_CURRENT_SHORT_MODE_SIGN_ZONE:
            m_callable_solenoid_manage_M_close();
            // m_ext_drive_solenoid_close_all();
            m_ext_drive_hc595_shiftout_reset();
            m_callable_drive_io_pin12_uninit();

            DEBUG_TEST(DB_I, "Short Point (%d)!", m_ext_drive_gpio_get_level(15));
            if (current_monitor.current_value1_averge == MAX_SHORT_CURRENT)
                return succ_r;
            current_monitor.current_monitor_flag = false;
            current_monitor.current_value1_averge = MAX_SHORT_CURRENT;
            current_monitor.current_value1_count = 1;
            break;
        case M_CURRENT_SHORT_MODE_WATER_HAMMER:
            DEBUG_TEST(DB_I, "Short Hammer !");
            current_monitor.current_monitor_flag = false;
            m_ext_drive_solenoid_direct_open_and_close(current_monitor.m_global_short_interrupt_zone, 0);
            m_callable_drive_io_pin12_uninit();
            break;
        case M_CURRENT_SHORT_MODE_VT:
            DEBUG_TEST(DB_I, "Short VT !");
            m_callable_drive_io_pin12_uninit();
            break;

        default:
            break;
        }
        current_monitor.current_monitor_flag = false;
        // m_callable_solenoid_manage_close_all();
        current_monitor.m_global_short_interrupt = false;
        // current_monitor.current_value1 = interrupt_short_invalid_count;
        // if (current_monitor.cur_short_mode == M_CURRENT_SHORT_MODE_SIGN_ZONE)
        // {
        m_ext_current_monitor_result(M_CURRENT_EVENT_INTERRUPT, interrupt_short_invalid_count, current_monitor.cur_short_mode,
                                     current_monitor.m_global_short_interrupt_zone, 0);
        // }
        // else if (current_monitor.cur_short_mode == M_CURRENT_SHORT_MODE_PUMP)
        // {
        //     m_callable_display_status(M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE, 0);
        // }
        DEBUG_TEST(DB_W, "Short Circuit  >  ???? Mode %d  @ %d interrupt_short_invalid_count: %d", current_monitor.cur_short_mode,
                   current_monitor.m_global_short_interrupt_zone,
                   interrupt_short_invalid_count);
        current_monitor.m_global_short_interrupt = false;
        interrupt_short_invalid_count = 0;
    }
    // else
    //     current_monitor.m_global_short_interrupt = false;
    // {
    //     m_callable_drive_io_pin12_uninit();
    // }
    return succ_r;
}

stat_m m_calllable_current_monitor(int device_status)
{
    // DEBUG_E("device_statusdevice_status %d ", device_status);

    if (current_monitor.current_monitor_flag)
    {
        m_callable_device_proper_status_get(&device_status);
        switch (device_status)
        {
        case M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING:
        case M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING:
        case M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING:

            // if (current_monitor.first_time < m_callable_timer_manage_get_device_time() - 2)
            if (m_callable_timer_manage_get_device_time() > current_monitor.first_time + 2)
            {
                if (current_monitor.current_value1_count == 0)
                {
                    current_monitor.first_time = m_callable_timer_manage_get_device_time();
                    DEBUG_TEST(DB_E, "Start Current 3 ..");

                    for (int i = 0; i < 60; i++)
                    {
                        current_monitor.current_value1_count++;
                        current_monitor.current_value1_averge += m_callable_current_check_get_max_value();
                    }
                    if (current_monitor.current_value1_count != 0)
                    {
                        /** 高电流检测 */
                        m_static_height_current_handle(current_monitor.current_value1_averge / current_monitor.current_value1_count, MAX_LIMIT_CURRENT_2S, m_callable_timer_manage_get_device_time());
                        // DEBUG_TEST(DB_I, "End Current 3[%d] ..", current_monitor.current_value1_averge / current_monitor.current_value1_count);
                    }
                }
            }
            if (m_callable_timer_manage_get_device_time() > current_monitor.first_time + 8)
            {
                DEBUG_TEST(DB_I, "Start Current 9 ..");
                current_monitor.first_time = m_callable_timer_manage_get_device_time();
                for (int i = 0; i < 10; i++)
                {
                    current_monitor.current_value1_count++;
                    current_monitor.current_value1_averge += m_callable_current_check_get_max_value();
                }
                if (current_monitor.current_value1_count != 0)
                {
                    /** 高电流检测 */           
                    m_static_height_current_handle(current_monitor.current_value1_averge / current_monitor.current_value1_count, MAX_LIMIT_CURRENT_10S, m_callable_timer_manage_get_device_time());
                    // DEBUG_TEST(DB_I, "End Current 9[%d] ..", current_monitor.current_value1_averge / current_monitor.current_value1_count);
                }
            }

            break;
            //  case M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING:
            //             current_monitor.first_time = m_callable_timer_manage_get_device_time();
            //             current_monitor.current_value1_count++;
            //             current_monitor.current_value1_averge += m_callable_current_check_get_max_value();
            // break;
        default:
            break;
        }
    }
    return succ_r;
}

/**
 * @brief 电流检测人物
 *
 * @param pvParameters
 */
void m_calllable_current_monitor_task(void *pvParameters)
{
    while (1)
    {
        m_static_current_interrupt_handle();
        // mDelay_ms(15);
        mDelay_ms(15);
    }
}
/**
 * @brief 根据传入模式调整在短路发生时，对应的处理
 *
 * @param in_csm 模式
 * @return stat_m
 */
stat_m m_callable_current_set_short_mode(enum current_short_mode in_csm)
{
    current_monitor.cur_short_mode = in_csm;
    // DEBUG_TEST( DB_I,"%d" , current_monitor.cur_short_mode  );
    return succ_r;
}
/**
 * @brief 电流中断短路通知
 *
 */
void m_callable_current_out_short_interrupt(void)
{
    // if (m_ext_drive_gpio_get_level(19) == 0)
    current_monitor.m_global_short_interrupt = true;
    interrupt_short_invalid_count++;
}

stat_m m_callable_current_calibration_channel_select(uint8_t channel)
{
    current_monitor.channel = channel;
    return succ_r;
}

stat_m m_callable_current_calibration_interrupt_processing(void)
{
    current_monitor.current_calibration_interrupt = true;
    m_callable_display_status(M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE, current_monitor.channel);
    return succ_r;
}

/*批量区域电流校准标志位设置 0表示关闭、1表示电流校准 、2表示电流测试*/
stat_m m_callable_current_batch_area_power_calibration_flag_set(int flag)
{
    area_power_calibration_flag = flag;
    return succ_r;
}

/*批量区域电流校准标志位获取 0表示关闭、1表示电流校准 、2表示电流测试*/
int m_callable_current_batch_area_power_calibration_flag_get(void)
{
    return area_power_calibration_flag;
}