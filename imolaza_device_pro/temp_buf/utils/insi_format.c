

#include "core/fset_lib.h"
#include "util.h"

#define ANALYZE_HEAD '['
#define ANALYZE_ZJ ','
#define ANALYZE_TAIL ']'
#define ANALYZE_INVA ' '
#define MAX_LOOP_NUM 300

stat_m m_insi_foramt_handle_server_sub_str(char *out_targat, char *in_src, int *len)
{
    stat_m stat = fail_r;
    memset(out_targat, 0, 100);

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
stat_m m_insi_foramt_int_cmd(int *out_targat, char *in_src)
{
    // if (*in_src == ANALYZE_HEAD)
    //     in_src = ANALYZE_INVA,
    //     in_src++;
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, "[%d", out_targat);
    printf("%d   %s", *out_targat, in_src);
    for (int i = 0; i < 10; i++)
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
    return succ_r;
}
stat_m m_insi_foramt_value(char *format, void *out_targat, char *in_src)
{
    stat_m stat = fail_r;

    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, format, out_targat);
    for (int i = 0; i < MAX_LOOP_NUM; i++)
    {
        if (*in_src != ANALYZE_ZJ && *in_src != '\0')
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
    // printf(" m_insi_foramt_value  %d\n", *(uint64_t*)out_targat);

    return stat;
}
stat_m m_insi_foramt_str(char *out_targat, char *in_src)
{
    stat_m stat = fail_r;
    // int len = strlen(in_src);
    int pac = 0;
    memset(out_targat, 0, strlen(out_targat));
    /** Use (M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(arr, "%*[ ]%[^,]", out_char_vue);) or */
    // printf("%s\n\t\t\t\t", in_src);
    if (out_targat != NULL && in_src != NULL)
    {
        for (int i = 0; i < MAX_LOOP_NUM; i++)
        {
            if ((*in_src == ANALYZE_HEAD || *in_src == ANALYZE_INVA) && stat != succ_r)
            {
                *in_src = ANALYZE_INVA;
            }
            else if (*in_src == ANALYZE_TAIL || *in_src == ANALYZE_ZJ)
            {
                *in_src = ANALYZE_INVA;
                break;
            }
            else
            {
                *out_targat = *in_src;
                // printf("--> %s\n" , out_targat);
                out_targat++;
                stat = succ_r;
                *in_src = ANALYZE_INVA;
            }
            in_src++;
        }
    }
    // printf(" m_insi_foramt_str  %s\n", out_targat);

    return stat;
}

stat_m m_insi_foramt_week_c(char *format, char span, void *pragma1, char *in_src)
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
            }
            else
            {
                // pragma1[pac] = ((*in_src) - 48);
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




stat_m m_insi_foramt_param(char *format, void *arg1, void *arg2, char *in_src)
{
    // int abb = 0;
    // for (int i = 0; i < MAX_LOOP_NUM; i++)
    // {
    //     if (*in_src != ANALYZE_ZJ)
    //     {
            // printf("%c\n",*in_src);
            // printf("%s\n",format);
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
