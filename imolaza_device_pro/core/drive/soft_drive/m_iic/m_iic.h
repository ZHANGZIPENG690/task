

#if !defined(M_IIC_H)
#define M_IIC_H

#include "core/fset_lib.h"

/**
 * @brief IIC 协议初始化
 *
 * @return stat_m
 */
stat_m m_callable_iic_init(void);
/**
 * @brief IIC 写入
 *
 * @param u8I2cSlaveAddr
 * @param u8Cmd
 * @param u8Value
 * @param iic_select
 * @return stat_m
 */
stat_m m_callable_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value);

/**
 * @brief IIC 读取
 *
 * @param u8I2cSlaveAddr
 * @param u8Cmd
 * @param pBuff
 * @param u8Cnt
 * @param iic_select
 * @return stat_m
 */
stat_m m_callable_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt);
#endif // M_IIC_H
