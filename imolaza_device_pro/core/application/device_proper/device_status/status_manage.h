
#ifndef __STATUS_MANAGE__H__
#define    __STATUS_MANAGE__H__

#include "core/fset_lib.h"

/**

    初始化状态
    空闲状态
    计划运行状态
    计划浸泡状态
    快速运行运行状态
    快速运行浸泡状态
    快速运行暂停状态
    手动运行运行状态
    设备测试状态
    更新状态

 */
// /** 设备状态概览 */
// enum device_status
// {
//     /** 初始化状态 */
//     M_DEVICE_GLOBAL_STATUS_INIT,
//     /** 离线状态 */
//     M_DEVICE_GLOBAL_STATUS_OFFLINE,
//     /** 空闲状态 */
//     M_DEVICE_GLOBAL_STATUS_ONLINE,
//     /** 快速运行状态 */
//     M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING,
//     /** 计划运行状态 */
//     M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING,
//     /** 手动运行状态 */
//     M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING,
//     /** 测试状态 */
//     M_DEVICE_GLOBAL_STATUS_TEST_RUNNING,
//     /** 更新状态 */
//     M_DEVICE_GLOBAL_STATUS_UPDATEING,
// };





/**
 * @brief 设备状态提更新到 初始化状态
 * 
 * @return stat_m 
 */
stat_m m_callable_device_proper_status_update_to_init(void);
/**
 * @brief 设备状态提更新到 在线状态
 * 
 * @return stat_m 
 */
stat_m m_callable_device_proper_status_update_to_idle(void);
/**
 * @brief 设备状态提更新到 快速运行状态
 * 
 * @return stat_m 
 */
stat_m m_callable_device_proper_status_update_to_fast_running(void);
/**
 * @brief 设备状态提更新到 计划状态
 * 
 * @return stat_m 
 */
stat_m m_callable_device_proper_status_update_to_schedule_running(void);

/**
 * @brief 设备状态提更新到 手动运行状态
 * 
 * @return stat_m 
 */
stat_m m_callable_device_proper_status_update_to_manual_running(void);
/**
 * @brief 设备状态提更新到 更新状态
 * 
 * @return stat_m 
 */
stat_m m_callable_device_proper_status_update_to_update(void);
/**
 * @brief 设备状态提更新到 更新状态
 * 
 * @return stat_m 
 */
stat_m m_callable_device_proper_status_update_to_test(void);
/**
 * @brief 设备状态提更新到 配置状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_config_net(void);
/**
 * @brief 查询当前设备的状态
 * 
 * @param out_ste 输出
 * @return stat_m 
 */
stat_m m_callable_device_proper_status_get(int *out_ste);



#endif