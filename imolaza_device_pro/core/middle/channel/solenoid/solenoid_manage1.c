#include "solenoid_manage1.h"

uint8_t chips595_lead = 0;
uint8_t number_of_io = 0;
uint32_t device_version_hc595;
uint64_t hc_flag = 0;

bool pumm = false;

stat_m configuration_modification(int number_of_channels_set, uint32_t _device_version_set, int io_chip_set)
{
    stat_m stat = fail_r;
    enum io_chip_m io_chip = io_chip_set;
    number_of_io = number_of_channels_set;
    device_version_hc595 = _device_version_set; // 用于HC595驱动代码判断
    DEBUG_TEST(DB_I, "11111device_version_hc595device_version_hc595 %d", device_version_hc595);
    if (io_chip == chip_ahh5)
    {
        m_ext_drive_solenoid_init(_device_version_set, number_of_channels_set, io_chip);
        stat = succ_r;
    }
    else if (io_chip == chip_hc595)
    {

        if (_device_version_set == DEVICE_HEARWARE_A001)
        {

            chips595_lead = 8;
            if (number_of_channels_set == DEVICE_4_ZONE)
                hc_flag = 0x800;
            if (number_of_channels_set == DEVICE_8_ZONE)
                hc_flag = 0x8000;
            stat = succ_r;
        }
        else if (_device_version_set == DEVICE_HEARWARE_A002 || _device_version_set == DEVICE_HEARWARE_C11)
        {
            chips595_lead = 16;
            if (number_of_channels_set == DEVICE_4_ZONE)
                hc_flag = 0x800;
            if (number_of_channels_set == DEVICE_8_ZONE)
                hc_flag = 0x8000;
            if (number_of_channels_set == DEVICE_12_ZONE)
                hc_flag = 0x80000;
            if (number_of_channels_set == DEVICE_16_ZONE)
                hc_flag = 0x800000;
            stat = succ_r;
        }
        else if (_device_version_set == DEVICE_HEARWARE_A003 || _device_version_set == DEVICE_HEARWARE_B11 || _device_version_set == DEVICE_HEARWARE_B12)
        {
            chips595_lead = 32;
            if (number_of_channels_set == DEVICE_4_ZONE)
                hc_flag = 0x800;
            if (number_of_channels_set == DEVICE_6_ZONE)
                hc_flag = 0x2000;
            if (number_of_channels_set == DEVICE_8_ZONE)
                hc_flag = 0x8000;
            if (number_of_channels_set == DEVICE_12_ZONE)
                hc_flag = 0x80000;
            if (number_of_channels_set == DEVICE_16_ZONE)
                hc_flag = 0x800000;
            if (number_of_channels_set == DEVICE_18_ZONE)
                hc_flag = 0x2000000;
            if (number_of_channels_set == DEVICE_24_ZONE)
                hc_flag = 0x80000000;
            if (number_of_channels_set == DEVICE_30_ZONE)
                hc_flag = 0x2000000000;
            if (number_of_channels_set == DEVICE_32_ZONE)
                hc_flag = 0x8000000000;
            stat = succ_r;
        }
        else
        {
            stat = fail_r;
        }
    }
    return stat;
}

stat_m m_callable_solenoid_manage_init(uint32_t _device_version_set, int number_of_channels_set, int io_chip)
{
    stat_m stat;
    stat = configuration_modification(number_of_channels_set, _device_version_set, io_chip);
    stat = m_ext_drive_solenoid_init(_device_version_set, number_of_channels_set, io_chip);
    // m_callable_light_display_initial(display_version, number_of_channels_set, Brightness_level_3, 3);
    return stat;
}

/**
 * @brief 仅仅打开一个区域不会影响其他区域
 *
 */
stat_m m_callable_solenoid_manage_direct_open(uint8_t channel)
{
    stat_m stat = fail_r;

    if (channel <= number_of_io && channel > 0 && !pumm)
    {
        stat = m_ext_drive_solenoid_direct_open_and_close(channel, 1);
    }
    return stat;
}
/**
 * @brief 仅仅关闭一个区域不会影响其他区域
 *
 */
stat_m m_callable_solenoid_manage_direct_close(uint8_t channel)
{
    stat_m stat = fail_r;

    if (channel <= number_of_io && channel > 0)
    {
        stat = m_ext_drive_solenoid_direct_open_and_close(channel, 0);
    }
    return stat;
}

