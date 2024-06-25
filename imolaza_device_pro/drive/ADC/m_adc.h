#if !defined(M_ADC_H)
#define M_ADC_H
#include <string.h>
#include "core/fset_lib.h"
#if !UNIT_TEST
#include "driver/adc.h"
#include "esp_adc_cal.h"
#endif

#define ADC_CHANNEL ADC1_CHANNEL_7
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define DEFAULT_VREF 900


/**
 * @brief adc init
 * @return void
 */
void m_ext_drive_adc_init();

/**
 * @brief adc read value
 * @return int
 */
int m_ext_drive_read_adc_value();

/**
 * @brief adc read value
 * @param adc_reading
 * @param voltage_float
 * @return float
 */
float m_ext_drive_adc_to_voltage(int adc_reading, float *voltage_float);


//**************************************************************************************


#define RTC_ADC_CHANNEL ADC1_CHANNEL_0
// #define RTC_ADC_ATTEN ADC_ATTEN_DB_11
// #define RTC_ADC_WIDTH ADC_WIDTH_BIT_12
#define RTC_DEFAULT_VREF 900


void m_ext_drive_adc_init_rtc();

// 读取ADC值的函数
int m_ext_drive_read_adc_value_rtc();

// ADC值转化成电压值
float m_ext_drive_adc_to_voltage_rtc(int adc_reading, float *voltage_float);

#endif // ADC_CURRENT_H
