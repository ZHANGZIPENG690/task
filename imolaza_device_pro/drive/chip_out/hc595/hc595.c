#include "../chip_out.h"
#include <stdio.h>
#if !UNIT_TEST
#include "driver/gpio.h"
#endif

// #include "hc595.h"
/**
 * @brief Turn on the pump { Just open the pin_index and there will be no other actions and no pre_pin_index that is already running }
 *
 * @param pin_index
 * @return stat_m
 */
extern uint8_t chips595_lead;
extern uint8_t number_of_io;
extern uint32_t device_version_hc595;
extern uint64_t hc_flag; // 16
static int vue = 0;
int m_static_drive_hc595_io_output_limit(int num);

void m_ext_drive_hc595_enable(void)
{
    gpio_set_level(HC595_MODEL_EN, 0);
}

void m_ext_drive_hc595_disable(void)
{
    gpio_set_level(HC595_MODEL_EN, 1);
}

stat_m m_ext_drive_hc595_init(void)
{
#if !UNIT_TEST
    gpio_set_level(HC595_MODEL_EN, 1);
    GPIO_INIT(GPIO_MODE_OUTPUT, HC595_MODEL_EN, 0, 0, GPIO_PIN_INTR_DISABLE); // EN
    gpio_set_level(HC595_MODEL_EN, 1);

    GPIO_INIT(GPIO_MODE_OUTPUT, CLOCK_PIN, 0, 0, GPIO_PIN_INTR_DISABLE); // OUT
                                                                         // gpio_set_level(CLOCK_PIN, 1);
    GPIO_INIT(GPIO_MODE_OUTPUT, LATCH_PIN, 0, 0, GPIO_PIN_INTR_DISABLE); // DIN
                                                                         // gpio_set_level(LATCH_PIN, 1);
    GPIO_INIT(GPIO_MODE_OUTPUT, DATA_PIN, 0, 0, GPIO_PIN_INTR_DISABLE);  // CLK
                                                                         // gpio_set_level(DATA_PIN, 0);
#endif
    return succ_r;
}

