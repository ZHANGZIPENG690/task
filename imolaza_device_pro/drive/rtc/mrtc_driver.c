#include "mrtc_driver.h"

#include "core/fset_lib.h"
#if !UNIT_TEST
#include "driver/i2c.h"
#include "esp_log.h"
#include "time.h"
// 定义I2C总线
static i2c_port_t i2c_num = I2C_NUM_0;
#endif

/**
 * @brief Write time to RTC
 *
 * @param timestamp
 * @return bool
 */
bool m_ext_drive_rtc_set_time(int year, int month, int tm_day, int tm_hour, int tm_min, int tm_sec, int tm_wday)
{
#if !UNIT_TEST
    // 将时间转换为十六进制格式
    uint8_t time_data[7];

    time_data[0] = ((tm_sec / 10) << 4) | (tm_sec % 10);
    time_data[1] = ((tm_min / 10) << 4) | (tm_min % 10);
    time_data[2] = ((tm_hour / 10) << 4) | (tm_hour % 10);
    time_data[3] = ((tm_day / 10) << 4) | (tm_day % 10);
    time_data[4] = ((tm_wday / 10) << 4) | (tm_wday % 10);
    time_data[5] = ((month / 10) << 4) | (month % 10);
    if (year < 2000)
    {
        time_data[6] = ((99 / 10) << 4) | (99 % 10); // set year to 2099
    }
    else
    {
        time_data[6] = (((year - 2000) / 10) << 4) | ((year - 2000) % 10);
    }
    DEBUG_TEST(DB_W, "m_ext_drive_rtc_set_time7777777");
    // 将转换后的数据写入PCF8563寄存器中
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, PCF8563_ADDR << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x02, true); // PCF8563年份寄存器地址
    i2c_master_write(cmd, time_data, 7, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    DEBUG_TEST(DB_W, "m_ext_drive_rtc_set_time8888888");
    return (ret == ESP_OK) ? true : false;
#endif
}

// 实现获取时间函数
bool m_ext_drvie_rtc_get_time(uint64_t *out_timestamp)
{
    stat_m stat = fail_r;
#if !UNIT_TEST
    struct tm timestamp;
    uint8_t data[7];

    // stat = m_callable_iic_read(0, 0x02, &data, 7, 2);
    // 从PCF8563寄存器中读取时间数据
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, PCF8563_ADDR << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x02, true); // PCF8563年份寄存器地址
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, PCF8563_ADDR << 1 | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, 7, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    {
        DEBUG_TEST(DB_E, "Failed to read time from PCF8563\r\n");
        return false;
    }

    // 将读取的数据转换为十进制格式并写入结构体中
    timestamp.tm_sec = (((data[0] >> 4) & 0x07) * 10) + (data[0] & 0x0F);
    timestamp.tm_min = (((data[1] >> 4) & 0x07) * 10) + (data[1] & 0x0F);
    timestamp.tm_hour = (((data[2] >> 4) & 0x03) * 10) + (data[2] & 0x0F);
    timestamp.tm_mday = (((data[3] >> 4) & 0x03) * 10) + (data[3] & 0x0F);
    // timestamp->weekday = data[4] & 0x07;
    timestamp.tm_mon = (((data[5] >> 4) & 0x01) * 10) + (data[5] & 0x0F);
    timestamp.tm_year = ((data[6] >> 4) & 0x0F) * 10 + (data[6] & 0x0F) + 2000;
    timestamp.tm_year -= 1900; // 年份从1900年开始计算
    timestamp.tm_mon -= 1;     // 月份从0开始计算

    *out_timestamp = mktime(&timestamp);

    DEBUG_TEST(DB_I, "Current time is: %04d-%02d-%02d %02d:%02d:%02d, weekday %d            %lld",
               timestamp.tm_year + 1900, timestamp.tm_mon + 1, timestamp.tm_mday,
               timestamp.tm_hour, timestamp.tm_min, timestamp.tm_sec, timestamp.tm_wday, *out_timestamp);

#endif
    return stat;
}

stat_m m_ext_read_rtc_vl_flag(void)
{
    uint8_t vlFlag = -1;
    // 发送读取指令以及VL位的地址
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (PCF8563_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x02, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (PCF8563_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &vlFlag, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    DEBUG_TEST(DB_I, "02h寄存器读取的值为: 0x%02x", vlFlag);
    if (vlFlag & 0x80)
    {
        DEBUG_TEST(DB_I, "VL位的值为 1");
        return fail_r;
    }
    else
    {
        DEBUG_TEST(DB_I, "VL位的值为 0");
        return succ_r;
    }
}

// // 实现设置时间函数
// stat_m m_callable_midlle_rtc_set_time(pcf8563_time_t *time)
// {
//     stat_m stat = fail_r;
//     if (m_ext_drive_rtc_set_time(&time) == true)
//     {
//         stat = succ_r;
//     }
//     else
//     {
//         stat = fail_r;
//     }
//     return stat;
// }

// // 实现获取时间函数
// stat_m m_callable_midlle_rtc_get_time(pcf8563_time_t *timestamp)
// {
//     stat_m stat = fail_r;
//     if (m_ext_drvie_rtc_get_time(&timestamp) == true)
//     {
//         stat = succ_r;
//     }
//     else
//     {
//         stat = fail_r;
//     }
//     return stat;
// }

// void m_static_rtc_time_runing_task(void *pvParameters)
// {
//     while (1)
//     {
//         pcf8563_time_t time;
//         m_callable_midlle_rtc_get_time(&time);
//         printf("Current time is: %04d-%02d-%02d %02d:%02d:%02d, weekday %d\n",
//                time.year, time.month, time.day,
//                time.hour, time.minute, time.second, time.weekday);
//         mDelay_ms(1000);
//     }
// }

// stat_m m_callable_midllle_rtc_init(void)
// {
//     m_callable_iic_init();

//     // pcf8563_time_t time;
//     // time.year = 2023;
//     // time.month = 10;
//     // time.day = 26;
//     // time.hour = 16;
//     // time.minute = 22;
//     // time.second = 0;
//     // time.weekday = 4; // 周五
//     // m_callable_midlle_rtc_set_time(&time);

//     // printf("Current time is: %04d-%02d-%02d %02d:%02d:%02d, weekday %d\n",
//     //        time.year, time.month, time.day,
//     //        time.hour, time.minute, time.second, time.weekday);

//     mTaskCreate(NULL, m_static_rtc_time_runing_task, "m_static_rtc_time_runing_task", 2048, NULL, 2, NULL);
//     return succ_r;
// }
