

#include "instance_stroage.h"

#define PLAN_JU "PA"
#define PLAN_XU "PAE"
#define PLA_ENABLE "enab"
#define PLANSIZE 30
#define PLANINFOSIZE 150

volatile int flash_id = 0;
static char pp_temp[25] = {0};
static char pp_value[512] = {0};

struct splan_core
{
    uint64_t sch_id;
    // m_schedule_info sch_info;
    uint8_t available;
    char *sch_dec;
};

struct splan_core m_splan_core[PLANSIZE];

stat_m m_callable_schedule_stroage_init(void)
{
    stat_m stat = fail_r;

    return stat;
}
// http://c.biancheng.net/ml_alg/env.html
/**
 * @brief Deposit a string that represents the contents of the schedule in flash_schedule_list_t
 * @todo Unit Input ...
 *
 * @param in_schedule_str A string that represents the contents of the plan
 * @param enable schedule enable
 * @return stat_m  success
 */
stat_m m_callable_schedule_stroage_write_string(uint64_t in_id, const char *in_schedule_str, bool enable)
{
    DEBUG_TEST(DB_I, "Add a schedule to storage: %s ", in_schedule_str);
    stat_m state = fail_r;
    bool m_is_new = true;
    int m_flag = 0;

    /** 1、先知道计划是替换还是新增 , 顺便找个坑位*/
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        // 需要替换
        if (m_splan_core[flash_id].sch_id == in_id)
        {
            m_is_new = false;
            m_flag = flash_id;
            state = succ_r;
            break;
        }
        // 找坑位
        if (m_splan_core[flash_id].sch_id == 0 && m_splan_core[flash_id].sch_dec == NULL && m_flag == 0)
        { // 没人的坑位找到了，不进行第二次找
            m_flag = flash_id;
            state = succ_r;
        }
    }
    /** 2、 释放之前按的内存  */
    // if (!m_is_new)
    // {
    //     M_FREE(m_splan_core[m_flag].sch_dec);
    //     m_splan_core[m_flag].sch_dec = NULL;
    // }

    if (state == succ_r)
    {
        m_splan_core[m_flag].sch_id = in_id;
        /** 3、 换掉*/
        if (!m_is_new)
        {
            // 如果不是新增的话
            strcpy(m_splan_core[m_flag].sch_dec, in_schedule_str);
        }
        else
        {
            // 如果是新增
            // static char *new_plan_info = NULL;
            // new_plan_info = M_MALLOC(strlen(in_schedule_str)+1);

            m_splan_core[m_flag].sch_dec = (char *)M_MALLOC(strlen(in_schedule_str) + 1);

            strlcpy(m_splan_core[m_flag].sch_dec, in_schedule_str, strlen(in_schedule_str) + 1);
            // strcpy(new_plan_info, plan_info);
            m_callable_device_attribute_change_schedule_num(true);
            // m_splan_core[m_flag].sch_dec = new_plan_info;
        }

        sprintf(pp_temp, "%s%d", PLAN_JU, m_flag);

        state = m_ext_flash_write(M_TYPE_Str, pp_temp, m_splan_core[m_flag].sch_dec);

        state = succ_r;
    }
    return state;
}

int test_ab = 0;
/**
 * @brief Test Add Recovery
 *
 * @param dec
 * @return stat_m
 */
stat_m m_rest_schedule_stroage_add(char *dec)
{
    sprintf(pp_temp, "%s%d", PLAN_JU, test_ab);

    m_ext_flash_write(M_TYPE_Str, pp_temp, dec);
    test_ab++;
    return 0;
}
/**
 * @brief Delete the storage item based on the schedule id
 * @todo Unit Input ...
 *
 * @param schedule_id schedule id
 * @return stat_m M_OOK success
 */
stat_m m_static_schedule_stroage_remove_by_id(uint64_t schedule_id)
{
    stat_m state = fail_r;

    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        if (m_splan_core[flash_id].sch_dec != NULL && m_splan_core[flash_id].sch_id == schedule_id)
        {
            state = succ_r;
            if (m_splan_core[flash_id].sch_dec != NULL)
                M_FREE(m_splan_core[flash_id].sch_dec);

            m_splan_core[flash_id].sch_dec = NULL;
            m_splan_core[flash_id].sch_id = 0;
            sprintf(pp_temp, "%s%d", PLAN_JU, flash_id);
            DEBUG_TEST(DB_I, "prepare delete it ....  : %s .. ", pp_temp);
            m_callable_device_attribute_change_schedule_num(false);
            state = m_ext_flash_remove(pp_temp);
            m_static_time_link_notece_node_remove_by_id(schedule_id);
            if (state == succ_r)
            {
                DEBUG_TEST(DB_I, "OKK");
            }
            else
            {
                DEBUG_TEST(DB_I, "ERR");
            }
            // _device.planSum--;
        }
    }
    // print_flash_id_info();
    return state;
}

