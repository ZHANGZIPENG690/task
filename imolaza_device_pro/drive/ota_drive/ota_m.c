#include "ota_m.h"
#if !UNIT_TEST

static bool image_header_was_checked = false;
static char ota_write_data[1024] = {0};
int expected_data_size = 1024; // 假设期望读取的数据大小为 1000 字节
int ota_start_flag_m = 0;

const esp_partition_t *runing_partition = NULL, *update_partition = NULL;
esp_http_client_handle_t http_client = NULL;

#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "esp_partition.h"
#include "esp_log.h"
// void print_partition_table()
// {
//     const esp_partition_t *partition = NULL;
//     const esp_partition_t *iterator = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
//     if (iterator != NULL)
//     {
//         while ((partition = esp_partition_next(iterator)) != NULL)
//         {
//             printf("Partition Label: %s, Type: %d, Subtype: %d, Size: %d bytes\n",
//                    partition->label, partition->type, partition->subtype, partition->size);
//         }
//         // esp_partition_iterator_release(iterator);
//     }
// }

// Get the string name of type enum values used in this example
static const char *get_type_str(esp_partition_type_t type)
{
    switch (type)
    {
    case ESP_PARTITION_TYPE_APP:
        return "ESP_PARTITION_TYPE_APP";
    case ESP_PARTITION_TYPE_DATA:
        return "ESP_PARTITION_TYPE_DATA";
    default:
        return "UNKNOWN_PARTITION_TYPE"; // type not used in this example
    }
}

// Get the string name of subtype enum values used in this example
static const char *get_subtype_str(esp_partition_subtype_t subtype)
{
    switch (subtype)
    {
    case ESP_PARTITION_SUBTYPE_DATA_NVS:
        return "ESP_PARTITION_SUBTYPE_DATA_NVS";
    case ESP_PARTITION_SUBTYPE_DATA_PHY:
        return "ESP_PARTITION_SUBTYPE_DATA_PHY";
    case ESP_PARTITION_SUBTYPE_APP_FACTORY:
        return "ESP_PARTITION_SUBTYPE_APP_FACTORY";
    case ESP_PARTITION_SUBTYPE_DATA_FAT:
        return "ESP_PARTITION_SUBTYPE_DATA_FAT";
    default:
        return "UNKNOWN_PARTITION_SUBTYPE"; // subtype not used in this example
    }
}

// Find the partition using given parameters
static void find_partition(esp_partition_type_t type, esp_partition_subtype_t subtype, const char *name)
{
    DEBUG_TEST(DB_W, "Find partition with type %s, subtype %s, label %s...", get_type_str(type), get_subtype_str(subtype),
               name == NULL ? "NULL (unspecified)" : name);

    const esp_partition_t *part = esp_partition_find_first(type, subtype, name);

    if (part != NULL)
    {
        DEBUG_TEST(DB_W, "\tfound partition '%s' at offset 0x%x with size 0x%x", part->label, part->address, part->size);
    }
    else
    {
        DEBUG_TEST(DB_W, "\tpartition not found!");
    }
}

void print_partition_table()
{
    DEBUG_TEST(DB_W, "----------------Find partitions---------------");

    // Find partitions using esp_partition_find_first(). This returns the first partition matching the passed constraints.
    find_partition(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
    find_partition(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_PHY, NULL);
    find_partition(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);
    find_partition(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);

    DEBUG_TEST(DB_W, "Find second FAT partition by specifying the label");
    // In case of multiple matches, `esp_partition_find_first` returns the first match.
    find_partition(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, "storage2");

    /* Second Part - Iterating over partitions */

    DEBUG_TEST(DB_W, "----------------Iterate through partitions---------------");

    esp_partition_iterator_t it;

    DEBUG_TEST(DB_W, "Iterating through app partitions...");
    it = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);

    // Loop through all matching partitions, in this case, all with the type 'data' until partition with desired
    // label is found. Verify if its the same instance as the one found before.
    for (; it != NULL; it = esp_partition_next(it))
    {
        const esp_partition_t *part = esp_partition_get(it);
        DEBUG_TEST(DB_W, "\tfound partition '%s' at offset 0x%x with size 0x%x", part->label, part->address, part->size);
    }
    // Release the partition iterator to release memory allocated for it
    // esp_partition_iterator_release(it);

    DEBUG_TEST(DB_W, "Iterating through data partitions...");
    it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);

    // Loop through all matching partitions, in this case, all with the type 'data' until partition with desired
    // label is found. Verify if its the same instance as the one found before.
    for (; it != NULL; it = esp_partition_next(it))
    {
        const esp_partition_t *part = esp_partition_get(it);
        DEBUG_TEST(DB_W, "\tfound partition '%s' at offset 0x%x with size 0x%x", part->label, part->address, part->size);
    }

    // Release the partition iterator to release memory allocated for it
    // esp_partition_iterator_release(it);

    DEBUG_TEST(DB_W, "Example end");
}

