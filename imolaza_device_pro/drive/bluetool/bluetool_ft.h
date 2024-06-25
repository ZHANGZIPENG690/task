





#ifndef __BLUETOOL_FT_H__
#define  __BLUETOOL_FT_H__
/**  */
#include "core/fset_lib.h"
/**  */
#if !UNIT_TEST
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include "sdkconfig.h"
#endif
#include "../drive.h"


#define PROFILE_NUM 2
#define PROFILE_A_APP_ID 0
#define PROFILE_B_APP_ID 1
#define ESP_GATT_IF_NONE 0xff

#define GATTS_SERVICE_UUID_TEST_A 0x00FF
#define GATTS_CHAR_UUID_TEST_A 0xFF01
#define GATTS_DESCR_UUID_TEST_A 0x3333
#define GATTS_NUM_HANDLE_TEST_A 4

#define GATTS_SERVICE_UUID_TEST_B 0x00EE
#define GATTS_CHAR_UUID_TEST_B 0xEE01
#define GATTS_DESCR_UUID_TEST_B 0x2222
#define GATTS_NUM_HANDLE_TEST_B 4

#define ESP_GATT_UUID_CHAR_CLIENT_CONFIG 0x2902

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40

#define PREPARE_BUF_MAX_SIZE 1024
#if !UNIT_TEST

static uint8_t adv_config_done = 0;
#define adv_config_flag (1 << 0)
#define scan_rsp_config_flag (1 << 1)


struct gatts_profile_inst
{
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t tan_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

#endif
typedef struct
{
    uint8_t *prepare_buf;
    int prepare_len;
} prepare_type_env_t;





stat_m m_ext_soft_drive_net_bluetool_init(void);

/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
stat_m m_ext_net_link_bluetool_receive_data(char *out_data, int *len,uint64_t  in_time_out_s);

#endif  /* __BLUETOOL_FT_H__ */