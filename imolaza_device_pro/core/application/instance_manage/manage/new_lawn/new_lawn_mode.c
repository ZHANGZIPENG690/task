/**
 * Establish a table of start times for new lawn patterns 2022.1.6
 */

#include "core/fset_lib.h"
#include "new_lawn.h"

struct NL_TimeList
{
    //
    uint64_t nl_id;
    uint32_t nl_time[MAX_TIME_NUM];
};

struct NL_TimeList nl_root[MAX_SCHEDU_NUM] = {0};
/**
 * Add a new lawn pattern to a start schedule
 */
stat_m m_static_new_lawn_time_list_add_time(uint64_t in_id, uint32_t *in_time, int in_len)
{
    stat_m stat = fail_r;
    int i = 0, j = 0, flag = -1;
    for (i = 0; i < MAX_SCHEDU_NUM; i++)
    {
        if (in_id == nl_root[i].nl_id || nl_root[i].nl_id == 0)
        {
            flag = i;
            stat = succ_r;
            break;
        }
    }
    if (flag == -1)
        flag = i;

    memset(nl_root[flag].nl_time, 0, sizeof(nl_root[flag].nl_time));
    nl_root[flag].nl_id = in_id;
    for (j = 0; j < in_len; j++)
    {
        nl_root[flag].nl_time[j] = in_time[j];
        // DEBUG_TEST(DB_I,"nl times : %d " , nl_root[flag].nl_time[j]);/
        DEBUG_TEST(DB_I,"nl times(%lld -  %d) %d .. ", nl_root[i].nl_id, j, nl_root[i].nl_time[j]);
    }
    return stat;
}
/**
 * Remove the new lawn mode from a start schedule
 */
stat_m m_static_new_lawn_time_list_rm_one_time(uint64_t in_id)
{
    stat_m stat = fail_r;
    int i = 0;
    for (i = 0; i < MAX_SCHEDU_NUM; i++)
    {
        if (in_id == nl_root[i].nl_id)
        {
            memset(&nl_root[i], 0, sizeof(nl_root[i]));
            nl_root[i].nl_id = 0;
        }
    }
    return stat;
}

uint64_t m_static_new_lawn_srech_pretime(uint64_t in_id, uint64_t pre_time)
{
    int i = 0, x = 0;
    uint64_t result = 0;

    for (i = 0; i < MAX_SCHEDU_NUM; i++)
    {
        if (in_id == nl_root[i].nl_id)
        {
            x = 0;
            DEBUG_TEST(DB_I,"search pre : %lld ..(%lld) ", nl_root[i].nl_id, pre_time);
            for (x = 0; x < MAX_TIME_NUM; x++)
            {
                // DEBUG_TEST(DB_I,"%d - > %d" , nl_root[i].nl_time[x] , x);
                if (nl_root[i].nl_time[x] < pre_time)
                {
                    // 说明找到了顶点
                    if (nl_root[i].nl_time[x] == 0 && x > 0)
                    {
                        break;
                    }
                    result = nl_root[i].nl_time[x];
                    DEBUG_TEST(DB_I,"(%lld -  %d)pre %d .. ", nl_root[i].nl_id, x, nl_root[i].nl_time[x]);
                    // if( nl_root[i].nl_time[j+1] != 0 && nl_root[i].nl_time[j+1] < pre_time)
                    //     continue;
                }
            }
            break;
        }
    }
    if (result == 0)
    {
        result = nl_root[i].nl_time[0];
        DEBUG_E("Err ! util/new_lawn_mode -- m_nllist_srech_pre time (%lld %d)", nl_root[i].nl_id, nl_root[i].nl_time[0]);
    }
    return result;
}

/**
 * @brief 返回附近的两个时间
 *
 * @param in_id 计划id
 * @param out_pre_time 返回的上一次最近的时间
 * @param out_next_time 返回的下一次最近的时间
 * @param pre_time 当前的时间
 * @return uint64_t
 */
stat_m m_static_new_lawn_srech_near_time(uint64_t in_id, uint64_t *out_pre_time, uint64_t *out_next_time, uint64_t pre_time)
{
    stat_m stat = fail_r;
    int i = 0, j = 0;
    int tpid = 0;

    *out_next_time = 0;
    for (i = 0; i < MAX_SCHEDU_NUM; i++)
    {
        if (in_id == nl_root[i].nl_id)
        {
            tpid = i;
            DEBUG_TEST(DB_I,"search : %lld ..(%lld) ", nl_root[i].nl_id, pre_time);
            for (j = 0; j < MAX_TIME_NUM; j++)
            {
                DEBUG_TEST(DB_I,"next %d .. ", nl_root[i].nl_time[j]);
                if (nl_root[i].nl_time[j] > pre_time && out_next_time != NULL)
                {
                    *out_next_time = nl_root[i].nl_time[j];
                    break;
                }
                else if (nl_root[i].nl_time[j] == 0 && j > 0)
                    break;
                else if (out_pre_time != NULL)
                {
                    *out_pre_time = nl_root[i].nl_time[j];
                    stat = succ_r;
                }
            }
            break;
        }
    }
    if (nl_root[tpid].nl_time[0] != 0 && out_pre_time != NULL && *out_pre_time == 0)
    {
        *out_pre_time = nl_root[tpid].nl_time[0];
        stat = succ_r;
    }
    if (out_next_time != NULL && *out_next_time == 0)
    {
        *out_next_time = nl_root[tpid].nl_time[0];
        DEBUG_TEST(DB_I,"Err!(%lld)  util/new_lawn_mode -- m_nllist_srech_nexttime", *out_next_time);
    }
    return stat;
}