#include "solenoid.h"

int flag = 0;
extern stat_m configuration_modification(int number_of_channels_set, uint32_t _device_version_set, int io_chip);
int io_chip = 0;

stat_m m_ext_drive_solenoid_init(uint32_t _device_version_set, int number_of_channels_set, int io_chip_set)
{
#if !UNIT_TEST
        stat_m stat = fail_r;
        io_chip = io_chip_set;
        if (io_chip == chip_ahh5)
        {
                if (flag == 0)
                {
                        stat = m_ext_drive_ahh5_init();
                        flag = 1;
                }
                stat = m_ext_drive_ahh5_reset();
        }
        else if (io_chip == chip_hc595)
        {
                stat = configuration_modification(number_of_channels_set, _device_version_set, chip_hc595);
                stat = m_ext_drive_hc595_init();
        }
        else
        {
                return fail_r;
        }
        return stat;
#endif
}
/**
 * @brief 直接打开一个区域,其他区域会关闭
 *
 * @param pin_index
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_open_and_close(int pin_index, int is_open)
{
#if !UNIT_TEST
        stat_m stat = fail_r;
        if (io_chip == chip_ahh5)
        {
                stat = m_ext_drive_ahh5_solenoidvalve(pin_index, is_open);
        }
        else if (io_chip == chip_hc595)
        {
                stat = m_ext_drive_hc595_solenoidvalve(pin_index, is_open);
        }
        else
        {
                return fail_r;
        }
        return stat;
#endif
}

/**
 * @brief 打开一个区域,不会影响其他区域
 *
 * @param pin_index
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_direct_open_and_close(int pin_index, int is_open)
{
#if !UNIT_TEST
        stat_m stat = fail_r;

        if (io_chip == chip_ahh5)
        {
                stat = m_ext_drive_ahh5_solenoidvalve_independent(pin_index, is_open);
        }
        else if (io_chip == chip_hc595)
        {
                stat = m_ext_drive_hc595_solenoidvalve_independent(pin_index, is_open);
        }
        else
        {
                return fail_r;
        }
        return stat;
#endif
}

stat_m m_ext_drive_solenoid_close(int pin_index)
{
#if !UNIT_TEST
        stat_m stat = fail_r;

        if (io_chip == chip_ahh5)
        {
                stat = m_ext_drive_ahh5_solenoidvalve_independent(pin_index, 0);
        }
        else if (io_chip == chip_hc595)
        {
                stat = m_ext_drive_hc595_solenoidvalve_independent(pin_index, 0);
        }
        else
        {
                return fail_r;
        }
        return stat;
#endif
}

stat_m m_ext_drive_solenoid_close_all(void)
{
#if !UNIT_TEST
        stat_m stat = fail_r;

        if (io_chip == chip_ahh5)
        {
                stat = m_ext_drive_ahh5_reset();
        }
        else if (io_chip == chip_hc595)
        {
                stat = m_ext_drive_hc595_shiftout_reset();
        }
        else
        {
                return fail_r;
        }
        return stat;
#endif
}

// stat_m m_ext_drive_solenoid_continuously(int pre_pin_index, int next_pin_index)
// {
// #if !UNIT_TEST
//         stat_m stat = fail_r;
//         if (io_chip == chip_ahh5)
//         {

//                 stat = m_ext_drive_ahh5_solenoidvalve_two_bit(pre_pin_index, next_pin_index, 1);
//         }
//         else if (io_chip == chip_hc595)
//         {
//                 stat = m_ext_drive_hc595_solenoidvalve_two_bit(pre_pin_index, next_pin_index, 1);
//         }
//         else
//         {
//                 return fail_r;
//         }
//         return stat;
// #endif
// }

// stat_m m_ext_drive_solenoid_together(int pre_pin_index, int next_pin_index, int on_off)
// {
// #if !UNIT_TEST
//         stat_m stat = fail_r;
//         if (on_off == 1)
//         {
//                 if (io_chip == chip_ahh5)
//                 {

//                         stat = m_ext_drive_ahh5_solenoidvalve_two_bit(pre_pin_index, next_pin_index, 1);
//                 }
//                 else if (io_chip == chip_hc595)
//                 {
//                         stat = m_ext_drive_hc595_solenoidvalve_two_bit(pre_pin_index, next_pin_index, 1);
//                 }
//                 else
//                 {
//                         stat = fail_r;
//                 }
//         }
//         else if (on_off == 0)
//         {
//                 if (io_chip == chip_ahh5)
//                 {

//                         stat = m_ext_drive_ahh5_solenoidvalve_two_bit(pre_pin_index, next_pin_index, 0);
//                 }
//                 else if (io_chip == chip_hc595)
//                 {
//                         stat = m_ext_drive_hc595_solenoidvalve_two_bit(pre_pin_index, next_pin_index, 0);
//                 }
//                 else
//                 {
//                         stat = fail_r;
//                 }
//         }
//         return stat;
// #endif
// }
