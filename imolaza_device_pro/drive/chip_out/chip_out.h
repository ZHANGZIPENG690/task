
#ifndef __CHIP_OUT__H__
#define  __CHIP_OUT__H__

#include "../../core/fset_lib.h"

#include "ahh5/pca9557.h"
#include "hc595/hc595.h"
#include "M_io/m_output.h"
#if !UNIT_TEST

#define GPIO_INIT(mode1, bit, up_en, down_en, isr) \
    io_conf.intr_type = isr;                       \
    io_conf.pin_bit_mask = (1ULL << bit);          \
    io_conf.mode = mode1;                          \
    io_conf.pull_up_en = up_en;                    \
    io_conf.pull_down_en = down_en;                \
    gpio_config(&io_conf);

gpio_config_t io_conf;
#endif


/**
 * @brief Turn on the pump { Just open the pin_index and there will be no other actions and no pre_pin_index that is already running }
 *
 * @param pin_index
 * @return stat_m
 */
stat_m m_ext_drive_solenoid_open(int pin_index);


#endif /*  __CHIP_OUT__H__ */