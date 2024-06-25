

/**
 * @file sensor.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @date 2023-07-10
 */

#ifndef __SENSOR__H__
#define __SENSOR__H__

#include "core/fset_lib.h"

#define BADGER_228PV15_K_VALUE 1.697
#define BADGER_228PV15_OFFSET_VALUE -0.316

#define BADGER_228PV20_K_VALUE 2.8429
#define BADGER_228PV20_OFFSET_VALUE 0.1435

#define BADGER_228PV30_K_VALUE 8.309
#define BADGER_228PV30_OFFSET_VALUE 0.227

#define BADGER_228PV40_K_VALUE 13.7428
#define BADGER_228PV40_OFFSET_VALUE 0.237

#define TORO_TFS_050_K_VALUE 0.078
#define TORO_TFS_050_OFFSET_VALUE 0.9

#define TORO_TFS_075_K_VALUE 0.1563
#define TORO_TFS_075_OFFSET_VALUE 0.9

#define TORO_TFS_100_K_VALUE 0.26112
#define TORO_TFS_100_OFFSET_VALUE 1.2

#define TORO_TFS_150_K_VALUE 1.699
#define TORO_TFS_150_OFFSET_VALUE -3.016

#define TORO_TFS_200_K_VALUE 2.8249
#define TORO_TFS_200_OFFSET_VALUE 0.1435

#define TORO_TFS_300_K_VALUE 8.309
#define TORO_TFS_300_OFFSET_VALUE 0.227

#define TORO_TFS_400_K_VALUE 13.74283
#define TORO_TFS_400_OFFSET_VALUE 0.23707

// #define CST_FSI_T10_K_VALUE 0.776
// #define CST_FSI_T10_OFFSET_VALUE 0

#define CST_FSI_T10_K_VALUE 0.2914
#define CST_FSI_T10_OFFSET_VALUE 0.20

#define CST_FSI_T15_K_VALUE 0.650
#define CST_FSI_T15_OFFSET_VALUE 0.75

#define CST_FSI_T20_K_VALUE 1.192
#define CST_FSI_T20_OFFSET_VALUE 0.94

#define EVERYBODY_1004_K_VALUE 0.322
#define EVERYBODY_1004_OFFSET_VALUE 0.2

#define FLOMEC_QS200_10_SCH40_K_VALUE 0.5575
#define FLOMEC_QS200_10_SCH40_OFFSET_VALUE 0

#define FLOMEC_QS200_10_SCH80_K_VALUE 0.5354
#define FLOMEC_QS200_10_SCH80_OFFSET_VALUE 0

#define FLOMEC_QS200_15_SCH40_K_VALUE 0.7923
#define FLOMEC_QS200_15_SCH40_OFFSET_VALUE 0

#define FLOMEC_QS200_15_SCH80_K_VALUE 0.7860
#define FLOMEC_QS200_15_SCH80_OFFSET_VALUE 0

#define FLOMEC_QS200_20_SCH40_K_VALUE 1.4610
#define FLOMEC_QS200_20_SCH40_OFFSET_VALUE 0

#define FLOMEC_QS200_20_SCH80_K_VALUE 1.4568
#define FLOMEC_QS200_20_SCH80_OFFSET_VALUE 0

#define FLOMEC_QS200_30_SCH40_K_VALUE 4.2630
#define FLOMEC_QS200_30_SCH40_OFFSET_VALUE 0

#define FLOMEC_QS200_30_SCH80_K_VALUE 4.0850
#define FLOMEC_QS200_30_SCH80_OFFSET_VALUE 0

#define FLOMEC_QS200_40_SCH40_K_VALUE 8.0881
#define FLOMEC_QS200_40_SCH40_OFFSET_VALUE 0

#define FLOMEC_QS200_40_SCH80_K_VALUE 7.9062
#define FLOMEC_QS200_40_SCH80_OFFSET_VALUE 0

// #define FLOMEC_QS100_SCH40_K_VALUE 0.4211
// #define FLOMEC_QS100_SCH40_OFFSET_VALUE 0

#define FLOMEC_QS100_SCH40_K_VALUE 0.8422
#define FLOMEC_QS100_SCH40_OFFSET_VALUE 0

