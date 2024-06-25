#include "display_manage.h"

extern enum light_anima_mode g_state;        // 状态变量   共有15种状态
extern enum light_anima_mode g_state_before; // 状态变量   共有15种状态

extern uint8_t arr4[];
extern uint8_t arr6[];
extern uint8_t arr8[];
extern uint8_t arr12[];
extern uint8_t arr16[];

extern uint8_t arr4_c10[];
extern uint8_t arr6_c10[];
extern uint8_t arr8_c10[];
extern uint8_t arr12_c10[];
extern uint8_t arr16_c10[];

extern uint8_t arr32_pro[];
extern uint8_t arr30_pro[];
extern uint8_t arr24_pro[];
extern uint8_t arr18_pro[];
extern uint8_t arr12_pro[];
extern uint8_t arr6_pro[];

uint8_t redchannel;
uint8_t greenchannel;
uint8_t bluechannel;

/*提取调整后的颜色通道值*/
void m_static_breathe_set_led_color(uint32_t adjustedColor, uint8_t brightness)
{
    if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B12 || display_attribute.display_device_version == DEVICE_HEARWARE_B11 || display_attribute.display_device_version == DEVICE_HEARWARE_C11)
    {
        //  提取调整后的颜色通道值
        redchannel = (adjustedColor * brightness >> 16) & 0xFF;
        greenchannel = (adjustedColor * brightness >> 8) & 0xFF;
        bluechannel = adjustedColor * brightness & 0xFF;
    }
    else
    {
        //  提取调整后的颜色通道值
        bluechannel = (adjustedColor * brightness >> 16) & 0xFF;
        greenchannel = (adjustedColor * brightness >> 8) & 0xFF;
        redchannel = adjustedColor * brightness & 0xFF;
    }
}

/*单个灯  呼吸*/
void m_static_breathe(uint32_t adjustedColor, uint8_t brightness_step)
{
    for (int brightness = 0; brightness <= display_attribute.display_lighting_strength; brightness += brightness_step)
    {
        mDelay_ms(20);
        if (g_state != g_state_before)
        {
            break;
        }
        m_static_breathe_set_led_color(adjustedColor, brightness);
        m_static_display_version_light();
        mDelay_ms(20);
    }
    for (int brightness = display_attribute.display_lighting_strength; brightness >= 0; brightness -= brightness_step)
    {
        mDelay_ms(20);
        if (g_state != g_state_before)
        {
            break;
        }
        m_static_breathe_set_led_color(adjustedColor, brightness);
        m_static_display_version_light();
        mDelay_ms(20);
    }
}

void m_static_breathe_alone(uint32_t adjustedColor, uint8_t brightness_step, uint8_t i)
{
    for (int brightness = 0; brightness <= display_attribute.display_lighting_strength; brightness += brightness_step)
    {
        if (g_state != g_state_before)
        {
            break;
        }
        m_static_breathe_set_led_color(adjustedColor, brightness);
        m_ext_rgb_pin_1(i, bluechannel, greenchannel, redchannel);
        mDelay_ms(8);
    }
    for (int brightness = display_attribute.display_lighting_strength; brightness >= 0; brightness -= brightness_step)
    {
        if (g_state != g_state_before)
        {
            break;
        }
        m_static_breathe_set_led_color(adjustedColor, brightness);
        m_ext_rgb_pin_1(i, bluechannel, greenchannel, redchannel);
        mDelay_ms(8);
    }
}
/*闪烁、非呼吸*/
void m_static_not_breathe(uint32_t adjustedColor)
{
    m_static_breathe_set_led_color(adjustedColor, display_attribute.display_lighting_strength);
    m_static_display_version_light();
}

/*一圈灯 呼吸*/
void m_static_breathe_around(uint32_t adjustedColor, uint8_t brightness_step)
{
    for (int brightness = 0; brightness <= display_attribute.display_lighting_strength; brightness += brightness_step)
    {
        if (g_state != g_state_before)
        {
            break;
        }
        m_static_breathe_set_led_color(adjustedColor, brightness);

        for (uint8_t i = 0; i < (display_attribute.display_numLeds); i++)
        {
            if (g_state != g_state_before)
            {
                break;
            }
            m_ext_rgb_pin_1(i, bluechannel, greenchannel, redchannel);
        }
    }

    for (int brightness = display_attribute.display_lighting_strength; brightness >= 0; brightness -= brightness_step)
    {
        if (g_state != g_state_before)
        {
            break;
        }
        m_static_breathe_set_led_color(adjustedColor, brightness);

        for (uint8_t i = 0; i < (display_attribute.display_numLeds); i++)
        {
            if (g_state != g_state_before)
            {
                break;
            }
            m_ext_rgb_pin_1(i, bluechannel, greenchannel, redchannel);
        }
    }
}

