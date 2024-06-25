#include "new_lwan_mode.h"






static    char temp_han[80] = {0};
    char temp_vue[80] = {0};

/**
 *  @brief Load the time list by schedule id
*/
void m_newlwan_mode_schedule_load(uint64_t in_id )
{

    // 1、montage handler string ... 
    sprintf(temp_han,"%s%lld" ,M_SCHEDULE_MODE_NEWLWAN_FLAG, in_id);

    // 2、form flash  loading ...
    m_ext_flash_read(Str ,temp_han , temp_vue , NULL);

    // 3、Analyze and append
    m_newlwan_mode_schedule_update_time_list(in_id , temp_vue , false);

}



/**
 *  @brief 1、Parse the time parameters one by one
 *         2、Join to the run list
*/
stat_m m_newlwan_mode_schedule_update_time_list(uint64_t in_id , char * consStr, bool update_flash)
{
    stat_m stat = fail_r;
    int i = 0 , b=0;
    uint32_t result[MAX_TIME_NUM] = {0};
    DEBUG_TEST(DB_I,"m_schedule_newlwan_mode_update (%lld,%s,%d)"  , in_id , consStr , update_flash);
    if(update_flash)
    {
        // 1、montage handler string ... 
        sprintf(temp_han,"%s%lld" ,M_SCHEDULE_MODE_NEWLWAN_FLAG, in_id);

        // 2、form flash  loading ...
        m_ext_flash_write(Str , temp_han , consStr , NULL);
    }

    //1、 还是先 找出所有的时间
    for (int j = 0; j < MAX_TIME_NUM; j++)     // 可以不破坏内容
    {
        if(*consStr >='0' && *consStr <= 'z')
        {    
            // 得到小时（前两位） &&  得到分钟（后两位）
            // 小时和分钟的计数方式有差异 0-24小时全部用"A - M"表示 ， 分钟用 0-9 A-Z a-z 循环表示 0-60
            result[i] = M_INSI_FORMAT_TIME_DECODE(*consStr ,true) ;
            result[i] *= 3600;
            consStr ++;
            b = M_INSI_FORMAT_TIME_DECODE(*consStr ,false);
            result[i] += b * 60;
            DEBUG_TEST(DB_I,"m_schedule_newlwan_mode_update : %d" , result[i]);
            i ++;   
        }
        consStr ++;
        if(*consStr == '\0' || *consStr == ']')
            break;
    }
    // 1.5 加入到链表
    //2、然后更新下开始时间表 
    m_nllist_add_schedule_time(in_id ,result, i);
    return stat;
}

stat_m m_newlwan_mode_schedule_flush(uint64_t in_id , uint64_t in_present_time ,uint64_t *out_result , bool recover )
{
    stat_m stat = fail_r;
    // 恢复这个计划 需要开始一个时间 
    uint64_t wctime = 0;
    DEBUG_TEST(DB_I,"m_schedule_newlwan_mode_flush");
    // 1、找到临近的时间，也就是最新开始的时间（在列表中）
    if(recover)
        wctime =  m_nllist_srech_pretime(in_id , in_present_time);
    else 
        wctime =  m_nllist_srech_nexttime(in_id  , in_present_time);
    DEBUG_TEST(DB_W,"%lld" , wctime);
    // 2、判断需不需要恢复(断电)
    // if(recover) // 
    {
        *out_result = wctime; 

    // 3、设置一些参数直接加时间节点
    // }else 
    // {
        // *out_result = wctime; 
        // 如果不是恢复的计划那就是启用的计划 
        // 我已经有了最新执行的计划时间，关掉启用的标记，添加运行时间点
        // new_add_time_node(wobj->schedule_id  ,wctime ,PLAN_RUNNING);
    }
    return stat;
}

















