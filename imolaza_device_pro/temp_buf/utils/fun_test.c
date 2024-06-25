

// #include "hot_so_fast.h"
// #include "fun.h"
// #include "bsp_hw.h"
// #include "tool.h"
// #include "text.h"
// #include "hot_so_fast.h"
// #include "wifi/conn_eth.h"
// #include "fun/fun.h"

// // extern bool ledtest;
// extern time_manage _time;
// extern wifi_manager _wifi;
// extern struct _errTable _errTestInfo;
// extern struct light_state_control_bit light_scb;

// char str[128] = {0};

// char sssid[40] = {0};
// char ppsw[40] = {0};

// enum test_mode
// {
//     LED_MDOE = 5,
//     WATERING_MODE = 10,
//     KEY_MODE = 15,
//     ConfigNet = 20,
// };

// bool denum = false,
//      sns = false;
// uint8_t TestMode = 0;

// extern device_info _device;

// void TestModes(uint8_t mode);

// void test_array_check()
// {
//     memset(str, 0, sizeof(str));
//     scanf("%s", str);
//     if (strstr(str, DEVICETYPE_HEAD) != NULL && strstr(str, DEVICETYPE_END) != NULL)
//     {
//         // printf("*\r\n");
//         M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(str, DEVICETYPE_HEAD "%hhd" DEVICETYPE_END, &TestMode);
//         // printf("--%hhd--\r\n", TestMode);
//         if (TestMode == 20)
//         {
//             M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(str, "%*[^,]%[^,]", sssid);
//             M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(str, "%*[^,]%*[^,]%[^,]", ppsw);
//             printf("%s\r\n", sssid);
//             printf("%s", ppsw);
//         }

//         TestModes(TestMode);
//     }
//     if (strstr(str, SERVERNUM_HEAD) != NULL && strstr(str, SERVERNUM_END) != NULL && !sns)
//     {
//         memset(_device.sn, 0, sizeof(_device.sn));
//         M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(str, SERVERNUM_HEAD "%[^}]", _device.sn);
//         if (strlen(_device.sn) > 8 && strlen(_device.sn) < 20)
//             save_snNUm(),
//                 printf("SN_SUCC : %s", _device.sn), sns = true;
//     }
//     if (strstr(str, ENCRYPTED_KEY_HEAD) != NULL && strstr(str, ENCRYPTED_KEY_END) != NULL && !denum)
//     {
//         memset(_device.key, 0, sizeof(_device.key));
//         M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(str, ENCRYPTED_KEY_HEAD "%[^}]", _device.key);
//         if (strlen(_device.key) > 25 && strlen(_device.key) < 40)
//             save_keyNUm(),
//                 printf("KEY_SUCC : %s", _device.key), denum = true;
//     }
// }

// /**
//  * @brief
//  *
//  * @param ssid
//  * @param psw
//  * @return stat_m
//  */
// stat_m fun_net_test(char *ssid, char *psw)
// {
//     stat_m st = fail_m;
//     put_in(CONN_RECE_REQ, OKK);
//     DEBUG_TEST(DB_I," tips: Already received POST request. SSID : %sPassword : %s ", ssid, psw);
//     if (strlen(ssid) > 0 && strlen(psw) > 0)
//     {
//         put_in(CONN_RECE_SSID_PSW, OKK);
//         st = _wifi.wifi_connect_test(ssid, psw);
//         if (st == ap_no_exist_m)
//             printf("AP_NO_EXIST");
//         else if (st == ap_time_out_m)
//             printf("AP_OUT_TIME");
//         else if (st == succ_m)
//             printf("AP_SUCC");
//     }
//     else
//         put_in(CONN_RECE_SSID_PSW, ERR);
//     if (st == succ_m)
//     {
//         memset(&STA_CONN, 0, sizeof(Sta_Conn));
//         memcpy(STA_CONN.psw, psw, strlen(psw) + 1);
//         memcpy(STA_CONN.ssid, ssid, strlen(ssid) + 1);
//         put_in(STAT_CONF_RECEIVER_PASW, OKK);
//         send_handle_loop(STAT_CONF_RECEIVER_PASW, NULL, 0);
//     }
//     return st;
//     // send_handle_loop(888 ,NULL);
// }
// void functionTestResulePrint(uint8_t mode)
// {
//     switch (mode)
//     {
//     case LED_MDOE:
//         printf("LED test comple");
//         printf("The Light communication link has timed out : %d", _errTestInfo.functionTest_linkTimeOut);
//         printf("The chip was not found : %d\r\n", _errTestInfo.functionTest_not_found_chip);
//         {
//             if (_errTestInfo.functionTest_not_found_chip || _errTestInfo.functionTest_linkTimeOut)
//                 printf("\tThe problem is serious, please pay attention to solve it !");
//             else
//                 printf("\tGreat, no problem was found for the moment !");
//         }
//         break;
//     case WATERING_MODE:
//         // fun_IOIO_test();
//         break;
//     case KEY_MODE:
//         // fun_key_test();
//         break;
//     case ConfigNet:
//         // fun_net_test(sssid, ppsw);
//         break;
//     default:
//         // printf("ERR");
//         break;
//     }
// }

// void fun_rgb_test()
// {
//     _errTestInfo.functionTest_is_led = light_scb.ledtest = true;
// }
// void fun_IOIO_test()
// {
//     for (size_t i = 1; i < 9; i++)
//     {
//         SolenoidValve(i, 1);
//         vTaskDelay(500);
//     }
//     SolenoidValve(8, 0);
//     for (size_t i = 1; i < 9; i++)
//     {
//         vTaskDelay(500);
//         SolenoidValve(9 - i, 1);
//     }
//     vTaskDelay(500);
//     SolenoidValve(1, 0);
//     printf("IOTESTCOMP");
// }
// void fun_key_test()
// {
//     printf("KEYTESTCOMP");
// }

// void TestModes(uint8_t mode)
// {

//     switch (mode)
//     {
//     case LED_MDOE:
//         fun_rgb_test();
//         break;
//     case WATERING_MODE:
//         fun_IOIO_test();
//         break;
//     case KEY_MODE:
//         fun_key_test();
//         break;
//     case ConfigNet:
//         fun_net_test(sssid, ppsw);
//         break;
//     default:
//         printf("ERR");
//         break;
//     }
// }