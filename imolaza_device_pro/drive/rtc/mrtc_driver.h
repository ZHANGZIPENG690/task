#if !defined(MRTC_H)
#define MRTC_H
#include "core/fset_lib.h"

// #include <string.h>
// #include "core/fset_lib.h"
// #if !UNIT_TEST
// #include "driver/adc.h"
// #include "esp_adc_cal.h"
// #endif

/**
 * @file mrtc.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */
/**
 * @brief Initialize the rtc
 */

#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_SCL_IO 17
#define I2C_MASTER_SDA_IO 16
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000 // 400000

// 定义PCF8563_I2C地址
#define PCF8563_ADDR 0x51

// 定义PCF8563寄存器常量
#define PCF8563_CTRL_STATUS1_REG 0x00
#define PCF8563_CTRL_STATUS2_REG 0x01

// 定义PCF8563 Control/Status 1 Register的比特位
#define PCF8563_CTRL_STATUS1_TE_BIT 7   // Timer Enable
#define PCF8563_CTRL_STATUS1_TD1_BIT 5  // Timer-Rate Select 1
#define PCF8563_CTRL_STATUS1_TD0_BIT 4  // Timer-Rate Select 0
#define PCF8563_CTRL_STATUS1_TF_BIT 3   // Timer Flag
#define PCF8563_CTRL_STATUS1_AF_BIT 2   // Alarm Flag
#define PCF8563_CTRL_STATUS1_AIE_BIT 1  // Alarm Interrupt Enable
#define PCF8563_CTRL_STATUS1_STOP_BIT 0 // Disable Timer

// 定义PCF8563 Control/Status 2 Register的比特位
#define PCF8563_CTRL_STATUS2_TI_TP_BIT 7 // Time/Timer Select
#define PCF8563_CTRL_STATUS2_TIIE_BIT 6  // Timer Interrupt Enable
#define PCF8563_CTRL_STATUS2_TLIE_BIT 5  // Timer Low Interrupt Enable
#define PCF8563_CTRL_STATUS2_AIE_BIT 2   // Alarm Interrupt Enable
#define PCF8563_CTRL_STATUS2_AF_BIT 1    // Alarm Flag
#define PCF8563_CTRL_STATUS2_TF_BIT 0    // Timer Flag


/**
 * @brief Write time to RTC
 *
 * @param timestamp
 * @return bool
 */
bool m_ext_drive_rtc_set_time(int year, int month, int tm_day, int tm_hour, int tm_min, int tm_sec, int tm_wday);
/**
 * @brief Get the time from the RTC
 *
 * @param timestamp
 * @return bool
 */
bool m_ext_drvie_rtc_get_time(uint64_t *out_timestamp);

// /**
//  * @brief RTC init
//  * @return stat_m
//  */
// stat_m m_callable_midllle_rtc_init(void);

// /**
//  * @brief Write time to RTC
//  *
//  * @param timestamp
//  * @return stat_m
//  */
// stat_m m_callable_midlle_rtc_set_time(pcf8563_time_t *time);
// /**
//  * @brief Get the time from the RTC
//  *
//  * @param timestamp
//  * @return stat_m
//  */
// stat_m m_callable_midlle_rtc_get_time(pcf8563_time_t *timestamp);

// /**
//  * @brief RTC 线程  内部自己调用
//  * @param a
//  * @return void
//  */
// void m_static_rtc_time_runing_task(void *pvParameters);

#endif // MRTC_H
