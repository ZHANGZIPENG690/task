#include "display_manage.h"

uint8_t rotational_speed = 85;
uint8_t ota_updata_light_flag_m = 1;

enum light_RGB_color rotational_status = greed;
enum light_RGB_color *rotational_status_set = &rotational_status;
enum light_anima_mode g_state = 0;        // 状态变量   共有15种状态
enum light_anima_mode g_state_before = 0; // 状态变量   共有15种状态

stat_m m_callable_light_display_initial(uint32_t _device_version_set1, uint8_t number_of_channels1, uint8_t light_strength1, uint8_t increment_lighting1)
{
    stat_m sta = fail_r;
    display_attribute.display_device_version = _device_version_set1;    // 新板子RGB灯布局       //旧板子  0xa002
    display_attribute.display_number_of_channels = number_of_channels1; // 通路个数
    display_attribute.display_lighting_strength = light_strength1;
    display_attribute.display_increment_lighting = increment_lighting1;
    display_attribute.display_set_light_close = 1;

    if (light_strength1 == 0)
    {
        display_attribute.display_lighting_strength = 245;
        display_attribute.display_set_light_close = 0;
    }
    if (_device_version_set1 == DEVICE_HEARWARE_A003 || _device_version_set1 == DEVICE_HEARWARE_B11 || _device_version_set1 == DEVICE_HEARWARE_B12)
    {
        display_attribute.display_numLeds = 33; // 灯的数量 用于计算更新百分比
        display_attribute.display_last_light_id = 9;
    }
    else if (_device_version_set1 == DEVICE_HEARWARE_C11)
    {
        display_attribute.display_numLeds = 19; // 灯的数量 用于计算更新百分比
        display_attribute.display_last_light_id = 4;
    }
    else if (_device_version_set1 == DEVICE_HEARWARE_D11)
    {
        display_attribute.display_numLeds = 9; // 灯的数量 用于计算更新百分比
        display_attribute.display_last_light_id = 3;
    }
    else
    {
        display_attribute.display_lighting_strength = 90;
        display_attribute.display_numLeds = 23; // 灯的数量 用于计算更新百分比
        display_attribute.display_last_light_id = 6;
    }
    if (display_attribute.display_lighting_strength == 30)
    {
        display_attribute.brightness_step = 1;
    }
    else if (display_attribute.display_lighting_strength == 90)
    {
        display_attribute.brightness_step = 3;
    }
    else
    {
        display_attribute.brightness_step = 6;
    }
    display_attribute.dispaly_flicker_speed = 500; // 闪烁频率  50
    m_ext_drive_lighting_IIC_init();
    sta = m_ext_drive_lighting_init();
    mTaskCreate(NULL, m_static_display_run, "StateTask", 2048, NULL, 2, NULL);
    DEBUG_TEST_W("Led Num: %d Hearware Version: %d Channel Num: %d", display_attribute.display_numLeds, _device_version_set1, number_of_channels1);
    return sta;
}
/**
 * 开机模式和找网络模式共用 M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE , param=1为绿色旋转，parm=2为紫色旋转
 */
stat_m m_callable_display_status(enum light_anima_mode light_mode, uint8_t param)
{
    stat_m sta = fail_r;
    if (light_mode != M_DISPLAY_FIRMWARE_UPDATE_MODE)
        DEBUG_TEST_W("Light Change %d", light_mode);

    if (light_mode >= M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE && light_mode <= M_DISPLAY_SET_MANUAL_DURATION)
    {
        sta = succ_r;
        if (ota_updata_light_flag_m == 1)
        {
            g_state = light_mode;
        }
        else
        {
            g_state = M_DISPLAY_FIRMWARE_UPDATE_MODE;
        }

        if (light_mode == M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE && param == 1)
        {
            *rotational_status_set = 0x00ff00; // 绿色
        }
        if (light_mode == M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE && param == 2)
        {
            *rotational_status_set = 0x800080; // 紫色
        }
        if (light_mode == M_DISPLAY_FIRMWARE_UPDATE_MODE)
        {
            display_attribute.display_ota_renewal_percent = param;
        }
        if (light_mode == M_DISPLAY_ZONE_RUNNING_MODE || light_mode == M_DISPLAY_ZONE_SELECT_MODE || light_mode == M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE)
        {
            display_attribute.display_zone_select = param;
        }
        if (light_mode == M_DISPLAY_SET_ZONE_MODE)
        {
            display_attribute.display_number_of_channels = param;
        }
        if (light_mode == M_DISPLAY_SET_DEVICE_VERSION_MODE)
        {
            display_attribute.display_device_version = param;
        }
        if (light_mode == M_DISPLAY_SET_MANUAL_DURATION)
        {
            display_attribute.manual_setting_light_num = param;
        }
    }
    return sta;
}

