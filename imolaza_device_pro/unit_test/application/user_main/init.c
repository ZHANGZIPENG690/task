

// #include "app_main.h"

#include "core/fset_lib.h"
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
    int out_hearward = 0;
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
    /** 电流检测初始化 */

    /** io 输出初始化 */
    m_callable_solenoid_manage_init(out_hearward, device_type, out_chip_type);
    //  m_callable_solenoid_manage_init(2, 16, 2, 2);
    /** LED初始化 */
    m_callable_light_display_initial(out_hearward, device_type, 90);

    m_callable_display_status(M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE, 1);
}

enum running_mode nmode;
enum connect_mode smode;
/**
 * @brief 软件驱动
 *            根据运行模式的wifi、和 蓝牙 或者两者都
 *            然后开启 3 个用于处理各种网络事件的线程
 */
void m_callable_software_drive_init(void)
{
    /** 根据找到的设备属性来初始化蓝牙或者wifi */
    stat_m stat = fail_r;

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
        DEBUG_TEST( DB_I,"设备参数缺失 ");
    m_callable_global_monitor_init();
    m_callable_current_monitor_init();
}

/** 软件部分初始化 */
void app_init(void)
{
  
    m_callable_device_proper_status_update_to_init();
    /** 初始化存储 */
    m_callable_drive_flash_init();
    /** 加载设备属性 */
    m_callable_device_attribute_init();

    /** 硬件驱动 驱动端口  iic   IO 中断 按键  */
    m_callable_hardware_drive_init();

    /** 软件驱动 wifi蓝牙  */
    m_callable_software_drive_init();

    m_callable_timer_manage_time_init();

    /** 软件和硬件初始化完成之后 应该是其他的属性初始化 */
    /** 计划数量 、 计划相关的属性初始化加载 */
    while (!UNIT_TEST && (nmode == M_DEVICE_RUNNING_MODE_CONFIG || nmode == M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG))
    {
        m_callable_device_device_factoryconfig();
        mDelay_ms(200);
    }
}
