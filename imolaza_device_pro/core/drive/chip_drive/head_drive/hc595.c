// #include <stdio.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <driver/gpio.h>
// #include "hc595.h"

// uint64_t hc_flag = 0x800000; // 16
// // uint64_t hc_flag = 0x800; // 4
// int chips595_lead = 16;
// uint8_t device_type = 7;
// int io_nums;
// extern int *number_of_channels;

// void HC595_init(void)
// {
//     GPIO_INIT(GPIO_MODE_OUTPUT, HC595_MODEL_EN, 0, 1, GPIO_PIN_INTR_DISABLE); // EN
//     gpio_set_level(HC595_MODEL_EN, 0);
//     GPIO_INIT(GPIO_MODE_OUTPUT, CLOCK_PIN, 1, 0, GPIO_PIN_INTR_DISABLE); // OUT
//     gpio_set_level(CLOCK_PIN, 1);
//     GPIO_INIT(GPIO_MODE_OUTPUT, LATCH_PIN, 1, 0, GPIO_PIN_INTR_DISABLE); // DIN
//     gpio_set_level(LATCH_PIN, 1);
    // GPIO_INIT(GPIO_MODE_OUTPUT, DATA_PIN, 1, 0, GPIO_PIN_INTR_DISABLE); // CLK
//     gpio_set_level(DATA_PIN, 0);
// }

// void shiftOut(int value)
// {
//     for (int i = 1; i <= io_nums; i++, value <<= 1)
//     {
//         gpio_set_level(CLOCK_PIN, 0);
//         if (value & hc_flag)
//         {
//             gpio_set_level(DATA_PIN, 1);
//             printf("1");
//         }
//         else
//         {
//             gpio_set_level(DATA_PIN, 0);
//             printf("0");
//         }
//         gpio_set_level(CLOCK_PIN, 1);
//     }
//     printf("\r\n");
//     gpio_set_level(LATCH_PIN, 0);

//     gpio_set_level(LATCH_PIN, 1);
// }

// void HC595_SolenoidValve(PinBit bit, uint8_t on_off)
// {
//     int hc595_vue = 0x100;
//     bit -= 1;
//     if (on_off)
//     {
//         hc595_vue <<= bit;
//         shiftOut_reset();
//         shiftOut(hc595_vue);
//     }
//     else
//     {
//         shiftOut(0x0000);
//     }
// }

// void shiftOut_reset(void)
// {
//     for (int i = 1; i <= chips595_lead; i++)
//     {
//         gpio_set_level(CLOCK_PIN, 0);
//         vTaskDelay(pdMS_TO_TICKS(1));
//         gpio_set_level(DATA_PIN, 0);
//         vTaskDelay(pdMS_TO_TICKS(1));
//         gpio_set_level(CLOCK_PIN, 1);
//     }
//     printf("\r\n");
//     gpio_set_level(LATCH_PIN, 0);
//     gpio_set_level(LATCH_PIN, 1);
// }

// void hammer_shiftout(int value)
// {
//     for (int i = 1; i <= 16; i++)
//     {
//         gpio_set_level(CLOCK_PIN, 0);
//         if (i < 16 + 1 - io_nums)
//         {
//             gpio_set_level(DATA_PIN, 0);
//             printf("0");
//         }
//         else
//         {
//             if (value & hc_flag)
//             {
//                 gpio_set_level(DATA_PIN, 1);
//                 printf("1");
//             }
//             else
//             {
//                 gpio_set_level(DATA_PIN, 0);
//                 printf("0");
//             }
//             value <<= 1;
//         }

//         gpio_set_level(CLOCK_PIN, 1);
//     }
//     printf("\r\n");
//     gpio_set_level(LATCH_PIN, 0);

//     gpio_set_level(LATCH_PIN, 1);
// }

// void HC595_SolenoidValve_two_bit(PinBit bito1, PinBit bito2, uint8_t on_off)
// {
//     int hc595111 = 0x100;
//     int hc595222 = 0x100;
//     int vue = 0;
//     printf("HC595_SolenoidValve_two_bit(%d--%d)\r\n", bito1, bito2);
//     bito1 -= 1;
//     bito2 -= 1;
//     int max = bito1 > bito2 ? bito1 : bito2;
//     for (uint8_t i = 0; i < max; i++)
//     {
//         if (i < bito1)
//             hc595111 <<= 1;
//         if (i < bito2)
//             hc595222 <<= 1;
//         hc595111 &= 0xffff00;
//         hc595222 &= 0xffff00;
//     }
//     if (on_off)
//     {
//         vue = hc595222 | hc595111;
//     }
//     else
//     {
//         vue = hc595222;
//     }
//     printf(" HC595_hammer_outVue(vue)%d\r\n", vue);
//     hammer_shiftout(vue);
// }
