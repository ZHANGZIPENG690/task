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
// #include "core/timer/mtime.h"
#include "time_link.h"
#include "event_distribution.h"
/**
 * 待删除区域
 *
 */

// extern time_manage _time;

/*    */

#define TURN_ON_UNIFIED_NOTIFICATION_SCHEDULE 1

static bool node_time_safe = false;

#define NOTIFY_WEATHER_585 (9 * 60 + 45)
#define NOTIFY_WEATHER_285 (4 * 60 + 45)
#if UNIT_TEST

#define NOTIFY_WEATHER_45 (30)
#define NOTIFY_WATER_HAMMER_10 (10)
#else
#define NOTIFY_WEATHER_45 (45)
#define NOTIFY_WATER_HAMMER_10 (10)
#endif
/*
外围的结构体需要一个
*/

struct schedule_run_notice_mem
{
    enum fsm_change_cause cause;
    /* 会留存上一个状态 */
    uint8_t schedule_run_notice_pre_tag;
    /* 当前的状态 */
    uint8_t schedule_run_notice_tag;

    /* 计划通知时间 可以是计划开始时间 ，也可以是计划运行中的时间 */
    uint64_t schedule_run_notice_time;
    /* 当前节点的对象 */
    void *schedule_run_notice_obj;
};
/** 回调函数  */
void (*time_node_cb)(struct schedule_run_notice_mem *) = NULL;

/**
 * @brief  有效代码区域
 *
 */
struct schedule_run_notice_time_tab
{
    /* 计划通知id ，一般和计划id相同 */
    uint64_t schedule_run_notice_time_id;

    bool is_disable;

    struct schedule_run_notice_time_tab *time_tab_previouds;
    struct schedule_run_notice_time_tab *time_tab_next;

    /* 包含的计划信息 */
    struct schedule_run_notice_mem m_schedule_notice_info;
};

// static struct schedule_run_notice_time_tab time_node_root = {99999, NULL, NULL, {0, 0, NULL}};
static struct schedule_run_notice_time_tab time_node_root = {99999, false, NULL, NULL, {0, 0, NULL}};

