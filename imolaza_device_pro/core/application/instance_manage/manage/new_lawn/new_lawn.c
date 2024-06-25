#include "core/fset_lib.h"

#include "new_lawn.h"

// void m_static_new_lawn_object_enable(instance_doc_t *wobj, uint64_t in_present_time, bool recover);

// void m_static_new_lawn_object_save(uint64_t in_id, char *in_param);

// /**
//  * @brief 为一个已经存在的新草坪计划设置时间参数，并且推算时间 如果是启用的话 还添加时间节点
//  *
//  * @param wobj       实例对象
//  * @param in_param   等待添加的时间参数
//  * @param in_time_s  当前时间 HHMMSS 格式
//  */
// void m_callable_new_lawn_object_init(instance_doc_t *wobj, char *in_param, uint64_t in_time_s)
// {
//     DEBUG_TEST( DB_I,m_newlawn_init");
//     /*  1、解析一下*/
//     m_static_new_lawn_parse_parameters(wobj->sch_info.cu_id, in_param);
//     /** 2、保存一下*/
//     // m_newlawn_save(in_id ,  in_param );
//     m_static_new_lawn_object_save(wobj->sch_info.cu_id, in_param);
//     /** 3、从内存中恢复一下*/
//     // m_mem_flash_splan_enable(in_id ,2);
//     // m_static_new_lawn_object_enable(wobj, in_time_s % NUM_OF_DAY_SEC_SUM);
// }
// /**
//  * @brief 为一个已经存在的新草坪计划设置时间参数，并且返回距离最近的下次运行时间
//  *
//  * @param in_param 时间参数信息
//  * @param out_time  输出最近的参数 可以为 NULL
//  * @param in_time_s 当前的时间 HHMMSS 格式
//  * @return stat_m
//  */
// stat_m m_callable_new_lawn_fix_and_add_time(uint64_t inst_id, char *in_param, uint64_t *out_time, uint64_t in_time_s)
// {
//     stat_m stat = fail_r;
//     // 解析时间、 更新内存
//     m_static_new_lawn_parse_parameters(inst_id, in_param);
//     /** 2、保存一下*/
//     m_static_new_lawn_object_save(inst_id, in_param);

//     // *out_time = m_static_new_lawn_srech_nexttime(inst_id, in_time_s);

//     return stat;
// }
/**
 * @brief 更新计划参数 并且返回下一次时间
 *
 * @param in_id 计划 ID
 * @param in_param 示例参数
 * @param out_pretime 输出上一次最接近的时间
 * @param out_nexttime 输出下一次最接近的时间
 * @param in_present_time 现在当前的时间  HHMMSS 格式
 * @return uint64_t
 */
stat_m m_static_new_lawn_object_update(uint64_t in_id, char *in_param, uint64_t *out_pretime, uint64_t *out_nexttime, uint64_t in_present_time)
{
    DEBUG_TEST( DB_I,"m_newlawn_update   %s", in_param);
    stat_m stat = fail_r;
    // bool result = delete_time_node(in_id  ,9);
    // 根据节点判断删除节点
    /*  1、解析一下*/
    if (in_param != NULL)
    {
        m_static_new_lawn_parse_parameters(in_id, in_param);
        /** 2、保存一下*/
        // m_newlawn_save(in_id ,  in_param );
    }

    stat = m_static_new_lawn_srech_near_time(in_id, out_pretime, out_nexttime, in_present_time);
    // if(!once || result)
    //     m_static_time_link_notece_node_add(in_id  ,wctime ,plan_runing);

    return stat;
}

// void m_static_new_lawn_object_enable(instance_doc_t *wobj, uint64_t in_present_time, bool recover)
// {
//     // 恢复这个计划 需要开始一个时间
//     // int wctime = 0;
//     // DEBUG_TEST( DB_I,m_newlawn_schedule_enable ( in_present_time : %lld recover %d)" , in_present_time , recover);
//     // delete_time_node(wobj->schedule_id  ,9);
//     // // 1、找到临近的时间，也就是最新开始的时间（在列表中）
//     // if(recover)
//     //     wctime =  m_nllist_srech_pretime(wobj->schedule_id , in_present_time);
//     // else
//     //     wctime =  m_nllist_srech_nexttime(wobj->schedule_id , in_present_time);
//     // DEBUG_TEST(DB_W,"%d" , wctime);
//     // // 2、判断需不需要恢复(断电)
//     // if(recover) //
//     // {
//     //     wobj->WSTime_clone = wobj->WSTime = wctime;

//     // // 3、设置一些参数直接加时间节点
//     // }else
//     // {
//     //     wobj->WSTime_clone = wobj->WSTime = wctime;
//     //     // 如果不是恢复的计划那就是启用的计划
//     //     // 我已经有了最新执行的计划时间，关掉启用的标记，添加运行时间点
//     //     m_static_time_link_notece_node_add(wobj->schedule_id  ,wctime ,plan_runing ,wobj);
//     // }
// }

// #define M_NEWlawn_SAVE_FLAG "nl"
// char temp_han[10] = {0};
// void m_static_new_lawn_object_del(uint64_t in_id)
// {
//     sprintf(temp_han, "nl%lld", in_id);
//     // nvs_delete_key(temp_han);
// }
// void m_static_new_lawn_object_save(uint64_t in_id, char *in_param)
// {
//     // DEBUG_TEST( DB_I,m_newlawn_save");
//     sprintf(temp_han, "nl%lld", in_id);
//     if (in_param != NULL)
//     {
//         printf("%s", temp_han);
//         // fm_splan_save_flash(temp_han,in_param);
//     }
// }

// stat_m m_static_new_lawn_object_load(uint64_t in_id)
// {
//     stat_m stat = fail_r;
//     DEBUG_TEST( DB_I,m_newlawn_load");
//     char temp_vue[80] = {0};

//     sprintf(temp_han, "nl%lld", in_id);

//     printf("%s", temp_han);
//     // stat = fm_splan_read_flash(temp_han , temp_vue);

//     if (stat == succ_r)
//         m_static_new_lawn_parse_parameters(in_id, temp_vue);

//     return stat;
// }