void m_callable_light_clean(void)
{
    if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B12 || display_attribute.display_device_version == DEVICE_HEARWARE_B11)
    {
        display_attribute.display_numLeds = 33;
    }
    else if (display_attribute.display_device_version == DEVICE_HEARWARE_C11)
    {
        display_attribute.display_numLeds = 19;
    }
    else if (display_attribute.display_device_version == DEVICE_HEARWARE_D11)
    {
        display_attribute.display_numLeds = 9;
    }
    else
    {
        display_attribute.display_numLeds = 23;
    }
    for (uint8_t i = 0; i < display_attribute.display_numLeds; i++)
    {
        m_ext_rgb_pin_1(i, 0, 0, 0);
    }
}

uint8_t start_index = 0;
uint8_t led_index = 0;
uint8_t black_index = 0;

void m_callable_display_start_up(void)
{
    uint8_t flag = 0;
    // 初始化灯的初始颜色
    m_callable_light_clean();
    // 开始动态转动
    while (1)
    {
        if (display_attribute.display_set_light_close == 0 && rotational_status == 0x800080)
        {
            if (flag == 0)
            {
                flag = 1;
                m_callable_light_clean();
            }
            mDelay_ms(300);
        }
        else
        {
            // 设置灯的颜色
            for (uint8_t i = 0; i < display_attribute.display_last_light_id; i++)
            {
                led_index = (start_index + i) % (display_attribute.display_numLeds);
                m_ext_drive_lighting_set_point_color(led_index, rotational_status);
                mDelay_ms(rotational_speed);
                black_index = (led_index - display_attribute.display_last_light_id + (display_attribute.display_numLeds)) % (display_attribute.display_numLeds);
                m_ext_drive_lighting_set_point_color(black_index, black);
            }
            mDelay_ms(15);
            start_index = (start_index + display_attribute.display_last_light_id) % (display_attribute.display_numLeds);
        }
        if (g_state != M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE)
        {
            break;
        }
    }
}
/*空闲状态*/
void m_callable_display_idle_status(void)
{
    m_callable_light_clean();
    while (1)
    {
        if (display_attribute.display_set_light_close == 0)
        {
            mDelay_ms(500);
        }
        else
        {
            m_static_breathe_around(MY_COLOR_CONCOC(blue), display_attribute.brightness_step);
        }
        mDelay_ms(2);
        if (g_state != g_state_before)
            break;
    }
}

/*区域运行状态*/
void m_callable_display_zone_running(void)
{
    m_callable_light_clean();
    while (1)
    {
        m_static_breathe(MY_COLOR_CONCOC(white), display_attribute.brightness_step);

        if (g_state != M_DISPLAY_ZONE_RUNNING_MODE)
            break;
    }
}

/*区域选择状态*/
void m_callable_display_zone_select(void)
{
    m_callable_light_clean();
    while (1)
    {

        m_static_not_breathe(MY_COLOR_CONCOC(white));

        if (g_state != M_DISPLAY_ZONE_SELECT_MODE)
            break;
        mDelay_ms(20);
    }
}

// 连接wifi 阶段1
void m_callable_display_wifi_config_status1(void)
{
    m_callable_light_clean();
    while (1)
    {

        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 1, blue, greed, true);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }

        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 1, blue, greed, false);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }

        if (g_state != g_state_before)
            break;
    }
}

