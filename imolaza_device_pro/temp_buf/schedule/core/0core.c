
// #include "fset_lib.h"

// #include <pthread.h>

// // #include "freertos/event_groups.h"
// // #include "esp_event_base.h"

// // static EventGroupHandle_t s_floop_event_group = NULL;

// void mDelay_ms(uint32_t in_ms)
// {

//     // vTaskDelay(in_ms / portTICK_RATE_MS);
//     // vTaskDelay(in_ms / portTICK_PERIOD_MS);
// }

// void mTaskCreate(void (*fun)(void *), const char *pcName, const uint32_t stack_depth, void *const pvParameters, int uxPriority, void *pvCreatedTask)
// {
//     pthread_t this;
//     struct sched_param sp;
//     bzero((void*)&sp, sizeof(sp));
//     sp.sched_priority = SCHED_OTHER;
//     pthread_create(&this , NULL , (void *)fun  ,NULL);
//     pthread_setschedparam(this, sp.sched_priority, &sp);
//     // xTaskCreate(fun, pcName, stack_depth, pvParameters, uxPriority, pvCreatedTask);
//     // xTaskCreate(task_handler_sw, "task_handler_sw", 1024 * 7, (void *)1, 2, NULL);
// }

// void mReboot(void)
// {
//     /*** close fun*/

//     /*** shutdown now*/
//     // esp_restart();
// }

// void mEventGroupReSetBits(const int thisBit)
// {
//     // xEventGroupClearBits(s_floop_event_group, thisBit);
// }
// void mEventGroupSetBits(const int thisBit)
// {
//     // xEventGroupSetBits(s_floop_event_group, thisBit);
// }

// int mEventGroupWaitBits(const int thisBit, bool waitForAllBits)
// {
//     // if (s_floop_event_group == NULL)
//     //     s_floop_event_group = xEventGroupCreate();
//     uint32_t uxBits  =0;   
//     //  = xEventGroupWaitBits(s_floop_event_group, thisBit, true, waitForAllBits, portMAX_DELAY);

//     return uxBits;
// }


