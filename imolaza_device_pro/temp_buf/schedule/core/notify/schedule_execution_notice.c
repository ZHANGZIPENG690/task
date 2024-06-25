/*
    计划通知时间表

    包含计划运行的列表，当然这个列表是时间列表，所有计划将会运行的时候都会通知，通知到提供一个函数给你处理，由你决定是否通知到服务器
    本文件包含 时间链表增删改查 和 查询通知函数，

    注意
        有一个特殊的函数，查询回调函数，如果同一时间有多个计划执行需要处理，他会回调多次
*/

// #include "hot_so_fast.h"
// #include "tool.h"
#include "core/fset_lib.h"
#include "core/notify/notify.h"
// #include "core/timer/mtime.h"
/**
 * 待删除区域
 *
 */

// extern time_manage _time;

/*    */

#define TURN_ON_UNIFIED_NOTIFICATION_SCHEDULE 1

static bool node_time_safe = false;

void (*time_node_cb)(struct schedule_run_notice_mem *);

/*
外围的结构体需要一个
*/

struct schedule_run_notice_mem
{
    /* 当前的状态 */
    uint8_t schedule_run_notice_tag;

    /* 计划通知时间 可以是计划开始时间 ，也可以是计划运行中的时间 */
    uint32_t schedule_run_notice_time;
    /* 当前节点的对象 */
    void *schedule_run_notice_obj;
};

/**
 * @brief  有效代码区域
 *
 */
struct schedule_run_notice_time_tab
{
    /* 计划通知id ，一般和计划id相同 */
    uint64_t schedule_run_notice_time_id;

    struct schedule_run_notice_time_tab *time_tab_previouds;
    struct schedule_run_notice_time_tab *time_tab_next;

    /* 包含的计划信息 */
    struct schedule_run_notice_mem m_schedule_notice_info;
};

static struct schedule_run_notice_time_tab time_node_root = {99999, NULL, NULL, {0, 0, NULL}};

stat_m m_callable_add_notice_schedule_run_node(uint64_t in_id, uint64_t in_notice_time, uint8_t in_tag, void *in_obj)
{
    stat_m stat = fail_r;
    if (in_id < 1 || in_tag < 1 || in_obj == NULL)
    {
        // create_send_data_to_server_debug(DEVICE_DEBUG, 20, in_id);
        printf("Fail m_callable_add_notice_schedule_run_node (%lld - %lld - %hhd)... ", in_id, in_notice_time, in_tag);
        stat = fail_r;
        goto end;
    }
    {
        // while (node_time_safe)
        //     vTaskDelay(10); // lock
        node_time_safe = true;
    }
    struct schedule_run_notice_time_tab *tp = NULL;
    DEBUG_TEST(DB_I,"m_add_schedule_run_notice_node( id : %lld time %lld status : %hhd )", in_id, in_notice_time, in_tag);
    in_notice_time %= 86400; // 先斩一🔪

    static struct schedule_run_notice_time_tab *new_schedule_run_notice_time_node = NULL;
    new_schedule_run_notice_time_node = (struct schedule_run_notice_time_tab *)malloc(sizeof(struct schedule_run_notice_time_tab));
    memset(new_schedule_run_notice_time_node, 0, sizeof(*new_schedule_run_notice_time_node));

    new_schedule_run_notice_time_node->schedule_run_notice_time_id = in_id;
    new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_time = in_notice_time;
    new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_tag = in_tag;
    new_schedule_run_notice_time_node->time_tab_next = NULL;
    new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_obj = in_obj;

    if (time_node_root.time_tab_next == NULL)
    {
        time_node_root.time_tab_next = new_schedule_run_notice_time_node;

        // create_send_data_to_server_debug_timenode(DEVICE_DEBUG,
        //                                           new_schedule_run_notice_time_node->tag,
        //                                           new_schedule_run_notice_time_node->schedule_run_notice_time_id,
        //                                           new_schedule_run_notice_time_node->schedule_run_notice_time_id,
        //                                           0);// hhmmss_time
        stat = succ_r;
        goto end;
    }
    tp = &time_node_root;
    while (1)
    {
        if (tp != NULL)
            if (tp->m_schedule_notice_info.schedule_run_notice_tag == new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_tag &&
                tp->schedule_run_notice_time_id == new_schedule_run_notice_time_node->schedule_run_notice_time_id)
            {
                node_time_safe = false;
                if (tp->schedule_run_notice_time_id == in_id)
                {
                    tp->m_schedule_notice_info.schedule_run_notice_tag = in_tag;
                    tp->m_schedule_notice_info.schedule_run_notice_obj = in_obj;
                    tp->m_schedule_notice_info.schedule_run_notice_time = in_notice_time;
                    free(new_schedule_run_notice_time_node);
                    new_schedule_run_notice_time_node = NULL;
                }
                // if (true)
                //     DEBUG_E("time info same :%d, %hhd", new_schedule_run_notice_time_node->schedule_run_notice_time,
                //             new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_tag);
                stat = fail_r;
                return stat;
            }
        if (tp->time_tab_next != NULL)
        {
            tp = tp->time_tab_next;
        }
        else
        {
            tp->time_tab_next = new_schedule_run_notice_time_node;
            // if (in_tag == plan_runing)
            // create_send_data_to_server_debug_timenode(DEVICE_DEBUG,
            //                                           new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_tag,
            //                                           new_schedule_run_notice_time_node->schedule_run_notice_time_id,
            //                                           new_schedule_run_notice_time_node->schedule_run_notice_time_id,
            //                                           _time.hhmmss);//_time.new_get_hhmmss()
            if (true)
                DEBUG_TEST(DB_I,"id : %lld ,timeinfo: %d , status : %hhd (%d)",
                        tp->time_tab_next->schedule_run_notice_time_id,
                        tp->time_tab_next->m_schedule_notice_info.schedule_run_notice_time,
                        tp->time_tab_next->m_schedule_notice_info.schedule_run_notice_tag,
                        0); // _time.timer_get_time_the_same_day_timeStamp()

            break;
        }
    }
end:
    node_time_safe = false;
    return stat;
}

