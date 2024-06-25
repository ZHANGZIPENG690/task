#if !defined(HC595_H)
#define HC595_H
#if !UNIT_TEST
#include <driver/gpio.h>
#endif
#include "core/fset_lib.h"
#include "../chip_out.h"

#define DATA_PIN 18
#define CLOCK_PIN 4
#define LATCH_PIN 2
#define HC595_MODEL_EN 25



#define HC_8_ZONE 8
#define HC_4_ZONE 4
#define HC_16_ZONE 16
#define HC_12_ZONE 12
#define HC_32_ZONE 32

/**
 * @brief hc595 初始化
 * @return stat_m
 */
stat_m m_ext_drive_hc595_init(void);

/**
 * @brief  控制hc595某一个引脚的电平高低
 * @param value
 * @return stat_m
 */
stat_m m_ext_driver_hc595_shiftout(int value);

/**
 * @brief 控制某一个IO 打开与关闭
 * @param bit
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_hc595_solenoidvalve(PinBit bit, uint8_t on_off);

/**
 * @brief hc595 芯片复位  关闭所有IO
 * @return stat_m
 */

/**
 * @brief 控制某一个IO 打开与关闭
 * @param bit
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_hc595_solenoidvalve_independent(PinBit bit, uint8_t on_off);

stat_m m_ext_drive_hc595_shiftout_reset(void);

/**
 * @brief hc595 水锤 控制
 * @param value
 * @return stat_m
 */
stat_m m_ext_drive_hc595_hammer_shiftout(int value);

/**
 * @brief 通过hc595 控制两个 IO同时打开或者关闭
 * @param bito1
 * @param bito2
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_hc595_solenoidvalve_two_bit(PinBit bito1, PinBit bito2, uint8_t on_off);

#endif // HC595_H
