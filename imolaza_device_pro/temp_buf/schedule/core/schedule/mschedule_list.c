
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
    m_watering_schedule *contec;
};

struct mitem mRoot;

stat_m m_callable_schedule_list_find(uint64_t in_uid , m_watering_schedule **out_obj)
{
    stat_m stat = fail_r;
    struct mitem *trage_temp = NULL;
    if( out_obj );
    if (mRoot.next != NULL)
    {
        // DEBUG_TEST(DB_W,"--\n");
        trage_temp = mRoot.next;
        while (trage_temp != NULL)
        {
            DEBUG_TEST(DB_W,"m_callable_schedule_list_find -%lld-\n" , trage_temp->contec->sch_info.cu_id     );
            if(trage_temp->contec->sch_info.cu_id == in_uid)
            {
                *out_obj = trage_temp->contec;
                stat = succ_r;
                break;
            }
            trage_temp = trage_temp->next; 
        }

    }
    // if(stat != succ_r)
    //     out_obj = NULL;
    return stat;
}
stat_m m_callable_schedule_list_remove(uint64_t in_uid)
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
                DEBUG_TEST(DB_I,"find schedule : %lld .. " , trage->contec->sch_info.cu_id);
                trage_next = trage_temp->next;
                trage_pre = trage_temp->pre;
                break;
            }
            if(in_uid == None)
            {
                state = succ_r;
                trage = trage_temp;
                trage_temp = trage_temp->next;
                trage->next = NULL;
                trage->pre = NULL;
                M_FREE(trage->contec);
                trage->contec = NULL;
                M_FREE(trage);
                trage= NULL;
            }else
            {            
                trage_pre = trage_temp;
                trage_temp = trage_temp->next;
            }
        }
    }
    if (trage != NULL && trage_pre != NULL && trage_next != NULL)
    {
        state = succ_r;
        M_FREE(trage->contec);
        trage->contec = NULL;
        M_FREE(trage);
        trage= NULL;
        trage_pre->next = trage_next;
        trage_next->pre = trage_pre;
    }
    return state;
}

stat_m m_callable_schedule_list_add(m_watering_schedule *mwc_d)
{
    stat_m state = fail_r;
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
                // 替换
                m_is_new = false;
                trage = trage_temp;
                trage->pre = trage_temp->pre;
                trage->next = trage_temp->next;
                state = succ_r;
                break;
            }
            trage_temp = trage_temp->next;
        }
    }
    if (m_is_new)
    {
        static m_watering_schedule *new_water_d = NULL;
        static struct mitem *new_d = NULL;

        // printf("%d" , sizeof(m_watering_schedule));
        new_water_d = M_MALLOC(sizeof(m_watering_schedule) );
        new_d = M_MALLOC(sizeof(struct mitem));

        memset(new_water_d, 0, sizeof(*new_water_d));
        memset(new_d, 0, sizeof(struct mitem));

        memcpy(new_water_d, mwc_d, sizeof(*mwc_d));


        new_d->contec = new_water_d;
        trage->next = new_d;
        new_d->pre = trage;
        new_d->next = NULL;
        state = succ_r;
        mwc_d = new_water_d;
    }
    else
    {
        memcpy(trage->contec, mwc_d, sizeof(*mwc_d));
        mwc_d = trage->contec;
    }
    return state;
}
