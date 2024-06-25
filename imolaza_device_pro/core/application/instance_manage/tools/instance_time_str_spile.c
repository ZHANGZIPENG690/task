


#include "instance_algorithm.h"
#include "../instance.h"





/** 测试工具 */
int m_decodem(char conchr, bool tm)
{
    int flag_vue = 0;
    if (tm)
    {
        flag_vue = (((conchr) - 'A'));
    }
    else
    {
        if (conchr >= 'a')
            flag_vue = ((conchr) - 'a') + 36;
        else if (conchr >= 'A')
            flag_vue = ((conchr) - 'A') + 10;
        else if (conchr >= '0')
            flag_vue = ((conchr) - '0');
        else
            flag_vue = 0;
    }
    return flag_vue;
}

/**
 * @brief 输入待解析的字符串,输出解析完成的时间数组
 * 
 * @param in_str 源字符串
 * @param out_array 数组
 * @return stat_m 
 */
stat_m m_callable_install_tools_start_time_encode_into_array(char *in_str , uint32_t *out_array)
{
    stat_m stat = fail_r;
    int i = 0;
    // 1、 还是先 找出所有的时间

    for (int j = 0; j < 32; j++) // 可以不破坏内容
    {
        if ((*in_str >= '0' && *in_str <= '9') || (*in_str >= 'A' && *in_str <= 'Z') || (*in_str >= 'a' && *in_str <= 'z'))
        {
            // DEBUG_TEST(DB_I,"%s", consStr);
            // 得到小时（前两位） &&  得到分钟（后两位）
            // 小时和分钟的计数方式有差异 0-24小时全部用"A - M"表示 ， 分钟用 0-9 A-Z a-z 循环表示 0-60
            out_array[i] = m_decodem(*in_str, true) * 3600;
            in_str++;
            out_array[i] += m_decodem(*in_str, false) * 60;
            // DEBUG_TEST(DB_I,"m_static_new_lawn_parse_parameters (%lld): %d" , in_id , result[i]);
            i++;
            stat = succ_r;
        }
        in_str++;
        if (*in_str == '\0' || *in_str == ']' || *in_str == ',')
            break;
    }
    return stat;
}


