stat_m m_ext_driver_hc595_shiftout(int value)
{
    int a = number_of_io;
    if (number_of_io == 32 || number_of_io == 30)
    {
        a = 32;
    }
#if !UNIT_TEST
    for (int i = 1; i <= a; i++, value <<= 1)
    {
        gpio_set_level(CLOCK_PIN, 0);
        if (value & hc_flag)
        {
            gpio_set_level(DATA_PIN, 1);
            // DEBUG_TEST("1");
        }
        else
        {
            gpio_set_level(DATA_PIN, 0);
            // DEBUG_TEST("0");
        }
        gpio_set_level(CLOCK_PIN, 1);
    }
    // DEBUG_TEST("\r\n");
    gpio_set_level(LATCH_PIN, 0);

    gpio_set_level(LATCH_PIN, 1);
#endif
    return succ_r;
}
/**
 * @brief
 *
 * @param bit
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_hc595_solenoidvalve(PinBit bit, uint8_t on_off)
{
    stat_m stat = fail_r;
#if !UNIT_TEST
    int hc595_vue;
    if ((device_version_hc595 == (DEVICE_HEARWARE_A003) || device_version_hc595 == (DEVICE_HEARWARE_B11) || device_version_hc595 == (DEVICE_HEARWARE_B12)) && (number_of_io == 32 || number_of_io == 30))
    {
        hc595_vue = 0x001;
    }
    else
    {
        hc595_vue = 0x100;
    }
    bit -= 1;
    if (on_off)
    {
        hc595_vue <<= bit;
        vue = hc595_vue;
        m_ext_drive_hc595_enable();
        stat = m_ext_driver_hc595_shiftout(hc595_vue);
    }
    else
    {
        vue = 0;
        stat = m_ext_driver_hc595_shiftout(0x0000);
    }
#endif
    return stat;
}

stat_m m_ext_drive_hc595_solenoidvalve_independent(PinBit bit, uint8_t on_off)
{

    stat_m stat = fail_r;
    int hc595_vue;
    if ((device_version_hc595 == (DEVICE_HEARWARE_A003) || device_version_hc595 == (DEVICE_HEARWARE_B11) || device_version_hc595 == (DEVICE_HEARWARE_B12)) && (number_of_io == 32 || number_of_io == 30))
    {
        hc595_vue = 0x001;
    }
    else
    {
        hc595_vue = 0x100;
    }
    bit -= 1;

    if (on_off)
    {
        hc595_vue <<= bit;
        if (m_static_drive_hc595_io_output_limit(vue) < 2)
        {
            vue |= hc595_vue;
        }

        m_ext_drive_hc595_enable();
        stat = m_ext_driver_hc595_shiftout(vue);
    }
    else
    {
        hc595_vue <<= bit;
        vue = (~hc595_vue) & vue;
        stat = m_ext_driver_hc595_shiftout(vue);
    }
    return stat;
}

stat_m m_ext_drive_hc595_shiftout_reset(void)
{
#if !UNIT_TEST
    vue = 0;
    m_ext_drive_hc595_disable();
    for (int i = 1; i <= chips595_lead; i++)
    {
        gpio_set_level(CLOCK_PIN, 0);
        mDelay_ms(1);
        gpio_set_level(DATA_PIN, 0);
        mDelay_ms(1);
        gpio_set_level(CLOCK_PIN, 1);
    }
    gpio_set_level(LATCH_PIN, 0);
    gpio_set_level(LATCH_PIN, 1);
#endif
    return succ_r;
}

// 计算二进制数中1的数量  目的时限制IO输出数量   最多两个 同时打开
int m_static_drive_hc595_io_output_limit(int num)
{
    int count = 0;

    for (int i = 0; i < 32; i++)
    {
        count += num & 1;
        num >>= 1;
    }

    return count;
}

// stat_m m_ext_drive_hc595_hammer_shiftout(int value)
// {
// #if !UNIT_TEST
//     for (int i = 1; i <= chips595_lead; i++)
//     {
//         gpio_set_level(CLOCK_PIN, 0);
//         if (i < 16 + 1 - number_of_io)
//         {
//             gpio_set_level(DATA_PIN, 0);
//             // DEBUG_TEST("0");
//         }
//         else
//         {
//             if (value & hc_flag)
//             {
//                 gpio_set_level(DATA_PIN, 1);
//                 // DEBUG_TEST("1");
//             }
//             else
//             {
//                 gpio_set_level(DATA_PIN, 0);
//                 // DEBUG_TEST("0");
//             }
//             value <<= 1;
//         }

//         gpio_set_level(CLOCK_PIN, 1);
//     }
//     // DEBUG_TEST("\r\n");
//     gpio_set_level(LATCH_PIN, 0);

//     gpio_set_level(LATCH_PIN, 1);
// #endif
//     return succ_r;
// }

// stat_m m_ext_drive_hc595_solenoidvalve_two_bit(PinBit bito1, PinBit bito2, uint8_t on_off)
// {
// #if !UNIT_TEST
//     int hc595111 = 0x100;
//     int hc595222 = 0x100;
//     int vue = 0;
//     DEBUG_TEST("HC595_SolenoidValve_two_bit(%d--%d)\r\n", bito1, bito2);
//     bito1 -= 1;
//     bito2 -= 1;
//     int max = bito1 > bito2 ? bito1 : bito2;
//     for (uint8_t i = 0; i < max; i++)
//     {
//         if (i < bito1)
//             hc595111 <<= 1;
//         if (i < bito2)
//             hc595222 <<= 1;
//         hc595111 &= 0xffff00;
//         hc595222 &= 0xffff00;
//     }
//     if (on_off)
//     {
//         vue = hc595222 | hc595111;
//     }
//     else
//     {
//         vue = hc595222;
//     }
//     DEBUG_TEST(" HC595_hammer_outVue(vue)%d\r\n", vue);
//     return m_ext_drive_hc595_hammer_shiftout(vue);
// #endif
// }