// #define FLOMEC_QS100_SCH80_K_VALUE 0.3963
// #define FLOMEC_QS100_SCH80_OFFSET_VALUE 0

#define FLOMEC_QS100_SCH80_K_VALUE 0.732
#define FLOMEC_QS100_SCH80_OFFSET_VALUE 0

#define FG100_K_VALUE 2.8017
#define FG100_OFFSET_VALUE 0

// #define FG100_K_VALUE 2.6756
// #define FG100_OFFSET_VALUE 0.1145

// #define FS350B_K_VALUE 1.07
// #define FS350B_OFFSET_VALUE 0

#define FS350B_K_VALUE 1.4097
#define FS350B_OFFSET_VALUE 0

/** 流量算法管理 */
struct m_sensor_flow_man
{
    /*第一个脉冲*/
    uint8_t s_pulse_1_num;
    /*第二个脉冲*/
    uint8_t s_pulse_2_num;
    /*脉冲检测标志位*/
    uint8_t s_pulese_detection;
    /*第一个脉冲*/
    uint8_t s_pulse_1_num_minu;
    /*第二个脉冲*/
    uint8_t s_pulse_2_num_minu;
    /*脉冲检测标志位*/
    uint8_t s_pulese_detection_minu;
};
struct m_sensor_flow_man flow_man;

// // 触发状态
// enum is_effective
// {
//     // 两种都需要触发
//     M_SENSOR_IS_EFFECTIVE_NONE,
//     // 还没触发
//     M_SENSOR_IS_EFFECTIVE_NOT_TRIG,
//     // 已经触发
//     M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG,
// };

/** 传感器管理 */
struct m_sensor_man
{
    /** 传感器类型  M_SENSOR_MANAGE_TYPE_NORMALLY_CLOSED_SWITCH  M_SENSOR_MANAGE_TYPE_NORMALLY_FLOW_RATE*/
    enum seneor_type sen1_type;
    /** 传感器类型  M_SENSOR_MANAGE_TYPE_NORMALLY_CLOSED_SWITCH  M_SENSOR_MANAGE_TYPE_NORMALLY_FLOW_RATE*/
    enum seneor_type sen2_type;

    /** 传感器 1  */
    int sen1_pin;
    /** 传感器 2 */
    int sen2_pin;

    /** 传感器 1 已经开启 */
    bool sen_1_is_opened;
    /** 传感器 2 已经开启 */
    bool sen_2_is_opened;

    /** 传感器 1 是否触发   */
    enum is_effective sen_1_is_effective;
    /** 传感器 2 是否触发 */
    enum is_effective sen_2_is_effective;

    /** 传感器 1 触发计数 */
    long long sen_1_trigger_count;
    /** 传感器 2 触发计数 */
    long long sen_2_trigger_count;

    /** 传感器 1 触发计数 */
    long long sen_1_trigger_count_backup_copy;
    /** 传感器 2 触发计数 */
    long long sen_2_trigger_count_backup_copy;

    /** 雨量传感器 检测时间间隔 */
    uint32_t sen_water_check_time_interval;
    /** 流量传感器检测时间间隔 */
    uint32_t sen_flow_check_time_interval;

    /**流量传感器型号*/
    uint8_t sensor_type;

    /**流量计算法 K值*/
    float k_value;

    /**流量计算法偏移值*/
    float offset_value;

    /*流量计漏水检测*/
    int sensor_pulsecount_leak;

    /*脉冲周期*/
    uint16_t s_pulse_time[35];

    /*流速*/
    float flow_speed[35];
};

struct m_sensor_man sen_man;

/**
 * @brief 传感器初始化 加载传感器配置
 * @todo Unit Input ...
 *
 *
 */
stat_m m_callable_sensor_init(int sen1_pin, int sen2_pin);
/**
 * @brief 以特定的格式设置传感器，开关和类型
 * @todo Unit Input ...
 *
 * @param channel
 * @param type
 */
stat_m m_callable_sensor_set(int tp_vue, enum sensor_type_m sensor_type, bool boo);
/**
 * @brief 以指定类型开启传感器
 * @todo Unit Input ...
 *
 * @param channel
 * @param type
 */
stat_m m_callable_sensor_set_open(uint8_t channel, uint8_t type);
/**
 * @brief 关闭传感器
 * @todo Unit Input ...
 *
 * @param channel
 * @param type
 */
