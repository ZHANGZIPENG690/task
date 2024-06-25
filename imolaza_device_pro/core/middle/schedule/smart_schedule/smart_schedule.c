

#include "smart_schedule.h"

struct m_smart_array smart_array[3] = {0};

stat_m m_static_smart_running_analyse_char(uint32_t *p1, struct m_smart_channel *p2, char *src);

/**
 * @brief 这个初始化的目的是 取出存在flash的智能计划数据
 *
 *
 * @return stat_m
 */
stat_m m_callable_smart_running_storage_init(void)
{
    stat_m stat = fail_r;

    return stat;
}

/**
 * @brief 追加一个参数到smart 数组，  如果是数组的0
 *
 * @param in_uid 计划id
 * @param in_src 源格式数据   1|2|3|4,2#10;3#5
 * @param in_time_s 时间 （这个时间是很有必要的，超过15min的都会被清除）
 * @return stat_m
 */
stat_m m_callable_smart_running_storage_append(uint64_t in_uid, char *in_src, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    /** 天气 Code */
    int weta = 0;
    if (in_src != NULL)
    {
        for (int i = 0; i < 3; i++)
        {
            if (smart_array[i].uid == 0 || smart_array[i].uid == in_uid || in_time_s >= smart_array[i].inflag_time_s + 15 * 60)
            {
                stat = succ_r;
                memset(smart_array[i].new_ignore_msg, 0, sizeof(smart_array[i].new_ignore_msg));
                memset(smart_array[i].new_water_msg, 0, sizeof(smart_array[i].new_water_msg));
                smart_array[i].uid = in_uid;
                m_static_smart_running_analyse_char(smart_array[i].new_ignore_msg, smart_array[i].new_water_msg, in_src);
                DEBUG_TEST(DB_W,"------ back weather code: %s  %d", in_src, weta);
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_src, "%*[ ],%d", &weta);
                DEBUG_TEST(DB_W,"------ back weather code:   %d", weta);
                m_callable_weather_manage_set(in_uid, weta, in_time_s);
                // DEBUG_TEST(DB_W,"%s" , in_src);
                m_callable_device_attribute_config_change_notify_event_callable(M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SMART_CONFIG,
                                                                                (void *)&in_uid, in_time_s);
                break;
            }
        }
    }
    if (stat == fail_r)
        DEBUG_E("严重错误，没有及时清理smart array");
    return stat;
}
/**
 * @brief 查询计划的区域是否有被忽略
 *
 * @param in_uid 计划id
 * @param in_cid 查询的区域id
 * @param in_time_s  当前的时间
 * @return stat_m succ_r 表示可以运行没有被忽略
 */
stat_m m_callable_smart_running_storage_is_ignore(uint64_t in_uid, uint8_t in_cid, uint64_t in_time_s)
{
    stat_m stat = succ_r;
    for (int i = 0; i < 3 && stat == succ_r; i++)
    {
        // DEBUG_TEST( DB_I,"0.0.0.0. %lld   %hhd ,",  in_uid, in_cid);
        if (smart_array[i].uid == in_uid)
        // && (in_time_s <= smart_array[i].inflag_time_s + 15 * 60 || 1))
        {
            for (int index_chann = 0; index_chann < 30; index_chann++)
            {
                if (smart_array[i].new_ignore_msg[index_chann] != 0 && smart_array[i].new_ignore_msg[index_chann] == in_cid)
                {
                    DEBUG_TEST( DB_I,"(%lld)Find Not Run%hhd", in_uid, smart_array[i].new_ignore_msg[index_chann]);
                    stat = fail_r;
                    // smart_array[i].uid = 0;
                    // memset(smart_array[i].new_water_msg, 0, sizeof(&smart_array[i].new_water_msg));
                    // memset(smart_array[i].new_ignore_msg, 0, sizeof(&smart_array[i].new_ignore_msg));
                    break;
                }
                // else
                //     DEBUG_TEST( DB_I,"-----   %hhd", smart_array[i].new_ignore_msg[index_chann]);
            }
        }
        // else
        //     printf("\t\tjj%lldj%lldj\n", smart_array[i].uid, in_uid);
    }
    return stat;
}
/**
 * @brief 查询区域是否有更改目标时间
 *
 * @param in_uid 计划id
 * @param in_cid 查询的区域id
 * @param out_time  如果改变的时间
 * @param in_time_s  当前的时间
 * @return stat_m succ_r 表示有修改 可以拿返回值
 */