/*区域选择*/
void m_static_diplay_zone_select(uint8_t *arr)
{
    if (display_attribute.display_zone_select > 0 && display_attribute.display_zone_select <= display_attribute.display_number_of_channels)
    {
        if (display_attribute.display_zone_current != display_attribute.display_zone_select)
        {
            m_ext_rgb_pin_2(arr[display_attribute.display_zone_current - 1], black); // 关灯
        }
        display_attribute.display_zone_current = display_attribute.display_zone_select;
        m_ext_rgb_pin_1(arr[display_attribute.display_zone_current - 1], redchannel, greenchannel, bluechannel);
    }
    else
    {
        m_ext_rgb_pin_1(arr[display_attribute.display_zone_current - 1], redchannel, greenchannel, bluechannel);
    }
}

void m_static_diplay_zone_immobilization(void)
{
    if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B12 || display_attribute.display_device_version == DEVICE_HEARWARE_B11)
    {
        m_ext_rgb_pin_1(9, redchannel, greenchannel, bluechannel);
        m_ext_rgb_pin_1(25, redchannel, greenchannel, bluechannel);
    }
    else if (display_attribute.display_device_version == DEVICE_HEARWARE_C11)
    {
        m_ext_rgb_pin_1(5, redchannel, greenchannel, bluechannel);
        m_ext_rgb_pin_1(14, redchannel, greenchannel, bluechannel);
    }
    else
    {
        m_ext_rgb_pin_1(6, redchannel, greenchannel, bluechannel);
        m_ext_rgb_pin_1(18, redchannel, greenchannel, bluechannel);
    }
}

/*局部灯光闪烁*/
void m_static_localized_light_flashing(uint32_t display_device_version, uint8_t display_numLeds, uint8_t step, enum light_RGB_color static_color, enum light_RGB_color flicker_color, bool isOn)
{
    for (uint8_t i = 0; i < display_numLeds; i++)
    {
        if (step == 1)
        {
            if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 1 && i <= 5) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 1 && i <= 9) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 1 && i < 7))
            {
                m_ext_drive_lighting_set_point_color(i, static_color);
            }
            else if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 6 && i <= 9) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 10 && i < 17) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 7 && i < 13))
            {
                if (isOn)
                {
                    m_ext_drive_lighting_set_point_color(i, flicker_color);
                }
                else
                {
                    m_ext_rgb_pin_1(i, 0, 0, 0);
                }
            }
        }
        else if (step == 2)
        {
            if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 1 && i <= 9) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 1 && i <= 17) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 1 && i < 13))
            {
                m_ext_drive_lighting_set_point_color(i, static_color); // 设置为蓝色静止状态的灯
            }
            else if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 10 && i <= 13) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 18 && i < 25) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 13 && i < 18))
            {
                if (isOn)
                {
                    m_ext_drive_lighting_set_point_color(i, flicker_color);
                }
                else
                {
                    m_ext_rgb_pin_1(i, 0, 0, 0);
                }
            }
        }
        else if (step == 3)
        {
            if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 1 && i <= 13) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 1 && i <= 25) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 1 && i < 18))
            {
                m_ext_drive_lighting_set_point_color(i, static_color); // 设置为蓝色静止状态的灯
            }
            else if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 14 && i <= 18) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 26 && i <= 32) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 18 && i <= 22))
            {
                if (isOn)
                {
                    m_ext_drive_lighting_set_point_color(i, flicker_color);
                }
                else
                {
                    m_ext_rgb_pin_1(i, 0, 0, 0);
                }
            }
        }
        if (g_state != g_state_before)
        {
            break;
        }
    }
}

