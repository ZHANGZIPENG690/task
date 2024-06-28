#include "mrtc.h"

int m_callable_rtc_get_percentage_battery_capacity(float voltage_float, char *percentage_battery_get)
{
    float voltage_battery_min = 0.66;
    float voltage_battery_max = 1.20;
    float percentage_battery = 0.0;
    if (voltage_float <= 0.66)
    {
        percentage_battery = 0.00;
    }
    else
    {
        percentage_battery = ((voltage_float - voltage_battery_min) / voltage_battery_max) * 100;
    }

    DEBUG_TEST(DB_I, "percentage_battery: %.1f%%", (percentage_battery));
    sprintf(percentage_battery_get, "%.1f%%", percentage_battery);

    if (percentage_battery <= 30 && percentage_battery >= 20)
    {
        DEBUG_TEST(DB_E, "RTC电量异常 异常等级  2");
        return 2;
    }
    else if (percentage_battery <= 20)
    {
        DEBUG_TEST(DB_E, "RTC电量异常 异常等级  1");
        return 1;
    }
    else
    {
        DEBUG_TEST(DB_E, "RTC电量正常");
        return 0;
    }
}

float m_callable_get_voltage_rtc(void)
{
    float voltage_float = 0.00; // 定义一个电压值变量.
    uint32_t adc_value = 0;
    for (int i = 0; i < 40; i++)
    {
        adc_value += m_ext_drive_read_adc_value_rtc();
    }
    adc_value = adc_value / 40;

    voltage_float = m_ext_drive_adc_to_voltage_rtc(adc_value, &voltage_float);

    DEBUG_TEST(DB_W, "读取的原始 voltage_float  %.02f", voltage_float);

    // if (voltage_float <= 2.0)
    // {
    //     voltage_float_magnification = 1.67;
    //     voltage_float = voltage_float * voltage_float_magnification;
    // }
    // else
    // {
    //     voltage_float_magnification = 1.775;
    //     voltage_float = voltage_float * voltage_float_magnification;
    // }
    // DEBUG_TEST(DB_E, "RTC电压值为: %.02f", voltage_float);

    return voltage_float;
}

int m_callable_rtc_power_warning(char *percentage_battery_get)
{
    uint64_t time_m = 0;
    float voltage_float = 0.00; // 定义一个电压值变量.
    m_callable_midlle_rtc_get_time(&time_m);
    DEBUG_TEST(DB_I, "time_m:%lld", time_m);

    voltage_float = m_callable_get_voltage_rtc();

    return m_callable_rtc_get_percentage_battery_capacity(voltage_float, percentage_battery_get);
}
