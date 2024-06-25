#if !defined(PCA9557_H)
#define PCA9557_H
#include "stdint.h"
#include "core/fset_lib.h"
#include "drive/chip_out/chip_out.h"

#define AD554_1 0x20
#define AD554_2 0x38

#define PCA9557_CONTROL_REG_0 0x00 // Input Port Register           (R)    BXXXXXXXX (Default)
#define PCA9557_CONTROL_REG_1 0x01 // Output Port Register          (R/W)  B00000000
#define PCA9557_CONTROL_REG_2 0x02 // Polarity Inversion Register   (R/W)  B11110000
#define PCA9557_CONTROL_REG_3 0x03 // Configuration Register        (R/W)  B11111111

/**
 * @brief ahh5_init
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_init(void);

/**
 * @brief ahh5_reset
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_reset(void);

/**
 * @brief  通过ahh5 打开IO
 * @param bit
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_solenoidvalve_set(uint8_t bit);

/**
 * @brief ahh5 芯片复位
 * @param bit
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_solenoidvalve_reset(uint8_t bit);

/**
 * @brief 通过ahh5 同时打开两个IO
 * @param bito1
 * @param bit2ss
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_solenoidvalve_two_bit(PinBit bito1, PinBit bit2, uint8_t on_off);

/**
 * @brief 控制某一个IO 是打开还是关闭
 * @param bit
 * @param on_off
 * @return stat_m m_ext_drive_ahh5_solenoidvalve(PinBit bit, uint8_t on_off);

 */
stat_m m_ext_drive_ahh5_solenoidvalve(PinBit bit, uint8_t on_off);


/**
 * @brief 控制某一个IO 是打开还是关闭,并且与其他IO独立，互不影响
 * @param bit
 * @param on_off
 * @return stat_m m_ext_drive_ahh5_solenoidvalve(PinBit bit, uint8_t on_off);
 */
stat_m m_ext_drive_ahh5_solenoidvalve_independent(PinBit bit, uint8_t on_off);

/**
 * @brief 暂时没写 暂时没用上
 * @param pinBit
 * @param x
 * @return esp_err_t
 */
stat_m m_ext_drive_pca9557_setiostate(PinBit pinBit, uint8_t x);

#endif // PCA9557_H
