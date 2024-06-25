#include "core/fset_lib.h"
#if !UNIT_TEST
#include "driver/i2c.h"
#endif

#include "./m_iic.h"

bool printf_err_log = true;
uint8_t iop = 0;
bool iic_initialized = false; // IIC初始化标志位

stat_m m_ext_drive_iic_init(void)
{
#if !UNIT_TEST
    if (!iic_initialized)
    {
        static i2c_config_t conf;
        memset(&conf, 0, sizeof(i2c_config_t));
        int i2c_master_port = I2C_MASTER_NUM;
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = I2C_MASTER_SDA_IO; //
        conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        conf.scl_io_num = I2C_MASTER_SCL_IO; //
        conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        conf.master.clk_speed = 200 * 1000;
        esp_err_t esr = i2c_param_config(i2c_master_port, &conf);
        esr = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
        esr = i2c_filter_enable(i2c_master_port, 4);
        iic_initialized = true; // 设置初始化标志位
        DEBUG_TEST(DB_I,"err: %s", esp_err_to_name(esr));
    }
#endif
    return succ_r;
}

stat_m m_ext_drive_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value)
{
#if !UNIT_TEST
    i2c_cmd_handle_t is31_cmd = i2c_cmd_link_create();
    i2c_master_start(is31_cmd);
    ESP_ERROR_CHECK(i2c_master_write_byte(is31_cmd, u8I2cSlaveAddr, ACK_CHECK_EN));
    ESP_ERROR_CHECK(i2c_master_write_byte(is31_cmd, u8Cmd, ACK_CHECK_EN));
    i2c_master_write_byte(is31_cmd, u8Value, ACK_CHECK_EN);
    i2c_master_stop(is31_cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, is31_cmd, 1000 / 1);
    i2c_cmd_link_delete(is31_cmd);
    // DEBUG_TEST( DB_I,"%s", esp_err_to_name(ret));
#endif
    return succ_r;
}

stat_m m_ext_driver_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt)
{
#if !UNIT_TEST
    esp_err_t ret;

    i2c_cmd_handle_t wr_cmd = i2c_cmd_link_create();
    i2c_master_start(wr_cmd);
    i2c_master_write_byte(wr_cmd, (u8I2cSlaveAddr << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(wr_cmd, u8Cmd, ACK_CHECK_EN);
    i2c_master_stop(wr_cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, wr_cmd, 1000 / ((TickType_t)1000 / 100));
    i2c_cmd_link_delete(wr_cmd);
    if (ret != ESP_OK)
    {
        return fail_r;
    }
    // vTaskDelay(30 / portTICK_RATE_MS);
    i2c_cmd_handle_t rd_cmd = i2c_cmd_link_create();
    i2c_master_start(rd_cmd);
    i2c_master_write_byte(rd_cmd, (u8I2cSlaveAddr << 1) | WRITE_BIT, ACK_CHECK_EN);
    ret = i2c_master_read(rd_cmd, pBuff, u8Cnt, I2C_MASTER_LAST_NACK);

    i2c_master_stop(rd_cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, rd_cmd, 1000 / ((TickType_t)1000 / 100));

    i2c_cmd_link_delete(rd_cmd);

#endif
    return succ_r;
}

// stat_m m_ext_drive_rtc_iic_write(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t u8Value)
// {
//     // 将转换后的数据写入PCF8563寄存器中
//     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//     i2c_master_start(cmd);
//     i2c_master_write_byte(cmd, PCF8563_ADDR << 1 | 0, true);
//     i2c_master_write_byte(cmd, 0x02, true); // PCF8563年份寄存器地址
//     i2c_master_write(cmd, u8Value, 7, true);
//     i2c_master_stop(cmd);
//     esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
//     i2c_cmd_link_delete(cmd);
//     return succ_r;
// }

// stat_m m_ext_driver_rtc_iic_read(uint8_t u8I2cSlaveAddr, uint8_t u8Cmd, uint8_t *pBuff, uint8_t u8Cnt)
// {
//     // 从PCF8563寄存器中读取时间数据
//     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//     i2c_master_start(cmd);
//     i2c_master_write_byte(cmd, PCF8563_ADDR << 1 | 0, true);
//     i2c_master_write_byte(cmd, 0x02, true); // PCF8563年份寄存器地址
//     i2c_master_start(cmd);
//     i2c_master_write_byte(cmd, PCF8563_ADDR << 1 | 1, true);
//     i2c_master_read(cmd, &pBuff, 7, 0x2);
//     i2c_master_stop(cmd);
//     esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
//     i2c_cmd_link_delete(cmd);
//     if (ret != ESP_OK)
//     {
//         DEBUG_TEST( DB_I,"Failed to read time from PCF8563");
//         return fail_r;
//     }
//     return succ_r;
// }
