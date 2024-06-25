// #include "ota_m.h"
// #include <string.h>
// #include <sys/param.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_log.h"
// #include "esp_ota_ops.h"
// #include "esp_http_client.h"
// #include "esp_https_ota.h"

// #define TAG "OTA_UPDATE"

// // static int content_length = 0;
// // int ota_start_flag_m = 0;
// // static int data_read = 0;
// // static bool image_header_was_checked = false;
// // static const esp_app_desc_t *desc;
// // static char ota_write_data[1024] = {0};
// // static esp_app_desc_t new_app_info;
// // static esp_ota_handle_t ota_handle;
// // static const esp_partition_t *runing_partition = NULL, *update_partition_2 = NULL;
// // static esp_http_client_handle_t http_client_2 = NULL;

// // esp_err_t http_event_handler(esp_http_client_event_t *evt)
// // {
// //     esp_err_t err = ESP_FAIL;
// //     switch (evt->event_id)
// //     {
// //     case HTTP_EVENT_ERROR:
// //         ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
// //         break;

// //     case HTTP_EVENT_ON_CONNECTED:
// //         ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");

// //         update_partition_2 = esp_ota_get_next_update_partition(NULL);
// //         esp_partition_erase_range(update_partition_2, 0, update_partition_2->size);

// //         assert(update_partition_2 != NULL);
// //         ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%x",
// //                  update_partition_2->subtype, update_partition_2->address);

// //         // OTA 开始
// //         err = esp_ota_begin(update_partition_2, OTA_SIZE_UNKNOWN, &ota_handle);
// //         if (err != ESP_OK)
// //         {
// //             ESP_LOGE(TAG, "OTA begin failed, error=%d", err);
// //             return err;
// //         }

// //         break;
// //     case HTTP_EVENT_HEADER_SENT:
// //         ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");

// //         break;

// //     case HTTP_EVENT_ON_HEADER:
// //         ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
// //         break;

// //     case HTTP_EVENT_ON_DATA:
// //         ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
// //         if (!esp_http_client_is_chunked_response(evt->client))
// //         {
// //             // OTA 写入
// //             err = esp_ota_write(ota_handle, evt->data, evt->data_len);
// //             if (err != ESP_OK)
// //             {
// //                 ESP_LOGE(TAG, "OTA write failed, error=%d", err);
// //                 return err;
// //             }
// //         }
// //         break;
// //     case HTTP_EVENT_ON_FINISH:
// //         ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
// //         m_ext_drive_ota_end();

// //         break;
// //     case HTTP_EVENT_DISCONNECTED:
// //         ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
// //         break;
// //     }

// //     return ESP_OK;
// // }

// // stat_m m_ext_drive_ota_enable_silent_update_2(char *in_url)
// // {
// //     esp_err_t err = ESP_FAIL;
// //     stat_m stat = fail_r;
// //     esp_http_client_config_t config = {
// //         .url = in_url,
// //         .cert_pem = NULL,
// //         .timeout_ms = 25 * 1000,
// //         .event_handler = http_event_handler,
// //     };
// //     http_client_2 = esp_http_client_init(&config);

// //     if (http_client_2 != NULL)
// //     {
// //         err = esp_http_client_perform(http_client_2);
// //         if (err == ESP_OK)
// //         {
// //             stat = succ_r;
// //         }
// //     }

// //     return stat;
// // }

// // void m_ext_drive_ota_end(void)
// // {
// // #if !UNIT_TEST
// //     // OTA 结束
// //     esp_err_t err = esp_ota_end(ota_handle);
// //     if (err != ESP_OK)
// //     {
// //         ESP_LOGE(TAG, "OTA end failed, error=%d", err);
// //         return err;
// //     }
// //     err = esp_ota_set_boot_partition(update_partition_2);
// //     vTaskDelay(10);
// //     DEBUG_TEST( DB_I,OTA updata over,device restar\r\n");
// //     m_ext_drive_ota_disable_silent_update();
// //     esp_restart();
// // #endif
// // }

