

#include "core/fset_lib.h"

#include "../drive.h"

// CONFIG_RGB_LED_RANK=50
// CONFIG_IIC_SCL_PIN=17
// CONFIG_IIC_SDA_PIN=16
// CONFIG_IIC_FREQ=20000
#define GPIO_KEY_PLAY 26
#define GPIO_KEY_PAUSE 27
#define GPIO_KEY_LEFT 32
#define GPIO_KEY_RIGHT 33
#define GPIO_MAIN_RESET 34
#define GPIO_SHORT_PIN 19

#define PULSE_INPUT_PIN_SENSOR1 13
#define PULSE_INPUT_PIN_SENSOR2 14

// # CONFIG_OTA_USE_HTTPS is not set
// CONFIG_HTTP_NETWORK_TEST_URL="http://192.168.5.247"
// CONFIG_TCP_SOCK_ADDR="192.168.5.247"
// CONFIG_TCP_SOCK_PORT=7788
// CONFIG_OTA_URL_HTTP="http://192.168.5.247:5566/hot_so_fast.bin"
// # CONFIG_HC595 is not set
// CONFIG_PW554_AHH5=y
// # CONFIG_PW554_other is not set
// CONFIG_HC595_OE=25
// CONFIG_HC595_DIN=18
// CONFIG_HC595_OUT=2
// CONFIG_HC595_CLK=4
#if !UNIT_TEST
void IRAM_ATTR gpio_isr_handler(void *arg);
#endif

// void m_static_sensor_1_init_setup(void)
// {
//     // 配置 GPIO 引脚为输入模式，并设置为上升沿中断
//     gpio_config_t io_conf = {};
//     io_conf.intr_type = GPIO_INTR_POSEDGE;
//     io_conf.mode = GPIO_MODE_INPUT;
//     io_conf.pin_bit_mask = (1ULL << PULSE_INPUT_PIN_SENSOR1);
//     io_conf.pull_up_en = 1;
//     gpio_config(&io_conf);
// }

// void m_static_sensor_2_init_setup(void)
// {
//     // 配置 GPIO 引脚为输入模式，并设置为上升沿中断
//     gpio_config_t io_conf = {};
//     io_conf.intr_type = GPIO_INTR_POSEDGE;
//     io_conf.mode = GPIO_MODE_INPUT;
//     io_conf.pin_bit_mask = (1ULL << PULSE_INPUT_PIN_SENSOR2);
//     io_conf.pull_up_en = 1;
//     gpio_config(&io_conf);
// }

