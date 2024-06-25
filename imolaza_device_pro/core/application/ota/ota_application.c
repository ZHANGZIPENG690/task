#include "ota_application.h"

extern char tmp_pudate_url[120];
stat_m m_callable_ota_init(void)
{
    return m_ext_drive_ota_init();
}

stat_m m_callable_ota_enable_silent_updata(char *in_url)
{
    //  stat_m stat = fail_r;
    // if (m_ext_drive_ota_enable_silent_update(in_url) == succ_r)
    // {
    //     stat = m_ext_ota_event_handle(M_STATIC_OTA_CHECK_APP_VERSION, 0);
    // }
    // return stat;
    return m_ext_drive_ota_enable_silent_update(in_url);
}

stat_m m_callable_ota_check_app_version(void)
{
    // stat_m stat = fail_r;
    // if (m_ext_drive_ota_check_app_version() == succ_r)
    // {
    //     stat = m_ext_ota_event_handle(M_STATIC_OTA_UPDATA_START, 0);
    // }
    // return stat;

    return m_ext_drive_ota_check_app_version();
}

uint64_t ota_start_time = 0;
uint64_t ota_end_time = 0;
int ota_start_flag = 1;

stat_m m_callable_ota_start(void)
{
    stat_m stat = fail_r;

    if (ota_start_flag)
    {
        ota_start_flag = 0;
        ota_start_time = m_callable_timer_manage_get_device_time();
    }

    if (m_ext_drive_ota_start() == succ_r)
    {
        stat = m_ext_ota_event_handle(M_STATIC_OTA_UPDATA_EDN, 0);
    }
    else
    {

        while (1)
        {
            ota_end_time = m_callable_timer_manage_get_device_time();
            if (ota_end_time - ota_start_time >= 300)
            {
                stat = fail_r;
                m_ext_drive_ota_overtime();
            }
            if (m_callable_network_get_net_connect_status() == succ_r)
            {
                if (m_callable_ota_enable_silent_updata(tmp_pudate_url) == succ_r)
                {
                    if (m_callable_ota_check_app_version() == succ_r)
                    {
                        m_callable_device_proper_status_update_to_update();
                        m_callable_ota_start();
                    }
                }
            }

            mDelay_ms(3000);
        }
    }
    return stat;
}

void m_callable_ota_end(void)
{
    m_ext_drive_ota_end();
}

stat_m m_callable_ota_stop(void)
{
    return m_ext_drive_ota_stop();
}

stat_m m_callable_ota_disable_silent_updata()
{
    return m_ext_drive_ota_disable_silent_update();
}

stat_m m_ext_ota_event_handle(uint8_t event_id, char *in_url)
{
    stat_m stat = fail_r;
    switch (event_id)
    {
    case M_STATIC_OTA_UPDATA_INIT:
        stat = m_callable_ota_init();
        break;

    case M_STATIC_OTA_ENABLE_SILENT:
        stat = m_callable_ota_enable_silent_updata(in_url);
        break;

    case M_STATIC_OTA_CHECK_APP_VERSION:
        stat = m_callable_ota_check_app_version();
        break;

    case M_STATIC_OTA_UPDATA_START:
        stat = m_callable_ota_start();
        break;

    case M_STATIC_OTA_UPDATA_EDN:
        m_callable_ota_end();
        stat = succ_r;
        break;

    default:

        break;
    }
    return stat;
}