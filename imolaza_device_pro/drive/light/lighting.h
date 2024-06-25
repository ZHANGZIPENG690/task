#if !defined(LIGHTING_H)
#define LIGHTING_H

#include "core/fset_lib.h"

#define ADDR1 0x7E
#define ADDR2 0x78
#define ADDR3 0x7C

typedef struct _led_fifo
{
    bool onOff;
    bool (*StaUpdate)();
    uint32_t _LS_PRO[34];
    uint32_t _LS[24];
} led_fifo;

/**
 * @brief 设置灯光颜色
 * @param pin
 * @param color
 * @return void
 */
void m_ext_rgb_pin_2(uint8_t pin, uint32_t color);


/**
 * @brief 设置灯光颜色
 * @param pin
 * @param red
 * @param green
 * @param blue
 * @return void
 */
void m_ext_rgb_pin_1(uint8_t pin, uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief 灯光 芯片驱动初始化
 * @return stat_m
 */
stat_m m_ext_drive_lighting_init(void);

/**
 * @brief 灯光 IIC驱动初始化
 * @return stat_m
 */
stat_m m_ext_drive_lighting_IIC_init(void);









// 功能实现了，但是没用上
void m_ext_drive_lighting_set_point_color(uint8_t point, uint32_t color);
void m_ext_drive_lighting_set_range_color(uint8_t s_point, uint8_t e_point, uint32_t color);
void m_ext_drive_lighting_blink_point(uint8_t blink_param, uint8_t point, uint32_t color);
void m_ext_lighting_blink_range_color(uint8_t blink_param, uint8_t s_point, uint8_t e_point, uint32_t color);

// /**
//  * @brief lighting init
//  * 
//  * @return stat_m 
//  */
// void m_ext_drive_lighting_init(void);


// /**
//  * @brief Turn on a light and color
//  * 
//  * @param point Light point
//  * @param color Light color
//  * @return stat_m 
//  */
// void m_ext_drive_lighting_set_point_color(uint8_t point,uint32_t color);

// /**
//  * @brief Turn on a light and color
//  * 
//  * @param point Start Light point
//  * @param point End Light point
//  * @param color Light color
//  * @return stat_m 
//  */
// void m_ext_drive_lighting_set_range_color(uint8_t s_point,uint8_t e_point,uint32_t color);


// /**
//  * @brief Blink a light in one color
//  *
//  * @param blink_param Control blinking frequency and breathing effect
//  * @param point light index
//  * @param color light color
//  * @return stat_m
//  */
// void m_ext_drive_lighting_blink_point(uint8_t blink_param, uint8_t point, uint32_t color);

// /**
//  * @brief Lights flash in the control area
//  *
//  * @param blink_param Control blinking frequency and breathing effect
//  * @param s_point  light start index
//  * @param e_point  light end index
//  * @param color light color
//  * @return stat_m
//  */
// void m_callable_lighting_blink_range_color(uint8_t blink_param, uint8_t s_point, uint8_t e_point, uint32_t color);
// void m_ext_rgb_point(uint8_t point, uint32_t color);
#endif