// 连接wifi 阶段2
void m_callable_display_wifi_config_status2(void)
{
    while (1)
    {

        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 2, blue, greed, true);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }

        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 2, blue, greed, false);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }
        if (g_state != g_state_before)
            break;
    }
}

// 正在连接服务器
void m_callable_display_connecting_to_server(void)
{
    while (1)
    {
        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 3, blue, greed, true);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }

        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 3, blue, greed, false);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }
        if (g_state != g_state_before)
            break;
    }
}

// 配网成功
void m_callable_display_distribution_network_success(void)
{
    // DEBUG_TEST("设备配网成功器动画...\r\n");
    m_callable_light_clean(); // 清除灯的初始颜色
    while ((1))
    {
        if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B12 || display_attribute.display_device_version == DEVICE_HEARWARE_B11)
        {
            for (uint8_t i = 1; i < display_attribute.display_numLeds; i++)
            {
                if ((i >= 5 && i <= 13) || (i >= 18 && i <= 23) || (i >= 27 && i <= 32))
                {
                    m_ext_drive_lighting_set_point_color(i, greed);
                }
            }
        }
        else if (display_attribute.display_device_version == DEVICE_HEARWARE_C11)
        {
            for (uint8_t i = 1; i < display_attribute.display_numLeds; i++)
            {
                if ((i >= 1 && i <= 4) || (i >= 6 && i <= 9) || (i >= 12 && i <= 16))
                {
                    m_ext_drive_lighting_set_point_color(i, greed);
                }
            }
        }
        else if (display_attribute.display_device_version == DEVICE_HEARWARE_D11)
        {
            for (uint8_t i = 1; i < display_attribute.display_numLeds; i++)
            {
                if ((i >= 1 && i <= 2) || (i >= 4 && i <= 5) || (i >= 7 && i <= 8))
                {
                    m_ext_drive_lighting_set_point_color(i, greed);
                }
            }
        }
        else
        {
            for (uint8_t i = 1; i < display_attribute.display_numLeds; i++)
            {
                if ((i >= 4 && i <= 9) || (i >= 13 && i <= 16) || (i >= 19 && i <= 22))
                {
                    m_ext_drive_lighting_set_point_color(i, greed);
                }
            }
        }
        mDelay_ms(5);
        if (g_state != M_DISPLAY_DISTRIBUTION_NETWORK_SUCCESS_MODE)
            break;
    }
}

void m_callable_display_wifi_connet_fail(void)
{
    // m_callable_light_clean();
    while (1)
    {
        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 1, blue, yellow, true);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }

        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 1, blue, yellow, false);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }
        if (g_state != g_state_before)
            break;
    }
}

void m_callable_display_wifi_no_network(void)
{

    // m_callable_light_clean();

    while (1)
    {
        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 2, blue, yellow, true);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }

        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 2, blue, yellow, false);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }
        if (g_state != g_state_before)
            break;
    }
}

void m_callable_display_server_connet_fail(void)
{
    // m_callable_light_clean();
    while (1)
    {
        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 3, blue, yellow, true);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }

        if (g_state != g_state_before)
            break;
        m_static_localized_light_flashing(display_attribute.display_device_version, display_attribute.display_numLeds, 3, blue, yellow, false);

        for (uint8_t i = 0; i <= 7; i++)
        {
            mDelay_ms(display_attribute.dispaly_flicker_speed / 8);
            if (g_state != g_state_before)
                break;
        }
        if (g_state != g_state_before)
            break;
    }
}

void m_callable_display_network_problem(void)
{
    m_callable_light_clean();
    while (1)
    {
        if (display_attribute.display_set_light_close == 0)
        {
            mDelay_ms(300);
        }
        else
        {
            m_static_breathe_around(MY_COLOR_CONCOC(yellow), display_attribute.brightness_step);
        }
        mDelay_ms(5);
        if (g_state != M_DISPLAY_NETWORK_PROBLEM_MODE)
        {
            break;
        }
    }
}

void m_callable_display_wifi_disconnection(void)
{

    m_callable_light_clean();
    while (1)
    {
        if (display_attribute.display_set_light_close == 0)
        {
            mDelay_ms(300);
        }
        else
        {
            m_static_breathe_around(MY_COLOR_CONCOC(purple), display_attribute.brightness_step);
        }
        mDelay_ms(5);
        // 检查状态是否改变，如果改变则退出循环
        if (g_state != M_DISPLAY_WIFI_DISCONNECTION_MODE)
        {
            break;
        }
    }
}

