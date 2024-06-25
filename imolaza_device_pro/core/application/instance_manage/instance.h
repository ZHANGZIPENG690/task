

/**
 * @file instance.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @date 2023-07-10
 */
#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include "core/fset_lib.h"
#include "../../middle/tool/data_parse/data_parse.h"
// #include "instance.h"

// /** */
// enum weather_info
// {
//     /** */
//     M_WEATHER_WIND,
//     /** */
//     M_WEATHER_RAIN,
//     /** */
//     M_WEATHER_FREEZ,
// };

#define MAX_CHANNEL 30
#define FIX_TIME_MIN 4
#define M_M_SCHEDULE_FUNCTION_SACK_CONFIG_A 'A'
#define M_M_SCHEDULE_FUNCTION_SACK_CONFIG_B 'B'

enum unit_status
{
    /** 单元是新的*/
    M_STATIC_UNIT_STATUS_NEW,
    /** 单元在运行的*/
    M_STATIC_UNIT_STATUS_RUNNING,
    /** 单元是中断的*/
    M_STATIC_UNIT_STATUS_INTERRUPT,
    /** 单元是完成的*/
    M_STATIC_UNIT_STATUS_DONE
};


/*************************************/
/*************************************/
/*************************************/

// 单个算法应该准备的是当前所有区域应该运行的时间
// 每次计算都会填充每一个单元
// 单元有 开始的区域   开始的时间  本次应该运行的时间  本次已经运行时间  结束时间
struct instance_running_unit
{
    // 区域的状态
    enum unit_status state;
    // 区域的通道
    uint8_t unit_running_channel;
    // 区域的应该运行时间
    uint32_t unit_should_running_time;
    // 区域的已经运行时间，标准，构建就生成，不是真实浇水
    uint32_t unit_already_running_time;


    // 区域的本次运行开始时间
    uint64_t unit_this_time_running_start_time;
    // 区域的本次运行结束时间
    uint64_t unit_this_time_running_end_time;

    // 区域的本次将要运行时间
    uint32_t unit_this_time_will_running_time;
    // 区域的本次耗费的运行时间 ，和 "unit_this_time_will_running_time" 对应， 当 与他相等的时候那么就表示这个单元序列是运行完成的 也需要通过该数值和他的差距来判断和计算应该继续运行多少时间，当时快速运行的时候最高时间表示单元总的已经运行时间，而 unit_this_time_real_water_statistics 则表示本次实际运行时间，每次都会从零开始
    uint32_t unit_this_time_cost_running_time;


    // 区域真实浇水统计，理论 ,表示本次实际运行时间，每次都会从零开始，直到被打断或者结束
    uint32_t unit_this_time_real_water_statistics;

    // 单元对应的区域总的运行时间
    uint32_t *unit_zone_real_total_time;


    // 相似单元的数量
    uint64_t unit_similar_num;
    // 单元完成度 1 未完成 2 完成
    int unit_done;
    // 单元进度 1 - 2 - 3 - 4 - 5 - 6
    int unit_progress;
    // 运行类型
    enum base_state unit_running_type;
    // 下一个运行单元体
    struct instance_running_unit *next_running_unit;
    // 上一个运行单元体
    struct instance_running_unit *pre_running_unit;
};
char *toname(int en);

// struct channel_config
// {
//     /** channel id */
//     uint8_t cd_cahnnel_id;
//     /** channel is enable */
//     uint8_t cd_enable;
//     /** channel is opened sack */
//     uint8_t cd_channel_type;
//     /** channel use sack : water time*/
//     uint8_t cd_sack_should_water_time;
//     /** channel use sack : sack time */
//     uint8_t cd_sack_should_sack_time;
//     /** channel should running time sum*/
//     uint32_t cd_should_run_time;
//     /** channel reality running time sum*/
//     uint32_t cd_reality_run_time;
//     /** 將要運行時間*/
//     uint32_t cd_curr_will_run_time;
//     /** 运行时间范围*/
//     uint64_t cd_running_end_time;
//     /** 最后一个区域运行的时间*/
//     uint32_t cd_last_running_time;
//     // m_time_range channel_running_time_region;
//     //     /** 暫停的時間 開始暫停和結束暫停的時間*/
//     // m_time_range pause_recover_time;
// };

