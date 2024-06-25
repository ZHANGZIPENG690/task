#include "m_iic.h"

bool iic_write_flag = true;
bool iic_read_flag = true;
extern bool iic_initialized;
stat_m m_callable_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value)
{
    stat_m sta = fail_r;
    // while (!iic_write_flag)
    //     ;
    iic_write_flag = false;

    // m_ext_drive_iic_write(u8I2cSlaveAddr, u8Cmd, u8Value);
    sta = m_ext_drive_iic_write(u8I2cSlaveAddr, u8Cmd, u8Value);
    iic_write_flag = true;

    return sta;
    // return m_ext_drive_iic_write(u8I2cSlaveAddr, u8Cmd, u8Value);
}

stat_m m_callable_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt)
{
    stat_m sta = fail_r;
    // while (!iic_read_flag)
    //     ;
    iic_read_flag = false;

    m_ext_driver_iic_read(u8I2cSlaveAddr, u8Cmd, pBuff, u8Cnt);
    iic_read_flag = true;

    return sta;
}

stat_m m_callable_iic_init(void)
{
    if (!iic_initialized)
    {
        return m_ext_drive_iic_init();
    }
    return succ_r;
}
