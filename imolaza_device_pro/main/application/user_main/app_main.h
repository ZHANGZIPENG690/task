























#ifndef __APP_MAIN__H
#define __APP_MAIN__H

#include "core/fset_lib.h"
#if !UNIT_TEST
#include <stdio.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_efuse.h"
#include "esp_efuse_table.h"
#include "stdatomic.h"
#include "hal/spi_hal.h"
#include <esp_ota_ops.h>
#include "driver/spi_common_internal.h"

#endif
/**
 * @brief 应用初始化
 * 
 */
void app_init(void);

#endif 












