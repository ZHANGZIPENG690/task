

#include "main.h"

extern void timer_callback_fun(void *arg);

void monitor(void *args)
{
    long in_time_ms = 0;
    long in_time_s = 0;
    while (1)
    {
        timer_callback_fun(NULL);
        // in_time_s = m_callable_timer_manage_get_device_time();
        // if (in_time_ms % 50 == 0)
        // {
        //     m_callable_timer_manage_time_monitor();
        //     m_callable_instance_running_monitor(in_time_s);
        //     m_callable_event_distribution_advancing(in_time_s);
        // }

        // // tmie_buv(in_time_s);
        // // m_callable_instance_running_monitor(in_time_s);
        // in_time_ms++;
        // // d();
        // // if (in_time_s == 1668104125)
        // //     // m_callable_instance_running_event_input(MANUAL_STOP, m_callable_timer_manage_get_device_time()),
        // //     m_callable_instance_running_event_input(SCHEDULE_CHANGE_STOP, in_time_s),

        // m_callable_drive_button_function_monitor(in_time_ms);

        mDelay_ms(150);
    }
    // printf("%d\n" , in_time_s);
}

/** Python 使用*/

extern void app_main();
/**
 * @brief 必须在使用之前调用初始化
 *
 */
void m_callable_main_device_manage_init(void)
{
    mTaskCreate(NULL, monitor, "monitor", 1024, NULL, 2, NULL);

    /** 获取当前的版本信息*/

    /** 获取当前的MAC地址*/
    m_callable_device_attribute_set_sn("26220202A002");

    // m_callable_instance_manage_init();
    /** 应用初始化 */
    app_main();
    m_callable_device_attribute_running_set_mode(M_DEVICE_RUNNING_MODE_DERECT);

    DEBUG_TEST(DB_W,"默认设备时间 2023年 10 月 26 日  10:24:40 ");
    // strcpy(recv_buf, "[12,1698309910,8]");

    // m_callable_network_data_rx("[12,1698309910,8]", strlen("[12,1698309910,8]"));
    // m_callable_network_data_rx("[12,1698287080,8]", strlen("[12,1698287080,8]"));
    // m_callable_network_data_rx("[82,114.3333,30.55,8]", strlen("[82,114.31,30.52,8]"));
}
/**
 * @brief 模拟服务器发送数据到设备 ，传入数据 ：m_callable_main_device_data_input("[12,16845151,8]")
 *
 * @param data [12,16845151,8]
 */
void m_callable_main_device_data_input(char *data)
{
    m_callable_network_data_rx(data, strlen(data));
}

#include <math.h>
/**
 * @brief 注册回调函数 设备反馈的数据会通过该函数传入
 *
 * @param m_python_callback_param void (*m_python_callback_param)(char *data)
 */
void m_callable_reg_callback_function(void (*m_python_callback_param)(char *data));

extern void m_set_level(int you);
static const char M_STR_EN_DE_CODE[] = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx"};

void m_callable_time_str_to_num_decrypt_deco0de(char *contr, int *out_num, int bit_num, int len)
{
    int count_bit = 0;
    int bt_sum = bit_num;
    for (int i = 0; i < len; i++)
    {
        if (*contr != ' ' && *contr != '\0')
        {
            DEBUG_TEST( DB_I,"C: %c", *contr);
            for (int x = 0; x < 60; x++)
            {
                if (*contr == M_STR_EN_DE_CODE[x])
                {
                    bit_num--;
                    DEBUG_TEST( DB_I,"N  %d", bit_num);

                    *out_num += x * (int)pow(60, bit_num);
                    // contr++;
                    count_bit++;
                    break;
                }
            }
            *contr = ' ';
            if (count_bit >= bt_sum)
                break;
        }
        else
        {
            contr++;
        }
    }
}
/**
 * @brief 主函数入口
 *
 */
