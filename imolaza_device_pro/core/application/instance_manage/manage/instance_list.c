
/***
 * 有一个 头    用作计划全局管理 包括全局跳过全局天气等
 *
 * 全局参数下面挂的是
 *
 */
#include "core/fset_lib.h"

typedef struct
{
    // 当前运行的计划
    // 设置计划跳过桉树
} WsMan;

struct mitem
{
    struct mitem *next;
    struct mitem *pre;
    // struct mitem *head;
    // struct mitem *tail;
    instance_doc_t *contec;
};

struct mitem mRoot;

extern void sch_print(instance_doc_t *water_d, uint64_t current_time);

/**
 * @brief 查找一个计划对象
 *
 * @param in_uid
 * @param out_water_d
 * @return stat_m
 */
stat_m m_static_instance_list_find_by_id(uint64_t in_uid, instance_doc_t **out_obj)
{
    stat_m stat = fail_r;
    struct mitem *trage_temp = NULL;
    if (out_obj)
    {
    }

    if (mRoot.next != NULL)
    {
        // DEBUG_TEST(DB_W,"--\n");
        trage_temp = mRoot.next;
        while (trage_temp != NULL)
        {
            // DEBUG_TEST(DB_W,"-%lld- %lld\n" , trage_temp->contec->sch_info.cu_id   ,in_uid  );
            if (trage_temp->contec->sch_info.cu_id == in_uid)
            {
                if (out_obj != NULL)
                    *out_obj = trage_temp->contec;
                stat = succ_r;
                break;
            }
            trage_temp = trage_temp->next;
        }
    }
    if (stat != succ_r)
        DEBUG_TEST(DB_W, "m_static_instance_list_find_by_id -> %lld    Not Found !", in_uid);
    //     out_obj = NULL;
    return stat;
}
stat_m m_static_instance_list_remove_by_id(uint64_t in_uid)
{
    stat_m state = fail_r;
    struct mitem *trage_temp = NULL, *trage = NULL, *trage_pre = NULL, *trage_next = NULL;

    if (mRoot.next == NULL)
    {
    }
    else
    {
        trage_temp = mRoot.next;
        trage_pre = trage_temp;

        while (trage_temp != NULL)
        {
            if (in_uid != None && trage_temp->contec->sch_info.cu_id == in_uid)
            {
                state = succ_r;
                trage = trage_temp;
                DEBUG_TEST(DB_I, "find schedule : %lld .. ", trage->contec->sch_info.cu_id);
                trage_next = trage_temp->next;
                trage_pre = trage_temp->pre;
                if (trage_next != NULL)
                    trage_next->pre = trage_temp->pre;
                if (trage_pre != NULL)
                    trage_pre->next = trage_temp->next;

                DEBUG_TEST(DB_I, "find schedule : Free .. ");
                if (trage_temp->contec != NULL)
                    M_FREE(trage_temp->contec);
                trage_temp->contec = NULL;
                M_FREE(trage_temp);
                trage_temp = NULL;
                break;
            }
            if (in_uid == None)
            {
                state = succ_r;
                trage = trage_temp;
                trage_temp = trage_temp->next;
                trage->next = NULL;
                trage->pre = NULL;
                M_FREE(trage->contec);
                trage->contec = NULL;
                M_FREE(trage);
                trage = NULL;
            }
            else
            {
                trage_pre = trage_temp;
                trage_temp = trage_temp->next;
            }
        }
        if (in_uid == None)
            mRoot.next = NULL;
    }
    // if (trage != NULL && trage_pre != NULL && trage_next != NULL)
    // {
    //     state = succ_r;
    //     M_FREE(trage->contec);
    //     trage->contec = NULL;
    //     M_FREE(trage);
    //     trage = NULL;
    //     trage_pre->next = trage_next;
    //     trage_next->pre = trage_pre;
    // }
    return state;
}