stat_m m_ext_drive_ota_init(void)
{
#if !UNIT_TEST
    ota_man.desc = esp_ota_get_app_description();
    const esp_partition_t *configured = esp_ota_get_boot_partition();

    if (runing_partition == NULL)
    {
        runing_partition = esp_ota_get_running_partition();

        if (configured != runing_partition)
        {
            DEBUG_TEST(DB_W, "Configured OTA boot partition at offset 0x%08x, but running from offset 0x%08x",
                       configured->address, runing_partition->address);
            DEBUG_TEST(DB_W, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
        }

        update_partition = esp_ota_get_next_update_partition(NULL);

        // esp_err_t err = esp_partition_erase_range(update_partition, 0, update_partition->size);
        // if (err != ESP_OK)
        // {
        //     printf("Failed to erase partition (%s) \n", esp_err_to_name(err));
        // }
        DEBUG_TEST(DB_E, "configured->address :%x ------ runing_partition->address: %x ----", configured->address, runing_partition->address);

        DEBUG_TEST(DB_E, "update_partition->address :%x ------ update_partition->size: %x ----", update_partition->address, update_partition->size);

        esp_partition_erase_range(update_partition, 0, update_partition->size);
    }
#endif
    return succ_r;
}

stat_m m_ext_drive_ota_check_app_version(void)
{
    stat_m stat = fail_r;
#if !UNIT_TEST

    memset(ota_write_data, 0, sizeof(ota_write_data));
    ota_man.data_read = esp_http_client_read(http_client, ota_write_data, sizeof(ota_write_data));
    if (ota_man.data_read > 0)
    {
        if (ota_man.content_length == 0)
        {
            ota_man.content_length = esp_http_client_get_content_length(http_client);
            if (ota_man.content_length <= 0)
            {
                printf("Failed to get content length\n");
                esp_http_client_cleanup(http_client);
            }
        }
        printf("OTA file size: %ld bytes\n", ota_man.content_length);
        if (!image_header_was_checked && breakpoint_resume.ota_range == 0)
        {
            memcpy(&ota_man.new_app_info, &ota_write_data[sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t)], sizeof(esp_app_desc_t));
            DEBUG_E("new ver :%s\r\n", ota_man.new_app_info.version);
            DEBUG_E("present ver :%s\r\n", ota_man.desc->version);
            if (ota_man.data_read >= sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t))
            {
                int n = strcmp(ota_man.new_app_info.version, ota_man.desc->version);
                DEBUG_TEST(DB_I, "-%d--%d-", n, sizeof(ota_man.new_app_info.secure_version));
                if (n == 0)
                {
                    DEBUG_TEST(DB_I, "----No updates are available----");
                    esp_http_client_cleanup(http_client);
                    stat = fail_r;
                }
                else
                {
                    DEBUG_TEST(DB_I, "how mach:%s\r\n", ota_man.new_app_info.date);
                    DEBUG_TEST(DB_I, "Writing to partition subtype %d at offset 0x%x",
                               update_partition->subtype, update_partition->address);
                    stat = succ_r;
                }
            }
        }
        else if (ota_start_flag_m == 1)
        {
            stat = succ_r;
        }
    }
#endif
    return stat;
}

