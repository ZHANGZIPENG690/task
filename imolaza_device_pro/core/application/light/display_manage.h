#if !defined(DISPLAY_MANAGE_H)
#define DISPLAY_MANAGE_H
#include "core/fset_lib.h"

#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 5 // 30

struct display_attribute_m
{
    /*灯光强度*/
    uint8_t display_lighting_strength;

    /*灯光变化跨度*/
    uint8_t brightness_step;

    /*灯光对应的不同通道数量属性*/
    uint8_t display_number_of_channels;

    /*灯光呼吸变化跨度*/
    int display_increment_lighting;

    /*灯光显示区域选择*/
    uint8_t display_zone_select;

    /*灯光当前的显示区域*/
    uint8_t display_zone_current;

    /*灯光OTA更新百分比*/
    uint8_t display_ota_renewal_percent;

    /*灯的总数*/
    uint8_t display_numLeds;

    /*灯光的旋转效果下,最后一个灯的序号*/
    uint8_t display_last_light_id;

    /*设置部分灯光效果是关闭还是打开*/
    uint8_t display_set_light_close;

    uint8_t manual_setting_light_num;

    /*灯光的闪烁延时*/
    int dispaly_flicker_speed;
    /*灯光对应的不同的硬件版本*/
    uint32_t display_device_version;
};
struct display_attribute_m display_attribute;

// 设备型号、区域通路数量、灯光亮度
void m_callable_display_initial(uint8_t _device_version_set, uint8_t number_of_channels, uint8_t light_strength, uint8_t increment_lighting);

// 开机动画
void m_callable_display_start_up(void);

// 正常使用找网络
void m_callable_display_find_network(void);

//******************正常运行******************//

// 空闲状态
void m_callable_display_idle_status(void);

// 区域灌溉进行中
void m_callable_display_zone_running(void);

// 区域灌溉选中
void m_callable_display_zone_select(void);

//******************配网过程******************//

// 连接wifi 阶段1
void m_callable_display_wifi_config_status1(void);

// 连接wifi 阶段2
void m_callable_display_wifi_config_status2(void);

// 正在连接服务器
void m_callable_display_connecting_to_server(void);

// 配网成功
void m_callable_display_distribution_network_success(void);

// wifi连接失败
void m_callable_display_wifi_connet_fail(void);

// wifi无网络
void m_callable_display_wifi_no_network(void);

// 服务器连接失败
void m_callable_display_server_connet_fail(void);

//******************网络异常******************//

// 网络出现问题
void m_callable_display_network_problem(void);

// wifi断开
void m_callable_display_wifi_disconnection(void);

//******************区域短路******************//

void m_callable_display_regional_short_circuit(void);

//******************异常反馈******************//

// void display_abnormal_feedback(void);

//******************固件更新******************//
void m_callable_display_firmware_update(void);

// 设置区域数量
void m_callable_display_set_zone(void);

// 设置设备版本
void m_callable_display_set_device_version(void);

// 设置灯光亮度
// void m_callable_display_set_light_strength(void);
void m_callable_display_set_light_strength(uint8_t param);

// 设置灯光打开与关闭
void m_callable_display_set_light_close_or_open(uint8_t param);

// 清除灯光
void m_callable_light_clean(void);

/**
 * @brief 灯光单元初始化
 *
 * @param _device_version_set1
 * @param number_of_channels1
 * @param light_strength1
 * @param increment_lighting1
 * @return stat_m
 */
stat_m m_callable_light_display_initial(uint32_t _device_version_set1, uint8_t number_of_channels1, uint8_t light_strength1, uint8_t increment_lighting1);

/**
 * @brief 选择灯光显示模式
 *
 * @param light_mode
 * @param param
 * @return stat_m
 */
stat_m m_callable_display_status(enum light_anima_mode light_mode, uint8_t param);

// 灯光显示 线程
void m_static_display_run(void *pvParameters);

/**
 * @brief 获取灯光状态

 * @return light_anima_mode
 */
enum light_anima_mode m_callable_display_status_get();

void m_static_display_version_light(void);
void m_static_breathe_set_led_color(uint32_t adjustedColor, uint8_t brightness);
void m_static_breathe(uint32_t adjustedColor, uint8_t brightness_step);
void m_static_not_breathe(uint32_t adjustedColor);
void m_static_breathe_around(uint32_t adjustedColor, uint8_t brightness_step);
void m_static_diplay_zone_select(uint8_t *arr);
void m_static_diplay_zone_immobilization(void);
void m_static_display_version_light(void);
void m_static_localized_light_flashing(uint32_t display_device_version, uint8_t display_numLeds, uint8_t step, enum light_RGB_color static_color, enum light_RGB_color flicker_color, bool isOn);
void m_static_breathe_alone(uint32_t adjustedColor, uint8_t brightness_step, uint8_t i);

#endif
