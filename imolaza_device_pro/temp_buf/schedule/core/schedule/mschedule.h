#ifndef __MSCHEDULE_HH__
#define __MSCHEDULE_HH__

// #ifndef(TEST_CODE)
#include "core/core.h"
// #endif

#define M_SCHEDULE_MODE_NEWLWAN_FLAG "nl"
#define MAX_SCHEDU_NUM 16
#define MAX_TIME_NUM 25

// struct _plan_manager_
// {
//     uint64_t current_id;
//     stat_m current_stete;
//     m_watering_schedule * current_running_obj;
// };

// typedef struct _plan_manager_ plan_manager;


/**
 * @brief The run type may be schedule or fast run
*/
enum _schedule_run_type
{
    /** Specifies the type schedule */
    RUN_TYPE_SCHEDULE,
    /** Specifies the type fast run*/
    RUN_TYPE_FAST_RUN,
    /** Specifies the type schedule */
    RUN_TYPE_UNIVERSAL,
};

/**
 * @brief The type of immersion to be run
*/
enum _schedule_sack_type
{
    /** Specifies the type None (Not Use)*/
    SACK_TYPE_NORMAL,
    /** Specifies the type Sack */
    SACK_TYPE_SACK,
    /** Specifies the type Pump*/
    SACK_TYPE_PUMP,
    /** Specifies the type Sack Plus*/
    SACK_TYPE_SACK_PLUS,
};






struct _schedule_run_instance
{
    /** schedule id  */
    uint64_t in_run_id;

    /** instance id (end Time)  */
    uint64_t in_instance_id;

    /** Run type 0 : schedule 1 : QuickRun (not support)  */
    enum _schedule_run_type in_run_type;

    /** channel parameter information, two-dimensional array, the first is the channel run-time, the second is the sack-time (unit: seconds)  */
    uint32_t in_sack_param_run_time;

    /** channel parameter information, two-dimensional array, the first is the channel run-time, the second is the sack-time (unit: seconds)  */
    uint32_t in_sack_param_sack_time;

    /** none and pump mode and sack mode {3 : Multi-region, multi-time} ( 0 : 1 : 2 : 3)  */
    enum _schedule_sack_type in_sack_type;

    /** water hammer is open ( disabe : 0 / open : 1)  */
    uint32_t in_water_hammer;

    /** List of all areas that should be run */
    m_chan_managec in_channel[M_LIMIT_CHANNEL_MAX_NUM]; 

    /** Running of the cache information */
    m_run_data in_running_buff;

    /** A list of valid region runs that should be generated */
    uint8_t disable_channel_list[M_LIMIT_CHANNEL_MAX_NUM];

    /** Run the critical time */
    uint64_t WSTime;

    /** Run the object */
    m_watering_schedule *mobj;
    /**  */
    bool in_hangup;
}; 
typedef struct _schedule_run_instance schedule_instance_m;

/**
 * @brief
 *
 * @param in_id
 * @return stat_m
 */
stat_m m_splan_remove(uint64_t in_id);
/**
 * @brief
 *
 * @param in_id
 * @return stat_m
 */
stat_m m_splan_remove_all(void);

/**
 * @brief 挂起ihua通知恢复
 *
 * @param in_obj
 * @return stat_m
 */
stat_m m_schedule_think_hangup(m_watering_schedule *in_obj);
/**
 * @brief 计划挂起
 *
 * @return stat_m
 */
stat_m m_schedule_think_wakeup(void);

/**
 * @brief 计划刷新
 *
 * @param in_obj
 * @return stat_m
 */
stat_m m_schedule_state_flushed(m_watering_schedule *in_obj);

/**
 * @brief
 *
 * @param ws_item
 * @return stat_m
 */
stat_m m_sch_forthwith_add(m_watering_schedule *ws_item, bool boo);

/**
 * @brief
 *
 * @return stat_m
 */
stat_m splan_flash_recover_item(void);

// plan_manager _plan;

/**
 * @brief 恢复计划
 *
 * @return stat_m
 */
stat_m m_splan_parse_all(void);

/** 从已有的表中 恢复一个计划 ，应用场景可以是 计划启用*/

stat_m m_splan_parse_one(uint64_t in_id);

stat_m m_callable_schedule_initial(void);

/**
 * @brief
 *
 * @param in_handle_type
 * @param specv1_id
 * @param specv2
 * @param specv3
 * @param in_option
 * @return stat_m
 */
stat_m m_callable_flash_flag_special_value_u64(int in_handle_type, uint64_t *specv1_id, uint64_t *specv2, uint64_t *specv3, int in_option);

/** 计划判断的*/
stat_m m_callable_today_need_run_odev(uint8_t in_run_type, uint8_t in_inva_type, uint32_t runtime_sum, uint32_t presenttime);

/**
 * @brief Find the scheduled object from the run list
 *
 * @param[in] in_uid schedule id
 * @param[out] out_obj output object
 * @return stat_m
 */
stat_m m_callable_schedule_list_find(uint64_t in_uid, m_watering_schedule **out_obj);
/**
 * @brief Update new lawn mode schedule time (storage optional, no notification node added)
 *
 * @param[in] in_id schedule id
 * @param[in] consStr time string
 * @param[in] update_flash true: Coverage save false : Do not save
 * @return stat_m
 */
stat_m m_newlwan_mode_schedule_update_time_list(uint64_t in_id, char *consStr, bool update_flash);

/**
 * @brief New lawn mode plans the refresh time and gets the most recent time
 *
 * @param[in] in_id schedule id
 * @param[in] in_present_time current time
 * @param[out] out_result The time of return
 * @param[in] recover Tracing forward or backward according on the scene
 * @return stat_m
 */
stat_m m_newlwan_mode_schedule_flush(uint64_t in_id, uint64_t in_present_time, uint64_t *out_result, bool recover);





/**
 * @brief New lawn mode plans the refresh time and gets the most recent time
 *
 * @param[in] in_inst_obj schedule id
 * @return stat_m
 */
stat_m m_instance_schedule_set( schedule_instance_m *in_inst_obj);

/**
 * @brief New lawn mode plans the refresh time and gets the most recent time
 *
 * @param[out] out_inst_obj schedule id
 * @return stat_m
 */
stat_m m_instance_schedule_is_running_and_get( schedule_instance_m **out_inst_obj);

#endif /**  __MSCHEDULE_HH__ */