//
// 定义一下 所有的区域的根节点指针
// 简单理解就是所有节点的集合    区域1 -> 分片1 分片2 分片3
//                           区域2 -> 分片1 分片2 分片3
// struct instance_running_unit *p_root_array[MAX_CHANNEL][MAX_CHANNEL];
// 还需要一个单元序列的队列   用来存储和改变计划的与运行顺序
// 保证长度可变
struct instance_running_sequences
{
    // 所有序列的头节点
    struct instance_running_unit *head_running_unit;
    // 在进入一个结束函数的时候 他一定是指向当前"正在运行" ”结束之前“ 的区域
    struct instance_running_unit *current_running_unit;
    // 所有序列的尾部
    struct instance_running_unit *tail_running_unit;
};

/*************************************/
/*************************************/
/*************************************/

//     /** 运行缓存数据*/
// struct run_buff
// {
//     /** 运行状态 运行中,浸泡,空闲 */
//     uint8_t rb_state;
//     /** 上一个区域指针*/
//     uint8_t rb_pre_pc;
//     /** 下一个区域指针*/
//     uint8_t rb_next_pc;
//     /** 当前指针*/
//     uint8_t rb_pc;
//     /** 最大运行区域*/
//     uint8_t maxChan;
//     /** 最大运行时间*/
//     uint64_t maxTime;
//     /** 可变时间缓存*/
//     uint64_t rb_WSTime_clone;
//     /** 跳过信息缓存*/
//     uint32_t wind_water_ice_skip;
//     /** 区域已经运行时间总和*/
//     uint32_t chan_already_RunTimeSum;
//     /** 运行时间范围*/
//     m_time_range current_running_time_region;
// }m_run_data;

/**
 * @brief schedule instance object Property description
 */
typedef struct
{
    /** schedule id */
    uint64_t sio_running_id;
    /** instance id  (end Time)*/
    uint64_t sio_instance_id;
    /** schedule status */
    uint64_t sio_status;
    /** max channel */
    uint64_t sio_max_channel;
    /** max channel */
    uint64_t sio_clone_max_channel;
    /** 最大耗费时间 运行和浸泡等等的总时长*/
    uint64_t sio_max_coust_time;
    /** Maximum consumption time for not including Soak (最大应该运行时间,不是总时间  不含浸泡)*/
    uint64_t sio_max_should_runtime;
    /** Run type    0 : schedule  1 : QuickRun (not support) */
    enum instance_run_type sio_run_type;
    /** none and pump mode and  sack mode {3 : Multi-region, multi-time} ( 0 : 1 : 2 : 3) */
    enum m_funcation sio_pump_or_sack;
    /** water hammer is open   ( disabe : 0 / open : 1) */
    uint8_t sio_hammer;
    // 当前的浇水无效 当为True时
    bool invalid;
    /** 是否产生与运行信息*/
    bool is_genger_info;
    /** 运行过程中的天气信息 ，这个开始是用来标记部分跳过不恢复的情况 */
    int sio_skip_weather;
    // /**  */
    uint64_t WSTime;
    /** 实例域已经运行时间总和*/
    uint32_t cb_already_run_time_sum;
    /** 已经运行时间 一輪*/
    uint32_t cb_total_already_run_time_one_round;
    /** channel parameter information, two-dimensional array, the first is the channel id, the second is the running time  */
    struct channel_config sio_channel[MAX_CHANNEL];
    /**  運行緩存數據 */
    // m_run_data *running_buff;
    struct instance_running_sequences *instance_running_root_unit;
} instance_obj;

/** 状态改变的原因合集 */
enum fsm_change_cause
{
    /** 自然原因 表示数据是Doc */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE = 0,
    /** 远端控制 */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_REMOTE,
    /** 本地 */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL,
    /** 本地左 */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_LEFT,
    /** 本地右 */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_MANUAL_RIGHT,

