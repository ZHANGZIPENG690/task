#if !defined(OTA_APPLICATION_H)
#define OTA_APPLICATION_H
#include "core/fset_lib.h"

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

#endif // OTA_APPLICATION_H