stat_m m_static_time_link_notece_node_add(uint64_t in_id, uint64_t in_notice_time, uint8_t in_tag, void *in_obj)
{
    stat_m stat = fail_r;
    if (in_id < 1 || in_tag < 1 || in_obj == NULL)
    {
        // create_send_data_to_server_debug(DEVICE_DEBUG, 20, in_id);
        // printf("Fail m_callable_add_notice_schedule_run_node (%lld - %lld - %hhd)... ", in_id, in_notice_time, in_tag);
        stat = fail_r;
        goto end;
    }
    {
        // while (node_time_safe)
        //     vTaskDelay(10); // lock
        node_time_safe = true;
    }
    struct schedule_run_notice_time_tab *tp = NULL;
    DEBUG_TEST(DB_I, "m_add_schedule_run_notice_node( id : %lld time %lld status : %hhd )", in_id, in_notice_time, in_tag);

    // in_notice_time %= 86400; // 先斩一🔪  {不斩}

    static struct schedule_run_notice_time_tab *new_schedule_run_notice_time_node = NULL;

    new_schedule_run_notice_time_node = (struct schedule_run_notice_time_tab *)malloc(sizeof(struct schedule_run_notice_time_tab));
    memset(new_schedule_run_notice_time_node, 0, sizeof(*new_schedule_run_notice_time_node));

    new_schedule_run_notice_time_node->schedule_run_notice_time_id = in_id;
    new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_time = in_notice_time;
    new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_tag = in_tag;
    new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_pre_tag = M_BASE_STATUS_IDLE;
    new_schedule_run_notice_time_node->is_disable = false;
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
            if (
                // tp->m_schedule_notice_info.schedule_run_notice_tag == new_schedule_run_notice_time_node->m_schedule_notice_info.schedule_run_notice_tag
                // &&
                tp->schedule_run_notice_time_id == new_schedule_run_notice_time_node->schedule_run_notice_time_id)
            {
                node_time_safe = false;
                if (tp->schedule_run_notice_time_id == in_id)
                {
                    // memset(tp, 0, sizeof(*tp));
                    tp->m_schedule_notice_info.cause = 0;
                    tp->m_schedule_notice_info.schedule_run_notice_pre_tag = M_BASE_STATUS_IDLE;
                    tp->schedule_run_notice_time_id = in_id;
                    tp->m_schedule_notice_info.schedule_run_notice_tag = in_tag;
                    tp->m_schedule_notice_info.schedule_run_notice_obj = in_obj;
                    tp->m_schedule_notice_info.schedule_run_notice_time = in_notice_time;
                    tp->is_disable = false;
                    M_FREE(new_schedule_run_notice_time_node);
                    new_schedule_run_notice_time_node = NULL;
                    DEBUG_TEST(DB_W, "Time Node Replace %lld , %lld", tp->schedule_run_notice_time_id, in_notice_time);
                }
                // if (true)
                //     DEBUG_E("time info same :%lld, %hhd", new_schedule_run_notice_time_node->schedule_run_notice_time,
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

            if (true)
                DEBUG_TEST(DB_I, "id : %lld ,timeinfo: %lld , status : %hhd (%d)",
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
stat_m m_static_time_link_notece_node_change_obj(uint64_t id, uint64_t in_notice_time, uint8_t state, void *data, int in_change_cause)
{
    stat_m stat = fail_r;
    struct schedule_run_notice_time_tab *tp = NULL;
    DEBUG_TEST(DB_W, "m_change_schedule_run_notice_node_obj :(%lld)(%lld)(%hhd)  -->", id, in_notice_time, state);
    if (time_node_root.time_tab_next != NULL)
    {
        tp = time_node_root.time_tab_next;
        struct schedule_run_notice_time_tab *tpl = tp;
        while (tp != NULL)
        {
            if (tp->schedule_run_notice_time_id == id)
            {
                if (data != NULL)
                {
                    tp->m_schedule_notice_info.schedule_run_notice_obj = data;
                    // DEBUG_TEST( DB_I,"%d , %d", sizeof(tp->m_schedule_notice_info.schedule_run_notice_obj), sizeof(data));
                    // DEBUG_TEST( DB_I,"* %d , %d", sizeof(*tp->m_schedule_notice_info.schedule_run_notice_obj), sizeof(*data));
                    // DEBUG_TEST( DB_I,"str %d , %d", strlen(tp->m_schedule_notice_info.schedule_run_notice_obj), strlen(data));
                }

                // if (data != NULL && state != M_BASE_STATUS_IDLE)
                //     tp->m_schedule_notice_info.cause = M_STATIC_FSM_CHANGE_STATE_CAUSE_INDOC_OR_NULL;
                if (data != NULL)
                    tp->m_schedule_notice_info.cause = in_change_cause;

                if (state > 0 && state != None)
                    // tp->m_schedule_notice_info.schedule_run_notice_tag = state;
                    tp->m_schedule_notice_info.schedule_run_notice_pre_tag = tp->m_schedule_notice_info.schedule_run_notice_tag,
                    tp->m_schedule_notice_info.schedule_run_notice_tag = state;

                if (in_notice_time != None)
                    tp->m_schedule_notice_info.schedule_run_notice_time = in_notice_time;
                //     tp->m_schedule_notice_info.cause = M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE;

                // if ( in_notice_time == None && state == None && data != NULL)
                //     tp->m_schedule_notice_info.cause = M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE;

                stat = succ_r;
                break;
            }
            tpl = tp;
            tp = tp->time_tab_next;
        }
    }
    if (stat != succ_r)
        DEBUG_TEST(DB_W, "Change Not Fount !");
    return stat;
}
stat_m m_static_time_link_notece_node_remove_by_id(uint64_t id)
{
    {
        // while (node_time_safe)
        //     vTaskDelay(10);
        node_time_safe = true;
    }
    struct schedule_run_notice_time_tab *tp = NULL;

    DEBUG_E("delete it  : id : %lld status : ", id);
    if (id == 666 || id == 665 || id == 664)
    {
        /*把m_run_time.zonenum 置为0,下一次快速运行实例重新赋值*/
        m_run_time.zonenum = 0;
    }

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
                    M_FREE(tp);
                    tp = tpl->time_tab_next;
                    continue;
                }
            }
            tpl = tp;
            tp = tp->time_tab_next;
        }
    }
    node_time_safe = false;
    return 0;
}
/**
 * @brief 获取一个节点的时间
 *
 * @param id
 * @param out_time
 * @return stat_m
 */
