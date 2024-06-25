

#include "core/fset_lib.h"
#include "data_parse.h"

#define ANALYZE_HEAD '['
#define ANALYZE_ZJ ','
#define ANALYZE_TAIL ']'
#define ANALYZE_INVA ' '
#define MAX_LOOP_NUM 500

stat_m m_callable_data_parse_init(void)
{
    return 0;
}
/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉',' 提取下一个到 ',' 为止的String参数
 *
 * @param out_targat  提取到的目标将从这里输出
 * @param in_src  输入提取源
 * @return stat_m 提取失败或者成功
 */
stat_m m_callable_data_parse_foramt_handle_server_sub_str(char *out_targat, char *in_src, int *len)
{
    stat_m stat = fail_r;
    memset(out_targat, 0, sizeof(*out_targat));

    *len = 0;
    while (1)
    {
        if (*in_src == '\0')
        {
            stat = fail_r;
            break;
        }
        if (*in_src == ANALYZE_TAIL)
        {
            *out_targat = *in_src;
            *in_src = ANALYZE_INVA;
            *len += 1;
            stat = succ_r;
            break;
        }
        if (*in_src == '#')
        {
            if (*(in_src + 1) == '@')
                if (*(in_src + 2) == '#')
                {
                    *in_src = ANALYZE_INVA;
                    *(in_src + 1) = ANALYZE_INVA;
                    *(in_src + 2) = ANALYZE_INVA;
                    in_src += 3;
                    stat = succ_r;
                    break;
                }
        }
        // if (*in_src != ANALYZE_INVA)
        {
            *len += 1;
            *out_targat = *in_src;
            *in_src = ANALYZE_INVA;
            out_targat++;
        }
        in_src++;
    }
    return stat;
}
stat_m m_callable_data_parse_format_cmd(int *out_targat, char *in_src)
{
    stat_m stat = fail_r;
    *out_targat = ' ';
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, "%*[^0-9]%d", out_targat);
    // printf("%d   %s\n", *out_targat, in_src);
    if (*out_targat > 0)
        stat = succ_r;
    for (int i = 0; i < 5; i++)
    {
        if (*in_src != ANALYZE_ZJ)
        {
            *in_src = ANALYZE_INVA;
        }
        else
        {
            *in_src = ANALYZE_INVA;
            break;
        }
        in_src++;
    }
    return stat;
}
stat_m m_callable_data_parse_format_vue(char *format, void *out_targat, char *in_src)
{
    stat_m stat = fail_r;

    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, out_targat);
    for (int i = 0; i < MAX_LOOP_NUM && in_src != NULL; i++)
    {
        if (*in_src != ANALYZE_ZJ)
        {
            *in_src = ANALYZE_INVA;
        }
        else
        {
            *in_src = ANALYZE_INVA;
            break;
        }
        in_src++;
    }

    // printf(" m_callable_data_parse_foramt_value  %s\n", (char*)out_targat);

    return stat;
}
stat_m m_callable_data_parse_foramt_str(char *out_targat, char *in_src)
{
    stat_m stat = fail_r;
    // int len = strlen(in_src);
    int pac = 0;
    memset(out_targat, 0, strlen(out_targat));
    /** Use (M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(arr, "%*[ ]%[^,]", out_char_vue);) or */

    if (out_targat != NULL && in_src != NULL)
    {
        for (int i = 0; i < MAX_LOOP_NUM; i++)
        {
            // printf("%s\n", in_src);
            if ((*in_src == ANALYZE_HEAD || *in_src == ANALYZE_INVA) && stat != succ_r)
            {
                *in_src = ANALYZE_INVA;
            }
            else if (*in_src == ANALYZE_TAIL || *in_src == ANALYZE_ZJ)
            {
                *in_src = ANALYZE_INVA;
                *out_targat = '\0';
                break;
            }
            else
            {
                *out_targat = *in_src;
                stat = succ_r;
                *in_src = ANALYZE_INVA;
                out_targat++;
            }
            in_src++;
        }
    }
    return stat;
}

