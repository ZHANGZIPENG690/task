

#include "core/fset_lib.h"
#include "error_handle.h"

struct main_error_handle
{
    enum error_net_code err_code;
    int count;
};

struct main_error_handle main_err[50] = {0};

enum net_connect_code main_net_progress = M_NET_CONNECT_CODE_INIT;

/**
 * @brief 设置错误信息
 *
 * @param ecd
 * @return stat_m
 */
stat_m m_callable_middle_err_handle_set_code(enum error_net_code ecd)
{
    stat_m stat = fail_r;
    for (int i = 0; i < 40; i++)
    {
        if (main_err[i].err_code == 0 || main_err[i].err_code == ecd)
        {
            main_err[i].err_code = ecd;
            main_err[i].count++;
            break;
        }
    }

    return stat;
}
/**
 * @brief 流程代码设置 包含配置网络过程和整个连接服务器过程
 *
 * @param hcd
 * @return stat_m
 */
stat_m m_callable_middle_connect_handle_set_code(enum net_connect_code hcd)
{
    stat_m stat = fail_r;
    /** 新的流程大于现在的流程 */
    if (hcd > main_net_progress || (hcd < main_net_progress && hcd >= M_CONNECT_ERROR_CODE_DNS_RESOLVE_FAIL) ||
        (hcd < main_net_progress && hcd < M_CONNECT_ERROR_CODE_DNS_RESOLVE_FAIL && main_net_progress < M_CONNECT_ERROR_CODE_DNS_RESOLVE_FAIL))
    {
        DEBUG_TEST(DB_ONLY_PROGRESS, "ex : %d", hcd);
        main_net_progress = hcd;
        stat = succ_r;
    }
    return stat;
}
/**
 * @brief 获取当前连接流程
 *
 * @param hcd
 * @return stat_m
 */
stat_m m_callable_middle_connect_handle_get_code(int *hcd)
{
    /** 新的流程大于现在的流程 */
    *hcd = main_net_progress;
    return succ_r;
}

/**
 * @brief 输出信息到服务器
 *
 * @return stat_m
 */
stat_m m_callable_middle_err_handle_printf(void)
{
    stat_m stat = fail_r;
    char temp_str_cm[512] = {0};
    char temp_str[40] = {0};
    for (int i = 0; i < 10; i++)
    {
        if (main_err[i].err_code > 0 && main_err[i].count > 0)
        {
            sprintf(temp_str, "0x%04x:%d-", main_err[i].err_code, main_err[i].count);
            main_err[i].err_code = 0;
            main_err[i].count = 0;
            strcat(temp_str_cm, temp_str);
            stat = succ_r;
        }
        else
            break;
    }
    if (stat == succ_r)
    {
        DEBUG_TEST(DB_I, "%s", temp_str_cm);
        m_callable_local_resp_to_remote(100, M_TYPE_Str, temp_str_cm, M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, 0, true);
    }
    return stat;
}