stat_m m_callable_smart_running_storage_is_change(uint64_t in_uid, uint8_t in_cid, uint32_t *out_time, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    for (int i = 0; i < 3; i++)
    {
        if (smart_array[i].uid == in_uid && (in_time_s < smart_array[i].inflag_time_s + 15 * 60 || 1))
        {
            for (int index_chann = 0; index_chann < 30; index_chann++)
            {
                if (smart_array[i].new_water_msg[index_chann].id != 0 && smart_array[i].new_water_msg[index_chann].id == in_cid)
                {
                    DEBUG_TEST( DB_I,"%lld 找到 修改时间 %hhd %d", in_uid, smart_array[i].new_water_msg[index_chann].id, smart_array[i].new_water_msg[index_chann].time);
                    *out_time = smart_array[i].new_water_msg[index_chann].time;
                    smart_array[i].new_water_msg[index_chann].id = 0;
                    smart_array[i].new_water_msg[index_chann].time = 0;
                    // smart_array[i].uid = 0;
                    return succ_r;
                }
            }
        }
    }
    return stat;
}
/**
 * @brief 删除一个
 *
 * @param in_uid
 * @return stat_m
 */
stat_m m_callable_smart_running_storage_clean(uint64_t in_uid)
{
    stat_m stat = fail_r;
    for (int i = 0; i < 3; i++)
    {
        if (smart_array[i].uid == in_uid)
        {
            smart_array[i].uid = 0;
            smart_array[i].is_update = false;
            memset(smart_array[i].new_ignore_msg, 0, sizeof(smart_array[i].new_ignore_msg));
            memset(smart_array[i].new_water_msg, 0, sizeof(smart_array[i].new_water_msg));
            stat = succ_r;
            break;
        }
    }
    return stat;
}

const char INV_I = '|';
const char INV_H = ';';

/**
 * @brief 解析工具  1|2|3|4,2#10;3#5
 *
 * @param p1 new_water_msg[32];   每一个计划都会有最多 32 个新的区域浇水时间信息
 * @param p2 new_ignore_msg[32];  每一个计划都会有最多 32 个不执行的区域列表
 * @param src 源字符
 * @return stat_m
 */
stat_m m_static_smart_running_analyse_char(uint32_t *p1, struct m_smart_channel *p2, char *src)
{
    stat_m stat = fail_r;
    bool ccflag = true;
    int c = 0;
    for (int i = 0; i < 300; i++)
    {
        if (ccflag)
        {
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(src, "%d", &p1[c]);
            if (p1[c] != 0)
            {
                DEBUG_TEST( DB_I,"Ignore Channel is %d", p1[c]);
                c++;
            }
            ccflag = false;
        }
        if (*src == '|')
        {
            *src = ' ';
            ccflag = true;
        }
        else if (*src == ',' || *src == '\0')
            break;
        else
            *src = ' ';
        src++;
    }
    if (*src == ',')
        *src = ' ';
    ccflag = true;
    c = 0;
    for (int i = 0; i < 300; i++)
    {
        if (ccflag)
        {
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(src, "%d#%d", &p2[c].id, &p2[c].time);
            if (p2[c].id != 0)
            {
                DEBUG_TEST( DB_I,"Change Channel is %d # %d", p2[c].id, p2[c].time);
                c++;
            }
            ccflag = false;
        }
        if (*src == ';')
            ccflag = true;
        else if (*src == ',' || *src == '\0')
            break;
        else
            *src = ' ';
        src++;
    }
    DEBUG_TEST( DB_I,"m_static_smart_running_analyse_char end : %s", src);
    return stat;
}
