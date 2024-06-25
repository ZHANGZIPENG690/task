

#ifndef __CURRENT_MONITOR__
#define __CURRENT_MONITOR__

// #include "../../../drive/ADC/m_adc.h"
#include "core/fset_lib.h"

#define ADC_CHANNEL ADC1_CHANNEL_7
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_WIDTH ADC_WIDTH_BIT_12

// 用于计算电流 3.36
#define VCC 3.36
#define AD 4.2
#define PRECISION 0.132

// 用于计算电流 3.36
// #define VCC 3.36
// #define AD 3.7
// #define PRECISION 0.132

struct current_monitor_m
{
    /** 短路设定模式，根据模式确定短路显示的状态 */
    enum current_short_mode cur_short_mode;

    /*电流检测开始标志位*/
    int current_monitor_flag;
    uint8_t channel;
    float max_voltage;
    float min_voltage;
    float max_voltage_output;
    float min_voltage_output;
    float max_current;
    float min_current;
    float avg_current;
    float read_adc_interval_time; // 采样时间
    float max_avg_current;
    uint64_t first_time;

    /** 最后采样得到的电流值  */
    float current_value1;
    /*计数*/
    int current_value1_count;
    /*累计值*/
    int current_value1_averge;
    /** 电流中断标记 */
    bool m_global_short_interrupt;
    /** 电流中断区域 */
    int m_global_short_interrupt_zone;

    bool current_calibration_interrupt;
};
struct current_monitor_m current_monitor;

/**
 * @brief 电流中断短路通知
 *
 */
void m_callable_current_out_short_interrupt(void);

/**
 * @brief 电流检测初始化 同时打开了 检测电流线程
 * @return stat_m
 */
stat_m m_callable_current_monitor_init(void);

/**
 * @brief 开始电流检测
 * @return stat_m
 */
stat_m m_callable_current_monitor_start(void);

/**
 * @brief 停止电流检测
 * @return stat_m
 */
stat_m m_callable_current_monitor_stop(void);

/**
 * @brief 查询短路是否
 *
 * @return stat_m  succ 短路
 */
stat_m m_callable_current_is_interrupt();

stat_m m_calllable_current_monitor(int device_status);

int m_callable_current_calibration(void);

/**
 * @brief 获取电流校准时对应的区域通道
 *
 * @param channel
 * @return stat_m
 */
stat_m m_callable_current_calibration_channel_select(uint8_t channel);

stat_m m_callable_current_calibration_interrupt_processing(void);

stat_m m_callable_current_monitor_pause_not_clear(void);

/**
 * @brief 根据传入模式调整在短路发生时，对应的处理
 *
 * @param in_csm 模式
 * @return stat_m
 */
stat_m m_callable_current_set_short_mode(enum current_short_mode in_csm);
/**
 * @brief 设置电流反馈的当前检测区域，需要再每次区域开启更新，保持水锤更新
 *
 * @param in_zone
 * @return stat_m
 */
stat_m m_callable_current_set_checkout_zone(int in_zone);
#endif /* __CURRENT_MONITOR__ */