stat_m m_static_time_link_notece_node_id_get_by_time(uint64_t id, uint64_t *out_time)
{
    struct schedule_run_notice_time_tab *tp = NULL;

    if (time_node_root.time_tab_next != NULL)
    {
        tp = time_node_root.time_tab_next;
        while (tp != NULL)
        {
            if (tp->schedule_run_notice_time_id == id)
            {
                *out_time = tp->m_schedule_notice_info.schedule_run_notice_time;
                break;
            }
            tp = tp->time_tab_next;
        }
    }
    node_time_safe = false;
    return 0;
}
stat_m m_static_time_link_notece_node_change(uint64_t id, uint64_t in_notice_time, uint8_t state)
{
    stat_m stat = fail_r;
    struct schedule_run_notice_time_tab *tp = NULL;
    DEBUG_TEST(DB_W, "m_change_schedule_run_notice_node :(id %lld) %lld - %hhd", id, in_notice_time, state);
    if (time_node_root.time_tab_next != NULL)
    {
        tp = time_node_root.time_tab_next;
        struct schedule_run_notice_time_tab *tpl = tp;
        while (tp != NULL)
        {
            if (tp->schedule_run_notice_time_id == id)
            {
                if (in_notice_time != None)
                    tp->m_schedule_notice_info.schedule_run_notice_time = in_notice_time;
                if (state != None)
                {
                    tp->m_schedule_notice_info.schedule_run_notice_pre_tag = tp->m_schedule_notice_info.schedule_run_notice_tag,
                    tp->m_schedule_notice_info.schedule_run_notice_tag = state;
                }
                stat = succ_r;
                break;
            }
            tpl = tp;
            tp = tp->time_tab_next;
        }
    }
    return stat;
}
stat_m m_remove_schedule_run_notice_node_all(void)
{
    DEBUG_TEST(DB_W, "will delete : all node");
    struct schedule_run_notice_time_tab *tpall = time_node_root.time_tab_next, *tp_temp;
    while (tpall != NULL)
    {
        tp_temp = tpall->time_tab_next;
        // tpall->time_tab_next = NULL;
        // tpall->time_tab_previouds = NULL;
        DEBUG_E("Delete node :(%lld:) ...", tpall->schedule_run_notice_time_id);
        M_FREE(tpall);
        tpall = NULL;
        tpall = tp_temp;
    }
    return 0;
}
stat_m m_remove_schedule_run_notice_node(uint64_t id, uint8_t state)
{
    {
        // while (node_time_safe)
        //     vTaskDelay(10);
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
                    M_FREE(tp);
                    tp = NULL;
                    tp = tpl->time_tab_next;
                    continue;
                }
                else
                {
                    M_FREE(tp);
                    tp = NULL;
                    node_time_safe = false;
                    return 0;
                }
            }
            tpl = tp;
            tp = tp->time_tab_next;
        }
    }
    node_time_safe = false;
    return 0;
}

struct schedule_run_notice_mem sh_run_notice[16];

struct schedule_run_notice_time_tab *tp_able = NULL;
stat_m m_static_time_link_notece_node_enable(uint64_t in_id)
{
    stat_m stat = fail_r;
    if (tp_able != NULL && tp_able->schedule_run_notice_time_id == in_id)
    {
        tp_able->is_disable = false;
        tp_able = NULL;
        stat = succ_r;
    }
    else if (time_node_root.time_tab_next != NULL)
    {
        tp_able = time_node_root.time_tab_next;
        struct schedule_run_notice_time_tab *tpl = tp_able;
        while (tp_able != NULL)
        {
            if (tp_able->schedule_run_notice_time_id == in_id)
            {
                tp_able->is_disable = false;
                break;
            }
            tpl = tp_able;
            tp_able = tp_able->time_tab_next;
        }
    }
    return stat;
}
stat_m m_static_time_link_notece_node_disable(uint64_t in_id)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_I, "m_static_time_link_notece_node_disable ,%lld", in_id);
    if (time_node_root.time_tab_next != NULL)
    {
        tp_able = time_node_root.time_tab_next;
        // struct schedule_run_notice_time_tab *tpl = tp_able;
        while (tp_able != NULL)
        {
            if (tp_able->schedule_run_notice_time_id == in_id)
            {

                tp_able->is_disable = true;
                tp_able->is_disable = true;
                stat = succ_r;
                break;
            }
            // tpl = tp_able;
            tp_able = tp_able->time_tab_next;
        }
    }
    if (stat != succ_r)
        tp_able = NULL;
    return stat;
}