// /*局部灯光闪烁*/
// void m_static_localized_light_flashing(uint32_t display_device_version, uint8_t display_numLeds, uint8_t step, enum light_RGB_color static_color, enum light_RGB_color flicker_color, bool isOn)
// {
//     for (uint8_t i = 0; i < display_numLeds; i++)
//     {
//         if (step == 1)
//         {
//             if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 1 && i <= 5) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i > 1 && i < 9) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 0 && i < 7))
//             {
//                 m_ext_drive_lighting_set_point_color(i, static_color);
//             }
//             else if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 6 && i <= 9) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 10 && i < 17) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 7 && i < 12))
//             {
//                 if (isOn)
//                 {
//                     m_ext_drive_lighting_set_point_color(i, flicker_color);
//                 }
//                 else
//                 {
//                     m_ext_rgb_pin_1(i, 0, 0, 0);
//                 }
//             }
//         }
//         else if (step == 2)
//         {
//             if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 1 && i <= 9) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i > 1 && i < 17 && i != 9) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 1 && i < 12))
//             {
//                 m_ext_drive_lighting_set_point_color(i, static_color); // 设置为蓝色静止状态的灯
//             }
//             else if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 10 && i <= 13) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 18 && i < 25) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 12 && i < 17))
//             {
//                 if (isOn)
//                 {
//                     m_ext_drive_lighting_set_point_color(i, flicker_color);
//                 }
//                 else
//                 {
//                     m_ext_rgb_pin_1(i, 0, 0, 0);
//                 }
//             }
//         }
//         else if (step == 3)
//         {
//             if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 1 && i <= 13) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i > 1 && i <= 24 && i != 9 && i != 17 && i != 27) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 1 && i < 17))
//             {
//                 m_ext_drive_lighting_set_point_color(i, static_color); // 设置为蓝色静止状态的灯
//             }
//             else if (((display_device_version == DEVICE_HEARWARE_C11) && i >= 14 && i <= 18) || ((display_device_version == DEVICE_HEARWARE_A003 || display_device_version == DEVICE_HEARWARE_B12 || display_device_version == DEVICE_HEARWARE_B11) && i >= 26 && i <= 32) || ((display_device_version == DEVICE_HEARWARE_A001 || display_device_version == DEVICE_HEARWARE_A002) && i >= 17 && i <= 22))
//             {
//                 if (isOn)
//                 {
//                     m_ext_drive_lighting_set_point_color(i, flicker_color);
//                 }
//                 else
//                 {
//                     m_ext_rgb_pin_1(i, 0, 0, 0);
//                 }
//             }
//         }
//         if (g_state != g_state_before)
//         {
//             break;
//         }
//     }
// }

void m_static_display_version_light(void)
{
    if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B12 || display_attribute.display_device_version == DEVICE_HEARWARE_B11)
    {
        switch (display_attribute.display_number_of_channels)
        {
        case 32:
            m_static_diplay_zone_select(arr32_pro);
            break;
        case 30:
            m_static_diplay_zone_select(arr30_pro);
            break;
        case 24:
            m_static_diplay_zone_select(arr24_pro);
            break;
        case 18:
            m_static_diplay_zone_select(arr18_pro);
            break;
        case 12:
            m_static_diplay_zone_select(arr12_pro);
            break;
        case 6:
            m_static_diplay_zone_select(arr6_pro);
            break;
        default:
            break;
        }
    }
    else if (display_attribute.display_device_version == DEVICE_HEARWARE_C11)
    {
        switch (display_attribute.display_number_of_channels)
        {
        case 16:
            m_static_diplay_zone_select(arr16_c10);
            break;
        case 12:
            m_static_diplay_zone_select(arr12_c10);
            break;
        case 8:
            m_static_diplay_zone_select(arr8_c10);
            break;
        case 6:
            m_static_diplay_zone_select(arr6_c10);
            break;
        case 4:
            m_static_diplay_zone_select(arr4_c10);
            break;

        default:
            break;
        }
    }
    else
    {
        switch (display_attribute.display_number_of_channels)
        {
        case 16:
            m_static_diplay_zone_select(arr16);
            break;
        case 12:
            m_static_diplay_zone_select(arr12);
            break;
        case 8:
            m_static_diplay_zone_select(arr8);
            break;
        case 6:
            m_static_diplay_zone_select(arr6);
            break;
        case 4:
            m_static_diplay_zone_select(arr4);
            break;

        default:
            break;
        }
    }
}