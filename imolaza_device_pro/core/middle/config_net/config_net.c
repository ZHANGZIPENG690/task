
#include "config_net.h"
#include "core/fset_lib.h"

static char user_name[32] = {0};
static char token[32] = {0};

#define INVGE "@_!B@"


void m_static_config_net_str_transf_str(char *strc)
{
    char trsns[80] = {0};
    int len = strlen(strc);
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if (strc[i] == '@' && strstr(strc, INVGE) != NULL)
        {
            i += 4;
            trsns[j] = ',';
        }
        else
        {
            trsns[j] = strc[i];
        }
        j++;
    }
    strcpy(strc, trsns);
}
/**
 * @brief
 *
 * @param strc
 */
void m_static_config_net_str_transf_str_resp(char *strc)
{
    char trsns[80] = {0};
    int len = strlen(strc);
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if (strc[i] == ',')
        {
            trsns[j++] = '@';
            trsns[j++] = '_';
            trsns[j++] = '!';
            trsns[j++] = 'B';
            trsns[j++] = '@';
        }
        else
        {
            trsns[j] = strc[i];
            j++;
        }
    }
    strcpy(strc, trsns);
}
/**
 * @brief 吧指定字符替换
 *
 * @param strc
 * @return stat_m
 */
stat_m m_callable_str_transf_str_resp(char *strc)
{
    m_static_config_net_str_transf_str_resp(strc);
    return succ_r;
}
/**
 * @brief 配置设备的入网 username
 *
 * @param in_psw
 * @param len
 * @return stat_m
 */
stat_m m_callable_config_net_set_username(char *in_username)
{
    stat_m st = fail_r;
    char *rest = NULL;
    if (in_username != NULL)
    {
        if (strstr(in_username, INVGE) != NULL)
            m_static_config_net_str_transf_str(in_username);
        strcpy(user_name, in_username);
    }
    return st;
}
/**
 * @brief 配置设备的token（密码）
 *
 * @param in_psw 密码
 * @param len 长度
 * @return stat_m
 */
stat_m m_callable_config_net_set_token(char *in_token)
{
    stat_m st = fail_r;
    char *rest = NULL;
    if (in_token != NULL)
    {
        if (strstr(in_token, INVGE) != NULL)
            m_static_config_net_str_transf_str(in_token);
        strcpy(token, in_token);
    }
    return st;
}

stat_m m_callable_config_net_recode(void)
{
    return m_ext_software_drive_net_wifi_sta_connect_to(user_name, token);
}
stat_m m_callable_config_net_start(void)
{
    // Operation of the keys is not allowed during testing
    // shield_leftRight_key = true;
    stat_m stat = fail_r;
    // _wifi.wifi_need_auto_connect = false;
    m_callable_network_set_auto_connect(false);
    m_ext_soft_drive_net_wifi_dis_connect();
    m_callable_network_set_connect_status(false);
    // wifi_sta_init(in_ssid, in_psw);
    // DEBUG_TEST(DB_I,"%s", in_ssid);
    // DEBUG_TEST(DB_I,"%s", in_psw);

    for (size_t i = 0; i < 3; i++)
    {
        DEBUG_TEST(DB_I, "wifi_sta_scan_spec");
        stat = m_ext_soft_drive_net_wifi_sta_scan_spec(user_name);
        mDelay_ms(150);
        if (stat == succ_r)
            break;
    }
    if (stat != succ_r)
    {
        stat = M_CONFIG_MODE_RESP_NOT_FOUND_TARGET_WIFI;
        m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_CONFIG_WIFI_NOT_FIND);
    }
    else
    {
        stat = M_CONFIG_MODE_RESP_NOT_FOUND_TARGET_WIFI;

        for (size_t i = 0; i < 3; i++)
        {
            DEBUG_TEST(DB_I, "Config try wifi_sta_connect_to --> %s (%s)", user_name, token);
            m_ext_software_drive_net_wifi_sta_connect_to(user_name, token);
            for (size_t i = 0; i < 100; i++)
            {
                if (m_callable_network_get_net_connect_status() == succ_r)
                {
                    DEBUG_TEST(DB_W, "Config  success");

                    // m_callable_device_attribute_running_mode_set_wifi_info(user_name, token);

                    stat = succ_r;
                    break;
                }
                else
                {
                    stat = M_CONFIG_MODE_RESP_PASSWORD_ERR;
                }
                mDelay_ms(60);
            }
            if (stat == succ_r)
                break;
        }
        if (stat == succ_r)
        {
            m_callable_middle_connect_handle_set_code(M_CONNECT__CODE__WIFI_IP_WAIT_ASSIGNMENT);
        }
        else

            if (stat == (stat_m)M_CONFIG_MODE_RESP_PASSWORD_ERR)
        {
            DEBUG_TEST(DB_E, "WIFI password err !");
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_CONFIG_FAIL_PASSWORD_ERR);
        }
        else
        {
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_CONFIG_FAIL_TIMEOUT);
        }
    }

    return stat;
}
stat_m m_callable_config_net_save(void)
{
    DEBUG_TEST(DB_W, "m_callable_config_net_save ..WW");
    m_callable_device_attribute_running_mode_set_wifi_info(user_name, token);
    m_callable_network_set_net_info(user_name, token);
    return 0;
}

char *M_CONFIG_NET_SUCCESS = "wifi_suc";
char *M_CONFIG_NET_FAIL = "time_out";
/**
 * @brief 配置网络成功或者失败的反馈
 *
 * @param stat
 * @return stat_m
 */
stat_m m_callable_config_net_resp(stat_m stat)
{
    if (stat == succ_r)
        stat = m_callable_network_specify_mode_data_tx(M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL, false, M_CONFIG_NET_SUCCESS, strlen(M_CONFIG_NET_SUCCESS));
    else
        stat = m_callable_network_specify_mode_data_tx(M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL, false, M_CONFIG_NET_FAIL, strlen(M_CONFIG_NET_SUCCESS));
    return stat;
}