void main()
{
    long in_time_s = 0;

    int event = 0;
    char config_data[60] = "{\"ssid\":\"HCTJ12_m\",\"psw\":\"123567456\"}";
    char normal_data_1[300] = "[20,2222,2,null,1702252800,4070995199,101 10:24,1#60,109,null,1]";
    char normal_data_2[300] = "[20,1111,3,null,1661129600,4101494399,100 10:24,1#60,109,null,1]";

    char normal_data_3[300] = "[20,232,0,60&30,1667991645,2084108936,101 17:00,10#600;12#600;6#580;2#580;3#200,105,1,1]";
    char normal_data_4[300] = "[20,233,0,60&30,1667991645,2084108936,101 17:00,10#600;12#600;6#580;2#580;3#200,106,1,1]";
    char normal_data_change[300] = "[20,816,0,20&10,1674008936,1794108936,100 4:32,1#30;2#30;3#30;4#30;6#30;5#30;7#30;10#30;8#30;9#30;13#30,104,1;2;3;4;5;6;7,0,1]";

    // char normal_data_change[300] = "[20,996,0,20&10,1674008936,2794108936,A0B0C0D0E0F0G0H0I0J0K0L0M0N0O0P0Q0R0S0T0U0V0W0,1#100;2#100;3#100;4#100;6#100;5#100;7#180;10#130;8#130;9#130;13#130,120,1,1]";
    // 计划添加
    char recv_buf[1200] = {0};
    char arr_config_vue[100] = {"1A1A1A1A1A1A1A1A1A1A1A1A"};
    int a = 0;
    m_callable_main_device_manage_init();

    for (size_t i = 1; i < 13; i++)
    {
        m_callable_time_str_to_num_decrypt_deco0de(arr_config_vue, &a, 2, strlen(arr_config_vue));
        printf("%d\n", a);
    }
    // [20,814,0,2&1,1674008936,1684108936,100 15:16,1#3;2#3;3#4;4#2;5#3;6#3;7#3;8#3,109,1,1]
    // m_callable_instance_manage_init();
    // DEBUG_TEST(DB_W,"默认设备时间 2023年 10 月 26 日  10:24:40 ");
    // strcpy(recv_buf, "[12,1698287080,8]\n");
    // m_callable_network_data_rx(recv_buf, strlen(recv_buf));
    // char result[3] = {"0"};

    // int num = 1800;
    // m_encode(result, num, 3);

    // printf("->   %s\n", result);
    // return;

    while (1)
    {
        scanf("%d", &event);
        DEBUG_TEST(DB_W,"%d", event);
        switch (event)
        {
        case 0:
            DEBUG_TEST( DB_I,"设置成直接连接模式");
            m_callable_device_attribute_running_set_mode(M_DEVICE_RUNNING_MODE_DERECT);

            // m_callable_device_attribute_running_mode_set_direct();
            break;
        case 1:
            DEBUG_TEST( DB_I,"向设备发送配网数据");
            m_callable_network_data_rx(config_data, strlen(config_data));
            break;
        case 2:
            DEBUG_TEST( DB_I,"设置网络连接成功");
            m_callable_network_set_connect_status(true);

            break;
        case 3:
            DEBUG_TEST( DB_I,"设置网络连接失败");
            m_callable_network_set_connect_status(false);
            break;
        case 4: // 连接成功
            DEBUG_TEST( DB_I,"设置服务器连接成功");
            m_callable_network_set_server_status(M_NETWORK_CONNECT_PROGRESS_START_LOGIN);
            break;
        case 5: //
            DEBUG_TEST( DB_I,"设置服务器连接失败");
            m_callable_network_set_server_status(M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS);
            break;

        case 6:
            DEBUG_TEST( DB_I,"改变传感器状态");
            scanf("%d", &event);
            m_set_level(event);
            break;
        case 8:
            DEBUG_TEST( DB_I,"设置传感器");
            scanf("%d", &event);
            m_callable_sensor_set(event, 0, true);
            // DEBUG_TEST( DB_I,"全部成功");
            // m_callable_network_set_server_status(M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS);
            break;

        case 7:

            DEBUG_TEST( DB_I,"按键输入 停止: 1 开始: 2 左: 3 右: 4\n");
            scanf("%d", &event);
            if (event == 1)
                m_callable_drive_button_event_input(M_KEY_EVENT_STOP);
            else if (event == 2)
                m_callable_drive_button_event_input(M_KEY_EVENT_START);
            else if (event == 3)
                m_callable_drive_button_event_input(M_KEY_EVENT_LEFT);
            else if (event == 4)
                m_callable_drive_button_event_input(M_KEY_EVENT_RIGHT);
            break;
        case 18:
            strcpy(recv_buf, "[18,null,null,8]");
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            break;
        case 9:
            strcpy(recv_buf, "[9]");
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            break;
        case 10:
            DEBUG_TEST( DB_I,"设置当前的时间");
            strcpy(recv_buf, "[12,1702549320,0]");

            // strcpy(recv_buf, "[12,1698364750,0]");
            // strcpy(recv_buf, "[12,1698265870,8]");
            // strcpy(recv_buf, "[12,1698361980,8]");
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            break;
        case 12:
            DEBUG_TEST( DB_I,"调整时间  直接输入时间搓 :\n");
            scanf("%d", &event);
            sprintf(recv_buf, "[12,%d,0]", event);
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));

            break;
        case 20:
            DEBUG_TEST( DB_I,"添加一个计划 输入 1 - 2 - 3 - 4");
            scanf("%d", &event);
            if (event == 1)
                m_callable_network_data_rx(normal_data_1, strlen(normal_data_1));
            else if (event == 2)
                m_callable_network_data_rx(normal_data_2, strlen(normal_data_2));
            else if (event == 3)
                m_callable_network_data_rx(normal_data_3, strlen(normal_data_3));
            else if (event == 4)
                m_callable_network_data_rx(normal_data_4, strlen(normal_data_4));
            else if (event == 5)
                m_callable_network_data_rx(normal_data_change, strlen(normal_data_change));
            else if (event == 87)
            {
                strcpy(recv_buf, "[20,814,0,C1,1674008936,1684108936,100 4:32,1#30;7#30;8#30,109,1,1]");
                m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            }
            else if (event == 35)
            {
                // 120 A0B0C0D0E0F0W0Q0K0L0P0O0I0U0Y0Z0M0N0V0
                strcpy(recv_buf, "[20,3335,0,50&20,1674008936,1684108936,0,4#300;2#300;1#30;7#50,109,1,1]");
                m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            }
            else if (event == 38)
            {
                strcpy(recv_buf, "[38,2,100]");
                m_callable_network_data_rx(recv_buf, strlen(recv_buf));
                strcpy(recv_buf, "[20,814,0,2|50,1674008936,1684108936,100 11:23,1#30;7#30;8#30,109,1,1]");
                m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            }
            else if (event == 5071)
            {
                strcpy(recv_buf, "[20,5071,2,null,1600697600,4070995199,A0C0,1#1;2#1;3#1;4#1;5#1;6#1;7#1;8#1;9#1;10#1;11#1;12#1;13#1;14#1;15#1;16#1;17#1;18#1;19#1;20#1;21#1;22#1;23#1;24#1,120,null,1]");
                m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            }
            else
            {
                strcpy(recv_buf, "[20,816,0,null,1674008936,1984108936,100 16:07,1#30,109,1,0,1]");
                m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            }
            break;

        case 22:
            DEBUG_TEST( DB_I,"删除一个计划 请输入 [22,id]\n");
            scanf("%s", recv_buf);
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            break;
        case 23:
            DEBUG_TEST( DB_I,"删除 ALL 计划 请输入 [22,254]\n");
            scanf("%s", recv_buf);
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            break;
        case 24:
            DEBUG_TEST( DB_I,"查看 ALL 计划 请输入 [202]\n");
            scanf("%s", recv_buf);
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));

            break;
        case 30:
            DEBUG_TEST( DB_I,"继续输入计划 id\n");
            scanf("%d", &event);
            sprintf(recv_buf, "[30,%d,null,null,1]", event);
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));

            break;
        case 31:
            DEBUG_TEST( DB_I,"三诉讼诉讼诉讼输入计划 \n");
            // scanf("%d", &event);
            // sprintf(recv_buf, "[30,%d,null,null,1]", event);
            strcpy(recv_buf, "[30,5071,null,1#1;2#1;3#1;4#1;5#1;6#1;7#1;8#1;9#1;10#1;11#1;12#1;13#1;14#1;15#1;16#1;17#1;18#1;19#1;20#1;21#1;22#1;23#1;24#1,0]");
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));

            break;
        case 37:
            DEBUG_TEST( DB_I,"设置休眠模式]\n");
            strcpy(recv_buf, "[37,19522210,1798105000,1]");
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            break;
        case 70:
            DEBUG_TEST( DB_I,"设置单个跳过]\n");
            strcpy(recv_buf, "[70,815,19522210,1798105000,1]");
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            break;
        case 62:
            DEBUG_TEST( DB_I,"设置全局跳过]\n");
            strcpy(recv_buf, "[62,19522210,1668105000,1]");
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            break;

        case 87:
            strcpy(normal_data_1, "[87,1*10*100,2*10*200,1]");
            m_callable_network_data_rx(normal_data_1, strlen(normal_data_1));
            break;
        case 39:
            strcpy(recv_buf, "[39]");
            m_callable_network_data_rx(recv_buf, strlen(normal_data_1));
            break;
        case 43:
            strcpy(recv_buf, "[43]");
            m_callable_network_data_rx(recv_buf, strlen(normal_data_1));
            break;
        case 44:
            strcpy(recv_buf, "[44]");
            m_callable_network_data_rx(recv_buf, strlen(normal_data_1));
            break;
        case 45:
            strcpy(recv_buf, "[45]");
            m_callable_network_data_rx(recv_buf, strlen(normal_data_1));
            break;
        case 40:
            DEBUG_TEST( DB_I,"快速运行\n");
            strcpy(recv_buf, "[40,1#1#60;2#2#60;3#3#60;4#4#60,10&30]");
            // strcpy(recv_buf, "[40,1#1#1000;2#2#2000;3#3#2000;4#4#2000;5#5#2000;6#6#2000;7#7#2000;8#8#2000;9#9#2000;10#10#2000;11#11#1000;12#12#2000;13#13#2000;14#14#2000;15#15#2000;16#16#2000;17#17#2000;18#18#200;19#19#2000;20#20#2000;21#21#100;22#22#2000;23#23#2000;24#24#2000;25#25#2000;26#26#2000;27#27#2000;28#28#200;29#29#2000;30#30#2000]");
            // strcpy(normal_data_1 , "[30,]" );
            m_callable_network_data_rx(recv_buf, strlen(normal_data_1));
            // 1#1000;2#2000;3#2000;4#2000;5#2000;6#2000;7#2000;8#2000;9#2000;10#2000;11#1000;12#2000;13#2000;14#2000;15#2000;16#2000;17#2000;18#200
            // [20,3335,3,null,1674008936,1984108936,106E0,1#5;2#5;3#5;4#5;5#5;6#5;7#5;8#5;9#5;10#5;11#5;12#5;13#5;14#5;15#5;16#5;17#5;18#5,109,null,1]
            // [30,3335,1#5;2#5;3#5;4#5;5#5;6#5;7#5;8#5;9#5;10#5;11#5;12#5;13#5;14#5;15#5;16#5;17#5;18#5,0]
            break;
        case 98:
            m_callable_network_data_rx(normal_data_1, strlen(normal_data_1));
            m_callable_network_data_rx(normal_data_2, strlen(normal_data_2));
            m_callable_network_data_rx(normal_data_3, strlen(normal_data_3));
            m_callable_network_data_rx(normal_data_change, strlen(normal_data_change));
            // event = 101;
            break;

        // case 87:
        //     strcpy(recv_buf, "[37,19522210,1798105000,1]");
        //     m_callable_network_data_rx(recv_buf, strlen(recv_buf));
        //     break;
        default:
            printf("自定义发送数据\n");
            scanf("%s", recv_buf);
            DEBUG_TEST(DB_W,"%s", recv_buf);
            m_callable_network_data_rx(recv_buf, strlen(recv_buf));
            while (1)
                ;
            break;
        }
    }
    // printf("lplplp\n");
}

/**
 * @brief
 *         case 11:
            DEBUG_TEST( DB_I,"传感器 1 物理接入");

            break;
        case 12:
            DEBUG_TEST( DB_I,"传感器 1 物理断开");
            break;
        case 13:
            DEBUG_TEST( DB_I,"传感器 2 物理接入");
            break;
        case 14:
            DEBUG_TEST( DB_I,"传感器 2 物理断开");
            break;
 *
 */
