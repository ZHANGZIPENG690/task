

/**
 * Establish a table of start times for new lawn patterns 2022.1.6
*/


#include "core/fset_lib.h"
#include "core/schedule/mschedule.h"


struct NL_TimeList {
    // 
    uint64_t nl_id;
    uint32_t nl_time[MAX_TIME_NUM];
};

struct NL_TimeList nl_root[MAX_SCHEDU_NUM];
/**
 * Add a new lawn pattern to a start schedule
*/
stat_m m_nllist_add_schedule_time(uint64_t in_id , uint32_t *in_time , int in_len)
{
    stat_m stat = fail_r;
    int i = 0 , j = 0 ,flag =0;
    for ( i = 0; i < MAX_SCHEDU_NUM; i++)
    {
        if( in_id == nl_root[i].nl_id || nl_root[i].nl_id == 0)
        {
            flag = i;
            stat = succ_r;
            break;
        }        
    }
    if(flag == 0)
           flag = i;

    memset(nl_root[flag].nl_time  , 0 , sizeof(nl_root[flag].nl_time));
    nl_root[flag].nl_id = in_id;
    for ( j = 0; j < in_len; j++)
    {
        nl_root[flag].nl_time[j] =  in_time[j];
        // DEBUG_TEST(DB_I,"nl times : %d " , nl_root[flag].nl_time[j]);
    }
    return stat;
}
/**
 * Remove the new lawn mode from a start schedule
*/
stat_m m_nllist_rm_schedule_time(uint64_t in_id )
{
    stat_m stat = fail_r;
    int i = 0;
    for ( i = 0; i < MAX_SCHEDU_NUM; i++)
    {
        if( in_id == nl_root[i].nl_id)
          {
            memset(&nl_root[i] , 0 , sizeof(nl_root[i]));
            nl_root[i].nl_id = 0;
          }
    }
    return stat;
}

uint64_t m_nllist_srech_pretime(uint64_t in_id , uint64_t pre_time)
{
    int i = 0 , j = 0 , result = 0;

    for ( i = 0; i < MAX_SCHEDU_NUM; i++)
    {
        if( in_id == nl_root[i].nl_id)
        {
            for ( j = 0; j < MAX_TIME_NUM; j++)
            {
                if( nl_root[i].nl_time[j] <  pre_time)
                {
                    result = nl_root[i].nl_time[j];
                    break;
                }else if(nl_root[i].nl_time[j] == 0 )
                    break;
            }
        }
    }
    if(result == 0 )
        DEBUG_E("Err ! util/new_lawn_mode -- m_nllist_srech_nexttime");
    return result;
}

/**
 * Find a recent time based on the current time ( Next Time )
*/
uint64_t m_nllist_srech_nexttime(uint64_t in_id , uint64_t pre_time)
{
    int i = 0 , j = 0 ;
    uint64_t result = 0 ,tpid = 0;

    for ( i = 0; i < MAX_SCHEDU_NUM; i++)
    {
        if( in_id == nl_root[i].nl_id)
        {
            tpid = i;
            DEBUG_TEST(DB_I,"search : %lld ..(%lld) " ,nl_root[i].nl_id , pre_time);
            for ( j = 0; j < MAX_TIME_NUM; j++)
            {
                if( nl_root[i].nl_time[j] >  pre_time)
                {
                    DEBUG_TEST(DB_I,"%d .. " ,nl_root[i].nl_time[j]);
                    result = nl_root[i].nl_time[j];
                    break;
                }else if(nl_root[i].nl_time[j] == 0 && j > 0)
                    break;
            }
        }
    }
    if(result == 0 )
    {
            // for ( j = 0; j < 20; j++)
            // {
            //     DEBUG_TEST(DB_I,"(%d) times : %d " ,i, nl_root[tpid].nl_time[j]);
            // }
            result = nl_root[tpid].nl_time[0];
            DEBUG_TEST(DB_I,"Err!(%lld)  util/new_lawn_mode -- m_nllist_srech_nexttime" ,  result);
    }
    return result;
}