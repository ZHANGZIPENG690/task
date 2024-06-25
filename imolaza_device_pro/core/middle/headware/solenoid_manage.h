






/**
 * @file solenoid_manage_manage.h
 * @brief 
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write 
 * @date 2023-07-10
 * 
 * @code 
 * 


   stat_m m_callable_solenoid_manage_ open / close / continuously (uint8_t channel)
   {

        m_callable_display_status(M_DISPLAY_STATUS_ANIM_WATERING , channel);

        m_ext_drive_solenoid_manage_open(channel);

 * }


 * @endcode
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */
#include "fset_lib.h"
#include "solenoid_manage.h"
#include "light/display.h"


// 主要保证在调用开启端口的时候应该是先打开端口单元，然后设置灯光


/**
 * @brief 
 * 
 * @return stat_m 
 */
stat_m m_callable_solenoid_manage_init(uint32_t _device_version_set, int number_of_channels_set, int io_chip);

/**
 * @brief 
 * 
 * @param channel 
 * @return stat_m 
 */
stat_m m_callable_solenoid_manage_open(uint8_t channel);

/**
 * @brief 
 * 
 * @param channel 
 * @return stat_m 
 */
stat_m m_callable_solenoid_manage_close(uint8_t channel);
/**
 * @brief 
 * 
 * @param channel 
 * @return stat_m 
 */
stat_m m_callable_solenoid_manage_M_open(void);

/**
 * @brief 
 * 
 * @param channel 
 * @return stat_m 
 */
stat_m m_callable_solenoid_manage_M_close(void);


/**
 * @brief 
 * 
 * @param pre_pin_index 
 * @param next_pin_index 
 * @return stat_m 
 */
stat_m m_callable_solenoid_manage_continuously(uint8_t pre_pin_index ,uint8_t next_pin_index);























