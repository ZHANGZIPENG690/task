#include "m_adc.h"
#if !UNIT_TEST
static esp_adc_cal_characteristics_t *adc_chars;
#endif

void m_ext_drive_adc_init()
{
#if !UNIT_TEST
    // 配置ADC参数
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    // 校准ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    (void)val_type;
#endif
}

// 读取ADC值的函数
int m_ext_drive_read_adc_value()
{
#if !UNIT_TEST
    int adc_reading = 0;
    // 进行ADC采集
    adc_reading = adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_7);
    return adc_reading;
#endif
}

// ADC值转化成电压值
float m_ext_drive_adc_to_voltage(int adc_reading, float *voltage_float)
{
    float voltage = 0;
#if !UNIT_TEST
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t cal_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);
    if (adc_reading)
    {
        uint32_t voltage_get = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars);
        voltage=voltage_get/1000.0;
    }
    // return (float)voltage / 1000.0;
    return voltage;
#endif
}

//********************************RTC           ADC**********************************//
#if !UNIT_TEST
static esp_adc_cal_characteristics_t *adc_chars_rtc;
#endif

void m_ext_drive_adc_init_rtc()
{
#if !UNIT_TEST
    // 配置ADC参数
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    // 校准ADC
    adc_chars_rtc = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, RTC_DEFAULT_VREF, adc_chars_rtc);
    (void)val_type;
#endif
}

// 读取ADC值的函数
int m_ext_drive_read_adc_value_rtc()
{
#if !UNIT_TEST
    int adc_reading = 0;
    // 进行ADC采集
    adc_reading = adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_0);
    return adc_reading;
#endif
}

// ADC值转化成电压值
float m_ext_drive_adc_to_voltage_rtc(int adc_reading, float *voltage_float)
{
#if !UNIT_TEST
    esp_adc_cal_characteristics_t adc_chars_rtc;
    esp_adc_cal_value_t cal_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, RTC_DEFAULT_VREF, &adc_chars_rtc);
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars_rtc);
    //*voltage_float = (float)voltage / 1000.0;
    return (float)voltage / 1000.0;
#endif
}
