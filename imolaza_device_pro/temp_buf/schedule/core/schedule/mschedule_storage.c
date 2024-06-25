#include "core/fset_lib.h"
#include "core/utils/util.h"
#include "core/schedule/mschedule.h"
/**
 *  保存的计划按照参数来
 *
 *       有一个计划的保存句柄枚举，列举了所有的计划参数
 *
 *
 */

// typedef m_fsm_status m_schedule_state;

// 1956 0 null 1666377000 1887301800 100&16:39 2#3;1#3;5#3;4#1;3#3 1 0 109 780
// 1956 0 null 1666377000 1887301800 100&11:43 1#20;2#20;3#19 1 1 108 3540
enum _index
{
    SCHEDULE_STORAGE_FLAG_ID = 0,
    SCHEDULE_STORAGE_FLAG_SMART = 1,
    SCHEDULE_STORAGE_FLAG_SOCK = 2,
    SCHEDULE_STORAGE_FLAG_EFFECTIVE_START_TIME = 3,
    SCHEDULE_STORAGE_FLAG_EFFECTIVE_END_TIME = 4,
    SCHEDULE_STORAGE_FLAG_EXECUTE_TYPE = 5,
    SCHEDULE_STORAGE_FLAG_EXECUTE_TIME = 6,
    SCHEDULE_STORAGE_FLAG_ZONE = 7,
    SCHEDULE_STORAGE_FLAG_WEEK_LIST = 8,          // week
    SCHEDULE_STORAGE_FLAG_INTERVAL_HOUR = 9,      // hour
    SCHEDULE_STORAGE_FLAG_RUN_INTERVAL_TYPE = 10, // type
    SCHEDULE_STORAGE_FLAG_RUN_TIME_SUM = 11,      // type
};

#define PLAN_JU "PA"
#define PLAN_XU "PAE"
#define PLANSIZE 30
#define PLANINFOSIZE 150

volatile int flash_id = 0;
static char pp_temp[25] = {0};
static char pp_value[200] = {0};

struct splan_core
{
    uint64_t sch_id;
    m_schedule_info sch_info;
    uint8_t available;
    char *sch_dec;
};

struct splan_core m_splan_core[PLANSIZE];

/**
 * @brief 打印全部的信息
 *
 */
void m_schedule_storage_buffer_print(void)
{
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
        if (m_splan_core[flash_id].sch_dec != NULL)
            printf("flash id: %d , \t info :%s\r\n", flash_id, m_splan_core[flash_id].sch_dec);
}
/**
 * @brief
 *
 * @param in_id
 * @return stat_m
 */
stat_m m_splan_remove(uint64_t in_id)
{
    // printf("\t splan_del_item\r\n");

    stat_m state = fail_r;

    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        if (m_splan_core[flash_id].sch_dec != NULL && m_splan_core[flash_id].sch_id == in_id)
        {

            state = succ_r;
            M_FREE(m_splan_core[flash_id].sch_dec);
            m_splan_core[flash_id].sch_dec = NULL;
            m_splan_core[flash_id].sch_id = 0;
            sprintf(pp_temp, "%s%d", PLAN_JU, flash_id);
            DEBUG_TEST(DB_I,"prepare delete it ....  : %s .. " , pp_temp);
            state = m_ext_flash_remove(pp_temp);
            if(state == succ_r)
                DEBUG_TEST(DB_I,"OKK");
            else
                DEBUG_TEST(DB_I,"ERR");
            // _device.planSum--;
        }
    }
    // print_flash_id_info();
    return state;
}
/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_splan_remove_all(void)
{
    // printf("\t splan_del_item_all\r\n");

    stat_m state = fail_r;
    // uint64_t temp_id = 0;
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {

        if (m_splan_core[flash_id].sch_dec != NULL && m_splan_core[flash_id].sch_id != 0)
        {
            state = succ_r;
            M_FREE(m_splan_core[flash_id].sch_dec);
            m_splan_core[flash_id].sch_dec = NULL;
            m_splan_core[flash_id].sch_id = 0;
            sprintf(pp_temp, "%s%d", PLAN_JU, flash_id);
            m_ext_flash_remove(pp_temp);
        }
    }
    // print_flash_id_info();
    return state;
}

/**
 * @brief
 *
 * @param mem_id
 * @param plan_info
 * @return stat_m
 */
