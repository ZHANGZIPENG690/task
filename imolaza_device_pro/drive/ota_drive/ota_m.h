#if !defined(OTA_M_H)
#define OTA_M_H

#include "core/fset_lib.h"
#if !UNIT_TEST

#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "nvs.h"
#include "esp_system.h"
#endif

// #define OTA_URL "http://device.imolaza.com/firmware/hot_so_fast_1_6_006.bin" // OTA服务器固件下载链接
// #define BUFFERSIZE 1024
#if !UNIT_TEST

// OTA更新断点续传的相关信息
typedef struct
{
    char char_value[100]; // OTA升级文件的下载地址
    int ota_range;        // 断点处的字节位置
} ota_breakpoint_resume_t;
ota_breakpoint_resume_t breakpoint_resume;
#endif

/** OTA错误处理 */
enum m_ota_error_handling
{
    ora_file_not_found,
    ota_read_erro,
    ota_write_erro,

};

/** OTA管理 */
struct m_ota_manage
{
    int data_read;
    int update_buf_offset;
    int total_read;
    float ota_percent;
    float ota_b;
    long content_length;
    esp_app_desc_t new_app_info;
    const esp_app_desc_t *desc;
};
struct m_ota_manage ota_man;

/**
 * @brief ota_init
 * @return stat_m
 */
stat_m m_ext_drive_ota_init(void);



/**
 * @brief ota_start
 * @return esp_err_t
 */
stat_m m_ext_drive_ota_start(void);

/**
 * @brief ota_enable_silent_update
 * @return esp_err_t
 */
stat_m m_ext_drive_ota_enable_silent_update(char *in_url);

/**
 * @brief ota_disable_silent_update
 * @return esp_err_t
 */
stat_m m_ext_drive_ota_disable_silent_update(void);

/**
 * @brief ota_stop
 * @return esp_err_t
 */
stat_m m_ext_drive_ota_stop(void);

/**
 * @brief OTA_check_app_version
 * @return stat_m
 */
stat_m m_ext_drive_ota_check_app_version(void);
/**
 * @brief ota_end
 * @return stat_m
 */
void m_ext_drive_ota_end(void);

stat_m m_static_drive_ota_re_read_attempt(void);

stat_m m_static_drive_ota_write_err_handle(void);

stat_m m_ext_drive_ota_erro_handle(enum m_ota_error_handling ota_error);

void m_ext_drive_ota_overtime(void);

void print_partition_table();
#endif // OTA_M_H
