
// /**
//  * @file solenoid.h
//  * @brief
//  * @public (Drive)stroage interface
//  * @author Mao .
//  * @version 1.0.1
//  * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
//  * @date 2023-07-10
//  */

// // #include "stdint.h"
// // #include "../head_drive/hc595.h"
// #ifndef __SOLENOID_H__
// #define __SOLENOID_H__

// #include "core/fset_lib.h"

// /**
//  * @brief solenoid init
//  *
//  * @return stat_m
//  */
// stat_m m_ext_drive_solenoid_init(int number_of_channels_set, int _device_version_set);

// /**
//  * @brief Turn on the pump { Just open the pin_index and there will be no other actions and no pre_pin_index that is already running }
//  *
//  * @param pin_index
//  * @return stat_m
//  */
// stat_m m_ext_drive_solenoid_open(int pin_index);

// /**
//  * @brief Turn off the pump
//  *
//  * @param pin_index
//  * @return stat_m
//  */
// stat_m m_ext_drive_solenoid_close(int pin_index);
// /**
//  * @brief Continuous operation solenoid {  opens next_pin_index after closing pre_pin_index  }
//  *
//  * @param pre_pin_index will be closed
//  * @param next_pin_index will be opened
//  * @return stat_m
//  */
// stat_m m_ext_drive_solenoid_continuously(int pre_pin_index, int next_pin_index);

// #endif /* __SOLENOID_H__ */
