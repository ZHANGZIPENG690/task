

#include "core/fset_lib.h"
#include "core/utils/util.h"

static uint64_t m_global_start_skip_time_sp = 0;
static uint64_t m_global_end_skip_time_sp = 0;

#define FLASH_DEBUG_START_FLAG "kk"
#define FLASH_DEBUG_END_FLAG "ke"

static uint64_t out_resp_u8 = 0;

stat_m m_global_schedule_skip_init(void)
{
    stat_m stat = fail_r;
    // 读取全局的skip信息
    stat = m_ext_flash_read(U64, FLASH_DEBUG_START_FLAG, NULL, &m_global_start_skip_time_sp);
    if (stat == succ_r)
    {
        stat = m_ext_flash_read(U64, FLASH_DEBUG_END_FLAG, NULL, &m_global_end_skip_time_sp);
    }
    // 不会有单个，单个使用单个的函数  -- > m_global_schedule_skip_alone_get
    return stat;
}

stat_m m_global_schedule_skip_alone_get(uint64_t in_id, uint64_t in_st, uint64_t in_end)
{
    stat_m stat = fail_r;

    return stat;
}
stat_m m_global_schedule_skip_alone_set(uint64_t in_id, uint64_t in_st, uint64_t in_end)
{
    stat_m stat = fail_r;

    return stat;
}
stat_m m_global_schedule_skip_all_set(uint64_t in_st, uint64_t in_end)
{
    stat_m stat = fail_r;
    stat = m_ext_flash_write(U64, FLASH_DEBUG_START_FLAG, NULL, &in_st);
    if (stat == succ_r)
    {
        stat = m_ext_flash_write(U64, FLASH_DEBUG_END_FLAG, NULL, &in_end);
    }
    return stat;
}
stat_m m_global_schedule_winter_sleep_set(uint64_t in_st, uint64_t in_end)
{
    stat_m stat = fail_r;

    return stat;
}

// 返回
// stat_m m_ext_生效一个操作而停止计划()
// {

// }