void m_callable_tool_string_find_char_postion(char *in_strc, int *in_posti)
{
    int ps_flag = 0, ps_sum = 0;
    for (int i = 0; i < 350; i++)
    {
        ps_sum++;
        in_strc++;
        if (*in_strc == ',')
            ps_flag++;
        if (ps_flag == *in_posti)
            break;
    }
    *in_posti = ps_sum + 1;
}

/**
 * @brief Extract the schedule content characters from the existing list by id
 * @todo Unit Out ...
 *
 * @param schedule_id schedule id
 * @param sch_str output schedule string
 * @param postion postion
 * @return stat_m
 */
stat_m m_callable_schedule_stroage_read_by_param(uint64_t schedule_id, char **out_str, int postion)
{
    stat_m stat = fail_r;
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        if (m_splan_core[flash_id].sch_dec != NULL && m_splan_core[flash_id].sch_id == schedule_id)
        {
            /** 查找 Pos处于 哪个位置 */
            m_callable_tool_string_find_char_postion(m_splan_core[flash_id].sch_dec, &postion);
            // DEBUG_TEST( DB_I,%d   %s" , postion , m_splan_core[flash_id].sch_dec  + postion);
            *out_str = m_splan_core[flash_id].sch_dec + postion;
            break;
        }
    }
    // DEBUG_TEST( DB_I,m_callable_schedule_stroage_read_by_param - %s" , *out_str);
    return stat;
}
/**
 * @brief Read all existing schedule from flash, Contains disabled and enabled schedules,Start from 0 to MAX_SCHEDU_NUM, after which it returns failure, and the pointer returns to 0 after receiving the failure
 * @todo Unit Out ...
 *
 * @return stat_m M_ERR: ：Indicates that the content has been restored
 */
stat_m m_callable_schedule_stroage_load_all_from_flash(void)
{
    stat_m stat = fail_r;
    int mschi = 0;
    // memset(&mschi, 0, sizeof(mschi));
    // printf("\tsplan_parse_item\r\n");
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        sprintf(pp_temp, "%s%d", PLAN_JU, flash_id); // 找到存在坑位的计划

        if (m_callable_drive_flash_read(M_TYPE_Str, pp_temp, pp_value) == succ_r) // 找到的坑 有人在确实
        {
            {
                // 如果是新增
                static char *new_plan_info = NULL;
                new_plan_info = M_MALLOC(sizeof(char) * strlen(pp_value));
                strlcpy(new_plan_info, pp_value, sizeof(char) * strlen(pp_value));
                m_splan_core[flash_id].sch_dec = new_plan_info;
            }
            /** 解析出 计划启用 计划数据和计划id 及统计计划数量*/
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(pp_value, "%lld", &m_splan_core[flash_id].sch_id);
            // 禁用还是启用
            sprintf(pp_temp, "%s%lld", PLAN_XU, m_splan_core[flash_id].sch_id);

            if (m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_GET) != succ_r) // 有
                m_splan_core[flash_id].available = true;
            else
                m_splan_core[flash_id].available = false;
            /** 兼容 */
            if (m_splan_core[flash_id].available)
            {
                sprintf(pp_temp, "%s%d", PLAN_XU, flash_id);

                if (m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_GET) != succ_r) // 有
                    m_splan_core[flash_id].available = true;
                else
                {
                    DEBUG_TEST(DB_I, "Old En/Dis Schedule Exevl ... (%lld) ", m_splan_core[flash_id].sch_id);
                    m_splan_core[flash_id].available = false;

                    m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_DEL);
                    sprintf(pp_temp, "%s%lld", PLAN_XU, m_splan_core[flash_id].sch_id);
                    m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_SET);
                }
            }
            /** End 兼容 */
            if (m_splan_core[flash_id].sch_id > 0 &&
                m_splan_core[flash_id].sch_dec != NULL)
            {
                if (m_splan_core[flash_id].available)
                {
                    DEBUG_TEST(DB_I, "Enable id: %d    %s", flash_id, m_splan_core[flash_id].sch_dec);
                    mschi++;
                }
                else
                {
                    DEBUG_TEST(DB_W, "Disable id: %d    %s", flash_id, m_splan_core[flash_id].sch_dec);
                }
                // DEBUG_TEST(DB_W,"%s" ,m_splan_core[flash_id].sch_dec);
                // if (m_splan_core[flash_id].available)
                // {
                //     m_callable_instance_manage_add(m_splan_core[flash_id].sch_dec, M_INSTANCE_ADD_MODE_RECOVER_ENABLE, m_callable_timer_manage_get_device_time(), NULL);
                // }
                // else
                //     m_callable_instance_manage_add(m_splan_core[flash_id].sch_dec, M_INSTANCE_ADD_MODE_RECOVER_DISABLE, m_callable_timer_manage_get_device_time(), NULL);
            }
        }
    }
    m_callable_device_attribute_set_schedule_num(mschi);
    DEBUG_TEST(DB_W, "Schedule Sum: %d", mschi);
    return stat;
}
/**
 * @brief Delete the storage item based on the schedule id
 * @todo Unit Input ...
 *
 * @param schedule_id schedule id
 * @return stat_m M_OOK success
 */