    /** 数据无效 表示数据是Obj */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL,
    /** 不需要返回 */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_NOOE_CALLBACK,
    /** 全局跳过状态反馈 */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_GLOBAL_BACK,
    /**单个跳过状态反馈 */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_SIGN_BACK,
    /** 水锤 */
    M_STATIC_FSM_CHANGE_STATE_CAUSE_WATER_HAMMER,
};
struct instance_fsm_ssmc
{
    uint64_t fs_dat;
    enum base_state fs_pre_state;
};
struct base_fsm
{
    // const enum base_state pre_state;
    const enum base_state current_state;
    stat_m (*base_fsm_fun)(enum fsm_change_cause, enum base_state, void *, uint64_t);
    void *data;
    enum fsm_change_cause change_cause;
    uint64_t time_s;
};

stat_m m_static_instance_manage_init(void);

stat_m m_static_instance_manage_set_current_instance(instance_obj *instance);

/**
 * @brief According to the information generation instance, it should be generated in 10min, the generated instance is stored in the linked list, and the subsequent execution is obtained from the beginning
 * @brief  step-->
 * @brief         1、Regional adjustment（ skip_channel_list ）
 * @brief         2、Weather information marker, recorded in instance middle
 * @brief         3、Build an instance
 * @brief         4、Join the instance run list
 *
 * @param sch_doc_obj schedule list object
 * @param skip_channel_list List of skipped regions uint8_t skip_channel_list[16]
 * @param weather wrether info  (see enum weather_info)
 * @param current_time current time
 * @return stat_m
 */
// stat_m m_callable_instance_manage_generate(instance_doc_t *sch_doc_obj, uint8_t *skip_channel_list, enum weather_info weather, uint64_t current_time);
/**
 * @brief Get the pre-generated running instance when you schedule the run, and if it is not found, directly generate a new one
 *
 * @param schedule_id schedule id
 * @param schedule_instance The resulting running instance
 * @param current_time current time
 * @return stat_m
 */
stat_m m_callable_instance_manage_get_next(uint64_t id, instance_obj *instance, uint64_t current_time);

/**
 * @brief Gets the current running instance
 *
 * @param instance The resulting running instance
 * @param current_time current time
 * @return stat_m
 */
stat_m m_static_instance_manage_get_current_instance(instance_obj **instance);

/**
 * @brief 获取当前实例的运行 id 和 区域运行 id
 *
 * @param out_sh_id 输出的实例 id
 * @param out_ch_id 输出的区域 id
 * @return stat_m
 */
stat_m m_callable_instance_manage_get_current_instance_sid_abd_cid(uint64_t *out_sh_id, int *out_ch_id);

/**
 * @brief 获取当前实例的运行 id 和 区域运行 id
 *
 * @param device_state 传入设备状态
 * @param out_sh_id 输出的计划 id
 * @param out_instance_id 输出的实例 id
 * @param ch_id 输出的区域 id
 * @param remainder_time 剩余运行时间
 * @param out_ch_state 输出的状态
 * @param out_running_queue 运行队列
 * @param out_running_queue_local  运行队列的位置
 * @return stat_m
 */
stat_m m_callable_instance_manage_get_current_instance_sid_abd_cid_adb_remainder_run_time(int device_state,
                                                                                          uint64_t *out_sh_id,
                                                                                          uint64_t *out_instance_id,
                                                                                          int *ch_id,
                                                                                          uint64_t *remainder_time,
                                                                                          int *out_ch_state,
                                                                                          char *out_running_queue,
                                                                                          int *out_running_queue_local,
                                                                                          uint64_t in_time_s);
/**
 * @brief 查看当前运行的计划是不是他
 *
 * @param in_uid 他
 * @return stat_m succ_r 是的
 */
stat_m m_static_instance_manage_current_instance_id_is_effective(uint64_t in_uid);
/**
 * @brief 查看当前为区域是影响
 *
 * @param in_uid 查询区域
 * @return stat_m
 */
