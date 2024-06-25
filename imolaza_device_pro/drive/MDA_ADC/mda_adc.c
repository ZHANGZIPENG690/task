// #include "mda_adc.h"

// static uint16_t adc1_chan_mask = BIT(7);
// static uint16_t adc2_chan_mask = 0;
// static adc_channel_t channel[1] = {ADC1_CHANNEL_7};

// static float max_voltage = 0.0;
// static float min_voltage = 999.99;
// static float max_current = 0.00;
// static float min_current = 0.00;
// static float max_current_get = 0.00;
// static float min_current_get = 0.00;
// static float avg_current = 0.00;
// static float voltage_float=0.00;
// static uint32_t ret_num = 0;

// static const char *TAG = "ADC DMA";

// static void m_static_mad_adc_continuous_adc_init(uint16_t adc1_chan_mask, uint16_t adc2_chan_mask, adc_channel_t *channel, uint8_t channel_num)
// {
//     adc_digi_init_config_t adc_dma_config = {
//         .max_store_buf_size = 1024,
//         .conv_num_each_intr = TIMES,
//         .adc1_chan_mask = adc1_chan_mask,
//         .adc2_chan_mask = adc2_chan_mask,
//     };
//     adc_digi_initialize(&adc_dma_config);
//     // ESP_ERROR_CHECK(adc_digi_initialize(&adc_dma_config));

//     adc_digi_configuration_t dig_cfg = {
//         .conv_limit_en = ADC_CONV_LIMIT_EN,
//         .conv_limit_num = 250,
//         .sample_freq_hz = 25 * 1000,
//         .conv_mode = ADC_CONV_MODE,
//         .format = ADC_OUTPUT_TYPE,
//     };

//     adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
//     dig_cfg.pattern_num = channel_num;
//     for (int i = 0; i < channel_num; i++)
//     {
//         uint8_t unit = GET_UNIT(channel[i]);
//         uint8_t ch = channel[i] & 0x7;
//         adc_pattern[i].atten = ADC_ATTEN_DB_11;
//         adc_pattern[i].channel = ch;
//         adc_pattern[i].unit = unit;
//         adc_pattern[i].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

//         ESP_LOGI(TAG, "adc_pattern[%d].atten is :%x", i, adc_pattern[i].atten);
//         ESP_LOGI(TAG, "adc_pattern[%d].channel is :%x", i, adc_pattern[i].channel);
//         ESP_LOGI(TAG, "adc_pattern[%d].unit is :%x", i, adc_pattern[i].unit);
//     }
//     dig_cfg.adc_pattern = adc_pattern;
//     adc_digi_controller_configure(&dig_cfg);
//     // ESP_ERROR_CHECK(adc_digi_controller_configure(&dig_cfg));
// }

// void m_ext_mda_adc_init(void)
// {
//     m_static_mad_adc_continuous_adc_init(adc1_chan_mask, adc2_chan_mask, channel, sizeof(channel) / sizeof(adc_channel_t));
// }

// // 计算电压最大值的函数
// float m_static_max_voltage(float voltage1, float voltage2)
// {
//     return (voltage1 > voltage2) ? voltage1 : voltage2;
// }

// // 计算电压最小值的函数
// float m_static_min_voltage(float voltage1, float voltage2)
// {
//     return (voltage1 < voltage2) ? voltage1 : voltage2;
// }

// // ADC值转化成电压值
// float m_ext_drive_adc_to_voltage(int adc_reading, float *voltage_float)
// {
//     esp_adc_cal_characteristics_t adc_chars;
//     esp_adc_cal_value_t cal_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF2, &adc_chars);
//     uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars);
//     return (float)voltage / 1000.0;
// }

// float m_callavle_volatage_transform_current(void)
// {
//     max_current_get = max_voltage;
//     min_current_get = min_voltage;
//     max_voltage = 0.0;
//     min_voltage = 99999.9;
//     max_current = (max_current_get - VCC2 / 2) / AD2 / PRECISION2 * 1000;
//     min_current = (min_current_get - VCC2 / 2) / AD2 / PRECISION2 * 1000;
//     avg_current = (max_current - min_current) / 2;
//     return avg_current;
// }

// float m_ext_mda_adc_get_current(void)
// {
//     esp_err_t ret;
//     uint8_t *result = NULL;
//     result = (uint8_t *)malloc(TIMES * sizeof(uint8_t)); // 这里必须使用动态开辟内存，不然程序会栈溢出崩溃重启
//     if (result != NULL)
//     {
//         // uint8_t result[TIMES] = {0};
//         // memset(result, 0xcc, TIMES);
//         m_static_mad_adc_continuous_adc_init(adc1_chan_mask, adc2_chan_mask, channel, sizeof(channel) / sizeof(adc_channel_t));
//         adc_digi_start();
//         ret = adc_digi_read_bytes(result, TIMES, &ret_num, 0xFFFFFFFFU); // 0xFFFFFFFFU
//         if (ret == ESP_OK || ret == ESP_ERR_INVALID_STATE)
//         {
//             if (ret == ESP_ERR_INVALID_STATE)
//             {
//             }
//             for (int i = 0; i < ret_num; i += ADC_RESULT_BYTE)
//             {
//                 adc_digi_output_data_t *p = (void *)&result[i];
//                 voltage_float = m_ext_drive_adc_to_voltage(p->type1.data, &voltage_float);
//                 max_voltage = m_static_max_voltage(max_voltage, voltage_float);
//                 min_voltage = m_static_min_voltage(min_voltage, voltage_float);
//             }

//             vTaskDelay(1);
//         }
//         else if (ret == ESP_ERR_TIMEOUT)
//         {
//             ESP_LOGW(TAG, "No data, increase timeout or reduce conv_num_each_intr");
//             vTaskDelay(10);
//         }

//         adc_digi_stop();
//         ret = adc_digi_deinitialize();
//         assert(ret == ESP_OK);
//     }

//     free(result);
//     result = NULL;

//     return m_callavle_volatage_transform_current();
// }
