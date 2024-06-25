
#include "core/fset_lib.h"
#include "new_lawn.h"

#define CONST_INVATE_NEW_LAWN '-'
#define CONST_STR "%d-"

// Calculate and match the parameters of the plan - the new lawn pattern

// static const NEWLAWN_HOUR[] = {"ABCDEFGHIJKLMNOPQRSTUVWX"};

int m_decode(char conchr, bool tm)
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

void m_static_new_lawn_parse_parameters(uint64_t in_id, char *consStr)
{
    int i = 0;
    uint32_t result[MAX_TIME_NUM] = {0};
    // char tpc = 0;
    // printf("%s\n" ,consStr);
    // 1、 还是先 找出所有的时间

    for (int j = 0; j < MAX_TIME_NUM; j++) // 可以不破坏内容
    {
        if ((*consStr >= '0' && *consStr <= '9') || (*consStr >= 'A' && *consStr <= 'Z') || (*consStr >= 'a' && *consStr <= 'z'))
        {
            // DEBUG_TEST(DB_I,"%s", consStr);
            // 得到小时（前两位） &&  得到分钟（后两位）
            // 小时和分钟的计数方式有差异 0-24小时全部用"A - M"表示 ， 分钟用 0-9 A-Z a-z 循环表示 0-60
            result[i] = m_decode(*consStr, true) * 3600;
            consStr++;
            result[i] += m_decode(*consStr, false) * 60;
            // DEBUG_TEST(DB_I,"m_static_new_lawn_parse_parameters (%lld): %d" , in_id , result[i]);
            i++;
        }
        consStr++;
        if (*consStr == '\0' || *consStr == ']' || *consStr == ',')
            break;
    }
    // 1.5 加入到链表
    // 2、然后更新下开始时间表
    m_static_new_lawn_time_list_add_time(in_id, result, i);
}