stat_m m_static_schedule_stroage_remove_all(void)
{
    stat_m state = fail_r;
    DEBUG_TEST(DB_W, " Warning : Will Delete All ! ");
    m_callable_device_attribute_change_schedule_num(9);
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {

        if (m_splan_core[flash_id].sch_dec != NULL && m_splan_core[flash_id].sch_id != 0)
        {
            state = succ_r;
            M_FREE(m_splan_core[flash_id].sch_dec);
            m_splan_core[flash_id].sch_dec = NULL;
            m_splan_core[flash_id].sch_id = 0;
            sprintf(pp_temp, "%s%d", PLAN_JU, flash_id);
            state = m_ext_flash_remove(pp_temp);
        }
    }
    return state;
}

// stat_m m_callable_stroage_schedule_print_all(){stat_m stat = fail_r;return stat;}

/**
 * @brief 打印全部的信息
 *
 */
void m_schedule_storage_buffer_print(void)
{
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
        if (m_splan_core[flash_id].sch_dec != NULL)
        {
            DEBUG_TEST(DB_I, "Flash id: %d , \t info :%s", flash_id, m_splan_core[flash_id].sch_dec);
        }
}

void m_static_repl_char_one(char *in_str, char in_wait_a, char in_new_b)
{
    while (in_str != NULL && *in_str != '\0')
    {
        if (*in_str == in_wait_a)
        {
            *in_str = in_new_b;
        }
        in_str++;
    }
}
/**
 * @brief 打印全部的信息
 *
 */
void m_callable_schedule_storage_buffer_print_to_remote(void)
{
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
        if (m_splan_core[flash_id].sch_dec != NULL)
        {
            m_static_repl_char_one(m_splan_core[flash_id].sch_dec, ']', ' ');
            DEBUG_TEST(DB_I, "Flash id: %d , \t info :%s", flash_id, m_splan_core[flash_id].sch_dec);
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_QUERY_SCHEDULE_STROAGE_INFO,
                                            M_TYPE_Str, (void *)m_splan_core[flash_id].sch_dec,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, m_callable_timer_manage_get_device_time(), false);
        }
}
/**
 * @brief 直接删掉禁用
 *
 * @param in_uid id
 * @return stat_m
 */
stat_m m_callable_exce_schedule_stroage_en_dis_able(uint64_t in_uid, bool in_en)
{
    stat_m stat = fail_r;
    sprintf(pp_temp, "%s%lld", PLAN_XU, in_uid);
    if (in_en)
        (stat = m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_DEL)); // 找到的坑 有人在确实
    else
        (stat = m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_SET)); // 找到的坑 有人在确实
    return stat;
}
/**
 * @brief 启用
 *
 * @param schedule_id
 * @param in_time_s
 * @return stat_m
 */
stat_m m_callable_schedule_stroage_enable(uint64_t in_uid, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        if (m_splan_core[flash_id].sch_dec != NULL && m_splan_core[flash_id].sch_id == in_uid)
        {
            if (!m_splan_core[flash_id].available)
            {
                sprintf(pp_temp, "%s%lld", PLAN_XU, m_splan_core[flash_id].sch_id);
                if (m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_DEL) == succ_r) // 找到的坑 有人在确实
                {
                    DEBUG_TEST(DB_I, "Enable %lld (%s) Yes", in_uid, pp_temp);
                    stat = succ_r;
                }
            }
            m_splan_core[flash_id].available = true;
        }
    }
    return stat;
}

/**
 * @brief 关闭一个
 *
 * @param schedule_id
 * @param in_time_s
 * @return stat_m
 */
stat_m m_callable_schedule_stroage_disable(uint64_t in_uid, uint64_t in_time_s)
{
    stat_m stat = fail_r;
    for (flash_id = 0; flash_id < PLANSIZE; flash_id++)
    {
        if (m_splan_core[flash_id].sch_dec != NULL && m_splan_core[flash_id].sch_id == in_uid)
        // if (m_splan_core[flash_id].sch_dec != NULL && m_splan_core[flash_id].sch_id != in_uid)
        {
            sprintf(pp_temp, "%s%lld", PLAN_XU, m_splan_core[flash_id].sch_id);
            if (m_callable_flash_rescue_message_flag(pp_temp, M_GLO_STROAGE_RESCUE_MESSAGE_SET) == succ_r) // 找到的坑 有人在确实
                stat = succ_r;
            m_splan_core[flash_id].available = false;
        }
    }
    return stat;
}
/**
 * @brief 恢复计划内容
 *
 * @return stat_m
 */
stat_m m_callable_schedule_stroage_recover_item_all(void)
{
    stat_m stat = fail_r;
    int fid = 0;
    for (fid = 0; fid < PLANSIZE; fid++)
    {
        if (m_splan_core[fid].sch_dec != NULL && m_splan_core[fid].sch_id > 0)
        {
            m_static_instance_manage_instance_recover(m_splan_core[fid].sch_dec, m_splan_core[fid].available, m_callable_timer_manage_get_device_time());
        }
    }
    m_callable_instance_manage_recover_done(m_callable_timer_manage_get_device_time());
    return stat;
}