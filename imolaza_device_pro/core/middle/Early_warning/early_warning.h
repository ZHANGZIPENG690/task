#if !defined(EARLY_WARNING_H)
#define EARLY_WARNING_H

#include "core/fset_lib.h"

#define CURRENT_FOR_A_LONG_TIME 1
#define NO_CURRENT_FOR_A_LONG_TIME 2
#define FLOW_LEAKAGE 3
#define FLOW_LEAKAGE_MINU 4

/*两个传感器通道的属性*/
struct m_sensor_warning
{
  /*判断传感器通道1是否打开*/
  bool sensor1_open_flag;

  /*判断传感器通道2是否打开*/
  bool sensor2_open_flag;

  /*流量计触发标志位*/
  bool flow_sensor_flag;

  /*判断是否需要流量漏水检测*/
  bool flow_start_flag;

  /*流量计触发标志位*/
  bool flow_sensor_flag_minu;

  /*判断是否需要流量漏水检测*/
  bool flow_start_flag_minu;

  /*判断传感器检测功能是否打开*/
  uint8_t sensor_monitor_open_or_close;

  /*设置流量计检测周期*/
  uint8_t flow_leakage_time;

  /*设置雨量计持续通电时间*/
  uint8_t rain_poweron_time;

  /*设置雨量计持续断电时间*/
  uint8_t rain_poweroff_time;

  /**流量传感器型号*/
  uint8_t sensor_type;

  /*获取雨量计 1  是常闭还是常开，用来后续判断是通电还是断电*/
  enum seneor_type sensor1_type_get;

  /*获取雨量计 2  是常闭还是常开，用来后续判断是通电还是断电*/
  enum seneor_type sensor2_type_get;

  /*获取雨感计1是已触发还是未触发*/
  enum is_effective is_effective_get_sensor1;

  /*获取雨感计2是已触发还是未触发*/
  enum is_effective is_effective_get_sensor2;

  /*设置流量计检测流量阈值*/
  float flow_value;
  
  /*设置流量计检测流量阈值*/
  float flow_value_minu;

  /*获取流量计 流量值*/
  float sensor_flow_value_sum;

    /*获取流量计 流量值*/
  float sensor_flow_value_sum_minu;

  /*流量计1检测首次触发时间*/
  uint64_t flow_sensor_first_time;

  /*流量计1检测首次触发时间*/
  uint64_t flow_sensor_first_time_minu;
  /*获取检测当前时间*/
  uint64_t last_trigger_time;

  /*获取流量计漏水检测脉冲*/
  uint64_t sensor_pulsecount_leak;
  /*获取流量计漏水检测脉冲*/
  uint64_t sensor_pulsecount_leak_minu;

  /*雨量计首次触发时间*/
  uint64_t sensor_first_trigger_time;
};

/*RTC电量检测属性属性*/
struct m_rtc_power_warning
{
  /*RTC首次检测时间*/
  uint64_t rtc_detection_first_time;
};

/**
 * @brief 设置 传感器损坏检测时间
 * @param Poweron_time_set
 * @param Poweroff_time_set
 * @return stat_m
 */
stat_m
m_callable_device_sensor_damage_detection_time_set(uint8_t poweron_time, uint8_t poweroff_time);

/**
 * @brief 设置 流量漏水检测时间
 * @param time_set
 * @param flow_value_set
 * @return stat_m
 */
stat_m m_callable_device_flow_leakage_detection_time_set(uint8_t time_set, float flow_value_set);

/**
 * @brief 雨量传感器损坏 检测清空
 * @return stat_m
 */
stat_m m_callable_rain_gauge_damage_detection_data_cleared(bool sensor_open_flag, enum seneor_type sen_type_get, uint8_t channel, enum is_effective is_effective_get_sensor);

/**
 * @brief 预警检测反馈 everyone
 * @return stat_m
 */
stat_m m_callable_early_warning_feedback_monitor(uint64_t current_time);

/**
 * @brief 流量计 漏水检测之 脉冲计数
 * @return stat_m
 */
stat_m m_callable_flow_sensor_meter_pulse_counting(void);

// /**
//  * @brief 传感器损坏以及漏水检测 是否需要打开或者关闭
//  *  @param channel
//  * @param flag
//  * @return stat_m
//  */
// stat_m m_callable_early_warning_monitor_open_or_close(bool flag);

/**
 * @brief 传感器损坏以及漏水检测设置 存储  防止断电数据清空
 * @return stat_m
 */
stat_m m_callable_early_warning_data_init(void);

/**
 * @brief 传感器损坏以及漏水检测 数据初始化
 * @return stat_m
 */
stat_m m_callable_flow_sensor_type_gets_m(void);

#endif // EARLY_WARNING_H