stat_m m_ext_drive_button_gpio_and_interrupt_init(void)
{
    /**
        h-typedef enum {
            GPIO_PIN_INTR_DISABLE = 0,
            GPIO_PIN_INTR_POSEDGE = 1,
            GPIO_PIN_INTR_NEGEDGE = 2,
            GPIO_PIN_INTR_ANYEDGE = 3,
            GPIO_PIN_INTR_LOLEVEL = 4,
            GPIO_PIN_INTR_HILEVEL = 5
        }GPIO_INT_TYPE;
    *
 */
    stat_m stat = fail_r;
#if !UNIT_TEST
    GPIO_INIT(GPIO_MODE_INPUT, GPIO_KEY_PLAY, 1, GPIO_INTR_ANYEDGE);
    GPIO_INIT(GPIO_MODE_INPUT, GPIO_KEY_PAUSE, 1, GPIO_INTR_ANYEDGE);
    GPIO_INIT(GPIO_MODE_INPUT, GPIO_KEY_LEFT, 1, GPIO_INTR_ANYEDGE);
    GPIO_INIT(GPIO_MODE_INPUT, GPIO_KEY_RIGHT, 1, GPIO_INTR_ANYEDGE);
    // GPIO_INIT(GPIO_MODE_INPUT, GPIO_KEY_LEFT, 0, GPIO_PIN_INTR_NEGEDGE); GPIO_INTR_POSEDGE
    // GPIO_INIT(GPIO_MODE_INPUT, GPIO_KEY_RIGHT, 0, GPIO_PIN_INTR_NEGEDGE);

    GPIO_INIT(GPIO_MODE_INPUT, GPIO_MAIN_RESET, 0, GPIO_PIN_INTR_NEGEDGE);
    DEBUG_TEST(DB_W, "Short Pin :%d", m_ext_drive_gpio_get_level(GPIO_SHORT_PIN));
    if (m_ext_drive_gpio_get_level(GPIO_SHORT_PIN) == 1)
    {
        GPIO_INIT(GPIO_MODE_INPUT, GPIO_SHORT_PIN, 1, GPIO_INTR_LOW_LEVEL); // GPIO_INTR_HIGH_LEVEL GPIO_INTR_LOW_LEVEL
    }
    else
    {
        GPIO_INIT(GPIO_MODE_INPUT, GPIO_SHORT_PIN, 1, GPIO_PIN_INTR_NEGEDGE);
    } // m_static_sensor_1_init_setup();
    // m_static_sensor_1_init_setup();
    gpio_pullup_en(GPIO_KEY_PLAY);
    gpio_pullup_en(GPIO_KEY_LEFT);
    gpio_pullup_en(GPIO_KEY_RIGHT);
    gpio_pullup_en(GPIO_KEY_PAUSE);
    gpio_pullup_en(GPIO_SHORT_PIN);

    // gpio_pullup_en(GPIO_MAIN_RESET);

    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL2);

    gpio_isr_handler_add(GPIO_SHORT_PIN, gpio_isr_handler, (void *)GPIO_SHORT_PIN);
    gpio_isr_handler_add(GPIO_KEY_PLAY, gpio_isr_handler, (void *)GPIO_KEY_PLAY);

    gpio_isr_handler_add(GPIO_KEY_LEFT, gpio_isr_handler, (void *)GPIO_KEY_LEFT);
    gpio_isr_handler_add(GPIO_KEY_RIGHT, gpio_isr_handler, (void *)GPIO_KEY_RIGHT);

    gpio_isr_handler_add(GPIO_KEY_PAUSE, gpio_isr_handler, (void *)GPIO_KEY_PAUSE);

    gpio_isr_handler_add(PULSE_INPUT_PIN_SENSOR1, gpio_isr_handler, (void *)PULSE_INPUT_PIN_SENSOR1);
    gpio_isr_handler_add(PULSE_INPUT_PIN_SENSOR2, gpio_isr_handler, (void *)PULSE_INPUT_PIN_SENSOR2);

    // gpio_isr_handler_add(GPIO_MAIN_RESET, gpio_isr_handler, (void *)GPIO_MAIN_RESET);
#endif
    return stat;
}
uint32_t gpio_num = 0;

#if !UNIT_TEST
// uint8_t ios = 0;
void IRAM_ATTR gpio_isr_handler(void *arg)
{
    gpio_num = (uint32_t)arg;
    // Variables_Interrupt_Animation = true;
    switch (gpio_num)
    {
    case GPIO_MAIN_RESET:
        m_callable_drive_button_event_input(M_KEY_EVENT_RESET);
        break;
    case GPIO_KEY_LEFT:
        m_callable_drive_button_event_input(M_KEY_EVENT_LEFT);

        break;
    case GPIO_KEY_RIGHT:
        m_callable_drive_button_event_input(M_KEY_EVENT_RIGHT);

        break;
    case GPIO_KEY_PLAY:
        m_callable_drive_button_event_input(M_KEY_EVENT_START);
        break;
    case GPIO_KEY_PAUSE:
        m_callable_drive_button_event_input(M_KEY_EVENT_STOP);
        break;
    case GPIO_SHORT_PIN:
        m_callable_current_out_short_interrupt();
        break;
    case PULSE_INPUT_PIN_SENSOR1:
        m_callable_sensor_flow_value_interrupt(1);
        break;
    case PULSE_INPUT_PIN_SENSOR2:
        m_callable_sensor_flow_value_interrupt(2);
        //
        // m_callable_sensor_flow_value_interrupt();
        // m_global_short_interrupt = true;
        // printf("lplp");
        break;
    }
}

#endif
/**
 * @brief  按键事件处理
 *
 * @param key_id 按键id
 * @param pre_key_index 操作前的按键id
 * @param current_key_index 操作后的按键id
 * @param current_time_ms 当前的时间
 * @return stat_m
 */
