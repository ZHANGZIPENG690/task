

#include "core/utils/util.h"



static const char start[2] = "{";
// static const char end[2] = "}";

static const char interval[2] = ",";

static const char *new_start = "[";
static const char *new_end = "]";

char json_temp[100] = {0};

stat_m m_mjson_declare_table_finish(char *out_buf, uint64_t in_timesp)
{
    stat_m stat = fail_r;
    if ((out_buf) != NULL)
    {
        memset(json_temp, 0, sizeof(json_temp));
        // printf("%s\n", json_buf);
        // if (strstr(out_buf, "]") == NULL)
        {
            stat = succ_r;
            if (in_timesp > 0)
                m_mjson_append_new_element(out_buf, U64, &in_timesp);

            strcat(out_buf, new_end);
            // DEBUG_TEST(DB_I,"resp  %s\n", out_buf);
        }
        // printf("**");
    }
    return stat;
}

// static char *info;

// char *JSON_analyze_ones_vue(char *re_vue, const char *key1, char *arr)
// {
//     const char *item = strstr(arr, key1);
//     if (item == NULL)
//         return "ERR";
//     info = strchr(item, ':');
//     // M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(item, "%*[^:]%s", info);
//     M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(info, ":\"%[^\"]", re_vue);
//     if (true)
//     {
//         DEBUG_TEST(DB_W,"%s:%s", key1, re_vue);
//     }
//     return re_vue;
// }
int result_len = 0;
int new_json_analyze_u64(uint64_t *out_vue, char *arr)
{
    if (arr == NULL)
        return -1;
    if (strstr(arr, "]") == NULL)
        return -1;
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(arr, "%lld", out_vue);
    result_len = strcspn(arr, ",");
    for (uint8_t i = 0; i <= result_len; i++)
        arr[i] = ' ';

    return *out_vue;
}
int new_json_analyze_int(int *out_vue, char *arr)
{
    if (arr == NULL)
        return -1;
    if (strstr(arr, "]") == NULL)
        return -1;
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(arr, "%d", out_vue);
    // printf("%d",*out_vue);
    result_len = strcspn(arr, ",");
    for (uint8_t i = 0; i <= result_len; i++)
        arr[i] = ' ';

    return *out_vue;
}
// static char tempzone[30] = {0};
char *new_json_analyze_str(char *out_char_vue, char *arr)
{
    bzero(out_char_vue, sizeof(*out_char_vue));
    if (arr == NULL)
        return NULL;
    if (strstr(arr, "]") == NULL)
        return NULL;
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(arr, "%*[ ]%[^,]", out_char_vue);
    result_len = strcspn(arr, ",");
    for (uint8_t i = 0; i <= result_len; i++)
        arr[i] = ' ';
    if (strstr(out_char_vue, "]") != NULL)
        out_char_vue[strcspn(out_char_vue, "]")] = '\0';
    return out_char_vue;
}

int new_json_analyze_cmd(char *arr)
{
    int result = 0;
    // if (strstr(arr, "p1") != NULL)
    // {
    //     sock_timeout = 0;
    //     return -1;
    // }
    if (strstr(arr, "[") == NULL && strstr(arr, "]") == NULL)
        return -1;
    arr[0] = ' ';
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(arr, "%d", &result);
    result_len = strcspn(arr, ",");
    for (uint8_t i = 0; i <= result_len; i++)
        arr[i] = ' ';
    return result;
}
// char *json_analyze_cmd(char *arr)
// {
//     return JSON_analyze_ones_vue(te, "cmd", arr);
// }

stat_m m_mjson_analyze_table(char *out_buf)
{
    stat_m stat = fail_r;
    if (out_buf != NULL)
    {
        memset(out_buf, 0, sizeof(*out_buf));

        strcat(out_buf, start);
        stat = succ_r;
    }
    return stat;
}
stat_m m_mjson_create_new_table(char *out_buf)
{
    stat_m stat = fail_r;
    if (out_buf != NULL)
    {
        memset(out_buf, 0, strlen(out_buf));
        printf("%s" ,out_buf );
        strcat(out_buf, new_start);
        stat = succ_r;
    }
    return stat;
}
stat_m m_mjson_done_a_table(char *out_buf)
{
    stat_m stat = fail_r;
    if (out_buf != NULL)
    {
        strcat(out_buf, new_end);
        stat = succ_r;
    }
    return stat;
}

stat_m m_mjson_prepare_cmd(char *out_buf, int cmd)
{
    stat_m stat = fail_r;
    if (out_buf != NULL && cmd > 0)
    {
        memset(json_temp, 0, sizeof(json_temp));
        sprintf(json_temp, "%d", cmd); // [
        strcat(out_buf, json_temp);
        // sprintf(json_temp, "%d", cmd); // cmd
        /**  不应该放在这*/
        // if (cmd == 11 || cmd == 10 || cmd == 15 || cmd == 67)   SN
        // strcat(out_buf, json_temp);
        stat = succ_r;
    }
    return stat;
}

void m_addU8(char *out_buf, uint8_t a)
{
    sprintf(out_buf, "%hhd", a);
}
void m_addU16(char *out_buf, uint16_t a)
{
    sprintf(out_buf, "%hd", a);
}
void m_addU32(char *out_buf, uint32_t a)
{
    sprintf(out_buf, "%d", a);
}
void m_addU64(char *out_buf, uint64_t a)
{
    sprintf(out_buf, "%lld", a);
}
void m_addStr(char *out_buf, char *a)
{
    sprintf(out_buf, "%s", a);
}

stat_m m_mjson_append_new_element(char *out_buf, int in_type, void *in_vue)
{
    stat_m stat = fail_r;
    if (out_buf != NULL && in_vue != NULL)
    {
        if (in_type != null)
            strcat(out_buf, interval);
        memset(json_temp, 0, sizeof(json_temp));
        switch (in_type)
        {
        case U8:
            sprintf(json_temp, "%hhd", *((uint8_t *)in_vue));
            // m_addU8(json_temp, *((uint8_t *)(in_vue)));
            break;
        case U16:
            // m_addU16(json_temp, *(uint16_t *)in_vue);
            sprintf(json_temp, "%hd", *((uint16_t *)in_vue));
            break;
        case U32:
            // m_addU32(json_temp, *(uint32_t *)in_vue);
            sprintf(json_temp, "%d", *((uint32_t *)in_vue));
            break;
        case U64:
            // m_addU64(json_temp, *(uint64_t *)in_vue);
            sprintf(json_temp, "%lld", *((uint64_t *)in_vue));
            // printf("%lld -- %s" , *((uint64_t *)in_vue) , json_temp);

            break;
        case 55/**Int*/:
            // sprintf(json_temp, "%d", *((int *)in_vue));
            m_addU32(json_temp, *((int *)in_vue));
            break;
        case Str:
            m_addStr(json_temp, in_vue);
            // sprintf(json_temp, "%s", ((char *)in_vue));
            break;

        default:
            /** ... */
            break;
        }
        strcat(out_buf, json_temp);
        stat = succ_r;
    }
    return stat;
}