void m_callable_display_regional_short_circuit(void)
{
    m_callable_light_clean();
    while (1)
    {
        m_static_breathe(MY_COLOR_CONCOC(blue), display_attribute.brightness_step);
        if (g_state != M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE)
            break;
    }
}

void m_callable_display_abnormal_feedback(void)
{
    m_callable_light_clean();
    while (1)
    {
        for (uint8_t i = 0; i <= (display_attribute.display_numLeds); i++)
        {
            if (i >= 0 && i <= (display_attribute.display_numLeds))
            {
                m_ext_drive_lighting_set_point_color(i, blue);
            }
        }

        if (g_state != M_DISPLAY_ABNORMAL_FEEDBACK_MODE)
            break;
        mDelay_ms(20);
    }
}

void m_callable_display_firmware_update(void)
{
    // DEBUG_TEST("OTA更新 动画...\r\n");
    uint8_t brightness_after = MIN_BRIGHTNESS; // 初始亮度
    uint8_t light_strength1 = display_attribute.display_lighting_strength;
    uint8_t light_strength2 = 0;
    int increment_lighting = 3;
    m_callable_light_clean();
    bool isOn = true; // 用于记录闪烁状态
    m_ext_drive_lighting_set_point_color(1, 0x1515);
    ota_updata_light_flag_m = 0;
    uint32_t adjustedColor = MY_COLOR_CONCOC(0x1515);
    //  提取调整后的颜色通道值
    uint8_t redchannel;
    uint8_t greenchannel;
    uint8_t bluechannel;
    while (1)
    {

        if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B12 || display_attribute.display_device_version == DEVICE_HEARWARE_B11 || display_attribute.display_device_version == DEVICE_HEARWARE_C11)
        {
            //  提取调整后的颜色通道值
            redchannel = (adjustedColor * brightness_after >> 16) & 0xFF;
            greenchannel = (adjustedColor * brightness_after >> 8) & 0xFF;
            bluechannel = adjustedColor * brightness_after & 0xFF;
        }
        else
        {
            //  提取调整后的颜色通道值
            bluechannel = (adjustedColor * brightness_after >> 16) & 0xFF;
            greenchannel = (adjustedColor * brightness_after >> 8) & 0xFF;
            redchannel = adjustedColor * brightness_after & 0xFF;
        }

        // 控制呼吸灯的亮度变化
        light_strength2 = light_strength1;
        light_strength1 = display_attribute.display_lighting_strength;
        brightness_after += increment_lighting;
        if (display_attribute.display_lighting_strength != 0)
        {
            if (brightness_after <= MIN_BRIGHTNESS || brightness_after >= light_strength1)
            {
                increment_lighting = -increment_lighting;
                if (light_strength2 > light_strength1)
                {
                    increment_lighting = -increment_lighting;
                }
            }
        }
        if (display_attribute.display_lighting_strength < light_strength2)
        {
            brightness_after = MIN_BRIGHTNESS + 1;
        }

        if (brightness_after <= 0)
        {
            brightness_after += 1;
        }
        if (brightness_after > 250)
        {
            brightness_after = 0;
        }

        if (display_attribute.display_ota_renewal_percent < 13 && display_attribute.display_ota_renewal_percent >= 0)
        {
            for (uint8_t i = 3; i < display_attribute.display_numLeds; i++)
            {
                m_ext_rgb_pin_1(i, 0, 0, 0); // 关闭该灯
            }
            m_ext_drive_lighting_set_point_color(1, 0x1515);
            m_ext_rgb_pin_1(2, bluechannel, greenchannel, redchannel);
        }
        else
        {
            for (uint8_t i = 0; i < display_attribute.display_numLeds; i++)
            {
                if (i > 0 && i < display_attribute.display_ota_renewal_percent * display_attribute.display_numLeds / 100)
                {
                    m_ext_drive_lighting_set_point_color(i, 0x1515);
                }
                else if (i >= display_attribute.display_ota_renewal_percent * display_attribute.display_numLeds / 100 && i < display_attribute.display_ota_renewal_percent * display_attribute.display_numLeds / 100 + 1)
                {
                    m_ext_rgb_pin_1(i, bluechannel, greenchannel, redchannel);
                }
            }
        }
        // 延时指定时间，控制呼吸灯的速度
        mDelay_ms(2);
        if (g_state != M_DISPLAY_FIRMWARE_UPDATE_MODE)
            break;
    }
}

