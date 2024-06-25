#if !defined(SOLENOID_H)
#define SOLENOID_H
#include "stdint.h"
#include "drive/chip_out/ahh5/pca9557.h"
#include "drive/chip_out/hc595/hc595.h"

/**
 * @brief IO驱动初始化
 *
 * @param _device_version_set 1、2、3  ==》 旧版子、新板子、pro机
 * @param number_of_channels_set 4、6、8、12、15、32
 * @param io_chip_set ahh5 or hc595
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_init(uint32_t _device_version_set, int number_of_channels_set, int io_chip_set);

/**
 * @brief 直接打开一个区域
 *
 * @param pin_index
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_open(int pin_index);
/**
 * @brief 打开一个区域,不会影响其他区域
 *
 * @param pin_index
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_direct_open(int pin_index);

/**
 * @brief 关闭IO
 * @param pin_index will be closed
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_close(int pin_index);

/**
 * @brief 关闭上一个，打开下一个
 * @param pre_pin_index will be closed
 * @param next_pin_index will be open
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_continuously(int pre_pin_index, int next_pin_index);

/**
 * @brief IO同时打开两个
 * @param pre_pin_index 上一个IO
 * @param next_pin_index 下一个IO
 * @param on_off 开关控制  开 1   or   关 0
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_together(int pre_pin_index, int next_pin_index, int on_off);

/**
 * @brief 关闭所有 IO 以及 M 端子
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_close_all(void);





/**
 * @brief 直接打开一个区域,其他区域会关闭
 *
 * @param pin_index
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_open_and_close(int pin_index, int is_open);
/**
 * @brief 打开一个区域,不会影响其他区域
 *
 * @param pin_index
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_direct_open_and_close(int pin_index , int is_open);

#endif // SOLENOID_H