void m_remove_all_schedule_run_notice_node(uint64_t id)
{
    {
        while (node_time_safe)
            mDelay_ms(10);
        node_time_safe = true;
    }
    struct schedule_run_notice_time_tab *tp = NULL;

    DEBUG_E("delete all  : id : %lld status : ", id);

    if (time_node_root.time_tab_next != NULL)
    {
        tp = time_node_root.time_tab_next;
        struct schedule_run_notice_time_tab *tpl = tp;
        while (tp != NULL)
        {
            if (tp->schedule_run_notice_time_id == id)
            {
                if (tp == time_node_root.time_tab_next)
                    time_node_root.time_tab_next = tp->time_tab_next;
                else
                {
                    tpl->time_tab_next = tp->time_tab_next;
                }
                // if (state == 9)
                {
                    // if (currentRunId.currId == tp->time_node_id)
                    //     currentRunId.currId = 0, currentRunId.isRuning = false;
                    tp = NULL;
                    free(tp);
                    tp = tpl->time_tab_next;
                    continue;
                }
            }
            tpl = tp;
            tp = tp->time_tab_next;
        }
    }
    node_time_safe = false;
    return;
}
void m_change_schedule_run_notice_node(uint64_t id, uint64_t in_notice_time, uint8_t state)
{
    struct schedule_run_notice_time_tab *tp = NULL;
    DEBUG_TEST(DB_W,"m_change_schedule_run_notice_node(%lld) : %lld - %hhd", id, in_notice_time, state);
    if (time_node_root.time_tab_next != NULL)
    {
        tp = time_node_root.time_tab_next;
        struct schedule_run_notice_time_tab *tpl = tp;
        while (tp != NULL)
        {
            if (tp->schedule_run_notice_time_id == id)
            {
                tp->m_schedule_notice_info.schedule_run_notice_time = in_notice_time;
                tp->m_schedule_notice_info.schedule_run_notice_tag = state;
                DEBUG_TEST(DB_W,"找到修改下 %lld Tm %lld Ste %hhd", tp->schedule_run_notice_time_id,
                             tp->m_schedule_notice_info.schedule_run_notice_time,
                             tp->m_schedule_notice_info.schedule_run_notice_tag);
                break;
            }
            tpl = tp;
            tp = tp->time_tab_next;
        }
    }
}
void m_remove_schedule_run_notice_node_all(void)
{
    DEBUG_TEST(DB_W,"will delete : all node");
    struct schedule_run_notice_time_tab *tpall = time_node_root.time_tab_next, *tp_temp;
    while (tpall != NULL)
    {
        tp_temp = tpall->time_tab_next;
        // tpall->time_tab_next = NULL;
        // tpall->time_tab_previouds = NULL;
        DEBUG_E("Delete node :(%lld:) ...", tpall->schedule_run_notice_time_id);
        free(tpall);
        tpall = NULL;
        tpall = tp_temp;
    }
}
void m_remove_schedule_run_notice_node(uint64_t id, uint8_t state)
{
    {
        while (node_time_safe)
            mDelay_ms(10);
        node_time_safe = true;
    }
    struct schedule_run_notice_time_tab *tp = NULL;

    DEBUG_E("delete : id : %lld status : %hhd\r\n", id, state);

    if (time_node_root.time_tab_next != NULL)
    {
        tp = time_node_root.time_tab_next;
        struct schedule_run_notice_time_tab *tpl = tp;
        while (tp != NULL)
        {
            if (tp->schedule_run_notice_time_id == id && (tp->m_schedule_notice_info.schedule_run_notice_tag == state || state == 9))
            {
                if (tp == time_node_root.time_tab_next)
                    time_node_root.time_tab_next = tp->time_tab_next;
                else
                    tpl->time_tab_next = tp->time_tab_next;
                if (state == 9)
                {
                    // if (currentRunId.currId == tp->time_node_id)
                    //     currentRunId.currId = 0, currentRunId.isRuning = false;
                    free(tp);
                    tp = NULL;
                    tp = tpl->time_tab_next;
                    continue;
                }
                else
                {
                    free(tp);
                    tp = NULL;
                    node_time_safe = false;
                    return;
                }
            }
            tpl = tp;
            tp = tp->time_tab_next;
        }
    }
    node_time_safe = false;
    return;
}

