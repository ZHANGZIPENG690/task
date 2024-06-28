#include "ota_application.h"

struct ota_manage_m ota_manage;

extern char tmp_pudate_url[120];
void *ota_silent_updating_loop = NULL;
char tmp_pudate_url_get[120];

stat_m m_callable_ota_init(void)
{
    return m_ext_drive_ota_init();
}

stat_m m_callable_ota_enable_silent_updata(char *in_url)
{
    stat_m stat = fail_r;
    stat = m_ext_drive_ota_enable_silent_update(in_url);
    if (stat == succ_r)
    {
        ota_manage.ota_http_flag = true;
    }
    return stat;
}
stat_m m_callable_ota_check_app_version(void)
{
    return m_ext_drive_ota_check_app_version();
}

stat_m m_callable_ota_start(void)
{
    stat_m stat = fail_r;
    // int device_status_o = 0;
    if (ota_manage.ota_start_flag == 0)
    {
        ota_manage.ota_start_flag = 1;
        ota_manage.ota_start_time = m_callable_timer_manage_get_device_time();
    }
    stat = m_ext_drive_ota_start();
    if (stat == succ_r)
    {
        if (m_callable_ota_silent_updata_flag_get() == true)
        {
            m_callable_device_proper_status_get(&ota_manage.device_status_o);

            if (ota_manage.device_status_o == M_DEVICE_GLOBAL_STATUS_IDLE && m_callable_ota_can_start_updating_time_get() == succ_r)
            {
                DEBUG_TEST(DB_W, "OTA静默更新完成,此时设备空闲,且5分钟内不会有计划执行,可以重启");
                stat = m_ext_ota_event_handle(M_STATIC_OTA_UPDATA_EDN, 0);
            }
            else
            {
                if (m_callable_flash_rescue_message_flag(OTA_COMPLETE, M_GLO_STROAGE_RESCUE_MESSAGE_SET) == succ_r)
                {
                    DEBUG_TEST(DB_W, "OTA静默更新完成,做个标记，等待设备空闲时刻进行重启");
                    return succ_r;
                }
                // else
                // {
                //     /*如果标记没能存储成功、但是恰好此时设备处于空闲状态则重启完成更新，否则等待下次静默更新*/
                //     m_callable_device_proper_status_get(&ota_manage.device_status_o);
                //     if (ota_manage.device_status_o == M_DEVICE_GLOBAL_STATUS_IDLE)
                //     {
                //         stat = m_ext_ota_event_handle(M_STATIC_OTA_UPDATA_EDN, 0);
                //     }
                // }
            }
        }
        else
        {
            stat = m_ext_ota_event_handle(M_STATIC_OTA_UPDATA_EDN, 0);
        }
    }
    else if (stat == fail_r)
    {
        while (1)
        {
            ota_manage.ota_end_time = m_callable_timer_manage_get_device_time();
            if (ota_manage.ota_end_time - ota_manage.ota_start_time >= 300)
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
                        if (m_callable_ota_silent_updata_flag_get() != true)
                        {
                            m_callable_device_proper_status_update_to_update();
                            DEBUG_TEST(DB_W, "设备提到OTA更新状态")
                        }
                        if (m_callable_ota_start() == succ_r)
                        {
                            break;
                        }
                    }
                }
            }

            mDelay_ms(3000);
        }
    }
    else
    {
        return disable_r;
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

stat_m m_callable_ota_client_cleanup()
{
    stat_m stat = fail_r;
    stat = m_ext_drive_ota_client_cleanup();
    if (stat == succ_r)
    {
        ota_manage.ota_http_flag = false;
    }
    return stat;
}

/**
 * @brief ota_can_start_updating_time
 * @param new_timestamp
 * @return stat_m
 */
stat_m m_callable_ota_can_start_updating_time_set(uint64_t new_timestamp)
{
    if (ota_manage.min_timestamp == 0)
    {
        ota_manage.min_timestamp = new_timestamp;
    }
    else
    {
        if (new_timestamp < ota_manage.min_timestamp)
        {
            ota_manage.min_timestamp = new_timestamp;
        }
    }
    if (m_callable_timer_manage_get_device_time() > ota_manage.min_timestamp)
    {
        ota_manage.min_timestamp = 0;
    }
    return succ_r;
}

/**
 * @brief ota_can_start_updating_time_get  如果返回succ_r,就可以进行静默更新否则不可以
 * @return stat_m
 */
stat_m m_callable_ota_can_start_updating_time_get(void)
{
    stat_m stat = fail_r;
    if (ota_manage.min_timestamp >= m_callable_timer_manage_get_device_time() + (6 * 60))
    {
        DEBUG_TEST(DB_W, "min_timestamp >")
        stat = succ_r;
    }
    DEBUG_TEST(DB_W, "min_timestamp <  %lld   %lld", ota_manage.min_timestamp, m_callable_timer_manage_get_device_time() + 6 * 60)
    return stat;
}

void *ota_silent_updata_loop = NULL;

void m_static_ota_silent_updata_loop(void *args)
{
    stat_m stat = fail_r;
    bool ota_updata_complete_flag = false;
    int num_resp = 0;
    while (1)
    {
        if (m_callable_ota_silent_updata_stop_flag_get() == true)
        {
            DEBUG_TEST(DB_W, "删除线程删除线程");
            ota_manage.ota_silent_flag = false;
            ota_manage.ota_silent_stop = false;
            mTaskDelete(); // 删除任务自身
        }
        m_callable_device_proper_status_get(&ota_manage.device_status_o);
        DEBUG_TEST(DB_W, "device_status %d", ota_manage.device_status_o);
        if (ota_manage.device_status_o == M_DEVICE_GLOBAL_STATUS_IDLE && m_callable_ota_can_start_updating_time_get() == succ_r)
        {
            if (m_callable_flash_rescue_message_flag(OTA_COMPLETE, M_GLO_STROAGE_RESCUE_MESSAGE_GET) == succ_r)
            {
                DEBUG_TEST(DB_W, "OTA更新完成标记读取成功,且设备5分钟之内没有计划运行,可以进行重启完成分区转换");
                m_callable_ota_end();
            }
        }
        if (ota_updata_complete_flag == false)
        {
            // if (m_callable_ota_can_start_updating_time_get() == succ_r)
            // {
            DEBUG_TEST(DB_W, "OTA静默更新start");
            while (1)
            {
                m_callable_ota_init();
                for (int i = 0; i < 6; i++)
                {
                    if (m_callable_ota_enable_silent_updata(tmp_pudate_url) == succ_r)
                    {
                        if (m_callable_ota_check_app_version() == succ_r)
                        {
                            num_resp = 2;
                            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIRM_UPDATA,
                                                            M_TYPE_Int, (void *)&num_resp,
                                                            M_TYPE_NULL, NULL,
                                                            M_TYPE_NULL, NULL,
                                                            M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time(), true);

                            ota_manage.ota_silent_flag = true;
                            stat = m_callable_ota_start();
                            if (stat == succ_r)
                            {
                                ota_updata_complete_flag = true;
                                break;
                            }
                            else if (stat == disable_r)
                            {
                                break;
                            }
                        }
                        else
                        {
                            ota_manage.ota_silent_flag = false;
                            ota_manage.ota_silent_stop = false;
                            if (m_callable_drive_flash_read(M_TYPE_Str, OTA_URL, tmp_pudate_url) == succ_r)
                            {
                                m_callable_drive_flash_remove(OTA_URL);
                            }
                            DEBUG_TEST(DB_W, "版本一致,无需更新");
                            mTaskDelete(); // 删除任务自身
                        }
                    }
                    else
                    {
                        DEBUG_E("ota http 请求失败");
                        ota_manage.http_connect_count_m++;
                    }
                }
                if (ota_manage.http_connect_count_m >= 5)
                {
                    DEBUG_E("ota http 请求失败 大于等于 5次,设备需要重启");
                    m_ext_drive_ota_overtime();
                }
                if (ota_updata_complete_flag)
                {
                    break;
                }
                if (stat == disable_r)
                {
                    DEBUG_TEST(DB_W, "ota stat == disable_r 删除线程");
                    ota_manage.ota_silent_flag = false;
                    ota_manage.ota_silent_stop = false;
                    mTaskDelete(); // 删除任务自身
                }
                mDelay_ms(5);
            }
            // }
        }

        mDelay_ms(3000);
    }
}
/**
 * @brief m_callable_ota_silent_updata_start_init
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_start_init(uint64_t in_time_s)
{
    stat_m stat = fail_r;
    int otaupdata_fail_count = 0;
    stat = m_callable_drive_flash_read(M_TYPE_Str, OTA_URL, tmp_pudate_url_get);
    if (stat == succ_r)
    {
        if (m_callable_drive_flash_read(M_TYPE_Int, OTA_UPDATA_FAIL_COUNT, &otaupdata_fail_count) == succ_r)
        {
            if (otaupdata_fail_count >= 2)
            {
                DEBUG_E("OTA静默更新连续失败两次、这里重启后取消恢复更新");
                m_callable_drive_flash_remove(OTA_UPDATA_FAIL_COUNT);
                stat = fail_r;
            }
        }
        if (stat == succ_r)
        {
            DEBUG_TEST(DB_W, "OTA更新路径存在,准备在空闲时刻进行OTA静默更新");
            ota_manage.ota_silent_flag = true;
            memcpy(tmp_pudate_url, tmp_pudate_url_get, strlen(tmp_pudate_url_get) + 1);
            /** 连接管理 */
            mTaskCreate(ota_silent_updata_loop, m_static_ota_silent_updata_loop, "ota_silent_updata_loop", 1024 * 5, (void *)3, 5, NULL); //--sock tcp link 5
        }
    }

    return succ_r;
}