stat_m m_callable_data_parse_foramt_week_c(char *format, char span, char *pragma1, char *in_src)
{
    stat_m stat = fail_r;
    int len = strlen(in_src);
    int pac = 0;
    if (in_src != NULL && pragma1 != NULL)
    {
        for (int i = 0; i < MAX_LOOP_NUM; i++)
        {
            if (*in_src == span)
            {
                *in_src = ' ';
            }
            else if (*in_src == ',' || *in_src == '\0')
                break;
            else if (*in_src > 48 && *in_src < 58)
            {
                pragma1[pac] = ((*in_src));
                DEBUG_TEST(DB_I, "%d", pragma1[pac] - 48);
                pac++;
                stat = succ_r;
            }
            in_src++;
        }
    }

    return stat;
}
/**
 * 精简时间格式 解析及各类函数
 */

// static char ttmp[80] = {0};

stat_m m_callable_data_parse_foramt_param_pro(char *format, void *arg1, void *arg2, void *arg3, char *in_src)
{
    // int abb = 0;
    // for (int i = 0; i < MAX_LOOP_NUM; i++)
    // {
    //     if (*in_src != ANALYZE_ZJ)
    //     {
    //         // printf("%c\n",*in_src);
    //         ttmp[abb] = *in_src;
    //         *in_src = ANALYZE_INVA;
    //     }
    //     else
    //     {
    //         *in_src = ANALYZE_INVA;
    //         abb++;
    //         break;
    //     }
    //     in_src++;
    // }
    // if (strcmp(ttmp, "null") != 0)
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, arg1, arg2, arg3);

    return succ_r;
}
/**
 * @brief  按照指定个格式  一次性解析多个参数 4个
 *
 * @param format
 * @param arg1
 * @param arg2
 * @param arg3
 * @param arg4
 * @param in_src
 * @return stat_m
 */
stat_m m_callable_data_parse_foramt_param_pro_max(char *format, void *arg1, void *arg2, void *arg3, void *arg4, char *in_src)
{

    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, arg1, arg2, arg3, arg4);

    return succ_r;
}
/**
 * @brief 解析 第一到第三个参数
 *
 * @param format
 * @param arg1
 * @param arg2
 * @param in_src
 * @return stat_m
 */
stat_m m_callable_data_parse_foramt_quick_time(char *format, void *arg1, void *arg2, void *arg3, char *in_src)
{
    stat_m stat = fail_r;
    uint8_t id_temp_x = 0;
    // printf("%s \n" , in_src);
    if (strstr(in_src, "#") != NULL)
    {
        stat = succ_r;
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, arg1, arg2, arg3);
        // 破坏数据结构
        for (size_t i = 0; i < 500; i++)
        {
            if (*in_src != ',' && *in_src != '\0' && *in_src != ';')
            {
                *in_src = ' ';
                in_src++;
            }
            else if (*in_src == ';')
            {
                *in_src = ' ';
                break;
            }
            else if (*in_src == '\0')
                break;
            else if (*in_src == ',')
                break;
            else
            {
                *in_src = ' ';
                in_src++;
            }
        }
    }
    return stat;
}
stat_m m_callable_data_parse_foramt_param(char *format, void *arg1, void *arg2, char *in_src)
{
    // int abb = 0;
    // for (int i = 0; i < MAX_LOOP_NUM; i++)
    // {
    //     if (*in_src != ANALYZE_ZJ)
    //     {
    //         // printf("%c\n",*in_src);
    //         ttmp[abb] = *in_src;
    //         *in_src = ANALYZE_INVA;
    //     }
    //     else
    //     {
    //         *in_src = ANALYZE_INVA;
    //         abb++;
    //         break;
    //     }
    //     in_src++;
    // }
    // if (strcmp(ttmp, "null") != 0)
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, arg1, arg2);

    return succ_r;
}

/**
 * @brief 得到一个 JSON 相关的 键值
 *
 * @param in_key 键
 * @param out_resp 解析出的值
 * @param in_src 源 json
 * @return stat_m
 */
static char *info;
stat_m m_callable_json_data_parse_foramt_one_key(char *in_key, char *out_resp, char *in_src)
{
    stat_m stat = fail_r;
    const char *item = strstr(in_src, in_key);
    if (item == NULL)
        return 0;
    info = strchr(item, ':');
    // M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(item, "%*[^:]%s", info);
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(info, ":\"%[^\"]", out_resp);

    DEBUG_TEST(DB_W, "%s:%s", in_key, out_resp);
    return stat;
}

