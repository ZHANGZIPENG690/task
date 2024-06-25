#include "offline_bluetooth.h"

struct offline_bluetooth offline_bluetooth_m;

/*离线模式 绑定标志*/
bool auth_flag = false;
/*离线模式 解除绑定标志*/
bool bind_flag = false;
// 函数原型声明
int checkSubstring(const char *str1, const char *str2);

// 封装函数，返回1表示通过，0表示失败
int validateStrings(const char *str1, const char *str2)
{
    // 判断字符串长度是否符合要求
    if (strlen(str1) != 16)
    {
        return 0; // 失败
    }
    // 调用内部函数检查子串是否匹配
    return checkSubstring(str1, str2);
}

// 检查str1是否是str2的前16个字符
int checkSubstring(const char *str1, const char *str2)
{
    // 使用strncmp函数比较前16个字符是否相等
    if (strncmp(str1, str2, 16) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief 离线蓝牙模式 手机与设备 进行 登录认证
 * @param token 登录认证密钥
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login_authentication(char *token)
{
    /** 登陆token*/
    char my_token_get[240] = "ccq";
    char tmp_use_str_a[50] = {0};
    stat_m stat = succ_r;
    int context_cmd;
    int temp_use_int_vue_b = 0;
    m_ext_network_config_user_login_cmd(my_token_get, 0);
    if (strstr(my_token_get, "[") != NULL)
    {
        if (m_callable_data_parse_format_cmd(&context_cmd, my_token_get) == succ_r)
        {
            char *comma_position = strchr(my_token_get, ',');

            if (comma_position != NULL)
            {
                strncpy(tmp_use_str_a, my_token_get, comma_position - my_token_get);
                tmp_use_str_a[comma_position - my_token_get] = '\0';
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(tmp_use_str_a, "%49s", tmp_use_str_a);
            }
            else
            {
                strcpy(tmp_use_str_a, my_token_get);
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(tmp_use_str_a, "%49s", tmp_use_str_a);
            }
        }
    }
    DEBUG_TEST(DB_I, "Token:   %s  %s", tmp_use_str_a, token);

    // 调用封装函数验证字符串
    if (validateStrings(token, tmp_use_str_a))
    {
        DEBUG_TEST(DB_I, "Token 一致");
        m_callable_device_proper_status_get(&temp_use_int_vue_b);
        if ((temp_use_int_vue_b != M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING && temp_use_int_vue_b != M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING && temp_use_int_vue_b != M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING))
        {
            m_callable_device_proper_status_update_to_idle();
        }

        auth_flag = true;
    }
    else
    {
        DEBUG_TEST(DB_I, "Token 错误");
        stat = fail_r;
    }
    return stat;
}

/**
 * @brief 离线蓝牙模式 手机登录
 * @param tmp_sid 登录认证密钥
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login(char *tmp_sid)
{
    if (m_callable_offline_bluetooth_login_authentication(tmp_sid) == succ_r)
    {
        DEBUG_TEST(DB_I, "离线蓝牙模式 认证成功");
        m_callable_display_status(M_DISPLAY_CONNECTING_TO_SERVER_MODE, 0);
        // m_callable_network_specify_mode_data_tx(M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL, false, "blu_login_succ", 15);
    }
    else
    {
        DEBUG_TEST(DB_I, "离线蓝牙模式 认证失败");
        m_callable_display_status(M_DISPLAY_WIFI_CONNET_FAIL_MODE, 0);
        // m_callable_network_specify_mode_data_tx(M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL, false, "blu_login_fail", 15);
    }
    return succ_r;
}

/**
 * @brief  手机与设备 绑定
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_bind_set(void)
{
    bind_flag = true;
    return succ_r;
}

/**
 * @brief  登录 标志位获取
 * @return stat_m
 */
bool m_callable_offline_bluetooth_unbind_flag_get(void)
{
    return bind_flag;
}

/**
 * @brief  手机与设备  退出登录
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_unbind(void)
{
    auth_flag = false;
    DEBUG_TEST(DB_I, "退出登录");
    bind_flag = false;
    return succ_r;
}

/**
 * @brief   认证是否成功 标志位
 * @param void
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login_flag(void)
{
    if (auth_flag == true)
    {   
        return succ_r;
    }  
    return fail_r;
}

/**
 * @brief 离线蓝牙模式  设备向手机返回设备状态
 * @param void
 * @return device_status
 */
enum device_status m_callable_offline_bluetooth_get_device_status(void)
{
    int device_status = 0;
    m_callable_device_proper_status_get(&device_status);
    return device_status;
}

/**
 * @brief 在离线蓝牙模式下，判断是否处于登录状态
 * @param void
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_logged_or_not_logged(void)
{
    stat_m stat = succ_r;
    int nmod = 0;
    m_callable_device_attribute_get_running_mode(&nmod);
    if (nmod == M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT)
    {
        if (m_callable_offline_bluetooth_login_flag() == fail_r)
        {
            stat = fail_r;
        }
    }
    return stat;
}

/**
 * @brief   筛选出在离线蓝牙模式中，需要直接返回结果的指令
 * @param in_spec_cmd
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_instruction_filtering(uint64_t in_spec_cmd)
{
    stat_m stat = fail_r;
    if (in_spec_cmd == (M_CMD_NOTIFY_TO_SERVER_REQUEST_LOCAL_INFO + 1) ||
        M_CMD_TWOWAY_SET_ZONE ||
        M_CMD_NOTIFY_TO_SERVER_ZONE_CONFIG_ABC ||
        M_CMD_NOTIFY_TO_DEVICE_RUNNING_RESP_CURRENT_AND_FLOW ||
        M_CMD_TWOWAY_SET_SCHEDULE_ABLE ||
        M_CMD_NOTIFY_TO_DEVICE_CHECK_CURRENT ||
        M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK ||
        M_CMD_NOTIFY_TO_SERVER_ADD_SCH_RESP ||
        M_CMD_NOTIFY_TO_SET_FLOW_RATE ||
        M_CMD_TWOWAY_DELE_SCHEDULE_RESP ||
        M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE ||
        M_CMD_TWOWAY_FACTORY_RESET ||
        M_CMD_TWOWAY_SET_SENSOR ||
        M_CMD_TWOWAY_PUMP_MODE ||
        M_CMD_TWOWAY_WATER_HAMMER ||
        M_CMD_TWOWAY_HIBERNATION_MODE ||
        M_CMD_NOTIFY_TO_SERVER_OFFLINE_ADJUST ||
        M_CMD_TWOWAY_UNDO_NOT_RUN_A_SCHEDULE_TIMESPC ||
        M_CMD_TWOWAY_UNDO_NOT_RUN_SCHEDULE_TIMESPC ||
        M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC ||
        M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC ||
        M_CMD_NOTIFY_TO_DEVICE_SET_TIME_SENSOR_BAD_CHECK ||
        M_CMD_NOTIFY_TO_SERVER_A_SCHEDULE_BE_SKIP ||
        M_CMD_NOTIFY_TO_DEVICE_REQUEST_LOCAL_INFO ||
        M_CMD_NOTIFY_TO_DEVICE_RTC_POWER_CHECK ||
        M_CMD__NUMBER_OF_TRAFFIC_DETECTION_PULSES ||
        M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL)
    {
        stat = succ_r;
    }
    else if (in_spec_cmd == M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME)
    {
        stat = M_ERR;
    }
    return stat;
}