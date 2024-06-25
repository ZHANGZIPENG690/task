

#include "app_main.h"

/**
 * @brief 硬件驱动
 *            GPIO 、 按键
 *            输出芯片 、 M段子
 *            传感器 、 RTC
 *            电流检测 、 灯光芯片
 *
 */
void m_callable_hardware_drive_init(void)
{
    int device_type = 0;
    int out_chip_type = 0;
    uint32_t out_hearward = 0;
    uint8_t led_strength = 0;
    /** 先初始化 GPIO */
    // m_callable_drive_gpio_init(16);

    m_callable_device_attribute_get_out_chip(&out_chip_type);
    m_callable_device_attribute_get_device_type(&device_type);
    m_callable_device_attribute_get_hearward_version(&out_hearward);

    /** 然后是初始化 按键 */
    m_callable_drive_button_init(device_type);

    /** 初始化 */
    /** 然后是初始化 HC595  或者 是其他输出的类型  */
    /** 传入当前在设备属性的类型*/
    // m_callable_drive_out_chip_init( 0 );






    /** 传感器初始化 */
    m_callable_sensor_init(13, 14);

    m_callable_manual_init();

    /** 电流检测初始化 */
    m_callable_current_monitor_init();

    /** io 输出初始化 */
    m_callable_solenoid_manage_init(out_hearward, device_type, out_chip_type);
    // m_callable_solenoid_manage_init(3, 30, 2);

    if (m_callable_drive_flash_read(M_TYPE_U8, "LL", &led_strength) == succ_r)
    {
        if (led_strength > 0 && led_strength < 250)
        {
        }
        else
            led_strength = 245;
    }
    else
        led_strength = 245;

    // m_callable_device_attribute_set_sn("29231123A003");
    /** LED初始化 */
    m_callable_light_display_initial(out_hearward, device_type, led_strength, 2);
    // m_callable_light_display_initial(3, 30, 90, 3);

    m_callable_solenoid_manage_M_init();

    /** 区域参数初始化记载 */
    m_callable_channel_manage_init(device_type);

    /** RTC初始化 */
    m_callable_midllle_rtc_init();

    m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 1);

    // 需要关闭所有端口
    m_callable_solenoid_manage_close_all();
    /*引脚12 直接设置为打开状态*/
    m_callable_drive_io_pin12_init();
}

int nmode = 0;
int smode = 0;
/**
 * @brief 软件驱动
 *            根据运行模式的wifi、和 蓝牙 或者两者都
 *            然后开启 3 个用于处理各种网络事件的线程
 */
void m_callable_software_drive_init(void)
{
    /** 根据找到的设备属性来初始化蓝牙或者wifi */
    stat_m stat = succ_r;
    /** 一个是运行模式 一个是 服务器的连接方式    */
    /** 这两个可以通过 设备属性加载得到 */
    stat = m_callable_device_attribute_get_running_mode(&nmode);

    stat = m_callable_device_attribute_get_net_connect_mode(&smode);
    if (stat == succ_r)
    {
        m_callable_network_set_connect_mode(nmode, smode);

        /** 根据传入的配置 直接初始化相关外设 */
        m_callable_network_init(); // 这里会直接会初始化蓝牙或者wifi ，
        // 如果是直连模式 会先连接 ，其他的会跳过
        //
    }
    else
        DEBUG_TEST(DB_I, "设备参数缺失 ");
    m_callable_global_monitor_init();
    m_callable_current_monitor_init();
    m_callable_instance_manage_init();

    // m_callable_drive_flash_write(M_TYPE_Str, "PA0", "220,0,60&30,1667991645,2084108936,101 00:00,10#600;12#600;6#580;2#580;3#200,105,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA1", "221,0,60&30,1667991645,2084108936,101 00:00,10#600;12#600;6#580;2#580;3#200,106,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA2", "222,0,60&30,1667991645,2084108936,101 04:00,10#600;12#600;6#580;2#580;3#200,105,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA3", "223,0,60&30,1667991645,2084108936,101 04:00,10#600;12#600;6#580;2#580;3#200,106,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA4", "224,0,60&30,1667991645,2084108936,101 10:00,10#600;12#600;6#580;2#580;3#200,105,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA5", "225,0,60&30,1667991645,2084108936,101 10:00,10#600;12#600;6#580;2#580;3#200,106,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA6", "226,0,60&30,1667991645,2084108936,101 12:00,10#600;12#600;6#580;2#580;3#200,105,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA7", "227,0,60&30,1667991645,2084108936,101 12:00,10#600;12#600;6#580;2#580;3#200,106,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA8", "228,0,60&30,1667991645,2084108936,101 13:30,10#600;12#600;6#580;2#580;3#200,105,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA9", "229,0,60&30,1667991645,2084108936,101 13:30,10#600;12#600;6#580;2#580;3#200,106,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA10", "230,0,60&30,1667991645,2084108936,101 15:00,10#600;12#600;6#580;2#580;3#200,105,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA11", "231,0,60&30,1667991645,2084108936,101 15:00,10#600;12#600;6#580;2#580;3#200,106,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA12", "232,0,60&30,1667991645,2084108936,101 17:00,10#600;12#600;6#580;2#580;3#200,105,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA13", "233,0,60&30,1667991645,2084108936,101 17:00,10#600;12#600;6#580;2#580;3#200,106,1,1]");

    // m_callable_drive_flash_write(M_TYPE_Str, "PA0", "814,0,60&30,1667991645,2084108936,100 18:00,1#600;7#300;2#300;8#590,108,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA1", "815,0,60&30,1667991645,2084108936,100 18:30,10#600;12#600;6#580,108,1,1]");

    // m_callable_drive_flash_write(M_TYPE_Str, "PA0", "996,0,60&30,1667991645,2084108936,A0B0C0D0E0F0G0H0I0J0K0L0M0N0O0P0Q0R0S0T0U0V0W0X0,1#600;7#300;2#300;8#590,120,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA1", "997,0,60&30,1667991645,2084108936,AUBUCUDUEUFUGUHUIUJUKULUMUNUOUPUQURUSUTUUUVUWUXU,1#600;7#300;2#300;8#590,120,1,1]");

    // m_callable_drive_flash_write(M_TYPE_Str, "PA0", "996,0,60&30,1667991645,2084108936,A0B0C0D0E0F0G0H0I0J0K0L0M0N0O0P0Q0R0S0T0U0V0W0X0,1#600;7#300;2#300;8#590,120,1,1]");
    // m_callable_drive_flash_write(M_TYPE_Str, "PA1", "997,0,60&30,1667991645,2084108936,AUBUCUDUEUFUGUHUIUJUKULUMUNUOUPUQURUSUTUUUVUWUXU,1#600;7#300;2#300;8#590,120,1,1]");
}

