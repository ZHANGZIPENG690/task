

/**
 * @brief
 WIFI连接频率：
Http连接频率：
Socket连接频率：

前3次5秒
前10次10秒
前20次20秒
前150次45秒
往后均为90秒

成功数据清空
 *
 */

#include "smart_connect.h"

/** 连接依据 WIFI */
int m_smart_type_wifi_count = 0;
/** 连接依据 HTTP */
int m_smart_type_http_count = 0;
/** 连接依据 TCP */
int m_smart_type_tcp_count = 0;

char *toname_smart(enum smart_connect_type type)
{
    switch (type)
    {
    case M_SMART_TYPE_WIFI:
        return "M_SMART_TYPE_WIFI";
        break;
    case M_SMART_TYPE_HTTP:
        return "M_SMART_TYPE_HTTP";
        break;
    case M_SMART_TYPE_TCP:
        return "M_SMART_TYPE_TCP";
        break;
    default:
        break;
    }
    return "Mol";
}
/**
 * @brief 智能处理连接频率，在需要连接某个类型前调用即可 目前支持 WIFI、Http、TCP三种 需要智能处理 in_need_pro = true 否则false会删除传入的依据：smart_count
 *
 * @param type 连接类型
//  * @param smart_count 连接智能依据
 * @param in_need_pro 需要使用或者清空依据
 * @return stat_m
 */
stat_m m_callable_middle_smart_connect_pro(enum smart_connect_type type, bool in_need_pro)
{
    int *smart_count = NULL;
    // DEBUG_TEST(DB_I, "%d  -  %d ", type, in_need_pro);
    switch (type)
    {
    case M_SMART_TYPE_WIFI:
        smart_count = &m_smart_type_wifi_count;
        if (*smart_count > 2 && *smart_count < 30)
            (*smart_count )*= 2;
        break;
    case M_SMART_TYPE_HTTP:
        smart_count = &m_smart_type_http_count;
        break;
    case M_SMART_TYPE_TCP:
        smart_count = &m_smart_type_tcp_count;
        ++(*smart_count);
        break;
    default:
        break;
    }
    if (smart_count != NULL)
    {
        DEBUG_TEST(DB_I, "smart connect pro Wait... -> %d  (  %s - %s) ", *smart_count, toname_smart(type), !in_need_pro ? "Clear ." : "Add .");
    }
    if (in_need_pro && smart_count != NULL)
    {
        DEBUG_TEST(DB_I, "--- will wait  %d ms --", *smart_count * 300);
        // if (*smart_count < 3)
        //     mDelay_ms(800);
        // else if (*smart_count < 6)
        //     mDelay_ms(5000);
        // else if (*smart_count < 10)
        //     mDelay_ms(10000);
        // else if (*smart_count < 20)
        //     mDelay_ms(20000);
        // else if (*smart_count < 150)
        //     mDelay_ms(45000);
        // else
        //     mDelay_ms(90000);
        for (size_t i = 0; i < *smart_count; i++)
        {
            mDelay_ms(300);
            DEBUG_TEST( DB_NONE_LINE, ".");
        }
        if (*smart_count < 250)
            (*smart_count)++;
        DEBUG_TEST(DB_I, "--- wait done  --");
    }
    else if (smart_count != NULL)
    {
        DEBUG_TEST(DB_I, "--- %d smart clear  --", type);
        *smart_count = 0;
    }
    else if (in_need_pro)
    {
        DEBUG_TEST(DB_I, "---- -Nnuk Err - \n");
        mDelay_ms(5000);
    }
    return succ_r;
}
