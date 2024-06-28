// #include "core/fset_lib.h"
// #include "../drive.h"
// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_task_wdt.h"

// // 定义任务句柄
// TaskHandle_t taskHandle = NULL;

// // 定义看门狗超时时间（单位：毫秒）
// #define WATCHDOG_TIMEOUT 3000

// // 超时处理函数
// void handleWatchdogTimeout()
// {
//     // 在这里执行设备重启的操作
//     printf("Watchdog timeout occurred! Rebooting device...\n");
//     esp_restart();
// }

// // 看门狗任务
// void watchdog_task(void *pvParameters)
// {
//     while (1)
//     {
//         esp_task_wdt_reset();
//         vTaskDelay(pdMS_TO_TICKS(10000));
//         DEBUG_E("esp_task_wdt_status(NULL) %d", esp_task_wdt_status(taskHandle))

//         if (esp_task_wdt_status(taskHandle) != ESP_OK)
//         {
//             handleWatchdogTimeout();
//         }
//     }
// }

// void m_callable_watchdog_task()
// {
//     // 初始化看门狗定时器
//     esp_task_wdt_init(WATCHDOG_TIMEOUT, true);

//     // 创建任务
//     xTaskCreate(watchdog_task, "Watchdog Task", 2048, NULL, 2, &taskHandle);

//     xTaskCreate(task1, "task1", 2048, NULL, 1, NULL);

//     // 将看门狗加到任务
//     esp_task_wdt_add(taskHandle);
// }