char app_version_result_b[30] = {0};

/** 软件部分初始化 */
void app_init(void)
{
    bzero(app_version_result_b, 30);

    m_callable_device_proper_status_update_to_init();
    /** 初始化存储 */
    m_callable_drive_flash_init();
    // m_callable_flash_rescue_message_flag("OA", M_GLO_STROAGE_RESCUE_MESSAGE_DEL);

    /** Timer*/
    m_callable_timer_manage_time_init(); // 4K
    // m_callable_system_printf_heap_free_info(12);
    /** 加载设备属性 */
    m_callable_device_attribute_init();
    // m_callable_system_printf_heap_free_info(13);

    m_callable_device_attribute_get_app_version(app_version_result_b);
    // m_callable_system_printf_heap_free_info(14);
    printf("version : %s\r\n", app_version_result_b);

    /** 硬件驱动 驱动端口  iic   IO 中断 按键  */
    m_callable_hardware_drive_init();
    // m_callable_system_printf_heap_free_info(15);

    /** 软件驱动 wifi蓝牙  */
    m_callable_software_drive_init(); // 70K

    /** 软件和硬件初始化完成之后 应该是其他的属性初始化 */
    /** 计划数量 、 计划相关的属性初始化加载 */

    // mDelay_ms(1000);
    // DEBUG_TEST(DB_I, "灯光测试 start");
    // DEBUG_TEST(DB_I, "测试项目1");
    // m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS1_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS2_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_CONNECTING_TO_SERVER_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_DISTRIBUTION_NETWORK_SUCCESS_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
    // mDelay_ms(3000);

    // DEBUG_TEST(DB_I, "测试项目2");
    // m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 1);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS1_MODE, 0);
    // mDelay_ms(3000);

    // DEBUG_TEST(DB_I, "测试项目3");
    // m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 1);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 2);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
    // mDelay_ms(3000);

    // DEBUG_TEST(DB_I, "测试项目4");
    // m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, 1);
    // mDelay_ms(1000);
    // m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, 1);
    // mDelay_ms(3000);

    // DEBUG_TEST(DB_I, "测试项目5  6");
    // m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, 2);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_ZONE_SELECT_MODE, 1);
    // mDelay_ms(1000);
    // m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, 1);
    // mDelay_ms(3000);

    // DEBUG_TEST(DB_I, "测试项目7");
    // m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS1_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_WIFI_CONNET_FAIL_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_WIFI_CONFIG_STATUS2_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_WIFI_NO_NETWORK_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_CONNECTING_TO_SERVER_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_SERVER_CONNET_FAIL_MODE, 0);
    // mDelay_ms(3000);

    // DEBUG_TEST(DB_I, "测试项目8");
    // m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_NETWORK_PROBLEM_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_WIFI_DISCONNECTION_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 2);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_NETWORK_PROBLEM_MODE, 0);
    // mDelay_ms(3000);

    // DEBUG_TEST(DB_I, "测试项目9");
    // m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, 2);
    // mDelay_ms(500);
    // m_callable_display_status(M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE, 2);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, 2);
    // mDelay_ms(500);
    // m_callable_display_status(M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE, 0);
    // mDelay_ms(3000);
    // m_callable_display_status(M_DISPLAY_VT_SHORT_CIRCUIT_MODE, 0);
    // mDelay_ms(2000);

    // DEBUG_TEST(DB_I, "测试项目10");
    // m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
    // mDelay_ms(3000);
    // for (int i = 0; i <= 100; i++)
    // {
    //     m_callable_display_status(M_DISPLAY_FIRMWARE_UPDATE_MODE, i);
    //     mDelay_ms(50);
    // }

    // print_partition_table();
    while (!UNIT_TEST && (nmode == M_DEVICE_RUNNING_MODE_NONE))
    {
        m_callable_device_device_factoryconfig();
        mDelay_ms(200);
    }
}