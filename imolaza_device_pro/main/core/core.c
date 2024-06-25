
#include "core/fset_lib.h"

#include <pthread.h>
#include <signal.h>

#if !UNIT_TEST
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "freertos.h"
#include "freertos/event_groups.h"
#include "esp_event_base.h"
#include "esp_log.h"
#include "core/text.h"

static EventGroupHandle_t s_floop_event_group = NULL;
#endif

int frist_mem = 0;

#if UNIT_TEST
int global_bus[30] = {0};

int AAA = 0;
int BBB = 0;
int CCC = 0;
int DDD = 0;
int EEE = 0;
int FFF = 0;
int GGG = 0;

void sig_end(int sig) {}

#endif
/**
 * @brief 电流检测延时
 *
 * @param in_ms
 */
void mDelay_ms_precision(uint32_t in_ms)
{
    vTaskDelay(pdMS_TO_TICKS(in_ms));
}
void mDelay_ms(uint32_t in_ms)
{
#if !UNIT_TEST
    // vTaskDelay(in_ms / portTICK_PERIOD_MS);
    if (in_ms <= 5)
    {
        vTaskDelay(9 / portTICK_RATE_MS);
    }
    else if (in_ms < 10)
        vTaskDelay(1);
    else
        vTaskDelay(in_ms / portTICK_RATE_MS);

#else
    // signal(SIGALRM,sig_end);
    // alarm(1);
    // pause();
    usleep(in_ms * 1000);
#endif
}

void *mTaskCreate(void *handl, void (*fun)(void *), const char *pcName, const uint32_t stack_depth, void *const pvParameters, int uxPriority, void *pvCreatedTask)
{
#if UNIT_TEST
    pthread_t this;
    struct sched_param sp;
    bzero((void *)&sp, sizeof(sp));
    sp.sched_priority = SCHED_OTHER;
    pthread_create(&this, NULL, (void *)fun, NULL);
    pthread_setschedparam(this, sp.sched_priority, &sp);
#else
    if (handl != NULL)
        ;
    {
        // s_floop_event_group =
        // handl =
        // xEventGroupCreate();
        //  s_floop_event_group;
    }
    if (s_floop_event_group == NULL)
        s_floop_event_group = xEventGroupCreate();
    xTaskCreate(fun, pcName, stack_depth, pvParameters, uxPriority, pvCreatedTask);
    // xTaskCreate(task_handler_sw, "task_handler_sw", 1024 * 7, (void *)1, 2, NULL);
    return NULL;
#endif
}
/**
 * @brief 获取时间戳
 *
 * @return uint64_t
 */
uint64_t mDeviceSystemTime(void)
{
    return esp_log_timestamp();
}

/**
 * @brief 重启函数
 *
 * @param cause 重启原因
 */
void mReboot(enum restart_cause_t cause)
{
    frist_mem = 0;
    DEBUG_TEST(DB_I, "Shutdown !");
    if (m_callable_drive_flash_read(M_TYPE_Int, RESTART_REUSE, &frist_mem) == fail_r)
    {
        if (frist_mem != cause)
        {
            m_callable_drive_flash_write(M_TYPE_Int, RESTART_REUSE, &frist_mem);
        }
    }
#if UNIT_TEST
    exit(0);
#else
    esp_restart();
#endif
}

void mEventGroupReSetBits(void *event_loop, const int thisBit)
{
#if UNIT_TEST
    global_bus[thisBit] = false;
    if (thisBit == 4)
        AAA = 0;
    else if (thisBit == 5)
        BBB = 0;
    else if (thisBit == 6)
        CCC = 0;
    else if (thisBit == 3)
        DDD = 0;
    else if (thisBit == 7)
        EEE = 0;
#else
    // xEventGroupClearBits(((EventGroupHandle_t)event_loop), thisBit);
    xEventGroupClearBits(s_floop_event_group, thisBit);
#endif
}
void mEventGroupSetBits(void *event_loop, const int thisBit)
{
#if UNIT_TEST
    global_bus[thisBit] = true;
    // printf("--允许-- >    %d", thisBit);
    if (thisBit == 4)
        AAA = 1;
    else if (thisBit == 5)
        BBB = 1;
    else if (thisBit == 6)
        CCC = 1;
    else if (thisBit == 3)
        DDD = 1;
    else if (thisBit == 7)
        EEE = 1;
#else
    // if (s_floop_event_group == NULL)
    //     s_floop_event_group = xEventGroupCreate();
    // xEventGroupSetBits(((EventGroupHandle_t)event_loop), thisBit);
    xEventGroupSetBits(s_floop_event_group, thisBit);
#endif
}

