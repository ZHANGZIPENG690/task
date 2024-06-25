// #include "current_monitor.h"

// #include <stdio.h>
// #include <stdlib.h>
// // #include <pthread.h>

// extern float max_avg_current;

// uint8_t num2 = -1;
// float param = 0;
// float param2 = 0;

// extern float max_avg_current;
// extern uint8_t is_interrupt;
// extern uint8_t channel_id;
// extern float max_voltage;
// extern float min_voltage;
// extern int current_monitor_flag;
// void uart_task()
// {

//     scanf("%hhd,%f,%f", &num2, &param, &param2);
//     // printf("You entered: %d %.03f %.03f,\n", num2, param, param2);
//     max_voltage = param;
//     min_voltage = param2;
//     // printf("最大电压:%.03f \r\n", max_voltage);
//     // printf("最小电压:%.03f \r\n", min_voltage);
//     switch (num2)
//     {
//     case 0:
//         m_callable_current_monitor_stop();
//         break;
//     case 1:
//         m_callable_current_monitor_start();
//         break;
//     case 2:
//         if (m_callable_volatage_transform_current(1200) == succ_r)
//         {
//             printf("检测成功\r\n");
//         }
//         else
//         {
//             printf("检测失败\r\n");
//         }
//         break;
//     default:
//         printf("模式选择无效..\r\n");
//         break;
//     }
//     // vTaskDelay(2);
//     fflush(stdin);
// }

// int main(void)
// {
//     while (1)
//     {
//         uart_task();
//     }
//     return 0;
// }