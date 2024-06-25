/**
 * @file transmission_guarantee.c
 * @author your name (you@domain.com)
 * @brief 发送保障
 * @version 0.1
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "core/fset_lib.h"

static int m_lock_value = 0;
/**
 * @brief 设置数据收发保障
 *
 * @param in_wait_value 值
 * @return stat_m
 */
stat_m m_static_tranfmiss_guarantee_set_value(int in_wait_value, char *in_buf)
{
    if (in_wait_value == 1 && in_buf != NULL)
    {
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_buf, "%d", &in_wait_value);
        if (in_wait_value != 100)
            m_lock_value = in_wait_value;
        // DEBUG_TEST( DB_I,"111 - - - %s-- - - - --%d", in_buf, m_lock_value);
    }
    else if (in_wait_value > 1 && in_wait_value != 100)
    {
        m_lock_value = in_wait_value;
        // DEBUG_TEST( DB_I," - - - -- - - - --%d", m_lock_value);
    }
    return succ_r;
}

// 1.发16 有可能会回 20、22 (2月28新增*35)
// 2.发21 有可能会回  20、22 (2月28新增*35)
// 3.发23 有可能会回  20、22 (2月28新增*35)
stat_m m_static_tranfmiss_guarantee_spec_explain_vue(int in_wait_value)
{
    stat_m stat = fail_r;
    if (
        (in_wait_value == M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE && (m_lock_value == M_CMD_TWOWAY_ADD_A_SCHEDULE || m_lock_value == M_CMD_TWOWAY_DELE_SCHEDULE)) ||
        (in_wait_value == M_CMD_NOTIFY_TO_SERVER_ADD_SCH_RESP && (m_lock_value == M_CMD_TWOWAY_ADD_A_SCHEDULE || m_lock_value == M_CMD_TWOWAY_DELE_SCHEDULE)) ||
        (in_wait_value == M_CMD_TWOWAY_DELE_SCHEDULE_RESP && (m_lock_value == M_CMD_TWOWAY_ADD_A_SCHEDULE || m_lock_value == M_CMD_TWOWAY_DELE_SCHEDULE)) ||
        (in_wait_value == M_CMD_NOTIFY_TO_DEVICE_REQUEST_LOCAL_INFO && (m_lock_value == M_CMD_TWOWAY_ADD_A_SCHEDULE || m_lock_value == M_CMD_NOTIFY_TO_SERVER_REQUEST_LOCAL_INFO))

    )
        stat = succ_r;

    return stat;
}

static bool wait_resp_dot_to_dot = false;
/**
 * @brief 数据校对
 *
 * @param in_value 收到的值  一般 1
 * @param in_buf 收到的缓存 只取第一个
 * @return stat_m
 */
stat_m m_static_tranfmiss_guarantee_proofread(int in_wait_value)
{
    stat_m stat = fail_r;

    if (!wait_resp_dot_to_dot)
    {
        DEBUG_TEST(DB_W,"Waiting %d  Come on %d", in_wait_value, m_lock_value);
    }
    else
    {
        DEBUG_TEST_NOT_LINE(".");
    }
    if (m_lock_value == in_wait_value)
    {
        stat = succ_r;
    }
    else
    {
        stat = m_static_tranfmiss_guarantee_spec_explain_vue(in_wait_value);
    }
    (stat == succ_r) ? (wait_resp_dot_to_dot = false, m_lock_value = 0) : (wait_resp_dot_to_dot = true);

    return stat;
}
