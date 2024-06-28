

#include "status_manage.h"

static enum device_status curr_device_status;

/** 设备状态概览 */
// enum device_status
// {
//     /** 初始化状态 */
//     M_DEVICE_GLOBAL_STATUS_INIT,
//     /** 空闲状态 */
//     M_DEVICE_GLOBAL_STATUS_IDLE,
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
stat_m m_callable_device_proper_status_update_to_init(void)
{
    stat_m stat = fail_r;
    curr_device_status = M_DEVICE_GLOBAL_STATUS_INIT;
    return stat;
}
/**
 * @brief 设备状态提更新到 在线状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_idle(void)
{
    stat_m stat = fail_r;
    curr_device_status = M_DEVICE_GLOBAL_STATUS_IDLE;
    return stat;
}
/**
 * @brief 更新到 M短路状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_short_M_pump(void)
{
    stat_m stat = fail_r;
    curr_device_status = M_DEVICE_GLOBAL_STATUS_SHORT_M_PUMP;
    return stat;
}
/**
 * @brief 设备状态提更新到 快速运行状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_fast_running(void)
{
    stat_m stat = fail_r;
    curr_device_status = M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING;

    return stat;
}
/**
 * @brief 设备状态提更新到 计划状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_schedule_running(void)
{
    stat_m stat = fail_r;
    curr_device_status = M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING;
    return stat;
}

/**
 * @brief 设备状态提更新到 手动运行状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_manual_running(void)
{
    stat_m stat = fail_r;

    curr_device_status = M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING;
    return stat;
}
/**
 * @brief 设备状态提更新到 更新状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_update(void)
{
    stat_m stat = fail_r;
    curr_device_status = M_DEVICE_GLOBAL_STATUS_UPDATEING;
    return stat;
}
/**
 * @brief 设备状态提更新到 测试状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_test(void)
{
    stat_m stat = fail_r;
    curr_device_status = M_DEVICE_GLOBAL_STATUS_TEST_RUNNING;
    return stat;
}
/**
 * @brief 设备状态提更新到 配置状态
 *
 * @return stat_m
 */
stat_m m_callable_device_proper_status_update_to_config_net(void)
{
    stat_m stat = fail_r;
    curr_device_status = M_DEVICE_GLOBAL_STATUS_CONFIG_NER;
    return stat;
}

/**
 * @brief 查询当前设备的状态
 *
 * @param out_ste 输出
 * @return stat_m
 */
stat_m m_callable_device_proper_status_get(int *out_ste)
{
    stat_m stat = fail_r;
    *out_ste = curr_device_status;
    return stat;
}