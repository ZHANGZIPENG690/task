










// #include "app_main.h"


// /**
//  * @brief 硬件驱动
//  *            GPIO 、 按键
//  *            输出芯片 、 M段子
//  *            传感器 、 RTC
//  *            电流检测 、 灯光芯片 
//  * 
//  */
// void m_callable_hardware_drive_init(void)
// {
//     /** 先初始化 GPIO */
//     m_callable_drive_gpio_init(16);

//     /** 然后是初始化 按键 */
//     m_callable_drive_button_init(m_callable_device_attribute_device_type_get());

//     /** 然后是初始化 HC595  或者 是其他输出的类型  */
//     /** 传入当前在设备属性的类型*/
//     m_callable_drive_out_chip_init( 0 );

//     /** 初始化 */
    
// }

//  /**
//   * @brief 软件驱动
//   *            根据运行模式的wifi、和 蓝牙 或者两者都   
//   *            然后开启 3 个用于处理各种网络事件的线程
//   */
//  void m_callable_software_drive_init(void)
// {
//     /** 根据找到的设备属性来初始化蓝牙或者wifi */
//     stat_m stat = fail_r;

//     /** 一个是运行模式 一个是 服务器的连接方式    */
//     /** 这两个可以通过 设备属性加载得到 */
//     enum running_mode *nmode ;
//     enum connect_mode *smode ;
//     stat = m_callable_device_attribute_running_mode_get(nmode);
//     stat = m_callable_device_attribute_net_connect_mode_get(smode);
    
//     if(stat == succ_r )
//     {
//         m_callable_network_set_connect_mode(*nmode , *smode);
//         /** 根据传入的配置 直接初始化相关外设 */
//         m_callable_network_init();  // 这里会直接会初始化蓝牙或者wifi ， 
//         // 如果是直连模式 会先连接 ，其他的会跳过
//         m_callable_network_connect();

//     } else DEBUG_TEST( DB_I,"设备参数缺失 ");

// }




// /** 软件部分初始化 */
// void app_init(void)
// {
//     /** 初始化存储 */
//     m_callable_flash_init();
//     /** 加载设备属性 */
//     m_callable_device_attribute_init();

//     /** 硬件驱动 驱动端口  iic   IO 中断 按键  */
//     m_callable_hardware_drive_init();
//     /** 软件驱动 wifi蓝牙  */
//     m_callable_software_drive_init();

//     /** 软件和硬件初始化完成之后 应该是其他的属性初始化 */
//     /** 计划数量 、 计划相关的属性初始化加载 */
//     // m_callable()

// }