struct schedule_run_notice_mem sh_run_notice[16];

// void m_query_notice_node_all(uint32_t in_present_time, bool in_back, struct schedule_run_notice_mem **mem)
void m_callable_query_notice_schedule_run_node(uint64_t in_present_time)
{
    // DEBUG_TEST(DB_I,"m_query_notice_node_all");
    memset(sh_run_notice, 0, sizeof(sh_run_notice));
    {
        // while (node_time_safe)
        //     vTaskDelay(10);
        node_time_safe = true;
    }
    int tric = 0;
    uint8_t node_sum = 0;
    struct schedule_run_notice_time_tab *query_tp = &time_node_root;

    {
        while (query_tp->time_tab_next != NULL)
        {
            query_tp = query_tp->time_tab_next;
            tric = ((query_tp->m_schedule_notice_info.schedule_run_notice_time + NUM_OF_DAY_SEC_SUM) - (in_present_time % NUM_OF_DAY_SEC_SUM)) % NUM_OF_DAY_SEC_SUM;
            // 查询到的计划距离现在还剩多长时间运行
            // printf("%d\n" , tric    );
            if ((tric == 3570 || tric == 90) && query_tp->m_schedule_notice_info.schedule_run_notice_tag == m_schedule__idle)
            {
                printf("-------------- tric %d \n", tric);
                // m_callable_schedule_fsm_post_event(m_schedule__prepare , query_tp->m_schedule_notice_info.schedule_run_notice_obj);
                sh_run_notice[node_sum].schedule_run_notice_obj = query_tp->m_schedule_notice_info.schedule_run_notice_obj;
                sh_run_notice[node_sum].schedule_run_notice_tag = m_schedule__prepare;
                node_sum += 1;
            }
            if (tric == 10 && query_tp->m_schedule_notice_info.schedule_run_notice_tag == m_schedule__running)
            {
                sh_run_notice[node_sum].schedule_run_notice_tag = m_schedule__water_hammer;
                sh_run_notice[node_sum].schedule_run_notice_obj = query_tp->m_schedule_notice_info.schedule_run_notice_obj;
                // m_event_transit_processing_schedule((void *)sh_run_notice[node_sum].schedule_run_notice_obj , sh_run_notice[node_sum].schedule_run_notice_tag  ,in_present_time);
                node_sum += 1;
            }
            if (tric == 0)
            {
#if TURN_ON_UNIFIED_NOTIFICATION_SCHEDULE // 一种是加入数组
                if (query_tp->m_schedule_notice_info.schedule_run_notice_obj != NULL)
                {
                    sh_run_notice[node_sum].schedule_run_notice_obj = query_tp->m_schedule_notice_info.schedule_run_notice_obj;
                    sh_run_notice[node_sum].schedule_run_notice_tag = query_tp->m_schedule_notice_info.schedule_run_notice_tag;
                    node_sum += 1;
                }
#else // 一种是回调
                if (time_node_cb != NULL)
                {
                    time_node_cb(query_tp->m_schedule_notice_info);
                }
#endif
            }

            DEBUG_TEST(DB_I," id: %lld   time   %d   tag:  %hhd     present time %d",
                    // wobj->in_run_id,
                    query_tp->schedule_run_notice_time_id,
                    query_tp->m_schedule_notice_info.schedule_run_notice_time,
                    query_tp->m_schedule_notice_info.schedule_run_notice_tag,
                    (in_present_time % NUM_OF_DAY_SEC_SUM));
        }
    }
    /***
        m_schedule__prepare = 0, // 29-24
        m_schedule__running,     // 24-x
        m_schedule__sacking,     // x-254
        m_schedule__will_stop,   // x-50
        m_schedule__idle
    */
    { //
        if (sh_run_notice[0].schedule_run_notice_obj != NULL)
        {
            // if(m_callable_zone_have_next(sh_run_notice[0].schedule_run_notice_obj , m_callable_get_utc_time_t()) == succ_r)
            //     sh_run_notice[0].schedule_run_notice_tag = m_schedule__stop;
            DEBUG_TEST(DB_I,"ai qing %d", sh_run_notice[0].schedule_run_notice_tag);
            m_event_transit_processing_schedule((void *)sh_run_notice[0].schedule_run_notice_obj, sh_run_notice[0].schedule_run_notice_tag, in_present_time);

            // m_callable_schedule_fsm_post_event(sh_run_notice[0].schedule_run_notice_tag , (void *)sh_run_notice[0].schedule_run_notice_obj);
            sh_run_notice[0].schedule_run_notice_obj = NULL;
        }
    }
    node_time_safe = false;
}
