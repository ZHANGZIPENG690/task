#if !defined(SOLENOID_MANAGE_H)
#define SOLENOID_MANAGE_H

// #include "solenoid.h"
// #include "hc595.h"
// #include "stdint.h"
//  #include "../../../application/light/display_manage.h"
// #include "tool_my.h"
#include "core/fset_lib.h"
// // #include "../../drive/chip_out/hc595/hc595.h"
// // #include "../../drive/chip_out/ahh5/pca9557.h"
// // #include "../../drive/chip_out/M_io/m_output.h"
// // #include "../../drive/chip_out/solenoid/solenoid.h"
// // // 全局互斥锁变量
// // SemaphoreHandle_t mutex;
// #include "core/fset_lib.h"


typedef enum _Brightness_level
{
    Brightness_level_1 = 30,
    Brightness_level_2 = 50,
    Brightness_level_3 = 90,
} Brightness_level;


/**
 * @brief solenoid init
 * @param _device_version_set 1、2、3  ==》 旧版子、新板子、pro机
 * @param number_of_channels_set 4、6、8、12、15、32
 * @param io_chip_set ahh5 or hc595
 * @param io_chip
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_init(uint32_t _device_version_set, int number_of_channels_set, int io_chip);

/**
 * @brief 打开某一个IO
 * @param channel
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_open(uint8_t channel);

/**
 * @brief 关闭某一个IO
 * @param channel
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_close(uint8_t channel);
/**
 * @brief Continuous operation solenoid {  opens next_pin_index after closing pre_pin_index  }
 *
 * @param pre_pin_index will be closed
 * @param next_pin_index will be opened
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_continuously(uint8_t pre_pin_index, uint8_t next_pin_index);

/**
 * @brief 同时打开 两个IO
 *
 * @param pre_pin_index will be opened
 * @param next_pin_index will be opened too
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_open_together(uint8_t pre_pin_index, uint8_t next_pin_index);

/**
 * @brief 同时关闭 两个IO
 *
 * @param pre_pin_index will be closed
 * @param next_pin_index will be closed too
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_close_together(uint8_t pre_pin_index, uint8_t next_pin_index);

/**
 * @brief IO管理  属性配置
 *
 * @param number_of_channels_set 通路数量设置
 * @param _device_version_set  版本型号设置
 * @param io_chip_set  芯片型号设置  ahh5 or hc595
 * @return stat_m
 */
stat_m configuration_modification(int number_of_channels_set, uint32_t _device_version_set, int io_chip_set);

/**
 * @brief 关闭所有 IO 以及 M 端子
 * @return stat_m
 */
stat_m m_callable_solenoid_manage_close_all(void);

/**
 * @brief 仅仅打开一个区域不会影响其他区域
 * 
 */
stat_m m_callable_solenoid_manage_direct_open(uint8_t channel);
/**
 * @brief 仅仅关闭一个区域不会影响其他区域
 * 
 */
stat_m m_callable_solenoid_manage_direct_close(uint8_t channel);





#endif // MACRO