/**
 * @brief 强行打开其一个区域
 *
 * @param channel
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_open(uint8_t channel)
{
    stat_m stat = fail_r;

    if (channel <= number_of_io && channel > 0)
    {
        // printf("m_callable_solenoid_manage_open %d\r\n", channel);
        DEBUG_TEST_W("m_callable_solenoid_manage_open ( %hhd (%d))", channel, number_of_io);
        m_callable_current_monitor_start();
        m_callable_current_set_checkout_zone(channel);
        m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_SIGN_ZONE);
        m_ext_drive_solenoid_open_and_close(channel, 1);
        m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, channel);
        mDelay_ms(400);

        // m_callable_current_monitor_stop();
        if (m_callable_display_status_get() != M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE)
        {
            m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_PUMP);
            m_callable_solenoid_manage_M_open();
            mDelay_ms(150);
            DEBUG_TEST_W("-- Done ");
        }
        m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_SIGN_ZONE);
        // m_callable_display_status(20, channel); // 必须先将灯光设置为一个空模式，此时再进行pca9557的IIC写入，不然会有冲突，两者共用一个IIC
    }
    else
    {
        DEBUG_TEST(DB_E, "Zone select %d num out ...", channel);
    }
    return stat;
}
/**
 * @brief 强行关闭区域
 *
 * @param channel
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_close(uint8_t channel)
{
    stat_m stat = fail_r;

    DEBUG_TEST_W("m_callable_solenoid_manage_close ( %hhd (%d))", channel, number_of_io);
    if (channel <= number_of_io && channel > 0)
    {
        m_callable_solenoid_manage_M_close();
        m_callable_current_monitor_stop();
        m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_VT);
        m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, channel);
        mDelay_ms(50);
        pumm = false;
        stat = m_ext_drive_solenoid_open_and_close(channel, 0);
    }
    else
    {
        DEBUG_TEST(DB_E, "Zone select %hhd num out ... ", channel);
    }
    return stat;
}
/**
 * @brief 关闭上一个 ，打开下一个
 *
 * @param pre_pin_index  上一个
 * @param next_pin_index  下一个
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_continuously(uint8_t pre_pin_index, uint8_t next_pin_index)
{

    stat_m stat = fail_r;

    if (pre_pin_index <= number_of_io && pre_pin_index > 0)
    {
        // stat = m_ext_drive_solenoid_close(pre_pin_index);
        m_callable_solenoid_manage_M_open();
        m_callable_current_monitor_start();
        stat = m_ext_drive_solenoid_open_and_close(next_pin_index, 1);
        m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, next_pin_index);
    }
    else
    {
        DEBUG_TEST(DB_E, "Zone select %hhd num out ... ", pre_pin_index);
    }
    return stat;
}
// stat_m m_callable_solenoid_manage_open_together(uint8_t pre_pin_index, uint8_t next_pin_index)
// {
//     stat_m stat = fail_r;
//     if (pre_pin_index > number_of_io)
//         pre_pin_index %= number_of_io;
//     if (pre_pin_index == 0)
//         pre_pin_index = number_of_io;

//     if (next_pin_index > number_of_io)
//         next_pin_index %= number_of_io;
//     if (next_pin_index == 0)
//         next_pin_index = number_of_io;
//     stat = m_ext_drive_solenoid_together(pre_pin_index, next_pin_index, 1);
//     return stat;
// }

// stat_m m_callable_solenoid_manage_close_together(uint8_t pre_pin_index, uint8_t next_pin_index)
// {
//     stat_m stat = fail_r;
//     if (pre_pin_index > number_of_io)
//         pre_pin_index %= number_of_io;
//     if (pre_pin_index == 0)
//         pre_pin_index = number_of_io;

//     if (next_pin_index > number_of_io)
//         next_pin_index %= number_of_io;
//     if (next_pin_index == 0)
//         next_pin_index = number_of_io;
//     stat = m_ext_drive_solenoid_together(pre_pin_index, next_pin_index, 0);
//     return stat;
// }

stat_m m_callable_solenoid_manage_close_all(void)
{
    stat_m stat;
    // m_callable_display_status(20, 0); // 必须先将灯光设置为一个空模式，此时再进行pca9557的IIC写入，不然会有冲突，两者共用一个IIC
    stat = m_ext_drive_solenoid_close_all();
    stat = m_callable_solenoid_manage_M_close();
    m_callable_current_monitor_stop();
    // m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_VT);
    return stat;
}

//*******************************************************