void m_callable_display_vt_short_status(void)
{
    m_callable_light_clean();
    while (1)
    {
        m_static_breathe_around(MY_COLOR_CONCOC(red), display_attribute.brightness_step);
        mDelay_ms(5);
        // 检查状态是否改变，如果改变则退出循环
        if (g_state != g_state_before)
        {
            break;
        }
    }
}

void m_callable_display_m_terminal_short_status(void)
{

    m_callable_light_clean();
    while (1)
    {
        for (int brightness = 0; brightness <= display_attribute.display_lighting_strength; brightness += 1)
        {
            m_static_breathe_set_led_color(MY_COLOR_CONCOC(blue), brightness);

            m_static_diplay_zone_immobilization();

            mDelay_ms(2);
            if (g_state != g_state_before)
            {
                break;
            }
        }

        for (int brightness = display_attribute.display_lighting_strength; brightness >= 0; brightness -= 1)
        {
            m_static_breathe_set_led_color(MY_COLOR_CONCOC(blue), brightness);

            m_static_diplay_zone_immobilization();
            mDelay_ms(2);
            if (g_state != g_state_before)
            {
                break;
            }
        }

        // 检查状态是否改变，如果改变则退出循环
        if (g_state != g_state_before)
        {
            break;
        }
    }
}

void m_callable_display_set_manual_duration(void)
{
    m_callable_light_clean();
    while (1)
    {
        for (int i = 0; i <= display_attribute.manual_setting_light_num; i++)
        {
            m_ext_drive_lighting_set_point_color(i, greed);
        }
        mDelay_ms(10);
        if (g_state != M_DISPLAY_SET_MANUAL_DURATION)
            break;
    }
}

void m_callable_display_set_zone(void)
{
    m_callable_light_clean();
    while (1)
    {
        mDelay_ms(10);
        if (g_state != M_DISPLAY_SET_ZONE_MODE)
            break;
    }
}

void m_callable_display_set_device_version(void)
{
    m_callable_light_clean();
    while (1)
    {
        mDelay_ms(10);
        if (g_state != M_DISPLAY_SET_DEVICE_VERSION_MODE)
            break;
    }
}

void m_callable_display_set_light_strength(uint8_t param)
{
    if (param >= 30 && param <= 245)
    {
        display_attribute.display_lighting_strength = param;
    }
    else
    {
        display_attribute.display_lighting_strength = 90;
    }

    if (display_attribute.display_lighting_strength == 30)
    {
        display_attribute.brightness_step = 1;
    }
    else if (display_attribute.display_lighting_strength == 90)
    {
        display_attribute.brightness_step = 3;
    }
    else
    {
        display_attribute.brightness_step = 6;
    }
}

void m_callable_display_set_light_close(void)
{

    display_attribute.display_set_light_close = 0;
    if (g_state_before != M_DISPLAY_ZONE_RUNNING_MODE)
    {
        m_callable_light_clean();
    }
    while (1)
    {
        mDelay_ms(10);
        if (g_state != M_DISPLAY_SET_LIGHT_CLOSE)
            break;
    }
}

void m_callable_display_set_light_open(void)
{
    display_attribute.display_set_light_close = 1;
    g_state = g_state_before;
}