// // stat_m m_ext_drive_ota_disable_silent_update(void)
// // {
// //     stat_m stat = fail_r;
// // #if !UNIT_TEST
// //     esp_err_t err = -1;
// //     err = esp_http_client_close(http_client_2);
// //     err = esp_http_client_cleanup(http_client_2);
// //     if (err == ESP_OK)
// //     {
// //         stat = succ_r;
// //     }
// //     else
// //     {
// //         stat = fail_r;
// //     }
// // #endif
// //     return stat;
// // }

// //********************************************************************//
// // #include <esp_http_client.h>

// // // OTA 更新的 URL
// // #define OTA_URL "http://your_ota_url"
// // static char ota_write_data[1024] = {0};
// // static const esp_partition_t *runing_partition = NULL, *update_partition_2 = NULL;
// // stat_m m_ext_drive_ota_enable_silent_update_2(char *in_url)
// // {
// //     esp_err_t err = ESP_OK;

// //     esp_http_client_config_t config = {
// //         .url = in_url,
// //         .cert_pem = NULL,
// //         .timeout_ms = 5000, // 设置超时时间
// //         // 其他配置项
// //     };

// //     esp_http_client_handle_t client = esp_http_client_init(&config);
// //     if (client == NULL)
// //     {
// //         ESP_LOGE(TAG, "Failed to initialize HTTP client");
// //         return ESP_FAIL;
// //     }

// //     err = esp_http_client_open(client, 0);
// //     if (err != ESP_OK)
// //     {
// //         ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
// //         esp_http_client_cleanup(client);
// //         return err;
// //     }

// //     esp_http_client_fetch_headers(client);

// //     // 检查响应码是否为 200，表示请求成功
// //     int response_code = esp_http_client_get_status_code(client);
// //     if (response_code != 200)
// //     {
// //         ESP_LOGE(TAG, "HTTP request failed with status code %d", response_code);
// //         esp_http_client_cleanup(client);
// //         return ESP_FAIL;
// //     }

// //     // 获取 OTA 信息中的固件大小
// //     int content_length = esp_http_client_get_content_length(client);
// //     if (content_length <= 0)
// //     {
// //         ESP_LOGE(TAG, "Invalid content length");
// //         esp_http_client_cleanup(client);
// //         return ESP_FAIL;
// //     }

// //     update_partition_2 = esp_ota_get_next_update_partition(NULL);
// //     esp_partition_erase_range(update_partition_2, 0, update_partition_2->size);

// //     assert(update_partition_2 != NULL);
// //     ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%x",
// //              update_partition_2->subtype, update_partition_2->address);

// //     // 创建 OTA 数据分区
// //     esp_ota_handle_t ota_handle;
// //     err = esp_ota_begin(update_partition_2, OTA_SIZE_UNKNOWN, &ota_handle);
// //     if (err != ESP_OK)
// //     {
// //         ESP_LOGE(TAG, "Failed to begin OTA update: %s", esp_err_to_name(err));
// //         esp_http_client_cleanup(client);
// //         return err;
// //     }

// //     // 读取 OTA 数据并写入分区
// //     int binary_written = 0;
// //     while (binary_written < content_length)
// //     {
// //         int data_read = esp_http_client_read(client, ota_write_data, sizeof(ota_write_data));
// //         if (data_read <= 0)
// //         {
// //             break; // 读取数据完成或出错
// //         }

// //         err = esp_ota_write(ota_handle, ota_write_data, data_read);
// //         if (err != ESP_OK)
// //         {
// //             ESP_LOGE(TAG, "Failed to write OTA data: %s", esp_err_to_name(err));
// //             esp_http_client_cleanup(client);
// //             return err;
// //         }

// //         binary_written += data_read;
// //     }

// //     esp_http_client_cleanup(client);

// //     // 结束 OTA 更新
// //     err = esp_ota_end(ota_handle);
// //     if (err != ESP_OK)
// //     {
// //         ESP_LOGE(TAG, "Failed to end OTA update: %s", esp_err_to_name(err));
// //         return err;
// //     }

// //     // 切换到新固件分区
// //     err = esp_ota_set_boot_partition(esp_ota_get_next_update_partition(NULL));
// //     if (err != ESP_OK)
// //     {
// //         ESP_LOGE(TAG, "Failed to set boot partition: %s", esp_err_to_name(err));
// //         return err;
// //     }

// //     ESP_LOGI(TAG, "OTA update complete");

// //     return ESP_OK;
// // }
