#include "mrtc.h"

float m_callable_get_voltage_rtc(void)
{
    float voltage_float; // 定义一个电压值变量.
    uint32_t adc_value = 0;
    for (int i = 0; i < 40; i++)
    {
        adc_value += m_ext_drive_read_adc_value_rtc();
    }
    adc_value = adc_value / 40;
    // DEBUG_I("adc_value  %d", adc_value);

    voltage_float = m_ext_drive_adc_to_voltage_rtc(adc_value, &voltage_float);
    if (voltage_float >= 1.05)
    {
        voltage_float = voltage_float * 1.465;
    }
    DEBUG_TEST(DB_E,"RTC电压值为: %.02f", voltage_float);
    return voltage_float;
}

stat_m m_callable_rtc_power_warning(void)
{
    stat_m stat = fail_r;
    if (m_callable_get_voltage_rtc() >= 2.4)
    {
        DEBUG_TEST(DB_E,"RTC电量正常");
        stat = succ_r;
    }
    else
    {
        DEBUG_TEST(DB_E,"RTC电量异常");
    }
    return stat;
}