stat_m m_ext_drive_ota_start(void)
{

    stat_m stat = succ_r;
#if !UNIT_TEST
    esp_err_t err = ESP_OK;
    ota_start_flag_m = 1;
    image_header_was_checked = true;
    while (ota_man.data_read > 0)
    {
        err = esp_partition_write(update_partition, ota_man.update_buf_offset, ota_write_data, ota_man.data_read);
        if (err != ESP_OK)
        {
            return m_ext_drive_ota_erro_handle(ota_write_erro);
        }

        ota_man.update_buf_offset += ota_man.data_read;

        ota_man.ota_b = (float)ota_man.update_buf_offset / ota_man.content_length;
        ota_man.ota_percent = ota_man.ota_b * 100;
        if (ota_man.ota_percent > 100)
        {
            DEBUG_E("ota_percent err %d", ota_man.update_buf_offset);
            DEBUG_E("content_length err %ld", ota_man.content_length);
        }

        if ((int)ota_man.ota_percent % 10 == 0)
        {
            DEBUG_TEST(DB_I, "Percentage: %.2f%%", ota_man.ota_percent);
        }
        m_callable_display_status(M_DISPLAY_FIRMWARE_UPDATE_MODE, (int)ota_man.ota_percent);
        breakpoint_resume.ota_range = ota_man.update_buf_offset;
        if (ota_man.update_buf_offset >= ota_man.content_length)
        {
            stat = succ_r;
            DEBUG_TEST(DB_I, "content_length == ota_man.update_buf_offset");
            return stat;
        }
        ota_man.data_read = esp_http_client_read_response(http_client, ota_write_data, sizeof(ota_write_data));
        if (ota_man.data_read < 1024 && (ota_man.data_read + ota_man.update_buf_offset) < ota_man.content_length)
        {
            if (m_ext_drive_ota_erro_handle(ota_read_erro) == succ_r)
            {
            }
            else
            {
                return fail_r;
            }
        }
    }

    if (ota_man.data_read <= 0)
    {
        DEBUG_TEST(DB_I, " <= 0 ");
        if (breakpoint_resume.ota_range < ota_man.content_length && !esp_http_client_is_complete_data_received(http_client))
        {
            m_ext_drive_ota_disable_silent_update();
            memset(ota_write_data, 0, sizeof(ota_write_data));
            stat = fail_r;
            return stat;
        }
    }

    if (image_header_was_checked)
    {
        memset(ota_write_data, 0, sizeof(ota_write_data));
        if (ota_man.update_buf_offset < ota_man.content_length - 5)
        {
            stat = fail_r;
        }
    }
    if (err == ESP_OK)
    {
        stat = succ_r;
        DEBUG_TEST(DB_I, "ota updata end!!!\r\n");
    }

    DEBUG_TEST(DB_I, "updata : %d K", ota_man.update_buf_offset / 1024);
#endif

    return stat;
}

void m_ext_drive_ota_end(void)
{
#if !UNIT_TEST
    esp_err_t err = esp_ota_set_boot_partition(update_partition);
    vTaskDelay(10);
    DEBUG_TEST(DB_I, "OTA updata over,device restar\r\n");
    m_ext_drive_ota_disable_silent_update();
    esp_restart();
#endif
}

