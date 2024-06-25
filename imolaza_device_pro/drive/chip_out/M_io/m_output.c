#include "m_output.h"

stat_m m_callable_solenoid_manage_M_init(void)
{
#if !UNIT_TEST
    GPIO_INIT(GPIO_MODE_OUTPUT, 15, 1, 0, GPIO_PIN_INTR_DISABLE);
#endif
    return succ_r;
}
stat_m m_callable_solenoid_manage_M_open(void)
{
#if !UNIT_TEST
    gpio_set_level(15, 1);
#endif
    return succ_r;
}
stat_m m_callable_solenoid_manage_M_close(void)
{
#if !UNIT_TEST
    gpio_set_level(15, 0);
#endif
    return succ_r;
}


/**
 * @brief 上电直接打开引脚12
 * @return stat_m
 */
stat_m m_callable_drive_io_pin12_init(void)
{
    // 配置GPIO引脚
    gpio_pad_select_gpio(GPIO_NUM_12);
    // 设置GPIO引脚为输出模式
    gpio_set_direction(GPIO_NUM_12, GPIO_MODE_OUTPUT);
    // 将引脚设置为打开状态
    gpio_set_level(GPIO_NUM_12, 1);
    return succ_r;
}
/**
 * @brief 上电直接打开引脚12
 * @return stat_m
 */
stat_m m_callable_drive_io_pin12_uninit(void)
{
    // 配置GPIO引脚
    gpio_pad_select_gpio(GPIO_NUM_12);
    // 设置GPIO引脚为输出模式
    gpio_set_direction(GPIO_NUM_12, GPIO_MODE_OUTPUT);
    // 将引脚设置为打开状态
    gpio_set_level(GPIO_NUM_12, 0);
    return succ_r;
}