/**
 * @brief m_callable_ota_silent_updata_flag_get
 * @return stat_m
 */
bool m_callable_ota_silent_updata_flag_get(void)
{
    return ota_manage.ota_silent_flag;
}

/**
 * @brief ota静默更新完成标记 移除    如果设备意外重启,则移除标记
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_clear_mark(void)
{
    stat_m stat = fail_r;
    if (m_callable_flash_rescue_message_flag(OTA_COMPLETE, M_GLO_STROAGE_RESCUE_MESSAGE_GET) == succ_r)
    {
        if (m_callable_flash_rescue_message_flag(OTA_COMPLETE, M_GLO_STROAGE_RESCUE_MESSAGE_DEL) == succ_r)
        {
            DEBUG_TEST(DB_W, "ota静默更新完成标记 移除成功");
            stat = succ_r;
        }
    }
    return stat;
}

/**
 * @brief ota静默更新  停止
 * @return stat_m
 */
stat_m m_callable_ota_silent_updata_stop(void)
{
    int num_resp = 0;
    if (m_callable_drive_flash_read(M_TYPE_Str, OTA_URL, tmp_pudate_url) == succ_r)
    {
        m_callable_drive_flash_remove(OTA_URL);
    }
    if (ota_manage.ota_silent_flag)
    {
        ota_manage.ota_silent_stop = true;
        DEBUG_TEST(DB_W, "m_callable_ota_silent_updata_stop");
        m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIRM_UPDATA,
                                        M_TYPE_Int, (void *)&num_resp,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time(), true);
    }

    return succ_r;
}

/**
 * @brief ota静默更新  停止标记位获取
 * @return stat_m
 */
bool m_callable_ota_silent_updata_stop_flag_get(void)
{
    return ota_manage.ota_silent_stop;
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
