

#include "core/fset_lib.h"
#include "../drive.h"
#if !UNIT_TEST
esp_timer_handle_t timer_handle = 0;
#endif

uint64_t ins = 0;



















/**
 * @brief 时间回馈
 * 
 * @param arg 
 */
void timer_callback_fun(void *arg)
{

    m_callable_timer_manage_time_update();

   
}

/***********************************
 *
 * @brief 定时器初始化
 *
 * @param void
 *
 * @return void
 *************************************/
void m_ext_drive_timer_init(void)
{
#if !UNIT_TEST
    esp_timer_init();

    esp_timer_create_args_t timer_args;
    timer_args.callback = &timer_callback_fun;
    timer_args.arg = NULL;
    timer_args.dispatch_method = 0;
    timer_args.name = "timer_one";
    esp_timer_create(&timer_args, &timer_handle);
    // esp_timer_stop();
    esp_timer_start_periodic(timer_handle, 1 * 1000 * 1000);
    // esp_timer_start_periodic(timer_handle , 10*500*1000);
    return;
#else 

#endif
}