void m_static_display_run(void *pvParameters)
{
    while (1)
    {
        switch (g_state)
        {
        case M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE:
            g_state_before = M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE;
            m_callable_display_start_up();
            break;

        case M_DISPLAY_IDLE_STATUS_MODE:
            g_state_before = M_DISPLAY_IDLE_STATUS_MODE;
            m_callable_display_idle_status();
            break;
        case M_DISPLAY_ZONE_RUNNING_MODE:
            g_state_before = M_DISPLAY_ZONE_RUNNING_MODE;
            m_callable_display_zone_running();
            break;
        case M_DISPLAY_ZONE_SELECT_MODE:
            g_state_before = M_DISPLAY_ZONE_SELECT_MODE;
            m_callable_display_zone_select();
            break;
        case M_DISPLAY_WIFI_CONFIG_STATUS1_MODE:
            g_state_before = M_DISPLAY_WIFI_CONFIG_STATUS1_MODE;
            m_callable_display_wifi_config_status1();
            break;
        case M_DISPLAY_WIFI_CONFIG_STATUS2_MODE:
            g_state_before = M_DISPLAY_WIFI_CONFIG_STATUS2_MODE;
            m_callable_display_wifi_config_status2();
            break;
        case M_DISPLAY_CONNECTING_TO_SERVER_MODE:
            g_state_before = M_DISPLAY_CONNECTING_TO_SERVER_MODE;
            m_callable_display_connecting_to_server();
            break;
        case M_DISPLAY_DISTRIBUTION_NETWORK_SUCCESS_MODE:
            g_state_before = M_DISPLAY_DISTRIBUTION_NETWORK_SUCCESS_MODE;
            m_callable_display_distribution_network_success();
            break;
        case M_DISPLAY_WIFI_CONNET_FAIL_MODE:
            g_state_before = M_DISPLAY_WIFI_CONNET_FAIL_MODE;
            m_callable_display_wifi_connet_fail();
            break;
        case M_DISPLAY_WIFI_NO_NETWORK_MODE:
            g_state_before = M_DISPLAY_WIFI_NO_NETWORK_MODE;
            m_callable_display_wifi_no_network();
            break;
        case M_DISPLAY_SERVER_CONNET_FAIL_MODE:
            g_state_before = M_DISPLAY_SERVER_CONNET_FAIL_MODE;
            m_callable_display_server_connet_fail();
            break;
        case M_DISPLAY_NETWORK_PROBLEM_MODE:
            g_state_before = M_DISPLAY_NETWORK_PROBLEM_MODE;
            m_callable_display_network_problem();
            break;
        case M_DISPLAY_WIFI_DISCONNECTION_MODE:
            g_state_before = M_DISPLAY_WIFI_DISCONNECTION_MODE;
            m_callable_display_wifi_disconnection();
            break;
        case M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE:
            g_state_before = M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE;
            m_callable_display_regional_short_circuit();
            break;
        case M_DISPLAY_ABNORMAL_FEEDBACK_MODE:
            g_state_before = M_DISPLAY_ABNORMAL_FEEDBACK_MODE;
            m_callable_display_abnormal_feedback();
            break;
        case M_DISPLAY_FIRMWARE_UPDATE_MODE:
            g_state_before = M_DISPLAY_FIRMWARE_UPDATE_MODE;
            m_callable_display_firmware_update();
            break;

        case M_DISPLAY_SET_ZONE_MODE:
            m_callable_display_set_zone();
            break;
        case M_DISPLAY_SET_DEVICE_VERSION_MODE:
            m_callable_display_set_device_version();
            break;

        case M_DISPLAY_SET_LIGHT_CLOSE:
            m_callable_display_set_light_close();
            break;
        case M_DISPLAY_SET_LIGHT_OPEN:
            m_callable_display_set_light_open();
            break;

        case M_DISPLAY_VT_SHORT_CIRCUIT_MODE:
            g_state_before = M_DISPLAY_VT_SHORT_CIRCUIT_MODE;
            m_callable_display_vt_short_status();

            break;
        case M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE:
            g_state_before = M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE;
            m_callable_display_m_terminal_short_status();
            break;
        case M_DISPLAY_SET_MANUAL_DURATION:
            g_state_before = M_DISPLAY_SET_MANUAL_DURATION;
            m_callable_display_set_manual_duration();
            break;

        default:

            break;
        }
        mDelay_ms(50);
    }
}

enum light_anima_mode m_callable_display_status_get()
{
    return g_state;
}