stat_m m_splan_add_mem_item(int mem_id, char *plan_info)
{
    // printf("\tsplan_add_mem_item %d\r\n", sizeof(char) * strlen(plan_info));

    stat_m state = fail_r;

    static char *new_plan_info = NULL;
    new_plan_info = M_MALLOC(sizeof(char) * strlen(plan_info) + 2);

    // strcpy(new_plan_info, plan_info, sizeof(char) * strlen(plan_info) + 2);
    // strcpy(new_plan_info, plan_info);

    m_splan_core[mem_id].sch_dec = new_plan_info;

    // m_splan_core[flash_id].sch_id = mem_id;

    state = succ_r;
    return state;
}
/**
 * @brief 将存储类型转换成一个存储类
 *
 * @param in_dec  输入一个你想转换的秒速字符
 * @param out_dec  输出存储类
 * @return stat_m
 */
static stat_m m_sch_get_cts(m_schedule_info *in_info, char *out_dec)
{
    in_info->cu_temp_param[3] = '&';
    sprintf(out_dec, "%lld %hhd %s %lld %lld %s %s %s %hhd %hhd %d",
            //  5 2 2021-09-28:08-00-00 2021-10-29:19-00-00 0 2#1,3#1 0 S 1,2,5 AT 08:02 NULL 0 0

            in_info->cu_id,
            in_info->cu_smart,
            /*s*/ in_info->cu_sack,                // str
            in_info->available_time.tr_start_time, // str
            in_info->available_time.tr_end_time,   // str
            /*s*/ in_info->cu_temp_param,          // str
            /*s*/ in_info->cu_zone,                // str
            /*s*/ in_info->cu_week_list,           // 1;2;3;                                            // AT*10:24
            in_info->cu_interval_hour_or_day,      // or 1 hour
            in_info->cu_run_interval_type,         // or 108
            in_info->cu_run_time_sum);             // 100
    return 0;
}
static stat_m m_sch_get_uct(char *in_dec, m_schedule_info *out_info)
{
    // m_schedule_info *  in_info
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(in_dec, "%lld %hhd %s %lld %lld %s %s %s %hhd %hhd %d", //  5 2 2021-09-28:08-00-00 2021-10-29:19-00-00 0 2#1,3#1 0 S 1,2,5 AT 08:02 NULL 0 0

           &out_info->cu_id,
           &out_info->cu_smart,
           /*s*/ out_info->cu_sack,                 // str
           &out_info->available_time.tr_start_time, // str
           &out_info->available_time.tr_end_time,   // str
           /*s*/ out_info->cu_temp_param,           // str
           /*s*/ out_info->cu_zone,                 // str
           /*s*/ out_info->cu_week_list,            // 1;2;3;                                            // AT*10:24
           &out_info->cu_interval_hour_or_day,      // or 1 hour
           &out_info->cu_run_interval_type,         // or 108
           &out_info->cu_run_time_sum);             // 100
    if (strstr(out_info->cu_temp_param, "&") != NULL)
    {
        m_insi_foramt_param("%hhd&%hhd", &out_info->cu_execute_type,
                            out_info->cu_execute_time, out_info->cu_temp_param);
    }
    return 0;
}

stat_m splan_flash_recover_item(void)
{
    // printf("\tplan_flash_recover_item\r\n");
    // printf("-----------------------------");
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        sprintf(pp_temp, "%s%d", PLAN_JU, flash_id); // 找存在的坑位

        if (m_ext_flash_read(Str, pp_temp, pp_value, NULL) == succ_r) // 有
        {
            /** 解析出 计划启用 计划数据和计划id 及统计计划数量*/
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(pp_value, "%lld", &m_splan_core[flash_id].sch_id);
            m_splan_add_mem_item(flash_id, pp_value);

            // 禁用还是启用
            sprintf(pp_temp, "%s%lld", PLAN_XU, m_splan_core[flash_id].sch_id);
            if (m_callable_flash_flag_rescue_message( pp_temp,  M_RESCUE_MESSAGE_GET) != succ_r) // 有
            {
                m_splan_core[flash_id].available = true;
                // _device.extreinfo.schedule_sum++;
            }
            else
                m_splan_core[flash_id].available = false;

            if (m_splan_core[flash_id].available)
                DEBUG_TEST(DB_I,"flash id: %d , \t info :%s\r\n", flash_id, m_splan_core[flash_id].sch_dec);
            else
                DEBUG_TEST(DB_W,"disable flash id: %d , \t info :%s\r\n", flash_id, m_splan_core[flash_id].sch_dec);

            // printf("%s\r\n", plan_mamager[flash_id]);
            // printf("------------------id: %d    %s\r\n", flash_id, plpl.plan_vue);

            // _device.planSum++;
        }
        // else
        //     break;
    }
    // DEBUG_TEST(DB_W,"_device.planSum:%hd", _device.planSum);
    // print_flash_id_info();

    return 0;
}