// void m_callable_json_replace_backslash_with_quote(char *str)
// {
//     int i;
//     int length = strlen(str);
//     for (i = 0; i < length; i++)
//     {
//         if (str[i] == '\\' && str[i + 1] == '\0')
//         {
//             str[i] = '"';
//         }
//     }
// }

stat_m m_callable_json_extract_wifiname(const char *json, char *wifiName)
{
    stat_m stat = fail_r;
    const char *ssidKey = "\"ssid\":\"";
    const char *quote = ",";

    const char *start = strstr(json, ssidKey);
    if (start == NULL)
    {
        return stat; // ssid键未找到
    }
    start += strlen(ssidKey);

    const char *end = strstr(start, quote);
    if (end == NULL)
    {
        return stat; // 双引号未找到
    }

    int wifiNameLength = end - start - 1;
    strncpy(wifiName, start, wifiNameLength);
    wifiName[wifiNameLength] = '\0'; // 添加字符串结束符

    return succ_r; // 双引号未找到
}

stat_m m_callable_json_extractpassword(const char *json, char *password)
{
    const char *passwordKey = "\"password\":\"";
    const char *quote = "\"";

    const char *start = strstr(json, passwordKey);
    if (start == NULL)
    {
        return fail_r; // password键未找到
    }
    start += strlen(passwordKey);

    const char *end = strstr(start, quote);
    if (end == NULL)
    {
        return fail_r; // 双引号未找到
    }

    int passwordLength = end - start;
    strncpy(password, start, passwordLength);
    password[passwordLength] = '\0'; // 添加字符串结束符
    return succ_r;
}

stat_m m_callable_json_extract_type(const char *json, char *type)
{
    const char *typeKey = "\"type\":\"";
    const char *quote = "\"";

    const char *start = strstr(json, typeKey);
    if (start == NULL)
    {
        return fail_r; // type键未找到
    }
    start += strlen(typeKey);

    const char *end = strstr(start, quote);
    if (end == NULL)
    {
        return fail_r; // 双引号未找到
    }

    int typeLength = end - start;
    strncpy(type, start, typeLength);
    type[typeLength] = '\0'; // 添加字符串结束符
    return succ_r;
}

stat_m m_callable_json_config_net_set_par(char *str, int len)
{
    stat_m stat = fail_r;
    // printf("str:%s\r\n", str);
    // stat = m_callable_json_extract_wifiname(str, mwifi.ssid);
    // stat = m_callable_json_extractpassword(str, mwifi.psw);
    // stat = m_callable_json_extract_type(str, connect_type);

    return stat;
    // printf("wifi_name: %s\n", mwifi.ssid);
    // printf("password: %s\n", mwifi.psw);
    // printf("type: %d\n", connect_type[0]);
}

/**
 * @brief 获取自上次到现在的流量值，并且指定是否清除缓存并且重新开始计算，如果流量传感器未打开 返回0
 *
 * @param is_clear 是否清除缓存
 * @return stat_m
 */
float m_static_divideInt_by_uint32(int dividend, uint32_t divisor)
{
    return (float)dividend / (float)divisor;
}

/**
 * @brief 获取自上次到现在的流量值，并且指定是否清除缓存并且重新开始计算，如果流量传感器未打开 返回0
 *
 * @param is_clear 是否清除缓存
 * @return stat_m
 */
float m_static_divideInt_by_uint16(float dividend, uint16_t divisor)
{
    return dividend / (float)divisor;
}

/**
 * @brief 获取 带有三位小数的 值
 *
 * @param value 需要转换的值
 * @return float
 */
float m_static_round_threedecimal(float value)
{
    return ((int)(value * 1000 + 0.5)) / 1000.0;
}

#define MAX_LEN 200

void m_callable_generatestring(int area[], uint64_t time[], int num_areas, char output[])
{
    char temp[MAX_LEN];
    output[0] = '\0';
    for (int i = 1; i <= num_areas; i++)
    {
        snprintf(temp, MAX_LEN, "%d#%lld", area[i], time[m_run_time.channel_number[i]]);
        strcat(output, temp);
        if (i <= num_areas - 1)
        {
            strcat(output, "+"); // 非最后一个区域，用+号分隔
        }
    }
}