stat_m m_ext_drive_ota_enable_silent_update(char *in_url)
{
    int retry_request = 0;
    stat_m stat = succ_r;
#if !UNIT_TEST

    esp_http_client_config_t config = {
        .url = in_url,
        .timeout_ms = 25 * 1000,
    };
    DEBUG_TEST(DB_I, "%s", in_url);

    // 打开HTTP客户端连接
    http_client = esp_http_client_init(&config);

    if (http_client != NULL)
    {
        stat = succ_r;
    }
    if (breakpoint_resume.ota_range != 0)
    {
        sprintf(breakpoint_resume.char_value, "bytes=%d-%ld", breakpoint_resume.ota_range, ota_man.content_length);
        esp_http_client_set_header(http_client, "Range", breakpoint_resume.char_value);
        DEBUG_TEST(DB_I, "Range :(%s)  {%d}\r\n", breakpoint_resume.char_value, breakpoint_resume.ota_range);
    }

    if (esp_http_client_open(http_client, 0) != ESP_OK)
    {
        m_ext_drive_ota_disable_silent_update();
        DEBUG_E("esp_http_client_open fail");
        breakpoint_resume.ota_range = ota_man.update_buf_offset;
        stat = fail_r;
        return stat;
    }

    while (esp_http_client_fetch_headers(http_client) == ESP_FAIL)
    {
        /*错误处理*/
        retry_request++;
        if (retry_request >= 3)
        {
            retry_request = 0;
            stat = fail_r;
            break;
        }
        DEBUG_E("esp_http_client_fetch_headers(http_client)  fail!!");
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    if (esp_http_client_get_status_code(http_client) == 404)
    {
        DEBUG_E("404   file not found !!");
        stat = fail_r;
    }

#endif
    return stat;
}
stat_m m_ext_drive_ota_disable_silent_update(void)
{
    stat_m stat = fail_r;
#if !UNIT_TEST
    esp_err_t err = -1;
    err = esp_http_client_close(http_client);
    err = esp_http_client_cleanup(http_client);
    if (err == ESP_OK)
    {
        stat = succ_r;
    }
    else
    {
        stat = fail_r;
    }
#endif
    return stat;
}
stat_m m_ext_drive_ota_stop(void)
{
#if !UNIT_TEST
    // OTA更新完成
    return m_ext_drive_ota_disable_silent_update();
#endif
}

stat_m m_ext_drive_ota_erro_handle(enum m_ota_error_handling ota_error)
{
    stat_m stat = fail_r;
    switch (ota_error)
    {
    case ora_file_not_found:
        /* code */
        break;
    case ota_read_erro:
        stat = m_static_drive_ota_re_read_attempt();
        break;
    case ota_write_erro:
        stat = m_static_drive_ota_write_err_handle();
        break;

    default:
        break;
    }
    return stat;
}

stat_m m_static_drive_ota_re_read_attempt(void)
{
    stat_m stat = fail_r;
    int read_count = 0;
    DEBUG_TEST(DB_W, "data_read err: %d \r\n", ota_man.data_read);

    while (ota_man.total_read < expected_data_size)
    {
        ota_man.total_read += ota_man.data_read;
        DEBUG_TEST(DB_W, "total_read :%d \r\n", ota_man.total_read);
        if (ota_man.total_read == 1024)
        {
            read_count = 0;
            ota_man.data_read = ota_man.total_read;
            ota_man.total_read = 0;
            stat = succ_r;
            break;
        }
        if (read_count >= 2)
        {
            break;
        }
        // esp_http_client_read
        ota_man.data_read = esp_http_client_read_response(http_client, &ota_write_data[ota_man.total_read], sizeof(ota_write_data) - ota_man.total_read);
        read_count++;
    }
    if (read_count >= 2)
    {
        read_count = 0;
        ota_man.data_read = ota_man.total_read;
        ota_man.total_read = 0;
        DEBUG_TEST(DB_I, "重新读取3次、仍然未成功,准备退出OTA更新");

        memset(ota_write_data, 0, sizeof(ota_write_data));
        breakpoint_resume.ota_range = ota_man.update_buf_offset;
        DEBUG_TEST(DB_I, "记录当前读取位置 %d", breakpoint_resume.ota_range);

        m_ext_drive_ota_disable_silent_update();
        stat = fail_r;
        return stat;
    }
    return stat;
}

stat_m m_static_drive_ota_write_err_handle(void)
{
    memset(ota_write_data, 0, sizeof(ota_write_data));
    DEBUG_E("write fail");
    breakpoint_resume.ota_range = ota_man.update_buf_offset;
    return fail_r;
}

void m_ext_drive_ota_overtime(void)
{
#if !UNIT_TEST

    DEBUG_E("OTA updata overtime !!!!!!!!\r\n");
    DEBUG_E("准备重启\r\n");

    esp_restart();
#endif
}

/*err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &ota_handle);
        err = esp_ota_write_with_offset(ota_handle, ota_man.update_buf_offset, ota_write_data, ota_man.data_read);
         ota_man.data_read = esp_http_client_read_response(http_client, ota_write_data, sizeof(ota_write_data));*/