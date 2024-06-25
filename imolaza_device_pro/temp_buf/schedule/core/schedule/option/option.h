#ifndef __OPTION__H__
#define  __OPTION__H__

#include "core/fset_lib.h"





struct m_zone_able
{
    /* 句柄*/
    char zone_list[16][5];
    /* 状态*/
    char zone_state[16];
};





struct __GLOBAL_OPTION
{
    m_time_range global_dormancy;
    m_time_range global_skip;
    struct m_zone_able mz_able;
};

typedef struct __GLOBAL_OPTION GLOBAL_OPTION;

extern GLOBAL_OPTION _option;





/**
 * @brief 初始化 加载flash中的数据
 *
 * @return stat_m
 */
stat_m m_global_zone_init(void);
/**
 * @brief Global zone enabled, he is only for the planned run
 *
 * @param in_zone
 * @return stat_m
 */
stat_m m_global_zone_enable(int in_zone);
/**
 * @brief The Global zone is disabled, and he only targets the planned run,Involplanning to run he will take effect on the next run
 *
 * @param in_zone disable zone num
 * @return stat_m
 */
stat_m m_global_zone_disable(int in_zone);



stat_m m_global_zone_query(int in_zone);




/**
 * @brief 计划区域解析工具
 *
 * @param [in] in_obj 计划对象
 * @param [in] format 区域展示格式
 * @param [in] zseparate 区域间分割符
 * @param [in] in_src 源字符
 * @param [out] out_should_time_array 如果需要的话，可以复制一份运行时间数组 作为后续时间计算
 * @return stat_m
 */
// stat_m m_insi_schedule_compute_zone_distill(m_watering_schedule *in_obj, char *format, char zseparate, char *in_src, uint32_t *out_should_time_array);
stat_m m_operation_schedule_compute_zone_distill(m_watering_schedule *in_obj, char *format, char zseparate, char *in_src, uint32_t *out_should_time_array);

/**
 * @brief Used to calculate whether the current plan is watered or soaked at some time, it can also be used to calculate the total duration of the area(When the update is a false)
 *
 * @param [in] waterObj Plan objects
 * @param [in] sockWater How long it should be watered
 * @param [in] sockTime How long it should be sacked
 * @param [in] tempChanWater Regional details of the current program,
 * @param [in] waterSum The total length of watering throughout the program
 * @param [in] present_time The time of the device now
 * @param [out] out_running_time
 * @return stat_m
 */
stat_m m_operation_sack_recover_schedule_time_and_status(m_watering_schedule *waterObj, int sockWater, int sockTime, uint32_t *tempChanWater, int waterSum, uint32_t present_time, uint32_t *out_running_time);

/**
 * @brief 没有使用浸泡的时候的恢复
 *
 * @param in_Obj 需要恢复的对象
 * @param present_time  恢复对应的当前的时间
 */
stat_m m_operation_normal_recover_schedule_time_and_status(m_watering_schedule *in_Obj , uint32_t present_time);



/**
 * @brief 根据计划开始时间和计算间隔5小时的开始时间
 *
 * @param in_present_time 传入现在的时间点
 * @param in_present_timespeam 传入现在的时间戳
 * @param inver_hour 运行的时间
 * @param planTimesp 计划开始时间戳
 * @param [out] out_waterTime 计算得出的结果存放
 */
void m_operation_schedule_calculate_the_interval_hour(uint32_t in_present_time, uint32_t in_present_timespeam, uint8_t inver_hour, uint64_t planTimesp, uint64_t *out_waterTime); // 传入计划的时间戳 ，

/**
 * @brief 计算计划开始时间
 *
 * @param in_start_time
 * @param in_wstime
 * @param watert_time_sum
 * @param in_schedule_start 计划开始时间
 * @return uint64_t
 */
uint64_t m_operation_schedule_calculate_near_start_timesp(uint64_t in_start_time, uint32_t in_wstime, uint32_t watert_time_sum, uint64_t *in_schedule_start);


/**
 * @brief Count the next execution between the scheduled hours (only 5)
 *
 * @param planTimesp Time stamp for the scheduled start
 * @param waterTime Current watering time buffer
 */
void m_operation_calculate_the_interval_5_hours(uint64_t planTimesp, uint64_t *waterTime , uint32_t hhmmss); // 传入计划的时间戳 ，
/**
 * @brief Count the next execution between the scheduled hours (Not only 5)
 *
 * @param planTimesp Time stamp for the scheduled start
 * @param waterTime Current watering time buffer
 * @param hour interval hour
 */
bool m_operation_nextruntime_interval_hour(uint64_t *wsTime_clo, uint64_t *wsTime, uint32_t hour , uint32_t hhmmss);

/**
 * @brief Count the next execution between the scheduled hours
 *
 * @param planTimesp Time stamp for the scheduled start
 * @param waterTime Current watering time buffer
 */
bool m_operation_date_subtract(uint64_t in_planstart , uint64_t in_planstop, uint8_t cy_day);

/**
 * @brief Count the next execution between the scheduled hours
 *
 * @param in_obj source obj
 * @param in_presenttime Current time 
 * @param in_reserver If there is a skip information to feed back to the serve
 */
bool m_operation_schedule_skip_checkout(m_watering_schedule *in_obj,uint64_t in_presenttime ,bool in_reserver);



/**
 * @brief 计算和执行恢复的新状态和执行操作
 *
 * @param out_plan_obj 对象
 * @param in_state  之前的状态
 * @return enum en_link_tab
 */
int m_operation_recovery_needs_to_run_or_change_the_state(m_watering_schedule *out_plan_obj, stat_m in_state);





#endif