// void m_query_notice_node_all(uint32_t in_present_time, bool in_back, struct schedule_run_notice_mem **mem)
stat_m m_static_time_link_notece_node_advancing(uint64_t in_present_time)
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
            if (!query_tp->is_disable)
            {
                // printf("ll+++(%lld)+\n" , query_tp->m_schedule_notice_info.schedule_run_notice_time);
                // printf("%lld.", query_tp->schedule_run_notice_time_id);
                if (in_present_time > query_tp->m_schedule_notice_info.schedule_run_notice_time + 10)
                {
                    DEBUG_TEST(DB_I, "current: %lld -> %lld", in_present_time, query_tp->m_schedule_notice_info.schedule_run_notice_time);
                    m_static_tools_instance_obj_will_refresh_time(query_tp->m_schedule_notice_info.schedule_run_notice_obj,
                                                                  &query_tp->m_schedule_notice_info.schedule_run_notice_time,
                                                                  &query_tp->m_schedule_notice_info.schedule_run_notice_tag,
                                                                  in_present_time);
                    continue;
                }
                else
                    tric = ((query_tp->m_schedule_notice_info.schedule_run_notice_time) - in_present_time);
                if (tric == NOTIFY_WATER_HAMMER_10 && m_callable_device_attribute_get_hammer() == succ_r)
                {
                    m_callable_event_distribution_tiansit(query_tp->m_schedule_notice_info.schedule_run_notice_pre_tag, query_tp->m_schedule_notice_info.schedule_run_notice_tag,
                                                          M_STATIC_FSM_CHANGE_STATE_CAUSE_WATER_HAMMER, query_tp->m_schedule_notice_info.schedule_run_notice_obj, in_present_time);
                }
                // 查询到的计划距离现在还剩多长时间运行
                if ((tric == NOTIFY_WEATHER_585 || tric == NOTIFY_WEATHER_285 || tric == NOTIFY_WEATHER_45) &&
                    query_tp->m_schedule_notice_info.schedule_run_notice_tag == M_BASE_STATUS_IDLE)
                {
                    DEBUG_TEST(DB_I, "------------->- tric %d (%lld)->%d\n",

                               tric, query_tp->schedule_run_notice_time_id, query_tp->m_schedule_notice_info.schedule_run_notice_tag);

                    m_callable_event_distribution_tiansit(M_BASE_STATUS_IDLE, M_BASE_STATUS_PREPARE, query_tp->m_schedule_notice_info.cause,
                                                          query_tp->m_schedule_notice_info.schedule_run_notice_obj, in_present_time + tric);
                    // sh_run_notice[node_sum].schedule_run_notice_obj = query_tp->m_schedule_notice_info.schedule_run_notice_obj;
                    // sh_run_notice[node_sum].schedule_run_notice_tag = M_BASE_STATUS_RUNNING;
                }
                if (tric == 0)
                {
#if TURN_ON_UNIFIED_NOTIFICATION_SCHEDULE // 一种是加入数组
                    if (query_tp->m_schedule_notice_info.schedule_run_notice_obj != NULL)
                    {
                        sh_run_notice[node_sum].schedule_run_notice_obj = query_tp->m_schedule_notice_info.schedule_run_notice_obj;
                        sh_run_notice[node_sum].schedule_run_notice_tag = query_tp->m_schedule_notice_info.schedule_run_notice_tag;
                        sh_run_notice[node_sum].schedule_run_notice_pre_tag = query_tp->m_schedule_notice_info.schedule_run_notice_pre_tag;
                        sh_run_notice[node_sum].cause = query_tp->m_schedule_notice_info.cause;

                        node_sum += 1;
                    }
#else // 一种是回调
                    if (time_node_cb != NULL)
                    {
                        time_node_cb(query_tp->m_schedule_notice_info);
                    }
#endif
                }
                if (in_present_time % 10 == 0)

                    DEBUG_TEST(DB_I, ",query notice node... pre: %hhd \t tag:  %hhd\ttime %lld\tid: %lld\tpresent time : %lld  ",
                               query_tp->m_schedule_notice_info.schedule_run_notice_pre_tag,
                               query_tp->m_schedule_notice_info.schedule_run_notice_tag,
                               query_tp->m_schedule_notice_info.schedule_run_notice_time,
                               query_tp->schedule_run_notice_time_id,
                               in_present_time);
            }
            else if (in_present_time % 10 == 0)

            {
                DEBUG_TEST_HIDE(",query notice node... pre: %hhd \t tag:  %hhd\ttime %lld\tid: %lld\tpresent time : %lld  ",
                                query_tp->m_schedule_notice_info.schedule_run_notice_pre_tag,
                                query_tp->m_schedule_notice_info.schedule_run_notice_tag,
                                query_tp->m_schedule_notice_info.schedule_run_notice_time,
                                query_tp->schedule_run_notice_time_id,
                                in_present_time);
            }
            if (query_tp->schedule_run_notice_time_id != 666 && query_tp->schedule_run_notice_time_id != 665 && query_tp->schedule_run_notice_time_id != 664)
            {
                m_callable_ota_can_start_updating_time_set(query_tp->m_schedule_notice_info.schedule_run_notice_time);
            }
        }
        if ((query_tp->time_tab_next == NULL))
        {
            m_callable_ota_can_start_updating_time_set(9702323389);
        }
    }
    int reX = 0;
    int reY = 1;
    /***
     * 先结束 ，再开始
     */
    if (node_sum > 1 && sh_run_notice[1].schedule_run_notice_tag == M_BASE_STATUS_STOP)
    {
        reX = 1;
        reY = -1;
    }
    /** 执行 */
    for (int x = reX; x < node_sum && x >= 0; x += reY)
    { //
        if (sh_run_notice[x].schedule_run_notice_obj != NULL)
        {
            // printf("BC %d - %d\n", node_sum, x);
            // printf("%d\n" , query_tp->is_disable);
            //     if(m_callable_zone_have_next(sh_run_notice[0].schedule_run_notice_obj , m_callable_get_utc_time_t()) == succ_r)
            //         sh_run_notice[0].schedule_run_notice_tag = m_schedule__stop;
            //     m_callable_schedule_fsm_post_event(sh_run_notice[0].schedule_run_notice_tag , (void *)sh_run_notice[0].schedule_run_notice_obj);
            //     DEBUG_TEST(DB_I,"ai qing %d", sh_run_notice[0].schedule_run_notice_tag );
            // if (sh_run_notice[x].schedule_run_notice_tag == M_BASE_STATUS_STOP)
            //     sh_run_notice[x].schedule_run_notice_tag = M_BASE_STATUS_IDLE;
            if (sh_run_notice[x].schedule_run_notice_pre_tag == M_BASE_STATUS_IDLE && sh_run_notice[x].schedule_run_notice_tag == M_BASE_STATUS_PREPARE)
                sh_run_notice[x].schedule_run_notice_pre_tag = M_BASE_STATUS_PREPARE, sh_run_notice[x].schedule_run_notice_tag = M_BASE_STATUS_RUNNING;
            m_callable_event_distribution_tiansit(sh_run_notice[x].schedule_run_notice_pre_tag, sh_run_notice[x].schedule_run_notice_tag,
                                                  sh_run_notice[x].cause, sh_run_notice[x].schedule_run_notice_obj, in_present_time);

            sh_run_notice[x].schedule_run_notice_obj = NULL;
        }
    }
    node_time_safe = false;
    return 0;
}
// /**
//  * @brief  获取该计划的上一个状态信息
//  *
//  * @param in_data
//  * @param out_target
//  * @return stat_m
//  */
// stat_m m_static_time_link_notece_node_get_pre_ste(void *in_data, uint8_t *out_target)
// {
//     if (time_node_root.time_tab_next != NULL)
//     {
//         tp_able = time_node_root.time_tab_next;
//         struct schedule_run_notice_time_tab *tpl = tp_able;
//         while (tp_able != NULL)
//         {
//             if (tp_able->m_schedule_notice_info.schedule_run_notice_obj == in_data)
//             {
//                 *out_target = tp_able->m_schedule_notice_info.schedule_run_notice_pre_tag;

//                 printf("get ..........%lld. (%lld)      %d\n", tp_able->schedule_run_notice_time_id, tp_able->m_schedule_notice_info.schedule_run_notice_obj,
//                        tp_able->m_schedule_notice_info.schedule_run_notice_pre_tag),
//                     tp_able->m_schedule_notice_info.schedule_run_notice_pre_tag = 0;
//                 printf(".AAAAAAAAAAA       %d\n", tp_able->m_schedule_notice_info.schedule_run_notice_pre_tag);
//                 break;
//             }
//             tpl = tp_able;
//             tp_able = tp_able->time_tab_next;
//         }
//     }
//     return succ_r;
// }
