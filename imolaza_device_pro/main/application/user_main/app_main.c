

/**
 * @file app_main.c
 * @author your name (you@domain.com)
 * @brief  函数的主文件
 * @version 0.1
 * @date 2023-10-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "app_main.h"

char result_a[30] = {0};
char result_b[30] = {0};

void m_static_get_device_mac_info()
{
#if !UNIT_TEST
    { //  mac地址
        // uint8_t macc[6];
        esp_efuse_read_field_blob(ESP_EFUSE_MAC_FACTORY, result_a, sizeof(result_a) * 8);

        // for (int i = 0, o = 0; i < 17; i += 3, o++)
        for (int i = 0, o = 0; i < 11; i += 2, o++)
        {
            if (result_a[o] / 16 < 10)

                result_b[i] = result_a[o] / 16 + 48;
            else
                result_b[i] = result_a[o] / 16 + 87;
            if (result_a[o] % 16 < 10)
                result_b[i + 1] = result_a[o] % 16 + 48;
            else
                result_b[i + 1] = result_a[o] % 16 + 87;
            // if (i + 2 <= 15)
            //     mac[i + 2] = 0x3a;
        }
        m_callable_device_attribute_set_mac_addr(result_b);
        bzero(result_a, 30);
        for (size_t i = 0; i < 4; i++)
        {
            result_a[i] = result_b[8 + i];
        }
        result_a[4] = '\0';

        bzero(result_b, 30);
        m_callable_device_attribute_set_mac_suffix_addr(result_a);
        m_callable_device_attribute_get_mac_addr(result_b);

        DEBUG_TEST(DB_W, "mac: %s", result_b);
    }
#else
    strcpy(result_b, "exe17002359");
    strcpy(result_a, "2359");
    m_callable_device_attribute_set_mac_addr(result_b);
    m_callable_device_attribute_set_mac_suffix_addr(result_a);
    DEBUG_TEST(DB_W, "mac: %s", result_b);
#endif
    // DEBUG_TEST(DB_I,"mac:%s", result_b);
}

/** 如果 0 老设备    1 新设备  */
extern int OLD_DEVICE_PEI;

/*
 */
void m_static_get_app_version()
{
#if !UNIT_TEST
    // int count = 0;
    OLD_DEVICE_PEI = 0;
    esp_partition_iterator_t it;
    const esp_app_desc_t *app_desc = esp_ota_get_app_description();
    const esp_partition_t *con = esp_ota_get_boot_partition();
    if (con->address == 0x1c0000)
    {
        OLD_DEVICE_PEI = 1;
    }
    else
    {
        it = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);
        it = esp_partition_next(it);
        const esp_partition_t *con_next = esp_partition_get(it);
        if (con_next->address == 0x1c0000)
        {
            OLD_DEVICE_PEI = 1;
        }
    }
    DEBUG_TEST(DB_W, "OLD_DEVICE_PEI = %d", OLD_DEVICE_PEI);

    m_callable_device_attribute_set_app_version(app_desc->version);
#else
    strcpy(result_b, "9.0.001");
    m_callable_device_attribute_set_app_version(result_b);
#endif
    bzero(result_b, 30);
    // DEBUG_TEST(DB_W,"addr :%06x", con->address);
}

/**
 * @brief 主函数入口
 *
 */
// void app_main()
void app_main()
{
    int ds = 0;
    /** 获取当前的版本信息*/
    m_static_get_app_version();
    /** 获取当前的MAC地址*/
    m_static_get_device_mac_info();
    /** 应用初始化 */
    app_init();

    mDelay_ms(6000);

    // m_callable_device_attribute_all_clear();
    // while (1)
    // {
    //     m_callable_system_printf_task_info();

    //         m_callable_device_proper_status_get(&ds);
    //         if (ds == M_DEVICE_GLOBAL_STATUS_IDLE
    //         && m_callable_timer_manage_get_hhmmss() % 1800 < 1790 &&  m_callable_timer_manage_get_hhmmss() % 1800 > 10
    //         )
    //         {
    //             DEBUG_TEST(DB_W,"- -- - %d - - " ,  m_callable_timer_manage_get_hhmmss() % 1800);
    //             m_callable_drive_button_event_direct_input(M_KEY_EVENT_LEFT);
    //             mDelay_ms(1000);
    //             m_callable_drive_button_event_direct_input(M_KEY_EVENT_START);
    //             mDelay_ms(1000);
    //             m_callable_drive_button_event_direct_input(M_KEY_EVENT_STOP);
    // }
    // mDelay_ms(800);
    // };
}