stat_m m_ext_drive_button_event_handle(enum key_id key_id, enum key_event key_event, uint8_t pre_key_index, uint8_t current_key_index, uint64_t current_time_ms)
{
    int out_ste;
    stat_m stat = succ_r;
    m_callable_device_proper_status_get(&out_ste);
    current_time_ms = m_callable_timer_manage_get_device_time();
    DEBUG_TEST(DB_I, "%s   (%hhd)  %d device state: %d", toname(key_id), pre_key_index, current_key_index, out_ste);
    if ((out_ste == M_DEVICE_GLOBAL_STATUS_UPDATEING || out_ste == M_DEVICE_GLOBAL_STATUS_INIT))
    {
        stat = fail_r;
    }
    // if(m_ext_drive_gpio_get_level(key_event) == 0  )
    // {
    // stat = fail_r;
    // }
    if((m_callable_manual_get_sol_even() == M_OPERATE_EVENT_RUNNING_SWITCH_ALL) && key_id != M_KEY_EVENT_STOP)
    {
        stat = fail_r;
    }
    if (stat == succ_r)
    {
        switch (out_ste)
        {
        case M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING:
            m_callable_instance_running_event_input(key_id, current_time_ms);
            break;
        case M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING:
            if (m_callable_display_status_get() == M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE && (key_id == M_KEY_EVENT_LEFT || key_id == M_KEY_EVENT_RIGHT))
                break;
            if(key_event == M_KEY_EVENT_LONG_PRESS)
                break;
            m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, current_key_index);
            m_callable_manual_event_input(key_id, pre_key_index, current_key_index, current_time_ms);
            break;
        case M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING:
            if (key_id == M_KEY_EVENT_STOP)
                m_callable_instance_running_event_input(key_id, current_time_ms);

            break;
        case M_DEVICE_GLOBAL_STATUS_TEST_RUNNING:
            break;

        case M_DEVICE_GLOBAL_STATUS_CONFIG_NER:

            break;
        case M_DEVICE_GLOBAL_STATUS_IDLE:
            if (key_id == M_KEY_EVENT_WATERED_ALL && m_callable_display_status_get() != M_DISPLAY_ZONE_SELECT_MODE)
            {
                m_callable_manual_event_input(key_id, pre_key_index, current_key_index, current_time_ms);
            }
            if (key_id == M_KEY_EVENT_START && m_callable_display_status_get() != M_DISPLAY_IDLE_STATUS_MODE && key_event != M_KEY_EVENT_LONG_PRESS)
            {
                m_callable_manual_event_input(key_id, pre_key_index, current_key_index, current_time_ms);
            }   
            else if (key_id == M_KEY_EVENT_STOP)
            {
                m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, current_key_index);
                m_callable_manual_event_input(key_id, pre_key_index, current_key_index, current_time_ms);
                m_callable_drive_io_pin12_init();
            }
            else if (key_id != M_KEY_EVENT_START && key_id != M_KEY_EVENT_RESET)
            {
                m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, current_key_index);
            }
            // }else if (key_id == M_KEY_EVENT_START && m_callable_display_status_get() == M_DISPLAY_ZONE_SELECT_MODE)
            // {
            //     printf("4\n");
            //     m_callable_manual_event_input(key_id, pre_key_index, current_key_index, current_time_ms);
            // }
            break;
        default:
            m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, current_key_index);
            break;
        }
    }
    if (key_id == M_KEY_EVENT_RESET)
    {
        // m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
        if (key_event == M_KEY_EVENT_LONG_PRESS)
        {
            // m_callable_device_attribute_all_clear();
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_SELF_BE_DELETE, M_TYPE_NULL,
                                            NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, current_time_ms, true);

            m_callable_drive_flash_remove("time");
            m_callable_device_attribute_running_save_mode(M_DEVICE_RUNNING_MODE_NONE);
            if (m_callable_offline_bluetooth_login_flag() == false)
            {
                m_callable_middle_network_adapter_clear();
            }
            mDelay_ms(1000);
            mReboot(M_RESTART_CAUSE_PREMEDITATED);
        }
    }

    return stat;
}

// stat_m m_ext_drive_button_reset_trig(uint64_t in_count)
uint64_t presc = 0;
stat_m m_ext_drive_button_reset_trig(uint64_t in_count)
{
    stat_m stat = fail_r;
#if !UNIT_TEST
    // DEBUG_TEST( DB_I,"On %d" , gpio_get_level(M_KEY_EVENT_RESET));
    if (gpio_get_level(GPIO_MAIN_RESET) == 0 && presc == 0 && in_count > 150)
    {
        presc = in_count;
        DEBUG_TEST(DB_I, "One ");
    }
    else
    {
        if (in_count > presc + 300 && presc > 0)
        {
            DEBUG_TEST(DB_I, "BUG   bUG     bug  fbhu ");
            stat = succ_r;
            presc = 0;
        }
        // else
        if (gpio_get_level(GPIO_MAIN_RESET) == 1)
            presc = 0;
    }
#endif
    return stat;
}