stat_m m_callable_sensor_set_close(uint8_t channel);

/**
 * @brief 传感器监控，需要循环调用 以保证在传感器主动动作后 设备能在指定的某个合适的时间内发现并且触发
 * @todo Unit Input ...
 *
 * @param current_time 当前的时间
 */
stat_m m_callable_sensor_monitor(uint64_t current_time);
/**
 * @brief 检测当前是否处于传感器触发
 *
 * @param sen_chan 如果触发，参数返回的是触发的通道
 * @param sen_type 如果触发，参数返回的是触发的传感器类型
 * @param current_time 当前的时间
 * @return stat_m succ_r 表示已经触发 否则否则
 */
stat_m m_callable_sensor_current_is_effective(enum seneor_chann *sen_chan, enum seneor_type *sen_type, uint64_t current_time);

/**
 * @brief 计算当前运行区域的中断次数
 *
 * @param flag  反映当前爱你区域的因较编号，可以根据缓存查询到对应的通道id
 * @return stat_m
 */
stat_m m_callable_sensor_interrupt_count(int flag);

/**
 * @brief 监控如果触发，会调用事件处理函数，你可以在里面做一些你想做的操作
 *
 * @param sensor_channel 触发的通道
 * @param is_open 通道是否被设置打开
 * @param is_eff 操作是断开还是触发
 * @param sen_type 触发的传感器类造型
 * @param num_sum 总的检测次数
 * @param num_curr 检测次数内被触发的次数
 * @return stat_m
 */
stat_m m_static_sensor_event_handle(enum seneor_chann sensor_channel, bool is_open,
                                    enum is_effective is_eff, enum seneor_type sen_type, uint64_t num_sum, uint64_t num_curr);

/**
 * @brief 传感器触发处理函数  作简单的清除 但是不会释放数据 ，等待恢复才会被释放 或者 超出运行时间
 *
 * @param in_obj 被触发的对象
 * @param in_time_s 这个不是触发的时间 而是 触发通道和触发类型的结合
 * @return stat_m
 */
stat_m m_callable_sensor_staging_trig_handle(void *in_obj, uint64_t in_time_s);

/**
 * @brief 解除触发处理函数，用于计算恢复 如果不需要恢复 将会被清理释放
 *
 * @param in_obj 解除触发对象
 * @param in_time_s 触发通道和触发类型的结合
 * @return stat_m
 */
stat_m m_callable_sensor_staging_distrig_handle(uint64_t in_time_s);

/**
 * @brief 清除传感器信息暂存
 *
 * @param in_time_s 依据
 * @return stat_m
 */
stat_m m_callable_sensor_staging_clean(uint64_t schid, uint64_t in_time_s);

/**
 * @brief 设置传感器检测时间间隔
 *
 * @param in_water_type_time 雨量传感器时间间隔
 * @param in_flow_type_time 流量传感器时间间隔
 * @return stat_m
 */
stat_m m_callable_sensor_set_check_interval_time(uint32_t in_water_type_time, uint32_t in_flow_type_time);
/**
 * @brief 直接添加一个传感器
 *
 * @param inobj 对象
 * @param in_time_s  时间
 * @return stat_m
 */
stat_m m_callable_sensor_staging_add_trig(void *inobj, uint64_t in_time_s);
/**
 * @brief 获取自上次到现在的流量值，并且指定是否清除缓存并且重新开始计算，如果流量传感器未打开 返回0
 *
 * @param is_clear 是否清除缓存
 * @return stat_m
 */
// stat_m m_callable_flow_sensor_get_total_value(float *out_final_vue, bool is_clear);
/**
 * @brief 检查被触发的id 是不是传入ID
 *
 * @param in_chk_id ID
 * @return stat_m
 */
stat_m m_callable_sensor_staging_check_obj_id(uint64_t in_chk_id);

stat_m m_callable_flow_sensor_get_total_value(uint8_t channel_id, uint32_t running_time, float *out_final_vue, bool is_clear);
stat_m m_callable_flow_sensor_type_gets(uint8_t *senseortype);

stat_m m_callable_flow_sensor_type_gets(uint8_t *senseortype);
#endif