stat_m m_static_instance_manage_current_channel_is_effective(int in_uid);
/**
 * @brief 查看当前有没有运行计划
 *
 * @param is_schedule 想知道是计划还是快速运行 true 判断计划 false 判断快速运行
 * @return stat_m succ_r 是的
 */
stat_m m_static_instance_manage_current_instance_is_effective(bool is_schedule);
/**
 * @brief
 *
 * @param in_info
 * @param in_time_s
 * @return stat_m
 */
stat_m m_static_instance_manage_explain_instance_obj(instance_doc_t *in_info, uint64_t in_time_s);

/**
 * @brief 输出实例信息 ，用来调试
 *
 * @param water_d  实例对象
 * @param current_time 当前的时间
 */
void sch_print(instance_doc_t *water_d, uint64_t current_time);

/**
 * @brief 根据 30 返回的参数生成对象的运行实例
 *
 * @param in_uid 对象的id
 * @param in_time_s 发生的时间
 * @return stat_m 成功或者失败
 */
stat_m m_callable_instance_running_gener_obj(uint64_t in_uid, uint64_t in_time_s);

/**
 * @brief 获取待恢复计划 并且执行
 *
 * @param in_time_s
 * @return stat_m
 */
stat_m m_callable_instance_manage_recover_queue_get_and_exec(uint64_t in_id ,uint64_t in_time_s);

/**
 * @brief 停止计划运行
 *
 * @param cause 如果是手动停止记录停止的时间
 * @param is_callback 是否需要反馈
 * @param in_time_s 触发的时间
 * @return stat_m
 */
stat_m m_callable_instance_running_forced_stop(enum fsm_change_cause cause, bool is_callback, uint64_t in_time_s);

/**
 * @brief 快速解析一个实例 快速运行
 *
 * @param src_str 快速运行格式
 * @param out_obj 发明阔的实例对象
 * @param in_time_s 当前的时间
 * @return stat_m
 */
stat_m m_static_tools_instance_obj_quick_explain(char *src_str, instance_obj *out_obj, uint64_t in_time_s);

/**
 * @brief 直接删掉禁用
 *
 * @param in_uid id
 * @return stat_m
 */
stat_m m_callable_exce_schedule_stroage_en_dis_able(uint64_t in_uid, bool in_en);

/**
 * @brief 清除实例队列
 *
 * @param root_inst_queue 队列 Root
 * @return int
 */
extern int m_static_instance_runnning_queue_clear(struct instance_running_sequences *root_inst_queue);


/**
 * @brief 清除传感器信息暂存
 *
 * @param in_time_s 依据
 * @return stat_m
 */
extern stat_m m_callable_sensor_staging_clean(uint64_t schid, uint64_t in_time_s);

/**
 * @brief Extract the schedule content characters from the existing list by id
 * @todo Unit Out ...
 *
 * @param schedule_id schedule id
 * @param sch_str output schedule string
 * @param postion postion
 * @return stat_m
 */
extern stat_m m_callable_schedule_stroage_read_by_param(uint64_t schedule_id, char **out_str, int postion);

/**
 * @brief 根据已有的区域参数生成运行队列
 *
 * @param arr 区域信息
 * @param max_channel 最大运行区域
 * @param water_start_time 浇水开始时间
 * @param out_sequences 生成的队列
 * @param in_tim_s 当前时间
 * @return int
 */
int m_static_instance_sack_algorithm_queue_gener(struct channel_config *arr, int max_channel, int water_start_time,
                                                 struct instance_running_sequences *root_sequences,
                                                 uint64_t *final_max_time,
                                                 bool is_build,
                                                 uint64_t in_tim_s);
/**
 * @brief 记录计划终止时间，防止下次计算
 *
 */
void m_static_instance_enreg_schedule_end_time(uint64_t uid, uint64_t in_time);
#endif /* __INSTANCE_H__ */

/**
 * -----------------------------------
 *
 * -----------------------------------
 */