int m_callable_event_group_wait_bits(void *event_loop, const int thisBit, int waitForAllBits)
{
    uint32_t uxBits = 0;

#if UNIT_TEST
    // printf("............等待.. %d (%d) \n", thisBit, global_bus[thisBit]);
    if (thisBit == 4)
        while (!AAA)
            ;
    else if (thisBit == 5)
        while (!BBB)
            ;
    else if (thisBit == 6)
        while (!CCC)
            ;
    else if (thisBit == 3)
        while (!DDD)
            0;
    else if (thisBit == 7)
        while (!EEE)
            ;
            // printf(".............突破.. %d (%d) \n", thisBit, global_bus[thisBit]);
#else
    if (s_floop_event_group == NULL)
        printf("lplp\n");
    //     s_floop_event_group = xEventGroupCreate();
    uxBits = xEventGroupWaitBits(s_floop_event_group, thisBit, true, waitForAllBits, portMAX_DELAY);
    // uxBits = xEventGroupWaitBits(((EventGroupHandle_t)event_loop), thisBit, true, waitForAllBits, portMAX_DELAY);
#endif

    return uxBits;
}
/**
 * @brief 获取堆栈使用情况
 *
 * @param out_heap_size 栈内存
 * @param min_heap_size 堆内存
 */
void m_callable_system_printf_to_remote_heap_free_info(int *out_heap_size, int *min_heap_size)
{
    if (out_heap_size != NULL)
    {
        *out_heap_size = esp_get_free_heap_size();
    }
    if (min_heap_size != NULL)
    {
        *min_heap_size = esp_get_minimum_free_heap_size();
    }
}

void m_callable_system_printf_heap_free_info(const char *fun, int line)
{
    DEBUG_TEST(DB_I, "%s - %d  \t {%d} esp_get_free_heap_size : %d ,, esp_get_minimum_free_heap_size : %d ", fun, line, 0, esp_get_free_heap_size(), esp_get_minimum_free_heap_size());

    if (esp_get_minimum_free_heap_size() < 6000)
    {
        int temp_use_int_vue_c = 100;
        m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO, M_TYPE_Int, &temp_use_int_vue_c,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL,
                                        M_TYPE_NULL, NULL, esp_get_minimum_free_heap_size(), false);

        mReboot(M_RESTART_CAUSE_HEAP_SPACE_LACK);
    }
}

#include "esp_heap_task_info.h"
/**
 * @brief 打印资源占用 任务
 *
 */
void m_callable_system_printf_task_info()
{
    // heap_task_stat_t tstat = {0};
    // bool begin = true;
    // printf("Task Heap Utilisation Stats:\n");
    // size_t ret = heap_caps_get_next_task_stat(&tstat, begin);
    // if (ret == 0)
    // {
    //     printf("\n");
    //     break;
    // }
    // const char *task_name = tstat.task ? pcTaskGetTaskName(tstat.task) : "Pre-Scheduler allocs";
    // if (!strcmp(task_name, "wifi") || !strcmp(task_name, "tiT") || !strcmp(task_name, "aws_iot_task"))
    // {
    //     printf("||\t%-12s\t|\t%-5d\t\t|\t%-5d\t\t|| \n",
    //            task_name, tstat.peak[0], tstat.peak[1]);
    // }
}
// #if !UNIT_TEST
// static portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

// void m_callable_exclusive_lock_enter(void)
// {
//     taskENTER_CRITICAL(&myMutex);
// }

// void m_callable_exclusive_lock_exit(void)
// {
//     taskEXIT_CRITICAL(&myMutex);
// }

// // 离开临界区
// #endif