void *recovery_completed = NULL;
/**
 * @brief 把提前准备的数据解析成计划
 *
 * @return stat_m  成功与否
 */
stat_m m_splan_parse_one(uint64_t in_id)
{
    m_watering_schedule mschi;
    memset(&mschi , 0 , sizeof(mschi));
    // printf("\tsplan_parse_item\r\n");
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        if(m_splan_core[flash_id].sch_id == in_id &&
            m_splan_core[flash_id].sch_id > 0 &&
            m_splan_core[flash_id].sch_dec != NULL)
        {
            DEBUG_TEST(DB_I,"Enable : id %d    %s\r\n", flash_id, m_splan_core[flash_id].sch_dec);
            m_splan_core[flash_id].available = true;
            m_sch_get_uct(m_splan_core[flash_id].sch_dec, &mschi.sch_info);
            m_sch_forthwith_add(&mschi,false);
            break;
        }
    }
    return succ_r;
}
/**
 * @brief 把提前准备的数据解析成计划
 *
 * @return stat_m  成功与否
 */
stat_m m_splan_parse_all(void)
{
    m_watering_schedule mschi;
    memset(&mschi , 0 , sizeof(mschi));
    // printf("\tsplan_parse_item\r\n");
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        if (m_splan_core[flash_id].available &&
            m_splan_core[flash_id].sch_id > 0 &&
            m_splan_core[flash_id].sch_dec != NULL)
        {
            DEBUG_TEST(DB_I,"id: %d    %s\r\n", flash_id, m_splan_core[flash_id].sch_dec);
            m_sch_get_uct(m_splan_core[flash_id].sch_dec, &mschi.sch_info);
            m_sch_forthwith_add(&mschi , true);
        }
    }
    // print_flash_id_info();
    // 全部恢复完成的通知
    // if (recovery_completed != NULL)
    //     recovery_completed();
    // else
    //     printf("NULL");
    m_callable_recove_schedule_comple();

    // DEBUG_TEST(DB_W,"_device.planSum:%hd", _device.planSum);
    return 0;
}
/**
 * @brief 添加一个计划到flash    添加一个计划到内存 （非队列）
 *
 * @param in_info  计划的存储信息
 * @return stat_m
 */
stat_m m_splan_add_item(m_schedule_info *in_info)
{
    stat_m state = fail_r;
    bool m_is_new = true;
    int m_flag = 0;
    /** 1、先知道计划是替换还是新增 , 顺便找个坑位*/
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        // 需要替换
        if (m_splan_core[flash_id].sch_id == in_info->cu_id)
        {
            m_is_new = false;
            m_flag = flash_id;
            break;
        }
        // 找坑位
        if (m_splan_core[flash_id].sch_id == 0 && m_splan_core[flash_id].sch_dec == NULL && m_flag == 0)
        { // 没人的坑位找到了，不进行第二次找
            m_flag = flash_id;
        }
    }
    /** 2、 释放之前按的内存*/
    if (!m_is_new)
    {
        M_FREE(m_splan_core[m_flag].sch_dec);
        m_splan_core[m_flag].sch_dec = NULL;
    }
    
    m_splan_core[m_flag].sch_id  = in_info->cu_id;

    m_sch_get_cts(in_info, pp_value); // 只用比较id

    /** 3、 换掉*/
    m_splan_add_mem_item(m_flag, pp_value);

    sprintf(pp_temp, "%s%d", PLAN_JU, m_flag);

    state = m_ext_flash_write(Str, pp_temp, m_splan_core[m_flag].sch_dec, NULL);

    state = succ_r;

    return state;
}

/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_splan_parse_all_low_version_tranform(void)
{
    return 0;
}

stat_m m_schedule_storage(m_schedule_info *in_msi)
{
    return 0;
}
