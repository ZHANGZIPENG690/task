// #if !defined(MDA_ADC_H)
// #define MDA_ADC_H
// #include "core/fset_lib.h"
// #include "../drive.h"
// #if !UNIT_TEST
// #include <string.h>
// #include <stdio.h>
// #include "sdkconfig.h"
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "driver/adc.h"
// #include <stdlib.h>
// #include "driver/adc.h"
// #include "esp_adc_cal.h"
// #endif

// #define TIMES 1024 // 256
// #define GET_UNIT(x) ((x >> 3) & 0x1)

// #define ADC_RESULT_BYTE 2
// #define ADC_CONV_LIMIT_EN 1                  // For ESP32, this should always be set to 1
// #define ADC_CONV_MODE ADC_CONV_SINGLE_UNIT_1 // ESP32 only supports ADC1 DMA mode
// #define ADC_OUTPUT_TYPE ADC_DIGI_OUTPUT_FORMAT_TYPE1
// #define DEFAULT_VREF2 900

// // 用于计算电流 3.36
// #define VCC2 3.36
// #define AD2 4.3
// #define PRECISION2 0.132 // 0.132

// /**
//  * @brief mda_adc init
//  * @return void
//  */
// void m_ext_mda_adc_init(void);

// /**
//  * @brief 直接获取当前电流值
//  * @return float
//  */
// float m_ext_mda_adc_get_current(void);
// #endif // MDA_ADC_H