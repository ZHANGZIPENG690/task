#if !defined(OTA_APPLICATION_H)
#define OTA_APPLICATION_H
#include "core/fset_lib.h"
#include "core/text.h"
enum ota_updata_status
{
    M_STATIC_OTA_UPDATA_INIT,
    M_STATIC_OTA_UPDATA_START,
    M_STATIC_OTA_ENABLE_SILENT,
    M_STATIC_OTA_CHECK_APP_VERSION,
    M_STATIC_OTA_UPDATA_STOP,
    M_STATIC_OTA_UPDATA_RUNNING,
    M_STATIC_OTA_UPDATA_RECOVER,
    M_STATIC_OTA_UPDATA_EDN
};

struct ota_manage_m
{
    bool ota_silent_flag;
    bool ota_http_flag;
    bool ota_silent_stop;
    int ota_start_flag;
    int http_connect_count_m;
    int device_status_o;
    uint64_t ota_start_time;
    uint64_t ota_end_time;
    uint64_t min_timestamp;
};

stat_m m_callable_ota_init(void);
/**
 * @brief ota_enable_silent_update
 * @return stat_m
 */
stat_m m_callable_ota_enable_silent_updata(char *in_url);
/**
 * @brief ota_disable_silent_update
 * @return stat_m
 */
stat_m m_callable_ota_disable_silent_updata(void);

/**
 * @brief ota_start
 * @return stat_m
 */
stat_m m_callable_ota_start(void);

/**
 * @brief ota_stop
 * @return stat_m
 */
stat_m m_callable_ota_stop(void);

/**
 * @brief ota_update_main_entrance
 * @param in_url
 * @return stat_m
 */
stat_m m_callable_ota_update_main_entrance(char *in_url);

/**
 * @brief ota_update_main_entrance
 * @param in_url
 * @return stat_m
 */
stat_m m_callable_ota_check_app_version(void);

/**
 * @brief ota_update_progress_get
 * @return float
 */
float m_callable_ota_updata_progress(void);

stat_m m_ext_ota_event_handle(uint8_t event_id, char *in_url);

void m_callable_ota_end(void);

/**
 * @brief ota_can_start_updating_time
 * @param new_timestamp
 * @return stat_m
 */
stat_m m_callable_ota_can_start_updating_time_set(uint64_t new_timestamp);

/**
 * @brief ota_can_start_updating_time_get  如果返回succ_r,就可以进行静默更新否则不可以
 * @return stat_m
 */
stat_m m_callable_ota_can_start_updating_time_get(void);

/**
 * @brief m_callable_ota_silent_updata_start_init
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_start_init(uint64_t in_time_s);

/**
 * @brief m_callable_ota_silent_updata_flag_get
 * @return stat_m
 */
bool m_callable_ota_silent_updata_flag_get(void);

/**
 * @brief m_callable_ota_client_cleanup
 * @return stat_m
 */
stat_m m_callable_ota_client_cleanup();

/**
 * @brief ota静默更新完成标记 移除    如果设备意外重启,则移除标记
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_clear_mark(void);


/**
 * @brief ota静默更新  停止
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_stop(void);


/**
 * @brief ota静默更新  停止标记位获取
 * @return stat_m
 */
bool m_callable_ota_silent_updata_stop_flag_get(void);

#endif // OTA_APPLICATION_H