instance_doc_t *m_static_instance_list_add(instance_doc_t *mwc_d)
{
    // stat_m state = fail_r;
    instance_doc_t *resp_obj = NULL;
    bool m_is_new = true;
    uint64_t m_temp_id = mwc_d->sch_info.cu_id;

    struct mitem *trage_temp = NULL, *trage = NULL;

    /** 还是添加还是新增*/

    if (mRoot.next == NULL)
    {
        m_is_new = true;
        trage = &mRoot;
    }
    else
    {
        trage_temp = mRoot.next;

        while (trage_temp != NULL)
        {
            // 如果是新增，记录上一个节点
            trage = trage_temp;
            // 满足替换条件
            if (trage_temp->contec->sch_info.cu_id == m_temp_id)
            {
                m_callable_device_attribute_config_change_notify_event_callable(
                    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_CHANGE,
                    (void *)&m_temp_id,
                    m_callable_timer_manage_get_device_time());
                // 替换
                m_is_new = false;
                trage = trage_temp;
                trage->pre = trage_temp->pre;
                trage->next = trage_temp->next;
                // state = succ_r;
                break;
            }
            trage_temp = trage_temp->next;
        }
    }
    if (m_is_new)
    {
        static instance_doc_t *new_water_d = NULL;
        static struct mitem *new_d = NULL;

        // printf("%d" , sizeof(instance_doc_t));
        new_water_d = M_MALLOC(sizeof(instance_doc_t));
        new_d = M_MALLOC(sizeof(struct mitem));

        memset(new_water_d, 0, sizeof(*new_water_d));
        memset(new_d, 0, sizeof(struct mitem));

        memcpy(new_water_d, mwc_d, sizeof(*mwc_d));

        new_d->contec = new_water_d;
        trage->next = new_d;
        new_d->pre = trage;
        new_d->next = NULL;
        // state = succ_r;
        mwc_d = new_water_d;
        resp_obj = new_water_d;
    }
    else
    {
        memcpy(trage->contec, mwc_d, sizeof(*mwc_d));
        mwc_d = trage->contec;
        resp_obj = mwc_d;
    }
    return resp_obj;
}

/**
 * @brief 返回值指定类型的计划
 *
 * @param sch_type
 * @param fun
 * @return stat_m
 */
stat_m m_static_instance_list_update_spec_type(int sch_type, stat_m *(fun)(void *))
{

    struct mitem *trage_temp = NULL;
    if (mRoot.next == NULL)
    {
    }
    else
    {
        trage_temp = mRoot.next;

        while (trage_temp != NULL)
        {
            if (
                (trage_temp->contec->execu_type == SCHEDULE_RUN_TYPE_SUNRISE_PLUS ||
                 trage_temp->contec->execu_type == SCHEDULE_RUN_TYPE_SUNSET_PLUS ||
                 trage_temp->contec->execu_type == SCHEDULE_RUN_TYPE_SUNRISE ||
                 trage_temp->contec->execu_type == SCHEDULE_RUN_TYPE_SUNSET))
            {
                fun(trage_temp->contec);
            }
            else
                DEBUG_TEST(DB_W, " kk   k k 『%d k k k", trage_temp->contec->execu_type);

            trage_temp = trage_temp->next;
        }
    }
    return succ_r;
}
/**
 * @brief 打印所有
 *
 * @return stat_m
 */
stat_m m_static_instance_list_query_print_all(void)
{
    stat_m stat = fail_r;
    struct mitem *trage_temp = NULL;
    if (mRoot.next == NULL)
    {
    }
    else
    {
        trage_temp = mRoot.next;

        while (trage_temp != NULL)
        {
            stat = succ_r;
            sch_print(trage_temp->contec, 0);
            trage_temp = trage_temp->next;
        }
    }

    return stat;
}
stat_m m_static_instance_list_remove_all(void)
{
    return m_static_instance_list_remove_by_id(None);
}
