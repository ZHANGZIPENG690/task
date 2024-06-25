




#ifndef __DRIVE_H__
#define __DRIVE_H__

#include "core/fset_lib.h"

#if !UNIT_TEST
#include "esp_efuse.h"
#include "esp_http_server.h"
#include "sdkconfig.h"
#include "esp_efuse.h"
#include <assert.h>
#include "esp_efuse_table.h"
#include <driver/gpio.h>
// #include "core/utils/util.h"
// #include "core/timer/mtime.h"
// #include "core/utils/text.h"
// #include "key/key.h"
// #include "core/net/mnet.h"

#include "lwip/ip_addr.h"
#include "lwip/api.h"
#include "lwip/dns.h"
#include "lwip/sockets.h"
#include "lwip/apps/sntp.h"
#include <netinet/in.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_netif.h"
#include <stdio.h>
#include <string.h>
#include <esp_err.h>
#include "driver/i2c.h"

gpio_config_t io_conf;

#define GPIO_INIT(mode1, bit, up_en, isr) \
    io_conf.intr_type = isr;              \
    io_conf.pin_bit_mask = (1ULL << bit); \
    io_conf.mode = mode1;                 \
    io_conf.pull_up_en = up_en;           \
    gpio_config(&io_conf);

#endif


struct receive_data
{
    char inout_data[512];
    int inout_data_len;
    bool is_receive;
};
/**
 * @brief wifi 软件驱动初始化
 * 
 * @return stat_m 
 */
stat_m m_ext_soft_drive_net_wifi_init(void);

/**
 * @brief 蓝牙 软件驱动初始化
 * 
 * @return stat_m 
 */
stat_m m_ext_soft_drive_net_bluetool_init(void);




#endif  /* __DRIVE_H__ */
