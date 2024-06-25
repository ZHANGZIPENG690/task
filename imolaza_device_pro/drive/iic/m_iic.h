

#ifndef M_IIC___H
#define M_IIC___H

#include <stdio.h>
// #if !UNIT_TEST

// #endif
// #include <unistd.h>
// #include "string.h"
// #include "stdint.h"
#include "../drive.h"

#define I2C_MASTER_SCL_IO 17      /*!< GPIO number for I2C master clock */
#define I2C_MASTER_SDA_IO 16      /*!< GPIO number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C主模式I2C端口号 */
// #define I2C_MASTER_FREQ_HZ 100000 /*!< I2C主模式时钟频率 */
#define ACK_CHECK_EN 0x1
#define WRITE_BIT I2C_MASTER_WRITE

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
 * @brief IIC驱动初始化
 * @return stat_m
 */
stat_m m_ext_drive_iic_init(void);

/**
 * @brief IIC写入驱动
 * @param u8I2cSlaveAddr
 * @param u8Cmd
 * @param u8Value
 * @return stat_m
 */
stat_m m_ext_driver_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value);

/**
 * @brief IIC 读取驱动
 * @param u8I2cSlaveAddr
 * @param u8Cmd
 * @param pBuff
 * @param u8Cnt
 * @return stat_m
 */
stat_m m_ext_driver_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt);

// stat_m m_ext_drive_rtc_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value);
// stat_m m_ext_driver_rtc_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt);

#endif // M_IIC_H
